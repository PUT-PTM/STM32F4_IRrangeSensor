#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

GPIO_InitTypeDef GPIOStructure;
TIM_OCInitTypeDef OutputChannelInit = {0,};
TIM_TimeBaseInitTypeDef TimerInitStructure;

void Delay(__IO uint32_t nCount)
{
	while(nCount--)
	{
	}
}

void Timer()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TimerInitStructure.TIM_Prescaler = 0;
    TimerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TimerInitStructure.TIM_Period = 420;
    TimerInitStructure.TIM_ClockDivision = 0;
    TimerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TimerInitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

void PWM()
{
    OutputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
    OutputChannelInit.TIM_Pulse = 0;
    OutputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    OutputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC3Init(TIM2, &OutputChannelInit);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_OC4Init(TIM2, &OutputChannelInit);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);
}

void LEDy()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIOStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIOStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIOStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIOStructure);
}

void Piny()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIOStructure.GPIO_Pin = GPIO_Pin_1;
    GPIOStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIOStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIOStructure);

    GPIOStructure.GPIO_Pin = GPIO_Pin_2;
    GPIOStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIOStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIOStructure);

    GPIOStructure.GPIO_Pin = GPIO_Pin_3;
    GPIOStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIOStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIOStructure);
}

int main()
{
    LEDy();
    Piny();
    Timer();
    PWM();

    while(1)
    {
    	int x = 0;

    	TIM2->CCR3 = 210;
    	TIM2->CCR4 = 0;
    	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
    	{
    		x=1;
    	}
    	Delay(0xFFFF);

    	TIM2->CCR3 = 0;
    	TIM2->CCR4 = 210;
    	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
    	{
    	    x+=2;
    	}
    	Delay(0xFFFF);

    	switch (x)
    	{
			case 0: //brak przeszkod
				GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
			break;
			case 1: //przeszkoda po lewej
			{
				GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
				GPIO_SetBits(GPIOD, GPIO_Pin_12);
				break;
			}
			case 2: //przeszkoda po prawej
			{
				GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
				GPIO_SetBits(GPIOD, GPIO_Pin_14);
				break;
			}
			case 3: //przeszkoda na wprost
			{
				GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
				GPIO_SetBits(GPIOD, GPIO_Pin_13 | GPIO_Pin_15);
				break;
			}
    	}
    	Delay(0xFFFF);
    }
}
