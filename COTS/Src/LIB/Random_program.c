/*
 * Random_program.c
 *
 *  Created on: Oct 8, 2022 1:50:17 PM
 *      Author: Ali Emad Ali
 */


/*	LIB	*/
#include <stdlib.h>
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Target_config.h"

/*	MCAL	*/
#include "BKP_interface.h"
#include "PWR_interface.h"

/*	SELF	*/
#include "random_interface.h"


/*
 * loads the previously stored initial number to "srand()"
 */
void Random_voidLoadInitialStart(void)
{
	srand(BKP_u16GetData(0));
}

/*
 * changes the value of the number called in
 * "Random_voidLoadInitialStart()", to emulate
 * a user-acceptable randomization.
 */
void Random_voidRefreshNextStartInitialNumber(void)
{
	/*	disable backup domain protection	*/
	PWR_voidDisableDBP();
	/*	write	*/
	BKP_voidStoreData(rand()%65535, 0);
	/*
	 * enable backup domain protection
	 * (very important, as protects the
	 * backup registers against parasitic
	 * writes).
	 */
	PWR_voidEnableDBP();
}
