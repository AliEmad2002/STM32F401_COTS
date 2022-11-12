/*
 * EXTI_private.h
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef EXTI_PRIVATE_H_
#define EXTI_PRIVATE_H_

typedef struct{
	u32 IMR;
	u32 EMR;
	u32 RTSR;
	u32 FTSR;
	u32 SWIER;
	u32 PR;
}EXTI_t;

#if TARGET_ID == STM32F401x
#define EXTI ((volatile EXTI_t*)0x40013C00)

#elif TARGET_ID == STM32F10x
#define EXTI ((volatile EXTI_t*)0x40010400)
#endif

#endif /* EXTI_PRIVATE_H_ */
