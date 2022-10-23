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

/*	LIB	*/
#include <stdlib.h>
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Delay_interface.h"

/*	MCAL	*/
#include "GPIO_interface.h"
#include "STK_interface.h"

/*	SELF	*/
#include "KEYPAD_interface.h"


/*
 * inits keypad object.
 * sets pin modes.
 * pin speed is up to user select, out of this scope.
 */
void Keypad_voidInit(
		Keypad_t* keypadPtr, u8 _rowsNumber, u8 _colsNumber,
		GPIO_Pin_t _rowPins[], GPIO_Pin_t _colPins[], s8 _buttons[][_colsNumber],
		u32 _msDebouncingDelay
		)
{
	// allocate pins and ports and buttons arrays:
	keypadPtr->rowPins = malloc(_rowsNumber * sizeof(u8));
	keypadPtr->rowPorts = malloc(_rowsNumber * sizeof(GPIO_PortName_t));

	keypadPtr->colPins = malloc(_colsNumber * sizeof(u8));
	keypadPtr->colPorts = malloc(_colsNumber * sizeof(GPIO_PortName_t));

	keypadPtr->buttons = malloc(_rowsNumber * sizeof(s8*));

	for (u8 i=0; i<_rowsNumber; i++)
	{
		// fill rows pins and ports:
		keypadPtr->rowPorts[i] = _rowPins[i]/16;
		keypadPtr->rowPins[i] = _rowPins[i]%16;

		/*
		 * using open drain to avoid short circuit in case user
		 * pushes 2 or more buttons in the same column.
		 * init row pins as open drain outputs:
		 */
		GPIO_voidSetPinGpoOpenDrain(keypadPtr->rowPorts[i], keypadPtr->rowPins[i]);
		// set them float:
		GPIO_voidSetPinOutputLevel(keypadPtr->rowPorts[i], keypadPtr->rowPins[i], GPIO_OutputLevel_OpenDrainFloat);

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

		// init col pins as pulled up inputs inputs:
		GPIO_voidSetPinInputPullUp(keypadPtr->colPorts[i], keypadPtr->colPins[i]);
	}

	// store other data in keypad object:
	keypadPtr->rowsNumber = _rowsNumber;
	keypadPtr->colsNumber = _colsNumber;

	keypadPtr->msDebouncingDelay = _msDebouncingDelay;
}

/*
 * scans the keypad for a press.
 * if press was made, it returns press's key value,
 * else wise it returns '\0'.
 */
s8 Keypad_s8Scan(Keypad_t* keypadPtr)
{
	/*	for every row in the keypad:	*/
	for (u8 row=0; row < keypadPtr->rowsNumber; row++)
	{
		/*	clear the row LOW	*/
		GPIO_voidSetPinOutputLevel(keypadPtr->rowPorts[row], keypadPtr->rowPins[row], GPIO_OutputLevel_Low);
		/*	for every column in the keypad:	*/
		for (u8 col=0; col < keypadPtr->colsNumber; col++)
		{
			/*	if a column reads LOW level, this is the one that just got connected	*/
			/*	to the lastly cleared LOW row pin.										*/
			if (GPIO_b8ReadPinDigital(keypadPtr->colPorts[col], keypadPtr->colPins[col]) == false)
			{
				/*	set the row high	*/
				GPIO_voidSetPinOutputLevel(keypadPtr->rowPorts[row], keypadPtr->rowPins[row], GPIO_OutputLevel_High);
				/*	denouncing delay	*/
				Delay_voidBlockingDelayMs(keypadPtr->msDebouncingDelay);
				/*	return the corresponding button value	*/
				return keypadPtr->buttons[row][col];
			}
		}
		/*	set the row HHIGH, thus it does not interfere with the next one	*/
		/*	or the next press- if any -										*/
		GPIO_voidSetPinOutputLevel(keypadPtr->rowPorts[row], keypadPtr->rowPins[row], GPIO_OutputLevel_High);
	}
	return '\0';
}

s8 Keypad_s8GetPress(Keypad_t* keypadPtr)
{
	s8 scanned;

	while(1)
		{
			scanned = Keypad_s8Scan(keypadPtr);

			if ( scanned != '\0')
				return scanned;
		}
}

s8 Keypad_s8GetPressTimeOut(Keypad_t* keypadPtr, u32 ticks)
{
	u64 timeStart = STK_u64GetElapsedTicks();
	u64 timeCurrent = timeStart;
	s8 scanned;

	while (timeCurrent - timeStart < ticks)
	{
		scanned = Keypad_s8Scan(keypadPtr);

		if ( scanned != '\0')
			return scanned;

		timeCurrent = STK_u64GetElapsedTicks();
	}

	return '\0';
}












