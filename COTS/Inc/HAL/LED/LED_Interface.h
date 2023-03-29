/*
 * LED_Interface.h
 *
 *  Created on: Mar 24, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_HAL_LED_LED_INTERFACE_H_
#define INCLUDE_HAL_LED_LED_INTERFACE_H_


typedef struct{
	GPIO_PortName_t port;
	u8 pin;
	GPIO_OutputLevel_t activeLevel;
}LED_t;


/*******************************************************************************
 * Init: (clock of the GPIO port must be initially enabled)
 ******************************************************************************/
void LED_voidInit(
	LED_t* ledPtr, GPIO_PortName_t port, u8 pin, GPIO_OutputLevel_t activeLevel);

/*******************************************************************************
 * Operation:
 ******************************************************************************/
void LED_voidSetActive(LED_t* ledPtr);

void LED_voidSetInactive(LED_t* ledPtr);

void LED_voidToggle(LED_t* ledPtr);

#endif /* INCLUDE_HAL_LED_LED_INTERFACE_H_ */
