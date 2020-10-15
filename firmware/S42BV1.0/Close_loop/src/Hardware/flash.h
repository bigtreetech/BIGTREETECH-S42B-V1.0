/*
 * Handles flash memory operations
 * 
 * Contributors:
 * - Jan Swanepoel
 * 
 */

#include "stdio.h"

void FlashUnlock(void);
void FlashLock(void);
uint8_t FlashGetStatus(void);
uint8_t FlashWaitDone(uint16_t time);
uint8_t FlashErasePage(uint32_t paddr);
void FlashErase32K(void);
uint8_t FlashWriteHalfWord(uint32_t faddr,uint16_t dat);
uint16_t FlashReadHalfWord(uint32_t faddr);
