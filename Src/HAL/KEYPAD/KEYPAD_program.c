/*
 * KEYPAD_program.c
 *
 *  Created on:	Aug 3, 2022
 *  Author:		Ali Emad Ali
 *	
 *	References:
 *		* https://arduinogetstarted.com/tutorials/arduino-keypad
 *
 */


#include "LIB/STD_TYPES.h"
#include "LIB/BIT_MATH.h"
#include "MCAL/GPIO/GPIO_interface.h"
#include <stdlib.h>
#include "Delay_interface.h"

#include "HAL/KEYPAD/KEYPAD_interface.h"


void KEYPAD_voidInit(
		KEYPAD_t* keypadPtr, u8 _rowsNumber, u8 _colsNumber,
		DIO_PIN _rowPins[], DIO_PIN _colPins[], s8 _buttons[][_colsNumber],
		u32 _msDebouncingDelay, DIO_OUTPUTSPEED outputSpeed
		)
{
	// allocate pins and ports and buttons arrays:
	keypadPtr->rowPins = malloc(_rowsNumber * sizeof(u8));
	keypadPtr->rowPorts = malloc(_rowsNumber * sizeof(DIO_PORT));

	keypadPtr->colPins = malloc(_colsNumber * sizeof(u8));
	keypadPtr->colPorts = malloc(_colsNumber * sizeof(DIO_PORT));

	keypadPtr->buttons = malloc(_rowsNumber * sizeof(s8*));

	for (u8 i=0; i<_rowsNumber; i++)
	{
		// fill rows pins and ports:
		keypadPtr->rowPorts[i] = _rowPins[i]/16;
		keypadPtr->rowPins[i] = _rowPins[i]%16;

		// init row pins as outputs:
		DIO_voidSetPinMode(keypadPtr->rowPorts[i], keypadPtr->rowPins[i], DIO_MODE_GPO);
		DIO_voidSetPinOutputType(keypadPtr->rowPorts[i], keypadPtr->rowPins[i], DIO_OUTPUTTYPE_openDrain);
		DIO_voidSetPinOutputSpeed(keypadPtr->rowPorts[i], keypadPtr->rowPins[i], outputSpeed);
		// set them float:
		DIO_voidSetPinOutputLevel(keypadPtr->rowPorts[i], keypadPtr->rowPins[i], DIO_OUTPUTLEVEL_openDrainFloat);
		/**
		 * using open drain to avoid short circuit in case user pushes 2 or more buttons in the same column
		 */


		// allocate i_th row of buttons:
		keypadPtr->buttons[i] = malloc(_colsNumber * sizeof(s8));
		// fill i_th row of buttons:
		for (u8 j=0; j<_colsNumber; j++)
			keypadPtr->buttons[i][j] = _buttons[i][j];
	}

	for (u8 i=0; i<_colsNumber; i++)
	{
		// fill cols pins and ports:
		keypadPtr->colPorts[i] = _colPins[i]/16;
		keypadPtr->colPins[i] = _colPins[i]%16;

		// init col pins as inputs:
		DIO_voidSetPinMode(keypadPtr->colPorts[i], keypadPtr->colPins[i], DIO_MODE_input);
		// pull them up:
		DIO_voidSetPinPull(keypadPtr->colPorts[i], keypadPtr->colPins[i], DIO_PULL_up);
	}

	// store other data in keypad object:
	keypadPtr->rowsNumber = _rowsNumber;
	keypadPtr->colsNumber = _colsNumber;

	keypadPtr->msDebouncingDelay = _msDebouncingDelay;
}

s8 KEYPAD_s8GetPress(KEYPAD_t* keypadPtr)
{
	while(1)
		{
			/*	for every row in the keypad:	*/
			for (u8 row=0; row < keypadPtr->rowsNumber; row++)
			{
				/*	clear the row LOW	*/
				DIO_voidSetPinOutputLevel(keypadPtr->rowPorts[row], keypadPtr->rowPins[row], DIO_OUTPUTLEVEL_low);
				/*	for every column in the keypad:	*/
				for (u8 col=0; col < keypadPtr->colsNumber; col++)
				{
					/*	if a column reads LOW level, this is the one that just got connected	*/
					/*	to the lastly cleared LOW row pin.										*/
					if (DIO_b8ReadPinDigital(keypadPtr->colPorts[col], keypadPtr->colPins[col]) == false)
					{
						/*	set the row high	*/
						DIO_voidSetPinOutputLevel(keypadPtr->rowPorts[row], keypadPtr->rowPins[row], DIO_OUTPUTLEVEL_high);
						/*	denouncing delay	*/
						Delay_voidBlockingDelayMs(keypadPtr->msDebouncingDelay);
						/*	return the corresponding button value	*/
						return keypadPtr->buttons[row][col];
					}
				}
				/*	set the row HHIGH, thus it does not interfere with the next one	*/
				/*	or the next press- if any -										*/
				DIO_voidSetPinOutputLevel(keypadPtr->rowPorts[row], keypadPtr->rowPins[row], DIO_OUTPUTLEVEL_high);
			}
		}
}
