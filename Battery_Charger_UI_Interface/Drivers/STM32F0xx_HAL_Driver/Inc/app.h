/*
 * app.h
 *
 * Header file for the main application logic of an STM32F0-based system.
 * Defines application states, function prototypes, and necessary includes.
 *
 * Created on			:Feb 9, 2025
 * Author				:Aly Mustafa
 * @GitHub				:https://github.com/AlyMustafa
 */

#ifndef STM32F0XX_HAL_DRIVER_INC_APP_H_
#define STM32F0XX_HAL_DRIVER_INC_APP_H_

#include "gpio.h"
#include "lcd.h"
#include "flash.h"
#include <stdbool.h>

/*
 * Enumeration for button states.
 * Values correspond to button identifiers for UI navigation and actions.
 */
typedef enum {
    _1 = 1, _2, _3, _4, _5, _6, _7, _8, _9, _10,  // Numeric button inputs
    ENTER,   // Confirm selection
    SETUP,   // Enter setup mode
    UP,      // Scroll up
    DOWN     // Scroll down
} SW_State;

/*
 * Enumeration for application states.
 * Used to track and manage the UI flow.
 */
typedef enum {
    STATE_IDLE,   // Default state when idle
    STATE_SETUP,  // Configuration/setup mode
    STATE_MESSAGE // Display message state
} AppState;

/* Global variable to track the current application state */
extern AppState currentState;

//////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                                         //////////////////////////////
///////////////////////////////       Private function prototypes       //////////////////////////////
///////////////////////////////                                         //////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/* Utility function to determine the length of a string */
uint16_t getStringLength(const char* str);

/* Prints text along with a numerical value at the specified LCD position */
void PRINT_TXT_VAL(uint8_t row, uint8_t coloumn, uint8_t const text[], uint32_t val);

/* Clears a single screen on the LCD */
void LCD_Clear_Screen_single(void);

/* Displays the introductory screen */
void Intro_Scr(void);

/* Displays the home screen and returns user input */
uint8_t Home_Scr(void);

/* Handles user interface navigation */
uint8_t user_UI(void);

/* Detects button press state and debouncing */
GPIO_PinState button_press(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t state);

/* Handles button press events and transitions between states */
void handleButtonPress(uint8_t state);

/* Initializes EEPROM or non-volatile memory handling */
void EE_Init(void);

#endif /* STM32F0XX_HAL_DRIVER_INC_APP_H_ */
