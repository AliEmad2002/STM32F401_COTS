/*
 * ESC_program.c
 *
 *  Created on: Nov 6, 2022
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Error_Handler_interface.h"

/*	MCAL	*/
#include "TIM_interface.h"

/*	SELF	*/
#include "ESC_interface.h"


/*
 * inits ESC.
 *
 * inits timer channel for PWM output.
 * init GPIO pin mapped to the channel.
 * copies channel to ESC object.
 */
void ESC_voidInit(
	ESC_t* esc, u8 timerUnitNumber, TIM_Channel_t timerChannel, u16 freqHz,
	u32 minPulseWidthUs, u32 maxPulseWidthUs, u8 afioTimerMap)
{
	/*	disable counter	*/
	TIM_voidDisableCounter(timerUnitNumber);

	u64 mHzFreq =
		TIM_u64InitPWM(timerUnitNumber, timerChannel,
		TIM_OutputCompareMode_PWM1, freqHz);

	TIM_voidInitOutputPin(timerUnitNumber, timerChannel, afioTimerMap);

	esc->timerUnitNumber = timerUnitNumber;
	esc->timerChannel = timerChannel;

	esc->minOCR = (mHzFreq * ((u64)minPulseWidthUs << 16)) / (u64)1000000000;
	esc->maxOCR = (mHzFreq * ((u64)maxPulseWidthUs << 16)) / (u64)1000000000;

	/*	enable counter	*/
	TIM_voidEnableCounter(timerUnitNumber);

	/*	initially set speed zero	*/
	ESC_voidSetSpeed(esc, 0);
}

/*
 * sets speed of ESC.
 *
 * speed is a value between -1000 and 1000
 */
void ESC_voidSetSpeed(ESC_t* esc, s16 speed)
{
	s32 ocrStop = (esc->minOCR + esc->maxOCR) >> 1;

	u16 OCR = ocrStop + (s32)speed * (esc->minOCR + esc->maxOCR) / 1000;

	if (OCR < esc->minOCR  ||  OCR > esc->maxOCR)
	{
		ErrorHandler_voidExecute(1);
	}

	else
	{
		TIM_voidSetDutyCycle(esc->timerUnitNumber, esc->timerChannel, OCR);
	}
}


