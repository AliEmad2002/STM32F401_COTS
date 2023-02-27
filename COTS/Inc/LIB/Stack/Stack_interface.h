/*
 * Stack_interface.h
 *
 *  Created on: Sep 21, 2022
 *      Author: Ali Emad
 *
 * Notice: every "Stack_t" object has a fixed length array of pointers, each
 * of them is allocated and freed (dynamically and statically) using two user
 * defined functions. Which depend on type of the data.
 */

#ifndef _STACK_INTERFACE_H_
#define _STACK_INTERFACE_H_


#include "Stack_Config.h"

#if ENABLE_STACK

typedef struct{
	/*	array of pointers in which data would be allocated	*/
	Stack_Data_t* ptrArr[STACK_MAX_LEN];

	/*	index in the previous array of stack's tail (last element)	*/
	u16 tailIndex;
}Stack_t;

/*******************************************************************************
 * Init:
 ******************************************************************************/
/*	Resets "tailIndex" and "OVF" of a stack object	*/
void Stack_voidInit(Stack_t* s);

/*
 * assigns array of allocated pointers to the pointer array of a stack object.
 * (If user does not want to use dynamic allocation)
 */
void Stack_voidAssignAllocatedPointers(
	Stack_t* s, Stack_Data_t* allocatedPtrArr[STACK_MAX_LEN]);

/*******************************************************************************
 * Info:
 ******************************************************************************/
/*	returns used length of the stack object	*/
u16 Stack_u16GetLenUsed(Stack_t* s);

/*******************************************************************************
 * Push:
 ******************************************************************************/
/*
 * Pushes new data (by copy) to end of the stack.
 *
 * If successful, it returns "true".
 *
 * Otherwise (if stack is full), it returns "false".
 */
b8 Stack_b8Push(Stack_t* s, Stack_Data_t* newDataPtr);

/*******************************************************************************
 * Pop:
 ******************************************************************************/
/*
 * Pops data (by copy) from end of the stack to a passed pointer to pointer
 * (using the user defined copy function), (it is pointer to pointer, not just
 * pointer, to have the ability of using "malloc()" and editing value of the
 * pointer and its data, not only its data), and calls user defined delete
 * function on the popped pointer.
 *
 * If successful, it returns "true".
 *
 * Otherwise (if stack is empty), it returns "false".
 */
b8 Stack_b8Pop(Stack_t* s, Stack_Data_t** dataPP);

/*******************************************************************************
 * Print:
 ******************************************************************************/
void Stack_voidPrint(Stack_t* s);

#endif	/*	ENABLE_STACK	*/


#endif /* _STACK_INTERFACE_H_ */
