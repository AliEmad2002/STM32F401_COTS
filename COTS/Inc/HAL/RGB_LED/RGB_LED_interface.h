/*
 * RGB_LED_interface.h
 *
 *  Created on: Oct 22, 2022
 *      Author: Ali Emad Ali
 */

#ifndef RGB_LED_INTERFACE_H_
#define RGB_LED_INTERFACE_H_


/*	this driver uses internal GPT	*/
typedef struct{
	GPIO_PortName_t portArr[3];
	u8 pinArr[3];
}RGB_LED_t;

void RGB_LED_voidInit(
	RGB_LED_t* ledPtr, GPIO_Pin_t redPin,
	GPIO_Pin_t greenPin, GPIO_Pin_t bluePin
	);


#endif /* RGB_LED_INTERFACE_H_ */
