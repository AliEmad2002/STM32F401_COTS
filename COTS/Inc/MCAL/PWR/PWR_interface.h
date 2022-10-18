/*
 * PWR_interface.h
 *
 *  Created on: Oct 8, 2022 1:24:01 PM
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MCAL_PWR_PWR_INTERFACE_H_
#define INCLUDE_MCAL_PWR_PWR_INTERFACE_H_

/*
 * enables backup domain protection
 * against parasitic write in reset cases.
 */
void PWR_voidEnableDBP(void);

/*
 * disables backup domain protection,
 * giving user the ability to write on RTC
 * and backup registers.
 */
void PWR_voidDisableDBP(void);



#endif /* INCLUDE_MCAL_PWR_PWR_INTERFACE_H_ */
