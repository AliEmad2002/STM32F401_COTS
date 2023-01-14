/*
 * Txt_interface.h
 *
 *  Created on: Jan 14, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_LIB_IMG_TXT_TXT_INTERFACE_H_
#define INCLUDE_LIB_IMG_TXT_TXT_INTERFACE_H_

/*
 * An enum representing state of orientation in degrees. (counter clock-wise)
 */
typedef enum{
	Txt_Orientation_0,
	Txt_Orientation_90,
	Txt_Orientation_180,
	Txt_Orientation_270
}Txt_Orientation_t;

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
void Txt_voidCpyCharToImg(
	u8 ch, u16** pixArr, u16 charColor, u16 bgColor, u8 size,
	Txt_Orientation_t orientation, u8 tlX, u8 tlY);


#endif /* INCLUDE_LIB_IMG_TXT_TXT_INTERFACE_H_ */
