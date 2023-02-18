/*
 * Colors.h
 *
 *  Created on: Nov 25, 2022
 *      Author: Ali Emad Ali
 *
 * Reference:
 * 	https://github.com/newdigate/rgb565_colors
 */

#ifndef INCLUDE_LIB_IMG_COLORS_H_
#define INCLUDE_LIB_IMG_COLORS_H_

/*
 * Read this to understand why data had to be converted when using DMA:
 * "DMA sending color problem.pdf" in: STM32F401x_10x_COTS\COTS\Inc\LIB\Img
 */
#define f1(normal565Code)	((u16)(normal565Code) >> 8)
#define f2(normal565Code)	((u16)(normal565Code) << 8)
#define GET_DMA_CONVERTION(normal565Code)	\
	((u16)(f1(normal565Code) | f2(normal565Code)))

static const Color_t colorBlack = {.code565 = 0};

static const Color_t colorWhite = {.code565 = 0xFFFF};

static const Color_t colorRed = {.code565 = GET_DMA_CONVERTION(0xD105)};

static const Color_t colorYellow = {.code565 = GET_DMA_CONVERTION(0xFFA0)};

static const Color_t colorBlue = {.code565 = GET_DMA_CONVERTION(0x001F)};

static const Color_t colorGreen = {.code565 = GET_DMA_CONVERTION(0x67E0)};

static const Color_t colorViolet = {.code565 = GET_DMA_CONVERTION(0x897B)};

static const u8 colorBlackU8Val = 0;
static const u8 colorGreenU8Val = 0xC;
static const u8 colorRedU8Val = 0xAA;



#endif /* INCLUDE_LIB_IMG_COLORS_H_ */
