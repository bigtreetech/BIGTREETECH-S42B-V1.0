
#ifndef __OLED_H
#define __OLED_H 

#include "main.h"




#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED控制用函数
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	//写数据、命令    

void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);							//更新整屏		   
							   		    
void OLED_Init(void);									//OLED初始化
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);					//OLED画一个点

void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);			//OLED填充区域
void OLED_ShowChar(uint8_t x,uint8_t y,char chr,uint8_t size,uint8_t mode);	//显示ASCII字符
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);	//显示数字
void OLED_ShowString(uint8_t x,uint8_t y,const char *p);	 		//显示字符串
  
uint32_t oled_pow(uint8_t m,uint8_t n);//m^n次方
void OLED_Showword(uint8_t x,uint8_t y,uint8_t *num,uint8_t mode);





























#endif


























