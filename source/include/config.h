#ifndef DEMON_CONFIG
#define DEMON_CONFIG

#define SYS_CLK  110592000L

#ifndef EXTERN
	#undef EXTERN
	#define EXTERN  extern
#endif /*EXTERN*/

typedef unsigned char u8;
typedef char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned long u32;
typedef long s32;

#define MAX197_IDLE   0   /*max197 is idle now.*/
#define MAX197_DOING  1   /*max197 is converting now.*/
#define MAX197_DONE   2   /*max197 converting done.*/
EXTERN void max197_initialize(void);
EXTERN void max197_start_ad(u8);
EXTERN u16  max197_async_read_ad(u8);
EXTERN void uart_printf(char *str);
EXTERN void jiaobanqi();

#define CONFIG_NEED_ADERR_REPORT     0
#define CONFIG_ENABLE_AD_DEBUG       0

#define CONFIG_E_JBQ_SPEED           30

#endif /*DEMON_CONFIG*/