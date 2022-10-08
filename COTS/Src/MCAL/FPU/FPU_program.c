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
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Target_config.h"

#if TARGET_ID == STM32F401x
#include "FPU_private.h"
#include "FPU_interface.h"

void FPU_voidEnable(void)
{
	FPU_SPACR |= (0b1111 << 20);
}
#endif
