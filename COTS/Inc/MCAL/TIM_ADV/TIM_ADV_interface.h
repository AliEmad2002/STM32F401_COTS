/*
 * TIM_ADV_interface.h
 *
 *  Created on: Oct 10, 2022
 *      Author: Ali Emad Ali
 */

#ifndef _TIM_ADV_TIM_ADV_INTERFACE_H_
#define _TIM_ADV_TIM_ADV_INTERFACE_H_


typedef enum{
	TIM_ADV_UnitNumber_1,
	TIM_ADV_UnitNumber_8
}TIM_ADV_UnitNumber_t;

typedef enum{
	/*
	 * update sources are:
	 * -	counter overflow / underflow.
	 * -	setting UG bit.
	 * -	update generation through slave mode controller.
	 */
	TIM_ADV_UpdateSource_OVF_UNF_UG_SlaveModeController,
	/*
	 * update source is counter overflow / underflow only.
	 */
	TIM_ADV_UpdateSource_OVF_UNF
}TIM_ADV_UpdateSource_t;

typedef enum{
	TIM_ADV_CountDirection_Up,
	TIM_ADV_CountDirection_Down
}TIM_ADV_CountDirection_t;

typedef enum{
	/*
	 * counter counts in a constant direction, based on
	 * "DIR" bit value.
	 */
	TIM_ADV_CenterAlignedMode_Disabled,
	/*
	 * counter counts up and down alternatively,
	 * output compare flag is set on the down count.
	 */
	TIM_ADV_CenterAlignedMode_1,
	/*
	 * counter counts up and down alternatively,
	 * output compare flag is set on the up count.
	 */
	TIM_ADV_CenterAlignedMode_2,
	/*
	 * counter counts up and down alternatively,
	 * output compare flag is set on both up and down counts.
	 */
	TIM_ADV_CenterAlignedMode_3
}TIM_ADV_CenterAlignedMode_t;

typedef enum{
	TIM_ADV_DeadTimeMultiplier_1,
	TIM_ADV_DeadTimeMultiplier_2,
	TIM_ADV_DeadTimeMultiplier_4
}TIM_ADV_DeadTimeMultiplier_t;

typedef enum{
	/*	clocks prescaler directly by internal clock	*/
	TIM_ADV_SlaveMode_Disabled,
	/*	*/
	TIM_ADV_SlaveMode_Encoder1,
	/*	*/
	TIM_ADV_SlaveMode_Encoder2,
	/*	*/
	TIM_ADV_SlaveMode_Encoder3,
	/*	Reinitializes counter and generates an update on "TRGI" rising edge	*/
	TIM_ADV_SlaveMode_Reset,
	/*
	 * enables counter clock only when "TRGI" is high,
	 * disables it again when "TRGI" becomes low.
	 */
	TIM_ADV_SlaveMode_Gated,
	/*	enables the counter on the rising edge of "TRGI".	*/
	TIM_ADV_SlaveMode_Trigger,
	/*	clocks the counter with the "TRGI" rising edge.	*/
	TIM_ADV_SlaveMode_ExternalClock
}TIM_ADV_SlaveMode_t;

typedef enum{
	TIM_ADV_TriggerSource_ITR0,
	TIM_ADV_TriggerSource_ITR1,
	TIM_ADV_TriggerSource_ITR2,
	TIM_ADV_TriggerSource_ITR3,
	TIM_ADV_TriggerSource_TI1F_ED,
	TIM_ADV_TriggerSource_TI1FP1,
	TIM_ADV_TriggerSource_TI2FP2,
}TIM_ADV_TriggerSource_t;

/*
 * selects f_sampling, and N.
 * used for:
 * 	- External Trigger input (ETRF).
 * 	- Input capture filter.
 *
 */
typedef enum{
	TIM_ADV_Sampling_F_Internal_N_2,
	TIM_ADV_Sampling_F_Internal_N_4,
	TIM_ADV_Sampling_F_Internal_N_8,
	TIM_ADV_Sampling_F_DeadTimeBy2_N_6,
	TIM_ADV_Sampling_F_DeadTimeBy2_N_8,
	TIM_ADV_Sampling_F_DeadTimeBy4_N_6,
	TIM_ADV_Sampling_F_DeadTimeBy4_N_8,
	TIM_ADV_Sampling_F_DeadTimeBy8_N_6,
	TIM_ADV_Sampling_F_DeadTimeBy8_N_8,
	TIM_ADV_Sampling_F_DeadTimeBy16_N_5,
	TIM_ADV_Sampling_F_DeadTimeBy16_N_6,
	TIM_ADV_Sampling_F_DeadTimeBy16_N_8,
	TIM_ADV_Sampling_F_DeadTimeBy32_N_5,
	TIM_ADV_Sampling_F_DeadTimeBy32_N_6,
	TIM_ADV_Sampling_F_DeadTimeBy32_N_8,
}TIM_ADV_Sampling_Mode_t;

typedef enum{
	TIM_ADV_ExternalTriggerPrescaler_1,
	TIM_ADV_ExternalTriggerPrescaler_2,
	TIM_ADV_ExternalTriggerPrescaler_4,
	TIM_ADV_ExternalTriggerPrescaler_8
}TIM_ADV_ExternalTriggerPrescaler_t;

typedef enum{
	TIM_ADV_ChannelPolarity_ActiveHigh_RisingEdge,
	TIM_ADV_ChannelPolarity_ActiveLow_FallingEdge,
}TIM_ADV_ChannelPolarity_t;

typedef enum{
	TIM_ADV_Interrupt_Update,
	TIM_ADV_Interrupt_CC1,
	TIM_ADV_Interrupt_CC2,
	TIM_ADV_Interrupt_CC3,
	TIM_ADV_Interrupt_CC4,
	TIM_ADV_Interrupt_COM,
	TIM_ADV_Interrupt_Trigger,
	TIM_ADV_Interrupt_Break
}TIM_ADV_Interrupt_t;

typedef enum{
	TIM_ADV_DMA_Update	= 8,
	TIM_ADV_DMA_CC1,
	TIM_ADV_DMA_CC2,
	TIM_ADV_DMA_CC3,
	TIM_ADV_DMA_CC4,
	TIM_ADV_DMA_COM,
	TIM_ADV_DMA_Trigger
}TIM_ADV_DMA_t;

typedef enum{
	TIM_ADV_Status_Update,
	TIM_ADV_Status_CC1,
	TIM_ADV_Status_CC2,
	TIM_ADV_Status_CC3,
	TIM_ADV_Status_CC4,
	TIM_ADV_Status_COM,
	TIM_ADV_Status_Trigger,
	TIM_ADV_Status_Break,
	TIM_ADV_Status_CC1OverCapture = 9
}TIM_ADV_Status_t;

typedef enum{
	TIM_ADV_Event_Update,
	TIM_ADV_Event_CC1,
	TIM_ADV_Event_CC2,
	TIM_ADV_Event_CC3,
	TIM_ADV_Event_CC4,
	TIM_ADV_Event_COM,
	TIM_ADV_Event_Trigger,
	TIM_ADV_Event_Break
}TIM_ADV_Event_t;

typedef enum{
	TIM_CaptureCompareSelection_Output,
	TIM_CaptureCompareSelection_Input_TI2,
	TIM_CaptureCompareSelection_Input_TI1,
	/*	works only if an internal trigger input is selected first.	*/
	TIM_CaptureCompareSelection_Input_TRC,
}TIM_ADV_CaptureCompareSelection_t;

typedef enum{
	TIM_ADV_Cannel_1	= 0,
	TIM_ADV_Cannel_2	= 1,
	TIM_ADV_Cannel_3	= 4,
	TIM_ADV_Cannel_4	= 5
}TIM_ADV_Cannel_t;

typedef enum{
	TIM_ADV_OutputCompareMode_KeepLevel,
	TIM_ADV_OutputCompareMode_SetActive,
	TIM_ADV_OutputCompareMode_SetInactive,
	TIM_ADV_OutputCompareMode_Toggle
}TIM_ADV_OutputCompareMode_t;

typedef enum{
	TIM_ADV_InputCapturePrescaler_1,
	TIM_ADV_InputCapturePrescaler_2,
	TIM_ADV_InputCapturePrescaler_4,
	TIM_ADV_InputCapturePrescaler_8
}TIM_ADV_InputCapturePrescaler_t;


/*	enable counter	*/
void TIM_ADV_voidEnableCounter(TIM_ADV_UnitNumber_t unitNumber);

/*	disable counter	*/
void TIM_ADV_voidDisableCounter(TIM_ADV_UnitNumber_t unitNumber);

/*	enable update event generation	*/
void TIM_ADV_voidEnableUpdateEvent(TIM_ADV_UnitNumber_t unitNumber);

/*	disable update event generation	*/
void TIM_ADV_voidDisableUpdateEvent(TIM_ADV_UnitNumber_t unitNumber);

/*
 * selects update source
 * (if update is enabled)
 */
void TIM_ADV_voidSelectUpdateSource(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_UpdateSource_t source);

/*	enables one pulse mode	*/
void TIM_ADV_voidEnableOnePulseMode(TIM_ADV_UnitNumber_t unitNumber);

/*	disables one pulse mode	*/
void TIM_ADV_voidDisableOnePulseMode(TIM_ADV_UnitNumber_t unitNumber);

/*	selects counting direction	*/
void TIM_ADV_voidSelectCountDirection(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_CountDirection_t dir);

/*
 * selects center aligned mode.
 * (can be only selected before enabling the counter)
 */
void TIM_ADV_voidSelectCenterAlignedMode(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_CenterAlignedMode_t mode);

/*	enables auto reload preload	*/
void TIM_ADV_voidEnableAutoReloadPreload(TIM_ADV_UnitNumber_t unitNumber);

/*	disables auto reload preload	*/
void TIM_ADV_voidDisableAutoReloadPreload(TIM_ADV_UnitNumber_t unitNumber);

/*
 * selects dead-time and sampling clock Multiplier by the internal clock input.
 * (t_DTS = tCK_INT * Multiplier)
 */
void TIM_ADV_voidSelectDeadTimeMultiplier(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_DeadTimeMultiplier_t multiplier);

/*	selects slave mode (external clock input)	*/
void TIM_ADV_voidSelectSlaveMode(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_SlaveMode_t mode);

/*
 * selects trigger source.
 * (must be selected when slave mode is disabled, to avoid faulty triggering)
 */
void TIM_ADV_voidSelectTriggerSource(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_TriggerSource_t source);

/*
 * selects the external trigger filter sampling frequency,
 * and number of consecutive events needed to validate
 * a transition on trigger output (TRGO).
 */
void TIM_ADV_voidSelectExternalTriggerFilterMode(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Sampling_Mode_t mode);

/*
 * selects external trigger input prescaler.
 * (external trigger input must be prescaled to a frequency that is at most
 * equal to 25% of timer peripheral bus clock)
 */
void TIM_ADV_voidSelectExternalTriggerPrescaler(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_ExternalTriggerPrescaler_t prescaler);

/*	enables external clock	*/
void TIM_ADV_voidEnableExternalClock(TIM_ADV_UnitNumber_t unitNumber);

/*	disables external clock	*/
void TIM_ADV_voidDisableExternalClock(TIM_ADV_UnitNumber_t unitNumber);

/*	selects external trigger polarity	*/
void TIM_ADV_voidSelectExternalTriggerPolarity(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_ChannelPolarity_t polarity);

/*	enables interrupt	*/
void TIM_ADV_voidEnableInterrupt(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Interrupt_t interrupt);

/*	disables interrupt	*/
void TIM_ADV_voidDisableInterrupt(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Interrupt_t interrupt);

/*	enables DMA request	*/
void TIM_ADV_voidEnableDMA(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_DMA_t request);

/*	disables DMA request	*/
void TIM_ADV_voidDisableDMA(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_DMA_t request);

/*	reads argumented status flag	*/
b8 TIM_ADV_b8ReadStatus(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Status_t status);

/*	clears argumented status flag	*/
void TIM_ADV_voidClearStatus(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Status_t status);

/*	generates event by SW	*/
void TIM_ADV_voidSoftwareGenerateEvent(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Event_t event);

/*	selects capture/compare	*/
void TIM_ADV_voidSelectCaptureCompare(	TIM_ADV_UnitNumber_t unitNumber,
										TIM_ADV_CaptureCompareSelection_t selection,
										TIM_ADV_Cannel_t channel);

/*	enable output compare fast	*/
void TIM_ADV_voidEnableOutputCompareFast(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Cannel_t channel);

/*	disable output compare fast	*/
void TIM_ADV_voidDisableOutputCompareFast(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Cannel_t channel);

/*	enable output compare preload	*/
void TIM_ADV_voidEnableOutputComparePreload(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Cannel_t channel);

/*	disable output compare preload	*/
void TIM_ADV_voidDisableOutputComparePreload(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Cannel_t channel);

/*	selects output compare mode	*/
void TIM_ADV_voidSelectCaptureCompareMode(	TIM_ADV_UnitNumber_t unitNumber,
											TIM_ADV_OutputCompareMode_t mode,
											TIM_ADV_Cannel_t channel);

/*	enable output compare clear	*/
void TIM_ADV_voidEnableOutputCompareClear(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Cannel_t channel);

/*	disable output compare clear	*/
void TIM_ADV_voidDisableOutputCompareClear(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Cannel_t channel);

/*	select input capture prescaler	*/
void TIM_ADV_voidSelectInputCapturePrescaler(	TIM_ADV_UnitNumber_t unitNumber,
												TIM_ADV_InputCapturePrescaler_t prescaler,
												TIM_ADV_Cannel_t channel);

/*	select input capture filter	*/
void TIM_ADV_voidSelectInputCaptureFilter(	TIM_ADV_UnitNumber_t unitNumber,
											TIM_ADV_Sampling_Mode_t filter,
											TIM_ADV_Cannel_t channel);

/*	enables capture/compare channel	*/
void TIM_ADV_voidEnableCaptureCompareChannel(	TIM_ADV_UnitNumber_t unitNumber,
												TIM_ADV_Cannel_t channel);

/*	disables capture/compare channel	*/
void TIM_ADV_voidDisableCaptureCompareChannel(	TIM_ADV_UnitNumber_t unitNumber,
												TIM_ADV_Cannel_t channel);

/*	selects capture/compare channel polarity	*/
void TIM_ADV_voidSelectCaptureCompareChannelPolarity(	TIM_ADV_UnitNumber_t unitNumber,
														TIM_ADV_Cannel_t channel,
														TIM_ADV_ChannelPolarity_t polarity);

/*	enables capture/compare channel's complementary output	*/
void TIM_ADV_voidEnableCaptureCompareChannelComplementaryOutput(	TIM_ADV_UnitNumber_t unitNumber,
																	TIM_ADV_Cannel_t channel);

/*	disables capture/compare channel's complementary output	*/
void TIM_ADV_voidDisableCaptureCompareChannelComplementaryOutput(	TIM_ADV_UnitNumber_t unitNumber,
																	TIM_ADV_Cannel_t channel);

/*	selects capture/compare channel's complementary output polarity	*/
void TIM_ADV_voidSelectCaptureCompareChannelComplementaryOutputPolarity(	TIM_ADV_UnitNumber_t unitNumber,
																			TIM_ADV_Cannel_t channel,
																			TIM_ADV_ChannelPolarity_t polarity);

/*	loads TCNT register	*/
void TIM_ADV_voidLoadTCNT(TIM_ADV_UnitNumber_t unitNumber, u16 value);

/*	reads and returns value of TCNT register	*/
u16 TIM_ADV_u16ReadTCNT(TIM_ADV_UnitNumber_t unitNumber);

/*
 * writes prescaler register.
 * (The counter clock frequency (CK_CNT) is equal to fCK_PSC / (PSC[15:0] + 1))
 */
void TIM_ADV_voidLoadPrescalerRegister(TIM_ADV_UnitNumber_t unitNumber, u16 value);

/*	writes ARR	*/
void TIM_ADV_voidLoadAutoReloadRegister(TIM_ADV_UnitNumber_t unitNumber, u16 value);

/*	writes repetition counter	*/
void TIM_ADV_voidLoadRepetitionCounter(TIM_ADV_UnitNumber_t unitNumber, u8 value);

/*	reads repetition counter	*/
u8 TIM_ADV_u8ReadRepetitionCounter(TIM_ADV_UnitNumber_t unitNumber);

/*
 * loads capture/compare channel's register.
 * (valid only when channel is configured as output)
 */
void TIM_ADV_voidLoadCaptureCompareRegister(	TIM_ADV_UnitNumber_t unitNumber,
												TIM_ADV_Cannel_t channel,
												u16 value);

/*	reads capture/compare channel's register	*/
u16 TIM_ADV_u16ReadCaptureCompareRegister(TIM_ADV_UnitNumber_t unitNumber, TIM_ADV_Cannel_t channel);

/*	still last 3 registers!!!!!!!!!!!!!	*/
















#endif /* _TIM_ADV_TIM_ADV_INTERFACE_H_ */
