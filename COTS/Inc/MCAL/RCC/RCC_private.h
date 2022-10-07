/*
 * RCC_program.c
 *
 *  Created on:	Jul 26, 2022
 *  Author:		Ali Emad Ali
 *
 *  References:
 *  	STM RM0368:	2.3 		Memory map
 *  	STM RM0368:	6.3.22 		RCC register map
 */


#ifndef		RCC_PRIVATE_H
#define		RCC_PRIVATE_H

#if TARGET_ID == STM32F401x
typedef struct{
	u32 CR;				//	00
	u32 PLLCFGR;		//	04
	u32 CFGR;			//	08
	u32 CIR;			//	0C
	u32 AHB1RSTR;		//	10
	u32 AHB2RSTR;		//	14
	u32 reserved0;		//	18
	u32 reserved1;		//	1C
	u32 APB1RSTR;		//	20
	u32 APB2RSTR;		//	24
	u32 reserved2;		//	28
	u32 reserved3;		//	2C
	u32 AHB1ENR;		//	30
	u32 AHB2ENR;		//	34
	u32 reserved4;		//	38
	u32 reserved5;		//	3C
	u32 APB1ENR;		//	40
	u32 APB2ENR;		//	44
	u32 reserved6;		//	48
	u32 reserved7;		//	4C
	u32 AHB1LPENR;		//	50
	u32 AHB2LPENR;		//	54
	u32 reserved8;		//	58
	u32 reserved9;		//	5C
	u32 APB1LPENR;		//	60
	u32 APB2LPENR;		//	64
	u32 reserved10;		//	68
	u32 reserved11;		//	6C
	u32 BDCR;			//	70
	u32 CSR;			//	74
	u32 reserved12;		//	78
	u32 reserved13;		//	7C
	u32 SSCGR;			//	80
	u32 PLLI2SCFGR;		//	84
	u32 reserved14;		//	88
	u32 DCKCFGR;		//	8C
}RCC_t;

#define RCC		((volatile RCC_t*)0x40023800)

/*	RCC_CR bits	*/
#define RCC_HSION								0
#define RCC_HSIRDY								1
#define RCC_HSITRIM_0							3
#define RCC_HSICAL_0							8
#define RCC_HSEON								16
#define RCC_HSERDY								17
#define RCC_HSEBYP								18
#define RCC_CSSON								19
#define RCC_PLLON								24
#define RCC_PLLRDY								25
#define RCC_PLLI2SON							26
#define RCC_PLLI2SRDY							27

/*	RCC_PLLCFGR bits	*/
#define RCC_PLLM_0								0
#define RCC_PLLN_0								6
#define RCC_PLLP_0								16
#define RCC_PLLP_1								17
#define RCC_PLLSRC								22
#define RCC_PLLQ_0								24

/*	RCC_CFGR bits	*/
#define RCC_SW_0							0
#define RCC_SW_1							1
#define RCC_SWS_0							2
#define RCC_SWS_1							3
#define RCC_HPRE_0							4
#define RCC_PPRE1_0							10
#define RCC_PPRE2_0							13
#define RCC_RTCPRE_0						16
#define RCC_MCO1_0							21
#define RCC_I2SSCR							23
#define RCC_MCO1PRE_0						24
#define RCC_MCO2PRE_0						27
#define RCC_MCO2_0							30

/*	RCC_CIR bits	*/
#define RCC_LSIRDYF							0
#define RCC_LSERDYF							1
#define RCC_HSIRDYF							2
#define RCC_HSERDYF							3
#define RCC_PLLRDYF							4
#define RCC_PLLI2SRDYF						5
#define RCC_CSSF							7
#define RCC_LSIRDYIE						8
#define RCC_LSERDYIE						9
#define RCC_HSIRDYIE						10
#define RCC_HSERDYIE						11
#define RCC_PLLRDYIE						12
#define RCC_PLLI2SRDYIE						13
#define RCC_LSIRDYC							16
#define RCC_LSERDYC							17
#define RCC_HSIRDYC							18
#define RCC_HSERDYC							19
#define RCC_PLLRDYC							20
#define RCC_PLLI2SRDYC						21
#define RCC_CSSC							23

/*	cont. later...	*/
#elif TARGET_ID == STM32F10x
typedef struct{
	u32 CR;				//	00
	u32 CFGR;			//	04
	u32 CIR;			//	08
	u32 APB2RSTR;		//	0C
	u32 APB1RSTR;		//	10
	u32 AHBENR;			//	14
	u32 APB2ENR;		//	18
	u32 APB1ENR;		//	1C
	u32 BDCR;			//	20
	u32 CSR;			//	24
	u32 AHBSTR;			//	28
	u32 CFGR2;			//	2C
}RCC_t;

#define RCC		((volatile RCC_t*)0x40021000)

/*	RCC_CR bits	*/
#define RCC_HSION								0
#define RCC_HSIRDY								1
#define RCC_HSITRIM_0							3
#define RCC_HSICAL_0							8
#define RCC_HSEON								16
#define RCC_HSERDY								17
#define RCC_HSEBYP								18
#define RCC_CSSON								19
#define RCC_PLLON								24
#define RCC_PLLRDY								25
#define RCC_PLL2ON								26
#define RCC_PLL2RDY								27
#define RCC_PLL3ON								28
#define RCC_PLL3RDY								29

/*	RCC_CFGR bits	*/
#define RCC_SW_0							0
#define RCC_SW_1							1
#define RCC_SWS_0							2
#define RCC_SWS_1							3
#define RCC_HPRE_0							4
#define RCC_PPRE1_0							8
#define RCC_PPRE2_0							11
#define RCC_ADCPRE_0						14
#define RCC_PLLSRC							16
#define RCC_PLLXTPRE						17
#define RCC_PLLMUL_0						18
#define RCC_OTGFSPRE						22
#define RCC_MCO_0							24

/*	RCC_CIR bits	*/
#define RCC_LSIRDYF							0
#define RCC_LSERDYF							1
#define RCC_HSIRDYF							2
#define RCC_HSERDYF							3
#define RCC_PLLRDYF							4
#define RCC_PLL2RDYF						5
#define RCC_PLL3RDYF						6
#define RCC_CSSF							7
#define RCC_LSIRDYIE						8
#define RCC_LSERDYIE						9
#define RCC_HSIRDYIE						10
#define RCC_HSERDYIE						11
#define RCC_PLLRDYIE						12
#define RCC_PLL2RDYIE						13
#define RCC_PLL3RDYIE						14
#define RCC_LSIRDYC							16
#define RCC_LSERDYC							17
#define RCC_HSIRDYC							18
#define RCC_HSERDYC							19
#define RCC_PLLRDYC							20
#define RCC_PLL2RDYC						21
#define RCC_PLL3RDYC						21
#define RCC_CSSC							23

/*	RCC_CFGR2	bits	*/
#define RCC_PREDIV1_0						0
#define RCC_PREDIV2_0						4
#define RCC_PLL2MUL_0						8
#define RCC_PLL3MUL_0						12
#define RCC_PREDIV1SRC						16
#define RCC_I2S2SRC							17
#define RCC_I2S3SRC							18
/*	cont. later...	*/
#endif

#endif
