/*****************************************************************************
 *   can.h:  Header file for NXP LPC230x Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/ 
#ifndef __CAN_H 
#define __CAN_H
/******************************************************************************/ 
#ifdef  CAN_Ex
   #define Extern_CAN          //定义变量
#else
   #define Extern_CAN  extern  //声明变量
#endif

#include "absacc.h"
/******************************************************************************/ 
#define ACCEPTANCE_FILTER_ENABLED	1

#define CAN_MEM_BASE		0xE0038000  //验收滤波器RAM: 0xE003 8000 - 0xE003 87FF

#define MAX_PORTS	2		/* Number of CAN port on the chip */		

/* BRP+1 = Fpclk/(CANBitRate * QUANTAValue)
   QUANTAValue = 1 + (Tseg1+1) + (Tseg2+1)
   QUANTA value varies based on the Fpclk and sample point
   e.g. (1) sample point is 87.5%, Fpclk is 48Mhz
   the QUANTA should be 16
        (2) sample point is 90%, Fpclk is 12.5Mhz
   the QUANTA should be 10 
   Fpclk = Fclk /APBDIV
   or
   BitRate = Fcclk/(APBDIV * (BRP+1) * ((Tseg1+1)+(Tseg2+1)+1))
*/ 	
/* Here are some popular bit timing settings for LPC23xx, google on "SJA1000"
CAN bit timing, the same IP used inside LPC2000 CAN controller. There are several 
bit timing calculators on the internet. 
http://www.port.de/engl/canprod/sv_req_form.html
http://www.kvaser.com/can/index.htm
*/

/* Bit Timing Values for 64MHz clk frequency */
/* Bit Timing Values for 64MHz clk frequency */
//APBDIV  = 1
//Fcclk   = 64MHz/4
//BitRate = Fcclk/(APBDIV * (39+1) * ((12+1)+(1+1)+1))//20K
//BitRate = Fcclk/(APBDIV * (1+1) * ((14+1)+(3+1)+1)) //400K
//BitRate = Fcclk/(APBDIV * (126+1) * ((14+1)+(4+1)+1)) //6K
#define BITRATE1M64MHZ            0x001C0000  //1835008
#define BITRATE500K64MHZ          0x001C0001  //1835009
#define BITRATE400K64MHZ          0x003E0001  //4063233
#define BITRATE250K64MHZ          0x001C0003  //1835011
#define BITRATE88K64MHZ           (9|(0<<14)|(12<<16)|(3<<20))//3932169
#define BITRATE25K64MHZ           0x001C0027  //1835047
#define BITRATE20K64MHZ           0x001C0031  //1835057
#define BITRATE6K64MHZ            (126|(0<<14)|(14<<16)|(4<<20))//5111934
//#define BITRATE6K64MHZ            0x001C00A5  //1835173
#define BITRATE5K64MHZ            0x001C00C8  //1835208

/* Bit Timing Values for 16MHz clk frequency */
#define BITRATE100K16MHZ          0x001C0009
#define BITRATE125K16MHZ          0x001C0007
#define BITRATE250K16MHZ          0x001C0003
#define BITRATE500K16MHZ          0x001C0001
#define BITRATE1000K16MHZ         0x001C0000
/* Bit Timing Values for 24MHz clk frequency */
#define BITRATE100K24MHZ          0x001C000E
#define BITRATE125K24MHZ          0x001C000B
#define BITRATE250K24MHZ          0x001C0005
#define BITRATE500K24MHZ          0x001C0002
#define BITRATE1000K24MHZ         0x00090001
/* Bit Timing Values for 48MHz clk frequency */
#define BITRATE100K48MHZ          0x001C001D
#define BITRATE125K48MHZ          0x001C0017
#define BITRATE250K48MHZ          0x001C000B
#define BITRATE500K48MHZ          0x001C0005
#define BITRATE1000K48MHZ         0x001C0002
/* Bit Timing Values for 60MHz clk frequency */
#define BITRATE100K60MHZ          0x00090031
#define BITRATE125K60MHZ          0x00090027
#define BITRATE250K60MHZ          0x00090013
#define BITRATE500K60MHZ          0x00090009
#define BITRATE1000K60MHZ         0x00090004
/* Bit Timing Values for 28.8MHz pclk frequency, 1/2 of 576.Mhz CCLK */
#define BITRATE100K28_8MHZ        0x00090017
/* 
   When Fcclk is 50Mhz and 60Mhz and APBDIV is 4,
   so Fpclk is 12.5Mhz and 15Mhz respectively. 
   when Fpclk is 12.5Mhz, QUANTA is 10 and sample point is 90% 
   when Fpclk is 15Mhz, QUANTA is 10 and sample point is 90% 
*/

/* Common CAN bit rates for 12.5Mhz(50Mhz CCLK) clock frequency */
#define BITRATE125K12_5MHZ		  0x00070009
#define BITRATE250K12_5MHZ		  0x00070004
/* Bit Timing Values for 15MHz(60Mhz CCLK) clk frequency */
#define BITRATE100K15MHZ		  0x0007000E
#define BITRATE125K15MHZ		  0x0007000B
#define BITRATE250K15MHZ		  0x00070005
#define BITRATE500K15MHZ		  0x00070002

/* Acceptance filter mode in AFMR register */
#define ACCF_OFF				0x01
#define ACCF_BYPASS			0x02
#define ACCF_ON					0x00
#define ACCF_FULLCAN		0x04

/* 
   This number applies to all FULLCAN IDs, explicit STD IDs, group STD IDs, 
   explicit EXT IDs, and group EXT IDs. 
*/ 
#define ACCF_IDEN_NUM			4

/* Identifiers for FULLCAN, EXP STD, GRP STD, EXP EXT, GRP EXT */
//#define FULLCAN_ID				0x100
//#define EXP_STD_ID				0x100
//#define GRP_STD_ID				0x200
//#define EXP_EXT_ID				0x000000
//#define GRP_EXT_ID				0x200000

Extern_CAN DWORD Can1ResetEnable;
Extern_CAN DWORD Can1ErrCount;
Extern_CAN DWORD Can2ResetEnable;
Extern_CAN DWORD Can2ErrCount;
Extern_CAN DWORD Can1RxPush;
Extern_CAN DWORD Can1RxPop;
Extern_CAN DWORD Can1TxPush;
Extern_CAN DWORD Can1TxPop;

Extern_CAN DWORD Can2RxPush;
Extern_CAN DWORD Can2RxPop;
Extern_CAN DWORD Can2TxPush;
Extern_CAN DWORD Can2TxPop;

Extern_CAN CAN_MSG Can1RxFifo[64];
Extern_CAN CAN_MSG Can1TxFifo[64];
Extern_CAN CAN_MSG Can2RxFifo[64];
Extern_CAN CAN_MSG Can2TxFifo[64];
/**************************************************************************
PUBLIC FUNCTIONS
***************************************************************************/
void CANx_Init( DWORD N, DWORD CAN_BTR );
void CAN_Init( void );
DWORD CAN_SendMessage( DWORD N, CAN_MSG *pTxBuf );

void CanRx_Pop(void);
void CanTx_Pop(void);
#endif
/******************************************************************************
**                            End Of File
******************************************************************************/

