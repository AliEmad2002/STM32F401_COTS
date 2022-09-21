/*
 * EXTI_program.c
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#include"STD_TYPES.h"
#include "BIT_MATH.h"

#include "EXTI_private.h"
#include "EXTI_interface.h"

#ifndef NULL
#define NULL ((void*)0x0)
#endif

static void (*EXTI_callback[22])(void) = {
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

b8 MEXTI_b8IsNotLine(u8 line)
{
	return line > 22 || line == 19 || line == 20;
}

void MEXTI_voidEnableLine(u8 line)
{
	/*	range check	*/
	if (MEXTI_b8IsNotLine(line))
	{
		/*	wrong line	*/
		while(1);
	}

	SET_BIT(EXTI_IMR, line);
}

void MEXTI_voidDisbleLine(u8 line)
{
	/*	range check	*/
	if (MEXTI_b8IsNotLine(line))
	{
		/*	wrong line	*/
		while(1);
	}

	CLR_BIT(EXTI_IMR, line);
}

void MEXTI_voidSetTriggeringEdge(u8 line, MEXTI_TRIGGER_t edge)
{
	/*	range check	*/
	if (MEXTI_b8IsNotLine(line))
	{
		/*	wrong line	*/
		while(1);
	}

	switch(edge)
	{
	case MEXTI_TRIGGER_risingEdge:
		SET_BIT(EXTI_RTSR, line);
		break;
	case MEXTI_TRIGGER_fallingEdge:
		SET_BIT(EXTI_FTSR, line);
		break;
	case MEXTI_TRIGGER_bothEdges:
		SET_BIT(EXTI_RTSR, line);
		SET_BIT(EXTI_FTSR, line);
		break;
	}
}

void MEXTI_voidSoftwareTrigger(u8 line)
{
	/*	range check	*/
	if (MEXTI_b8IsNotLine(line))
	{
		/*	wrong line	*/
		while(1);
	}

	SET_BIT(EXTI_SWIER, line);
}

void MEXTI_voidSetCallBack(u8 line, void(*callBack)(void))
{
	/*	range check	*/
	if (MEXTI_b8IsNotLine(line))
	{
		/*	wrong line	*/
		while(1);
	}

	EXTI_callback[line] = callBack;
}

//void EXTI0_IRQHandler(void)
//{
//	/*	execute callback if it exists	*/
//	if(EXTI_callback[0] != NULL)
//		EXTI_callback[0]();
//
//	/*	clear pending flag	*/
//	SET_BIT(EXTI_PR, 0);
//}

void EXTI0_IRQHandler(void) {if(EXTI_callback[0] != NULL) EXTI_callback[0](); SET_BIT(EXTI_PR, 0);}
void EXTI1_IRQHandler(void) {if(EXTI_callback[1] != NULL) EXTI_callback[1](); SET_BIT(EXTI_PR, 1);}
void EXTI2_IRQHandler(void) {if(EXTI_callback[2] != NULL) EXTI_callback[2](); SET_BIT(EXTI_PR, 2);}
void EXTI3_IRQHandler(void) {if(EXTI_callback[3] != NULL) EXTI_callback[3](); SET_BIT(EXTI_PR, 3);}
void EXTI4_IRQHandler(void) {if(EXTI_callback[4] != NULL) EXTI_callback[4](); SET_BIT(EXTI_PR, 4);}
