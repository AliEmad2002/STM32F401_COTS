/*
 * LED_Matrix_interface.h
 *
 *  Created on:	Sep 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef _LED_MATRIX_INTERFACE_H_
#define _LED_MATRIX_INTERFACE_H_


typedef enum{
	LED_Matrix_ActiveType_ColHighRowLow,
	LED_Matrix_ActiveType_ColLowRowHigh
}LED_Matrix_ActiveType_t;

typedef struct{
	/*	number of rows in LED matrix	*/
	u8 rows;
	/*	number of cols in LED matrix	*/
	u8 cols;

	/*	the row level at which an LED will lighten	*/
	GPIO_OutputLevel_t rowActiveLevel;	//colActiveLevel is logically the inverse of that.

	/*	row pins' numbers	*/
	u8* rowPinArr;
	/*	row pins' ports	*/
	GPIO_PortName_t* rowPortArr;

	/*	col pins' numbers	*/
	u8* colPinArr;
	/*	col pins' ports	*/
	GPIO_PortName_t* colPortArr;

	/*	stored image, to be shown	*/
	u16* rowsImg;
	/*	is "rowsImg" currently allocated?	*/
	b8 imgAllocated;
	/*	number of the row to be shown at any "DrawNextRow" call	*/
	u16 nextRow;

	/*	ticks to be delayed before disabling a row and enablin the next	*/
	u64 rowIntervalTicks;
}LED_Matrix_Parallel_t;

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
	);

/*
 * draws the required "row" of the stored image.
 *
 * user must be sure that "rowsImg" is currently allocated and accessible, "row" is valid.
 */
void LED_Matrix_voidDrawRowParallel(LED_Matrix_Parallel_t* ledMatrixPtr, u8 row);

/*
 * disables the required "row" of the stored image.
 *
 * user must be sure that "row" is valid.
 */
void LED_Matrix_voidDisableRowParallel(LED_Matrix_Parallel_t* ledMatrixPtr, u8 row);

/*
 * draws required number of frames of the stored "rowsImg" - blocking -.
 *
 * user must be sure that "rowsImg" is currently allocated and accessible.
 */
void LED_Matrix_voidDrawBlockingParallel(LED_Matrix_Parallel_t* ledMatrixPtr, u32 frames);

/*
 * stores - copies - an image to "rowsImg".
 *
 * it allocates "rowsImg" if it was not.
 *
 * user must be sure that "_rowsImg[]" length is at least equal to "rows".
 */
void LED_Matrix_voidStoreImgParallel(LED_Matrix_Parallel_t* ledMatrixPtr, u16 _rowsImg[]);

/*
 * links - by reference - an image to "rowsImg".
 *
 * user must be sure that _"rowsImgPtr" is allocated and its length is at least equal to "rows".
 */
void LED_Matrix_voidLinkImgParallel(LED_Matrix_Parallel_t* ledMatrixPtr, u16* _rowsImgPtr);

/*
 * frees allocated "rowsImg" - if is -.
 */
void LED_Matrix_voidFreeImgParallel(LED_Matrix_Parallel_t* ledMatrixPtr);

/*
 * draws the next row in queue.
 *
 * user must be sure that "rowsImg" is allocated.
 *
 * used in RTOS.
 */
void LED_Matrix_voidDrawNextRowParallel(LED_Matrix_Parallel_t* ledMatrixPtr);
/*	*****************************************************************	*/
/*	*****************************************************************	*/

/*
 * same as the previous type, except that it uses a (rows+cols) shift register)
 *
 * first (rows) pins of the shift register are connected to row pins from 0,
 * and the following pins are connected to the col pins from 0.
 */
typedef struct{
	/*	number of rows in LED matrix	*/
	u8 rows;
	/*	number of cols in LED matrix	*/
	u8 cols;

	/*	the row level at which an LED will lighten	*/
	GPIO_OutputLevel_t rowActiveLevel;	//colActiveLevel is logically the inverse of that.

	/*	SIPO shift register connected to this LED matrix	*/
	SR_SerialInParallelOut_t* shiftRegsiterPtr;

	/*	stored image, to be shown	*/
	u16* rowsImg;
	/*	is "rowsImg" currently allocated?	*/
	b8 imgAllocated;
	/*	next row to be drawn	*/
	u8 nextRow;

	/*	ticks to be delayed before disabling a row and enablin the next	*/
	u64 rowIntervalTicks;
}LED_Matrix_Serial_t;

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
	);

/*	turns off all LEDs	*/
void LED_Matrix_voidBlankSerial(LED_Matrix_Serial_t* ledMatrixPtr);

/*
 * draws the required "row" of the stored image.
 *
 * user must be sure that "rowsImg" is currently allocated and accessible, "row" is valid.
 */
void LED_Matrix_voidDrawRowSerial(LED_Matrix_Serial_t* ledMatrixPtr, u8 row);

/*
 * draws required number of frames of the stored "rowsImg" - blocking -.
 *
 * user must be sure that "rowsImg" is currently allocated and accessible.
 */
void LED_Matrix_voidDrawBlockingSerial(LED_Matrix_Serial_t* ledMatrixPtr, u32 frames);

/*
 * stores - copies - an image to "rowsImg".
 *
 * it allocates "rowsImg" if it was not.
 *
 * user must be sure that "_rowsImg[]" length is at least equal to "rows".
 */
void LED_Matrix_voidStoreImgSerial(LED_Matrix_Serial_t* ledMatrixPtr, u16 _rowsImg[]);

/*
 * links - by reference - an image to "rowsImg".
 *
 * user must be sure that _"rowsImgPtr" is allocated and its length is at least equal to "rows".
 */
void LED_Matrix_voidLinkImgSerial(LED_Matrix_Serial_t* ledMatrixPtr, u16* _rowsImgPtr);

/*
 * frees allocated "rowsImg" - if is -.
 */
void LED_Matrix_voidFreeImgSerial(LED_Matrix_Serial_t* ledMatrixPtr);

void LED_Matrix_voidDrawNextRowSerial(LED_Matrix_Serial_t* ledMatrixPtr);

#endif /* _LED_MATRIX_INTERFACE_H_ */
