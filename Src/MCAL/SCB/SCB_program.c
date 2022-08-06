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


#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "SCB_private.h"
#include "SCB_interface.h"


void MSCB_voidSetPriorityGroupsAndSubGroupsNumber(MSCB_PRIGROUP_t set)
{
	EDT_REG(SCB_AIRCR, 8, set, 3);
}
