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
#include "Img_config.h"
#include "Img_interface.h"
#include "Colors.h"

/*
 * inits frame object.
 */
void IMG_voidinitFrame(
	Frame_t* framePtr, Color_t _backgroundColor)
{
	framePtr->backgroundColor = _backgroundColor;

	framePtr->rectCount = 0;
	framePtr->lineCount = 0;
	framePtr->circleCount = 0;
	framePtr->triangleCount = 0;
}

/*	sets frame background color	*/
void IMG_voidSetBackgroundColor(Frame_t* framePtr, Color_t _backgroundColor)
{
	framePtr->backgroundColor = _backgroundColor;
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
b8 IMG_b8AreRectanglesOverlapped(
	Rectangle_t* rctangle1, Rectangle_t* rctangle2)
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
