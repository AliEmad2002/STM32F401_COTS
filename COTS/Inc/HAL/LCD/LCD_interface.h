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
	LCD_Mode_8bits	= 0,
	LCD_Mode_4bits	= 1
}LCD_Mode_t;
	
typedef enum{
	HLCD_CursorMode_CursorOffBlinkOff	= 0x0C,
	HLCD_CursorMode_CursorOffBlinkOn	= 0x0D,
	HLCD_CursorMode_CursorOnBlinkOff	= 0x0E,
	HLCD_CursorMode_CursorOnBlinkOn		= 0x0F
}HLCD_CursorMode_t;

typedef struct{
	u8 dataPinArr[8];		GPIO_PortName_t dataPortArr[8];
	u8 rsPin;				GPIO_PortName_t rsPort;
	u8 rwPin;				GPIO_PortName_t rwPort;
	u8 enPin;				GPIO_PortName_t enPort;
	LCD_Mode_t mode;
	u8 count;
}LCD_t;

void LCD_voidInit(
	LCD_t* lcdPtr,					LCD_Mode_t _mode,
	GPIO_Pin_t _dataPinArr[],		GPIO_Pin_t _rsPin,
	GPIO_Pin_t _rwPin,				GPIO_Pin_t _enPin,
	HLCD_CursorMode_t cursorMode
	);

void LCD_voidSetCursorMode(LCD_t* lcdPtr, HLCD_CursorMode_t cursorMode);

void LCD_voidClearDisplay(LCD_t* lcdPtr);

void LCD_voidSendCMD(LCD_t* lcdPtr, u8 cmd);

void LCD_voidSendData(LCD_t* lcdPtr, u8 data);

void LCD_voidWriteBusAndLatch(LCD_t* lcdPtr, u8 busVal);

void LCD_voidLatch(LCD_t* lcdPtr);

void LCD_voidPrintString(LCD_t* lcdPtr, char* str);

void LCD_voidPrintCharAt(LCD_t* lcdPtr, char ch, u8 row, u8 col);

void LCD_voidWaitForCurrentInstruction(LCD_t* lcdPtr);

void LCD_voidLoadCustomChar (LCD_t* lcdPtr, u8 loc, u8 cChar[]);

#endif /* LCD_INTERFACE_H_ */
