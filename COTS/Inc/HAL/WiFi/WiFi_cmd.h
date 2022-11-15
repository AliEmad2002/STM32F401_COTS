/*
 * WiFi_cmd.h
 *
 *  Created on: Oct 14, 2022
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_HAL_WIFI_WIFI_CMD_H_
#define INCLUDE_HAL_WIFI_WIFI_CMD_H_

#define WIFI_REPLY_TERMINATOR		'\n'

#define WIFI_WIFI_CMD_HEADER		"AT+CW"
#define WIFI_IP_CMD_HEADER			"AT+CIP"

#define WIFI_DISABLE_ECHO			"ATE0\r\n"

#define WIFI_STORE_IN_FLASH			"_DEF"
#define WIFI_STORE_IN_RAM			"_CUR"

#define WIFI_MODE					"MODE"

#define WIFI_CONNECT_AP				"JAP"

#define WIFI_CREATE_AP				"SAP"

#define WIFI_CONNECT_TCP_IP			"START"

#define WIFI_MULTIPLE_CONNECTIONS	"MUX"

#define WIFI_SEND_TCP				"SEND"

#define WIFI_CREATE_TCP_SERVER		"SERVER"

#define WIFI_SET_IP					"STA"


#endif /* INCLUDE_HAL_WIFI_WIFI_CMD_H_ */
