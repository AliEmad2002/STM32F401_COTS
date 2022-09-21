/*
 * RCC_program.c
 *
 *  Created on:	Jul 26, 2022
 *  Author:		Ali Emad Ali
 *
 *  References:
 *  	STM RM0368:	6.	Reset and clock control (RCC) for STM32F401xB/C and STM32F401xD/E
 */

#include "LIB/BIT_MATH.h"
#include "LIB/STD_TYPES.h"

#include "MCAL/RCC/RCC_interface.h"
#include "MCAL/RCC/RCC_config.h"
#include "MCAL/RCC/RCC_private.h"


void RCC_voidMCO1Init(RCC_MCO1CLOCKSOURCE clkSource, RCC_MCOPRESCALER prescaler)
{
	EDT_REG(RCC_CFGR, RCC_MCO10, clkSource, 2);
	EDT_REG(RCC_CFGR, RCC_MCO1PRE0, prescaler, 3);
}

void RCC_voidMCO2Init(RCC_MCO2CLOCKSOURCE clkSource, RCC_MCOPRESCALER prescaler)
{
	EDT_REG(RCC_CFGR, RCC_MCO20, clkSource, 2);
	EDT_REG(RCC_CFGR, RCC_MCO2PRE0, prescaler, 3);
}

void RCC_voidSysClockInit(void)
{
	/*	Setting HSE bypass option (if enabled)	*/
	#if RCC_HSE_EN
		WRT_BIT(RCC_CR, RCC_HSEBYP, RCC_HSE_BYPASSED);
	#endif	/*	RCC_HSE_EN	*/
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

	/*	configuring PLL (if enabled)	*/
	#if RCC_PLL_EN
		/*	setting PLL clock source	*/
		WRT_BIT(RCC_PLLCFGR, RCC_PLLSRC, RCC_PLL_CLK_SOURCE);
		/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

		/*	Setting PLLM factor	*/
		/*	Check on RCC_PLL_MAIN_DIVISION_FACTOR value limits	*/
		#if 2 > RCC_PLL_MAIN_DIVISION_FACTOR || RCC_PLL_MAIN_DIVISION_FACTOR > 63
			#error worng "RCC_PLL_MAIN_DIVISION_FACTOR" value!
		#endif	/*	2 > RCC_PLL_MAIN_DIVISION_FACTOR || RCC_PLL_MAIN_DIVISION_FACTOR > 63	*/

		/*	check on vcoInputFrequency value limits	*/
		#if RCC_PLL_CLK_SOURCE == RCC_PLLSOURCE_HSE
		#define pllInputFrequency	RCC_HSE_CLK
		#else
		#define pllInputFrequency	RCC_HSI_CLK
		#endif	/*	RCC_PLL_CLK_SOURCE == RCC_PLLSOURCE_HSE	*/
		#define vcoInputFrequency	(pllInputFrequency / RCC_PLL_MAIN_DIVISION_FACTOR)
		#if 1000000 > vcoInputFrequency || vcoInputFrequency > 2000000
			#error worng "RCC_PLL_MAIN_DIVISION_FACTOR" value!
		#endif	/*	1000000 > vcoInputFrequency || vcoInputFrequency > 2000000	*/

		EDT_REG(RCC_PLLCFGR, RCC_PLLM0, RCC_PLL_MAIN_DIVISION_FACTOR, 6);
		/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

		/*	Setting PLLN factor	*/
		/*	Check on PLLN value limits	*/
		#if 2 > RCC_PLL_MULTIPLICATION_FACTOR || RCC_PLL_MULTIPLICATION_FACTOR > 432
			#error worng "RCC_PLL_MULTIPLICATION_FACTOR" value!
		#endif	/*	2 > RCC_PLL_MULTIPLICATION_FACTOR || RCC_PLL_MULTIPLICATION_FACTOR > 432	*/

		/*	Check on vcoOutputFrequency limits	*/
		#define vcoOutputFrequency  (vcoInputFrequency * RCC_PLL_MULTIPLICATION_FACTOR)
		#if 192000000 > vcoOutputFrequency || vcoOutputFrequency > 432000000
			#error worng "RCC_PLL_MULTIPLICATION_FACTOR" value!
		#endif	/*	192000000 > vcoOutputFrequency || vcoOutputFrequency > 432000000	*/

		EDT_REG(RCC_PLLCFGR, RCC_PLLN0, RCC_PLL_MULTIPLICATION_FACTOR, 9);
		/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

		/*	Setting PLLP factor	*/
		/*	Check on vcoOutputFrequency limits	*/
		#define pllOugftputFrequency (vcoOutputFrequency / ((RCC_PLL_SYS_DIVISION_FACTOR+1) * 2))
		#if pllOutputFrequency > 84000000
			#error worng "RCC_PLL_SYS_DIVISION_FACTOR" value!
		#endif	/*	pllOutputFrequency > 84000000	*/

		EDT_REG(RCC_PLLCFGR, RCC_PLLP0, RCC_PLL_SYS_DIVISION_FACTOR, 2);
		/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/
	#endif	/*	RCC_PLL_EN	*/
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

	/*	Enabling selected sources, and clock security system	*/
	#if RCC_HSE_EN
		SET_BIT(RCC_CR, RCC_HSEON);
		while(!GET_BIT(RCC_CR, RCC_HSERDY));
	#endif	/*	RCC_HSE_EN	*/

	#if RCC_HSI_EN
		SET_BIT(RCC_CR, RCC_HSION);
		while(!GET_BIT(RCC_CR, RCC_HSIRDY));
	#endif	/*	RCC_HSI_EN	*/

	#if RCC_PLL_EN
		SET_BIT(RCC_CR, RCC_PLLON);
		while(!GET_BIT(RCC_CR, RCC_PLLRDY));
	#endif	/*	RCC_HSE_EN	*/

	#if RCC_CSS_EN
		SET_BIT(RCC_CR, RCC_CSSON);
	else
		CLR_BIT(RCC_CR, RCC_CSSON);
	#endif	/*	RCC_CSS_EN	*/
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

	/*	Setting system clock switch	*/
	RCC_CFGR = (RCC_CFGR & ~(0b11<<RCC_SW0)) | RCC_SYS_SOURCE;
	while(
		(GET_BIT(RCC_CFGR, RCC_SWS0) | GET_BIT(RCC_CFGR, RCC_SWS1)<<1) !=  RCC_SYS_SOURCE
		);
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/
}

void RCC_voidAHBClockInit(RCC_AHBPRESCALER	prescaler)
{
	EDT_REG(RCC_CFGR, RCC_HPRE0, prescaler, 4);
}

void RCC_voidAPB1ClockInit(RCC_APBPRESCALER	prescaler)
{
	EDT_REG(RCC_CFGR, RCC_PPRE10, prescaler, 3);
}

void RCC_voidAPB2ClockInit(RCC_APBPRESCALER	prescaler)
{
	EDT_REG(RCC_CFGR, RCC_PPRE20, prescaler, 3);
}

void RCC_voidEnablePeripheralClk(RCC_BUS_t bus, u8 peripheral)
{
	switch(bus)
	{
	case RCC_BUS_AHB1:
		SET_BIT(RCC_AHB1ENR, peripheral);
		break;
	case RCC_BUS_AHB2:
		SET_BIT(RCC_AHB2ENR, peripheral);
		break;
	case RCC_BUS_APB1:
		SET_BIT(RCC_APB1ENR, peripheral);
		break;
	case RCC_BUS_APB2:
		SET_BIT(RCC_APB2ENR, peripheral);
		break;
	}
}

void RCC_voidDisablePeripheralClk(RCC_BUS_t bus, u8 peripheral)
{
	switch(bus)
	{
	case RCC_BUS_AHB1:
		CLR_BIT(RCC_AHB1ENR, peripheral);
		break;
	case RCC_BUS_AHB2:
		CLR_BIT(RCC_AHB2ENR, peripheral);
		break;
	case RCC_BUS_APB1:
		CLR_BIT(RCC_APB1ENR, peripheral);
		break;
	case RCC_BUS_APB2:
		CLR_BIT(RCC_APB2ENR, peripheral);
		break;
	}
}

u32 RCC_u32GetSysClk(void)
{
	// talsema, fix it:
	return 16000000;
}

u16 RCC_u16GetBusPrescaler(RCC_BUS_t bus)
{
	u8 index;
	if (bus == RCC_BUS_AHB1		||		bus == RCC_BUS_AHB2)
	{
		index = GET_REG_SEC(RCC_CFGR, RCC_HPRE0, 4);
		if (index == 0)
			return 1;
		else if (index < 12)
			return POW_TWO(index - 7);
		else
			return POW_TWO(index - 6);
	}
	else
	{
		if (bus == RCC_BUS_APB1)
			index = GET_REG_SEC(RCC_CFGR, RCC_PPRE10, 3);
		else	// if (bus == RCC_BUS_APB2)
			index = GET_REG_SEC(RCC_CFGR, RCC_PPRE20, 3);

		if (index == 0)
			return 1;
		else
			return POW_TWO(index - 3);
	}
}
