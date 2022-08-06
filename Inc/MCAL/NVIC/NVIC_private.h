/*
 * NVIC_private.h
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef NVIC_PRIVATE_H_
#define NVIC_PRIVATE_H_

/*	define pointers to be used as arrays	*/
#define NVIC_ISER	((volatile u32*)0xE000E100)

#define NVIC_ICER	((volatile u32*)0xE000E180)

#define NVIC_ISPR	((volatile u32*)0xE000E200)

#define NVIC_ICPR	((volatile u32*)0xE000E280)

#define NVIC_IABR	((volatile u32*)0xE000E300)

#define NVIC_IPR	((volatile u32*)0xE000E400)

/*	not an array, defined as u32	*/
#define NVIC_STIR	(*(volatile u32*)0xE000EF00)

#endif /* NVIC_PRIVATE_H_ */
