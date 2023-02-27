/*
 * Stack_program.c
 *
 *  Created on: Sep 21, 2022
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"

/*	SELF	*/
#include "Stack_Config.h"
#include "Stack_Interface.h"

#if ENABLE_STACK

/*******************************************************************************
 * Init:
 ******************************************************************************/
void Stack_voidInit(Stack_t* s)
{
	s->tailIndex = 0;
}

void Stack_voidAssignAllocatedPointers(
	Stack_t* s, Stack_Data_t* allocatedPtrArr[STACK_MAX_LEN])
{
	for (u16 i = 0; i < STACK_MAX_LEN; i++)
	{
		s->ptrArr[i] = allocatedPtrArr[i];
	}
}

/*******************************************************************************
 * Info:
 ******************************************************************************/
u16 Stack_u16GetLenUsed(Stack_t* s)
{
	return s->tailIndex;
}

/*******************************************************************************
 * Push:
 ******************************************************************************/
b8 Stack_b8Push(Stack_t* s, Stack_Data_t* newDataPtr)
{
	/*	check for full space	*/
	if (s->tailIndex == STACK_MAX_LEN)
		return false;

	/*	copy new data at "tailIndex"	*/
	Stack_Data_t** distPP = &(s->ptrArr[s->tailIndex]);

	STACK_COPY_ELEMENT(distPP, newDataPtr);

	/*	increment "tailIndex"	*/
	s->tailIndex++;

	/*	push successful	*/
	return true;
}

/*******************************************************************************
 * Pop:
 ******************************************************************************/
b8 Stack_b8Pop(Stack_t* s, Stack_Data_t** dataPP)
{
	/*	check if stack is empty	*/
	if (s->tailIndex == 0)
		return false;

	/*
	 * copy end of the stack (at "tailIndex" - 1) to the argument pointer, using
	 * the user defined copy function.
	 */
	Stack_Data_t* srcPtr = s->ptrArr[s->tailIndex - 1];

	STACK_COPY_ELEMENT(dataPP, srcPtr);

	/*
	 * delete (free) end of the stack using the user defined delete function.
	 */
	STACK_FREE_ELEMENT(srcPtr);

	/*	decrement "tailIndex"	*/
	s->tailIndex--;

	/*	pop successful	*/
	return true;
}

///*******************************************************************************
// * Print:
// ******************************************************************************/
//void Stack_voidPrint(Stack_t* s)
//{
//	PRINTF("Printing Stack object @ %u\n", (u32)s);
//
//	u16 i = 0;
//
//	Stack_Data_t* dPtr;
//
//	while(Stack_b8Pop(s, &dPtr))
//	{
//		PRINTF("%u: ", i++);
//		STACK_PRINT_ELEMENT(dPtr);
//		PRINTF("\n");
//	}
//
//	PRINTF("=========================\n");
//}

#endif	/*	ENABLE_STACK	*/
