/*
 * STK_program.c
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "RCC_interface.h"

/*	SELF	*/
#include "STK_private.h"
#include "STK_interface.h"
#include "STK_config.h"

#ifndef NULL
#define NULL	((void*)0x0)
#endif

#ifndef MAX_u24
#define MAX_u24	16777215
#endif

static void (*interruptCallback)(void) = NULL;

static b8 interruptOnlyOnce = false;
static b8 hadExcutedIsr = false;

static u32 ovfCount = 0;
static b8 ovfCountEnabled = false;

/*
 * init only (using params configured in ".config" file, does not enable yet.
 */
void STK_voidInit(void)
{
	WRT_BIT(STK->CTRL, STK_CLKSOURCE, CLK_SOURCE);
	STK->LOAD = 0;
	STK->VAL = 0;
}

/*	enable	*/
void STK_voidEnableSysTick(void)
{
	SET_BIT(STK->CTRL, STK_ENABLE);
}

/*	disable	*/
void STK_voidDisableSysTick(void)
{
	CLR_BIT(STK->CTRL, STK_ENABLE);
}

/*	setting callback	*/
void STK_voidSetInterruptCallBack(void(*callBack)(void))
{
	interruptCallback = callBack;
}

/*	enabling interrupt	*/
void STK_voidEnableInterrupt(STK_InterruptType_t intType)
{
	/*	erase any previous request	*/
	(void)GET_BIT(STK->CTRL, STK_COUNTFLAG);

	interruptOnlyOnce = intType;
	SET_BIT(STK->CTRL, STK_TICKINT);
}

/*	disabling interrupt	*/
void STK_voidDisableInterrupt(void)
{
	CLR_BIT(STK->CTRL, STK_TICKINT);
}

/*	writing load register	*/
/* notice that load register is to be copied to val register and started counting from
 * only and only after val register reaches zero and count flag is raised.
 */
void STK_voidReload(u32 load)
{
	STK->LOAD = load;
}

/*	reading val register	*/
u32 STK_u32GetCurrentVal(void)
{
	return STK->VAL;
}

/*	accurate delay	*/
/*	if a much more larger delay was needed, it's better use the "nop" function	*/
void STK_voidBusyWait(u32 ticks)
{
	/*	preload	*/
	STK->LOAD = ticks;

	/*	clear val register and count flag	*/
	/*	both are cleared by writing any value to val register	*/
	STK->VAL = 0;

	/* ticks value must've been copied to val register and
	 * is being counted down to zero, wait for it to reach zero.
	 */
	while(!GET_BIT(STK->CTRL, STK_COUNTFLAG));
}

/*	start tick measure	*/
/*
 * This function loads the load register with maximum value, and enables interrupt,
 * and every time the ISR is excuted (val register transits from 0 to 1) ovfCount
 * increments by one (if enabled).
 *
 * It's important to not write the load register with any value other than the max value,
 * and not write the val register at all. Other wise count process could miss.
 *
 * using OVF counter would overflow the counting process every ~92 years,
 * while not using it would  overflow the counting process every ~671ms.
 * both cases are assumed to be using 25MHz AHB CLK without prescaling on sysTick,
 * both are clearly calculated in the comment of the "MSTK_u32GetElapsedTicks()" and
 * "MSTK_u64GetElapsedTicks()" functions.
 **/
void STK_voidStartTickMeasure(STK_TickMeasureType_t type)
{
	if (type == STK_TickMeasureType_OverflowCount)
	{
		ovfCountEnabled = true;
		STK_voidEnableInterrupt(STK_InterruptType_periodic);
	}

	STK->LOAD = MAX_u24;

	/*
	 * Remember:
	 * STK_LOAD value gets copied into STK_VAL only and only when the last becomes zero.
	 */
	STK->VAL = 0;
}

void STK_voidStopTickMeasure(STK_TickMeasureType_t type)
{
	if (type == STK_TickMeasureType_OverflowCount)
	{
		ovfCountEnabled = false;
		STK_voidDisableInterrupt();
		ovfCount = 0;
	}

	STK->LOAD = 0;
	STK->VAL = 0;
}

/*	getting timestamp	*/
/*
 * used only when ovfCount is disabled, because it has a maximum counting capability of:
 * (2^24 - 1) / F_CLK second =~ 671ms @ F_CLK = 25MHz, sisTick_prescaler = 1;
 */
u32 STK_u32GetElapsedTicks(void)
{
	return MAX_u24 - STK->VAL;
}

/*
 * used only when ovfCOunt is enabled, has a maximum counting capability of:
 * (2^24 - 1) * (2^32 - 1) / F_CLK =~ 92 years. 😅😂 suit your self.
 */
u64 STK_u64GetElapsedTicks(void)
{
	return ((u64)ovfCount+1) * (u64)MAX_u24 - (u64)STK->VAL;
}

/*	notice that countFlag clears to zero once read	*/
b8 STK_b8GetAndClearCountFlag(void)
{
	return GET_BIT(STK->CTRL, STK_COUNTFLAG);
}

u32 STK_u32GetTicksPerSecond(void)
{
	/*	ticksPerSecond = F_sys / AHB_prescaler / sysTick_over8	*/
	u32 stkOver8 = (CLK_SOURCE == STK_CLOCKSOURCE_AHB_BY8) ? 8 : 1;
	#if TARGET_ID == STM32F401x
	return RCC_u32GetBusClk(RCC_Bus_AHB1) / stkOver8;
	#elif TARGET_ID == STM32F10x
	return RCC_u32GetBusClk(RCC_Bus_AHB) / stkOver8;
	#endif
}

/*	Handler	*/
void SysTick_Handler(void)
{
	return;
	/* increment ovfCount (if enabled)	*/
	if (ovfCountEnabled)
		ovfCount++;

	/*	only once?	*/
	if (interruptOnlyOnce)
	{
		if (hadExcutedIsr)
			goto clearingInterruptFlag;
		else
			hadExcutedIsr = true;
	}

	/*	callBack notification	*/
	if (interruptCallback != NULL)
		interruptCallback();

	/*	clearing interrupt flag (does not get cleared by hardware, gets cleared by SW read)	*/
	clearingInterruptFlag:
	(void)GET_BIT(STK->CTRL, STK_COUNTFLAG);
}

