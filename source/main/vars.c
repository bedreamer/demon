#include "../include/stdafx.h"

u8 addr = 0x55;
u16 T0 = 0x0000, T1 = 0x0000;
u16 PR0 = 0x0000, PR1 = 0x0000, PR2 = 0x0000;
u8 V0 = 0x00, V1 = 0x00, V2 = 0x00;
u8 now_V0 = 0x00, now_V1 = 0x00, now_V2 = 0x00;
u32 status = 0x00000000;
u32 inner_flags = 0x00000000;
u16 rcv_chars = 0x0000;
u8 alrm = 0;
struct cmd_struct cs, valid_cs;
struct return_struct rs;