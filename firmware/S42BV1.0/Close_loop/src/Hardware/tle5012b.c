#include "tle5012b.h"



void TLE5012B_Init()
{
    // Configure chip select pin
    LL_GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = NSS_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(NSS_GPIO_Port, &GPIO_InitStruct);

    // Angle range 360°
    WriteValue(WRITE_MOD2_VALUE, MOD2_VALUE);
}


bool CheckHealth()
{
  // Read status register
  uint16_t state = ReadState();
 
  // Check for Magnitude out of limit error
  if(state&0x0080)
  {

  }
}


uint16_t ReadValue(uint16_t RegAdd)
{
    uint16_t data;
    NSS_L;
    while(LL_SPI_IsActiveFlag_TXE(SPI1)==0);
    LL_SPI_TransmitData16(SPI1,RegAdd);
    while(LL_SPI_IsActiveFlag_RXNE(SPI1)==0);
    data=LL_SPI_ReceiveData16(SPI1);
    SPI_TX_OD;
    while(LL_SPI_IsActiveFlag_TXE(SPI1)==0);
    LL_SPI_TransmitData16(SPI1,0xFFFF);
    while(LL_SPI_IsActiveFlag_RXNE(SPI1)==0);
    data=LL_SPI_ReceiveData16(SPI1)&0x7FFF;
    NSS_H;
    SPI_TX_PP;
    return data;
}


void WriteValue(uint16_t RegAdd,uint16_t RegValue)
{
  NSS_L;
  while(LL_SPI_IsActiveFlag_TXE(SPI1)==0);
  LL_SPI_TransmitData16(SPI1,RegAdd);
  while(LL_SPI_IsActiveFlag_RXNE(SPI1)==0);
  LL_SPI_ReceiveData16(SPI1);
  while(LL_SPI_IsActiveFlag_TXE(SPI1)==0);
  LL_SPI_TransmitData16(SPI1,RegValue);
  while(LL_SPI_IsActiveFlag_RXNE(SPI1)==0);
  LL_SPI_ReceiveData16(SPI1);
  NSS_H;
}


uint16_t ReadState(void)
{
  return (ReadValue(READ_STATUS));
}


uint16_t ReadAngle(void)
{
  return (ReadValue(READ_ANGLE_VALUE)>>1);
}