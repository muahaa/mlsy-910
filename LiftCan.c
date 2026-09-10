/*****************************************************************************
 *  can.c:  CAN module API file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.13  ver 1.00 Prelimnary version, first Release
 *
*****************************************************************************/
//驱动
#include "LPC23xx.h"
#include "type.h"
#include "irq.h"
#include "target.h"
#include "absacc.h"
#include "timer.h"
#include "rtc.h"
#include "can.h"
//电梯
#define  LiftCan_Ex
#include "IO.h"
#include "Lift.h"
#include "Flash.h"
#include "Menu.h"
#include "LiftCan.h"
#include "Main.h"

/*****************************************************************************************************
*****************************************************************************************************/
DWORD CanTxOnLine(DWORD Can)
{
   CAN_MSG CAN_TX; 
   (CAN_TX.Frame) = 0x00080000; // 29-bit, no RTR, DLC is 8 bytes
   (CAN_TX.MsgID) = (0xf<<7)|(0x7<<0)|(0xe<<3); // CAN ID
	 //if ( (LiftFuntion&(PowerFun|PowerFunNoGroup)) == (PowerFun|PowerFunNoGroup) ) { 
	 //   (CAN_TX.DataA) = 0x00000009;
	 //} 
	 //else {
      (CAN_TX.DataA) = 0x01000009;
	 //}
   (CAN_TX.DataB) = 0;
	 
   return(CAN_SendMessage(Can, &CAN_TX ));
}
/*****************************************************************************************************
*****************************************************************************************************/
DWORD CanTxLED(DWORD Can, DWORD Floor, DWORD Order, DWORD i)
{
   CAN_MSG CAN_TX;
   (CAN_TX.Frame) = 0x00080000; // 29-bit, no RTR, DLC is 8 bytes
   (CAN_TX.MsgID) = (0xf<<7)|(0x7<<0)|(0xe<<3); // CAN ID
   if(i) { (CAN_TX.DataA) = 13; }
   else  { (CAN_TX.DataA) = 15; }

   (CAN_TX.DataA) |= ((Floor<<8)&0x0000ff00)|
                     ((Order<<16)&0x00ff0000);
   (CAN_TX.DataB) = 0;
   return(CAN_SendMessage(Can, &CAN_TX ));
}
/*****************************************************************************************************
*****************************************************************************************************/
DWORD CanTxLEDSingle(DWORD Can, DWORD Lift, DWORD Floor, DWORD Order, DWORD i)
{
   CAN_MSG CAN_TX;
	 DWORD Addr = SetAddr[Lift] ;
   (CAN_TX.Frame) = 0x00080000; // 29-bit, no RTR, DLC is 8 bytes
   (CAN_TX.MsgID) = ((Addr&0xf)<<7)|((Addr>>4)&0x7)|(0xe<<3); // CAN ID
   if(i) { (CAN_TX.DataA) = 13; }
   else  { (CAN_TX.DataA) = 15; }

   (CAN_TX.DataA) |= ((Floor<<8)&0x0000ff00)|
                     ((Order<<16)&0x00ff0000);
   (CAN_TX.DataB) = 0;	 
   return(CAN_SendMessage(Can, &CAN_TX ));
}
/*****************************************************************************************************
*****************************************************************************************************/
DWORD CanTxOrder(DWORD Can, DWORD Lift, DWORD Floor, DWORD Order, DWORD i)
{
   CAN_MSG CAN_TX;
   DWORD Addr = SetAddr[Lift] ; 
   (CAN_TX.Frame) = 0x00080000; // 29-bit, no RTR, DLC is 8 bytes
   (CAN_TX.MsgID) = ((Addr&0xf)<<7)|((Addr>>4)&0x7)|(0xe<<3); // CAN ID
   (CAN_TX.DataA) = 11|
                    ((Floor<<8)&0x0000ff00)|
                    ((Order<<16)&0x00ff0000)|
					          ((i<<24)&0xff000000);
   (CAN_TX.DataB) = 0;
   return(CAN_SendMessage(Can, &CAN_TX )); 
}
/*****************************************************************************************************
*****************************************************************************************************/
DWORD CanTxFrame31(DWORD Can, DWORD Lift, DWORD Cmd)
{
   CAN_MSG CAN_TX;
   DWORD Addr = SetAddr[Lift] ;
   (CAN_TX.Frame) = 0x00080000; // 29-bit, no RTR, DLC is 8 bytes
   (CAN_TX.MsgID) = ((Addr&0xf)<<7)|((Addr>>4)&0x7)|(0xe<<3); // CAN ID
   (CAN_TX.DataA) = 31;
   (CAN_TX.DataB) = Cmd<<16;
   return(CAN_SendMessage(Can, &CAN_TX )); 
}
/*****************************************************************************************************
*****************************************************************************************************/
//参数查询指令
DWORD CanTxFrame52(DWORD Can, DWORD Lift, DWORD Cmd)
{
   CAN_MSG CAN_TX;
   DWORD Addr = SetAddr[Lift];
	 if (Addr==0) { return(FALSE); }
   (CAN_TX.Frame) = 0x00080000; // 29-bit, no RTR, DLC is 8 bytes
   (CAN_TX.MsgID) = ((Addr&0xf)<<7)|((Addr>>4)&0x7)|(0xe<<3); // CAN ID
   (CAN_TX.DataA) = 52| 
					          (Cmd&0xffff0000); //0不用  主地址  0低32，1高32
   (CAN_TX.DataB) = 0;
   return(CAN_SendMessage(Can, &CAN_TX )); 
}
/*****************************************************************************************************
*****************************************************************************************************/
//上呼102=0x66    下呼107=0x6B   内呼112=0x70 
const DWORD OrderCmd[] = {
   0x00660000, 0x006B0000, 0x00700000, 0x01660000, 0x016B0000, 0x01700000,
};

DWORD Can1ScanLift = 0;
DWORD Can1ScanCmd = 0;
DWORD Can2ScanLift = 0;
DWORD Can2ScanCmd = 0;

void ScanOrder(void)
{
	 DWORD i,m;	 
	 for (i=0; i<4; i++) {
	      if (Can1_OK==0) { break; }
			  Can1ScanLift = (Can1ScanLift+1)&0x3;
		    if (Can1ScanLift==0) { //第1台时变化指令
					  if (++Can1ScanCmd > 5) {
							  Can1ScanCmd = 0;
						}
				}					
	      if (SetAddr[Can1ScanLift]) { //该地址需要查询
					  m = OrderCmd[Can1ScanCmd];
					  if (MaxFloor[Can1ScanLift]<33) { m &= 0x00ffffff; } //小于等于32层
	          CanTxFrame52(1, Can1ScanLift, m );
					  break;					
		    }
	 }

	 for (i=0; i<4; i++) {
	      if (Can2_OK==0) { break; }
			  Can2ScanLift = (Can2ScanLift+1)&0x3;
		    if (Can2ScanLift==0) { //第1台时变化指令
					  if (++Can2ScanCmd > 5) {
							  Can2ScanCmd = 0;
						}
				}					
	      if (SetAddr[Can2ScanLift]) { //该地址需要查询
					  m = OrderCmd[Can2ScanCmd];
					  if (MaxFloor[Can2ScanLift]<33) { m &= 0x00ffffff; } //小于等于32层
	          CanTxFrame52(2, Can2ScanLift, m );
					  break;					
		    }
	 }
}

/*****************************************************************************************************
*****************************************************************************************************/
DWORD SendDelay1=0;
DWORD SendDelay2=0;
void CanCheckLed(void)
{
   DWORD i,j,k;
   DWORD CanPort;
	 if (++SendDelay1>=200)  { SendDelay1 = 0; } 
	 if (++SendDelay2>=500)  { SendDelay2 = 0; } 

   i = GroupFirstFloor;		//楼层循环   
//   for (i = GroupFirstFloor; i <= GroupMaxFloor; i++) {		//楼层循环   
	     for (j=0; j<8; j++) {   //电梯循环
				   if (SetAddr[j]==0) { continue; }//地址位0
				   //切换端口
				   if (j<4) { 
						  if (Can1_OK==0) { continue; }
					    CanPort = 1; 
					 }
					 else { 
              if (Can2_OK==0) { continue; }						 
					    CanPort = 2;
					 }
					 
           k = BitFun(CanUpLed[j], i, 2);
					 if ( ((CanUpLedTimer[j][i]==2)&&(SendDelay2==0)&&(k==0)) //1000ms发送
              ||((CanUpLedTimer[j][i]==1)&&(SendDelay1==0)) //1000ms发送 					 
						  || (CanUpLedTimer[j][i]==0) ) { //10ms发送2次
		          if (CanTxLEDSingle(CanPort, j, i, 0xa0, k) == TRUE) { CanUpLedTimer[j][i]++; }
	         }
					 
					 k = BitFun(CanDnLed[j], i, 2);
					 if ( ((CanDnLedTimer[j][i]==2)&&(SendDelay2==0)&&(k==0)) //1000ms发送
              ||((CanDnLedTimer[j][i]==1)&&(SendDelay1==0)) //1000ms发送 					 
						  || (CanDnLedTimer[j][i]==0) ) { //10ms发送2次
              if (CanTxLEDSingle(CanPort, j, i, 0x60, k) == TRUE) { CanDnLedTimer[j][i]++; }
	         }					 	     
	     }
//   }
}
/*****************************************************************************************************
*****************************************************************************************************/
//标准灯控制
DWORD GpUp;
DWORD GpDn;
void CanStdCheckLed(void)
{
   DWORD i,j;
   for (i = GroupFirstFloor+1; i <= GroupMaxFloor; i++) {		//楼层循环   
			 GpUp = 0;
			 GpDn = 0;			 
 		   for (j=0; j<3; j++) {   //电梯循环			 
					 if (CanUpLedTimer[j][i]<2) {
              GpUp = 1; 
					    CanUpLedTimer[j][i]++;
	         }
					 
					 if (CanDnLedTimer[j][i]<2) {
						  GpDn = 1; 
              CanDnLedTimer[j][i]++;
	         }
			 }
			 
			 if (GpUp) {
			     if (BitFun(GroupUpOrder[0], i, 2) > 0) { j = 1; }
					 else                                   { j = 0; }  
           CanTxLED(1, i, 0xa0, j); 
	     }
					 
			 if (GpDn) {
			     if (BitFun(GroupDnOrder[0], i, 2) > 0) { j = 1; }
					 else                                   { j = 0; }  
           CanTxLED(1, i, 0x60, j); 
	     }
   }
}

//特殊发送
DWORD LiftStateTemp[3];
DWORD SendLedFloor = 0;
void CanSpecLed(void)
{
   DWORD i;
   for(i=0; i<3; i++) {
	     //if ((LiftState[i]&0x00c2) != (LiftStateTemp[i]&0xc2)) {
			 if (LiftState[i] != LiftStateTemp[i]) {
					    LiftStateTemp[i] = LiftState[i];
				    SendLedFloor = GroupFirstFloor+1;
			 }
	 }
	
	 if (SendLedFloor==0) { return; }
	 
   while (1) {		//楼层循环 
       i = 0;		 
       if (BitFun(GroupUpOrder[0], SendLedFloor, 2) > 0) {
           CanTxLED(1, SendLedFloor, 0xa0, 1);
           i = 1;				 
			 }
			 if (BitFun(GroupDnOrder[0], SendLedFloor, 2) > 0) { 
           CanTxLED(1, SendLedFloor, 0x60, 1);				 
           i = 1;				 
			 }		
       if (++SendLedFloor>GroupMaxFloor) {
				   SendLedFloor = 0;
				   break;
			 }
       if (i) { break; } 			 
   }
}

/*****************************************************************************************************
*****************************************************************************************************/
void CanCheckOrder(void)
{
   DWORD i,j,k;
   DWORD CanPort;
	
   for (i = GroupFirstFloor; i <= GroupMaxFloor; i++) {
			 //呼梯
       for (j=0; j<8; j++) { 
           if (SetAddr[j]==0) { continue; }//地址位0
				   //切换端口
				   if (j<4) { 
						  if (Can1_OK==0) { continue; }
					    CanPort = 1; 
					 }
					 else { 
              if (Can2_OK==0) { continue; }						 
					    CanPort = 2;
					 }

		       k = BitFunTwo(UpOrder[j], UpOrderTemp[j], i);
           if (k == 0x10) {
		          if (CanTxOrder(CanPort, j, i, 0xa0, 1) == TRUE) { BitFun(UpOrderTemp[j], i, 1); }
              if (i!=GroupFirstFloor) { //重发亮灯
								  //CanTxLED(CanPort, i, 0xa0, 1);
							}
							else {
								  CanTxLEDSingle(CanPort, j, i, 0xa0, 1);
							}
           }
           if (k == 0x01) {
		          if (CanTxOrder(CanPort, j, i, 0xa0, 0) == TRUE) { BitFun(UpOrderTemp[j], i, 0); }
              if ((i!=GroupFirstFloor)&&(BitFun(GroupUpOrder[0], i, 2)>0)) { //重发亮灯
								  CanTxLED(CanPort, i, 0xa0, 1);
								  //CanUpLedTimer[j][i] = 0;
							}
           }

           k = BitFunTwo(DnOrder[j], DnOrderTemp[j], i); 
	         if (k == 0x10) { 
	            if (CanTxOrder(CanPort, j, i, 0x60, 1) == TRUE) { BitFun(DnOrderTemp[j], i, 1); }
              if (i!=GroupFirstFloor) { //重发亮灯
								  //CanTxLED(CanPort, i, 0x60, 1);
							}
							else {
								  CanTxLEDSingle(CanPort, j, i, 0x60, 1);
							}
           }
           if (k == 0x01) { 
              if (CanTxOrder(CanPort, j, i, 0x60, 0) == TRUE) { BitFun(DnOrderTemp[j], i, 0); } 
              if ((i!=GroupFirstFloor)&&(BitFun(GroupDnOrder[0], i, 2)>0)) { //重发亮灯
								  CanTxLED(CanPort, i, 0x60, 1);
								  //CanDnLedTimer[j][i] = 0;
							}
           }
					 //----------------------------------------------------------------------------------					 
	     }
   }

   if (Can1_OK==0) { 
      for (i=0; i<4; i++) {
		      UpOrderTemp[i][0] = UpOrder[i][0];
		      UpOrderTemp[i][1] = UpOrder[i][1];
	        DnOrderTemp[i][0] = DnOrder[i][0];
		      DnOrderTemp[i][1] = DnOrder[i][1];
	    }
   }

   if (Can2_OK==0) { 
      for (i=4; i<8; i++) {
		      UpOrderTemp[i][0] = UpOrder[i][0];
		      UpOrderTemp[i][1] = UpOrder[i][1];
	        DnOrderTemp[i][0] = DnOrder[i][0];
		      DnOrderTemp[i][1] = DnOrder[i][1];
	    }
   }
}
/*****************************************************************************************************
*****************************************************************************************************/
void CanReSendOrder(void)
{
   DWORD i,j,k;
   DWORD CanPort;
   for (i = GroupFirstFloor; i <= GroupMaxFloor; i++) {
		   //重发呼梯开始
	     for (j=0; j<8; j++) {
				   //切换端口
				   if (SetAddr[j]==0) { continue; }//地址位0
				   if (j<4) { 
						   if (Can1_OK==0) { continue; }
					     CanPort = 1; 
					 }
					 else { 
               if (Can2_OK==0) { continue; }						 
					     CanPort = 2;
					 }
					 					 
	         if ((Lift_OK[j]>0)&&(LiftOnline[j]>0)&&(LiftNotMoveTimer[j]>0)) {//有方向不走梯，脱离 						 
						   k = BitFunTwo(UpOrder[j], LiftUpOrder[j], i);
				       if (k==0x10)       { CanTxOrder(CanPort, j, i, 0xa0, 1); }
               else if (k==0x01)  { CanTxOrder(CanPort, j, i, 0xa0, 0); }							 
				   
					     k = BitFunTwo(DnOrder[j], LiftDnOrder[j], i);
				       if (k==0x10)       { CanTxOrder(CanPort, j, i, 0x60, 1); }
               else if (k==0x01)  { CanTxOrder(CanPort, j, i, 0x60, 0); }
							 
		       }
           //--------------------------------------------------------------------------------					 
	     }
   }
}
/*****************************************************************************************************
要求登记的电梯号/呼梯的楼层/呼梯方向
*****************************************************************************************************/
void SetEnableOrder(DWORD orderlift, DWORD flr, DWORD updn)
{
   DWORD i,j;
   for (i=0; i<8; i++) {//8组
      if (updn==1) {
         if (BitFun(EnUpLift[i][orderlift], flr, 2)>0) {//判断各组是否允许该梯登记
					  BitFun(GroupUpOrder[i], flr, 1);
			      for (j=0; j<8; j++) {
			         if (BitFun(EnUpGroup[i][j], flr, 2)>0) {//判断各组是否包含该梯
			            BitFun(CanUpLed[j], flr, 1);
		              CanUpLedTimer[j][flr] = 0;
		           }
			      }
	       }
	    }
	    if (updn==2) {
				 if (BitFun(EnDnLift[i][orderlift], flr, 2)>0) {
		        BitFun(GroupDnOrder[i], flr, 1);
			      for (j=0; j<8; j++) {
			         if (BitFun(EnDnGroup[i][j], flr, 2)>0) {//判断各组是否包含该梯
			            BitFun(CanDnLed[j], flr, 1);
		              CanDnLedTimer[j][flr] = 0;
		           }
			      }
			   }
	    }
   }
}


/*****************************************************************************************************
到站的电梯号/消号楼层/消号方向
*****************************************************************************************************/
void SetCancelOrder(DWORD orderlift, DWORD flr, DWORD updn)
{
   DWORD i,j,k,p;
   for (i=0; i<8; i++) {//8组
      if (updn==1) {
	       if (BitFun(EnUpGroup[i][orderlift], flr, 2)>0) {//判断各组是否包含该梯
		        BitFun(GroupUpOrder[i], flr, 0);
			      for (j=0; j<8; j++) {
			          if (BitFun(EnUpGroup[i][j], flr, 2)>0) {//有登记，灭该组的灯
			             p = 0;
			             for (k=0; k<8; k++) {
				               if (k==i) { continue; }
					             if (BitFun(GroupUpOrder[k], flr, 2)==0) { continue; }//其他组无登记
											 if (BitFun(EnUpGroup[k][j], flr, 2)==0) { continue; }//该梯不参与
					             p = 1; break;
				           }
				           if (p==0) {
			                 BitFun(CanUpLed[j], flr, 0);
		                   CanUpLedTimer[j][flr] = 0;
				           }
		            }
			      }
		     }
	    }
			
	    if (updn==2) {
	       if (BitFun(EnDnGroup[i][orderlift], flr, 2)>0) {//判断各组是否包含该梯
		        BitFun(GroupDnOrder[i], flr, 0);
			      for (j=0; j<8; j++) {
			          if (BitFun(EnDnGroup[i][j], flr, 2)>0) {//有登记，灭该组的灯
			             p = 0;
			             for (k=0; k<8; k++) {
				               if (k==i) { continue; }
					             if (BitFun(GroupDnOrder[k], flr, 2)==0) { continue; }//该组无登记
					             if (BitFun(EnDnGroup[k][j], flr, 2)==0) { continue; }//该梯不参与
					             p = 1; break;
				           }
				           if (p==0) {
			                 BitFun(CanDnLed[j], flr, 0);
		                   CanDnLedTimer[j][flr] = 0;
				           }
		            }
			      }
		     }
	    }
   }
}
//特殊全部一起消号
void SetCancelAllOrder(DWORD orderlift, DWORD flr, DWORD updn)
{
   DWORD i,j,k,p;
   for (i=0; i<8; i++) {//8组
      if (updn==1) {
				 if (BitFun(EnUpLift[i][orderlift], flr, 2)>0) {//判断各组是否允许该梯登记，如果允许登记就允许消号
	       //if (BitFun(EnUpGroup[i][orderlift], flr, 2)>0) {//判断各组是否包含该梯
		        BitFun(GroupUpOrder[i], flr, 0);
			      for (j=0; j<8; j++) {
			          if (BitFun(EnUpLift[i][j], flr, 2)>0) {//有登记，灭该组的灯
			          //if (BitFun(EnUpGroup[i][j], flr, 2)>0) {//有登记，灭该组的灯
			             p = 0;
			             for (k=0; k<8; k++) {
				               if (k==i) { continue; }
					             if (BitFun(GroupUpOrder[k], flr, 2)==0) { continue; }//其他组无登记
											 if (BitFun(EnUpLift[k][j], flr, 2)==0) { continue; } 
											 //if (BitFun(EnUpGroup[k][j], flr, 2)==0) { continue; }//该梯不参与
					             p = 1; break;
				           }
				           if (p==0) {
			                 BitFun(CanUpLed[j], flr, 0);
		                   CanUpLedTimer[j][flr] = 0;
				           }
		            }
			      }
		     }
	    }
			
	    if (updn==2) {
				 if (BitFun(EnDnLift[i][orderlift], flr, 2)>0) {
	       //if (BitFun(EnDnGroup[i][orderlift], flr, 2)>0) {//判断各组是否包含该梯
		        BitFun(GroupDnOrder[i], flr, 0);
			      for (j=0; j<8; j++) {
			          if (BitFun(EnDnLift[i][j], flr, 2)>0) {//有登记，灭该组的灯
			          //if (BitFun(EnDnGroup[i][j], flr, 2)>0) {//有登记，灭该组的灯
			             p = 0;
			             for (k=0; k<8; k++) {
				               if (k==i) { continue; }
					             if (BitFun(GroupDnOrder[k], flr, 2)==0) { continue; }//该组无登记
					             if (BitFun(EnDnLift[k][j], flr, 2)==0) { continue; }//该梯不参与
					             //if (BitFun(EnDnGroup[k][j], flr, 2)==0) { continue; }//该梯不参与
					             p = 1; break;
				           }
				           if (p==0) {
			                 BitFun(CanDnLed[j], flr, 0);
		                   CanDnLedTimer[j][flr] = 0;
				           }
		            }
			      }
		     }
	    }
   }
}


/*****************************************************************************************************
*****************************************************************************************************/
void CanRxGroupProcess(DWORD Can, CAN_MSG *CAN_RX)
{
   DWORD i,j,Num;
	
   Num = (((CAN_RX->MsgID)>>3)&0xf) | (((CAN_RX->MsgID)&0x7)<<4);

   //计算梯号
   if (Can==1) {
      for (i=0; i<4; i++) {
         if(SetAddr[i] == Num) { Num = i; break; }
      }
      if (i>3) { return; }//没有匹配的地址
   }
	 
   if (Can==2) {
      for (i=4; i<8; i++) {
         if(SetAddr[i] == Num) { Num = i; break; }
      }
      if(i>7) { return; }//没有匹配的地址
   }

   Lift_OK[Num] = 9;
   switch ((CAN_RX->DataA)&0xff)
   {
   	   case 8:	//EC 状态帧
			 { 
	         if (((CAN_RX->DataA)&0x01000000)==0) { 
		          LiftOnline[Num] = 0;
		       }
		       else { 
		          LiftOnline[Num] = 9;
		       }
	         
		       LiftABSFloor[Num] = ((CAN_RX->DataA)>>8)&0xff; //电梯当前所在楼层					 
		       LiftState[Num] = (((CAN_RX->DataB)>>8)&0xff00)|(((CAN_RX->DataB)>>24)&0xff);
                                            
		       if ((NewFlag[Num]==0)||(NewFlag[Num]==2)) {
		          LiftRunFloor[Num] = 0;//目标楼层		 
		          LiftEnDecFloor[Num] = 0;//允许减速楼层		   
		       }
		       else {
		          LiftRunFloor[Num] = ((CAN_RX->DataB)>>8)&0xff;//目标楼层		 
		          LiftEnDecFloor[Num] = (CAN_RX->DataB)&0xff;//允许减速楼层		   
		       }
		       //减速距离变楼层 目标楼层==0 允许减速楼层==0 电梯停止
					 if (NewFlag[Num]==0) {
              LiftNowFloor[Num] = LiftABSFloor[Num];
						  //i = (LiftState[Num]&0x0030);
						  //if ((LiftState[Num]&0x0002)==0) {
		          //LiftNowFloor[Num] = LiftABSFloor[Num];
							//}
						  //else if (i==0x0030) { 
		          //    LiftNowFloor[Num] = LiftABSFloor[Num]+2;
							//}
						  //else if (i==0x0010) { 
		          //    LiftNowFloor[Num] = LiftABSFloor[Num]+1;
							//}
							//else {
		          //    LiftNowFloor[Num] = LiftABSFloor[Num];
							//}
           }						
					 else if (NewFlag[Num]==2) {					 
		          i = ((CAN_RX->DataA)>>16)&0xff; //显示的楼层
					    if (i>=65) { i = 1; }
					    else       { i++; }
					    LiftNowFloor[Num] = i;
           }						
		       break;
			 }
			 
	     case 10: //呼梯申请
			 {
					 i = (((CAN_RX->DataA)>>8)&0xff);//
				   j = ((CAN_RX->DataA)&0x00ff0000);
				 
				   //if ((LiftFuntion&TimeSpecial)&&(BaseFloorGroup<6))//高峰时禁外呼
					 //{
           //    if (CheckDisableOrder(i)) { break; }//无匹配的楼层不登记
					 //} 
					 
		       if (j == 0x00a00000) {
			        SetEnableOrder(Num, i, 1);
					 }	
				   else if (j == 0x00600000) {
						  SetEnableOrder(Num, i, 2);
		       }
		       break;
			 }
			 
			 case 12: //呼梯登记成功
			 { 
	         i = ((CAN_RX->DataA)&0x00ff0000);   
		       if (i==0x00a00000) {
  			      i = (((CAN_RX->DataA)>>8)&0xff);
				      if (i==0xff) {
				         LiftUpOrder[Num][0] = 0; LiftUpOrder[Num][1] = 0;
				      }
				      else if ((CAN_RX->DataB)&0xff) {
								    BitFun(LiftUpOrder[Num], i, 1);
				            //if (BitFun(UpOrder[Num], i, 2)==0) { BitFun(LiftUpOrder[Num], i, 0); } //????????????
				            //else                               { BitFun(LiftUpOrder[Num], i, 1); }
				      }
				      else {
				            BitFun(LiftUpOrder[Num], i, 0);
				      }
			    }
					else if (i==0x00600000) {
		          i = (((CAN_RX->DataA)>>8)&0xff);
     					if (i==0xff) {
				         LiftDnOrder[Num][0] = 0; LiftDnOrder[Num][1] = 0;
				      }
			        else if ((CAN_RX->DataB)&0xff) {
								    BitFun(LiftDnOrder[Num], i, 1);
				            //if (BitFun(DnOrder[Num], i, 2)==0) { BitFun(LiftDnOrder[Num], i, 0); } //?????????????
				            //else                               { BitFun(LiftDnOrder[Num], i, 1); }
				      }
				      else {
				            BitFun(LiftDnOrder[Num], i, 0);
				      }
				  }
					else if (i==0x00100000) {
		          i = (((CAN_RX->DataA)>>8)&0xff);
			        if (i==0xff) {
				         LiftInOrder[Num][0] = 0; LiftInOrder[Num][1] = 0;
				      }
				      else if ((CAN_RX->DataB)&0xff) {
				         BitFun(LiftInOrder[Num], i, 1);
				      }
				      else {
				         BitFun(LiftInOrder[Num], i, 0);
				      }
					}
					else if (i==0x00f00000) {
			        LiftUpOrder[Num][0] = 0; LiftUpOrder[Num][1] = 0;
			        LiftDnOrder[Num][0] = 0; LiftDnOrder[Num][1] = 0;
			        LiftInOrder[Num][0] = 0; LiftInOrder[Num][1] = 0;						
					}
					//----------------------------------------------------
		      break;
			}
			 
	    case 14: //电梯销号通知
			{	
				  i = (((CAN_RX->DataA)>>8)&0xff);
		      BitFun(LiftInOrder[Num], i, 0);   //消内选
				  j = ((CAN_RX->DataA)&0x00ff0000);  

				  if ((j ==0x00a00000)||(j ==0x00a10000)||(j ==0x00a30000)) {
		         BitFun(LiftUpOrder[Num], i, 0); //消本梯上指令
				     SetCancelOrder(Num, i, 1);
				  }
					if ((j ==0x00600000)||(j ==0x00610000)||(j ==0x00630000)) {
		         BitFun(LiftDnOrder[Num], i, 0); //消本梯上指令
				     SetCancelOrder(Num, i, 2);
		      }
		      break;
	    }
			
	     default: break;
   }
}
/*****************************************************************************************************
*****************************************************************************************************/
void CanRxLiftProcess(DWORD Can, CAN_MSG *CAN_RX)
{
   DWORD i,Num;

   if ( ((CAN_RX->DataA)&0xffff) != 0x0134 ) {  //非修改参数	
	     return; 
	 }
	 
   Num = (((CAN_RX->MsgID)>>7)&0xf) | (((CAN_RX->MsgID)&0x7)<<4); //接收ID
	 	 
	 //时间修改
	 i = CAN_RX->DataA; 
	 if ( (i == 0x00440134)||(i == 0x00470134) ) { 
       RTCSetOne(CAN_RX->DataA, CAN_RX->DataB);
       return;		 
	 }
	 
   //计算梯号
   if (Can==1) {
       for (i=0; i<4; i++) {
           if((SetAddr[i]>0)&&(SetAddr[i] == Num)) { Num = i; break; }
       }
       if (i>3) { return; }//没有匹配的地址
   }
	 
   if (Can==2) {
       for (i=4; i<8; i++) {
           if((SetAddr[i]>0)&&(SetAddr[i] == Num)) { Num = i; break; }
       }
       if(i>7) { return; }//没有匹配的地址
   }

	 i = (CAN_RX->DataA >> 16) & 0xff; //主地址
	 if (i == 0x0e) {
	     i = ((CAN_RX->DataA)>>24)&0xff; //子地址
			 if ((i>=0x64)&&(i<=0x6b)) { //时间段设置
				   OrderTimeEnable[Num][i-0x64] = CAN_RX->DataB;
				   Modified = 1;
			 }
       else if ((i>=0x6c)&&(i<=0x7b)) {
				   i = i-0x6c;
				   OrderFlagEnable[Num][i/2][i%2] = CAN_RX->DataB;
				   Modified = 1;
			 }
	 }
}
 
/*****************************************************************************************************
*****************************************************************************************************/
void CanRxProcess(DWORD Can, CAN_MSG *CAN_RX)
{
   if ( ((((CAN_RX->MsgID)>>7)&0xf) == 0xe)     //群控板接收
		 && (((CAN_RX->DataA)&0xffff) != 0x0134)) { //微机修改参数
		   CanRxGroupProcess(Can, CAN_RX);
	 }
	 else { //监控修改微机
		   CanRxLiftProcess(Can, CAN_RX);
	 }
}

/*****************************************************************************************************
*****************************************************************************************************/

//DWORD CheckDisableOrder(DWORD Flr)
//{
//	  DWORD i;
//    for (i=0; i<8; i++)//查找8台电梯可用的基站
//		{
//		    if ( (SetAddr[i]>0)&&(WaitFloor[i][BaseFloorGroup]==Flr) ) { return(0); }//该电梯有定义使用，该基站匹配
//		}
//		return(1);
//}							
/*****************************************************************************************************
*****************************************************************************************************/
