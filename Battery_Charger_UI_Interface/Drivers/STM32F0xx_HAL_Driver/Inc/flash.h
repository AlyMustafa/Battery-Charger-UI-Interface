/*
 * flash.h
 *
 * Created on			:Feb 9, 2025
 * Author				:Aly Mustafa
 * @GitHub				:https://github.com/AlyMustafa
 *
 *  Description:
 *  This header file provides function declarations for Flash memory operations on STM32F0xx microcontrollers.
 *  It includes functions for erasing, reading, and writing Flash memory, as well as EEPROM emulation.
 */

#ifndef STM32F0XX_HAL_DRIVER_INC_FLASH_H_
#define STM32F0XX_HAL_DRIVER_INC_FLASH_H_

#include "stm32f0xx_hal.h"
#include <string.h>
#include <stdio.h>

// ================================================================================
//                                     MACROS
// ================================================================================
#define FLASH_PAGE_1  0x0800F800  // First EEPROM Page
#define FLASH_PAGE_2  0x0800FC00  // Second EEPROM Page
#define VALID_FLASH_MARKER 0xABCDABCD // Marker to identify valid flash page

// Virtual addresses for each variable
#define VIRTUAL_ADDR_START 0x0000
#define VIRTUAL_ADDR_END   (VIRTUAL_ADDR_START + (NUM_ITEMS * sizeof(uint32_t)))
// ================================================================================
//                            FLASH MEMORY OPERATIONS
// ================================================================================

/**
 * @brief Erases a specific Flash page.
 * @param ADDR Address of the Flash page to be erased.
 * @return HAL status (HAL_OK if successful, error code otherwise).
 */
HAL_StatusTypeDef Flash_Erase_Page(uint32_t ADDR);

/**
 * @brief Writes a full word (32-bit) to Flash memory.
 * @param ADDR Address to write the data.
 * @param data 32-bit data to be written.
 */
void Flash_Write_Fullword(uint32_t ADDR, uint32_t data);

/**
 * @brief Writes a half word (16-bit) to Flash memory.
 * @param ADDR Address to write the data.
 * @param data 16-bit data to be written.
 */
void Flash_Write_HalfWord(uint32_t ADDR, uint16_t data);

/**
 * @brief Reads a single byte from Flash memory.
 * @param ADDR Address to read from.
 * @return Read byte.
 */
uint8_t Flash_Read_Byte(uint32_t ADDR);

/**
 * @brief Reads a half word (16-bit) from Flash memory.
 * @param ADDR Address to read from.
 * @return Read half word.
 */
uint16_t Flash_Read_HalfWord(uint32_t ADDR);

/**
 * @brief Reads a full word (32-bit) from Flash memory.
 * @param ADDR Address to read from.
 * @return Read full word.
 */
uint32_t Flash_Read_FullWord(uint32_t ADDR);

/**
 * @brief Reads the full Flash page into a RAM buffer.
 * @param ADDR Address of the Flash page to read.
 * @param data Pointer to a buffer where the page data will be stored.
 */
void Flash_Backup_Page(uint32_t ADDR, uint32_t* data);

/**
 * @brief Modifies a single byte in the backup buffer.
 * @param offset Offset within the page (0 to FLASH_PAGE_SIZE-1).
 * @param value New byte value to write.
 */
void Flash_Modify_Backup(uint32_t offset, uint8_t value);

/**
 * @brief Erases the last Flash page.
 * @return HAL status (HAL_OK if successful, error code otherwise).
 */
HAL_StatusTypeDef Flash_Erase_Page_LastPage(void);

/**
 * @brief Writes the modified backup buffer back to Flash.
 * @param ADDR Address where the buffer should be restored.
 */
void Flash_Restore_Page(uint32_t ADDR);

// ================================================================================
//                      FLASH EEPROM EMULATION APIS
// ================================================================================

/**
 * @brief Initializes Flash EEPROM emulation.
 * @return HAL status (HAL_OK if successful, error code otherwise).
 */
HAL_StatusTypeDef FEE_Init(void);

/**
 * @brief Reads a value from EEPROM emulation.
 * @param virtualAddress Virtual address in EEPROM to read from.
 * @param data Pointer to store the read value.
 * @return HAL status (HAL_OK if successful, error code otherwise).
 */
HAL_StatusTypeDef FEE_Read(uint16_t virtualAddress, uint32_t *data);

/**
 * @brief Writes a value to EEPROM emulation.
 * @param virtualAddress Virtual address in EEPROM to write to.
 * @param data 32-bit data to be stored.
 * @return HAL status (HAL_OK if successful, error code otherwise).
 */
HAL_StatusTypeDef FEE_Write(uint16_t virtualAddress, uint32_t data);

#endif /* STM32F0XX_HAL_DRIVER_INC_FLASH_H_ */
