/*
 * BypassFilter_0.c
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
#include "BypassFilter_config_0.h"

#define WM						BPF_0_WM
#define FS						BPF_0_FS
#define Q						BPF_0_Q
#define K						BPF_0_K
#define TIM_UNIT				BPF_0_TIM_UNIT
#define INTERRUPT_PRI_GROUP		BPF_0_INTERRUPT_PRI_GROUP
#define INTERRUPT_PRI_SUB		BPF_0_INTERRUPT_PRI_SUB

#define C 	(4.0f * FS * FS + 2.0f * FS * WM / Q + WM * WM)
#define C0	((K * 2.0f * FS * WM / Q) / C)
#define C1	((-8.0f * FS * FS + 2.0f * WM * WM) / C)
#define C2	((4.0f * FS * FS - 2.0f * FS * WM / Q + WM * WM) / C)


static f32 xPrev[2] = {0.0f, 0.0f};
static f32 yPrev[2] = {0.0f, 0.0f};

static void filter()
{
	// get new sample:
	f32 x = BPF_0_GET_RAW_SAMPLE;

	// perform filter equation:
	f32 y = (x - xPrev[1]) * C0 - yPrev[0] * C1 - yPrev[1] * C2;

	// execute callabck:
	BPF_0_SFC_CALLBACK(y);

	// update prev:
	xPrev[1] = xPrev[0];
	xPrev[0] = x;

	yPrev[1] = yPrev[0];
	yPrev[0] = y;
}

void BypassFilter_0_voidInit()
{
	TIM_u64SetFrequency(TIM_UNIT, (u64)(FS * 1000.0f));
	TIM_voidEnableInterrupt(TIM_UNIT, TIM_Interrupt_Update);

#if TIM_UNIT == 1 || TIM_UNIT == 8
	TIM_voidSetCallbackADV(TIM_UNIT, TIM_ADV_Vector_UP, filter);
#else
	TIM_voidSetCallbackGP(TIM_UNIT, filter);
#endif

	const NVIC_Interrupt_t intNumber[] = {
		NVIC_Interrupt_TIM1UP, NVIC_Interrupt_TIM2,
		NVIC_Interrupt_TIM3, NVIC_Interrupt_TIM4,
		NVIC_Interrupt_TIM5, NVIC_Interrupt_TIM6,
		NVIC_Interrupt_TIM7, NVIC_Interrupt_TIM8UP
	};
	NVIC_voidSetInterruptPriority(intNumber[TIM_UNIT-1], INTERRUPT_PRI_GROUP, INTERRUPT_PRI_SUB);
	NVIC_voidEnableInterrupt(intNumber[TIM_UNIT-1]);
}

void ByPassFilter_0_voidEnable()
{
	TIM_voidEnableCounter(TIM_UNIT);
}

void ByPassFilter_0_voidDisable()
{
	TIM_voidDisableCounter(TIM_UNIT);
}
