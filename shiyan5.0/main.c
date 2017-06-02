#include "LPC11xx.h"   // Device header
uint8_t table[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uint8_t counter=0;
void delay(int time )
{
	 int a ,b;
    for(time = a;a >= 0;a--)
        for(b = 20;b >= 0;b--);
}
void Display(int value)
{
	LPC_GPIO2->DATA&=~(0XFF);
	LPC_GPIO2->DATA&=~(1<<8);
	LPC_GPIO2->DATA|=table [value/1000];
	delay(200);
	LPC_GPIO2->DATA|=(1<<8);
	//LPC_GPIO2->DATA=0xFF;
	
	LPC_GPIO2->DATA&=~(0XFF);
	LPC_GPIO2->DATA&=~(1<<9);
	LPC_GPIO2->DATA|=table [value%1000/100];
	delay(200);
	LPC_GPIO2->DATA|=(1<<9);
	//LPC_GPIO2->DATA=0xFF;
	
	LPC_GPIO2->DATA&=~(0XFF);
	LPC_GPIO2->DATA&=~(1<<10);
	LPC_GPIO2->DATA|=table [value%1000%100/10];
	delay(200);
	LPC_GPIO2->DATA|=(1<<10);
	//LPC_GPIO2->DATA=0xFF;
	
	LPC_GPIO2->DATA&=~(0XFF);
	LPC_GPIO2->DATA&=~(1<<11);
	LPC_GPIO2->DATA|=table [value%1000%100%10];
	delay(200);
	LPC_GPIO2->DATA|=(1<<11);
	//LPC_GPIO2->DATA=0xFF;
	
	
}

 void PIOINT3_IRQHandler()
{
	if((LPC_GPIO3->MIS&(1<<0))==(1<<0))
	{
		counter ++;
		LPC_GPIO3->IC=(1<<0);
	}
	
	if((LPC_GPIO3->MIS&(1<<1))==(1<<1))
	{
		counter ++;
		LPC_GPIO3->IC=(1<<1);
	}
	
	if((LPC_GPIO3->MIS&(1<<2))==(1<<2))
	{
		counter ++;
		LPC_GPIO3->IC=(1<<2);
	}
	
	if((LPC_GPIO3->MIS&(1<<3))==(1<<3))
	{
		counter ++;
		LPC_GPIO3->IC=(1<<3);
	}
	
	if((LPC_GPIO3->MIS&(1<<4))==(1<<4))
	{
		counter ++;
		LPC_GPIO3->IC=(1<<4);
	}
}

int main()
{
	LPC_GPIO2->DIR|=0xFFF;//��P2_0~P2_7����Ϊ���
	LPC_GPIO2->DATA|=(1<<8);//��P2_8~p2_11����Ϊ�ߵ�ƽ
	LPC_GPIO2->DATA|=(1<<9);
	LPC_GPIO2->DATA|=(1<<10);
	LPC_GPIO2->DATA|=(1<<11);
	LPC_GPIO3->DIR&=~(0xFF);//�����ư�����p3_0~p3_4������Ϊ����
	
	LPC_GPIO3->IE|=(1<<0);
	LPC_GPIO3->IE|=(1<<1);
	LPC_GPIO3->IE|=(1<<2);
	LPC_GPIO3->IE|=(1<<3);
	LPC_GPIO3->IE|=(1<<4);//����p3_0~p3_4�ϲ����ж�
	
	LPC_GPIO3->IS&=~(1<<0);
	LPC_GPIO3->IS&=~(1<<1);
	LPC_GPIO3->IS&=~(1<<2);
	LPC_GPIO3->IS&=~(1<<3);
	LPC_GPIO3->IS&=~(1<<4);//����p3_0~p3_4�ϵ��жϱ��ش���
	
	LPC_GPIO3->IEV&=~(1<<0);
	LPC_GPIO3->IEV&=~(1<<1);
	LPC_GPIO3->IEV&=~(1<<2);
	LPC_GPIO3->IEV&=~(1<<3);
	LPC_GPIO3->IEV&=~(1<<4);//����p3_0~p3_4�ϵ��ж��½�����Ч
	NVIC_EnableIRQ(EINT3_IRQn);
	while(1)
	{
		Display(counter);
	}
	
}

