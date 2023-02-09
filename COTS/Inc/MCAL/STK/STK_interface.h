/*
 * STK_interface.h
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 */

#ifndef STK_INTERFACE_H_
#define STK_INTERFACE_H_


#define STK_CLOCKSOURCE_AHB_BY8		0
#define STK_CLOCKSOURCE_AHB			1

typedef enum{
	STK_InterruptType_periodic	=	0,
	STK_InterruptType_onlyOnce	=	1
}STK_InterruptType_t;

typedef enum{
	STK_TickMeasureType_noOverflowCount	=	0,
	STK_TickMeasureType_OverflowCount	=	1
}STK_TickMeasureType_t;

u32 STK_u32GetTicksPerSecond(void);

/*
 * init only (using params configured in ".config" file, does not enable yet.
 */
void STK_voidInit(void);

/*	enable	*/
void STK_voidEnableSysTick(void);

/*	disable	*/
void STK_voidDisableSysTick(void);

/*	setting callback	*/
void STK_voidSetInterruptCallBack(void(*callBack)(void));

/*	enabling interrupt	*/
void STK_voidEnableInterrupt(STK_InterruptType_t intType);

/*	disabling interrupt	*/
void STK_voidDisableInterrupt(void);

/*	writing load register	*/
void STK_voidReload(u32 load);

/*	reading val register	*/
u32 STK_u32GetCurrentVal(void);

/*	writing val register	*/
void STK_voidSetCurrentVal(u32 val);

/*	accurate delay	*/
/*	if a much more larger delay was needed, it's better use the "nop" function	*/
void STK_voidBusyWait(u32 ticks);

/*	start tick measure	*/
/* using OVF counter would overflow the counting process every 92 years,
 * while not using it would  overflow the counting process every 171 seconds
 * both cases are assumed to be using 25MHz AHB CLK without prescaling on sysTick,
 * both are calculated in here:
 **/
void STK_voidStartTickMeasure(STK_TickMeasureType_t type);

void STK_voidStopTickMeasure(STK_TickMeasureType_t type);

/*	getting timestamp	*/
u32 STK_u32GetElapsedTicks(void);
u64 STK_u64GetElapsedTicks(void);

/*	notice that countFlag clears to zero once read	*/
b8 STK_b8GetCountFlag(void);

void STK_voidUpdatetTicksPerSecond(void);

b8 STK_b8IsTickDivBy8(void);

/*	sysTick ticks per mS	*/
#define STK_TICKS_PER_MS			(STK_u32GetTicksPerSecond() / 1000)

#endif /* STK_INTERFACE_H_ */
