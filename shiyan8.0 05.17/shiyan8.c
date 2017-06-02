#include "LPC11xx.h"                    // Device header
#define UART_BPS 115200
uint8_t ucRcvData=0xff; 
void UART_Init(void)
{
	uint16_t usFdiv;
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<16);
	LPC_IOCON->PIO1_6&=~0x07;
	LPC_IOCON->PIO1_6|=0x01;
	LPC_IOCON->PIO1_7&=~0x07;
	LPC_IOCON->PIO1_7|=0x01;
	LPC_SYSCON->SYSAHBCLKCTRL&=~(1<<16);
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<12);
	LPC_SYSCON->UARTCLKDIV=0x01;
	LPC_UART->LCR=0x83;
	usFdiv=(SystemCoreClock/LPC_SYSCON->UARTCLKDIV/16)/UART_BPS;
	LPC_UART->DLM=usFdiv/256;
	LPC_UART->DLL=usFdiv%256;
	LPC_UART->LCR=0x03;
	LPC_UART->FCR=0x07;
}
void delay(void)
{
	uint16_t i=4000;
	while(i--);
}
void LED_ON(ucBuf)
{
		LPC_GPIO2->DATA &=~(0xff);
		LPC_GPIO2->DATA |= ucBuf;
		delay();
		LPC_GPIO2->DATA = 0xFF;
}
uint8_t UART_GetByte(void)
{

	while((LPC_UART->LSR&0x01)==0)
			LED_ON(ucRcvData);
	;
	ucRcvData=LPC_UART->RBR;
	return(ucRcvData);
}
void UART_SendByte(uint8_t ucDat)
{
	LPC_UART->THR=ucDat;
	while((LPC_UART->LSR&0x040)==0);
}
void LEDInit(void)
{
	LPC_GPIO2->DIR = 0xFF;
}


int main(void)
{
	uint8_t ucBuf;
	UART_Init();
	LEDInit();
	while(1)
	{
		ucBuf=UART_GetByte();
		UART_SendByte(ucBuf);
	}
}