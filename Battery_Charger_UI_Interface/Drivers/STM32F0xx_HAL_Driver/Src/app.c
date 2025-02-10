/*
 * app.c
 *
 * Created on			:Feb 9, 2025
 * Author				:Aly Mustafa
 * @GitHub				:https://github.com/AlyMustafa
 *
 * Description: This file manages user interaction with an embedded system, including UI handling,
 * flash EEPROM emulation for storing data, button press handling, and screen display updates.
 * The system handles values for voltage, current, power, energy, and other parameters in an interactive
 * menu-based UI.
 *
 * Dependencies:
 *   - LCD display driver functions (LCD_set_cursor, LCD_send_string, LCD_send_Num, etc.)
 *   - Flash EEPROM Emulation functions (FEE_Init, FEE_Write, FEE_Read)
 *   - HAL library for GPIO and delay functions (HAL_GPIO_ReadPin, HAL_Delay, etc.)
 */
#include "app.h"

#define NUM_ITEMS 20
#define NUM_VISIBLE_ROWS 4
//DELAYS
#define DEBOUNCE_DELAY 50
#define TIMEOUT_VALUE 2500  // Timeout value in milliseconds
// List of variable names
const uint8_t *var[NUM_ITEMS] = {
		"1.Voltage", "2.Current", "3.ALPHA  ", "4.BETA  ",
		"5.POWER  ", "6.ENERGY ", "7.Vin_A  ", "8.Vin_B ",
		"9.Vin_C  ", "10.Iin_A ", "11.Iin_B ", "12.Iin_C",
		"13.THD_A ", "14.THD_B ", "15.THD_C ", "16.R1   ",
		"17.R2    ", "18.R3    ", "19.R4    ", "20.R5   "
};

// Array to store the values of each variable
uint32_t val[NUM_ITEMS];
uint8_t NUM_S[6] = "";
uint8_t NUM_count = 0;

uint8_t scroll = 0; // Tracks the starting point for the visible items
uint8_t cursorRow = 0; // Tracks the current cursor position (0 to 3, corresponding to Row 1 to Row 4)

uint8_t massege[] = "Value?: _____";
//Page for writing
uint32_t activePage;
AppState currentState = STATE_IDLE;  // ✅ Defined in ONE .c file

//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////                   FLAGS             			//////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct{
	uint8_t sw_flags[10];  // Array for 10 button flags
	bool CNT_flag:1;
	bool UP_flag:1;
	bool DOWN_flag:1;
	bool SET_flag:1;
	bool ENT_flag:1;
	bool Scr_flag:1;
	bool HomeScr_flag:1;
}FLAGS;
FLAGS flags= {0};

//////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                                         //////////////////////////////
///////////////////////////////       Private function Definitions      //////////////////////////////
///////////////////////////////                                         //////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Manages the user interface, handling button presses to input values and confirm them.
 *
 * This function manages the display of input fields, button press handling, and value entry
 * with a timeout feature to reset the process if no action is taken within a specified time.
 *
 * @return 1 if user interaction is valid; 0 if an error or timeout occurs.
 */
uint8_t user_UI(void) {
	if (NUM_count == 0) {
		LCD_set_cursor(0, 0);
		LCD_send_string(var[scroll + cursorRow]);
		LCD_set_cursor(2, 0);
		LCD_send_string("Value?: _____");
		LCD_set_cursor(2, getStringLength("Value?: _____"));
	}
	if (NUM_count < 5) {
		// Handle button presses to enter value
		button_press(SW_01_GPIO_Port,SW_01_Pin,_1 );
		button_press(SW_02_GPIO_Port,SW_02_Pin,_2 );
		button_press(SW_03_GPIO_Port,SW_03_Pin,_3 );
		button_press(SW_04_GPIO_Port,SW_04_Pin,_4 );
		button_press(SW_05_GPIO_Port,SW_05_Pin,_5 );
		button_press(SW_06_GPIO_Port,SW_06_Pin,_6 );
		button_press(SW_07_GPIO_Port,SW_07_Pin,_7 );
		button_press(SW_08_GPIO_Port,SW_08_Pin,_8 );
		button_press(SW_09_GPIO_Port,SW_09_Pin,_9 );
		button_press(SW_10_GPIO_Port,SW_10_Pin,_10);
	} else if (NUM_count == 5) {
		if (!flags.CNT_flag) {
			HAL_Delay(100);
			LCD_Clear_Screen();
			LCD_send_Num(atoi(NUM_S)); // Convert string to integer
			LCD_send_string(" is correct?");
			LCD_set_cursor(2, 0);
			LCD_send_string("ENTER FOR SAVE");
			LCD_set_cursor(3, 0);
			LCD_send_string("SETUP FOR CANCEL");
			flags.CNT_flag = 1;
		}
		if (button_press(SW_10_GPIO_Port, SW_10_Pin, ENTER)) {
			val[scroll + cursorRow] = atoi(NUM_S); // Update the value in RAM

			// Save the updated value to flash
			uint16_t virtualAddress = VIRTUAL_ADDR_START + ((scroll + cursorRow) * sizeof(uint16_t));
			if (FEE_Write(virtualAddress, val[scroll + cursorRow]) != HAL_OK) {
				Error_Handler(); // Handle write error
			}

			LCD_Clear_Screen();
			LCD_set_cursor(0, 0);
			LCD_send_Num(val[scroll + cursorRow]);
			LCD_send_string(" Entered");
			LCD_set_cursor(2, 0);
			LCD_send_string("SETUP FOR CLOSE");
		}
		if (button_press(SW_04_GPIO_Port, SW_04_Pin, SETUP)) {
			LCD_Clear_Screen();
			flags.HomeScr_flag = 0;
			flags.Scr_flag = 0;
			flags.CNT_flag = 0;
			NUM_count = 0;
			currentState = STATE_SETUP; // Return to list menu
			return 0;
		}
		else{
			static uint32_t timeoutStart = 0;  // Store timeout start time

			if (timeoutStart == 0) {  // Start timeout tracking when function is entered
				timeoutStart = HAL_GetTick();
			}

			// Check if timeout has passed
			if (HAL_GetTick() - timeoutStart >= TIMEOUT_VALUE) {
				LCD_Clear_Screen();
				flags.HomeScr_flag = 0;
				flags.Scr_flag = 0;
				flags.CNT_flag = 0;
				NUM_count = 0;
				currentState = STATE_SETUP;
				timeoutStart = 0;  // Reset timeout for future calls
				return 0;
			}
		}

	}
	return 1;
}

/**
 * @brief Displays a text string and its corresponding value at a specific row and column on the LCD.
 *
 * @param row The row on the LCD where the text and value should be displayed.
 * @param coloumn The column where the text and value should start.
 * @param text The text string to be displayed.
 * @param val The numerical value associated with the text.
 */
void PRINT_TXT_VAL(uint8_t row, uint8_t coloumn, uint8_t const text[], uint32_t val){
	LCD_set_cursor(row,coloumn);
	LCD_send_string(text);
	LCD_set_cursor(row, 13);
	LCD_send_Num(val);
}

/**
 * @brief Displays the home screen with a list of variables and their values.
 *
 * The home screen shows a scrollable list of items with their current values. It ensures that
 * the list of variables is updated correctly and displays the selected item in a readable format.
 *
 * @return 1 to indicate successful display of the home screen.
 */
uint8_t Home_Scr(void) {
	// Clear only if necessary
	if (!flags.HomeScr_flag) {
		for (uint8_t i = 0; i < NUM_VISIBLE_ROWS; i++) {
			uint8_t itemIndex = (scroll + i) % NUM_ITEMS; // Wrap around
			//val[itemIndex]=(uint16_t)Flash_Read_Last_Word();
			PRINT_TXT_VAL(i, 0, var[itemIndex],val[itemIndex]);
		}
		LCD_set_cursor(cursorRow, 0);
		flags.HomeScr_flag = 1;
	}
	return 1;
}

/**
 * @brief Initializes the Flash EEPROM emulation and loads saved values from flash into RAM.
 *
 * This function prepares the EEPROM for use, ensuring that saved data is available after the system restarts.
 * It handles cases where reading from flash fails and assigns default values.
 */
void EE_Init(void) {
	// Initialize Flash EEPROM Emulation
	if (FEE_Init() != HAL_OK) {
		Error_Handler(); // Handle initialization error
	}
	// Load the latest values from flash to RAM
	for (uint8_t i = 0; i < NUM_ITEMS; i++) {
		uint16_t virtualAddress = VIRTUAL_ADDR_START + (i * 2 /*sizeof(uint16_t)*/);
		if(FEE_Read(virtualAddress,&val[i]) != HAL_OK){
			val[i] = 0; // Default value if read fails
		}
	}
}

/**
 * @brief Detects and processes button presses with debouncing.
 *
 * This function ensures that button presses are correctly detected with debouncing to avoid false presses.
 * It also triggers the corresponding action for the button press, based on the state parameter.
 *
 * @param GPIOx The GPIO port where the button is connected.
 * @param GPIO_Pin The pin number where the button is connected.
 * @param state The state of the button press (e.g., enter, setup).
 *
 * @return GPIO_PIN_SET if a valid button press was detected, GPIO_PIN_RESET otherwise.
 */
GPIO_PinState button_press(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t state) {
	static uint32_t lastPressTime = 0;  // Store last button press time
	static GPIO_PinState lastState = GPIO_PIN_SET;  // Track last stable state

	GPIO_PinState currentState = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);  // Read button state
	if (currentState == GPIO_PIN_RESET && lastState == GPIO_PIN_SET) {  // Detect button press
		if (HAL_GetTick() - lastPressTime > DEBOUNCE_DELAY) {  // Check debounce delay
			lastPressTime = HAL_GetTick();  // Update last press time
			lastState = currentState;  // Update last stable state
			// Call function when button is pressed
			handleButtonPress(state);
			return GPIO_PIN_SET;  // Confirm valid button press
		}
	} else if (currentState == GPIO_PIN_SET) {
		lastState = GPIO_PIN_SET;  // Reset last state when button is released
		if (state >= 1 && state <= 10) {  // If state is between 1-10, reset corresponding flag
			flags.sw_flags[state - 1] = 0;
		} else {  // Handle other flags separately
			switch (state) {
			case ENTER:
				flags.ENT_flag = 0;
				break;
			case SETUP:
				flags.SET_flag = 0;
				break;
			case UP:
				flags.UP_flag = 0;
				break;
			case DOWN:
				flags.DOWN_flag = 0;
				break;
			}
		}
	}
	return GPIO_PIN_RESET;  // No valid press detected
}

/**
 * @brief Handles actions for each button press based on its state.
 *
 * This function triggers different actions based on which button was pressed, such as entering digits
 * for a value, navigating up or down, or confirming the entered value.
 *
 * @param state The state associated with the button press.
 */
void handleButtonPress(uint8_t state){
	switch (state){
	// Handle cases for button presses from _1 to _10 (digits 0-9), as well as ENTER, SETUP, UP, DOWN
	// (Handling value entry, scrolling, etc.)
	}
}

/**
 * @brief Displays the introductory screen with basic information.
 *
 * This function shows the introductory screen, providing basic details about the system, such as its name,
 * version, and company.
 */
void Intro_Scr(void){
	LCD_set_cursor(0,4);
	LCD_send_string("DC Rectifier");
	LCD_set_cursor(1,0);
	LCD_send_string("Battery charger Sys.");
	LCD_set_cursor(2,0);
	LCD_send_string("SW Ver: 1.0");
	LCD_set_cursor(3,0);
	LCD_send_string(" SIGMA ENGINEERING");
}

/**
 * @brief Computes the length of a given string.
 *
 * This function calculates and returns the length of a null-terminated string.
 *
 * @param str The string whose length is to be computed.
 */
uint16_t getStringLength(const char* str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}
