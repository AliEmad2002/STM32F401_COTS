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
	u8* rowPins;			GPIO_PortName_t* rowPorts;
	u8* colPins;			GPIO_PortName_t* colPorts;
	s8** buttons;
	u8 msDebouncingDelay;
}Keypad_t;

/*
 * inits keypad object.
 * sets pin modes.
 * pin speed is up to user select, out of this scope.
 */
void Keypad_voidInit(
		Keypad_t* keypadPtr, u8 _rowsNumber, u8 _colsNumber,
		GPIO_Pin_t _rowPins[], GPIO_Pin_t _colPins[], s8 _buttons[][_colsNumber],
		u32 _msDebouncingDelay
		);

/*
 * scans the keypad for a press.
 * if press was made, it returns press's key value,
 * else wise it returns '\0'.
 */
s8 Keypad_s8Scan(Keypad_t* keypadPtr);

/*	blocks until a key is pressed, returns the key value.	*/
s8 Keypad_s8GetPress(Keypad_t* keypad);

/*
 * blocks until either key is pressed, or time is out.
 * returns the key value, or '\0' if time was out.
 * sysTick tick measure should be enabled first.
 */
s8 Keypad_s8GetPressTimeOut(Keypad_t* keypadPtr, u32 ticks);

#endif /* KEYPAD_INTERFACE_H_ */
