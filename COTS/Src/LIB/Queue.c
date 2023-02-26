/*
 * Queue.c
 *
 *  Created on: Feb 26, 2023
 *      Author: ali20
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"

/*	SELF	*/
#include "Queue_Config.h"
#include "Queue_Interface.h"



/*******************************************************************************
 * Init:
 ******************************************************************************/
void Queue_voidInit(Queue_t* q)
{
	q->tailIndex = 0;
	q->headIndex = 0;
	q->count = 0;
}

void Queue_voidAssignAllocatedPointers(
	Queue_t* q, Queue_Data_t* allocatedPtrArr[QUEUE_MAX_LEN])
{
	for (u16 i = 0; i < QUEUE_MAX_LEN; i++)
	{
		q->ptrArr[i] = allocatedPtrArr[i];
	}
}

/*******************************************************************************
 * Info:
 ******************************************************************************/
u16 Queue_u16GetLenUsed(Queue_t* q)
{
	return q->count;
}

/*******************************************************************************
 * Index manipulation:
 ******************************************************************************/
void Queue_voidIncrementTailIndex(Queue_t* q)
{
	/*
	 * if "tailIndex" is end of the array, circularly, next to it would be first
	 * of the array.
	 */
	if (q->tailIndex == QUEUE_MAX_LEN - 1)
		q->tailIndex = 0;

	/*	otherwise, it is "tailIndex" + 1	 */
	else
		q->tailIndex++;
}

void Queue_voidIncrementHeadIndex(Queue_t* q)
{
	/*
	 * if "headIndex" is end of the array, circularly, next to it would be first
	 * of the array.
	 */
	if (q->headIndex == QUEUE_MAX_LEN - 1)
		q->headIndex = 0;

	/*	otherwise, it is "headIndex" + 1	 */
	else
		q->headIndex++;
}

/*******************************************************************************
 * Push:
 ******************************************************************************/
b8 Queue_b8Push(Queue_t* q, Queue_Data_t* newDataPtr)
{
	/*	check for full space	*/
	if (q->count == QUEUE_MAX_LEN)
		return false;

	/*	copy new data at "tailIndex"	*/
	Queue_Data_t** distPP = &(q->ptrArr[q->tailIndex]);

	QUEUE_COPY_ELEMENT(distPP, newDataPtr);

	/*	increment "tailIndex"	*/
	Queue_voidIncrementTailIndex(q);

	/*	increment used len	*/
	q->count++;

	/*	push successful	*/
	return true;
}

/*******************************************************************************
 * Pop:
 ******************************************************************************/
b8 Queue_b8Pop(Queue_t* q, Queue_Data_t** dataPP)
{
	/*	check if queue is empty	*/
	if (q->count == 0)
		return false;

	/*
	 * copy first of the queue (at "headIndex") to the argument pointer, using
	 * the user defined copy function.
	 */
	Queue_Data_t* srcPtr = q->ptrArr[q->headIndex];

	QUEUE_COPY_ELEMENT(dataPP, srcPtr);

	/*
	 * delete (free) head of the queue using the user defined delete function.
	 */
	QUEUE_FREE_ELEMENT(srcPtr);

	/*	increment "headIndex"	*/
	Queue_voidIncrementHeadIndex(q);

	/*	decrement used len	*/
	q->count--;

	/*	pop successful	*/
	return true;
}

///*******************************************************************************
// * Print:
// ******************************************************************************/
//void Queue_voidPrint(Queue_t* q)
//{
//	PRINTF("Printing Queue object @ %u\n", (u32)q);
//
//	u16 i = 0;
//
//	Queue_Data_t* dPtr;
//
//	while(Queue_b8Pop(q, &dPtr))
//	{
//		PRINTF("%u: ", i++);
//		QUEUE_PRINT_ELEMENT(dPtr);
//		PRINTF("\n");
//	}
//
//	PRINTF("=========================\n");
//}


















