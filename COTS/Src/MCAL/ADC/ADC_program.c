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

/*	MCAL	*/


/*	SELF	*/
#include "ADC_interface.h"
#include "ADC_private.h"

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

/******************************************************************************
 * Functional control:
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
		ErrorHandler_voidExecute(ErrorHandler_ADC_ErrorSource_WrongDISCNUM);
	}
	else
	{
		EDT_REG(ADC[un]->CR1, CR1_DISCNUM_0, n, 3);
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























