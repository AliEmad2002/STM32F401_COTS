/*
 * SCB_interface.h
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef SCB_INTERFACE_H_
#define SCB_INTERFACE_H_

typedef enum{
	SCB_PRIGROUP_group16_sub0	=	3,
	SCB_PRIGROUP_group8_sub2	=	4,
	SCB_PRIGROUP_group4_sub4 	=	5,
	SCB_PRIGROUP_group2_sub8 	=	6,
	SCB_PRIGROUP_group0_sub16 	=	7,
}SCB_PRIGROUP_t;

typedef enum{
	SCB_VTOR_Code,
	SCB_VTOR_SRAM
}SCB_VTOR_t;

void SCB_voidSetPriorityGroupsAndSubGroupsNumber(SCB_PRIGROUP_t set);

SCB_PRIGROUP_t SCB_enumGetPriorityGroupsAndSubGroupsNumber(void);

void SCB_voidSetCorePeripheralPriority(u8 sysHandlerInex, u8 priVal);

void SCB_voidResetSystem(void);

/*
 * address alignment is user's responsibility.
 */
void SCB_voidWriteVectorTableOffset(u32 offset, SCB_VTOR_t type);

u32 SCB_u32GetVectorTableOffset(void);

#include "SCB_private.h"

#define SCB_SET_PENDSV	(SET_BIT(scb->ICSR, SCB_ICSR_PENDSVSET))

#define SCB_CLR_PENDSV	(SET_BIT(scb->ICSR, SCB_ICSR_PENDSVCLR))

#define SCB_SET_PENDSTK	(SET_BIT(scb->ICSR, SCB_ICSR_PENDSTSET))

#define SCB_CLR_PENDSTK	(SET_BIT(scb->ICSR, SCB_ICSR_PENDSTCLR))

#endif /* SCB_INTERFACE_H_ */
