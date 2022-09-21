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
	DIO_PORT stepPort;
	
	u8 dirPin;
	DIO_PORT dirPort;
	
	/*	last time the object stepped	*/
	u64 lastTimeStamp;
	
	/*	current position of the object (in steps)	*/
	s32 currentPos;
}Stepper_t;

typedef enum {Stepper_dir_forward, Stepper_dir_backward}Stepper_dir;

/*
 * assigns stepper related pins, and initializes them as very high speed outputs.
 * zeros its time-stamp and position.
 */
void Stepper_voidInit(Stepper_t* stepperPtr, DIO_PIN _stepPin, DIO_PIN _dirPin);

/*
 * steps the object in wanted direction by one steep, and stamps current time.
 */
void Stepper_voidStep(Stepper_t* stepperPtr, Stepper_dir dir, u64 currentTime);

#endif /* STEPPER_INTERFACE_H_ */
