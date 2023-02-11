/*
 * Check_List_program.c
 *
 *  Created on: Feb 10, 2023
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"

/*	SELF	*/
#include "Check_List_interface.h"

void Check_List_voidSelecttNextElement(Check_List_t* checkListPtr)
{
	for (u8 i = 0; i < checkListPtr->numberOfElements; i++)
	{
		/*	increment "currentSelectedElement"	*/
		checkListPtr->currentSelectedElement++;

		/*	if it hits "numberOfElements", reset it to zero	*/
		if (checkListPtr->currentSelectedElement == checkListPtr->numberOfElements)
			checkListPtr->currentSelectedElement = 0;

		/*
		 * if "currentSelectedElement" is not hidden, break. Hence, it is marked
		 * as the selected one.
		 */
		if (!CHECK_LIST_IS_CURRENT_SELECTED_ELEMENT_HIDDEN(checkListPtr))
			break;
	}
}

void Check_List_voidSelecttPreviousElement(Check_List_t* checkListPtr)
{
	for (u8 i = 0; i < checkListPtr->numberOfElements; i++)
	{
		/*
		 * if "currentSelectedElement" equals zero, set it to maximum value
		 */
		if (checkListPtr->currentSelectedElement == 0)
		{
			checkListPtr->currentSelectedElement =
				checkListPtr->numberOfElements - 1;
		}

		/*	otherwise, decrement it	*/
		else
		{
			checkListPtr->currentSelectedElement--;
		}

		/*
		 * if "currentSelectedElement" is not hidden, break. Hence, it is marked
		 * as the selected one.
		 */
		if (!CHECK_LIST_IS_CURRENT_SELECTED_ELEMENT_HIDDEN(checkListPtr))
			break;
	}
}

void Check_List_voidCheckCurrentSelectedElement(Check_List_t* checkListPtr)
{
	/*	extract pointer to current selected check-list element	*/
	Check_List_Element_t* currentCheckedElementPtr =
		&checkListPtr->elementArr[checkListPtr->currentSelectedElement];

	/*	set the "isChecked" flag	*/
	*(currentCheckedElementPtr->isChecked) = true;
}

void Check_List_voidUnCheckCurrentSelectedElement(Check_List_t* checkListPtr)
{
	/*	extract pointer to current selected check-list element	*/
	Check_List_Element_t* currentCheckedElementPtr =
		&checkListPtr->elementArr[checkListPtr->currentSelectedElement];

	/*	reset the "isChecked" flag	*/
	*(currentCheckedElementPtr->isChecked) = false;
}









