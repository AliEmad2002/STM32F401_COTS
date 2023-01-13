/*
 * NVIC_program.c
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Target_config.h"

/*	MCAL	*/
#include "SCB_interface.h"

/*	SELF	*/
#include "NVIC_private.h"
#include "NVIC_interface.h"


void NVIC_voidEnableInterrupt(NVIC_Interrupt_t interrupt)
{
	NVIC->ISER[interrupt/32] = (1 << interrupt%32);
}

void NVIC_voidDisableInterrupt(NVIC_Interrupt_t interrupt)
{
	NVIC->ICER[interrupt/32] = (1 << interrupt%32);
}

void NVIC_voidSetPendingFlag(NVIC_Interrupt_t interrupt)
{
	NVIC->ISPR[interrupt/32] = (1 << interrupt%32);
}

void NVIC_voidClearPendingFlag(NVIC_Interrupt_t interrupt)
{
	NVIC->ICPR[interrupt/32] = (1 << interrupt%32);
}

b8 NVIC_b8GetActiveFlag(NVIC_Interrupt_t interrupt)
{
	return GET_BIT(NVIC->IABR[interrupt/32], interrupt%32);
}

void NVIC_voidSetInterruptPriority(NVIC_Interrupt_t interrupt, u8 group, u8 sub)
{
	// read current priority grouping settings:
	u8 priGroupSettings = SCB_enumGetPriorityGroupsAndSubGroupsNumber();

	// calculate binary point:
	u8 binPoint;

	switch(priGroupSettings)
	{
	case SCB_PRIGROUP_group16_sub0:
		binPoint = group;
		break;

	case SCB_PRIGROUP_group8_sub2:
		binPoint = sub | (group << 1);
		break;

	case SCB_PRIGROUP_group4_sub4:
		binPoint = sub | (group << 2);
		break;

	case SCB_PRIGROUP_group2_sub8:
		binPoint = sub | (group << 3);
		break;

	default:	//case SCB_PRIGROUP_group0_sub16:
		binPoint = sub;
		break;
	}

	switch (interrupt)
	{
	case NVIC_Interrupt_MemManage:
		SCB_voidSetCorePeripheralPriority(4, binPoint);
		break;

	case NVIC_Interrupt_BusFault:
		SCB_voidSetCorePeripheralPriority(5, binPoint);
		break;

	case NVIC_Interrupt_UsageFault:
		SCB_voidSetCorePeripheralPriority(6, binPoint);
		break;

	case NVIC_Interrupt_SVCall:
		SCB_voidSetCorePeripheralPriority(11, binPoint);
		break;

	case NVIC_Interrupt_PendSV:
		SCB_voidSetCorePeripheralPriority(14, binPoint);
		break;

	case NVIC_Interrupt_Systick:
		SCB_voidSetCorePeripheralPriority(15, binPoint);
		break;

	default:
		// write it on IPR:
		NVIC->IPR[interrupt] = (binPoint << 4);
	}
}








