/*
 * Downloading_FTP_File.c
 *
 *  Created on: Mar 3, 2023
 *      Author: Ali Emad
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Delay_interface.h"
#include "diag/trace.h"

/*	MCAL	*/
#include "RCC_interface.h"
#include "STK_interface.h"
#include "GPIO_interface.h"
#include "UART_interface.h"

/*	HAL	*/
#include "WiFi_interface.h"

#define AP_SSID		"WE2.4"
#define AP_PASS		"ali&anas123"

//#define FTP_IP		"172.107.162.11"
//#define FTP_PORT	"21"
//#define FTP_USER	"alitrypage.coolpage.biz"
//#define FTP_PASS	"900900"

#define FTP_IP		"185.27.134.11"
#define FTP_PORT	"21"
#define FTP_USER	"epiz_33715505"
#define FTP_PASS	"scpXn6XYdSexg"

int main(void)
{
	/*	init RCC system clock	*/
	RCC_voidSysClockInit();

	/*	init SysTick	*/
	STK_voidInit();
	STK_voidStartTickMeasure(STK_TickMeasureType_OverflowCount);
	STK_voidEnableSysTick();

	/*	power stabilization delay	*/
	Delay_voidBlockingDelayMs(1000);

	/*	init WiFi (ESP8266) object	*/
	WiFi_t esp8266;
	WiFi_voidInit(
		&esp8266, GPIO_Pin_NotConnected, UART_UnitNumber_2, 115200, 0);

	/*	wait for module to be ready	*/
	while(!WiFi_b8IsModuleAvailable(&esp8266));

	Delay_voidBlockingDelayMs(2);	//	Avoiding "non-valid echo" problem.

	/*	reset module	*/
	while(!WiFi_b8SoftReset(&esp8266));

	Delay_voidBlockingDelayMs(2);	//	Avoiding "non-valid echo" problem.

	/*	connect to WiFi network	*/
	while(!WiFi_b8SelectMode(&esp8266, WiFi_Mode_SoftAP_Station, true));

	Delay_voidBlockingDelayMs(2);	//	Avoiding "non-valid echo" problem.

	while(!WiFi_b8ConnectToAP(&esp8266, AP_SSID, AP_PASS, true));

	Delay_voidBlockingDelayMs(100);	//	Avoiding "non-valid echo" problem.

	/*	enable multiple connections	*/
	while(!WiFi_b8SetMultipleConnections(&esp8266, true));

	Delay_voidBlockingDelayMs(2);

	volatile b8 commandSuccess;

	/*	connect to FTP server	*/
	commandSuccess =
		WiFi_b8ConnectToFTP(&esp8266, FTP_IP, FTP_PORT, FTP_USER, FTP_PASS, 0);

	if (!commandSuccess)
	{
		trace_printf("can't connect to FTP server!\n");
		trace_printf("Terminating program.\n");
		return -1;
	}

	Delay_voidBlockingDelayMs(1);

	/*	download file	*/
	commandSuccess =
		WiFi_b8DownloadSmallFtpFile(
			&esp8266, 0, 2, WiFi_FtpFile_Ascii, "htdocs/file.txt");

	if (commandSuccess)
	{
		trace_printf("received file:\n");
		trace_printf(esp8266.buffer);
	}

	else
	{
		/*	get status (check that server is connected)	*/
		WiFi_Status_t s;
		WiFi_b8GetStatus(&esp8266, &s);

		/*	if server was not connected, re-connect and download	*/
		if (!s.link[0].isUsed)
		{
			/*	connect to FTP server	*/
			commandSuccess =
				WiFi_b8ConnectToFTP(&esp8266, FTP_IP, FTP_PORT, FTP_USER, FTP_PASS, 0);

			Delay_voidBlockingDelayMs(2);

			/*	download file	*/
			commandSuccess =
				WiFi_b8DownloadSmallFtpFile(
					&esp8266, 0, 2, WiFi_FtpFile_Ascii, "htdocs/file.txt");

			/*
			 * this time, if file still can't be downloaded, it must be another
			 * problem than connecting to the FTP server. like:
			 * 	-	Wrong user or password.
			 * 	-	Wrong file name or directory.
			 * 	-	cmdLinkID and/or dataLinkID is/are being used in other connection.
			 *
			 * And the application may implement its own way in encountering this
			 * fault. It does not have to terminate always as this example does.
			 */
			if (!commandSuccess)
			{
				trace_printf("Can't download file!");
				trace_printf("Terminating program.\n");
				return -1;
			}

		}
	}

	trace_printf("\n\nProgram done.\n\n");

	while(1)
	{

	}

	return 0;
}
