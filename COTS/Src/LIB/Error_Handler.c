/*
 * Error_Handler.c
 *
 *  Created on: Oct 12, 2022
 *      Author: Ali Emad Ali
 */


/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"

/*	SELF	*/
#include "Error_Handler_config.h"
#include "Error_Handler_interface.h"

/*
 * optionally used and assigned in the config file,
 * if there're no other handlers made.
 */
void ErrorHandler(u32 errCode)
{
	while(1)
	{
		// do no thing.
	}
}

/*	error handler execution	*/
void ErrorHandler_voidExecute(u32 errCode)
{
	ERR_HANDLER(errCode);
}
