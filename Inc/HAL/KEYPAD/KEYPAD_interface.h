/*
 * KEYPAD_interface.h
 *
 *  Created on:	Aug 3, 2022
 *  Author:		Ali Emad Ali
 *	
 *	References:
 *		* https://arduinogetstarted.com/tutorials/arduino-keypad
 *
 */

#ifndef KEYPAD_INTERFACE_H_
#define KEYPAD_INTERFACE_H_


typedef struct{
	u8 rowsNumber;
	u8 colsNumber;
	u8* rowPins;			DIO_PORT* rowPorts;
	u8* colPins;			DIO_PORT* colPorts;
	s8** buttons;
	u8 msDebouncingDelay;
}KEYPAD_t;

void KEYPAD_voidInit(
		KEYPAD_t* keypadPtr, u8 _rowsNumber, u8 _colsNumber,
		DIO_PIN _rowPins[], DIO_PIN _colPins[], s8 _buttons[][_colsNumber],
		u32 _msDebouncingDelay, DIO_OUTPUTSPEED outputSpeed
		);

s8 KEYPAD_s8GetPress(KEYPAD_t* keypad);

#endif /* KEYPAD_INTERFACE_H_ */
