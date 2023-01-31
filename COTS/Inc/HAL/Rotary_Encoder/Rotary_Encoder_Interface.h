/*
 * Rotary_Encoder_Interface.h
 *
 *  Created on: Jan 30, 2023
 *      Author: Ali Emad Ali
 *
 * Note: debouncing is user's responsibility, and may be implemented in counting
 * callbacks.
 */

#ifndef INCLUDE_HAL_ROTARY_ENCODER_ROTARY_ENCODER_INTERFACE_H_
#define INCLUDE_HAL_ROTARY_ENCODER_ROTARY_ENCODER_INTERFACE_H_


typedef struct{
	GPIO_PortName_t outAPort;
	GPIO_PortName_t outBPort;

	u8 outAPin;
	u8 outBPin;

	s32 count;

	void (*countUpCallback)(void);
	b8 countUpCallbackEnabled;

	void (*countDownCallback)(void);
	b8 countDownCallbackEnabled;

	u64 lastTimeStamp;
}Rotary_Encoder_t;

/*	inits all encoders defined in config.h file	*/
void Rotary_Encoder_voidInit(void);


#endif /* INCLUDE_HAL_ROTARY_ENCODER_ROTARY_ENCODER_INTERFACE_H_ */
