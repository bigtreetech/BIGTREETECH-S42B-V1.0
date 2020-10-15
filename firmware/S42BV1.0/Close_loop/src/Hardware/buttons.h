/*
 * Handles hardware buttons
 * 
 * Contributors:
 * - Jan Swanepoel
 * 
 */
#include "stdio.h"
#include "stdbool.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_utils.h"

#define KEY_Select_Pin          LL_GPIO_PIN_15      
#define KEY_Select_GPIO_Port    GPIOA               
#define KEY_Back_Pin            LL_GPIO_PIN_11      
#define KEY_Back_GPIO_Port      GPIOA  
#define KEY_Confirm_Pin         LL_GPIO_PIN_8       
#define KEY_Confirm_GPIO_Port   GPIOA 

// Key press bit field
enum KEYS_PRESSED
{
    KEY_PRESSED_CONFIRM = 1,
    KEY_PRESSED_BACK = 2,
    KEY_PRESSED_SELECT = 4
};

// Scan the buttons to see if they are pressed
bool KeyScan(uint8_t* key);