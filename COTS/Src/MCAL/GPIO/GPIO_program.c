/*
 * GPIO_program.h
 *
 *  Created on:	Jul 27, 2022
 *  Author:		Ali Emad Ali
 *
 *  References:
 *  	STM RM0368:	8.	General-purpose I/Os (GPIO)
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Target_config.h"

/*	SELF	*/
#include "GPIO_interface.h"
#include "GPIO_private.h"

/*	setting pin mode	*/
void GPIO_voidSetPinMode(GPIO_PortName_t port, u8 pin, GPIO_Mode_t mode)
{
	#if TARGET_ID == STM32F401x
	EDT_REG(GPIO[port]->MODER, 2*pin, mode, 2);
	#elif TARGET_ID == STM32F10x
	EDT_REG(GPIO[port]->CR, 4*pin+2, mode, 2);
	#endif
}

void GPIO_voidSetPinGpoPushPull(GPIO_PortName_t port, u8 pin)
{
	#if TARGET_ID == STM32F401x
	GPIO_voidSetPinMode(port, pin, GPIO_Mode_GPO);
	GPIO_voidSetPinOutputType(port, pin, GPIO_OutputType_PushPull);
	#elif TARGET_ID == STM32F10x
	GPIO_voidSetPinMode(port, pin, GPIO_Mode_GPO_PushPull);
	GPIO_voidSetPinOutputSpeed(port, pin, GPIO_OutputSpeed_10MHz);
	#endif
}

void GPIO_voidSetPinGpoOpenDrain(GPIO_PortName_t port, u8 pin)
{
	#if TARGET_ID == STM32F401x
	GPIO_voidSetPinMode(port, pin, GPIO_Mode_GPO);
	GPIO_voidSetPinOutputType(port, pin, GPIO_OutputType_OpenDrain);
	#elif TARGET_ID == STM32F10x
	GPIO_voidSetPinMode(port, pin, GPIO_Mode_GPO_OpenDrain);
	GPIO_voidSetPinOutputSpeed(port, pin, GPIO_OutputSpeed_10MHz);
	#endif
}

void GPIO_voidSetPinInputPullUp(GPIO_PortName_t port, u8 pin)
{
	#if TARGET_ID == STM32F401x
	GPIO_voidSetPinMode(port, pin, GPIO_Mode_input);
	GPIO_voidSetPinPull(port, pin, GPIO_Pull_up);
	#elif TARGET_ID == STM32F10x
	GPIO_voidSetPinMode(port, pin, GPIO_Mode_Input_PullUp);
	GPIO_voidSetPinOutputSpeed(port, pin, GPIO_OutputSpeed_Null);
	#endif
}

#if TARGET_ID == STM32F401x
/*	setting pin output type	*/
void GPIO_voidSetPinOutputType(GPIO_PortName_t port, u8 pin, GPIO_OutputType_t outputType)
{
	WRT_BIT(GPIO[port]->OTYPER, pin, outputType);
}

/*	setting pin pull up/down	*/
void GPIO_voidSetPinPull(GPIO_PortName_t port, u8 pin, GPIO_Pull_t pull)
{
	EDT_REG(GPIO[port]->PUPDR, 2*pin, pull, 2);
}

void GPIO_voidSetAlternateFunction(GPIO_PortName_t port, u8 pin, GPIO_AlternateFunction_t af)
{
	EDT_REG(GPIO[port]->AFR, 4*pin, af, 4);
}

#endif

/*	setting pin output speed	*/
void GPIO_voidSetPinOutputSpeed(GPIO_PortName_t port, u8 pin, GPIO_OutputSpeed_t outputSpeed)
{
	#if TARGET_ID == STM32F401x
	EDT_REG(GPIO[port]->OSPEEDR, 2*pin, outputSpeed, 2);
	#elif TARGET_ID == STM32F10x
	EDT_REG(GPIO[port]->CR, 4*pin, outputSpeed, 2);
	#endif
}

/*	reading pin input level	*/
b8 GPIO_b8ReadPinDigital(GPIO_PortName_t port, u8 pin)
{
	return GET_BIT(GPIO[port]->IDR, pin);
}

/*	setting pin output level	*/
void GPIO_voidSetPinOutputLevel(GPIO_PortName_t port, u8 pin, GPIO_OutputLevel_t level)
{
	#if TARGET_ID == STM32F401x
	if (level == GPIO_OutputLevel_High)
		GPIO[port]->BSRRL = 1<<pin;
	else
		GPIO[port]->BSRRH = 1<<pin;

	#elif TARGET_ID == STM32F10x
	if (level == GPIO_OutputLevel_High)
	{
		GPIO[port]->BSRR = 1u<<pin;
	}
	else
	{
		GPIO[port]->BSRR = 1u<<(pin+16);
	}
	#endif
}

/*	toggling pin output level	*/
void GPIO_voidTogglePinOutputLevel(GPIO_PortName_t port, u8 pin)
{
	TGL_BIT(GPIO[port]->ODR, pin);
}

/*	setting pin configuration lock 	*/
void GPIO_voidLockPinConfiguration(GPIO_PortName_t port, u8 pin)
{
	u32 tempLCKR = (1<<16) | (1<<pin);
	GPIO[port]->LCKR = tempLCKR;
	GPIO[port]->LCKR = 1<<pin;
	GPIO[port]->LCKR = tempLCKR;
	(void)(GPIO[port]->LCKR);
}

#if TARGET_ID == STM32F10x
/*
 * assigns a pin to the EXTI line.
 *
 * pin number is same as EXTI line number
 * (forced by HW).
 *
 * port is selectable.
 */
void AFIO_voidSetExtiLineSrc(u8 line, GPIO_PortName_t port)
{
	EDT_REG(AFIO->EXTICR[line/4], 4*(line%4), port, 4);
}

void AFIO_voidRemap(AFIO_Peripheral_t peripheral, u8 map)
{
	const u8 startBitArr[] = {
		0, 1, 2, 3, 4, 6, 8, 10, 12, 13,
		15, 16, 17, 18, 19, 20, 24, 27
	};

	u8 startBit = startBitArr[peripheral];

	u8 len;
	/*	one odd case	*/
	if (peripheral == AFIO_Peripheral_ADC2_ETRGREG)
		len = 1;
	/*	other normal cases	*/
	else
		len = startBitArr[peripheral+1] - startBit;

	EDT_REG(AFIO->MAPR, startBit, map, len);
}

#endif








