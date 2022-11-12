
/*
 * MATH.c
 *
 * Created: 17/03/2022 07:57:29 م
 *  Author: Ali Emad
 */ 

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include <math.h>

/*	SELF	*/
#include "My_Math.h"



s16 MATH_u16Abs (s16 x)
{
	return (x>0) ? x : -x ;
}

u16 MATH_u16GetNearestIndex (u16 x, u16* arr, u8 arrLen)
{
	// takes a value x, an array of values, and returns the index of the nearest value to x in that array.
	u16 deltaMin = 65535 ;
	u8 nearestIndex = 0 ;
	for (u8 i=0; i<arrLen; i++)
	{
		u16 delta = MATH_u16Abs(x - arr[i]) ;
		if (delta < deltaMin)
		{
			deltaMin = delta ;
			nearestIndex = i ;
		}
	}
	return nearestIndex ;
}

u16 MATH_u16GetFirstSmallerIndex (u16 x, u16* arr, u8 arrLen) 
{
	// takes a value x, an array of values, and returns the index of the first value smaller than x in that array.
	
	u16 firstSmaller = 0 ;
	u8 firstSmallerIndex = 0 ;
	for (u8 i=0; i<arrLen; i++)
	{
		if (arr[i] < x  &&  firstSmaller > arr[i])
		{
			firstSmaller = arr[i] ;
			firstSmallerIndex = i ;
		}
	}
	return firstSmallerIndex ;
}

u16 MATH_u16GetFirstLargerIndex (u16 x, u16* arr, u8 arrLen)
{
	// takes a value x, an array of values, and returns the index of the first value larger than x in that array.
	
	u16 firstLarger = 65535 ;
	u8 firstLargerIndex = 0 ;
	for (u8 i=0; i<arrLen; i++)
	{
		if (arr[i] > x  &&  arr[i] < firstLarger)
		{
			firstLarger = arr[i] ;
			firstLargerIndex = i ;
		}
	}
	return firstLargerIndex ;
}

s16 MATH_s16FindSignOf (s32 n)
{
	if (n > 0)
		return 1;
	return -1;
}

/*
* larger and smaller are required to be argumented
* in correct order, thus to minimize execution time
* of the function.
*
* ordering arguments is user's responsibility.
*/
void Math_voidFindSimplistForm(u32 larger, u32 smaller, u32* largerSimplestPtr, u32* smallerSimplestPtr)
{
	u32 i = 2;

	*largerSimplestPtr = larger;
	*smallerSimplestPtr = smaller;

	while (1)
	{
		/*	if both larger and smaller are divisible by i	*/
		if ((*largerSimplestPtr % i == 0) && (*smallerSimplestPtr % i == 0))
		{
			*largerSimplestPtr /= i;
			*smallerSimplestPtr /= i;
			i = 2;
			continue;
		}

		if (i > smaller)
			break;

		i++;
	}
}

/*
 * converts a sequence of characters stored in a string, from "start" to "end"
 * to float number and returns it.
 */
f32 Math_f32StrToFloat(char* str, s16 start, s16 end)
{
	f32 n = 0.0f;

	u8 negative = 0;
	if (str[start] == '-')
	{
		negative = 1;
		start++;
	}

	u8 point = 0;
	s16 pointIndex;
	for (pointIndex = start; pointIndex <= end; pointIndex++)
	{
		if (str[pointIndex] == '.')
		{
			point = 1;
			break;
		}
		else if (str[pointIndex] == 'e')
			break;
	}

	u8 e = 0;
	s16 eIndex;
	for (eIndex = end - 1; eIndex > start; eIndex--)
		if (str[eIndex] == 'e')
		{
			e = 1;
			break;
		}
	if (!e)
		eIndex = end + 1;

	f32 decadeCounter = 1.0;
	for (s16 i = pointIndex - 1; i >= start; i--)
	{
		n += decadeCounter * (f32)(str[i] - '0');
		decadeCounter *= 10.0;
	}

	if (point)
	{
		f32 fDecadeCounter = 0.1;
		for (s16 i = pointIndex + 1; i <= eIndex - 1; i++)
		{
			n += fDecadeCounter * (f32)(str[i] - '0');
			fDecadeCounter /= 10.0;
		}
	}

	if (e)
	{
		f32 power = Math_f32StrToFloat(str, eIndex + 1, end);
		n *= (f32)pow(10.0, power);
	}

	return n * (negative ? -1.0 : 1.0);
}

/*
 * checks if a certain section in a string is only digits, point, or negative
 * sign
 */
b8 Math_b8IsNumber(char* str, u16 start, u16 end)
{
	for (u16 i = start; i <= end; i++)
	{
		if ((str[i] >= '0' && str[i] <= '9') || str[i] == '.' || str[i] == '-')
			return true;
		else
			return false;
	}
	return false;
}








