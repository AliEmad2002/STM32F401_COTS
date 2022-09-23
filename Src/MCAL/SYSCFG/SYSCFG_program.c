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


#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include "GPIO_interface.h"

#include "SYSCFG_private.h"
#include "SYSCFG_interface.h"

void MSYSCFG_voidSetEXTILineSource(u8 line, DIO_PORT port)
{
	/*	range check	*/
	if (line > 15)
	{
		/*	wrong line	*/
		while(1);
	}

	EDT_REG(SYSCFG_EXTICR[line/4], 4*(line%4), port, 4);
}
