/*
 * TIM_ADV_program.c
 *
 *  Created on: Oct 10, 2022
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Target_config.h"

/*	SELF	*/
#include "TIM_ADV_interface.h"
#include "TIM_ADV_private.h"



/*	enable counter	*/
void TIM_ADV_voidEnableCounter(TIM_ADV_UnitNumber_t unitNumber)
{
	SET_BIT(TIM_ADV[unitNumber]->CR1, TIM_ADV_CR1_CEN);
}

/*	disable counter	*/
void TIM_ADV_voidDisableCounter(TIM_ADV_UnitNumber_t unitNumber)
{
	CLR_BIT(TIM_ADV[unitNumber]->CR1, TIM_ADV_CR1_CEN);
}

/*	enable update event generation	*/
void TIM_ADV_voidEnableUpdateEvent(TIM_ADV_UnitNumber_t unitNumber)
{
	CLR_BIT(TIM_ADV[unitNumber]->CR1, TIM_ADV_CR1_UDIS);
}

/*	disable update event generation	*/
void TIM_ADV_voidDisableUpdateEvent(TIM_ADV_UnitNumber_t unitNumber)
{
	SET_BIT(TIM_ADV[unitNumber]->CR1, TIM_ADV_CR1_UDIS);
}

/*
 * selects update source
 * (if update is enabled)
 */
void TIM_ADV_voidSelectUpdateSource(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_UpdateSource_t source)
{
	WRT_BIT(TIM_ADV[unitNumber]->CR1, TIM_ADV_CR1_URS, source);
}

/*
 * enables one pulse mode.
 * (stops counter after next update event)
 */
void TIM_ADV_voidEnableOnePulseMode(TIM_ADV_UnitNumber_t unitNumber)
{
	SET_BIT(TIM_ADV[unitNumber]->CR1, TIM_ADV_CR1_OPM);
}

/*	disables one pulse mode	*/
void TIM_ADV_voidDisableOnePulseMode(TIM_ADV_UnitNumber_t unitNumber)
{
	CLR_BIT(TIM_ADV[unitNumber]->CR1, TIM_ADV_CR1_OPM);
}

/*	selects counting direction	*/
void TIM_ADV_voidSelectCountDirection(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_CountDirection_t dir)
{
	WRT_BIT(TIM_ADV[unitNumber]->CR1, TIM_ADV_CR1_DIR, dir);
}

/*
 * selects center aligned mode.
 * (can be only selected before enabling the counter)
 */
void TIM_ADV_voidSelectCenterAlignedMode(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_CenterAlignedMode_t mode)
{
	EDT_REG(TIM_ADV[unitNumber]->CR1, TIM_ADV_CR1_CMS_0, mode, 2);
}

/*
 * enables auto reload preload
 * (thus "ARR" register is buffered to the shadow register,
 * and affects after next update event)
 */
void TIM_ADV_voidEnableAutoReloadPreload(TIM_ADV_UnitNumber_t unitNumber)
{
	SET_BIT(TIM_ADV[unitNumber]->CR1, TIM_ADV_CR1_ARPE);
}

/*
 * disables auto reload preload
 * (thus "ARR" register is not buffered, and affects immediately)
 */
void TIM_ADV_voidDisableAutoReloadPreload(TIM_ADV_UnitNumber_t unitNumber)
{
	CLR_BIT(TIM_ADV[unitNumber]->CR1, TIM_ADV_CR1_ARPE);
}

/*
 * selects dead-time and sampling clock Multiplier by the internal clock input.
 * (t_DTS = tCK_INT * Multiplier)
 */
void TIM_ADV_voidSelectDeadTimeMultiplier(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_DeadTimeMultiplier_t multiplier)
{
	EDT_REG(TIM_ADV[unitNumber]->CR1, TIM_ADV_CR1_CKD_0, multiplier, 2);
}

/*	selects slave mode (external clock input)	*/
void TIM_ADV_voidSelectSlaveMode(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_SlaveMode_t mode)
{
	EDT_REG(TIM_ADV[unitNumber]->SMCR, TIM_ADV_SMCR_SMS_0, mode, 3);
}

/*
 * selects trigger source.
 * (must be selected when slave mode is disabled, to avoid faulty triggering)
 */
void TIM_ADV_voidSelectTriggerSource(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_TriggerSource_t source)
{
	EDT_REG(TIM_ADV[unitNumber]->SMCR, TIM_ADV_SMCR_TS_0, source, 3);
}

/*
 * selects the external trigger filter sampling frequency,
 * and number of consecutive events needed to validate
 * a transition on trigger output (TRGO).
 */
void TIM_ADV_voidSelectExternalTriggerFilterMode(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Sampling_Mode_t mode)
{
	EDT_REG(TIM_ADV[unitNumber]->SMCR, TIM_ADV_SMCR_ETF_0, mode, 4);
}

/*
 * selects external trigger input prescaler.
 * (external trigger input must be prescaled to a frequency that is at most
 * equal to 25% of timer peripheral bus clock)
 */
void TIM_ADV_voidSelectExternalTriggerPrescaler(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_ExternalTriggerPrescaler_t prescaler)
{
	EDT_REG(TIM_ADV[unitNumber]->SMCR, TIM_ADV_SMCR_ETPS_0, prescaler, 2);
}

/*	enables external clock	*/
void TIM_ADV_voidEnableExternalClock(TIM_ADV_UnitNumber_t unitNumber)
{
	SET_BIT(TIM_ADV[unitNumber]->SMCR, TIM_ADV_SMCR_ECE);
}

/*	disables external clock	*/
void TIM_ADV_voidDisableExternalClock(TIM_ADV_UnitNumber_t unitNumber)
{
	CLR_BIT(TIM_ADV[unitNumber]->SMCR, TIM_ADV_SMCR_ECE);
}

/*	selects external trigger polarity	*/
void TIM_ADV_voidSelectExternalTriggerPolarity(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_ChannelPolarity_t polarity)
{
	WRT_BIT(TIM_ADV[unitNumber]->SMCR, TIM_ADV_SMCR_ETP, polarity);
}

/*	enables interrupt	*/
void TIM_ADV_voidEnableInterrupt(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Interrupt_t interrupt)
{
	SET_BIT(TIM_ADV[unitNumber]->DIER, interrupt);
}

/*	disables interrupt	*/
void TIM_ADV_voidDisableInterrupt(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Interrupt_t interrupt)
{
	CLR_BIT(TIM_ADV[unitNumber]->DIER, interrupt);
}

/*	enables DMA request	*/
void TIM_ADV_voidEnableDMA(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_DMA_t request)
{
	SET_BIT(TIM_ADV[unitNumber]->DIER, request);
}

/*	disables DMA request	*/
void TIM_ADV_voidDisableDMA(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_DMA_t request)
{
	CLR_BIT(TIM_ADV[unitNumber]->DIER, request);
}

/*	reads argumented status flag	*/
b8 TIM_ADV_b8ReadStatus(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Status_t status)
{
	return GET_BIT(TIM_ADV[unitNumber]->SR, status);
}

/*	clears argumented status flag	*/
void TIM_ADV_voidClearStatus(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Status_t status)
{
	SET_BIT(TIM_ADV[unitNumber]->SR, status);
}

/*	generates event by SW	*/
void TIM_ADV_voidSoftwareGenerateEvent(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Event_t event)
{
	SET_BIT(TIM_ADV[unitNumber]->EGR, event);
}

/*	selects capture/compare	*/
void TIM_ADV_voidSelectCaptureCompare(	TIM_ADV_UnitNumber_t unitNumber,
										TIM_ADV_CaptureCompareSelection_t selection,
										TIM_ADV_Cannel_t channel)
{
	EDT_REG(TIM_ADV[unitNumber]->CCMR[channel], TIM_ADV_CCMR_CCx_0, selection, 2);
}

/*	enable output compare fast	*/
void TIM_ADV_voidEnableOutputCompareFast(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Cannel_t channel)
{
	SET_BIT(TIM_ADV[unitNumber]->CCMR[channel], TIM_ADV_CCMR_OCxFE);
}

/*	disable output compare fast	*/
void TIM_ADV_voidDisableOutputCompareFast(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Cannel_t channel)
{
	CLR_BIT(TIM_ADV[unitNumber]->CCMR[channel], TIM_ADV_CCMR_OCxFE);
}

/*	enable output compare preload	*/
void TIM_ADV_voidEnableOutputComparePreload(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Cannel_t channel)
{
	SET_BIT(TIM_ADV[unitNumber]->CCMR[channel], TIM_ADV_CCMR_OCxPE);
}

/*	disable output compare preload	*/
void TIM_ADV_voidDisableOutputComparePreload(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Cannel_t channel)
{
	CLR_BIT(TIM_ADV[unitNumber]->CCMR[channel], TIM_ADV_CCMR_OCxPE);
}

/*	selects output compare mode	*/
void TIM_ADV_voidSelectCaptureCompareMode(	TIM_ADV_UnitNumber_t unitNumber,
											TIM_ADV_OutputCompareMode_t mode,
											TIM_ADV_Cannel_t channel)
{
	EDT_REG(TIM_ADV[unitNumber]->CCMR[channel], TIM_ADV_CCMR_OCxM_0, mode, 3);
}

/*	enable output compare clear	*/
void TIM_ADV_voidEnableOutputCompareClear(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Cannel_t channel)
{
	SET_BIT(TIM_ADV[unitNumber]->CCMR[channel], TIM_ADV_CCMR_OCxCE);
}

/*	disable output compare clear	*/
void TIM_ADV_voidDisableOutputCompareClear(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Cannel_t channel)
{
	CLR_BIT(TIM_ADV[unitNumber]->CCMR[channel], TIM_ADV_CCMR_OCxCE);
}

/*	select input capture prescaler	*/
void TIM_ADV_voidSelectInputCapturePrescaler(	TIM_ADV_UnitNumber_t unitNumber,
												TIM_ADV_InputCapturePrescaler_t prescaler,
												TIM_ADV_Cannel_t channel)
{
	EDT_REG(TIM_ADV[unitNumber]->CCMR[channel], TIM_ADV_CCMR_ICxPSC_0, prescaler, 3);
}

/*	select input capture filter	*/
void TIM_ADV_voidSelectInputCaptureFilter(	TIM_ADV_UnitNumber_t unitNumber,
											TIM_ADV_Sampling_Mode_t filter,
											TIM_ADV_Cannel_t channel)
{
	EDT_REG(TIM_ADV[unitNumber]->CCMR[channel], TIM_ADV_CCMR_ICxF_0, filter, 3);
}

/*	enables capture/compare channel	*/
void TIM_ADV_voidEnableCaptureCompareChannel(	TIM_ADV_UnitNumber_t unitNumber,
												TIM_ADV_Cannel_t channel)
{
	if (channel > TIM_ADV_Cannel_2)
		channel -= 2;

	SET_BIT(TIM_ADV[unitNumber]->CCER, TIM_ADV_CCER_CC1E + channel * 4);
}

/*	disables capture/compare channel	*/
void TIM_ADV_voidDisableCaptureCompareChannel(	TIM_ADV_UnitNumber_t unitNumber,
												TIM_ADV_Cannel_t channel)
{
	if (channel > TIM_ADV_Cannel_2)
		channel -= 2;

	CLR_BIT(TIM_ADV[unitNumber]->CCER, TIM_ADV_CCER_CC1E + channel * 4);
}

/*	selects capture/compare channel polarity	*/
void TIM_ADV_voidSelectCaptureCompareChannelPolarity(	TIM_ADV_UnitNumber_t unitNumber,
														TIM_ADV_Cannel_t channel,
														TIM_ADV_ChannelPolarity_t polarity)
{
	if (channel > TIM_ADV_Cannel_2)
		channel -= 2;

	WRT_BIT(TIM_ADV[unitNumber]->CCER, TIM_ADV_CCER_CC1P + channel * 4, polarity);
}

/*	enables capture/compare channel's complementary output	*/
void TIM_ADV_voidEnableCaptureCompareChannelComplementaryOutput(	TIM_ADV_UnitNumber_t unitNumber,
																	TIM_ADV_Cannel_t channel)
{
	if (channel > TIM_ADV_Cannel_2)
		channel -= 2;

	SET_BIT(TIM_ADV[unitNumber]->CCER, TIM_ADV_CCER_CC1NE + channel * 4);
}

/*	disables capture/compare channel's complementary output	*/
void TIM_ADV_voidDisableCaptureCompareChannelComplementaryOutput(	TIM_ADV_UnitNumber_t unitNumber,
																	TIM_ADV_Cannel_t channel)
{
	if (channel > TIM_ADV_Cannel_2)
		channel -= 2;

	CLR_BIT(TIM_ADV[unitNumber]->CCER, TIM_ADV_CCER_CC1NE + channel * 4);
}

/*	selects capture/compare channel's complementary output polarity	*/
void TIM_ADV_voidSelectCaptureCompareChannelComplementaryOutputPolarity(	TIM_ADV_UnitNumber_t unitNumber,
																			TIM_ADV_Cannel_t channel,
																			TIM_ADV_ChannelPolarity_t polarity)
{
	if (channel > TIM_ADV_Cannel_2)
		channel -= 2;

	WRT_BIT(TIM_ADV[unitNumber]->CCER, TIM_ADV_CCER_CC1NP + channel * 4, polarity);
}

/*	loads TCNT register	*/
void TIM_ADV_voidLoadTCNT(TIM_ADV_UnitNumber_t unitNumber, u16 value)
{
	TIM_ADV[unitNumber]->CNT = value;
}

/*	reads and returns value of TCNT register	*/
u16 TIM_ADV_u16ReadTCNT(TIM_ADV_UnitNumber_t unitNumber)
{
	return TIM_ADV[unitNumber]->CNT;
}

/*
 * writes prescaler register.
 * (The counter clock frequency (CK_CNT) is equal to fCK_PSC / (PSC[15:0] + 1))
 */
void TIM_ADV_voidLoadPrescalerRegister(TIM_ADV_UnitNumber_t unitNumber, u16 value)
{
	TIM_ADV[unitNumber]->PSC = value;
}

/*	writes ARR	*/
void TIM_ADV_voidLoadAutoReloadRegister(TIM_ADV_UnitNumber_t unitNumber, u16 value)
{
	TIM_ADV[unitNumber]->ARR = value;
}

/*	writes repetition counter	*/
void TIM_ADV_voidLoadRepetitionCounter(TIM_ADV_UnitNumber_t unitNumber, u8 value)
{
	TIM_ADV[unitNumber]->RCR = value;
}

/*	reads repetition counter	*/
u8 TIM_ADV_u8ReadRepetitionCounter(TIM_ADV_UnitNumber_t unitNumber)
{
	return TIM_ADV[unitNumber]->RCR;
}

#define GET_CCRX_ADDRESS(unitNumber, channel, address)	\
	switch (channel)									\
	{                                       			\
	case TIM_ADV_Cannel_1:                  			\
		address = &TIM_ADV[unitNumber]->CCR1;  			\
		break;                              			\
														\
	case TIM_ADV_Cannel_2:                  			\
		address = &TIM_ADV[unitNumber]->CCR2; 			\
		break;                              			\
														\
	case TIM_ADV_Cannel_3:                  			\
		address = &TIM_ADV[unitNumber]->CCR3; 			\
		break;                              			\
														\
	default:				                  			\
		address = &TIM_ADV[unitNumber]->CCR4; 			\
	}                                       			\

/*
 * loads capture/compare channel's register.
 * (valid only when channel is configured as output)
 */
void TIM_ADV_voidLoadCaptureCompareRegister(	TIM_ADV_UnitNumber_t unitNumber,
												TIM_ADV_Cannel_t channel,
												u16 value)
{
	volatile u32* ccrxPtr;
	GET_CCRX_ADDRESS(unitNumber, channel, ccrxPtr);

	*ccrxPtr = value;
}

/*	reads capture/compare channel's register	*/
u16 TIM_ADV_u16ReadCaptureCompareRegister(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Cannel_t channel)
{
	volatile u32* ccrxPtr;
	GET_CCRX_ADDRESS(unitNumber, channel, ccrxPtr);

	return (u16)(*ccrxPtr);
}

































