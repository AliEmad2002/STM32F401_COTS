/*
 * FPU_private.h
 *
 *  Created on:	Sep 18, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef _FPU_PRIVATE_H_
#define _FPU_PRIVATE_H_

#include "Target_config.h"

#if TARGET_ID == STM32F401x
#define FPU_SPACR (*(volatile u32*) 0xE000ED88)
#endif

#endif /* _FPU_PRIVATE_H_ */
