/*
 * WiFi_private.h
 *
 *  Created on: Oct 14, 2022
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_HAL_WIFI_WIFI_PRIVATE_H_
#define INCLUDE_HAL_WIFI_WIFI_PRIVATE_H_

void WiFi_voidEnter(WiFi_t* module);

b8 WiFi_b8ValidateJustExecuted(WiFi_t* module, u16 timeout);

#endif /* INCLUDE_HAL_WIFI_WIFI_PRIVATE_H_ */
