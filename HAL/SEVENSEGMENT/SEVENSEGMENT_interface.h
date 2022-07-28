/*
 * SEVENSEGMENT_interface.h
 *
 *  Created on:	Jul 27, 2022
 *  Author:		Ali Emad
 */

#ifndef SEVENSEGMENT_INTERFACE_H_
#define SEVENSEGMENT_INTERFACE_H_

typedef enum{
	SEVENSEGMENT_COMTYPE_cathode	= 0,
	SEVENSEGMENT_COMTYPE_anode		= 1
}SEVENSEGMENT_COMTYPE;

typedef enum{
	SEVENSEGMENT_POINTSTATE_off	= 0,
	SEVENSEGMENT_POINTSTATE_on	= 1
}SEVENSEGMENT_POINTSTATE;

typedef enum{
	SEVENSEGMENT_POINTCONNECTION_notConnected	= 0,
	SEVENSEGMENT_POINTCONNECTION_connected		= 1
}SEVENSEGMENT_POINTCONNECTION;

typedef struct{
	DIO_PORT portArr[8];
	u8 pinArr[8];
	SEVENSEGMENT_COMTYPE comType;
	SEVENSEGMENT_POINTCONNECTION pointConnection;
}SEVENSEGMENT_t;

void SEVENSEGMENT_voidInit(
	SEVENSEGMENT_t* ssPtr, SEVENSEGMENT_COMTYPE _comType,
	DIO_PIN pinA, DIO_PIN pinB, DIO_PIN pinC, DIO_PIN pinD,
	DIO_PIN pinE, DIO_PIN pinF, DIO_PIN pinG, DIO_PIN pinPoint,
	SEVENSEGMENT_POINTCONNECTION _pointConnection,
	DIO_OUTPUTSPEED pinSpeed
	);

void SEVENSEGMENT_voidWrite(SEVENSEGMENT_t* ssPtr, u8 num, SEVENSEGMENT_POINTSTATE pointState);

void SEVENSEGMENT_voidTurnOff(SEVENSEGMENT_t* ssPtr);

#endif /* SEVENSEGMENT_INTERFACE_H_ */
