/*
 * Menu_config.h
 *
 *  Created on: Jan 18, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_LIB_MENU_MENU_CONFIG_H_
#define INCLUDE_LIB_MENU_MENU_CONFIG_H_

/*	max length of the string that represents the menu element	*/
#define MENU_MAX_STR_LEN						25

/*	max number of menu elements in one menu	*/
#define MENU_MAX_NUMBER_OF_ELEMENTS				5

/*
 * function that enters a menu.
 * This function MUST be user defined, as menu entering vary.
 * For example this variation could be due to:
 *  - Switching using rotary encoder or push-buttons.
 *  - Displaying on TFT-LCD, or char-LCD (like 2x16-LCD).
 *
 * Note: this function returns void, and takes one argument that is a pinter to
 * a menu.
 * Example: void funcEnterMenu(void* menuPtr);
 */
/**	extern this function here:	**/
extern void OSC_voidOpenMenu();

/**	define that externed function so that menu_program.c can handle it	**/
#define MENU_ENTERING_FUNCTION			OSC_voidOpenMenu



#endif /* INCLUDE_LIB_MENU_MENU_CONFIG_H_ */
