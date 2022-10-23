/*
 * Stepper_program.c
 *
 * Created: 7/11/2022 8:11:42 PM
 *  Author: Ali Emad
 */ 


/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "delay_interface.h"

/*	MCAL	*/
#include "RCC_interface.h"
#include "STK_interface.h"
#include "GPIO_interface.h"

/*	SELF	*/
#include "Stepper_interface.h"


/*
 * assigns stepper related pins, and initializes them as very high speed
 * outputs.
 * zeros its time-stamp and position.
 */
void Stepper_voidInit(
	Stepper_t* stepperPtr, GPIO_Pin_t _stepPin, GPIO_Pin_t _dirPin)
{
	/*	init pins	*/
	stepperPtr->stepPin = _stepPin % 16;
	stepperPtr->stepPort = _stepPin / 16;
	
	stepperPtr->dirPin = _dirPin % 16;
	stepperPtr->dirPort = _dirPin / 16;
	
	GPIO_voidSetPinGpoPushPull(stepperPtr->stepPort, stepperPtr->stepPin);
	GPIO_voidSetPinGpoPushPull(stepperPtr->dirPort, stepperPtr->dirPin);

	/*	init parameters	*/
	stepperPtr->lastTimeStamp = 0;
	stepperPtr->currentPos = 0;
}

/*
 * steps the object in wanted direction by one steep, and stamps current time.
 */
void Stepper_voidStep(
	Stepper_t* stepperPtr, Stepper_Direction_t dir, u64 currentTime)
{
	/*	output prober direction signal	*/
	switch (dir)
	{
		case Stepper_Direction_forward:
			GPIO_voidSetPinOutputLevel(
				stepperPtr->dirPort, stepperPtr->dirPin, GPIO_OutputLevel_High);
			stepperPtr->currentPos++;
			break;
		default:	//	i.e.: Stepper_dir_backward:
			GPIO_voidSetPinOutputLevel(
				stepperPtr->dirPort, stepperPtr->dirPin, GPIO_OutputLevel_Low);
			stepperPtr->currentPos--;
	}
	
	/*	step	*/
	GPIO_voidSetPinOutputLevel(
		stepperPtr->stepPort, stepperPtr->stepPin, GPIO_OutputLevel_High);
	Delay_voidDelay2Us();
	GPIO_voidSetPinOutputLevel(
		stepperPtr->stepPort, stepperPtr->stepPin, GPIO_OutputLevel_Low);
	
	/*	refresh timeStamp	*/
	stepperPtr->lastTimeStamp = currentTime;
}
