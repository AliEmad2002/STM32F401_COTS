/*
 * SCB_private.h
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef SCB_PRIVATE_H_
#define SCB_PRIVATE_H_

#define SCB_BASE_ADD	((u32)0xE000ED00)

#define SCB_AIRCR		(*(volatile u32*)(SCB_BASE_ADD + 0x0C))

#endif /* SCB_PRIVATE_H_ */
