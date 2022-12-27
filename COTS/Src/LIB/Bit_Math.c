/*
 * Bit_Math.c
 *
 *  Created on:	Sep 24, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#include "Std_Types.h"
#include "Bit_Math.h"

inline void BitMath_voidEditRegister(u32* regPtr, u8 start, u32 val, u8 len)
{
	*regPtr = (*regPtr & ~((POW_TWO(len)-1) << start)) | (val << start);
}
