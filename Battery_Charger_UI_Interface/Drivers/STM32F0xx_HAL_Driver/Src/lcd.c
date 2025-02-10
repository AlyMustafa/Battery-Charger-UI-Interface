/*************************************************************************/
/* Author        : Ali Mustafa                                   		 	*/
/* Project       : STM32F103C8T6_Drivers  	                          	   	*/
/* File          : LCD.c 			                             			*/
/* Date          : Jan 28, 2024                                         	*/
/* Version       : V1                                                   	*/
/* GitHub        :             		     									*/
/*************************************************************************/

#include "lcd.h"
#include "stm32f0xx_hal.h"

int i=0;
void LCD_init()
{
	HAL_Delay(30); // Wait for power stabilization

	// Set all control pins to RESET
	HAL_GPIO_WritePin(E_4_20_LCD_GPIO_Port, E_4_20_LCD_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RS_4_20_LCD_GPIO_Port, RS_4_20_LCD_Pin, GPIO_PIN_RESET);

	// Begin 4-bit initialization as per datasheet
	HAL_Delay(20); // Wait more than 30ms after power-on

	// Send Function Set command (first nibble for 4-bit mode)
	HAL_GPIO_WritePin(DB4_4_20_LCD_GPIO_Port, DB4_4_20_LCD_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DB5_4_20_LCD_GPIO_Port, DB5_4_20_LCD_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DB6_4_20_LCD_GPIO_Port, DB6_4_20_LCD_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DB7_4_20_LCD_GPIO_Port, DB7_4_20_LCD_Pin, GPIO_PIN_RESET);
	LCD_Kick_Enable(); // Pulse the Enable pin
	HAL_Delay(5);      // Wait for the command to process

	// Send Function Set command again for confirmation
	LCD_Kick_Enable(); // Repeat the Enable pulse
	HAL_Delay(1);      // Wait at least 1ms

	// Enter 4-bit mode fully
	LCD_send_command(LCD_FUNCTION_4BIT_2LINES); // Set 4-bit mode, 2 lines
	HAL_Delay(1); // Command processing delay

	// Clear the display and set entry mode

	LCD_send_command(LCD_BEGIN_AT_FIRST_ROW); // Set DDRAM address to the beginning
	LCD_send_command(LCD_ENTRY_MODE);         // Entry mode: increment, no shift
	LCD_send_command(LCD_DISP_ON_CURSOR_BLINK); // Turn on display, cursor, and blink
	LCD_Clear_Screen(); // Clear the display
	HAL_Delay(2); // Allow the LCD to stabilize
}

void LCD_Kick_Enable(void)
{
	HAL_GPIO_WritePin(E_4_20_LCD_GPIO_Port, E_4_20_LCD_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(E_4_20_LCD_GPIO_Port, E_4_20_LCD_Pin, GPIO_PIN_RESET);
	 HAL_Delay(1);
}

void LCD_send_command(unsigned char command)
{
	HAL_GPIO_WritePin(RS_4_20_LCD_GPIO_Port, RS_4_20_LCD_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(lcd_control_port, RW, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(E_4_20_LCD_GPIO_Port, E_4_20_LCD_Pin, GPIO_PIN_RESET);

	//#if LCD_MODE == FOUR_BIT_MODE
	HAL_GPIO_WritePin(DB4_4_20_LCD_GPIO_Port , DB4_4_20_LCD_Pin , (command&0x10) );
	HAL_GPIO_WritePin(DB5_4_20_LCD_GPIO_Port , DB5_4_20_LCD_Pin , (command&0x20) );
	HAL_GPIO_WritePin(DB6_4_20_LCD_GPIO_Port , DB6_4_20_LCD_Pin , (command&0x40) );
	HAL_GPIO_WritePin(DB7_4_20_LCD_GPIO_Port , DB7_4_20_LCD_Pin , (command&0x80) );
	LCD_Kick_Enable();
	HAL_GPIO_WritePin(DB4_4_20_LCD_GPIO_Port , DB4_4_20_LCD_Pin , (command&0x01) );
	HAL_GPIO_WritePin(DB5_4_20_LCD_GPIO_Port , DB5_4_20_LCD_Pin , (command&0x02) );
	HAL_GPIO_WritePin(DB6_4_20_LCD_GPIO_Port , DB6_4_20_LCD_Pin , (command&0x04) );
	HAL_GPIO_WritePin(DB7_4_20_LCD_GPIO_Port , DB7_4_20_LCD_Pin , (command&0x08) );
	//HAL_Delay(1);
	LCD_Kick_Enable();
	//#endif
}

void LCD_send_character(unsigned char character)
{
	HAL_GPIO_WritePin(RS_4_20_LCD_GPIO_Port, RS_4_20_LCD_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DB4_4_20_LCD_GPIO_Port , DB4_4_20_LCD_Pin , (character&0x10) );
	HAL_GPIO_WritePin(DB5_4_20_LCD_GPIO_Port , DB5_4_20_LCD_Pin , (character&0x20) );
	HAL_GPIO_WritePin(DB6_4_20_LCD_GPIO_Port , DB6_4_20_LCD_Pin , (character&0x40) );
	HAL_GPIO_WritePin(DB7_4_20_LCD_GPIO_Port , DB7_4_20_LCD_Pin , (character&0x80) );
	LCD_Kick_Enable();
	HAL_GPIO_WritePin(DB4_4_20_LCD_GPIO_Port , DB4_4_20_LCD_Pin , (character&0x01) );
	HAL_GPIO_WritePin(DB5_4_20_LCD_GPIO_Port , DB5_4_20_LCD_Pin , (character&0x02) );
	HAL_GPIO_WritePin(DB6_4_20_LCD_GPIO_Port , DB6_4_20_LCD_Pin , (character&0x04) );
	HAL_GPIO_WritePin(DB7_4_20_LCD_GPIO_Port , DB7_4_20_LCD_Pin , (character&0x08) );
	LCD_Kick_Enable();
}

void LCD_send_string( const char* string)
{
	// to tell how much the display fills
	while(*string > 0){
		//i++;
		LCD_send_character(*string++);
//		if(i == 20)
//			LCD_set_cursor(1,0);
//		if (i == 40)
//		{
//			LCD_set_cursor(2,0);
//		}
//		if (i == 60)
//		{
//			LCD_set_cursor(3,0);
//		}
//		if (i == 80)
//		{
//			LCD_set_cursor(0,0);
//			i=0;
//		}
	}
}

void LCD_Clear_Screen(){
	LCD_send_command(LCD_CLEAR_SCREEN);
}
/**=============================================
 * @Fn				- LCD_Set_Cursor
 * @brief 			- Sets the location of the cursor
 * @param [in] 	- row: Selects the row number of the displayed character @ref LCD_ROWS_POS_define
 * @param [in] 	- column: Selects the column number of the displayed character (1...16)
 * @param [out] 	- None
 * @retval 		- None
 * Note			- None
 */
void LCD_set_cursor(unsigned char row , unsigned char column ){
	if (row == 0)
	{
		if(column >= 0 && column < 20)
			LCD_send_command(LCD_BEGIN_AT_FIRST_ROW + column);
	}
	if (row == 1)
	{
		if(column >= 0 && column < 20)
			LCD_send_command(LCD_BEGIN_AT_SECOND_ROW + column);
	}
	if (row == 2)
	{
		if(column >= 0 && column < 20)
			LCD_send_command(LCD_BEGIN_AT_THIRD_ROW + column);
	}
	if (row == 3)
	{
		if(column >= 0 && column < 20)
			LCD_send_command(LCD_BEGIN_AT_FOURTH_ROW + column);
	}
}


void LCD_send_Num (uint32_t NUM){
	uint32_t string[6];
	sprintf (string, "%d" ,NUM);
	LCD_send_string(string);


}
