/*
 * Txt_program.c
 *
 *  Created on: Jan 14, 2023
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "diag/trace.h"

/*	SELF	*/
#include "Txt_drawings.h"
#include "Txt_interface.h"

#define GET_PIX(ch, i, j)	(GET_BIT(Txt_drawings[(ch)][(i)], (j)))

void Txt_voidCpyCharToStaticPixArrNormalOrientation(
	u8 ch, u16 charColor, u16 bgColor, u8 size,
	Txt_HorizontalMirroring_t hMirror, Txt_VerticalMirroring_t vMirror,
	u8 tlX, u8 tlY, u8 pixArrWidth, u16 pixArr[][pixArrWidth])
{
	s8 jStart, jEnd, jIncrementer;
	s8 iStart, iEnd, iIncrementer;

	switch (hMirror)
	{
	case Txt_HorizontalMirroring_Disabled:
		jStart = 0;		jEnd = 8;	jIncrementer = 1;
		break;
	default:
		jStart = 7;		jEnd = -1;	jIncrementer = -1;
	}

	switch (vMirror)
	{
	case Txt_VerticalMirroring_Disabled:
		iStart = 0;		iEnd = 5;	iIncrementer = 1;
		break;
	default:
		iStart = 4;		iEnd = -1;	iIncrementer = -1;
	}

	u8 x = tlX;
	for (s8 i = iStart; i != iEnd; i += iIncrementer)
	{
		/*	see description in "Txt_voidCpyCharToDynamicPixArr()"	*/
		u8 y = tlY;
		for (s8 j = jStart; j != jEnd; j += jIncrementer)
		{
			/*	find color of a pixel before resizing	*/
			u16 color = GET_PIX(ch, i, j) ? charColor : bgColor;

			for (u8 Y = y; Y < y + size; Y++)
			{
				for (u8 X = x; X < x + size; X++)
				{
					pixArr[Y][X] = color;
				}
			}
			y += size;
		}
		x += size;
	}
}

void Txt_voidCpyCharToStaticPixArrRightOrientation(
	u8 ch, u16 charColor, u16 bgColor, u8 size,
	Txt_HorizontalMirroring_t hMirror, Txt_VerticalMirroring_t vMirror,
	u8 tlX, u8 tlY, u8 pixArrWidth, u16 pixArr[][pixArrWidth])
{
	for (u8 i = 0; i < 8; i++)
	{
		for (u8 j = 0; j < 5; j++)
		{
			u8 x = tlX + 7 - i;
			u8 y = tlY + j;
			pixArr[y][x] = GET_PIX(ch, i, j) ? charColor : bgColor;
		}

	}
}

void Txt_voidCpyCharToStaticPixArrLeftOrientation(
	u8 ch, u16 charColor, u16 bgColor, u8 size,
	Txt_HorizontalMirroring_t hMirror, Txt_VerticalMirroring_t vMirror,
	u8 tlX, u8 tlY, u8 pixArrWidth, u16 pixArr[][pixArrWidth]);

void Txt_voidCpyCharToStaticPixArr180DegreesOrientation(
	u8 ch, u16 charColor, u16 bgColor, u8 size,
	Txt_HorizontalMirroring_t hMirror, Txt_VerticalMirroring_t vMirror,
	u8 tlX, u8 tlY, u8 pixArrWidth, u16 pixArr[][pixArrWidth]);

void Txt_voidCpyStrToStaticPixArrNormalOrientation(
	const u8* str, u16 charColor, u16 bgColor, u8 size,
	Txt_HorizontalMirroring_t hMirror, Txt_VerticalMirroring_t vMirror,
	u8 tlXInitial, u8 tlYInitial,
	u8 pixArrHieght, u8 pixArrWidth, u16 pixArr[][pixArrWidth])
{
	u8 xIncrementer = size * 6;
	u8 yIncrementer = size * 8;

	u8 tlX = tlXInitial;
	u8 tlY = tlYInitial;

	u8 maxX = tlXInitial + pixArrWidth;
	u8 maxY = tlYInitial + pixArrHieght;

	for (u16 i = 0;; i++)
	{
		/*	case end-line or terminating character	*/
		if (str[i] == '\n' || str[i] == '\0')
		{
			/*	fill with bgColor till end of the line	*/
			for (u8 y = tlY; y < tlY + yIncrementer && y < maxY; y++)
			{
				for (u8 x = tlX; x < maxX; x++)
					pixArr[y][x] = bgColor;
			}

			/*	move counters to next line	*/
			tlX = tlXInitial;
			tlY += yIncrementer;

			if (str[i] == '\n')
				continue;
			else
				break;
		}

		/*	chase other characters	*/
		Txt_voidCpyCharToStaticPixArrNormalOrientation(
			str[i], charColor, bgColor, size,
			hMirror, vMirror, tlX, tlY, pixArrWidth, pixArr);

		/*	calculate position of next character	*/
		tlX += xIncrementer;

		/*	if next character is in the next line	*/
		if (tlX >= maxX)
		{
			/*	fill with bgColor till end of the line	*/
			for (u8 y = tlY; y < tlY + yIncrementer; y++)
			{
				for (u8 x = tlX - xIncrementer; x < maxX; x++)
					pixArr[y][x] = bgColor;
			}
			/*	set counters to start of next line	*/
			tlX = tlXInitial;
			tlY += yIncrementer;
			continue;
		}
		/*
		 * if next character is in the same line, fill the line segment between
		 * current and next characters with bgColor
		 */
		else
		{
			for (u8 y = tlY; y < tlY + yIncrementer; y++)
			{
				pixArr[y][tlX - 1] = bgColor;
			}
		}
	}

	/*	if there's still non written space in pixArr, fill it with bgColor	*/
	for (u8 y = tlY; y < maxY; y++)
	{
		for (u8 x = tlX; x < maxX; x++)
			pixArr[y][x] = bgColor;
	}
}

void Txt_voidCpyStrToStaticPixArrRightOrientation(
	const u8* str, u16 charColor, u16 bgColor, u8 size,
	Txt_HorizontalMirroring_t hMirror, Txt_VerticalMirroring_t vMirror,
	u8 tlXInitial, u8 tlYInitial,
	u8 pixArrHieght, u8 pixArrWidth, u16 pixArr[][pixArrWidth])
{
	u8 tlX = tlXInitial + pixArrWidth - 8;
	u8 tlY = tlYInitial;

	u8 maxX = tlXInitial + pixArrWidth;
	u8 maxY = tlYInitial + pixArrHieght;

	for (u16 i = 0; str[i] != '\0'; i++)
	{
		Txt_voidCpyCharToStaticPixArrRightOrientation(
			str[i], charColor, bgColor, size, hMirror, vMirror, tlX, tlY,
			pixArrWidth, pixArr);

		tlY += 6;

		if (tlY >= maxY)
		{
			tlY = tlYInitial;
			tlX -= 8;
		}
	}
}










