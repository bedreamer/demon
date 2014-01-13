#include "../include/stdafx.h"

sbit JDQ = P1 ^ 2;
u16 ttl = 15, times = 0, ttljdq = 0, thizspeed = 100;
#define RX_SIZE 12
u8 rx_nr=0, rx_i=0;
u8 rx_buf[RX_SIZE];
u8 keypush = 0;

void isr_timer0() interrupt 1
{
    TH0 = 0xD4; //每一次进入中断都要给定时器赋值，以确定下一次进入中断的时间
    TL0 = 0xCC;

    P1M1 = 0x00;
    P1M0 = 0xFF;
    times ++;

    if (P1 & 0xFE)
        keypush ++;
    else keypush = 0;
/*
    if ( keypush > 10)
        ENABLE_ALRM;
*/
    if (!STATUS_MC) {
        JDQ = 0;
        DISABLE_MC;
    } else {
        ENABLE_MC;
        JDQ = 1;
    }

    if ( times >= ttl ) {
        times = 0;
        jiaobanqi();
    }
}

void isr_uart() interrupt 4
{
    u8 t, x;
    u8 *rx_cmd = (u8*)&cs;

    if (rx_nr > RX_SIZE) rx_nr = 0;
    if (rx_i > RX_SIZE) rx_i = 0;

    if ( RI ) {
        RI = 0;
        rx_buf[rx_i] = SBUF;
        x = rx_buf[rx_i];
        if ( rx_buf[0] != 0x55 ) {
            while ( rx_buf[0] != 0x55 && rx_nr > 0) {
                for ( t = 0; t < RX_SIZE - 1; t ++ )
                    rx_buf[ t ] = rx_buf[ t + 1 ];
                rx_nr --, rx_i --;
            }
            if ( x == 0x55 ) {
                rx_nr = 1, rx_i = 1;
            }
        } else {
            if ( rx_nr == 0 ) { 
                rx_nr = 1, rx_i = 1;
            } else {
                rx_nr ++, rx_i ++;
                if ( rx_nr >= RX_SIZE ) {
                    for ( x = 0; x < RX_SIZE; x ++ )
                        rx_cmd[ x ]  = rx_buf[ x ];
                    for ( x = 0; x < RX_SIZE - 2; x ++ )
                        rx_buf[ x ] = rx_buf[ x + 2 ];
                    rx_nr --, rx_i --;
                    SET_NEW_CMD;
                }
            }
        }
    } else {
        TI = 0;
    }
}