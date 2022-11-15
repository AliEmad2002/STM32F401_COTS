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



void WiFi_voidInit(
	WiFi_t* module, GPIO_Pin_t rstPin,
	UART_UnitNumber_t uartUnitNumber, u32 baudrate, u8 uartAfioMap)
{
	module->rstPort = rstPin / 16;
	module->rstPin = rstPin % 16;
	GPIO_voidSetPinGpoPushPull(module->rstPort, module->rstPin);
	GPIO_voidSetPinOutputLevel(
		module->rstPort, module->rstPin, GPIO_OutputLevel_High);

	module->uartUnitNumber = uartUnitNumber;

	UART_voidFastInit(uartUnitNumber, baudrate, uartAfioMap);
}

b8 WiFi_b8HardReset(WiFi_t* module)
{
	GPIO_voidSetPinOutputLevel(module->rstPort, module->rstPin, GPIO_OutputLevel_Low);
	Delay_voidBlockingDelayMs(1000);
	GPIO_voidSetPinOutputLevel(module->rstPort, module->rstPin, GPIO_OutputLevel_High);

	char str[WIFI_MAX_RESPONSE_LEN];
	b8 ok = UART_b8ReceiveStringTimeout(module->uartUnitNumber, str, 5000, "ready");

	#if DEBUG_ON
	if (ok)
		trace_printf("Hard reset success\n");
	#endif

	return ok;
}

void WiFi_voidEnter(WiFi_t* module)
{
	UART_voidSendByteEcho(module->uartUnitNumber, '\r');
	UART_enumSendByte(module->uartUnitNumber, '\n');
}

b8 WiFi_b8ValidateJustExecuted(WiFi_t* module, u32 timeout)
{
	char str[WIFI_MAX_RESPONSE_LEN];

	b8 ok = UART_b8ReceiveStringTimeout(module->uartUnitNumber, str, timeout, "OK");

	return ok;
}

b8 WiFi_b8SelectMode(WiFi_t* module, WiFi_Mode_t mode, b8 storeInFlash)
{
	/*	flush parasitic remaining data byte	*/
	UART_voidFlushDataReceiveRegister(module->uartUnitNumber);

	/*	send command class header	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_WIFI_CMD_HEADER);

	/*	send command	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_MODE);

	/*	send command storing type	*/
	if (storeInFlash == true)
		UART_voidSendStringEcho(module->uartUnitNumber, WIFI_STORE_IN_FLASH);
	else
		UART_voidSendStringEcho(module->uartUnitNumber, WIFI_STORE_IN_RAM);

	/*	send command param	*/
	UART_voidSendByteEcho(module->uartUnitNumber, '=');
	UART_voidSendByteEcho(module->uartUnitNumber, '0' + mode);

	/*	enter command	*/
	WiFi_voidEnter(module);

	/*	validate execution	*/
	b8 valid = WiFi_b8ValidateJustExecuted(module, 2000);

	#if DEBUG_ON
	if (valid)
		trace_printf("select mode success\n");
	#endif

	return valid;
}

b8 WiFi_b8ConnectToAP(WiFi_t* module, char* SSID, char* pass, b8 storeInFlash)
{
	/*	flush parasitic remaining data byte	*/
	UART_voidFlushDataReceiveRegister(module->uartUnitNumber);

	/*	send command class header	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_WIFI_CMD_HEADER);

	/*	send command	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_CONNECT_AP);

	/*	send command storing type	*/
	if (storeInFlash == true)
		UART_voidSendStringEcho(module->uartUnitNumber, WIFI_STORE_IN_FLASH);
	else
		UART_voidSendStringEcho(module->uartUnitNumber, WIFI_STORE_IN_RAM);

	/*	send command params	*/
	UART_voidSendStringEcho(module->uartUnitNumber, "=\"");
	UART_voidSendStringEcho(module->uartUnitNumber, SSID);
	UART_voidSendStringEcho(module->uartUnitNumber, "\",\"");
	UART_voidSendStringEcho(module->uartUnitNumber, pass);
	UART_voidSendStringEcho(module->uartUnitNumber, "\"");

	/*	enter command	*/
	WiFi_voidEnter(module);

	/*	validate execution	*/
	b8 valid = WiFi_b8ValidateJustExecuted(module, 5000);

	#if DEBUG_ON
	if (valid)
		trace_printf("connect to AP success\n");
	#endif

	return valid;
}

b8 WiFi_b8CreateAP(WiFi_t* module, char* SSID, char* pass, b8 storeInFlash)
{
	/*	flush parasitic remaining data byte	*/
	UART_voidFlushDataReceiveRegister(module->uartUnitNumber);

	/*	send command class header	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_WIFI_CMD_HEADER);

	/*	send command	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_CREATE_AP);

	/*	send command storing type	*/
	if (storeInFlash == true)
		UART_voidSendStringEcho(module->uartUnitNumber, WIFI_STORE_IN_FLASH);
	else
		UART_voidSendStringEcho(module->uartUnitNumber, WIFI_STORE_IN_RAM);

	/*	send command params	*/
	UART_voidSendStringEcho(module->uartUnitNumber, "=\"");
	UART_voidSendStringEcho(module->uartUnitNumber, SSID);
	UART_voidSendStringEcho(module->uartUnitNumber, "\",\"");
	UART_voidSendStringEcho(module->uartUnitNumber, pass);
	UART_voidSendStringEcho(module->uartUnitNumber, "\",5,3");

	/*	enter command	*/
	WiFi_voidEnter(module);

	/*	validate execution	*/
	b8 valid = WiFi_b8ValidateJustExecuted(module, 10000);

	#if DEBUG_ON
	if (valid)
		trace_printf("created AP success\n");
	#endif

	return valid;
}

b8 WiFi_b8SelectMultipleConnections(
	WiFi_t* module, WiFi_MultiplieConnection_t mode)
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

b8 WiFi_b8SendStringTCP(WiFi_t* module, char* str, u16 len)
{
	/*	flush parasitic remaining data byte	*/
	UART_voidFlushDataReceiveRegister(module->uartUnitNumber);

	/*	send command class header	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_IP_CMD_HEADER);

	/*	send command	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_SEND_TCP);

	/*	send command params	*/
	UART_voidSendByteEcho(module->uartUnitNumber, '=');
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

u64 WiFi_u64SendByteTCPMeasureTime(WiFi_t* module, u8 byte, u8 linkId)
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

	/*	calculate elapsed time	*/
	u64 tElapsed = STK_u64GetElapsedTicks() - tSent;

	if (valid)
		return tElapsed;
	else
		return 0;
}

/*	TCP server must be initially created	*/
u16 WiFi_u16ReadStringTCP(WiFi_t* module, char* str, u8* senderId)
{
	/*	flush parasitic remaining data byte	*/
	UART_voidFlushDataReceiveRegister(module->uartUnitNumber);

	char headerStr[20];

	/*	receive header string (until ':')	*/
	u16 headerLen = UART_u16ReadString(module->uartUnitNumber, headerStr, ':', 1);

	/*	extract payload length from the header	*/
	u16 payloadLen = headerStr[headerLen - 2] - '0';

	/*	receive payload	*/
	for (u16 i = 0; i < payloadLen; i++)
	{
		UART_enumReciveByte(module->uartUnitNumber, str + i);
	}
	str[payloadLen] = '\0';

	/*	extract senderId	*/
	*senderId = headerStr[headerLen - 4] - '0';

	return payloadLen;
}

b8 WiFi_b8SetIP(WiFi_t* module, char* ip, char* gateway, char* netmask, b8 storeInFlash)
{
	/*	flush parasitic remaining data byte	*/
	UART_voidFlushDataReceiveRegister(module->uartUnitNumber);

	/*	send command class header	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_IP_CMD_HEADER);

	/*	send command	*/
	UART_voidSendStringEcho(module->uartUnitNumber, WIFI_SET_IP);

	/*	send command storing type	*/
	if (storeInFlash == true)
		UART_voidSendStringEcho(module->uartUnitNumber, WIFI_STORE_IN_FLASH);
	else
		UART_voidSendStringEcho(module->uartUnitNumber, WIFI_STORE_IN_RAM);

	/*	send command params	*/
	UART_voidSendStringEcho(module->uartUnitNumber, "=\"");
	UART_voidSendStringEcho(module->uartUnitNumber, ip);
	UART_voidSendStringEcho(module->uartUnitNumber, "\",\"");
	UART_voidSendStringEcho(module->uartUnitNumber, gateway);
	UART_voidSendStringEcho(module->uartUnitNumber, "\",\"");
	UART_voidSendStringEcho(module->uartUnitNumber, netmask);
	UART_voidSendStringEcho(module->uartUnitNumber, "\"");

	/*	enter command	*/
	WiFi_voidEnter(module);

	/*	validate execution	*/
	b8 valid = WiFi_b8ValidateJustExecuted(module, 10000);
	return valid;
}

b8 WiFi_b8FactoryReset(WiFi_t* module)
{
	/*	flush parasitic remaining data byte	*/
	UART_voidFlushDataReceiveRegister(module->uartUnitNumber);

	/*	send command	*/
	UART_voidSendStringEcho(module->uartUnitNumber, " AT+RESTORE");

	/*	enter command	*/
	WiFi_voidEnter(module);

	/*	wait for "ready"	*/
	char str[WIFI_MAX_RESPONSE_LEN];
	b8 ok = UART_b8ReceiveStringTimeout(module->uartUnitNumber, str, 5000, "ready");
	return ok;
}






