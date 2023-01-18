/*
 * Menu_interface.h
 *
 *  Created on: Jan 18, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_LIB_MENU_MENU_INTERFACE_H_
#define INCLUDE_LIB_MENU_MENU_INTERFACE_H_

typedef struct{
	u8 str[MENU_MAX_STR_LEN];	// string that expresses the menu elemnt.

	void (*callback)(void);		// callback function, to be called when men
								// element is selected.
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
void Menu_voidInitMenuElement(
	Menu_Element_t* menuElement, const u8* str, void (*callback)(void));

void Menu_voidInitMenu(Menu_t* menu);

/*******************************************************************************
 * Add / Remove functions:
 ******************************************************************************/
void Menu_voidAddElement(Menu_t* menu, const u8* str, void (*callback)(void));

void Menu_voidRemoveElement(Menu_t* menu, u8 numberOfElement);

/*******************************************************************************
 * Selection fucntions:
 * (do not call callback function. it is just like hovering over a selection,
 * nothing more)
 ******************************************************************************/
inline void Menu_voidSelectElement(Menu_t* menu, u8 numberOfElement);

void Menu_voidSelectNextElement(Menu_t* menu);

void Menu_voidSelectPreviousElement(Menu_t* menu);

/*******************************************************************************
 * Execute menu element callback:
 ******************************************************************************/
inline void Menu_voidExecuteSelectedElementCallback(Menu_t* menu);

inline void Menu_voidExecuteElementCallback(Menu_t* menu, u8 numberOfElement);

#endif /* INCLUDE_LIB_MENU_MENU_INTERFACE_H_ */
