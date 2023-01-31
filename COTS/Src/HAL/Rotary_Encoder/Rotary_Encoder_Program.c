/*
 * Rotary_Encoder_Program.c
 *
 *  Created on: Jan 30, 2023
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Delay_interface.h"

/*	MCAL	*/
#include "GPIO_interface.h"
#include "EXTI_interface.h"
#include "STK_interface.h"

/*	SELF	*/
#include "Rotary_Encoder_Interface.h"
#include "Rotary_Encoder_Config.h"

/*	wait for falling edge of A	*/

/*	wait for falling edge of B	*/

/*	if there was a rising edge of A while waiting for that of B ==> CCW	*/

/*	else ==> CW	*/



void Rotary_Encoder_voidACallback(void)
{
	/*
	 * loop on all defined encoders, pick the first one who's pinA is low
	 */
	for (u8 i = 0; i < NUMBER_OF_ENCODERS; i++)
	{
		Rotary_Encoder_t* encoder = Rotray_Encoder_PtrArr[i];

		if (
			GPIO_DIGITAL_READ(encoder->outAPort, encoder->outAPin) ==
			GPIO_OutputLevel_Low
		)
		{
			/*	wait for B transition	*/
			volatile b8 initBState =
				GPIO_DIGITAL_READ(encoder->outBPort, encoder->outBPin);

			volatile b8 currentBState = initBState;

			u64 startTime = STK_u64GetElapsedTicks();

			while(currentBState == initBState)
			{
				if (STK_u64GetElapsedTicks() - startTime > TRANSITION_TIMEOUT)
					return;

				currentBState =
					GPIO_DIGITAL_READ(encoder->outBPort, encoder->outBPin);
			}

			/*	if transition was rising edge ==> CCW	*/
			if (initBState == 0)
			{
				encoder->count--;
				if (encoder->countDownCallbackEnabled)
					encoder->countDownCallback();
			}
			else
			{
				encoder->count++;
				if (encoder->countUpCallbackEnabled)
					encoder->countUpCallback();
			}

			Delay_voidBlockingDelayMs(10);
			EXTI_CLEAR_FLAG(encoder->outBPin);
			return;
		}
	}
}

void Rotary_Encoder_voidBCallback(void)
{
	/*
	 * loop on all defined encoders, pick the first one who's pinB is low
	 */
	for (u8 i = 0; i < NUMBER_OF_ENCODERS; i++)
	{
		Rotary_Encoder_t* encoder = Rotray_Encoder_PtrArr[i];

		if (
			GPIO_DIGITAL_READ(encoder->outBPort, encoder->outBPin) ==
			GPIO_OutputLevel_Low
		)
		{
			/*	wait for A transition	*/
			volatile b8 initAState =
				GPIO_DIGITAL_READ(encoder->outAPort, encoder->outAPin);

			volatile b8 currentAState = initAState;

			u64 startTime = STK_u64GetElapsedTicks();

			while(currentAState == initAState)
			{
				if (STK_u64GetElapsedTicks() - startTime > TRANSITION_TIMEOUT)
					return;

				currentAState =
					GPIO_DIGITAL_READ(encoder->outAPort, encoder->outAPin);
			}

			/*	if transition was falling edge ==> CCW	*/
			if (initAState == 1)
			{
				encoder->count--;
				if (encoder->countDownCallbackEnabled)
					encoder->countDownCallback();
			}
			else
			{
				encoder->count++;
				if (encoder->countUpCallbackEnabled)
					encoder->countUpCallback();
			}

			Delay_voidBlockingDelayMs(10);
			EXTI_CLEAR_FLAG(encoder->outAPin);
			return;
		}
	}
}

void Rotary_Encoder_voidInit(void)
{
	for (u8 i = 0; i < NUMBER_OF_ENCODERS; i++)
	{
		Rotary_Encoder_t* encoder = Rotray_Encoder_PtrArr[i];

		/*	reset counter	*/
		encoder->count = 0;

		/*	reset timestamp	*/
		encoder->lastTimeStamp = 0;

		/*	init pins as pulled up inputs	*/
		GPIO_voidSetPinInputPullUp(encoder->outAPort, encoder->outAPin);
		GPIO_voidSetPinInputPullUp(encoder->outBPort, encoder->outBPin);
		//GPIO_voidSetPinMode(encoder->outAPort, encoder->outAPin, GPIO_Mode_Input_Floating);
		//GPIO_voidSetPinMode(encoder->outBPort, encoder->outBPin, GPIO_Mode_Input_Floating);

		/*	init EXTI of pinA	*/
		EXTI_voidEnableLine(			encoder->outAPin);
		EXTI_voidMapLine(				encoder->outAPin, encoder->outAPort);
		EXTI_voidSetCallBack(			encoder->outAPin, Rotary_Encoder_voidACallback);
		EXTI_voidSetTriggeringEdge(		encoder->outAPin, EXTI_Trigger_fallingEdge);
		EXTI_voidEnableLineInterrupt(	encoder->outAPin);

		/*	init EXTI of pinB	*/
		EXTI_voidEnableLine(			encoder->outBPin);
		EXTI_voidMapLine(				encoder->outBPin, encoder->outBPort);
		EXTI_voidSetCallBack(			encoder->outBPin, Rotary_Encoder_voidBCallback);
		EXTI_voidSetTriggeringEdge(		encoder->outBPin, EXTI_Trigger_fallingEdge);
		EXTI_voidEnableLineInterrupt(	encoder->outBPin);
	}
}










