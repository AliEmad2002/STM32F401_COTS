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

/*	LIB	*/
#include <stdlib.h>
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Delay_interface.h"

/*	MCAL	*/
#include "RCC_interface.h"
#include "GPIO_interface.h"

/*	SELF	*/
#include "Shift_Register_interface.h"

/*
 * initializes a serial input parallel output sift register.
 * GPIO speed is up to user to initially configure.
 */
void SR_voidInitSerialInParallelOut(
	SR_SerialInParallelOut_t* regPtr,
	GPIO_Pin_t _clkPin, GPIO_Pin_t _dataInPin, GPIO_Pin_t _latchPin,
	SR_ClockTriggeringEdge_t _clkTrigEdge, GPIO_OutputLevel_t _latchActiveType,
	u32 maxClkFrequency, u32 _dataInStableTimeNanoSec, u32 latchStableTimeNanoSec
	)
{
	regPtr->clkPin = _clkPin % 16;
	regPtr->clkPort = _clkPin / 16;

	regPtr->dataInPin =  _dataInPin % 16;
	regPtr->dataInPort = _dataInPin / 16;

	regPtr->latchPin =  _latchPin % 16;
	regPtr->latchPort = _latchPin / 16;

	GPIO_voidSetPinGpoPushPull(regPtr->clkPort, regPtr->clkPin);
	GPIO_voidSetPinGpoPushPull(regPtr->dataInPort, regPtr->dataInPin);
	GPIO_voidSetPinGpoPushPull(regPtr->latchPort, regPtr->latchPin);

	regPtr->clkTrigEdge = _clkTrigEdge;

	regPtr->latchActiveType = _latchActiveType;

	u32 ahbClk = RCC_u32GetBusClk(RCC_Bus_AHB);

	regPtr->minClkTicks = ahbClk / maxClkFrequency;

	regPtr->minDataInTicks = (u64)ahbClk * (u64)_dataInStableTimeNanoSec / 1000000000ull;

	regPtr->minLatchTicks = (u64)ahbClk * (u64)latchStableTimeNanoSec / 1000000000ull;
}

/*
 * writes the states of the serial input pin.
 */
void SR_voidWriteInputSerialInParallelOut(SR_SerialInParallelOut_t* regPtr, b8 dataBit)
{
	GPIO_OutputLevel_t level =
		dataBit ?
			GPIO_OutputLevel_High : GPIO_OutputLevel_Low;

	GPIO_voidSetPinOutputLevel(regPtr->dataInPort, regPtr->dataInPin, level);
}

/*
 * disables clock pin (writes it with a logic inverse to its active logic).
 */
void SR_voidClkDisableSerialInParallelOut(SR_SerialInParallelOut_t* regPtr)
{
	GPIO_OutputLevel_t level =
		(regPtr->clkTrigEdge == SR_ClockTriggeringEdge_rising) ?
				GPIO_OutputLevel_Low : GPIO_OutputLevel_High;

	GPIO_voidSetPinOutputLevel(regPtr->clkPort, regPtr->clkPin, level);
}

/*
 * enables clock pin (writes it with its active logic).
 */
void SR_voidClkEnableSerialInParallelOut(SR_SerialInParallelOut_t* regPtr)
{
	GPIO_OutputLevel_t level =
		(regPtr->clkTrigEdge == SR_ClockTriggeringEdge_rising) ?
				GPIO_OutputLevel_High : GPIO_OutputLevel_Low;

	GPIO_voidSetPinOutputLevel(regPtr->clkPort, regPtr->clkPin, level);
}

/*
 * triggers a clock pulse on the clock pin.
 */
void SR_voidClkSerialInParallelOut(SR_SerialInParallelOut_t* regPtr)
{
	SR_voidClkDisableSerialInParallelOut(regPtr);
	Delay_voidBlockingDelayTicks(regPtr->minClkTicks);
	SR_voidClkEnableSerialInParallelOut(regPtr);
	Delay_voidBlockingDelayTicks(regPtr->minClkTicks);
}

/*
 * enables latch.
 */
void SR_voidEnableLatchSerialInParallelOut(SR_SerialInParallelOut_t* regPtr)
{
	GPIO_voidSetPinOutputLevel(regPtr->latchPort, regPtr->latchPin, regPtr->latchActiveType);
}

/*
 * disables latch.
 */
void SR_voidDisableLatchSerialInParallelOut(SR_SerialInParallelOut_t* regPtr)
{
	GPIO_voidSetPinOutputLevel(regPtr->latchPort, regPtr->latchPin, !(regPtr->latchActiveType));
}

/*
 * latches the data in the shift register to the output pins (enables then disables it).
 */
void SR_voidLatchSerialInParallelOut(SR_SerialInParallelOut_t* regPtr)
{
	SR_voidEnableLatchSerialInParallelOut(regPtr);
	Delay_voidBlockingDelayTicks(regPtr->minLatchTicks);
	SR_voidDisableLatchSerialInParallelOut(regPtr);
}

/*
 * shifts the whole register by one, adding a new bit.
 * does not latch to output pins.
 */
void SR_voidShiftSerialInParallelOut(SR_SerialInParallelOut_t* regPtr, b8 dataBit)
{
	/*	setting input pin value	*/
	SR_voidWriteInputSerialInParallelOut(regPtr, dataBit);

	Delay_voidBlockingDelayTicks(regPtr->minDataInTicks);

	/*	clock	*/
	SR_voidClkSerialInParallelOut(regPtr);/////////////////////////////////this is not the time
	////////////////////////////////////////////////////////////// clk frequency and clk active time are not the same!
}

/*
 * shifts the whole register by "numberOfBitsToBeLoaded" bits, from "byteArr[]".
 * does not latch to output pins.
 */
void SR_voidLoadByteArrSerialInParallelOut(SR_SerialInParallelOut_t* regPtr, u8 byteArr[], u16 numberOfBitsToBeLoaded)
{
	while(numberOfBitsToBeLoaded--)
	{
		u8 byte = byteArr[numberOfBitsToBeLoaded / 8];
		SR_voidShiftSerialInParallelOut(regPtr, GET_BIT(byte, numberOfBitsToBeLoaded % 8));
	}
}

/*
 * shifts the whole register by "numberOfBitsToBeLoaded" bits, from "val".
 * does not latch to output pins.
 */
void SR_voidLoadFromU32SerialInParallelOut(SR_SerialInParallelOut_t* regPtr, u32 val, u8 numberOfBitsToBeLoaded)
{
	while(numberOfBitsToBeLoaded--)
	{
		SR_voidShiftSerialInParallelOut(regPtr, GET_BIT(val, numberOfBitsToBeLoaded));
	}
}

/*	copies reg2 to reg1, reg1 should be previously allocated	*/
void SR_voidCopySerialInParallelOut(SR_SerialInParallelOut_t* regPtr1, SR_SerialInParallelOut_t* regPtr2)
{
	regPtr1->clkPin				=	regPtr2->clkPin;
	regPtr1->clkPort			=	regPtr2->clkPort;
	regPtr1->clkTrigEdge		=	regPtr2->clkTrigEdge;
	regPtr1->minClkTicks		=	regPtr2->minClkTicks;
	regPtr1->dataInPin			=	regPtr2->dataInPin;
	regPtr1->dataInPort			=	regPtr2->dataInPort;
	regPtr1->minDataInTicks		=	regPtr2->minDataInTicks;
	regPtr1->latchPin			=	regPtr2->latchPin;
	regPtr1->latchPort			=	regPtr2->latchPort;
	regPtr1->latchActiveType	=	regPtr2->latchActiveType;
	regPtr1->minLatchTicks		=	regPtr2->minLatchTicks;
}










