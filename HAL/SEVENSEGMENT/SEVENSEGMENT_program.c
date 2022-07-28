/*
 * SEVENSEGMENT_program.c
 *
 *  Created on:	Jul 28, 2022
 *  Author:		Ali Emad
 */

#include "LIB/BIT_MATH.h"
#include "LIB/STD_TYPES.h"
#include "MCAL/GPIO/GPIO_interface.h"

#include "HAL/SEVENSEGMENT/SEVENSEGMENT_interface.h"

const u8 SEVENSEGMENT_PINSTATES[] = {
	0b00111111,	/*	0	*/
	0b00000110,	/*	1	*/
	0b01011011,	/*	2	*/
	0b01001111,	/*	3	*/
	0b01100110,	/*	4	*/
	0b01101101,	/*	5	*/
	0b01111101,	/*	6	*/
	0b00000111,	/*	7	*/
	0b01111111,	/*	8	*/
	0b01101111	/*	9	*/
};

void SEVENSEGMENT_voidInit(
	SEVENSEGMENT_t* ssPtr, SEVENSEGMENT_COMTYPE _comType,
	DIO_PIN pinA, DIO_PIN pinB, DIO_PIN pinC, DIO_PIN pinD,
	DIO_PIN pinE, DIO_PIN pinF, DIO_PIN pinG, DIO_PIN pinPoint,
	SEVENSEGMENT_POINTCONNECTION _pointConnection,
	DIO_OUTPUTSPEED pinSpeed
	)
{
	/*	calculate and store ports	*/
	ssPtr->portArr[0] = pinA / 16;
	ssPtr->portArr[1] = pinB / 16;
	ssPtr->portArr[2] = pinC / 16;
	ssPtr->portArr[3] = pinD / 16;
	ssPtr->portArr[4] = pinE / 16;
	ssPtr->portArr[5] = pinF / 16;
	ssPtr->portArr[6] = pinG / 16;
	if (_pointConnection == SEVENSEGMENT_POINTCONNECTION_connected)
		ssPtr->portArr[7] = pinPoint / 16;
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	calculate and store pins	*/
	ssPtr->pinArr[0] = pinA % 16;
	ssPtr->pinArr[1] = pinB % 16;
	ssPtr->pinArr[2] = pinC % 16;
	ssPtr->pinArr[3] = pinD % 16;
	ssPtr->pinArr[4] = pinE % 16;
	ssPtr->pinArr[5] = pinF % 16;
	ssPtr->pinArr[6] = pinG % 16;
	if (_pointConnection == SEVENSEGMENT_POINTCONNECTION_connected)
		ssPtr->pinArr[7] = pinPoint % 16;
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	store other parameters	*/
	ssPtr->comType = _comType;
	ssPtr->pointConnection = _pointConnection;
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	init pins as push-pull outputs, with a speed of "pinSpeed"	*/
	for (u8 i=0; i<7; i++)
	{
		DIO_voidSetPinMode(ssPtr->portArr[i], ssPtr->pinArr[i], DIO_MODE_GPO);
		DIO_voidSetPinOutputType(ssPtr->portArr[i], ssPtr->pinArr[i], DIO_OUTPUTTYPE_pushPull);
		DIO_voidSetPinOutputSpeed(ssPtr->portArr[i], ssPtr->pinArr[i], pinSpeed);
	}
	if (_pointConnection == SEVENSEGMENT_POINTCONNECTION_connected)
	{
		DIO_voidSetPinMode(ssPtr->portArr[7], ssPtr->pinArr[7], DIO_MODE_GPO);
		DIO_voidSetPinOutputType(ssPtr->portArr[7], ssPtr->pinArr[7], DIO_OUTPUTTYPE_pushPull);
		DIO_voidSetPinOutputSpeed(ssPtr->portArr[7], ssPtr->pinArr[7], pinSpeed);
	}

	/*	initially turn the 7-segment element off	*/
	SEVENSEGMENT_voidTurnOff(ssPtr);
}

void SEVENSEGMENT_voidWrite(SEVENSEGMENT_t* ssPtr, u8 num, SEVENSEGMENT_POINTSTATE pointState)
{
	/*	range check	*/
	if (num > 9)
	{
		/*	wrong number	*/
		while(1);
	}
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	write number	*/
	u8 pinLevels = SEVENSEGMENT_PINSTATES[num];
	if (ssPtr->comType == SEVENSEGMENT_COMTYPE_anode)
		pinLevels = ~pinLevels;
	for (u8 i=0; i<7; i++)
		DIO_voidSetPinOutputLevel(ssPtr->portArr[i], ssPtr->pinArr[i], GET_BIT(pinLevels, i));
	if (ssPtr->pointConnection == SEVENSEGMENT_POINTCONNECTION_connected)
		DIO_voidSetPinOutputLevel(ssPtr->portArr[7], ssPtr->pinArr[7], GET_BIT(pinLevels, 7));
}

void SEVENSEGMENT_voidTurnOff(SEVENSEGMENT_t* ssPtr)
{
	DIO_OUTPUTLEVEL turnOffLevel = (ssPtr->comType == SEVENSEGMENT_COMTYPE_cathode) ? DIO_OUTPUTLEVEL_low : DIO_OUTPUTLEVEL_high;
	for (u8 i=0; i<7; i++)
		DIO_voidSetPinOutputLevel(ssPtr->portArr[i], ssPtr->pinArr[i], turnOffLevel);
	if (ssPtr->pointConnection == SEVENSEGMENT_POINTCONNECTION_connected)
		DIO_voidSetPinOutputLevel(ssPtr->portArr[7], ssPtr->pinArr[7], turnOffLevel);
}











