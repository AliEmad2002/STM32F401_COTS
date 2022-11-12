/*
 * Seven_Segment_interface.h
 *
 *  Created on:	Jul 27, 2022
 *  Author:		Ali Emad
 */

#ifndef SEVEN_SEGMENT_INTERFACE_H_
#define SEVEN_SEGMENT_INTERFACE_H_

typedef enum{
	Seven_Segment_ComType_Cathode	= 0,
	Seven_Segment_ComType_Anode		= 1
}Seven_Segment_ComType_t;

typedef enum{
	Seven_Segment_PointState_Off	= 0,
	Seven_Segment_PointState_On		= 1
}Seven_Segment_PointState_t;

typedef enum{
	Seven_Segment_PointConnection_notConnected	= 0,
	Seven_Segment_PointConnection_connected		= 1
}Seven_Segment_PointConnection_t;


typedef struct{
	GPIO_PortName_t * portArr;
	u8* pinArr;
	Seven_Segment_ComType_t comType;
	Seven_Segment_PointConnection_t pointConnection;
}Seven_Segment_t;

/*
 * assigns pins to seven-segment object, and inits them as GPO push pull.
 * pin speed is previously user selectable.
 */
void Seven_Segment_voidInit(
	Seven_Segment_t* ssPtr, Seven_Segment_ComType_t _comType,
	GPIO_Pin_t pinArr[], Seven_Segment_PointConnection_t _pointConnection
	);

/*	writes a number on seven-segment object. point state is selectable - if connected -.	*/
void Seven_Segment_voidWrite(Seven_Segment_t* ssPtr, u8 num, Seven_Segment_PointState_t pointState);

/*	turns off all segments	*/
void Seven_Segment_voidTurnOff(Seven_Segment_t* ssPtr);

#endif /* SEVENSEGMENT_INTERFACE_H_ */
