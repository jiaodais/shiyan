#include "LPC11xx.h"                  // Device header
int delay(void)
{
 uint16_t i=4000;
while(i--);

}	
int main(void)
{
	LPC_GPIO2->DIR = 0xFFF;
while(1)
{
LPC_GPIO2->DATA &=~(0XFF);
LPC_GPIO2->DATA |=0XC0;
LPC_GPIO2->DATA &=~(1<<8);
	delay();
	LPC_GPIO2->DATA =0xFFF;
	
	LPC_GPIO2->DATA &=~(0XFF);
LPC_GPIO2->DATA |=0XF9;
LPC_GPIO2->DATA &=~(1<<9);
	delay();
	LPC_GPIO2->DATA =0xFFF;
	
	LPC_GPIO2->DATA &=~(0XFF);
LPC_GPIO2->DATA |=0XA4;
LPC_GPIO2->DATA &=~(1<<10);
	delay();
	LPC_GPIO2->DATA =0xFFF;
	
	LPC_GPIO2->DATA &=~(0XFF);
LPC_GPIO2->DATA |=0XB0;
LPC_GPIO2->DATA &=~(1<<11);
	delay();
	LPC_GPIO2->DATA =0xFFF;
	
	
	
}
}