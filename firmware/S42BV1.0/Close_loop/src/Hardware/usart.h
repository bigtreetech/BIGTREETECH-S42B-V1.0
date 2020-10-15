#ifndef __USART_H
#define __USART_H


#include "main.h"
#include "stdbool.h"

//typedef void(* volatile byteReceivedCallbackType)(uint8_t);

void PID_Cal_value_init(void);
void MX_USART1_UART_Init(void);
//void usart_Receive_Process(void);
//void SerialCheck(void);
void USART1_IRQHandler(void);
//void UART1_SendByte(uint8_t data);
//uint8_t UART1_ReceiveByte(void);
//void UART1_SendStr(char *str);
//void Uart_Data_Conversion(int16_t Conversion_value,uint8_t num);

//void RegisterByteReceived(byteReceivedCallbackType);
uint8_t UART1_BytesToRead();
uint8_t UART1_Read();
bool UART1_WriteByte(uint8_t data);                     // Write a single byte
uint8_t UART1_Write(uint8_t* data, uint8_t len);           // Write multiple bytes, returns actual number of bytes that could be pushed to TX buffer. 

#endif


