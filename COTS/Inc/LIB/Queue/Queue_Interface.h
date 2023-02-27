/*
 * Queue_Interface.h
 *
 *  Created on: Feb 26, 2023
 *      Author: Ali Emad
 *
 * Notice: every "Queue_t" object has a fixed length array of pointers, each
 * of them is allocated and freed (dynamically and statically) using two user
 * defined functions. Which depend on type of the data.
 */

#ifndef INC_DATA_STRUCTURES_QUEUE_QUEUE_INTERFACE_H_
#define INC_DATA_STRUCTURES_QUEUE_QUEUE_INTERFACE_H_

#include "Queue_Config.h"

#if ENABLE_QUEUE

typedef struct{
	/*	array of pointers in which data would be allocated	*/
	Queue_Data_t* ptrArr[QUEUE_MAX_LEN];

	/*	index in the previous array of queue's tail (last element)	*/
	u16 tailIndex;

	/*	index in the previous array of queue's head (first element)	*/
	u16 headIndex;

	/*	number of elements currently in queue	*/
	u16 count;
}Queue_t;

/*******************************************************************************
 * Init:
 ******************************************************************************/
/*	Resets "tailIndex", "headIndex" and "count" of a queue object	*/
void Queue_voidInit(Queue_t* q);

/*
 * assigns array of allocated pointers to the pointer array of a queue object.
 * (If user does not want to use dynamic allocation)
 */
void Queue_voidAssignAllocatedPointers(
	Queue_t* q, Queue_Data_t* allocatedPtrArr[QUEUE_MAX_LEN]);

/*******************************************************************************
 * Info:
 ******************************************************************************/
/*	returns used length of the queue object	*/
u16 Queue_u16GetLenUsed(Queue_t* q);

/*******************************************************************************
 * Push:
 ******************************************************************************/
/*
 * Pushes new data (by copy) to end of the queue.
 *
 * If successful, it returns "true".
 *
 * Otherwise (if queue is full), it returns "false".
 */
b8 Queue_b8Push(Queue_t* q, Queue_Data_t* newDataPtr);

/*******************************************************************************
 * Pop:
 ******************************************************************************/
/*
 * Pops data (by copy) from first of the queue to a passed pointer to pointer
 * (using the user defined copy function), (it is pointer to pointer, not just
 * pointer, to have the ability of using "malloc()" and editing value of the
 * pointer and its data, not only its data), and calls user defined delete
 * function on the popped pointer.
 *
 * If successful, it returns "true".
 *
 * Otherwise (if queue is empty), it returns "false".
 */
b8 Queue_b8Pop(Queue_t* q, Queue_Data_t** dataPP);

///*******************************************************************************
// * Print:
// ******************************************************************************/
//void Queue_voidPrint(Queue_t* q);

#endif	/*	ENABLE_QUEUE	*/

#endif /* INC_DATA_STRUCTURES_QUEUE_QUEUE_INTERFACE_H_ */










