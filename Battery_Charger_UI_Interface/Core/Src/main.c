/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 * @Author         : Aly Mustafa Enaya
 * @GitHub		   : https://github.com/AlyMustafa
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <app.h>

void SystemClock_Config(void);

/**
 * @brief Main function - Entry point of the program
 * @retval int
 */
int main(void)
{
    // Initialize the HAL Library
    HAL_Init();

    // Configure the system clock
    SystemClock_Config();

    // Initialize GPIO, LCD, and EEPROM (Flash) storage
    MX_GPIO_Init();
    LCD_init();
    EE_Init();

    // Display the introduction screen
    Intro_Scr();

    // Main program loop
    while (1) {
        switch (currentState) {
        case STATE_IDLE:
            // Check if SETUP button is pressed to enter setup mode
            if (button_press(SW_04_GPIO_Port, SW_04_Pin, SETUP)) {
                LCD_Clear_Screen();
                currentState = STATE_SETUP;
            }
            break;

        case STATE_SETUP:
            // Display the Home Screen
            Home_Scr();

            // Check for button presses to navigate through the menu
            if (button_press(SW_08_GPIO_Port, SW_08_Pin, DOWN)) {
                // Handle DOWN button action
            }
            if (button_press(SW_03_GPIO_Port, SW_03_Pin, UP)) {
                // Handle UP button action
            }
            if (button_press(SW_10_GPIO_Port, SW_10_Pin, ENTER)) {
                // Confirm selection and move to message state
                currentState = STATE_MESSAGE;
                LCD_Clear_Screen();
            }
            break;

        case STATE_MESSAGE:
            // User interface handling for message state
            if (user_UI()) {
                // Uncomment below if saving variables is required
                // EE_Save_All_Variables(); // Save all variables to flash
            }
            break;

        default:
            // Handle unexpected states (if any)
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////           SYSTEM FUNCTION              ///////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Configures the system clock
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB, and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief  Error handler function - Infinite loop in case of failure
 * @retval None
 */
void Error_Handler(void)
{
    /* Disable interrupts and enter infinite loop */
    __disable_irq();
    while (1)
    {
    }
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add their own implementation to report the file name and line number */
    /* Example: printf("Wrong parameters value: file %s on line %d\r\n", file, line); */
}
#endif /* USE_FULL_ASSERT */
