/*
 * Shift_Register_program.c
 *
 *  Created on:	Sep 10, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include <stdlib.h>

#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "Delay_interface.h"

#include "Shift_Register_interface.h"

void SHIFT_REGISTER_voidInitSIPO(
	SIPO_SHIFT_REGISTER_t* regPtr,
	DIO_PIN _clkPin, DIO_PIN _dataInPin, DIO_PIN _latchPin,
	CLK_TRIG_EDGE_t _clkTrigEdge, DIO_OUTPUTTYPE _latchActiveType,
	u32 maxClkFrequency, u32 _dataInStableTimeNanoSec, u32 latchStableTimeNanoSec
	)
{
	regPtr->clkPin = _clkPin % 16;
	regPtr->clkPort = _clkPin / 16;

	regPtr->dataInPin =  _dataInPin % 16;
	regPtr->dataInPort = _dataInPin / 16;

	regPtr->latchPin =  _latchPin % 16;
	regPtr->latchPort = _latchPin / 16;

	DIO_voidSetPinMode(			regPtr->clkPort, regPtr->clkPin, DIO_MODE_GPO);
	DIO_voidSetPinOutputType(	regPtr->clkPort, regPtr->clkPin, DIO_OUTPUTTYPE_pushPull);
	DIO_voidSetPinOutputSpeed(	regPtr->clkPort, regPtr->clkPin, DIO_OUTPUTSPEED_veryHigh);

	DIO_voidSetPinMode(			regPtr->dataInPort, regPtr->dataInPin, DIO_MODE_GPO);
	DIO_voidSetPinOutputType(	regPtr->dataInPort, regPtr->dataInPin, DIO_OUTPUTTYPE_pushPull);
	DIO_voidSetPinOutputSpeed(	regPtr->dataInPort, regPtr->dataInPin, DIO_OUTPUTSPEED_veryHigh);

	DIO_voidSetPinMode(			regPtr->latchPort, regPtr->latchPin, DIO_MODE_GPO);
	DIO_voidSetPinOutputType(	regPtr->latchPort, regPtr->latchPin, DIO_OUTPUTTYPE_pushPull);
	DIO_voidSetPinOutputSpeed(	regPtr->latchPort, regPtr->latchPin, DIO_OUTPUTSPEED_veryHigh);

	regPtr->clkTrigEdge = _clkTrigEdge;

	regPtr->latchActiveType = _latchActiveType;

	regPtr->minClkTicks = RCC_u32GetSysClk() / maxClkFrequency;

	regPtr->minDataInTicks = (u64)RCC_u32GetSysClk() * (u64)_dataInStableTimeNanoSec / 1000000000ull;

	regPtr->minLatchTicks = (u64)RCC_u32GetSysClk() * (u64)latchStableTimeNanoSec / 1000000000ull;
}

void SHIFT_REGISTER_voidWriteInputSIPO(SIPO_SHIFT_REGISTER_t* regPtr, b8 dataBit)
{
	DIO_OUTPUTLEVEL level =
		dataBit ?
			DIO_OUTPUTLEVEL_high : DIO_OUTPUTLEVEL_low;

	DIO_voidSetPinOutputLevel(regPtr->dataInPort, regPtr->dataInPin, level);
}

void SHIFT_REGISTER_voidClkDisableSIPO(SIPO_SHIFT_REGISTER_t* regPtr)
{
	DIO_OUTPUTLEVEL level =
		(regPtr->clkTrigEdge == CLK_TRIG_EDGE_rising) ?
			DIO_OUTPUTLEVEL_low : DIO_OUTPUTLEVEL_high;

	DIO_voidSetPinOutputLevel(regPtr->clkPort, regPtr->clkPin, level);
}

void SHIFT_REGISTER_voidClkEnableSIPO(SIPO_SHIFT_REGISTER_t* regPtr)
{
	DIO_OUTPUTLEVEL level =
		(regPtr->clkTrigEdge == CLK_TRIG_EDGE_rising) ?
			DIO_OUTPUTLEVEL_high : DIO_OUTPUTLEVEL_low;

	DIO_voidSetPinOutputLevel(regPtr->clkPort, regPtr->clkPin, level);
}

void SHIFT_REGISTER_voidClkSIPO(SIPO_SHIFT_REGISTER_t* regPtr)
{
	SHIFT_REGISTER_voidClkDisableSIPO(regPtr);
	Delay_voidBlockingDelayTicks(regPtr->minClkTicks);
	SHIFT_REGISTER_voidClkEnableSIPO(regPtr);
	Delay_voidBlockingDelayTicks(regPtr->minClkTicks);
}

void SHIFT_REGISTER_voidEnableLatchSIPO(SIPO_SHIFT_REGISTER_t* regPtr)
{
	DIO_voidSetPinOutputLevel(regPtr->latchPort, regPtr->latchPin, regPtr->latchActiveType);
}

void SHIFT_REGISTER_voidDisableLatchSIPO(SIPO_SHIFT_REGISTER_t* regPtr)
{
	DIO_voidSetPinOutputLevel(regPtr->latchPort, regPtr->latchPin, !(regPtr->latchActiveType));
}

void SHIFT_REGISTER_voidLatchSIPO(SIPO_SHIFT_REGISTER_t* regPtr)
{
	SHIFT_REGISTER_voidEnableLatchSIPO(regPtr);
	delayTicks(regPtr->minLatchTicks);
	SHIFT_REGISTER_voidDisableLatchSIPO(regPtr);
}

void SHIFT_REGISTER_voidShiftSIPO(SIPO_SHIFT_REGISTER_t* regPtr, b8 dataBit)
{
	/*	setting input pin value	*/
	SHIFT_REGISTER_voidWriteInputSIPO(regPtr, dataBit);

	delayTicks(regPtr->minDataInTicks);

	/*	clock	*/
	SHIFT_REGISTER_voidClkSIPO(regPtr);/////////////////////////////////this is not the time
	////////////////////////////////////////////////////////////// clk frequency and clk active time are not the same!
}

void SHIFT_REGISTER_voidLoadByteArrSIPO(SIPO_SHIFT_REGISTER_t* regPtr, u8 byteArr[], u16 numberOfBitsToBeLoaded)
{
	while(numberOfBitsToBeLoaded--)
	{
		u8 byte = byteArr[numberOfBitsToBeLoaded / 8];
		SHIFT_REGISTER_voidShiftSIPO(regPtr, GET_BIT(byte, numberOfBitsToBeLoaded % 8));
	}
}

void SHIFT_REGISTER_voidLoadFromU32SIPO(SIPO_SHIFT_REGISTER_t* regPtr, u32 val, u8 numberOfBitsToBeLoaded)
{
	while(numberOfBitsToBeLoaded--)
	{
		SHIFT_REGISTER_voidShiftSIPO(regPtr, GET_BIT(val, numberOfBitsToBeLoaded));
	}
}













