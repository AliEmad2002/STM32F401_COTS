/*
 * Img_program.c
 *
 *  Created on: Oct 7, 2022 10:57:59 AM
 *      Author: Ali Emad Ali
 */


/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include <stdlib.h>

/*	SELF	*/
#include "Img_interface.h"

/*
 * inits frame object.
 */
void IMG_voidinitFrame(Frame_t* framePtr, Color_t _backgroundColor, u16 initialNumberOfRectangles)
{
	framePtr->backgroundColor = _backgroundColor;

	framePtr->rectCount = 0;

	framePtr->maxNumberOfRectangles = initialNumberOfRectangles;

	framePtr->rectPtrArr = (Rectangle_t**)malloc(initialNumberOfRectangles * sizeof(Rectangle_t*));
}

/*
 * frees memory of a previously allocated frame.
 */
void IMG_voidFreeFrame(Frame_t* framePtr)
{
	/*	only if frame is previously allocated	*/
	if (framePtr->maxNumberOfRectangles != 0)
	{
		for (u16 i = 0; i < framePtr->rectCount; i++)
			free(framePtr->rectPtrArr[i]);
		free(framePtr->rectPtrArr);
		framePtr->rectCount = 0;
		framePtr->maxNumberOfRectangles = 0;
	}
}

/*
 * sets the color of a pixel in the frame.
 */
//void Img_voidSetPixel(Frame_t* framePtr, u16 x, u16 y, Color_t color)


/*
 * gets the color of a pixel in the frame.
 */
Color_t IMG_ColorTypeGetPixel(Frame_t* framePtr, Point_t point)
{
	s16 containingRectIndex = IMG_u16FindPoint(framePtr, point);
	if (containingRectIndex == -1)
		return framePtr->backgroundColor;
	else
		return framePtr->rectPtrArr[containingRectIndex]->color;
}

/*
 * returns the index of the rectangle containing the point.
 * if not in a rectangle, returns -1
 */
s16 IMG_u16FindPoint(Frame_t* framePtr, Point_t point)
{
	for (u16 i = 0; i < framePtr->rectCount; i++)
	{
		if (IMG_b8IsPointInsideRectangle(point, framePtr->rectPtrArr[i]))
			return (s16)i;
	}
	return -1;
}

/*
 * enlarges the allocated memory for the frame to double.
 */
void IMG_voidLargenFrameStorageSpace(Frame_t* framePtr)
{
	/*	create a new array of length: 2 * framePtr->maxNumberOfRectangles	*/
	framePtr->maxNumberOfRectangles *= 2;
	Rectangle_t** newPtrArr = (Rectangle_t**)malloc(framePtr->maxNumberOfRectangles * sizeof(Rectangle_t*));

	/*	copy "framePtr->rectPtrArr" to "newPtrArr"	*/
	for (u8 i=0; i<framePtr->rectCount; i++)
	{
		newPtrArr[i] = framePtr->rectPtrArr[i];
	}

	/*	delete "framePtr->rectPtrArr"	*/
	free(framePtr->rectPtrArr);

	/*make "framePtr->rectPtrArr" point at "newPtrArr"	*/
	framePtr->rectPtrArr = newPtrArr;
}

/*
 * sets the color of a rectangle in the frame.
 */
void IMG_voidSetRectangle(Frame_t* framePtr, Point_t point1, Point_t point2, Color_t color)
{
	/*	if pointers to rectangles array is full	*/
	if (framePtr->rectCount == framePtr->maxNumberOfRectangles)
	{
		IMG_voidLargenFrameStorageSpace(framePtr);
	}

	/*	allocate rectangle	*/
	framePtr->rectPtrArr[framePtr->rectCount] = (Rectangle_t*)malloc(sizeof(Rectangle_t));

	/*	assign rectangle params	*/
	framePtr->rectPtrArr[framePtr->rectCount]->color = color;
	framePtr->rectPtrArr[framePtr->rectCount]->pointStart = point1;
	framePtr->rectPtrArr[framePtr->rectCount]->pointEnd = point2;

	/*	increment rectangle counter of the frame	*/
	framePtr->rectCount++;
}

/*
 * returns a random point.
 * uses "rand()", to be changed to use RNG in future...
 */
Point_t IMG_pointTypeGenerateRandom(u16 xMax, u16 yMax)
{
	Point_t point = {
		.x = rand() % xMax,
		.y = rand() % yMax
	};

	return point;
}

/*
 * checks if a certain point is inside rectangle.
 */
b8 IMG_b8IsPointInsideRectangle(Point_t point, Rectangle_t* rectPtr)
{
	if (
			(point.x >= rectPtr->pointStart.x)	&&	(point.y >= rectPtr->pointStart.y)	&&
			(point.x <= rectPtr->pointEnd.x)	&&	(point.y <= rectPtr->pointEnd.y)
		)
	{
		return true;
	}
	return false;
}

/*
 * checks if two rectangles are overlapped.
 */
b8 IMG_b8AreOverlapped(Rectangle_t* rctangle1, Rectangle_t* rctangle2)
{
	/*
	 * two rectangles are overlapped when one point at least of any of them
	 * is inside the other.
	 */

	if (
		IMG_b8IsPointInsideRectangle(rctangle1->pointStart, rctangle2)	||
		IMG_b8IsPointInsideRectangle(rctangle1->pointEnd, rctangle2)	||
		IMG_b8IsPointInsideRectangle(rctangle2->pointStart, rctangle1)	||
		IMG_b8IsPointInsideRectangle(rctangle2->pointEnd, rctangle1)
		)
	{
		return true;
	}
	return false;
}
