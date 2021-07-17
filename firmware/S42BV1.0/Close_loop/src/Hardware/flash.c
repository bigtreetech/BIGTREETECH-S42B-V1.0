#include "flash.h"
#include "stm32f0xx_ll_system.h"


void UsDelay(uint16_t us)
{
  us*=10;
  while(us)	
  {
    __NOP();
	__NOP();
	us--;
  }
}


void FlashUnlock(void)
{
  FLASH->KEYR=FLASH_KEY1;
  FLASH->KEYR=FLASH_KEY2;
}


void FlashLock(void)
{
  FLASH->CR|=1<<7; 
}


uint8_t FlashGetStatus(void)
{
  uint32_t res;
  res=FLASH->SR;
  if(res&(1<<0))//flash busy
    return 1; 
  else if(res&(1<<2))  //Programming error
	return 2; 
  else if(res&(1<<4))  //
	return 3; 
  return 0;     
}


uint8_t FlashWaitDone(uint16_t time)
{
  uint8_t res;
  do
  {
    res=FlashGetStatus();
    if(res!=1)
	  break;
	UsDelay(1);	
    time--;
  }while(time);
  if(time==0)
    res=0xff;
  return res;
}


uint8_t FlashErasePage(uint32_t paddr)
{
  uint8_t res=0;
  res=FlashWaitDone(0X5FFF);
  if(res==0)
  {
    FLASH->CR|=1<<1;                            // Enter page erase mode
    FLASH->AR=paddr;                            // Address of page to erase
    FLASH->CR|=1<<6;                            // Start page erase
    res=FlashWaitDone(0X5FFF);                  // Wait for page erase to complete
    if(res!=1) 
      FLASH->CR&=~(1<<1);                       // Exit page erase mode
  }
  return res;
}

// Erase the last 32kB of flash memory.
void FlashErase32K(void)
{
  FlashErasePage(0x08008000);
  FlashErasePage(0x08008400);
  FlashErasePage(0x08008800);
  FlashErasePage(0x08008C00);
  FlashErasePage(0x08009000);
  FlashErasePage(0x08009400);
  FlashErasePage(0x08009800);
  FlashErasePage(0x08009C00);
  FlashErasePage(0x0800A000);
  FlashErasePage(0x0800A400);
  FlashErasePage(0x0800A800);
  FlashErasePage(0x0800AC00);
  FlashErasePage(0x0800B000);
  FlashErasePage(0x0800B400);
  FlashErasePage(0x0800B800);
  FlashErasePage(0x0800BC00);
  FlashErasePage(0x0800C000);
  FlashErasePage(0x0800C400);
  FlashErasePage(0x0800C800);
  FlashErasePage(0x0800CC00);
  FlashErasePage(0x0800D000);
  FlashErasePage(0x0800D400);
  FlashErasePage(0x0800D800);
  FlashErasePage(0x0800DC00);
  FlashErasePage(0x0800E000);
  FlashErasePage(0x0800E400);
  FlashErasePage(0x0800E800);
  FlashErasePage(0x0800EC00);
  FlashErasePage(0x0800F000);
  FlashErasePage(0x0800F400);
  FlashErasePage(0x0800F800); 
  FlashErasePage(0x0800FC00);
}


uint8_t FlashWriteHalfWord(uint32_t faddr,uint16_t dat)
{
  uint8_t  res;
  res=FlashWaitDone(0XFF);
  if(res==0)
  {
    FLASH->CR|=1<<0;
    *(volatile uint16_t*)faddr=dat; 
    res=FlashWaitDone(0XFF);
	if(res!=1)
    {
      FLASH->CR&=~(1<<0); 
    }
  }
  return res;
}


uint16_t FlashReadHalfWord(uint32_t faddr)
{
  return *(volatile uint16_t*)faddr;
}