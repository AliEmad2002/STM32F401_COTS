/*
 * RCC_program.c
 *
 *  Created on:	Jul 26, 2022
 *  Author:		Ali Emad Ali
 *
 *  References:
 *  	STM RM0368:	6.	Reset and clock control (RCC) for STM32F401xB/C and STM32F401xD/E
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Target_config.h"

/*	SELF	*/
#include "RCC_interface.h"
#include "RCC_config.h"
#include "RCC_private.h"

#if RCC_PLL_EN
/*
 * to store PLL frequency calculated in init function,
 * to be used - when needed - in the GetClk function.
 */
volatile u32 pllOutputFreq;

/*	inits PLL (target dependent)	*/
void RCC_voidInitPLL(void);
#endif

/*	inits the RCC HW according to the chosen configuration in "RCC_config.h".	*/
void RCC_voidSysClockInit(void)
{
	/*	Enabling selected sources, and clock security system	*/
	#if RCC_HSE_EN
		/*	Setting HSE bypass option before enabling it	*/
		WRT_BIT(RCC->CR, RCC_HSEBYP, RCC_HSE_BYPASSED);
		SET_BIT(RCC->CR, RCC_HSEON);
		while(!GET_BIT(RCC->CR, RCC_HSERDY));
	#endif	/*	RCC_HSE_EN	*/

	#if RCC_HSI_EN
		SET_BIT(RCC->CR, RCC_HSION);
		while(!GET_BIT(RCC->CR, RCC_HSIRDY));
	#endif	/*	RCC_HSI_EN	*/

	#if RCC_PLL_EN
		/*	configuring PLL first before enabling it	*/
		RCC_voidInitPLL();
		SET_BIT(RCC->CR, RCC_PLLON);
		while(!GET_BIT(RCC->CR, RCC_PLLRDY));
	#endif	/*	RCC_HSE_EN	*/

	#if RCC_CSS_EN
		SET_BIT(RCC->CR, RCC_CSSON);
	#else
		CLR_BIT(RCC->CR, RCC_CSSON);
	#endif	/*	RCC_CSS_EN	*/
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

	/*	Setting system clock switch	*/
	EDT_REG(RCC->CFGR, RCC_SW_0, RCC_SYS_SOURCE, 2);
	while((GET_BIT(RCC->CFGR, RCC_SWS_0) | GET_BIT(RCC->CFGR, RCC_SWS_1)<<1) !=  RCC_SYS_SOURCE);
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/
}


/*
 * sets prescaler of AHB bus.
 * (better be called before initing RCC)
 */
void RCC_voidAHBClockInit(RCC_AHB_Prescaler_t prescaler)
{
	EDT_REG(RCC->CFGR, RCC_HPRE_0, prescaler, 4);
}

/*
 * sets prescaler of APB1 bus.
 * (better be called before initing RCC)
 */
void RCC_voidAPB1ClockInit(RCC_APB_Prescaler_t prescaler)
{
	EDT_REG(RCC->CFGR, RCC_PPRE1_0, prescaler, 3);
}

/*
 * sets prescaler of APB2 bus.
 * (better be called before initing RCC)
 */
void RCC_voidAPB2ClockInit(RCC_APB_Prescaler_t prescaler)
{
	EDT_REG(RCC->CFGR, RCC_PPRE2_0, prescaler, 3);
}

#if TARGET_ID == STM32F10x
/*	sets ADC prescaler	*/
void RCC_voidSetAdcPrescaler(RCC_APB_Prescaler_t prescaler)
{
	EDT_REG(RCC->CFGR, RCC_ADCPRE_0, prescaler, 2);
}
#endif

/*	enables peripheral clock on certain bus	*/
void RCC_voidEnablePeripheralClk(RCC_Bus_t bus, u8 peripheral)
{
	switch(bus)
	{
	#if TARGET_ID == STM32F401x
	case RCC_Bus_AHB1:
		SET_BIT(RCC->AHB1ENR, peripheral);
		break;
	case RCC_Bus_AHB2:
		SET_BIT(RCC->AHB2ENR, peripheral);
		break;
	#elif TARGET_ID == STM32F10x
	case RCC_Bus_AHB:
		SET_BIT(RCC->AHBENR, peripheral);
		break;
	#endif

	case RCC_Bus_APB1:
		SET_BIT(RCC->APB1ENR, peripheral);
		break;
	case RCC_Bus_APB2:
		SET_BIT(RCC->APB2ENR, peripheral);
		break;
	}
}

/*	disables peripheral clock on certain bus	*/
void RCC_voidDisablePeripheralClk(RCC_Bus_t bus, u8 peripheral)
{
	switch(bus)
	{
	#if TARGET_ID == STM32F401x
	case RCC_Bus_AHB1:
		CLR_BIT(RCC->AHB1ENR, peripheral);
		break;
	case RCC_Bus_AHB2:
		CLR_BIT(RCC->AHB2ENR, peripheral);
		break;
	#elif TARGET_ID == STM32F10x
	case RCC_Bus_AHB:
		CLR_BIT(RCC->AHBENR, peripheral);
		break;
	#endif

	case RCC_Bus_APB1:
		CLR_BIT(RCC->APB1ENR, peripheral);
		break;
	case RCC_Bus_APB2:
		CLR_BIT(RCC->APB2ENR, peripheral);
		break;
	}
}

/*	returns system clock in Hz	*/
u32 RCC_u32GetSysInClk(void)
{
	#if RCC_SYS_SOURCE == RCC_SYSCLOCKSWITCH_HSI
	return RCC_HSI_CLK;

	#elif RCC_SYS_SOURCE == RCC_SYSCLOCKSWITCH_HSE
	return RCC_HSE_CLK;

	#elif RCC_SYS_SOURCE == RCC_SYSCLOCKSWITCH_PLL
	return pllOutputFreq;
	#endif
}

/*	returns bus clock in Hz	*/
u32 RCC_u32GetBusClk(RCC_Bus_t bus)
{
	#if TARGET_ID == STM32F401x
	u32 ahbClk = RCC_u32GetSysInClk() / RCC_u16GetBusPrescaler(RCC_Bus_AHB1);
	if (bus == RCC_Bus_AHB1		||		bus == RCC_Bus_AHB2)
		return ahbClk;
	#elif TARGET_ID == STM32F10x
	u32 ahbClk = RCC_u32GetSysInClk() / RCC_u16GetBusPrescaler(RCC_Bus_AHB);
	if (bus == RCC_Bus_AHB)
		return ahbClk;
	#endif

	else if(bus == RCC_Bus_APB1)
		return ahbClk / RCC_u16GetBusPrescaler(RCC_Bus_APB1);
	else
		return ahbClk / RCC_u16GetBusPrescaler(RCC_Bus_APB2);
}

/*	returns bus prescaler value	*/
u16 RCC_u16GetBusPrescaler(RCC_Bus_t bus)
{
	u8 index;
	#if TARGET_ID == STM32F401x
	if (bus == RCC_Bus_AHB1		||		bus == RCC_Bus_AHB2)
	#elif TARGET_ID == STM32F10x
	if (bus == RCC_Bus_AHB)
	#endif
	{
		index = GET_REG_SEC(RCC->CFGR, RCC_HPRE_0, 4);
		if (index == 0)
			return 1;
		else if (index < 12)
			return POW_TWO(index - 7);
		else
			return POW_TWO(index - 6);
	}
	else
	{
		if (bus == RCC_Bus_APB1)
			index = GET_REG_SEC(RCC->CFGR, RCC_PPRE1_0, 3);
		else	// if (bus == RCC_BUS_APB2)
			index = GET_REG_SEC(RCC->CFGR, RCC_PPRE2_0, 3);

		if (index == 0)
			return 1;
		else
			return POW_TWO(index - 3);
	}
}

/*	inits MCO	*/
#if TARGET_ID == STM32F401x
void RCC_voidMCO1Init(RCC_MCO1_ClockSource_t clkSource, RCC_MCO_Prescaler_t prescaler)
{
	EDT_REG(RCC->CFGR, RCC_MCO1_0, clkSource, 2);
	EDT_REG(RCC->CFGR, RCC_MCO1PRE_0, prescaler, 3);
}

void RCC_voidMCO2Init(RCC_MCO2_ClockSource_t clkSource, RCC_MCO_Prescaler_t prescaler)
{
	EDT_REG(RCC->CFGR, RCC_MCO2_0, clkSource, 2);
	EDT_REG(RCC->CFGR, RCC_MCO2PRE_0, prescaler, 3);
}

#elif TARGET_ID == STM32F10x
void RCC_voidMCOInit(RCC_MCO_ClockSource_t clkSource)
{
	EDT_REG(RCC->CFGR, RCC_MCO_0, clkSource, 4);
}
#endif

/*	inits PLL (target dependent)	*/
#if TARGET_ID == STM32F401x		&&		RCC_PLL_EN == true
void RCC_voidInitPLL(void)
{
	/*	setting PLL clock source	*/
	WRT_BIT(RCC->PLLCFGR, RCC_PLLSRC, RCC_PLL_CLK_SOURCE);
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

	EDT_REG(RCC->PLLCFGR, RCC_PLLM_0, RCC_PLL_MAIN_DIVISION_FACTOR, 6);
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

	EDT_REG(RCC->PLLCFGR, RCC_PLLN_0, RCC_PLL_MULTIPLICATION_FACTOR, 9);
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

	/*	Setting PLLP factor	*/
	/*	Check on vcoOutputFrequency limits	*/
	#define pllOugftputFrequency (vcoOutputFrequency / ((RCC_PLL_SYS_DIVISION_FACTOR+1) * 2))
	#if pllOutputFrequency > 84000000
		#error worng "RCC_PLL_SYS_DIVISION_FACTOR" value!
	#endif	/*	pllOutputFrequency > 84000000	*/
	#if RCC_SYS_SOURCE == RCC_SYSCLOCKSWITCH_PLL
	pllOutputFreq = pllOugftputFrequency;
	#endif
	EDT_REG(RCC->PLLCFGR, RCC_PLLP_0, RCC_PLL_SYS_DIVISION_FACTOR, 2);

	/*undef un-necessary defines	*/
	#undef	pllOugftputFrequency
	#undef	vcoOutputFrequency
	#undef	vcoInputFrequency
	#undef	pllInputFrequency
}

#elif TARGET_ID == STM32F10x	&&		RCC_PLL_EN == true
								/*	! ! ! ! ! ! N O T E ! ! ! ! ! !	*/
/*	THIS FUUNCTION DOES NOT IMPLEMENT PLL2 & PLL3 CONFIGURATION, TO BE IMPLEMENTED NEXT VERSION	*/
void RCC_voidInitPLL(void)
{
	/*	setting PLLXTPRE clock source (if chosen)	*/
	#if RCC_PLL_CLK_SOURCE == RCC_PLLSOURCE_PLLXTPRE
		WRT_BIT(RCC->CFGR, RCC_PLLSRC, RCC_PLLSOURCE_PLLXTPRE);
		WRT_BIT(RCC->CFGR, RCC_PLLXTPRE, RCC_PLLXTPRESOURCE);
		#define PLL_SRC		RCC_HSE_CLK / (1 + RCC_PLLXTPRESOURCE)
	/*	else, PLL source is HSI by 2	*/
	#else
		WRT_BIT(RCC->CFGR, RCC_PLLSRC, 0);
		#define PLL_SRC		RCC_HSI_CLK / 2
	#endif

	/*	setting PLL MUL	*/
	EDT_REG(RCC->CFGR, RCC_PLLMUL_0, RCC_PLLMUL - 2, 4);

	/*	storing PLL output freq, to be used in sime functions	*/
	pllOutputFreq = PLL_SRC * RCC_PLLMUL;

	/*undef un-necessary defines	*/
	#undef  PLL_SRC
}
#endif
















