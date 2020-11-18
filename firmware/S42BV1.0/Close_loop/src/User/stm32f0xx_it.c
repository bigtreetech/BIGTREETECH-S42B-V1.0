#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "tle5012b.h"


void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  while (1)
  {
  }
} 


void SVC_Handler(void)
{
}

void PendSV_Handler(void)
{
}

// Handle System Timer interrupt. Configured for 1ms ticks.
void SysTick_Handler(void)
{
  tickCount++;
}


// Handle DIR pin interrupt
void EXTI0_1_IRQHandler(void)
{
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
    if(DIRIN == 1)
    {
      if(Motor_Dir == 1)
      {
        LL_TIM_SetCounterMode(TIM1,LL_TIM_COUNTERMODE_UP);
      }
      else
      {
        LL_TIM_SetCounterMode(TIM1,LL_TIM_COUNTERMODE_DOWN);
      }
    }
	  else
    {
      if(Motor_Dir == 1)
      {
        LL_TIM_SetCounterMode(TIM1,LL_TIM_COUNTERMODE_DOWN);
      }
      else
      {
        LL_TIM_SetCounterMode(TIM1,LL_TIM_COUNTERMODE_UP);
      }
    }
  }
}


// Handle EN pin interrupt
void EXTI2_3_IRQHandler(void)
{
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
        
    if(ENIN==1)
      enmode=1;
    else
    {
      enmode=0;
      LL_TIM_OC_SetCompareCH1(TIM3,0);  
      LL_TIM_OC_SetCompareCH2(TIM3,0);  
    }
      
    if(ENIN==0)
      enmode=1;
    else
    {
      enmode=0;
      LL_TIM_OC_SetCompareCH1(TIM3,0);  
      LL_TIM_OC_SetCompareCH2(TIM3,0);   
    }
  }
}


// Handle Timer 6 interrupt which performs the control loop.
void TIM6_IRQHandler(void)
{
  if(LL_TIM_IsActiveFlag_UPDATE(TIM6) == 1)
  {
    LL_TIM_ClearFlag_UPDATE(TIM6);
    
    // Kick the dog
    LL_IWDG_ReloadCounter(IWDG);

    // Increase counter (used elsewhere for 100us ticks)
    tim6Counter++;

    if(enmode==1)
    {
      if(closemode==1) 
      { 
        // Read angle sensor and apply correction from calibration table   
        y =* (volatile uint16_t*)((ReadValue(READ_ANGLE_VALUE)>>1) * 2 + 0x08008000);

        // Read counter steps
        s = LL_TIM_GetCounter(TIM1);
        
        // Handles cases where the 16bit TIM1 ticks over. 
        // s_sum (which is an offset for the desired angle) is then adjusted. 
        if(s - s_1 < -32768)
          s_sum += stepangle * 65536;
        else if(s - s_1 > 32768)
          s_sum -= stepangle * 65536;

        // Calculate the desired angle (r)
        r = s_sum + stepangle * s;
        s_1 = s;
      
        if(y-y_1>8192)
        { 
          wrap_count--;  
          start_measu_V_flag = 1;  
        }
        else if(y - y_1 < -8192) 
        {
          wrap_count++; 
          start_measu_V_flag=1;
        }
        else 
          measure_once_flag =0;

        // Calculate the actual angle (yw) 
        yw = y + 16384 * wrap_count; 

        // Calculate the error          
        pid_e = r - yw;

        // Switch LED on error 
        if(pid_e > 1638)
        {
            pid_e=1638;
            LED_H;  
        }
        else if(pid_e < -1638)
        {
          pid_e=-1638;
          LED_H;  
        }
        else 
        {
          LED_L;
        }
        
        iterm += ki * pid_e / 32;

        #if 1
          if(iterm > UMAXSUM)
            iterm = UMAXSUM;
          else if(iterm < -UMAXSUM) 
            iterm =- UMAXSUM; 
        #else 
                  if(iterm>(Currents*128))//积分饱和限制
                    iterm = Currents*128;
                  else if(iterm<-(Currents*128)) 
                    iterm = -(Currents*128); 
        #endif
            
        dterm = LPFA * dterm / 128 - LPFB * kd * (yw - yw_1) / 8;
        u=(kp * pid_e + iterm + dterm) / 128; //PID
        
        // JaSw: Lyk of advance die verskil tussen die vorige en huidige hoek is wat gemaal word met n konstante. Dit word dan gebruik as theta
        // en die uitset van die PID (u) word dan die effort.
        //For feedforward angle calculation, change 1.5 to 2 can increase the speed a little, if the motor is easy to run and lose, change to 1.2
        advance=(yw-yw_1)*2;//前馈角计算，1.5改为2可以提高些转速，如果电机容易跑丢改为1.2 
        y_1=y;  
        yw_1=yw;
    
        if(u>0)            
        {
  //		  if(advance>68)//
  //		    advance=68;
  //          else if(advance<0)
  //			advance=0; 		  
          y+=(82+advance);//
        }
        else if(u<0)
        {
  //		  if(advance<-68)
  //		    advance=-68; 
  //		  else if(advance>0)
  //			advance=0; 
          y-=(82-advance);
          u=-u;
        }
          //
        #if 1
          // Limit the current
          if(u>Currents)
          {
            u=Currents;//电流矢量最大值限制
            //LED_H;
          }
        #else 
          if(u>UMAXCL)     
          {
            u=UMAXCL;//电流矢量最大值限制
            LED_H;
          }
        #endif
  //        else
  //          LED_L;
          Output(y,u);    //JaSw: 'u' is type int16, while output expects uint8_t ?
      }           
        else 
      {		
        // JaSw: Open loop mode
        s = LL_TIM_GetCounter(TIM1);
        if(s - s_1 < -32768)
          s_sum += stepangle * 65536;
        else if(s - s_1 > 32768)
          s_sum -= stepangle * 65536;
      
        r = s_sum + stepangle * s;
        s_1 = s;
      
        // Check if motor is in idle
        if(r == r_1)
        {
          hccount++;
          if(hccount>=1000)
            hccount=1000;
        }
        else
          hccount=0;
        
        // When in idle reduce current to half
        if(hccount >= 1000)
          Output(r,UMAXOP/2);
        else
          Output(r,UMAXOP);
        r_1 = r;
      }
      
      // Disable motor speed calculation while in tuning mode.
      // This is to make sure this controller loop uses the minimum time.
      if (!tuningMode)
      {
        Motor_speed_count++;
          if(start_measu_V_flag==1)
          {
              Data_update_Count++;
              if(Data_update_Count>=10000)
              {       //
                  Data_update_Count=0;
                  wap1=wap2;
                  wap2=wrap_count;
                  start_measu_V_flag=0;           //
                  measure_once_flag =1;           //
                  
              }
          }
          if(Motor_speed_count>=2000)
          {
              Motor_speed_count-=2000;
              Data_update_flag=1;           //
              
              if(measure_once_flag == 0)    //  
              start_measu_V_flag=1;
              if(wap1== wap2) start_measu_V_flag=0; 
          }
      }   
    }
  }
}




