/*
 * LCD.h
 * Created on: Jan 27, 2024
 * Author: Ali Mustafa
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "tim.h"
#include "stm32f0xx_hal.h"


/******CTRL_PINS******/
#define RS_4_20_LCD_Pin GPIO_PIN_4
#define RS_4_20_LCD_GPIO_Port GPIOC
#define E_4_20_LCD_Pin GPIO_PIN_5
#define E_4_20_LCD_GPIO_Port GPIOC

#define DB4_4_20_LCD_Pin GPIO_PIN_0
#define DB4_4_20_LCD_GPIO_Port GPIOB
#define DB5_4_20_LCD_Pin GPIO_PIN_1
#define DB5_4_20_LCD_GPIO_Port GPIOB
#define DB6_4_20_LCD_Pin GPIO_PIN_2
#define DB6_4_20_LCD_GPIO_Port GPIOD
#define DB7_4_20_LCD_Pin GPIO_PIN_3
#define DB7_4_20_LCD_GPIO_Port GPIOB

/*
#define lcd_port					GPIOA
#define lcd_control_port			GPIOA
#define DATA_shift  0
#define RS			GPIO_PIN_8
#define RW			GPIO_PIN_9		//Read_write_pin
#define EN			GPIO_PIN_10 	//Enable_pin
*/

/*#define D0_PIN				GPIO_PIN_0
#define D1_PIN				GPIO_PIN_1
#define D2_PIN				GPIO_PIN_2
#define D3_PIN				GPIO_PIN_3

#define D4_PIN				GPIO_PIN_4
#define D5_PIN				GPIO_PIN_5
#define D6_PIN				GPIO_PIN_6
#define D7_PIN				GPIO_PIN_7
*/

//Modes

#define	EIGHT_BIT_MODE   8
#define FOUR_BIT_MODE    4
//MODE
#define LCD_MODE  		FOUR_BIT_MODE


//COMMANDS_VALUES
#define LCD_FUNCTION_8BIT_2LINES   					(0x38)
#define LCD_FUNCTION_4BIT_2LINES   					(0x28)
#define LCD_MOVE_DISP_RIGHT       					(0x1C)
#define LCD_MOVE_DISP_LEFT   						(0x18)
#define LCD_MOVE_CURSOR_RIGHT   					(0x14)
#define LCD_MOVE_CURSOR_LEFT 	  					(0x10)
#define LCD_DISP_OFF   								(0x08)
#define LCD_DISP_ON_CURSOR   						(0x0E)
#define LCD_DISP_ON_CURSOR_BLINK   					(0x0F)
#define LCD_DISP_ON_BLINK   						(0x0D)
#define LCD_DISP_ON   								(0x0C)
#define LCD_ENTRY_DEC   							(0x04)
#define LCD_ENTRY_DEC_SHIFT   						(0x05)
#define LCD_ENTRY_INC_   							(0x06)
#define LCD_ENTRY_INC_SHIFT   						(0x07)
#define LCD_BEGIN_AT_FIRST_ROW						(0x80) //default DDRAM address (10000000) to get first line and 1 position ,to set any position ,it will be added to the default
#define LCD_BEGIN_AT_SECOND_ROW						(0xC0)//C0
#define LCD_BEGIN_AT_THIRD_ROW						(0x94)
#define LCD_BEGIN_AT_FOURTH_ROW						(0xD4)
#define LCD_CLEAR_SCREEN							(0x01)
#define LCD_ENTRY_MODE								(0x06)

//LCD_Functions

void LCD_check_isbusy(void);
void LCD_init(void);
void LCD_send_command(unsigned char command);
void LCD_send_character(unsigned char character);
void LCD_send_string(const char* string);
void LCD_Kick_Enable(void);
void LCD_set_cursor(unsigned char line , unsigned char position );
void LCD_Clear_Screen(void);
void LCD_GPIO_init(void);
void LCD_send_Num (uint32_t NUM);
#endif /* INC_LCD_H_ */
