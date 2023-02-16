/*
 * IR_interface.h
 *
 *  Created on: Oct 30, 2022
 *      Author: Ali Emad Ali
 *
 *  This module uses any timer unit - mostly sysTick, configured in elapsed tick
 *  counting mode, and EXTI.
 */

#ifndef INCLUDE_HAL_IR_IR_INTERFACE_H_
#define INCLUDE_HAL_IR_IR_INTERFACE_H_

typedef enum{
	IR_ReceiveStatus_Out,						//	out of any of reception
												//	levels.

	IR_ReceiveStatus_LeadingPulseReceived,		//	received the 9ms leading
												//	pulse.

	IR_ReceiveStatus_SpaceReceived,				//	received the 4.5ms space.

	IR_ReceiveStatus_BitHeaderReceived			//	received the 560us pulse
												//	Indicating start of bit.
}IR_ReceiveStatus_t;

typedef struct{
	u8 extiLine;			//	EXTI line connected to IR receiver output.

	u32 data;				//	Stores the last received data.

	u8 bitCounter;			//	Counts how many bits have been received since
							//	last complete data reception.

	u64 lastTimeStamp;		//	timestamp of last edge in input signal.

	u8 recvStat;
							//	status of the current reception operation.

	GPIO_OutputLevel_t level;
							//	current logic level of the input signal.

	void (*receiveCompleteCallback) (void);
							//	Callback of end of reception.

	void (*EXTICallback) (void);
							//	callback of the chosen EXTI line. on object
							//	creation, user should create a function that
							//	takes and returns void, inside which, he/she
							//	does nothing but calling the following function:
							//	"IR_voidEXTICallbackTamplate()", passing a
							//	pointer to the object to it.
}IR_Receiver_t;

/*
 * look at documentation of "void (*EXTICallback) (void)" in "IR_Receiver_t".
 */
void IR_TEST(IR_Receiver_t* IR);
void IR_voidEXTICallbackTamplate(IR_Receiver_t* IR);

/*
 * for info about the arguments, refer to the documentation of the structure
 * "IR_Receiver_t".
 *
 * EXTI, AFIO, GPIOx peripherals should be RCC clock enabled.
 *
 * EXTI line interrupt is advised to be in higher groups.
 */
void IR_voidInit(
	IR_Receiver_t* IR, u8 extiLine, GPIO_PortName_t port,
	void (*receiveCompleteCallback) (void),
	void (*EXTICallback) (void)
	);



















#endif /* INCLUDE_HAL_IR_IR_INTERFACE_H_ */
