#include "main.h"
#include "display.h"
#include "oled.h"
#include "menu.h"

// Declare menu and menu items (should be in global space)
//struct Menu menuMain;
struct menuItem menuItemCalibrate;
struct menuItem menuItemCurrent;
struct menuItem menuItemStepSize;
struct menuItem menuItemEnablePin;
struct menuItem menuItemDirectionPin;
struct menuItem menuItemClosedLoopMode;
struct menuItem menuItemPID_P;
struct menuItem menuItemPID_I;
struct menuItem menuItemPID_D;
struct menuItem menuItemOLEDFreq;
struct menuItem menuItemSave;
struct menuItem menuItemExit;


// Convert to mA by multiplying by 6.5
uint16_t Converter_Current(uint16_t valueIn)
{
  uint16_t valueOut = (valueIn * 13) / 2;
  return valueOut;
}


// Convert to microstep ratio
uint16_t Converter_Stepsize(uint16_t valueIn)
{
  uint16_t valueOut = 64 / valueIn;
  return valueOut;
}


// Override function for the default item value changer
void Changer_StepSize(struct Menu *menu, int16_t val)
{
  uint8_t newVal;

  if (val > 0)
  {
    newVal = *(uint8_t*)menu->items[menu->selectedItemIndex]->variable.value >> 1;
    if (newVal >= menu->items[menu->selectedItemIndex]->variable.minValue)
      *(uint8_t*)menu->items[menu->selectedItemIndex]->variable.value = newVal;
  }
  else
  {
    newVal = *(uint8_t*)menu->items[menu->selectedItemIndex]->variable.value << 1;
    if (newVal <= menu->items[menu->selectedItemIndex]->variable.maxValue)
      *(uint8_t*)menu->items[menu->selectedItemIndex]->variable.value = newVal;
  } 
}


// Override function for the default item value changer
void Changer_ClosedLoopMode(struct Menu *menu, int16_t val)
{  
  if (val == 1)
  {
    closemode = 1;
    PID_Cal_value_init();    
  }    
  else
  {
    closemode = 0;
  }  
}


void ShowStartupScreen()
{
    OLED_Clear();                              
    OLED_ShowString(0,0,"   TrueStep   ");
}


void ShowInfoScreen()
{
  OLED_ShowString(0,2,"Simp:  0000 RPM");
  OLED_ShowString(0,22," Err:  000.00 ");
  OLED_ShowString(0,42," Deg:  0000.0");
}


void ShowCalibrateScreen()
{
    OLED_Clear();  
    OLED_ShowString(48,16,"NOT");
    OLED_ShowString(16,32,"Calibrated");
    OLED_ShowString(0,48,"Please Calibrate");
}


void ShowCalibrateOKScreen()
{
    OLED_ShowString(16,25,"Calibration");
    OLED_ShowString(48,45,"OK!");
}


void ShowCalibrateCompleteScreen()
{
    OLED_Clear();
    OLED_ShowString(0,16,"   Finished!  ");
    OLED_ShowString(0,32,"  Please Press ");
    OLED_ShowString(0,48,"Reset to Reboot");
}


void ShowBootloaderScreen()
{
    OLED_Clear();
    OLED_ShowString(32,25,"Running");
    OLED_ShowString(16,45,"Bootloader");
}

void ExitMenu()
{
  menuActive = 0;
  OLED_Clear();
  ShowInfoScreen();
}


void BuildMenu()
{
  // Register function that will be used for drawing
  Menu_Register_Draw(OLED_ShowString);

  Menu_Register_Clear(OLED_Clear);

  Menu_Item_Init(&menuItemCalibrate);
  menuItemCalibrate.title = "Calibrate";
  menuItemCalibrate.type = MENU_ITEM_TYPE_ACTION;
  menuItemCalibrate.action = &CalibrateEncoder;

  Menu_Item_Init(&menuItemCurrent);
  menuItemCurrent.title = "Current";
  menuItemCurrent.type = MENU_ITEM_TYPE_VARIABLE_UINT8;
  menuItemCurrent.variable.value = &Currents;
  menuItemCurrent.variable.maxValue = 300;                          // Limit max current to just below 2A
  menuItemCurrent.variable.valueConverter = &Converter_Current;

  Menu_Item_Init(&menuItemStepSize);
  menuItemStepSize.title = "Step Size";
  menuItemStepSize.type = MENU_ITEM_TYPE_VARIABLE_UINT8;                            
  menuItemStepSize.variable.value = &stepangle;  
  menuItemStepSize.variable.maxValue = 32;
  menuItemStepSize.variable.minValue = 1; 
  menuItemStepSize.variable.valueConverter = &Converter_Stepsize;
  menuItemStepSize.variable.change = &Changer_StepSize;             // Override the default value change function        

  Menu_Item_Init(&menuItemEnablePin);
  menuItemEnablePin.title = "EN Pin";                   
  menuItemEnablePin.type = MENU_ITEM_TYPE_VARIABLE_UINT8;
  menuItemEnablePin.variable.value = &Motor_ENmode_flag;  
  menuItemEnablePin.variable.maxValue = 1;

  Menu_Item_Init(&menuItemDirectionPin);
  menuItemDirectionPin.title = "DIR Pin";
  menuItemDirectionPin.type = MENU_ITEM_TYPE_VARIABLE_UINT8;
  menuItemDirectionPin.variable.value = &Motor_Dir;
  menuItemDirectionPin.variable.maxValue = 1;

  Menu_Item_Init(&menuItemClosedLoopMode);
  menuItemClosedLoopMode.title = "Closed M";
  menuItemClosedLoopMode.type = MENU_ITEM_TYPE_VARIABLE_UINT8;
  menuItemClosedLoopMode.variable.value = &closemode;
  menuItemClosedLoopMode.variable.maxValue = 1;
  menuItemClosedLoopMode.variable.minValue = 0;
  menuItemClosedLoopMode.variable.change = &Changer_ClosedLoopMode;   // Override the default value change function

  Menu_Item_Init(&menuItemPID_P);
  menuItemPID_P.title = "PID P";
  menuItemPID_P.type = MENU_ITEM_TYPE_VARIABLE_UINT16;
  menuItemPID_P.variable.value = &kp; 
  
  Menu_Item_Init(&menuItemPID_I);
  menuItemPID_I.title = "PID I";
  menuItemPID_I.type = MENU_ITEM_TYPE_VARIABLE_UINT16;
  menuItemPID_I.variable.value = &ki;

  Menu_Item_Init(&menuItemPID_D);
  menuItemPID_D.title = "PID D";
  menuItemPID_D.type = MENU_ITEM_TYPE_VARIABLE_UINT16;
  menuItemPID_D.variable.value = &kd;

  Menu_Item_Init(&menuItemOLEDFreq);
  menuItemOLEDFreq.title = "OLED Freq";
  menuItemOLEDFreq.type = MENU_ITEM_TYPE_ACTION_VAR_UINT8;
  menuItemOLEDFreq.variable.value = &oledClock;
  menuItemOLEDFreq.action = &ChangeOLEDClock;

  Menu_Item_Init(&menuItemSave);
  menuItemSave.title = "Save";
  menuItemSave.type = MENU_ITEM_TYPE_ACTION;
  menuItemSave.action = &StoreCurrentParameters;

  Menu_Item_Init(&menuItemExit);
  menuItemExit.title = "Exit";
  menuItemExit.type = MENU_ITEM_TYPE_ACTION;
  menuItemExit.action = &ExitMenu;

  Menu_Init(&menuMain);
  Menu_Add_Item(&menuMain, &menuItemCalibrate);
  Menu_Add_Item(&menuMain, &menuItemCurrent);
  Menu_Add_Item(&menuMain, &menuItemStepSize);
  Menu_Add_Item(&menuMain, &menuItemEnablePin);
  Menu_Add_Item(&menuMain, &menuItemDirectionPin);
  Menu_Add_Item(&menuMain, &menuItemClosedLoopMode);
  Menu_Add_Item(&menuMain, &menuItemPID_P);
  Menu_Add_Item(&menuMain, &menuItemPID_I);
  Menu_Add_Item(&menuMain, &menuItemPID_D);
  Menu_Add_Item(&menuMain, &menuItemOLEDFreq);
  Menu_Add_Item(&menuMain, &menuItemSave);
  Menu_Add_Item(&menuMain, &menuItemExit);

}


//
void Motor_data_dis(void)
{
    //uint8_t temp[17]={0};
    uint8_t strRPM[6] = {0};
    uint8_t strERR[8] = {0};
    uint8_t strDEG[8] = {0};

    //int32_t deg_temp=0;
    uint32_t e_temp=0;
    static uint16_t e_a=0,e_b=0;
    
    if(Data_update_flag ==1 /*&& Menu_Num==1*/)
    {
        Data_update_flag=0;
/*************SIMP*****************************************************/
        Motor_speed = wap2-wap1;//
        if(Motor_speed>=0)
            //temp[8]=' ';
            strRPM[0] = ' ';
        else
        {
            //temp[8]='-';
            strRPM[0] = '-';
            Motor_speed = ~Motor_speed ;
            Motor_speed+=1;
        }
        Motor_speed=Motor_speed*60;//
       /* temp[0]=Motor_speed/1000;
        temp[1]=Motor_speed%1000/100;
        temp[2]=Motor_speed%100/10;
        temp[3]=Motor_speed%10;
        */
        strRPM[1] = '0' + Motor_speed/1000;
        strRPM[2] = '0' + Motor_speed%1000/100;
        strRPM[3] = '0' + Motor_speed%100/10;
        strRPM[4] = '0' + Motor_speed%10;
/*************ERR*************************************************/
        if(pid_e>=0)
        {
            //temp[4]=' ';
            strERR[0] = ' ';
            e_temp=pid_e;
        }
        else
        {
            //temp[4]='-';
            strERR[0] = '-';
            e_temp=-pid_e;
        }
        //e_temp=e_temp*0.021972*100;
        e_temp=e_temp*2.1972f;          // JaSw: Changed to float (increases performance and flash memory use)  
        e_a =e_temp/100;                   
        e_b =e_temp%100;                    
        //e_b =(e_temp -e_a) *100;        
/*   
        temp[5]=e_a/100;
        temp[6]=e_a%100/10;
        temp[7]=e_a%10;
//        temp[8]='.';
        temp[9]=e_b/10;
        temp[10]=e_b%10;
*/
        strERR[1] = '0' + e_a/100;
        strERR[2] = '0' + e_a%100/10;
        strERR[3] = '0' + e_a%10;
        strERR[4] = '.';
        strERR[5] = '0' + e_b/10;
        strERR[6] = '0' + e_b%10;

/*************Deg***************************************************/
        if(yw>=0)
        {
            strDEG[0] = ' ';
            e_temp= yw;
        }
        else
        {
            strDEG[0] = '-';
            e_temp=-yw;
        }
        e_temp=e_temp*360*10/16384; //
        if(e_temp>99999)
        {
            
            strDEG[1] = 29;//M
            strDEG[2] = 49;//a
            strDEG[3] = 72;//x
            strDEG[4] = ' ';
            strDEG[5] = ' ';
            strDEG[6] = ' ';
            
        }
        else
        {
            e_a =e_temp/10;                    //
            if(e_a>9999){
                strDEG[1] = 29;//M
                strDEG[2] = 49;//a
                strDEG[3] = 72;//x
                strDEG[4] = ' ';
                strDEG[5] = ' ';
                strDEG[6] = ' ';
            }
            else
            {
                e_b =e_temp%10;                    //
                strDEG[1] = '0' + e_a/1000;
                strDEG[2] = '0' + e_a%1000/100;
                strDEG[3] = '0' + e_a%100/10;
                strDEG[4] = '0' + e_a%10;
                strDEG[5] = '.';
                strDEG[6] = '0' + e_b%10;
            }
        }
/**********显示*****************************************************/ 
    /*    OLED_ShowChar( 48, 2, temp[8], 16, 1);
        OLED_ShowChar( 56, 2, '0'+temp[0], 16, 1);
        OLED_ShowChar( 64, 2, '0'+temp[1], 16, 1);
        OLED_ShowChar( 72, 2, '0'+temp[2], 16, 1);
        OLED_ShowChar( 80, 2, '0'+temp[3], 16, 1);
        */
        OLED_ShowString(48, 2, strRPM);
    /*    
        OLED_ShowChar( 48, 22, temp[4], 16, 1);
        OLED_ShowChar( 56, 22, '0'+temp[5], 16, 1);
        OLED_ShowChar( 64, 22, '0'+temp[6], 16, 1);
        OLED_ShowChar( 72, 22, '0'+temp[7], 16, 1);
//        OLED_ShowChar( 80, 22, temp[8], 16, 1);
        OLED_ShowChar( 88, 22, '0'+temp[9], 16, 1);
        OLED_ShowChar( 96, 22, '0'+temp[10], 16, 1);
      */
        OLED_ShowString(48, 22, strERR);
/*
        OLED_ShowChar( 48, 42, temp[12], 16, 1);
        OLED_ShowChar( 56, 42, '0'+temp[13], 16, 1);
        OLED_ShowChar( 64, 42, '0'+temp[14], 16, 1);
        OLED_ShowChar( 72, 42, '0'+temp[15], 16, 1);
        OLED_ShowChar( 80, 42, '0'+temp[16], 16, 1);
        OLED_ShowChar( 88, 42, '0'+temp[17], 16, 1);
        OLED_ShowChar( 96, 42, '0'+temp[11], 16, 1);
        */
       OLED_ShowString(48, 42, strDEG);
    }
}























