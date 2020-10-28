/*
 * TLE5012B Driver
 * 
 * Contributors:
 * - Jan Swanepoel
 * 
*/
#include "stdio.h"
#include "stdbool.h"
#include "stm32f0xx_ll_spi.h"
#include "stm32f0xx_ll_gpio.h"

#define NSS_Pin         LL_GPIO_PIN_4
#define NSS_GPIO_Port   GPIOA
#define NSS_H           LL_GPIO_SetOutputPin(NSS_GPIO_Port, NSS_Pin)  
#define NSS_L           LL_GPIO_ResetOutputPin(NSS_GPIO_Port, NSS_Pin) 
#define SPI_TX_OD       LL_GPIO_SetPinOutputType(MOSI_GPIO_Port, MOSI_Pin, LL_GPIO_OUTPUT_OPENDRAIN)
#define SPI_TX_PP       LL_GPIO_SetPinOutputType(MOSI_GPIO_Port, MOSI_Pin, LL_GPIO_OUTPUT_PUSHPULL)
#define MISO_Pin        LL_GPIO_PIN_6
#define MISO_GPIO_Port  GPIOA
#define MOSI_Pin        LL_GPIO_PIN_7
#define MOSI_GPIO_Port  GPIOA

/* SPI command for TLE5012 */
#define READ_STATUS				0x8001			//8000
#define READ_ANGLE_VALUE		0x8021			//8020
#define READ_SPEED_VALUE		0x8031			//8030

#define WRITE_MOD1_VALUE		0x5060			//0_1010_0_000110_0001
#define MOD1_VALUE	0x0001

#define WRITE_MOD2_VALUE		0x5080			//0_1010_0_001000_0001
#define MOD2_VALUE	0x0800

#define WRITE_MOD3_VALUE		0x5091			//0_1010_0_001001_0001
#define MOD3_VALUE	0x0000

#define WRITE_MOD4_VALUE		0x50E0			//0_1010_0_001110_0001
#define MOD4_VALUE	0x0098						//9bit 512

#define WRITE_IFAB_VALUE		0x50B1
#define IFAB_VALUE 0x000D


void TLE5012B_Init();

bool CheckHealth();

// Reads the specified register
uint16_t ReadValue(uint16_t RegAdd);

void WriteValue(uint16_t RegAdd,uint16_t RegValue);

uint16_t ReadState(void);

uint16_t ReadAngle(void);