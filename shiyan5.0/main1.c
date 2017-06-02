#include "LPC11xx.h"                    // Device header
uint8_t table[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uint8_t counter =0;
void PIOINT1_IRQHandler()
{
if((LPC_GPIO1->MIS&(1<<3))==(1<<3))
{counter++;
	LPC_GPIO1->IC=(1<<3);
}if((LPC_GPIO3->MIS&(1<<3))==(1<<3))
{counter--;
	LPC_GPIO1->IC=(1<<3);
}
}

void delay()
{
	
	uint16_t i=4000;
	while(i--);


}


void SEGInit(void)
{
LPC_GPIO2->DIR|=0xFF;
LPC_GPIO3->DIR|=0x0F;
	
}


void KEYInit(void)
{
	LPC_GPIO3->DIR  &=~(1<<3);

	
}


void Display(uint32_t value)
	
{
LPC_GPIO2->DATA &=~(0xFF);

LPC_GPIO2->DATA|=table[value/1000];
	LPC_GPIO2->DATA &=~(1<<8);
delay();
	LPC_GPIO2->DATA|=(1<<8);
	LPC_GPIO2->DATA=0xFF;
	LPC_GPIO2->DATA &=~(0xFF);

	LPC_GPIO2->DATA|=table[value%1000/100];
	LPC_GPIO2->DATA&=~(1<<9);
	delay();
	LPC_GPIO2->DATA|=(1<<9);
	LPC_GPIO2->DATA=0xFF;
	LPC_GPIO2->DATA&=~(1<<11);
	LPC_GPIO2->DATA|=table[value%1000%100/10];
	delay();
	LPC_GPIO2->DATA|=(1<<8);
	LPC_GPIO2->DATA=0xFF;
	
	LPC_GPIO2->DATA&=~(1<<9);
	LPC_GPIO2->DATA |=table[value%1000%1000%10];
	delay();
	LPC_GPIO2->DATA|=(1<<9);
	LPC_GPIO2->DATA=0xFF;
}

int main()
{

SEGInit();
KEYInit();
LPC_GPIO1->IE|=(1<<9);
LPC_GPIO1->IE|=(1<<10);
LPC_GPIO1->IS&=~(1<<9);
LPC_GPIO1->IS&=~(1<<10);
LPC_GPIO1->IEV&=~(1<<9);
LPC_GPIO1->IEV&=~(1<<10);
NVIC_EnableIRQ(EINT1_IRQn);
	while(1)
		
	
	{	
		Display(counter);
	}
}








