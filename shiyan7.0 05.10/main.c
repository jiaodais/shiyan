#include "LPC11xx.h"                    // Device header
void CT32B1_Init(uint32_t interval)
{
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<16);//��ʱ��
	LPC_IOCON->R_PIO1_2&=~(0x07);//�Ƚ����Ÿ�λ
	LPC_IOCON->R_PIO1_2|=0x03;//������1.2����ΪCT32B1_MAT1���
	LPC_SYSCON->SYSAHBCLKCTRL &=~(1<<16);//�ر�ʱ��
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<10);//��32λ��ʱ������1��ʱ��
	LPC_TMR32B1->TC=0X02;//TC��λ
	LPC_TMR32B1->PR=0x00;//Ԥ��ƵΪ0
	LPC_TMR32B1->MCR=(0x02<<9);//TC��MR3ƥ��ʱ��λ
	LPC_TMR32B1->PWMC =0x02;//CT32B1_MAT1��ΪPWM���
	LPC_TMR32B1->MR1=interval/2;//CT32B1_MAT1��������ռ�ձ�
	LPC_TMR32B1->MR3=interval;//PWM������
	LPC_TMR32B1->TCR =0x01;//������ʱ��
}
void Key_Init()
{
	LPC_GPIO3->DIR&=~(1<<0);
	LPC_GPIO3->DIR&=~(1<<1);//����P3.0��P3.1Ϊ����
	LPC_GPIO3->IE|=(1<<0);
	LPC_GPIO3->IE|=(1<<1);//����P3.0 P3.1�����ж�
	LPC_GPIO3->IS&=~(1<<0);
	LPC_GPIO3->IS&=~(1<<1);//����P3.0 P3.1 Ϊ���ش���
	LPC_GPIO3->IEV&=~(1<<0);
	LPC_GPIO3->IEV&=~(1<<1);//����P3.0 P3.1Ϊ�½��ش���
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
	CT32B1_Init(SystemCoreClock/1000);//������Ϊ1ms
	Key_Init();
	while(1)
	{
		;
	}
}
	