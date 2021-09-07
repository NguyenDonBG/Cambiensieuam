/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f10x_conf.h"
#include "delay.h"
#include "driver_uart.h"
#define PIN_SRF05_TRIGGER    GPIO_Pin_0
#define PIN_SRF05_ECHO       GPIO_Pin_1
#define SOUND_SPEED			((float)0.0452f)
void GPIO_ultra_sonnic_config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = PIN_SRF05_TRIGGER;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin   = PIN_SRF05_ECHO;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, PIN_SRF05_TRIGGER);
}

float SRF05_Read_Data(void)
{
    uint32_t time, time_out = 1000000;
    float distance;
    GPIO_ResetBits(GPIOA, PIN_SRF05_TRIGGER);
    delay_us(2);
    GPIO_SetBits(GPIOA, PIN_SRF05_TRIGGER);
    delay_us(10);
    while(!GPIO_ReadInputDataBit(GPIOA, PIN_SRF05_ECHO))
    {
        if(time_out -- == 0)
        {
            return 1;
        }
    }
    time = 0;
    while(GPIO_ReadInputDataBit(GPIOA, PIN_SRF05_ECHO))
    {
        time++;
        delay_us(1);
    }
    distance = (float)time*SOUND_SPEED;
    return distance;
}
int main(void)
{
    GPIO_ultra_sonnic_config();
    SysTick_Init();
    UART1_Init_A9A10();
    while(1)
    {
        printf("%0.2f \n",SRF05_Read_Data());
        delay_ms(500);
    }
}
