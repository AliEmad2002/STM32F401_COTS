/*
 * Error_Handler.c
 *
 *  Created on: Oct 12, 2022
 *      Author: Ali Emad Ali
 */


/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Debug_active.h"

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
		#if DEBUG_ON == 1
		trace_printf("Entered Error handler with code %d\n", errCode);
		#endif
	}
}

/*	error handler execution	*/
void ErrorHandler_voidExecute(ErrorHandler_ErrorSource_t errSource)
{
	ERR_HANDLER(errSource);
}

void HardFault_IRQ(void)
{
	ErrorHandler(55);
}
