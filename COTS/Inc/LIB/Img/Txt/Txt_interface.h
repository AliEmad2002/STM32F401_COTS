/*
 * Txt_interface.h
 *
 *  Created on: Jan 14, 2023
 *      Author: Ali Emad Ali
 *
 * Why buffering into a pixel array then sending to the TFT?
 * 	- DMA which makes the image data transfer much faster.
 * 	- Less bound setting operations, which are monsters eating cycles.
 */

#ifndef INCLUDE_LIB_IMG_TXT_TXT_INTERFACE_H_
#define INCLUDE_LIB_IMG_TXT_TXT_INTERFACE_H_

/*
 * Prints char on a pixel array.
 * size of pix array must be enough. It's user's responsibility.
 */
void Txt_voidPrintCharOnPixArrNormalOrientation(
	char ch, u16 charColor, u16 bgColor, u8 tlX, u8 tlY, u16* pixArr,
	u8 pixArrWidth);

void Txt_voidPrintCharOnPixArrRigthOrientation(
	char ch, u16 charColor, u16 bgColor, u8 tlX, u8 tlY, u16* pixArr,
	u8 pixArrWidth);

/*
 * Prints string on a pixel array.
 * size of pix array must be enough. It's user's responsibility.
 */
void Txt_voidPrintStrOnPixArrNormalOrientation(
	char* str, u16 charColor, u16 bgColor, u8 tlX, u8 tlY, u16* pixArr,
	u8 pixArrWidth, u8 pixArrHeight);

void Txt_voidPrintStrOnPixArrRightOrientation(
	char* str, u16 charColor, u16 bgColor, u8 tlX, u8 tlY, u16* pixArr,
	u8 pixArrWidth, u8 pixArrHeight);

#endif /* INCLUDE_LIB_IMG_TXT_TXT_INTERFACE_H_ */
