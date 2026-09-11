/**********************************************************************/
#include "LPC23xx.h" /* LPC23xx/24xx Peripheral Registers */
#include "type.h"
#include "irq.h"
#include "target.h"
#include "absacc.h"
#include "timer.h"
#include "rtc.h"
#include "can.h"
#include "uart.h"
#include "lcd1602.h"

#define  MAIN_Ex
#include "main.h"
#include "IO.h"
#include "Call.h"
#include "Lift.h"
#include "Flash.h"
#include "Menu.h"
#include "LiftCan.h"
/*****************************************************************************/
//#define _dog  0

//int kkkkk;
int main ( void )
{ 
	 ConfigurePLL();

   #ifdef _dog
   WDMOD = 0x03;
   WDTC = 3000000;//3秒
   WDFEED = 0xaa;
   WDFEED = 0x55;
   #endif
	
	 //kkkkk = sizeof(LIFT_FUNCCODE);
		
//   while(*((const DWORD *)0x200)!=0x2e791126) { ; }

   GPIOInit();
   LiftInit();
   UARTInit(38400);
   RTCInit();
	 GetRTCTime();
   CANx_Init(1, BITRATE20K64MHZ );
   CANx_Init(2, BITRATE20K64MHZ );
   CAN_Init();
   LCD_Init();
   Menu_Ini();
	 
	 init_timer( 2, TIME2_INTERVAL );
	 T1mS = T2TC;
   T10mS = T2TC;
   T100mS = T2TC;
   T1S = T2TC;
	 T2S = T2TC;

   while(1)
   {
			 #ifdef _dog
       WDFEED = 0xaa;
       WDFEED = 0x55; 	
       #endif
   //#ifdef _debug
   //PasswordOK[0] = 1; 	
   //PasswordOK[1] = 1; 	
   //PasswordOK[2] = 1; 	
   //#endif
	  CanRx_Pop();
    LiftMain();
		 
	  if ((T2TC-T1mS)>10) { //1ms
	     T1mS = T2TC;
			 //if ((LiftFuntion&(StdFun|StdLedSendFun)) == (StdFun|StdLedSendFun)) { CanStdCheckLed(); }
			 //else                                                                { CanCheckLed(); }
			 CanCheckLed();
			 CanStdCheckLed();
			 CanTx_Pop();
			 MenuMain();
	  }
		
    if ((T2TC-T10mS)>100) { //10ms
	     T10mS = T2TC;
			 CanCheckOrder();
    }
		
    if ((T2TC-T100mS)>1000) { //100ms
	     T100mS = T2TC;
			 //CanSpecLed();
			
			 //if (LiftFuntion&ScanOrderEn100ms) { ScanOrder(); }//扫描查询呼梯
			 //CheckRecord_100ms();
			
		   Can1ResetEnable++;
		   if (Can1_OK==0) { ; } 
       else if ((Can1ErrCount > 100) || (Can1ResetEnable > 50)) {//5S
		       Can1ErrCount = 0;
		       Can1ResetEnable = 0;
           CANx_Init(1, BITRATE20K64MHZ );
       }
		   Can2ResetEnable++;
       if (Can2_OK==0) { ; } 
       else if ((Can2ErrCount > 100) || (Can2ResetEnable > 50)) {//5S
		       Can2ErrCount = 0;
		       Can2ResetEnable = 0;
           CANx_Init(2, BITRATE20K64MHZ );
       }
		   ScanOutput();
    }
		
	  if ((T2TC-T1S)>10000) { //1s
	     T1S+=10000;
			 GetRTCTime(); 
       if (LiftFuntion&WaitSpecial) { CheckNoOrderIndividual(); BaseFloorGroup = 6; }//ɢ
       Check_NoMove();
			 CanReSendOrder();
			 
       if (LiftFuntion&WaitSpecial) { CheckNoOrder(); BaseFloorGroup = 6; }//分散待梯
		   else                         { WaitTimeControl(); }//时间控制

			 TimeSetEnOrder();
			 CheckNoLiftEnable();
			 Check_DelInOrder(); //根据状态消内选指令
	  }
		
		if ((T2TC-T2S)>30000) { //3s
	     T2S+=30000;
			 if ( (SetAddr[0]>0)||(SetAddr[1]>0)||(SetAddr[2]>0)||(SetAddr[3]>0) ) { Can1_OK = 1; }
			 else                                                                  { Can1_OK = 0; }
			 if ( (SetAddr[4]>0)||(SetAddr[5]>0)||(SetAddr[6]>0)||(SetAddr[7]>0) ) { Can2_OK = 1; }
			 else                                                                  { Can2_OK = 0; }
		   if (Can1_OK>0) { CanTxOnLine(1); }
		   if (Can2_OK>0) { CanTxOnLine(2); }
			 Check_Online();
		   if (LiftFuntion&StdFun) { //自动设置
		       SetStd();
		   }
		}
  }
}

/**********************************************************************/
DWORD NoDirCallFloor[8];//没箭头呼梯楼层	 
DWORD LiftNoDirTimer[8];
void Check_NoMove(void)
{
   DWORD i,k,n;
   for(i=0; i<8; i++) {
       k = 0;		 
			 if ( ((LiftFuntion&LiftErrorDeal)>0) && (Lift_OK[i]>0) && (LiftOnline[i]>0) ) { //不起动不走模式
	         if ((LiftState[i]&0x00c0)==0) //电梯没有方向
	         {
				       if ( (UpOrder[i][0]>0)||(UpOrder[i][1]>0)||
	                  (DnOrder[i][0]>0)||(DnOrder[i][1]>0) ) 
					     { //有呼梯
						       if (LiftNoDirTimer[i]>0) { LiftNoDirTimer[i]--; }
								   else  //时间为0 
							     {  
			                 LiftNoDirTimer[i] = 3; 
				               NoDirCallFloor[i] = FirstFloor[i];
									     k = 1;
								   }  
					     } 
	         }
			 }
			 
			 while (k) 
			 {
			     NoDirCallFloor[i]++;
           if (NoDirCallFloor[i] > MaxFloor[i])   { NoDirCallFloor[i] = FirstFloor[i]; break; }
				   if (i<4) { n = 1; }
           else     { n = 2; }							 
				       
           if (BitFun(UpOrder[i], NoDirCallFloor[i], 2)==1)  { CanTxOrder(n, i, NoDirCallFloor[i], 0xa0, 1); k = 0; }
					 if (BitFun(DnOrder[i], NoDirCallFloor[i], 2)==1)  { CanTxOrder(n, i, NoDirCallFloor[i], 0x60, 1); k = 0; }
			 }
       			 
			 if (((LiftState[i]&0x00c0)>0)&&((LiftState[i]&0x0002)==0)) //有方向，没有运行
			 { 
           if (LiftNotMoveTimer[i]>0) { LiftNotMoveTimer[i]--; }
	     }
	     else { LiftNotMoveTimer[i] = LiftNotMove; }
   }
}
/**********************************************************************/

DWORD NoDirTimer[8];
void Check_DelInOrder(void)
{
   DWORD i,j;
   for(i=0; i<8; i++) {		   
		   if ((LiftState[i]&0x03c2)==0x000) { //关好门 没有方向 没有运行
			    if (NoDirTimer[i]<5) { NoDirTimer[i]++; } //5秒
					else {
					    LiftInOrder[i][0] = 0; LiftInOrder[i][1] = 0; 
			        LiftUpOrder[i][0] = 0; LiftUpOrder[i][1] = 0; //电梯的登记标志
              LiftDnOrder[i][0] = 0; LiftDnOrder[i][1] = 0;
              NoDirTimer[i] = 0;
			    }
		   }
       else {
			    NoDirTimer[i] = 0;
		   }		 
			 			 			
	     if ( ((LiftState[i]&0x0300)==0x300)   //开门到位
			   || ((LiftState[i]&0x0300)==0x100) ) //正在开门
		   {
				  j = LiftABSFloor[i];
			    BitFun(LiftInOrder[i], j, 0); //消内选
		   }		 
   }
}
/**********************************************************************/
void Check_Online(void)
{
   DWORD i;
   for(i=0; i<8; i++) {
      if (LiftOnline[i]) { LiftOnline[i]--; } 
	    if (Lift_OK[i])    { Lift_OK[i]--; }
			else { //无通讯时，清电梯的状态
			    LiftUpOrder[i][0] = 0; 
			    LiftDnOrder[i][0] = 0; 
			    LiftInOrder[i][0] = 0; 

					LiftUpOrder[i][1] = 0; 
			    LiftDnOrder[i][1] = 0; 
			    LiftInOrder[i][1] = 0; 

				  LiftState[i] = 0;
			}
   }
}
/**********************************************************************/
void SetStd(void)
{
   DWORD i,j;
   for(i=0; i<8; i++) {
      EnUpLift[0][i][0]  = EnUpOrder[i][0]; //8组电梯登记使能标志
      EnDnLift[0][i][0]  = EnDnOrder[i][0]; //8组电梯登记使能标志
      EnUpGroup[0][i][0] = EnUpOrder[i][0]; //8组电梯调度使能标志
      EnDnGroup[0][i][0] = EnDnOrder[i][0]; //8组电梯调度使能标志

      EnUpLift[0][i][1]  = EnUpOrder[i][1]; //8组电梯登记使能标志
      EnDnLift[0][i][1]  = EnDnOrder[i][1]; //8组电梯登记使能标志
      EnUpGroup[0][i][1] = EnUpOrder[i][1]; //8组电梯调度使能标志
      EnDnGroup[0][i][1] = EnDnOrder[i][1]; //8组电梯调度使能标志
   }
   for(j=1; j<8; j++) {
      for(i=0; i<8; i++) {
          EnUpLift[j][i][0]  = 0; //8组电梯登记使能标志
          EnDnLift[j][i][0]  = 0; //8组电梯登记使能标志
          EnUpGroup[j][i][0] = 0; //8组电梯调度使能标志
          EnDnGroup[j][i][0] = 0; //8组电梯调度使能标志

          EnUpLift[j][i][1]  = 0; //8组电梯登记使能标志
          EnDnLift[j][i][1]  = 0; //8组电梯登记使能标志
          EnUpGroup[j][i][1] = 0; //8组电梯调度使能标志
          EnDnGroup[j][i][1] = 0; //8组电梯调度使能标志
      }
   }
}
/**********************************************************************/
//设置禁呼时间判断
DWORD St, Et, timeOk;
DWORD CheckWeek(DWORD En)
{
	   DWORD j;
	
     if ((En&0x01)==0) { //该时间段无效	
			   return(0);
	   }
		 
		 St = ((En>>12)&0x7ff);  //高16为0,作时间标记
		 Et = ((En>>1)&0x7ff);
		 j  = (T_HOUR<<6)|T_MIN;

		 timeOk = 0;
		 if ( (St<Et) &&((j>=St)&&(j<Et)) ) { timeOk = 1; }  //开始时间<结束时间
		 if ( (St>=Et)&&(j>=St) ) { timeOk = 1; }            //开始时间>=结束时间时的当天  
		 if ( (St>=Et)&&(j<Et) )  { timeOk = 2; }            //开始时间>=结束时间 跨天  
		 
		 j = RTC_DOW;
     if (timeOk==2) { //跨天时取前一天的星期
		     if (j==0) { j = 6; }
		     else      { j--; }
     }
		 
		 if (j==0) {
				 j = 1<<6;
		 }
		 else {
				 j = (1ul<<(j-1));
		 }	
		 
		 if (((En>>23)&j)==0) { //无星期使能
			    return(0);
		 }
		 	 
		 if ( timeOk )  //时间使能
		 { 
					return(1);
		 }			 					 
		 return(0);    			 
}

/**********************************************************************/
//设置禁呼功能
DWORD mh,ml;
void TimeSetEnOrder(void)
{
   DWORD i,j,k;
	
	 for(i=0; i<8; i++) { //8台电梯
		   k = 0;
       mh = 0;
       ml = 0;				 
       for(j=0; j<8; j++) { //计算8个时间内，要禁止呼梯的楼层		
					 if ( (CheckWeek(OrderTimeEnable[i][j])>0) //当前时间在设置的时间内
             && ((LiftFuntion&DisableOrderFun)>0) )  //禁呼功能启动使能 
					 { 
						   ml |= OrderFlagEnable[i][j][0];
							 mh |= OrderFlagEnable[i][j][1];						 
							 k = 1;
					 }
			 }
			 
			 if (k) {
					 ml = ~ml; //取反作禁止楼层
					 mh = ~mh;
						 
					 RunEnUpOrder[i][0]  = EnUpOrder[i][0]  & ml; //8台梯A门呼梯使能运行时使用
					 RunEnDnOrder[i][0]  = EnDnOrder[i][0]  & ml;

					 RunEnUpOrder[i][1]  = EnUpOrder[i][1]  & mh; //8台梯A门呼梯使能运行时使用
					 RunEnDnOrder[i][1]  = EnDnOrder[i][1]  & mh;
			 }
			 else {
					 RunEnUpOrder[i][0]  = EnUpOrder[i][0];  //8台梯A门呼梯使能运行时使用
					 RunEnDnOrder[i][0]  = EnDnOrder[i][0];

					 RunEnUpOrder[i][1]  = EnUpOrder[i][1];  //8台梯A门呼梯使能运行时使用
					 RunEnDnOrder[i][1]  = EnDnOrder[i][1];
			 }
   }
}
/**********************************************************************/
//查询禁呼后没有电梯用，灭外呼灯
DWORD EnFlag[2],UpCal[2],DnCal[2];
void CheckNoLiftEnable(void)
{
   DWORD i,j;
	
	 EnFlag[0] = 0;
	 EnFlag[1] = 0; 
	 UpCal[0] = 0;
	 UpCal[1] = 0; 
	 DnCal[0] = 0;
	 DnCal[1] = 0; 	
	 for(i=0; i<8; i++) { //8台电梯
       if ( (SetAddr[i]>0)
				  &&(LiftOnline[i])
          &&(Lift_OK[i]) ) 
			 { 
			     EnFlag[0] |= RunEnUpOrder[i][0];  //8台梯A门呼梯使能运行时使用          
			     EnFlag[1] |= RunEnUpOrder[i][1];  //8台梯A门呼梯使能运行时使用          
			 }
			 
	     UpCal[0] |= GroupUpOrder[i][0];//8组登记标志有指令
	     UpCal[1] |= GroupUpOrder[i][1];
	     DnCal[0] |= GroupDnOrder[i][0];//8组登记标志有指令
	     DnCal[1] |= GroupDnOrder[i][1];

	     UpCal[0] |= CanUpLed[i][0];//8组登记标志有指令
	     UpCal[1] |= CanUpLed[i][1];
	     DnCal[0] |= CanDnLed[i][0];//8组登记标志有指令
	     DnCal[1] |= CanDnLed[i][1];
   }
	 
	 for(i=0; i<GroupMaxFloor; i++) {
		   if ((BitFun(EnFlag, i, 2)==0)&&(BitFun(UpCal, i, 2)>0)) {
				   for (j=0; j<8; j++) {
				 	      BitFun(CanUpLed[j], i, 0);
		            CanUpLedTimer[j][i] = 0;
				        BitFun(GroupUpOrder[j], i, 0);
					 }
			 }
		   if ((BitFun(EnFlag, i, 2)==0)&&(BitFun(DnCal, i, 2)>0)) {
				   for (j=0; j<8; j++) {
				 	      BitFun(CanDnLed[j], i, 0);
		            CanDnLedTimer[j][i] = 0;
				        BitFun(GroupDnOrder[j], i, 0);
					 }
			 }
	 }
}
/**********************************************************************/


