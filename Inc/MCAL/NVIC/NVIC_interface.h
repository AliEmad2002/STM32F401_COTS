/*
 * NVIC_interface.h
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef NVIC_INTERFACE_H_
#define NVIC_INTERFACE_H_

typedef enum{
	MNVIC_INTERRUPT_WWDG =					0,
	MNVIC_INTERRUPT_EXTI16_PVD =			1,
	MNVIC_INTERRUPT_EXTI21_TAMP_STAMP =		2,
	MNVIC_INTERRUPT_EXTI22_RTC_WKUP =		3,
	MNVIC_INTERRUPT_FLASH =					4,
	MNVIC_INTERRUPT_RCC =					5,
	MNVIC_INTERRUPT_EXTI0 =					6,
	MNVIC_INTERRUPT_EXTI1 =					7,
	MNVIC_INTERRUPT_EXTI2 =					8,
	MNVIC_INTERRUPT_EXTI3 =					9,
	MNVIC_INTERRUPT_EXTI4 =					10
	/*	CONTINUE LATER	*/
}MNVIC_INTERRUPT_t;

void MNVIC_voidEnableInterrupt(MNVIC_INTERRUPT_t interrupt);

void MNVIC_voidDisbleInterrupt(MNVIC_INTERRUPT_t interrupt);

void MNVIC_voidSetPendingFlag(MNVIC_INTERRUPT_t interrupt);

void MNVIC_voidClearPendingFlag(MNVIC_INTERRUPT_t interrupt);

b8 MNVIC_b8GetActiveFlag(MNVIC_INTERRUPT_t interrupt);



#endif /* NVIC_INTERFACE_H_ */
