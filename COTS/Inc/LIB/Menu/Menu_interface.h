/*
 * Menu_interface.h
 *
 *  Created on: Jan 18, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_LIB_MENU_MENU_INTERFACE_H_
#define INCLUDE_LIB_MENU_MENU_INTERFACE_H_

/*
 * an element of a menu could be just a callback function, a sub-menu or a check
 * list.
 */
typedef enum{
	Menu_ElementType_Callback,
	Menu_ElementType_SubMenu,
	Menu_ElementType_CheckList
}Menu_ElementType_t;

typedef struct{
	char* str;					// String that expresses the menu element.

	Menu_ElementType_t type;	// Type of element.
								// (see "Menu_ElementType_t" description)

	void* childPtr;				//	pointer to the element.
}Menu_Element_t;

typedef struct{
	/*	array of menu elements forming a menu	*/
	Menu_Element_t* elementArr;

	/*	number of menu elements in menu	*/
	u8 numberOfElements;

	/*	index of currently selected menu element	*/
	u8 currentSelected;
}Menu_t;

/*******************************************************************************
 * Init functions:
 ******************************************************************************/
void Menu_voidInitMenuElementAsFunction(
	Menu_Element_t* menuElement, const u8* str, void (*callback)(void));

void Menu_voidInitMenuElementAsSubMenu(
	Menu_Element_t* menuElement, const u8* str, Menu_t* subMenuPtr);

void Menu_voidInitMenu(Menu_t* menu);

/*******************************************************************************
 * Selection functions:
 * (do not call callback function. it is just like hovering over a selection,
 * nothing more)
 ******************************************************************************/
void Menu_voidSelectElement(Menu_t* menu, u8 numberOfElement);

void Menu_voidSelectNextElement(Menu_t* menu);

void Menu_voidSelectPreviousElement(Menu_t* menu);

/*******************************************************************************
 * Enter menu element:
 ******************************************************************************/
void Menu_voidEnterSelectedElement(Menu_t* menu);

#endif /* INCLUDE_LIB_MENU_MENU_INTERFACE_H_ */
