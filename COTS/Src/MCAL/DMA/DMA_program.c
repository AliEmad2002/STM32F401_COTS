/*
 * DMA_program.c
 *
 *  Created on: Oct 11, 2022
 *      Author: Ali Emad Ali
 */



/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Error_Handler_interface.h"

/*	SELF	*/
#include "DMA_interface.h"
#include "DMA_private.h"

void (*DMA1GlobalInterruptCallbackArr[7])(void) = {NULL};

void (*DMA2GlobalInterruptCallbackArr[4])(void) = {NULL};

/*	reads certain argumented DMA status	*/
b8 DMA_b8ReadFlag(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, DMA_Flag_t flag)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	return GET_BIT(DMA[unitNumber]->ISR, flag + channelNumber * 4);
}

/*	clears certain argumented DMA status	*/
void DMA_voidClearFlag(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, DMA_Flag_t flag)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	SET_BIT(DMA[unitNumber]->ISR, flag + channelNumber * 4);
}

/*	enables interrupt	*/
void DMA_voidEnableInterrupt(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, DMA_Interrupt_t interrupt)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	SET_BIT(DMA[unitNumber]->c[channelNumber].CR, interrupt + 1);
}

/*	disables interrupt	*/
void DMA_voidDisableInterrupt(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, DMA_Interrupt_t interrupt)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	CLR_BIT(DMA[unitNumber]->c[channelNumber].CR, interrupt + 1);
}


/*	sets interrupt callback	*/
void DMA_voidSetInterruptCallback(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, void (*functionPointer)(void))
{
	if (unitNumber == DMA_UnitNumber_1)
	{
		DMA1GlobalInterruptCallbackArr[channelNumber] = functionPointer;
	}

	else
	{
		if (channelNumber > DMA_ChannelNumber_5)
		{
			ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
		}
		else if (channelNumber > DMA_ChannelNumber_3)
		{
			DMA2GlobalInterruptCallbackArr[3] = functionPointer;
		}
		else
		{
			DMA2GlobalInterruptCallbackArr[channelNumber] = functionPointer;
		}
	}
}

/*	enables channel	*/
void DMA_voidEnableChannel(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	SET_BIT(DMA[unitNumber]->c[channelNumber].CR, DMA_CCR_EN);
}

/*	disables channel	*/
void DMA_voidDisableChannel(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	CLR_BIT(DMA[unitNumber]->c[channelNumber].CR, DMA_CCR_EN);
}

/*	selects data transfer direction	*/
void DMA_voidSelectDataTransferDirection(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, DMA_Direction_t dir)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	WRT_BIT(DMA[unitNumber]->c[channelNumber].CR, DMA_CCR_DIR, dir);
}

/*	enables circular mode	*/
void DMA_voidEnableCircularMode(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	SET_BIT(DMA[unitNumber]->c[channelNumber].CR, DMA_CCR_CIRC);
}

/*
 * enables circular mode.
 * (auto reloads pointer and number of data registers to
 * their initially programmed values, and repeat data transfer
 * automatically).
 */
void DMA_voidDisableCircularMode(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	CLR_BIT(DMA[unitNumber]->c[channelNumber].CR, DMA_CCR_CIRC);
}

/*	enables peripheral increment	*/
void DMA_voidEnablePeripheralIncrement(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	SET_BIT(DMA[unitNumber]->c[channelNumber].CR, DMA_CCR_PINC);
}

/*	disables peripheral increment	*/
void DMA_voidDisablePeripheralIncrement(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	CLR_BIT(DMA[unitNumber]->c[channelNumber].CR, DMA_CCR_PINC);
}

/*	enables memory increment	*/
void DMA_voidEnableMemoryIncrement(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	SET_BIT(DMA[unitNumber]->c[channelNumber].CR, DMA_CCR_MINC);
}

/*	disables memory increment	*/
void DMA_voidDisableMemoryIncrement(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	CLR_BIT(DMA[unitNumber]->c[channelNumber].CR, DMA_CCR_MINC);
}

/*	selects peripheral size	*/
void DMA_voidSelectPeripheralSize(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, DMA_Size_t size)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	EDT_REG(DMA[unitNumber]->c[channelNumber].CR, DMA_CCR_PSIZE_0, size, 2);
}

/*	selects memory size	*/
void DMA_voidSelectMemorySize(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, DMA_Size_t size)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	EDT_REG(DMA[unitNumber]->c[channelNumber].CR, DMA_CCR_MSIZE_0, size, 2);
}

/*	selects channel priority level	*/
void DMA_voidSelectPriority(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, DMA_Priority_t priority)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	EDT_REG(DMA[unitNumber]->c[channelNumber].CR, DMA_CCR_PL_0, priority, 2);
}

/*
 * enables memory to memory mode.
 * (no need for peripheral call, DMA works directly)
 */
void DMA_voidEnableMemToMemMode(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	SET_BIT(DMA[unitNumber]->c[channelNumber].CR, DMA_CCR_MEM2MEM);
}

/*	disables memory to memory mode	*/
void DMA_voidDisableMemToMemMode(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	CLR_BIT(DMA[unitNumber]->c[channelNumber].CR, DMA_CCR_MEM2MEM);
}

/*
 * sets number of data transfers to be made by argumented DMA channel.
 * (can only be written when channel is not enabled)
 */
void DMA_voidSetNumberOfData(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, u16 numberOfData)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	DMA[unitNumber]->c[channelNumber].NDTR = numberOfData;
}

/*
 * sets peripheral address.
 * (can only be written when channel is not enabled)
 */
void DMA_voidSetPeripheralAddress(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, void* pointer)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	DMA[unitNumber]->c[channelNumber].PAR = (u32)pointer;
}

/*
 * sets memory address.
 * (can only be written when channel is not enabled)
 */
void DMA_voidSetMemoryAddress(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, void* pointer)
{
	/*	range check	*/
	if (DMA_NOT_IN_RANGE(unitNumber, channelNumber))
	{
		ErrorHandler_voidExecute(DMA_NOT_IN_RANGE_ERR_CODE);
	}

	DMA[unitNumber]->c[channelNumber].MAR = (u32)pointer;
}

/*	ISR handlers	*/
void DMA1_Channel1_IRQHandler(void)		{DMA1GlobalInterruptCallbackArr[0]();}
void DMA1_Channel2_IRQHandler(void)		{DMA1GlobalInterruptCallbackArr[1]();}
void DMA1_Channel3_IRQHandler(void)		{DMA1GlobalInterruptCallbackArr[2]();}
void DMA1_Channel4_IRQHandler(void)		{DMA1GlobalInterruptCallbackArr[3]();}
void DMA1_Channel5_IRQHandler(void)		{DMA1GlobalInterruptCallbackArr[4]();}
void DMA1_Channel6_IRQHandler(void)		{DMA1GlobalInterruptCallbackArr[5]();}
void DMA1_Channel7_IRQHandler(void)		{DMA1GlobalInterruptCallbackArr[6]();}

void DMA2_Channel1_IRQHandler(void)		{DMA2GlobalInterruptCallbackArr[0]();}
void DMA2_Channel2_IRQHandler(void)		{DMA2GlobalInterruptCallbackArr[1]();}
void DMA2_Channel3_IRQHandler(void)		{DMA2GlobalInterruptCallbackArr[2]();}
void DMA2_Channel4_5_IRQHandler(void)	{DMA2GlobalInterruptCallbackArr[3]();}



















