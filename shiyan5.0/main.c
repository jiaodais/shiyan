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
	LPC_GPIO2->DIR|=0xFFF;//将P2_0~P2_7设置为输出
	LPC_GPIO2->DATA|=(1<<8);//将P2_8~p2_11设置为高电平
	LPC_GPIO2->DATA|=(1<<9);
	LPC_GPIO2->DATA|=(1<<10);
	LPC_GPIO2->DATA|=(1<<11);
	LPC_GPIO3->DIR&=~(0xFF);//将控制按键的p3_0~p3_4都设置为输入
	
	LPC_GPIO3->IE|=(1<<0);
	LPC_GPIO3->IE|=(1<<1);
	LPC_GPIO3->IE|=(1<<2);
	LPC_GPIO3->IE|=(1<<3);
	LPC_GPIO3->IE|=(1<<4);//允许p3_0~p3_4上产生中断
	
	LPC_GPIO3->IS&=~(1<<0);
	LPC_GPIO3->IS&=~(1<<1);
	LPC_GPIO3->IS&=~(1<<2);
	LPC_GPIO3->IS&=~(1<<3);
	LPC_GPIO3->IS&=~(1<<4);//允许p3_0~p3_4上的中断边沿触发
	
	LPC_GPIO3->IEV&=~(1<<0);
	LPC_GPIO3->IEV&=~(1<<1);
	LPC_GPIO3->IEV&=~(1<<2);
	LPC_GPIO3->IEV&=~(1<<3);
	LPC_GPIO3->IEV&=~(1<<4);//允许p3_0~p3_4上的中断下降沿有效
	NVIC_EnableIRQ(EINT3_IRQn);
	while(1)
	{
		Display(counter);
	}
	
}

