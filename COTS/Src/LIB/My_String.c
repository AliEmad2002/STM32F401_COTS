/*
 * My_String.c
 *
 *  Created on: Oct 23, 2022
 *      Author: Ali Emad Ali
 */


#include "Std_Types.h"
#include "Bit_Math.h"

#include "My_String.h"


/*
 * looks for a certain character in a string after certain start index, and
 * returns its index. if not found it returns -1
 */
s16 String_s16Find(char* str, char ch, s16 start)
{
	for (s16 i=start; str[i]!='\0'; i++)
		if (str[i] == ch)
			return i;

	return -1;
}

