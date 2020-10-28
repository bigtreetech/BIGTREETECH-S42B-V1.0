/*
 * Handles flash memory storage
 * 
 * Contributors:
 * - Jan Swanepoel
 * 
 * 
 * FLASH STORAGE LAYOUT
 * The first 31kB (31744 bytes) holds the program code.
 * The next 1kB holds the parameters.
 * The remaining 32kB holds the calibration table
 * 
 * Parameter Layout
 * HalfWord     Description
 * ----------   -----------
 * 0            Calibration Flag. Calibrated = 0xAA
 * 1            Motor Current
 * 2            
 * 3            Step Size
 * 4
 * 5            Motor Enable Mode
 * 6
 * 7            Motor Direction
 * 
 */
#include "flash.h"
#include "main.h"

#define STM32_FLASH_SIZE 64
#define STM32_FLASH_BASE        0x8000000           // SOC Flash Base Address 
#define Data_Store_Address      0x8007C00           // Flash data storage address (last page of first 32kB, make sure program size stays below it!)  

#if STM32_FLASH_SIZE < 256
    #define STM_SECTOR_SIZE 1024
#else 
    #define STM_SECTOR_SIZE	2048
#endif		 


void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);
void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);
