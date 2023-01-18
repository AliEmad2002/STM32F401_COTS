/*
 * Menu_program.c
 *
 *  Created on: Jan 18, 2023
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include <string.h>

/*	SELF	*/
#include "Menu_config.h"
#include "Menu_interface.h"


/*******************************************************************************
 * Init functions:
 ******************************************************************************/
void Menu_voidInitMenuElement(
	Menu_Element_t* menuElement, const u8* str, void (*callback)(void))
{
	/*	copy 'str' to 'menuElement->str'	*/
	strcpy((char*)menuElement->str, (const char*)str);

	/*	copy 'callback' to 'menuElement->callback'	*/
	menuElement->callback = callback;
}

void Menu_voidInitMenu(Menu_t* menu)
{
	/*	reset counters	*/
	menu->currentSelected = 0;
	menu->numberOfElements = 0;
}

/*******************************************************************************
 * Add / Remove functions:
 ******************************************************************************/
void Menu_voidAddElement(Menu_t* menu, const u8* str, void (*callback)(void))
{
	Menu_Element_t* firstEmptyElementPtr =
		&menu->elementArr[menu->numberOfElements++];

	Menu_voidInitMenuElement(firstEmptyElementPtr, str, callback);
}

/*******************************************************************************
 * Selection fucntions:
 * (do not call callback function. it is just like hovering over a selection,
 * nothing more)
 ******************************************************************************/
void Menu_voidSelectElement(Menu_t* menu, u8 numberOfElement)
{
	menu->currentSelected = numberOfElement;
}

void Menu_voidSelectNextElement(Menu_t* menu)
{
	menu->currentSelected++;

	if (menu->currentSelected >= menu->numberOfElements)
		menu->currentSelected = 0;
}

void Menu_voidSelectPreviousElement(Menu_t* menu)
{
	if (menu->currentSelected == 0)
		menu->currentSelected = menu->numberOfElements - 1;

	else
		menu->currentSelected--;
}

/*******************************************************************************
 * Execute menu element callback:
 ******************************************************************************/
void Menu_voidExecuteSelectedElementCallback(Menu_t* menu)
{
	menu->elementArr[menu->currentSelected].callback();
}

void Menu_voidExecuteElementCallback(Menu_t* menu, u8 numberOfElement)
{
	menu->elementArr[numberOfElement].callback();
}
