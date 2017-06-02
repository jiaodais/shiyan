#include "LPC11xx.h"                    // Device header
void CT32B1_Init(uint32_t interval)
{
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<16);//打开时钟
	LPC_IOCON->R_PIO1_2&=~(0x07);//先将引脚复位
	LPC_IOCON->R_PIO1_2|=0x03;//将引脚1.2设置为CT32B1_MAT1输出
	LPC_SYSCON->SYSAHBCLKCTRL &=~(1<<16);//关闭时钟
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<10);//打开32位定时计数器1的时钟
	LPC_TMR32B1->TC=0X02;//TC复位
	LPC_TMR32B1->PR=0x00;//预分频为0
	LPC_TMR32B1->MCR=(0x02<<9);//TC与MR3匹配时复位
	LPC_TMR32B1->PWMC =0x02;//CT32B1_MAT1作为PWM输出
	LPC_TMR32B1->MR1=interval/2;//CT32B1_MAT1输出脉冲的占空比
	LPC_TMR32B1->MR3=interval;//PWM的周期
	LPC_TMR32B1->TCR =0x01;//启动定时器
}
void Key_Init()
{
	LPC_GPIO3->DIR&=~(1<<0);
	LPC_GPIO3->DIR&=~(1<<1);//设置P3.0、P3.1为输入
	LPC_GPIO3->IE|=(1<<0);
	LPC_GPIO3->IE|=(1<<1);//允许P3.0 P3.1产生中断
	LPC_GPIO3->IS&=~(1<<0);
	LPC_GPIO3->IS&=~(1<<1);//设置P3.0 P3.1 为边沿触发
	LPC_GPIO3->IEV&=~(1<<0);
	LPC_GPIO3->IEV&=~(1<<1);//设置P3.0 P3.1为下降沿触发
	NVIC_EnableIRQ(EINT3_IRQn);
}
void PIOINT3_IRQHandler()
{
	uint32_t interval=SystemCoreClock/1000;
	
	if((LPC_GPIO3->MIS&(1<<0))==(1<<0))
	{
		interval =interval+(SystemCoreClock/1000)/5;
		LPC_TMR32B1->MR1=(interval/2);
		LPC_GPIO3->IC=(1<<0);
	}
	
	if((LPC_GPIO3->MIS&(1<<1))==(1<<1))
	{
		interval =interval-(SystemCoreClock/1000)/5;
		LPC_TMR32B1->MR1=(interval/2);
		LPC_GPIO3->IC=(1<<1);
	}
	

}
int main()
{
	CT32B1_Init(SystemCoreClock/1000);//周期设为1ms
	Key_Init();
	while(1)
	{
		;
	}
}
	