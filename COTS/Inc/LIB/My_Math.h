/*
 * MATH.h
 *
 * Created: 17/03/2022 07:57:46 م
 *  Author: Ali Emad
 */ 


#ifndef MATH_H_
#define MATH_H_

s16 MATH_u16Abs (s16 x) ;

u16 MATH_u16GetNearestIndex (u16 x, u16* arr, u8 arrLen) ;

u16 MATH_u16GetFirstSmallerIndex (u16 x, u16* arr, u8 arrLen) ;

u16 MATH_u16GetFirstLargerIndex (u16 x, u16* arr, u8 arrLen) ;

s16 MATH_s16FindSignOf (s32 n);

/*
* larger and smaller are required to be argumented
* in correct order, thus to minimize execution time
* of the function.
*
* ordering arguments is user's responsibility.
*/
void Math_voidFindSimplistForm(
	u32 larger, u32 smaller, u32* largerSimplestPtr, u32* smallerSimplestPtr);

/*
 * converts a sequence of characters stored in a string, from "start" to "end"
 * to float number and returns it.
 */
f32 Math_f32StrToFloat(char* str, s16 start, s16 end);

/*
 * checks if a certain section in a string is only digits, point, or negative
 * sign
 */
b8 Math_b8IsNumber(char* str, u16 start, u16 end);

/*	gets the smaller of two values	*/
#define GET_SMALLER(a, b)		((a < b) ? a : b)

/*	gets the larger of two values	*/
#define GET_LARGER(a, b)		((a > b) ? a : b)

/*	if (b < a)	==>>	a = b	*/
#define WRITE_IF_SMALLER(a, b)	\
{                               \
	if ((b) < (a))              \
		(a) = (b);              \
}

/*	if (b > a)	==>>	a = b	*/
#define WRITE_IF_LARGER(a, b)	\
{                               \
	if ((b) > (a))              \
		(a) = (b);              \
}

#define SORT_VALUES(new, smaller, larger)             \
{                                                     \
	if ((new) > (larger))                             \
	{                                                 \
		(larger) = (new);                             \
	}                                                 \
	                                                  \
	else if ((new) < (smaller))                       \
	{                                                 \
		(smaller) = (new);                            \
	}                                                 \
}


#endif /* MATH_H_ */
