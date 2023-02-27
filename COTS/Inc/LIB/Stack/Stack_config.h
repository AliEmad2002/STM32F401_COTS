/*
 * Stack_config.h
 *
 *  Created on: Sep 21, 2022
 *      Author: Ali Emad
 *
 * Notice: every "Stack_t" object has a fixed length array of pointers, each
 * of them is allocated and freed (dynamically and statically) using two user
 * defined functions. Which depend on type of the data.
 */

#ifndef _STACK_CONFIG_H_
#define _STACK_CONFIG_H_

#define ENABLE_STACK	0


#if ENABLE_STACK

/**
 * Maximum length of a stack object.
 *
 * (Notice that this number is not going to be allocated on "Stack_t" object
 * creation, an array of pointer only will. Then, each pointer could be allocated
 * and freed by need).
 **/
#define STACK_MAX_LEN			64

/**
 * Data type of stack's element (same for all stack objects).
 *
 * (It can be variable per stack object, but this would consume space and time).
 **/
#define STACK_DATA_TYPE			s32

typedef STACK_DATA_TYPE	Stack_Data_t;

/**
 * Function that copies data of stack's element.
 *
 * It takes pointers to pointer to destination data object (to be able to use
 * "malloc()" and edit pointer value, in case dynamic allocation used), and
 * pointer to source data object.
 *
 * If user uses dynamic allocation (and by extension did not use the function
 * "Statck_voidAssignAllocatedPointers()"), he / she must allocate destination
 * pointer in this function.
 *
 * Example: void copyFunc(Stack_Data_t* distPtr, Stack_Data_t* srcPtr);
 *
 * User may extern here.
 **/
extern void copyData (s32** distPP, const s32* srcP);

#define STACK_COPY_ELEMENT		copyData

/**
 * Function that deletes element of the stack.
 *
 * It takes pointer to the data object to be deleted (freed).
 *
 * Example: void deleteFunc(Stack_Data_t* dPtr);
 * User may extern here.
 **/
extern void freeData (s32* dataPtr);

#define STACK_FREE_ELEMENT		freeData

///**
// * Function that prints element of the stack.
// *
// * It takes pointer to the data object to be printed.
// *
// * Example: void printFunc(Stack_Data_t* dPtr);
// * User may extern here.
// **/
//extern void printData (s32* dPtr);
//
//#define STACK_PRINT_ELEMENT		printData

#endif	/*	ENABLE_STACK	*/

#endif /* _STACK_CONFIG_H_ */
