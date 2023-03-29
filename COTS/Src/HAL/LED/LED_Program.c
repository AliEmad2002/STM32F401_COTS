/*
 * LED_Program.c
 *
 *  Created on: Mar 24, 2023
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"

/*	MCAL	*/
#include "GPIO_interface.h"

/*	SELF	*/
#include "LED_Interface.h"


/*******************************************************************************
 * Init:
 ******************************************************************************/
void LED_voidInit(
	LED_t* ledPtr, GPIO_PortName_t port, u8 pin, GPIO_OutputLevel_t activeLevel)
{
	ledPtr->port = port;
	ledPtr->pin = pin;
	ledPtr->activeLevel = activeLevel;

	GPIO_voidSetPinGpoPushPull(port, pin);

	GPIO_voidSetPinOutputLevel(port, pin, !activeLevel);
}

/*******************************************************************************
 * Operation:
 ******************************************************************************/
void LED_voidSetActive(LED_t* ledPtr)
{
	GPIO_voidSetPinOutputLevel(ledPtr->port, ledPtr->pin, ledPtr->activeLevel);
}

void LED_voidSetInactive(LED_t* ledPtr)
{
	GPIO_voidSetPinOutputLevel(ledPtr->port, ledPtr->pin, !ledPtr->activeLevel);
}

void LED_voidToggle(LED_t* ledPtr)
{
	GPIO_voidTogglePinOutputLevel(ledPtr->port, ledPtr->pin);
}



















