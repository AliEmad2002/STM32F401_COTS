/*
 * GPIO_interface.h
 *
 *  Created on:	Jul 27, 2022
 *  Author:		Ali Emad Ali
 *
 *  References:
 *  	STM RM0368:	8.	General-purpose I/Os (GPIO)
 */

#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_

typedef enum{
	/*	PORTA pins	*/
	DIO_PIN_A0 	= 0,
	DIO_PIN_A1 	= 1,
	DIO_PIN_A2 	= 2,
	DIO_PIN_A3 	= 3,
	DIO_PIN_A4 	= 4,
	DIO_PIN_A5 	= 5,
	DIO_PIN_A6 	= 6,
	DIO_PIN_A7 	= 7,
	DIO_PIN_A8 	= 8,
	DIO_PIN_A9 	= 9,
	DIO_PIN_A10 = 10,
	DIO_PIN_A11 = 11,
	DIO_PIN_A12 = 12,
	DIO_PIN_A13 = 13,
	DIO_PIN_A14 = 14,
	DIO_PIN_A15 = 15,
	/*	~~~~~~~~~~~~	*/

	/*	PORTB pins	*/
	DIO_PIN_B0 	= 16 + 0,
	DIO_PIN_B1 	= 16 + 1,
	DIO_PIN_B2 	= 16 + 2,
	DIO_PIN_B3 	= 16 + 3,
	DIO_PIN_B4 	= 16 + 4,
	DIO_PIN_B5 	= 16 + 5,
	DIO_PIN_B6 	= 16 + 6,
	DIO_PIN_B7 	= 16 + 7,
	DIO_PIN_B8 	= 16 + 8,
	DIO_PIN_B9 	= 16 + 9,
	DIO_PIN_B10 = 16 + 10,
	DIO_PIN_B11 = 16 + 11,
	DIO_PIN_B12 = 16 + 12,
	DIO_PIN_B13 = 16 + 13,
	DIO_PIN_B14 = 16 + 14,
	DIO_PIN_B15 = 16 + 15,
	/*	~~~~~~~~~~~~	*/

	/*	PORTC pins	*/
	DIO_PIN_C0 	= 16*2 + 0,
	DIO_PIN_C1 	= 16*2 + 1,
	DIO_PIN_C2 	= 16*2 + 2,
	DIO_PIN_C3 	= 16*2 + 3,
	DIO_PIN_C4 	= 16*2 + 4,
	DIO_PIN_C5 	= 16*2 + 5,
	DIO_PIN_C6 	= 16*2 + 6,
	DIO_PIN_C7 	= 16*2 + 7,
	DIO_PIN_C8 	= 16*2 + 8,
	DIO_PIN_C9 	= 16*2 + 9,
	DIO_PIN_C10 = 16*2 + 10,
	DIO_PIN_C11 = 16*2 + 11,
	DIO_PIN_C12 = 16*2 + 12,
	DIO_PIN_C13 = 16*2 + 13,
	DIO_PIN_C14 = 16*2 + 14,
	DIO_PIN_C15 = 16*2 + 15
	/*	~~~~~~~~~~~~	*/
}DIO_PIN;

typedef enum{
	DIO_PORT_A = 0,
	DIO_PORT_B = 1,
	DIO_PORT_C = 2
}DIO_PORT;

typedef enum{
	DIO_MODE_input	= 0,
	DIO_MODE_GPO	= 1,
	DIO_MODE_AF		= 2,
	DIO_MODE_analog	= 3
}DIO_MODE;

typedef enum{
	DIO_OUTPUTTYPE_pushPull		= 0,
	DIO_OUTPUTTYPE_openDrain	= 1
}DIO_OUTPUTTYPE;

typedef enum{
	DIO_OUTPUTSPEED_low			= 0,
	DIO_OUTPUTSPEED_medium		= 1,
	DIO_OUTPUTSPEED_high		= 2,
	DIO_OUTPUTSPEED_veryHigh	= 3
}DIO_OUTPUTSPEED;

typedef enum{
	DIO_PULL_noPull	= 0,
	DIO_PULL_up		= 1,
	DIO_PULL_down	= 2
}DIO_PULL;

typedef enum{
	DIO_OUTPUTLEVEL_low		= 0,
	DIO_OUTPUTLEVEL_high	= 1
}DIO_OUTPUTLEVEL;


/*	setting pin mode	*/
void DIO_voidSetPinMode(DIO_PORT port, u8 pin, DIO_MODE mode);
/*	no need to input "port", but more iterations!	*/
void DIO_voidSetPinModeHeavy(DIO_PIN pin, DIO_MODE mode);
/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

/*	setting pin output type	*/
void DIO_voidSetPinOutputType(DIO_PORT port, u8 pin, DIO_OUTPUTTYPE outputType);
/*	no need to input "port", but more iterations!	*/
void DIO_voidSetPinOutputTypeHeavy(DIO_PIN pin, DIO_OUTPUTTYPE outputType);
/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

/*	setting pin output speed	*/
void DIO_voidSetPinOutputSpeed(DIO_PORT port, u8 pin, DIO_OUTPUTSPEED outputSpeed);
/*	no need to input "port", but more iterations!	*/
void DIO_voidSetPinOutputSpeedHeavy(DIO_PIN pin, DIO_OUTPUTSPEED outputSpeed);
/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

/*	setting pin pull up/down	*/
void DIO_voidSetPinPull(DIO_PORT port, u8 pin, DIO_PULL pull);
/*	no need to input "port", but more iterations!	*/
void DIO_voidSetPinPullHeavy(DIO_PIN pin, DIO_PULL pull);
/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

/*	reading pin input level	*/
b8 DIO_b8ReadPinDigital(DIO_PORT port, u8 pin);
/*	no need to input "port", but more iterations!	*/
b8 DIO_b8ReadPinDigitalHeavy(DIO_PIN pin);
/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

/*	setting pin output level	*/
void DIO_voidSetPinOutputLevel(DIO_PORT port, u8 pin, DIO_OUTPUTLEVEL level);
/*	no need to input "port", but more iterations!	*/
void DIO_voidSetPinOutputLevelHeavy(DIO_PIN pin, DIO_OUTPUTLEVEL level);
/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/
#endif /* GPIO_INTERFACE_H_ */
