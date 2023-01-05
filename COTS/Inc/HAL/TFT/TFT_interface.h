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

	/*	timer and timer channel connected to brightness control switch	*/
	u8 bcTimUnitNumber;
	TIM_Channel_t bcTimChannel;

	/*	current SPI unit's frame format (stored in object for fast access	*/
	SPI_DataFrameFormat_t spiFrameFormat;
}TFT_t;

/*
 * assigns main parameters to TFT object.
 * executes init sequence.
 * sets the cursor at zero position.
 * SPI and timer peripheral must be previously initialized.
 */
void TFT_voidInit(
	TFT_t* tftPtr, SPI_UnitNumber_t _spiUnit, GPIO_Pin_t _rstPin,
	GPIO_Pin_t _A0Pin);

/*	inits PWM channel to control TFT LED brightness	*/
void TFT_voidInitBrightnessControl(
	TFT_t* tftPtr, u8 bcTimUnitNumber, TIM_Channel_t bcTimChannel, u32 freq,
	u8 map);

#define TFT_WRITE_CMD(tftPtr, cmd)						    \
{                                                           \
	SPI_SET_FRAME_FORMAT_8_BIT((tftPtr)->spiUnit);          \
	GPIO_SET_PIN_LOW((tftPtr)->A0Port, (tftPtr)->A0Pin);    \
	SPI_TRANSMIT((tftPtr)->spiUnit, (u8)(cmd)); 	        \
}

#define TFT_WRITE_DATA(tftPtr, data)					     \
{                                                            \
	SPI_SET_FRAME_FORMAT_8_BIT((tftPtr)->spiUnit); 	         \
	GPIO_SET_PIN_HIGH((tftPtr)->A0Port, (tftPtr)->A0Pin);    \
	SPI_TRANSMIT((tftPtr)->spiUnit, (u8)(data));	         \
}                                                            \

/*
 * resets TFT.
 */
void TFT_voidReset(TFT_t* tftPtr);

/*
 * sets boundaries of the rectangle to work on.
 * (won't use "TFT_voidWriteData()", to save GPIO time)
 */
#define TFT_SET_X_BOUNDARIES(tftPtr, xStart, xEnd)				  \
{																  \
	/*	set x boundaries command	*/                            \
	TFT_WRITE_CMD((tftPtr), 0x2A);                                \
	/*	write data mode	*/                                        \
	SPI_SET_FRAME_FORMAT_16_BIT((tftPtr)->spiUnit);               \
	GPIO_SET_PIN_HIGH((tftPtr)->A0Port, (tftPtr)->A0Pin);         \
	/*	send x boundaries	*/                                    \
	SPI_TRANSMIT((tftPtr)->spiUnit, (xStart));	         		  \
	SPI_TRANSMIT((tftPtr)->spiUnit, (xEnd));	         		  \
}

#define TFT_SET_Y_BOUNDARIES(tftPtr, yStart, yEnd)				  \
{																  \
	/*	set y boundaries command	*/                            \
	TFT_WRITE_CMD((tftPtr), 0x2B);                                \
	/*	write data mode	*/                                        \
	SPI_SET_FRAME_FORMAT_16_BIT((tftPtr)->spiUnit);               \
	GPIO_SET_PIN_HIGH((tftPtr)->A0Port, (tftPtr)->A0Pin);         \
	/*	send y boundaries	*/                                    \
	SPI_TRANSMIT((tftPtr)->spiUnit, (yStart));	         		  \
	SPI_TRANSMIT((tftPtr)->spiUnit, (yEnd));	         		  \
}

#define TFT_SET_BOUNDARIES(tftPtr, point1, point2)                \
{                                                                 \
	/*	set x boundaries	*/		                              \
	TFT_SET_X_BOUNDARIES((tftPtr), (point1).x, (point2).x)	      \
																  \
	/*	set y boundaries	*/		                              \
	TFT_SET_Y_BOUNDARIES((tftPtr), (point1).y, (point2).y)	      \
}

#define TFT_SET_PIXEL(tftPtr, p, c)                                \
{                                                                  \
	TFT_SET_BOUNDARIES((tftPtr), (p), (p));                        \
	TFT_WRITE_CMD((tftPtr), 0x2C);                                 \
	SPI_SET_FRAME_FORMAT_16_BIT((tftPtr)->spiUnit);                \
	GPIO_SET_PIN_HIGH((tftPtr)->A0Port, (tftPtr)->A0Pin);          \
	SPI_TRANSMIT((tftPtr)->spiUnit, (c).code565);                  \
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
void TFT_voidDrawFrame(TFT_t* tftPtr, Frame_t* framePtr);

/*
 * sets brightness of LCD, takes a number from 0 to (2^16 - 1).
 * Brightness control must be initially enabled.
 */
#define TFT_SET_BRIGHTNESS(tftPtr, brightness)	\
	(TIM_voidSetDutyCycle(						\
		(tftPtr)->bcTimUnitNumber, (tftPtr)->bcTimChannel, (brightness)))

/*	gets current level of brightness (stored in CCR)	*/
#define TFT_GET_BRIGHTNESS(tftPtr)	\
	(TIM_u16GetCCR(					\
		(tftPtr)->bcTimUnitNumber, (tftPtr)->bcTimChannel))

/*
 * fills using "color" in the previously set rectangle:
 * {(xStart, yStart), (xEnd, yEnd)}
 */
void TFT_voidFill(TFT_t* tftPtr, Color_t color);

/*
 * inits vertical scroll mode.
 * Note:
 * sum of 'topFixedAreaLen' + 'scrollAreaLen' + 'bottomFixedAreaLen' must be
 * equal to 162. otherwise, mode may not work properly.
 */
void TFT_voidInitScroll(
	TFT_t* tftPtr, u8 topFixedAreaLen, u8 scrollAreaLen,
	u8 bottomFixedAreaLen);

/*
 * scrolls the screen.
 * 'startingLine' is the index of the line to start the scrolling area with. it
 * is absolute index (as in frame memory).
 */
void TFT_voidScroll(TFT_t* tftPtr, const u8 startingLine);

#endif /* _TFT_TFT_INTERFACE_H_ */
