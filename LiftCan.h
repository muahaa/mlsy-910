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
#ifndef __LIFTCAN_H
#define __LIFTCAN_H
/******************************************************************************/ 
#ifdef  LiftCan_Ex
   #define LiftCan_Extern          //定义变量
#else
   #define LiftCan_Extern  extern  //声明变量
#endif
/******************************************************************************/
LiftCan_Extern DWORD Can1_OK, Can2_OK;
LiftCan_Extern DWORD Lift_OK[8];
	 
DWORD CanTxOnLine(DWORD Can);
DWORD CanTxLED(DWORD Can, DWORD Floor, DWORD Order, DWORD i);
DWORD CanTxOrder(DWORD Can, DWORD Lift, DWORD Floor, DWORD Order, DWORD i);
DWORD CanTxFrame31(DWORD Can, DWORD Lift, DWORD Cmd);	 
DWORD CanTxFrame52(DWORD Can, DWORD Lift, DWORD Cmd);	 
void ScanOrder(void);	 
void CanCheckLed(void);
void CanStdCheckLed(void);//标准时发送
void CanReSendOrder(void);
//void CanTestLed(void);
void CanCheckOrder(void);
void CanRxProcess(DWORD Can, CAN_MSG *CAN_RX);
//DWORD CheckDisableOrder(DWORD Flr);	 
//void CheckRecord_100ms(void);

void CanSpecLed(void);	 
/******************************************************************************/
#endif
/******************************************************************************
**                            End Of File
******************************************************************************/
