/*
 * ADC_program.c
 *
 *  Created on: Nov 25, 2022
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Error_Handler_interface.h"

/*	SELF	*/
#include "ADC_private.h"
#include "ADC_interface.h"
#include "ADC_config.h"

/******************************************************************************
 * ADC unit power control:
 ******************************************************************************/
/*	enables power of ADC peripheral	*/
inline void ADC_voidEnablePower(ADC_UnitNumber_t un)
{
	SET_BIT(ADC[un]->CR2, CR2_ADON);
}

/*	disables power of ADC peripheral	*/
inline void ADC_voidDisablePower(ADC_UnitNumber_t un)
{
	CLR_BIT(ADC[un]->CR2, CR2_ADON);
}

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
inline void ADC_voidStartCalibration(ADC_UnitNumber_t un)
{
	SET_BIT(ADC[un]->CR2, CR2_CAL);
}

/*	checks running state of calibration	*/
inline b8 ADC_b8IsCalibrationRunning(ADC_UnitNumber_t un)
{
	return GET_BIT(ADC[un]->CR2, CR2_CAL);
}

/*	waits until calibration is done	*/
inline void ADC_voidWaitCalibration(ADC_UnitNumber_t un)
{
	while(ADC_b8IsCalibrationRunning(un));
}

/*	resets calibration data registers (starts)	*/
inline void ADC_voidResetCalibrationDataReg(ADC_UnitNumber_t un)
{
	SET_BIT(ADC[un]->CR2, CR2_RSTCAL);
}

/*	checks ending of resetting calibration data registers	*/
inline b8 ADC_b8IsCalibrationResetDone(ADC_UnitNumber_t un)
{
	return !GET_BIT(ADC[un]->CR2, CR2_RSTCAL);
}

/*	waits until calibration reset is done	*/
void ADC_voidWaitCalibrationReset(ADC_UnitNumber_t un)
{
	while(!ADC_b8IsCalibrationResetDone(un));
}

/******************************************************************************
 * Status:
 * Note:
 * all flags are set by HW and cleared by SW, except for 'EOC', it's cleared by
 * SW or by reading 'ADC_DR'.
 ******************************************************************************/
/*	gets value of status flag	*/
inline b8 ADC_b8GetStatusFlag(ADC_UnitNumber_t un, ADC_StatusFlag_t flag)
{
	return GET_BIT(ADC[un]->SR, flag);
}

/*	clears status flag	*/
inline void ADC_voidClearStatusFlag(ADC_UnitNumber_t un, ADC_StatusFlag_t flag)
{
	CLR_BIT(ADC[un]->SR, flag);
}

/******************************************************************************
 * Analog Watchdog (AWD):
 ******************************************************************************/
/*	selects the single channel to be watchdog-ed in single channel WD mode	*/
inline void ADC_voidSetAWDChannel(ADC_UnitNumber_t un, ADC_ChannelNumber_t cn)
{
	EDT_REG(ADC[un]->CR1, CR1_AWDCH_0, cn, 5);
}

/*	sets AWD mode	*/
inline void ADC_voidSetAWDMode(ADC_UnitNumber_t un, ADC_WatchdogMode_t mode)
{
	WRT_BIT(ADC[un]->CR1, CR1_AWDSGL, mode);
}

/*	enables analog watchdog on regular channels	*/
inline void ADC_voidEnableAWDRegularCh(ADC_UnitNumber_t un)
{
	SET_BIT(ADC[un]->CR1, CR1_AWDEN);
}

/*	disables analog watchdog on regular channels	*/
inline void ADC_voidDisableAWDRegularCh(ADC_UnitNumber_t un)
{
	CLR_BIT(ADC[un]->CR1, CR1_AWDEN);
}

/*	enables analog watchdog on injected channels	*/
inline void ADC_voidEnableAWDInjectedCh(ADC_UnitNumber_t un)
{
	SET_BIT(ADC[un]->CR1, CR1_JAWDEN);
}

/*	disables analog watchdog on injected channels	*/
inline void ADC_voidDisableAWDInjectedCh(ADC_UnitNumber_t un)
{
	CLR_BIT(ADC[un]->CR1, CR1_JAWDEN);
}

/*
 * sets analog watchdog high threshold value.
 * 'threshold' is 12-bit right aligned value. Remember that threshold checking
 * in AWD happens before aligning converted value.
 */
inline void ADC_voidSetAWDHighThreshold(ADC_UnitNumber_t un, u16 threshold)
{
	ADC[un]->HTR = threshold;
}

/*
 * sets analog watchdog high threshold value.
 * 'threshold' is 12-bit right aligned value.
 */
inline void ADC_voidSetAWDLowThreshold(ADC_UnitNumber_t un, u16 threshold)
{
	ADC[un]->LTR = threshold;
}

/******************************************************************************
 * Interrupts:
 ******************************************************************************/
/*	enables interrupt 'i' in ADC unit 'un'	*/
inline void ADC_voidEnableInterrupt(ADC_UnitNumber_t un, ADC_Interrupt_t i)
{
	SET_BIT(ADC[un]->CR1, CR1_EOCIE + i);
	/*
	switch(i)
	{
	case ADC_Interrupt_EOC:
		SET_BIT(ADC[un]->CR1, CR1_EOCIE);
		break;
	case ADC_Interrupt_AWD:
		SET_BIT(ADC[un]->CR1, CR1_AWDIE);
		break;
	case ADC_Interrupt_JEOC:
		SET_BIT(ADC[un]->CR1, CR1_JEOCIE);
		break;
	}
	*/
}

/*	disables interrupt 'i' in ADC unit 'un'	*/
inline void ADC_voidDisableInterrupt(ADC_UnitNumber_t un, ADC_Interrupt_t i)
{
	CLR_BIT(ADC[un]->CR1, CR1_EOCIE + i);
}

static void (*ADC_fPtrCallback)(void);

/*	sets callback function	*/
void ADC_voidSetInterruptCallback(void (*callback)(void))
{
	ADC_fPtrCallback = callback;
}

/*	ISR	*/
void ADC1_2_IRQHandler(void)
{
	ADC_fPtrCallback();
}

/******************************************************************************
 * Mode control:
 ******************************************************************************/
/**	Scan mode	**/
/*
 * enables scan mode.
 * it's recommended to use scan mode with regular mode channels and enable DMA.
 */
inline void ADC_voidEnableScanMode(ADC_UnitNumber_t un)
{
	SET_BIT(ADC[un]->CR1, CR1_SCAN);
}

/*	disables scan mode	*/
inline void ADC_voidDisableScanMode(ADC_UnitNumber_t un)
{
	CLR_BIT(ADC[un]->CR1, CR1_SCAN);
}

/**	auto injected group conversion	**/
/*	enables auto injected group conversion	*/
inline void ADC_voidEnableAutoInjectedGroupConversion(ADC_UnitNumber_t un)
{
	SET_BIT(ADC[un]->CR1, CR1_JAUTO);
}

/*	disables auto injected group conversion	*/
inline void ADC_voidDisableAutoInjectedGroupConversion(ADC_UnitNumber_t un)
{
	CLR_BIT(ADC[un]->CR1, CR1_JAUTO);
}

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
inline void ADC_voidEnableDiscontinuousModeRegularCh(ADC_UnitNumber_t un)
{
	SET_BIT(ADC[un]->CR1, CR1_DISCEN);
}

/*	disables discontinuous mode on regular channels	*/
inline void ADC_voidDisableDiscontinuousModeRegularCh(ADC_UnitNumber_t un)
{
	CLR_BIT(ADC[un]->CR1, CR1_DISCEN);
}

/*
 * Sets sub-group length of discontinuous mode on regular channels.
 * 'n' is a 1-starting counter, and has maximum value of 8.
 */
void ADC_voidSetCountDiscontinuousModeRegularCh(ADC_UnitNumber_t un, u8 n)
{
	if (n < 1  ||  n > 8)
	{
		ErrorHandler_voidExecute(ErrorHandler_ErrorSource_ADC_WrongDISCNUM);
	}
	else
	{
		EDT_REG(ADC[un]->CR1, CR1_DISCNUM_0, n-1, 3);
	}
}

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
inline void ADC_voidEnableDiscontinuousModeInjectedCh(ADC_UnitNumber_t un)
{
	SET_BIT(ADC[un]->CR1, CR1_JDISCEN);
}

/*	disables discontinuous mode on injected channels	*/
inline void ADC_voidDisableDiscontinuousModeInjectedCh(ADC_UnitNumber_t un)
{
	CLR_BIT(ADC[un]->CR1, CR1_JDISCEN);
}

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
inline void ADC_voidSetDualMode(ADC_DualMode_t mode)
{
	EDT_REG(ADC[ADC_UnitNumber_1]->CR1, CR1_DUALMOD_0, mode, 4);
}

/*
 * disables dual mode.
 * (set to independent dual mode)
 */
inline void ADC_voidDisableDualMode(void)
{
	ADC_voidSetDualMode(ADC_DualMode_Independent);
}

/**	continuous conversion mode	**/
/*
 * enables continuous conversion mode (disables single conversion mode)
 *
 * Note:
 * user can enable only one of 'continuous' and 'single' conversion modes.
 */
inline void ADC_voidEnableContinuousConversionMode(ADC_UnitNumber_t un)
{
	SET_BIT(ADC[un]->CR2, CR2_CONT);
}

/*
 * enables single conversion mode (disables continuous conversion mode)
 *
 * Note:
 * user can enable only one of 'continuous' and 'single' conversion modes.
 */
inline void ADC_voidEnableSingleConversionMode(ADC_UnitNumber_t un)
{
	CLR_BIT(ADC[un]->CR2, CR2_CONT);
}

/******************************************************************************
 * DMA:
 * Note:
 * DMA is only available in ADC1 and ADC3.
 ******************************************************************************/
/*	enable DMA	*/
inline void ADC_voidEnableDMA(ADC_UnitNumber_t un)
{
	SET_BIT(ADC[un]->CR2, CR2_DMA);
}

/*	disable DMA	*/
inline void ADC_voidDisableDMA(ADC_UnitNumber_t un)
{
	CLR_BIT(ADC[un]->CR2, CR2_DMA);
}

/******************************************************************************
 * Data alignment (how 12-bit converted value is to be fit in 16-bit register)
 ******************************************************************************/
/*	sets data alignment	*/
inline void ADC_voidSetDataAlignment(
	ADC_UnitNumber_t un, ADC_DataAlignment_t alignment)
{
	WRT_BIT(ADC[un]->CR2, CR2_ALIGN, alignment);
}

/******************************************************************************
 * External events and start of conversion sources:
 ******************************************************************************/
/*	sets external event source for injected group	*/
inline void ADC_voidSetExternalEventInjected(
	ADC_UnitNumber_t un, ADC_ExternalEventInjected_t source)
{
	EDT_REG(ADC[un]->CR2, CR2_JEXTSEL_0, source, 3);
}

/*	enables external event triggering on injected channels	*/
inline void ADC_voidEnableExternalTriggerInjected(ADC_UnitNumber_t un)
{
	SET_BIT(ADC[un]->CR2, CR2_JEXTTRIG);
}

/*	disables external event triggering on injected channels	*/
inline void ADC_voidDisableExternalTriggerInjected(ADC_UnitNumber_t un)
{
	CLR_BIT(ADC[un]->CR2, CR2_JEXTTRIG);
}

/*	sets external event source for regular group	*/
inline void ADC_voidSetExternalEventRegular(
	ADC_UnitNumber_t un, ADC_ExternalEventRegular_t source)
{
	EDT_REG(ADC[un]->CR2, CR2_EXTSEL_0, source, 3);
}

/*	enables external event triggering on regular channels	*/
inline void ADC_voidEnableExternalTriggerRegular(ADC_UnitNumber_t un)
{
	SET_BIT(ADC[un]->CR2, CR2_EXTTRIG);
}

/*	disables external event triggering on regular channels	*/
inline void ADC_voidDisableExternalTriggerRegular(ADC_UnitNumber_t un)
{
	CLR_BIT(ADC[un]->CR2, CR2_EXTTRIG);
}

/*
 * SW-starts a conversion of injected channels.
 * Note:
 * 'JSWSTART' should be selected as external event source using the function:
 * "ADC_voidSetExternalEventInjected()"
 */
inline void ADC_voidStartSWConversionInjected(ADC_UnitNumber_t un)
{
	SET_BIT(ADC[un]->CR2, CR2_JSWSTART);
}

/*
 * SW-starts a conversion of regular channels.
 * Note:
 * 'SWSTART' should be selected as external event source using the function:
 * "ADC_voidSetExternalEventRegular()"
 */
inline void ADC_voidStartSWConversionRegular(ADC_UnitNumber_t un)
{
	SET_BIT(ADC[un]->CR2, CR2_SWSTART);
}

/******************************************************************************
 * Temperature sensor:
 * Notes:
 * - temperature sensor is only available in ADC1 and internally connected to
 * channel-16.
 * - it's recommended to use sampling time near 17.1us
 * - it takes about 4-10 microseconds for the internal temperature sensor to be
 * usable after enabling it.
 ******************************************************************************/
#define V_25		((s16)(ADC_V_25_VOLTS * 4096.0 / ADC_V_REF_VOLTS))
#define AVG_SLOPE	((f32)(ADC_AVG_SLOPE_VOLTS * 4096.0 / ADC_V_REF_VOLTS))

/*	enable temperature sensor	*/
inline void ADC_voidEnableTemperatureSensor(void)
{
	SET_BIT(ADC[ADC_UnitNumber_1]->CR2, CR2_TSVREFE);
}

/*	disable temperature sensor	*/
inline void ADC_voidDisableTemperatureSensor(void)
{
	CLR_BIT(ADC[ADC_UnitNumber_1]->CR2, CR2_TSVREFE);
}

/*
 * get temperature sensor's reading.
 * 'vSense' is the 12-bit ADC converted value of the sensor's internally
 * connected channel.
 *
 * 's8' return function is lighter if there is not an FPU, but less precise.
 */
inline s8 ADC_s8GetTemperatureSensorReading(u16 vSense)
{
	return (s8)((V_25 - (s16)vSense) / ((s16)AVG_SLOPE)) + 25;
}

f32 ADC_f32GetTemperatureSensorReading(u16 vSense)
{
	return ((f32)(V_25 - (s16)vSense)) / AVG_SLOPE + 25.0f;
}

/******************************************************************************
 * Sample time selection:
 ******************************************************************************/
/*	sets sample time of a channel	*/
void ADC_voidSetSampleTime(
		ADC_UnitNumber_t un, ADC_ChannelNumber_t ch, ADC_SampleTime_t time)
{
	if (ch < ADC_ChannelNumber_10)
	{
		EDT_REG(ADC[un]->SMPR2, ch*3, time, 3);
	}
	else
	{
		EDT_REG(ADC[un]->SMPR1, (ch-10)*3, time, 3);
	}
}

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
inline void ADC_voidSetOffset(
	ADC_UnitNumber_t un, ADC_InjectedSequenceNumber_t seqN,  u16 offset)
{
	ADC[un]->JOFR[seqN] = offset;
}

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
	ADC_ChannelNumber_t ch)
{
	if (seqN > ADC_RegularSequenceNumber_12)
	{
		EDT_REG(
			ADC[un]->SQR1, (seqN - ADC_RegularSequenceNumber_13) * 5, ch, 5);
	}
	else if (seqN > ADC_RegularSequenceNumber_6)
	{
		EDT_REG(
			ADC[un]->SQR2, (seqN - ADC_RegularSequenceNumber_7) * 5, ch, 5);
	}
	else
	{
		EDT_REG(
			ADC[un]->SQR3, (seqN - ADC_RegularSequenceNumber_1) * 5, ch, 5);
	}
}

/*
 * sets regular group sequence length.
 * 'len' is in the range: 1-conversion to 16 conversions.
 */
void ADC_voidSetSequenceLenRegular(ADC_UnitNumber_t un, u8 len)
{
	if (len < 1 || len > 16)
	{
		ErrorHandler_voidExecute(ErrorHandler_ErrorSource_ADC_WrongRegSeqLen);
	}
	else
	{
		EDT_REG(ADC[un]->SQR1, SQR1_L_0, len-1, 4);
	}
}

/*	sets injected group sequence	*/
inline void ADC_voidSetSequenceInjected(
	ADC_UnitNumber_t un, ADC_InjectedSequenceNumber_t seqN,
	ADC_ChannelNumber_t ch)
{
	EDT_REG(ADC[un]->JSQR, seqN * 5, ch,5);
}

/*
 * sets injected group sequence length.
 * 'len' is in the range: 1-conversion to 4 conversions.
 */
void ADC_voidSetSequenceLenInjected(ADC_UnitNumber_t un, u8 len)
{
	if (len < 1 || len > 4)
	{
		ErrorHandler_voidExecute(ErrorHandler_ErrorSource_ADC_WrongInjSeqLen);
	}
	else
	{
		EDT_REG(ADC[un]->JSQR, JSQR_JL_0, len-1, 2);
	}
}

/******************************************************************************
 * Data reading:
 ******************************************************************************/
/*
 * reads data of injected conversions.
 * Note: result may be negative due to subtraction of pre-defined offset value.
 */
inline s16 ADC_s16GetDataInjected(
	ADC_UnitNumber_t un, ADC_InjectedDataRegister_t regI)
{
	return (u16)(ADC[un]->JDR[regI]);
}

/*	reads data of regular conversions	*/
inline u16 ADC_u16GetDataRegular(ADC_UnitNumber_t un)
{
	return (u16)(ADC[un]->DR);
}

/*
 * reads data of regular conversions in dual mode.
 * ADC1_DR is in the first half word of the returned word.
 * ADC2_DR is in the second half word of the returned word.
 *
 */
inline u32 ADC_u32GetDataRegularDual(void)
{
	return ADC[ADC_UnitNumber_1]->DR;
}






































































