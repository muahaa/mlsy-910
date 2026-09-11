/***********************************************************************
 **********************************************************************/
//驱动
#include "LPC23xx.h"
#include "type.h"
#include "irq.h"
#include "target.h"
#include "absacc.h"
#include "timer.h"
//#include "ex_int.h"
#include "rtc.h"
#include "can.h"
#include "uart.h"
//电梯
#include "main.h"
#define  Lift_Ex
#include "IO.h"
#include "Lift.h"
#include "Flash.h"
#include "Menu.h"
#include "LiftCan.h"
/**********************************************************************/
DWORD Bit64(DWORD *p, DWORD i, DWORD Value)
{ 
   if(i>63) { return(0); }
   if(i>31) { p++; i -= 32; }
   i = ((DWORD)1<<i);
   if(Value == 0) { (*p) &= (~i); return(0); }
   else if(Value == 1) { (*p) |= i; return(1); }
   else if(((*p)&i)==0) { return(0); }
   else { return(1); }
}
/**********************************************************************/
DWORD BitFun(DWORD *p, DWORD i, DWORD Value)
{ 
   if((i==0)||(i>64)) { return(0); }
   if(i>32) { p++; i -= 32; }
   i = ((DWORD)1<<(i-1));
   if(Value == 0) { (*p) &= (~i); return(0); }
   else if(Value == 1) { (*p) |= i; return(1); }
   else if(((*p)&i)==0) { return(0); }
   else { return(1); }
}
/**********************************************************************/
DWORD BitFunTwo(DWORD *p, DWORD *p1, DWORD i)
{ 
   DWORD Value;
   if((i==0)||(i>64)) { return(0); }
   if(i>32) { p++; p1++; i -= 32; }
   i = ((DWORD)1<<(i-1));
   Value = 0;
   if(((*p)&i)>0) { Value |= 0x10; }
   if(((*p1)&i)>0) { Value |= 0x01; }
   return(Value);
}
/**********************************************************************/
DWORD LiftNoOrderTimer[8]; // per-lift idle timer for distributed waiting
/**********************************************************************/
/**********************************************************************/
void LiftInit(void)
{ 
   DWORD i,j;
   /**************************************************/
   if(ReadStaticPara() == FALSE) {
      InitData(0);
	    Modified = 1;
   }
   /**************************************************/
   //if(ReadNowPara() == FALSE) {
   //   InitData(0x01);
   //   SaveNowPara();
   //}
   /**************************************************/
   if (Password[0]>99999) { Password[0] = 0; }
   if (Password[1]>99999) { Password[1] = 0; }
   for(i=0; i<8; i++) { 
      if (SetAddr[i]>99) { SetAddr[i] = 0; }
	    if ((FirstFloor[i]==0)||(FirstFloor[i]>64)) { FirstFloor[i] = 1; }
	    if ((MaxFloor[i]==0)||(MaxFloor[i]>64)) { MaxFloor[i] = 8; }
	    for(j=0; j<6; j++) {
	       if ((WaitFloor[i][j]==0)||(WaitFloor[i][j]>64)) { WaitFloor[i][j] = 1; }
	    }
			if ((PowerSetp[i]>16)||(PowerSetp[i]==0)) { PowerSetp[i] = i+1; }
			if ((FireSetp[i]>16)||(FireSetp[i]==0)) { FireSetp[i] = i+1; }
   }

   if ((LiftNotMove==0)||(LiftNotMove>9999)) { LiftNotMove = 90; }
   for(j=0; j<6; j++) {
      if (WaitFloorTime[j]>9999) { WaitFloorTime[j] = 120; } 
   }
	 
	 if (Mode2_WaitFloorTime>9999) { Mode2_WaitFloorTime = 30; }
	 NoOrderTimer = 0;
	 StartupTime = Mode2_WaitFloorTime;
   /**************************************************/
   Can1_OK = 0;
   Can2_OK = 0;
   for(i=0; i<8; i++) {
      LiftNotMoveTimer[i] = 0;
	    Lift_OK[i] = 0;
      LiftOnline[i] = 0;
		  LiftNowFloor[i] = 1;
      LiftABSFloor[i] = 1;
		  for(j=0; j<2; j++) {
         LiftUpOrder[i][j] = 0;
         LiftDnOrder[i][j] = 0;
         LiftInOrder[i][j] = 0;
         
				 //LiftUpButton[i][j] = 0;
         //LiftDnButton[i][j] = 0;
         //LiftUpButtonB[i][j] = 0;
         //LiftDnButtonB[i][j] = 0;
	    
         UpOrder[i][j] = 0;	DoUpOrder[i][j] = 0; DoUpOrderOld[i][j] = 0; 
         DnOrder[i][j] = 0;	DoDnOrder[i][j] = 0; DoDnOrderOld[i][j] = 0;
         UpOrderTemp[i][j] = 0;
         DnOrderTemp[i][j] = 0;
         //UpOrderSp[i][j] = 0;
         //DnOrderSp[i][j] = 0;
				
         //UpOrderSpB[i][j] = 0;
         //DnOrderSpB[i][j] = 0;
	    }
      LiftState[i] = 0;
      LiftRunFloor[i] = 0;
      LiftEnDecFloor[i] = 0;
	    LiftWaitTimer[i] = 0;
      LiftNoOrderTimer[i] = 0;
   }
   LiftWaitFlag = 0;

   if((GroupFirstFloor<1)||(GroupFirstFloor>64)) { GroupFirstFloor = 1;	} 
   if((GroupMaxFloor<1)||(GroupMaxFloor>64)) { GroupMaxFloor = 8; }
   for(i=0; i<8; i++) {
      for(j=0; j<2; j++) {
         GroupUpOrder[i][j] = 0;//群控的呼梯标志
		     CanUpLed[i][j] = 0; 
		     GroupDnOrder[i][j] = 0;
		     CanDnLed[i][j] = 0; 				
	    }
   }
   Do_Floor = 0xff;
   for(i=0; i<8; i++) {
	    IncallNum[i] = 0;
      BackFloor[i] = 0;
	 	  PowerFlag[i] = 0; 
   }
	 PowerRunSetp = 0;
	 
   for(i=0; i<8; i++) {
      for(j=0; j<72; j++) {
				  CanUpLedTimer[i][j] = 0xff;//发送次数
          CanDnLedTimer[i][j] = 0xff;//发送次数
			}
	 }
	 
   for(i=0; i<8; i++) {
      for(j=0; j<2; j++) {
				  RunEnUpOrder[i][j]  = 0; //8台梯A门呼梯使能运行时使用
          RunEnDnOrder[i][j]  = 0;
			}
	 }
}
/**********************************************************************/
DWORD Bit64Temp[2]; //64位临时变量

DWORD UpOrderBit(DWORD Lift, DWORD Floor)
{
		 Bit64Temp[0] = DoUpOrder[Lift][0];
		 Bit64Temp[1] = DoUpOrder[Lift][1];
		 return(BitFun(Bit64Temp, Floor, 2));
}

DWORD DnOrderBit(DWORD Lift, DWORD Floor)
{
     Bit64Temp[0] = DoDnOrder[Lift][0];
		 Bit64Temp[1] = DoDnOrder[Lift][1];
		 return(BitFun(Bit64Temp, Floor, 2)); 
}			          									
									
DWORD InOrderBit(DWORD Lift, DWORD Floor)
{
		 Bit64Temp[0] = LiftInOrder[Lift][0];
		 Bit64Temp[1] = LiftInOrder[Lift][1];
		 return(BitFun(Bit64Temp, Floor, 2));
}

DWORD OrderBit_AB(DWORD Lift, DWORD Floor, DWORD Order)
{
	   if (Order==1) {
		     Bit64Temp[0] = DoUpOrder[Lift][0];
		     Bit64Temp[1] = DoUpOrder[Lift][1];
		 }
		 else if (Order==2) {
		     Bit64Temp[0] = DoDnOrder[Lift][0];
		     Bit64Temp[1] = DoDnOrder[Lift][1];
		 }
		 return(BitFun(Bit64Temp, Floor, 2));
}

//-------------------------------
//OrderUD:1A上，2A下，3B上，4B下
void LiftDistance(DWORD Lift, DWORD OrderUD, DWORD OrderFlr)
{
   DWORD i,j;
   CNum = 0; 
   ONum = 0; 
   INum = 0;
	
   if (SetAddr[Lift]==0) { FNum = 0x1fffffff; FTime = 0x1fffffff; return; }
   if (LiftOnline[Lift]==0) { FNum = 0x1fffffff; FTime = 0x1fffffff; return; }
	 i = 0;
	 if (LiftNotMoveTimer[Lift]==0)  { i = 1; } //有方向超时不走,开门到位
	 if ((LiftState[Lift]&0x0800)>0) { i = 1; } //超载
   if ((LiftState[Lift]&0x1000)>0) { i = 1; } //满载
   if (OrderFlr<FirstFloor[Lift])  { i = 1; }
	 if (OrderFlr>MaxFloor[Lift])    { i = 1; }
	 if ((OrderUD == 1)&&(BitFun(RunEnUpOrder[Lift], OrderFlr, 2)==0)) { i = 1; }
	 if ((OrderUD == 2)&&(BitFun(RunEnDnOrder[Lift], OrderFlr, 2)==0)) { i = 1; }
	 if (i) { FNum = 0x1fffffff; FTime = 0x1fffffff; return; } 
	 
	 //计算最高和最低楼层
   Max = LiftNowFloor[Lift];
   Min = Max;
   for(i=LiftNowFloor[Lift]; i<=MaxFloor[Lift]; i++) {
			if (UpOrderBit(Lift, i)>0) { Max = MaxFloor[Lift];	break; }		
			if ((InOrderBit(Lift, i)>0)||(DnOrderBit(Lift, i)>0)) { Max = i; } 
   }
   for(i=LiftNowFloor[Lift]; i>=FirstFloor[Lift]; i--) {
		  if (DnOrderBit(Lift, i)>0) { Min = FirstFloor[Lift]; break; } 
			if ((InOrderBit(Lift, i)>0)||(UpOrderBit(Lift, i)>0)) { Min = i; } 
   }
   if (OrderFlr<Min) { Min = OrderFlr; }//修正最短距离
   if (OrderFlr>Max) { Max = OrderFlr; }//修正最大距离

	 //上呼 
   if (OrderUD == 1) { 
      if((LiftState[Lift]&0x00c0)==0x80) { //上行
         if(OrderFlr>=LiftNowFloor[Lift]) { //上行顺向 
		        FNum = OrderFlr-LiftNowFloor[Lift];
			      for(i=LiftNowFloor[Lift]; i<OrderFlr; i++) {
			          j = 0;
			          if (UpOrderBit(Lift, i)>0) { ONum++; j = 1; }
								if (InOrderBit(Lift, i)>0) { INum++; j = 1; }
			          if (j>0) {
			             CNum++;
			          } 
			      } 
		     }
         else {//上行反向
		        FNum = (Max-LiftNowFloor[Lift])+(Max-Min)+(OrderFlr-Min);
			      for(i=LiftNowFloor[Lift]; i<=Max; i++) {
			          j = 0;
							  if (UpOrderBit(Lift, i)>0) { ONum++; j = 1; }
			          if (InOrderBit(Lift, i)>0) { INum++; j = 1; }
			          if(j>0) {
			              CNum++;
			          } 
			      } 
			      for(i=Max; i>=Min; i--) {
							  if (DnOrderBit(Lift, i)>0) { 
			              ONum++; CNum++; 
			          }
			      } 
			      for(i=Min; i<OrderFlr; i++) {
							  if (UpOrderBit(Lift, i)>0) {
			              ONum++; CNum++;
			         } 
			      }
		    }
	  }
	  else if((LiftState[Lift]&0x00c0)==0x40) {//下行
	      FNum = (LiftNowFloor[Lift]-Min)+(OrderFlr-Min);
		    for(i=LiftNowFloor[Lift]; i>=Min; i--) {
		        j = 0;
					  if (DnOrderBit(Lift, i)>0) { ONum++; j = 1; }
			      if (InOrderBit(Lift, i)>0) { INum++; j = 1; }
			      if(j>0) {
			          CNum++;
			      } 
		    } 
		    for(i=Min; i<OrderFlr; i++) {
					  if (UpOrderBit(Lift, i)>0) {
			          ONum++; CNum++;
			      } 
		    } 
	  }
	  else {//无方向
        if(OrderFlr>=LiftNowFloor[Lift]) { 
		       FNum = OrderFlr-LiftNowFloor[Lift];
			     for(i=LiftNowFloor[Lift]; i<OrderFlr; i++) {
			         j = 0;
						   if (UpOrderBit(Lift, i)>0) { ONum++; j = 1; }
							 if (InOrderBit(Lift, i)>0) { INum++; j = 1; }
			         if(j>0) {
			             CNum++;
			         } 
			     } 
		    }
        else { 
		       FNum = (LiftNowFloor[Lift]-Min)+(OrderFlr-Min); 
		       for(i=LiftNowFloor[Lift]; i>=Min; i--) {
			         j = 0;
						   if (DnOrderBit(Lift, i)>0) { ONum++; j = 1; } 
							 if (InOrderBit(Lift, i)>0) { INum++; j = 1; }
			         if(j>0) { 
			            CNum++;
			         } 
		       } 
		       for(i=Min; i<OrderFlr; i++) {
						   if (UpOrderBit(Lift, i)>0) {
			             ONum++; CNum++;
			         } 
		       } 
		     }
	    }
   }

   //下呼	 
   else { 
      if((LiftState[Lift]&0x00c0)==0x80) {//上行
         FNum = Max-LiftNowFloor[Lift]+(Max-OrderFlr);
		     for(i=LiftNowFloor[Lift]; i<=Max; i++) {
		        j = 0;
						if (UpOrderBit(Lift, i)>0) { ONum++; j = 1; }
						if (InOrderBit(Lift, i)>0) { INum++; j = 1; }
			      if (j>0) {
			         CNum++; 
			      } 
		     } 
		     for(i=Max; i>OrderFlr; i--) {
					  if (DnOrderBit(Lift, i)>0) { 
			         ONum++; CNum++;
			      } 
		     } 
	   }
	   else if((LiftState[Lift]&0x00c0)==0x40) {//下行
	       if(LiftNowFloor[Lift]>=OrderFlr) { 
		     FNum = LiftNowFloor[Lift]-OrderFlr; 
		     for(i=LiftNowFloor[Lift]; i>OrderFlr; i--) {
			      j = 0;
					  if (DnOrderBit(Lift, i)>0) { ONum++; j = 1; }						
						if (InOrderBit(Lift, i)>0) { INum++; j = 1; }
			      if(j>0) {
			         CNum++;
			      } 
		     } 
		  }
      else { 
		     FNum = (LiftNowFloor[Lift]-Min)+(Max-Min)+(Max-OrderFlr); 
		     for(i=LiftNowFloor[Lift]; i>=Min; i--) {
			      j = 0;
					  if (DnOrderBit(Lift, i)>0) { ONum++; j = 1; }
						if (InOrderBit(Lift, i)>0) { INum++; j = 1; }
			      if(j>0) { 
			         CNum++;
			      } 
		     } 
		     for(i=Min; i<=Max; i++) {
					  if (UpOrderBit(Lift, i)>0) {
			         ONum++; CNum++;
			      } 
		     } 
			   for(i=Max; i>OrderFlr; i--) {
			      if (DnOrderBit(Lift, i)>0) {
			         ONum++; CNum++;
			      } 
		     }
		  }
	  }
    else {//无方向
       if(LiftNowFloor[Lift]>=OrderFlr) { 
		      FNum = LiftNowFloor[Lift]-OrderFlr;
			    for(i=LiftNowFloor[Lift]; i>OrderFlr; i--) {
			       j = 0;
			       if (DnOrderBit(Lift, i)>0) { ONum++; j = 1; }
			       if (InOrderBit(Lift, i)>0) { INum++; j = 1; }
			       if(j>0) {
			          CNum++;
			       } 
		      }
       }
       else { 
		      FNum = Max-LiftNowFloor[Lift]+Max-OrderFlr; 
		      for(i=LiftNowFloor[Lift]; i<=Max; i++) {
			       j = 0;
			       if (UpOrderBit(Lift, i)>0) { ONum++; j = 1; } 
			       if (InOrderBit(Lift, i)>0) { INum++; j = 1; }
			       if(j>0) { 
			          CNum++;
			       } 
		      } 
		      for(i=Max; i>OrderFlr; i--) {
			       if (DnOrderBit(Lift, i)>0) {
			          ONum++; CNum++;
			       } 
		      }
		   }
	   }
   }

   if (CNum==0) {  //只有一个呼梯
      if(LiftNowFloor[Lift]>=OrderFlr) { FNum = LiftNowFloor[Lift]- OrderFlr; }
	    else                             { FNum = OrderFlr - LiftNowFloor[Lift]; }
   }
	
   FTime = FloorRunTime*FNum + CNum*RunStopTime;
   j = RunOrderTime; 	 
   if(OrderUD == 1) {
			if (OrderBit_AB(Lift, OrderFlr, 1)>0) { j = 0; }//之前没分配
	 }
	 else if(OrderUD == 2) {
			if (OrderBit_AB(Lift, OrderFlr, 2)>0) { j = 0; }//之前没分配
	 }
	 FTime += j;
   
   if(FNum==0) { //本层处理(用低8位)
		  j = 0;
      if(OrderUD == 1) {
				 if((LiftState[Lift]&0x00c0)==0x40) { FNum |= 0x80; } //不同向
				 if (OrderBit_AB(Lift, OrderFlr, 1)==0) { FNum |= 0x10; }//之前没分配
	    }
	    else if(OrderUD == 2) {
				 if((LiftState[Lift]&0x00c0)==0x80) { FNum |= 0x80; } //不同向
				 if (OrderBit_AB(Lift, OrderFlr, 2)==0) { FNum |= 0x10; }//之前没分配
	    }

      if((LiftState[Lift]&0x0002)==0) { ; }//停止
	    else if((LiftState[Lift]&0x0020)==0) { FNum |= 0x20; }//本层减速 
	    else { FNum |= 0x40; } 

      i = (LiftState[Lift]&0x0300);
      			
	    if((i==0x300)||(j>0)) { ; } //开门到位 或者 该层呼梯按钮被按下
	    else if(i==0x100) { FNum |= 0x01; } //正在开门
	    else if(i==0x000) { FNum |= 0x02; } //关门到位
	    else if(i==0x200) { FNum |= 0x04; } //正在关门
   }
   else {//(用高8位)
      FNum = (FNum<<16)&(0xffff0000);
      if(OrderUD == 1) {
				 if (OrderBit_AB(Lift, OrderFlr, 1)==0) { FNum |= 0x0100; }//之前没分配
		     if((LiftNowFloor[Lift]>OrderFlr)||((LiftState[Lift]&0x00c0)!=0x80)) { FNum |= 0x4000; } //顺向优先 
	    }
	    else if(OrderUD == 2) {
	       if (OrderBit_AB(Lift, OrderFlr, 2)==0) { FNum |= 0x0100; }//之前没分配
		     if((LiftNowFloor[Lift]<OrderFlr)||((LiftState[Lift]&0x00c0)!=0x40)) { FNum |= 0x4000; } //顺向优先 
	    }
			
			if ((INum==1)||(ONum>0)||(InOrderBit(Lift, OrderFlr)==0)) { FNum |= 0x2000; } //只有一个到达该层的内选
   }
}
/*****************************************************************************************************
*****************************************************************************************************/
DWORD Sq_PL[8];
void Seqence(void)
{
   DWORD i,j,M;
   for(i=0; i<8; i++) {//有方向的距离
	    Sq_PL[i] = L[i];
   }
   for(i=0; i<8; i++) {
      for(M=0,j=1; j<8; j++) {
         if(Sq_PL[j]<Sq_PL[M]) { M = j; }
      }
      N[i] = M;
      Sq_PL[M] = 0xffffffff;
   }
}
/*****************************************************************************************************
*****************************************************************************************************/
DWORD LiftOrderCounter[8];
void CounterLiftOrder(void)//计算电梯登记数
{
   DWORD i,k,num;
	 for (i=0; i<8; i++) {
      num = 0;
      for (k=FirstFloor[i]; k<MaxFloor[i]; k++) {
         if( (BitFun(LiftInOrder[i], k, 2)>0)
           ||(BitFun(LiftUpOrder[i], k, 2)>0) 
           ||(BitFun(LiftDnOrder[i], k, 2)>0) ) { num++; }
      }
			LiftOrderCounter[i] = num;
	 }
}


//10ms定时
DWORD GP;
void LiftMain( void )
{
   DWORD j,k,m,n;
	
	 CounterLiftOrder();
	
	//计算所有允许的登记
   j=0; k=0; m=0; n=0;
   for (GP=0; GP<8; GP++) {
      if ((LiftOnline[GP]>0)&&(SetAddr[GP]>0)) {
	       j |= RunEnUpOrder[GP][0];
		     k |= RunEnUpOrder[GP][1];
	       m |= RunEnDnOrder[GP][0];
		     n |= RunEnDnOrder[GP][1];
	    }
   }
	 
	 //屏蔽群控中不允许的呼梯 
   for (GP=0; GP<8; GP++) {
      GroupUpOrder[GP][0] &= j;
      GroupUpOrder[GP][1] &= k;
      GroupDnOrder[GP][0] &= m;
      GroupDnOrder[GP][1] &= n;  
   }
	 //---------------------------------------
   //计算内选数目
   for(j=0; j<8; j++) { 
      m = 0;
      for (k=FirstFloor[j]; k<MaxFloor[j]; k++) {
         if(BitFun(LiftInOrder[j], k, 2)>0) { m++; }
      }
      IncallNum[j] = m;
   }

	 //下一楼层
   Do_Floor++;
   if(Do_Floor>GroupMaxFloor) { Do_Floor = GroupFirstFloor; }
	 
	 //-----------------------------------------------------------------------------------------------
   //上呼处理
   k = 0; 
   for(GP=0; GP<8; GP++) {  //8组循环
      if (BitFun(GroupUpOrder[GP], Do_Floor, 2)>0) { //某组中的登记
				 //计算该组各台梯的距离情况
	       for (j=0; j<8; j++) {
		        LiftDistance(j, 1, Do_Floor); 
		        COut[j] = ONum;
		        CIn[j] = INum;
		        C[j] = CNum;
			      if (BitFun(EnUpGroup[GP][j], Do_Floor, 2)==0) { L[j] = 0x1fffffff; } //本梯不参与本组群控
		        else if (C[j]>2) { L[j] = 0x1fffffff; } //登记号大于3,不接收登记
		        else { L[j] = FNum; }
						T[j] = FTime;
         }
				 
	       Seqence();

         m = N[0];
	       if (L[m]!=0x1fffffff) { //有梯可用
	          if (((LiftState[m]&0x00c0)==0x80)&&(LiftNowFloor[m]>Do_Floor)) { //同方向,走过头
	             for(j=1; j<8; j++) {
		              n = N[j];//次级距离梯号
		              if (L[n]==0x1fffffff) { break; } //次级不可用				  
		              if ((C[n]==0)&&(C[m]>0)&&((LiftFuntion&RunNoOrder)>0)&&(LiftOrderCounter[n]<=1)) { m = n; break; } //无登记
									if ((C[n]==0)&&(T[m]>=T[n])&&((LiftFuntion&RunNoOrderTimeCtrl)>0)) { m = n; break; } //无登记
				          if ((LiftNowFloor[n]>=Do_Floor)&&((LiftState[n]&0x00c0)==0x40)&&((LiftFuntion&RunOneInOrder)>0)) { 
			            if ((CIn[n]==1)&&(COut[n]==0)&&(BitFun(LiftInOrder[n], Do_Floor, 2)>0)
										&&((LiftState[n]&0x0300)==0x0300)&&(L[m]>3)&&(LiftRunFloor[m]!=Do_Floor))  { m = n; break; } //只有一个到达该层的内选
			            }
			            if ((LiftNowFloor[n]<=Do_Floor)&&((LiftState[n]&0x00c0)==0x80)&&((LiftFuntion&RunOneInOrder)>0)) {
			            if ((CIn[n]==1)&&(COut[n]==0)&&(BitFun(LiftInOrder[n], Do_Floor, 2)>0)
										&&((LiftState[n]&0x0300)==0x0300)&&(L[m]>3)&&(LiftRunFloor[m]!=Do_Floor))  { m = n; break; } //只有一个到达该层的内选
			            }
		           } 
            }
	          else if ((LiftState[m]&0x00c0)==0x40) {//首选梯反向
               if ((LiftNowFloor[m]>=Do_Floor)&&(CIn[m]==1)&&(COut[m]==0)
                 &&(BitFun(LiftInOrder[m], Do_Floor, 2)>0))  { ; } //只有一个到达该层的内选
		           else if (Do_Floor==FirstFloor[m])  { ; } //为本梯最低楼层
		           else if ((C[m]>0)||((LiftNowFloor[m]==Do_Floor)&&((LiftState[m]&0x0002)==0))) { //有别的登记,本层停止
                  for(j=1; j<8; j++) {
		                 n = N[j];//次级距离梯号
			               if (L[n]==0x1fffffff) { break; } //次级不可用
					           if ((C[n]==0)&&(C[m]>0)&&((LiftFuntion&RunNoOrder)>0)) { m = n; break; } //无登记
										 if ((C[n]==0)&&(T[m]>=T[n])&&((LiftFuntion&RunNoOrderTimeCtrl)>0)) { m = n; break; } //无登记
			               if ((LiftNowFloor[n]>=Do_Floor)&&((LiftState[n]&0x00c0)==0x40)&&((LiftFuntion&RunOneInOrder)>0)) { 
			               if ((CIn[n]==1)&&(COut[n]==0)&&(BitFun(LiftInOrder[n], Do_Floor, 2)>0)
											 &&((LiftState[n]&0x0300)==0x0300)&&(L[m]>3)&&(LiftRunFloor[m]!=Do_Floor))  { m = n; break; } //只有一个到达该层的内选
			               }
			               if ((LiftNowFloor[n]<=Do_Floor)&&((LiftState[n]&0x00c0)==0x80)&&((LiftFuntion&RunOneInOrder)>0)) {
			               if ((CIn[n]==1)&&(COut[n]==0)&&(BitFun(LiftInOrder[n], Do_Floor, 2)>0)
											 &&((LiftState[n]&0x0300)==0x0300)&&(L[m]>3)&&(LiftRunFloor[m]!=Do_Floor))  { m = n; break; } //只有一个到达该层的内选
			               }
			            } 
		           }
            }
						
	          BitFun(DoUpOrder[m], Do_Floor, 1);
		        k |= ((DWORD)1<<m); //标记某台电梯登记了，用于循环结束后处理
         }
	    }
   }
	 
   for(j=0; j<8; j++) {//8台电梯
	    if ((k&((DWORD)1<<j))==0) { //判断电梯是否无登记
	       if ((LiftNotMoveTimer[j]==0)&&(LiftOnline[j]>0)) { ; }//在群控状态下阻塞时不消改电梯外呼
		     else { BitFun(DoUpOrder[j], Do_Floor, 0); } 
	    }
   }

   k = 0;
   for(j=0; j<8; j++) {
      if ( BitFun(DoUpOrder[j], Do_Floor, 2) == BitFun(DoUpOrderOld[j], Do_Floor, 2) ) {
	       if (BitFun(DoUpOrder[j], Do_Floor, 2)>0) { 
		        BitFun(UpOrder[j], Do_Floor, 1); 
		        if (BitFun(LiftUpOrder[j], Do_Floor, 2)==0) { k = 1; } //没登记,其它不能消号
		     }
	    }
	    else { k = 1; }
   }

   if (k==0) {//允许其他消号
      for(j=0; j<8; j++) {
         if (BitFun(DoUpOrder[j], Do_Floor, 2)==0) { 
		        BitFun(UpOrder[j], Do_Floor, 0); 
		     }
	    }
   }

   for(j=0; j<8; j++) {//保全
      if (BitFun(DoUpOrder[j], Do_Floor, 2)==0)	{ BitFun(DoUpOrderOld[j], Do_Floor, 0); }
	    else                                      { BitFun(DoUpOrderOld[j], Do_Floor, 1); }
   }

   //下呼处理
   k = 0; 
   for(GP=0; GP<8; GP++) {
      if (BitFun(GroupDnOrder[GP], Do_Floor, 2)>0) {
	       for(j=0; j<8; j++) {
	          LiftDistance(j, 2, Do_Floor);
		        COut[j] = ONum;
		        CIn[j] = INum;
		        C[j] = CNum;
			      if (BitFun(EnDnGroup[GP][j], Do_Floor, 2)==0) { L[j] = 0x1fffffff; } //本梯不参与本组群控
		        else if (C[j]>2) { L[j] = 0x1fffffff; }//登记号大于3,不接收登记
			      else { L[j] = FNum; }
						T[j] = FTime;
				 }
				 
	          Seqence();
	          m = N[0];
						
	          if (L[m]!=0x1fffffff) { //有梯可用
		           if (((LiftState[m]&0x00c0)==0x40)&&(LiftNowFloor[m]<Do_Floor)) { //同方向,走过头
		              for(j=1; j<8; j++) {
		                 n = N[j];//次级距离梯号
		                 if (L[n]==0x1fffffff) { break; } //次级不可用
		                 if ((C[n]==0)&&(C[m]>0)&&((LiftFuntion&RunNoOrder)>0)&&(LiftOrderCounter[n]<=1)) { m = n; break; } //无登记
										 if ((C[n]==0)&&(T[m]>=T[n])&&((LiftFuntion&RunNoOrderTimeCtrl)>0)) { m = n; break; } //无登记
			               if ((LiftNowFloor[n]>=Do_Floor)&&((LiftState[n]&0x00c0)==0x40)&&((LiftFuntion&RunOneInOrder)>0)) { 
			               if ((CIn[n]==1)&&(COut[n]==0)&&(BitFun(LiftInOrder[n], Do_Floor, 2)>0)
											 &&((LiftState[n]&0x0300)==0x0300)&&(L[m]>3)&&(LiftRunFloor[m]!=Do_Floor))  { m = n; break; } //只有一个到达该层的内选
			               }
			               if ((LiftNowFloor[n]<=Do_Floor)&&((LiftState[n]&0x00c0)==0x80)&&((LiftFuntion&RunOneInOrder)>0)) {
			               if ((CIn[n]==1)&&(COut[n]==0)&&(BitFun(LiftInOrder[n], Do_Floor, 2)>0)
											 &&((LiftState[n]&0x0300)==0x0300)&&(L[m]>3)&&(LiftRunFloor[m]!=Do_Floor))  { m = n; break; } //只有一个到达该层的内选
			               }
		              }
               }
	             else if ((LiftState[m]&0x00c0)==0x80) {//首选梯反向
                  if ((LiftNowFloor[m]<=Do_Floor)&&(CIn[m]==1)&&(COut[m]==0)
                    &&(BitFun(LiftInOrder[m], Do_Floor, 2)>0))  { ; } //只有一个到达该层的内选
                  else if (Do_Floor==FirstFloor[m])  { ; } //为本梯最低楼层
		              else if ((C[m]>0)||((LiftNowFloor[m]==Do_Floor)&&((LiftState[m]&0x0002)==0))) { //有别的登记,本层停止
                     for(j=1; j<8; j++) {
		                    n = N[j];//次级距离梯号
			                  if (L[n]==0x1fffffff) { break; } //次级不可用
			                  if ((C[n]==0)&&(C[m]>0)&&((LiftFuntion&RunNoOrder)>0)) { m = n; break; } //无登记
										    if ((C[n]==0)&&(T[m]>=T[n])&&((LiftFuntion&RunNoOrderTimeCtrl)>0)) { m = n; break; } //无登记
			                  if ((LiftNowFloor[n]>=Do_Floor)&&((LiftState[n]&0x00c0)==0x40)&&((LiftFuntion&RunOneInOrder)>0)) { 
			                  if ((CIn[n]==1)&&(COut[n]==0)&&(BitFun(LiftInOrder[n], Do_Floor, 2)>0)
													&&((LiftState[n]&0x0300)==0x0300)&&(L[m]>3)&&(LiftRunFloor[m]!=Do_Floor))  { m = n; break; } //只有一个到达该层的内选
			               }
			               if ((LiftNowFloor[n]<=Do_Floor)&&((LiftState[n]&0x00c0)==0x80)&&((LiftFuntion&RunOneInOrder)>0)) {
			               if ((CIn[n]==1)&&(COut[n]==0)&&(BitFun(LiftInOrder[n], Do_Floor, 2)>0)
											 &&((LiftState[n]&0x0300)==0x0300)&&(L[m]>3)&&(LiftRunFloor[m]!=Do_Floor))  { m = n; break; } //只有一个到达该层的内选
			               }
		 	            } 
		           }
            }
							 
		        BitFun(DoDnOrder[m], Do_Floor, 1);
		        k |= ((DWORD)1<<m);
         }
	    }
   }
   for(j=0; j<8; j++) {
	    if ((k&((DWORD)1<<j))==0) {
	        if ((LiftNotMoveTimer[j]==0)&&(LiftOnline[j]>0)) { ; }//在群控状态下阻塞时不消改电梯外呼
		      else { BitFun(DoDnOrder[j], Do_Floor, 0); } 
	    }
   }

   k = 0;
   for(j=0; j<8; j++) {
      if ( BitFun(DoDnOrder[j], Do_Floor, 2) == BitFun(DoDnOrderOld[j], Do_Floor, 2) ) {
	       if (BitFun(DoDnOrder[j], Do_Floor, 2)>0) { 
		        BitFun(DnOrder[j], Do_Floor, 1); 
		        if (BitFun(LiftDnOrder[j], Do_Floor, 2)==0) { k = 1; } //没登记,其它不能消号
		     }
	    }
	    else { k = 1; }
   }

   if (k==0) {//允许其他消号
      for(j=0; j<8; j++) {
         if (BitFun(DoDnOrder[j], Do_Floor, 2)==0) { 
		        BitFun(DnOrder[j], Do_Floor, 0); 
		     }
	    }
   }

   for(j=0; j<8; j++) {//保全
      if (BitFun(DoDnOrder[j], Do_Floor, 2)==0)	{ BitFun(DoDnOrderOld[j], Do_Floor, 0); }
	    else                                      { BitFun(DoDnOrderOld[j], Do_Floor, 1); }
   }	 
}
/*****************************************************************************************************
*****************************************************************************************************/
void LiftCancelWait(DWORD wf, DWORD n)
{
   if ((LiftWaitFlag&((DWORD)1<<wf))>0) { //到达候梯楼层,并且发过内选
		  LiftWaitFlag &= ~((DWORD)1<<wf);
		  if ((LiftFuntion & BackNotDelInOrder)==0) {
          if(wf<4) { 
             if(CanTxOrder(1, wf, WaitFloor[wf][n], 0x10, 0)==TRUE) { LiftWaitFlag &= ~((DWORD)1<<wf); }  
          }
          else { 
             if(CanTxOrder(2, wf, WaitFloor[wf][n], 0x10, 0)==TRUE) { LiftWaitFlag &= ~((DWORD)1<<wf); }  
          }
		  }
   }  
}

void LiftWait(DWORD n)
{
   DWORD i,w[2];
   for(i=0; i<8; i++) {
      w[0] = LiftInOrder[i][0]; 
	    w[1] = LiftInOrder[i][1]; 
      BitFun(w, WaitFloor[i][n], 0);
	    if ((UpOrder[i][0]>0)||(UpOrder[i][1]>0)||
	        (DnOrder[i][0]>0)||(DnOrder[i][1]>0)||
	        (w[0]>0)||(w[1]>0)
		     ) {//有其他呼梯 
		     LiftCancelWait(i, n);
		     LiftWaitTimer[i] = 0; 
	    }	

	    if (LiftOnline[i]==0) { LiftWaitTimer[i] = 0; continue; } //退离群控
      if (LiftNowFloor[i]==WaitFloor[i][n]) { //到达候梯楼层
	       LiftCancelWait(i, n);
	       LiftWaitTimer[i] = 0; 
		     continue; 
	    }  
	    if ((LiftState[i]&0x00c0)>0) { LiftWaitTimer[i] = 0; continue; } //电梯有方向
	    if (WaitFloor[i][n]==0) { LiftWaitTimer[i] = 0; continue; } //此参数无效 
	    if ((WaitFloor[i][n]<FirstFloor[i])||(WaitFloor[i][n]>MaxFloor[i])) { LiftWaitTimer[i] = 0; continue; } //超出范围 
			
      if (LiftWaitTimer[i]<WaitFloorTime[n]) { LiftWaitTimer[i]++; continue; }//呼内选
	    else {//条件不变时,每5秒重发
	       if (i<4) { 
		        if (CanTxOrder(1, i, WaitFloor[i][n], 0x10, 1)==TRUE) { LiftWaitFlag |= ((DWORD)1<<i); }  
		     }
		     else { 
		        if (CanTxOrder(2, i, WaitFloor[i][n], 0x10, 1)==TRUE) { LiftWaitFlag |= ((DWORD)1<<i); }  
		     }
		     if (WaitFloorTime[n]>4) {
	           LiftWaitTimer[i] = WaitFloorTime[n]-5; 
		     }
		     else {
		         LiftWaitTimer[i] = 0; 
		     }
	    }
   }
}

void WaitTimeControl(void) {
   DWORD i,j;
   for(i=0; i<6; i++) {
      if (WaitStartTime[i] == WaitEndTime[i]) { //无设置的时间段
				 break;
			}
      j = (T_HOUR<<16)|(T_MIN<<8)|T_SEC; 
      if ( ( (WaitStartTime[i] <  WaitEndTime[i])&&((j>WaitStartTime[i])&&(j<WaitEndTime[i])) ) 
        || ( (WaitStartTime[i] >  WaitEndTime[i])&&((j>WaitStartTime[i])||(j<WaitEndTime[i])) ) ) //在高峰时间
	    { 
			   LiftWait(i); 
			   BaseFloorGroup = i;  
			   return;
			}
   }
	 
	 if (LiftFuntion&BackMode1) { CheckNoOrder(); BaseFloorGroup = 6; } //当前时间不在设置的时间段内,分散待梯
	 else                       { BaseFloorGroup = 7; } //无分散待梯
}
/*****************************************************************************************************
*****************************************************************************************************/
void LiftLong(DWORD Lift, DWORD OrderFlr)//计算距离
{
   if((SetAddr[Lift]==0)||
      (LiftOnline[Lift]==0)||
      (OrderFlr<FirstFloor[Lift])||
	    (OrderFlr>MaxFloor[Lift])||
	    (  (BitFun(RunEnUpOrder[Lift], OrderFlr, 2)==0)
	     &&(BitFun(RunEnDnOrder[Lift], OrderFlr, 2)==0)
      )
	 ) {
	     L[Lift] = 0x1fffffff; 
   }
   else if(LiftNowFloor[Lift]>=OrderFlr) { 
	     L[Lift] = LiftNowFloor[Lift]-OrderFlr; 
   }
   else {
	     L[Lift] = OrderFlr - LiftNowFloor[Lift]; 
   }
}


DWORD SeqPL[8];
void Seqence2(void)
{
   DWORD i,j,M;

   for(i=0; i<8; i++) {
	    SeqPL[i] = L[i];
   }

   for(i=0; i<8; i++) {
      for(M=0,j=1; j<8; j++) {
         if (SeqPL[j]<SeqPL[M]) { M = j; }
      }
      N[i] = M;
      SeqPL[M] = 0xffffffff;
   }
}

DWORD SearchLift(DWORD *En, DWORD floor)
{
   DWORD i;
   for(i=0; i<8; i++) {     
	    if((*En)&(1<<i)) { LiftLong(i, floor); }
	    else { L[i] = 0x1fffffff; }
   } 
   Seqence2();
   i = N[0];//最近电梯
   if((LiftNowFloor[i] != floor)&&(L[i] != 0x1fffffff))	{
      BackFloor[i] = floor;
      if(i<4) { 
         if(CanTxOrder(1, i, floor, 0x10, 1)==TRUE) { LiftWaitFlag |= ((DWORD)1<<i); }  
      }
      else { 
         if(CanTxOrder(2, i, floor, 0x10, 1)==TRUE) { LiftWaitFlag |= ((DWORD)1<<i); }  
      }
	    (*En)	&= ~(1<<i);
	    return(1);
   }
   else {
      LiftWaitFlag &= ~((DWORD)1<<i);//取消
	    (*En)	&= ~(1<<i);
	    return(0);//已经到达
   }
}

void WaitLiftSend(void) {
   DWORD i,j,k;
   j = 0;	
   k = 0;
   for(i=0; i<8; i++) {
   	  if(LiftOnline[i]>0) { j++; k |= (1<<i); } //计算在线电梯台数
   }
	 if (LiftFuntion&WaitData)	{ j = 8; } //只用第8组数据
   switch (j)
   {
   	case 2:
	    i = SearchLift(&k, WaitFloor2[0]);
		  if ((i==0)&&(WaitFloor2[1]>0)) { i = SearchLift(&k, WaitFloor2[1]); }
		  break;
	  case 3:
	    i = SearchLift(&k, WaitFloor3[0]);
		  if ((i==0)&&(WaitFloor3[1]>0)) { i = SearchLift(&k, WaitFloor3[1]); }
	    if ((i==0)&&(WaitFloor3[2]>0)) { i = SearchLift(&k, WaitFloor3[2]); }
		  break;
	  case 4:
	    i = SearchLift(&k, WaitFloor4[0]);
		  if ((i==0)&&(WaitFloor4[1]>0)) { i = SearchLift(&k, WaitFloor4[1]); }
	    if ((i==0)&&(WaitFloor4[2]>0)) { i = SearchLift(&k, WaitFloor4[2]); }
		  if ((i==0)&&(WaitFloor4[3]>0)) { i = SearchLift(&k, WaitFloor4[3]); }
		  break;
	  case 5:
	    i = SearchLift(&k, WaitFloor5[0]);
		  if ((i==0)&&(WaitFloor5[1]>0)) { i = SearchLift(&k, WaitFloor5[1]); }
	    if ((i==0)&&(WaitFloor5[2]>0)) { i = SearchLift(&k, WaitFloor5[2]); }
		  if ((i==0)&&(WaitFloor5[3]>0)) { i = SearchLift(&k, WaitFloor5[3]); }
		  if ((i==0)&&(WaitFloor5[4]>0)) { i = SearchLift(&k, WaitFloor5[4]); }
		  break;
	  case 6:
	    i = SearchLift(&k, WaitFloor6[0]);
		  if ((i==0)&&(WaitFloor6[1]>0)) { i = SearchLift(&k, WaitFloor6[1]); }
	    if ((i==0)&&(WaitFloor6[2]>0)) { i = SearchLift(&k, WaitFloor6[2]); }
		  if ((i==0)&&(WaitFloor6[3]>0)) { i = SearchLift(&k, WaitFloor6[3]); }
	    if ((i==0)&&(WaitFloor6[4]>0)) { i = SearchLift(&k, WaitFloor6[4]); }
		  if ((i==0)&&(WaitFloor6[5]>0)) { i = SearchLift(&k, WaitFloor6[5]); }
		  break;
	  case 7:
	    i = SearchLift(&k, WaitFloor7[0]);
		  if ((i==0)&&(WaitFloor7[1]>0)) { i = SearchLift(&k, WaitFloor7[1]); }
	    if ((i==0)&&(WaitFloor7[2]>0)) { i = SearchLift(&k, WaitFloor7[2]); }
		  if ((i==0)&&(WaitFloor7[3]>0)) { i = SearchLift(&k, WaitFloor7[3]); }
	    if ((i==0)&&(WaitFloor7[4]>0)) { i = SearchLift(&k, WaitFloor7[4]); }
		  if ((i==0)&&(WaitFloor7[5]>0)) { i = SearchLift(&k, WaitFloor7[5]); }
	    if ((i==0)&&(WaitFloor7[6]>0)) { i = SearchLift(&k, WaitFloor7[6]); }
		  break;
	case 8:
	    i = SearchLift(&k, WaitFloor8[0]);
		  if ((i==0)&&(WaitFloor8[1]>0)) { i = SearchLift(&k, WaitFloor8[1]); }
	    if ((i==0)&&(WaitFloor8[2]>0)) { i = SearchLift(&k, WaitFloor8[2]); }
		  if ((i==0)&&(WaitFloor8[3]>0)) { i = SearchLift(&k, WaitFloor8[3]); }
	    if ((i==0)&&(WaitFloor8[4]>0)) { i = SearchLift(&k, WaitFloor8[4]); }
		  if ((i==0)&&(WaitFloor8[5]>0)) { i = SearchLift(&k, WaitFloor8[5]); }
	    if ((i==0)&&(WaitFloor8[6]>0)) { i = SearchLift(&k, WaitFloor8[6]); }
		  if ((i==0)&&(WaitFloor8[7]>0)) { i = SearchLift(&k, WaitFloor8[7]); }
		  break;
	default:
		  break;
   }
   BaseFloorGroup = 0; 
}

void LiftCancelCall(DWORD n, DWORD f)
{
   if((LiftWaitFlag&((DWORD)1<<n))>0) { //到达候梯楼层,并且发过内选
		  LiftWaitFlag &= ~((DWORD)1<<n);
      if ((LiftFuntion &	BackNotDelInOrder)==0) {	 
           if(n<4) { 
               if(CanTxOrder(1, n, f, 0x10, 0)==TRUE) { LiftWaitFlag &= ~((DWORD)1<<n); }
           }
           else { 
               if(CanTxOrder(2, n, f, 0x10, 0)==TRUE) { LiftWaitFlag &= ~((DWORD)1<<n); } 
           }
			}
   }  
}
   
void CheckNoOrder(void)
{
   DWORD i,j,w[2];
   j=0;
   for(i=0; i<8; i++) {
      w[0] = LiftInOrder[i][0];
	    w[1] = LiftInOrder[i][1];
	    if ((LiftWaitFlag&((DWORD)1<<i))>0) {//回基站标志 
         BitFun(w, BackFloor[i], 0);
	    }																	 
	    if (LiftOnline[i]==0) { ; }
	    else if ( (UpOrder[i][0]>0)||(UpOrder[i][1]>0)||
	              (DnOrder[i][0]>0)||(DnOrder[i][1]>0)||
	              (w[0]>0)||(w[1]>0) ) 
			{//有其他呼梯 
		     j = 1;
	    }	
   }

/*********************************************************************
 * Distributed waiting mode, per-lift variant.
 *
 * Unlike CheckNoOrder(), this path starts an idle timer independently for
 * every elevator.  Each elevator selects the nearest unreserved configured
 * waiting floor for the current number of online elevators.
 *********************************************************************/
DWORD WaitFloorCount(void)
{
   DWORD i, count;

   count = 0;
   for (i=0; i<8; i++) {
      if (LiftOnline[i]>0) { count++; }
   }
   if (LiftFuntion&WaitData) { count = 8; }
   return(count);
}

DWORD WaitFloorByCount(DWORD count, DWORD index)
{
   switch (count)
   {
      case 2: return(WaitFloor2[index]);
      case 3: return(WaitFloor3[index]);
      case 4: return(WaitFloor4[index]);
      case 5: return(WaitFloor5[index]);
      case 6: return(WaitFloor6[index]);
      case 7: return(WaitFloor7[index]);
      case 8: return(WaitFloor8[index]);
      default: return(0);
   }
}

DWORD LiftIdleForWait(DWORD lift)
{
   DWORD order[2];

   if (LiftOnline[lift]==0) { return(0); }

   order[0] = LiftInOrder[lift][0];
   order[1] = LiftInOrder[lift][1];
   if (LiftWaitFlag&((DWORD)1<<lift)) {
      BitFun(order, BackFloor[lift], 0);
   }

   if ((UpOrder[lift][0]>0)||(UpOrder[lift][1]>0)||
       (DnOrder[lift][0]>0)||(DnOrder[lift][1]>0)||
       (order[0]>0)||(order[1]>0)||
       ((LiftState[lift]&0x00c0)>0)) {
      return(0);
   }
   return(1);
}

DWORD WaitFloorReserved(DWORD lift, DWORD floor)
{
   DWORD i;

   for (i=0; i<8; i++) {
      if ((i==lift)||(LiftOnline[i]==0)) { continue; }
      if ((LiftWaitFlag&((DWORD)1<<i))&&(BackFloor[i]==floor)) {
         return(1);
      }
      if ((LiftNowFloor[i]==floor)&&LiftIdleForWait(i)) {
         return(1);
      }
   }
   return(0);
}

DWORD FindNearestWaitFloor(DWORD lift, DWORD count)
{
   DWORD i, floor, distance, bestFloor, bestDistance, pass;

   bestFloor = 0;
   for (pass=0; pass<2; pass++) {
      bestDistance = 0xffffffff;
      for (i=0; i<count; i++) {
         floor = WaitFloorByCount(count, i);
         if ((floor==0)||(floor<FirstFloor[lift])||(floor>MaxFloor[lift])) {
            continue;
         }
         if ((pass==0)&&WaitFloorReserved(lift, floor)) { continue; }

         if (LiftNowFloor[lift]>=floor) { distance = LiftNowFloor[lift]-floor; }
         else                            { distance = floor-LiftNowFloor[lift]; }
         if (distance<bestDistance) {
            bestDistance = distance;
            bestFloor = floor;
         }
      }
      if (bestFloor>0) { return(bestFloor); }
   }
   return(0);
}

void CheckNoOrderIndividual(void)
{
   DWORD i, count, floor, can;

   count = WaitFloorCount();
   if ((count<2)||(count>8)) {
      for (i=0; i<8; i++) { LiftNoOrderTimer[i] = 0; }
      return;
   }

   for (i=0; i<8; i++) {
      if (LiftOnline[i]==0) {
         LiftNoOrderTimer[i] = 0;
         continue;
      }

      if ((LiftWaitFlag&((DWORD)1<<i))&&(LiftNowFloor[i]==BackFloor[i])) {
         LiftWaitFlag &= ~((DWORD)1<<i);
         LiftNoOrderTimer[i] = 0;
         continue;
      }

      if (!LiftIdleForWait(i)) {
         LiftCancelCall(i, BackFloor[i]);
         LiftNoOrderTimer[i] = 0;
         continue;
      }

      if (LiftWaitFlag&((DWORD)1<<i)) {
         LiftNoOrderTimer[i] = 0;
         continue;
      }

      if (LiftNoOrderTimer[i]<Mode2_WaitFloorTime) {
         LiftNoOrderTimer[i]++;
         continue;
      }

      floor = FindNearestWaitFloor(i, count);
      if ((floor==0)||(floor==LiftNowFloor[i])) {
         LiftNoOrderTimer[i] = 0;
         continue;
      }

      if (i<4) { can = CanTxOrder(1, i, floor, 0x10, 1); }
      else     { can = CanTxOrder(2, i, floor, 0x10, 1); }
      if (can==TRUE) {
         BackFloor[i] = floor;
         LiftWaitFlag |= ((DWORD)1<<i);
         LiftNoOrderTimer[i] = 0;
      }
   }
}

   if(j==1) {//有呼梯
      for(i=0; i<8; i++) {
		     LiftCancelCall(i, BackFloor[i]);//取消内选
      }
	    NoOrderTimer = 0;
	    StartupTime = Mode2_WaitFloorTime;//有呼梯时取菜单时间 
   }
   else {//无呼梯
      j=0;
      for(i=0; i<8; i++) {
	       if (LiftOnline[i]==0) { ; }
      	 else if((LiftState[i]&0x00c0)>0) { j = 1; } 
      }
	    if (j==0) {//电梯无方向
	       if(NoOrderTimer<StartupTime) { NoOrderTimer++;  }
		     else { 
		        WaitLiftSend();
			      StartupTime = 1;//1秒
		     }
	    }
	    else {
	       NoOrderTimer = 0; 
	    }
   }	  	
}
/*****************************************************************************************************
30号帧
字节6 // 电源状态改变报告标志
		位7 : 1 - 表示此字节内容有效, 0 - 此字节内容无效
		位6 : 暂不用，总为0
		位5 : 1 - 停电, 0 - 供电正常(恢复供电)
		位4 - 1 - 非消防, 0 - 消防
		位3 - 暂不用，总为0
		位2 - 1- 电梯不运行，0-电梯可运行
		位0,1  0-等待中 1-返回中 2-已返回，3-已返回并投入运行 

31号帧
字节6 // 电源状态改变报告标志
		位7 - 1 - 表示此字节内容有效, 0 - 此字节内容无效
		位6 - 暂不用
		位5 - 暂不用
		位4 - 暂不用
		位2 - 暂不用
		位1,0  0-无命令 1-返回命令 2-运行命令
*****************************************************************************************************/
void DelAllOrder(void)
{
   DWORD i,j;
   for(i=0; i<8; i++) { //消号处理
      for (j=0; i<MaxFloor[j]; j++) { //灭灯
		      if (BitFun(CanUpLed[i], j, 2)>0) { 
			        BitFun(CanUpLed[j], j, 0);
		          CanUpLedTimer[i][j] = 0;
					}
		      if (BitFun(CanUpLed[i], j, 2)>0) { 
			        BitFun(CanUpLed[j], j, 0);
		          CanUpLedTimer[i][j] = 0;
					}
			}	
			
		  for(j=0; j<2; j++) {
         LiftUpOrder[i][j] = 0;
         LiftDnOrder[i][j] = 0;
         LiftInOrder[i][j] = 0;
         
//				 LiftUpButton[i][j] = 0; LiftUpButtonB[i][j] = 0;
//         LiftDnButton[i][j] = 0; LiftDnButtonB[i][j] = 0;
         
         UpOrder[i][j] = 0;	DoUpOrder[i][j] = 0; DoUpOrderOld[i][j] = 0; 
         DnOrder[i][j] = 0;	DoDnOrder[i][j] = 0; DoDnOrderOld[i][j] = 0;
         UpOrderTemp[i][j] = 0;
         DnOrderTemp[i][j] = 0;
         //UpOrderSp[i][j] = 0;
         //DnOrderSp[i][j] = 0;
				
	    }		
   }
}
/*****************************************************************************************************
*****************************************************************************************************/


