/*
 * STK_interface.h
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef STK_INTERFACE_H_
#define STK_INTERFACE_H_

typedef enum{
	MSTK_CLKSOURCE_AHB_by8	=	0,
	MSTK_CLKSOURCE_AHB		=	1
}MSTK_CLKSOURCE_t;

typedef enum{
	MSTK_INTTYPE_periodic	=	0,
	MSTK_INTTYPE_onlyOnce	=	1
}MSTK_INTTYPE_t;

typedef enum{
	MSTK_TICKMEASURETYPE_noOverflowCount	=	0,
	MSTK_TICKMEASURETYPE_OverflowCount		=	1
}MSTK_TICKMEASURETYPE_t;

/*	init only, does not enable yet	*/
void MSTK_voidInit(MSTK_CLKSOURCE_t clkSource, u32 preload);

/*	enable	*/
void MSTK_voidEnableSysTick(void);

/*	disable	*/
void MSTK_voidDisableSysTick(void);

/*	setting callback	*/
void MSTK_voidSetInterruptCallBack(void(*callBack)(void));

/*	enabling interrupt	*/
void MSTK_voidEnableInterrupt(MSTK_INTTYPE_t intType);

/*	disabling interrupt	*/
void MSTK_voidDisableInterrupt(void);

/*	writing load register	*/
void MSTK_voidReload(u32 load);

/*	reading val register	*/
u32 MSTK_u32GetCurrentVal(void);

/*	writing val register	*/
void MSTK_voidSetCurrentVal(u32 val);

/*	accurate delay	*/
/*	if a much more larger delay was needed, it's better use the "nop" function	*/
void MSTK_voidBusyWait(u32 ticks);

/*	start tick measure	*/
/* using OVF counter would overflow the counting process every 92 years,
 * while not using it would  overflow the counting process every 171 seconds
 * both cases are assumed to be using 25MHz AHB CLK without prescaling on sysTick,
 * both are calculated in here:
 **/
void MSTK_voidStartTickMeasure(MSTK_TICKMEASURETYPE_t type);

/*	getting timestamp	*/
u32 MSTK_u32GetElapsedTicks(void);
u64 MSTK_u64GetElapsedTicks(void);

/*	notice that countFlag clears to zero once read	*/
b8 MSTK_b8GetCountFlag(void);

u32 MSTK_u32GetTicksPerSecond(void);

b8 MSTK_b8IsTickDivBy8(void);

#endif /* STK_INTERFACE_H_ */
