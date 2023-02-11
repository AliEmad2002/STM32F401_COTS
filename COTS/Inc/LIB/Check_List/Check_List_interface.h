/*
 * Check_List_interface.h
 *
 *  Created on: Feb 10, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_LIB_CHECK_LIST_CHECK_LIST_INTERFACE_H_
#define INCLUDE_LIB_CHECK_LIST_CHECK_LIST_INTERFACE_H_

typedef struct{
	/*	string that represents name of the element.	*/
	char* str;

	/*
	 * pointer to the flag / variable that indicates element is checked or not.
	 */
	b8* isChecked;

	/*	flag that indicates if an element is currently hidden or not	*/
	b8 isHidden;
}Check_List_Element_t;

typedef struct{
	Check_List_Element_t* elementArr;
	u8 numberOfElements;
	u8 currentSelectedElement;
}Check_List_t;

/*	checks if an element is hidden or not	*/
#define CHECK_LIST_IS_CURRENT_SELECTED_ELEMENT_HIDDEN(checkListPtr)	           \
(                                                                              \
	(checkListPtr)->elementArr[(checkListPtr)->currentSelectedElement].isHidden\
)

#define CHECK_LIST_IS_ELEMENT_HIDDEN(checkListPtr, i)\
(                                                    \
	(checkListPtr)->elementArr[(i)].isHidden         \
)

/*	toggles "isChecked" flag	*/
#define CHECK_LIST_TOGGLE_CHECK(checkListPtr, i)	\
	*((checkListPtr)->elementArr[(i)].isChecked) ^= 1;

/*	selects next non-hidden element	*/
void Check_List_voidSelecttNextElement(Check_List_t* checkListPtr);

/*	selects previous non-hidden element	*/
void Check_List_voidSelecttPreviousElement(Check_List_t* checkListPtr);

/*	checks current selected element	*/
void Check_List_voidCheckCurrentSelectedElement(Check_List_t* checkListPtr);

/*	un-checks current selected element	*/
void Check_List_voidUnCheckCurrentSelectedElement(Check_List_t* checkListPtr);

#endif /* INCLUDE_LIB_CHECK_LIST_CHECK_LIST_INTERFACE_H_ */
