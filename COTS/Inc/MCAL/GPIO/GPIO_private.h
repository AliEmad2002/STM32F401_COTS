/*
 * GPIO_private.h
 *
 *  Created on:	Jul 27, 2022
 *  Author:		Ali Emad Ali
 *
 *  References:
 *  	STM RM0368:	2.3 		Memory map
 *  	STM RM0368:	8.4.11 		GPIO register map
 */

#ifndef 	GPIO_PRIVATE_H_
#define		GPIO_PRIVATE_H_

#include "Target_config.h"

#if TARGET_ID == STM32F401x
typedef struct{
	u32 MODER;
	u32 OTYPER;
	u32 OSPEEDR;
	u32 PUPDR;
	u32 IDR;
	u32 ODR;
	u16 BSRRL;
	u16 BSRRH;
	u32 LCKR;
	u64 AFR;
}GPIO_t;

#define GPIO_A	((volatile GPIO_t*)0x40020000)
#define GPIO_B	((volatile GPIO_t*)0x40020400)
#define GPIO_C	((volatile GPIO_t*)0x40020800)

#elif TARGET_ID == STM32F10x
typedef struct{
	u64 CR;
	u32 IDR;
	u32 ODR;
	/*	in M3, BSRR should be accessed fully at once, can't other wise.	*/
	//u16 BSRRL;
	//u16 BSRRH;
	u32 BSRR;
	u32 BRR;
	u32 LCKR;
}GPIO_t;

#define GPIO_A	((volatile GPIO_t*)0x40010800)
#define GPIO_B	((volatile GPIO_t*)0x40010C00)
#define GPIO_C	((volatile GPIO_t*)0x40011000)

typedef struct{
	u32 EVCR;
	u32 MAPR;
	u32 EXTICR[4];
}AFIO_t;

#define AFIO	((volatile AFIO_t*)0x40010000)

#endif

volatile GPIO_t* const GPIO[] = {GPIO_A, GPIO_B, GPIO_C};

#endif	/*	GPIO_PRIVATE_H_	*/








