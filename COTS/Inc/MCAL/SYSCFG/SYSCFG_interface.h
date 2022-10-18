/*
 * SYSCFG_interface.h
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef SYSCFG_INTERFACE_H_
#define SYSCFG_INTERFACE_H_

#include "Target_config.h"

#if TARGET_ID == STM32F401x
void MSYSCFG_voidSetEXTILineSource(u8 line, GPIO_PortName_t port);
#endif

#endif /* SYSCFG_INTERFACE_H_ */
