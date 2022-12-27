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

/******************************************************************************
 * Interrupts:
 ******************************************************************************/
/*	enables interrupt 'i' in ADC unit 'un'	*/
void ADC_voidEnableInterrupt(ADC_UnitNumber_t un, ADC_Interrupt_t i);

/*	disables interrupt 'i' in ADC unit 'un'	*/
void ADC_voidDisableInterrupt(ADC_UnitNumber_t un, ADC_Interrupt_t i);

/******************************************************************************
 * Functional control:
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




#endif /* INCLUDE_MCAL_ADC_ADC_INTERFACE_H_ */


















