/*
 * PWR_program.c
 *
 *  Created on: Oct 8, 2022 1:25:21 PM
 *      Author: Ali Emad Ali
 */


/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Target_config.h"

/*	SELF	*/
#include "PWR_interface.h"
#include "PWR_private.h"


/*
 * enables backup domain protection
 * against parasitic write in reset cases.
 */
void PWR_voidEnableDBP(void)
{
	CLR_BIT(PWR->CR, 8);
}

/*
 * disables backup domain protection,
 * giving user the ability to write on RTC
 * and backup registers.
 */
void PWR_voidDisableDBP(void)
{
	SET_BIT(PWR->CR, 8);
}
