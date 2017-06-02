#include "LPC11xx.h"                    // Device header

void delay(void)
{ int i,j;
	for (i=0;i < 5000;i++)
for (j=0;j<200;j++);

}
int main(void){
//LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);// 
//LPC_IOCON->PIO2_5 &=0xF8;
LPC_GPIO2->DIR |=(1<<5);
	
	while(1)
	{
		LPC_GPIO2->DATA &=~(1<<5);
		delay();
		
		LPC_GPIO2->DATA |=(1<<5);
		delay();
		
	}

}
