/*
 * FPU_program.c
 *
 *  Created on:	Sep 18, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */


/*	LIB	*/
#include "BIT_MATH.h"
#include "STD_TYPES.h"

/*	SELF	*/
#include "FPU_private.h"
#include "FPU_interface.h"

void FPU_voidEnable(void)
{
	FPU_SPACR |= (0b1111 << 20);
}
