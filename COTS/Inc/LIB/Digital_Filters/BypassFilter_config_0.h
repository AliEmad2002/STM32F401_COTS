/*
 * BypassFilter_0.h
 *
 *  Created on: May 27, 2023
 *      Author: Ali Emad
 */

#ifndef INCLUDE_LIB_DIGITAL_FILTERS_BYPASSFILTER_0_H_
#define INCLUDE_LIB_DIGITAL_FILTERS_BYPASSFILTER_0_H_

#include "Std_Types.h"
#include "Acoustic_Distance_Meter_config.h"
#include "ADC_Interface.h"
#include "Acoustic_Distance_Meter.h"

// middle frequency (in rad/sec) (must be float):
#define BPF_0_WM								ADM_MIC_FILTER_WM

// sampling frequency (in Hz) (must be float):
#define BPF_0_FS								ADM_MIC_FILTER_FS

// Gain (must be float):
#define PBF_0_K									ADM_MIC_FILTER_K

// Quality factor (must be float):
#define BPF_0_Q									ADM_MIC_FILTER_Q

// Timer unit:
#define BPF_0_TIM_UNIT							ADM_MIC_FILTER_TIM_UNIT

// Interrupt priority:
#define BPF_0_INTERRUPT_PRI_GROUP				ADM_MIC_FILTER_INTERRUPT_PRI_GROUP
#define BPF_0_INTERRUPT_PRI_SUB					ADM_MIC_FILTER_INTERRUPT_PRI_SUB

/*
 * raw sample getter, could be:
 * 		-	Function (with brackets, not just a pointer) that takes void and returns float.
 * 		-	Macro that evaluates to a floating point value. (For faster performance)
 */
#define BPF_0_GET_RAW_SAMPLE					((f32)ADC_GET_DATA_REGULAR(ADM_MIC_ADC_UNIT))

/*
 * sample filtering complete callback , could be:
 * 		-	Pointer to a function (just a pointer, without brackets) that takes
 * 			float and returns void.
 * 		-	Macro that takes a floating point value. (For faster performance)
 */
#define BPF_0_SFC_CALLBACK(filteredSample)		(ADM_voidMicFilterCallback(filteredSample))


#endif /* INCLUDE_LIB_DIGITAL_FILTERS_BYPASSFILTER_0_H_ */
