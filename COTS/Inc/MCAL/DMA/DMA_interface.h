/*
 * DMA_interface.h
 *
 *  Created on: Oct 11, 2022
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MCAL_DMA_DMA_INTERFACE_H_
#define INCLUDE_MCAL_DMA_DMA_INTERFACE_H_

typedef enum{
	DMA_UnitNumber_1,
	DMA_UnitNumber_2
}DMA_UnitNumber_t;

typedef enum{
	DMA_Flag_Global,
	DMA_Flag_TransferComplete,
	DMA_Flag_HalfTransfer,
	DMA_Flag_TransferError
}DMA_Flag_t;

typedef enum{
	DMA_Interrupt_TransferComplete,
	DMA_Interrupt_HalfTransfer,
	DMA_Interrupt_TransferError
}DMA_Interrupt_t;

typedef enum{
	DMA_ChannelNumber_1,
	DMA_ChannelNumber_2,
	DMA_ChannelNumber_3,
	DMA_ChannelNumber_4,
	DMA_ChannelNumber_5,
	DMA_ChannelNumber_6,
	DMA_ChannelNumber_7
}DMA_ChannelNumber_t;

typedef enum{
	DMA_Direction_ReadPeripheral,
	DMA_Direction_ReadMemory
}DMA_Direction_t;

typedef enum{
	DMA_Size_8bits,
	DMA_Size_16bits,
	DMA_Size_32bits
}DMA_Size_t;

typedef enum{
	DMA_Priority_Low,
	DMA_Priority_Medium,
	DMA_Priority_High,
	DMA_Priority_VeryHigh
}DMA_Priority_t;

/*	range check	*/
#define DMA_NOT_IN_RANGE(uN, cN)	(((uN) == DMA_UnitNumber_2)	&&		((cN) > DMA_ChannelNumber_5))

/*	reads certain argumented DMA status	*/
b8 DMA_b8ReadFlag(
	const DMA_UnitNumber_t unitNumber, const DMA_ChannelNumber_t channelNumber,
	const DMA_Flag_t flag);

/*	clears certain argumented DMA status	*/
void DMA_voidClearFlag(
	const DMA_UnitNumber_t unitNumber, const DMA_ChannelNumber_t channelNumber,
	const DMA_Flag_t flag);

/*	enables interrupt	*/
void DMA_voidEnableInterrupt(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, DMA_Interrupt_t interrupt);

/*	disables interrupt	*/
void DMA_voidDisableInterrupt(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, DMA_Interrupt_t interrupt);

/*	sets interrupt callback	*/
void DMA_voidSetInterruptCallback(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, void (*functionPointer)(void));

/*	enables channel	*/
void DMA_voidEnableChannel(
	const DMA_UnitNumber_t unitNumber, const DMA_ChannelNumber_t channelNumber);

/*	disables channel	*/
void DMA_voidDisableChannel(
	const DMA_UnitNumber_t unitNumber, const DMA_ChannelNumber_t channelNumber);

/*	checks if certain channel is enabled	*/
b8 DMA_b8IsEnabledChannel(
	const DMA_UnitNumber_t unitNumber, const DMA_ChannelNumber_t channelNumber);

/*	selects data transfer direction	*/
void DMA_voidSelectDataTransferDirection(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, DMA_Direction_t dir);

/*
 * enables circular mode.
 * (auto reloads pointer and number of data registers to
 * their initially programmed values, and repeat data transfer
 * automatically).
 */
void DMA_voidEnableCircularMode(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber);

/*	disables circular mode	*/
void DMA_voidDisableCircularMode(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber);

/*	enables peripheral increment	*/
void DMA_voidEnablePeripheralIncrement(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber);

/*	disables peripheral increment	*/
void DMA_voidDisablePeripheralIncrement(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber);

/*	enables memory increment	*/
void DMA_voidEnableMemoryIncrement(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber);

/*	disables memory increment	*/
void DMA_voidDisableMemoryIncrement(
	const DMA_UnitNumber_t unitNumber, const DMA_ChannelNumber_t channelNumber);

/*	selects peripheral size	*/
void DMA_voidSelectPeripheralSize(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, DMA_Size_t size);

/*	selects memory size	*/
void DMA_voidSelectMemorySize(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, DMA_Size_t size);

/*	selects channel priority level	*/
void DMA_voidSelectPriority(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, DMA_Priority_t priority);

/*
 * enables memory to memory mode.
 * (no need for peripheral call, DMA works directly)
 */
void DMA_voidEnableMemToMemMode(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber);

/*	disables memory to memory mode	*/
void DMA_voidDisableMemToMemMode(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber);

/*
 * sets number of data transfers to be made by argumented DMA channel.
 * (can only be written when channel is not enabled)
 */
void DMA_voidSetNumberOfData(
	const DMA_UnitNumber_t unitNumber, const DMA_ChannelNumber_t channelNumber,
	const u16 numberOfData);

/*	gets number of data	*/
u16 DMA_u16GetNumberOfData(
	const DMA_UnitNumber_t unitNumber, const DMA_ChannelNumber_t channelNumber);

/*
 * sets peripheral address.
 * (can only be written when channel is not enabled)
 */
void DMA_voidSetPeripheralAddress(DMA_UnitNumber_t unitNumber, DMA_ChannelNumber_t channelNumber, void* pointer);

/*
 * sets memory address.
 * (can only be written when channel is not enabled)
 */
void DMA_voidSetMemoryAddress(
	const DMA_UnitNumber_t unitNumber, const DMA_ChannelNumber_t channelNumber,
	void* const pointer);

#endif /* INCLUDE_MCAL_DMA_DMA_INTERFACE_H_ */
