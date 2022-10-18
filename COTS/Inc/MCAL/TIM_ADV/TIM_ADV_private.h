/*
 * TIM_ADV_private.h
 *
 *  Created on: Oct 10, 2022
 *      Author: Ali Emad Ali
 */

#ifndef _TIM_ADV_TIM_ADV_PRIVATE_H_
#define _TIM_ADV_TIM_ADV_PRIVATE_H_



typedef struct{
	u32 CR1;
	u32 CR2;
	u32 SMCR;
	u32 DIER;
	u32 SR;
	u32 EGR;
	/*	notice that CCMR[2, 3] are reserved	*/
	u8	CCMR[6];
	u16 reserved;
	u32 CCER;
	u32 CNT;
	u32 PSC;
	u32 ARR;
	u32 RCR;
	u32 CCR1;
	u32 CCR2;
	u32 CCR3;
	u32 CCR4;
	u32 BDTR;
	u32 DCR;
	u32 DMAR;
}TIM_ADV_t;

/*	CR1 bits	*/
#define TIM_ADV_CR1_CEN			0
#define TIM_ADV_CR1_UDIS		1
#define TIM_ADV_CR1_URS			2
#define TIM_ADV_CR1_OPM			3
#define TIM_ADV_CR1_DIR			4
#define TIM_ADV_CR1_CMS_0		5
#define TIM_ADV_CR1_ARPE		7
#define TIM_ADV_CR1_CKD_0		8

/*	SMCR bits	*/
#define TIM_ADV_SMCR_SMS_0		0
#define TIM_ADV_SMCR_TS_0		4
#define TIM_ADV_SMCR_ETF_0		8
#define TIM_ADV_SMCR_ETPS_0		12
#define TIM_ADV_SMCR_ECE		14
#define TIM_ADV_SMCR_ETP		15

/*	CCMR bits	*/
#define TIM_ADV_CCMR_CCx_0		 0
#define TIM_ADV_CCMR_OCxFE		 2
#define TIM_ADV_CCMR_OCxPE		 3
#define TIM_ADV_CCMR_OCxM_0		 4
#define TIM_ADV_CCMR_OCxCE		 7

#define TIM_ADV_CCMR_ICxPSC_0	2
#define TIM_ADV_CCMR_ICxF_0		4

/*	CCER bits	*/
#define TIM_ADV_CCER_CC1E		0
#define TIM_ADV_CCER_CC1P		1
#define TIM_ADV_CCER_CC1NE		2
#define TIM_ADV_CCER_CC1NP		3








#define TIM_ADV1	((volatile TIM_ADV_t*)0x40012C00)
#define TIM_ADV8	((volatile TIM_ADV_t*)0x40013400)


volatile TIM_ADV_t* TIM_ADV[] = {TIM_ADV1, TIM_ADV8};



#endif /* _TIM_ADV_TIM_ADV_PRIVATE_H_ */

