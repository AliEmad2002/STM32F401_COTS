/*
 * Txt_program.c
 *
 *  Created on: Jan 14, 2023
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"

/*	SELF	*/
#include "Txt_drawings.h"
#include "Txt_interface.h"


void Txt_voidCpyCharToDynamicPixArr(
	u8 ch, u16 charColor, u16 bgColor, u8 size,
	Txt_HorizontalMirroring_t hMirror, Txt_VerticalMirroring_t vMirror,
	u8 tlX, u8 tlY, u16** pixArr)
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
		/*
		 * after resizing, the pixel located in (i, j), covers the rectangle
		 * of top left corner: 		   (i * size, j * size),
		 * and of bottom right corner: ((i+1) * size - 1, (j+1) * size - 1)
		 */
		/*
		 * since the previously mentioned rectangle is shifted by:
		 * (tlX, tlY), then the actual boundaries of the pixel (i, j) after
		 * resizing is:
		 * of top left corner: 		   (i * size + tlX, j * size + tlY),
		 * and of bottom right corner:
		 * 		((i+1) * size + tlX - 1, (j+1) * size + tlY - 1)
		 */
		/*
		 * this commented loop is the one that exactly puts the previous
		 * math to work. But.. there's always the programmer's way, which
		 * is the loop implemented after this one, and the variables 'x',
		 * 'y'.
		 */
		u8 y = tlY;
		for (s8 j = jStart; j != jEnd; j += jIncrementer)
		{
			/*	find color of a pixel before resizing	*/
			u16 color = GET_BIT(Txt_drawings[ch][i], j) ? charColor : bgColor;

			/*for (u8 y = j * size + tlY; y < (j+1) * size + tlY; y++)
			{
				for (u8 x = i * size + tlX; x < (i+1) * size + tlX; x++)
				{
					pixArr[y][x] = color;
				}
			}*/
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

void Txt_voidCpyCharToStaticPixArr(
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
			u16 color = GET_BIT(Txt_drawings[ch][i], j) ? charColor : bgColor;

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

void Txt_voidCpyStrToDynamicPixArr(
	const u8* str, u16 charColor, u16 bgColor, u8 size,
	Txt_HorizontalMirroring_t hMirror, Txt_VerticalMirroring_t vMirror,
	u8 tlXInitial, u8 tlYInitial, u8 pixArrWidth, u16** pixArr)
{
	u8 xIncrementer = size * 6;
	u8 yIncrementer = size * 8;

	u8 tlX = tlXInitial;
	u8 tlY = tlYInitial;

	for (u16 i = 0; str[i] != '\0'; i++)
	{
		Txt_voidCpyCharToDynamicPixArr(
			str[i], charColor, bgColor, size,
			hMirror, vMirror, tlX, tlY, pixArr);

		tlX += xIncrementer;
		if (tlX > pixArrWidth)
		{
			tlX = tlXInitial;
			tlY += yIncrementer;
		}
	}
}

void Txt_voidCpyStrToStaticPixArr(
	const u8* str, u16 charColor, u16 bgColor, u8 size,
	Txt_HorizontalMirroring_t hMirror, Txt_VerticalMirroring_t vMirror,
	u8 tlXInitial, u8 tlYInitial, u8 pixArrWidth, u16 pixArr[][pixArrWidth])
{
	u8 xIncrementer = size * 6;
	u8 yIncrementer = size * 8;

	u8 tlX = tlXInitial;
	u8 tlY = tlYInitial;

	for (u16 i = 0; str[i] != '\0'; i++)
	{
		Txt_voidCpyCharToStaticPixArr(
			str[i], charColor, bgColor, size,
			hMirror, vMirror, tlX, tlY, pixArrWidth, pixArr);

		tlX += xIncrementer;
		if (tlX > pixArrWidth)
		{
			tlX = tlXInitial;
			tlY += yIncrementer;
		}
	}
}











