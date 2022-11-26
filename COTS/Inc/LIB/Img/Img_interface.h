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
	b8 filled;
}Rectangle_t;

typedef struct{
	Point_t pointStart;
	Point_t pointEnd;
	Color_t color;
}Line_t;

typedef struct{
	Point_t pointCenter;
	u16 radious;
	Color_t color;
	b8 filled;
}Circle_t;

typedef struct{
	Point_t point1;
	Point_t point2;
	Point_t point3;
	Color_t color;
	b8 filled;
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
