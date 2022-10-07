/*
 * Img_interface.h
 *
 *  Created on: Oct 7, 2022 10:57:32 AM
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_LIB_IMG_INTERFACE_H_
#define INCLUDE_LIB_IMG_INTERFACE_H_

typedef struct{
	u16 blue:	5;
	u16 green:	6;
	u16 red:	5;
}ColorMap_t;

typedef union{
	ColorMap_t map;
	u16 code565;
}Color_t;

typedef struct{
	Color_t* colorArr;
	u16 width;
	u16 height;
}Frame_t;

/*
 * allocates memory for new created frame.
 * re-allocation of previously allocated frame is user's responsibility.
 */
void Img_voidAllocFrame(Frame_t* framePtr, u16 _width, u16 _height);

/*
 * frees memory of a previously allocated frame.
 * freeing a non-allocated frame is user's responsibility.
 */
void Img_voidFreeFrame(Frame_t* framePtr);

/*
 * sets the color of a pixel in the frame.
 */
void Img_voidSetPixel(Frame_t* framePtr, u16 x, u16 y, Color_t color);

/*
 * gets the color of a pixel in the frame.
 */
Color_t Img_ColorTypeGetPixel(Frame_t* framePtr, u16 x, u16 y);

/*
 * sets the color of a rectangle in the frame.
 */
void Img_voidSetRectangle(Frame_t* framePtr, u16 x1, u16 y1, u16 x2, u16 y2, Color_t color);

#endif /* INCLUDE_LIB_IMG_INTERFACE_H_ */
