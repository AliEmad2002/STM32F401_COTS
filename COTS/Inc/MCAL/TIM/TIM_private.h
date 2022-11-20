/*
 * TIM_private.h
 *
 *  Created on: Nov 7, 2022
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MCAL_TIM_TIM_PRIVATE_H_
#define INCLUDE_MCAL_TIM_TIM_PRIVATE_H_


/*
 * note that all timers share the same memory map, despite not all of them has
 * all of the registers mentioned below, but any way must take care when
 * writing program.c
 */
typedef struct{
	volatile u32 CR1;
	volatile u32 CR2;
	volatile u32 SMCR;
	volatile u32 DIER;
	volatile u32 SR;
	volatile u32 EGR;
	volatile u32 CCMR[2];
	volatile u32 CCER;
	volatile u32 CNT;
	volatile u32 PSC;
	volatile u32 ARR;
	volatile u32 RCR;
	volatile u32 CCR1;
	volatile u32 CCR2;
	volatile u32 CCR3;
	volatile u32 CCR4;
	volatile u32 BDTR;
	volatile u32 DCR;
	volatile u32 DMAR;
}TIM_t;

#define TIM1	((volatile TIM_t*)0x40012C00)
#define TIM2	((volatile TIM_t*)0x40000000)
#define TIM3	((volatile TIM_t*)0x40000400)
#define TIM4	((volatile TIM_t*)0x40000800)
#define TIM5	((volatile TIM_t*)0x40000C00)
#define TIM6	((volatile TIM_t*)0x40001000)
#define TIM7	((volatile TIM_t*)0x40001400)
#define TIM8	((volatile TIM_t*)0x40013400)
#define TIM9	((volatile TIM_t*)0x40014C00)
#define TIM10	((volatile TIM_t*)0x40015000)
#define TIM11	((volatile TIM_t*)0x40015400)
#define TIM12	((volatile TIM_t*)0x40001800)
#define TIM13	((volatile TIM_t*)0x40001C00)
#define TIM14	((volatile TIM_t*)0x40002000)


static volatile TIM_t* const TIM[] = {
	NULL, TIM1, TIM2, TIM3, TIM4, TIM5, TIM6,
	TIM7, TIM8, TIM9, TIM10, TIM11, TIM12, TIM13,
	TIM14};

/*	CR1 bits	*/
#define TIM_CR1_CEN			0
#define TIM_CR1_UDIS		1
#define TIM_CR1_URS			2
#define TIM_CR1_OPM			3
#define TIM_CR1_DIR			4
#define TIM_CR1_CMS_0		5
#define TIM_CR1_ARPE		7
#define TIM_CR1_CKD_0		8

/*	CR2 bits	*/
#define TIM_CR2_MMS_0		4

/*	SMCR bits	*/
#define TIM_SMCR_SMS_0		0
#define TIM_SMCR_TS_0		4
#define TIM_SMCR_ETF_0		8
#define TIM_SMCR_ETPS_0		12
#define TIM_SMCR_ECE		14
#define TIM_SMCR_ETP		15

/*	CCMR bits	*/
#define TIM_CCMR_CCxS_0		 0
#define TIM_CCMR_OCxFE		 2
#define TIM_CCMR_OCxPE		 3
#define TIM_CCMR_OCxM_0		 4
#define TIM_CCMR_OCxCE		 7

#define TIM_CCMR_ICxPSC_0	2
#define TIM_CCMR_ICxF_0		4

/*	CCER bits	*/
#define TIM_CCER_CC1E		0
#define TIM_CCER_CC1P		1
#define TIM_CCER_CC1NE		2
#define TIM_CCER_CC1NP		3

/*	BDTR bits	*/
#define TIM_BDTR_LOCK_0		8
#define TIM_BDTR_MOE		15





#endif /* INCLUDE_MCAL_TIM_TIM_PRIVATE_H_ */
