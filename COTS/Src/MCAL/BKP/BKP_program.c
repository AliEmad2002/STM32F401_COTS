/*
 * BKP_program.c
 *
 *  Created on: Oct 8, 2022 2:16:39 AM
 *      Author: Ali Emad Ali
 */


/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"

/*	MCAL	*/
#include "PWR_interface.h"

/*	SELF	*/
#include "BKP_interface.h"
#include "BKP_private.h"

void BKP_voidUnlockStoreLock(u16 data, u8 location)
{
	/*	disable backup domain protection	*/
	PWR_voidDisableDBP();

	/*	write	*/
	BKP_voidStoreData(data, location);

	/*
	 * enable backup domain protection
	 * (very important, as protects the
	 * backup registers against parasitic
	 * writes).
	 */
	PWR_voidEnableDBP();
}

void BKP_voidStoreData(u16 data, u8 location)
{
	BKP->data1[location] = data;
}

u16 BKP_u16GetData(u8 location)
{
	return BKP->data1[location];
}
