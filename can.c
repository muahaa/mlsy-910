/*****************************************************************************
 *  can.c:  CAN module API file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.13  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include "LPC23xx.h"					/* LPC23xx definitions */
#include "type.h"
#include "irq.h"
#include "target.h"
#include "timer.h"
#define  CAN_Ex
#include "can.h"
#include "LiftCan.h"
/*****************************************************************************
** Function name:		CAN_Handler
*****************************************************************************/
void CAN_Handler(void) __irq 
{
  if ( CAN_RX_SR & (1 << 8) ) {
     Can1ResetEnable = 0;
		 if ( (((Can1RxPush+1)&0x3f) != Can1RxPop) && (((CAN1RDA&0xffff)==0x0134)||((CAN1RDA&0xff)<31)||((CAN1RDA&0xff)==36)) ) {//直接屏蔽30号帧以上数据
          (Can1RxFifo[Can1RxPush].Frame) = CAN1RFS;
          (Can1RxFifo[Can1RxPush].MsgID) = CAN1RID; 		
          (Can1RxFifo[Can1RxPush].DataA) = CAN1RDA;
          (Can1RxFifo[Can1RxPush].DataB) = CAN1RDB;
		    Can1RxPush = ((Can1RxPush+1)&0x3f);
     }
     CAN1CMR = 0x04;
  }
  if ( CAN_RX_SR & (1 << 9) ) {
     Can2ResetEnable = 0;	 		
     if ( (((Can2RxPush+1)&0x3f) != Can2RxPop) && (((CAN2RDA&0xffff)==0x0134)||((CAN2RDA&0xff)<31)||((CAN2RDA&0xff)==36)) ) {//直接屏蔽30号帧以上数据
          (Can2RxFifo[Can2RxPush].Frame) = CAN2RFS;
          (Can2RxFifo[Can2RxPush].MsgID) = CAN2RID; 		
          (Can2RxFifo[Can2RxPush].DataA) = CAN2RDA;
          (Can2RxFifo[Can2RxPush].DataB) = CAN2RDB;
		    Can2RxPush = ((Can2RxPush+1)&0x3f);
     }
     CAN2CMR = 0x04;
  }
  if ( CAN1GSR & (1 << 6 ) ) { Can1ErrCount = (CAN1GSR >> 16 ); }
  if ( CAN2GSR & (1 << 6 ) ) { Can2ErrCount = (CAN2GSR >> 16 ); }
  VICVectAddr = 0;
}

void CanRx_Pop(void)
{
   while (Can1RxPush != Can1RxPop) {
      CanRxProcess(1, &(Can1RxFifo[Can1RxPop]));
      Can1RxPop = ((Can1RxPop+1)&0x3f);
   }

   while (Can2RxPush != Can2RxPop) {
      CanRxProcess(2, &(Can2RxFifo[Can2RxPop]));
      Can2RxPop = ((Can2RxPop+1)&0x3f);
   }
}
/******************************************************************************
** Function name:		CAN1_Init
******************************************************************************/
void CANx_Init( DWORD N, DWORD CAN_BTR )
{
  if (N==1) {//CAN1
     PINSEL0 &= ~0x0000000F;
     PINSEL0 |=  0x00000005; // port0.0~1, function 0x01
     PCONP |= PCAN1;

     CAN1MOD = 1;	 //CAN2MOD Reset CAN
     CAN1IER = 0;	 //CAN2IER Disable Receive Interrupt
     CAN1GSR = 0;	 //CAN2GSR Reset error counter when CANxMOD is in reset
     CAN1BTR = CAN_BTR; //CAN2BTR
     CAN1MOD = 0x0;     //CAN2MOD CAN in normal operation mode
     CAN1IER = 0x01;	  //CAN2IER
  }
  else {//CAN2
     PINSEL0 &= ~0x00000F00; // port0.4~5, function 0x10
     PINSEL0 |=  0x00000A00;	 
     PCONP |= PCAN2;
		
     CAN2MOD = 1;	 //CAN2MOD Reset CAN
     CAN2IER = 0;	 //CAN2IER Disable Receive Interrupt
     CAN2GSR = 0;	 //CAN2GSR Reset error counter when CANxMOD is in reset
     CAN2BTR = CAN_BTR; //CAN2BTR
     CAN2MOD = 0x0;     //CAN2MOD CAN in normal operation mode
     CAN2IER = 0x01;	  //CAN2IER
  }
}
/******************************************************************************
** Function name:		CAN2_Init
******************************************************************************/
const DWORD EXP_EXT_ID[4] = {
   0x0000, 0x0000, 0x0000, 0x0000
};
const DWORD GRP_EXT_ID[4] = {
   0x0000, 0x0000, 0x0000, 0x0000
};

void CAN_Init( void )
{
  DWORD address = 0;
  DWORD i;

  CAN_AFMR = ACCF_OFF; //关闭模式

  // Set explicit standard Frame  
  CAN_SFF_SA = address;
//  for ( i = 0; i < ACCF_IDEN_NUM; i += 2 )
//  {
//	ID_low = (i << 29) | (EXP_STD_ID << 16);
//	ID_high = ((i+1) << 13) | (EXP_STD_ID << 0);
//	*((volatile DWORD *)(CAN_MEM_BASE + address)) = ID_low | ID_high;
//	address += 4; 
//  }
		
  // Set group standard Frame 
  CAN_SFF_GRP_SA = address;
//  for ( i = 0; i < ACCF_IDEN_NUM; i += 2 )
//  {
//	ID_low = (i << 29) | (GRP_STD_ID << 16);
//	ID_high = ((i+1) << 13) | (GRP_STD_ID << 0);
//	*((volatile DWORD *)(CAN_MEM_BASE + address)) = ID_low | ID_high;
//	address += 4; 
//  }

  // Set explicit extended Frame
  CAN_EFF_SA = address;
  for ( i = 0; i < ACCF_IDEN_NUM; i++  ) {
	   *((volatile DWORD *)(CAN_MEM_BASE + address)) = (i << 29) | (EXP_EXT_ID[i] << 0);
	   address += 4; 
  }

  // Set group extended Frame 
  CAN_EFF_GRP_SA = address;
  for ( i = 0; i < ACCF_IDEN_NUM; i++  ) {	
	   *((volatile DWORD *)(CAN_MEM_BASE + address)) = (i << 29) | (GRP_EXT_ID[i] << 0);
	   address += 4; 
  }
   
  // Set End of Table 
  CAN_EOT = address; 
  CAN_AFMR = ACCF_BYPASS;//ACCF_ON; //工作模式
  //-------------------------------------------------------------------------
  install_irq( CAN_INT, (void *)CAN_Handler, LOWEST_PRIORITY );
}

/******************************************************************************
** Function name:		CAN1_SendMessage
**
** Descriptions:		Send message block to CAN1	
**
** parameters:			pointer to the CAN message
** Returned value:		true or false, if message buffer is available,
**						message can be sent successfully, return TRUE,
**						otherwise, return FALSE.
** 
******************************************************************************/

DWORD CAN_SendMsg( DWORD N, CAN_MSG *pTxBuf )
{
  if (N == 1) //CAN1 
  { 
     if ( CAN1SR & 0x00000004 ) {
        CAN1TFI1 = (pTxBuf->Frame) & 0xC00F0000; 
        CAN1TID1 = (pTxBuf->MsgID);
        CAN1TDA1 = (pTxBuf->DataA);
        CAN1TDB1 = (pTxBuf->DataB);
        CAN1CMR = 0x21;
     }
/*
     else if ( CAN1SR & 0x00000400 ) {
        CAN1TFI2 = (pTxBuf->Frame) & 0xC00F0000; 
        CAN1TID2 = (pTxBuf->MsgID);
        CAN1TDA2 = (pTxBuf->DataA);
        CAN1TDB2 = (pTxBuf->DataB);
        CAN1CMR = 0x41;
     }
     else if ( CAN1SR & 0x00040000 ) {
        CAN1TFI3 = (pTxBuf->Frame) & 0xC00F0000; 
        CAN1TID3 = (pTxBuf->MsgID);
        CAN1TDA3 = (pTxBuf->DataA);
        CAN1TDB3 = (pTxBuf->DataB);
        CAN1CMR = 0x81;
     }
*/
     else {
        return ( FALSE );
     }
	}
  else //CAN2
  { 
     if ( CAN2SR & 0x00000004 ) {
        CAN2TFI1 = (pTxBuf->Frame) & 0xC00F0000; 
        CAN2TID1 = (pTxBuf->MsgID);
        CAN2TDA1 = (pTxBuf->DataA);
        CAN2TDB1 = (pTxBuf->DataB);
        CAN2CMR = 0x21;
	   }
/*		 
     else if ( CAN2SR & 0x00000400 ) {
        CAN2TFI2 = (pTxBuf->Frame) & 0xC00F0000; 
        CAN2TID2 = (pTxBuf->MsgID);
        CAN2TDA2 = (pTxBuf->DataA);
        CAN2TDB2 = (pTxBuf->DataB);
        CAN2CMR = 0x41;
	   }
     else if ( CAN2SR & 0x00040000 ) {
        CAN2TFI3 = (pTxBuf->Frame) & 0xC00F0000; 
        CAN2TID3 = (pTxBuf->MsgID);
        CAN2TDA3 = (pTxBuf->DataA);
        CAN2TDB3 = (pTxBuf->DataB);
        CAN2CMR = 0x81;
	   }
*/
     else {
        return ( FALSE );
     }
	}
  return ( TRUE );
}

void CanTx_Pop(void)
{
	 if (Can1TxPush != Can1TxPop) {
      if(CAN_SendMsg(1, &Can1TxFifo[Can1TxPop])==TRUE) { Can1TxPop = ((Can1TxPop+1)&0x3f); }
   }

   if (Can2TxPush != Can2TxPop) {
      if(CAN_SendMsg(2, &Can2TxFifo[Can2TxPop])==TRUE) { Can2TxPop = ((Can2TxPop+1)&0x3f); }
   }
}

DWORD CAN_SendMessage( DWORD N, CAN_MSG *pTxBuf )
{
  if (N == 1) {
     if(((Can1TxPush+1)&0x3f) == Can1TxPop) { return ( FALSE ); }
	   else {
        (Can1TxFifo[Can1TxPush].Frame) = pTxBuf->Frame;
        (Can1TxFifo[Can1TxPush].MsgID) = pTxBuf->MsgID; 		
        (Can1TxFifo[Can1TxPush].DataA) = pTxBuf->DataA;
        (Can1TxFifo[Can1TxPush].DataB) = pTxBuf->DataB;
		    Can1TxPush = ((Can1TxPush+1)&0x3f);
     }
  }
  else {
     if (((Can2TxPush+1)&0x3f) == Can2TxPop) { return ( FALSE ); }
	   else {
        (Can2TxFifo[Can2TxPush].Frame) = pTxBuf->Frame;
        (Can2TxFifo[Can2TxPush].MsgID) = pTxBuf->MsgID; 		
        (Can2TxFifo[Can2TxPush].DataA) = pTxBuf->DataA;
        (Can2TxFifo[Can2TxPush].DataB) = pTxBuf->DataB;
		    Can2TxPush = ((Can2TxPush+1)&0x3f);
     }
  }
  return ( TRUE );
}
/******************************************************************************
**                            通信处理
******************************************************************************/
