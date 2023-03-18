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

typedef struct{
	u32 CPUID;
	u32 ICSR;
	u32 VTOR;
	u32 AIRCR;
	u32 SCR;
	u32 CCR;
	u8 SHPR[12];
	u32 SHCRS;
	u32 CFSR;
	u32 HFSR;
	u32 MMAR;
	u32 BFAR;
}SCB_t;

#define SCB_BASE_ADD	((u32)0xE000ED00)

static SCB_t* const scb = (SCB_t* const)SCB_BASE_ADD;

#define SCB_VECTKEY	0x5FA

/*	AIRCR bits	*/
#define SCB_AIRCR_SYSRESETREQ	2

#endif /* SCB_PRIVATE_H_ */
