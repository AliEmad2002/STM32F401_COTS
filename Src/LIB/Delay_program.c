/*
 * TEMPDELAY.c
 *
 *  Created on:	Jul 28, 2022
 *  Author:		Ali Emad Ali
 *
 */

/*	LIB	*/
#include "STD_TYPES.h"
#include "RCC_interface.h"

/*	SELF	*/
#include "Delay_interface.h"


void Delay_voidBlockingDelayMs(u32 ms)
{
	register u32 ticks = (RCC_u32GetSysClk()/7000)*ms;
	// 10000 is an experimental number, do better !
	while(ticks--)
		__asm__ volatile("NOP"); // assembly instruction that does nothing.
}

void Delay_voidBlockingDelayTicks(u64 ticks)
{
	while(ticks--)
		__asm__ volatile("NOP"); // assembly instruction that does nothing.
}

void Delay_voidDelay2Us(void)
{
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
	__asm__ volatile("NOP");
}
