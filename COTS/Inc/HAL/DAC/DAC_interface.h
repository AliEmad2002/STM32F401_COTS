/*
 * DAC_interface.h
 *
 *  Created on:	Sep 22, 2022
 *  Author:		Ali Emad Ali
 *	
 */

#ifndef HAL_DAC_DAC_INTERFACE_H_
#define HAL_DAC_DAC_INTERFACE_H_

typedef enum{
	DAC_Connection_direct,
	DAC_Connection_ShiftRegister
}DAC_Connection_t;

typedef struct{
	DAC_Connection_t connection;
	u8 resolution;
	SR_SerialInParallelOut_t* shiftRegisterPtr;
	u8* pinArr;
	GPIO_PortName_t* portArr;
}DAC_t;

/*	allocates memory needed for the DAC object	*/
void DAC_voidAlloc(DAC_t* dacPtr, DAC_Connection_t _connection, u8 _resolution);

/*	sets the SR of the DAC, it should be initially init.	*/
void DAC_voidSetShiftRegister(DAC_t* dacPtr, SR_SerialInParallelOut_t* _shiftRegisterPtr);

/*
 * sets pins of the DAC, and inits them all as Push-Pull outputs.
 * pin speed is user selectable before this.
 */
void DAC_voidSetPins(DAC_t* dacPtr, GPIO_Pin_t _pinArr[]);

/*	outputs a value on the DAC object	*/
void DAC_voidOut(DAC_t* dacPtr, u32 val);

#endif /* HAL_DAC_DAC_INTERFACE_H_ */
