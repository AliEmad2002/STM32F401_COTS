/*
 * STK_private.h
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef STK_PRIVATE_H_
#define STK_PRIVATE_H_

typedef struct{
u32 CTRL;
u32 LOAD;
u32 VAL;
u32 CALIB;
}STK_t;

#define STK		((volatile STK_t*)0xE000E010)

#define STK_ENABLE			0
#define STK_TICKINT			1
#define STK_CLKSOURCE		2
#define STK_COUNTFLAG		16

#endif /* STK_PRIVATE_H_ */
