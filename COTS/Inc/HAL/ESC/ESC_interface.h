/*
 * ESC_interface.h
 *
 *  Created on: Nov 6, 2022
 *      Author: Ali Emad Ali
 *
 *  uses advanced timer 1. (over kill? to be changed to GPT soon).
 */

#ifndef _ESC_ESC_INTERFACE_H_
#define _ESC_ESC_INTERFACE_H_

#include "TIM_interface.h"

typedef struct{
	u8 timerUnitNumber;
	TIM_Channel_t timerChannel;
	s16 speed;
	u16 minOCR;
	u16 maxOCR;
}ESC_t;

/*
 * inits ESC.
 *
 * inits timer channel for PWM output.
 * init GPIO pin mapped to the channel.
 */
void ESC_voidInit(
	ESC_t* esc, u8 timerUnitNumber, TIM_Channel_t timerChannel, u16 freqHz,
	u32 minPulseWidthUs, u32 maxPulseWidthUs, u8 afioTimerMap);

/*
 * sets speed of ESC.
 *
 * speed is a value between -1000 and 1000
 */
void ESC_voidSetSpeed(ESC_t* esc, s16 speed);


#endif /* _ESC_ESC_INTERFACE_H_ */
