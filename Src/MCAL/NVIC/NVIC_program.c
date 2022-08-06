/*
 * NVIC_program.c
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#include "BIT_MATH.h"
#include "STD_TYPES.h"

#include "NVIC_interface.h"
#include "NVIC_private.h"


void MNVIC_voidEnableInterrupt(MNVIC_INTERRUPT_t interrupt)
{
	SET_BIT(NVIC_ISER[interrupt/32], interrupt%32);
}

void MNVIC_voidDisbleInterrupt(MNVIC_INTERRUPT_t interrupt)
{
	SET_BIT(NVIC_ICER[interrupt/32], interrupt%32);
}

void MNVIC_voidSetPendingFlag(MNVIC_INTERRUPT_t interrupt)
{
	SET_BIT(NVIC_ISPR[interrupt/32], interrupt%32);
}

void MNVIC_voidClearPendingFlag(MNVIC_INTERRUPT_t interrupt)
{
	SET_BIT(NVIC_ICPR[interrupt/32], interrupt%32);
}

b8 MNVIC_b8GetActiveFlag(MNVIC_INTERRUPT_t interrupt)
{
	return GET_BIT(NVIC_IABR[interrupt/32], interrupt%32);
}
