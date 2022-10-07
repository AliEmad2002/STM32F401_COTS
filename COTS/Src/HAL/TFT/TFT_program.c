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
	STK_voidBusyWait(15000);

	/*	set color mode (always RGB565, perfect for STM32)	*/
	TFT_voidWriteCmd(tftPtr, 0x3A);
	TFT_voidWriteData(tftPtr, 0x05);

	/*	start display	*/
	TFT_voidWriteCmd(tftPtr, 0x29);

	/*	set start position at (0, 0)	*/
	TFT_voidSetBoundaries(tftPtr, 0, 0, 128, 160);
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

void TFT_voidSetBoundaries(TFT_t* tftPtr, u16 xStart, u16 yStart, u16 xEnd, u16 yEnd)
{
	/*	won't use "TFT_voidWriteData()", to save GPIO time.	*/

	/*	set x boundaries command	*/
	TFT_voidWriteCmd(tftPtr, 0x2A);
	/*	write data mode	*/
	GPIO_voidSetPinOutputLevel(tftPtr->A0Port, tftPtr->A0Pin, GPIO_OutputLevel_High);
	/*	send x boundaries	*/
	SPI_voidTransmitData(tftPtr->spiUnit, xStart >> 8);
	SPI_voidTransmitData(tftPtr->spiUnit, xStart & 0xFF);
	SPI_voidTransmitData(tftPtr->spiUnit, xEnd >> 8);
	SPI_voidTransmitData(tftPtr->spiUnit, xEnd & 0xFF);

	/*	set y boundaries command	*/
	TFT_voidWriteCmd(tftPtr, 0x2B);
	/*	write data mode	*/
	GPIO_voidSetPinOutputLevel(tftPtr->A0Port, tftPtr->A0Pin, GPIO_OutputLevel_High);
	/*	send y boundaries	*/
	SPI_voidTransmitData(tftPtr->spiUnit, yStart >> 8);
	SPI_voidTransmitData(tftPtr->spiUnit, yStart & 0xFF);
	SPI_voidTransmitData(tftPtr->spiUnit, yEnd >> 8);
	SPI_voidTransmitData(tftPtr->spiUnit, yEnd & 0xFF);

	/*	store dimensions	*/
	tftPtr->xStart = xStart;
	tftPtr->xEnd = xEnd;
	tftPtr->yStart = yStart;
	tftPtr->yEnd = 	yEnd;
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
	u16 nTft = (tftPtr->xEnd - tftPtr->xStart) * (tftPtr->yEnd - tftPtr->yStart);

	u16 nFrame = framePtr->height * framePtr->width;

	u16 n = (nTft > nFrame) ? nTft : nFrame;

	TFT_voidWriteCmd(tftPtr, 0x2C);
	/*	write data mode	*/
	GPIO_voidSetPinOutputLevel(tftPtr->A0Port, tftPtr->A0Pin, GPIO_OutputLevel_High);
	for (u16 i=0; i<n; i++)
	{
		SPI_voidTransmitData(tftPtr->spiUnit, framePtr->colorArr[i].code565 >> 8);
		SPI_voidTransmitData(tftPtr->spiUnit, framePtr->colorArr[i].code565 & 0xFF);
	}
}

/*
 * fills using "color" in the previously set rectangle:
 * {(xStart, yStart), (xEnd, yEnd)}
 */
void TFT_voidFill(TFT_t* tftPtr, Color_t color)
{
	u16 nPixels = (tftPtr->xEnd - tftPtr->xStart) * (tftPtr->yEnd - tftPtr->yStart);

	TFT_voidWriteCmd(tftPtr, 0x2C);

	/*	write data mode	*/
	GPIO_voidSetPinOutputLevel(tftPtr->A0Port, tftPtr->A0Pin, GPIO_OutputLevel_High);

	while(nPixels--)
	{
		SPI_voidTransmitData(tftPtr->spiUnit, color.code565 >> 8);
		SPI_voidTransmitData(tftPtr->spiUnit, color.code565 & 0xFF);
	}
}
