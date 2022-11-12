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

#define SCB_VTOR		(*(volatile u32*)(SCB_BASE_ADD + 0x08))

#define SCB_AIRCR		(*(volatile u32*)(SCB_BASE_ADD + 0x0C))

#define SCB_SHPR		((volatile u8*)(SCB_BASE_ADD + 0x18))

#define SCB_VECTKEY	0x5FA

/*	AIRCR bits	*/
#define SCB_AIRCR_SYSRESETREQ	2

#endif /* SCB_PRIVATE_H_ */
