/*
 * TFT_interface.h
 *
 *  Created on: Oct 3, 2022 3:17:31 PM
 *      Author: Ali Emad Ali
 */

#ifndef _TFT_TFT_INTERFACE_H_
#define _TFT_TFT_INTERFACE_H_



typedef struct{
	/*
	 * number of SPI peripheral used to communicate the TFT,
	 * must be previously initialized.
	 */
	SPI_UnitNumber_t spiUnit;

	/*
	 * number of SPI peripheral used to communicate the TFT,
	 * must be previously initialized.
	 */
	//TIM_UnitNumber_t timUnit;

	/*	reset pin of the TFT module	*/
	u8 rstPin;
	GPIO_PortName_t rstPort;

	/*	A0 pin of the TFT module	*/
	u8 A0Pin;
	GPIO_PortName_t A0Port;

	/*	start position	*/
	u16 xStart;
	u16 yStart;

	/*	end position	*/
	u16 xEnd;
	u16 yEnd;
}TFT_t;

/*
 * assigns main parameters to TFT object.
 * executes init sequence.
 * sets the cursor at zero position.
 * SPI and timer peripheral must be previously initialized.
 */
void TFT_voidInit(TFT_t* tftPtr, SPI_UnitNumber_t _spiUnit, GPIO_Pin_t _rstPin, GPIO_Pin_t _A0Pin);

void TFT_voidWriteCmd(TFT_t* tftPtr, u16 cmd);

void TFT_voidWriteData(TFT_t* tftPtr, u16 data);

/*
 * resets TFT.
 */
void TFT_voidReset(TFT_t* tftPtr);

void TFT_voidSetBoundaries(TFT_t* tftPtr, u16 xStart, u16 yStart, u16 xEnd, u16 yEnd);

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
void TFT_voidDrawFrame(TFT_t* tftPtr, Frame_t* framePtr);

/*
 * fills using "color" in the previously set rectangle:
 * {(xStart, yStart), (xEnd, yEnd)}
 */
void TFT_voidFill(TFT_t* tftPtr, Color_t color);




#endif /* _TFT_TFT_INTERFACE_H_ */
