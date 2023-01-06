/*
 * TFT_interface.h
 *
 *  Created on: Jan 6, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_HAL_TFT_V2_TFT_INTERFACE_H_
#define INCLUDE_HAL_TFT_V2_TFT_INTERFACE_H_


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
inline void TFT2_voidSetBrightness(TFT2_t* tftPtr, u16 brightness);

/*	gets current level of brightness (stored in CCR)	*/
inline u16 TFT2_u16GetBrightness(TFT2_t* tftPtr);



#endif /* INCLUDE_HAL_TFT_V2_TFT_INTERFACE_H_ */
