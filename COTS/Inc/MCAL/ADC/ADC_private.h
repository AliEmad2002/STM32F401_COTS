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
	u32 JOFR1;
	u32 JOFR2;
	u32 JOFR3;
	u32 JOFR4;
	u32 HTR;
	u32 LTR;
	u32 SQR1;
	u32 SQR2;
	u32 SQR3;
	u32 JSQR;
	u32 JDR1;
	u32 JDR2;
	u32 JDR3;
	u32 JDR4;
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


#endif /* ADC_PRIVATE_H_ */
