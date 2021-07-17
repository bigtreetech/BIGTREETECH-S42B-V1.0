/*
 * Handles USART operations
 * 
 * Contributors:
 * - Jan Swanepoel
 * 
 * 
 */

#ifndef __USART_H
#define __USART_H


#include "main.h"
#include "stdbool.h"


void MX_USART1_UART_Init(void);
void USART1_IRQHandler(void);

// Returns the number of bytes in the receive buffer
uint8_t UART1_BytesToRead();

// Read one byte from the receive buffer
uint8_t UART1_Read();

// Write one byte to the transmit buffer. Returns false if buffer is full.
bool UART1_WriteByte(uint8_t data);                     

// Write multiple bytes, returns actual number of bytes that could be pushed to TX buffer.
uint8_t UART1_Write(uint8_t* data, uint8_t len);         

#endif


