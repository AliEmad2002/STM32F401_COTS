/*
 * Txt_program.c
 *
 *  Created on: Jan 14, 2023
 *      Author: Ali Emad Ali
 *
 * TODO: Use DMA to accelerate.
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "diag/trace.h"

/*	SELF	*/
#include "Txt_drawings.h"
#include "Txt_interface.h"

#define GET_PIX(ch, i, j)	(GET_BIT(Txt_drawings[(u8)(ch)][(i)], (j)))

void Txt_voidPrintCharOnPixArrNormalOrientation(
	char ch, u16 charColor, u16 bgColor, u8 tlX, u8 tlY, u16* pixArr,
	u8 pixArrWidth)
{
	for (u8 i = 0; i < 8; i++)
	{
		for (u8 j = 0; j < 5; j++)
		{
			u16 color = GET_PIX(ch, j, i) ? charColor : bgColor;

			pixArr[(tlY + i) * pixArrWidth + (tlX + j)] = color;
		}
	}
}

void Txt_voidPrintCharOnPixArrRigthOrientation(
	char ch, u16 charColor, u16 bgColor, u8 tlX, u8 tlY, u16* pixArr,
	u8 pixArrWidth)
{
	for (u8 i = 0; i < 8; i++)
	{
		for (u8 j = 0; j < 5; j++)
		{
			u16 color = GET_PIX(ch, j, i) ? charColor : bgColor;

			pixArr[(tlY + j) * pixArrWidth +7 - (tlX + i)] = color;
		}
	}
}

void Txt_voidPrintStrOnPixArrNormalOrientation(
	char* str, u16 charColor, u16 bgColor, u8 tlX, u8 tlY, u16* pixArr,
	u8 pixArrWidth, u8 pixArrHeight)
{
	/*	clear all pixArr area.	*/
	u16 pixArrSize = pixArrHeight * pixArrWidth;
	for (u16 i = 0; i < pixArrSize; i++)
		pixArr[i] = bgColor;

	/*	draw characters	*/
	for (u8 i = 0; str[i] != '\0'; i++)
	{
		Txt_voidPrintCharOnPixArrNormalOrientation(
			str[i], charColor, bgColor, tlX + i * 6, tlY, pixArr, pixArrWidth);
	}
}

void Txt_voidPrintStrOnPixArrRightOrientation(
	char* str, u16 charColor, u16 bgColor, u8 tlX, u8 tlY, u16* pixArr,
	u8 pixArrWidth, u8 pixArrHeight)
{
	/*	clear all pixArr area.	*/
	u16 pixArrSize = pixArrHeight * pixArrWidth;
	for (u16 i = 0; i < pixArrSize; i++)
		pixArr[i] = bgColor;

	/*	draw characters	*/
	for (u8 i = 0; str[i] != '\0'; i++)
	{
		Txt_voidPrintCharOnPixArrRigthOrientation(
			str[i], charColor, bgColor, tlX, tlY + i * 6, pixArr, pixArrWidth);
	}
}

