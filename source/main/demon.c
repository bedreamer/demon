#include "../include/stdafx.h"
#if 1
sbit ADHBEN = P4^6;   //HBEN口
sbit ADCS = P4^5;
sbit ADWR = P3^6;
sbit ADRD = P3^7;
sbit ADINT0 = P4^4;

//sfr IE2     = 0xAF;                 //中断使能寄存器2
sfr T4T3M 	= 0xD1;                 //定时器3和定时器4控制寄存器
sfr T4H     = 0xD2;                 //定时器4高8位
sfr T4L     = 0xD3;                 //定时器4低8位
sfr T3H     = 0xD4;                 //定时器3高8位
sfr T3L     = 0xD5;                 //定时器3低8位

sbit JDQ = P1^2;

u16  max197_read_ad(u8 ch);
void fack_proc(void);
u8 check_cmd_valid(void);
static void delay()
{
    u8 seed = 12;
    for (;seed; seed --);
}
#define CHECK_FACK_EWQUEST  fack_proc()
#define RS485_RX() delay();P1 &= ~0x20
#define RS485_TX() delay();P1 |= 0x20

void uart_send(const u8 *buf, u8 len)
{
    u8 i;

    RS485_TX();

    for (i=0; i< len ;i ++ ) {
        SBUF = buf[ i ];
        while(TI==0);
        TI = 0;
    }

    RS485_RX();
}

#if (CONFIG_ENABLE_AD_DEBUG>=1)
void uart_print_u16(u16 d)
{
    SBUF = d >> 8;
    while(TI==0);
    TI = 0;
    SBUF = d;
    while(TI==0);
    TI = 0;
    SBUF = '\n';
    while(TI==0);
    TI = 0;
}

void uart_printf(char *str)
{
    while ( * str ) {
        SBUF = *str;
        while(TI==0);
        TI = 0;
        str ++;
    }
}

void yy_delay(u16 ttl)
{
    while (ttl) ttl --;
}
#else
#define uart_print_u16(d)
#define uart_printf(d)
#define yy_delay(d)
#endif
unsigned int tom = 0;

void main()
{
#if 1
    SCON  = 0x50;          // SCON: 模式 1, 8-bit UART, 使能接收  
    TMOD |= 0x22;          // TMOD: timer 1, mode 2, 8-bit 重装
                           //       timer 0, mode 2, 16-bit 
    TH0   = 0xFF;
    TL0   = 0xF7;
    TH1   = 0xFD;          // TH1:  重装值 9600 波特率 晶振 11.0592MHz  
    TR1   = 1;             // TR1:  timer 1 打开  
    TR0   = 1;
    ET0   = 1;
    ES = 1;

    EA    = 1;             //打开总中断

    P0M1 = 0x00;
    P0M0 = 0x00;

    /*---PWM---*/
    CMOD = 0X80;
    CCON = 0X00;
    CH = 0;
    CL = 0;

    CCAPM0 = 0x42;
    PCA_PWM0 = 0x00;

    CCAPM1 = 0x42;
    PCA_PWM1 = 0x00;
    CR = 1;

    CCAP0L = 0;
    CCAP1L = 0;
    CCAP0H = 0;
    CCAP1H = 0;

    /*
    P2M1 &= 0x0F;
    P2M0 |= 0xF0;
    */

    /*设置P4^4 5 6 为普通I/O口.*/
    P4SW |= 0x10 | 0x20 | 0x40;

    /*P4.4 为ADINT0, 仅输入. */
    P4M1 |= 0x10;
    P4M0 &= ~(0x10);

    /*P4.5, P4.6 CS, HBEN 仅推挽输出.*/
    P4M1 &= ~(0x60);
    P4M0 |= 0x60;

    ADCS = 1;
    ADWR = 1;
 
    //P1M0 |= 0x04;
    //P1M1 &= ~0x04;
    P1 &= 0xFB;
    #if 0
	uart_printf("*******************Welcome to MCU world !******************\n");
    uart_printf("####let's begin!\n");
    #endif
    P1 |= 0x02;


    //P3M0 |= 0x3C;
    //P3M1 &= (~0x3C); 
    P3 |= 0x04;
    P3 |= 0x10;
    P3 &= ~(0x08);
    P3 &= ~(0x20);
    
    RS485_RX();

	while(1)
	{
        CHECK_FACK_EWQUEST;
        PR0 = max197_read_ad(0);
        CHECK_FACK_EWQUEST;
        PR1 = max197_read_ad(1);
        CHECK_FACK_EWQUEST;
        PR2 = max197_read_ad(2);
        CHECK_FACK_EWQUEST;
        T0 =  max197_read_ad(3);
        CHECK_FACK_EWQUEST;
        T1 =  max197_read_ad(4);
        CHECK_FACK_EWQUEST;

        if (!STATUS_M0) {
            P3 &= ~0x04;
        } else {
            P3 |= 0x04;
        }
        if (!STATUS_M1) {
            P3 &= ~0x10;
        } else {
            P3 |= 0x10;
        }
        if (!STATUS_J) {
            P2 &= ~0x08;
        } else {
            P2 |= 0x08;
        }

        if (!STATUS_ALRM) {
            P1 |= 0x02;
        } else {
            P1 &= ~0x02;
        }
    }
#else
    P1 |= 0xA2;
    while (1) {
        for (status = 0; status < 10000; status ++ )
            P1 &= 0x00;
        for (status = 0; status < 20000; status ++ )
            P1 |= 0x02;
    }
#endif
}

#define AD_DBUS      P0
#define AD_START     ADCS = 0; ADWR = 0; ADRD = 1
#define AD_STOP      ADWR = 1; ADRD = 1; ADCS = 1
u16  max197_read_ad(u8 ch)
{
#if (CONFIG_NEED_ADERR_REPORT>0)
    u16 j;
#endif /*(CONFIG_NEED_ADERR_REPORT>0)*/
    u16 i;
    u8 adh, adl;

    AD_START;

    P0M1 = 0x00;
    P0M0 = 0xFF;
    i = 0;
    while (i++<100);
    P0 = 0x40 | (ch & 0x07);

    ADCS = 1;
    ADWR = 1;
    i = 0;
    while (i++<100);

    P0M1 = 0xFF;
    P0M0 = 0x00;

    i = 0;
    ADINT0 = 1;
    while ( ADINT0 && i ++ < 5000 );

#if (CONFIG_NEED_ADERR_REPORT>0)
    if ( ADINT0 ) j = 1;
    else j = 0;
#endif /*(CONFIG_NEED_ADERR_REPORT>0)*/

    ADCS = 0;
    ADRD = 0;
    ADHBEN = 0;

    i = 0;
    while (i++<100);       
    adl = AD_DBUS;

    ADHBEN = 1;
    i = 0;
    while (i++<100);
    adh = AD_DBUS;

    AD_STOP;

#if (CONFIG_ENABLE_AD_DEBUG > 0)
    //uart_print_u16(adl | (adh << 8));
#endif /*CONFIG_ENABLE_AD_DEBUG*/

#if (CONFIG_NEED_ADERR_REPORT > 0)
    return (( adh & 0x0F ) << 8) | adl | (j ? 0x8000 : 0);
#else
    return (( adh & 0x0F ) << 8) | adl;
#endif /*(CONFIG_NEED_ADERR_REPORT>0)*/
}

#define speed_jbq_set V2
#define speed_jbq_current now_V2
u16 jbq_speed_ttl = 0, e_jbq_speed = 0;
void jiaobanqi()
{
    const u8 jbqtools[] = {
        0x08, 0x0A, 0x02, 0x06,
        0x04, 0x05, 0x01, 0x09, 0x00
    };
    static u8 thiz = 0;
    u8 _p2;

    if ( 0 != speed_jbq_set ) {

        if ( speed_jbq_set > speed_jbq_current ) {
            if (e_jbq_speed > CONFIG_E_JBQ_SPEED) {
                speed_jbq_current ++;
                e_jbq_speed = 0;
            } else {
                e_jbq_speed ++;
            }
        } else if ( speed_jbq_set < speed_jbq_current ) {
            if (e_jbq_speed > CONFIG_E_JBQ_SPEED) {
                speed_jbq_current --;
                e_jbq_speed = 0;
            } else {
                e_jbq_speed ++;
            }
        } else {
            e_jbq_speed = 0;
        }

        jbq_speed_ttl ++;
        if ( jbq_speed_ttl > 100 - speed_jbq_current ) {
            jbq_speed_ttl = 0;
            if ( jbqtools[ thiz ] == 0 ) thiz = 0;
            _p2 = P2 & 0x07;
            P2 = ( (jbqtools[ thiz ] & 0x0F) << 4 )| 0x08 | _p2;
            thiz ++;
        }
    } else {
        speed_jbq_current = 0;
    }
}
#endif

void fack_proc()
{
    CMD_SRV_PROC src_proc;
    u8 i, j;

    if ( STATUS_NEW_CMD ) {
        if ( check_cmd_valid() ) {

            i = i_srv[ valid_cs.cmd ];
            src_proc = cmd_srvs[ i ];

            (*src_proc)();
            rcv_chars = 0;

            rs.crc = crc16((u8*)&rs, sizeof(struct return_struct) - sizeof(u16));
            i = rs.crc & 0xFF;
            j = rs.crc >> 8;
            rs.crc = (i << 8) | j;
            //memcpy(&rs, &cs, sizeof(rs));
            uart_send((u8*)&rs, sizeof(rs));

        }
        CLR_NEW_CMD;
    }
    return;
}

u8 check_cmd_valid()
{
    u16 crc = crc16((u8*)&cs, sizeof(cs)-2);
    u8 h, l;
    h = crc & 0xff;
    l = crc >> 8;
    crc = (h << 8) | l;
    if (crc == cs.crc ) {
        memcpy(&valid_cs, &cs, sizeof(cs));
        return 1;
    }
    return 0;
}
