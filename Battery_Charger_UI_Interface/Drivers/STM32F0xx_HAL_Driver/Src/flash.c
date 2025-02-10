/*
 * flash.c
 *
 * Created on			:Feb 9, 2025
 * Author				:Aly Mustafa
 * @GitHub				:https://github.com/AlyMustafa
 *
 *  Description:
 *  This source file implements Flash memory operations and EEPROM emulation
 *  for STM32F0xx microcontrollers.
 */

#include "Flash.h"
// ================================================================================
//                            FLASH MEMORY OPERATIONS
// ================================================================================

/**
 * @brief Erases a Flash page.
 * @param addr Flash address (must be page-aligned)
 * @return HAL status (HAL_OK if successful, error code otherwise).
 */
HAL_StatusTypeDef Flash_Erase_Page(uint32_t addr) {
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef eraseInit;
    uint32_t pageError = 0;
    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.PageAddress = addr;
    eraseInit.NbPages = 1;
    HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&eraseInit, &pageError);
    HAL_FLASH_Lock();
    return status;
}

/**
 * @brief Writes a full word (32-bit) to Flash memory.
 * @param addr Flash address.
 * @param data 32-bit data to be written.
 */
void Flash_Write_Word(uint32_t addr, uint32_t data) {
    HAL_FLASH_Unlock();
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, data) != HAL_OK) {
        printf("Flash Write Failed at 0x%08lX\n", addr);
    }
    HAL_FLASH_Lock();
}

/**
 * @brief Reads a half word (16-bit) from Flash memory.
 * @param addr Flash address.
 * @return Read data.
 */
uint16_t Flash_Read_HalfWord(uint32_t addr) {
    return *(volatile uint16_t*)(addr);
}

/**
 * @brief Writes a half word (16-bit) to Flash memory.
 * @param ADDR Address to write the data.
 * @param data 16-bit data to be written.
 */
void Flash_Write_HalfWord(uint32_t ADDR, uint16_t data) {
    HAL_FLASH_Unlock();
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, ADDR, data) != HAL_OK) {
        printf("Flash Write Failed at 0x%08lX\n", ADDR);
    }
    HAL_FLASH_Lock();
}

// ================================================================================
//                      FLASH EEPROM EMULATION APIS
// ================================================================================

/**
 * @brief Initializes Flash EEPROM emulation.
 * @return HAL status (HAL_OK if successful, error code otherwise).
 */
HAL_StatusTypeDef FEE_Init(void) {
    uint32_t page1_marker = *(volatile uint32_t*)FLASH_PAGE_1;
    uint32_t page2_marker = *(volatile uint32_t*)FLASH_PAGE_2;

    if (page1_marker != VALID_FLASH_MARKER && page2_marker != VALID_FLASH_MARKER) {
        HAL_FLASH_Unlock();
        FLASH_EraseInitTypeDef eraseInit;
        uint32_t pageError;
        eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
        eraseInit.PageAddress = FLASH_PAGE_1;
        eraseInit.NbPages = 2;
        if (HAL_FLASHEx_Erase(&eraseInit, &pageError) != HAL_OK) {
            HAL_FLASH_Lock();
            return HAL_ERROR;
        }
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_PAGE_1, VALID_FLASH_MARKER) != HAL_OK) {
            HAL_FLASH_Lock();
            return HAL_ERROR;
        }
        HAL_FLASH_Lock();
    }
    return HAL_OK;
}

/**
 * @brief Reads a value from EEPROM emulation.
 * @param virtualAddress Virtual address in EEPROM to read from.
 * @param data Pointer to store the read value.
 * @return HAL status (HAL_OK if successful, error code otherwise).
 */
HAL_StatusTypeDef FEE_Read(uint16_t virtualAddress, uint32_t *data) {
    uint32_t activePage = FLASH_PAGE_1;
    if (*(volatile uint32_t*)FLASH_PAGE_2 == VALID_FLASH_MARKER) {
        activePage = FLASH_PAGE_2;
    }
    uint32_t physicalAddress = activePage + FLASH_PAGE_SIZE - 4;
    while (physicalAddress >= activePage + 4) {
        uint32_t word = *(volatile uint32_t*)physicalAddress;
        uint8_t storedVirtualAddress = (word >> 24) & 0xFF;
        uint32_t storedData = word & 0xFFFFFF;
        if (storedVirtualAddress == virtualAddress) {
            *data = storedData;
            return HAL_OK;
        }
        physicalAddress -= 4;
    }
    return HAL_ERROR;
}

/**
 * @brief Writes a value to EEPROM emulation.
 * @param virtualAddress Virtual address in EEPROM to write to.
 * @param data 32-bit data to be stored.
 * @return HAL status (HAL_OK if successful, error code otherwise).
 */
HAL_StatusTypeDef FEE_Write(uint16_t virtualAddress, uint32_t data) {
    uint32_t activePage = FLASH_PAGE_1;
    if (*(volatile uint32_t*)FLASH_PAGE_2 == VALID_FLASH_MARKER) {
        activePage = FLASH_PAGE_2;
    }
    uint32_t physicalAddress = activePage + 4;
    while (physicalAddress < activePage + FLASH_PAGE_SIZE) {
        if (*(volatile uint32_t*)physicalAddress == 0xFFFFFFFF) {
            HAL_FLASH_Unlock();
            uint32_t wordToWrite = ((uint8_t)virtualAddress << 24) | data;
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, physicalAddress, wordToWrite) != HAL_OK) {
                HAL_FLASH_Lock();
                return HAL_ERROR;
            }
            HAL_FLASH_Lock();
            return HAL_OK;
        }
        physicalAddress += 4;
    }
    return HAL_ERROR;
}
