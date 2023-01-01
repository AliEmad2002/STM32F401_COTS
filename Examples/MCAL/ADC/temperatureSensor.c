/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Delay_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Error_Handler_interface.h"
#include "diag/trace.h"

/*	MCAL	*/
#include "UART_interface.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "NVIC_interface.h"
#include "STK_interface.h"
#include "DMA_interface.h"
#include "TIM_interface.h"
#include "FPEC_interface.h"
#include "EXTI_interface.h"
#include "ADC_interface.h"

/*	HAL	*/
#include "IR_interface.h"
#include "ESC_interface.h"
#include "RGB_LED_interface.h"
#include "WiFi_interface.h"



int main(void)
{
	/*	init RCC	*/
	RCC_voidSysClockInit();
	// ADC clock:
	RCC_voidEnablePeripheralClk(RCC_Bus_APB2, RCC_PERIPHERAL_ADC1);
	RCC_voidSetAdcPrescaler(RCC_ADC_Prescaler_PCLK2_by6);

	/*	ADC1 configuration	*/
	// enable continuous mode:
	ADC_voidEnableContinuousConversionMode(ADC_UnitNumber_1);
	// enable scan mode:
	ADC_voidEnableScanMode(ADC_UnitNumber_1);
	// select right data alignment:
	ADC_voidSetDataAlignment(ADC_UnitNumber_1, ADC_DataAlignment_Right);
	// set channel sample time:
	ADC_voidSetSampleTime(
		ADC_UnitNumber_1, ADC_ChannelNumber_16, ADC_SampleTime_239_5);
	// write channel in injected sequence:
	ADC_voidSetSequenceInjected(
		ADC_UnitNumber_1, ADC_InjectedSequenceNumber_4, ADC_ChannelNumber_16);
	// set injected sequence len to 1:
	ADC_voidSetSequenceLenInjected(ADC_UnitNumber_1, 1);
	// set injected channels trigger to JSWSTART:
	ADC_voidSetExternalEventInjected(
		ADC_UnitNumber_1, ADC_ExternalEventInjected_JSWSTART);
	// enable conversion on external event:
	ADC_voidEnableExternalTriggerInjected(ADC_UnitNumber_1);

	// power on:
	ADC_voidEnablePower(ADC_UnitNumber_1);
	// temperature sensor on:
	ADC_voidEnableTemperatureSensor();
	// calibrate ADC:
	ADC_voidStartCalibration(ADC_UnitNumber_1);
	ADC_voidWaitCalibration(ADC_UnitNumber_1);

	while(1)
	{
		// trigger start of conversion:
		ADC_voidStartSWConversionInjected(ADC_UnitNumber_1);
		// wait for end of conversion:
		while(!ADC_b8GetStatusFlag(ADC_UnitNumber_1, ADC_StatusFlag_JEOC));
		// clear JEOC, JSTRT flags:
		ADC_voidClearStatusFlag(ADC_UnitNumber_1, ADC_StatusFlag_JEOC);
		ADC_voidClearStatusFlag(ADC_UnitNumber_1, ADC_StatusFlag_JSTRT);
		// get and print temperature:
		volatile s16 adcRead = ADC_s16GetDataInjected(
			ADC_UnitNumber_1, ADC_InjectedDataRegister_1);
		volatile s8 temperature =
			ADC_s8GetTemperatureSensorReading((u16)adcRead);
		trace_printf("adcRead = %d, ", adcRead);
		trace_printf("temperature = %d\n", temperature);
	}

	return 0;
}