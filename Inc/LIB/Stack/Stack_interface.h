/*
 * Stack_interface.h
 *
 *  Created on: Sep 21, 2022
 *      Author: ali20
 */

#ifndef _STACK_INTERFACE_H_
#define _STACK_INTERFACE_H_

#if STACK_ENABLE

void Stack_voidInit(void);

/*
 * takes ptr of the stack's data-type, copies it at the
 * current count location of the stack.
 */
void Stack_voidPush(void* ptr);

/*
 * takes ptr of the stack's data-type, copies the first of
 * stack to it, then deletes the second.
 */
void Stack_ptrPop(void** ptrPtr);

u16 Stack_u16GetCount(void);

#endif

#endif /* _STACK_INTERFACE_H_ */
