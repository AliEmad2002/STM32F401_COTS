/*
 * SPI_interface.h
 *
 *  Created on: Oct 1, 2022 10:23:45 PM
 *      Author: Ali Emad Ali
 */

#ifndef _SPI_SPI_INTERFACE_H_
#define _SPI_SPI_INTERFACE_H_


typedef enum{
	SPI_UnitNumber_1,
	SPI_UnitNumber_2,
	SPI_UnitNumber_3
}SPI_UnitNumber_t;

typedef enum{
	SPI_Directional_Mode_Uni,		//	2-line
	SPI_Directional_Mode_Bi			//	1-line
}SPI_Directional_Mode_t;

typedef enum{
	SPI_Bidirectional_Output_Disabled,
	SPI_Bidirectional_Output_Enabled
}SPI_Bidirectional_Output_t;

typedef enum{
	SPI_DataFrameFormat_8bit,
	SPI_DataFrameFormat_16bit
}SPI_DataFrameFormat_t;

typedef enum{
	SPI_Unidirectional_ReceiveOnly_Enabled,
	SPI_Unidirectional_ReceiveOnly_Disabled
}SPI_Unidirectional_ReceiveOnly_t;

typedef enum{
	SPI_SW_SlaveManagement_Disabled,
	SPI_SW_SlaveManagement_Enabled
}SPI_SW_SlaveManagement_t;

typedef enum{
	SPI_FrameDirection_MSB_First,
	SPI_FrameDirection_LSB_First
}SPI_FrameDirection_t;

typedef enum{
	SPI_Prescaler_2,
	SPI_Prescaler_4,
	SPI_Prescaler_8,
	SPI_Prescaler_16,
	SPI_Prescaler_32,
	SPI_Prescaler_64,
	SPI_Prescaler_128,
	SPI_Prescaler_256
}SPI_Prescaler_t;

typedef enum{
	SPI_Mode_Slave,
	SPI_Mode_Master
}SPI_Mode_t;

typedef enum{
	SPI_ClockPolarity_0Idle,
	SPI_ClockPolarity_1Idle
}SPI_ClockPolarity_t;

typedef enum{
	SPI_ClockPhase_CaptureFirst,
	SPI_ClockPhase_CaptureSecond
}SPI_ClockPhase_t;

typedef enum{
	SPI_Interrupt_RxDMA,
	SPI_Interrupt_TxDMA,
	SPI_Interrupt_Error,
	SPI_Interrupt_RxNotEmpty,
	SPI_Interrupt_TxEmpty
}SPI_Interrupt_t;

typedef enum{
	SPI_Flag_RxNotEmpty,
	SPI_Flag_TxEmpty,
	SPI_Flag_Underrun = 3,
	SPI_Flag_CRC_Error,
	SPI_Flag_ModeFault,
	SPI_Flag_Overrun,
	SPI_Flag_Busy
}SPI_Flag_t;

/*
 * enables SPI peripheral
 */
void SPI_voidEnableUnit(SPI_UnitNumber_t unitNumber);

/*
 * disables SPI peripheral
 */
void SPI_voidDisableUnit(SPI_UnitNumber_t unitNumber);

/*
 * inits a SPI peripheral.
 * output is disabled by default, use "", or "" to enable it.
 */
void SPI_voidInit(
		SPI_UnitNumber_t unitNumber, SPI_Directional_Mode_t directionalMode,
		SPI_DataFrameFormat_t frameFormat, SPI_FrameDirection_t frameDirection,
		SPI_Prescaler_t prescaler, SPI_Mode_t mode, SPI_ClockPolarity_t clockPolarity,
		SPI_ClockPhase_t clockPhase
	);

/*
 * enables output for any of the two directional modes.
 */
void SPI_voidEnableOutput(SPI_UnitNumber_t unitNumber, SPI_Directional_Mode_t directionalMode);

/*
 * disables output for any of the two directional modes.
 */
void SPI_voidDisableOutput(SPI_UnitNumber_t unitNumber, SPI_Directional_Mode_t directionalMode);

/*
 * enables SPI peripheral interrupt.
 */
void SPI_voidEnableInterrupt(SPI_UnitNumber_t unitNumber, SPI_Interrupt_t interrupt);

/*
 * disables SPI peripheral interrupt.
 */
void SPI_voidDisableInterrupt(SPI_UnitNumber_t unitNumber, SPI_Interrupt_t interrupt);

/*
 * SW slave management enable
 */
void SPI_voidEnableSSM(SPI_UnitNumber_t unitNumber);

/*
 * SW slave management disable
 */
void SPI_voidDisableSSM(SPI_UnitNumber_t unitNumber);

/*
 * SW slave management select (self select)
 */
void SPI_voidSoftwareSlaveSelect(SPI_UnitNumber_t unitNumber);

/*
 * SW slave management deselect (self deselect)
 */
void SPI_voidSoftwareSlaveDeselect(SPI_UnitNumber_t unitNumber);

/*
 * enables SS output when the peripheral is in master mode.
 */
void SPIvoidMasterEnableSlaveSelectOutput(SPI_UnitNumber_t unitNumber);

/*
 * disables SS output when the peripheral is in master mode.
 * (thus it can be selected as a slave by other master, used
 * in multi-master network).
 */
void SPIvoidMasterDisableSlaveSelectOutput(SPI_UnitNumber_t unitNumber);

/*
 * returns the value of the given flag
 */
b8 SPI_b8ReadFlag(SPI_UnitNumber_t unitNumber, SPI_Flag_t flag);

/*
 * transceives data
 */
u16 SPI_u16TransceiveData(SPI_UnitNumber_t unitNumber, u16 data);

/*
 * send data only
 * (faster when interfacing devices that do not response)
 */
void SPI_voidTransmitData(SPI_UnitNumber_t unitNumber, u16 data);

#endif /* _SPI_SPI_INTERFACE_H_ */










