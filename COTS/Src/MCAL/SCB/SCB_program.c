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


void SCB_voidSetPriorityGroupsAndSubGroupsNumber(SCB_PRIGROUP_t set)
{
	EDT_REG(SCB_AIRCR, 8, set, 3);
}

// returns the value of PRIGROUP bits in SCB_AIRCR
SCB_PRIGROUP_t SCB_enumGetPriorityGroupsAndSubGroupsNumber(void)
{
	return SCB_AIRCR & 0x700;
}

void SCB_voidSetCorePeripheralPriority(u8 sysHandlerInex, u8 priVal)
{
	SCB_SHPR[sysHandlerInex - 4] = (priVal << 4);
}

void SCB_voidResetSystem(void)
{
	SCB_AIRCR |= ((SCB_VECTKEY << 16) | (1 << SCB_AIRCR_SYSRESETREQ));
}

/*
 * address alignment is user's responsibility.
 */
void SCB_voidWriteVectorTableOffset(u32 offset, SCB_VTOR_t type)
{
	SCB_VTOR = (type << 29) | offset;
}

u32 SCB_u32GetVectorTableOffset(void)
{
	return SCB_VTOR;
}
