/*
 * RGB_LED_program.c
 *
 *  Created on: Oct 22, 2022
 *      Author: Ali Emad Ali
 */


/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"

/*	MCAL	*/
#include "GPIO_interface.h"

/*	SELF	*/
#include "RGB_LED_interface.h"

void RGB_LED_voidInit(
	RGB_LED_t* ledPtr, GPIO_Pin_t redPin,
	GPIO_Pin_t greenPin, GPIO_Pin_t bluePin
	)
{
	/*	calculate, store and init pins	*/
	ledPtr->pinArr[0] = redPin % 16;
	ledPtr->pinArr[1] = greenPin % 16;
	ledPtr->pinArr[2] = bluePin % 16;

	ledPtr->portArr[0] = redPin / 16;
	ledPtr->portArr[1] = greenPin / 16;
	ledPtr->portArr[2] = bluePin / 16;

	for (u8 i = 0; i < 3; i++)
	{
		GPIO_voidSetPinMode(ledPtr->portArr[i], ledPtr->pinArr[i], GPIO_Mode_GPO_PushPull);
		GPIO_voidSetPinOutputSpeed(ledPtr->portArr[i], ledPtr->pinArr[i], GPIO_OutputSpeed_50MHz);
		GPIO_voidSetPinOutputLevel(ledPtr->portArr[i], ledPtr->pinArr[i], GPIO_OutputLevel_Low);
	}
}























