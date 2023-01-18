/*
 * ADC_interface.h
 *
 *  Created on: Nov 25, 2022
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MCAL_ADC_ADC_INTERFACE_H_
#define INCLUDE_MCAL_ADC_ADC_INTERFACE_H_

typedef enum{
	ADC_UnitNumber_1,
	ADC_UnitNumber_2
}ADC_UnitNumber_t;

typedef enum{
	ADC_InjectedDataRegister_1,
	ADC_InjectedDataRegister_2,
	ADC_InjectedDataRegister_3,
	ADC_InjectedDataRegister_4
}ADC_InjectedDataRegister_t;

typedef enum{
	ADC_DataAlignment_Right,
	ADC_DataAlignment_Left
}ADC_DataAlignment_t;

typedef enum{
	ADC_RegularSequenceNumber_1	,
	ADC_RegularSequenceNumber_2 ,
	ADC_RegularSequenceNumber_3 ,
	ADC_RegularSequenceNumber_4 ,
	ADC_RegularSequenceNumber_5 ,
	ADC_RegularSequenceNumber_6 ,
	ADC_RegularSequenceNumber_7 ,
	ADC_RegularSequenceNumber_8 ,
	ADC_RegularSequenceNumber_9 ,
	ADC_RegularSequenceNumber_10,
	ADC_RegularSequenceNumber_11,
	ADC_RegularSequenceNumber_12,
	ADC_RegularSequenceNumber_13,
	ADC_RegularSequenceNumber_14,
	ADC_RegularSequenceNumber_15,
	ADC_RegularSequenceNumber_16
}ADC_RegularSequenceNumber_t;

typedef enum{
	ADC_InjectedSequenceNumber_1,
	ADC_InjectedSequenceNumber_2,
	ADC_InjectedSequenceNumber_3,
	ADC_InjectedSequenceNumber_4
}ADC_InjectedSequenceNumber_t;

typedef enum{
	/*
	 * in this enum, decimal point is represented by the '_'
	 * ex: ADC_SampleTime_1_5 ==> means: sample time of 1.5 cycles
	 */
	ADC_SampleTime_1_5,
	ADC_SampleTime_7_5,
	ADC_SampleTime_13_5,
	ADC_SampleTime_28_5,
	ADC_SampleTime_41_5,
	ADC_SampleTime_55_5,
	ADC_SampleTime_71_5,
	ADC_SampleTime_239_5
}ADC_SampleTime_t;

typedef enum{
	ADC_ExternalEventInjected_TIM1TRGO,
	ADC_ExternalEventInjected_TIM1CC4,
	ADC_ExternalEventInjected_TIM2TRGO,
	ADC_ExternalEventInjected_TIM2CC1,
	ADC_ExternalEventInjected_TIM3CC4,
	ADC_ExternalEventInjected_TIM4TRGO,
	ADC_ExternalEventInjected_EXTI15,
	ADC_ExternalEventInjected_JSWSTART
}ADC_ExternalEventInjected_t;

typedef enum{
	ADC_ExternalEventRegular_TIM1CC1,
	ADC_ExternalEventRegular_TIM1CC2,
	ADC_ExternalEventRegular_TIM1CC3,
	ADC_ExternalEventRegular_TIM2CC2,
	ADC_ExternalEventRegular_TIM3TRGO,
	ADC_ExternalEventRegular_TIM4CC4,
	ADC_ExternalEventRegular_EXTI11,
	ADC_ExternalEventRegular_SWSTART
}ADC_ExternalEventRegular_t;

typedef enum{
	ADC_ChannelNumber_0,
	ADC_ChannelNumber_1,
	ADC_ChannelNumber_2,
	ADC_ChannelNumber_3,
	ADC_ChannelNumber_4,
	ADC_ChannelNumber_5,
	ADC_ChannelNumber_6,
	ADC_ChannelNumber_7,
	ADC_ChannelNumber_8,
	ADC_ChannelNumber_9,
	ADC_ChannelNumber_10,
	ADC_ChannelNumber_11,
	ADC_ChannelNumber_12,
	ADC_ChannelNumber_13,
	ADC_ChannelNumber_14,
	ADC_ChannelNumber_15,
	ADC_ChannelNumber_16,
	ADC_ChannelNumber_17,
}ADC_ChannelNumber_t;

typedef enum{
	ADC_Interrupt_EOC,
	ADC_Interrupt_AWD,
	ADC_Interrupt_JEOC
}ADC_Interrupt_t;

typedef enum{
	ADC_WatchdogMode_AllChannels,
	ADC_WatchdogMode_SingleChannel
}ADC_WatchdogMode_t;

typedef enum{
	ADC_StatusFlag_AWD,
	ADC_StatusFlag_EOC,
	ADC_StatusFlag_JEOC,
	ADC_StatusFlag_JSTRT,
	ADC_StatusFlag_STRT
}ADC_StatusFlag_t;

typedef enum{
	ADC_DualMode_Independent,
	ADC_DualMode_RegularSimultaneous_InjectedSimultaneous,
	ADC_DualMode_RegularSimultaneous_AlternateTrigger,
	ADC_DualMode_InjectedSimultaneous_FastInterleaved,
	ADC_DualMode_InjectedSimultaneous_SlowInterleaved,
	ADC_DualMode_InjectedSimultaneousOnly,
	ADC_DualMode_RegularSimultaneousOnly,
	ADC_DualMode_FastInterleavedOnly,
	ADC_DualMode_SlowInterleavedOnly,
	ADC_DualMode_AlternateTriggerOnly,

}ADC_DualMode_t;

/******************************************************************************
 * ADC unit power control:
 * Note:
 * it is recommended to do a calibration after each power up. see
 * calibration code (next block).
 ******************************************************************************/
/*	enables power of ADC peripheral	*/
void ADC_voidEnablePower(ADC_UnitNumber_t un);

/*	disables power of ADC peripheral	*/
void ADC_voidDisablePower(ADC_UnitNumber_t un);

/******************************************************************************
 * Calibration:
 * Capacitors inside ADC may introduce voltage measurement error due to the
 * the voltage formed on them. This error is HW-calculated and stored when a
 * calibration is performed. During conversions, this stored data is used to
 * eliminate errors.
 * Notes:
 * - it is recommended to do a calibration after each power up.
 * - Before starting a calibration, the ADC must have been in power-on state for
 * at least two ADC clock cycles.
 * - calibration data is stored in ADC_DR right after calibration is done.
 ******************************************************************************/
/*	starts calibration	*/
void ADC_voidStartCalibration(ADC_UnitNumber_t un);

/*	checks ending of calibration	*/
b8 ADC_b8IsCalibrationRunning(ADC_UnitNumber_t un);

/*	waits until calibration is done	*/
void ADC_voidWaitCalibration(ADC_UnitNumber_t un);

/*	resets calibration data registers (starts)	*/
void ADC_voidResetCalibrationDataReg(ADC_UnitNumber_t un);

/*	checks ending of resetting calibration data registers	*/
b8 ADC_b8IsCalibrationResetDone(ADC_UnitNumber_t un);

/*	waits until calibration reset is done	*/
void ADC_voidWaitCalibrationReset(ADC_UnitNumber_t un);

/******************************************************************************
 * GPIO pins connections:
 ******************************************************************************/
/*
 * returns number of port in the most significant 4-bits, and number of pin in
 * the other 4-bits.
 *
 * Notice that in blue-pill, ADC1 & ADC2 share the same pins
 */
u8 ADC_u8GetPortAndPinOfChannel(ADC_ChannelNumber_t ch);

/******************************************************************************
 * Status:
 * Note:
 * all flags are set by HW and cleared by SW, except for 'EOC', it's cleared by
 * SW or by reading 'ADC_DR'.
 ******************************************************************************/
/*	gets value of status flag	*/
b8 ADC_b8GetStatusFlag(ADC_UnitNumber_t un, ADC_StatusFlag_t flag);

/*	clears status flag	*/
void ADC_voidClearStatusFlag(ADC_UnitNumber_t un, ADC_StatusFlag_t flag);

/******************************************************************************
 * Analog Watchdog (AWD):
 ******************************************************************************/
/*	selects the single channel to be watchdog-ed in single channel WD mode	*/
void ADC_voidSetAWDChannel(ADC_UnitNumber_t un, ADC_ChannelNumber_t cn);

/*	sets AWD mode	*/
void ADC_voidSetAWDMode(ADC_UnitNumber_t un, ADC_WatchdogMode_t mode);

/*	enables analog watchdog on regular channels	*/
void ADC_voidEnableAWDRegularCh(ADC_UnitNumber_t un);

/*	disables analog watchdog on regular channels	*/
void ADC_voidDisableAWDRegularCh(ADC_UnitNumber_t un);

/*	enables analog watchdog on injected channels	*/
void ADC_voidEnableAWDInjectedCh(ADC_UnitNumber_t un);

/*	disables analog watchdog on injected channels	*/
void ADC_voidDisableAWDInjectedCh(ADC_UnitNumber_t un);

/*
 * sets analog watchdog high threshold value.
 * 'threshold' is 12-bit right aligned value. Remember that threshold checking
 * in AWD happens before aligning converted value.
 */
void ADC_voidSetAWDHighThreshold(ADC_UnitNumber_t un, u16 threshold);

/*
 * sets analog watchdog high threshold value.
 * 'threshold' is 12-bit right aligned value.
 */
void ADC_voidSetAWDLowThreshold(ADC_UnitNumber_t un, u16 threshold);

/******************************************************************************
 * Interrupts:
 * NOTE: ADC1 and ADC2 share only one single vector.
 ******************************************************************************/
/*	enables interrupt 'i' in ADC unit 'un'	*/
void ADC_voidEnableInterrupt(ADC_UnitNumber_t un, ADC_Interrupt_t i);

/*	disables interrupt 'i' in ADC unit 'un'	*/
void ADC_voidDisableInterrupt(ADC_UnitNumber_t un, ADC_Interrupt_t i);

/*	sets callback function	*/
void ADC_voidSetInterruptCallback(void (*callback)(void));

/******************************************************************************
 * Mode control:
 ******************************************************************************/
/**	Scan mode	**/
/*
 * enables scan mode.
 * it's recommended to use scan mode with regular mode channels and enable DMA.
 */
void ADC_voidEnableScanMode(ADC_UnitNumber_t un);

/*	disables scan mode	*/
void ADC_voidDisableScanMode(ADC_UnitNumber_t un);

/**	auto injected group conversion	**/
/*
 * enables auto injected group conversion.
 * It is a mode at which injected group channels are to be converted after end
 * of regular group conversion. Giving a maximum total sequence of 20
 * conversions.
 *
 * NOTE: it is not possible to use auto injected group conversion mode along
 * with discontinuous mode at the same time.
 */
void ADC_voidEnableAutoInjectedGroupConversion(ADC_UnitNumber_t un);

/*	disables auto injected group conversion	*/
void ADC_voidDisableAutoInjectedGroupConversion(ADC_UnitNumber_t un);

/**	discontinuous mode on regular channels	**/
/*
 * enables discontinuous mode on regular channels.
 * It is a mode at which a sub-group of conversions is to be done on each
 * trigger.
 *
 * NOTES:
 * - it is not possible to use auto injected group conversion mode along
 * with discontinuous mode at the same time.
 * - it is not possible to enable discontinuous mode for both regular and
 * injected channels.
 */
void ADC_voidEnableDiscontinuousModeRegularCh(ADC_UnitNumber_t un);

/*	disables discontinuous mode on regular channels	*/
void ADC_voidDisableDiscontinuousModeRegularCh(ADC_UnitNumber_t un);

/*
 * Sets sub-group length of discontinuous mode on regular channels.
 * 'n' is a 1-starting counter, and has maximum value of 8.
 */
void ADC_voidSetCountDiscontinuousModeRegularCh(ADC_UnitNumber_t un, u8 n);

/**	discontinuous mode on injected channels	**/
/*
 * enables discontinuous mode on injected channels.
 * It is a mode at which a sub-group of conversions is to be done on each
 * trigger.
 *
 * NOTES:
 * - it is not possible to use auto injected group conversion mode along
 * with discontinuous mode at the same time.
 * - it is not possible to enable discontinuous mode for both regular and
 * injected channels.
 *
 */
void ADC_voidEnableDiscontinuousModeInjectedCh(ADC_UnitNumber_t un);

/*	disables discontinuous mode on injected channels	*/
void ADC_voidDisableDiscontinuousModeInjectedCh(ADC_UnitNumber_t un);

/**	dual mode	**/
/*
 * sets dual mode.
 *
 * NOTES:
 * - In dual mode, a change of channel configuration generates a restart
 * that can produce a loss of synchronization. It is recommended to disable dual
 * mode (set to independent dual mode) before any configuration change.
 * - Dual mode is only set in ADC1, ADC2 and ADC3(if available) will follow.
 */
void ADC_voidSetDualMode(ADC_DualMode_t mode);

/*
 * disables dual mode.
 * (set to independent dual mode)
 */
void ADC_voidDisableDualMode(void);

/**	continuous conversion mode	**/
/*
 * enables continuous conversion mode (disables single conversion mode)
 *
 * Note:
 * user can enable only one of 'continuous' and 'single' conversion modes.
 */
void ADC_voidEnableContinuousConversionMode(ADC_UnitNumber_t un);

/*
 * enables single conversion mode (disables continuous conversion mode)
 *
 * Note:
 * user can enable only one of 'continuous' and 'single' conversion modes.
 */
void ADC_voidEnableSingleConversionMode(ADC_UnitNumber_t un);

/******************************************************************************
 * DMA:
 * Note:
 * DMA is only available in ADC1 and ADC3.
 ******************************************************************************/
/*	enable DMA	*/
void ADC_voidEnableDMA(ADC_UnitNumber_t un);

/*	disable DMA	*/
void ADC_voidDisableDMA(ADC_UnitNumber_t un);

/******************************************************************************
 * Data alignment (how 12-bit converted value is to be fit in 16-bit register)
 ******************************************************************************/
/*	sets data alignment	*/
void ADC_voidSetDataAlignment(
	ADC_UnitNumber_t un, ADC_DataAlignment_t alignment);

/******************************************************************************
 * External events and start of conversion sources:
 ******************************************************************************/
/*	sets external event source for injected group	*/
void ADC_voidSetExternalEventInjected(
	ADC_UnitNumber_t un, ADC_ExternalEventInjected_t source);

/*	enables external event triggering on injected channels	*/
void ADC_voidEnableExternalTriggerInjected(ADC_UnitNumber_t un);

/*	disables external event triggering on injected channels	*/
void ADC_voidDisableExternalTriggerInjected(ADC_UnitNumber_t un);

/*	sets external event source for regular group	*/
void ADC_voidSetExternalEventRegular(
	ADC_UnitNumber_t un, ADC_ExternalEventRegular_t source);

/*	enables external event triggering on regular channels	*/
void ADC_voidEnableExternalTriggerRegular(ADC_UnitNumber_t un);

/*	disables external event triggering on regular channels	*/
void ADC_voidDisableExternalTriggerRegular(ADC_UnitNumber_t un);

/*
 * SW-starts a conversion of injected channels.
 * Note:
 * 'JSWSTART' should be selected as external event source using the function:
 * "ADC_voidSetExternalEventInjected()"
 */
void ADC_voidStartSWConversionInjected(ADC_UnitNumber_t un);

/*
 * SW-starts a conversion of regular channels.
 * Note:
 * 'SWSTART' should be selected as external event source using the function:
 * "ADC_voidSetExternalEventRegular()"
 */
void ADC_voidStartSWConversionRegular(ADC_UnitNumber_t un);

/******************************************************************************
 * Temperature sensor:
 * Notes:
 * - temperature sensor is only available in ADC1 and internally connected to
 * channel-16.
 * - it's recommended to use sampling time near 17.1us
 * - it takes about 4-10 microseconds for the internal temperature sensor to be
 * usable after enabling it.
 ******************************************************************************/
/*	enable temperature sensor	*/
void ADC_voidEnableTemperatureSensor(void);

/*	disable temperature sensor	*/
void ADC_voidDisableTemperatureSensor(void);

/*
 * get temperature sensor's reading.
 * 'vSense' is the 12-bit ADC converted value of the sensor's internally
 * connected channel.
 *
 * 's8' return function is lighter if there is not an FPU, but less precise.
 */
s8 ADC_s8GetTemperatureSensorReading(u16 vSense);

f32 ADC_f32GetTemperatureSensorReading(u16 vSense);

/******************************************************************************
 * Sample time selection:
 ******************************************************************************/
/*	sets sample time of a channel	*/
void ADC_voidSetSampleTime(
		ADC_UnitNumber_t un, ADC_ChannelNumber_t ch, ADC_SampleTime_t time);

/******************************************************************************
 * Offset (in injected mode only):
 ******************************************************************************/
/*
 * sets offset to be subtracted from the converted value before storing the
 * result in ADC_JDRx. Thus injected group conversions could result in negative
 * value.
 *
 * 'offset' is unsigned 12-bit max.
 */
void ADC_voidSetOffset(
	ADC_UnitNumber_t un, ADC_InjectedSequenceNumber_t seqN,  u16 offset);

/******************************************************************************
 * Group sequence:
 *
 * Notes:
 * - in regular group, let sequence length be set to seqN, then the ADC converts
 * as follows: SQ1 ==> SQ2 ==> ... ==> SQ_seqN
 *
 * - while in injected group, then the ADC converts as follows:
 * JSQ_<5-seqN> ==> JSQ_<5-seqN +1> ==> JSQ4
 * (refer to the example in page 250 of RM0008 rev21 datasheet).
 ******************************************************************************/
/*	sets regular group sequence	*/
void ADC_voidSetSequenceRegular(
	ADC_UnitNumber_t un, ADC_RegularSequenceNumber_t seqN,
	ADC_ChannelNumber_t ch);

/*
 * sets regular group sequence length.
 * 'len' is in the range: 1-conversion to 16 conversions.
 */
void ADC_voidSetSequenceLenRegular(ADC_UnitNumber_t un, u8 len);

/*	sets injected group sequence	*/
void ADC_voidSetSequenceInjected(
	ADC_UnitNumber_t un, ADC_InjectedSequenceNumber_t seqN,
	ADC_ChannelNumber_t ch);

/*
 * sets injected group sequence length.
 * 'len' is in the range: 1-conversion to 4 conversions.
 */
void ADC_voidSetSequenceLenInjected(ADC_UnitNumber_t un, u8 len);

/******************************************************************************
 * Data reading:
 ******************************************************************************/
/*
 * reads data of injected conversions.
 * Note: result may be negative due to subtraction of pre-defined offset value.
 */
s16 ADC_s16GetDataInjected(
	ADC_UnitNumber_t un, ADC_InjectedDataRegister_t seqN);

/*	reads data of regular conversions	*/
u16 ADC_u16GetDataRegular(ADC_UnitNumber_t un);

/*
 * reads data of regular conversions in dual mode.
 * ADC1_DR is in the first half word of the returned word.
 * ADC2_DR is in the second half word of the returned word.
 *
 */
u32 ADC_u32GetDataRegularDual(void);

#endif /* INCLUDE_MCAL_ADC_ADC_INTERFACE_H_ */


















