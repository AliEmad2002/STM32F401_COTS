/*
 * Seven_Segment_program.c
 *
 *  Created on:	Jul 28, 2022
 *  Author:		Ali Emad
 */

/*	LIB	*/
#include <stdlib.h>
#include "Std_Types.h"
#include "Bit_Math.h"

/*	MCAL	*/
#include "GPIO_interface.h"

/*	SELF	*/
#include "Seven_Segment_interface.h"



/*
 * assigns pins to seven-segment object, and inits them as GPO push pull.
 * pin speed is previously user selectable.
 */
void Seven_Segment_voidInit(
	Seven_Segment_t* ssPtr, Seven_Segment_ComType_t _comType,
	GPIO_Pin_t pinArr[], Seven_Segment_PointConnection_t _pointConnection
	)
{
	/*	calculate and store pins and ports and init pins	*/
	if (_pointConnection == Seven_Segment_PointConnection_connected)
	{
		ssPtr->portArr = malloc(8 * sizeof(GPIO_PortName_t));
		ssPtr->pinArr = malloc(8 * sizeof(u8));

		ssPtr->portArr[7] = pinArr[7] / 16;
		ssPtr->pinArr[7] = pinArr[7] % 16;

		GPIO_voidSetPinGpoPushPull(ssPtr->portArr[7], ssPtr->pinArr[7]);
	}
	else
	{
		ssPtr->portArr = malloc(7 * sizeof(GPIO_PortName_t));
		ssPtr->pinArr = malloc(7 * sizeof(u8));
	}

	for (u8 i=0; i<7; i++)
	{
		ssPtr->portArr[i] = pinArr[i] / 16;
		ssPtr->pinArr[i] = pinArr[i] % 16;

		GPIO_voidSetPinGpoPushPull(ssPtr->portArr[i], ssPtr->pinArr[i]);
	}
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	store other parameters	*/
	ssPtr->comType = _comType;
	ssPtr->pointConnection = _pointConnection;
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	initially turn the 7-segment element off	*/
	Seven_Segment_voidTurnOff(ssPtr);
}

/*	writes a number on seven-segment object. point state is selectable - if connected -.	*/
void Seven_Segment_voidWrite(Seven_Segment_t* ssPtr, u8 num, Seven_Segment_PointState_t pointState)
{
	const u8 pinStates[] = {
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

	/*	write number	*/
	u8 pinLevels = pinStates[num];

	if (ssPtr->comType == Seven_Segment_ComType_Anode)
		pinLevels = ~pinLevels;

	for (u8 i=0; i<7; i++)
		GPIO_voidSetPinOutputLevel(ssPtr->portArr[i], ssPtr->pinArr[i], GET_BIT(pinLevels, i));

	if (ssPtr->pointConnection == Seven_Segment_PointConnection_connected)
		GPIO_voidSetPinOutputLevel(ssPtr->portArr[7], ssPtr->pinArr[7], (GPIO_OutputLevel_t)pointState);
}

/*	turns off all segments	*/
void Seven_Segment_voidTurnOff(Seven_Segment_t* ssPtr)
{
	GPIO_OutputLevel_t turnOffLevel =
		(ssPtr->comType == Seven_Segment_ComType_Cathode) ?
			GPIO_OutputLevel_Low : GPIO_OutputLevel_High;

	for (u8 i=0; i<7; i++)
		GPIO_voidSetPinOutputLevel(ssPtr->portArr[i], ssPtr->pinArr[i], turnOffLevel);

	if (ssPtr->pointConnection == Seven_Segment_PointConnection_connected)
		GPIO_voidSetPinOutputLevel(ssPtr->portArr[7], ssPtr->pinArr[7], turnOffLevel);
}











