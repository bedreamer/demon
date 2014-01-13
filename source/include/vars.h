#ifndef _VARS_INCLUDED_H_
#define _VARS_INCLUDED_H_

#define MASK_UNSED 0x0101010F

#define MASK_ALRM    0x00000004
#define STATUS_ALRM  (MASK_ALRM & status)
#define ENABLE_ALRM  (status = MASK_ALRM | status)
#define DISABLE_ALRM (status = (~MASK_ALRM) & status)

#define MASK_MC    0x00000008
#define STATUS_MC  (MASK_MC & status)
#define ENABLE_MC  (status = MASK_MC | status)
#define DISABLE_MC (status = (~MASK_MC) & status)

#define MASK_M0    0x00000100
#define STATUS_M0  (MASK_M0 & status)
#define ENABLE_M0  (status = MASK_M0 | status)
#define DISABLE_M0 (status = (~MASK_M0) & status)

#define MASK_M1    0x00010000
#define STATUS_M1  (MASK_M1 & status)
#define ENABLE_M1  (status = MASK_M1 | status)
#define DISABLE_M1 (status = (~MASK_M1) & status)

#define MASK_J    0x01000000
#define STATUS_J  (MASK_J & status)
#define ENABLE_J  (status = MASK_J | status)
#define DISABLE_J (status = (~MASK_J) & status)
/*----------------------------------------------*/
#define NEW_CMD_MASK 0x80000000
#define STATUS_NEW_CMD (NEW_CMD_MASK & inner_flags)
#define SET_NEW_CMD   (inner_flags = NEW_CMD_MASK | inner_flags)
#define CLR_NEW_CMD   (inner_flags = (~NEW_CMD_MASK) & inner_flags)

#define B2L(d)  (d >> 24) | ( (d >> 16) )

extern u8 addr;
extern u16 T0, T1;
extern u16 PR0, PR1, PR2;
extern u8 V0, V1, V2;
extern u8 now_V0, now_V1, now_V2;
extern u32 status, inner_flags;
extern struct cmd_struct cs, valid_cs;
extern volatile u16 rcv_chars;
extern struct return_struct rs;

extern volatile u8 speed_jbq_set;
extern u8 keypush;
extern u8 alrm;

#endif /*_VARS_INCLUDED_H_*/