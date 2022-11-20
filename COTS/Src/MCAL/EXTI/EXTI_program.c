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
#include "NVIC_interface.h"

/*	SELF	*/
#include "EXTI_private.h"
#include "EXTI_interface.h"

#ifndef NULL
#define NULL ((void*)0x0)
#endif

#if TARGET_ID == STM32F401x
#define EXTI_VECTORS_COUNT	23
#elif TARGET_ID == STM32F10x
#define EXTI_VECTORS_COUNT	16
#endif

static void (*EXTI_callback[EXTI_VECTORS_COUNT])(void);

/*
 * returns vector number of the vector corresponding to the input line event
 */
u8 EXTI_u8FindVector(u8 line)
{
	if (line < 5)
		return NVIC_Interrupt_EXTI0 + line;
	else if (line < 10)
		return NVIC_Interrupt_EXTI5To9;
	else
		return NVIC_Interrupt_EXTI10To15;
}

void EXTI_voidEnableLineInterrupt(u8 line)
{
	NVIC_voidEnableInterrupt(EXTI_u8FindVector(line));
}

void EXTI_voidDisbleLineInterrupt(u8 line)
{
	NVIC_voidDisableInterrupt(EXTI_u8FindVector(line));
}

void EXTI_voidEnableLine(u8 line)
{
	SET_BIT(EXTI->IMR, line);
}

void EXTI_voidDisableLine(u8 line)
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

void EXTI0_IRQHandler(void) 	{EXTI_callback[0](); SET_BIT(EXTI->PR, 0);}
void EXTI1_IRQHandler(void) 	{EXTI_callback[1](); SET_BIT(EXTI->PR, 1);}
void EXTI2_IRQHandler(void) 	{EXTI_callback[2](); SET_BIT(EXTI->PR, 2);}
void EXTI3_IRQHandler(void) 	{EXTI_callback[3](); SET_BIT(EXTI->PR, 3);}
void EXTI4_IRQHandler(void) 	{EXTI_callback[4](); SET_BIT(EXTI->PR, 4);}
void EXTI9_5_IRQHandler(void)
{
	/*
	 * as this IRQ is common among 5 different EXTI lines, loop on the status
	 * of them five, and execute and clear flag of whatever of them is high.
	 */
	for (u8 i = 5; i < 10; i++)
	{
		if (GET_BIT(EXTI->PR, i))
		{
			EXTI_callback[i]();
			SET_BIT(EXTI->PR, i);
		}
	}
}
void EXTI15_10_IRQHandler(void)
{
	/*
	 * as this IRQ is common among 6 different EXTI lines, loop on the status
	 * of them six, and execute and clear flag of whatever of them is high.
	 */
	for (u8 i = 10; i < 16; i++)
	{
		if (GET_BIT(EXTI->PR, i))
		{
			EXTI_callback[i]();
			SET_BIT(EXTI->PR, i);
		}
	}
}
