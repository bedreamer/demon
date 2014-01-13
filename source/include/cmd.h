#ifndef _CMD_INCLUDED_H_
#define _CMD_INCLUDED_H_

struct cmd_struct {
	u8 addr;
	u8 cmd;
	u16 rsv0;
	u32 parm;
	u16 rsv1;
	u16 crc;
};

struct return_struct {
	u8 addr;
	u32 rt;
	u8 rsv;
	u16 crc;
};

typedef void (*CMD_SRV_PROC)() reentrant;

const CMD_SRV_PROC cmd_srvs[];
code u8 i_srv[];
EXTERN u16 crc16(u8 *pchMsg, u8 wDataLen);

#endif /*_CMD_INCLUDED_H_*/
