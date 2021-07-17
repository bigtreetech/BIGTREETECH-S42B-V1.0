#ifndef __DISPLAY_H
#define __DISPLAY_H


#include "main.h"

struct Menu menuMain;

// Create the menu
void BuildMenu(void);

void Motor_data_dis(void);

void ShowStartupScreen();

void ShowInfoScreen();

void ShowCalibrateScreen();

void ShowCalibrateOKScreen();

void ShowCalibrateCompleteScreen();

void ShowBootloaderScreen();

#endif

