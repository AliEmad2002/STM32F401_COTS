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
	EXTI_Trigger_risingEdge	=	0,
	EXTI_Trigger_fallingEdge	=	1,
	EXTI_Trigger_bothEdges		=	2
}EXTI_Trigger_t;

void EXTI_voidEnableLineInterrupt(u8 line);

void EXTI_voidDisbleLineInterrupt(u8 line);

/*	EXTI event can wake up the MCU but won't generate ISR call	*/
void EXTI_voidEnableLineEvent(u8 line);

void EXTI_voidDisbleLineEvent(u8 line);

void EXTI_voidSetTriggeringEdge(u8 line, EXTI_Trigger_t edge);

void EXTI_voidSoftwareTrigger(u8 line);

void EXTI_voidSetCallBack(u8 line, void(*callBack)(void));

/*
 * assigns a pin to the EXTI line.
 *
 * pin number is same as EXTI line number
 * (forced by HW).
 *
 * port is selectable.
 */
void EXTI_voidMapLine(u8 line, GPIO_PortName_t port);

#endif /* EXTI_INTERFACE_H_ */
