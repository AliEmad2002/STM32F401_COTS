/*
 * Menu_interface.h
 *
 *  Created on: Jan 18, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_LIB_MENU_MENU_INTERFACE_H_
#define INCLUDE_LIB_MENU_MENU_INTERFACE_H_

/*
 * an element of a menu could be just a callback function, or it could be a sub
 * menu
 */
typedef enum{
	Menu_ElementType_Callback,
	Menu_ElementType_SubMenu
}Menu_ElementType_t;

typedef struct{
	u8 str[MENU_MAX_STR_LEN];	// string that expresses the menu element.

	Menu_ElementType_t type;	// type of element.
								// (see "Menu_ElementType_t" description)

	void* childPtr;				// type of this pointer depends on "type" of the
								// element.
								// if "type" was "Menu_ElementType_Callback",
								// then this "childPtr" is a pointer to a
								// void function (void) that is to be called
								// when this element is entered.
								// otherwise, if "type" was
								// "Menu_ElementType_SubMenu", then this
								// "childPtr" is a pointer to the sub menu that
								// is to be entered when this element is
								// entered.
}Menu_Element_t;

typedef struct{
	/*	array of menu elements forming a menu	*/
	Menu_Element_t elementArr[MENU_MAX_NUMBER_OF_ELEMENTS];

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
 * Selection fucntions:
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
