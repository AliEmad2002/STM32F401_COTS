/*
 * Stack_interface.h
 *
 *  Created on: Sep 21, 2022
 *      Author: ali20
 */

#ifndef _STACK_INTERFACE_H_
#define _STACK_INTERFACE_H_


void Stack_voidInit(void);

u16 Stack_u16GetUsedLen(void);

b8 Stack_b8IsStackFull(void);

/*
 * This function:
 *
 * 	-	Takes ptr of the stack's data-type.
 *
 * 	-	To avoid OVF, If there was  enough space in stack, it copies the new
 * 		data to "tail" of the stack. And returns "true".
 *
 * 	-	Otherwise, it returns "false".
 */
b8 Stack_b8Push(void* ptr);

/*
 * takes ptr of the stack's data-type, copies the first of
 * stack to it, then deletes the second.
 */
void Stack_ptrPop(void** ptrPtr);


#endif /* _STACK_INTERFACE_H_ */
