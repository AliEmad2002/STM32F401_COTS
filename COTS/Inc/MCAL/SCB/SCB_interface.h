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

typedef enum{
	MSCB_VTOR_Code,
	MSCB_VTOR_SRAM
}MSCB_VTOR_t;

void MSCB_voidSetPriorityGroupsAndSubGroupsNumber(MSCB_PRIGROUP_t set);

MSCB_PRIGROUP_t MSCB_enumGetPriorityGroupsAndSubGroupsNumber(void);

void MSCB_voidSetCorePeripheralPriority(u8 sysHandlerInex, u8 priVal);

void MSCB_voidResetSystem(void);

/*
 * address alignment is user's responsibility.
 */
void MSCB_voidWriteVectorTableOffset(u32 offset, MSCB_VTOR_t type);

u32 MSCB_u32GetVectorTableOffset(void);

#endif /* SCB_INTERFACE_H_ */
