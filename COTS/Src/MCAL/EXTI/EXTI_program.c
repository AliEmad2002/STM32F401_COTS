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

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Target_config.h"

/*	MCAL	*/
#include "GPIO_interface.h"

/*	SELF	*/
#include "EXTI_private.h"
#include "EXTI_interface.h"

#ifndef NULL
#define NULL ((void*)0x0)
#endif

#if TARGET_ID == STM32F401x
#define EXTI_LINES_COUNT	23
#elif TARGET_ID == STM32F10x
#define EXTI_LINES_COUNT	20
#endif

static void (*EXTI_callback[EXTI_LINES_COUNT])(void) = {NULL};

void EXTI_voidEnableLine(u8 line)
{
	SET_BIT(EXTI->IMR, line);
}

void EXTI_voidDisbleLine(u8 line)
{
	CLR_BIT(EXTI->IMR, line);
}

/*	EXTI event can wake up the MCU but won't generate ISR call	*/
void EXTI_voidEnableLineEvent(u8 line)
{
	SET_BIT(EXTI->EMR, line);
}

void EXTI_voidDisbleLineEvent(u8 line)
{
	CLR_BIT(EXTI->EMR, line);
}

void EXTI_voidSetTriggeringEdge(u8 line, EXTI_Trigger_t edge)
{

	switch(edge)
	{
	case EXTI_Trigger_risingEdge:
		SET_BIT(EXTI->RTSR, line);
		CLR_BIT(EXTI->FTSR, line);
		break;
	case EXTI_Trigger_fallingEdge:
		SET_BIT(EXTI->FTSR, line);
		CLR_BIT(EXTI->RTSR, line);
		break;
	case EXTI_Trigger_bothEdges:
		SET_BIT(EXTI->RTSR, line);
		SET_BIT(EXTI->FTSR, line);
		break;
	}
}

void EXTI_voidSoftwareTrigger(u8 line)
{
	SET_BIT(EXTI->SWIER, line);
}

void EXTI_voidSetCallBack(u8 line, void(*callBack)(void))
{
	EXTI_callback[line] = callBack;
}

void EXTI_voidMapLine(u8 line, GPIO_PortName_t port)
{
	#if TARGET_ID == STM32F401x
	MSYSCFG_voidSetEXTILineSource(line, port);
	#elif TARGET_ID == STM32F10x
	AFIO_voidSetExtiLineSrc(line, port);
	#endif
}

void EXTI0_IRQHandler(void) {if(EXTI_callback[0] != NULL) EXTI_callback[0](); SET_BIT(EXTI->PR, 0);}
void EXTI1_IRQHandler(void) {if(EXTI_callback[1] != NULL) EXTI_callback[1](); SET_BIT(EXTI->PR, 1);}
void EXTI2_IRQHandler(void) {if(EXTI_callback[2] != NULL) EXTI_callback[2](); SET_BIT(EXTI->PR, 2);}
void EXTI3_IRQHandler(void) {if(EXTI_callback[3] != NULL) EXTI_callback[3](); SET_BIT(EXTI->PR, 3);}
void EXTI4_IRQHandler(void) {if(EXTI_callback[4] != NULL) EXTI_callback[4](); SET_BIT(EXTI->PR, 4);}
void EXTI5_IRQHandler(void) {if(EXTI_callback[5] != NULL) EXTI_callback[5](); SET_BIT(EXTI->PR, 5);}
void EXTI6_IRQHandler(void) {if(EXTI_callback[6] != NULL) EXTI_callback[6](); SET_BIT(EXTI->PR, 6);}
void EXTI7_IRQHandler(void) {if(EXTI_callback[7] != NULL) EXTI_callback[7](); SET_BIT(EXTI->PR, 7);}
void EXTI8_IRQHandler(void) {if(EXTI_callback[8] != NULL) EXTI_callback[8](); SET_BIT(EXTI->PR, 8);}
void EXTI9_IRQHandler(void) {if(EXTI_callback[9] != NULL) EXTI_callback[9](); SET_BIT(EXTI->PR, 9);}
void EXTI10_IRQHandler(void) {if(EXTI_callback[10] != NULL) EXTI_callback[10](); SET_BIT(EXTI->PR, 10);}
void EXTI11_IRQHandler(void) {if(EXTI_callback[11] != NULL) EXTI_callback[11](); SET_BIT(EXTI->PR, 11);}
void EXTI12_IRQHandler(void) {if(EXTI_callback[12] != NULL) EXTI_callback[12](); SET_BIT(EXTI->PR, 12);}
void EXTI13_IRQHandler(void) {if(EXTI_callback[13] != NULL) EXTI_callback[13](); SET_BIT(EXTI->PR, 13);}
void EXTI14_IRQHandler(void) {if(EXTI_callback[14] != NULL) EXTI_callback[14](); SET_BIT(EXTI->PR, 14);}
void EXTI15_IRQHandler(void) {if(EXTI_callback[15] != NULL) EXTI_callback[15](); SET_BIT(EXTI->PR, 15);}
void EXTI16_IRQHandler(void) {if(EXTI_callback[16] != NULL) EXTI_callback[16](); SET_BIT(EXTI->PR, 16);}
void EXTI17_IRQHandler(void) {if(EXTI_callback[17] != NULL) EXTI_callback[17](); SET_BIT(EXTI->PR, 17);}
void EXTI18_IRQHandler(void) {if(EXTI_callback[18] != NULL) EXTI_callback[18](); SET_BIT(EXTI->PR, 18);}
#if TARGET_ID == STM32F10x
void EXTI19_IRQHandler(void) {if(EXTI_callback[19] != NULL) EXTI_callback[19](); SET_BIT(EXTI->PR, 19);}
#elif TARGET_ID == STM32F401x
void EXTI21_IRQHandler(void) {if(EXTI_callback[21] != NULL) EXTI_callback[21](); SET_BIT(EXTI_PR, 21);}
void EXTI22_IRQHandler(void) {if(EXTI_callback[22] != NULL) EXTI_callback[22](); SET_BIT(EXTI_PR, 22);}
#endif
