/*
 * FPU_interface.h
 *
 *  Created on:	Sep 18, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef _FPU_INTERFACE_H_
#define _FPU_INTERFACE_H_

#include "Target_config.h"

#if TARGET_ID == STM32F401x
void FPU_voidEnable(void);
#endif

#endif /* _FPU_INTERFACE_H_ */
