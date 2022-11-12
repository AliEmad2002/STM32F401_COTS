/*
 * random_interface.h
 *
 *  Created on: Oct 8, 2022 1:46:05 PM
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_LIB_RANDOM_INTERFACE_H_
#define INCLUDE_LIB_RANDOM_INTERFACE_H_

/*
 * loads the previously stored initial number to "srand()"
 */
void Random_voidLoadInitialStart(void);

/*
 * changes the value of the number called in
 * "Random_voidLoadInitialStart()", to emulate
 * a user-acceptable randomization.
 */
void Random_voidRefreshNextStartInitialNumber(void);


#endif /* INCLUDE_LIB_RANDOM_INTERFACE_H_ */
