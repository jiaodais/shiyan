#include "LPC11xx.h"                    // Device header
void PIOINT3_IRQHandler(void)
{
	
	
	if((LPC_GPIO3->MIS &(1<<2))==(1<<2))
	{
		LPC_GPIO2->DATA &=-(1<<6);
		LPC_GPIO3->IC |=(1<<2);
	}
}  

int main(void)
{
LPC_GPIO2->DIR|=(1<<2);
LPC_GPIO3->DIR&=-(1<<2);
LPC_GPIO3->IE |=(1<<2);
LPC_GPIO3->IS &=(1<<2);
LPC_GPIO3->IEV &=(1<<2);
NVIC_EnableIRQ(EINT3_IRQn);
	while(1)
	{
}
}


