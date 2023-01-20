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

typedef enum{
	Txt_VerticalMirroring_Disabled,
	Txt_VerticalMirroring_Enabled
}Txt_VerticalMirroring_t;

typedef enum{
	Txt_HorizontalMirroring_Disabled,
	Txt_HorizontalMirroring_Enabled
}Txt_HorizontalMirroring_t;

/*
 * a function that copies character image in a given size to a given position
 * and rotation in array of pixels.
 *
 * Arguments of this function:
 * 	- 'ch': the character to be drawn.
 * 	- 'pixArr': a 2D array of pixels. (the image to be drawn on)
 * 	- 'charColor': color of the character when drawn.
 * 	- 'bgColor': color of the background in the rectangle containing the drawn
 * 	  character, that is defined by: {('tlX', 'tlY'), 'size'}.
 * 	- 'size': size of the character when drawn.
 * 	- 'orientation': orientation in degrees of the character when drawn.
 * 	- 'tlX', 'tlY': x, y position of the top left corner of the rectangle to
 * 	  draw the character on.
 *
 * Notice that the pixArr must be large enough to have the char drawn on it.
 * Otherwise, memory fault may occur!
 *
 * Size example:
 * 	- if 'size' equals 1 ==> char will take a rectangle of 5x8   pixels.
 * 	- if 'size' equals 2 ==> char will take a rectangle of 10x16 pixels.
 *  And  so on...
 */
void Txt_voidCpyCharToStaticPixArrNormalOrientation(
	u8 ch, u16 charColor, u16 bgColor, u8 size,
	Txt_HorizontalMirroring_t hMirror, Txt_VerticalMirroring_t vMirror,
	u8 tlX, u8 tlY, u8 pixArrWidth, u16 pixArr[][pixArrWidth]);

void Txt_voidCpyCharToStaticPixArrRightOrientation(
	u8 ch, u16 charColor, u16 bgColor, u8 size,
	Txt_HorizontalMirroring_t hMirror, Txt_VerticalMirroring_t vMirror,
	u8 tlX, u8 tlY, u8 pixArrWidth, u16 pixArr[][pixArrWidth]);

void Txt_voidCpyCharToStaticPixArrLeftOrientation(
	u8 ch, u16 charColor, u16 bgColor, u8 size,
	Txt_HorizontalMirroring_t hMirror, Txt_VerticalMirroring_t vMirror,
	u8 tlX, u8 tlY, u8 pixArrWidth, u16 pixArr[][pixArrWidth]);

void Txt_voidCpyCharToStaticPixArr180DegreesOrientation(
	u8 ch, u16 charColor, u16 bgColor, u8 size,
	Txt_HorizontalMirroring_t hMirror, Txt_VerticalMirroring_t vMirror,
	u8 tlX, u8 tlY, u8 pixArrWidth, u16 pixArr[][pixArrWidth]);

/*
 * A function that draws a string on a pixel array. Uses previous function
 */
void Txt_voidCpyStrToStaticPixArrNormalOrientation(
	const u8* str, u16 charColor, u16 bgColor, u8 size,
	Txt_HorizontalMirroring_t hMirror, Txt_VerticalMirroring_t vMirror,
	u8 tlXInitial, u8 tlYInitial,
	u8 pixArrHieght, u8 pixArrWidth, u16 pixArr[][pixArrWidth]);

void Txt_voidCpyStrToStaticPixArrRightOrientation(
	const u8* str, u16 charColor, u16 bgColor, u8 size,
	Txt_HorizontalMirroring_t hMirror, Txt_VerticalMirroring_t vMirror,
	u8 tlXInitial, u8 tlYInitial,
	u8 pixArrHieght, u8 pixArrWidth, u16 pixArr[][pixArrWidth]);


#endif /* INCLUDE_LIB_IMG_TXT_TXT_INTERFACE_H_ */
