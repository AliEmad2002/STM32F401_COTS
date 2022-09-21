/*
 * BIT_MATH.h
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
#define EDT_REG(reg, start, val, len)			\
	for (u8 n=0; n<(len); n++){					\
		WRT_BIT((reg), ((start)+(n)), GET_BIT((val), (n)));}							\


#endif /* BIT_MATH_H_ */
