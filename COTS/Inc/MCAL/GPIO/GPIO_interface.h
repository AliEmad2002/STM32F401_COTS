/*
 * GPIO_interface.h
 *
 *  Created on:	Jul 27, 2022
 *  Author:		Ali Emad Ali
 *
 *  References:
 *  	STM RM0368:	8.	General-purpose I/Os (GPIO)
 */

/*	N O T E ! ! ! ! !	*/
/*	THIS DRIVER DOES NOT YET IMPLEMENT AFIO OF STM32F10x	*/
/*	TO BE DONE NEXT VERSION...	*/


#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_

#include "Target_config.h"
#include "GPIO_private.h"

typedef enum{
	/*	PORTA pins	*/
	GPIO_Pin_A0 	= 0,
	GPIO_Pin_A1 	= 1,
	GPIO_Pin_A2 	= 2,
	GPIO_Pin_A3 	= 3,
	GPIO_Pin_A4 	= 4,
	GPIO_Pin_A5 	= 5,
	GPIO_Pin_A6 	= 6,
	GPIO_Pin_A7 	= 7,
	GPIO_Pin_A8 	= 8,
	GPIO_Pin_A9 	= 9,
	GPIO_Pin_A10    = 10,
	GPIO_Pin_A11    = 11,
	GPIO_Pin_A12    = 12,
	GPIO_Pin_A13    = 13,
	GPIO_Pin_A14    = 14,
	GPIO_Pin_A15    = 15,
	/*	~~~~~~~~~~~~	*/

	/*	PORTB pins	*/
	GPIO_Pin_B0 	= 16 + 0,
	GPIO_Pin_B1 	= 16 + 1,
	GPIO_Pin_B2 	= 16 + 2,
	GPIO_Pin_B3 	= 16 + 3,
	GPIO_Pin_B4 	= 16 + 4,
	GPIO_Pin_B5 	= 16 + 5,
	GPIO_Pin_B6 	= 16 + 6,
	GPIO_Pin_B7 	= 16 + 7,
	GPIO_Pin_B8 	= 16 + 8,
	GPIO_Pin_B9 	= 16 + 9,
	GPIO_Pin_B10    = 16 + 10,
	GPIO_Pin_B11    = 16 + 11,
	GPIO_Pin_B12    = 16 + 12,
	GPIO_Pin_B13    = 16 + 13,
	GPIO_Pin_B14    = 16 + 14,
	GPIO_Pin_B15    = 16 + 15,
	/*	~~~~~~~~~~~~	*/

	/*	PORTB pins	*/
	GPIO_Pin_C0 	= 16*2 + 0,
	GPIO_Pin_C1 	= 16*2 + 1,
	GPIO_Pin_C2 	= 16*2 + 2,
	GPIO_Pin_C3 	= 16*2 + 3,
	GPIO_Pin_C4 	= 16*2 + 4,
	GPIO_Pin_C5 	= 16*2 + 5,
	GPIO_Pin_C6 	= 16*2 + 6,
	GPIO_Pin_C7 	= 16*2 + 7,
	GPIO_Pin_C8 	= 16*2 + 8,
	GPIO_Pin_C9 	= 16*2 + 9,
	GPIO_Pin_C10    = 16*2 + 10,
	GPIO_Pin_C11    = 16*2 + 11,
	GPIO_Pin_C12    = 16*2 + 12,
	GPIO_Pin_C13    = 16*2 + 13,
	GPIO_Pin_C14    = 16*2 + 14,
	GPIO_Pin_C15    = 16*2 + 15
	/*	~~~~~~~~~~~~	*/
}GPIO_Pin_t;

typedef enum{
	GPIO_PortName_A = 0,
	GPIO_PortName_B = 1,
	GPIO_PortName_C = 2
}GPIO_PortName_t;

#if TARGET_ID == STM32F401x
typedef enum{
	GPIO_Mode_input		= 0,
	GPIO_Mode_GPO		= 1,
	GPIO_Mode_AF		= 2,
	GPIO_Mode_analog	= 3
}GPIO_Mode_t;

typedef enum{
	GPIO_OutputType_PushPull	= 0,
	GPIO_OutputType_OpenDrain	= 1
}GPIO_OutputType_t;

typedef enum{
	GPIO_OutputSpeed_Low		= 0,
	GPIO_OutputSpeed_Medium		= 1,
	GPIO_OutputSpeed_High		= 2,
	GPIO_OutputSpeed_VeryHigh	= 3
}GPIO_OutputSpeed_t;

typedef enum{
	GPIO_Pull_floating	= 0,
	GPIO_Pull_up		= 1,
	GPIO_Pull_down		= 2
}GPIO_Pull_t;

typedef enum{
	GPIO_AlternateFunction_system,
	GPIO_AlternateFunction_tim1And2,
	GPIO_AlternateFunction_tim3To5,
	GPIO_AlternateFunction_tim9To11,
	GPIO_AlternateFunction_I2C1To3,
	GPIO_AlternateFunction_SPI1To4,
	GPIO_AlternateFunction_SPI3,
	GPIO_AlternateFunction_USART1And2,
	GPIO_AlternateFunction_USART6,
	GPIO_AlternateFunction_I2C2And3,
	GPIO_AlternateFunction_OTG,
	GPIO_AlternateFunction_11,
	GPIO_AlternateFunction_SDIO,
	GPIO_AlternateFunction_13,
	GPIO_AlternateFunction_14,
	GPIO_AlternateFunction_eventOut
}GPIO_AlternateFunction_t;

#elif TARGET_ID == STM32F10x
typedef enum{
	GPIO_Mode_GPO_PushPull,
	GPIO_Mode_GPO_OpenDrain,
	GPIO_Mode_AF_PushPull,
	GPIO_Mode_AF_OpenDrain,
	GPIO_Mode_Input_Analog,
	GPIO_Mode_Input_Floating,
	GPIO_Mode_Input_Pull
}GPIO_Mode_t;

typedef enum{
	GPIO_OutputSpeed_Null,	//	used when pin is input.
	GPIO_OutputSpeed_10MHz,
	GPIO_OutputSpeed_2MHz,
	GPIO_OutputSpeed_50MHz
}GPIO_OutputSpeed_t;
#endif

typedef enum{
	/*
	 * low and high should be always kept 0 and 1, as many other drivers/modules
	 * depend on that in operation.
	 */
	GPIO_OutputLevel_Low				= 0,
	GPIO_OutputLevel_High				= 1,
	GPIO_OutputLevel_OpenDrainFloat		= 1		/*	remember 8.3.10 Output configuration in datasheet	*/
}GPIO_OutputLevel_t;

/*	setting pin mode	*/
void GPIO_voidSetPinMode(GPIO_PortName_t port, u8 pin, GPIO_Mode_t mode);

void GPIO_voidSetPinGpoPushPull(GPIO_PortName_t port, u8 pin);

void GPIO_voidSetPinGpoOpenDrain(GPIO_PortName_t port, u8 pin);

void GPIO_voidSetPinInputPullUp(GPIO_PortName_t port, u8 pin);

#if TARGET_ID == STM32F401x
/*	setting pin output type	*/
void GPIO_voidSetPinOutputType(GPIO_PortName_t port, u8 pin, GPIO_OutputType_t outputType);

/*	setting pin pull up/down	*/
void GPIO_voidSetPinPull(GPIO_PortName_t port, u8 pin, GPIO_Pull_t pull);

/*	setting pin's AF - if used as AF -.	*/
void GPIO_voidSetAlternateFunction(GPIO_PortName_t port, u8 pin, GPIO_AlternateFunction_t af);
#endif

/*	setting pin output speed	*/
void GPIO_voidSetPinOutputSpeed(GPIO_PortName_t port, u8 pin, GPIO_OutputSpeed_t outputSpeed);

/*	reading pin input level	*/
b8 GPIO_b8ReadPinDigital(GPIO_PortName_t port, u8 pin);

/*	setting pin output level	*/
void GPIO_voidSetPinOutputLevel(GPIO_PortName_t port, u8 pin, GPIO_OutputLevel_t level);
#define GPIO_SET_PIN_HIGH(port, pin)		(GPIO[(port)]->BSRR = 1u<<(pin))
#define GPIO_SET_PIN_LOW(port, pin)		(GPIO[(port)]->BSRR = 1u<<(pin+16))

/*	toggling pin output level	*/
void GPIO_voidTogglePinOutputLevel(GPIO_PortName_t port, u8 pin);

/*	setting pin configuration lock 	*/
void GPIO_voidLockPinConfiguration(GPIO_PortName_t port, u8 pin);

#if TARGET_ID == STM32F10x
/*
 * assigns a pin to the EXTI line.
 *
 * pin number is same as EXTI line number
 * (forced by HW).
 *
 * port is selectable.
 */
void AFIO_voidSetExtiLineSrc(u8 line, GPIO_PortName_t port);

typedef enum{
	AFIO_Peripheral_SPI1,
	AFIO_Peripheral_I2C1,
	AFIO_Peripheral_USART1,
	AFIO_Peripheral_USART2,
	AFIO_Peripheral_USART3,
	AFIO_Peripheral_TIM1,
	AFIO_Peripheral_TIM2,
	AFIO_Peripheral_TIM3,
	AFIO_Peripheral_TIM4,
	AFIO_Peripheral_CAN,
	AFIO_Peripheral_PD01,
	AFIO_Peripheral_TIM5CH4,
	AFIO_Peripheral_ADC1_ETRGINJ,
	AFIO_Peripheral_ADC1_ETRGREG,
	AFIO_Peripheral_ADC2_ETRGINJ,
	AFIO_Peripheral_ADC2_ETRGREG,
	AFIO_Peripheral_SWJ_CFG
}AFIO_Peripheral_t;

typedef enum{
	AFIO_Usart1_Map_TxA9_RxA10,
	AFIO_Usart1_Map_TxB6_RxB7
}AFIO_Usart1_Map_t;

typedef enum{
	AFIO_Usart2_Map_TxA2_RxA3,
	AFIO_Usart2_Map_TxD5_RxD6,
}AFIO_Usart2_Map_t;

typedef enum{
	AFIO_SPI1_Map_NssA4_SckA5_MisoA6_MosiA7,
	AFIO_SPI1_Map_NssA15_SckB3_MisoB4_MosiB5,
}AFIO_SPI1_Map_t;

void AFIO_voidRemap(AFIO_Peripheral_t peripheral, u8 map);
#endif

#endif /* GPIO_INTERFACE_H_ */
