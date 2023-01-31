/*
 * Rotary_Encoder_Config.h
 *
 *  Created on: Jan 30, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_HAL_ROTARY_ENCODER_ROTARY_ENCODER_CONFIG_H_
#define INCLUDE_HAL_ROTARY_ENCODER_ROTARY_ENCODER_CONFIG_H_

#define TRANSITION_TIMEOUT				(250 * 72000ul)	// STK ticks

#define DEBOUNCING_TIMEOUT				(50 * 72000ul)	// STK ticks

/*
 * Define your encoders here, then list their addresses in the following array:
 * "Rotray_Encoder_PtrArr[]"
 */
#define NUMBER_OF_ENCODERS		1

extern void OSC_voidUpButtonCallBack(void);
extern void OSC_voidDownButtonCallBack(void);

volatile Rotary_Encoder_t OSC_RotaryEncoder = {
	.outAPort = GPIO_PortName_B, .outAPin = 0,
	.outBPort = GPIO_PortName_B, .outBPin = 6,

	.countUpCallback = OSC_voidUpButtonCallBack,
	.countUpCallbackEnabled = true,

	.countDownCallback = OSC_voidDownButtonCallBack,
	.countDownCallbackEnabled = true,
};

static Rotary_Encoder_t* Rotray_Encoder_PtrArr[NUMBER_OF_ENCODERS] = {
	&OSC_RotaryEncoder
};


#endif /* INCLUDE_HAL_ROTARY_ENCODER_ROTARY_ENCODER_CONFIG_H_ */
