/*
 * LowPassFilter.c
 *
 *  Created on: May 27, 2023
 *      Author: Ali Emad
 */

/*	LIB	*/
#include "Std_Types.h"

/*	MCAL	*/
#include "NVIC_interface.h"
#include "TIM_interface.h"

/*	SELF	*/
#include "LowPassFilter.h"

void LowPassFilter_voidInit(
	LowPassFilter_t* f, f32 wc, f32 fs, f32 k,
	f32(*getter)(void), void(*callback)(f32),
	u8 timerUnitNumber, u8 priGroup, u8 priSub
)
{
	f->c[0] = k * wc / (wc + 2.0f * fs);
	f->c[1] = k * (wc - 2.0f * fs) / (wc + 2.0f * fs);

	f->getSample = getter;
	f->callback = callback;

	f->xPrev = 0;
	f->yPrev = 0;

	f->timerUnitNumber = timerUnitNumber;

	TIM_u64SetFrequency(timerUnitNumber, (u64)(fs * 1000.0f));
	TIM_voidEnableInterrupt(timerUnitNumber, TIM_Interrupt_Update);

	const NVIC_Interrupt_t intNumber[] = {
		NVIC_Interrupt_TIM1UP, NVIC_Interrupt_TIM2,
		NVIC_Interrupt_TIM3, NVIC_Interrupt_TIM4,
		NVIC_Interrupt_TIM5, NVIC_Interrupt_TIM6,
		NVIC_Interrupt_TIM7, NVIC_Interrupt_TIM8UP
	};
	NVIC_voidSetInterruptPriority(intNumber[timerUnitNumber-1], priGroup, priSub);
	NVIC_voidEnableInterrupt(intNumber[timerUnitNumber-1]);
}

void LowPassFilter_voidFilter(LowPassFilter_t* f)
{
	// get new sample:
	f32 x = f->getSample();

	// perform filter equation:
	f32 y = (x + f->xPrev) * f->c[0] - f->yPrev * f->c[1];

	// callabck:
	f->callback(y);

	// update prev:
	f->xPrev = x;
	f->yPrev = y;
}

void LowPassFilter_voidEnableFilter(LowPassFilter_t* f)
{
	TIM_voidEnableCounter(f->timerUnitNumber);
}

void LowPassFilter_voidDisableFilter(LowPassFilter_t* f)
{
	TIM_voidDisableCounter(f->timerUnitNumber);
}
