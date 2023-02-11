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
#include "Check_List_interface.h"

/*	SELF	*/
#include "Menu_config.h"
#include "Menu_interface.h"


/*******************************************************************************
 * Init functions:
 ******************************************************************************/
void Menu_voidInitMenuElementAsFunction(
	Menu_Element_t* menuElement, const u8* str, void (*callback)(void))
{
	/*	copy 'str' to 'menuElement->str'	*/
	strcpy((char*)menuElement->str, (const char*)str);

	/*	set type	*/
	menuElement->type = Menu_ElementType_Callback;

	/*	copy 'callback' to 'menuElement->childPtr'	*/
	menuElement->childPtr = (void*)callback;
}

void Menu_voidInitMenuElementAsSubMenu(
	Menu_Element_t* menuElement, const u8* str, Menu_t* subMenuPtr)
{
	/*	copy 'str' to 'menuElement->str'	*/
	strcpy((char*)menuElement->str, (const char*)str);

	/*	set type	*/
	menuElement->type = Menu_ElementType_SubMenu;

	/*	copy 'subMenuPtr' to 'menuElement->childPtr'	*/
	menuElement->childPtr = (void*)subMenuPtr;
}

void Menu_voidInitMenu(Menu_t* menu)
{
	/*	reset counters	*/
	menu->currentSelected = 0;
	menu->numberOfElements = 0;
}

/*******************************************************************************
 * Selection functions:
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

inline void Menu_voidSelectPreviousElement(Menu_t* menu)
{
	if (menu->currentSelected == 0)
		menu->currentSelected = menu->numberOfElements - 1;

	else
		menu->currentSelected--;
}

/*******************************************************************************
 * Enter menu element:
 ******************************************************************************/
void Menu_voidEnterSelectedElement(Menu_t* menu)
{
	Menu_Element_t* selectedElementPtr =
		&menu->elementArr[menu->currentSelected];

	switch(selectedElementPtr->type)
	{
	case Menu_ElementType_Callback:
		((vvFunc_t)selectedElementPtr->childPtr)();
		break;

	case Menu_ElementType_SubMenu:
		MENU_ENTERING_FUNCTION((Menu_t*)selectedElementPtr->childPtr);
		break;

	case Menu_ElementType_CheckList:
		CHECK_LIST_ENTERING_FUNCTION(
			(Check_List_t*)selectedElementPtr->childPtr);
	}
}
