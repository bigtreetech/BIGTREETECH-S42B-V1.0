#include "storage.h"

uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];


void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
    pBuffer[i]=FlashReadHalfWord(ReadAddr);//
		ReadAddr+=2;//
	}
}


void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)   
{ 			 		 
	uint16_t i;
	for(i=0;i<NumToWrite;i++)
	{
		FlashWriteHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//
	}  
} 


void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)	
{
	uint32_t secpos;	    // Sector position
	uint16_t secoff;	    //
	uint16_t secremain;   //
 	uint16_t i;    
	uint32_t offaddr;     //
	
  // Check if arguments are valid
  if(WriteAddr < STM32_FLASH_BASE || (WriteAddr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))
    return;
	
  FlashUnlock();						
	offaddr = WriteAddr - STM32_FLASH_BASE;		//
	secpos = offaddr / STM_SECTOR_SIZE;			//
	secoff = (offaddr % STM_SECTOR_SIZE) /2;		//
	secremain =STM_SECTOR_SIZE/2-secoff;		//   
	
  if(NumToWrite <= secremain)
    secremain=NumToWrite;//
	
  while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//
		for(i = 0; i < secremain; i++)
		{
			if(STMFLASH_BUF[secoff+i] != 0XFFFF)
        break;
		}
		if(i < secremain)
		{
			FlashErasePage(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE);
			for(i = 0; i < secremain; i++)
			{
				STMFLASH_BUF[i+secoff] = pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//  
		}
    else 
      STMFLASH_Write_NoCheck(WriteAddr, pBuffer, secremain);
		
    if (NumToWrite == secremain)
      break;
		else
		{
		  secpos++;				//
			secoff=0;				// 	 
		  pBuffer+=secremain;  	
			WriteAddr+=secremain*2;	//   
		  NumToWrite-=secremain;	//
			
      if(NumToWrite>(STM_SECTOR_SIZE/2))
        secremain=STM_SECTOR_SIZE/2;
			else 
        secremain=NumToWrite;
		}	 
	};	
	FlashLock();
}


void StoreCurrentParameters()
{
  LED_H;

  table1[0]   = 0xAA;                 
  table1[1]   = Currents;
  //table1[2]   = 16;
  table1[3]   = stepangle;
  //table1[4]   = 3;
  table1[5]   = Motor_ENmode_flag;
  //table1[6]   = 1;
  table1[7]   = Motor_Dir;
  //table1[8]   = 1;
  table1[11]  = kp;                  
  table1[12]  = ki;
  table1[13]  = kd;
  table1[14]  = closemode;

  NVIC_DisableIRQ(USART1_IRQn);
  STMFLASH_Write(Data_Store_Address, table1, sizeof(table1));
  NVIC_EnableIRQ(USART1_IRQn);

  LL_mDelay(250);
  LED_L;
}