/*
 * TEMPDELAY.c
 *
 *  Created on:	Jul 28, 2022
 *  Author:		Ali Emad Ali
 *
 */

#include "LIB/STD_TYPES.h"

#include "TEMPDELAY.h"


void tempDelay(u32 ms)
{
	register u32 ticks = (16000000/7000)*ms;
	// 7000 is an experimental number, do better !
	while(ticks--)
		__asm__ volatile("NOP"); // assembly instruction that does nothing.
}
