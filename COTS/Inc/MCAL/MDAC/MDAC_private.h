/*
 * MDAC_private.h
 *
 *  Created on: Oct 13, 2022
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MCAL_MDAC_MDAC_PRIVATE_H_
#define INCLUDE_MCAL_MDAC_MDAC_PRIVATE_H_


typedef struct{
	volatile u32 CR			;
	volatile u32 SWTRGIGR    ;
	volatile u32 DHR12R1     ;
	volatile u32 DHR12L1     ;
	volatile u32 DHR8R1      ;
	volatile u32 DHR12R2     ;
	volatile u32 DHR12L2     ;
	volatile u32 DHR8R2      ;
	volatile u32 DHR12RD     ;
	volatile u32 DHR12LD     ;
	volatile u32 DHR8RD      ;
	volatile u32 DOR1        ;
	volatile u32 DOR2        ;
}MDAC_t;


#define MDAC	((volatile MDAC_t*)0x40007400)


#endif /* INCLUDE_MCAL_MDAC_MDAC_PRIVATE_H_ */
