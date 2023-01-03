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

/*
 * in line definition, "pointStart" should exist before "pointEnd".
 */
typedef struct{
	Point_t pointStart;
	Point_t pointEnd;
	Color_t color;
	/*
	 * values stored to lessen overhead in "IMG_IS_X_Y_IN_LINE()" macro.
	 * Must be evaluated and stored on line creation. (happens in
	 * "IMG_INIT_LINE()"
	 */
	u16 deltaY; // = pointEnd.y - pointStart.y
	u16 deltaX; // = pointEnd.x - pointStart.x
	s16 k;		// = pointStart.y * deltaX - pointStart.x * deltaY
}Line_t;

typedef struct{
	Point_t pointCenter;
	u16 radious;
	Color_t color;
}Circle_t;

typedef struct{
	Point_t point1;
	Point_t point2;
	Point_t point3;
	Color_t color;
}Triangle_t;


typedef struct{
	Rectangle_t rectArr[MAX_NUMBER_OF_EACH_SHAPE_PER_FRAME];
	Line_t lineArr[MAX_NUMBER_OF_EACH_SHAPE_PER_FRAME];
	Circle_t circleArr[MAX_NUMBER_OF_EACH_SHAPE_PER_FRAME];
	Triangle_t triangleArr[MAX_NUMBER_OF_EACH_SHAPE_PER_FRAME];

	u16 rectCount;
	u16 lineCount;
	u16 circleCount;
	u16 triangleCount;

	Color_t backgroundColor;
}Frame_t;

/*
 * contains boundaries of certain area and pixels in it.
 * Dynamic. Hence, must be used carefully.
 */
typedef struct{
	Point_t pointTL;	// top-left point
	Point_t pointBR;	// bottom-right point

}ImgSegment_t;

/*
 * inits frame object.
 */
void IMG_voidinitFrame(
	Frame_t* framePtr, Color_t _backgroundColor);

/*	sets frame background color	*/
void IMG_voidSetBackgroundColor(Frame_t* framePtr, Color_t _backgroundColor);

/*
 * macro for accessing the current rectangle in "rectArr[]"
 * "frame" is of type: "Frame_t".
 */
#define IMG_CURRENT_RECT(frame)	((frame).rectArr[(frame).rectCount])

/*
 * macro for accessing the current line in "lineArr[]"
 * "frame" is of type: "Frame_t".
 */
#define IMG_CURRENT_LINE(frame)	((frame).lineArr[(frame).lineCount])

/*
 * macro for accessing the current circle in "circleArr[]"
 * "frame" is of type: "Frame_t".
 */
#define IMG_CURRENT_CIRCLE(frame)	((frame).circleArr[(frame).circleCount])

/*
 * macro for accessing the current triangle in "triangleArr[]"
 * "frame" is of type: "Frame_t".
 */
#define IMG_CURRENT_TRIANGLE(frame)	((frame).triangleArr[(frame).triangleCount])

/*
 * inits line. Must be used on creating a new line.
 * remember that in line definition, "pointStart" should exist before
 * "pointEnd".
 */
#define IMG_INIT_LINE (line, xS, yS, xE, yE, color)                         \
{                                                                           \
	(line).pointStart.x = (xS);                                             \
	(line).pointStart.y = (yS);                                             \
	(line).pointEnd.x = (xE);                                               \
	(line).pointEnd.y = (yE);                                               \
	(line).color.code565 = (color).code565;                                 \
                                                                            \
	(line).deltaY = (yE) - (yS);                                            \
	(line).deltaX = (xE) - (xS);                                            \
	(line).k = (s16)((yS) * (line).deltaX) - (s16)((xS) * (line).deltaY);   \
}

/*
 * checks if point{x, y} is on "line".
 * "x" & "y" are of type: "u16",
 * "line" is of type: "Line_t".
 */
#define IMG_IS_X_Y_ON_LINE(x, y, line)	\
	((s16)((y) * (line).deltaX) - (s16)((x) * (line).deltaY) == (line).k)



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
b8 IMG_b8AreRectanglesOverlapped(
	Rectangle_t* rctangle1, Rectangle_t* rctangle2);

#endif /* INCLUDE_LIB_IMG_INTERFACE_H_ */
