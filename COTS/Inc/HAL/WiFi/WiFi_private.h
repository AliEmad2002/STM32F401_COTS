/*
 * WiFi_private.h
 *
 *  Created on: Oct 14, 2022
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_HAL_WIFI_WIFI_PRIVATE_H_
#define INCLUDE_HAL_WIFI_WIFI_PRIVATE_H_

/*******************************************************************************
 *	Command entering:
 ******************************************************************************/
/*
 * Send command given:
 * 	-	Command string. (ex.: "AT+UART")
 *
 * 	-	Command parameters array. (An array of strings)
 *
 * 	-	Command type array.
 * 		Defines whether a certain parameter is a string (to be sent between double
 * 		quotations), or a numeric value (to be sent without quotes).
 *
 * 	-	Number of parameters.
 *
 * 	-	ESP success response. (string. ex.: "OK", "ready", etc)
 *
 * 	-	Timeout for success validation.
 */
b8 WiFi_b8SendCommand(
	WiFi_t* module,
	char* cmd,
	char* paramArr[],
	WiFi_Parameter_t paramTypeArr[], u8 nParams,
	char* successResponse, u16 msTimeout
);

/*
 * Sends the terminating "\r\n".
 */
void WiFi_voidEnter(WiFi_t* module);

//b8 WiFi_b8ValidateJustExecuted(WiFi_t* module, u32 timeout);

#endif /* INCLUDE_HAL_WIFI_WIFI_PRIVATE_H_ */
