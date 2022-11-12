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


void MSCB_voidSetPriorityGroupsAndSubGroupsNumber(MSCB_PRIGROUP_t set)
{
	EDT_REG(SCB_AIRCR, 8, set, 3);
}

// returns the value of PRIGROUP bits in SCB_AIRCR
MSCB_PRIGROUP_t MSCB_enumGetPriorityGroupsAndSubGroupsNumber(void)
{
	return SCB_AIRCR & 0x700;
}

void MSCB_voidSetCorePeripheralPriority(u8 sysHandlerInex, u8 priVal)
{
	SCB_SHPR[sysHandlerInex - 4] = (priVal << 4);
}

void MSCB_voidResetSystem(void)
{
	SCB_AIRCR |= ((SCB_VECTKEY << 16) | (1 << SCB_AIRCR_SYSRESETREQ));
}

/*
 * address alignment is user's responsibility.
 */
void MSCB_voidWriteVectorTableOffset(u32 offset, MSCB_VTOR_t type)
{
	SCB_VTOR = (type << 29) | offset;
}

u32 MSCB_u32GetVectorTableOffset(void)
{
	return SCB_VTOR;
}
