/*
 * WiFi_interface.h
 *
 *  Created on: Oct 14, 2022
 *      Author: Ali Emad Ali
 */

#ifndef HAL_WIFI_WIFI_INTERFACE_H_
#define HAL_WIFI_WIFI_INTERFACE_H_


typedef struct{
	GPIO_PortName_t 	rstPort;
	u8 					rstPin;
	UART_UnitNumber_t	uartUnitNumber;
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

typedef enum{
	WiFi_MultiplieConnection_Off,
	WiFi_MultiplieConnection_On
}WiFi_MultiplieConnection_t;

/*
 * note: UART unit must be previously initialized with what is most compatible
 * with WiFi module.
 */
void WiFi_voidInit(
	WiFi_t* module, GPIO_Pin_t rstPin,
	UART_UnitNumber_t uartUnitNumber, u32 baudrate, u8 uartAfioMap);

b8 WiFi_b8HardReset(WiFi_t* module);

void WiFi_voidRead(WiFi_t* module, char* str);

b8 WiFi_b8SelectMode(WiFi_t* module, WiFi_Mode_t mode, b8 storeInFlash);

b8 WiFi_b8ConnectToAP(WiFi_t* module, char* SSID, char* pass, b8 storeInFlash);

b8 WiFi_b8CreateAP(WiFi_t* module, char* SSID, char* pass, b8 storeInFlash);

b8 WiFi_b8SelectMultipleConnections(
	WiFi_t* module, WiFi_MultiplieConnection_t mode);

b8 WiFi_b8ConnectToTCP(WiFi_t* module, char* address, char* port, u8 linkId);

b8 WiFi_b8ConnectToUDP(WiFi_t* module, char* address, char* port, u8 linkId);

b8 WiFi_b8CreateTCP(WiFi_t* module, char* port);

b8 WiFi_b8SendStringTCP(WiFi_t* module, char* str, u16 len);

u64 WiFi_u64TCPMeasureTime(WiFi_t* module, u8 byte, u8 linkId);

u16 WiFi_u16ReadStringTCP(WiFi_t* module, char* byte, u8* senderId);

b8 WiFi_b8SetIP(WiFi_t* module, char* ip, char* gateway, char* netmask, b8 storeInFlash);

b8 WiFi_b8FactoryReset(WiFi_t* module);








#endif /* HAL_WIFI_WIFI_INTERFACE_H_ */
