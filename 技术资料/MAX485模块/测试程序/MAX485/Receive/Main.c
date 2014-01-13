/*
 * MAX485ģ��
 * 
 * ��;��MAX485ģ����Գ���
 * 
 * ����					����				��ע
 * Huafeng Lin			2011/11/22			����
 * Huafeng Lin			2011/11/22			�޸�
 * 
 */

#include <reg52.h>
#include<stdio.h>
#include<math.h>
#include<INTRINS.H>

#define uchar unsigned char
#define uint  unsigned int

#define DataPort P2	   //LCD1602 ���ݶ˿�
	
sbit    LCM_RS=P0^2;   //LCD1602 ���ƶ˿�		
sbit    LCM_RW=P0^1;   //LCD1602 ���ƶ˿�	
sbit    LCM_EN=P0^0;   //LCD1602 ���ƶ˿�

uchar DAT2 = 0;
sbit  CS1 = P1^1;
sbit  CS2 = P1^2;

//********���庯��*****************************
void    delay(unsigned int k);
void    InitLcd();
void    WriteDataLCM(uchar dataW);
void    WriteCommandLCM(uchar CMD,uchar Attribc);
void    DisplayOneChar(uchar X,uchar Y,uchar DData);

//void Delay(uchar k)
//{
//    uchar j;
//    while((k--)!=0)           
//	{
//        for(j=0;j<125;j++)
//    	{;}
//	}
//}

//*********LCD1602��ʼ��**********************
void InitLcd()				
{			
	WriteCommandLCM(0x38,1);	
	WriteCommandLCM(0x08,1);	
	WriteCommandLCM(0x01,1);
	WriteCommandLCM(0x06,1);	
	WriteCommandLCM(0x0c,1);
}

//**********���æ�ź�************************
void WaitForEnable(void)	
{					
	DataPort=0xff;		
	LCM_RS=0;LCM_RW=1;_nop_();
	LCM_EN=1;_nop_();_nop_();
	while(DataPort&0x80);	
	LCM_EN=0;				
}
					
//**********д������LCD***********************
void WriteCommandLCM(uchar CMD,uchar Attribc)
{					
	if(Attribc)WaitForEnable();	
	LCM_RS=0;LCM_RW=0;_nop_();
	DataPort=CMD;_nop_();	
	LCM_EN=1;_nop_();_nop_();LCM_EN=0;
}	
				
//**********д������LCD************************
void WriteDataLCM(uchar dataW)
{					
	WaitForEnable();		
	LCM_RS=1;LCM_RW=0;_nop_();
	DataPort=dataW;_nop_();	
	LCM_EN=1;_nop_();_nop_();LCM_EN=0;
}
					
//*********дһ���ַ����ݵ�ָ����Ŀ��***********
void DisplayOneChar(uchar X,uchar Y,uchar DData)
{						
	Y&=1;						
	X&=15;						
	if(Y)X|=0x40;					
	X|=0x80;			
	WriteCommandLCM(X,0);		
	WriteDataLCM(DData);		
}

void display_1602(uchar *DData,X,Y)//��ʾ����  
{ 
	uchar ListLength=0;
	Y&=0x01;
	X&=0x0f;
	while(X<16)
	{
	    DisplayOneChar(X,Y,DData[ListLength]);
	    ListLength++;
	    X++;
	}
}

//**********��ʱ����***************
void delay(unsigned int k)	
{						
	unsigned int i,j;				
	for(i=0;i<k;i++)
	{			
		for(j=0;j<121;j++)			
		{;}
	}						
}

void Init(void)
{
    TMOD=0x20;   	//��ʱ��1��������ʽ2              
    TH1=0xfd;		//������Ϊ9600
    TL1=0xfd;
    PCON=0x00;              
    TR1=1;     		//��ʱ��1��ʼ���� 
    SCON=0x50;		//0101 0000B��������ʽ1,8λUART�������ʿɱ䣬�������             
	RI = 0;
	TI = 0;   
} 

void Receive(void)
{                       
	while(RI==0)
	{
	;
	}
	DAT2=SBUF;
	RI=0; 
}

void main(void)
{
	delay(10); 
	InitLcd();      //lcd��ʼ��
	display_1602("(C)2011 LCSTUDIO", 0, 0);

	//P1 = 0;
	Init();
	CS1 = 0;
	CS2 = 0;

	while(1)
	{
		Receive();
		delay(100);
		//P1 = DAT2;

		DisplayOneChar(0, 1, (DAT2/100)+0x30);
		DisplayOneChar(1, 1, ((DAT2%100)/10)+0x30);
		DisplayOneChar(2, 1, (DAT2%10)+0x30);

		DisplayOneChar(15, 1, DAT2);
	}
}