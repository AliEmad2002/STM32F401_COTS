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

#define STK_BASE_ADD 		((u32)0xE000E010)

#define STK_CTRL			(*(volatile u32*)(STK_BASE_ADD + 0x00))

#define STK_LOAD			(*(volatile u32*)(STK_BASE_ADD + 0x04))

#define STK_VAL				(*(volatile u32*)(STK_BASE_ADD + 0x08))

#define STK_CALIB			(*(volatile u32*)(STK_BASE_ADD + 0x0C))


#define STK_ENABLE_BIT		0
#define STK_TICKINT_BIT		1
#define STK_CLKSOURCE_BIT	2
#define STK_COUNTFLAG_BIT	16

#endif /* STK_PRIVATE_H_ */
