/*
 * WiFi_program.c
 *
 *  Created on: Oct 14, 2022
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Delay_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Error_Handler_interface.h"
#include "Debug_active.h"
#include "My_String.h"

/*	MCAL	*/
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "NVIC_interface.h"
#include "STK_interface.h"
#include "UART_interface.h"

/*	SELF	*/
#include "WiFi_interface.h"
#include "WiFi_private.h"
#include "WiFi_config.h"
#include "WiFi_cmd.h"

/*******************************************************************************
 *	Command entering:
 ******************************************************************************/
b8 WiFi_b8SendCommand(
	WiFi_t* module,
	char* cmd,
	char* paramArr[],
	WiFi_Parameter_t paramTypeArr[], u8 nParams,
	char* successResponse, u16 msTimeout
)
{
	/*	flush parasitic remaining data byte	*/
	UART_voidFlushDataReceiveRegister(module->uartUnitNumber);

	/*	send command (this one has no header)	*/
	UART_voidSendStringEcho(module->uartUnitNumber, cmd);

	/*	if number of parameters is not zero, send them	*/
	if (nParams != 0)
	{
		/*	send '=' right after command	*/
		UART_voidSendByteEcho(module->uartUnitNumber, '=');

		WiFi_Parameter_t type;

		for (u8 i = 0; i < nParams; i++)
		{
			type = paramTypeArr[i];

			/*	guard the parameter with double quotes if its type was "string"	*/
			if (type == WiFi_Parameter_String)
				UART_voidSendByteEcho(module->uartUnitNumber, '\"');

			UART_voidSendStringEcho(module->uartUnitNumber, paramArr[i]);

			/*	guard the parameter with double quotes if its type was "string"	*/
			if (type == WiFi_Parameter_String)
				UART_voidSendByteEcho(module->uartUnitNumber, '\"');

			/*
			 * as long ass this parameter is not the last one, send ',' to
			 * separate from next parameter
			 */
			if (i != nParams - 1)
				UART_voidSendByteEcho(module->uartUnitNumber, ',');
		}
	}

	/*	enter	*/
	WiFi_voidEnter(module);

	/*	check for success	*/
	b8 cmdSuccessful = UART_b8ReceiveStringTimeout(
		module->uartUnitNumber, module->buffer, msTimeout, successResponse);

	/*	return check status	*/
	return cmdSuccessful;
}

void WiFi_voidEnter(WiFi_t* module)
{
	UART_voidSendByteEcho(module->uartUnitNumber, '\r');
	UART_enumSendByte(module->uartUnitNumber, '\n');
}

/*******************************************************************************
 *	Init:
 ******************************************************************************/
void WiFi_voidInit(
	WiFi_t* module, GPIO_Pin_t rstPin,
	UART_UnitNumber_t uartUnitNumber, u32 baudrate, u8 uartAfioMap)
{
	/**	Init UART	**/
	UART_voidFastInit(uartUnitNumber, baudrate, uartAfioMap);

	module->uartUnitNumber = uartUnitNumber;

	/**	Init reset pin (GPIO)	**/
	module->rstPort = rstPin / 16;

	module->rstPin = rstPin % 16;

	GPIO_voidSetPinGpoPushPull(module->rstPort, module->rstPin);

	/*	as pin is active low, set normally high	*/
	GPIO_voidSetPinOutputLevel(
		module->rstPort, module->rstPin, GPIO_OutputLevel_High);
}

/*******************************************************************************
 *	Module availability:
 ******************************************************************************/
b8 WiFi_b8IsModuleAvailable(WiFi_t* module)
{
	return WiFi_b8SendCommand(
		module, "AT", NULL, NULL, 0, "OK",
		WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS);
}

/*******************************************************************************
 *	Reset:
 ******************************************************************************/
b8 WiFi_b8HardReset(WiFi_t* module)
{
	/*	1 - second pulse on rst pin	*/
	GPIO_voidSetPinOutputLevel(module->rstPort, module->rstPin, GPIO_OutputLevel_Low);

	Delay_voidBlockingDelayMs(1000);

	GPIO_voidSetPinOutputLevel(module->rstPort, module->rstPin, GPIO_OutputLevel_High);

	/*	check for success	*/
	b8 resetSuccessful = UART_b8ReceiveStringTimeout(
		module->uartUnitNumber, module->buffer,
		WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS, "ready");

	/*	return status	*/
	return resetSuccessful;
}

b8 WiFi_b8SoftReset(WiFi_t* module)
{
	return WiFi_b8SendCommand(
		module, "AT+RST", NULL, NULL, 0, "OK",
		WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS);
}

b8 WiFi_b8FactoryReset(WiFi_t* module)
{
	return WiFi_b8SendCommand(
		module, "AT+RESTORE", NULL, NULL, 0, "ready",
		WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS);
}

/*******************************************************************************
 *	RF antenna voltage:
 ******************************************************************************/
b8 WiFi_b8SetRfVdd(WiFi_t* module, u16 voltageMultipliedBy1000)
{
	/*	prepare paramArr	*/
	char paramStr[WIFI_MAX_PARAM_LEN];
	sprintf(paramStr, "%u", (u32)voltageMultipliedBy1000);

	char* paramArr[] = {paramStr};

	/*	prepare paramTypeArr	*/
	WiFi_Parameter_t paramTypeArr[] = {WiFi_Parameter_Numerical};

	/*	enter command	*/
	return WiFi_b8SendCommand(
		module, "AT+RFVDD", paramArr, paramTypeArr, 1, "OK",
		WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS);
}

/*******************************************************************************
 *	WiFi:
 ******************************************************************************/
b8 WiFi_b8SelectMode(WiFi_t* module, WiFi_Mode_t mode, b8 storeInFlash)
{
	/*	prepare paramArr	*/
	char paramStr[2];
	paramStr[0] = '0' + mode;
	paramStr[1] = '\0';

	char* paramArr[] = {paramStr};

	/*	prepare paramTypeArr	*/
	WiFi_Parameter_t paramTypeArr[] = {WiFi_Parameter_Numerical};

	/*	enter command	*/
	const char cmdDef[] = "AT+CWMODE_DEF";
	const char cmdCur[] = "AT+CWMODE_CUR";

	char* cmd = (char*)(storeInFlash ? cmdDef : cmdCur);

	return WiFi_b8SendCommand(
		module, cmd, paramArr, paramTypeArr, 1, "OK",
		WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS);
}

b8 WiFi_b8ConnectToAP(WiFi_t* module, char* SSID, char* pass, b8 storeInFlash)
{
	/*	prepare paramArr	*/
	char* paramArr[] = {SSID, pass};

	/*	prepare paramTypeArr	*/
	WiFi_Parameter_t paramTypeArr[] = {
		WiFi_Parameter_String, WiFi_Parameter_String};

	/*	enter command	*/
	const char cmdDef[] = "AT+CWJAP_DEF";
	const char cmdCur[] = "AT+CWJAP_CUR";

	char* cmd = (char*)(storeInFlash ? cmdDef : cmdCur);

	return WiFi_b8SendCommand(
		module, cmd, paramArr, paramTypeArr, 2, "OK",
		WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS);
}

b8 WiFi_b8CreateAP(WiFi_t* module, char* SSID, char* pass, b8 storeInFlash)
{
	/*	prepare paramArr	*/
	char* paramArr[] = {SSID, pass};

	/*	prepare paramTypeArr	*/
	WiFi_Parameter_t paramTypeArr[] = {
		WiFi_Parameter_String, WiFi_Parameter_String};

	/*	enter command	*/
	const char cmdDef[] = "AT+CWSAP_DEF";
	const char cmdCur[] = "AT+CWSAP_CUR";

	char* cmd = (char*)(storeInFlash ? cmdDef : cmdCur);

	return WiFi_b8SendCommand(
		module, cmd, paramArr, paramTypeArr, 2, "OK",
		WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS);
}

b8 WiFi_b8SetIP(
	WiFi_t* module, char* ip, char* gateway, char* netmask, b8 storeInFlash)
{
	/*	prepare paramArr	*/
	char* paramArr[] = {ip, gateway, netmask};

	/*	prepare paramTypeArr	*/
	WiFi_Parameter_t paramTypeArr[] = {
		WiFi_Parameter_String, WiFi_Parameter_String, WiFi_Parameter_String};

	/*	enter command	*/
	const char cmdDef[] = "AT+CIPSTA_DEF";
	const char cmdCur[] = "AT+CIPSTA_CUR";

	char* cmd = (char*)(storeInFlash ? cmdDef : cmdCur);

	return WiFi_b8SendCommand(
		module, cmd, paramArr, paramTypeArr, 3, "OK",
		WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS);
}

/*******************************************************************************
 *	TCP (and UDP):
 ******************************************************************************/
//b8 WiFi_b8GetStatus(WiFi_t* module, WiFi_Status_t* s)	//TODO
//{
//
//}

b8 WiFi_b8GetIpFromDns(WiFi_t* module, char* domainStr)
{
	/*	prepare paramArr	*/
	char* paramArr[] = {domainStr};

	/*	prepare paramTypeArr	*/
	WiFi_Parameter_t paramTypeArr[] = {WiFi_Parameter_String};

	/*	enter command	*/
	/*
	 * TODO: try it. what does sit terminate with? it is not 'OK'.
	 */
	return WiFi_b8SendCommand(
		module, "AT+CIPDOMAIN", paramArr, paramTypeArr, 1, "OK",
		WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS);
}

b8 WiFi_b8SetMultipleConnections(WiFi_t* module, b8 state)
{
	/*	prepare paramArr	*/
	char paramStr[2];
	paramStr[0] = '0' + state;
	paramStr[1] = '\0';

	char* paramArr[] = {paramStr};

	/*	prepare paramTypeArr	*/
	WiFi_Parameter_t paramTypeArr[] = {WiFi_Parameter_Numerical};

	/*	enter command	*/
	return WiFi_b8SendCommand(
		module, "AT+CIPMUX", paramArr, paramTypeArr, 1, "OK",
		WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS);
}

b8 WiFi_b8ConnectToTcpSingleConnection(
	WiFi_t* module, char* address, char* port)
{
	/*	prepare paramArr	*/
	char* paramArr[] = {"TCP", address, port};

	/*	prepare paramTypeArr	*/
	WiFi_Parameter_t paramTypeArr[] = {
		WiFi_Parameter_String, WiFi_Parameter_String, WiFi_Parameter_Numerical};

	/*	enter command	*/
	return WiFi_b8SendCommand(
		module, "AT+CIPSTART", paramArr, paramTypeArr, 3, "OK",
		WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS);
}

b8 WiFi_b8ConnectToTcpMultipleConnections(
	WiFi_t* module, u8 linkId, char* address, char* port)
{
	/*	prepare paramArr	*/
	char linkIdStr[2];
	linkIdStr[0] = '0' + linkId;
	linkIdStr[1] = '\0';

	char* paramArr[] = {linkIdStr, "TCP", address, port};

	/*	prepare paramTypeArr	*/
	WiFi_Parameter_t paramTypeArr[] = {
		WiFi_Parameter_Numerical, WiFi_Parameter_String,
		WiFi_Parameter_String, WiFi_Parameter_Numerical
	};

	/*	enter command	*/
	return WiFi_b8SendCommand(
		module, "AT+CIPSTART", paramArr, paramTypeArr, 4, "OK",
		WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS);
}

//void WiFi_voidWaitLastSendToBeDone(WiFi_t* module)
//{
//	//	TODO
//}








b8 WiFi_b8ValidateJustExecuted(WiFi_t* module, u32 timeout)
{
	char str[WIFI_MAX_RESPONSE_LEN];

	b8 ok = UART_b8ReceiveStringTimeout(module->uartUnitNumber, str, timeout, "OK");

	return ok;
}





b8 WiFi_b8SelectMultipleConnections(
	WiFi_t* module, b8 mode)
{
	/*	flush parasitic remaining data byte	*/
	UART_voidFlushDataReceiveRegister(module->uartUnitNumber);

	/*	send command class header	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_IP_CMD_HEADER);

	/*	send command	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_MULTIPLE_CONNECTIONS);

	/*	send command param	*/
	UART_voidSendByteEcho(module->uartUnitNumber, '=');
	UART_voidSendByteEcho(module->uartUnitNumber, mode + '0');

	/*	enter command	*/
	WiFi_voidEnter(module);

	/*	validate execution	*/
	b8 valid = WiFi_b8ValidateJustExecuted(module, 2000);

	#if DEBUG_ON
	if (valid)
		trace_printf("selected multiple connections success\n");
	#endif

	return valid;
}

b8 WiFi_b8ConnectToTCP(WiFi_t* module, char* address, char* port, u8 linkId)
{
	/*	flush parasitic remaining data byte	*/
	UART_voidFlushDataReceiveRegister(module->uartUnitNumber);

	/*	send command class header	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_IP_CMD_HEADER);

	/*	send command	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_CONNECT_TCP_IP);

	/*	send command params	*/
	UART_voidSendByteEcho(module->uartUnitNumber, '=');
	UART_voidSendByteEcho(module->uartUnitNumber, linkId + '0');
	UART_voidSendStringEcho(module->uartUnitNumber, ",\"TCP\",\"");
	UART_voidSendStringEcho(module->uartUnitNumber, address);
	UART_voidSendStringEcho(module->uartUnitNumber, "\",");
	UART_voidSendStringEcho(module->uartUnitNumber, port);

	/*	enter command	*/
	WiFi_voidEnter(module);

	/*	validate execution	*/
	b8 valid = WiFi_b8ValidateJustExecuted(module, 5000);

	#if DEBUG_ON
	if (valid)
		trace_printf("connected to TCP success\n");
	#endif

	return valid;
}

b8 WiFi_b8ConnectToUDP(WiFi_t* module, char* address, char* port, u8 linkId)
{
	/*	flush parasitic remaining data byte	*/
	UART_voidFlushDataReceiveRegister(module->uartUnitNumber);

	/*	send command class header	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_IP_CMD_HEADER);

	/*	send command	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_CONNECT_TCP_IP);

	/*	send command params	*/
	UART_voidSendByteEcho(module->uartUnitNumber, '=');
	UART_voidSendByteEcho(module->uartUnitNumber, linkId + '0');
	UART_voidSendStringEcho(module->uartUnitNumber, ",\"UDP\",\"");
	UART_voidSendStringEcho(module->uartUnitNumber, address);
	UART_voidSendStringEcho(module->uartUnitNumber, "\",");
	UART_voidSendStringEcho(module->uartUnitNumber, port);

	/*	enter command	*/
	WiFi_voidEnter(module);

	/*	validate execution	*/
	b8 valid = WiFi_b8ValidateJustExecuted(module, 5000);

	#if DEBUG_ON
	if (valid)
		trace_printf("connected to UDP success\n");
	#endif

	return valid;
}

/*
 * (notice: multiple connections mode must be on)
 */
b8 WiFi_b8CreateTCP(WiFi_t* module, char* port)
{
	/*	flush parasitic remaining data byte	*/
	UART_voidFlushDataReceiveRegister(module->uartUnitNumber);

	/*	send command class header	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_IP_CMD_HEADER);

	/*	send command	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_CREATE_TCP_SERVER);

	/*	send command params	*/
	UART_voidSendStringEcho(module->uartUnitNumber, "=1,");
	UART_voidSendStringEcho(module->uartUnitNumber, port);

	/*	enter command	*/
	WiFi_voidEnter(module);

	/*	validate execution	*/
	b8 valid = WiFi_b8ValidateJustExecuted(module, 10000);

	#if DEBUG_ON
	if (valid)
		trace_printf("created TCP success\n");
	#endif

	return valid;
}

b8 WiFi_b8SendStringTCP(WiFi_t* module, char* str, u16 len, u8 linkId)
{
	/*	flush parasitic remaining data byte	*/
	UART_voidFlushDataReceiveRegister(module->uartUnitNumber);

	/*	send command class header	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_IP_CMD_HEADER);

	/*	send command	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_SEND_TCP);

	/*	send command params	*/
	UART_voidSendByteEcho(module->uartUnitNumber, '=');
	UART_voidSendByteEcho(module->uartUnitNumber, linkId + '0');
	UART_voidSendByteEcho(module->uartUnitNumber, ',');
	char tempStr[6];
	sprintf(tempStr, "%u", len);
	UART_voidSendStringEcho(module->uartUnitNumber, tempStr);

	/*	enter command	*/
	WiFi_voidEnter(module);

	/*	wait for wrap return	*/
	b8 wrapReturn =
		UART_b8ReceiveStringTimeout(module->uartUnitNumber, tempStr, 1000, ">");
	if (!wrapReturn)
	{
		WIFI_ERROR_HANDLER;
	}

	/*	flush after wrap return	*/
	UART_voidFlushDataReceiveRegister(module->uartUnitNumber);

	/*	send string	*/
	UART_voidSendString(module->uartUnitNumber, str);

	/*	validate execution	*/
	b8 valid = WiFi_b8ValidateJustExecuted(module, 10000);
	return valid;
}

u64 WiFi_u64TCPMeasureTime(WiFi_t* module, u8 byte, u8 linkId)
{
	/*	flush parasitic remaining data byte	*/
	UART_voidFlushDataReceiveRegister(module->uartUnitNumber);

	/*	send command class header	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_IP_CMD_HEADER);

	/*	send command	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_SEND_TCP);

	/*	send command params	*/
	UART_voidSendByteEcho(module->uartUnitNumber, '=');
	UART_voidSendByteEcho(module->uartUnitNumber, linkId + '0');
	UART_voidSendStringEcho(module->uartUnitNumber, ",1");

	/*	enter command	*/
	WiFi_voidEnter(module);

	/*	wait for wrap return	*/
	char tempStr[WIFI_MAX_RESPONSE_LEN];
	b8 wrapReturn =
		UART_b8ReceiveStringTimeout(module->uartUnitNumber, tempStr, 1000, ">");
	if (!wrapReturn)
	{
		WIFI_ERROR_HANDLER;
	}

	/*	flush after wrap return	*/
	UART_voidFlushDataReceiveRegister(module->uartUnitNumber);

	/*	send byte	*/
	UART_enumSendByte(module->uartUnitNumber, byte);

	/*	take timestamp	*/
	u64 tSent = STK_u64GetElapsedTicks();

	/*	validate execution	*/
	b8 valid = WiFi_b8ValidateJustExecuted(module, 10000);

	#if DEBUG_ON
	if (!valid)
		trace_printf("could not send byte!\n");
	#endif

	/*	wait for ack byte	*/
	char str[WIFI_MAX_RESPONSE_LEN];
	u8 senderId;
	WiFi_u16ReadStringTCP(module, str, &senderId);

	/*	calculate elapsed time	*/
	u64 tElapsed = STK_u64GetElapsedTicks() - tSent;

	if (valid)
		return tElapsed;
	else
		return 0;
}






