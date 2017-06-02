#include "LPC11xx.h"                    // Device header
#include <stdio.h>
#include <string.h>
#define UART_BPS 115200
        char  GcRcvBuf[20];//AD�ɼ�������
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
	LPC_IOCON ->R_PIO0_11 &=~0XBF;//����PIO0_11Ϊģ������ģʽ
	LPC_IOCON ->R_PIO0_11 |=0X02;//PO0_11ģ������ͨ��0
	LPC_SYSCON->PDRUNCFG &=~(0X01<<4);//ADCģ���ϵ�
	LPC_SYSCON ->SYSAHBCLKCTRL |=(0X01<<13);//ʹ��ADCģ��ʱ��
	LPC_ADC ->CR = (0X01<<0)|//SEL=1��ѡ��ADC��ADC0
	               ((SystemCoreClock /1000000-1)<<8)|//ת��ʱ��1M
								 (0<<16)|//�������ת������
								 (0<<17)|//ʹ��1clocksת��
								 (0<<24)|//ADCת��ֹͣ
								 (0<<27);//ֱ������ADCת������λ��Ч

}
void UART_Init()
{
	uint16_t usFdiv;
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<16);//ʹ��IOCONʱ��
	LPC_IOCON->PIO1_6&=~0x07;//��P1.6�ȸ�λ
	LPC_IOCON->PIO1_6|=0x01;//��P1.6����ΪRXD����������
	LPC_IOCON->PIO1_7&=~0x07;//��P1.7�ȸ�λ
	LPC_IOCON->PIO1_7|=0x01;//��P1.7����ΪTXD����������
	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16);//�ر�IOCONʱ��
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);//��UARTʱ��
	LPC_SYSCON->UARTCLKDIV|=0x01;//����UARTʱ�ӷ�ƵֵΪ1
	LPC_UART->LCR=0x83;//0X83������λ1000 0011������Ϊ8λ�ַ����� һ��ֹͣλ ����żУ�� ������Գ����������ķ���
	//LCRΪUART�߿��ƼĴ��������շ��������ַ���ʽ
	
	usFdiv=(SystemCoreClock /LPC_SYSCON ->UARTCLKDIV/16)/UART_BPS;//����ò�����Ҫ��ĳ���������ֵ
	LPC_UART->DLM=usFdiv/256;//д�����������ĸ�8λ
	LPC_UART->DLL=usFdiv%256;//д�����������ĵ�8λ
	//DLM��DLL����UART�ĳ���������DLLΪ��8λ DLMΪ��8λ
	LPC_UART->LCR=0x03;//����������
	LPC_UART->FCR=0x07;//ʹ��FIFO ����8�ֽڴ���
	
}
void UART_SendByte(uint8_t ucDAT)
{
		LPC_UART->THR=ucDAT;//��������
		while((LPC_UART->LSR&0x40)==0);//�ȴ������꣬TEMT��λ
} 
void UART_SendStr(char  *pucStr)
{
	
		while(1)
		{
			if(*pucStr =='\0')break;//�����������˳�
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
			LPC_ADC ->CR |=(1<<24);//����ת��
			while((LPC_ADC->DR[0]&0X80000000)==0);//��ȡDR0��Done
			LPC_ADC->CR |=(1<<24);
			while((LPC_ADC->DR[0]&0X80000000)==0);//��ȡDR0��Done
			ulADCBuf=LPC_ADC ->DR[0];//��ȡ����Ĵ���
			ulADCBuf=(ulADCBuf>>6)&0x3ff;//���� �ӵ���λ��ʼ�洢��ռ��10λ
			ulADCData+=ulADCBuf ;//�����ۼ�
			
		}
		ulADCData=ulADCData/10;//����10�ν����˲�
		ulADCData=(ulADCData*3300)/1024;//������ת����ģ����
		sprintf (GcRcvBuf ,"VINO=%4dmV\r\n",ulADCData);//�����ݷ��͵�������ʾ
		while(times==100)
		{		UART_SendStr(GcRcvBuf );
		times=0;
		}
	LED_ON(ulADCData);
		times++;
	} 
}

