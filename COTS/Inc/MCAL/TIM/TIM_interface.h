/*
 * TIM_interface.h
 *
 *  Created on: Nov 7, 2022
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MCAL_TIM_TIM_INTERFACE_H_
#define INCLUDE_MCAL_TIM_TIM_INTERFACE_H_


/******************************************************************************
 * Callback functions.
 * (all of type void (void))
 *
 * N O T E : ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
 * so far it is implemented only for advanced timers.
 *****************************************************************************/
typedef enum{
	TIM_ADV_Vector_BRK,
	TIM_ADV_Vector_UP,
	TIM_ADV_Vector_TRG_COM,
	TIM_ADV_Vector_CC
}TIM_ADV_Vector_t;

/*	sets callback of an advanced timer unit	*/
void TIM_voidSetCallbackADV(
	u8 unitNumber, TIM_ADV_Vector_t vect, void(*callback)(void));

/******************************************************************************
 * Enums of timers and channels.
 *****************************************************************************/
typedef enum{
	TIM_ADV_UnitNumber_1 = 1,
	TIM_ADV_UnitNumber_8 = 8
}TIM_AD_UnitNumber_t;

typedef enum{
	TIM_GP_UnitNumber_2		= 2,
	TIM_GP_UnitNumber_3		= 3,
	TIM_GP_UnitNumber_4		= 4,
	TIM_GP_UnitNumber_5		= 8,
	TIM_GP_UnitNumber_9		= 9,
	TIM_GP_UnitNumber_10	= 10,
	TIM_GP_UnitNumber_11	= 11,
	TIM_GP_UnitNumber_12	= 12
}TIM_GP_UnitNumber_t;

typedef enum{
	TIM_BSC_UnitNumber_6 = 6,
	TIM_BSC_UnitNumber_7 = 7
}TIM_BSC_UnitNumber_t;

typedef enum{
	TIM_Channel_1	= 0,
	TIM_Channel_2	= 1,
	TIM_Channel_3	= 4,
	TIM_Channel_4	= 5
}TIM_Channel_t;


/******************************************************************************
 * update event sources.
 *
 * Available for all timers.
 *****************************************************************************/
typedef enum{
	/*
	 * update sources are:
	 * -	counter overflow / underflow.
	 * -	setting UG bit.
	 * -	update generation through slave mode controller. (for advanced
	 * 		timers and GPT's 2 to 5 and basic timers)
	 */
	TIM_UpdateSource_OVF_UNF_UG_SlaveModeController,
	/*
	 * update source is counter overflow / underflow only.
	 */
	TIM_UpdateSource_OVF_UNF
}TIM_UpdateSource_t;

/*
 * selects update source
 * (if update is enabled)
 */
void TIM_voidSetUpdateSource(u8 unitNumber, TIM_UpdateSource_t source);


/******************************************************************************
 * Counting direction.
 *
 * Available only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 *****************************************************************************/
typedef enum{
	TIM_CountDirection_Up,
	TIM_CountDirection_Down
}TIM_CountDirection_t;

/*
 * selects counting direction
 */
void TIM_voidSetCounterDirection(u8 unitNumber, TIM_CountDirection_t dir);


/******************************************************************************
 * Center aligned mode.
 *
 * Available only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 *****************************************************************************/
typedef enum{
	/*
	 * counter counts in a constant direction, based on
	 * "DIR" bit value.
	 */
	TIM_CenterAlignedMode_Disabled,
	/*
	 * counter counts up and down alternatively,
	 * output compare flag is set on the down count.
	 */
	TIM_CenterAlignedMode_1,
	/*
	 * counter counts up and down alternatively,
	 * output compare flag is set on the up count.
	 */
	TIM_CenterAlignedMode_2,
	/*
	 * counter counts up and down alternatively,
	 * output compare flag is set on both up and down counts.
	 */
	TIM_CenterAlignedMode_3
}TIM_CenterAlignedMode_t;

/*
 * selects center aligned mode.
 * (can be only selected before enabling the counter)
 */
void TIM_voidSetCenterAlignedMode(
	u8 unitNumber, TIM_CenterAlignedMode_t mode);


/******************************************************************************
 * Dead-time multiplier.
 *
 * Available only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 * - GPT's 9 and 12.
 * - GPT's 10/11/13/14.
 *****************************************************************************/
typedef enum{
	TIM_DeadTimeMultiplier_1,
	TIM_DeadTimeMultiplier_2,
	TIM_DeadTimeMultiplier_4
}TIM_DeadTimeMultiplier_t;

/*
 * selects dead-time and sampling clock Multiplier by the internal clock input.
 * (t_DTS = tCK_INT * Multiplier)
 */
void TIM_voidSetDeadTimeMultiplier(
	u8 unitNumber, TIM_DeadTimeMultiplier_t multiplier);

/******************************************************************************
 * Slave mode.
 *
 * Available only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 * - GPT's 9 and 12.
 *****************************************************************************/
typedef enum{
	/*	clocks prescaler directly by internal clock	*/
	TIM_SlaveMode_Disabled,
	/*	*/
	TIM_SlaveMode_Encoder1,
	/*	*/
	TIM_SlaveMode_Encoder2,
	/*	*/
	TIM_SlaveMode_Encoder3,
	/*	Reinitializes counter and generates an update on "TRGI" rising edge	*/
	TIM_SlaveMode_Reset,
	/*
	 * enables counter clock only when "TRGI" is high,
	 * disables it again when "TRGI" becomes low.
	 */
	TIM_SlaveMode_Gated,
	/*	enables the counter on the rising edge of "TRGI".	*/
	TIM_SlaveMode_Trigger,
	/*	clocks the counter with the "TRGI" rising edge.	*/
	TIM_SlaveMode_ExternalClock
}TIM_SlaveMode_t;

/*
 * selects slave mode (external clock input)
 */
void TIM_voidSetSlaveMode(u8 unitNumber, TIM_SlaveMode_t mode);

/******************************************************************************
 * Trigger source.
 *
 * Available only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 * - GPT's 9 and 12.
 *****************************************************************************/
typedef enum{
	TIM_TriggerSource_ITR0,
	TIM_TriggerSource_ITR1,
	TIM_TriggerSource_ITR2,
	TIM_TriggerSource_ITR3,
	TIM_TriggerSource_TI1F_ED,
	TIM_TriggerSource_TI1FP1,
	TIM_TriggerSource_TI2FP2,
}TIM_TriggerSource_t;

/*
 * selects trigger source.
 * (must be selected when slave mode is disabled, to avoid faulty triggering)
 */
void TIM_voidSetTriggerSource(u8 unitNumber, TIM_TriggerSource_t source);


/******************************************************************************
 * External clock enable control
 *
 * Available only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 *****************************************************************************/
/*	enables external clock	*/
void TIM_voidEnableExternalClock(u8 unitNumber);

/*	disables external clock	*/
void TIM_voidDisableExternalClock(u8 unitNumber);


/******************************************************************************
 * External trigger filter / sampling mode
 *
 * Available for trigger filter mode selection only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 *
 * Available for capture input filter only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 * - GPT's 9 and 12.
 * - GPT's 10/11/13/14.
 *****************************************************************************/
/*
 * selects f_sampling, and N.
 * used for:
 * 	- External Trigger input (ETRF).
 * 	- Input capture filter.
 */
typedef enum{
	TIM_SamplingMode_F_DeadTimeBy1_N_1,		//	i.e.: no filter
	TIM_SamplingMode_F_Internal_N_2,
	TIM_SamplingMode_F_Internal_N_4,
	TIM_SamplingMode_F_Internal_N_8,
	TIM_SamplingMode_F_DeadTimeBy2_N_6,
	TIM_SamplingMode_F_DeadTimeBy2_N_8,
	TIM_SamplingMode_F_DeadTimeBy4_N_6,
	TIM_SamplingMode_F_DeadTimeBy4_N_8,
	TIM_SamplingMode_F_DeadTimeBy8_N_6,
	TIM_SamplingMode_F_DeadTimeBy8_N_8,
	TIM_SamplingMode_F_DeadTimeBy16_N_5,
	TIM_SamplingMode_F_DeadTimeBy16_N_6,
	TIM_SamplingMode_F_DeadTimeBy16_N_8,
	TIM_SamplingMode_F_DeadTimeBy32_N_5,
	TIM_SamplingMode_F_DeadTimeBy32_N_6,
	TIM_SamplingMode_F_DeadTimeBy32_N_8,
}TIM_SamplingMode_t;

/*
 * selects the external trigger filter sampling frequency,
 * and number of consecutive events needed to validate
 * a transition on trigger output (TRGO).
 */
void TIM_voidSetExternalTriggerFilterMode(
	u8 unitNumber, TIM_SamplingMode_t mode);

/*	select input capture filter	*/
void TIM_voidSetInputCaptureFilter(
	u8 unitNumber, TIM_Channel_t channelNumber, TIM_SamplingMode_t filter);


/******************************************************************************
 * External trigger prescaler
 *
 * Available for trigger filter mode selection only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 *****************************************************************************/
typedef enum{
	TIM_ExternalTriggerPrescaler_1,
	TIM_ExternalTriggerPrescaler_2,
	TIM_ExternalTriggerPrescaler_4,
	TIM_ExternalTriggerPrescaler_8
}TIM_ExternalTriggerPrescaler_t;

/*
 * selects external trigger input prescaler.
 * (external trigger input must be prescaled to a frequency that is at most
 * equal to 25% of timer peripheral bus clock)
 */
void TIM_voidSetExternalTriggerPrescaler(
	u8 unitNumber, TIM_ExternalTriggerPrescaler_t prescaler);


/******************************************************************************
 * Channel polarity.
 *
 * Available for external trigger polarity selection only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 *
 * Available for capture/compare channel polarity selection only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 * - GPT's 9 and 12.
 * - GPT's 10/11/13/14.
 *
 * Available for capture/compare channel complement polarity selection only in:
 * - Advanced timers.
 * - GPT's 9 and 12.
 * - GPT's 10/11/13/14.
 *****************************************************************************/
typedef enum{
	TIM_ChannelPolarity_ActiveHigh_RisingEdge,
	TIM_ChannelPolarity_ActiveLow_FallingEdge,
}TIM_ChannelPolarity_t;

/*	selects external trigger polarity	*/
void TIM_voidSetExternalTriggerPolarity(
	u8 unitNumber, TIM_ChannelPolarity_t polarity);

/*	selects capture/compare channel polarity	*/
void TIM_voidSetCaptureCompareChannelPolarity(
	u8 unitNumber, TIM_Channel_t channel, TIM_ChannelPolarity_t polarity);

/*	selects capture/compare channel's complementary output polarity	*/
void TIM_voidSetCaptureCompareChannelComplementaryOutputPolarity(
	u8 unitNumber, TIM_Channel_t channel, TIM_ChannelPolarity_t polarity);


/******************************************************************************
 * Update control.
 *
 * Available for all timers.
 *****************************************************************************/
/*	enable update event generation	*/
void TIM_voidEnableUpdate(u8 unitNumber);

/*	disable update event generation	*/
void TIM_voidDisableUpdate(u8 unitNumber);


/******************************************************************************
 * Interrupts.
 *
 * Availability of each interrupt event is described in the "TIM_Interrupt_t"
 * enum.
 *****************************************************************************/
typedef enum{
	TIM_Interrupt_Update,		//	Available in all timers.

	TIM_Interrupt_CC1,			//	Available in all timers except base timers
								//	(TIM6 & TIM7).

	TIM_Interrupt_CC2,			//	Available only in advanced timers, GPT's
								//	2 to 5, 9 and 12.

	TIM_Interrupt_CC3,			//	Available only in advanced timers, GPT's
								//	2 to 5.

	TIM_Interrupt_CC4,			//	Available only in advanced timers, GPT's
    							//	2 to 5.

	TIM_Interrupt_COM,			//	Available only in advanced timers.

	TIM_Interrupt_Trigger,      //	Available only in advanced timers, GPT's
    							//	2 to 5, 9 and 12.

	TIM_Interrupt_Break			//	Available only in advanced timers.
}TIM_Interrupt_t;

/*	enables interrupt	*/
void TIM_voidEnableInterrupt(u8 unitNumber, TIM_Interrupt_t interrupt);

/*	disables interrupt	*/
void TIM_voidDisableInterrupt(u8 unitNumber, TIM_Interrupt_t interrupt);

/*	sets interrupt callback function	*/
void TIM_voidSetInterruptCallback(
	u8 unitNumber, TIM_Interrupt_t interrupt, void (*callback)(void));

/*	generates interrupt event by SW	*/
void TIM_voidGenerateSoftwareInterruptEvent(
	u8 unitNumber, TIM_Interrupt_t event);


/******************************************************************************
 * DMA.
 *
 * Availability of DMA requests for different timers is as follows:
 * 	- Advanced timers (TIM1 & TIM8) and GP timers (TIM2 To TIM5) have all DMA
 * 	requests available.
 *
 *  - Basic timers (TIM6 & TIM7) have only DMA update request available.
 *
 *  - Other than the mentioned before, is not available.
 *****************************************************************************/
typedef enum{
	TIM_DMA_Update	= 8,
	TIM_DMA_CC1,
	TIM_DMA_CC2,
	TIM_DMA_CC3,
	TIM_DMA_CC4,
	TIM_DMA_COM,
	TIM_DMA_Trigger
}TIM_DMA_t;

/*	enables DMA request	*/
void TIM_voidEnableDMA(u8 unitNumber, TIM_DMA_t request);

/*	disables DMA request	*/
void TIM_voidDisableDMA(u8 unitNumber, TIM_DMA_t request);


/******************************************************************************
 * Status flags.
 *
 * Availability of each state flag is described in the "TIM_Status_t" enum.
 *****************************************************************************/
typedef enum{
	TIM_Status_Update,			//	Available in all timers.

	TIM_Status_CC1,         	//	Available in all timers except base timers
								//	(TIM6 & TIM7).

	TIM_Status_CC2,         	//	Available only in advanced timers, GPT's
    							//	2 to 5, 9 and 12.

	TIM_Status_CC3,         	//	Available only in advanced timers, GPT's
    							//	2 to 5.

	TIM_Status_CC4,         	//	Available only in advanced timers, GPT's
    							//	2 to 5.

	TIM_Status_COM,         	//	Available only in advanced timers.

	TIM_Status_Trigger,     	//	Available only in advanced timers, GPT's
    							//	2 to 5, 9 and 12.

	TIM_Status_Break,       	//	Available only in advanced timers.

	TIM_Status_CC1OvrCptr = 9,  //	Available in all timers except base timers
    							//	(TIM6 & TIM7).

	TIM_Status_CC2OvrCptr,      //	Available only in advanced timers, GPT's
    							//	2 to 5, 9 and 12.

	TIM_Status_CC3OvrCptr,      //	Available only in advanced timers, GPT's
    							//	2 to 5.

	TIM_Status_CC4OvrCptr       //	Available only in advanced timers, GPT's
    							//	2 to 5.
}TIM_Status_t;

/*	returns state of argumented status flag	*/
b8 TIM_b8GetStatusFlag(u8 unitNumber, TIM_Status_t status);

/*	clears state of argumented status flag	*/
void TIM_voidClearStatusFlag(u8 unitNumber, TIM_Status_t status);


/******************************************************************************
 * Capture compare selection.
 *
 * Availability depends on that of channels. Refer to "TIM_Interrupt_t" enum
 * for more information.
 *****************************************************************************/
typedef enum{
	TIM_CaptureCompareSelection_Output,
	TIM_CaptureCompareSelection_Input_TI_NPlus1,	//	'N' is the channel num.
	TIM_CaptureCompareSelection_Input_TI_N,
	TIM_CaptureCompareSelection_Input_TRC,	// works only if an internal trigger
											// input is selected first.
}TIM_CaptureCompareSelection_t;

/*	selects capture/compare	*/
void TIM_voidSetCaptureCompareSelection(
	u8 unitNumber, TIM_Channel_t channel,
	TIM_CaptureCompareSelection_t selection);


/******************************************************************************
 * Input capture prescaler.
 *
 * Availability depends on that of channels. Refer to "TIM_Interrupt_t" enum
 * for more information.
 *****************************************************************************/
typedef enum{
	TIM_InputCapturePrescaler_1,
	TIM_InputCapturePrescaler_2,
	TIM_InputCapturePrescaler_4,
	TIM_InputCapturePrescaler_8
}TIM_InputCapturePrescaler_t;

/*	select input capture prescaler	*/
void TIM_voidSetInputCapturePrescaler(
	u8 unitNumber, TIM_Channel_t channel,
	TIM_InputCapturePrescaler_t prescaler);


/******************************************************************************
 * Lock configuration.
 *
 * Available only in advanced timers (TIM1 & TIM8).
 *****************************************************************************/
typedef enum{
	TIM_LockConfiguration_Off,
	TIM_LockConfiguration_Level1,
	TIM_LockConfiguration_Level2,
	TIM_LockConfiguration_Level3
}TIM_LockConfiguration_t;

/*
 * selects lock configuration in "TIMx_BDTR".
 *
 * (the lock bits are one time writable per system reset).
 */
void TIM_voidSetLockConfiguration(u8 unitNumber, TIM_LockConfiguration_t lock);


/******************************************************************************
 * Counter control.
 *
 * All of the following functions are available for all timers, except the ones
 * mentioned below:
 * 	- "TIM_voidSetRepetitionCounter()" for advanced timers only.
 * 	- "TIM_u8GetRepetitionCounter()" for advanced timers only.
 *****************************************************************************/
/*	enable counter	*/
void TIM_voidEnableCounter(u8 unitNumber);

/*	disable counter	*/
void TIM_voidDisableCounter(u8 unitNumber);

/*	loads CNT register	*/
void TIM_voidSetCNT(u8 unitNumber, u16 value);

/*	reads and returns value of CNT register	*/
u16 TIM_u16GetCNT(u8 unitNumber);

/*
 * writes prescaler register.
 * (The counter clock frequency (CK_CNT) is equal to fCK_PSC / (PSC[15:0] + 1))
 */
void TIM_voidSetPrescalerRegister(u8 unitNumber, u16 value);

/*
 * writes Auto reload register.
 * ARR is the value that the counter counts up to, then generates an OVF,
 * (or counts from it to 0 in case of down counting).
 */
void TIM_voidSetARR(u8 unitNumber, u16 value);

/*	writes repetition counter	*/
void TIM_voidSetRepetitionCounter(u8 unitNumber, u8 value);

/*	reads repetition counter	*/
u8 TIM_u8GetRepetitionCounter(u8 unitNumber);

/*
 * enables auto reload preload
 * (thus "ARR" register is buffered to the shadow register, and affects after
 * next update event).
 */
void TIM_voidEnableAutoReloadPreload(u8 unitNumber);

/*	disables auto reload preload	*/
void TIM_voidDisableAutoReloadPreload(u8 unitNumber);

/*	enables one pulse mode	*/
void TIM_voidEnableOnePulseMode(u8 unitNumber);

/*	disables one pulse mode	*/
void TIM_voidDisableOnePulseMode(u8 unitNumber);


/******************************************************************************
 * Output compare control.
 *
 * Availability depends on that of channels. Refer to "TIM_Interrupt_t" enum
 * for more information. Refer to "Channel polarity" section above for
 * information about availability of complementary outputs.
 *
 * "TIM_voidEnableMainOutput()", "TIM_voidDisableMainOutput()" and
 * "TIM_b8IsMainOutputEnabled()" are only available for advanced timers.
 *****************************************************************************/
typedef enum{
	TIM_OutputCompareMode_KeepLevel,
	TIM_OutputCompareMode_SetActive,
	TIM_OutputCompareMode_SetInactive,
	TIM_OutputCompareMode_Toggle,
	TIM_OutputCompareMode_ForceInactive,
	TIM_OutputCompareMode_ForceActive,
	TIM_OutputCompareMode_PWM1,
	TIM_OutputCompareMode_PWM2
}TIM_OutputCompareMode_t;

/*	selects output compare mode	*/
void TIM_voidSetOutputCompareMode(
	u8 unitNumber, TIM_Channel_t channel, TIM_OutputCompareMode_t mode);

/*	enable output compare fast	*/
void TIM_voidEnableOutputCompareFast(u8 unitNumber, TIM_Channel_t channel);

/*	disable output compare fast	*/
void TIM_voidDisableOutputCompareFast(u8 unitNumber, TIM_Channel_t channel);

/*	enable output compare preload	*/
void TIM_voidEnableOutputComparePreload(u8 unitNumber, TIM_Channel_t channel);

/*	disable output compare preload	*/
void TIM_voidDisableOutputComparePreload(u8 unitNumber, TIM_Channel_t channel);

/*	enable output compare clear	*/
void TIM_voidEnableOutputCompareClear(u8 unitNumber, TIM_Channel_t channel);

/*	disable output compare clear	*/
void TIM_voidDisableOutputCompareClear(u8 unitNumber, TIM_Channel_t channel);

/*
 * loads capture/compare channel's register.
 * (valid only when channel is configured as output)
 */
void TIM_voidSetCCR(u8 unitNumber, TIM_Channel_t channel, u16 value);

/*	enables capture/compare channel's complementary output	*/
void TIM_voidEnableCaptureCompareChannelComplementaryOutput(
	u8 unitNumber, TIM_Channel_t channel);

/*	disables capture/compare channel's complementary output	*/
void TIM_voidDisableCaptureCompareChannelComplementaryOutput(
	u8 unitNumber, TIM_Channel_t channel);

/*
 * when main output is enabled, OCx and OCNx are connected to their respective
 * CC configuration, otherwise they are in idle state.
 * notice that main output is: "asynchronously by hardware as soon as the break
 * input is active. It is set by software or automatically depending on the
 * Automatic output enable state".
 */
void TIM_voidEnableMainOutput(u8 unitNumber);

void TIM_voidDisableMainOutput(u8 unitNumber);

b8 TIM_b8IsMainOutputEnabled(u8 unitNumber);


/******************************************************************************
 * Capture compare control.
 *
 * Availability depends on that of channels. Refer to "TIM_Interrupt_t" enum
 * for more information.
 *****************************************************************************/
/*	reads capture/compare channel's register	*/
u16 TIM_u16GetCCR(u8 unitNumber, TIM_Channel_t channel);

/*	returns port and pin of certain AFIO mapping	*/
u8 TIM_u8GetPortAndPin(u8 unitNumber, TIM_Channel_t ch, u8 map);

/*	inits channel pin as input	*/
void TIM_voidInitInputPin(u8 unitNumber, TIM_Channel_t ch, u8 map);


/******************************************************************************
 * Capture compare channel enable.
 *
 * Availability depends on that of channels. Refer to "TIM_Interrupt_t" enum
 * for more information.
 *****************************************************************************/
/*	enables capture/compare channel	*/
void TIM_voidEnableCaptureCompareChannel(u8 unitNumber, TIM_Channel_t channel);

/*	disables capture/compare channel	*/
void TIM_voidDisableCaptureCompareChannel(u8 unitNumber, TIM_Channel_t channel);


/******************************************************************************
 * Signal generation.
 *
 * Availability depends on that of channels. Refer to "TIM_Interrupt_t" enum
 * for more information.
 *****************************************************************************/
/*
 * returns CLK_INT in Hz.
 *
 * same for both timer 1 and timer 8.
 */
u32 TIM_u32GetClockInternalInput(u8 unitNumber);

/*
 * sets prescaler of timer unit, to achieve certain overflow frequency.
 * calls error handler if requested frequency is not achievable.
 *
 * assumes that clock source is internal clock source, as it's the most common
 * when using PWM.
 *
 * returns actual running overflow frequency in milli-Hz.
 */
u64 TIM_u64SetFreq(u8 unitNumber, u16 freqHz);

/*
 * inits channel as PWM output, configures and connects channel's non inverting
 * GPIO pin.
 * returns actual running overflow frequency in milli-Hz.
 */
u64 TIM_u64InitPWM(u8 unitNumber, TIM_Channel_t ch, u16 freqHz);

/*
 * configures and connects channel's non inverting GPIO pin.
 *
 * notice that output compare channels connected to GP pins are only the
 * following:
 *
 */
void TIM_voidInitOutputPin(u8 unitNumber, TIM_Channel_t ch, u8 map);

/*
 * sets active duty cycle of PWM signal.
 *
 * for less overhead, channel must be configured to PWM1 mode, with an active
 * high polarity fist, (which happens in the function: "TIM_ADV_voidInitPWM()")
 */
void TIM_voidSetDutyCycle(u8 unitNumber, TIM_Channel_t ch, u16 duty);


#endif /* INCLUDE_MCAL_TIM_TIM_INTERFACE_H_ */
