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
#include "NVIC_interface.h"
#include "NVIC_private.h"


void NVIC_voidEnableInterrupt(NVIC_Interrupt_t interrupt)
{
	NVIC->ISER[interrupt/32] = (1 << interrupt%32);
}

void NVIC_voidDisbleInterrupt(NVIC_Interrupt_t interrupt)
{
	NVIC->ICER[interrupt/32] = (1 << interrupt%32);
}

void MNVIC_voidSetPendingFlag(NVIC_Interrupt_t interrupt)
{
	NVIC->ISPR[interrupt/32] = (1 << interrupt%32);
}

void MNVIC_voidClearPendingFlag(NVIC_Interrupt_t interrupt)
{
	NVIC->ICPR[interrupt/32] = (1 << interrupt%32);
}

b8 MNVIC_b8GetActiveFlag(NVIC_Interrupt_t interrupt)
{
	return GET_BIT(NVIC->IABR[interrupt/32], interrupt%32);
}

void MNVIC_voidSetInterruptPriority(NVIC_Interrupt_t interrupt, u8 group, u8 sub)
{
	// read current priority grouping settings:
	u8 priGroupSettings = MSCB_enumGetPriorityGroupsAndSubGroupsNumber();

	// calculate binary point:
	u8 binPoint;

	switch(priGroupSettings)
	{
	case MSCB_PRIGROUP_group16_sub0:
		binPoint = group;
		break;

	case MSCB_PRIGROUP_group8_sub2:
		binPoint = sub | (group << 1);
		break;

	case MSCB_PRIGROUP_group4_sub4:
		binPoint = sub | (group << 2);
		break;

	case MSCB_PRIGROUP_group2_sub8:
		binPoint = sub | (group << 3);
		break;

	default:	//case MSCB_PRIGROUP_group0_sub16:
		binPoint = sub;
		break;
	}

	switch (interrupt)
	{
	case NVIC_Interrupt_MemManage:
		MSCB_voidSetCorePeripheralPriority(4, binPoint);
		break;

	case NVIC_Interrupt_BusFault:
		MSCB_voidSetCorePeripheralPriority(5, binPoint);
		break;

	case NVIC_Interrupt_UsageFault:
		MSCB_voidSetCorePeripheralPriority(6, binPoint);
		break;

	case NVIC_Interrupt_SVCall:
		MSCB_voidSetCorePeripheralPriority(11, binPoint);
		break;

	case NVIC_Interrupt_PendSV:
		MSCB_voidSetCorePeripheralPriority(14, binPoint);
		break;

	case NVIC_Interrupt_Systick:
		MSCB_voidSetCorePeripheralPriority(15, binPoint);
		break;

	default:
		// write it on IPR:
		NVIC->IPR[interrupt] = (binPoint << 4);
	}
}










