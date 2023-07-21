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

#include "EXTI_private.h"
#include "GPIO_Interface.h"

typedef enum{
	EXTI_Trigger_risingEdge	=	0,
	EXTI_Trigger_fallingEdge	=	1,
	EXTI_Trigger_bothEdges		=	2
}EXTI_Trigger_t;


/*
 * returns vector number of the vector corresponding to the input line event
 */
u8 EXTI_u8FindVector(u8 line);

/*
 * enables line's corresponding vector's interrupt in NVIC
 */
void EXTI_voidEnableLineInterrupt(u8 line);

void EXTI_voidDisableLineInterrupt(u8 line);

/*	Enables line in EXTI itself	*/
void EXTI_voidEnableLine(u8 line);
#define EXTI_ENABLE_LINE(line) (SET_BIT(EXTI->IMR, (line)))

void EXTI_voidDisableLine(u8 line);
#define EXTI_DISABLE_LINE(line) (CLR_BIT(EXTI->IMR, (line)))

/*	EXTI event can wake up the MCU but won't generate ISR call	*/
void EXTI_voidEnableLineEvent(u8 line);

void EXTI_voidDisbleLineEvent(u8 line);

void EXTI_voidSetTriggeringEdge(u8 line, EXTI_Trigger_t edge);

void EXTI_voidSoftwareTrigger(u8 line);

void EXTI_voidSetCallBack(u8 line, void(*callBack)(void*), void* args);

/*
 * assigns a pin to the EXTI line.
 *
 * pin number is same as EXTI line number
 * (forced by HW).
 *
 * port is selectable.
 */
void EXTI_voidMapLine(u8 line, GPIO_PortName_t port);

#define EXTI_CLEAR_FLAG(line)  (SET_BIT(EXTI->PR, (line)))

#define EXTI_GET_FLAG(line)  	(GET_BIT(EXTI->PR, (line)))

#endif /* EXTI_INTERFACE_H_ */
