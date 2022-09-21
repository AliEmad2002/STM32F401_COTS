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



#define RCC_BASE_ADD				((u32)(0x40023800))

#define RCC_CR						(*(volatile u32*)(RCC_BASE_ADD + 0x00))

#define RCC_PLLCFGR					(*(volatile u32*)(RCC_BASE_ADD + 0x04))

#define RCC_CFGR					(*(volatile u32*)(RCC_BASE_ADD + 0x08))

#define RCC_CIR						(*(volatile u32*)(RCC_BASE_ADD + 0x0C))

#define RCC_AHB1RSTR				(*(volatile u32*)(RCC_BASE_ADD + 0x10))

#define RCC_AHB2RSTR				(*(volatile u32*)(RCC_BASE_ADD + 0x14))


#define RCC_APB1RSTR				(*(volatile u32*)(RCC_BASE_ADD + 0x20))

#define RCC_APB2RSTR				(*(volatile u32*)(RCC_BASE_ADD + 0x24))


#define RCC_AHB1ENR					(*(volatile u32*)(RCC_BASE_ADD + 0x30))

#define RCC_AHB2ENR					(*(volatile u32*)(RCC_BASE_ADD + 0x34))


#define RCC_APB1ENR					(*(volatile u32*)(RCC_BASE_ADD + 0x40))

#define RCC_APB2ENR					(*(volatile u32*)(RCC_BASE_ADD + 0x44))


#define RCC_AHB1LPENR				(*(volatile u32*)(RCC_BASE_ADD + 0x50))

#define RCC_AHB2LPENR				(*(volatile u32*)(RCC_BASE_ADD + 0x54))


#define RCC_APB1LPENR				(*(volatile u32*)(RCC_BASE_ADD + 0x60))

#define RCC_APB2LPENR				(*(volatile u32*)(RCC_BASE_ADD + 0x64))


#define RCC_BDCR					(*(volatile u32*)(RCC_BASE_ADD + 0x70))

#define RCC_CSR						(*(volatile u32*)(RCC_BASE_ADD + 0x74))


#define RCC_SSCGR					(*(volatile u32*)(RCC_BASE_ADD + 0x80))

#define RCC_PLLI2SCFGR				(*(volatile u32*)(RCC_BASE_ADD + 0x84))


#define RCC_DCKCFGR					(*(volatile u32*)(RCC_BASE_ADD + 0x8C))



/*	Bits	*/

/*	RCC_CR bits	*/
#define RCC_HSION								0
#define RCC_HSIRDY								1
#define RCC_HSITRIM0							3
#define RCC_HSICAL0								8
#define RCC_HSEON								16
#define RCC_HSERDY								17
#define RCC_HSEBYP								18
#define RCC_CSSON								19
#define RCC_PLLON								24
#define RCC_PLLRDY								25
#define RCC_PLLI2SON							26
#define RCC_PLLI2SRDY							27
/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

/*	RCC_PLLCFGR bits	*/
#define RCC_PLLM0								0
#define RCC_PLLN0								6
#define RCC_PLLP0								16
#define RCC_PLLP1								17
#define RCC_PLLSRC								22
#define RCC_PLLQ0								24
/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

/*	RCC_CFGR bits	*/
#define RCC_SW0								0
#define RCC_SW1								1
#define RCC_SWS0							2
#define RCC_SWS1							3
#define RCC_HPRE0							4
#define RCC_PPRE10							10
#define RCC_PPRE20							13
#define RCC_RTCPRE0							16
#define RCC_MCO10							21
#define RCC_I2SSCR							23
#define RCC_MCO1PRE0						24
#define RCC_MCO2PRE0						27
#define RCC_MCO20							30


#endif
