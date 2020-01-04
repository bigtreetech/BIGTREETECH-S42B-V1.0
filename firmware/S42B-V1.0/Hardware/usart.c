#include "usart.h"

#define H_ENABLE_STATUS   "Enable =%d \r\n"
#define L_ENABLE_STATUS   "!Enable =%d \r\n"
#define CCW_DIR           "Dir =%d, CCW\r\n"
#define CW_DIR            "Dir =%d, CW\r\n"

//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//STDIO.H串口输出重载函数
int fputc(int c,FILE *stream) 
{	   
  LL_USART_TransmitData8(USART1,c);
  while(!LL_USART_IsActiveFlag_TXE(USART1)) ;//__NOP();

//  USART1->TDR=(uint8_t)c;
//  LL_USART_ClearFlag_TC(USART1); 
  return c;
}
////STDIO.H串口输入重载函数
//int fgetc(FILE *stream) 
//{
//  while(!LL_USART_IsActiveFlag_RXNE(USART1)) __NOP();
//  return ((char)LL_USART_ReceiveData8(USART1));
//}


void MX_USART1_UART_Init(void)
{
//  LL_USART_DeInit(USART1);
    
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
  
  NVIC_SetPriority(USART1_IRQn,4);  
  
  LL_USART_EnableIT_PE( USART1);
  LL_USART_EnableIT_RXNE(USART1);
  LL_USART_EnableIT_IDLE( USART1);
//  LL_USART_EnableDirectionRx(USART1);
 

  LL_USART_Enable(USART1);
  NVIC_EnableIRQ(USART1_IRQn);                          //使能串口中断
                                        
}
//串口数据接收处理
void usart_Receive_Process(void)
{
    static uint8_t t;
//    uint32_t Temporary_val=0;
    uint16_t CRC_val=0;
    if(Communications_Process_flag){
        Communications_Process_flag=0;
        if(Uart_CRC_flag ){                                   //开始校验   
            Uart_CRC_flag=0;
            for(t=2;t<Rx1_temp_num-2;t++){//8-2
                CRC_val+=Rx1_buff[t];
                if(t==Rx1_temp_num-3) CRC_val=CRC_val & 0xff;//数据校验计算
            } 
            if(Rx1_buff[t]==CRC_val){
                Urat_CRC_Correct_flag=1;                    //校验正确标志开
                //返回正确数据信号 OK
                //UART1_SendStr("OK\r\n");
                UART1_SendStr("OK\r\n");
            }
            else{
//                Uart_CRC_Error_flag=1;                      //校验错误标志置位
                //错误待处理,返回错误数据
                UART1_SendStr("CRC verify err\r\n");
            }
        }
        if(Urat_CRC_Correct_flag){               //检验正确标志
            Urat_CRC_Correct_flag=0;
            if(Rx1_buff[2] ==0x05){                 //数据长度正确
                value_Temp = Rx1_buff[4];
                value_Temp<<=8;
                value_Temp|= Rx1_buff[5];
                if(Rx1_buff[3]>=0xA0 && Rx1_buff[3]<=0xBF){
                    switch(Rx1_buff[3]){         //解析指令码
                        case 0xA0:
                                kp = value_Temp;
                                table1[11]=kp;
                                printf("new kp = %d\r\n",kp);
                        break;
                        case 0xA1:
                                ki = value_Temp;
                                table1[12]=ki;
                                printf("new ki = %d\r\n",ki);
                            break;
                        case 0xA2: kd = value_Temp;
                                table1[13]=kd;
                                printf("new kd = %d\r\n",kd);
                            break;
                        case 0xA3:      //set Current 
                                Currents = (value_Temp/100)<<3;
                                Menu_Num2_item = Currents>>3;
                        
                                table1[1]=Currents;
                                table1[2]=Menu_Num2_item;
                            break;                                      //功能命令自行扩展
                        case 0xA4:
                                PID_Cal_value_init();                   /*从新初始化数据，防止调细分后突然位移*/
                                stepangle = 64/value_Temp;
                                //stepangle =Microstep_Set;              //设置电子齿轮
                                table1[3]=stepangle;                    //数组3、4号元素为电子齿轮的设置
                                switch(stepangle & 0x3e){
                                    case 0x02:Menu_Num3_item =4;  break;
                                    case 0x04:Menu_Num3_item =3;  break;
                                    case 0x08:Menu_Num3_item =2;  break;
                                    case 0x10:Menu_Num3_item =1;  break;
                                    case 0x20:Menu_Num3_item =0;  break;
                                    default: Menu_Num3_item =3; break;
                                }
                                table1[4]=Menu_Num3_item;
                            break;
                        case 0xA5:
                                PID_Cal_value_init();                               //重新设置初始值
                                if(0xaa == value_Temp){
                                    Motor_ENmode_flag=0;Menu_Num4_item=1;           //低电平使能
                                }else if(0x55 == value_Temp) {
                                    Motor_ENmode_flag=1;Menu_Num4_item=0;
                                }
                                table1[5]=Motor_ENmode_flag;                        //数组5、6号元素为使能信号的设置
                                table1[6]=Menu_Num4_item;
                            break; 
                        case 0xA6:                                                  //设置电机方向
                                if(0x11 == value_Temp){
                                    Motor_Dir=1;Menu_Num5_item=0;                   //逆时针
                                }else if(0x22 == value_Temp) {
                                    Motor_Dir=0;Menu_Num5_item=1;
                                }
                                table1[7]=Motor_Dir;                                //数组7、8号元素为电方向的设置
                                table1[8]=Menu_Num5_item;
                            break; 
                        case 0xB0: if((Rx1_buff[4]==0xaa) && (Rx1_buff[5]== 0xaa)){                     //读PID参数
                                    t=1;
                                    printf("p ---- kp =%d\r\n",kp);
                                    printf("i ---- ki =%d\r\n",ki);
                                    printf("d ---- kd =%d\r\n",kd);
                                }else{
                                    UART1_SendStr("Read PID err\r\n");
                                }
                            break;
                        case 0xB1: if((Rx1_buff[4]==0xaa) && (Rx1_buff[5]== 0xaa)){                     //读电流参数
                                    t=1;
                                    printf("Currents =%d mA\r\n",(Currents>>3)*100);
                                }else{
                                    UART1_SendStr("Read Current err\r\n");
                                }
                            break;
                        case 0xB2: if((Rx1_buff[4]==0xaa) && (Rx1_buff[5]== 0xaa)){                     //读细分状态
                                    t=1;
                                    printf("Microstep =%d -->%d Gear\r\n",64/stepangle,16384/stepangle);
                                }else{
                                    UART1_SendStr("Read Microstep err\r\n");
                                }
                            break;
                        case 0xB3: if((Rx1_buff[4]==0xaa) && (Rx1_buff[5]== 0xaa)){                     //读使能信号状态
                                    t=1;
                                    if(1 == Motor_ENmode_flag)
                                        printf(H_ENABLE_STATUS,Motor_ENmode_flag);
                                    if (0== Motor_ENmode_flag)
                                        printf(L_ENABLE_STATUS,Motor_ENmode_flag);
                                }else{
                                    UART1_SendStr("Read enable err\r\n");
                                }
                            break;
                                
                        case 0xB4: if((Rx1_buff[4]==0xaa) && (Rx1_buff[5]== 0xaa)){                     //读方向信号状态
                                    t=1;
                                    if(0 == Motor_Dir)
                                        printf(CCW_DIR,Motor_Dir);
                                    if(1 == Motor_Dir)
                                        printf(CW_DIR,Motor_Dir);
                                }else{
                                    UART1_SendStr("Read Dir err\r\n");
                                }
                            break;
                        default: UART1_SendStr("Function Code Undefined\r\n");break;
                    }
                }else{
                    UART1_SendStr("Function Code err\r\n");
                }
                //标记写flash存储
                if(t!=1){
                    flash_store_flag=1;
                }
            }
            else{
                ////错误待处理,返回错误数据
                UART1_SendStr("Data Length err\r\n");
            }
        }
        NVIC_EnableIRQ(USART1_IRQn); //
    }
}
/************发送一个字节*********************************/
void UART1_SendByte(uint8_t data)
{
    LL_USART_TransmitData8(USART1, data);
    /* 等待传输结束 */
    while (LL_USART_IsActiveFlag_TXE(USART1) == RESET);
}

/*******************************************************************************
* 名称: UART1_SendByte
* 功能: UART1发送一个字节
* 形参: data -> 要发送的字节
* 返回: 无
* 说明: 无 
******************************************************************************/
uint8_t UART1_ReceiveByte(void)
{
    uint8_t UART1_RX_BUF; 
     
    /* 等待接收完成 */
    while (LL_USART_IsActiveFlag_RXNE(USART1) == RESET);
    UART1_RX_BUF = LL_USART_ReceiveData8(USART1);
    return  UART1_RX_BUF;
}
/*******************************************************************************
* 名称: UART1_SendStr
* 功能: UART1发送len个字符
* 形参: data -> 指向要发送的字符串
*       len -> 要发送的字节数
* 返回: 无
* 说明: 无 
******************************************************************************/
void UART1_SendStr(char *str)
{
    while(*str != '\0')
    {
        UART1_SendByte(*str++); /* 循环调用发送一个字符函数 */
    }   
}
//#if 0
////简单的ASICC控制协议，可以直接使用printf和scanf函数进行扩展
//void SerialCheck(void) 
//{        
//  printf("----- HyperStepper 20180829 -----\r\n");
//  uint8_t quit=0;
//  char command;
//  while(!quit)
//  {
//    printf("Edit PID gains:\r\n");
//    printf("p ----- kp =%d\r\n",kp);
//    printf("i ----- ki =%d\r\n",ki);
//    printf("d ----- kd =%d\r\n",kd);
//    printf("q ----- quit\r\n");
//	printf("Please input your command:\r\n"); 
//	scanf("%c",&command);
//	printf("Your input command is:%c\r\n",command);
//    switch(command) 
//	{
//      case 'p':
//      {
//        printf("kp = ?\r\n");
//		scanf("%hd",&kp);
//        printf("new kp = %d\r\n",kp);
//      }
//      break;
//      case 'i':
//      {
//        printf("ki = ?\r\n");
//		scanf("%hd",&ki);
//		printf("new ki = %d\r\n",ki);
//      }
//      break;
//      case 'd':
//      {
//        printf("kd = ?\r\n");
//		scanf("%hd",&kd);
//        printf("new kd = %d\r\n",kd);
//      }
//      break;
//      case 'q':
//      {  
//        quit=1;
//        printf("done...\r\n");
//      }
//      default:
////      {quit=1;}
//      break;
//    }
//  }
//}
//#endif

//串口中断服务函数
void USART1_IRQHandler(void)
{
//    uint8_t bb;
    if(USART1->ISR & (1<<5))//接收到数据 //LL_USART_IsActiveFlag_IDLE
    {
        Rx1_buff[Receive_Count++]= USART1->RDR;             //接收数据的同时会清除中断标志
        
//        bb=USART1->RDR; 
//        USART1->TDR=bb;// 将接收到的数据返回给串口助手
    }
    else if(LL_USART_IsActiveFlag_IDLE(USART1) == 1){
        LL_USART_ClearFlag_IDLE(USART1);                    //清除空闲中断标志
        //Receive_finish_flag=1;
        if((Rx1_buff[0]==0xfe)&&(Rx1_buff[1]==0xfe)&&(Rx1_buff[Receive_Count-1]==0x16))//判断是否为帧头和帧尾                                
        {
//            LL_USART_Disable(USART1);                   //一帧接收成功后关闭串口使能和串口中断，次方发适用于是适用于实时性不高的情况
            NVIC_DisableIRQ(USART1_IRQn);

            //Rx_Datanum==Rx_Databuff[13]+16;            //接收数据是否等于数据计算个数
            Rx1_temp_num=Receive_Count;                  
            Receive_Count=0;
//            Communications_flag=1;                      //数据传输完成
            Communications_Process_flag=1;              //通信处理标志
            Uart_CRC_flag=1;                            //数据校验标志开
        }
        else{
            Receive_Count=0;
            //帧错误
             frame_Error_flag=1;   
        }  
    }
}

void PID_Cal_value_init(void)
{
//    enmode =0;
 /*从新初始化数据，防止调细分后突然位移*/
    s=0;
    s_1=0;
    
    s_sum=0;
    r=0;
    r_1=0;
    y=0;
    y_1=0;
    yw=0;
    e=0;
    u=0;
    dterm=0;
    //LL_TIM_SetCounter(TIM1,0);
    WRITE_REG(TIM1->CNT, 0);
    
    r=*(volatile uint16_t*)((ReadValue(READ_ANGLE_VALUE)>>1)*2+0x08008000); //指令电机位置
    s_sum=r;   //外部控制脉冲输入累计值
    y=r;
    y_1=y;
    yw=y;  
    yw_1=yw;
    
}




















