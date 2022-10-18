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
	MSCB_PRIGROUP_group16_sub0	=	3,
	MSCB_PRIGROUP_group8_sub2	=	4,
	MSCB_PRIGROUP_group4_sub4 	=	5,
	MSCB_PRIGROUP_group2_sub8 	=	6,
	MSCB_PRIGROUP_group0_sub16 	=	7,
}MSCB_PRIGROUP_t;

void MSCB_voidSetPriorityGroupsAndSubGroupsNumber(MSCB_PRIGROUP_t set);

MSCB_PRIGROUP_t MSCB_enumGetPriorityGroupsAndSubGroupsNumber(void);

void MSCB_voidSetCorePeripheralPriority(u8 sysHandlerInex, u8 priVal);

#endif /* SCB_INTERFACE_H_ */
