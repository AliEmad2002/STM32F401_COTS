/*
 * ADC_private.h
 *
 *  Created on: Nov 25, 2022
 *      Author: Ali Emad Ali
 */

#ifndef ADC_PRIVATE_H_
#define ADC_PRIVATE_H_


typedef struct{
	u32 SR;
	u32 CR1;
	u32 CR2;
	u32 SMPR1;
	u32 SMPR2;
	u32 JOFR[4];
	u32 HTR;
	u32 LTR;
	u32 SQR1;
	u32 SQR2;
	u32 SQR3;
	u32 JSQR;
	u32 JDR[4];
	u32 DR;
}ADC_t;

#define ADC1	((volatile ADC_t*)0x40012400)
#define ADC2	((volatile ADC_t*)0x40012800)

static volatile ADC_t* const ADC[] = {ADC1, ADC2};

/*	CR1 registers	*/
#define CR1_AWDCH_0				0
#define CR1_EOCIE				5
#define CR1_AWDIE				6
#define CR1_JEOCIE				7
#define CR1_SCAN				8
#define CR1_AWDSGL				9
#define CR1_JAUTO				10
#define CR1_DISCEN				11
#define CR1_JDISCEN				12
#define CR1_DISCNUM_0			13
#define CR1_DUALMOD_0			16
#define CR1_JAWDEN				22
#define CR1_AWDEN				23

/*	CR2 registers	*/
#define CR2_ADON				0
#define CR2_CONT				1
#define CR2_CAL					2
#define CR2_RSTCAL				3
#define CR2_DMA					8
#define CR2_ALIGN				11
#define CR2_JEXTSEL_0			12
#define CR2_JEXTTRIG			15
#define CR2_EXTSEL_0			17
#define CR2_EXTTRIG				20
#define CR2_JSWSTART			21
#define CR2_SWSTART				22
#define CR2_TSVREFE				23

/*	SQR1 register	*/
#define SQR1_L_0				20

/*	JSQR register	*/
#define JSQR_JL_0				20


#endif /* ADC_PRIVATE_H_ */
