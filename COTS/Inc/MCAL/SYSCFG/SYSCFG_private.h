/*
 * SYSCFG_private.h
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef SYSCFG_PRIVATE_H_
#define SYSCFG_PRIVATE_H_

#include "Target_config.h"
#if TARGET_ID == STM32F401x
#define SYSCFG_BASE_ADD			((u32)0x40013800)

#define SYSCFG_EXTICR			((volatile u32*)(SYSCFG_BASE_ADD + 0x08))

#define SYSCFG_EXTICR1			(*(volatile u32*)(SYSCFG_BASE_ADD + 0x08))
#endif

#endif /* SYSCFG_PRIVATE_H_ */
