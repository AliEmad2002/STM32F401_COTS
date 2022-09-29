/*
 * Bit_Math.h
 *
 *  Created on:	Jul 26, 2022
 *  Author:		Ali Emad
 */

#ifndef BIT_MATH_H_
#define BIT_MATH_H_


#define SET_BIT(reg, n)					((reg) |= 1<<(n))

#define CLR_BIT(reg, n)					((reg) &= ~(1<<(n)))

#define TGL_BIT(reg, n)					((reg) ^= 1<<(n))

#define GET_BIT(reg, n)					(((reg)>>(n)) & 1)

#define WRT_BIT(reg, n, val)			((val) ? SET_BIT((reg), (n)) : CLR_BIT((reg), (n)))

/*	2^n	*/
#define POW_TWO(n)						(1 << (n))

#define GET_REG_SEC(reg, start, len)	(((reg) >> (start)) & (POW_TWO((len)) - 1))

/*
 * Edit a block of bytes in a register to a new value
 * reg:		the register to be edited.
 * start:	number of the bit to start editing upwards from.
 * val:		value that is going to replace whatever is in "reg" starting from "start" bit.
 * len:		length of val.
 */
void BitMath_voidEditRegister(u64* regPtr, u8 start, u64 val, u8 len);

#define EDT_REG(reg, start, val, len)		BitMath_voidEditRegister((u64*)&reg, start, val, len)


#endif /* BIT_MATH_H_ */
