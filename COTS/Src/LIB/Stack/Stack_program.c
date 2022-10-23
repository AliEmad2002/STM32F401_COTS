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

/*	stack's current count	*/
static u16 stackCount;

void Stack_voidInit(void)
{
	stack = (STACK_TYPE_PTR*)malloc(STACK_MAX_LEN * sizeof(STACK_TYPE_PTR));
	stackCount = 0;
}

/*
 * takes ptr of the stack's data-type, copies it at the
 * current count location of the stack.
 */
void Stack_voidPush(void* ptr)
{
	/*	copy argument to the stack	*/
	STACK_COPY_FUNCTION(
		(STACK_TYPE_PTR*)(stack + stackCount++), (STACK_TYPE_PTR)ptr);

	/*	check max len	*/
	if (stackCount == STACK_MAX_LEN)
		stackCount = 0;
}

/*
 * takes pointer to pointer of the stack's data-type to the object
 * to be popped into, copies the first of stack to it,then deletes
 * the second.
 */
void Stack_ptrPop(void** ptrPtr)
{
	/*	if stack is empty	*/
	if (stackCount == 0)
		return;

	/*	copy first of the stack to the argument	*/
	STACK_COPY_FUNCTION((STACK_TYPE_PTR*)ptrPtr, (STACK_TYPE_PTR)stack[0]);

	/*	free the first of the stack	*/
	STACK_FREE_FUNCTION((STACK_TYPE_PTR*)stack);

	/*	decrement stack count	*/
	stackCount--;

	/*	shift rest of the stack up by one	*/
	for (u16 i=0; i<stackCount; i++)
		stack[i] = stack[i+1];
}

u16 Stack_u16GetCount(void)
{
	return stackCount;
}

#endif
