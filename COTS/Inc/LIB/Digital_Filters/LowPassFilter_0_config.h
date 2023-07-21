/*
 * LowPassFilter_0_config.h
 *
 *  Created on: May 28, 2023
 *      Author: Ali Emad
 */

#ifndef INCLUDE_LIB_DIGITAL_FILTERS_LOWPASSFILTER_0_CONFIG_H_
#define INCLUDE_LIB_DIGITAL_FILTERS_LOWPASSFILTER_0_CONFIG_H_

#include "Std_Types.h"
#include "ADC_Interface.h"
#include "Acoustic_Distance_Meter_config.h"
#include "Acoustic_Distance_Meter.h"
#include "Microphone.h"

// critical frequency (in rad/sec) (must be float):
#define LPF_0_WC								ADM_OUT_FILTER_WC

// sampling frequency (in Hz) (must be float):
#define LPF_0_FS								1000.0f / (f32)(ADM_SPK_ON_TIME + ADM_SPK_OFF_TIME)

// Gain (must be float):
#define LPF_0_K									1.0f

// Timer unit:
#define LPF_0_TIM_UNIT							ADM_OUT_FILTER_TIM_UNIT

// Interrupt priority:
#define LPF_0_INTERRUPT_PRI_GROUP				ADM_OUT_FILTER_INTERRUPT_PRI_GROUP
#define LPF_0_INTERRUPT_PRI_SUB					ADM_OUT_FILTER_INTERRUPT_PRI_SUB

// Index of the current variable in filtering loop (will not be changed by this driver):
extern volatile u8 SpkCtl_u8currentSpeakerIndex;
#define LPF_0_I									SpkCtl_u8currentSpeakerIndex

// Number of variables to be filtered:
#define LPF_0_N									ADM_SPK_N

/*
 * raw sample getter, could be:
 * 		-	Function (with brackets, not just a pointer) that takes void and returns float.
 * 		-	Macro that evaluates to a floating point value. (For faster performance)
 */
extern volatile u32 ADM_u32TravelTimeRawArr[ADM_SPK_N];
#define LPF_0_GET_RAW_SAMPLE					\
	((f32)ADM_u32TravelTimeRawArr[SpkCtl_u8currentSpeakerIndex])

/*
 * sample filtering complete callback , could be:
 * 		-	Pointer to a function (just a pointer, without brackets) that takes
 * 			float and returns void.
 * 		-	Macro that takes a floating point value. (For faster performance)
 */
extern volatile f32 ADM_u32TravelTimeFilteredArr[ADM_SPK_N];
#define LPF_0_SFC_CALLBACK(filteredSample)		\
	(ADM_u32TravelTimeFilteredArr[SpkCtl_u8currentSpeakerIndex] = filteredSample)



#endif /* INCLUDE_LIB_DIGITAL_FILTERS_LOWPASSFILTER_0_CONFIG_H_ */
