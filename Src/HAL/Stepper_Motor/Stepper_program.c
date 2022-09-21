/*
 * Stepper_program.c
 *
 * Created: 7/11/2022 8:11:42 PM
 *  Author: Ali Emad
 */ 


/*	LIB	*/
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "delay_interface.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
/*	MCAL	*/
#include "RCC_interface.h"
#include "STK_interface.h"
#include "GPIO_interface.h"

/*	SELF	*/
#include "Stepper_interface.h"


void Stepper_voidInit(Stepper_t* stepperPtr, DIO_PIN _stepPin, DIO_PIN _dirPin)
{
	/*	init pins	*/
	stepperPtr->stepPin = _stepPin % 16;
	stepperPtr->stepPort = _stepPin / 16;
	
	stepperPtr->dirPin = _dirPin % 16;
	stepperPtr->dirPort = _dirPin / 16;
	
	DIO_voidSetPinMode(			stepperPtr->stepPort, stepperPtr->stepPin, DIO_MODE_GPO);
	DIO_voidSetPinOutputType(	stepperPtr->stepPort, stepperPtr->stepPin, DIO_OUTPUTTYPE_pushPull);
	DIO_voidSetPinOutputSpeed(	stepperPtr->stepPort, stepperPtr->stepPin, DIO_OUTPUTSPEED_veryHigh);

	DIO_voidSetPinMode(			stepperPtr->dirPort, stepperPtr->dirPin, DIO_MODE_GPO);
	DIO_voidSetPinOutputType(	stepperPtr->dirPort, stepperPtr->dirPin, DIO_OUTPUTTYPE_pushPull);
	DIO_voidSetPinOutputSpeed(	stepperPtr->dirPort, stepperPtr->dirPin, DIO_OUTPUTSPEED_veryHigh);

	/*	init parameters	*/
	stepperPtr->lastTimeStamp = 0;
	stepperPtr->currentPos = 0;
}

void Stepper_voidStep(Stepper_t* stepperPtr, Stepper_dir dir, u64 currentTime)
{
	/*	output prober direction signal	*/
	switch (dir)
	{
		case Stepper_dir_forward:
			DIO_voidSetPinOutputLevel(stepperPtr->dirPort, stepperPtr->dirPin, DIO_OUTPUTLEVEL_high);
			stepperPtr->currentPos++;
			break;
		default:	//	i.e.: Stepper_dir_backward:
			DIO_voidSetPinOutputLevel(stepperPtr->dirPort, stepperPtr->dirPin, DIO_OUTPUTLEVEL_low);
			stepperPtr->currentPos--;
	}
	
	/*	step	*/
	DIO_voidSetPinOutputLevel(stepperPtr->stepPort, stepperPtr->stepPin, DIO_OUTPUTLEVEL_high);
	Delay_voidDelay2Us();
	DIO_voidSetPinOutputLevel(stepperPtr->stepPort, stepperPtr->stepPin, DIO_OUTPUTLEVEL_low);
	
	/*	refresh timeStamp	*/
	stepperPtr->lastTimeStamp = currentTime;
}
