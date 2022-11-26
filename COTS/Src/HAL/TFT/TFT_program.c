/*
 * TFT_program.c
 *
 *  Created on: Oct 3, 2022 3:18:14 PM
 *      Author: Ali Emad Ali
 */


/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Delay_interface.h"
#include "Img_config.h"
#include "Img_interface.h"

/*	MCAL	*/
#include "GPIO_interface.h"
#include "TIM_interface.h"
#include "SPI_interface.h"

/*	SELF	*/
#include "TFT_config.h"
#include "TFT_interface.h"




/*
 * assigns main parameters to TFT object.
 * executes init sequence.
 * sets the start position at (0, 0).
 * end position is user's responsibility to set next.
 * initializes GPIO pins reset and A0.
 * SPI and timer peripheral must be previously initialized.
 */
void TFT_voidInit(
	TFT_t* tftPtr, SPI_UnitNumber_t _spiUnit, GPIO_Pin_t _rstPin,
	GPIO_Pin_t _A0Pin)
{
	/*	store pins data and init pins	*/
	tftPtr->rstPin 	= _rstPin % 16;
	tftPtr->rstPort = _rstPin / 16;

	tftPtr->A0Pin 	= _A0Pin % 16;
	tftPtr->A0Port 	= _A0Pin / 16;

	GPIO_voidSetPinGpoPushPull(tftPtr->rstPort, tftPtr->rstPin);
	GPIO_voidSetPinGpoPushPull(tftPtr->A0Port, tftPtr->A0Pin);

	/*	store SPI unit number to be used	*/
	tftPtr->spiUnit = _spiUnit;

	/*	executing reset sequence	*/
	TFT_voidReset(tftPtr);

	/*	sleep out cmd	*/
	TFT_WRITE_CMD(tftPtr, 0x11);
	TFT_WRITE_CMD(tftPtr, 0x11);

	/*	TFT self init time	*/
	Delay_voidBlockingDelayMs(150);

	/*	set color mode (always RGB565, perfect for STM32)	*/
	TFT_WRITE_CMD(tftPtr, 0x3A);
	TFT_WRITE_DATA(tftPtr, 0x05);

	/*	start display	*/
	TFT_WRITE_CMD(tftPtr, 0x29);

	/*	set start position at (0, 0)	*/
	TFT_SET_BOUNDARIES(tftPtr, ((Point_t){0, 0}), ((Point_t){128, 160}));
}

/*	inits PWM channel to control TFT LED brightness	*/
void TFT_voidInitBrightnessControl(
	TFT_t* tftPtr, u8 bcTimUnitNumber, TIM_Channel_t bcTimChannel, u32 freq,
	u8 map)
{
	/*
	 * using PWM2 as control is by convention going to be through a PNP
	 * transistor.	*/
	TIM_u64InitPWM(
		bcTimUnitNumber, bcTimChannel, TIM_OutputCompareMode_PWM2, freq);

	TIM_voidInitOutputPin(bcTimUnitNumber, bcTimChannel, map);

	TIM_voidEnableCounter(bcTimUnitNumber);

	tftPtr->bcTimUnitNumber = bcTimUnitNumber;
	tftPtr->bcTimChannel = bcTimChannel;
}

/*
 * resets TFT.
 */
void TFT_voidReset(TFT_t* tftPtr)
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
 * draws "frame" in the previously set rectangle:
 * {(xStart, yStart), (xEnd, yEnd)}.
 *
 * frame dimensions must be equal those of the previously mentioned
 * rectangle, to avoid fatal errors.
 *
 * if one of them is smaller than the other, function will end with
 * the smaller end.
 */
void TFT_voidDrawFrame(TFT_t* tftPtr, Frame_t* framePtr)
{
	/*	draw background color	*/
	TFT_SET_BOUNDARIES(tftPtr, ((Point_t){0, 0}), ((Point_t){128, 160}));
	TFT_WRITE_CMD(tftPtr, 0x2C);
	SPI_SET_FRAME_FORMAT_16_BIT(tftPtr->spiUnit);
	GPIO_SET_PIN_HIGH(tftPtr->A0Port, tftPtr->A0Pin);

	for (u16 i = 0; i < 20480; i++)
	{
		SPI_TRANSMIT(tftPtr->spiUnit, framePtr->backgroundColor.code565);
	}

	/*	draw rectangles	*/
	for (u16 iRect = 0; iRect < framePtr->rectCount; iRect++)
	{
		/*	set TFT boundaries	*/
		TFT_SET_BOUNDARIES(
			tftPtr,
			framePtr->rectArr[iRect].pointStart,
			framePtr->rectArr[iRect].pointEnd);

		/*	write color	*/
		TFT_WRITE_CMD(tftPtr, 0x2C);
		SPI_SET_FRAME_FORMAT_16_BIT(tftPtr->spiUnit);
		GPIO_SET_PIN_HIGH(tftPtr->A0Port, tftPtr->A0Pin);
		u16 n =
			(framePtr->rectArr[iRect].pointEnd.y -
			framePtr->rectArr[iRect].pointStart.y + 1) *
			(framePtr->rectArr[iRect].pointEnd.x -
			framePtr->rectArr[iRect].pointStart.x + 1);
		for (u16 i = 0; i < n; i++)
		{
			SPI_TRANSMIT(
				tftPtr->spiUnit, framePtr->rectArr[iRect].color.code565);
		}
	}
}











