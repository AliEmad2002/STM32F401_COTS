///*
// * LowPassFilter_0.c
// *
// *  Created on: May 28, 2023
// *      Author: Ali Emad
// */
//
///*	LIB	*/
//#include "Std_Types.h"
//
///*	MCAL	*/
//#include "NVIC_interface.h"
//#include "STK_interface.h"
//#include "TIM_interface.h"
//
///*	SELF	*/
//#include "LowPassFilter_0_config.h"
//#include "LowPassFilter_0.h"
//
//#define WC						LPF_0_WC
//#define FS						LPF_0_FS
//#define K						LPF_0_K
//#define TIM_UNIT				LPF_0_TIM_UNIT
//#define INTERRUPT_PRI_GROUP		LPF_0_INTERRUPT_PRI_GROUP
//#define INTERRUPT_PRI_SUB		LPF_0_INTERRUPT_PRI_SUB
//#define N						LPF_0_N
//#define I						LPF_0_I
//
//#define C0 (K * WC / (WC + 2.0f * FS))
//#define C1 ((WC - 2.0f * FS) / (WC + 2.0f * FS))
//
//static f32 xPrev[N];
//static f32 yPrev[N];
//static f32 a = 0.25f;
//
//static void filter()
//{
//	// get new sample:
//	f32 x = LPF_0_GET_RAW_SAMPLE;
//
//	// perform filter equation:
//	//f32 y = (x + xPrev[I]) * C0 - yPrev[I] * C1;
//	f32 y = x*a - yPrev[I]*(a-1.0f);
//
//	// execute callabck:
//	LPF_0_SFC_CALLBACK(y);
//
//	// update prev:
//	xPrev[I] = x;
//	yPrev[I] = y;
//
//	// clear status flag:
//	TIM_CLEAR_STATUS_FLAG(TIM_UNIT, TIM_Status_Update);
//}
//
//void LowPassFilter_0_voidInit()
//{
//	trace_printf("&a = %d\n", &a);
//	/*	Enable RCC clock	*/
//	TIM_voidEnableTimRCC(TIM_UNIT);
//
//	TIM_u64SetFrequency(TIM_UNIT, (u64)(FS * 1000.0f));
//	TIM_voidEnableInterrupt(TIM_UNIT, TIM_Interrupt_Update);
//
//#if TIM_UNIT == 1 || TIM_UNIT == 8
//	TIM_voidSetCallbackADV(TIM_UNIT, TIM_ADV_Vector_UP, filter);
//#else
//	TIM_voidSetCallbackGP(TIM_UNIT, filter);
//#endif
//
//	NVIC_Interrupt_t intNumber = TIM_u8GetUpdateEventInterruptNumber(TIM_UNIT);
//	NVIC_voidSetInterruptPriority(intNumber, INTERRUPT_PRI_GROUP, INTERRUPT_PRI_SUB);
//	NVIC_voidEnableInterrupt(intNumber);
//
//	for (u8 i = 0; i < N; i++)
//	{
//		xPrev[i] = 50000.0f;
//		yPrev[i] = 50000.0f;
//	}
//}
//
//void LowPassFilter_0_voidEnable()
//{
//	TIM_voidEnableCounter(TIM_UNIT);
//}
//
//void LowPassFilter_0_voidDisable()
//{
//	TIM_voidDisableCounter(TIM_UNIT);
//}
