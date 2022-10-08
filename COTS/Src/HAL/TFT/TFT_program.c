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
#include "Img_interface.h"

/*	MCAL	*/
#include "GPIO_interface.h"
#include "SPI_interface.h"

/*	SELF	*/
#include "TFT_interface.h"
#include "TFT_config.h"



/*
 * assigns main parameters to TFT object.
 * executes init sequence.
 * sets the start position at (0, 0).
 * end position is user's responsibility to set next.
 * initializes GPIO pins reset and A0.
 * SPI and timer peripheral must be previously initialized.
 */
void TFT_voidInit(TFT_t* tftPtr, SPI_UnitNumber_t _spiUnit, GPIO_Pin_t _rstPin, GPIO_Pin_t _A0Pin)
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
	TFT_voidWriteCmd(tftPtr, 0x11);
	TFT_voidWriteCmd(tftPtr, 0x11);

	/*	TFT self init time	*/
	Delay_voidBlockingDelayMs(150);

	/*	set color mode (always RGB565, perfect for STM32)	*/
	TFT_voidWriteCmd(tftPtr, 0x3A);
	TFT_voidWriteData(tftPtr, 0x05);

	/*	start display	*/
	TFT_voidWriteCmd(tftPtr, 0x29);

	/*	set start position at (0, 0)	*/
	TFT_voidSetBoundaries(tftPtr, (Point_t){0, 0}, (Point_t){128, 160});
}

void TFT_voidWriteCmd(TFT_t* tftPtr, u16 cmd)
{
	GPIO_voidSetPinOutputLevel(tftPtr->A0Port, tftPtr->A0Pin, GPIO_OutputLevel_Low);
	SPI_voidTransmitData(tftPtr->spiUnit, cmd);
}

void TFT_voidWriteData(TFT_t* tftPtr, u16 data)
{
	GPIO_voidSetPinOutputLevel(tftPtr->A0Port, tftPtr->A0Pin, GPIO_OutputLevel_High);
	SPI_voidTransmitData(tftPtr->spiUnit, data);
}

/*
 * resets TFT.
 */
void TFT_voidReset(TFT_t* tftPtr)
{
	GPIO_voidSetPinOutputLevel(tftPtr->rstPort, tftPtr->rstPin, GPIO_OutputLevel_High);
	Delay_voidBlockingDelayUs(100);
	//STK_voidBusyWait(100);

	GPIO_voidSetPinOutputLevel(tftPtr->rstPort, tftPtr->rstPin, GPIO_OutputLevel_Low);
	Delay_voidBlockingDelayUs(1);
	//STK_voidBusyWait(1);

	GPIO_voidSetPinOutputLevel(tftPtr->rstPort, tftPtr->rstPin, GPIO_OutputLevel_High);
	Delay_voidBlockingDelayUs(100);
	//STK_voidBusyWait(100);

	GPIO_voidSetPinOutputLevel(tftPtr->rstPort, tftPtr->rstPin, GPIO_OutputLevel_Low);
	Delay_voidBlockingDelayUs(100);
	//STK_voidBusyWait(100);

	GPIO_voidSetPinOutputLevel(tftPtr->rstPort, tftPtr->rstPin, GPIO_OutputLevel_High);
	Delay_voidBlockingDelayMs(120);
	//STK_voidBusyWait(120000);
}


void TFT_voidSetBoundaries(TFT_t* tftPtr, Point_t point1, Point_t point2)
{
	/*	won't use "TFT_voidWriteData()", to save GPIO time.	*/

	/*	set x boundaries command	*/
	TFT_voidWriteCmd(tftPtr, 0x2A);
	/*	write data mode	*/
	GPIO_voidSetPinOutputLevel(tftPtr->A0Port, tftPtr->A0Pin, GPIO_OutputLevel_High);
	/*	send x boundaries	*/
	SPI_voidTransmitData(tftPtr->spiUnit, point1.x >> 8);
	SPI_voidTransmitData(tftPtr->spiUnit, point1.x & 0xFF);
	SPI_voidTransmitData(tftPtr->spiUnit, point2.x >> 8);
	SPI_voidTransmitData(tftPtr->spiUnit, point2.x & 0xFF);

	/*	set y boundaries command	*/
	TFT_voidWriteCmd(tftPtr, 0x2B);
	/*	write data mode	*/
	GPIO_voidSetPinOutputLevel(tftPtr->A0Port, tftPtr->A0Pin, GPIO_OutputLevel_High);
	/*	send y boundaries	*/
	SPI_voidTransmitData(tftPtr->spiUnit, point1.y >> 8);
	SPI_voidTransmitData(tftPtr->spiUnit, point1.y & 0xFF);
	SPI_voidTransmitData(tftPtr->spiUnit, point2.y >> 8);
	SPI_voidTransmitData(tftPtr->spiUnit, point2.y & 0xFF);
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
	#ifdef BG_THEN_RECTANGLES
	/*	draw background color	*/
	TFT_voidSetBoundaries(tftPtr, (Point_t){0, 0}, (Point_t){128, 160});
	TFT_voidWriteCmd(tftPtr, 0x2C);
	GPIO_voidSetPinOutputLevel(tftPtr->A0Port, tftPtr->A0Pin, GPIO_OutputLevel_High);
	for (u16 i = 0; i < 20480; i++)
	{
		SPI_voidTransmitData(tftPtr->spiUnit, framePtr->backgroundColor.code565 >> 8);
		SPI_voidTransmitData(tftPtr->spiUnit, framePtr->backgroundColor.code565 & 0xFF);
	}

	/*	draw rectangles	*/
	for (u16 iRect = 0; iRect < framePtr->rectCount; iRect++)
	{
		Rectangle_t* rectPtr = framePtr->rectPtrArr[iRect];

		/*	set TFT boundaries	*/
		TFT_voidSetBoundaries(tftPtr, rectPtr->pointStart, rectPtr->pointEnd);

		/*	write color	*/
		TFT_voidWriteCmd(tftPtr, 0x2C);
		GPIO_voidSetPinOutputLevel(tftPtr->A0Port, tftPtr->A0Pin, GPIO_OutputLevel_High);
		u16 n = (rectPtr->pointEnd.y - rectPtr->pointStart.y + 1) * (rectPtr->pointEnd.x - rectPtr->pointStart.x + 1);
		for (u16 i = 0; i < n; i++)
		{
			SPI_voidTransmitData(tftPtr->spiUnit, rectPtr->color.code565 >> 8);
			SPI_voidTransmitData(tftPtr->spiUnit, rectPtr->color.code565 & 0xFF);
		}
	}

	#else
	/*	draw pixel by pixel	*/
	TFT_voidSetBoundaries(tftPtr, (Point_t){0, 0}, (Point_t){128, 160});
	TFT_voidWriteCmd(tftPtr, 0x2C);
	GPIO_voidSetPinOutputLevel(tftPtr->A0Port, tftPtr->A0Pin, GPIO_OutputLevel_High);
	Color_t color;
	for (u16 y = 0; y < 160; y++)
	{
		for (u16 x = 0; x < 128; x++)
		{
			color = IMG_ColorTypeGetPixel(framePtr, (Point_t){x, y});
			SPI_voidTransmitData(tftPtr->spiUnit, color.code565 >> 8);
			SPI_voidTransmitData(tftPtr->spiUnit, color.code565 & 0xFF);
		}
	}
	#endif
}
