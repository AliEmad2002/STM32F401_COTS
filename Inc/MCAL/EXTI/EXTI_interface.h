/*
 * EXTI_interface.h
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef EXTI_INTERFACE_H_
#define EXTI_INTERFACE_H_

typedef enum{
	MEXTI_TRIGGER_risingEdge	=	0,
	MEXTI_TRIGGER_fallingEdge	=	1,
	MEXTI_TRIGGER_bothEdges		=	2
}MEXTI_TRIGGER_t;

b8 MEXTI_b8IsNotLine(u8 line);

void MEXTI_voidEnableLine(u8 line);

void MEXTI_voidDisbleLine(u8 line);

void MEXTI_voidSetTriggeringEdge(u8 line, MEXTI_TRIGGER_t edge);

void MEXTI_voidSoftwareTrigger(u8 line);

void MEXTI_voidSetCallBack(u8 line, void(*callBack)(void));

#endif /* EXTI_INTERFACE_H_ */
