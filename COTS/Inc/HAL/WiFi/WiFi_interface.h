/*
 * WiFi_interface.h
 *
 *  Created on: Oct 14, 2022
 *      Author: Ali Emad Ali
 *
 *
 * All "b8" functions return "true" if done successfully, "false" otherwise. Unless
 * otherwise mentioned.
 */

#ifndef HAL_WIFI_WIFI_INTERFACE_H_
#define HAL_WIFI_WIFI_INTERFACE_H_

#include "WiFi_config.h"

typedef enum{
	WiFi_Parameter_String,
	WiFi_Parameter_Numerical
}WiFi_Parameter_t;

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

typedef enum{
	WiFi_AP_Status_GotIP = 2,
	WiFi_AP_Status_Connected,
	WiFi_AP_Status_Disonnected,
	WiFi_AP_Status_ConnectionFail
}WiFi_AP_Status_t;

typedef enum{
	WiFi_Link_Protocol_TCP,
	WiFi_Link_Protocol_UDP
}WiFi_Link_Protocol_t;

typedef enum{
	WiFi_Link_Connection_Client,
	WiFi_Link_Connection_Server
}WiFi_Link_Connection_t;

typedef struct{
	b8 isUsed;
	WiFi_Link_Protocol_t protocol;
	char remoteIpStr[16];
	char remotePortStr[6];
	char localPortStr[6];
	WiFi_Link_Connection_t connectionType;
}WiFi_Link_Status_t;

typedef struct{
	WiFi_AP_Status_t apStatus;
	WiFi_Link_Status_t link[5];
}WiFi_Status_t;

typedef enum{
	WiFi_FtpFile_Ascii,
	WiFi_FtpFile_Binary
}WiFi_FtpFile_t;

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
 *	Module availability:
 ******************************************************************************/
/*
 * Checks availability of module.
 *
 * Notice: can't be used while module is performing an operation, like: connecting
 * to WiFi AP, sending TCP segment, etc.
 */
b8 WiFi_b8IsModuleAvailable(WiFi_t* module);

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
 *
 * Output may be read in: "module->buffer"
 */
b8 WiFi_b8GetIpFromDns(WiFi_t* module, char* domainStr);

/*	enables / disables multiple TCP connections	*/
b8 WiFi_b8SetMultipleConnections(WiFi_t* module, b8 state);

/*
 * Connects to TCP server.
 */
b8 WiFi_b8ConnectToTcpSingleConnection(
	WiFi_t* module, char* address, char* port);

b8 WiFi_b8ConnectToTcpMultipleConnections(
	WiFi_t* module, u8 linkId, char* address, char* port);

b8 WiFi_b8WaitSendOK(WiFi_t* module);

/*
 * Sends data. (Selected connection must be previously open)
 *
 * Length of "str" is maximumly 2048 bytes.
 *
 * Do not call this function two consecutive times without waiting for first
 * send operation to be done. Use: "WiFi_voidWaitLastSendToBeDone()"
 *
 * "linkId" is ignored if multiple connections was not enabled.
 */
b8 WiFi_b8SendData(WiFi_t* module, char* str, u8 linkId);

/*
 * Same as the previous function, except that it send required length and does not
 * terminate at '\0'. Useful when sending binary data.
 */
b8 WiFi_b8SendDataCustomLen(
	WiFi_t* module, char* dataStr, u16 dataLen, u8 linkId);

/*
 * Waits for new received data with certain timeout, then receives in modules buffer
 */
b8 WiFi_b8Recv(WiFi_t* module, u8* linkIdPtr, u16 msTimeout);

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

/*******************************************************************************
 *	FTP:
 *	Note: multiple connections MUST be enabled.
 ******************************************************************************/
/*
 * Connects as a client to an FTP server (over TCP).
 *
 * For anonymous logins, set "user" and "pass" to: "NULL".
 */
b8 WiFi_b8ConnectToFTP(
	WiFi_t* module, char* ip, char* port, char* user, char* pass, u8 cmdlinkId);

/*
 * sets FTP transfer data type.
 *
 * When transferring text data, set "type" to "WiFi_FtpFile_Ascii". And to
 * prevent data loss when transferring images, executables, .elf's, ..etc, set
 * "type" to "WiFi_FtpFile_Binary".
 */
b8 WiFi_b8SetFtpDataType(WiFi_t* module, u8 cmdLinkId, WiFi_FtpFile_t type);

/*	opens passive FTP connection	*/
b8 WiFi_b8OpenFtpPassiveConnection(WiFi_t* module, u8 cmdLinkId, u8 dataLinkId);

/*
 * Downloads small file from FTP server (stored in "module"'s "buffer")
 *
 * Notice that:
 * 	-	FTP server must be initially connected and logged into, on "cmdLinkId".
 *
 * 	-	"dataLinkId" would be used in passive data transfer, so, make sure it is
 * 		not being used before using in this function.
 *
 *	-	When transferring text data, set "type" to "WiFi_FtpFile_Ascii". And to
 *		prevent data loss when transferring images, executables, .elf's, ..etc,
 *		set "type" to "WiFi_FtpFile_Binary".
 *
 * 	-	if size of the file is larger than "WIFI_MAX_RESPONSE_LEN" (buffer's
 * 		size), only the buffer would be filled, and rest of the data would be
 * 		ignored.
 * */
b8 WiFi_b8DownloadSmallFtpFile(
	WiFi_t* module, u8 cmdLinkId, u8 dataLinkId, WiFi_FtpFile_t type,
	char* fileNameStr);

/*
 * Uploads small file to FTP server.
 *
 * Notice that:
 * 	-	FTP server must be initially connected and logged into, on "cmdLinkId".
 *
 * 	-	"dataLinkId" would be used in passive data transfer, so, make sure it is
 * 		not being used before using in this function.
 *
 *	-	When transferring text data, set "type" to "WiFi_FtpFile_Ascii". And to
 *		prevent data loss when transferring images, executables, .elf's, ..etc,
 *		set "type" to "WiFi_FtpFile_Binary".
 * */
b8 WiFi_b8UploadSmallFtpFile(
	WiFi_t* module, u8 cmdLinkId, u8 dataLinkId, WiFi_FtpFile_t type,
	char* fileNameStr, char* filedataArr, u16 fileSizeInBytes);





#endif /* HAL_WIFI_WIFI_INTERFACE_H_ */
