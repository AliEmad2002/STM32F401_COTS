/*
 * LCD_Menu_program.c
 *
 *  Created on:	Aug 23, 2022
 *  Author:		Ali Emad Ali
 *
 *	Resources:
 *		*
 *
 */

/*	LIB	*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include "Delay_interface.h"

#include "GPIO_interface.h"
/*	HAL	*/
#include "LCD_interface.h"
/*	APP	*/
#include "LCD_Menu_interface.h"

#ifndef NULL
#define NULL ((void*)0x0)
#endif


static DIO_PIN UP_BUTTON;
static DIO_PIN DOWN_BUTTON;
static DIO_PIN ENTER_BUTTON;
static DIO_PIN RETURN_BUTTON;

static b8 ACTIVE_LEVEL = false;

static u16 msDebouncingDelay = 50;

void LCD_Menu_voidInit(Menu_t* menuPtr, char* _strArr[], u8 _len)
{
	menuPtr->len = _len;

	menuPtr->strArr = malloc(_len * sizeof(char*));

	menuPtr->callBackArr = malloc(_len * sizeof(void*));

	menuPtr->subMenuPtrArr = malloc(_len * sizeof(Menu_t*));

	for (u8 i=0; i<_len; i++)
	{
		menuPtr->strArr[i] = malloc((strlen(_strArr[i])+1) * sizeof(char));

		strcpy(menuPtr->strArr[i], _strArr[i]);

		menuPtr->callBackArr[i] = NULL;

		menuPtr->subMenuPtrArr[i] = NULL;
	}
}

void LCD_Menu_voidSetCallBack(Menu_t* menuPtr, void(*callBack)(void), u8 index)
{
	menuPtr->callBackArr[index] = callBack;
}

void LCD_Menu_voidSetAtiveLevel(b8 level)
{
	ACTIVE_LEVEL = level;
}

void LCD_Menu_voidSetSubMenu(Menu_t* menuPtr, Menu_t* subMenuPtr, u8 index)
{
	menuPtr->subMenuPtrArr[index] = (Menu_t*)subMenuPtr;
}

void LCD_Menu_voidInitButtons(DIO_PIN up, DIO_PIN down, DIO_PIN enter, DIO_PIN _return)
{
	UP_BUTTON = up;
	DOWN_BUTTON = down;
	ENTER_BUTTON = enter;
	RETURN_BUTTON = _return;

	DIO_voidSetPinModeHeavy(up, DIO_MODE_input);
	DIO_voidSetPinModeHeavy(down, DIO_MODE_input);
	DIO_voidSetPinModeHeavy(enter, DIO_MODE_input);
	DIO_voidSetPinModeHeavy(_return, DIO_MODE_input);

	DIO_PULL pullType = ACTIVE_LEVEL ? DIO_PULL_down : DIO_PULL_up;
	DIO_voidSetPinPullHeavy(up, pullType);
	DIO_voidSetPinPullHeavy(down, pullType);
	DIO_voidSetPinPullHeavy(enter, pullType);
	DIO_voidSetPinPullHeavy(_return, pullType);
}

void LCD_Menu_voidSetDebouncingDelay(u16 _msDebouncingDelay)
{
	msDebouncingDelay = _msDebouncingDelay;
}

void LCD_Menu_voidEnter(LCD_t* lcdPtr, Menu_t* menuPtr)
{
	// print first 4 selections of this menu, and highlight first one:
	LCD_Menu_voidDisp(menuPtr, lcdPtr, 0, 0);

		s8 currentPos = 0;
		s8 start = 0;
		while(1)
		{
			if (DIO_b8ReadPinDigitalHeavy(DOWN_BUTTON) == ACTIVE_LEVEL)
			{
				currentPos++;
				if (currentPos == menuPtr->len)
				{
					currentPos = 0;
					start = 0;
				}

				if (currentPos - start > 3)
					start++;

				if (start >= menuPtr->len)
					start--;

				LCD_Menu_voidDisp(menuPtr, lcdPtr, start, currentPos);

				// debouncing delay:
				Delay_voidBlockingDelayMs(msDebouncingDelay);
			}

			else if (DIO_b8ReadPinDigitalHeavy(UP_BUTTON) == ACTIVE_LEVEL)
			{
				currentPos--;
				if (currentPos == -1)
				{
					currentPos = menuPtr->len  - 1;
					start = menuPtr->len - 4;
				}

				if (currentPos - start < 0)
					start--;

				if (start < 0)
					start++;

				LCD_Menu_voidDisp(menuPtr, lcdPtr, start, currentPos);

				// debouncing delay:
				Delay_voidBlockingDelayMs(msDebouncingDelay);
			}

			else if (DIO_b8ReadPinDigitalHeavy(ENTER_BUTTON) == ACTIVE_LEVEL)
			{
				// debouncing delay:
				Delay_voidBlockingDelayMs(msDebouncingDelay);


				if(menuPtr->callBackArr[currentPos] != NULL)
				{
					menuPtr->callBackArr[currentPos]();

					// display on return from callBack:
					LCD_Menu_voidDisp(menuPtr, lcdPtr, start, currentPos);
				}

				else if (menuPtr->subMenuPtrArr[currentPos] != NULL)
				{
					LCD_Menu_voidEnter(lcdPtr, (Menu_t*)(menuPtr->subMenuPtrArr[currentPos]));

					// display on return from callBack:
					LCD_Menu_voidDisp(menuPtr, lcdPtr, start, currentPos);
				}
			}

			else if (DIO_b8ReadPinDigitalHeavy(RETURN_BUTTON) == ACTIVE_LEVEL)
			{
				// debouncing delay:
				Delay_voidBlockingDelayMs(msDebouncingDelay);

				return;
			}
		}
}

void LCD_Menu_voidDisp(Menu_t* menuPtr, LCD_t* lcdPtr, u8 start, u8 HighLighted)
{
	LCD_voidClearDisplay(lcdPtr);

	for (u8 i=start; i<HighLighted; i++)
	{
		LCD_voidPrintString(lcdPtr, menuPtr->strArr[i]);
		LCD_voidPrintString(lcdPtr, "\n");
	}

	LCD_voidPrintString(lcdPtr, "->");
	LCD_voidPrintString(lcdPtr, menuPtr->strArr[HighLighted]);

	for (u8 i=HighLighted+1; i<start+4 && i<menuPtr->len; i++)
	{
		LCD_voidPrintString(lcdPtr, "\n");
		LCD_voidPrintString(lcdPtr, menuPtr->strArr[i]);
	}
}
