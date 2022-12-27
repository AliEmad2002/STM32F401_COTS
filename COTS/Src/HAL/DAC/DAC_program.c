/*
 * DAC_program.c
 *
 *  Created on:	Sep 22, 2022
 *  Author:		Ali Emad Ali
 *
 */

/*	LIB	*/
#include <stdlib.h>
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Target_config.h"

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
		dacPtr->shiftRegisterPtr = (SR_SerialInParallelOut_t*)malloc(sizeof(SR_SerialInParallelOut_t));
	}
	else
	{
		dacPtr->pinArr = (u8*)malloc(_resolution * sizeof(u8));
		dacPtr->portArr = (GPIO_PortName_t*)malloc(_resolution * sizeof(GPIO_PortName_t));
	}
}

/*	sets the SR of the DAC, it should be initially init.	*/
void DAC_voidSetShiftRegister(DAC_t* dacPtr, SR_SerialInParallelOut_t* _shiftRegisterPtr)
{
	/*	copy	*/
	SR_voidCopySerialInParallelOut(dacPtr->shiftRegisterPtr, _shiftRegisterPtr);
}

/*
 * sets pins of the DAC, and inits them all as Push-Pull outputs.
 * pin speed is user selectable before this.
 */
void DAC_voidSetPins(DAC_t* dacPtr, GPIO_Pin_t _pinArr[])
{
	for (u8 i = 0; i < dacPtr->resolution; i++)
	{
		dacPtr->pinArr[i] = _pinArr[i] % 16;
		dacPtr->portArr[i] = _pinArr[i] / 16;
		GPIO_voidSetPinGpoPushPull(dacPtr->portArr[i], dacPtr->pinArr[i]);
	}
}

/*	outputs a value on the DAC object	*/
void DAC_voidOut(DAC_t* dacPtr, u32 val)
{
	if (dacPtr->connection == DAC_Connection_ShiftRegister)
	{
		SR_voidLoadFromU32SerialInParallelOut(dacPtr->shiftRegisterPtr, val, dacPtr->resolution);
		SR_voidLatchSerialInParallelOut(dacPtr->shiftRegisterPtr);
	}
	else
	{
		for (u8 i = 0; i < dacPtr->resolution; i++)
		{
			GPIO_voidSetPinOutputLevel(dacPtr->portArr[i], dacPtr->pinArr[i], GET_BIT(val, i));
		}
	}
}
