/*
 * Img_program.c
 *
 *  Created on: Oct 7, 2022 10:57:59 AM
 *      Author: Ali Emad Ali
 */


/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"

/*	SELF	*/
#include "Img_interface.h"

/*
 * allocates memory for new created frame.
 * re-allocation of previously allocated frame is user's responsibility.
 */
void Img_voidAllocFrame(Frame_t* framePtr, u16 _width, u16 _height)
{
	framePtr->colorArr = (Color_t*)malloc(_width * _height * sizeof(Color_t));

	framePtr->width = _width;
	framePtr->height = _height;
}

/*
 * frees memory of a previously allocated frame.
 * freeing a non-allocated frame is user's responsibility.
 */
void Img_voidFreeFrame(Frame_t* framePtr)
{
	free(framePtr->colorArr);
}

/*
 * sets the color of a pixel in the frame.
 */
void Img_voidSetPixel(Frame_t* framePtr, u16 x, u16 y, Color_t color)
{
	u16 index = y * framePtr->width + x;
	framePtr->colorArr[index] = color;
}

/*
 * gets the color of a pixel in the frame.
 */
Color_t Img_ColorTypeGetPixel(Frame_t* framePtr, u16 x, u16 y)
{
	u16 index = y * framePtr->width + x;
	return framePtr->colorArr[index];
}

/*
 * sets the color of a rectangle in the frame.
 */
void Img_voidSetRectangle(Frame_t* framePtr, u16 x1, u16 y1, u16 x2, u16 y2, Color_t color)
{
	u16 indexStart	= y1 * framePtr->width + x1;
	u16 indexEnd	= y2 * framePtr->width + x2 + 1;

	for (u16 i = indexStart; i < indexEnd; i++)
		framePtr->colorArr[i] = color;
}

