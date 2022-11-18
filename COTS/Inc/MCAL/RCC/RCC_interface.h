/*
 * RCC_interface.h
 *
 *  Created on:	Jul 26, 2022
 *  Author:		Ali Emad Ali
 *
 *  References:
 *  	STM RM0368:	6.	Reset and clock control (RCC) for STM32F401xB/C and STM32F401xD/E
 */

#ifndef RCC_INTERFACE_H_
#define RCC_INTERFACE_H_

#include "Target_config.h"

/*	PERIPHERALS	*/
#if TARGET_ID == STM32F401x
/*	AHB1 peripherals	*/
#define 	RCC_PERIPHERAL_GPIOA	 0
#define 	RCC_PERIPHERAL_GPIOB	 1
#define 	RCC_PERIPHERAL_GPIOC	 2
#define 	RCC_PERIPHERAL_GPIOD	 3
#define 	RCC_PERIPHERAL_GPIOE	 4
#define 	RCC_PERIPHERAL_GPIOH	 7
#define 	RCC_PERIPHERAL_CRC		 12
#define 	RCC_PERIPHERAL_DMA1		 21
#define 	RCC_PERIPHERAL_DMA2		 22

/*	AHB2 peripherals	*/
#define		RCC_PERIPHERAL_OTGFS	7
/*	~~~~~~~~~~~~~~~~~~~	*/

/*	APB1 peripherals	*/
#define 	RCC_PERIPHERAL_TIM2		0
#define 	RCC_PERIPHERAL_TIM3		1
#define 	RCC_PERIPHERAL_TIM4		2
#define 	RCC_PERIPHERAL_TIM5		3
#define 	RCC_PERIPHERAL_WWDG		11
#define 	RCC_PERIPHERAL_SPI2		14
#define 	RCC_PERIPHERAL_SPI3		15
#define 	RCC_PERIPHERAL_USART2	17
#define 	RCC_PERIPHERAL_I2C1		21
#define 	RCC_PERIPHERAL_I2C2		22
#define 	RCC_PERIPHERAL_I2C3		23
#define 	RCC_PERIPHERAL_PWR		28
/*	~~~~~~~~~~~~~~~~~~~	*/

/*	APB2 peripherals	*/
#define 	RCC_PERIPHERAL_TIM1		0
#define 	RCC_PERIPHERAL_USART1	4
#define 	RCC_PERIPHERAL_USART6	5
#define 	RCC_PERIPHERAL_ADC1		8
#define 	RCC_PERIPHERAL_SDIO		11
#define 	RCC_PERIPHERAL_SPI1		12
#define 	RCC_PERIPHERAL_SPI4		13
#define 	RCC_PERIPHERAL_SYSCFG	14
#define 	RCC_PERIPHERAL_TIM9		16
#define 	RCC_PERIPHERAL_TIM10	17
#define 	RCC_PERIPHERAL_TIM11	18
/*	~~~~~~~~~~~~~~~~~~~	*/

#elif TARGET_ID == STM32F10x
/*	AHB peripherals	*/
#define RCC_PERIPHERAL_DMA1			0
#define RCC_PERIPHERAL_DMA2			1
#define RCC_PERIPHERAL_SRAM			2
#define RCC_PERIPHERAL_FLITF		4
#define RCC_PERIPHERAL_CRC			6
#define RCC_PERIPHERAL_OTG			12
#define RCC_PERIPHERAL_ETHMAC		14
#define RCC_PERIPHERAL_ETHMACTX		15
#define RCC_PERIPHERAL_ETHMACRX		16

/*	APB1 peripherals	*/
#define RCC_PERIPHERAL_TIM2			0
#define RCC_PERIPHERAL_TIM3			1
#define RCC_PERIPHERAL_TIM4			2
#define RCC_PERIPHERAL_TIM5			3
#define RCC_PERIPHERAL_TIM6			4
#define RCC_PERIPHERAL_TIM7			5
#define RCC_PERIPHERAL_WWDG			11
#define RCC_PERIPHERAL_SPI2			14
#define RCC_PERIPHERAL_SPI3			15
#define RCC_PERIPHERAL_USART2		17
#define RCC_PERIPHERAL_USART3		18
#define RCC_PERIPHERAL_UART4		19
#define RCC_PERIPHERAL_UART5		20
#define RCC_PERIPHERAL_I2C1			21
#define RCC_PERIPHERAL_I2C2			22
#define RCC_PERIPHERAL_CAN1			25
#define RCC_PERIPHERAL_CAN2			26
#define RCC_PERIPHERAL_BKP			27
#define RCC_PERIPHERAL_PWR			28
#define RCC_PERIPHERAL_DAC			29

/*	APB2 peripherals	*/
#define RCC_PERIPHERAL_AFIO			0
#define RCC_PERIPHERAL_EXTI			1
#define RCC_PERIPHERAL_IOPA			2
#define RCC_PERIPHERAL_IOPB			3
#define RCC_PERIPHERAL_IOPC			4
#define RCC_PERIPHERAL_IOPD			5
#define RCC_PERIPHERAL_IOPE			6
#define RCC_PERIPHERAL_ADC1			9
#define RCC_PERIPHERAL_ADC2			10
#define RCC_PERIPHERAL_TIM1			11
#define RCC_PERIPHERAL_SPI1			12
#define RCC_PERIPHERAL_TIM8			13
#define RCC_PERIPHERAL_USART1		14
#define RCC_PERIPHERAL_ADC3			15
#define RCC_PERIPHERAL_TIM9			19
#define RCC_PERIPHERAL_TIM10		20
#define RCC_PERIPHERAL_TIM11		21

#endif	/*	PERIPHERALS	*/

typedef enum{
	#if TARGET_ID == STM32F401x
	RCC_Bus_AHB1,
	RCC_Bus_AHB2,
	#define RCC_Bus_AHB 0		// to be used in "getClk() & getPrescaler()"
	#elif TARGET_ID == STM32F10x
	RCC_Bus_AHB,
	#endif
	RCC_Bus_APB1,
	RCC_Bus_APB2
}RCC_Bus_t;

#define RCC_SYSCLOCKSWITCH_HSI	0
#define RCC_SYSCLOCKSWITCH_HSE	1
#define RCC_SYSCLOCKSWITCH_PLL	2

typedef enum{
	RCC_AHB_Prescaler_1 		= 0,
	RCC_AHB_Prescaler_2 		= 8,
	RCC_AHB_Prescaler_4 		= 9,
	RCC_AHB_Prescaler_8 		= 10,
	RCC_AHB_Prescaler_16		= 11,
	RCC_AHB_Prescaler_64		= 12,
	RCC_AHB_Prescaler_128		= 13,
	RCC_AHB_Prescaler_256		= 14,
	RCC_AHB_Prescaler_512		= 15,
}RCC_AHB_Prescaler_t;

typedef enum{
	RCC_APB_Prescaler_1 		= 0,
	RCC_APB_Prescaler_2 		= 4,
	RCC_APB_Prescaler_4 		= 5,
	RCC_APB_Prescaler_8 		= 6,
	RCC_APB_Prescaler_16 		= 7,
}RCC_APB_Prescaler_t;

#if TARGET_ID == STM32F10x
typedef enum{
	RCC_ADC_Prescaler_PCLK2_by2,
	RCC_ADC_Prescaler_PCLK2_by4,
	RCC_ADC_Prescaler_PCLK2_by6,
	RCC_ADC_Prescaler_PCLK2_by8
}RCC_ADC_Prescaler_t;
#endif

#if TARGET_ID == STM32F401x
typedef enum{
	RCC_MCO1_ClockSource_HSI		= 0,
	RCC_MCO1_ClockSource_LSE		= 1,
	RCC_MCO1_ClockSource_HSE		= 2,
	RCC_MCO1_ClockSource_PLL		= 3
}RCC_MCO1_ClockSource_t;

typedef enum{
	RCC_MCO2_ClockSource_SYSCLK		= 0,
	RCC_MCO2_ClockSource_PLLI2S		= 1,
	RCC_MCO2_ClockSource_HSE		= 2,
	RCC_MCO2_ClockSource_PLL		= 3
}RCC_MCO2_ClockSource_t;

typedef enum{
	RCC_MCO_Prescaler_1				= 0,
	RCC_MCO_Prescaler_2				= 4,
	RCC_MCO_Prescaler_3				= 5,
	RCC_MCO_Prescaler_4				= 6,
	RCC_MCO_Prescaler_5				= 7,
}RCC_MCO_Prescaler_t;

#elif TARGET_ID == STM32F10x
typedef enum{
	RCC_MCO_ClockSource_NoClock		= 0,
	RCC_MCO_ClockSource_SYSCLK		= 4,
	RCC_MCO_ClockSource_HSI			= 5,
	RCC_MCO_ClockSource_HSE			= 6,
	RCC_MCO_ClockSource_PLL_by2		= 7
}RCC_MCO_ClockSource_t;
#endif

#if TARGET_ID == STM32F401x
#define RCC_PLLDEVISIONFACTOR_2		0
#define RCC_PLLDEVISIONFACTOR_4		1
#define RCC_PLLDEVISIONFACTOR_6		2
#define RCC_PLLDEVISIONFACTOR_8		3
#define RCC_PLLSOURCE_HSI			0
#define RCC_PLLSOURCE_HSE			1

#elif TARGET_ID == STM32F10x
#define RCC_PLLSOURCE_HSI_by2		0
#define RCC_PLLSOURCE_PLLXTPRE		1

#define RCC_PLLXTPRESOURCE_HSE		0
#define RCC_PLLXTPRESOURCE_HSE_by2	1
#endif

#if TARGET_ID == STM32F401x
#define RCC_HSI_CLK		16000000
#define RCC_LSI_CLK		32000
#elif TARGET_ID == STM32F10x
#define RCC_HSI_CLK		8000000
#define RCC_LSI_CLK		40000
#endif

/*	inits the RCC HW according to the chosen configuration in "RCC_config.h".	*/
void RCC_voidSysClockInit(void);

/*
 * sets prescaler of AHB bus.
 * (better be called before initing RCC)
 */
void RCC_voidAHBClockInit(RCC_AHB_Prescaler_t	prescaler);

/*
 * sets prescaler of APB1 bus.
 * (better be called before initing RCC)
 */
void RCC_voidAPB1ClockInit(RCC_APB_Prescaler_t	prescaler);

/*
 * sets prescaler of APB2 bus.
 * (better be called before initing RCC)
 */
void RCC_voidAPB2ClockInit(RCC_APB_Prescaler_t	prescaler);

#if TARGET_ID == STM32F10x
/*	sets ADC prescaler	*/
void RCC_voidSetAdcPrescaler(RCC_APB_Prescaler_t prescaler);
#endif

/*	enables peripheral clock on certain bus	*/
void RCC_voidEnablePeripheralClk(RCC_Bus_t bus, u8 peripheral);

/*	disables peripheral clock on certain bus	*/
void RCC_voidDisablePeripheralClk(RCC_Bus_t bus, u8 peripheral);

#if RCC_PLL_EN
/*	returns PLL clock in Hz	*/
u32 RCC_u32GetPllClk(void);
#endif

/*	returns system clock in Hz	*/
u32 RCC_u32GetSysInClk(void);

/*	returns bus clock in Hz	*/
u32 RCC_u32GetBusClk(RCC_Bus_t bus);

/*	returns bus prescaler value	*/
u16 RCC_u16GetBusPrescaler(RCC_Bus_t bus);

/*	inits MCO	*/
#if TARGET_ID == STM32F401x
void RCC_voidMCO1Init(RCC_MCO1_ClockSource_t clkSource, RCC_MCO_Prescaler_t prescaler);
void RCC_voidMCO2Init(RCC_MCO2_ClockSource_t clkSource, RCC_MCO_Prescaler_t prescaler);
#elif TARGET_ID == STM32F10x
void RCC_voidMCOInit(RCC_MCO_ClockSource_t clkSource);
#endif

#endif		/*	RCC_INTERFACE_H_	*/

