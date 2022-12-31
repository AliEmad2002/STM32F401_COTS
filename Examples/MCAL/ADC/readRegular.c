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
	// GPIO & AFIO clocks:
	RCC_voidEnablePeripheralClk(RCC_Bus_APB2, RCC_PERIPHERAL_IOPA);
	RCC_voidEnablePeripheralClk(RCC_Bus_APB2, RCC_PERIPHERAL_AFIO);
	// ADC clock:
	RCC_voidEnablePeripheralClk(RCC_Bus_APB2, RCC_PERIPHERAL_ADC1);
	RCC_voidSetAdcPrescaler(RCC_ADC_Prescaler_PCLK2_by6);

	/*	init GPIO pin	*/
	GPIO_voidSetPinMode(GPIO_PortName_A, 3, GPIO_Mode_Input_Analog);

	/*	ADC1 configuration	*/
	// enable continuous mode:
	ADC_voidEnableContinuousConversionMode(ADC_UnitNumber_1);
	// enable scan mode:
	ADC_voidEnableScanMode(ADC_UnitNumber_1);
	// select right data alignment:
	ADC_voidSetDataAlignment(ADC_UnitNumber_1, ADC_DataAlignment_Right);
	// set channel sample time:
	ADC_voidSetSampleTime(
		ADC_UnitNumber_1, ADC_ChannelNumber_3, ADC_SampleTime_1_5);
	// write channel in regular sequence:
	ADC_voidSetSequenceRegular(
		ADC_UnitNumber_1, ADC_RegularSequenceNumber_1, ADC_ChannelNumber_3);
	// set regular sequence len to 1, as only one channel is to be converted:
	ADC_voidSetSequenceLenRegular(ADC_UnitNumber_1, 1);
	// set regular channels trigger to SWSTART:
	ADC_voidSetExternalEventRegular(
		ADC_UnitNumber_1, ADC_ExternalEventRegular_SWSTART);
	// enable conversion on external event:
	ADC_voidEnableExternalTriggerRegular(ADC_UnitNumber_1);

	// power on:
	ADC_voidEnablePower(ADC_UnitNumber_1);
	// calibrate:
	ADC_voidStartCalibration(ADC_UnitNumber_1);
	ADC_voidWaitCalibration(ADC_UnitNumber_1);

	// trigger start of conversion:
	ADC_voidStartSWConversionRegular(ADC_UnitNumber_1);

	while(1)
	{
		// wait for end of conversion:
		while(!ADC_b8GetStatusFlag(ADC_UnitNumber_1, ADC_StatusFlag_EOC));
		// print converted value:
		volatile u16 adcRead = ADC_u16GetDataRegular(ADC_UnitNumber_1);
		trace_printf("%u\n", adcRead);
	}



	return 0;
}