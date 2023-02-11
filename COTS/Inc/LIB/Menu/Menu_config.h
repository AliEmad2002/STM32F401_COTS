/*
 * Menu_config.h
 *
 *  Created on: Jan 18, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_LIB_MENU_MENU_CONFIG_H_
#define INCLUDE_LIB_MENU_MENU_CONFIG_H_

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
extern void OSC_voidOpenCheckList();

/**	define that externed function so that menu_program.c can handle it	**/
#define MENU_ENTERING_FUNCTION			OSC_voidOpenMenu

#define CHECK_LIST_ENTERING_FUNCTION	OSC_voidOpenCheckList


#endif /* INCLUDE_LIB_MENU_MENU_CONFIG_H_ */
