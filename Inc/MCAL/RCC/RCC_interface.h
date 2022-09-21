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

typedef enum{
	RCC_BUS_AHB1,
	RCC_BUS_AHB2,
	RCC_BUS_APB1,
	RCC_BUS_APB2
}RCC_BUS_t;

#define RCC_SYSCLOCKSWITCH_HSI	0
#define RCC_SYSCLOCKSWITCH_HSE	1
#define RCC_SYSCLOCKSWITCH_PLL	2

typedef enum{
	RCC_AHBPRESCALER_1 		= 0,
	RCC_AHBPRESCALER_2 		= 8,
	RCC_AHBPRESCALER_4 		= 9,
	RCC_AHBPRESCALER_8 		= 10,
	RCC_AHBPRESCALER_16		= 11,
	RCC_AHBPRESCALER_64		= 12,
	RCC_AHBPRESCALER_128	= 13,
	RCC_AHBPRESCALER_256	= 14,
	RCC_AHBPRESCALER_512	= 15,
}RCC_AHBPRESCALER;

typedef enum{
	RCC_APBPRESCALER_1 		= 0,
	RCC_APBPRESCALER_2 		= 4,
	RCC_APBPRESCALER_4 		= 5,
	RCC_APBPRESCALER_8 		= 6,
	RCC_APBPRESCALER_16 	= 7,
}RCC_APBPRESCALER;

typedef enum{
	RCC_MCO1CLOCKSOURCE_HSI	= 0,
	RCC_MCO1CLOCKSOURCE_LSE	= 1,
	RCC_MCO1CLOCKSOURCE_HSE	= 2,
	RCC_MCO1CLOCKSOURCE_PLL	= 3
}RCC_MCO1CLOCKSOURCE;

typedef enum{
	RCC_MCO2CLOCKSOURCE_SYSCLK	= 0,
	RCC_MCO2CLOCKSOURCE_PLLI2S	= 1,
	RCC_MCO2CLOCKSOURCE_HSE		= 2,
	RCC_MCO2CLOCKSOURCE_PLL		= 3
}RCC_MCO2CLOCKSOURCE;

typedef enum{
	RCC_MCOPRESCALER_1 = 0,
	RCC_MCOPRESCALER_2 = 4,
	RCC_MCOPRESCALER_3 = 5,
	RCC_MCOPRESCALER_4 = 6,
	RCC_MCOPRESCALER_5 = 7,
}RCC_MCOPRESCALER;

#define RCC_PLLDEVISIONFACTOR_2		0
#define RCC_PLLDEVISIONFACTOR_4		1
#define RCC_PLLDEVISIONFACTOR_6		2
#define RCC_PLLDEVISIONFACTOR_8		3


#define RCC_PLLSOURCE_HSI	0
#define RCC_PLLSOURCE_HSE	1

#define RCC_HSI_CLK		16000000
#define RCC_LSI_CLK		8000000

void RCC_voidMCO1Init(RCC_MCO1CLOCKSOURCE clkSource, RCC_MCOPRESCALER prescaler);

void RCC_voidMCO2Init(RCC_MCO2CLOCKSOURCE clkSource, RCC_MCOPRESCALER prescaler);

void RCC_voidSysClockInit(void);

/*	prescaler functions are better be used before init RCC function	*/
void RCC_voidAHBClockInit(RCC_AHBPRESCALER	prescaler);

void RCC_voidAPB1ClockInit(RCC_APBPRESCALER	prescaler);

void RCC_voidAPB2ClockInit(RCC_APBPRESCALER	prescaler);

void RCC_voidEnablePeripheralClk(RCC_BUS_t bus, u8 peripheral);

void RCC_voidDisablePeripheralClk(RCC_BUS_t bus, u8 peripheral);

u32 RCC_u32GetSysClk(void);

u16 RCC_u16GetBusPrescaler(RCC_BUS_t bus);

#endif		/*	RCC_INTERFACE_H_	*/
