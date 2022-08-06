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

#include "MCAL/RCC/RCC_config.h"
#include "MCAL/RCC/RCC_interface.h"
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

void RCC_voidSysClockInit(
	b8 hsiEnable, b8 hseEnable, b8 pllEnable, b8 cssEnable,
	b8							hseBypass,
	RCC_SYSCLOCKSWITCH			sysClkSwitch,
	RCC_PLLSOURCE				pllClockSource,
	u16							pllMultiplicationFactor	/* i.e.: PLLN, from 2 to 432 */,
	u16							pllMainDivisionFactor	/* i.e.: PLLM, from 2 to 63 */,
	RCC_PLLSYSDEVISIONFACTOR	pllSysDivisionFactor	/* i.e.: PLLP */
	)
{
	/*	Enabling sources and clock security system	*/
	//WRT_BIT(RCC_CR, RCC_HSION, hsiEnable);
	//WRT_BIT(RCC_CR, RCC_HSEON, hseEnable);
	//WRT_BIT(RCC_CR, RCC_PLLON, pllEnable);
	//while(
	//	!GET_BIT(RCC_CR, HSI_RDY)	||
	//	!GET_BIT(RCC_CR, HSE_RDY)	||
	//	!GET_BIT(RCC_CR, PLL_RDY)
	//	);
	if (hseEnable)
	{
		SET_BIT(RCC_CR, RCC_HSEON);
		while(!GET_BIT(RCC_CR, RCC_HSERDY));
	}

	if (hsiEnable)
	{
		SET_BIT(RCC_CR, RCC_HSION);
		while(!GET_BIT(RCC_CR, RCC_HSIRDY));
	}

	if (pllEnable)
	{
		SET_BIT(RCC_CR, RCC_PLLON);
		while(!GET_BIT(RCC_CR, RCC_PLLRDY));
	}

	if (cssEnable)
		SET_BIT(RCC_CR, RCC_CSSON);
	else
		CLR_BIT(RCC_CR, RCC_CSSON);
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

	/*	Setting HSE bypass option	*/
	WRT_BIT(RCC_CR, RCC_HSEBYP, hseBypass);
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

	/*	setting PLL clock source	*/
	WRT_BIT(RCC_PLLCFGR, RCC_PLLSRC, pllClockSource);
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

	/*	Setting PLLM factor	*/
	/*	Check on pllMainDivisionFactor value limits	*/
	if (2 > pllMainDivisionFactor || pllMainDivisionFactor > 63)
	{
		/*	Wrong PLLM value	*/
		while(1);
	}
	/*	check on vcoInputFrequency value limits	*/
	u32 pllInputFrequency = pllClockSource ? RCC_HSE_CLK : RCC_HSI_CLK;
	u32 vcoInputFrequency = pllInputFrequency / pllMainDivisionFactor;
	if (1000000 > vcoInputFrequency || vcoInputFrequency > 2000000)
	{
		/*	Wrong vcoInputFrequency	*/
		while(1);
	}
	EDT_REG(RCC_PLLCFGR, RCC_PLLM0, pllMainDivisionFactor, 6);
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

	/*	Setting PLLN factor	*/
	/*	Check on PLLN value limits	*/
	if (2 > pllMultiplicationFactor || pllMultiplicationFactor > 432)
	{
		/*	Wrong pllMultiplicationFactor	*/
		while(1);
	}
	/*	Check on vcoOutputFrequency limits	*/
	u32 vcoOutputFrequency = vcoInputFrequency * pllMultiplicationFactor;
	if (192000000 > vcoOutputFrequency || vcoOutputFrequency > 432000000)
	{
		/*	Wrong vcoOutputFrequency	*/
		while(1);
	}
	EDT_REG(RCC_PLLCFGR, RCC_PLLN0, pllMultiplicationFactor, 9);
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

	/*	Setting PLLP factor	*/
	/*	Check on vcoOutputFrequency limits	*/
	u32 pllOutputFrequency = vcoOutputFrequency / ((pllSysDivisionFactor+1) * 2);
	if (pllOutputFrequency > 84000000)
	{
		/*	Wrong pllOutputFrequency	*/
		while(1);
	}
	EDT_REG(RCC_PLLCFGR, RCC_PLLP0, pllSysDivisionFactor, 2);
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

	/*	Setting system clock switch	*/
	EDT_REG(RCC_CFGR, RCC_SW0, sysClkSwitch, 2);
	while(
		(GET_BIT(RCC_CFGR, RCC_SWS0) | GET_BIT(RCC_CFGR, RCC_SWS1)<<1) !=  sysClkSwitch
		);
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

	/* Turn off the non-enabled sources to save power:
	 * Turning them off at the beginning of the function won't
	 * always work! as their ON bits may be kept 1 by hardware
	 * until a stable clock configuration is made.
	 */
	if (!hsiEnable)
	{
		CLR_BIT(RCC_CR, RCC_HSION);
	}
	if (!hseEnable)
	{
		CLR_BIT(RCC_CR, RCC_HSEON);
	}
	if (!pllEnable)
	{
		CLR_BIT(RCC_CR, RCC_PLLON);
	}
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

void RCC_voidEnablePeripheralClk(RCC_PERIPHERAL peripheral)
{
	/*	"very heavy" one could say... dude it happens once in the initialization.	*/
	SET_BIT(*(volatile u32*)(&RCC_AHB1ENR + peripheral/32 + (peripheral/32+1)/2), (peripheral % 32));
}

void RCC_voidDisablePeripheralClk(RCC_PERIPHERAL peripheral)
{
	/*	"very heavy" one could say... dude it happens once in the initialization.	*/
	CLR_BIT(*(volatile u32*)(&RCC_AHB1ENR + peripheral/32 + (peripheral/32+1)/2), (peripheral % 32));
}
