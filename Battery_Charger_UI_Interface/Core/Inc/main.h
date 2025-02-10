/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Rect_G_Pin GPIO_PIN_13
#define Rect_G_GPIO_Port GPIOC
#define Rect_R_Pin GPIO_PIN_14
#define Rect_R_GPIO_Port GPIOC

#define Buzzer_Pin GPIO_PIN_15
#define Buzzer_GPIO_Port GPIOC

#define Batt_B_Pin GPIO_PIN_0
#define Batt_B_GPIO_Port GPIOF
#define Batt_G_Pin GPIO_PIN_1
#define Batt_G_GPIO_Port GPIOF
#define Batt_R_Pin GPIO_PIN_0
#define Batt_R_GPIO_Port GPIOC

#define CHR_B_Pin GPIO_PIN_1
#define CHR_B_GPIO_Port GPIOC
#define CHR_G_Pin GPIO_PIN_2
#define CHR_G_GPIO_Port GPIOC
#define CHR_R_Pin GPIO_PIN_3
#define CHR_R_GPIO_Port GPIOC

#define RS_4_20_LCD_Pin GPIO_PIN_4
#define RS_4_20_LCD_GPIO_Port GPIOC
#define E_4_20_LCD_Pin GPIO_PIN_5
#define E_4_20_LCD_GPIO_Port GPIOC

#define DB4_4_20_LCD_Pin GPIO_PIN_0
#define DB4_4_20_LCD_GPIO_Port GPIOB
#define DB5_4_20_LCD_Pin GPIO_PIN_1
#define DB5_4_20_LCD_GPIO_Port GPIOB
#define EEPROM_WP_Pin GPIO_PIN_12
#define EEPROM_WP_GPIO_Port GPIOB
#define SW_05_Pin GPIO_PIN_13
#define SW_05_GPIO_Port GPIOB
#define SW_10_Pin GPIO_PIN_14
#define SW_10_GPIO_Port GPIOB
#define SW_04_Pin GPIO_PIN_15
#define SW_04_GPIO_Port GPIOB
#define SW_09_Pin GPIO_PIN_6
#define SW_09_GPIO_Port GPIOC
#define SW_03_Pin GPIO_PIN_7
#define SW_03_GPIO_Port GPIOC
#define SW_08_Pin GPIO_PIN_8
#define SW_08_GPIO_Port GPIOC
#define SW_02_Pin GPIO_PIN_9
#define SW_02_GPIO_Port GPIOC
#define SW_07_Pin GPIO_PIN_8
#define SW_07_GPIO_Port GPIOA
#define SW_06_Pin GPIO_PIN_11
#define SW_06_GPIO_Port GPIOA
#define SW_01_Pin GPIO_PIN_12
#define SW_01_GPIO_Port GPIOA
#define DB6_4_20_LCD_Pin GPIO_PIN_2
#define DB6_4_20_LCD_GPIO_Port GPIOD
#define DB7_4_20_LCD_Pin GPIO_PIN_3
#define DB7_4_20_LCD_GPIO_Port GPIOB
#define Mains_B_Pin GPIO_PIN_4
#define Mains_B_GPIO_Port GPIOB
#define Mains_G_Pin GPIO_PIN_5
#define Mains_G_GPIO_Port GPIOB
#define Mains_R_Pin GPIO_PIN_8
#define Mains_R_GPIO_Port GPIOB
#define Rect_B_Pin GPIO_PIN_9
#define Rect_B_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
