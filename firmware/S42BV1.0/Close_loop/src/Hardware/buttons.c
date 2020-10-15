#include "buttons.h"

// Macros used to handle buttons
#define PIN_CONFIRM         LL_GPIO_IsInputPinSet(KEY_Confirm_GPIO_Port,KEY_Confirm_Pin)
#define PIN_BACK            LL_GPIO_IsInputPinSet(KEY_Back_GPIO_Port,KEY_Back_Pin)
#define PIN_SELECT          LL_GPIO_IsInputPinSet(KEY_Select_GPIO_Port,KEY_Select_Pin)

#define KEY_SCAN_INTERVAL   50          // Set key scan interval in ms

extern volatile uint32_t tickCount;
uint32_t prevTickCount = 0;


bool KeyScan(uint8_t* key)
{
    // Scan keys at lower rate to debounce
    if ((tickCount - prevTickCount) < KEY_SCAN_INTERVAL)
        return false;

    prevTickCount = tickCount;

    *key = 0;

    if(PIN_CONFIRM == 0)
    {
        //LL_mDelay(10);
        if(PIN_CONFIRM == 0)
        {
            *key += KEY_PRESSED_CONFIRM;
        }
    }

    if(PIN_BACK == 0)
    {       
        //LL_mDelay(10);
        if(PIN_BACK == 0)
        {
            *key += KEY_PRESSED_BACK;
        }
    }

    if(PIN_SELECT == 0)
    {       
        //LL_mDelay(10);
        if(PIN_SELECT == 0)
        {
            *key += KEY_PRESSED_SELECT;
        }
    }

    if (*key > 0)
        return true;

    return false;
};