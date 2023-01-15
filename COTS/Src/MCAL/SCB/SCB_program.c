/*
 * SCB_program.c
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */


#include "Std_Types.h"
#include "Bit_Math.h"
#include "SCB_private.h"
#include "SCB_interface.h"

/*
 * as mentioned in datasheet, "AIRCR" writes are ignored unless "VECTKEY" is
 * written in the most significant 16-bits at every write operation, this is a
 * macro that does this.
 */
#define WRITE_AIRCR(start, val, len)						\
	(scb->AIRCR =						   					\
		(scb->AIRCR & ~((POW_TWO((len))-1) << (start))) | 	\
		((val) << (start)) | (SCB_VECTKEY << 16))

void SCB_voidSetPriorityGroupsAndSubGroupsNumber(SCB_PRIGROUP_t set)
{
	WRITE_AIRCR(8, set, 3);
}

// returns the value of PRIGROUP bits in SCB_AIRCR
SCB_PRIGROUP_t SCB_enumGetPriorityGroupsAndSubGroupsNumber(void)
{
	return (SCB_PRIGROUP_t)GET_REG_SEC(scb->AIRCR, 8, 3);
}

void SCB_voidSetCorePeripheralPriority(u8 sysHandlerInex, u8 priVal)
{
	scb->SHPR[sysHandlerInex - 4] = (priVal << 4);
}

void SCB_voidResetSystem(void)
{
	WRITE_AIRCR(SCB_AIRCR_SYSRESETREQ, 1, 1);
}

/*
 * address alignment is user's responsibility.
 */
void SCB_voidWriteVectorTableOffset(u32 offset, SCB_VTOR_t type)
{
	scb->VTOR = (type << 29) | offset;
}

u32 SCB_u32GetVectorTableOffset(void)
{
	return scb->VTOR;
}
