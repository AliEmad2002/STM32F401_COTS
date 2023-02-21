/*
 * Stack_program.c
 *
 *  Created on: Sep 21, 2022
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include <stdlib.h>
#include "Std_Types.h"
#include "Bit_Math.h"
/*	SELF	*/
#include "Stack_config.h"
#include "Stack_interface.h"

#if STACK_ENABLE == 1

/*	stack array (an array of pointers to pointers to stack's data-type)	*/
static STACK_TYPE_PTR* stack;

/*	Index of stack's current tail (after last object)	*/
static u16 tailIndex = 0;

/*	Index of stack's current head	(first object)	*/
static u16 headIndex = 0;

void Stack_voidInit(void)
{
	stack = (STACK_TYPE_PTR*)malloc(STACK_MAX_LEN * sizeof(STACK_TYPE_PTR));
}

void Stack_voidIncrementHeadIndex(void)
{
	/*
	 * if "head" is end of the array, circularly, next to it would be first of
	 * the array.
	 */
	if (headIndex == STACK_MAX_LEN - 1)
		headIndex = 0;

	/*	otherwise, it is "tail" + 1	 */
	else
		headIndex++;
}

void Stack_voidIncrementTailIndex(void)
{
	/*
	 * if "tail" is end of the array, circularly, next to it would be first of
	 * the array.
	 */
	if (tailIndex == STACK_MAX_LEN - 1)
		tailIndex = 0;

	/*	otherwise, it is "tail" + 1	 */
	else
		tailIndex++;
}

u16 Stack_u16GetUsedLen(void)
{
	if (headIndex <= tailIndex)
		return tailIndex - headIndex;

	else
	{
		/*	len =
		 * 			from "head" to "max" 	+
		 * 			from "0" 	to "tail"
		 */
		return (STACK_MAX_LEN - 1 - headIndex) + (tailIndex + 1);
	}
}

b8 Stack_b8IsStackFull(void)
{
	if (Stack_u16GetUsedLen() == STACK_MAX_LEN)
		return true;

	else
		return false;
}


b8 Stack_b8Push(void* ptr)
{
	/*	check for full stack	*/
	if (Stack_b8IsStackFull())
		return false;

	/*	if stack is not full 	*/

	/*	copy argument data at new tail index	*/
	STACK_COPY_FUNCTION(
		(STACK_TYPE_PTR*)(&stack[tailIndex]), (STACK_TYPE_PTR)ptr);

	/*	increment "tail"	*/
	Stack_voidIncrementTailIndex();

	return true;
}

/*
 * takes pointer to pointer of the stack's data-type to the object
 * to be popped into, copies the first of stack to it,then deletes
 * the second.
 */
void Stack_ptrPop(void** ptrPtr)
{
	/*	if stack is empty	*/
	if (Stack_u16GetUsedLen() == 0)
		return;

	/*	copy head of the stack to the argument	*/
	STACK_COPY_FUNCTION((STACK_TYPE_PTR*)ptrPtr, (STACK_TYPE_PTR)stack[headIndex]);

	/*	free the head of the stack	*/
	STACK_FREE_FUNCTION((STACK_TYPE_PTR*)&stack[headIndex]);

	/*	increment stack's head	*/
	Stack_voidIncrementHeadIndex();
}

#endif
