/*
 * Stack_config.h
 *
 *  Created on: Sep 21, 2022
 *      Author: ali20
 */

#ifndef _STACK_CONFIG_H_
#define _STACK_CONFIG_H_

#define STACK_ENABLE	0


#if STACK_ENABLE
/*
 * max len of the stack.
 * exceeding this max len would overwrite the first of
 * the stack, and reset the stack's counter.
 * */
#include "CNC_config.h"
#define STACK_MAX_LEN			LOOK_AHEAD_STACK_LEN

/*
 * pointer to data-type to be stored in that stack.
 * (user may #include the file that contains this data-type here)
 */
#include "G_code_interface.h"
#define STACK_TYPE_PTR			G_Code_Msg_t*

/*
 * pointer to the function that copies one object of the
 * stack's data-type to another one, the second is to be
 * allocated in it. it should take a pointer to pointer to
 * the object to be copied into, and a pointer to the object
 * to be copied from, and return void.
 * allocation of the object to be copied into is the responsibility
 * of this function.
 */
#define STACK_COPY_FUNCTION		G_Code_voidCopyMsg

#define STACK_FREE_FUNCTION		G_Code_voidFreeMsg

#endif

#endif /* _STACK_CONFIG_H_ */
