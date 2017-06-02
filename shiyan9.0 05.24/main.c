#include "LPC11xx.h"                    // Device header
#include <stdio.h>
#include <string.h>
#define UART_BPS 115200
        char  GcRcvBuf[20];//AD采集的数据
				uint8_t  table[9]={0xFF,0x07F,0x03F,0x01F,0x0F,0x07,0x03,0x01,0x00};
void delay(uint32_t ulTime)
{
	uint32_t i;
	i = 0;
	while(ulTime--)
	{
		for(i=0;i<5000;i++);
		
	}	
}
void delay_mine(void)
{
uint16_t i=4000;
	while(i--);


}

void ADC_Init(void)
{
	LPC_SYSCON ->SYSAHBCLKCTRL|=(1<<16);
	LPC_IOCON ->R_PIO0_11 &=~0XBF;//配置PIO0_11为模拟输入模式
	LPC_IOCON ->R_PIO0_11 |=0X02;//PO0_11模拟输入通道0
	LPC_SYSCON->PDRUNCFG &=~(0X01<<4);//ADC模块上电
	LPC_SYSCON ->SYSAHBCLKCTRL |=(0X01<<13);//使能ADC模块时钟
	LPC_ADC ->CR = (0X01<<0)|//SEL=1，选择ADC即ADC0
	               ((SystemCoreClock /1000000-1)<<8)|//转换时钟1M
								 (0<<16)|//软件控制转换操作
								 (0<<17)|//使用1clocks转换
								 (0<<24)|//ADC转换停止
								 (0<<27);//直接启动ADC转换，此位无效

}
void UART_Init()
{
	uint16_t usFdiv;
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<16);//使能IOCON时钟
	LPC_IOCON->PIO1_6&=~0x07;//将P1.6先复位
	LPC_IOCON->PIO1_6|=0x01;//将P1.6设置为RXD即接收数据
	LPC_IOCON->PIO1_7&=~0x07;//将P1.7先复位
	LPC_IOCON->PIO1_7|=0x01;//将P1.7设置为TXD即发送数据
	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16);//关闭IOCON时钟
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);//打开UART时钟
	LPC_SYSCON->UARTCLKDIV|=0x01;//设置UART时钟分频值为1
	LPC_UART->LCR=0x83;//0X83二进制位1000 0011即设置为8位字符长度 一个停止位 无奇偶校验 且允许对除数锁存器的访问
	//LCR为UART线控制寄存器决定收发的数据字符格式
	
	usFdiv=(SystemCoreClock /LPC_SYSCON ->UARTCLKDIV/16)/UART_BPS;//计算该波特率要求的除数锁存器值
	LPC_UART->DLM=usFdiv/256;//写除数锁存器的高8位
	LPC_UART->DLL=usFdiv%256;//写除数锁存器的低8位
	//DLM与DLL均是UART的除数锁存器DLL为低8位 DLM为高8位
	LPC_UART->LCR=0x03;//锁定波特率
	LPC_UART->FCR=0x07;//使能FIFO 设置8字节触发
	
}
void UART_SendByte(uint8_t ucDAT)
{
		LPC_UART->THR=ucDAT;//发送数据
		while((LPC_UART->LSR&0x40)==0);//等待发送完，TEMT置位
} 
void UART_SendStr(char  *pucStr)
{
	
		while(1)
		{
			if(*pucStr =='\0')break;//遇到结束符退出
			UART_SendByte(*pucStr++);  
		}
}
void LED_Init()
{  
		LPC_GPIO2 ->DIR |= 0xFF;
		
}

void LED_ON(ucBuf)
{
	LPC_GPIO2->DATA =~table[ucBuf/410];
	delay_mine();

}

int main(void)
{
	uint32_t i;
	uint32_t ulADCData;
	uint32_t ulADCBuf;
	int times;
	ADC_Init();
	UART_Init();
	LED_Init();
	while(1)
	{
		ulADCData=0;
		for(i=0;i<10;i++)
		{
			LPC_ADC ->CR |=(1<<24);//立即转换
			while((LPC_ADC->DR[0]&0X80000000)==0);//读取DR0的Done
			LPC_ADC->CR |=(1<<24);
			while((LPC_ADC->DR[0]&0X80000000)==0);//读取DR0的Done
			ulADCBuf=LPC_ADC ->DR[0];//读取结果寄存器
			ulADCBuf=(ulADCBuf>>6)&0x3ff;//数据 从第六位开始存储，占据10位
			ulADCData+=ulADCBuf ;//数据累加
			
		}
		ulADCData=ulADCData/10;//采样10次进行滤波
		ulADCData=(ulADCData*3300)/1024;//数字量转换成模拟量
		sprintf (GcRcvBuf ,"VINO=%4dmV\r\n",ulADCData);//将数据发送到串口显示
		while(times==100)
		{		UART_SendStr(GcRcvBuf );
		times=0;
		}
	LED_ON(ulADCData);
		times++;
	} 
}

