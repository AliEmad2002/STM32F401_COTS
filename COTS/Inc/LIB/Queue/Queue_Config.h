/*
 * Queue_Config.h
 *
 *  Created on: Feb 26, 2023
 *      Author: Ali Emad
 *
 * Notice: every "Queue_t" object has a fixed length array of pointers, each
 * that are allocated and freed (dynamically and statically) using two user
 * defined functions. Which depend on type of the data.
 */

#ifndef INC_DATA_STRUCTURES_QUEUE_QUEUE_CONFIG_H_
#define INC_DATA_STRUCTURES_QUEUE_QUEUE_CONFIG_H_

/**
 * Maximum length of a queue object.
 *
 * (Notice that this number is not going to be allocated on "Queue_t" object
 * creation, an array of pointer only will. Then, each pointer could be allocated
 * and freed by need).
 **/
#include "CNC_config.h"
#define QUEUE_MAX_LEN			LOOK_AHEAD_STACK_LEN

/**
 * Data type of queue's element (same for all queue objects).
 *
 * (It can be variable per queue object, but this would consume space and time).
 **/
#include "G_code_interface.h"
#define QUEUE_DATA_TYPE			G_Code_Msg_t

typedef QUEUE_DATA_TYPE	Queue_Data_t;

/**
 * Function that copies data of queue's element.
 *
 * It takes pointers to pointer to destination data object (to be able to use
 * "malloc()" and edit pointer value, in case dynamic allocation used), and
 * pointer to source data object.
 *
 * If user uses dynamic allocation (and by extension did not use the function
 * "Queue_voidAssignAllocatedPointers()"), he / she must allocate destination
 * pointer in this function.
 *
 * Example: void copyFunc(Queue_Data_t* distPtr, Queue_Data_t* srcPtr);
 *
 * User may extern here.
 **/
#define QUEUE_COPY_ELEMENT		G_Code_voidCopyMsg

/**
 * Function that deletes element of the queue.
 *
 * It takes pointer to the data object to be deleted (freed).
 *
 * Example: void deleteFunc(Queue_Data_t* dPtr);
 * User may extern here.
 **/
#define QUEUE_FREE_ELEMENT		G_Code_voidFreeMsg

///**
// * Function that prints element of the queue.
// *
// * It takes pointer to the data object to be printed.
// *
// * Example: void printFunc(Queue_Data_t* dPtr);
// * User may extern here.
// **/
//extern void printData (s32* dPtr);
//
//#define QUEUE_PRINT_ELEMENT		printData

#endif /* INC_DATA_STRUCTURES_QUEUE_QUEUE_CONFIG_H_ */
