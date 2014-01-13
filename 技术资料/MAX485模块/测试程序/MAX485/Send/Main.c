/*
 * MAX485模块
 * 
 * 用途：MAX485模块接收测试程序
 * 
 * 作者					日期				备注
 * Huafeng Lin			2011/11/22			新增
 * Huafeng Lin			2011/11/22			修改
 * 
 */

#include <reg52.h>

#define uchar unsigned char
#define uint  unsigned int

uchar DAT = 0;

sbit Key1 = P3^2;
sbit  CS1 = P1^1;
sbit  CS2 = P1^2;

void Delay(uint k)
    {
        uchar j;
        while((k--)!=0)           
        	{
                for(j=0;j<125;j++)
                	{;}
        	}
    }


void Init(void)
{
    TMOD=0x20;   	//定时器1，工作方式2              
    TH1=0xfd;		//波特率为9600
    TL1=0xfd;
    PCON=0x00;              
    TR1=1;     		//定时器1开始工作 
    SCON=0x50;		//0101 0000B，工作方式1,8位UART，波特率可变，允许接收             
	RI = 0;
	TI = 0;           
}            

void KeyCount(void)
{
	 if(Key1 == 0)
	 {
	 	  Delay(50);
		  if(Key1 == 0)
		  {
		  	DAT++;
		  }
	 }
}
                              
void Send(uchar ShowData)
{
	SBUF=ShowData;	 		//写发送寄存器
	while(TI==0)		 	//等待串行发送完成
	{
	;
	}
	TI=0;  					//对标志位清零
}

void main(void)
{
	//P1 = 0;
    Init();
 	CS = 1;
    while(1)
    {
		KeyCount();
        Send(DAT);
        Delay(100);
    }
}