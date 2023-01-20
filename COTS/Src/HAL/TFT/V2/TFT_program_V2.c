/*
 * TFT_program.c
 *
 *  Created on: Jan 6, 2023
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Delay_interface.h"
#include "Img_config.h"
#include "Img_interface.h"

/*	MCAL	*/
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "TIM_interface.h"
#include "SPI_interface.h"
#include "DMA_interface.h"
#include "NVIC_interface.h"

/*	SELF	*/
#include "TFT_cmd.h"
#include "TFT_interface_V2.h"

/*
 * Inits TFT object.
 * This function:
 * - Assigns object's elements with proper values, based on arguments.
 * - Inits GPIO, AFIO, SPI, TIM and DMA peripherals associated, all at maximum
 * speeds. Removing much outer overhead.
 * - Executes TFT screen's reset sequence.
 *
 * Notes:
 * - If brightness control was selected, initially brightness is 0%. Hence, user
 * must consider it after 'TFT2_voidInit()' function.
 */
void TFT2_voidInit(
	TFT2_t* tftPtr, SPI_UnitNumber_t spiUnit, u8 spiAFIOMap,
	GPIO_Pin_t rstPin, GPIO_Pin_t A0Pin, u8 bcTimUnitNumber,
	TIM_Channel_t bcTimChannel, u8 bcTimAFIOMap)
{
	/*	store pins data	*/
	tftPtr->rstPin 	= rstPin % 16;
	tftPtr->rstPort = rstPin / 16;

	tftPtr->A0Pin 	= A0Pin % 16;
	tftPtr->A0Port 	= A0Pin / 16;

	/*	enable port's clock (if not)	*/
	u8 peripheralNumber = RCC_PERIPHERAL_IOPA + tftPtr->A0Port;
	if (!RCC_b8IsPeripheralEnabled(RCC_Bus_APB2, peripheralNumber))
		RCC_voidEnablePeripheralClk(RCC_Bus_APB2, peripheralNumber);

	peripheralNumber = RCC_PERIPHERAL_IOPA + tftPtr->rstPort;
	if (!RCC_b8IsPeripheralEnabled(RCC_Bus_APB2, peripheralNumber))
		RCC_voidEnablePeripheralClk(RCC_Bus_APB2, peripheralNumber);

	/*	init pins	*/
	GPIO_voidSetPinGpoPushPull(tftPtr->rstPort, tftPtr->rstPin);
	GPIO_voidSetPinGpoPushPull(tftPtr->A0Port, tftPtr->A0Pin);

	/*	store SPI unit number to be used	*/
	tftPtr->spiUnit = spiUnit;

	/*
	 * as mentioned in datasheet "RM0008 Rev 21" P281/1136: DMA channels
	 * connected to SPIx_Tx are only:
	 * - DMA1, ch3, for SPI1_Tx.
	 * - DMA1, ch5, for SPI2_Tx.
	 * Hence, DMA1 is to be init and always used, channel number is decided
	 * based on 'spiUnit'.
	 */
	RCC_Bus_t bus;
	if (spiUnit == SPI_UnitNumber_1)
	{
		tftPtr->dmaCh = DMA_ChannelNumber_3;
		peripheralNumber = RCC_PERIPHERAL_SPI1;
		bus = RCC_Bus_APB2;
		NVIC_voidEnableInterrupt(NVIC_Interrupt_DMA1_Ch3);
	}
	else
	{
		tftPtr->dmaCh = DMA_ChannelNumber_5;
		peripheralNumber = RCC_PERIPHERAL_SPI2;
		bus = RCC_Bus_APB1;
		NVIC_voidEnableInterrupt(NVIC_Interrupt_DMA1_Ch5);
	}

	/*	enable SPI clock (if not enabled)	*/
	if (!RCC_b8IsPeripheralEnabled(bus, peripheralNumber))
		RCC_voidEnablePeripheralClk(bus, peripheralNumber);

	/*	init SPI	*/
	SPI_voidInit(
		spiUnit, SPI_Directional_Mode_Uni, SPI_DataFrameFormat_8bit,
		SPI_FrameDirection_MSB_First, SPI_Prescaler_2, SPI_Mode_Master,
		SPI_ClockPolarity_1Idle, SPI_ClockPhase_CaptureSecond);

	SPI_voidInitPins(spiUnit, spiAFIOMap, false, false, true);

	SPI_voidEnableDMA(spiUnit, SPI_DMA_Request_Tx);

	SPI_ENABLE_PERIPHERAL(spiUnit);

	/*	enable DMA1 clock (if not enabled)	*/
	DMA_voidEnableRCCClock(DMA_UnitNumber_1);

	/*	init DMA	*/
	DMA_voidSelectPeripheralSize(
		DMA_UnitNumber_1, tftPtr->dmaCh, DMA_Size_8bits);

	DMA_voidSelectMemorySize(
		DMA_UnitNumber_1, tftPtr->dmaCh, DMA_Size_8bits);

	DMA_voidSelectDataTransferDirection(
		DMA_UnitNumber_1, tftPtr->dmaCh, DMA_Direction_ReadMemory);

	SPI_voidSetDMAPARTx(spiUnit);

	/*	store brightness control timer data	*/
	tftPtr->bcTimUnitNumber = bcTimUnitNumber;
	tftPtr->bcTimChannel = bcTimChannel;

	/*	enable TIMx clock (if not enabled)	*/
	TIM_voidEnableTimRCC(bcTimUnitNumber);

	/*	init TIMx	*/
	/*
	 * using PWM2 as control is by convention going to be through a PNP
	 * transistor.
	 */
	TIM_u64InitPWM(
		bcTimUnitNumber, bcTimChannel, TIM_OutputCompareMode_PWM2, 100000);

	TIM_voidInitOutputPin(bcTimUnitNumber, bcTimChannel, bcTimAFIOMap);

	TIM_voidEnableCounter(bcTimUnitNumber);

	/*	Execute reset sequence	*/
	TFT2_voidReset(tftPtr);

	/*	sleep out	*/
	TFT2_WRITE_CMD(tftPtr, TFT_CMD_SLEEP_OUT);
	TFT2_WRITE_CMD(tftPtr, TFT_CMD_SLEEP_OUT); /* check: should it be twice?
										 	   * datasheet does not say so.. */

	/*	set color mode (always RGB565, perfect for STM32)	*/
	TFT2_WRITE_CMD(tftPtr, TFT_CMD_INTERFACE_PIXEL_FORMAT);
	TFT2_WRITE_DATA(tftPtr, 0x05);

	/*	display on	*/
	TFT2_WRITE_CMD(tftPtr, TFT_CMD_DISPLAY_ON);
}

/*
 * resets TFT (executes reset sequence).
 */
void TFT2_voidReset(TFT2_t* tftPtr)
{
	GPIO_voidSetPinOutputLevel(
		tftPtr->rstPort, tftPtr->rstPin, GPIO_OutputLevel_High);
	Delay_voidBlockingDelayUs(100);

	GPIO_voidSetPinOutputLevel(
		tftPtr->rstPort, tftPtr->rstPin, GPIO_OutputLevel_Low);
	Delay_voidBlockingDelayUs(1);

	GPIO_voidSetPinOutputLevel(
		tftPtr->rstPort, tftPtr->rstPin, GPIO_OutputLevel_High);
	Delay_voidBlockingDelayUs(100);

	GPIO_voidSetPinOutputLevel(
		tftPtr->rstPort, tftPtr->rstPin, GPIO_OutputLevel_Low);
	Delay_voidBlockingDelayUs(100);

	GPIO_voidSetPinOutputLevel(
		tftPtr->rstPort, tftPtr->rstPin, GPIO_OutputLevel_High);
	Delay_voidBlockingDelayMs(120);
}

/*
 * sets brightness of LCD, takes a number from 0 to (2^16 - 1).
 * Brightness control must be initially enabled in 'TFT2_voidInit()' function.
 */
inline void TFT2_voidSetBrightness(TFT2_t* tftPtr, u16 brightness)
{
	TIM_voidSetDutyCycle(
			(tftPtr)->bcTimUnitNumber, (tftPtr)->bcTimChannel, (brightness));
}

/*	gets current level of brightness (stored in CCR)	*/
inline u16 TFT2_u16GetBrightness(TFT2_t* tftPtr)
{
	return TIM_u16GetCCR(
		tftPtr->bcTimUnitNumber, tftPtr->bcTimChannel);
}

/*
 * sends pixel data after boundaries are set.
 * (uses DMA)
 * Note:
 * - 'pixCount' should be equal to the area of the previously set boundaries.
 */
void TFT2_voidSendPixels(
	TFT2_t* const tftPtr, const u16 pixColorArr[], const u16 pixCount)
{
	TFT2_WRITE_CMD(tftPtr, TFT_CMD_MEM_WRITE);

	TFT2_ENTER_DATA_MODE(tftPtr);

	/*
	 * wait for previous operations on DMA channel to be done
	 * (only if channel was currently enabled)
	 */
	TFT2_voidWaitCurrentDataTransfer(tftPtr);

	/*
	 * since:
	 * 	- 8-bit DMA size was selected in the "TFT2_voidInit()" function.
	 * 	- pixel color is of 16-bits.
	 * Hence:
	 * 	- number of data of 8-bits each, to be sent to TFT is 2 * pixCount.
	 */
	DMA_voidSetNumberOfData(
		DMA_UnitNumber_1, tftPtr->dmaCh, (const u32)pixCount << 1);

	DMA_voidSetMemoryAddress(
		DMA_UnitNumber_1, tftPtr->dmaCh, (void*)pixColorArr);

	DMA_voidEnableMemoryIncrement(DMA_UnitNumber_1, tftPtr->dmaCh);

	DMA_voidEnableChannel(DMA_UnitNumber_1, tftPtr->dmaCh);
}

/*
 * fills area of boundaries that are previously set with one color
 * (uses DMA)
 * Notes:
 * - 'pixCount' should be equal to the area of the previously set boundaries.
 * - actual sent color value is: (*pixColorPtr) | ((*pixColorPtr) << 8)
 * this is due to SPI, TFT and DMA constrains. Hence, not all colors may be
 * used in this function.
 * - MEM_WRITE command must be sent before this function call.
 * - Data mode must be entered before this function call.
 */
void TFT2_voidFillDMA(
	TFT2_t* const tftPtr, const u8* pixColorPtr, const u16 pixCount)
{
	if (pixCount == 0)
		return;
	/*
	 * wait for previous operations on DMA channel to be done
	 * (only if channel was currently enabled)
	 */
	TFT2_voidWaitCurrentDataTransfer(tftPtr);

	/*
	 * since:
	 * 	- 8-bit DMA size was selected in the "TFT2_voidInit()" function.
	 * 	- pixel color is of 16-bits.
	 * Hence:
	 * 	- number of data of 8-bits each, to be sent to TFT is 2 * pixCount.
	 */
	DMA_voidSetNumberOfData(
		DMA_UnitNumber_1, tftPtr->dmaCh, (const u32)pixCount << 1);

	DMA_voidSetMemoryAddress(
		DMA_UnitNumber_1, tftPtr->dmaCh, (void*)pixColorPtr);

	DMA_voidDisableMemoryIncrement(DMA_UnitNumber_1, tftPtr->dmaCh);

	DMA_voidEnableChannel(DMA_UnitNumber_1, tftPtr->dmaCh);
}

/*	blocks until end of current data block transfer	*/
inline void TFT2_voidWaitCurrentDataTransfer(TFT2_t* const tftPtr)
{
	if (DMA_b8IsEnabledChannel(DMA_UnitNumber_1, tftPtr->dmaCh))
	{
		while(1)
		{
			if (DMA_u16GetNumberOfData(DMA_UnitNumber_1, tftPtr->dmaCh) == 0)
				break;

			if (
				DMA_b8ReadFlag(
					DMA_UnitNumber_1, tftPtr->dmaCh, DMA_Flag_TransferComplete))
				break;
		}

		DMA_voidClearFlag(
			DMA_UnitNumber_1, tftPtr->dmaCh, DMA_Flag_TransferComplete);
		DMA_voidDisableChannel(DMA_UnitNumber_1, tftPtr->dmaCh);
	}
}

/*
 * inits vertical scroll mode.
 * Note:
 * sum of 'topFixedAreaLen' + 'scrollAreaLen' + 'bottomFixedAreaLen' must be
 * equal to 162. otherwise, mode may not work properly.
 */
void TFT2_voidInitScroll(
	const TFT2_t* tftPtr, u8 topFixedAreaLen, u8 scrollAreaLen,
	u8 bottomFixedAreaLen)
{
	TFT2_WRITE_CMD(tftPtr, 0x33);
	TFT2_WRITE_DATA(tftPtr, 0);
	TFT2_WRITE_DATA(tftPtr, topFixedAreaLen);
	TFT2_WRITE_DATA(tftPtr, 0);
	TFT2_WRITE_DATA(tftPtr, scrollAreaLen);
	TFT2_WRITE_DATA(tftPtr, 0);
	TFT2_WRITE_DATA(tftPtr, bottomFixedAreaLen);
}

/*
 * scrolls the screen.
 * Notes:
 * - 'startingLine' is the index of the line to start the scrolling area with. it
 * is absolute index (as in frame memory).
 * -  'startingLine' maximum value is 160 decimal. other wise non desired image
 * is shown.
 *
 */
inline void TFT2_voidScroll(TFT2_t* const tftPtr, const u8 startingLine)
{
	TFT2_WRITE_CMD(tftPtr, TFT_CMD_SCROLL);
	TFT2_ENTER_DATA_MODE(tftPtr);
	TFT2_SEND_BYTE(tftPtr, 0);
	TFT2_SEND_BYTE(tftPtr, startingLine);
}

/*	sets TFT's corresponding DMA interrupt callback	*/
inline void TFT2_voidSetDMATransferCompleteCallback(
	TFT2_t* const tftPtr, void (* callback)(void))
{
	DMA_voidSetInterruptCallback(
		DMA_UnitNumber_1, tftPtr->dmaCh, callback);
}

/*	enables TFT's corresponding DMA transfer complete interrupt	*/
inline void TFT2_voidEnableDMATransferCompleteInterrupt(TFT2_t* const tftPtr)
{
	DMA_voidEnableInterrupt(
		DMA_UnitNumber_1, tftPtr->dmaCh, DMA_Interrupt_TransferComplete);
}

/*	clears TFT's corresponding DMA transfer complete flag	*/
inline void TFT2_voidClearDMATCFlag(TFT2_t* const tftPtr)
{
	DMA_voidClearFlag(
		DMA_UnitNumber_1, tftPtr->dmaCh, DMA_Flag_TransferComplete);
}

/*	disables TFT's corresponding DMA channel	*/
inline void TFT2_voidDisableDMAChannel(TFT2_t* const tftPtr)
{
	DMA_voidDisableChannel(DMA_UnitNumber_1, tftPtr->dmaCh);
}























