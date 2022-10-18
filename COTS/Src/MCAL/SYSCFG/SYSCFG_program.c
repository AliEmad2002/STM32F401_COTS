/*
 * SYSCFG_program.c
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
#include "SYSCFG_interface.h"

#if TARGET_ID == STM32F401x

/*	SELF	*/
#include "SYSCFG_private.h"
#include "SYSCFG_interface.h"

void MSYSCFG_voidSetEXTILineSource(u8 line, GPIO_PortName_t port)
{
	EDT_REG(SYSCFG_EXTICR[line/4], 4*(line%4), port, 4);
}
#endif
