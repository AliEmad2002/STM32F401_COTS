/*
 * LED_Matrix_program.c
 *
 *  Created on:	Sep 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */


#include <stdlib.h>
#include "Std_Types.h"
#include "Bit_Math.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "Shift_Register_interface.h"
#include "Delay_interface.h"

#include "LED_Matrix_interface.h"

/*
 * initializes an LED Matrix object.
 *
 * inputs:
 * -	"ledMatrixPtr":		pointer to the object to be initialized.
 * -	"_rows":				number of rows.
 * -	"_cols":				number of cols.
 * -	"fps":					number of frames per second.
 * -	"_activeType":			row an col levels required to lighten LED[row][col].
 * -	"rowPinArr":			array of row pins.
 * -	"colPinArr":			array of col pins.
 */
void LED_Matrix_voidInitParallel(
	LED_Matrix_Parallel_t* ledMatrixPtr, u8 _rows, u8 _cols, u16 fps,
	LED_Matrix_ActiveType_t _activeType, GPIO_Pin_t rowPinArr[], GPIO_Pin_t colPinArr[]
	)
{
	ledMatrixPtr->rows = _rows;
	ledMatrixPtr->cols = _cols;

	ledMatrixPtr->rowActiveLevel =
			(_activeType == LED_Matrix_ActiveType_ColHighRowLow) ?
				GPIO_OutputLevel_Low : GPIO_OutputLevel_High;

	ledMatrixPtr->rowIntervalTicks = RCC_u32GetBusClk(RCC_Bus_AHB) / fps / _rows;

	ledMatrixPtr->rowPinArr = malloc(_rows * sizeof(u8));
	ledMatrixPtr->rowPortArr = malloc(_rows * sizeof(GPIO_PortName_t));

	ledMatrixPtr->colPinArr = malloc(_cols * sizeof(u8));
	ledMatrixPtr->colPortArr = malloc(_cols * sizeof(GPIO_PortName_t));

	ledMatrixPtr->rowsImg = malloc(_rows * sizeof(u16));
	ledMatrixPtr->imgAllocated = true;

	for (u8 i=0; i<_rows; i++)
	{
		ledMatrixPtr->rowPortArr[i] = rowPinArr[i] / 16;
		ledMatrixPtr->rowPinArr[i] = rowPinArr[i] % 16;
		GPIO_voidSetPinGpoPushPull(ledMatrixPtr->rowPortArr[i], ledMatrixPtr->rowPinArr[i]);

		// initially turn off all LEDs:
		GPIO_voidSetPinOutputLevel(ledMatrixPtr->rowPortArr[i], ledMatrixPtr->rowPinArr[i], !(ledMatrixPtr->rowActiveLevel));
	}

	for (u8 i=0; i<_rows; i++)
	{
		ledMatrixPtr->colPortArr[i] = colPinArr[i] / 16;
		ledMatrixPtr->colPinArr[i] = colPinArr[i] % 16;
		GPIO_voidSetPinGpoPushPull(ledMatrixPtr->colPortArr[i], ledMatrixPtr->colPinArr[i]);
	}
}

/*
 * draws the required "row" of the stored image.
 *
 * user must be sure that "rowsImg" is currently allocated and accessible, "row" is valid.
 */
void LED_Matrix_voidDrawRowParallel(LED_Matrix_Parallel_t* ledMatrixPtr, u8 row)
{
	// enable row:
	GPIO_voidSetPinOutputLevel(
		ledMatrixPtr->rowPortArr[row],
		ledMatrixPtr->rowPinArr[row],
		ledMatrixPtr->rowActiveLevel
		);

	// lightening this row's LEDs according to the stored image:
	for (u8 col = 0; col < ledMatrixPtr->cols; col++)
	{
		GPIO_voidSetPinOutputLevel(
			ledMatrixPtr->colPortArr[col],
			ledMatrixPtr->colPinArr[col],
			ledMatrixPtr->rowActiveLevel ^ GET_BIT(ledMatrixPtr->rowsImg[row], col)
			);
	}
}

/*
 * disables the required "row" of the stored image.
 *
 * user must be sure that "row" is valid.
 */
void LED_Matrix_voidDisableRowParallel(LED_Matrix_Parallel_t* ledMatrixPtr, u8 row)
{
	GPIO_voidSetPinOutputLevel(
		ledMatrixPtr->rowPortArr[row],
		ledMatrixPtr->rowPinArr[row],
		!(ledMatrixPtr->rowActiveLevel)
		);

	// (optional) disable cols to avoid shade repentance in next row:
	for (u8 col = 0; col < ledMatrixPtr->cols; col++)
	{
		GPIO_voidSetPinOutputLevel(
			ledMatrixPtr->colPortArr[col],
			ledMatrixPtr->colPinArr[col],
			ledMatrixPtr->rowActiveLevel
			);
	}
}

/*
 * draws required number of frames of the stored "rowsImg" - blocking -.
 *
 * user must be sure that "rowsImg" is currently allocated and accessible.
 */
void LED_Matrix_voidDrawBlockingParallel(LED_Matrix_Parallel_t* ledMatrixPtr, u32 frames)
{
	while(frames--)
	{
		// for every row:
		for (u8 row = 0; row < ledMatrixPtr->rows; row++)
		{
			// enable row:
			LED_Matrix_voidDrawRowParallel(ledMatrixPtr, row);

			// wait multiplexing time:
			Delay_voidBlockingDelayTicks(ledMatrixPtr->rowIntervalTicks);

			// disable row:
			LED_Matrix_voidDisableRowParallel(ledMatrixPtr, row);
		}
	}
}

/*
 * stores - copies - an image to "rowsImg".
 *
 * it allocates "rowsImg" if it was not.
 *
 * user must be sure that "_rowsImg[]" length is at least equal to "rows".
 */
void LED_Matrix_voidStoreImgParallel(LED_Matrix_Parallel_t* ledMatrixPtr, u16 _rowsImg[])
{
	/*	allocate if not allocated	*/
	if (!ledMatrixPtr->imgAllocated)
	{
		ledMatrixPtr->rowsImg = malloc(ledMatrixPtr->rows * sizeof(u16));
		ledMatrixPtr->imgAllocated = true;
	}

	/*	copy	*/
	for (u8 row = 0; row < ledMatrixPtr->rows; row++)
		ledMatrixPtr->rowsImg[row] = _rowsImg[row];

	/*	clear counter	*/
	ledMatrixPtr->nextRow = 0;
}

/*
 * links - by reference - an image to "rowsImg".
 *
 * user must be sure that _"rowsImgPtr" is allocated and its length is at least equal to "rows".
 */
void LED_Matrix_voidLinkImgParallel(LED_Matrix_Parallel_t* ledMatrixPtr, u16* _rowsImgPtr)
{
	ledMatrixPtr->rowsImg = _rowsImgPtr;

	/*	clear counter	*/
	ledMatrixPtr->nextRow = 0;
}

/*
 * frees allocated "rowsImg" - if is -.
 */
void LED_Matrix_voidFreeImgParallel(LED_Matrix_Parallel_t* ledMatrixPtr)
{
	if (ledMatrixPtr->imgAllocated)
	{
		free(ledMatrixPtr->rowsImg);
		ledMatrixPtr->imgAllocated = false;
	}
}

/*
 * draws the next row in queue.
 *
 * user must be sure that "rowsImg" is allocated.
 *
 * used in RTOS.
 */
void LED_Matrix_voidDrawNextRowParallel(LED_Matrix_Parallel_t* ledMatrixPtr)
{
	/*	disable current row:	*/
	u8 currentRow =
		(ledMatrixPtr->nextRow == 0) ?
				ledMatrixPtr->rows - 1 : ledMatrixPtr->nextRow - 1;

	LED_Matrix_voidDisableRowParallel(ledMatrixPtr, currentRow);

	/*	draw next row:	*/
	LED_Matrix_voidDrawRowParallel(ledMatrixPtr, ledMatrixPtr->nextRow);

	/*	increment next row, check row OVF:	*/
	ledMatrixPtr->nextRow++;

	if (ledMatrixPtr->nextRow == ledMatrixPtr->rows)
		ledMatrixPtr->nextRow = 0;
}

/*
 * initializes an LED Matrix object.
 *
 * inputs:
 * -	"ledMatrixPtr":		pointer to the object to be initialized.
 * -	"_rows":				number of rows.
 * -	"_cols":				number of cols.
 * -	"_activeType":			row an col levels required to lighten LED[row][col].
 */
void LED_Matrix_voidInitSerial(
	LED_Matrix_Serial_t* ledMatrixPtr, u8 _rows, u8 _cols, u16 fps,
	LED_Matrix_ActiveType_t _activeType, SR_SerialInParallelOut_t* _shiftRegsiterPtr
	)
{
	ledMatrixPtr->rows = _rows;
	ledMatrixPtr->cols = _cols;

	ledMatrixPtr->rowActiveLevel =
		(_activeType == LED_Matrix_ActiveType_ColHighRowLow) ?
			GPIO_OutputLevel_Low : GPIO_OutputLevel_High;

	ledMatrixPtr->rowIntervalTicks = RCC_u32GetBusClk(RCC_Bus_AHB) / fps / _rows;

	ledMatrixPtr->rowsImg = malloc(_rows * sizeof(u16));
	ledMatrixPtr->imgAllocated = true;

	ledMatrixPtr->shiftRegsiterPtr = _shiftRegsiterPtr;

	ledMatrixPtr->nextRow = 0;

	// initially all LEDs are off:
	LED_Matrix_voidBlankSerial(ledMatrixPtr);
}

/*	turns off all LEDs	*/
void LED_Matrix_voidBlankSerial(LED_Matrix_Serial_t* ledMatrixPtr)
{
	/*	give all rows a state that is inverse to their active one	*/
	u8 level = !(ledMatrixPtr->rowActiveLevel);

	for (u8 row=0; row < ledMatrixPtr->rows; row++)
	{
		SR_voidShiftSerialInParallelOut(ledMatrixPtr->shiftRegsiterPtr, level);
	}

	/*	latch	*/
	SR_voidLatchSerialInParallelOut(ledMatrixPtr->shiftRegsiterPtr);

	/*	reset row counter	*/
	ledMatrixPtr->nextRow = 0;
}

/*
 * draws the required "row" of the stored image.
 *
 * user must be sure that "rowsImg" is currently allocated and accessible, "row" is valid.
 */
void LED_Matrix_voidDrawRowSerial(LED_Matrix_Serial_t* ledMatrixPtr, u8 row)
{
	/*	assuming that each of rows and cols won't be more than 16 in quantity	*/
	// enable row (disables all other rows):
	u32 shiftRegVal = (ledMatrixPtr->rowActiveLevel) << row;

	// lightening this row's LEDs according to the stored image:
	for (u8 col = 0; col < ledMatrixPtr->cols; col++)
	{
		u8 bit = ledMatrixPtr->rowActiveLevel ^ GET_BIT(ledMatrixPtr->rowsImg[row], col);
		shiftRegVal |= bit << (ledMatrixPtr->rows + col);
	}

	/*	load and latch	*/
	SR_voidLoadFromU32SerialInParallelOut(ledMatrixPtr->shiftRegsiterPtr, shiftRegVal, ledMatrixPtr->rows + ledMatrixPtr->cols);

	/*	latch	*/
	SR_voidLatchSerialInParallelOut(ledMatrixPtr->shiftRegsiterPtr);
}

/*
 * draws required number of frames of the stored "rowsImg" - blocking -.
 *
 * user must be sure that "rowsImg" is currently allocated and accessible.
 */
void LED_Matrix_voidDrawBlockingSerial(LED_Matrix_Serial_t* ledMatrixPtr, u32 frames)
{
	while(frames--)
	{
		// for every row:
		for (u8 row = 0; row < ledMatrixPtr->rows; row++)
		{
			// draw row:
			LED_Matrix_voidDrawRowSerial(ledMatrixPtr, row);

			// wait multiplexing time:
			Delay_voidBlockingDelayTicks(ledMatrixPtr->rowIntervalTicks);
		}
	}
}

/*
 * stores - copies - an image to "rowsImg".
 *
 * it allocates "rowsImg" if it was not.
 *
 * user must be sure that "_rowsImg[]" length is at least equal to "rows".
 */
void LED_Matrix_voidStoreImgSerial(LED_Matrix_Serial_t* ledMatrixPtr, u16 _rowsImg[])
{
	/*	allocate if not allocated	*/
	if (!ledMatrixPtr->imgAllocated)
	{
		ledMatrixPtr->rowsImg = malloc(ledMatrixPtr->rows * sizeof(u16));
		ledMatrixPtr->imgAllocated = true;
	}

	/*	copy	*/
	for (u8 row = 0; row < ledMatrixPtr->rows; row++)
		ledMatrixPtr->rowsImg[row] = _rowsImg[row];

	/*	clear counter	*/
	ledMatrixPtr->nextRow = 0;
}

void LED_Matrix_voidDrawNextRowSerial(LED_Matrix_Serial_t* ledMatrixPtr)
{
	/*	enable next row (disables all other row):	*/
	LED_Matrix_voidDrawRowSerial(ledMatrixPtr, ledMatrixPtr->nextRow);

	/*	increment next row, check row OVF:	*/
	ledMatrixPtr->nextRow++;

	if (ledMatrixPtr->nextRow == ledMatrixPtr->rows)
		ledMatrixPtr->nextRow = 0;
}


