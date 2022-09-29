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

void BitMath_voidEditRegister(u64* regPtr, u8 start, u64 val, u8 len)
{
	/*
	 * reg = reg & (~((2^(len)-1) << start) | (val << start)
	 */
	*regPtr = (*regPtr & ~((POW_TWO(len)-1) << start)) | (val << start);
	/*for (u8 n=0; n<len; n++)
	{
		WRT_BIT(*regPtr, start+n, GET_BIT(val, n));
	}*/
}
