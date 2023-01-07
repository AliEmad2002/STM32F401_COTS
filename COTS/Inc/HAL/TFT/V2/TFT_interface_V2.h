/*
 * TFT_interface.h
 *
 *  Created on: Jan 6, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_HAL_TFT_V2_TFT_INTERFACE_H_
#define INCLUDE_HAL_TFT_V2_TFT_INTERFACE_H_

#include "TFT_cmd.h"

typedef struct{
	/*	number of SPI unit used to communicate the TFT.	*/
	SPI_UnitNumber_t spiUnit;

	/*	DMA1 channel associated with SPI<spiUnit>_Tx	*/
	DMA_ChannelNumber_t dmaCh;

	/*	reset pin of the TFT module.	*/
	u8 rstPin;
	GPIO_PortName_t rstPort;

	/*	A0 pin of the TFT module.	*/
	u8 A0Pin;
	GPIO_PortName_t A0Port;

	/*
	 * timer and timer channel connected to brightness control switch (if any).
	 */
	u8 bcTimUnitNumber;
	TIM_Channel_t bcTimChannel;
}TFT2_t;

#define TFT2_ENTER_CMD_MODE(tftPtr)							    \
{															    \
	while(SPI_GET_FLAG((tftPtr)->spiUnit, SPI_Flag_Busy)); 		\
	__asm__ volatile("NOP");									\
	GPIO_SET_PIN_LOW((tftPtr)->A0Port, (tftPtr)->A0Pin);        \
}

#define TFT2_ENTER_DATA_MODE(tftPtr)						    \
{															    \
	while(SPI_GET_FLAG((tftPtr)->spiUnit, SPI_Flag_Busy));	 	\
	__asm__ volatile("NOP");									\
	GPIO_SET_PIN_HIGH((tftPtr)->A0Port, (tftPtr)->A0Pin);       \
}

#define TFT2_SEND_BYTE(tftPtr, byte)                        \
{                                                           \
	SPI_TRANSMIT((tftPtr)->spiUnit, byte); 	 		        \
}

#define TFT2_WRITE_CMD(tftPtr, cmd)						    \
{                                                           \
	TFT2_ENTER_CMD_MODE((tftPtr));							\
	TFT2_SEND_BYTE((tftPtr), (cmd)); 				        \
}

#define TFT2_WRITE_DATA(tftPtr, data)					     \
{                                                            \
	TFT2_ENTER_DATA_MODE((tftPtr));						  	 \
	TFT2_SEND_BYTE((tftPtr), (data)); 				         \
}

/*
 * Inits TFT object.
 * This function:
 * - Assigns object's elements with proper values, based on arguments.
 * - Inits GPIO, AFIO, SPI, TIM and DMA peripherals associated, all at maximum
 * speeds. Removing much outer overhead.
 * - Executes TFT screen's reset sequence.
 */
void TFT2_voidInit(
	TFT2_t* tftPtr, SPI_UnitNumber_t spiUnit, u8 spiAFIOMap,
	GPIO_Pin_t rstPin, GPIO_Pin_t A0Pin, u8 bcTimUnitNumber,
	TIM_Channel_t bcTimChannel, u8 bcTimAFIOMap);

/*
 * resets TFT (executes reset sequence).
 */
void TFT2_voidReset(TFT2_t* tftPtr);

/*
 * sets brightness of LCD, takes a number from 0 to (2^16 - 1).
 * Brightness control must be initially enabled in 'TFT2_voidInit()' function.
 */
void TFT2_voidSetBrightness(TFT2_t* tftPtr, u16 brightness);

/*	gets current level of brightness (stored in CCR)	*/
u16 TFT2_u16GetBrightness(TFT2_t* tftPtr);

/*	sets boundaries of the rectangle to work on.	*/
#define TFT2_SET_X_BOUNDARIES(tftPtr, xStart, xEnd)				  \
{																  \
	/*	set x boundaries command	*/                            \
	TFT2_WRITE_CMD((tftPtr), TFT_CMD_SET_X_BOUNDARIES);           \
	/*	write data mode	*/                                        \
	TFT2_ENTER_DATA_MODE((tftPtr));						  	 	  \
	/*	send x boundaries	*/                                    \
	TFT2_SEND_BYTE((tftPtr), 0);								  \
	TFT2_SEND_BYTE((tftPtr), (xStart));							  \
	TFT2_SEND_BYTE((tftPtr), 0);								  \
	TFT2_SEND_BYTE((tftPtr), (xEnd));							  \
}

#define TFT2_SET_Y_BOUNDARIES(tftPtr, yStart, yEnd)				  \
{																  \
	/*	set y boundaries command	*/                            \
	TFT2_WRITE_CMD((tftPtr), TFT_CMD_SET_Y_BOUNDARIES);           \
	/*	write data mode	*/                                        \
	TFT2_ENTER_DATA_MODE((tftPtr));						  	 	  \
	/*	send y boundaries	*/                                    \
	TFT2_SEND_BYTE((tftPtr), 0);								  \
	TFT2_SEND_BYTE((tftPtr), (yStart));							  \
	TFT2_SEND_BYTE((tftPtr), 0);								  \
	TFT2_SEND_BYTE((tftPtr), (yEnd));							  \
}

/*
 * sends pixel data after boundaries are set.
 * (uses DMA)
 * Note:
 * - 'pixCount' should be equal to the area of the previously set boundaries.
 */
void TFT2_voidSendPixels(
	TFT2_t* const tftPtr, const u16 pixColorArr[], const u16 pixCount);

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
	TFT2_t* const tftPtr, const u8* pixColorPtr, const u16 pixCount);

/*	blocks until end of current data block transfer	*/
void TFT2_voidWaitCurrentDataTransfer(TFT2_t* const tftPtr);

/*
 * scrolls the screen.
 * Notes:
 * - 'startingLine' is the index of the line to start the scrolling area with. it
 * is absolute index (as in frame memory).
 * -  'startingLine' maximum value is 160 decimal. other wise non desired image
 * is shown.
 *
 */
void TFT2_voidScroll(TFT2_t* const tftPtr, const u8 startingLine);



#endif /* INCLUDE_HAL_TFT_V2_TFT_INTERFACE_H_ */
