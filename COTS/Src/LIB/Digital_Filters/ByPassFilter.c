/*
 * ByPassFilter.c
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
#include "ByPassFilter.h"

void ByPassFilter_voidInit(
	ByPassFilter_t* f, f32 wm, f32 fs, f32 k, f32 q,
	f32(*getter)(void), void(*callback)(f32),
	u8 timerUnitNumber, u8 priGroup, u8 priSub
)
{
	f32 c = 4.0f * fs * fs + 2.0f * fs * wm / q + wm * wm;
	f->c[0] = (k * 2.0f * fs * wm / q) / c;
	f->c[1] = (-8.0f * fs * fs + 2.0f * wm * wm) / c;
	f->c[2] = (4.0f * fs * fs - 2.0f * fs * wm / q + wm * wm) / c;

	f->getSample = getter;
	f->callback = callback;

	f->xPrev[0] = 0;
	f->xPrev[1] = 0;
	f->yPrev[0] = 0;
	f->yPrev[1] = 0;

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

void ByPassFilter_voidFilter(ByPassFilter_t* f)
{
	// get new sample:
	f32 x = f->getSample();

	// perform filter equation:
	f32 y = (x - f->xPrev[1]) * f->c[0] -
			f->yPrev[0] * f->c[1] -
			f->yPrev[1] * f->c[2];

	// callabck:
	f->callback(y);

	// update prev:
	f->xPrev[1] = f->xPrev[0];
	f->xPrev[0] = x;

	f->yPrev[1] = f->yPrev[0];
	f->yPrev[0] = y;
}

void ByPassFilter_voidEnableFilter(ByPassFilter_t* f)
{
	TIM_voidEnableCounter(f->timerUnitNumber);
}

void ByPassFilter_voidDisableFilter(ByPassFilter_t* f)
{
	TIM_voidDisableCounter(f->timerUnitNumber);
}
