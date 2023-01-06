/*
 * TFT_config.h
 *
 *  Created on: Oct 8, 2022
 *      Author: ali20
 */

#ifndef INCLUDE_HAL_TFT_TFT_CONFIG_H_
#define INCLUDE_HAL_TFT_TFT_CONFIG_H_


/*
 * un-commenting this option will make the TFT draw the background
 * then draw the rectangles.
 * takes less iterations, but causes frame shatter in some applications.
 */
#define BG_THEN_RECTANGLES

/*
 * un-commenting this option will make the TFT treat the background
 * as rectangles, and draw both in the same loop.
 * takes more iterations, but prevents frame shatter in some applications.
 */
//#define BG_AND_RECTANGLES


#endif /* INCLUDE_HAL_TFT_TFT_CONFIG_H_ */
