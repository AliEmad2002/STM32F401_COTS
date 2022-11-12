/*
 * TIM_program.c
 *
 *  Created on: Nov 7, 2022
 *      Author: Ali Emad Ali
 */


/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Error_Handler_interface.h"
#include "Debug_active.h"
#include "diag/trace.h"

/*	MCAL	*/
#include "RCC_interface.h"
#include "GPIO_interface.h"

/*	SELF	*/
#include "TIM_interface.h"
#include "TIM_private.h"


/******************************************************************************
 * Callback functions.
 * (all of type void (void))
 *****************************************************************************/



/******************************************************************************
 * Range check macros
 *****************************************************************************/
/*
 * checks if "unitNumber" is a valid timer unit number.
 * "unitNumber" is of type "u8".
 */
#define IS_TIM_UNIT_NUMBER(unitNumber)		\
	((0 < (unitNumber)  &&  (unitNumber) <= 14) ?  true  :  false)

/* checks if "unitNumber" is an advanced timer.
 * "unitNumber" is of type "u8".
 */
#define IS_ADV(unitNumber)  							\
	(((unitNumber) == 1  ||  (unitNumber) == 8) ?  true  :  false)

/* checks if "unitNumber" is a GPT in the range 2 to 5.
 * "unitNumber" is of type "u8".
 */
#define IS_GP_2_TO_5(unitNumber)  							\
	((2 <= (unitNumber)  ||  (unitNumber) <= 5) ?  true  :  false)

/*
 * checks if "unitNumber" is in range of advanced timers or GPT's, all GPT's.
 * "unitNumber" is of type "u8".
 */
#define IS_ADV_GP(unitNumber)  							\
	(((0 < (unitNumber)  &&  (unitNumber) <= 5) ||  	\
	(8 <= (unitNumber)  &&  (unitNumber) <= 14)) ?  true  :  false)

/*
 * checks if "unitNumber" is in range of advanced timers or GPT's 2 to 5.
 * "unitNumber" is of type "u8".
 */
#define IS_ADV_GP_2_TO_5(unitNumber)  					\
	(((0 < (unitNumber)  &&  (unitNumber) <= 5) ||  	\
	(unitNumber) == 8) ?  true  :  false)

/*
 * checks if "unitNumber" is one of GPT's 9 and 12.
 * "unitNumber" is of type "u8".
 */
#define IS_GP_9_AND_12(unitNumber)  						\
	(((unitNumber) == 9  ||  (unitNumber) == 12)  ?  true  :  false)

/*
 * checks if "unitNumber" is one of GPT's 10/11/13/14.
 * "unitNumber" is of type "u8".
 */
#define IS_GP_10_11_13_14(unitNumber)  						\
	(((unitNumber) == 10  ||  (unitNumber) == 11  ||		\
	  (unitNumber) == 13  ||  (unitNumber) == 14)  ?  true  :  false)

/*
 * checks if "unitNumber" is one of advanced timers, or GPT's 9 to 14.
 * "unitNumber" is of type "u8".
 */
#define IS_ADV_GP_9_TO_14(unitNumber)  						\
	(((unitNumber) == 1  ||  (8 <= (unitNumber)  &&  (unitNumber) <= 14)) \
		?true  :  false)

/*
 * checks if "unitNumber" is one of basic timers 6 & 7.
 * "unitNumber" is of type "u8".
 */
#define IS_BSC_6_AND_7(unitNumber)  						\
	(((unitNumber) == 6  ||  (unitNumber) == 7)  ?  true  :  false)

/*
 * checks if "unitNumber" is clocked by APB2 bus
 */
#define IS_ON_APB2(unitNumber)  						\
	(((8 <= (unitNumber)  &&  (unitNumber) <= 11) ||  	\
	(unitNumber) == 1) ?  true  :  false)


/******************************************************************************
 * Range check functions.
 *****************************************************************************/
/*
 * checks if "ch" is an available channel in the timer "unitNumber".
 */
static b8 TIM_b8IsChannelInTimer(u8 unitNumber, TIM_Channel_t ch)
{
	if (IS_ADV_GP_2_TO_5(unitNumber))
		return true;

	else if (IS_GP_9_AND_12(unitNumber))
		return (ch <= TIM_Channel_2);

	else if (IS_GP_10_11_13_14(unitNumber))
		return (ch == TIM_Channel_1);

	else if (IS_BSC_6_AND_7(unitNumber))
		return false;

	else
	{
		ErrorHandler_voidExecute(0);
		return false;
	}
}

/******************************************************************************
 * update event sources.
 *
 * Available for all timers.
 *****************************************************************************/
/*
 * selects update source
 * (if update is enabled)
 *
 * works for all timers.
 */
void TIM_voidSetUpdateSource(u8 unitNumber, TIM_UpdateSource_t source)
{
	if (IS_TIM_UNIT_NUMBER(unitNumber))
	{
		WRT_BIT(TIM[unitNumber]->CR1, TIM_CR1_URS, source);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/******************************************************************************
 * Counting direction.
 *
 * Available only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 *****************************************************************************/
/*
 * selects counting direction
 */
void TIM_voidSetCounterDirection(u8 unitNumber, TIM_CountDirection_t dir)
{
	if (IS_ADV_GP_2_TO_5(unitNumber))
	{
		WRT_BIT(TIM[unitNumber]->CR1, TIM_CR1_DIR, dir);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


/******************************************************************************
 * Center aligned mode.
 *
 * Available only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 *****************************************************************************/
/*
 * selects center aligned mode.
 * (can be only selected before enabling the counter)
 */
void TIM_voidSetCenterAlignedMode(
	u8 unitNumber, TIM_CenterAlignedMode_t mode)
{
	if (IS_ADV_GP_2_TO_5(unitNumber))
	{
		EDT_REG(TIM[unitNumber]->CR1, TIM_CR1_CMS_0, mode, 2);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


/******************************************************************************
 * Dead-time multiplier.
 *
 * Available only in:
 * - Advanced timers.
 * - GPT's 2 to 5
 * - GPT's 9 and 12
 * - GPT's 10/11/13/14
 *****************************************************************************/
/*
 * selects dead-time and sampling clock Multiplier by the internal clock input.
 * (t_DTS = tCK_INT * Multiplier)
 */
void TIM_voidSetDeadTimeMultiplier(
	u8 unitNumber, TIM_DeadTimeMultiplier_t multiplier)
{
	if (IS_ADV_GP(unitNumber))
	{
		EDT_REG(TIM[unitNumber]->CR1, TIM_CR1_CKD_0, multiplier, 2);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


/******************************************************************************
 * Slave mode.
 *
 * Available only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 * - GPT's 9 and 12.
 *****************************************************************************/
/*
 * selects slave mode (external clock input)
 */
void TIM_voidSetSlaveMode(u8 unitNumber, TIM_SlaveMode_t mode)
{
	if (IS_ADV_GP_2_TO_5(unitNumber)  ||  IS_GP_9_AND_12(unitNumber))
	{
		EDT_REG(TIM[unitNumber]->SMCR, TIM_SMCR_SMS_0, mode, 3);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


/******************************************************************************
 * Trigger source.
 *
 * Available only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 * - GPT's 9 and 12.
 *****************************************************************************/
/*
 * selects trigger source.
 * (must be selected when slave mode is disabled, to avoid faulty triggering)
 */
void TIM_voidSetTriggerSource(u8 unitNumber, TIM_TriggerSource_t source)
{
	if (IS_ADV_GP_2_TO_5(unitNumber)  ||  IS_GP_9_AND_12(unitNumber))
	{
		EDT_REG(TIM[unitNumber]->SMCR, TIM_SMCR_TS_0, source, 3);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


/******************************************************************************
 * External clock enable control
 *
 * Available only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 *****************************************************************************/
/*	enables external clock	*/
void TIM_voidEnableExternalClock(u8 unitNumber)
{
	if (IS_ADV_GP_2_TO_5(unitNumber))
	{
		SET_BIT(TIM[unitNumber]->SMCR, TIM_SMCR_ECE);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	disables external clock	*/
void TIM_voidDisableExternalClock(u8 unitNumber)
{
	if (IS_ADV_GP_2_TO_5(unitNumber))
	{
		CLR_BIT(TIM[unitNumber]->SMCR, TIM_SMCR_ECE);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


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
 * selects the external trigger filter sampling frequency,
 * and number of consecutive events needed to validate
 * a transition on trigger output (TRGO).
 */
void TIM_voidSetExternalTriggerFilterMode(
	u8 unitNumber, TIM_SamplingMode_t mode)
{
	if (IS_ADV_GP_2_TO_5(unitNumber))
	{
		EDT_REG(TIM[unitNumber]->SMCR, TIM_SMCR_ETF_0, mode, 4);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	select input capture filter	*/
void TIM_voidSetInputCaptureFilter(
	u8 unitNumber, TIM_Channel_t channelNumber, TIM_SamplingMode_t filter)
{
	if (TIM_b8IsChannelInTimer(unitNumber, channelNumber))
	{
		EDT_REG(
			TIM[unitNumber]->CCMR[channelNumber / 3],
			TIM_CCMR_ICxF_0 + (channelNumber - (channelNumber / 3) * 4) * 8,
			filter, 3);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


/******************************************************************************
 * External trigger prescaler
 *
 * Available for trigger filter mode selection only in:
 * - Advanced timers.
 * - GPT's 2 to 5.
 *****************************************************************************/
/*
 * selects external trigger input prescaler.
 * (external trigger input must be prescaled to a frequency that is at most
 * equal to 25% of timer peripheral bus clock)
 */
void TIM_voidSetExternalTriggerPrescaler(
	u8 unitNumber, TIM_ExternalTriggerPrescaler_t prescaler)
{
	if (IS_ADV_GP_2_TO_5(unitNumber))
	{
		EDT_REG(TIM[unitNumber]->SMCR, TIM_SMCR_ETPS_0, prescaler, 2);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


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
 *****************************************************************************/
/*	selects external trigger polarity	*/
void TIM_voidSetExternalTriggerPolarity(
	u8 unitNumber, TIM_ChannelPolarity_t polarity)
{
	if (IS_ADV_GP_2_TO_5(unitNumber))
	{
		WRT_BIT(TIM[unitNumber]->SMCR, TIM_SMCR_ETP, polarity);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	selects capture/compare channel polarity	*/
void TIM_voidSetCaptureCompareChannelPolarity(
	u8 unitNumber, TIM_Channel_t channel, TIM_ChannelPolarity_t polarity)
{
	if (IS_ADV_GP(unitNumber))
	{
		if (channel > TIM_Channel_2)
			channel -= 2;

		WRT_BIT(
			TIM[unitNumber]->CCER, TIM_CCER_CC1P + channel * 4, polarity);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	selects capture/compare channel's complementary output polarity	*/
void TIM_voidSetCaptureCompareChannelComplementaryOutputPolarity(
	u8 unitNumber, TIM_Channel_t channel, TIM_ChannelPolarity_t polarity)
{
	if (
		IS_ADV_GP_9_TO_14(unitNumber)  					&&
		TIM_b8IsChannelInTimer(unitNumber, channel))
	{
		if (channel > TIM_Channel_2)
			channel -= 2;

		WRT_BIT(
			TIM[unitNumber]->CCER, TIM_CCER_CC1NP + channel * 4, polarity);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


/******************************************************************************
 * Update control.
 *
 * Available for all timers.
 *****************************************************************************/
/*	enable update event generation	*/
void TIM_voidEnableUpdate(u8 unitNumber)
{
	if (IS_ADV(unitNumber))
	{
		CLR_BIT(TIM[unitNumber]->CR1, TIM_CR1_UDIS);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	disable update event generation	*/
void TIM_voidDisableUpdate(u8 unitNumber)
{
	if (IS_ADV(unitNumber))
	{
		SET_BIT(TIM[unitNumber]->CR1, TIM_CR1_UDIS);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


/******************************************************************************
 * Interrupts.
 *
 * Availability of each interrupt event is described in the "TIM_Interrupt_t"
 * enum.
 *****************************************************************************/
/*
 * private function that checks if "interrupt" event is available in the timer
 * of "unitNumber".
 */
static b8 TIM_b8IsInterruptAvailable(u8 unitNumber, TIM_Interrupt_t interrupt)
{
	const u8 gpt2To5InterruptMask				= 0b01011111;
	const u8 gpt9And12InterruptMask				= 0b01000111;
	const u8 gpt10And11And13And14InterruptMask	= 0b00000011;

	if (IS_ADV(unitNumber))
		return true;

	else if (IS_GP_2_TO_5(unitNumber))
		return (1 << interrupt)  &  gpt2To5InterruptMask;

	else if (IS_GP_9_AND_12(unitNumber))
		return (1 << interrupt)  &  gpt9And12InterruptMask;

	else if (IS_GP_10_11_13_14(unitNumber))
		return (1 << interrupt)  &  gpt10And11And13And14InterruptMask;

	else if (IS_BSC_6_AND_7(unitNumber))
		return (interrupt == TIM_Interrupt_Update);

	else
	{
		ErrorHandler_voidExecute(0);
		return false;
	}
}

/*	enables interrupt	*/
void TIM_voidEnableInterrupt(u8 unitNumber, TIM_Interrupt_t interrupt)
{
	if (TIM_b8IsInterruptAvailable(unitNumber, interrupt))
	{
		SET_BIT(TIM[unitNumber]->DIER, interrupt);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	disables interrupt	*/
void TIM_voidDisableInterrupt(u8 unitNumber, TIM_Interrupt_t interrupt)
{
	if (TIM_b8IsInterruptAvailable(unitNumber, interrupt))
	{
		CLR_BIT(TIM[unitNumber]->DIER, interrupt);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	generates interrupt event by SW	*/
void TIM_voidGenerateSoftwareInterruptEvent(
	u8 unitNumber, TIM_Interrupt_t event)
{
	if (TIM_b8IsInterruptAvailable(unitNumber, event))
	{
		SET_BIT(TIM[unitNumber]->EGR, event);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


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
/*
 * private function that checks if "request" is available in the timer
 * of "unitNumber".
 */
static b8 TIM_b8IsDMARequestAvailable(u8 unitNumber, TIM_DMA_t request)
{
	if (IS_ADV_GP_2_TO_5(unitNumber))
		return true;

	else if (IS_ADV_GP_9_TO_14(unitNumber))
		return 0;

	else if (IS_BSC_6_AND_7(unitNumber))
		return (request == TIM_DMA_Update);

	else
	{
		ErrorHandler_voidExecute(0);
		return false;
	}
}

/*	enables DMA request	*/
void TIM_voidEnableDMA(u8 unitNumber, TIM_DMA_t request)
{
	if (TIM_b8IsDMARequestAvailable(unitNumber, request))
	{
		SET_BIT(TIM[unitNumber]->DIER, request);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	disables DMA request	*/
void TIM_voidDisableDMA(u8 unitNumber, TIM_DMA_t request)
{
	if (TIM_b8IsDMARequestAvailable(unitNumber, request))
	{
		CLR_BIT(TIM[unitNumber]->DIER, request);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


/******************************************************************************
 * Status flags.
 *
 * Availability of each state flag is described in the "TIM_Status_t" enum.
 *****************************************************************************/
/*
 * private function that checks if "status" event is available in the timer
 * of "unitNumber".
 */
static b8 TIM_b8IsStatusFlagAvailable(u8 unitNumber, TIM_Status_t status)
{
	const u16 gpt2To5StatusMask					= 0b1111001011111;
	const u16 gpt9And12StatusMask				= 0b0011001000111;
	const u16 gpt10And11And13And14StatusMask	= 0b0001000000011;

	if (IS_ADV(unitNumber))
		return true;

	else if (IS_GP_2_TO_5(unitNumber))
		return (1 << status)  &  gpt2To5StatusMask;

	else if (IS_GP_9_AND_12(unitNumber))
		return (1 << status)  &  gpt9And12StatusMask;

	else if (IS_GP_10_11_13_14(unitNumber))
		return (1 << status)  &  gpt10And11And13And14StatusMask;

	else if (IS_BSC_6_AND_7(unitNumber))
		return (status == TIM_Status_Update);

	else
	{
		ErrorHandler_voidExecute(0);
		return false;
	}
}

/*	returns state of argumented status flag	*/
b8 TIM_b8GetStatusFlag(u8 unitNumber, TIM_Status_t status)
{
	if (TIM_b8IsStatusFlagAvailable(unitNumber, status))
	{
		return GET_BIT(TIM[unitNumber]->SR, status);
	}

	else
	{
		ErrorHandler_voidExecute(0);
		return false;
	}
}

/*	clears state of argumented status flag	*/
void TIM_voidClearStatusFlag(u8 unitNumber, TIM_Status_t status)
{
	if (TIM_b8IsStatusFlagAvailable(unitNumber, status))
	{
		CLR_BIT(TIM[unitNumber]->SR, status);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


/******************************************************************************
 * Capture compare selection.
 *
 * Availability depends on that of channels. Refer to "TIM_Interrupt_t" enum
 * for more information.
 *****************************************************************************/
/*	selects capture/compare	*/
void TIM_voidSetCaptureCompareSelection(
	u8 unitNumber, TIM_Channel_t channel,
	TIM_CaptureCompareSelection_t selection)
{
	if (TIM_b8IsChannelInTimer(unitNumber, channel))
	{
		EDT_REG(
			TIM[unitNumber]->CCMR[channel / 3],
			TIM_CCMR_CCxS_0 + (channel - (channel / 3) * 4) * 8,
			selection, 2);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


/******************************************************************************
 * Input capture prescaler.
 *
 * Availability depends on that of channels. Refer to "TIM_Interrupt_t" enum
 * for more information.
 *****************************************************************************/
/*	select input capture prescaler	*/
void TIM_voidSetInputCapturePrescaler(
	u8 unitNumber, TIM_Channel_t channel,
	TIM_InputCapturePrescaler_t prescaler)
{
	if (TIM_b8IsChannelInTimer(unitNumber, channel))
	{
		EDT_REG(
			TIM[unitNumber]->CCMR[channel / 3],
			TIM_CCMR_ICxPSC_0 + (channel - (channel / 3) * 4) * 8,
			prescaler, 3);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


/******************************************************************************
 * Lock configuration.
 *
 * Available only in advanced timers (TIM1 & TIM8).
 *****************************************************************************/
/*
 * selects lock configuration in "TIMx_BDTR".
 *
 * (the lock bits are one time writable per system reset).
 */
void TIM_voidSetLockConfiguration(u8 unitNumber, TIM_LockConfiguration_t lock)
{
	if (IS_ADV(unitNumber))
	{
		EDT_REG(TIM[unitNumber]->BDTR, TIM_BDTR_LOCK_0, lock, 2);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


/******************************************************************************
 * Counter control.
 *
 * All of the following functions are available for all timers, except the ones
 * mentioned below:
 * 	- "TIM_voidSetRepetitionCounter()" for advanced timers only.
 * 	- "TIM_u8GetRepetitionCounter()" for advanced timers only.
 *****************************************************************************/
/*	enable counter	*/
void TIM_voidEnableCounter(u8 unitNumber)
{
	if (IS_TIM_UNIT_NUMBER(unitNumber))
	{
		SET_BIT(TIM[unitNumber]->CR1, TIM_CR1_CEN);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	disable counter	*/
void TIM_voidDisableCounter(u8 unitNumber)
{
	if (IS_TIM_UNIT_NUMBER(unitNumber))
	{
		CLR_BIT(TIM[unitNumber]->CR1, TIM_CR1_CEN);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	loads CNT register	*/
void TIM_voidSetCNT(u8 unitNumber, u16 value)
{
	if (IS_TIM_UNIT_NUMBER(unitNumber))
	{
		TIM[unitNumber]->CNT = value;
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	reads and returns value of CNT register	*/
u16 TIM_u16GetCNT(u8 unitNumber)
{
	if (IS_TIM_UNIT_NUMBER(unitNumber))
	{
		return TIM[unitNumber]->CNT;
	}

	else
	{
		ErrorHandler_voidExecute(0);
		return 0;
	}
}

/*
 * writes prescaler register.
 * (The counter clock frequency (CK_CNT) is equal to fCK_PSC / (PSC[15:0] + 1))
 */
void TIM_voidSetPrescalerRegister(u8 unitNumber, u16 value)
{
	if (IS_TIM_UNIT_NUMBER(unitNumber))
	{
		TIM[unitNumber]->PSC = value;
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*
 * writes Auto reload register.
 * ARR is the value that the counter counts up to, then generates an OVF,
 * (or counts from it to 0 in case of down counting).
 */
void TIM_voidSetARR(u8 unitNumber, u16 value)
{
	if (IS_TIM_UNIT_NUMBER(unitNumber))
	{
		TIM[unitNumber]->ARR = value;
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	writes repetition counter	*/
void TIM_voidSetRepetitionCounter(u8 unitNumber, u8 value)
{
	if (IS_ADV(unitNumber))
	{
		TIM[unitNumber]->RCR = value;
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	reads repetition counter	*/
u8 TIM_u8GetRepetitionCounter(u8 unitNumber)
{
	if (IS_ADV(unitNumber))
	{
		return TIM[unitNumber]->RCR;
	}

	else
	{
		ErrorHandler_voidExecute(0);
		return 0;
	}
}

/*
 * enables auto reload preload
 * (thus "ARR" register is buffered to the shadow register, and affects after
 * next update event).
 */
void TIM_voidEnableAutoReloadPreload(u8 unitNumber)
{
	if (IS_ADV(unitNumber))
	{
		SET_BIT(TIM[unitNumber]->CR1, TIM_CR1_ARPE);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	disables auto reload preload	*/
void TIM_voidDisableAutoReloadPreload(u8 unitNumber)
{
	if (IS_ADV(unitNumber))
	{
		CLR_BIT(TIM[unitNumber]->CR1, TIM_CR1_ARPE);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	enables one pulse mode	*/
void TIM_voidEnableOnePulseMode(u8 unitNumber)
{
	if (IS_ADV(unitNumber))
	{
		SET_BIT(TIM[unitNumber]->CR1, TIM_CR1_OPM);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


/*	disables one pulse mode	*/
void TIM_voidDisableOnePulseMode(u8 unitNumber)
{
	if (IS_ADV(unitNumber))
	{
		CLR_BIT(TIM[unitNumber]->CR1, TIM_CR1_OPM);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


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
/*	selects output compare mode	*/
void TIM_voidSetOutputCompareMode(
	u8 unitNumber, TIM_Channel_t channel, TIM_OutputCompareMode_t mode)
{
	if (TIM_b8IsChannelInTimer(unitNumber, channel))
	{
		EDT_REG(
			TIM[unitNumber]->CCMR[channel / 3],
			TIM_CCMR_OCxM_0 + (channel - (channel / 3) * 4) * 8, mode, 3);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	enable output compare fast	*/
void TIM_voidEnableOutputCompareFast(u8 unitNumber, TIM_Channel_t channel)
{
	if (TIM_b8IsChannelInTimer(unitNumber, channel))
	{
		SET_BIT(
			TIM[unitNumber]->CCMR[channel / 3],
			TIM_CCMR_OCxFE + (channel - (channel / 3) * 4) * 8);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	disable output compare fast	*/
void TIM_voidDisableOutputCompareFast(u8 unitNumber, TIM_Channel_t channel)
{
	if (TIM_b8IsChannelInTimer(unitNumber, channel))
	{
		CLR_BIT(
			TIM[unitNumber]->CCMR[channel / 3],
			TIM_CCMR_OCxFE + (channel - (channel / 3) * 4) * 8);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	enable output compare preload	*/
void TIM_voidEnableOutputComparePreload(u8 unitNumber, TIM_Channel_t channel)
{
	if (TIM_b8IsChannelInTimer(unitNumber, channel))
	{
		SET_BIT(
			TIM[unitNumber]->CCMR[channel / 3],
			TIM_CCMR_OCxPE + (channel - (channel / 3) * 4) * 8);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	disable output compare preload	*/
void TIM_voidDisableOutputComparePreload(u8 unitNumber, TIM_Channel_t channel)
{
	if (TIM_b8IsChannelInTimer(unitNumber, channel))
	{
		CLR_BIT(
			TIM[unitNumber]->CCMR[channel / 3],
			TIM_CCMR_OCxPE + (channel - (channel / 3) * 4) * 8);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	enable output compare clear	*/
void TIM_voidEnableOutputCompareClear(u8 unitNumber, TIM_Channel_t channel)
{
	if (TIM_b8IsChannelInTimer(unitNumber, channel))
	{
		SET_BIT(
			TIM[unitNumber]->CCMR[channel / 3],
			TIM_CCMR_OCxCE + (channel - (channel / 3) * 4) * 8);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	disable output compare clear	*/
void TIM_voidDisableOutputCompareClear(u8 unitNumber, TIM_Channel_t channel)
{
	if (TIM_b8IsChannelInTimer(unitNumber, channel))
	{
		CLR_BIT(
			TIM[unitNumber]->CCMR[channel / 3],
			TIM_CCMR_OCxCE + (channel - (channel / 3) * 4) * 8);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*
 * loads capture/compare channel's register.
 * (valid only when channel is configured as output)
 */
void TIM_voidSetCCR(u8 unitNumber, TIM_Channel_t channel, u16 value)
{
	if (TIM_b8IsChannelInTimer(unitNumber, channel))
	{
		if (channel > TIM_Channel_2)
			channel -= 2;

		(&(TIM[unitNumber]->CCR1))[channel] = value;
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	enables capture/compare channel's complementary output	*/
void TIM_voidEnableCaptureCompareChannelComplementaryOutput(
	u8 unitNumber, TIM_Channel_t channel)
{
	if (
		IS_ADV_GP_9_TO_14(unitNumber)  					&&
		TIM_b8IsChannelInTimer(unitNumber, channel))
	{
		if (channel > TIM_Channel_2)
			channel -= 2;

		SET_BIT(TIM[unitNumber]->CCER, TIM_CCER_CC1NE + channel * 4);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	disables capture/compare channel's complementary output	*/
void TIM_voidDisableCaptureCompareChannelComplementaryOutput(
	u8 unitNumber, TIM_Channel_t channel)
{
	if (
		IS_ADV_GP_9_TO_14(unitNumber)  					&&
		TIM_b8IsChannelInTimer(unitNumber, channel))
	{
		if (channel > TIM_Channel_2)
			channel -= 2;

		CLR_BIT(TIM[unitNumber]->CCER, TIM_CCER_CC1NE + channel * 4);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*
 * when main output is enabled, OCx and OCNx are connected to their respective
 * CC configuration, otherwise they are in idle state.
 * notice that main output is: "asynchronously by hardware as soon as the break
 * input is active. It is set by software or automatically depending on the
 * Automatic output enable state".
 */
void TIM_voidEnableMainOutput(u8 unitNumber)
{
	if (IS_ADV(unitNumber))
	{
		SET_BIT(TIM[unitNumber]->BDTR, TIM_BDTR_MOE);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

void TIM_voidDisableMainOutput(u8 unitNumber)
{
	if (IS_ADV(unitNumber))
	{
		CLR_BIT(TIM[unitNumber]->BDTR, TIM_BDTR_MOE);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

b8 TIM_b8IsMainOutputEnabled(u8 unitNumber)
{
	if (IS_ADV(unitNumber))
	{
		return GET_BIT(TIM[unitNumber]->BDTR, TIM_BDTR_MOE);
	}

	else
	{
		ErrorHandler_voidExecute(0);
		return false;
	}
}


/******************************************************************************
 * Capture compare control.
 *
 * Availability depends on that of channels. Refer to "TIM_Interrupt_t" enum
 * for more information.
 *****************************************************************************/
/*	reads capture/compare channel's register	*/
u16 TIM_u16GetCCR(u8 unitNumber, TIM_Channel_t channel)
{
	if (TIM_b8IsChannelInTimer(unitNumber, channel))
	{
		if (channel > TIM_Channel_2)
			channel -= 2;

		return (&(TIM[unitNumber]->CCR1))[channel];
	}

	else
	{
		ErrorHandler_voidExecute(0);
		return 0;
	}
}


/******************************************************************************
 * Capture compare channel enable.
 *
 * Availability depends on that of channels. Refer to "TIM_Interrupt_t" enum
 * for more information.
 *****************************************************************************/
/*	enables capture/compare channel	*/
void TIM_voidEnableCaptureCompareChannel(u8 unitNumber, TIM_Channel_t channel)
{
	if (TIM_b8IsChannelInTimer(unitNumber, channel))
	{
		if (channel > TIM_Channel_2)
			channel -= 2;

		SET_BIT(TIM[unitNumber]->CCER, TIM_CCER_CC1E + channel * 4);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}

/*	disables capture/compare channel	*/
void TIM_voidDisableCaptureCompareChannel(u8 unitNumber, TIM_Channel_t channel)
{
	if (TIM_b8IsChannelInTimer(unitNumber, channel))
	{
		if (channel > TIM_Channel_2)
			channel -= 2;

		CLR_BIT(TIM[unitNumber]->CCER, TIM_CCER_CC1E + channel * 4);
	}

	else
	{
		ErrorHandler_voidExecute(0);
	}
}


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
u32 TIM_u32GetClockInternalInput(u8 unitNumber)
{
	RCC_Bus_t bus;
	if (IS_ON_APB2(unitNumber))
		bus = RCC_Bus_APB2;

	else if (!IS_TIM_UNIT_NUMBER(unitNumber))
	{
		ErrorHandler_voidExecute(0);
		return 0;
	}

	else
		bus = RCC_Bus_APB1;

	u32 abpFreq = RCC_u32GetBusClk(bus);

	if (RCC_u16GetBusPrescaler(bus) == 1)
		return abpFreq;
	else
		return 2 * abpFreq;
}

/*
 * sets prescaler of timer unit, to achieve certain overflow frequency.
 * calls error handler if requested frequency is not achievable.
 *
 * assumes that clock source is internal clock source, as it's the most common
 * when using PWM.
 *
 * returns actual running overflow frequency in milli-Hz.
 */
u64 TIM_u64SetFreq(u8 unitNumber, u16 freqHz)
{
	u32 clkInt = TIM_u32GetClockInternalInput(unitNumber);

	/*	Frequency = clk_int / (2^16 * prescaler)	*/
	u32 prescaler = clkInt / ((u32)freqHz << 16);

	if (prescaler == 0  ||  prescaler > 65535)
	{
		ErrorHandler_voidExecute(0);
		return 0;
	}

	else
	{
		TIM[unitNumber]->PSC = prescaler;
		return ((u64)clkInt*1000) / (u64)(prescaler << 16);
	}
}

/*
 * inits channel as PWM output, configures and connects channel's non inverting
 * GPIO pin.
 * returns actual running overflow frequency in milli-Hz.
 */
u64 TIM_u64InitPWM(u8 unitNumber, TIM_Channel_t ch, u16 freqHz)
{
	/*	use internal clock	*/
	if (IS_ADV_GP_2_TO_5(unitNumber)  ||  IS_GP_9_AND_12(unitNumber))
		TIM_voidSetSlaveMode(unitNumber, TIM_SlaveMode_Disabled);

	/*	set frequency (prescaler)	*/
	u64 mHzFreq = TIM_u64SetFreq(unitNumber, freqHz);

	/*	select up-counting mode	*/
	if (IS_ADV_GP_2_TO_5(unitNumber))
		TIM_voidSetCounterDirection(unitNumber, TIM_CountDirection_Up);

	/*	load TCNT with zero	*/
	TIM[unitNumber]->CNT = 0;

	/*	load ARR with maximum value	*/
	TIM[unitNumber]->ARR = (1 << 16) - 1;

	/*	load repetition counter with zero	*/
	TIM[unitNumber]->RCR = 0;

	/*	init capture compare channel as output	*/
	TIM_voidSetCaptureCompareSelection(
		unitNumber, ch, TIM_CaptureCompareSelection_Output);

	/*	enable output compare fast	*/
	TIM_voidEnableOutputCompareFast(unitNumber, ch);

	/*	enable output compare preload	*/
	TIM_voidEnableOutputComparePreload(unitNumber, ch);

	/*	select output compare mode	*/
	TIM_voidSetOutputCompareMode(unitNumber, ch, TIM_OutputCompareMode_PWM1);

	/*	select capture/compare channel polarity	*/
	TIM_voidSetCaptureCompareChannelPolarity(
		unitNumber, ch, TIM_ChannelPolarity_ActiveHigh_RisingEdge);

	/*	enable capture/compare channel	*/
	TIM_voidEnableCaptureCompareChannel(unitNumber, ch);

	if (IS_ADV(unitNumber))
	{
		/*	enable auto reload	*/
		TIM_voidEnableAutoReloadPreload(unitNumber);
		/*	enable main output	*/
		TIM_voidEnableMainOutput(unitNumber);
	}

	return mHzFreq;
}

/*
 * configures and connects channel's non inverting GPIO pin.
 *
 * notice that output compare channels connected to GP pins are only the
 * following:
 *
 */
void TIM_voidInitOutputPin(u8 unitNumber, TIM_Channel_t ch, u8 map)
{
	u8 const portAndPinArr[14][2][4] =
	{
		/*	TIM1	*/
		{
			/*	Memory map 0	*/
			{
				(GPIO_PortName_A << 4) | 8, (GPIO_PortName_A << 4) | 9,
				(GPIO_PortName_A << 4) | 10, (GPIO_PortName_A << 4) | 11
			},
			/*	Memory map 1	*/
			{
				0xFF, 0xFF, 0xFF, 0xFF
			}
		},

		/*	TIM2	*/
		{
			/*	Memory map 0	*/
			{
				(GPIO_PortName_A << 4) | 0, (GPIO_PortName_A << 4) | 1,
				(GPIO_PortName_A << 4) | 2, (GPIO_PortName_A << 4) | 3
			},
			/*	Memory map 1	*/
			{
				(GPIO_PortName_A << 4) | 15, (GPIO_PortName_B << 4) | 3,
				(GPIO_PortName_A << 4) | 2, (GPIO_PortName_A << 4) | 3
			}
		},

		/*	TIM3	*/
		{
			/*	Memory map 0	*/
			{
				(GPIO_PortName_A << 4) | 6, (GPIO_PortName_A << 4) | 7,
				(GPIO_PortName_B << 4) | 0, (GPIO_PortName_B << 4) | 1
			},
			/*	Memory map 1	*/
			{
				(GPIO_PortName_B << 4) | 4, (GPIO_PortName_B << 4) | 5,
				(GPIO_PortName_B << 4) | 0, (GPIO_PortName_B << 4) | 1
			}
		},

		/*	TIM4	*/
		{
			/*	Memory map 0	*/
			{
				(GPIO_PortName_B << 4) | 6, (GPIO_PortName_B << 4) | 7,
				(GPIO_PortName_B << 4) | 8, (GPIO_PortName_B << 4) | 9
			},
			/*	Memory map 1	*/
			{
				0xFF, 0xFF, 0xFF, 0xFF
			}
		},

		/*	TIM5	*/
		{
			/*	Memory map 0	*/
			{
				0xFF, 0xFF, 0xFF, (GPIO_PortName_A << 4) | 3
			},
			/*	Memory map 1	*/
			{
				0xFF, 0xFF, 0xFF, 0xFF
			}
		},

		/*	TIM6	*/
		{
			/*	Memory map 0	*/
			{
				0xFF, 0xFF, 0xFF, 0xFF
			},
			/*	Memory map 1	*/
			{
				0xFF, 0xFF, 0xFF, 0xFF
			}
		},

		/*	TIM7	*/
		{
			/*	Memory map 0	*/
			{
				0xFF, 0xFF, 0xFF, 0xFF
			},
			/*	Memory map 1	*/
			{
				0xFF, 0xFF, 0xFF, 0xFF
			}
		},

		/*	TIM8	*/
		{
			/*	Memory map 0	*/
			{
				0xFF, 0xFF, 0xFF, 0xFF
			},
			/*	Memory map 1	*/
			{
				0xFF, 0xFF, 0xFF, 0xFF
			}
		},

		/*	TIM9	*/
		{
			/*	Memory map 0	*/
			{
				(GPIO_PortName_A << 4) | 2, (GPIO_PortName_A << 4) | 3,
				0xFF, 0xFF
			},
			/*	Memory map 1	*/
			{
				0xFF, 0xFF, 0xFF, 0xFF
			}
		},

		/*	TIM10	*/
		{
			/*	Memory map 0	*/
			{
				(GPIO_PortName_B << 4) | 8, 0xFF, 0xFF, 0xFF
			},
			/*	Memory map 1	*/
			{
				0xFF, 0xFF, 0xFF, 0xFF
			}
		},

		/*	TIM11	*/
		{
			/*	Memory map 0	*/
			{
				(GPIO_PortName_B << 4) | 9, 0xFF, 0xFF, 0xFF
			},
			/*	Memory map 1	*/
			{
				0xFF, 0xFF, 0xFF, 0xFF
			}
		},

		/*	TIM12	*/
		{
			/*	Memory map 0	*/
			{
				0xFF, 0xFF, 0xFF, 0xFF
			},
			/*	Memory map 1	*/
			{
				0xFF, 0xFF, 0xFF, 0xFF
			}
		},

		/*	TIM13	*/
		{
			/*	Memory map 0	*/
			{
				(GPIO_PortName_A << 4) | 6, 0xFF, 0xFF, 0xFF
			},
			/*	Memory map 1	*/
			{
				0xFF, 0xFF, 0xFF, 0xFF
			}
		},

		/*	TIM14	*/
		{
			/*	Memory map 0	*/
			{
				(GPIO_PortName_A << 4) | 7, 0xFF, 0xFF, 0xFF
			},
			/*	Memory map 1	*/
			{
				0xFF, 0xFF, 0xFF, 0xFF
			}
		}
	};


	if (map > 1)
	{
		ErrorHandler_voidExecute(0);
	}

	if (ch > TIM_Channel_2)
		ch -= 2;

	u8 portAndPin = portAndPinArr[unitNumber-1][map][ch];

	if (portAndPin == 0xFF)
	{
		ErrorHandler_voidExecute(0);
	}

	else
	{
		u8 pin = portAndPin & 0x0F;
		GPIO_PortName_t port = (GPIO_PortName_t)(portAndPin >> 4);

		#if DEBUG_ON == 1
		trace_printf(
			"PWM channel initialized on port: %u, pin: %u\n", port, pin);
		#endif

		GPIO_voidSetPinMode(port, pin, GPIO_Mode_AF_PushPull);
		GPIO_voidSetPinOutputSpeed(port, pin, GPIO_OutputSpeed_50MHz);

		if (unitNumber == 3)
			AFIO_voidRemap(AFIO_Peripheral_TIM3, map);
		else if (unitNumber == 2)
			AFIO_voidRemap(AFIO_Peripheral_TIM2, map);
	}
}

/*
 * sets active duty cycle of PWM signal.
 *
 * for less overhead, channel must be configured to PWM1 mode, with an active
 * high polarity fist, (which happens in the function: "TIM_ADV_voidInitPWM()")
 *
 * skips range checking to reduce overhead, as it's used in a high rate. Hence,
 * user must be careful when to use it.
 */
void TIM_voidSetDutyCycle(u8 unitNumber, TIM_Channel_t ch, u16 duty)
{
	if (ch > TIM_Channel_2)
		ch -= 2;

	(&(TIM[unitNumber]->CCR1))[ch] = duty;
}


/******************************************************************************
 * IRQ's
 *****************************************************************************/
void TIM1_BRK_IRQHandler(void)
{

}

void TIM1_UP_IRQHandler(void)
{

}

void TIM1_TRG_COM_IRQHandler(void)
{

}

void TIM1_CC_IRQHandler(void)
{

}

void TIM8_BRK_IRQHandler(void)
{

}

void TIM8_UP_IRQHandler(void)
{

}

void TIM8_TRG_COM_IRQHandler(void)
{

}

void TIM8_CC_IRQHandler(void)
{

}
























