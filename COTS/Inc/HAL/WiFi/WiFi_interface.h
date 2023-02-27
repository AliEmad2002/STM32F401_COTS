/*
 * WiFi_interface.h
 *
 *  Created on: Oct 14, 2022
 *      Author: Ali Emad Ali
 *
 *
 * All "b8" functions return "true" if done successfully, "false" otherwise. Unless
 * otherwise mentioned.
 *
 * TODO: This driver is under update.
 * Done interface file.
 * Implement!.
 */

#ifndef HAL_WIFI_WIFI_INTERFACE_H_
#define HAL_WIFI_WIFI_INTERFACE_H_

#include "WiFi_config.h"

typedef struct{
	GPIO_PortName_t 	rstPort;
	u8 					rstPin;
	UART_UnitNumber_t	uartUnitNumber;
	char				buffer[WIFI_MAX_RESPONSE_LEN];
}WiFi_t;

typedef enum{
	WiFi_Mode_Station = 1,
	WiFi_Mode_SoftAP,
	WiFi_Mode_SoftAP_Station
}WiFi_Mode_t;

typedef enum{
	WiFi_CommandType_WiFi,
	WiFi_CommandType_TCP_IP
}WiFi_CommandType_t;

typedef struct{

}WiFi_Status_t;

/*******************************************************************************
 *	Init:
 ******************************************************************************/
/*
 * Inits UART unit and rst pin.
 */
void WiFi_voidInit(
	WiFi_t* module, GPIO_Pin_t rstPin,
	UART_UnitNumber_t uartUnitNumber, u32 baudrate, u8 uartAfioMap);

/*******************************************************************************
 *	Reset:
 ******************************************************************************/
/*	Resets by HW, using the connected reset pin.	*/
b8 WiFi_b8HardReset(WiFi_t* module);

/*	resets by SW, using the AT command. (faster than HW reset)	*/
b8 WiFi_b8SoftReset(WiFi_t* module);

/*	Performs factory reset on the module.	*/
b8 WiFi_b8FactoryReset(WiFi_t* module);

/*******************************************************************************
 *	RF antenna voltage:
 ******************************************************************************/
/*
 * Sets RF VDD.
 *
 * Range: 1900 (i.e.:1.9V), 3300 (i.e.:3.3V)
 */
b8 WiFi_b8SetRfVdd(WiFi_t* module, u16 voltageMultipliedBy1000);

/*******************************************************************************
 *	WiFi:
 ******************************************************************************/
/*	Selects WiFi mode	*/
b8 WiFi_b8SelectMode(WiFi_t* module, WiFi_Mode_t mode, b8 storeInFlash);

/*
 * Connects to WiFi Access point. WiFi mode must be previously set to
 * "WiFi_Mode_Station" or "WiFi_Mode_SoftAP_Station".
 *
 * Notice: by try, hidden networks are not connectable.
 */
b8 WiFi_b8ConnectToAP(WiFi_t* module, char* SSID, char* pass, b8 storeInFlash);

/*
 * Creates Access point. WiFi mode must be previously set to
 * "WiFi_Mode_SoftAP" or "WiFi_Mode_SoftAP_Station".
 */
b8 WiFi_b8CreateAP(WiFi_t* module, char* SSID, char* pass, b8 storeInFlash);

/*	selects self IP address that DHCP will use.	*/
b8 WiFi_b8SetIP(
	WiFi_t* module, char* ip, char* gateway, char* netmask, b8 storeInFlash);

/*******************************************************************************
 *	TCP (and UDP):
 ******************************************************************************/
/*	Gets WiFi and TCP status of the module.	*/
b8 WiFi_b8GetStatus(WiFi_t* module, WiFi_Status_t* s);

/*
 * Gets IP address of a given string domain using DNS. Must be connected to the
 * Internet first.
 */
b8 WiFi_b8GetIpFromDns(WiFi_t* module, char* domainStr);

/*	enables / disables multiple TCP connections	*/
b8 WiFi_b8EnableMultipleConnections(WiFi_t* module);

b8 WiFi_b8DisableMultipleConnections(WiFi_t* module);

/*
 * Connects to TCP server.
 *
 * "linkId" is ignored if multiple connections was not enabled.
 */
b8 WiFi_b8ConnectToTCP(WiFi_t* module, char* address, char* port, u8 linkId);

/*
 * Connects to UDP server.
 *
 * "linkId" is ignored if multiple connections was not enabled.
 */
b8 WiFi_b8ConnectToUDP(WiFi_t* module, char* address, char* port, u8 linkId);

/*
 * Waits for TCP send operation to be done.
 *
 * It is done either by receiving "SEND OK" from the module, or one second since
 * last send operation started (stored statically) passes.
 *
 * If one second since last send operation passes, without even receiving
 * "SEND OK", function assumes previous send is done.
 */
void WiFi_voidWaitLastSendToBeDone(WiFi_t* module);

/*
 * Sends data. (Selected connection must be previously open)
 *
 * Length of "str" is maximumly 2048 bytes.
 *
 * Do not call this function two consecutive times without waiting for first
 * send operation to be done. Use: "WiFi_voidWaitLastSendToBeDone()"
 *
 * "linkId" is ignored if multiple connections was not enabled.
 *
 * TODO: use "AT+CIPSENDEX".
 */
b8 WiFi_b8SendData(WiFi_t* module, char* str, u8 linkId);

/*
 * Waits for new received data with certain timeout, then receives in modules buffer
 */
b8 WiFi_b8Recv(WiFi_t* module, u16 msTimeout);

/*
 * Closes TCP connection.
 *
 * "linkId" is ignored if multiple connections was not enabled.
 */
b8 WiFi_b8CloseConnection(WiFi_t* module, u8 linkId);

/*	Gets local IP address	*/
b8 WiFi_b8GetIpLocal(WiFi_t* module, char* ipStr);

/*
 * Configure module as TCP server.
 *
 * "timeOutInSeconds" is in range: 0, 7200.
 * Where 0 means non-ending timeout. (Espressif does not recommend that)
 *
 * Multiple connections must be previously enabled.
 * Use: "WiFi_b8EnableMultipleConnections()"
 */
b8 WiFi_b8CreateTcpServer(WiFi_t* module, char* port, u16 timeOutInSeconds);

/*	Deletes configured TCP server	*/
b8 WiFi_b8DeleteTcpServer(WiFi_t* module);

/*	pinging a certain domain	*/
b8 WiFi_b8Ping(WiFi_t* module, char* domainStr, u16* msTimeoutPtr);








#endif /* HAL_WIFI_WIFI_INTERFACE_H_ */
