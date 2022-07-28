/*
 * GPIO_program.h
 *
 *  Created on:	Jul 27, 2022
 *  Author:		Ali Emad Ali
 *
 *  References:
 *  	STM RM0368:	8.	General-purpose I/Os (GPIO)
 */


#include "LIB/BIT_MATH.h"
#include "LIB/STD_TYPES.h"

#include "MCAL/GPIO/GPIO_interface.h"
#include "MCAL/GPIO/GPIO_private.h"

b8 DIO_b8PinRangeCheck(DIO_PORT port, u8 pin)
{
	switch(port)
	{
	case DIO_PORT_A:
		if (pin > 15)	return false;	/*	wrong pin	*/
		break;
	case DIO_PORT_B:
		if (pin > 15)	return false;	/*	wrong pin	*/
		break;
	case DIO_PORT_C:
		if (pin > 3)	return false;	/*	wrong pin	*/
		break;
	default:
		return false;					/*	wrong port	*/
	}
	return true;						/*	passed the check	*/
}

void DIO_voidSetPinMode(DIO_PORT port, u8 pin, DIO_MODE mode)
{
	/*	Range check	*/
	if (!DIO_b8PinRangeCheck(port, pin))
	{
		/*	Pin does not exist	*/
		while(1);
	}
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/* Writing "mode" to the two corresponding bits on "pin",
	 * in the GPIO"port"_MODER register
	 */
	switch(port)
	{
	case DIO_PORT_A:
		EDT_REG(GPIOA_MODER, 2*pin, mode, 2);
		break;
	case DIO_PORT_B:
		EDT_REG(GPIOB_MODER, 2*pin, mode, 2);
		break;
	case DIO_PORT_C:
		EDT_REG(GPIOC_MODER, 2*pin, mode, 2);
		break;
	}
}

void DIO_voidSetPinModeHeavy(DIO_PIN pin, DIO_MODE mode)
{
	/*	calculate port	*/
	DIO_PORT port = pin / 16;
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	write mode	*/
	DIO_voidSetPinMode(port, pin%16, mode);
}

void DIO_voidSetPinOutputType(DIO_PORT port, u8 pin, DIO_OUTPUTTYPE outputType)
{
	/*	Range check	*/
	if (!DIO_b8PinRangeCheck(port, pin))
	{
		/*	Pin does not exist	*/
		while(1);
	}
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	write "outputType"	*/
	switch(port)
	{
	case DIO_PORT_A:
		WRT_BIT(GPIOA_OTYPER, pin, outputType);
		break;
	case DIO_PORT_B:
		WRT_BIT(GPIOB_OTYPER, pin, outputType);
		break;
	case DIO_PORT_C:
		WRT_BIT(GPIOC_OTYPER, pin, outputType);
		break;
	}
}

void DIO_voidSetPinOutputTypeHeavy(DIO_PIN pin, DIO_OUTPUTTYPE outputType)
{
	/*	calculate port	*/
	DIO_PORT port = pin / 16;
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	write "outputType"	*/
	DIO_voidSetPinOutputType(port, pin%16, outputType);
}

void DIO_voidSetPinOutputSpeed(DIO_PORT port, u8 pin, DIO_OUTPUTSPEED outputSpeed)
{
	/*	Range check	*/
	if (!DIO_b8PinRangeCheck(port, pin))
	{
		/*	Pin does not exist	*/
		while(1);
	}
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	write "outputSpeed"	*/
	switch(port)
	{
	case DIO_PORT_A:
		EDT_REG(GPIOA_OSPEEDR, 2*pin, outputSpeed, 2);
		break;
	case DIO_PORT_B:
		EDT_REG(GPIOB_OSPEEDR, 2*pin, outputSpeed, 2);
		break;
	case DIO_PORT_C:
		EDT_REG(GPIOC_OSPEEDR, 2*pin, outputSpeed, 2);
		break;
	}
}

void DIO_voidSetPinOutputSpeedHeavy(DIO_PIN pin, DIO_OUTPUTSPEED outputSpeed)
{
	/*	calculate port	*/
	DIO_PORT port = pin / 16;
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	write "outputSpeed"	*/
	DIO_voidSetPinOutputSpeed(port, pin%16, outputSpeed);
}

void DIO_voidSetPinPull(DIO_PORT port, u8 pin, DIO_PULL pull)
{
	/*	Range check	*/
	if (!DIO_b8PinRangeCheck(port, pin))
	{
		/*	Pin does not exist	*/
		while(1);
	}
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	write "pull"	*/
	switch(port)
	{
	case DIO_PORT_A:
		EDT_REG(GPIOA_PUPDR, 2*pin, pull, 2);
		break;
	case DIO_PORT_B:
		EDT_REG(GPIOB_PUPDR, 2*pin, pull, 2);
		break;
	case DIO_PORT_C:
		EDT_REG(GPIOC_PUPDR, 2*pin, pull, 2);
		break;
	}
}

void DIO_voidSetPinPullHeavy(DIO_PIN pin, DIO_PULL pull)
{
	/*	calculate port	*/
	DIO_PORT port = pin / 16;
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	write "pull"	*/
	DIO_voidSetPinPull(port, pin%16, pull);
}

b8 DIO_b8ReadPinDigital(DIO_PORT port, u8 pin)
{
	/*	Range check	*/
	if (!DIO_b8PinRangeCheck(port, pin))
	{
		/*	Pin does not exist	*/
		while(1);
	}
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	read IDR"pin"	*/
	switch(port)
	{
	case DIO_PORT_A:
		return GET_BIT(GPIOA_IDR, pin);
	case DIO_PORT_B:
		return GET_BIT(GPIOB_IDR, pin);
	case DIO_PORT_C:
		return GET_BIT(GPIOC_IDR, pin);
	}
	/*	foo return to avoid warning, it would never be reached any way	*/
	return false;
}

b8 DIO_b8ReadPinDigitalHeavy(DIO_PIN pin)
{
	/*	calculate port	*/
	DIO_PORT port = pin / 16;
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	read IDR"pin"	*/
	return DIO_b8ReadPinDigital(port, pin%16);
}

void DIO_voidSetPinOutputLevel(DIO_PORT port, u8 pin, DIO_OUTPUTLEVEL level)
{
	/*	Range check	*/
	if (!DIO_b8PinRangeCheck(port, pin))
	{
		/*	Pin does not exist	*/
		while(1);
	}
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	write "level"	*/
	switch(port)
	{
	case DIO_PORT_A:
		WRT_BIT(GPIOA_ODR, pin, level);
		break;
	case DIO_PORT_B:
		WRT_BIT(GPIOB_ODR, pin, level);
		break;
	case DIO_PORT_C:
		WRT_BIT(GPIOC_ODR, pin, level);
		break;
	}
}

void DIO_voidSetPinOutputLevelHeavy(DIO_PIN pin, DIO_OUTPUTLEVEL level)
{
	/*	calculate port	*/
	DIO_PORT port = pin / 16;
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	write "level"	*/
	DIO_voidSetPinOutputLevel(port, pin%16, level);
}

















