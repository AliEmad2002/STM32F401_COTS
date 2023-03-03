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
	u16 bufferUsedLen;
	b8 cmdSuccessful = UART_b8ReceiveStringTimeout(
		module->uartUnitNumber, module->buffer, msTimeout, successResponse,
		&bufferUsedLen);

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
	if (rstPin != GPIO_Pin_NotConnected)
	{
		module->rstPort = rstPin / 16;

		module->rstPin = rstPin % 16;

		GPIO_voidSetPinGpoPushPull(module->rstPort, module->rstPin);

		/*	as pin is active low, set normally high	*/
		GPIO_voidSetPinOutputLevel(
			module->rstPort, module->rstPin, GPIO_OutputLevel_High);
	}
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
	if (module->rstPin == GPIO_Pin_NotConnected)
		return false;

	/*	1 - second pulse on rst pin	*/
	GPIO_voidSetPinOutputLevel(module->rstPort, module->rstPin, GPIO_OutputLevel_Low);

	Delay_voidBlockingDelayMs(1000);

	GPIO_voidSetPinOutputLevel(module->rstPort, module->rstPin, GPIO_OutputLevel_High);

	/*	check for success	*/
	u16 bufferUsedLen;
	b8 resetSuccessful = UART_b8ReceiveStringTimeout(
		module->uartUnitNumber, module->buffer,
		WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS, "ready", &bufferUsedLen);

	/*	return status	*/
	return resetSuccessful;
}

b8 WiFi_b8SoftReset(WiFi_t* module)
{
	return WiFi_b8SendCommand(
		module, "AT+RST", NULL, NULL, 0, "ready",
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
		WIFI_AP_CONNECT_TIMEOUT_MS);
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

b8 WiFi_b8SendData(WiFi_t* module, char* str, u8 linkId)
{
	/*	get length of data	*/
	u16 dataLen = strlen(str);

	if (dataLen > 2048)
		return false;

	/*	prepare paramArr	*/
	char linkIdStr[2];
	linkIdStr[0] = '0' + linkId;
	linkIdStr[1] = '\0';

	char dataLenStr[5];
	sprintf(dataLenStr, "%u", (u32)dataLen);

	char* paramArr[] = {linkIdStr, dataLenStr};

	/*	prepare paramTypeArr	*/
	WiFi_Parameter_t paramTypeArr[] = {
		WiFi_Parameter_Numerical, WiFi_Parameter_Numerical
	};

	/*	enter command, and wait for wrap return	*/
	b8 cmdSuccess = WiFi_b8SendCommand(
		module, "AT+CIPSEND", paramArr, paramTypeArr, 2, ">",
		WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS);

	if (!cmdSuccess)
		return false;

	Delay_voidBlockingDelayMs(1);

	/*	send data	*/
	UART_voidSendString(module->uartUnitNumber, str);

	return true;

}

b8 WiFi_b8Recv(WiFi_t* module, u8* linkIdPtr, u16 msTimeout)
{
	u64 startTime = STK_u64GetElapsedTicks();

	/**
	 * Notice: TCP segments are sent from esp8266 as the following example:
	 * +IPD,0,6:Hello!
	 *
	 * Such that first number '0' is the receiving linkId, and second number '6'
	 * is length of the data.
	 **/
	/*	wait for ':'	*/
	u16 bufferUsedLen;
	if (!UART_b8ReceiveStringTimeout(
		module->uartUnitNumber, module->buffer, msTimeout, ":", &bufferUsedLen))
	{
		trace_printf("failed at 1\n");
		return false;
	}

	/*
	 * extract length of the received data
	 * it would be characters between the second ',' and the ':'.
	 *
	 * Remember that ':' would be at the index: "bufferUsedLen" - 1
	 */
	u16 dataLen = 0;
	u16 tenPow = 1;

	u8 i = bufferUsedLen - 2;

	for (; module->buffer[i] != ','; i--)
	{
		dataLen += (u16)(module->buffer[i] - '0') * tenPow;

		tenPow *= 10;
	}

	/*	extract linkID	*/
	*linkIdPtr = (u8)(module->buffer[i - 1] - '0');

	/*	read message / segment.	*/
	/**
	 * TODO: extracting message length and linkID may cause program missing some
	 * bytes from the message in case baudrate was too high for processing frequency.
	 * To solve this problem, use interrupt receiving in another buffer.
	 **/
	for (u16 i = 0; i < dataLen; i++)
	{
		(void)UART_enumReciveByte(module->uartUnitNumber, &module->buffer[i]);

		if (STK_u64GetElapsedTicks() - startTime > msTimeout * STK_TICKS_PER_MS)
		{
			trace_printf("failed at 2\n");
			return false;
		}
	}

	trace_printf("LinkID = %u\n", (u32)(*linkIdPtr));

	module->buffer[dataLen] = '\0';

	return true;
}

/*******************************************************************************
 *	FTP:
 ******************************************************************************/
b8 WiFi_b8ConnectToFTP(
	WiFi_t* module, char* ip, char* port, char* user, char* pass, u8 cmdlinkId)
{
	b8 cmdSuccess;
	u8 receivedLinkId;

	/*	enable multiple connections	*/
	cmdSuccess = WiFi_b8SetMultipleConnections(module, true);

	if (!cmdSuccess)
		return false;

	Delay_voidBlockingDelayMs(1);	//	Avoiding "non-valid echo" problem.

	/*	connect to server over TCP	*/
	cmdSuccess =
		WiFi_b8ConnectToTcpMultipleConnections(module, cmdlinkId, ip, port);

	if (!cmdSuccess)
		return false;

	/*	wait for server's welcoming info	*/
	cmdSuccess = WiFi_b8Recv(
		module, &receivedLinkId, WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS);

	if (!cmdSuccess || (receivedLinkId != cmdlinkId))
		return false;

	/*	log-in	*/
	char tempBuffer[WIFI_MAX_PARAM_LEN];

	if (user != NULL)
	{
		Delay_voidBlockingDelayMs(1);

		/*	prepare "USER" command	*/
		sprintf(tempBuffer, "USER %s\r\n", user);

		/*	send it	*/
		cmdSuccess = WiFi_b8SendData(module, tempBuffer, cmdlinkId);

		if (!cmdSuccess)
			return false;
	}

	if (pass != NULL)
	{
		/*	wait for server to require password	*/
		cmdSuccess = WiFi_b8Recv(
			module, &receivedLinkId, WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS);

		if (!cmdSuccess || (receivedLinkId != cmdlinkId))
			return false;

		/*	prepare "PASS" command	*/
		sprintf(tempBuffer, "PASS %s\r\n", pass);

		/*	send it	*/
		cmdSuccess = WiFi_b8SendData(module, tempBuffer, cmdlinkId);

		if (!cmdSuccess)
			return false;
	}

	/*	wait for server to send ack	*/
	cmdSuccess = WiFi_b8Recv(
		module, &receivedLinkId, WIFI_COMMAND_ACK_RESPONSE_TIMEOUT_MS);

	return true;
}













