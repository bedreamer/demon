/*
 * MAX485ģ��
 * 
 * ��;��MAX485ģ����ղ��Գ���
 * 
 * ����					����				��ע
 * Huafeng Lin			2011/11/22			����
 * Huafeng Lin			2011/11/22			�޸�
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
    TMOD=0x20;   	//��ʱ��1��������ʽ2              
    TH1=0xfd;		//������Ϊ9600
    TL1=0xfd;
    PCON=0x00;              
    TR1=1;     		//��ʱ��1��ʼ���� 
    SCON=0x50;		//0101 0000B��������ʽ1,8λUART�������ʿɱ䣬�������             
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
	SBUF=ShowData;	 		//д���ͼĴ���
	while(TI==0)		 	//�ȴ����з������
	{
	;
	}
	TI=0;  					//�Ա�־λ����
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