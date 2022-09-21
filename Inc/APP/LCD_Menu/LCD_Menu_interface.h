/*
 * LCD_Menu_interface.h
 *
 *  Created on:	Aug 23, 2022
 *  Author:		Ali Emad Ali
 *
 *	Resources:
 *		*
 *
 */

#ifndef LCD_MENU_INTERFACE_H_
#define LCD_MENU_INTERFACE_H_

typedef struct{
	char** strArr;
	u8 len;

	/*
	 * whenever a row in a menu is selected, MCU would go excute either one of:
	 * -	void callBack function that takes void, and return back to menu.
	 * -	enter an other menu.
	 * they both are implemented in the two following arrays, all elements are NULL by
	 * default once object is initialized, both could be modified using their corresponding
	 * set functions, SW would go into the non-NULL one of them, if both are NULL, it won't leave
	 * the menu.
	 */
	void (**callBackArr)(void);
	void** subMenuPtrArr;
}Menu_t;


void LCD_Menu_voidInit(Menu_t* menuPtr, char* _strArr[], u8 _len);

void LCD_Menu_voidSetSubMenu(Menu_t* menuPtr, Menu_t* subMenuPtr, u8 index);

void LCD_Menu_voidSetCallBack(Menu_t* menuPtr, void(*callBack)(void), u8 index);

void LCD_Menu_voidEnter(LCD_t* lcdPtr, Menu_t* menuPtr);

void LCD_Menu_voidSetAtiveLevel(b8 level);

void LCD_Menu_voidSetDebouncingDelay(u16 _msDebouncingDelay);

void LCD_Menu_voidInitButtons(DIO_PIN up, DIO_PIN down, DIO_PIN enter, DIO_PIN _return);

void LCD_Menu_voidDisp(Menu_t* menuPtr, LCD_t* lcdPtr, u8 start, u8 HighLighted);


#endif /* LCD_MENU_INTERFACE_H_ */
