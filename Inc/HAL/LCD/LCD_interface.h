/*
 * LCD_interface.h
 *
 *  Created on:	Jul 28, 2022
 *  Author:		Ali Emad Ali
 *
 *  References:
 *  	XIAMEN AMOTEC DISPLAY LCD MODULE DATASHEET: https://www.sparkfun.com/datasheets/LCD/ADM1602K-NSW-FBS-3.3v.pdf
 *  	https://circuitdigest.com/article/16x2-lcd-display-module-pinout-datasheet
 */


#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_

typedef enum{
	LCD_MODE_8bits	= 0,
	LCD_MODE_4bits	= 1
}LCD_MODE;
	
typedef enum{
	LCD_CURSORMODE_cursorOffBlinkOff	= 0x0C,
	LCD_CURSORMODE_cursorOffBlinkOn		= 0x0D,
	LCD_CURSORMODE_cursorOnBlinkOff		= 0x0E,
	LCD_CURSORMODE_cursorOnBlinkOn		= 0x0F
}HLCD_CURSORMODE;

typedef struct{
	u8 dataPinArr[8];		DIO_PORT dataPortArr[8];
	u8 rsPin;				DIO_PORT rsPort;
	u8 rwPin;				DIO_PORT rwPort;
	u8 enPin;				DIO_PORT enPort;
	LCD_MODE mode;
}LCD_t;

void LCD_voidInit(
	LCD_t* lcdPtr,				LCD_MODE _mode,
	DIO_PIN _dataPinArr[],		DIO_PIN _rsPin,
	DIO_PIN _rwPin,				DIO_PIN _enPin,
	HLCD_CURSORMODE cursorMode,	DIO_OUTPUTSPEED pinOutputSpeed
	);

void LCD_voidSetCursorMode(LCD_t* lcdPtr, HLCD_CURSORMODE cursorMode);

void LCD_voidClearDisplay(LCD_t* lcdPtr);

void LCD_voidSendCMD(LCD_t* lcdPtr, u8 cmd);

void LCD_voidSendData(LCD_t* lcdPtr, u8 data);

void LCD_voidWriteBusAndLatch(LCD_t* lcdPtr, u8 busVal);

void LCD_voidLatch(LCD_t* lcdPtr);

void LCD_voidPrintString(LCD_t* lcdPtr, char* str);

#endif /* LCD_INTERFACE_H_ */
