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

/*	SELF	*/
#include "TFT_cmd.h"
#include "TFT_interface_V2.h"


#define TFT2_WRITE_CMD(tftPtr, cmd)						    \
{                                                           \
	GPIO_SET_PIN_LOW((tftPtr)->A0Port, (tftPtr)->A0Pin);    \
	SPI_TRANSMIT((tftPtr)->spiUnit, (u8)(cmd)); 	        \
}

#define TFT2_WRITE_DATA(tftPtr, data)					     \
{                                                            \
	GPIO_SET_PIN_HIGH((tftPtr)->A0Port, (tftPtr)->A0Pin);    \
	SPI_TRANSMIT((tftPtr)->spiUnit, (u8)(data));	         \
}                                                            \

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
	}
	else
	{
		tftPtr->dmaCh = DMA_ChannelNumber_5;
		peripheralNumber = RCC_PERIPHERAL_SPI2;
		bus = RCC_Bus_APB1;
	}

	/*	enable SPI clock (if not enabled)	*/
	if (!RCC_b8IsPeripheralEnabled(bus, peripheralNumber))
		RCC_voidEnablePeripheralClk(bus, peripheralNumber);

	/*	init SPI	*/
	SPI_voidInit(
		spiUnit, SPI_Directional_Mode_Uni, SPI_DataFrameFormat_8bit,
		SPI_FrameDirection_MSB_First, SPI_Prescaler_2, SPI_Mode_Master,
		SPI_ClockPolarity_0Idle, SPI_ClockPhase_CaptureFirst);

	SPI_voidInitPins(spiUnit, spiAFIOMap, false, false, true);

	SPI_ENABLE_PERIPHERAL(spiUnit);

	/*	enable DMA1 clock (if not enabled)	*/
	if (!RCC_b8IsPeripheralEnabled(RCC_Bus_AHB, RCC_PERIPHERAL_DMA1))
		RCC_voidEnablePeripheralClk(RCC_Bus_AHB, RCC_PERIPHERAL_DMA1);

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
	bus = TIM_EnumGetBus(bcTimUnitNumber);
	peripheralNumber = TIM_u8GetPeripheralIndex(bcTimUnitNumber);
	if (!RCC_b8IsPeripheralEnabled(bus, peripheralNumber))
		RCC_voidEnablePeripheralClk(bus, peripheralNumber);

	/*	init TIMx	*/
	/*
	 * using PWM2 as control is by convention going to be through a PNP
	 * transistor.
	 */
	TIM_u64InitPWM(
		bcTimUnitNumber, bcTimChannel, TIM_OutputCompareMode_PWM2, 300);

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





















