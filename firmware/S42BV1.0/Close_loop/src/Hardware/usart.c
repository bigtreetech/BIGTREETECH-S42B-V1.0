/*
 * usart.c
 * 
 * 
 */

#include "usart.h"


#define BUFFER_TX_SIZE      64                      // Transmit buffer size
#define BUFFER_RX_SIZE      64                      // Receive buffer size


volatile uint8_t rxBuffer[BUFFER_RX_SIZE];           // Software RX buffer
volatile uint8_t rxbIndexIn;                         // Index in rxBuffer where the next byte will be stored
volatile uint8_t rxbIndexOut;                        // Index in rxBuffer from where the next byte will be transmitted
volatile bool rxbFull;                               // True if receive buffer is full
volatile bool rxbEmpty;                              // True if receive buffer is empty
volatile uint8_t txBuffer[BUFFER_TX_SIZE];           // Software TX buffer
volatile uint8_t txbIndexIn;                         // Index in txBuffer where the next byte will be stored
volatile uint8_t txbIndexOut;                        // Index in txBuffer from where the next byte will be transmitted
volatile bool txbFull;                               // True if transmit buffer is full
volatile bool txbEmpty;                              // True if transmit buffer is empty


//char Charbuff[16];
//                
//struct __FILE 
//{ 
//	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
//}; 

/* FILE is typedef’ d in stdio.h. */ 
/*
FILE __stdout;       
//  
void _sys_exit(int x) 
{ 
	x = x; 
} 
*/
//STDIO.H
/*
int fputc(int c,FILE *stream) 
{	   
  LL_USART_TransmitData8(USART1,c);
  while(!LL_USART_IsActiveFlag_TXE(USART1)) ;//__NOP();

//  USART1->TDR=(uint8_t)c;
//  LL_USART_ClearFlag_TC(USART1); 
  return c;
}
*/
////STDIO.H
//int fgetc(FILE *stream) 
//{
//  while(!LL_USART_IsActiveFlag_RXNE(USART1)) __NOP();
//  return ((char)LL_USART_ReceiveData8(USART1));
//}

void MX_USART1_UART_Init(void)
{

    rxbIndexIn = 0;
    rxbIndexOut = 0;
    rxbFull = false;
    rxbEmpty = true;

    txbIndexIn = 0;
    txbIndexOut = 0;
    txbFull = false;
    txbEmpty = true;
    
  LL_USART_InitTypeDef USART_InitStruct;
  LL_GPIO_InitTypeDef GPIO_InitStruct;
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);
  
  /**USART1 GPIO Configuration  
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  USART_InitStruct.BaudRate = 38400;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);

  LL_USART_DisableIT_CTS(USART1);
  LL_USART_DisableOverrunDetect(USART1);
  LL_USART_ConfigAsyncMode(USART1);
  
  //NVIC_SetPriority(USART1_IRQn,4);  
  
  //LL_USART_EnableIT_PE(USART1);                         // Enable Parity Error - Is dit regtig nodig
  LL_USART_EnableIT_RXNE(USART1);                       // Enable Receive register not empty interrupt
  //LL_USART_EnableIT_IDLE(USART1);                       // Enable IDLE line interrupt (end of transmission)
  //LL_USART_EnableIT_TXE(USART1);                        // Enable Transmit data register empty interrupt

  LL_USART_Enable(USART1);
  NVIC_EnableIRQ(USART1_IRQn);                          //
                                        
}


// Write one byte to the transmit buffer. Returns false if buffer is full.
bool UART1_WriteByte(uint8_t data)
{
    // Add bytes to software TX buffer for writing when the
    // hardware buffer is empty.

    if (txbFull)
        return false;

    txBuffer[txbIndexIn] = data;
    txbIndexIn++;

    if (txbIndexIn >= BUFFER_TX_SIZE)
        txbIndexIn = 0;
    
    txbEmpty = false;

    if (txbIndexIn == txbIndexOut)
        txbFull = true;

    // Enable TX data register empty interrupt, which will fire
    // almost immediatedly if no bytes are loaded in the hardware
    // register. 
    LL_USART_EnableIT_TXE(USART1);

    return true;
}


uint8_t UART1_Write(uint8_t *data, uint8_t len)
{
    if ((txbFull) | (len == 0))
        return 0;

    int i;
    for (i = 0; i < len; i++)
    {
        txBuffer[txbIndexIn] = data[i];
        txbIndexIn++;

        if (txbIndexIn >= BUFFER_TX_SIZE)
            txbIndexIn = 0;

        if (txbIndexIn == txbIndexOut)
        {
            txbFull = true;
            break;
        }
    }

    txbEmpty = false;
    LL_USART_EnableIT_TXE(USART1);

    return i;
}


// Returns the number of bytes in the receive buffer
uint8_t UART1_BytesToRead()
{
    if (rxbFull)
		return BUFFER_RX_SIZE;
		
	if (rxbIndexIn >= rxbIndexOut)
		return rxbIndexIn - rxbIndexOut;
	else
		return (BUFFER_RX_SIZE - rxbIndexOut) + rxbIndexIn;
}


// Read one byte from the receive buffer
uint8_t UART1_Read()
{
    uint8_t data = 0;

    if (!rxbEmpty)
    {
        data = rxBuffer[rxbIndexOut];
        rxbIndexOut++;

        if (rxbIndexOut >= BUFFER_RX_SIZE)
            rxbIndexOut = 0;

        rxbFull = false;

        if (rxbIndexOut == rxbIndexIn)
            rxbEmpty = true;
    }

    return data;
}


// USART1 Interrupt Service Handler
void USART1_IRQHandler(void)
{
    // Test for received bytes    
    if((USART1->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
    {
        // New byte has been received. Interrupt flag is cleared by reading the register.
        uint8_t data = USART1->RDR;
        
        if (!rxbFull)
        {
            rxBuffer[rxbIndexIn] = data;
            rxbIndexIn++;

            if (rxbIndexIn >= BUFFER_RX_SIZE)
                rxbIndexIn = 0;
    
            rxbEmpty = false;

            if (rxbIndexIn == rxbIndexOut)
                rxbFull = true;
        }
    }
    // Test for idle line (end of transmission)
    else if(LL_USART_IsActiveFlag_IDLE(USART1) == 1)
    {
        // The idle line interrupt is generated when the line goes to idle
        // which typically means the end of transmission. This could be a
        // good time to perform processing of the received bytes.
        // Currently not used anymore.

        LL_USART_ClearFlag_IDLE(USART1);
    } 
    // Test if new byte can be send
    else if ((USART1->ISR & USART_ISR_TXE) == USART_ISR_TXE)
    {
        // Interrupt is generated when the transmit data register is empty.
        // Now would be a good time to load the next byte that needs to be
        // transmitted. Flag is cleared by writing to the data register. 

        // Check if the software buffer has bytes to send and if so loads
        // the next byte. 
        if (!txbEmpty)
        {
            USART1->TDR = txBuffer[txbIndexOut];
            txbIndexOut++;

            if (txbIndexOut >= BUFFER_TX_SIZE)
                txbIndexOut = 0;

            txbFull = false;

            if (txbIndexOut == txbIndexIn)
                txbEmpty = true;
        }
        else
        {
            // No more bytes in software buffer so disable the interrupt
            LL_USART_DisableIT_TXE(USART1);
        }
    }
}






















