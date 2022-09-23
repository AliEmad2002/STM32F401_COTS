/*
 * DAC_program.c
 *
 *  Created on:	Sep 22, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

/*	LIB	*/
#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include <stdlib.h>
/*	MCAL	*/
#include "GPIO_interface.h"
/*	HAL	*/
#include "Shift_Register_interface.h"

/*	SELF	*/
#include "DAC_interface.h"

/*	allocates memory needed for the DAC object	*/
void DAC_voidAlloc(DAC_t* dacPtr, DAC_Connection_t _connection, u8 _resolution)
{
	dacPtr->connection = _connection;
	dacPtr->resolution = _resolution;

	if (_connection == DAC_Connection_ShiftRegister)
	{
		dacPtr->shiftRegisterPtr = (SIPO_SHIFT_REGISTER_t*)malloc(sizeof(SIPO_SHIFT_REGISTER_t));
	}
	else
	{
		dacPtr->pinArr = (u8*)malloc(_resolution * sizeof(u8));
		dacPtr->portArr = (DIO_PORT*)malloc(_resolution * sizeof(DIO_PORT));
	}
}

/*	sets the SR of the DAC, it should be initially init.	*/
void DAC_voidSetShiftRegister(DAC_t* dacPtr, SIPO_SHIFT_REGISTER_t* _shiftRegisterPtr)
{
	/*	copy	*/
	SHIFT_REGISTER_voidCopy(dacPtr->shiftRegisterPtr, _shiftRegisterPtr);
}

/*
 * sets pins of the DAC, and inits them all as Push-Pull outputs
 * with the desired speed.
 */
void DAC_voidSetPins(DAC_t* dacPtr, DIO_PIN _pinArr[], DIO_OUTPUTSPEED speed)
{
	for (u8 i = 0; i < dacPtr->resolution; i++)
	{
		dacPtr->pinArr[i] = _pinArr[i] % 16;
		dacPtr->portArr[i] = _pinArr[i] / 16;
		DIO_voidSetPinMode(dacPtr->portArr[i], dacPtr->pinArr[i], DIO_MODE_GPO);
		DIO_voidSetPinOutputType(dacPtr->portArr[i], dacPtr->pinArr[i], DIO_OUTPUTTYPE_pushPull);
		DIO_voidSetPinOutputSpeed(dacPtr->portArr[i], dacPtr->pinArr[i], speed);
	}
}

/*	outputs a value on the DAC object	*/
void DAC_voidOut(DAC_t* dacPtr, u32 val)
{
	if (dacPtr->connection == DAC_Connection_ShiftRegister)
	{
		SHIFT_REGISTER_voidLoadFromU32SIPO(dacPtr->shiftRegisterPtr, val, dacPtr->resolution);
		SHIFT_REGISTER_voidLatchSIPO(dacPtr->shiftRegisterPtr);
	}
	else
	{
		for (u8 i = 0; i < dacPtr->resolution; i++)
		{
			DIO_voidSetPinOutputLevel(dacPtr->portArr[i], dacPtr->pinArr[i], GET_BIT(val, i));
		}
	}
}
