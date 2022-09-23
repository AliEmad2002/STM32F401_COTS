/*
 * STK_program.c
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

/*	LIB	*/
#include "STD_TYPES.h"
#include "BIT_MATH.h"
/*	MCAL	*/
#include "RCC_interface.h"

/*	SELF	*/
#include "STK_private.h"
#include "STK_interface.h"

#ifndef NULL
#define NULL	((void*)0x0)
#endif

#ifndef MAX_u24
#define MAX_u24	16777215
#endif

static void (*MSTK_INT_callBack)(void) = NULL;

static b8 MSTK_b8InterruptOnlyOnce = false;
static b8 hadExcutedISR = false;

static u32 ovfCount = 0;
static b8 ovfCountEnabled = false;

/*	init only, does not enable yet	*/
void MSTK_voidInit(MSTK_CLKSOURCE_t clkSource, u32 preload)
{
	WRT_BIT(STK_CTRL, STK_CLKSOURCE_BIT, clkSource);
	STK_LOAD = preload;
	STK_VAL = 0;
}

/*	enable	*/
void MSTK_voidEnableSysTick(void)
{
	SET_BIT(STK_CTRL, STK_ENABLE_BIT);
}

/*	disable	*/
void MSTK_voidDisableSysTick(void)
{
	CLR_BIT(STK_CTRL, STK_ENABLE_BIT);
}

/*	setting callback	*/
void MSTK_voidSetInterruptCallBack(void(*callBack)(void))
{
	MSTK_INT_callBack = callBack;
}

/*	enabling interrupt	*/
void MSTK_voidEnableInterrupt(MSTK_INTTYPE_t intType)
{
	/*	erase any previous request	*/
	(void)GET_BIT(STK_CTRL, STK_COUNTFLAG_BIT);

	MSTK_b8InterruptOnlyOnce = intType;
	SET_BIT(STK_CTRL, STK_TICKINT_BIT);
}

/*	disabling interrupt	*/
void MSTK_voidDisableInterrupt(void)
{
	CLR_BIT(STK_CTRL, STK_TICKINT_BIT);
}

/*	writing load register	*/
/* notice that load register is to be copied to val register and started counting from
 * only and only after val register reaches zero and count flag is raised.
 */
void MSTK_voidReload(u32 load)
{
	STK_LOAD = load;
}

/*	reading val register	*/
u32 MSTK_u32GetCurrentVal(void)
{
	return STK_VAL;
}

/*	accurate delay	*/
/*	if a much more larger delay was needed, it's better use the "nop" function	*/
void MSTK_voidBusyWait(u32 ticks)
{
	/*	preload	*/
	STK_LOAD = ticks;

	/*	clear val register and count flag	*/
	/*	both are cleared by writing any value to val register	*/
	STK_VAL = 0;

	/* ticks value must've been copied to val register and
	 * is being counted down to zero, wait for it to reach zero.
	 */
	while(!GET_BIT(STK_CTRL, STK_COUNTFLAG_BIT));
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
void MSTK_voidStartTickMeasure(MSTK_TICKMEASURETYPE_t type)
{
	if (type == MSTK_TICKMEASURETYPE_OverflowCount)
		ovfCountEnabled = true;

	STK_LOAD = MAX_u24;

	/*
	 * Remember:
	 * STK_LOAD value gets copied into STK_VAL only and only when the last becomes zero.
	 */
	STK_VAL = 0;

	MSTK_voidEnableInterrupt(MSTK_INTTYPE_periodic);
}

/*	getting timestamp	*/
/*
 * used only when ovfCount is disabled, because it has a maximum counting capability of:
 * (2^24 - 1) / F_CLK second =~ 671ms @ F_CLK = 25MHz, sisTick_prescaler = 1;
 */
u32 MSTK_u32GetElapsedTicks(void)
{
	return MAX_u24 - STK_VAL;
}

/*
 * used only when ovfCOunt is enabled, has a maximum counting capability of:
 * (2^24 - 1) * (2^32 - 1) / F_CLK =~ 92 years. 😅😂 suit your self.
 */
u64 MSTK_u64GetElapsedTicks(void)
{
	return ((u64)ovfCount+1) * (u64)MAX_u24 - (u64)STK_VAL;
}

/*	notice that countFlag clears to zero once read	*/
b8 MSTK_b8GetAndClearCountFlag(void)
{
	return GET_BIT(STK_CTRL, STK_COUNTFLAG_BIT);
}

u32 MSTK_u32GetTicksPerSecond(void)
{
	/*	ticksPerSecond = F_sys / AHB_prescaler / sysTick_over8	*/
	u32 stkOver8 = MSTK_b8IsTickDivBy8() ? 8 : 1;
	return RCC_u32GetSysClk() / ((u32)RCC_u16GetBusPrescaler(RCC_BUS_AHB1) * stkOver8);
}

b8 MSTK_b8IsTickDivBy8(void)
{
	return !GET_BIT(STK_CTRL, STK_CLKSOURCE_BIT);
}

/*	Handler	*/
void SysTick_Handler(void)
{
	/* increment ovfCount (if enabled)	*/
	if (ovfCountEnabled)
		ovfCount++;

	/*	only once?	*/
	if (MSTK_b8InterruptOnlyOnce)
	{
		if (hadExcutedISR)
			goto clearingInterruptFlag;
		else
			hadExcutedISR = true;
	}

	/*	callBack notification	*/
	if (MSTK_INT_callBack != NULL)
		MSTK_INT_callBack();

	/*	clearing interrupt flag (does not get cleared by hardware, gets cleared by SW read)	*/
	clearingInterruptFlag:
	(void)GET_BIT(STK_CTRL, STK_COUNTFLAG_BIT);
}

