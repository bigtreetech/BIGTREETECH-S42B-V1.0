
#ifndef __OLED_H
#define __OLED_H 
#include "stdio.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_gpio.h"
//#include "main.h"

// Define pins
#define OLED_CS_Pin             LL_GPIO_PIN_12		
#define OLED_CS_GPIO_Port       GPIOB
#define OLED_RST_Pin            LL_GPIO_PIN_13 		
#define OLED_RST_GPIO_Port      GPIOC
#define OLED_RS_Pin             LL_GPIO_PIN_13		
#define OLED_RS_GPIO_Port       GPIOB

#define OLED_SCLK_Pin           LL_GPIO_PIN_15		
#define OLED_SCLK_GPIO_Port     GPIOB
#define OLED_SDIN_Pin           LL_GPIO_PIN_14	    
#define OLED_SDIN_GPIO_Port     GPIOB

#define OLED_CS_H       LL_GPIO_SetOutputPin(OLED_CS_GPIO_Port, OLED_CS_Pin)  		
#define OLED_CS_L       LL_GPIO_ResetOutputPin(OLED_CS_GPIO_Port, OLED_CS_Pin)

#define OLED_RST_H      LL_GPIO_SetOutputPin(OLED_RST_GPIO_Port,OLED_RST_Pin) 		
#define OLED_RST_L      LL_GPIO_ResetOutputPin(OLED_RST_GPIO_Port,OLED_RST_Pin) 	

#define OLED_RS_H       LL_GPIO_SetOutputPin(OLED_RS_GPIO_Port,OLED_RS_Pin)		
#define OLED_RS_L       LL_GPIO_ResetOutputPin(OLED_RS_GPIO_Port,OLED_RS_Pin)		

#define OLED_SCLK_H     LL_GPIO_SetOutputPin(OLED_SCLK_GPIO_Port,OLED_SCLK_Pin)		
#define OLED_SCLK_L     LL_GPIO_ResetOutputPin(OLED_SCLK_GPIO_Port,OLED_SCLK_Pin)		

#define OLED_SDIN_H     LL_GPIO_SetOutputPin(OLED_SDIN_GPIO_Port,OLED_SDIN_Pin)	    
#define OLED_SDIN_L     LL_GPIO_ResetOutputPin(OLED_SDIN_GPIO_Port,OLED_SDIN_Pin)		

#define OLED_CMD  0	
#define OLED_DATA 1	

extern uint8_t oledClock;               // JaSw: Holds OLED Display Clock Divide Ratio/Oscillator Frequency

void OLED_WR_Byte(uint8_t dat, uint8_t cmd);	

void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);					   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);					
void OLED_SetDisplayClock(uint8_t val);

void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);	
void OLED_ShowChar(uint8_t x,uint8_t y,char chr,uint8_t size,uint8_t mode);	
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y,const char *p);	 		
  
uint32_t oled_pow(uint8_t m,uint8_t n);
//void OLED_Showword(uint8_t x,uint8_t y,uint8_t *num,uint8_t mode);





























#endif


























