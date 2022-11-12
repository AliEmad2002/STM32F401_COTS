/*
 * Stepper_interface.h
 *
 * Created: 7/11/2022 7:47:43 PM
 *  Author: Ali Emad
 */ 


#ifndef STEPPER_INTERFACE_H_
#define STEPPER_INTERFACE_H_

#define X	0
#define Y	1
#define Z	2

typedef struct
{
	/*	pins	*/
	u8 stepPin;
	GPIO_PortName_t stepPort;
	
	u8 dirPin;
	GPIO_PortName_t dirPort;
	
	/*	last time the object stepped	*/
	u64 lastTimeStamp;
	
	/*	current position of the object (in steps)	*/
	s32 currentPos;
}Stepper_t;

typedef enum{
	Stepper_Direction_forward,
	Stepper_Direction_backward
}Stepper_Direction_t;

/*
 * assigns stepper related pins, and initializes them as very high speed
 * outputs.
 * zeros its time-stamp and position.
 */
void Stepper_voidInit(
	Stepper_t* stepperPtr, GPIO_Pin_t _stepPin, GPIO_Pin_t _dirPin);

/*
 * steps the object in wanted direction by one steep, and stamps current time.
 */
void Stepper_voidStep(
	Stepper_t* stepperPtr, Stepper_Direction_t dir, u64 currentTime);

#endif /* STEPPER_INTERFACE_H_ */
