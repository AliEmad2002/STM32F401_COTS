/*
 * DAC_interface.h
 *
 *  Created on:	Sep 22, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
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
	SIPO_SHIFT_REGISTER_t* shiftRegisterPtr;
	u8* pinArr;
	DIO_PORT* portArr;
}DAC_t;

/*	allocates memory needed for the DAC object	*/
void DAC_voidAlloc(DAC_t* dacPtr, DAC_Connection_t _connection, u8 _resolution);

/*	sets the SR of the DAC, it should be initially init.	*/
void DAC_voidSetShiftRegister(DAC_t* dacPtr, SIPO_SHIFT_REGISTER_t* _shiftRegisterPtr);

/*
 * sets pins of the DAC, and inits them all as Push-Pull outputs
 * with the desired speed.
 */
void DAC_voidSetPins(DAC_t* dacPtr, DIO_PIN _pinArr[], DIO_OUTPUTSPEED speed);

/*	outputs a value on the DAC object	*/
void DAC_voidOut(DAC_t* dacPtr, u32 val);

#endif /* HAL_DAC_DAC_INTERFACE_H_ */
