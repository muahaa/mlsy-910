/***********************************************************************
 **********************************************************************/
//驱动
#include "LPC23xx.h"
#include "type.h"
#include "irq.h"
#include "target.h"
#include "absacc.h"
#include "timer.h"
#include "rtc.h"
#include "can.h"
#include "uart.h"
//电梯
#include "main.h"
#include "IO.h"
#include "Lift.h"
#include "Flash.h"
#include "Menu.h"
#include "LiftCan.h"
/*-----------------------------------------------------------------------------------*/
void MaskProcess(DWORD *P, DWORD flr_min, DWORD flr_max)
{
      DWORD h,l,m;
	    if ((flr_min==0)||(flr_min>64)) { return; }
	    if ((flr_max==0)||(flr_max>64)) { return; }
			
      if (flr_max<32) {
				  l = (1ul<<flr_max)-1;
					h = 0;
      }
      else { 
				  l = 0xffffffff;
				  if (flr_max==32) {
				      h = 0;
          }
          else if (flr_max<64) {
				      h = (1ul<<(flr_max-32))-1;
          }
			    else {
				      h = l; 
			    }
		  }
			
			if (flr_min<32) {
				  m = (1ul<<(flr_min-1))-1;
          l &= ~m;
      }
      else { 
				  l = 0;
				  if (flr_min==64) {
				      h = 0; 
          }
          else if (flr_min>32) {
				      m = (1ul<<(flr_min-33))-1;
						  h &= ~m;
          }
		  }
			
	    *P  = l;     
			*(P+1) = h;
}

void P00_Init(void);
void P01_Init(void);
void P02_Init(void);
void P03_Init(void);
void P04_Init(void);
void P05_Init(void);
void P06_Init(void);
void P07_Init(void);
void P08_Init(void);
void P09_Init(void);
void P10_Init(void);
void P11_Init(void);
void P12_Init(void);
void P13_Init(void);
void P14_Init(void);
void P15_Init(void);
void P16_Init(void);
void P17_Init(void);
void P18_Init(void);
void P19_Init(void);
void P20_Init(void);
void P21_Init(void);
void P22_Init(void);
void P23_Init(void);
void P24_Init(void);
void P25_Init(void);
void P26_Init(void);
void P27_Init(void);
void P28_Init(void);
void P29_Init(void);
void P30_Init(void);
void P31_Init(void);
void Std_Init(DWORD n);
void P39_Init(void);
void P40_Init(void);
void P41_Init(void);
void P42_Init(void);
void P43_Init(void);
/**********************************************************************/
//程序处理开始
/**********************************************************************/
void InitData(DWORD x)
{
   DWORD i,j;
   //LiftFuntion = 0x03|0x80000000|0x40000000;
   LiftFuntion =     RunNoOrder        //调用无呼梯微机 
                 //| RunOneInOrder;    //调用一个内选微机
	               //  | ScanOrderEn100ms  //用菜单方式100ms查询登记指令
	               //  | LiftErrorDeal     //有指令给微机，但微机无方向，退离并且发内选
	                 | BackNotDelInOrder; //在待梯回基站时有外呼，不取消内选
                 //| PowerFunNoGroup   //发电机无群控功能
                 //| PowerFun          //发电机功能	 
                 //| TaiWangFun        //台湾残疾人外呼专用
                 //| WaitSpecial       //特殊等待楼层
                 //| RunSpecial;       //特殊功能处理
	 if ((x>=17)&&(x<=20)) { LiftFuntion |= TaiWangFun|BackMode1|StdFun|DisableOrderFun; }
	 if ((x<2)||((x>31)&&(x<39))) { LiftFuntion |= StdFun; } //标准群控标志

   FloorRunTime = 3; //电梯单层运行时间
   RunStopTime  = 10;//电梯停靠时间	
   RunOrderTime = 3; //电梯登记优先时间	
	 
   for(i=0; i<8; i++) {
      for(j=0; j<8; j++) {
				  OrderTimeEnable[i][j] = 0;
          OrderFlagEnable[i][j][0] = 0;
          OrderFlagEnable[i][j][1] = 0;
			}
	 }
	
   if (x!=3) {
      for (j=0; j<8; j++) { 
	       EnUpOrder[j][0]  = 0;     EnUpOrder[j][1]  = 0;
	       EnDnOrder[j][0]  = 0;     EnDnOrder[j][1]  = 0;
	       EnUpOrderB[j][0] = 0;     EnUpOrderB[j][1] = 0;	  
	       EnDnOrderB[j][0] = 0;     EnDnOrderB[j][1] = 0;    
      }
   }
	 
   for(i=0; i<8; i++) { NewFlag[i] = 1; }
   for(i=0; i<8; i++) {//所以组
	    for(j=0; j<8; j++) {
         EnUpLift[i][j][0]  = 0; EnUpLift[i][j][1]  = 0;
         EnDnLift[i][j][0]  = 0; EnDnLift[i][j][1]  = 0;
         EnUpGroup[i][j][0] = 0; EnUpGroup[i][j][1] = 0;
         EnDnGroup[i][j][0] = 0; EnDnGroup[i][j][1] = 0;
      }
   }
   for(i=0; i<8; i++) {//候梯方式1的候梯楼层全设为1
	    for(j=0; j<6; j++) {
	       WaitFloor[i][j] = 1;
      }
   }
   for(j=0; j<6; j++) {//候梯方式1的候梯时间设为120秒
      WaitFloorTime[j] = 120; 
	    WaitStartTime[j] = 0; 
	    WaitEndTime[j] = 0;
   }
   for(j=0; j<8; j++) {//候梯方式2的候梯楼层全设为1
	    WaitFloor8[j] = 1;
   }
   for(j=0; j<7; j++) {
	    WaitFloor7[j] = 1;
   }
   for(j=0; j<6; j++) {
	    WaitFloor6[j] = 1;
   }
   for(j=0; j<5; j++) {
	    WaitFloor5[j] = 1;
   }
   for(j=0; j<4; j++) {
	    WaitFloor4[j] = 1;
   }
   for(j=0; j<3; j++) {
	    WaitFloor3[j] = 1;
   }
   for(j=0; j<2; j++) {
	    WaitFloor2[j] = 1;
   }
   LiftNotMove = 40;
	 Mode2_WaitFloorTime = 30;//30秒
	 
	 for(i=0; i<8; i++) { 
		  PowerSetp[i] = i;
		  FireSetp[i] = i;
	 }
   if (((x>=4)&&(x<=31))||((x>=39)&&(x<=60))) {
	    P00_Init();
	 }
	 switch (x)
   {
	    case 0:  P00_Init(); break;
	    case 1:  P01_Init(); break;
		  case 2:  P02_Init(); break;
		  case 3:  P03_Init(); break;
		  case 4:  P04_Init(); break;
		  case 5:  P05_Init(); break;
		  case 6:  P06_Init(); break;
		  case 7:  P07_Init(); break;
		  case 8:  P08_Init(); break;
		  case 9:  P09_Init(); break;
		  case 10: P10_Init(); break;
		  case 11: P11_Init(); break;
		  case 12: P12_Init(); break;
		  case 13: P13_Init(); break;
		  case 14: P14_Init(); break;
		  case 15: P15_Init(); break;
		  case 16: P16_Init(); break;
		  case 17: P17_Init(); break;
		  case 18: P18_Init(); break;
		  case 19: P19_Init(); break;
		  case 20: P20_Init(); break;
		  case 21: P21_Init(); break;
		  case 22: P22_Init(); break;
		  case 23: P23_Init(); break;			
		  case 24: P24_Init(); break;	//IFE澳大利亚		
		  case 25: P25_Init(); break;	//备用		
		  case 26: P26_Init(); break;	//备用		
		  case 27: P27_Init(); break;	//备用		
		  case 28: P28_Init(); break;	//备用		
		  case 29: P29_Init(); break;	//备用		
		  case 30: P30_Init(); break;	//备用		
		  case 31: P31_Init(); break;	//备用		
		  case 32: Std_Init(2); break; //以最高楼层初始化，2层标准			
		  case 33: Std_Init(3); break; //以最高楼层初始化，3层标准			
		  case 34: Std_Init(4); break; //以最高楼层初始化，4层标准			
		  case 35: Std_Init(5); break; //以最高楼层初始化，5层标准			
		  case 36: Std_Init(6); break; //以最高楼层初始化，6层标准			
		  case 37: Std_Init(7); break; //以最高楼层初始化，7层标准			
		  case 38: Std_Init(8); break; //以最高楼层初始化，8层标准	
		  case 39: P39_Init(); break;	//备用	
		  case 40: P40_Init(); break;	//备用
		  case 41: P41_Init(); break;	//备用
		  case 42: P42_Init(); break;	//备用
		  case 43: P43_Init(); break;	//备用			
	    default: P00_Init(); break;
   }
}
/*-----------------------------------------------------------------------------------*/
//8台标准32层
void P00_Init(void)
{
	 DWORD i;
   GroupFirstFloor = 1;
   GroupMaxFloor = 32;
	 for(i=0; i<8; i++) {
	        SetAddr[i] = i+1;
	        FirstFloor[i] = 1;
	        MaxFloor[i] = 32; 
	        EnUpOrder[i][0] = 0xffffffff;	    //EnUpOrder[i][1] = 0x0;
	        EnDnOrder[i][0] = 0xffffffff;     //EnDnOrder[i][1] = 0x0;

	        EnUpLift[0][i][0]  = 0xffffffff;  //EnUpLift[0][i][1]  = 0;
          EnDnLift[0][i][0]  = 0xffffffff;  //EnDnLift[0][i][1]  = 0;
          EnUpGroup[0][i][0] = 0xffffffff;  //EnUpGroup[0][i][1] = 0;
          EnDnGroup[0][i][0] = 0xffffffff;  //EnDnGroup[0][i][1] = 0 ;
	 }
}
/*-----------------------------------------------------------------------------------*/
//先设置最高楼层，然后按梯数选择初始化
void Std_Init(DWORD n)
{
	 DWORD i;
	
	 GroupFirstFloor = 1;
	 if (GroupMaxFloor>64) { GroupMaxFloor = 8; }
	 for(i=0; i<8; i++) { 
	     FirstFloor[i] = 1; 
	     MaxFloor[i] = GroupMaxFloor;
	 }	  
	
	 for(i=0; i<8; i++) {		 
       MaskProcess(EnUpOrder[i], FirstFloor[i], MaxFloor[i]-1);
       MaskProcess(EnDnOrder[i], FirstFloor[i]+1, MaxFloor[i]);
       MaskProcess(EnUpOrderB[i], FirstFloor[i], MaxFloor[i]-1);
       MaskProcess(EnDnOrderB[i], FirstFloor[i]+1, MaxFloor[i]);  			
		 
       MaskProcess(EnUpLift[0][i], FirstFloor[i], MaxFloor[i]-1);
       MaskProcess(EnDnLift[0][i], FirstFloor[i]+1, MaxFloor[i]);
       MaskProcess(EnUpGroup[0][i], FirstFloor[i], MaxFloor[i]-1);
       MaskProcess(EnDnGroup[0][i], FirstFloor[i]+1, MaxFloor[i]);  			
	 }
	 
	 for(i=0; i<8; i++) {
		   if (i<n) { SetAddr[i] = i+1; }
		   else     { SetAddr[i] = 0; }			 
   }

	 WaitStartTime[0] = (0<<16)|(0<<8)|0; 
	 WaitEndTime[0] = (23<<16)|(59<<8)|59;
}

/*-----------------------------------------------------------------------------------*/
//按MaxFloor定义最高楼层
//按SetAddr[i]定义禁呼					 
void P01_Init(void)
{
	 DWORD i;
	 for(i=0; i<8; i++) {
		     if (SetAddr[i]>0) { 
		        MaskProcess(EnUpOrder[i], FirstFloor[i], MaxFloor[i]-1);
            MaskProcess(EnDnOrder[i], FirstFloor[i]+1, MaxFloor[i]);
            MaskProcess(EnUpOrderB[i], FirstFloor[i], MaxFloor[i]-1);
            MaskProcess(EnDnOrderB[i], FirstFloor[i]+1, MaxFloor[i]);  			
		 
            MaskProcess(EnUpLift[0][i], FirstFloor[i], MaxFloor[i]-1);
            MaskProcess(EnDnLift[0][i], FirstFloor[i]+1, MaxFloor[i]);
            MaskProcess(EnUpGroup[0][i], FirstFloor[i], MaxFloor[i]-1);
            MaskProcess(EnDnGroup[0][i], FirstFloor[i]+1, MaxFloor[i]);  			
		     }
	 }
	 for(i=0; i<8; i++) { FirstFloor[i] = 1; }
	 GroupFirstFloor = 1;
	 GroupMaxFloor = MaxFloor[0];
	 for(i=1; i<8; i++) {
	    if (MaxFloor[i]>GroupMaxFloor) { GroupMaxFloor = MaxFloor[i]; }
	 }
}
/*******************************************************************************/

//按MaxFloor定义最高楼层,3台单双层分开
void P02_Init(void)
{
	    DWORD j;
	    for(j=0; j<8; j++) { FirstFloor[j] = 1; }
	    GroupFirstFloor = 1;
	    GroupMaxFloor = MaxFloor[0];
	    for(j=1; j<3; j++) {
	       if (MaxFloor[j]>GroupMaxFloor) { GroupMaxFloor = MaxFloor[j]; }
	    }

	    SetAddr[0] = 3;
		  MaskProcess(EnUpOrder[0], FirstFloor[0], MaxFloor[0]-1);
      MaskProcess(EnDnOrder[0], FirstFloor[0]+1, MaxFloor[0]);
	  
	    SetAddr[1] = 2;  //双数层
		  MaskProcess(EnUpOrder[1], FirstFloor[1], MaxFloor[1]-1);
      MaskProcess(EnDnOrder[1], FirstFloor[1]+1, MaxFloor[1]);
	    EnUpOrder[1][0] &= 0xaaaaaaab;
	    EnUpOrder[1][1] &= 0xaaaaaaaa; 
	    EnDnOrder[1][0] &= 0xaaaaaaab;
	    EnDnOrder[1][1] &= 0xaaaaaaaa;

	    SetAddr[2] = 1;  //单数层
		  MaskProcess(EnUpOrder[2], FirstFloor[2], MaxFloor[2]-1);
      MaskProcess(EnDnOrder[2], FirstFloor[2]+1, MaxFloor[2]);
	    EnUpOrder[2][0] &= 0x55555555;
	    EnUpOrder[2][1] &= 0x55555555;
	    EnDnOrder[2][0] &= 0x55555555;
	    EnDnOrder[2][1] &= 0x55555555;
			
	    for(j=3; j<8; j++) {//其他 
	       SetAddr[j] = 0; 
	    } 

	    //第1组只调度1号梯
	    EnUpLift[0][0][0] = EnUpOrder[0][0];  
	    EnUpLift[0][0][1] = EnUpOrder[0][1];
	    EnDnLift[0][0][0] = 0;           
	    EnDnLift[0][0][1] = 0;
      EnUpGroup[0][0][0]= EnUpOrder[0][0];  
	    EnUpGroup[0][0][1]= EnUpOrder[1][1];
      EnDnGroup[0][0][0]= 0;  
	    EnDnGroup[0][0][1]= 0;

	    //第2组只调度1/2号梯	双数
	    EnUpLift[1][0][0] = 0;           
	    EnUpLift[1][0][1] = 0;
	    EnDnLift[1][0][0] = EnDnOrder[1][0];           
	    EnDnLift[1][0][1] = EnDnOrder[1][1];
      EnUpGroup[1][0][0]= EnUpOrder[1][0];           
	    EnUpGroup[1][0][1]= EnUpOrder[1][1]; 
      EnDnGroup[1][0][0]= EnDnOrder[1][0]; 
	    EnDnGroup[1][0][1]= EnDnOrder[1][1];

	    EnUpLift[1][1][0] = EnUpOrder[1][0];           
	    EnUpLift[1][1][1] = EnUpOrder[1][1]; 
	    EnDnLift[1][1][0] = EnDnOrder[1][0];           
	    EnDnLift[1][1][1] = EnDnOrder[1][1];
      EnUpGroup[1][1][0]= EnUpOrder[1][0];           
	    EnUpGroup[1][1][1]= EnUpOrder[1][1]; 
      EnDnGroup[1][1][0]= EnDnOrder[1][0]; 
	    EnDnGroup[1][1][1]= EnDnOrder[1][1];

	    //第3组只调度1/3号梯	单数
	    EnUpLift[2][0][0] = 0;           
	    EnUpLift[2][0][1] = 0;
	    EnDnLift[2][0][0] = EnDnOrder[2][0];           
	    EnDnLift[2][0][1] = EnDnOrder[2][1];
      EnUpGroup[2][0][0]= EnUpOrder[2][0];           
	    EnUpGroup[2][0][1]= EnUpOrder[2][1];
      EnDnGroup[2][0][0]= EnDnOrder[2][0];
	    EnDnGroup[2][0][1]= EnDnOrder[2][1];

	    EnUpLift[2][2][0] = EnUpOrder[2][0];           
	    EnUpLift[2][2][1] = EnUpOrder[2][1];
	    EnDnLift[2][2][0] = EnDnOrder[2][0];          
	    EnDnLift[2][2][1] = EnDnOrder[2][1];
      EnUpGroup[2][2][0]= EnUpOrder[2][0];           
	    EnUpGroup[2][2][1]= EnUpOrder[2][1];
      EnDnGroup[2][2][0]= EnDnOrder[2][0]; 
	    EnDnGroup[2][2][1]= EnDnOrder[2][1];
}
/*******************************************************************************/

//按MaxFloor定义最高楼层,Enable定义第4组,3台单双层分开
void P03_Init(void)
{
	    DWORD i,j,m,n;
	    DWORD mask[2];
	
      for(i=0; i<8; i++) {
	       FirstFloor[i] = 1;
	    }
	    GroupFirstFloor = 1;
	    GroupMaxFloor = MaxFloor[0];
	    for(i=1; i<3; i++) {
	       if(MaxFloor[i]>GroupMaxFloor) { GroupMaxFloor = MaxFloor[i]; }
	    }

	    SetAddr[0] = 3;  //层层停
	    SetAddr[1] = 2;  //双数层
	    SetAddr[2] = 1;  //单数层

	    i = EnUpOrder[0][0]^EnUpOrder[1][0];//异或
	    i |= EnUpOrder[0][0]^EnUpOrder[2][0];//异或
	    i |= EnUpOrder[1][0]^EnUpOrder[2][0];//异或
	    m = EnUpOrder[0][0]&(~i);//取相同的位

	    j = EnDnOrder[0][0]^EnDnOrder[1][0];//异或
	    j |= EnDnOrder[0][0]^EnDnOrder[2][0];//异或
	    j |= EnDnOrder[1][0]^EnDnOrder[2][0];//异或
	    n = EnDnOrder[0][0]&(~j);//取相同的位

	    WaitFloor3[0] = GroupMaxFloor/2;
	    WaitFloor3[1] = 1;
	    WaitFloor3[2] = 1;
			
	    //第1组只调度1号梯
		  MaskProcess(mask, FirstFloor[0], MaxFloor[0]-1);			
      EnUpLift[0][0][0]  = mask[0];
	    EnUpLift[0][0][1]  = mask[1];
      EnDnLift[0][0][0]  = 0;
	    EnDnLift[0][0][1]  = 0;
      EnUpGroup[0][0][0] = mask[0];
	    EnUpGroup[0][0][1] = mask[1];
      EnDnGroup[0][0][0] = 0; 
	    EnDnGroup[0][0][1] = 0;
	    for(j=1; j<8; j++) {
         EnUpLift[0][j][0]  = 0x00000000; EnUpLift[0][j][1]  = 0x00000000;
         EnDnLift[0][j][0]  = 0x00000000; EnDnLift[0][j][1]  = 0x00000000;
         EnUpGroup[0][j][0] = 0x00000000; EnUpGroup[0][j][1] = 0x00000000;
         EnDnGroup[0][j][0] = 0x00000000; EnDnGroup[0][j][1] = 0x00000000;
	    }
	    //第2组只调度1/3号梯	双数
			MaskProcess(mask, FirstFloor[0], MaxFloor[0]);
      EnUpLift[1][0][0]  = 0; 
	    EnUpLift[1][0][1]  = 0;
      EnDnLift[1][0][0]  = mask[0]&0xaaaaaaab & (~n);
	    EnDnLift[1][0][1]  = mask[1]&0xaaaaaaaa;
      EnUpGroup[1][0][0] = mask[0]&0xaaaaaaab | m;
	    EnUpGroup[1][0][1] = mask[1]&0xaaaaaaaa;
      EnDnGroup[1][0][0] = mask[0]&0xaaaaaaab & (~n);
	    EnDnGroup[1][0][1] = mask[1]&0xaaaaaaaa;

			MaskProcess(mask, FirstFloor[1], MaxFloor[1]);
      EnUpLift[1][1][0]  = mask[0]&0xaaaaaaab | m;
	    EnUpLift[1][1][1]  = mask[1]&0xaaaaaaaa;
      EnDnLift[1][1][0]  = mask[0]&0xaaaaaaab & (~n);
	    EnDnLift[1][1][1]  = mask[1]&0xaaaaaaaa;
      EnUpGroup[1][1][0] = mask[0]&0xaaaaaaab | m;
	    EnUpGroup[1][1][1] = mask[1]&0xaaaaaaaa;
      EnDnGroup[1][1][0] = mask[0]&0xaaaaaaab & (~n);
	    EnDnGroup[1][1][1] = mask[1]&0xaaaaaaaa;
	    //第3组只调度1/2号梯	奇数
			MaskProcess(mask, FirstFloor[0], MaxFloor[0]);
      EnUpLift[2][0][0]  = 0; 
	    EnUpLift[2][0][1]  = 0;
      EnDnLift[2][0][0]  = mask[0]&0x55555555 & (~n); 
	    EnDnLift[2][0][1]  = mask[1]&0x55555555;
      EnUpGroup[2][0][0] = mask[0]&0x55555555 | m;
	    EnUpGroup[2][0][1] = mask[1]&0x55555555;
      EnDnGroup[2][0][0] = mask[0]&0x55555555 & (~n);
	    EnDnGroup[2][0][1] = mask[1]&0x55555555;

			MaskProcess(mask, FirstFloor[2], MaxFloor[2]);
      EnUpLift[2][2][0]  = mask[0]&0x55555555 | m;
	    EnUpLift[2][2][1]  = mask[1]&0x55555555;
      EnDnLift[2][2][0]  = mask[0]&0x55555555 & (~n);
	    EnDnLift[2][2][1]  = mask[1]&0x55555555;
      EnUpGroup[2][2][0] = mask[0]&0x55555555 | m;
	    EnUpGroup[2][2][1] = mask[1]&0x55555555;
      EnDnGroup[2][2][0] = mask[0]&0x55555555 & (~n);
	    EnDnGroup[2][2][1] = mask[1]&0x55555555;

	  //第4组只调度1/2/3号梯
      EnUpLift[3][0][0]  = 0x00000000; 
	    EnUpLift[3][0][1]  = 0x00000000;
      EnDnLift[3][0][0]  = n; 
	    EnDnLift[3][0][1]  = 0x00000000;
      EnUpGroup[3][0][0] = 0x00000000; 
	    EnUpGroup[3][0][1] = 0x00000000;
      EnDnGroup[3][0][0] = n; 
	    EnDnGroup[3][0][1] = 0x00000000;

      EnUpLift[3][1][0]  = 0x00000000; 
	    EnUpLift[3][1][1]  = 0x00000000;
      EnDnLift[3][1][0]  = n; 
	    EnDnLift[3][1][1]  = 0x00000000;
      EnUpGroup[3][1][0] = 0x00000000; 
	    EnUpGroup[3][1][1] = 0x00000000;
      EnDnGroup[3][1][0] = n; 
	    EnDnGroup[3][1][1] = 0x00000000;

      EnUpLift[3][2][0]  = 0x00000000; 
	    EnUpLift[3][2][1]  = 0x00000000;
      EnDnLift[3][2][0]  = n; 
	    EnDnLift[3][2][1]  = 0x00000000;
      EnUpGroup[3][2][0] = 0x00000000; 
	    EnUpGroup[3][2][1] = 0x00000000;
      EnDnGroup[3][2][0] = n; 
	    EnDnGroup[3][2][1] = 0x00000000;
}
/*******************************************************************************/

//_debug_floor14 曾少强
void P04_Init(void)
{
      LiftFuntion = 0x03;

	    SetAddr[0] = 4;
	    FirstFloor[0] = 3;
	    MaxFloor[0] = 14; 
	    EnUpOrder[0][0] = 0x00001ffc;
	    EnUpOrder[0][1] = 0x0;
	    EnDnOrder[0][0] = 0x00003ff8;
	    EnDnOrder[0][1] = 0x0;
	    NewFlag[0] = 1;

	    SetAddr[1] = 2;
	    FirstFloor[1] = 3;
	    MaxFloor[1] = 14; 
	    EnUpOrder[1][0] = 0x00001ffc;
	    EnUpOrder[1][1] = 0x0;
	    EnDnOrder[1][0] = 0x00003ff8;
	    EnDnOrder[1][1] = 0x0;
	    NewFlag[1] = 1;

	    SetAddr[2] = 1;
	    FirstFloor[2] = 1;
	    MaxFloor[2] = 14; 
	    EnUpOrder[2][0] = 0x00001fff;
	    EnUpOrder[2][1] = 0x0;
	    EnDnOrder[2][0] = 0x00003ffe;
	    EnDnOrder[2][1] = 0x0;
	    NewFlag[2] = 1;

      GroupFirstFloor = 1;
	    GroupMaxFloor = 14;

	    WaitFloor[0][0] = 3;
	    WaitFloor[1][0] = 3;
	    WaitFloor[2][0] = 3;
	    WaitStartTime[0] = (8<<16)|(30<<8)|0; 
	    WaitEndTime[0] = (9<<16)|(15<<8)|0;
	    WaitFloorTime[0] = 1;

	    WaitFloor[0][1] = 3;
	    WaitFloor[1][1] = 8;
	    WaitFloor[2][1] = 3;
	    WaitStartTime[1] = 0; 
	    WaitEndTime[1] = 0;
	    WaitFloorTime[1] = 120;

	    EnUpLift[0][0][0]  = 0x00001ffc; //EnUpLift[0][0][1]  = 0x00000000;
      EnDnLift[0][0][0]  = 0x00003ff8; //EnDnLift[0][0][1]  = 0x00000000;
      EnUpGroup[0][0][0] = 0x00001ffc; //EnUpGroup[0][0][1] = 0x00000000;
      EnDnGroup[0][0][0] = 0x00003ff8; //EnDnGroup[0][0][1] = 0x00000000;

      EnUpLift[0][1][0]  = 0x00001ffc; //EnUpLift[0][1][1]  = 0x00000000;
      EnDnLift[0][1][0]  = 0x00003ff8; //EnDnLift[0][1][1]  = 0x00000000;
      EnUpGroup[0][1][0] = 0x00001ffc; //EnUpGroup[0][1][1] = 0x00000000;
      EnDnGroup[0][1][0] = 0x00003ff8; //EnDnGroup[0][1][1] = 0x00000000;

      EnUpLift[0][2][0]  = 0x00001fff; //EnUpLift[0][2][1]  = 0x00000000;
      EnDnLift[0][2][0]  = 0x00003ffe; //EnDnLift[0][2][1]  = 0x00000000;
      EnUpGroup[0][2][0] = 0x00001fff; //EnUpGroup[0][2][1] = 0x00000000;
      EnDnGroup[0][2][0] = 0x00003ffe; //EnDnGroup[0][2][1] = 0x00000000;
}
/*******************************************************************************/

//_debug_floor16 曾少强
void P05_Init(void)
{
    LiftFuntion = 0x03;

	  SetAddr[0] = 2;
	  FirstFloor[0] = 3;
	  MaxFloor[0] = 16; 
	  EnUpOrder[0][0] = 0x00007f84;
	  EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0x0000ff80;
	  EnDnOrder[0][1] = 0x0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 4;
	  FirstFloor[1] = 3;
	  MaxFloor[1] = 16; 
	  EnUpOrder[1][0] = 0x00007f84;
	  EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = 0x0000ff80;
	  EnDnOrder[1][1] = 0x0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 1;
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 16; 
	  EnUpOrder[2][0] = 0x00007f87;
	  EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0x0000ff86;
	  EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 16;

	  WaitFloor[0][0] = 3;
	  WaitFloor[1][0] = 3;
	  WaitFloor[2][0] = 3;
	  WaitStartTime[0] = (8<<16)|(30<<8)|0; 
	  WaitEndTime[0] = (9<<16)|(15<<8)|0;
	  WaitFloorTime[0] = 1;

	  WaitFloor[0][1] = 3;
	  WaitFloor[1][1] = 8;
	  WaitFloor[2][1] = 3;
	  WaitStartTime[1] = (0<<16)|(0<<8)|0; 
	  WaitEndTime[1] = (0<<16)|(0<<8)|0;
	  WaitFloorTime[1] = 120;

	  EnUpLift[0][0][0]  = 0x00007f84; //EnUpLift[0][0][1]  = 0x00000000;
    EnDnLift[0][0][0]  = 0x0000ff80; //EnDnLift[0][0][1]  = 0x00000000;
    EnUpGroup[0][0][0] = 0x00007f84; //EnUpGroup[0][0][1] = 0x00000000;
    EnDnGroup[0][0][0] = 0x0000ff80; //EnDnGroup[0][0][1] = 0x00000000;

    EnUpLift[0][1][0]  = 0x00007f84; //EnUpLift[0][1][1]  = 0x00000000;
    EnDnLift[0][1][0]  = 0x0000ff80; //EnDnLift[0][1][1]  = 0x00000000;
    EnUpGroup[0][1][0] = 0x00007f84; //EnUpGroup[0][1][1] = 0x00000000;
    EnDnGroup[0][1][0] = 0x0000ff80; //EnDnGroup[0][1][1] = 0x00000000;

    EnUpLift[0][2][0]  = 0x00007f87; //EnUpLift[0][2][1]  = 0x00000000;
    EnDnLift[0][2][0]  = 0x0000ff86; //EnDnLift[0][2][1]  = 0x00000000;
    EnUpGroup[0][2][0] = 0x00007f87; //EnUpGroup[0][2][1] = 0x00000000;
    EnDnGroup[0][2][0] = 0x0000ff86; //EnDnGroup[0][2][1] = 0x00000000;
}
/*******************************************************************************/

//_debug_group_floor32_5fsame	曾少强
void P06_Init(void)
{
	  SetAddr[0] = 1;
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 32; 
	  EnUpOrder[0][0] = 0x7fffffff;
	  //EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0xfffffffe;
	  //EnDnOrder[0][1] = 0x0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2;
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 32; 
	  EnUpOrder[1][0] = 0x5555555f;
	  //EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = 0x5555555e;
	  //EnDnOrder[1][1] = 0x0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 4;
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 32; 
	  EnUpOrder[2][0] = 0x2aaaaabf;
	  //EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0xaaaaaabe;
	  //EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 32;

	  WaitFloor[0][0] = 16;
	  WaitFloor[1][0] = 18;
	  WaitFloor[2][0] = 20;
	  WaitStartTime[0] = (7<<16)|(30<<8)|0; 
	  WaitEndTime[0] = (10<<16)|(0<<8)|0;
	  WaitFloorTime[0] = 1;

	  WaitFloor[0][1] = 1;
	  WaitFloor[1][1] = 1;
	  WaitFloor[2][1] = 1;
	  WaitStartTime[1] = (5<<16)|(0<<8)|0; 
	  WaitEndTime[1] = (7<<16)|(0<<8)|0;
	  WaitFloorTime[1] = 1;

	  WaitFloor[0][2] = 1;
	  WaitFloor[1][2] = 1;
	  WaitFloor[2][2] = 20;
	  WaitStartTime[2] = 0; 
	  WaitEndTime[2] = 0;
	  WaitFloorTime[2] = 30;
	  //第1组只调度1号梯
      EnUpLift[0][0][0]  = 0xffffffff; //EnUpLift[0][0][1]  = 0x00000000;
      EnDnLift[0][0][0]  = 0x00000000; //EnDnLift[0][0][1]  = 0x00000000;
      EnUpGroup[0][0][0] = 0xffffffff; //EnUpGroup[0][0][1] = 0x00000000;
      EnDnGroup[0][0][0] = 0x00000000; //EnDnGroup[0][0][1] = 0x00000000;
	    
	  //第2组只调度1/2号梯	奇数
      EnUpLift[1][0][0]  = 0x00000000; //EnUpLift[1][0][1]  = 0x00000000;
      EnDnLift[1][0][0]  = 0x55555540; //EnDnLift[1][0][1]  = 0x00000000;
      EnUpGroup[1][0][0] = 0x5555555f; //EnUpGroup[1][0][1] = 0x00000000;
      EnDnGroup[1][0][0] = 0x5555555f; //EnDnGroup[1][0][1] = 0x00000000;

      EnUpLift[1][1][0]  = 0x5555555f; //EnUpLift[1][1][1]  = 0x00000000;
      EnDnLift[1][1][0]  = 0x55555540; //EnDnLift[1][1][1]  = 0x00000000;
      EnUpGroup[1][1][0] = 0x5555555f; //EnUpGroup[1][1][1] = 0x00000000;
      EnDnGroup[1][1][0] = 0x5555555f; //EnDnGroup[1][1][1] = 0x00000000;
	  //第3组只调度1/3号梯	双数
      EnUpLift[3][0][0]  = 0x00000000; //EnUpLift[3][0][1]  = 0x00000000;
      EnDnLift[3][0][0]  = 0xaaaaaaa0; //EnDnLift[3][0][1]  = 0x00000000;
      EnUpGroup[3][0][0] = 0xaaaaaabf; //EnUpGroup[3][0][1] = 0x00000000;
      EnDnGroup[3][0][0] = 0xaaaaaabf; //EnDnGroup[3][0][1] = 0x00000000;

      EnUpLift[3][2][0]  = 0xaaaaaabf; //EnUpLift[3][2][1]  = 0x00000000;
      EnDnLift[3][2][0]  = 0xaaaaaaa0; //EnDnLift[3][2][1]  = 0x00000000;
      EnUpGroup[3][2][0] = 0xaaaaaabf; //EnUpGroup[3][2][1] = 0x00000000;
      EnDnGroup[3][2][0] = 0xaaaaaabf; //EnDnGroup[3][2][1] = 0x00000000;
	  //第4组只调度1/2/3号梯
      EnUpLift[4][0][0]  = 0x00000000; //EnUpLift[4][0][1]  = 0x00000000;
      EnDnLift[4][0][0]  = 0x0000001f; //EnDnLift[4][0][1]  = 0x00000000;
      EnUpGroup[4][0][0] = 0x00000000; //EnUpGroup[4][0][1] = 0x00000000;
      EnDnGroup[4][0][0] = 0x0000001f; //EnDnGroup[4][0][1] = 0x00000000;

      EnUpLift[4][1][0]  = 0x00000000; //EnUpLift[4][1][1]  = 0x00000000;
      EnDnLift[4][1][0]  = 0x0000001f; //EnDnLift[4][1][1]  = 0x00000000;
      EnUpGroup[4][1][0] = 0x00000000; //EnUpGroup[4][1][1] = 0x00000000;
      EnDnGroup[4][1][0] = 0x0000001f; //EnDnGroup[4][1][1] = 0x00000000;

      EnUpLift[4][2][0]  = 0x00000000; //EnUpLift[4][2][1]  = 0x00000000;
      EnDnLift[4][2][0]  = 0x0000001f; //EnDnLift[4][2][1]  = 0x00000000;
      EnUpGroup[4][2][0] = 0x00000000; //EnUpGroup[4][2][1] = 0x00000000;
      EnDnGroup[4][2][0] = 0x0000001f; //EnDnGroup[4][2][1] = 0x00000000;
}
/*******************************************************************************/

//_debug_group_floor28
void P07_Init(void)
{		
	  SetAddr[0] = 3;
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 28; 
	  EnUpOrder[0][0] = 0x07fffff7;
	  EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0x0ffffffa;
	  EnDnOrder[0][1] = 0x0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 1;
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 27; 
	  EnUpOrder[1][0] = 0x05555553;
	  EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = 0x05555553;
	  EnDnOrder[1][1] = 0x0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 2;
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 28; 
	  EnUpOrder[2][0] = 0x02aaaaab;
	  EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0x0aaaaaab;
	  EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 28;

	  WaitFloor3[0] = 14;
	  WaitFloor3[1] = 1;
	  WaitFloor3[2] = 1;
		
	  //第1组只调度1号梯
      EnUpLift[0][0][0]  = 0xfffffff7; //EnUpLift[0][0][1]  = 0x00000000;
      EnDnLift[0][0][0]  = 0x00000000; //EnDnLift[0][0][1]  = 0x00000000;
      EnUpGroup[0][0][0] = 0xfffffff7; //EnUpGroup[0][0][1] = 0x00000000;
      EnDnGroup[0][0][0] = 0x00000000; //EnDnGroup[0][0][1] = 0x00000000;

      //第2组只调度1/2号梯	奇数
      EnUpLift[1][0][0]  = 0x00000000; //EnUpLift[1][0][1]  = 0x00000000;
      EnDnLift[1][0][0]  = 0x55555554; //EnDnLift[1][0][1]  = 0x00000000;
      EnUpGroup[1][0][0] = 0x55555553; //EnUpGroup[1][0][1] = 0x00000000;
      EnDnGroup[1][0][0] = 0x55555553; //EnDnGroup[1][0][1] = 0x00000000;

      EnUpLift[1][1][0]  = 0x55555553; //EnUpLift[1][1][1]  = 0x00000000;
      EnDnLift[1][1][0]  = 0x55555554; //EnDnLift[1][1][1]  = 0x00000000;
      EnUpGroup[1][1][0] = 0x55555553; //EnUpGroup[1][1][1] = 0x00000000;
      EnDnGroup[1][1][0] = 0x55555553; //EnDnGroup[1][1][1] = 0x00000000;
	  //第3组只调度1/3号梯	双数
      EnUpLift[2][0][0]  = 0x00000000; //EnUpLift[2][0][1]  = 0x00000000;
      EnDnLift[2][0][0]  = 0xaaaaaaa8; //EnDnLift[2][0][1]  = 0x00000000;
      EnUpGroup[2][0][0] = 0xaaaaaaab; //EnUpGroup[2][0][1] = 0x00000000;
      EnDnGroup[2][0][0] = 0xaaaaaaaa; //EnDnGroup[2][0][1] = 0x00000000;

      EnUpLift[2][2][0]  = 0xaaaaaaab; //EnUpLift[2][2][1]  = 0x00000000;
      EnDnLift[2][2][0]  = 0xaaaaaaa0; //EnDnLift[2][2][1]  = 0x00000000;
      EnUpGroup[2][2][0] = 0xaaaaaaab; //EnUpGroup[2][2][1] = 0x00000000;
      EnDnGroup[2][2][0] = 0xaaaaaaaa; //EnDnGroup[2][2][1] = 0x00000000;
	  //第4组只调度1/2/3号梯
      EnUpLift[3][0][0]  = 0x00000000; //EnUpLift[3][0][1]  = 0x00000000;
      EnDnLift[3][0][0]  = 0x00000003; //EnDnLift[3][0][1]  = 0x00000000;
      EnUpGroup[3][0][0] = 0x00000000; //EnUpGroup[3][0][1] = 0x00000000;
      EnDnGroup[3][0][0] = 0x00000003; //EnDnGroup[3][0][1] = 0x00000000;

      EnUpLift[3][1][0]  = 0x00000000; //EnUpLift[3][1][1]  = 0x00000000;
      EnDnLift[3][1][0]  = 0x00000003; //EnDnLift[3][1][1]  = 0x00000000;
      EnUpGroup[3][1][0] = 0x00000000; //EnUpGroup[3][1][1] = 0x00000000;
      EnDnGroup[3][1][0] = 0x00000003; //EnDnGroup[3][1][1] = 0x00000000;

      EnUpLift[3][2][0]  = 0x00000000; //EnUpLift[3][2][1]  = 0x00000000;
      EnDnLift[3][2][0]  = 0x00000003; //EnDnLift[3][2][1]  = 0x00000000;
      EnUpGroup[3][2][0] = 0x00000000; //EnUpGroup[3][2][1] = 0x00000000;
      EnDnGroup[3][2][0] = 0x00000003; //EnDnGroup[3][2][1] = 0x00000000;
}
/*******************************************************************************/

void P08_Init(void)
{//兴托大夏
	  SetAddr[0] = 1;
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 28; 
	  EnUpOrder[0][0] = 0x07ffffff;
	  EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0x0ffffffe;
	  EnDnOrder[0][1] = 0x0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2;
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 27; 
	  EnUpOrder[1][0] = 0x05555557;
	  EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = 0x05555557;
	  EnDnOrder[1][1] = 0x0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 4;
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 28; 
	  EnUpOrder[2][0] = 0x02aaaaab;
	  EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0x0aaaaaab;
	  EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 28;

	  WaitFloor3[0] = 13;
	  WaitFloor3[1] = 1;
	  WaitFloor3[2] = 1;
	  //第1组只调度1号梯
      EnUpLift[0][0][0]  = 0x07fffffb; //EnUpLift[0][0][1]  = 0x00000000;
      EnDnLift[0][0][0]  = 0x00000000; //EnDnLift[0][0][1]  = 0x00000000;
      EnUpGroup[0][0][0] = 0x07fffffb; //EnUpGroup[0][0][1] = 0x00000000;
      EnDnGroup[0][0][0] = 0x00000000; //EnDnGroup[0][0][1] = 0x00000000;
	  //第2组只调度1/2号梯	奇数
      EnUpLift[1][0][0]  = 0x00000000; //EnUpLift[1][0][1]  = 0x00000000;
      EnDnLift[1][0][0]  = 0x55555554; //EnDnLift[1][0][1]  = 0x00000000;
      EnUpGroup[1][0][0] = 0x55555553; //EnUpGroup[1][0][1] = 0x00000000;
      EnDnGroup[1][0][0] = 0x55555553; //EnDnGroup[1][0][1] = 0x00000000;

      EnUpLift[1][1][0]  = 0x55555557; //EnUpLift[1][1][1]  = 0x00000000;
      EnDnLift[1][1][0]  = 0x55555554; //EnDnLift[1][1][1]  = 0x00000000;
      EnUpGroup[1][1][0] = 0x55555557; //EnUpGroup[1][1][1] = 0x00000000;
      EnDnGroup[1][1][0] = 0x55555557; //EnDnGroup[1][1][1] = 0x00000000;
	  //第3组只调度1/3号梯	双数
      EnUpLift[3][0][0]  = 0x00000000; //EnUpLift[3][0][1]  = 0x00000000;
      EnDnLift[3][0][0]  = 0xaaaaaaa8; //EnDnLift[3][0][1]  = 0x00000000;
      EnUpGroup[3][0][0] = 0xaaaaaaab; //EnUpGroup[3][0][1] = 0x00000000;
      EnDnGroup[3][0][0] = 0xaaaaaaaa; //EnDnGroup[3][0][1] = 0x00000000;

      EnUpLift[3][2][0]  = 0xaaaaaaab; //EnUpLift[3][2][1]  = 0x00000000;
      EnDnLift[3][2][0]  = 0xaaaaaaa0; //EnDnLift[3][2][1]  = 0x00000000;
      EnUpGroup[3][2][0] = 0xaaaaaaab; //EnUpGroup[3][2][1] = 0x00000000;
      EnDnGroup[3][2][0] = 0xaaaaaaaa; //EnDnGroup[3][2][1] = 0x00000000;
	  //第4组只调度1/2/3号梯
      EnUpLift[4][0][0]  = 0x00000000; //EnUpLift[4][0][1]  = 0x00000000;
      EnDnLift[4][0][0]  = 0x00000003; //EnDnLift[4][0][1]  = 0x00000000;
      EnUpGroup[4][0][0] = 0x00000000; //EnUpGroup[4][0][1] = 0x00000000;
      EnDnGroup[4][0][0] = 0x00000003; //EnDnGroup[4][0][1] = 0x00000000;

      EnUpLift[4][1][0]  = 0x00000000; //EnUpLift[4][1][1]  = 0x00000000;
      EnDnLift[4][1][0]  = 0x00000003; //EnDnLift[4][1][1]  = 0x00000000;
      EnUpGroup[4][1][0] = 0x00000000; //EnUpGroup[4][1][1] = 0x00000000;
      EnDnGroup[4][1][0] = 0x00000003; //EnDnGroup[4][1][1] = 0x00000000;

      EnUpLift[4][2][0]  = 0x00000000; //EnUpLift[4][2][1]  = 0x00000000;
      EnDnLift[4][2][0]  = 0x00000003; //EnDnLift[4][2][1]  = 0x00000000;
      EnUpGroup[4][2][0] = 0x00000000; //EnUpGroup[4][2][1] = 0x00000000;
      EnDnGroup[4][2][0] = 0x00000003; //EnDnGroup[4][2][1] = 0x00000000;
}
/*******************************************************************************/

//_debug_group_floor26 澳门建富新村
void P09_Init(void)
{   
    DWORD j;
	
    for(j=0; j<8; j++) { FirstFloor[j] = 1; }
    GroupFirstFloor = 1;

	  SetAddr[0] = 3;  //层层停
	  MaxFloor[0] = 26;

	  SetAddr[1] = 2;  //单数层
	  MaxFloor[1] = 25; 
		
	  SetAddr[2] = 1;  //双数层
	  MaxFloor[2] = 26; 
		
    for(j=0; j<3; j++) { 
		    MaskProcess(EnUpOrder[j], FirstFloor[j], MaxFloor[j]-1);
        MaskProcess(EnDnOrder[j], FirstFloor[j]+1, MaxFloor[j]);
        MaskProcess(EnUpOrderB[j], FirstFloor[j], MaxFloor[j]-1);
        MaskProcess(EnDnOrderB[j], FirstFloor[j]+1, MaxFloor[j]);  			
		}
	
	  //单数层
	  EnUpOrder[1][0] = EnUpOrder[1][0] & 0x55555555 | 0x03;
	  EnUpOrder[1][1] = EnUpOrder[1][1] & 0x55555555;
	  EnDnOrder[1][0] = EnDnOrder[1][0] & 0x55555554 | 0x02;
	  EnDnOrder[1][1] = EnDnOrder[1][1] & 0x55555555;

	  //双数层
	  EnUpOrder[2][0] = EnUpOrder[2][0] & 0xaaaaaaab | 0x03;
	  EnUpOrder[2][1] = EnUpOrder[2][1] & 0xaaaaaaaa; 
	  EnDnOrder[2][0] = EnDnOrder[2][0] & 0xaaaaaaaa;
	  EnDnOrder[2][1] = EnDnOrder[2][1] & 0xaaaaaaaa;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 26;

	  WaitFloor3[0] = 13;
	  WaitFloor3[1] = 1;
	  WaitFloor3[2] = 1;
	  //第1组只调度1号梯
    EnUpLift[0][0][0]  = EnUpOrder[0][0];
	  EnUpLift[0][0][1]  = EnUpOrder[0][1];
    //EnDnLift[0][0][0]  = 0;
	  //EnDnLift[0][0][1]  = 0;
    EnUpGroup[0][0][0] = EnUpOrder[0][0];
	  EnUpGroup[0][0][1] = EnUpOrder[0][1];
    //EnDnGroup[0][0][0] = 0; 
	  //EnDnGroup[0][0][1] = 0;

      //第2组只调度1/2号梯	奇数
    //EnUpLift[1][0][0]  = 0; 
	  //EnUpLift[1][0][1]  = 0;
    EnDnLift[1][0][0]  = EnUpOrder[0][0]&0x55555554 & (~0x03); 
	  EnDnLift[1][0][1]  = EnUpOrder[0][1]&0x55555555;
    EnUpGroup[1][0][0] = EnUpOrder[0][0]&0x55555555 | 0x03;
	  EnUpGroup[1][0][1] = EnUpOrder[0][1]&0x55555555;
    EnDnGroup[1][0][0] = EnDnOrder[0][0]&0x55555554 & (~0x03);
	  EnDnGroup[1][0][1] = EnDnOrder[0][1]&0x55555555;

    EnUpLift[1][1][0]  = EnUpOrder[1][0]&0x55555555 | 0x03;
	  EnUpLift[1][1][1]  = EnUpOrder[1][1]&0x55555555;
    EnDnLift[1][1][0]  = EnDnOrder[1][0]&0x55555554 & (~0x03);
	  EnDnLift[1][1][1]  = EnDnOrder[1][1]&0x55555555;
    EnUpGroup[1][1][0] = EnUpOrder[1][0]&0x55555555 | 0x03;
	  EnUpGroup[1][1][1] = EnUpOrder[1][1]&0x55555555;
    EnDnGroup[1][1][0] = EnDnOrder[1][0]&0x55555554 & (~0x03);
	  EnDnGroup[1][1][1] = EnDnOrder[1][1]&0x55555555;
	  //第3组只调度1/3号梯	双数
    //EnUpLift[2][0][0]  = 0; 
	  //EnUpLift[2][0][1]  = 0;
    EnDnLift[2][0][0]  = EnDnOrder[2][0]&0xaaaaaaaa & (~0x03);
	  EnDnLift[2][0][1]  = EnDnOrder[2][1]&0xaaaaaaaa;
    EnUpGroup[2][0][0] = EnUpOrder[2][0]&0xaaaaaaab | 0x03;
	  EnUpGroup[2][0][1] = EnUpOrder[2][1]&0xaaaaaaaa;
    EnDnGroup[2][0][0] = EnDnOrder[2][0]&0xaaaaaaaa & (~0x03);
	  EnDnGroup[2][0][1] = EnDnOrder[2][1]&0xaaaaaaaa;

    EnUpLift[2][2][0]  = EnUpOrder[2][0]&0xaaaaaaab | 0x03;
	  EnUpLift[2][2][1]  = EnUpOrder[2][1]&0xaaaaaaaa;
    EnDnLift[2][2][0]  = EnDnOrder[2][0]&0xaaaaaaaa & (~0x03);
	  EnDnLift[2][2][1]  = EnDnOrder[2][1]&0xaaaaaaaa;
    EnUpGroup[2][2][0] = EnUpOrder[2][0]&0xaaaaaaab | 0x03;
	  EnUpGroup[2][2][1] = EnUpOrder[2][1]&0xaaaaaaaa;
    EnDnGroup[2][2][0] = EnDnOrder[2][0]&0xaaaaaaaa & (~0x03);
	  EnDnGroup[2][2][1] = EnDnOrder[2][1]&0xaaaaaaaa;
	  //第4组只调度1/2/3号梯
    //EnUpLift[3][0][0]  = 0x00000000; 
	  //EnUpLift[3][0][1]  = 0x00000000;
    EnDnLift[3][0][0]  = 0x00000002; 
	  //EnDnLift[3][0][1]  = 0x00000000;
    //EnUpGroup[3][0][0] = 0x00000000; 
	  //EnUpGroup[3][0][1] = 0x00000000;
    EnDnGroup[3][0][0] = 0x00000002; 
	  //EnDnGroup[3][0][1] = 0x00000000;

    //EnUpLift[3][1][0]  = 0x00000000; 
	  //EnUpLift[3][1][1]  = 0x00000000;
    EnDnLift[3][1][0]  = 0x00000002; 
	  //EnDnLift[3][1][1]  = 0x00000000;
    //EnUpGroup[3][1][0] = 0x00000000; 
	  //EnUpGroup[3][1][1] = 0x00000000;
    EnDnGroup[3][1][0] = 0x00000002; 
	  //EnDnGroup[3][1][1] = 0x00000000;

    //EnUpLift[3][2][0]  = 0x00000000; 
	  //EnUpLift[3][2][1]  = 0x00000000;
    EnDnLift[3][2][0]  = 0x00000002; 
	  //EnDnLift[3][2][1]  = 0x00000000;
    //EnUpGroup[3][2][0] = 0x00000000; 
	  //EnUpGroup[3][2][1] = 0x00000000;
    EnDnGroup[3][2][0] = 0x00000002; 
	  //EnDnGroup[3][2][1] = 0x00000000;
}
/*******************************************************************************/

//新宝花园
void P10_Init(void)
{		
    SetAddr[0] = 3;
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 31; 
	  EnUpOrder[0][0] = 0x3fffffff & (~((DWORD)1<<(14-1)));//无14
	  EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0x7ffffffe & (~((DWORD)1<<(14-1)));//无14
	  EnDnOrder[0][1] = 0x0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2;//1到13单层，16到30双层  101010101010101001010101010111
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 30; 
	  EnUpOrder[1][0] = 0xAAAA9557 & (~((DWORD)1<<(14-1))) & (~((DWORD)7<<(30-1)));//无14 30 31 32
	  EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = 0xAAAA9556 & (~((DWORD)1<<(14-1))) & (~((DWORD)3<<(31-1)));//无14 31 32
	  EnDnOrder[1][1] = 0x0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 1;//1到12双层，15到31单层  1010101010101010100101010101001
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 31; 
	  EnUpOrder[2][0] = 0x55554AA9 & (~((DWORD)1<<(14-1))) & (~((DWORD)3<<(31-1)));//无14 31 32
	  EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0x55554AA8 & (~((DWORD)1<<(14-1))) & (~((DWORD)1<<(32-1)));//无14 32
	  EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 31;

	  WaitFloor3[0] = 16;
	  WaitFloor3[1] = 1;
	  WaitFloor3[2] = 1;
	  //第1组只调度1号梯
    EnUpLift[0][0][0]  = 0x3fffdfff; //EnUpLift[0][0][1]  = 0x00000000;
    EnDnLift[0][0][0]  = 0x00000000; //EnDnLift[0][0][1]  = 0x00000000;
    EnUpGroup[0][0][0] = 0x3fffdfff; //EnUpGroup[0][0][1] = 0x00000000;
    EnDnGroup[0][0][0] = 0x00000000; //EnDnGroup[0][0][1] = 0x00000000;

    //第2组只调度1/2号梯	奇数
      EnUpLift[1][0][0]  = 0x00000000; //EnUpLift[1][0][1]  = 0x00000000;
      EnDnLift[1][0][0]  = 0x2AAA9557; //EnDnLift[1][0][1]  = 0x00000000;
      EnUpGroup[1][0][0] = 0x2AAA9557; //EnUpGroup[1][0][1] = 0x00000000;
      EnDnGroup[1][0][0] = 0x2AAA9557; //EnDnGroup[1][0][1] = 0x00000000;

      EnUpLift[1][1][0]  = 0x2AAA9557; //EnUpLift[1][1][1]  = 0x00000000;
      EnDnLift[1][1][0]  = 0x2AAA9557; //EnDnLift[1][1][1]  = 0x00000000;
      EnUpGroup[1][1][0] = 0x2AAA9557; //EnUpGroup[1][1][1] = 0x00000000;
      EnDnGroup[1][1][0] = 0x2AAA9557; //EnDnGroup[1][1][1] = 0x00000000;
	  //第3组只调度1/3号梯	双数
      EnUpLift[2][0][0]  = 0x00000000; //EnUpLift[2][0][1]  = 0x00000000;
      EnDnLift[2][0][0]  = 0x55554AA9; //EnDnLift[2][0][1]  = 0x00000000;
      EnUpGroup[2][0][0] = 0x55554AA9; //EnUpGroup[2][0][1] = 0x00000000;
      EnDnGroup[2][0][0] = 0x55554AA9; //EnDnGroup[2][0][1] = 0x00000000;

      EnUpLift[2][2][0]  = 0x55554AA9; //EnUpLift[2][2][1]  = 0x00000000;
      EnDnLift[2][2][0]  = 0x55554AA9; //EnDnLift[2][2][1]  = 0x00000000;
      EnUpGroup[2][2][0] = 0x55554AA9; //EnUpGroup[2][2][1] = 0x00000000;
      EnDnGroup[2][2][0] = 0x55554AA9; //EnDnGroup[2][2][1] = 0x00000000;
}
/*******************************************************************************/
	 
//_debug_group_floor33
void P11_Init(void)
{	  
	  SetAddr[0] = 3; //层层停
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 33; 
	  EnUpOrder[0][0] = 0xffffffff;
	  EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0xfffffffe;
	  EnDnOrder[0][1] = 0x1;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2; //单层停
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 33; 
	  EnUpOrder[1][0] = 0x55555555;
	  EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = 0x55555555;
	  EnDnOrder[1][1] = 0x1;
	  NewFlag[1] = 1;

	  SetAddr[2] = 1; //双层停
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 32; 
	  EnUpOrder[2][0] = 0xaaaaaaab;
	  EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0xaaaaaaab;
	  EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 33;

	  WaitFloor3[0] = 16;
	  WaitFloor3[1] = 1;
	  WaitFloor3[2] = 1;
	  //第1组只调度1号梯
      EnUpLift[0][0][0]  = 0x0000001f; //EnUpLift[0][0][1]  = 0x00000000;
      EnDnLift[0][0][0]  = 0x00000000; //EnDnLift[0][0][1]  = 0x00000000;
      EnUpGroup[0][0][0] = 0x0000001f; //EnUpGroup[0][0][1] = 0x00000000;
      EnDnGroup[0][0][0] = 0x00000000; //EnDnGroup[0][0][1] = 0x00000000;
	    
	  //第2组只调度1/2号梯	奇数
      EnUpLift[1][0][0]  = 0x55555540; //EnUpLift[1][0][1]  = 0x00000000;
      EnDnLift[1][0][0]  = 0x55555555; EnDnLift[1][0][1]  = 0x00000001;
      EnUpGroup[1][0][0] = 0x55555555; //EnUpGroup[1][0][1] = 0x00000000;
      EnDnGroup[1][0][0] = 0x55555555; EnDnGroup[1][0][1] = 0x00000001;

      EnUpLift[1][1][0]  = 0x55555555; //EnUpLift[1][1][1]  = 0x00000000;
      EnDnLift[1][1][0]  = 0x55555555; EnDnLift[1][1][1]  = 0x00000001;
      EnUpGroup[1][1][0] = 0x55555555; //EnUpGroup[1][1][1] = 0x00000000;
      EnDnGroup[1][1][0] = 0x55555555; EnDnGroup[1][1][1] = 0x00000001;
	  //第3组只调度1/3号梯	双数
      EnUpLift[2][0][0]  = 0xaaaaaaa0; //EnUpLift[2][0][1]  = 0x00000000;
      EnDnLift[2][0][0]  = 0xaaaaaaaa; //EnDnLift[2][0][1]  = 0x00000000;
      EnUpGroup[2][0][0] = 0xaaaaaaab; //EnUpGroup[2][0][1] = 0x00000000;
      EnDnGroup[2][0][0] = 0xaaaaaaaa; //EnDnGroup[2][0][1] = 0x00000000;

      EnUpLift[2][2][0]  = 0xaaaaaaab; //EnUpLift[2][2][1]  = 0x00000000;
      EnDnLift[2][2][0]  = 0xaaaaaaaa; //EnDnLift[2][2][1]  = 0x00000000;
      EnUpGroup[2][2][0] = 0xaaaaaaab; //EnUpGroup[2][2][1] = 0x00000000;
      EnDnGroup[2][2][0] = 0xaaaaaaaa; //EnDnGroup[2][2][1] = 0x00000000;
}
/*******************************************************************************/

//_debug_group_floor22
void P12_Init(void)
{
		SetAddr[0] = 3;
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 22; 
	  EnUpOrder[0][0] = 0x001fffff;
	  EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0x003ffffe;
	  EnDnOrder[0][1] = 0x0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2;
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 21; 
	  EnUpOrder[1][0] = 0x000fffff;
	  EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = 0x001ffffe;
	  EnDnOrder[1][1] = 0x0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 1;
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 21; 
	  EnUpOrder[2][0] = 0x000fffff;
	  EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0x001ffffe;
	  EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 22;

	  WaitFloor[0][0] = 13;
	  WaitFloor[1][0] = 1;
	  WaitFloor[2][0] = 1;
	  WaitStartTime[0] = 0; 
	  WaitEndTime[0] = 0;
	  WaitFloorTime[0] = 120;
	  //第1组只调度1号梯
      EnUpLift[0][0][0]  = 0x003fffff; //EnUpLift[0][0][1]  = 0x00000000;
      EnDnLift[0][0][0]  = 0x00200000; //EnDnLift[0][0][1]  = 0x00000000;
      EnUpGroup[0][0][0] = 0x003fffff; //EnUpGroup[0][0][1] = 0x00000000;
      EnDnGroup[0][0][0] = 0x00200000; //EnDnGroup[0][0][1] = 0x00000000;
	  //第2组只调度1/2/3号梯 
      EnUpLift[1][0][0]  = 0x00000000; //EnUpLift[1][0][1]  = 0x00000000;
      EnDnLift[1][0][0]  = 0x001fffff; //EnDnLift[1][0][1]  = 0x00000000;
      EnUpGroup[1][0][0] = 0x001fffff; //EnUpGroup[1][0][1] = 0x00000000;
      EnDnGroup[1][0][0] = 0x001fffff; //EnDnGroup[1][0][1] = 0x00000000;

      EnUpLift[1][1][0]  = 0x001fffff; //EnUpLift[1][1][1]  = 0x00000000;
      EnDnLift[1][1][0]  = 0x001fffff; //EnDnLift[1][1][1]  = 0x00000000;
      EnUpGroup[1][1][0] = 0x001fffff; //EnUpGroup[1][1][1] = 0x00000000;
      EnDnGroup[1][1][0] = 0x001fffff; //EnDnGroup[1][1][1] = 0x00000000;

      EnUpLift[1][2][0]  = 0x001fffff; //EnUpLift[1][2][1]  = 0x00000000;
      EnDnLift[1][2][0]  = 0x001fffff; //EnDnLift[1][2][1]  = 0x00000000;
      EnUpGroup[1][2][0] = 0x001fffff; //EnUpGroup[1][2][1] = 0x00000000;
      EnDnGroup[1][2][0] = 0x001fffff; //EnDnGroup[1][2][1] = 0x00000000;
}
/*******************************************************************************/
	 
//_debug_group_floor20
void P13_Init(void)
{
  	SetAddr[0] = 3;
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 20; 
	  EnUpOrder[0][0] = 0x0007ffff;
	  EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0x000ffffe;
	  EnDnOrder[0][1] = 0x0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2;
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 19; 
	  EnUpOrder[1][0] = 0x0003ffff;
	  EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = 0x0007fffe;
	  EnDnOrder[1][1] = 0x0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 1;
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 19; 
	  EnUpOrder[2][0] = 0x0003ffff;
	  EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0x0007fffe;
	  EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 20;

	  WaitFloor[0][0] = 13;
	  WaitFloor[1][0] = 1;
	  WaitFloor[2][0] = 1;
	  WaitStartTime[0] = 0; 
	  WaitEndTime[0] = 0;
	  WaitFloorTime[0] = 120;
	  //第1组只调度1号梯
      EnUpLift[0][0][0]  = 0x0007ffff; //EnUpLift[0][0][1]  = 0x00000000;
      EnDnLift[0][0][0]  = 0x00080000; //EnDnLift[0][0][1]  = 0x00000000;
      EnUpGroup[0][0][0] = 0x0007ffff; //EnUpGroup[0][0][1] = 0x00000000;
      EnDnGroup[0][0][0] = 0x00080000; //EnDnGroup[0][0][1] = 0x00000000
	  //第2组只调度1/2/3号梯 
      EnUpLift[1][0][0]  = 0x00000000; //EnUpLift[1][0][1]  = 0x00000000;
      EnDnLift[1][0][0]  = 0x0007ffff; //EnDnLift[1][0][1]  = 0x00000000;
      EnUpGroup[1][0][0] = 0x0007ffff; //EnUpGroup[1][0][1] = 0x00000000;
      EnDnGroup[1][0][0] = 0x0007ffff; //EnDnGroup[1][0][1] = 0x00000000;

      EnUpLift[1][1][0]  = 0x0007ffff; //EnUpLift[1][1][1]  = 0x00000000;
      EnDnLift[1][1][0]  = 0x0007ffff; //EnDnLift[1][1][1]  = 0x00000000;
      EnUpGroup[1][1][0] = 0x0007ffff; //EnUpGroup[1][1][1] = 0x00000000;
      EnDnGroup[1][1][0] = 0x0007ffff; //EnDnGroup[1][1][1] = 0x00000000;

      EnUpLift[1][2][0]  = 0x0007ffff; //EnUpLift[1][2][1]  = 0x00000000;
      EnDnLift[1][2][0]  = 0x0007ffff; //EnDnLift[1][2][1]  = 0x00000000;
      EnUpGroup[1][2][0] = 0x0007ffff; //EnUpGroup[1][2][1] = 0x00000000;
      EnDnGroup[1][2][0] = 0x0007ffff; //EnDnGroup[1][2][1] = 0x00000000;
}
/*******************************************************************************/
	 
//_debug_group_floor13
void P14_Init(void)
{	  
	  SetAddr[0] = 3;
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 13; 
	  EnUpOrder[0][0] = 0x00000fff;
	  EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0x00001ffe;
	  EnDnOrder[0][1] = 0x0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2;  //双数层
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 12; 
	  EnUpOrder[1][0] = 0x00000aab;
	  EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = 0x00000aaa;
	  EnDnOrder[1][1] = 0x0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 1;  //单数层
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 13; 
	  EnUpOrder[2][0] = 0x00000555;
	  EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0x00001554;
	  EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 13;

	  WaitFloor[0][0] = 6;
	  WaitFloor[1][0] = 1;
	  WaitFloor[2][0] = 1;
	  WaitStartTime[0] = 0; 
	  WaitEndTime[0] = 0;
	  WaitFloorTime[0] = 120;
	  //第1组只调度1号梯
      EnUpLift[0][0][0]  = 0x00000fff; //EnUpLift[0][0][1]  = 0x00000000;
      EnDnLift[0][0][0]  = 0x00000000; //EnDnLift[0][0][1]  = 0x00000000;
      EnUpGroup[0][0][0] = 0x00000fff; //EnUpGroup[0][0][1] = 0x00000000;
      EnDnGroup[0][0][0] = 0x00000000; //EnDnGroup[0][0][1] = 0x00000000;
	  //第2组只调度1/2号梯	奇数
      EnUpLift[1][0][0]  = 0x00000000; //EnUpLift[1][0][1]  = 0x00000000;
      EnDnLift[1][0][0]  = 0x00000aaa; //EnDnLift[1][0][1]  = 0x00000000;
      EnUpGroup[1][0][0] = 0x00000aab; //EnUpGroup[1][0][1] = 0x00000000;
      EnDnGroup[1][0][0] = 0x00000aaa; //EnDnGroup[1][0][1] = 0x00000000;

      EnUpLift[1][1][0]  = 0x00000aab; //EnUpLift[1][1][1]  = 0x00000000;
      EnDnLift[1][1][0]  = 0x00000aaa; //EnDnLift[1][1][1]  = 0x00000000;
      EnUpGroup[1][1][0] = 0x00000aab; //EnUpGroup[1][1][1] = 0x00000000;
      EnDnGroup[1][1][0] = 0x00000aaa; //EnDnGroup[1][1][1] = 0x00000000;
	  //第3组只调度1/3号梯	双数
      EnUpLift[2][0][0]  = 0x00000000; //EnUpLift[2][0][1]  = 0x00000000;
      EnDnLift[2][0][0]  = 0x00001554; //EnDnLift[2][0][1]  = 0x00000000;
      EnUpGroup[2][0][0] = 0x00000555; //EnUpGroup[2][0][1] = 0x00000000;
      EnDnGroup[2][0][0] = 0x00001554; //EnDnGroup[2][0][1] = 0x00000000;

      EnUpLift[2][2][0]  = 0x00000555; //EnUpLift[2][2][1]  = 0x00000000;
      EnDnLift[2][2][0]  = 0x00001554; //EnDnLift[2][2][1]  = 0x00000000;
      EnUpGroup[2][2][0] = 0x00000555; //EnUpGroup[2][2][1] = 0x00000000;
      EnDnGroup[2][2][0] = 0x00001554; //EnDnGroup[2][2][1] = 0x00000000;
}
/*******************************************************************************/
	 
//邱红光调试 _debug_20_19_19
void P15_Init(void)
{	  
	  SetAddr[0] = 3;
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 20; 
	  EnUpOrder[0][0] = 0x000fffff;
	  //EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0x000fffff;
	  //EnDnOrder[0][1] = 0x0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2;
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 19; 
	  EnUpOrder[1][0] = 0x0003ffff;
	  //EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = 0x0007ffff;
	  //EnDnOrder[1][1] = 0x0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 1;
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 19; 
	  EnUpOrder[2][0] = 0x0003ffff;
	  //EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0x0007ffff;
	  //EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 20;

	  WaitFloor[0][0] = 13;
	  WaitFloor[1][0] = 1;
	  WaitFloor[2][0] = 1;
	  WaitStartTime[0] = 0; 
	  WaitEndTime[0] = 0;
	  WaitFloorTime[0] = 120;

	  //第1组只调度1号梯
      EnUpLift[0][0][0]  = 0x000c0001; //EnUpLift[0][0][1]  = 0x00000000;
      EnDnLift[0][0][0]  = 0x00080000; //EnDnLift[0][0][1]  = 0x00000000;//20楼下呼按钮
      EnUpGroup[0][0][0] = 0x000c0001; //EnUpGroup[0][0][1] = 0x00000000;
      EnDnGroup[0][0][0] = 0x00080000; //EnDnGroup[0][0][1] = 0x00000000;
	    EnUpLift[0][1][0]  = 0x000c0000; //19 20楼上呼按钮
      EnUpLift[0][2][0]  = 0x000c0000;
      
	  //第2组只调度1/2/3号梯 
      EnUpLift[1][0][0]  = 0x0003fffe; //EnUpLift[1][0][1]  = 0x00000000;//1-18楼
      EnDnLift[1][0][0]  = 0x0007ffff; //EnDnLift[1][0][1]  = 0x00000000;//1-19楼
      EnUpGroup[1][0][0] = 0x000fffff; //EnUpGroup[1][0][1] = 0x00000000;//1-20楼
      EnDnGroup[1][0][0] = 0x000fffff; //EnDnGroup[1][0][1] = 0x00000000;//1-20楼

      EnUpLift[1][1][0]  = 0x0003ffff; //EnUpLift[1][1][1]  = 0x00000000;//1-18楼
      EnDnLift[1][1][0]  = 0x0007ffff; //EnDnLift[1][1][1]  = 0x00000000;//1-19楼
      EnUpGroup[1][1][0] = 0x0003ffff; //EnUpGroup[1][1][1] = 0x00000000;//1-18楼
      EnDnGroup[1][1][0] = 0x0007ffff; //EnDnGroup[1][1][1] = 0x00000000;//1-19楼

      EnUpLift[1][2][0]  = 0x0003ffff; //EnUpLift[1][2][1]  = 0x00000000;//1-18楼
      EnDnLift[1][2][0]  = 0x0007ffff; //EnDnLift[1][2][1]  = 0x00000000;//1-19楼
      EnUpGroup[1][2][0] = 0x0003ffff; //EnUpGroup[1][2][1] = 0x00000000;//1-18楼
      EnDnGroup[1][2][0] = 0x0007ffff; //EnDnGroup[1][2][1] = 0x00000000;//1-19楼
}
/*******************************************************************************/

//邱红光调试 _debug_22_21_21
void P16_Init(void)
{
	  SetAddr[0] = 3;
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 22; 
	  EnUpOrder[0][0] = 0x003fffff;
	  //EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0x003fffff;
	  //EnDnOrder[0][1] = 0x0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2;
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 21; 
	  EnUpOrder[1][0] = 0x000fffff;
	  //EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = 0x001fffff;
	  //EnDnOrder[1][1] = 0x0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 1;
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 21; 
	  EnUpOrder[2][0] = 0x000fffff;
	  //EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0x001fffff;
	  //EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 22;

	  WaitFloor[0][0] = 13;
	  WaitFloor[1][0] = 1;
	  WaitFloor[2][0] = 1;
	  WaitStartTime[0] = 0; 
	  WaitEndTime[0] = 0;
	  WaitFloorTime[0] = 120;

	  //第1组只调度1号梯
      EnUpLift[0][0][0]  = 0x00300001; //EnUpLift[0][0][1]  = 0x00000000;
      EnDnLift[0][0][0]  = 0x00200000; //EnDnLift[0][0][1]  = 0x00000000;//22楼下呼按钮
      EnUpGroup[0][0][0] = 0x00300001; //EnUpGroup[0][0][1] = 0x00000000;
      EnDnGroup[0][0][0] = 0x00200000; //EnDnGroup[0][0][1] = 0x00000000;
	    EnUpLift[0][1][0]  = 0x00300000; //21 22楼上呼按钮
      EnUpLift[0][2][0]  = 0x00300000;
      
	  //第2组只调度1/2/3号梯 
      EnUpLift[1][0][0]  = 0x000ffffe; //EnUpLift[1][0][1]  = 0x00000000;//1-20楼
      EnDnLift[1][0][0]  = 0x001fffff; //EnDnLift[1][0][1]  = 0x00000000;//1-21楼
      EnUpGroup[1][0][0] = 0x003fffff; //EnUpGroup[1][0][1] = 0x00000000;//1-22楼
      EnDnGroup[1][0][0] = 0x003fffff; //EnDnGroup[1][0][1] = 0x00000000;//1-22楼

      EnUpLift[1][1][0]  = 0x000fffff; //EnUpLift[1][1][1]  = 0x00000000;//1-20楼
      EnDnLift[1][1][0]  = 0x001fffff; //EnDnLift[1][1][1]  = 0x00000000;//1-21楼
      EnUpGroup[1][1][0] = 0x000fffff; //EnUpGroup[1][1][1] = 0x00000000;//1-20楼
      EnDnGroup[1][1][0] = 0x001fffff; //EnDnGroup[1][1][1] = 0x00000000;//1-21楼

      EnUpLift[1][2][0]  = 0x000fffff; //EnUpLift[1][2][1]  = 0x00000000;//1-20楼
      EnDnLift[1][2][0]  = 0x001fffff; //EnDnLift[1][2][1]  = 0x00000000;//1-21楼
      EnUpGroup[1][2][0] = 0x000fffff; //EnUpGroup[1][2][1] = 0x00000000;//1-20楼
      EnDnGroup[1][2][0] = 0x001fffff; //EnDnGroup[1][2][1] = 0x00000000;//1-21楼
}
/*******************************************************************************/
//台湾8台 1、9->23层				 
void P17_Init(void)
{
	 DWORD i;
	 DWORD mask[2];
	
	 for (i=0; i<8; i++) {
		   SetAddr[i] = i+1;
		   FirstFloor[i] = 1;
       MaxFloor[i] = 23;
		   MaskProcess(mask, FirstFloor[i], MaxFloor[i]);
       mask[0] &= (~(0x7ful<<1));		 
		   EnUpOrder[i][0] = mask[0];	   
	     EnDnOrder[i][0] = mask[0];
	     EnUpLift[0][i][0]  = mask[0]; 
       EnDnLift[0][i][0]  = mask[0];
       EnUpGroup[0][i][0] = mask[0];
       EnDnGroup[0][i][0] = mask[0];
	 }
	 GroupFirstFloor = 1;
	 GroupMaxFloor = 23;
   
	 //高峰设置
	 WaitStartTime[0] = (8<<16)|(0<<8)|0; 
	 WaitEndTime[0]   = (9<<16)|(0<<8)|0;
	 WaitFloorTime[0] = 1;
	 WaitFloor[0][0] = 1;//等待楼层
	 WaitFloor[1][0] = 1;
	 WaitFloor[2][0] = 1;
	 WaitFloor[3][0] = 1;
	 WaitFloor[4][0] = 1;
	 WaitFloor[5][0] = 1;
	 WaitFloor[6][0] = 1;
	 WaitFloor[7][0] = 1;
	 
	 WaitStartTime[1] = (11<<16)|(30<<8)|0; 
	 WaitEndTime[1]   = (12<<16)|(30<<8)|0;
	 WaitFloorTime[1] = 3;
	 WaitFloor[0][1] = 9;
	 WaitFloor[1][1] = 9;
	 WaitFloor[2][1] = 14;
	 WaitFloor[3][1] = 14;
	 WaitFloor[4][1] = 19;
	 WaitFloor[5][1] = 19;
	 WaitFloor[6][1] = 23;
	 WaitFloor[7][1] = 23;

	 WaitStartTime[2] = (12<<16)|(30<<8)|0; 
	 WaitEndTime[2]   = (13<<16)|(30<<8)|0;
	 WaitFloorTime[2] = 1;
	 WaitFloor[0][2] = 1;
	 WaitFloor[1][2] = 1;
	 WaitFloor[2][2] = 1;
	 WaitFloor[3][2] = 1;
	 WaitFloor[4][2] = 1;
	 WaitFloor[5][2] = 1;
	 WaitFloor[6][2] = 1;
	 WaitFloor[7][2] = 1;

	 WaitStartTime[3] = (17<<16)|(00<<8)|0; 
	 WaitEndTime[3]   = (18<<16)|(00<<8)|0;
	 WaitFloorTime[3] = 3;
	 WaitFloor[0][3] = 9;
	 WaitFloor[1][3] = 9;
	 WaitFloor[2][3] = 14;
	 WaitFloor[3][3] = 14;
	 WaitFloor[4][3] = 19;
	 WaitFloor[5][3] = 19;
	 WaitFloor[6][3] = 23;
	 WaitFloor[7][3] = 23;

   //分散待梯设置
   Mode2_WaitFloorTime = 60;
	 WaitFloor8[0] = 1;
	 WaitFloor8[1] = 1;
	 WaitFloor8[2] = 9;
	 WaitFloor8[3] = 9;
	 WaitFloor8[4] = 16;
	 WaitFloor8[5] = 16;
	 WaitFloor8[6] = 23;
	 WaitFloor8[7] = 23;

	 WaitFloor7[0] = 1;
	 WaitFloor7[1] = 1;
	 WaitFloor7[2] = 9;
	 WaitFloor7[3] = 9;
	 WaitFloor7[4] = 16;
	 WaitFloor7[5] = 16;
	 WaitFloor7[6] = 23;

	 WaitFloor6[0] = 1;
	 WaitFloor6[1] = 1;
	 WaitFloor6[2] = 9;
	 WaitFloor6[3] = 9;
	 WaitFloor6[4] = 16;
	 WaitFloor6[5] = 16;

	 WaitFloor5[0] = 1;
	 WaitFloor5[1] = 1;
	 WaitFloor5[2] = 9;
	 WaitFloor5[3] = 9;
	 WaitFloor5[4] = 16;

	 WaitFloor4[0] = 1;
	 WaitFloor4[1] = 1;
	 WaitFloor4[2] = 9;
	 WaitFloor4[3] = 16;

	 WaitFloor3[0] = 1;
	 WaitFloor3[1] = 1;
	 WaitFloor3[2] = 9;
	 
	 WaitFloor2[0] = 1;
	 WaitFloor2[1] = 9;
}

/*******************************************************************************/
//台湾8台 1、9->25层				 
void P18_Init(void)
{
	 DWORD i;
	 DWORD mask[2];
	
	 for (i=0; i<8; i++) {
		   SetAddr[i] = i+1;
		   FirstFloor[i] = 1;
       MaxFloor[i] = 25;
		   MaskProcess(mask, FirstFloor[i], MaxFloor[i]);
       mask[0] &= (~(0x7ful<<1));		 
		   EnUpOrder[i][0] = mask[0];	   
	     EnDnOrder[i][0] = mask[0];
	     EnUpLift[0][i][0]  = mask[0]; 
       EnDnLift[0][i][0]  = mask[0];
       EnUpGroup[0][i][0] = mask[0];
       EnDnGroup[0][i][0] = mask[0];
	 }
	 GroupFirstFloor = 1;
	 GroupMaxFloor = 25;
   
	 //高峰设置
	 WaitStartTime[0] = (8<<16)|(0<<8)|0; 
	 WaitEndTime[0]   = (9<<16)|(0<<8)|0;
	 WaitFloorTime[0] = 1;
	 WaitFloor[0][0] = 1;//等待楼层
	 WaitFloor[1][0] = 1;
	 WaitFloor[2][0] = 1;
	 WaitFloor[3][0] = 1;
	 WaitFloor[4][0] = 1;
	 WaitFloor[5][0] = 1;
	 WaitFloor[6][0] = 1;
	 WaitFloor[7][0] = 1;
	 
	 WaitStartTime[1] = (11<<16)|(30<<8)|0; 
	 WaitEndTime[1]   = (12<<16)|(30<<8)|0;
	 WaitFloorTime[1] = 3;
	 WaitFloor[0][1] = 9;
	 WaitFloor[1][1] = 9;
	 WaitFloor[2][1] = 15;
	 WaitFloor[3][1] = 15;
	 WaitFloor[4][1] = 20;
	 WaitFloor[5][1] = 20;
	 WaitFloor[6][1] = 25;
	 WaitFloor[7][1] = 25;

	 WaitStartTime[2] = (12<<16)|(30<<8)|0; 
	 WaitEndTime[2]   = (13<<16)|(30<<8)|0;
	 WaitFloorTime[2] = 1;
	 WaitFloor[0][2] = 1;
	 WaitFloor[1][2] = 1;
	 WaitFloor[2][2] = 1;
	 WaitFloor[3][2] = 1;
	 WaitFloor[4][2] = 1;
	 WaitFloor[5][2] = 1;
	 WaitFloor[6][2] = 1;
	 WaitFloor[7][2] = 1;

	 WaitStartTime[3] = (17<<16)|(00<<8)|0; 
	 WaitEndTime[3]   = (18<<16)|(00<<8)|0;
	 WaitFloorTime[3] = 3;
	 WaitFloor[0][3] = 9;
	 WaitFloor[1][3] = 9;
	 WaitFloor[2][3] = 15;
	 WaitFloor[3][3] = 15;
	 WaitFloor[4][3] = 20;
	 WaitFloor[5][3] = 20;
	 WaitFloor[6][3] = 25;
	 WaitFloor[7][3] = 25;

   //分散待梯设置
   Mode2_WaitFloorTime = 60;
	 WaitFloor8[0] = 1;
	 WaitFloor8[1] = 1;
	 WaitFloor8[2] = 9;
	 WaitFloor8[3] = 9;
	 WaitFloor8[4] = 15;
	 WaitFloor8[5] = 15;
	 WaitFloor8[6] = 25;
	 WaitFloor8[7] = 25;

	 WaitFloor7[0] = 1;
	 WaitFloor7[1] = 1;
	 WaitFloor7[2] = 9;
	 WaitFloor7[3] = 9;
	 WaitFloor7[4] = 15;
	 WaitFloor7[5] = 15;
	 WaitFloor7[6] = 25;

	 WaitFloor6[0] = 1;
	 WaitFloor6[1] = 1;
	 WaitFloor6[2] = 9;
	 WaitFloor6[3] = 9;
	 WaitFloor6[4] = 15;
	 WaitFloor6[5] = 15;

	 WaitFloor5[0] = 1;
	 WaitFloor5[1] = 1;
	 WaitFloor5[2] = 9;
	 WaitFloor5[3] = 9;
	 WaitFloor5[4] = 16;

	 WaitFloor4[0] = 1;
	 WaitFloor4[1] = 1;
	 WaitFloor4[2] = 9;
	 WaitFloor4[3] = 16;

	 WaitFloor3[0] = 1;
	 WaitFloor3[1] = 1;
	 WaitFloor3[2] = 9;
	 
	 WaitFloor2[0] = 1;
	 WaitFloor2[1] = 9;
}
/*******************************************************************************/
//台湾8台 18层				 
void P19_Init(void)
{
	 DWORD i;
	 DWORD mask[2];
	
	 for (i=0; i<8; i++) {
		   SetAddr[i] = i+1;
		   FirstFloor[i] = 1;
       MaxFloor[i] = 18;
       MaskProcess(mask, FirstFloor[i], MaxFloor[i]);
       EnUpOrder[i][0] = mask[0];	   
	     EnDnOrder[i][0] = mask[0];
	     EnUpLift[0][i][0]  = mask[0]; 
       EnDnLift[0][i][0]  = mask[0];
       EnUpGroup[0][i][0] = mask[0];
       EnDnGroup[0][i][0] = mask[0];
	 }
	 GroupFirstFloor = 1;
	 GroupMaxFloor = 18;
   
	 //高峰设置
	 WaitStartTime[0] = (8<<16)|(0<<8)|0; 
	 WaitEndTime[0]   = (9<<16)|(0<<8)|0;
	 WaitFloorTime[0] = 1;
	 WaitFloor[0][0] = 1;//等待楼层
	 WaitFloor[1][0] = 1;
	 WaitFloor[2][0] = 1;
	 WaitFloor[3][0] = 1;
	 WaitFloor[4][0] = 1;
	 WaitFloor[5][0] = 1;
	 WaitFloor[6][0] = 1;
	 WaitFloor[7][0] = 1;
	 
	 WaitStartTime[1] = (11<<16)|(30<<8)|0; 
	 WaitEndTime[1]   = (12<<16)|(30<<8)|0;
	 WaitFloorTime[1] = 3;
	 WaitFloor[0][1] = 5;
	 WaitFloor[1][1] = 5;
	 WaitFloor[2][1] = 10;
	 WaitFloor[3][1] = 10;
	 WaitFloor[4][1] = 15;
	 WaitFloor[5][1] = 15;
	 WaitFloor[6][1] = 18;
	 WaitFloor[7][1] = 18;

	 WaitStartTime[2] = (12<<16)|(30<<8)|0; 
	 WaitEndTime[2]   = (13<<16)|(30<<8)|0;
	 WaitFloorTime[2] = 1;
	 WaitFloor[0][2] = 1;
	 WaitFloor[1][2] = 1;
	 WaitFloor[2][2] = 1;
	 WaitFloor[3][2] = 1;
	 WaitFloor[4][2] = 1;
	 WaitFloor[5][2] = 1;
	 WaitFloor[6][2] = 1;
	 WaitFloor[7][2] = 1;

	 WaitStartTime[3] = (17<<16)|(00<<8)|0; 
	 WaitEndTime[3]   = (18<<16)|(00<<8)|0;
	 WaitFloorTime[3] = 3;
	 WaitFloor[0][3] = 5;
	 WaitFloor[1][3] = 5;
	 WaitFloor[2][3] = 10;
	 WaitFloor[3][3] = 10;
	 WaitFloor[4][3] = 15;
	 WaitFloor[5][3] = 15;
	 WaitFloor[6][3] = 18;
	 WaitFloor[7][3] = 18;

   //分散待梯设置
   Mode2_WaitFloorTime = 60;
	 WaitFloor8[0] = 1;
	 WaitFloor8[1] = 1;
	 WaitFloor8[2] = 5;
	 WaitFloor8[3] = 5;
	 WaitFloor8[4] = 12;
	 WaitFloor8[5] = 12;
	 WaitFloor8[6] = 18;
	 WaitFloor8[7] = 18;

	 WaitFloor7[0] = 1;
	 WaitFloor7[1] = 1;
	 WaitFloor7[2] = 5;
	 WaitFloor7[3] = 5;
	 WaitFloor7[4] = 12;
	 WaitFloor7[5] = 12;
	 WaitFloor7[6] = 18;

	 WaitFloor6[0] = 1;
	 WaitFloor6[1] = 1;
	 WaitFloor6[2] = 5;
	 WaitFloor6[3] = 5;
	 WaitFloor6[4] = 12;
	 WaitFloor6[5] = 12;

	 WaitFloor5[0] = 1;
	 WaitFloor5[1] = 1;
	 WaitFloor5[2] = 5;
	 WaitFloor5[3] = 5;
	 WaitFloor5[4] = 12;

	 WaitFloor4[0] = 1;
	 WaitFloor4[1] = 1;
	 WaitFloor4[2] = 5;
	 WaitFloor4[3] = 5;

	 WaitFloor3[0] = 1;
	 WaitFloor3[1] = 1;
	 WaitFloor3[2] = 5;
	 
	 WaitFloor2[0] = 1;
	 WaitFloor2[1] = 5;
}
/*******************************************************************************/
//台湾6台 10层				 
void P20_Init(void)
{
	 DWORD i;
	 DWORD mask[2];
	
	 for (i=0; i<6; i++) {
		   SetAddr[i] = i+1;
		   FirstFloor[i] = 1;
       MaxFloor[i] = 10;
       MaskProcess(mask, FirstFloor[i], MaxFloor[i]);
       EnUpOrder[i][0] = mask[0];	   
	     EnDnOrder[i][0] = mask[0];
	     EnUpLift[0][i][0]  = mask[0]; 
       EnDnLift[0][i][0]  = mask[0];
       EnUpGroup[0][i][0] = mask[0];
       EnDnGroup[0][i][0] = mask[0];
	 }
	 GroupFirstFloor = 1;
	 GroupMaxFloor = 10;
   
	 //高峰设置
	 WaitStartTime[0] = (8<<16)|(0<<8)|0; 
	 WaitEndTime[0]   = (9<<16)|(0<<8)|0;
	 WaitFloorTime[0] = 1;
	 WaitFloor[0][0] = 1;//等待楼层
	 WaitFloor[1][0] = 1;
	 WaitFloor[2][0] = 1;
	 WaitFloor[3][0] = 1;
	 WaitFloor[4][0] = 1;
	 WaitFloor[5][0] = 1;
	 
	 WaitStartTime[1] = (11<<16)|(30<<8)|0; 
	 WaitEndTime[1]   = (12<<16)|(30<<8)|0;
	 WaitFloorTime[1] = 3;
	 WaitFloor[0][1] = 4;
	 WaitFloor[1][1] = 4;
	 WaitFloor[2][1] = 6;
	 WaitFloor[3][1] = 6;
	 WaitFloor[4][1] = 8;
	 WaitFloor[5][1] = 8;

	 WaitStartTime[2] = (12<<16)|(30<<8)|0; 
	 WaitEndTime[2]   = (13<<16)|(30<<8)|0;
	 WaitFloorTime[2] = 1;
	 WaitFloor[0][2] = 1;
	 WaitFloor[1][2] = 1;
	 WaitFloor[2][2] = 1;
	 WaitFloor[3][2] = 1;
	 WaitFloor[4][2] = 1;
	 WaitFloor[5][2] = 1;

	 WaitStartTime[3] = (17<<16)|(00<<8)|0; 
	 WaitEndTime[3]   = (18<<16)|(00<<8)|0;
	 WaitFloorTime[3] = 3;
	 WaitFloor[0][3] = 4;
	 WaitFloor[1][3] = 4;
	 WaitFloor[2][3] = 6;
	 WaitFloor[3][3] = 6;
	 WaitFloor[4][3] = 8;
	 WaitFloor[5][3] = 8;

   //分散待梯设置
   Mode2_WaitFloorTime = 60;
	 WaitFloor8[0] = 1;
	 WaitFloor8[1] = 1;
	 WaitFloor8[2] = 6;
	 WaitFloor8[3] = 6;
	 WaitFloor8[4] = 8;
	 WaitFloor8[5] = 8;

	 WaitFloor7[0] = 1;
	 WaitFloor7[1] = 1;
	 WaitFloor7[2] = 6;
	 WaitFloor7[3] = 6;
	 WaitFloor7[4] = 8;
	 WaitFloor7[5] = 8;

	 WaitFloor6[0] = 1;
	 WaitFloor6[1] = 1;
	 WaitFloor6[2] = 6;
	 WaitFloor6[3] = 6;
	 WaitFloor6[4] = 8;
	 WaitFloor6[5] = 8;

	 WaitFloor5[0] = 1;
	 WaitFloor5[1] = 1;
	 WaitFloor5[2] = 6;
	 WaitFloor5[3] = 6;
	 WaitFloor5[4] = 8;

	 WaitFloor4[0] = 1;
	 WaitFloor4[1] = 1;
	 WaitFloor4[2] = 6;
	 WaitFloor4[3] = 6;

	 WaitFloor3[0] = 1;
	 WaitFloor3[1] = 1;
	 WaitFloor3[2] = 6;
	 
	 WaitFloor2[0] = 1;
	 WaitFloor2[1] = 6;
}

/*******************************************************************************/
//G 1 2 ->36

/*
void P21_Init(void)
{
    SetAddr[3] = 0;
    SetAddr[4] = 0;
    SetAddr[5] = 0;
    SetAddr[6] = 0;
    SetAddr[7] = 0;
	
	  SetAddr[2] = 1;
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 36; 
	  EnUpOrder[2][0] = 0xffffffff; EnUpOrder[2][1] = 0x0000000f;
	  EnDnOrder[2][0] = 0xffffffff; EnDnOrder[2][1] = 0x0000000f;
	  NewFlag[2] = 1;

	  SetAddr[1] = 2;
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 35; 
	  EnUpOrder[1][0] = 0x55555557;	  EnUpOrder[1][1] = 0x00000005;
	  EnDnOrder[1][0] = 0x55555557;	  EnDnOrder[1][1] = 0x00000005;
	  NewFlag[1] = 1;

	  SetAddr[0] = 3;
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 36; 
	  EnUpOrder[0][0] = 0xaaaaaaaf;	  EnUpOrder[0][1] = 0x0000000a;
	  EnDnOrder[0][0] = 0xaaaaaaaf;	  EnDnOrder[0][1] = 0x0000000a;
	  NewFlag[0] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 36;

	  WaitFloor3[0] = 1;
	  WaitFloor3[1] = 1;
	  WaitFloor3[2] = 13;
	  //第1组只调度1号梯
      EnUpLift[0][2][0]  = 0xffffffff;     EnUpLift[0][2][1]  = 0x0000000f; //层层停
      EnDnLift[0][2][0]  = 0x00000000;     EnDnLift[0][2][1]  = 0x00000000;
      EnUpGroup[0][2][0] = 0xffffffff;     EnUpGroup[0][2][1] = 0x0000000f;
      EnDnGroup[0][2][0] = 0x00000000;     EnDnGroup[0][2][1] = 0x00000000;
	  //第2组只调度1/2号梯	奇数
      EnUpLift[1][2][0]  = 0x00000000;     EnUpLift[1][2][1]  = 0x00000000; //层层停
      EnDnLift[1][2][0]  = 0x00000000;     EnDnLift[1][2][1]  = 0x00000000;
      EnUpGroup[1][2][0] = 0x55555555|0x7; EnUpGroup[1][2][1] = 0x00000005;
      EnDnGroup[1][2][0] = 0x00000000;     EnDnGroup[1][2][1] = 0x00000000;

      EnUpLift[1][1][0]  = 0x55555555|0x7; EnUpLift[1][1][1]  = 0x00000005; //单层停
      EnDnLift[1][1][0]  = 0x00000000;     EnDnLift[1][1][1]  = 0x00000000;
      EnUpGroup[1][1][0] = 0x55555555|0x7; EnUpGroup[1][1][1] = 0x00000005;
      EnDnGroup[1][1][0] = 0x00000000;     EnDnGroup[1][1][1] = 0x00000000;
	  //第3组只调度1/3号梯	双数
      EnUpLift[2][2][0]  = 0x00000000;     EnUpLift[2][2][1]  = 0x00000000; //层层停
      EnDnLift[2][2][0]  = 0x00000000;     EnDnLift[2][2][1]  = 0x00000000;
      EnUpGroup[2][2][0] = 0xaaaaaaaa|0x7; EnUpGroup[2][2][1] = 0x0000000a;
      EnDnGroup[2][2][0] = 0x00000000;     EnDnGroup[2][2][1] = 0x00000000;

      EnUpLift[2][0][0]  = 0xaaaaaaaa|0x7; EnUpLift[2][0][1]  = 0x0000000a; //双层停
      EnDnLift[2][0][0]  = 0x00000000;     EnDnLift[2][0][1]  = 0x00000000;
      EnUpGroup[2][0][0] = 0xaaaaaaaa|0x7; EnUpGroup[2][0][1] = 0x0000000a;
      EnDnGroup[2][0][0] = 0x00000000;     EnDnGroup[2][0][1] = 0x00000000;
	  //第4组只调度1/2/3号梯
      EnUpLift[3][0][0]  = 0x00000000;     EnUpLift[3][0][1]  = 0x00000000;
      EnDnLift[3][0][0]  = 0xffffffff;     EnDnLift[3][0][1]  = 0x0000000f;
      EnUpGroup[3][0][0] = 0x00000000;     EnUpGroup[3][0][1] = 0x00000000;
      EnDnGroup[3][0][0] = 0xffffffff;     EnDnGroup[3][0][1] = 0x0000000f;

      EnUpLift[3][1][0]  = 0x00000000;     EnUpLift[3][1][1]  = 0x00000000;
      EnDnLift[3][1][0]  = 0xffffffff;     EnDnLift[3][1][1]  = 0x0000000f;
      EnUpGroup[3][1][0] = 0x00000000;     EnUpGroup[3][1][1] = 0x00000000;
      EnDnGroup[3][1][0] = 0xffffffff;     EnDnGroup[3][1][1] = 0x0000000f;

      EnUpLift[3][2][0]  = 0x00000000;     EnUpLift[3][2][1]  = 0x00000000; //层层停
      EnDnLift[3][2][0]  = 0xffffffff;     EnDnLift[3][2][1]  = 0x0000000f;
      EnUpGroup[3][2][0] = 0x00000000;     EnUpGroup[3][2][1] = 0x00000000;
      EnDnGroup[3][2][0] = 0xffffffff;     EnDnGroup[3][2][1] = 0x0000000f;
}
*/

/*******************************************************************************/
//0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35   层层停
//0 1 2   4   6   8   10    12    14    16    18    20    22    24    26    28    30    32    34      单
//0 1 2 3   5   7   9    11    13    15    17    19    21    23    25    27    29    31    33    35   双

void P21_Init(void)
{	  
	  SetAddr[0] = 3; //层层停
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 36; 
	  EnUpOrder[0][0] = 0xffffffff;
	  EnUpOrder[0][1] = 0xf;
	  EnDnOrder[0][0] = 0xffffffff;
	  EnDnOrder[0][1] = 0xf;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2; //单层停
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 35; 
	  EnUpOrder[1][0] = 0x55555557;
	  EnUpOrder[1][1] = 0x5;
	  EnDnOrder[1][0] = 0x55555557;
	  EnDnOrder[1][1] = 0x5;
	  NewFlag[1] = 1;

	  SetAddr[2] = 1; //双层停
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 36; 
	  EnUpOrder[2][0] = 0xaaaaaaaf;
	  EnUpOrder[2][1] = 0xa;
	  EnDnOrder[2][0] = 0xaaaaaaaf;
	  EnDnOrder[2][1] = 0xa;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 36;

    WaitFloor3[0] = 20;
	  WaitFloor3[1] = 1;
	  WaitFloor3[2] = 1;

    //第1组只调度1号梯
    EnUpLift[0][0][0]  = 0xfffffff8;  EnUpLift[0][0][1]  = 0xf;
    EnDnLift[0][0][0]  = 0xffffffff;  EnDnLift[0][0][1]  = 0xf;
    EnUpGroup[0][0][0] = 0xfffffff8;  EnUpGroup[0][0][1] = 0xf; 
    EnDnGroup[0][0][0] = 0xffffffff;  EnDnGroup[0][0][1] = 0xf; 

    EnUpLift[0][1][0]  = 0x55555550;  EnUpLift[0][1][1]  = 0x5;
    EnDnLift[0][1][0]  = 0x55555557;  EnDnLift[0][1][1]  = 0x5; 
    EnUpGroup[0][1][0] = 0x55555550;  EnUpGroup[0][1][1] = 0x5; 
    EnDnGroup[0][1][0] = 0x55555557;  EnDnGroup[0][1][1] = 0x5;

    EnUpLift[0][2][0]  = 0xaaaaaaa8;  EnUpLift[0][2][1]  = 0xa;
    EnDnLift[0][2][0]  = 0xaaaaaaaf;  EnDnLift[0][2][1]  = 0xa;
    EnUpGroup[0][2][0] = 0xaaaaaaa8;  EnUpGroup[0][2][1] = 0xa; 
    EnDnGroup[0][2][0] = 0xaaaaaaaf;  EnDnGroup[0][2][1] = 0xa;
		
	  //第2组只调度1/2号梯	奇数
    EnUpLift[1][0][0]  = 0x00000000;  EnUpLift[1][0][1]  = 0x0;
    EnDnLift[1][0][0]  = 0x00000000;  EnDnLift[1][0][1]  = 0x0;
    EnUpGroup[1][0][0] = 0x00000007;  EnUpGroup[1][0][1] = 0x0;
    EnDnGroup[1][0][0] = 0x00000000;  EnDnGroup[1][0][1] = 0x0;

    EnUpLift[1][1][0]  = 0x00000007;  EnUpLift[1][1][1]  = 0x0;
    EnDnLift[1][1][0]  = 0x00000000;  EnDnLift[1][1][1]  = 0x0; 
    EnUpGroup[1][1][0] = 0x00000007;  EnUpGroup[1][1][1] = 0x0; 
    EnDnGroup[1][1][0] = 0x00000000;  EnDnGroup[1][1][1] = 0x0; 

    //第3组只调度1/3号梯	双数
    EnUpLift[2][0][0]  = 0x00000000;  EnUpLift[2][0][1]  = 0x0;
    EnDnLift[2][0][0]  = 0x00000000;  EnDnLift[2][0][1]  = 0x0;
    EnUpGroup[2][0][0] = 0x00000007;  EnUpGroup[2][0][1] = 0x0;
    EnDnGroup[2][0][0] = 0x00000000;  EnDnGroup[2][0][1] = 0x0;

    EnUpLift[2][2][0]  = 0x00000007;  EnUpLift[2][2][1]  = 0x0;
    EnDnLift[2][2][0]  = 0x00000000;  EnDnLift[2][2][1]  = 0x0;
    EnUpGroup[2][2][0] = 0x00000007;  EnUpGroup[2][2][1] = 0x0;
    EnDnGroup[2][2][0] = 0x00000000;  EnDnGroup[2][2][1] = 0x0;
		
    //第4组只调度1号梯	
    EnUpLift[3][0][0]  = 0x00000007;  EnUpLift[3][0][1]  = 0x0;
    EnDnLift[3][0][0]  = 0x00000000;  EnDnLift[3][0][1]  = 0x0;  
    EnUpGroup[3][0][0] = 0x00000007;  EnUpGroup[3][0][1] = 0x0; 
    EnDnGroup[3][0][0] = 0x00000000;  EnDnGroup[3][0][1] = 0x0; 
}
/*******************************************************************************/
/*******************************************************************************/
//G 1 2 3->21, G 1 2 3 5 7->21, G 1 2 4 6->20
void P22_Init(void)
{
    SetAddr[3] = 0;
    SetAddr[4] = 0;
    SetAddr[5] = 0;
    SetAddr[6] = 0;
    SetAddr[7] = 0;
	
	  SetAddr[2] = 1;
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 22; 
	  EnUpOrder[2][0] = 0x003fffff;
	  EnDnOrder[2][0] = 0x003fffff;
	  NewFlag[2] = 1;

	  SetAddr[1] = 3;
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 21; 
	  EnUpOrder[1][0] = 0x00155557;
	  EnDnOrder[1][0] = 0x00155557;
	  NewFlag[1] = 1;

	  SetAddr[0] = 2;
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 22; 
	  EnUpOrder[0][0] = 0x002aaaaf;
	  EnDnOrder[0][0] = 0x002aaaaf;
	  NewFlag[0] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 22;

	  WaitStartTime[0] = (7<<16)|(0<<8)|0; 
	  WaitEndTime[0]   = (9<<16)|(0<<8)|0;
	  WaitFloorTime[0] = 2;
		
		WaitFloor[0][0] = 22;
		WaitFloor[1][0] = 21;
		WaitFloor[2][0] = 22;

	  WaitStartTime[0] = (9<<16)|(0<<8)|0; 
	  WaitEndTime[0]   = (8<<16)|(0<<8)|0;
	  WaitFloorTime[0] = 120;
		WaitFloor[0][1] = 1;
		WaitFloor[1][1] = 1;
		WaitFloor[2][1] = 19;

    WaitFloor3[0] = 1;
	  WaitFloor3[1] = 1;
	  WaitFloor3[2] = 13;
		
	  //第1组只调度1号梯
      EnUpLift[0][2][0]  = 0xffffffff;     EnUpLift[0][2][1]  = 0x00000000; //层层停
      EnDnLift[0][2][0]  = 0x00000000;     EnDnLift[0][2][1]  = 0x00000000;
      EnUpGroup[0][2][0] = 0xffffffff;     EnUpGroup[0][2][1] = 0x00000000;
      EnDnGroup[0][2][0] = 0x00000000;     EnDnGroup[0][2][1] = 0x00000000;
	  //第2组只调度1/2号梯	奇数
      EnUpLift[1][2][0]  = 0x00000000;     EnUpLift[1][2][1]  = 0x00000000; //层层停
      EnDnLift[1][2][0]  = 0x00000000;     EnDnLift[1][2][1]  = 0x00000000;
      EnUpGroup[1][2][0] = 0x55555555|0x7; EnUpGroup[1][2][1] = 0x00000000;
      EnDnGroup[1][2][0] = 0x00000000;     EnDnGroup[1][2][1] = 0x00000000;

      EnUpLift[1][1][0]  = 0x55555555|0x7; EnUpLift[1][1][1]  = 0x00000000; //单层停
      EnDnLift[1][1][0]  = 0x00000000;     EnDnLift[1][1][1]  = 0x00000000;
      EnUpGroup[1][1][0] = 0x55555555|0x7; EnUpGroup[1][1][1] = 0x00000000;
      EnDnGroup[1][1][0] = 0x00000000;     EnDnGroup[1][1][1] = 0x00000000;
	  //第3组只调度1/3号梯	双数
      EnUpLift[2][2][0]  = 0x00000000;     EnUpLift[2][2][1]  = 0x00000000; //层层停
      EnDnLift[2][2][0]  = 0x00000000;     EnDnLift[2][2][1]  = 0x00000000;
      EnUpGroup[2][2][0] = 0xaaaaaaaa|0x7; EnUpGroup[2][2][1] = 0x00000000;
      EnDnGroup[2][2][0] = 0x00000000;     EnDnGroup[2][2][1] = 0x00000000;

      EnUpLift[2][0][0]  = 0xaaaaaaaa|0x7; EnUpLift[2][0][1]  = 0x00000000; //双层停
      EnDnLift[2][0][0]  = 0x00000000;     EnDnLift[2][0][1]  = 0x00000000;
      EnUpGroup[2][0][0] = 0xaaaaaaaa|0x7; EnUpGroup[2][0][1] = 0x00000000;
      EnDnGroup[2][0][0] = 0x00000000;     EnDnGroup[2][0][1] = 0x00000000;
	  //第4组只调度1/2/3号梯
      EnUpLift[3][0][0]  = 0x00000000;     EnUpLift[3][0][1]  = 0x00000000;
      EnDnLift[3][0][0]  = 0xffffffff;     EnDnLift[3][0][1]  = 0x00000000;
      EnUpGroup[3][0][0] = 0x00000000;     EnUpGroup[3][0][1] = 0x00000000;
      EnDnGroup[3][0][0] = 0xffffffff;     EnDnGroup[3][0][1] = 0x00000000;

      EnUpLift[3][1][0]  = 0x00000000;     EnUpLift[3][1][1]  = 0x00000000;
      EnDnLift[3][1][0]  = 0xffffffff;     EnDnLift[3][1][1]  = 0x00000000;
      EnUpGroup[3][1][0] = 0x00000000;     EnUpGroup[3][1][1] = 0x00000000;
      EnDnGroup[3][1][0] = 0xffffffff;     EnDnGroup[3][1][1] = 0x00000000;

      EnUpLift[3][2][0]  = 0x00000000;     EnUpLift[3][2][1]  = 0x00000000; //层层停
      EnDnLift[3][2][0]  = 0xffffffff;     EnDnLift[3][2][1]  = 0x00000000;
      EnUpGroup[3][2][0] = 0x00000000;     EnUpGroup[3][2][1] = 0x00000000;
      EnDnGroup[3][2][0] = 0xffffffff;     EnDnGroup[3][2][1] = 0x00000000;
}
/*******************************************************************************/
//G 1 2 ->21
void P23_Init(void)
{
    SetAddr[3] = 0;
    SetAddr[4] = 0;
    SetAddr[5] = 0;
    SetAddr[6] = 0;
    SetAddr[7] = 0;
	
	  SetAddr[2] = 1;
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 22; 
	  EnUpOrder[2][0] = 0x003fffff;
	  EnDnOrder[2][0] = 0x003fffff;
	  NewFlag[2] = 1;

	  SetAddr[1] = 3;
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 21; 
	  EnUpOrder[1][0] = 0x00155555|0x03;
	  EnDnOrder[1][0] = 0x00155555|0x03;
	  NewFlag[1] = 1;

	  SetAddr[0] = 2;
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 22; 
	  EnUpOrder[0][0] = 0x002aaaaa|0x03;
	  EnDnOrder[0][0] = 0x002aaaaa|0x03;
	  NewFlag[0] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 22;

	  WaitFloor3[0] = 12;
	  WaitFloor3[1] = 2;
	  WaitFloor3[2] = 2;
	  //第1组只调度1号梯
      EnUpLift[0][2][0]  = 0x00000003;        //EnUpLift[0][2][1]  = 0x00000000; //层层停
      EnDnLift[0][2][0]  = 0x00000000;        //EnDnLift[0][2][1]  = 0x00000000;
      EnUpGroup[0][2][0] = 0x00000003;        //EnUpGroup[0][2][1] = 0x00000000;
      EnDnGroup[0][2][0] = 0x00000000;        //EnDnGroup[0][2][1] = 0x00000000;
	  //第2组只调度1/2号梯	奇数
      EnUpLift[1][2][0]  = 0x55555555&(~0x3); //EnUpLift[1][2][1]  = 0x00000000; //层层停
      EnDnLift[1][2][0]  = 0x55555555&(~0x3); //EnDnLift[1][2][1]  = 0x00000000;
      EnUpGroup[1][2][0] = 0x55555555|0x3;    //EnUpGroup[1][2][1] = 0x00000000;
      EnDnGroup[1][2][0] = 0x55555555&(~0x3); //EnDnGroup[1][2][1] = 0x00000000;

      EnUpLift[1][1][0]  = 0x55555555|0x3;    //EnUpLift[1][1][1]  = 0x00000000; //单层停
      EnDnLift[1][1][0]  = 0x55555555&(~0x3); //EnDnLift[1][1][1]  = 0x00000000;
      EnUpGroup[1][1][0] = 0x55555555|0x3;    //EnUpGroup[1][1][1] = 0x00000000;
      EnDnGroup[1][1][0] = 0x55555555&(~0x3); //EnDnGroup[1][1][1] = 0x00000000;
	  //第3组只调度1/3号梯	双数
      EnUpLift[2][2][0]  = 0xaaaaaaaa&(~0x3); //EnUpLift[2][2][1]  = 0x00000000; //层层停
      EnDnLift[2][2][0]  = 0xaaaaaaaa&(~0x3); //EnDnLift[2][2][1]  = 0x00000000;
      EnUpGroup[2][2][0] = 0xaaaaaaaa|0x3;    //EnUpGroup[2][2][1] = 0x00000000;
      EnDnGroup[2][2][0] = 0xaaaaaaaa&(~0x3); //EnDnGroup[2][2][1] = 0x00000000;

      EnUpLift[2][0][0]  = 0xaaaaaaaa|0x3;    //EnUpLift[2][0][1]  = 0x00000000; //双层停
      EnDnLift[2][0][0]  = 0xaaaaaaaa&(~0x3); //EnDnLift[2][0][1]  = 0x00000000;
      EnUpGroup[2][0][0] = 0xaaaaaaaa|0x3;    //EnUpGroup[2][0][1] = 0x00000000;
      EnDnGroup[2][0][0] = 0xaaaaaaaa&(~0x3); //EnDnGroup[2][0][1] = 0x00000000;
	  //第4组只调度1/2/3号梯
      EnUpLift[3][0][0]  = 0x00000000;        //EnUpLift[3][0][1]  = 0x00000000;
      EnDnLift[3][0][0]  = 0x00000003;        //EnDnLift[3][0][1]  = 0x00000000;
      EnUpGroup[3][0][0] = 0x00000000;        //EnUpGroup[3][0][1] = 0x00000000;
      EnDnGroup[3][0][0] = 0x00000003;        //EnDnGroup[3][0][1] = 0x00000000;

      EnUpLift[3][1][0]  = 0x00000000;        //EnUpLift[3][1][1]  = 0x00000000;
      EnDnLift[3][1][0]  = 0x00000003;        //EnDnLift[3][1][1]  = 0x00000000;
      EnUpGroup[3][1][0] = 0x00000000;        //EnUpGroup[3][1][1] = 0x00000000;
      EnDnGroup[3][1][0] = 0x00000003;        //EnDnGroup[3][1][1] = 0x00000000;

      EnUpLift[3][2][0]  = 0x00000000;        //EnUpLift[3][2][1]  = 0x00000000; //层层停
      EnDnLift[3][2][0]  = 0x00000003;        //EnDnLift[3][2][1]  = 0x00000000;
      EnUpGroup[3][2][0] = 0x00000000;        //EnUpGroup[3][2][1] = 0x00000000;
      EnDnGroup[3][2][0] = 0x00000003;        //EnDnGroup[3][2][1] = 0x00000000;
}
/*******************************************************************************/
/*  L1/L2=1-28楼  L3/L4=2-20楼  地址L1=1 L2=3 L3=2 L4=4 待梯L1=5 L2=5 L3=9 L4=16*/
void P24_Init(void)
{
    SetAddr[2] = 0;
    SetAddr[3] = 0;
    SetAddr[6] = 0;
    SetAddr[7] = 0;
	
	  SetAddr[0] = 1;
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 28; 
	  EnUpOrder[0][0]    = 0x07ffffff;
	  EnDnOrder[0][0]    = 0x0ffffffe;
	  EnUpLift[0][0][0]  = 0x07ffffff;  //按钮使能
    EnDnLift[0][0][0]  = 0x0ffffffe;  //按钮使能
    EnUpGroup[0][0][0] = 0x07ffffff;  //登记调度使能
    EnDnGroup[0][0][0] = 0x0ffffffe;  //登记调度使能
	  NewFlag[0] = 1;
	 
	  SetAddr[1] = 3;
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 28; 
	  EnUpOrder[1][0]    = 0x07ffffff;
	  EnDnOrder[1][0]    = 0x0ffffffe;
	  EnUpLift[0][1][0]  = 0x07ffffff;  //按钮使能
    EnDnLift[0][1][0]  = 0x0ffffffe;  //按钮使能
    EnUpGroup[0][1][0] = 0x07ffffff;  //登记调度使能
    EnDnGroup[0][1][0] = 0x0ffffffe;  //登记调度使能
	  NewFlag[1] = 1;

	  SetAddr[4] = 2;
	  FirstFloor[4] = 2;
	  MaxFloor[4] = 20; 
	  EnUpOrder[4][0]    = 0x0007fffe;
	  EnDnOrder[4][0]    = 0x000ffffc;
	  EnUpLift[0][4][0]  = 0x0007fffe; //按钮使能
    EnDnLift[0][4][0]  = 0x000ffffc;
    EnUpGroup[0][4][0] = 0x0007fffe;
    EnDnGroup[0][4][0] = 0x000ffffc;
	  NewFlag[4] = 1;

	  SetAddr[5] = 4;
	  FirstFloor[5] = 2;
	  MaxFloor[5] = 20; 
	  EnUpOrder[5][0]    = 0x0007fffe;
	  EnDnOrder[5][0]    = 0x000ffffc;
	  EnUpLift[0][5][0]  = 0x0007fffe; 
    EnDnLift[0][5][0]  = 0x000ffffc;
    EnUpGroup[0][5][0] = 0x0007fffe;
    EnDnGroup[0][5][0] = 0x000ffffc;
	  NewFlag[5] = 1;

    GroupFirstFloor = 1;
	  GroupMaxFloor = 28;

	  WaitStartTime[0] = (0<<16)|(0<<8)|0; 
	  WaitEndTime[0]   = (23<<16)|(59<<8)|59;
	  WaitFloorTime[0] = 120;//2分钟
	  WaitFloor[0][0] = 5;//等待楼层
	  WaitFloor[1][0] = 5;
	  WaitFloor[2][0] = 1;
	  WaitFloor[3][0] = 1;
	  WaitFloor[4][0] = 9;
	  WaitFloor[5][0] = 16;
	  WaitFloor[6][0] = 2;
	  WaitFloor[7][0] = 2;
}
/*******************************************************************************/
//14层单双
void P25_Init(void)
{
	  SetAddr[0] = 1;
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 14; 
	  EnUpOrder[0][0] = 0x1fff;
	  //EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0x3ffe;
	  //EnDnOrder[0][1] = 0x0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2;
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 13; 
	  EnUpOrder[1][0] = (0x55555557&0x1fff);
	  //EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = (0x55555557&0x3ffe);
	  //EnDnOrder[1][1] = 0x0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 4;
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 14; 
	  EnUpOrder[2][0] = (0xaaaaaaab&0x1fff);
	  //EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = (0xaaaaaaab&0x3ffe);
	  //EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 14;

	  WaitFloor3[0] = 1;
	  WaitFloor3[1] = 1;
	  WaitFloor3[2] = 1;
	  //第1组只调度1号梯
      EnUpLift[0][0][0]  = 0x1fff; //EnUpLift[0][0][1]  = 0x00000000;
      EnDnLift[0][0][0]  = 0x0000; //EnDnLift[0][0][1]  = 0x00000000;
      EnUpGroup[0][0][0] = 0x1fff; //EnUpGroup[0][0][1] = 0x00000000;
      EnDnGroup[0][0][0] = 0x0000; //EnDnGroup[0][0][1] = 0x00000000;
	  //第2组只调度1/2号梯	奇数
      EnUpLift[1][0][0]  = 0x00000000; //EnUpLift[1][0][1]  = 0x00000000;
      EnDnLift[1][0][0]  = (0x55555557&0x3ffc); //EnDnLift[1][0][1]  = 0x00000000;
      EnUpGroup[1][0][0] = (0x55555557&0x1fff); //EnUpGroup[1][0][1] = 0x00000000;
      EnDnGroup[1][0][0] = (0x55555557&0x3ffc); //EnDnGroup[1][0][1] = 0x00000000;

      EnUpLift[1][1][0]  = (0x55555557&0x1fff); //EnUpLift[1][1][1]  = 0x00000000;
      EnDnLift[1][1][0]  = (0x55555557&0x3ffc); //EnDnLift[1][1][1]  = 0x00000000;
      EnUpGroup[1][1][0] = (0x55555557&0x1fff); //EnUpGroup[1][1][1] = 0x00000000;
      EnDnGroup[1][1][0] = (0x55555557&0x3ffc); //EnDnGroup[1][1][1] = 0x00000000;
	  //第3组只调度1/3号梯	双数
      EnUpLift[3][0][0]  = 0x00000000; //EnUpLift[3][0][1]  = 0x00000000;
      EnDnLift[3][0][0]  = (0xaaaaaaab&0x3ffc); //EnDnLift[3][0][1]  = 0x00000000;
      EnUpGroup[3][0][0] = (0xaaaaaaab&0x1fff); //EnUpGroup[3][0][1] = 0x00000000;
      EnDnGroup[3][0][0] = (0xaaaaaaab&0x3ffc); //EnDnGroup[3][0][1] = 0x00000000;

      EnUpLift[3][2][0]  = (0xaaaaaaab&0x1fff); //EnUpLift[3][2][1]  = 0x00000000;
      EnDnLift[3][2][0]  = (0xaaaaaaab&0x3ffc); //EnDnLift[3][2][1]  = 0x00000000;
      EnUpGroup[3][2][0] = (0xaaaaaaab&0x1fff); //EnUpGroup[3][2][1] = 0x00000000;
      EnDnGroup[3][2][0] = (0xaaaaaaab&0x3ffc); //EnDnGroup[3][2][1] = 0x00000000;
	  //第4组只调度1/2/3号梯
      EnUpLift[4][0][0]  = 0x00000000; //EnUpLift[4][0][1]  = 0x00000000;
      EnDnLift[4][0][0]  = 0x00000003; //EnDnLift[4][0][1]  = 0x00000000;
      EnUpGroup[4][0][0] = 0x00000000; //EnUpGroup[4][0][1] = 0x00000000;
      EnDnGroup[4][0][0] = 0x00000003; //EnDnGroup[4][0][1] = 0x00000000;

      EnUpLift[4][1][0]  = 0x00000000; //EnUpLift[4][1][1]  = 0x00000000;
      EnDnLift[4][1][0]  = 0x00000003; //EnDnLift[4][1][1]  = 0x00000000;
      EnUpGroup[4][1][0] = 0x00000000; //EnUpGroup[4][1][1] = 0x00000000;
      EnDnGroup[4][1][0] = 0x00000003; //EnDnGroup[4][1][1] = 0x00000000;

      EnUpLift[4][2][0]  = 0x00000000; //EnUpLift[4][2][1]  = 0x00000000;
      EnDnLift[4][2][0]  = 0x00000003; //EnDnLift[4][2][1]  = 0x00000000;
      EnUpGroup[4][2][0] = 0x00000000; //EnUpGroup[4][2][1] = 0x00000000;
      EnDnGroup[4][2][0] = 0x00000003; //EnDnGroup[4][2][1] = 0x00000000;
}
/*******************************************************************************/
//_debug_group_floor33
void P26_Init(void)
{	  
	  SetAddr[0] = 3; //层层停
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 33; 
	  EnUpOrder[0][0] = 0xffffffff;
	  EnUpOrder[0][1] = 0x1;
	  EnDnOrder[0][0] = 0xffffffff;
	  EnDnOrder[0][1] = 0x1;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2; //单层停
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 33; 
	  EnUpOrder[1][0] = 0x55555555;
	  EnUpOrder[1][1] = 0x1;
	  EnDnOrder[1][0] = 0x55555555;
	  EnDnOrder[1][1] = 0x1;
	  NewFlag[1] = 1;

	  SetAddr[2] = 1; //双层停
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 32; 
	  EnUpOrder[2][0] = 0xaaaaaaab;
	  EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0xaaaaaaab;
	  EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 33;

    WaitFloor3[0] = 16;
	  WaitFloor3[1] = 1;
	  WaitFloor3[2] = 1;

    //第1组只调度1号梯
    EnUpLift[0][0][0]  = 0xfffffffe; EnUpLift[0][0][1]  = 0x1;
    EnDnLift[0][0][0]  = 0xfffffffe; EnDnLift[0][0][1]  = 0x1;
    EnUpGroup[0][0][0] = 0xfffffffe; EnUpGroup[0][0][1] = 0x1;
    EnDnGroup[0][0][0] = 0xfffffffe; EnDnGroup[0][0][1] = 0x1;

    EnUpLift[0][1][0]  = 0x55555554; EnUpLift[0][1][1]  = 0x1;
    EnDnLift[0][1][0]  = 0x55555554; EnDnLift[0][1][1]  = 0x1;
    EnUpGroup[0][1][0] = 0x55555554; EnUpGroup[0][1][1] = 0x1;
    EnDnGroup[0][1][0] = 0x55555554; EnDnGroup[0][1][1] = 0x1;

    EnUpLift[0][2][0]  = 0xaaaaaaaa; EnUpLift[0][2][1]  = 0;
    EnDnLift[0][2][0]  = 0xaaaaaaaa; EnDnLift[0][2][1]  = 0;
    EnUpGroup[0][2][0] = 0x2aaaaaaa; EnUpGroup[0][2][1] = 0;
    EnDnGroup[0][2][0] = 0xaaaaaaaa; EnDnGroup[0][2][1] = 0;
		
	  //第2组只调度1/2号梯	奇数
    EnUpLift[1][0][0]  = 0x00000000;
    EnDnLift[1][0][0]  = 0x00000000;
    EnUpGroup[1][0][0] = 0x00000001;
    EnDnGroup[1][0][0] = 0x00000001;

    EnUpLift[1][1][0]  = 0x00000001;
    EnDnLift[1][1][0]  = 0x00000001; 
    EnUpGroup[1][1][0] = 0x00000001; 
    EnDnGroup[1][1][0] = 0x00000001; 

    //第3组只调度1/3号梯	双数
    EnUpLift[2][0][0]  = 0x00000000;
    EnDnLift[2][0][0]  = 0x00000000;
    EnUpGroup[2][0][0] = 0x00000001;
    EnDnGroup[2][0][0] = 0x00000001;

    EnUpLift[2][2][0]  = 0x00000001;
    EnDnLift[2][2][0]  = 0x00000001;
    EnUpGroup[2][2][0] = 0x00000001;
    EnDnGroup[2][2][0] = 0x00000001;
		
    //第4组只调度1号梯	
    EnUpLift[3][0][0]  = 0x00000001; 
    EnDnLift[3][0][0]  = 0x00000001; 
    EnUpGroup[3][0][0] = 0x00000001; 
    EnDnGroup[3][0][0] = 0x00000001; 
}
/*******************************************************************************/
//皱越丰 3台错层
void P27_Init(void)
{	  
	  SetAddr[0] = 1; //
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 10; 
	  EnUpOrder[0][0] = 0x3ff;
	  //EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0x3ff;
	  //EnDnOrder[0][1] = 0x0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2; //错层
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 10; 
	  EnUpOrder[1][0] = 0x3f8;
	  //EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = 0x3f0;
	  //EnDnOrder[1][1] = 0x0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 4; //错层
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 10; 
	  EnUpOrder[2][0] = 0x3f8;
	  //EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0x3f0;
	  //EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;
		
		SetAddr[3] = 0; //
		SetAddr[4] = 0; //
		SetAddr[5] = 0; //
		SetAddr[6] = 0; //
		SetAddr[7] = 0; //

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 10;

    LiftFuntion |= WaitSpecial; //候梯方式2
	  WaitFloor2[0] = 4;
	  WaitFloor2[1] = 7;
    WaitFloor3[0] = 4;
	  WaitFloor3[1] = 7;
	  WaitFloor3[2] = 10;
		Mode2_WaitFloorTime = 60;//60秒

    //第1组只调度1号梯
    EnUpLift[0][0][0]  = 0x3ff;   EnUpLift[0][0][1]  = 0;
    EnDnLift[0][0][0]  = 0x3ff;   EnDnLift[0][0][1]  = 0;
    EnUpGroup[0][0][0] = 0x3ff;   EnUpGroup[0][0][1] = 0;
    EnDnGroup[0][0][0] = 0x3ff;   EnDnGroup[0][0][1] = 0;

    EnUpLift[0][1][0]  = 0x3f8;   EnUpLift[1][1][1]  = 0;
    EnDnLift[0][1][0]  = 0x3f0;   EnDnLift[1][1][1]  = 0;
    EnUpGroup[0][1][0] = 0x3f8;   EnUpGroup[1][1][1] = 0;
    EnDnGroup[0][1][0] = 0x3f0;   EnDnGroup[1][1][1] = 0;

    EnUpLift[0][2][0]  = 0x3f8;   EnUpLift[2][2][1]  = 0;
    EnDnLift[0][2][0]  = 0x3f0;   EnDnLift[2][2][1]  = 0;
    EnUpGroup[0][2][0] = 0x3f8;   EnUpGroup[2][2][1] = 0;
    EnDnGroup[0][2][0] = 0x3f0;   EnDnGroup[2][2][1] = 0;
}
/*******************************************************************************/
//诚丰商业中心
// 1 2 3  4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
// x G SO 1 2 x 4 x 6 x  8  x  10 x  12 x  14 x  16 x  18 x  20 x    (addr:2   number:2)
// x G SO 1 x x 4 x 6 x  8  x  10 x  12 x  14 x  16 x  18 x  20 x    (addr:4   number:5)
//-1 G SO 1 2 3 4 5 6 7  8  9  10 11 12 13 14 15 16 17 18 19 20 21   (addr:3   number:3)

void P28_Init(void)
{	  
	  SetAddr[0] = 3; //层层停
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 24; 
	  EnUpOrder[0][0] = 0x007fffff;
	  //EnUpOrder[0][1] = 0;
	  EnDnOrder[0][0] = 0x00fffffe;
	  //EnDnOrder[0][1] = 0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 4; //单层停
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 23; 
	  EnUpOrder[1][0] = 0x0015554e;
	  //EnUpOrder[1][1] = 0;
	  EnDnOrder[1][0] = 0x0055554c;
	  //EnDnOrder[1][1] = 0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 2; //单层停
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 23; 
	  EnUpOrder[2][0] = 0x0015555e;
	  //EnUpOrder[2][1] = 0;
	  EnDnOrder[2][0] = 0x0055555c;
	  //EnDnOrder[2][1] = 0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 24;

    LiftFuntion |= DelAllFunction;

		//候梯方式1的候梯时间设为120秒
	  WaitFloor[0][0] = 2;
	  WaitFloor[1][0] = 2;
	  WaitFloor[2][0] = 2;
		WaitFloorTime[0] = 30;//120;
		WaitStartTime[0] = (0<<16)|(0<<8)|0; 
	  WaitEndTime[0] = (23<<16)|(59<<8)|59;

    //第1组只调度1号梯
    EnUpLift[0][0][0]  = 0x007fffff;            
    EnDnLift[0][0][0]  = 0x00fffffe;           
    EnUpGroup[0][0][0] = 0x007fffff;            
    EnDnGroup[0][0][0] = 0x00fffffe;       

    EnUpLift[0][1][0]  = 0x0015554e;
    EnDnLift[0][1][0]  = 0x0055554c;
    EnUpGroup[0][1][0] = 0;  
    EnDnGroup[0][1][0] = 0;   

    EnUpLift[0][2][0]  = 0x0015555e; 
    EnDnLift[0][2][0]  = 0x0055555c;  
    EnUpGroup[0][2][0] = 0; 
    EnDnGroup[0][2][0] = 0;    
		
	  //第2组只调度2号梯
    EnUpLift[1][0][0]  = 0x0015554e;
    EnDnLift[1][0][0]  = 0x0055554c;
    EnUpGroup[1][0][0] = 0;
    EnDnGroup[1][0][0] = 0; 

    EnUpLift[1][1][0]  = 0x0015554e;  
    EnDnLift[1][1][0]  = 0x0055554c;
    EnUpGroup[1][1][0] = 0x0015554e;
    EnDnGroup[1][1][0] = 0x0055554c;

    EnUpLift[1][2][0]  = 0x0015554e;
    EnDnLift[1][2][0]  = 0x0055554c;  
    EnUpGroup[1][2][0] = 0; 
    EnDnGroup[1][2][0] = 0;
    
    //第3组只调度3号梯
    EnUpLift[2][0][0]  = 0x0015555e;
    EnDnLift[2][0][0]  = 0x0055555c;
    EnUpGroup[2][0][0] = 0; 
    EnDnGroup[2][0][0] = 0;

    EnUpLift[2][1][0]  = 0x0015555e; 
    EnDnLift[2][1][0]  = 0x0055555c;
    EnUpGroup[2][1][0] = 0;
    EnDnGroup[2][1][0] = 0;

    EnUpLift[2][2][0]  = 0x0015555e;
    EnDnLift[2][2][0]  = 0x0055555c;
    EnUpGroup[2][2][0] = 0x0015555e; 
    EnDnGroup[2][2][0] = 0x0055555c;
}

void P29_Init(void)
{	  
	  SetAddr[0] = 3; //层层停
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 24; 
	  EnUpOrder[0][0] = 0x007fffff;
	  //EnUpOrder[0][1] = 0;
	  EnDnOrder[0][0] = 0x00fffffe;
	  //EnDnOrder[0][1] = 0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 4; //单层停
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 23; 
	  EnUpOrder[1][0] = 0x0015554e;
	  //EnUpOrder[1][1] = 0;
	  EnDnOrder[1][0] = 0x0055554c;
	  //EnDnOrder[1][1] = 0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 2; //单层停
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 23; 
	  EnUpOrder[2][0] = 0x0015555e;
	  //EnUpOrder[2][1] = 0;
	  EnDnOrder[2][0] = 0x0055555c;
	  //EnDnOrder[2][1] = 0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 24;

		//候梯方式1的候梯时间设为120秒
	  WaitFloor[0][0] = 2;
	  WaitFloor[1][0] = 2;
	  WaitFloor[2][0] = 2;
		WaitFloorTime[0] = 30;//120
		WaitStartTime[0] = (0<<16)|(0<<8)|0; 
	  WaitEndTime[0] = (23<<16)|(59<<8)|59;

    //第1组只调度1号梯
    EnUpLift[0][0][0]  = 0x007fffff;            
    EnDnLift[0][0][0]  = 0x00fffffe;           
    EnUpGroup[0][0][0] = 0x007fffff;            
    EnDnGroup[0][0][0] = 0x00fffffe;       

    EnUpLift[0][1][0]  = 0x0015554e;
    EnDnLift[0][1][0]  = 0x0055554c;
    EnUpGroup[0][1][0] = 0x0015554e; 
    EnDnGroup[0][1][0] = 0x0055554c; 

    EnUpLift[0][2][0]  = 0x0015555e; 
    EnDnLift[0][2][0]  = 0x0055555c;  
    EnUpGroup[0][2][0] = 0x0015555e;
    EnDnGroup[0][2][0] = 0x0055555c;    
}

/*******************************************************************************/
//IFE 4台错层特殊
//  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  
// B1 B2 B3  G  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 x  17 18 19 20 21 22 23 24  (L4  addr:1)
//  x  x  x  G  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 x  17 18 19 20 21 22 23 24  (L3  addr:2)
//  x  x  x  G  1  x  3  x  5  x  7  x  9  10 11 12 13 14 15 x  17 18 19 20  x  x  x  x  (L2  addr:4)
//  x  x  x  G  1  x  3  x  5  x  7  x  9  10 11 12 13 14 15 x  17 18 19  x  x  x  x  x  (L1  addr:6)
                           
void P30_Init(void)//标准处理
{	  
	  SetAddr[0] = 1; 
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 28; 
	  EnUpOrder[0][0] = 0x07ffffff;
	  //EnUpOrder[0][1] = 0;
	  EnDnOrder[0][0] = 0x0ffffffe;
	  //EnDnOrder[0][1] = 0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2; 
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 28; 
	  EnUpOrder[1][0] = 0x07f7fff8;
	  //EnUpOrder[1][1] = 0;
	  EnDnOrder[1][0] = 0x0ff7fff0;
	  //EnDnOrder[1][1] = 0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 4; 
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 24; 
	  EnUpOrder[2][0] = 0x0077ff58;
	  //EnUpOrder[2][1] = 0;
	  EnDnOrder[2][0] = 0x00f7ff50;
	  //EnDnOrder[2][1] = 0;
	  NewFlag[2] = 1;

	  SetAddr[3] = 6; 
	  FirstFloor[3] = 1;
	  MaxFloor[3] = 23; 
	  EnUpOrder[3][0] = 0x0057ff58;
	  //EnUpOrder[3][1] = 0;
	  EnDnOrder[3][0] = 0x0077ff50;
	  //EnDnOrder[3][1] = 0;
	  NewFlag[3] = 1;

    GroupFirstFloor = 1;
	  GroupMaxFloor = 28;
		LiftFuntion |= ScanOrderEn100ms;

		//候梯方式1的候梯时间设为120秒
	  WaitFloor[0][0] = 2;
	  WaitFloor[1][0] = 2;
	  WaitFloor[2][0] = 2;
		WaitFloorTime[0] = 30;//120
		WaitStartTime[0] = (0<<16)|(0<<8)|0; 
	  WaitEndTime[0] = (23<<16)|(59<<8)|59;

    //第1组只调度1号梯
    EnUpLift[0][0][0]  = EnUpOrder[0][0];          
    EnDnLift[0][0][0]  = EnDnOrder[0][0];         
    EnUpGroup[0][0][0] = EnUpOrder[0][0];            
    EnDnGroup[0][0][0] = EnDnOrder[0][0];       

    EnUpLift[0][1][0]  = EnUpOrder[1][0];
    EnDnLift[0][1][0]  = EnDnOrder[1][0];
    EnUpGroup[0][1][0] = EnUpOrder[1][0]; 
    EnDnGroup[0][1][0] = EnDnOrder[1][0];

    EnUpLift[0][2][0]  = EnUpOrder[2][0];
    EnDnLift[0][2][0]  = EnDnOrder[2][0];  
    EnUpGroup[0][2][0] = EnUpOrder[2][0];
    EnDnGroup[0][2][0] = EnDnOrder[2][0];  

    EnUpLift[0][3][0]  = EnUpOrder[3][0];
    EnDnLift[0][3][0]  = EnDnOrder[3][0];
    EnUpGroup[0][3][0] = EnUpOrder[3][0];
    EnDnGroup[0][3][0] = EnDnOrder[3][0];
}
/*******************************************************************************/

void P31_Init(void) //分组处理
{	  
	  SetAddr[0] = 1; 
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 28; 
	  EnUpOrder[0][0] = 0x07ffffff;
	  EnUpOrder[0][1] = 0;
	  EnDnOrder[0][0] = 0x0ffffffe;
	  EnDnOrder[0][1] = 0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2; 
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 28; 
	  EnUpOrder[1][0] = 0x07f7fff8;
	  EnUpOrder[1][1] = 0;
	  EnDnOrder[1][0] = 0x0ff7fff0;
	  EnDnOrder[1][1] = 0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 4; 
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 24; 
	  EnUpOrder[2][0] = 0x0077ff58;
	  EnUpOrder[2][1] = 0;
	  EnDnOrder[2][0] = 0x00f7ff50;
	  EnDnOrder[2][1] = 0;
	  NewFlag[2] = 1;

	  SetAddr[3] = 6; 
	  FirstFloor[3] = 1;
	  MaxFloor[3] = 23; 
	  EnUpOrder[3][0] = 0x0057ff58;
	  EnUpOrder[3][1] = 0;
	  EnDnOrder[3][0] = 0x0077ff50;
	  EnDnOrder[3][1] = 0;
	  NewFlag[3] = 1;

    GroupFirstFloor = 1;
	  GroupMaxFloor = 28;
		LiftFuntion |= ScanOrderEn100ms;

		//候梯方式1的候梯时间设为120秒
	  WaitFloor[0][0] = 2;
	  WaitFloor[1][0] = 2;
	  WaitFloor[2][0] = 2;
		WaitFloorTime[0] = 30;//120
		WaitStartTime[0] = (0<<16)|(0<<8)|0; 
	  WaitEndTime[0] = (23<<16)|(59<<8)|59;

    //第1组只调度1号梯
    EnUpLift[0][0][0]  = 0; //EnUpOrder[0][0];          
    EnDnLift[0][0][0]  = 0; //EnDnOrder[0][0];         
    EnUpGroup[0][0][0] = EnUpOrder[0][0];            
    EnDnGroup[0][0][0] = EnDnOrder[0][0];       

    EnUpLift[0][1][0]  = 0; //EnUpOrder[1][0];
    EnDnLift[0][1][0]  = 0; //EnDnOrder[1][0];
    EnUpGroup[0][1][0] = EnUpOrder[1][0]; 
    EnDnGroup[0][1][0] = EnDnOrder[1][0];

    EnUpLift[0][2][0]  = EnUpOrder[2][0];
    EnDnLift[0][2][0]  = EnDnOrder[2][0];  
    EnUpGroup[0][2][0] = EnUpOrder[2][0];
    EnDnGroup[0][2][0] = EnDnOrder[2][0];  

    EnUpLift[0][3][0]  = EnUpOrder[3][0];
    EnDnLift[0][3][0]  = EnDnOrder[3][0];
    EnUpGroup[0][3][0] = EnUpOrder[3][0];
    EnDnGroup[0][3][0] = EnDnOrder[3][0];
//---------------------------------------------
    EnUpLift[1][0][0]  = EnUpOrder[0][0];          
    EnDnLift[1][0][0]  = EnDnOrder[0][0];         
    EnUpGroup[1][0][0] = EnUpOrder[0][0];            
    EnDnGroup[1][0][0] = EnDnOrder[0][0];       

    EnUpLift[1][1][0]  = EnUpOrder[1][0];
    EnDnLift[1][1][0]  = EnDnOrder[1][0];
    EnUpGroup[1][1][0] = EnUpOrder[1][0]; 
    EnDnGroup[1][1][0] = EnDnOrder[1][0];

    EnUpLift[1][2][0]  = 0; //EnUpOrder[2][0];
    EnDnLift[1][2][0]  = 0; //EnDnOrder[2][0];  
    EnUpGroup[1][2][0] = EnUpOrder[2][0];
    EnDnGroup[1][2][0] = EnDnOrder[2][0];  

    EnUpLift[1][3][0]  = 0; //EnUpOrder[3][0];
    EnDnLift[1][3][0]  = 0; //EnDnOrder[3][0];
    EnUpGroup[1][3][0] = EnUpOrder[3][0];
    EnDnGroup[1][3][0] = EnDnOrder[3][0];
}
/*******************************************************************************/
//澳门 3台
//  1  2  3  4  5  6  7  8  9  10 11 12 13 14   
//  G  1  2  3  4  5  6  7  8  9  10 11 12 13  (L2  addr:3)
//  G  1  x  3  x  5  x  7  x  9  x  11 x  13  (L1  addr:1)
//  G  1  2  x  4  x  6  x  8  x  10 x  12 x   (L3  addr:2)

void P39_Init(void)
{	  
	  SetAddr[0] = 3; //层层停
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 14; 
	  EnUpOrder[0][0] = 0x1fff;
	  EnUpOrder[0][1] = 0;
	  EnDnOrder[0][0] = 0x3ffe;
	  EnDnOrder[0][1] = 0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 1; //单层停
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 14; 
	  EnUpOrder[1][0] = (1ul<<0)|(1ul<<1)|(1ul<<3)|(1ul<<5)|(1ul<<7)|(1ul<<9)|(1ul<<11);
	  EnUpOrder[1][1] = 0;
	  EnDnOrder[1][0] = (1ul<<1)|(1ul<<3)|(1ul<<5)|(1ul<<7)|(1ul<<9)|(1ul<<11)|(1ul<<13);
	  EnDnOrder[1][1] = 0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 2; //单层停
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 14; 
	  EnUpOrder[2][0] = (1ul<<0)|(1ul<<1)|(1ul<<2)|(1ul<<4)|(1ul<<6)|(1ul<<8)|(1ul<<10);
	  EnUpOrder[2][1] = 0;
	  EnDnOrder[2][0] = (1ul<<1)|(1ul<<2)|(1ul<<4)|(1ul<<6)|(1ul<<8)|(1ul<<10)|(1ul<<12);
	  EnDnOrder[2][1] = 0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 14;

		//候梯方式1的候梯时间设为120秒
	  WaitFloor[0][0] = 2;   //2
	  WaitFloor[1][0] = 8;   //7
	  WaitFloor[2][0] = 7;   //6
		WaitFloorTime[0] = 30; //120
		WaitStartTime[0] = (0<<16)|(0<<8)|0; 
	  WaitEndTime[0] = (23<<16)|(59<<8)|59;


    //第1组只调度1号梯
    EnUpLift[0][0][0]  = 0x3;          
    EnDnLift[0][0][0]  = 0;           
    EnUpGroup[0][0][0] = 0x3;           
    EnDnGroup[0][0][0] = 0;       

    EnUpLift[0][1][0]  = 0;
    EnDnLift[0][1][0]  = 0;
    EnUpGroup[0][1][0] = 0; 
    EnDnGroup[0][1][0] = 0;   

    EnUpLift[0][2][0]  = 0;
    EnDnLift[0][2][0]  = 0;  
    EnUpGroup[0][2][0] = 0;
    EnDnGroup[0][2][0] = 0;    
		
	  //第2组只调度1/2号梯
    EnUpLift[1][0][0]  = (1ul<<3)|(1ul<<5)|(1ul<<7)|(1ul<<9)|(1ul<<11);
    EnDnLift[1][0][0]  = (1ul<<3)|(1ul<<5)|(1ul<<7)|(1ul<<9)|(1ul<<11)|(1ul<<13);
    EnUpGroup[1][0][0] = (3ul<<0)|(1ul<<3)|(1ul<<5)|(1ul<<7)|(1ul<<9)|(1ul<<11);
    EnDnGroup[1][0][0] = (1ul<<3)|(1ul<<5)|(1ul<<7)|(1ul<<9)|(1ul<<11)|(1ul<<13);

    EnUpLift[1][1][0]  = (3ul<<0)|(1ul<<3)|(1ul<<5)|(1ul<<7)|(1ul<<9)|(1ul<<11);
    EnDnLift[1][1][0]  = (1ul<<3)|(1ul<<5)|(1ul<<7)|(1ul<<9)|(1ul<<11)|(1ul<<13);
    EnUpGroup[1][1][0] = (3ul<<0)|(1ul<<3)|(1ul<<5)|(1ul<<7)|(1ul<<9)|(1ul<<11);
    EnDnGroup[1][1][0] = (1ul<<3)|(1ul<<5)|(1ul<<7)|(1ul<<9)|(1ul<<11)|(1ul<<13);

    EnUpLift[1][2][0]  = 0;
    EnDnLift[1][2][0]  = 0;  
    EnUpGroup[1][2][0] = 0;
    EnDnGroup[1][2][0] = 0;
    
    //第3组只调度1/3号梯
    EnUpLift[2][0][0]  = (1ul<<2)|(1ul<<4)|(1ul<<6)|(1ul<<8)|(1ul<<10);
    EnDnLift[2][0][0]  = (1ul<<2)|(1ul<<4)|(1ul<<6)|(1ul<<8)|(1ul<<10)|(1ul<<12);
    EnUpGroup[2][0][0] = (3ul<<0)|(1ul<<2)|(1ul<<4)|(1ul<<6)|(1ul<<8)|(1ul<<10);
    EnDnGroup[2][0][0] = (1ul<<2)|(1ul<<4)|(1ul<<6)|(1ul<<8)|(1ul<<10)|(1ul<<12);

    EnUpLift[2][1][0]  = 0; 
    EnDnLift[2][1][0]  = 0;
    EnUpGroup[2][1][0] = 0;
    EnDnGroup[2][1][0] = 0;

    EnUpLift[2][2][0]  = (3ul<<0)|(1ul<<2)|(1ul<<4)|(1ul<<6)|(1ul<<8)|(1ul<<10);
    EnDnLift[2][2][0]  = (1ul<<2)|(1ul<<4)|(1ul<<6)|(1ul<<8)|(1ul<<10)|(1ul<<12);
    EnUpGroup[2][2][0] = (3ul<<0)|(1ul<<2)|(1ul<<4)|(1ul<<6)|(1ul<<8)|(1ul<<10);
    EnDnGroup[2][2][0] = (1ul<<2)|(1ul<<4)|(1ul<<6)|(1ul<<8)|(1ul<<10)|(1ul<<12);

    //第4组只调度1/2/3号梯
    EnUpLift[3][0][0]  = 0;
    EnDnLift[3][0][0]  = (1ul<<1);
    EnUpGroup[3][0][0] = 0; 
    EnDnGroup[3][0][0] = (1ul<<1);

    EnUpLift[3][1][0]  = 0; 
    EnDnLift[3][1][0]  = (1ul<<1);
    EnUpGroup[3][1][0] = 0;
    EnDnGroup[3][1][0] = (1ul<<1);

    EnUpLift[3][2][0]  = 0;
    EnDnLift[3][2][0]  = (1ul<<1);
    EnUpGroup[3][2][0] = 0; 
    EnDnGroup[3][2][0] = (1ul<<1);   
}
/*******************************************************************************/
//0 1 2 3 4 5 6 7 8 9 10 11   addr=1
//0   2   4   6   8   10      addr=3
//0 1   3   5   7   9    11   addr=2
void P40_Init(void)
{	  
	  SetAddr[0] = 1; //层层停
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 12; 
	  EnUpOrder[0][0] = 0xfff;
	  EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0xfff;
	  EnDnOrder[0][1] = 0x0;
	  NewFlag[0] = 0;

	  SetAddr[1] = 3; //单层停
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 11; 
	  EnUpOrder[1][0] = 0x555;
	  EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = 0x555;
	  EnDnOrder[1][1] = 0x0;
	  NewFlag[1] = 0;

	  SetAddr[2] = 2; //双层停
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 12; 
	  EnUpOrder[2][0] = 0xaab;
	  EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0xaab;
	  EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 0;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 12;

    WaitFloor3[0] = 6;
	  WaitFloor3[1] = 1;
	  WaitFloor3[2] = 1;

    //第1组只调度1号梯
    EnUpLift[0][0][0]  = 0xffe; 
    EnDnLift[0][0][0]  = 0xffe; 
    EnUpGroup[0][0][0] = 0xffe; 
    EnDnGroup[0][0][0] = 0xffe; 

    EnUpLift[0][1][0]  = 0x154; 
    EnDnLift[0][1][0]  = 0x554; 
    EnUpGroup[0][1][0] = 0x154; 
    EnDnGroup[0][1][0] = 0x554; 

    EnUpLift[0][2][0]  = 0xaaa; 
    EnDnLift[0][2][0]  = 0xaaa; 
    EnUpGroup[0][2][0] = 0xaaa; 
    EnDnGroup[0][2][0] = 0xaaa; 
		
	  //第2组只调度1号梯
    EnUpLift[1][0][0]  = 0x00000001;
    EnDnLift[1][0][0]  = 0x00000001;
    EnUpGroup[1][0][0] = 0x00000001;
    EnDnGroup[1][0][0] = 0x00000001;

    //第3组只调度2号梯
    EnUpLift[2][1][0]  = 0x00000001;
    EnDnLift[2][1][0]  = 0x00000001;
    EnUpGroup[2][1][0] = 0x00000001;
    EnDnGroup[2][1][0] = 0x00000001;
		
    //第4组只调度3号梯	
    EnUpLift[3][2][0]  = 0x00000001; 
    EnDnLift[3][2][0]  = 0x00000001; 
    EnUpGroup[3][2][0] = 0x00000001; 
    EnDnGroup[3][2][0] = 0x00000001; 
}
/*******************************************************************************/
//RC P 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24   层层停
//RC P 1   3   5   7   9    11    13    15    17    19    21    23      单
//RC P   2   4   6   8   10    12    14    16    18    20    22    24   双
void P41_Init(void)
{	  
	  SetAddr[0] = 3; //层层停
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 26; 
	  EnUpOrder[0][0] = 0x03ffffff;
	  EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0x03ffffff;
	  EnDnOrder[0][1] = 0x0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2; //单层停
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 25; 
	  EnUpOrder[1][0] = 0x01555557;
	  EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = 0x01555557;
	  EnDnOrder[1][1] = 0x0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 1; //双层停
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 26; 
	  EnUpOrder[2][0] = 0x02aaaaab;
	  EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0x02aaaaab;
	  EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 26;

    WaitFloor3[0] = 18;
	  WaitFloor3[1] = 1;
	  WaitFloor3[2] = 1;

    //第1组只调度1号梯
    EnUpLift[0][0][0]  = 0x03fffffc; 
    EnDnLift[0][0][0]  = 0x03fffffc;
    EnUpGroup[0][0][0] = 0x03fffffc; 
    EnDnGroup[0][0][0] = 0x03fffffc; 

    EnUpLift[0][1][0]  = 0x01555554;
    EnDnLift[0][1][0]  = 0x01555554; 
    EnUpGroup[0][1][0] = 0x01555554; 
    EnDnGroup[0][1][0] = 0x01555554;

    EnUpLift[0][2][0]  = 0x02aaaaa8;
    EnDnLift[0][2][0]  = 0x02aaaaa8;
    EnUpGroup[0][2][0] = 0x02aaaaa8; 
    EnDnGroup[0][2][0] = 0x02aaaaa8;
		
	  //第2组只调度1/2号梯	奇数
    EnUpLift[1][0][0]  = 0x00000000;
    EnDnLift[1][0][0]  = 0x00000000;
    EnUpGroup[1][0][0] = 0x00000003;
    EnDnGroup[1][0][0] = 0x00000003;

    EnUpLift[1][1][0]  = 0x00000003;
    EnDnLift[1][1][0]  = 0x00000003; 
    EnUpGroup[1][1][0] = 0x00000003; 
    EnDnGroup[1][1][0] = 0x00000003; 

    //第3组只调度1/3号梯	双数
    EnUpLift[2][0][0]  = 0x00000000;
    EnDnLift[2][0][0]  = 0x00000000;
    EnUpGroup[2][0][0] = 0x00000003;
    EnDnGroup[2][0][0] = 0x00000003;

    EnUpLift[2][2][0]  = 0x00000003;
    EnDnLift[2][2][0]  = 0x00000003;
    EnUpGroup[2][2][0] = 0x00000003;
    EnDnGroup[2][2][0] = 0x00000003;
		
    //第4组只调度1号梯	
    EnUpLift[3][0][0]  = 0x00000003; 
    EnDnLift[3][0][0]  = 0x00000003; 
    EnUpGroup[3][0][0] = 0x00000003; 
    EnDnGroup[3][0][0] = 0x00000003; 
}
/*******************************************************************************/
/*******************************************************************************/
//B G 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20    层层停
//B G 1   3   5   7   9    11    13    15    17    19       单
//B G   2   4   6   8   10    12    14    16    18    20    双
void P42_Init(void)
{	  
	  SetAddr[0] = 3; //层层停
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 22; 
	  EnUpOrder[0][0] = 0x003fffff;
	  EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0x003fffff;
	  EnDnOrder[0][1] = 0x0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2; //单层停
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 21; 
	  EnUpOrder[1][0] = 0x00155557;
	  EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = 0x00155557;
	  EnDnOrder[1][1] = 0x0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 1; //双层停
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 22; 
	  EnUpOrder[2][0] = 0x002aaaab;
	  EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0x002aaaab;
	  EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 22;

    WaitFloor3[0] = 12;
	  WaitFloor3[1] = 1;
	  WaitFloor3[2] = 1;

    //第1组只调度1号梯
    EnUpLift[0][0][0]  = 0x003ffffc;
    EnDnLift[0][0][0]  = 0x003ffffc;
    EnUpGroup[0][0][0] = 0x003ffffc; 
    EnDnGroup[0][0][0] = 0x003ffffc; 

    EnUpLift[0][1][0]  = 0x00155554;
    EnDnLift[0][1][0]  = 0x00155554; 
    EnUpGroup[0][1][0] = 0x00155554; 
    EnDnGroup[0][1][0] = 0x00155554;

    EnUpLift[0][2][0]  = 0x002aaaa8;
    EnDnLift[0][2][0]  = 0x002aaaa8;
    EnUpGroup[0][2][0] = 0x002aaaa8; 
    EnDnGroup[0][2][0] = 0x002aaaa8;
		
	  //第2组只调度1/2号梯	奇数
    EnUpLift[1][0][0]  = 0x00000000;
    EnDnLift[1][0][0]  = 0x00000000;
    EnUpGroup[1][0][0] = 0x00000003;
    EnDnGroup[1][0][0] = 0x00000003;

    EnUpLift[1][1][0]  = 0x00000003;
    EnDnLift[1][1][0]  = 0x00000003; 
    EnUpGroup[1][1][0] = 0x00000003; 
    EnDnGroup[1][1][0] = 0x00000003; 

    //第3组只调度1/3号梯	双数
    EnUpLift[2][0][0]  = 0x00000000;
    EnDnLift[2][0][0]  = 0x00000000;
    EnUpGroup[2][0][0] = 0x00000003;
    EnDnGroup[2][0][0] = 0x00000003;

    EnUpLift[2][2][0]  = 0x00000003;
    EnDnLift[2][2][0]  = 0x00000003;
    EnUpGroup[2][2][0] = 0x00000003;
    EnDnGroup[2][2][0] = 0x00000003;
		
    //第4组只调度1号梯	
    EnUpLift[3][0][0]  = 0x00000003; 
    EnDnLift[3][0][0]  = 0x00000003; 
    EnUpGroup[3][0][0] = 0x00000003; 
    EnDnGroup[3][0][0] = 0x00000003; 
}
/*******************************************************************************/
//G 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24    层层停
//G 1 2   4   6   8   10    12    14    16    18    20    22    24    单
//G 1 2 3   5   7   9    11    13    15    17    19    21    23       双
void P43_Init(void)
{	  
	  SetAddr[0] = 3; //层层停
	  FirstFloor[0] = 1;
	  MaxFloor[0] = 25; 
	  EnUpOrder[0][0] = 0x01ffffff;
	  EnUpOrder[0][1] = 0x0;
	  EnDnOrder[0][0] = 0x01ffffff;
	  EnDnOrder[0][1] = 0x0;
	  NewFlag[0] = 1;

	  SetAddr[1] = 2; //单层停 
	  FirstFloor[1] = 1;
	  MaxFloor[1] = 25; 
	  EnUpOrder[1][0] = 0x01555557;
	  EnUpOrder[1][1] = 0x0;
	  EnDnOrder[1][0] = 0x01555557;
	  EnDnOrder[1][1] = 0x0;
	  NewFlag[1] = 1;

	  SetAddr[2] = 1; //双层停
	  FirstFloor[2] = 1;
	  MaxFloor[2] = 24; 
	  EnUpOrder[2][0] = 0x00aaaaaf;
	  EnUpOrder[2][1] = 0x0;
	  EnDnOrder[2][0] = 0x00aaaaaf;
	  EnDnOrder[2][1] = 0x0;
	  NewFlag[2] = 1;

	  GroupFirstFloor = 1;
	  GroupMaxFloor = 25;

    WaitFloor3[0] = 12;
	  WaitFloor3[1] = 1;
	  WaitFloor3[2] = 1;

    //第1组只调度1号梯
    EnUpLift[0][0][0]  = 0x01fffff8;
    EnDnLift[0][0][0]  = 0x01fffff8;
    EnUpGroup[0][0][0] = 0x01fffff8; 
    EnDnGroup[0][0][0] = 0x01fffff8; 

    EnUpLift[0][1][0]  = 0x01555550;
    EnDnLift[0][1][0]  = 0x01555550; 
    EnUpGroup[0][1][0] = 0x01555550; 
    EnDnGroup[0][1][0] = 0x01555550;

    EnUpLift[0][2][0]  = 0x00aaaaa8;
    EnDnLift[0][2][0]  = 0x00aaaaa8;
    EnUpGroup[0][2][0] = 0x00aaaaa8; 
    EnDnGroup[0][2][0] = 0x00aaaaa8;
		
	  //第2组只调度1/2号梯	奇数
    EnUpLift[1][0][0]  = 0x00000000;
    EnDnLift[1][0][0]  = 0x00000000;
    EnUpGroup[1][0][0] = 0x00000007;
    EnDnGroup[1][0][0] = 0x00000007;

    EnUpLift[1][1][0]  = 0x00000007;
    EnDnLift[1][1][0]  = 0x00000007; 
    EnUpGroup[1][1][0] = 0x00000007; 
    EnDnGroup[1][1][0] = 0x00000007; 

    //第3组只调度1/3号梯	双数
    EnUpLift[2][0][0]  = 0x00000000;
    EnDnLift[2][0][0]  = 0x00000000;
    EnUpGroup[2][0][0] = 0x00000007;
    EnDnGroup[2][0][0] = 0x00000007;

    EnUpLift[2][2][0]  = 0x00000007;
    EnDnLift[2][2][0]  = 0x00000007;
    EnUpGroup[2][2][0] = 0x00000007;
    EnDnGroup[2][2][0] = 0x00000007;
		
    //第4组只调度1号梯	
    EnUpLift[3][0][0]  = 0x00000007; 
    EnDnLift[3][0][0]  = 0x00000007; 
    EnUpGroup[3][0][0] = 0x00000007; 
    EnDnGroup[3][0][0] = 0x00000007; 
}
/*******************************************************************************/
