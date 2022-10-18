/*
 * SPI_program.c
 *
 *  Created on: Oct 1, 2022 10:32:24 PM
 *      Author: Ali Emad Ali
 */


/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Target_config.h"

/*	MCAL	*/
#include "STK_interface.h"
#include "DMA_interface.h"


/*	SELF	*/
#include "SPI_private.h"
#include "SPI_interface.h"


/*
 * enables SPI peripheral
 */
void SPI_voidEnableUnit(SPI_UnitNumber_t unitNumber)
{
	SET_BIT(SPI[unitNumber]->CR1, SPI_CR1_SPE);
}

/*
 * disables SPI peripheral
 */
void SPI_voidDisableUnit(SPI_UnitNumber_t unitNumber)
{
	CLR_BIT(SPI[unitNumber]->CR1, SPI_CR1_SPE);
}

/*
 * inits a SPI peripheral.
 * output is disabled by default, use "SPI_voidEnableOutput()" to enable it.
 * high buadrates could cause byte shifting, which is not preferred.
 */
void SPI_voidInit(
		SPI_UnitNumber_t unitNumber, SPI_Directional_Mode_t directionalMode,
		SPI_DataFrameFormat_t frameFormat, SPI_FrameDirection_t frameDirection,
		SPI_Prescaler_t prescaler, SPI_Mode_t mode, SPI_ClockPolarity_t clockPolarity,
		SPI_ClockPhase_t clockPhase
	)
{
	WRT_BIT(SPI[unitNumber]->CR1, SPI_CR1_BIDIMODE, directionalMode);

	WRT_BIT(SPI[unitNumber]->CR1, SPI_CR1_DFF, frameFormat);

	WRT_BIT(SPI[unitNumber]->CR1, SPI_CR1_LSBFIRST, frameDirection);

	EDT_REG(SPI[unitNumber]->CR1, SPI_CR1_BR_0, prescaler, 3);

	/*
	 * when slave select pin is not used, and is left floating or
	 * driven high, the chip will deselect its self - if in slave mode.
	 * while in master mode, if it is left floating or driven low,
	 * the chip will select its self and turn into a slave and result
	 * a mode fault. so software slave select is enabled by default at
	 * init, and self select if slave, self deselect if mater.
	 */
	SPI_voidEnableSSM(unitNumber);

	if (mode == SPI_Mode_Slave)
		SPI_voidSoftwareSlaveSelect(unitNumber);
	else
		SPI_voidSoftwareSlaveDeselect(unitNumber);

	WRT_BIT(SPI[unitNumber]->CR1, SPI_CR1_MSTR, mode);

	WRT_BIT(SPI[unitNumber]->CR1, SPI_CR1_CPOL, clockPolarity);

	WRT_BIT(SPI[unitNumber]->CR1, SPI_CR1_CPHA, clockPhase);
}

/*
 * enables output for any of the two directional modes.
 */
void SPI_voidEnableOutput(SPI_UnitNumber_t unitNumber, SPI_Directional_Mode_t directionalMode)
{
	switch (directionalMode)
	{
	case SPI_Directional_Mode_Uni:
		CLR_BIT(SPI[unitNumber]->CR1, SPI_CR1_RXONLY);
		break;
	default:	//case SPI_Directional_Mode_Bi:
		SET_BIT(SPI[unitNumber]->CR1, SPI_CR1_BIDIOE);
	}
}

/*
 * disables output for any of the two directional modes.
 */
void SPI_voidDisableOutput(SPI_UnitNumber_t unitNumber, SPI_Directional_Mode_t directionalMode)
{
	switch (directionalMode)
	{
	case SPI_Directional_Mode_Uni:
		SET_BIT(SPI[unitNumber]->CR1, SPI_CR1_RXONLY);
		break;
	default:	//case SPI_Directional_Mode_Bi:
		CLR_BIT(SPI[unitNumber]->CR1, SPI_CR1_BIDIOE);
	}
}

/*
 * used in "SPI_voidEnableInterrupt()" and "SPI_voidDisableInterrupt()"
 */
const u8 bitNumber[] = {
		SPI_CR2_RXDMAEN, SPI_CR2_TXDMAEN, SPI_CR2_ERRIE, SPI_CR2_RXNEIE, SPI_CR2_TXEIE
	};

/*
 * enables SPI peripheral interrupt.
 */
void SPI_voidEnableInterrupt(SPI_UnitNumber_t unitNumber, SPI_Interrupt_t interrupt)
{
	SET_BIT(SPI[unitNumber]->CR2, bitNumber[interrupt]);
}

/*
 * disables SPI peripheral interrupt.
 */
void SPI_voidDisableInterrupt(SPI_UnitNumber_t unitNumber, SPI_Interrupt_t interrupt)
{
	CLR_BIT(SPI[unitNumber]->CR2, bitNumber[interrupt]);
}

/*
 * SW slave management enable
 */
void SPI_voidEnableSSM(SPI_UnitNumber_t unitNumber)
{
	SET_BIT(SPI[unitNumber]->CR1, SPI_CR1_SSM);
}

/*
 * SW slave management disable
 */
void SPI_voidDisableSSM(SPI_UnitNumber_t unitNumber)
{
	CLR_BIT(SPI[unitNumber]->CR1, SPI_CR1_SSM);
}

/*
 * SW slave management select (self select)
 */
void SPI_voidSoftwareSlaveSelect(SPI_UnitNumber_t unitNumber)
{
	CLR_BIT(SPI[unitNumber]->CR1, SPI_CR1_SSI);
}

/*
 * SW slave management deselect (self deselect)
 */
void SPI_voidSoftwareSlaveDeselect(SPI_UnitNumber_t unitNumber)
{
	SET_BIT(SPI[unitNumber]->CR1, SPI_CR1_SSI);
}

/*
 * enables SS output when the peripheral is in master mode.
 */
void SPIvoidMasterEnableSlaveSelectOutput(SPI_UnitNumber_t unitNumber)
{
	SET_BIT(SPI[unitNumber]->CR2, SPI_CR2_SSOE);
}

/*
 * disables SS output when the peripheral is in master mode.
 * (thus it can be selected as a slave by other master, used
 * in multi-master network).
 */
void SPIvoidMasterDisableSlaveSelectOutput(SPI_UnitNumber_t unitNumber)
{
	CLR_BIT(SPI[unitNumber]->CR2, SPI_CR2_SSOE);
}

/*
 * returns the value of the given flag
 */
b8 SPI_b8ReadFlag(SPI_UnitNumber_t unitNumber, SPI_Flag_t flag)
{
	return GET_BIT(SPI[unitNumber]->SR, flag);
}

/*
 * transceives data.
 */
u16 SPI_u16TransceiveData(SPI_UnitNumber_t unitNumber, u16 data)
{
	/*	wait for SPI peripheral to finish whatever it is doing	*/
	while(SPI_b8ReadFlag(unitNumber, SPI_Flag_Busy));

	/*
	 * wait for Tx buffer to be empty
	 * (to avoid overrunning last sent word/dword)
	 */
	while(!SPI_b8ReadFlag(unitNumber, SPI_Flag_TxEmpty));

	/*	load data register	*/
	SPI[unitNumber]->DR = data;

	/*	wait for SPI peripheral to finish transceiving	*/
	while(SPI_b8ReadFlag(unitNumber, SPI_Flag_Busy));

	/*
	 * wait for Rx buffer to be filled (not empty)
	 * (to avoid overrunning last received word/dword)
	 */
	while(!SPI_b8ReadFlag(unitNumber, SPI_Flag_RxNotEmpty));

	/*	return received data	*/
	u16 received = SPI[unitNumber]->DR;

	return received;
}

/*
 * send data only
 * (faster when interfacing devices that do not response)
 */
void SPI_voidTransmitData(SPI_UnitNumber_t unitNumber, u16 data)
{
	/*	wait for SPI peripheral to finish whatever it is doing	*/
	while(SPI_b8ReadFlag(unitNumber, SPI_Flag_Busy));

	/*
	 * wait for Tx buffer to be empty
	 * (to avoid overrunning last sent word/dword)
	 */
	while(!SPI_b8ReadFlag(unitNumber, SPI_Flag_TxEmpty));

	/*	load data register	*/
	SPI[unitNumber]->DR = data;
}

/*	enables DMA request	*/
void SPI_voidEnableDMA(SPI_UnitNumber_t unitNumber, SPI_DMA_Request_t request)
{
	SET_BIT(SPI[unitNumber]->CR2, request);
}

/*	disables DMA request	*/
void SPI_voidDisableDMA(SPI_UnitNumber_t unitNumber, SPI_DMA_Request_t request)
{
	CLR_BIT(SPI[unitNumber]->CR2, request);
}

/*
 * assigns address of SPI peripheral data register to corresponding
 * chosen DMA channel's peripheral address register.
 */
void SPI_voidSetDMAPARTx(SPI_UnitNumber_t unitNumber)
{
	DMA_UnitNumber_t dmaUnitNumber;
	DMA_ChannelNumber_t dmaChNumber;
	switch (unitNumber)
	{
	case SPI_UnitNumber_1:
		dmaUnitNumber = DMA_UnitNumber_1;
		dmaChNumber = DMA_ChannelNumber_3;
		break;

	case SPI_UnitNumber_2:
		dmaUnitNumber = DMA_UnitNumber_1;
		dmaChNumber = DMA_ChannelNumber_5;
		break;

		default:	//case SPI_UnitNumber_3:
		dmaUnitNumber = DMA_UnitNumber_2;
		dmaChNumber = DMA_ChannelNumber_2;
		break;
	}

	DMA_voidSetPeripheralAddress(dmaUnitNumber, dmaChNumber, (void*)&(SPI[unitNumber]->DR));
}

void SPI_voidSetDMAPARRx(SPI_UnitNumber_t unitNumber)
{
	DMA_UnitNumber_t dmaUnitNumber;
	DMA_ChannelNumber_t dmaChNumber;
	switch (unitNumber)
	{
	case SPI_UnitNumber_1:
		dmaUnitNumber = DMA_UnitNumber_1;
		dmaChNumber = DMA_ChannelNumber_2;
		break;

	case SPI_UnitNumber_2:
		dmaUnitNumber = DMA_UnitNumber_1;
		dmaChNumber = DMA_ChannelNumber_4;
		break;

	default:	//case SPI_UnitNumber_3:
		dmaUnitNumber = DMA_UnitNumber_2;
		dmaChNumber = DMA_ChannelNumber_1;
		break;
	}

	DMA_voidSetPeripheralAddress(dmaUnitNumber, dmaChNumber, (void*)&(SPI[unitNumber]->DR));
}
