/*
 * Colors.h
 *
 *  Created on: Nov 25, 2022
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_LIB_IMG_COLORS_H_
#define INCLUDE_LIB_IMG_COLORS_H_


static const Color_t colorRed = {
	.map.blue = 0, .map.green = 0, .map.red = POW_TWO(5) - 1
};

static const Color_t colorBlack = {
	.map.blue = 0, .map.green = 0, .map.red = 0
};

static const Color_t colorWhite = {
	.map.blue = POW_TWO(5) - 1,
	.map.green = POW_TWO(6) - 1,
	.map.red = POW_TWO(5) - 1
};

static const u8 colorBlackU8Val = 0;
static const u8 colorGreenU8Val = 0xC;
static const u8 colorRedU8Val = 0xAA;



#endif /* INCLUDE_LIB_IMG_COLORS_H_ */
