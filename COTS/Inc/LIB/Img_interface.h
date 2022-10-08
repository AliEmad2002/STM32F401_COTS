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
	u16 x;
	u16 y;
}Point_t;

typedef struct{
	Point_t pointStart;
	Point_t pointEnd;
	Color_t color;
}Rectangle_t;

/*	RIP. one way to represent a frame, but eats ram like a goat.	*/
/*
typedef struct{
	Color_t* colorArr;
	u16 width;
	u16 height;
}Frame_t;
*/

typedef struct{
	Rectangle_t** rectPtrArr;
	u16 rectCount;
	u16 maxNumberOfRectangles;
	Color_t backgroundColor;
}Frame_t;

/*
 * inits frame object.
 */
void IMG_voidinitFrame(Frame_t* framePtr, Color_t _backgroundColor, u16 initialNumberOfRectangles);

/*
 * frees memory of a previously allocated frame.
 */
void IMG_voidFreeFrame(Frame_t* framePtr);

/*
 * sets the color of a pixel in the frame.
 */
void IMG_voidSetPixel(Frame_t* framePtr, Point_t point, Color_t color);

/*
 * returns the index of the rectangle containing the point.
 * if not in a rectangle, returns -1
 */
s16 IMG_u16FindPoint(Frame_t* framePtr, Point_t point);

/*
 * gets the color of a pixel in the frame.
 */
Color_t IMG_ColorTypeGetPixel(Frame_t* framePtr, Point_t point);

/*
 * enlarges the allocated memory for the frame to double.
 */
void IMG_voidLargenFrameStorageSpace(Frame_t* framePtr);

/*
 * sets the color of a rectangle in the frame.
 */
void IMG_voidSetRectangle(Frame_t* framePtr, Point_t point1, Point_t point2, Color_t color);

/*
 * returns a random point.
 * uses "rand()", to be changed to use RNG in future...
 */
Point_t IMG_pointTypeGenerateRandom(u16 xMax, u16 yMax);

/*
 * checks if a certain point is inside rectangle.
 */
b8 IMG_b8IsPointInsideRectangle(Point_t point, Rectangle_t* rectPtr);

/*
 * checks if two rectangles are overlapped.
 */
b8 IMG_b8AreOverlapped(Rectangle_t* rctangle1, Rectangle_t* rctangle2);

#endif /* INCLUDE_LIB_IMG_INTERFACE_H_ */
