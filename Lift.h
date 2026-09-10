/*****************************************************************************
  全局变量
******************************************************************************/
#ifndef __LIFT_H 
#define __LIFT_H
/*****************************************************************************/
#ifdef Lift_Ex
   #define Extern_Lift         //定义变量
#else
   #define Extern_Lift extern  //声明变量
#endif
#include "absacc.h"
/*****************************************************************************/
#define RunNoOrder         (1ul<<0)  //调用无呼梯微机
#define RunOneInOrder      (1ul<<1)  //调用一个内选微机
#define BackMode1      	   (1ul<<2)  //在时间段功能时,非设定时间外采用分散待梯	
#define BackNotDelInOrder	 (1ul<<3)  //在待梯回基站时有外呼，不取消内选
#define LiftErrorDeal   	 (1ul<<4)  //有指令给微机，但微机无方向，退离并且发内选
#define DisableOrderFun    (1ul<<5)  //增加电脑禁止呼功能
#define MainSubEn          (1ul<<6)  //主副群控板使能(V5.0版本用)
#define SubFlag            (1ul<<7)  //副群控板(V5.0版本用)
#define RunNoOrderTimeCtrl (1ul<<8)  //调用无呼梯微机(并考虑时间)
#define ScanOrderEn100ms   (1ul<<9)  //用菜单方式100ms查询登记指令	 
#define ScanOrderEn1S      (1ul<<10) //用菜单方式1s查询登记指令	 
#define FloorSendEn1S      (1ul<<11) //电梯停止时，1秒间隔发送本层登记指令	 
#define DelAllFunction     (1ul<<12) //澳门采用消号按按钮使能处理	 
	
#define StdLedSendFun      (1ul<<23) //标准模式下，灯控制采用群发
#define WaitData           (1ul<<24) //分散待梯只有8台的组数据有效
#define TimeSpecial        (1ul<<25) //高峰期专用功能
#define PowerFunNoGroup	   (1ul<<26) //发电机无群控功能
#define PowerFun           (1ul<<27) //发电机功能	 
#define TaiWangFun         (1ul<<28) //台湾残疾人外呼专用
#define StdFun             (1ul<<29) //标准群控标志
#define WaitSpecial        (1ul<<30) //分散待梯
#define RunSpecial         (1ul<<31) //特殊功能处理
	 
//菜单变量
//A
typedef struct LIFT_FUNCCODE_STRUCT
{ 
	 DWORD password[2];
   DWORD liftFuntion;
   DWORD setAddr[8];
   DWORD firstFloor[8];
   DWORD maxFloor[8];
   DWORD enUpOrder[8][2];  //8台梯A门呼梯使能
   DWORD enDnOrder[8][2];
   DWORD enUpOrderB[8][2]; //8台梯B门呼梯使能
   DWORD enDnOrderB[8][2];
   DWORD newFlag[8];
   DWORD liftNotMove;
   DWORD waitFloorTime[6];
	 DWORD waitStartTime[6];
	 DWORD waitEndTime[6];
   BYTE  waitFloor[8][8]; //6个侯梯楼层
   DWORD groupFirstFloor;
   DWORD groupMaxFloor;

   DWORD enUpLift[8][8][2]; //8组电梯登记使能标志
   DWORD enDnLift[8][8][2]; //8组电梯登记使能标志
   DWORD enUpGroup[8][8][2];//8组电梯调度使能标志
   DWORD enDnGroup[8][8][2];//8组电梯调度使能标志

   BYTE  waitFloor8[8]; //8个侯梯楼层
   BYTE  waitFloor7[8]; //7个侯梯楼层
   BYTE  waitFloor6[8]; //6个侯梯楼层
   BYTE  waitFloor5[8]; //5个侯梯楼层
   BYTE  waitFloor4[4]; //4个侯梯楼层
   BYTE  waitFloor3[4]; //3个侯梯楼层
   BYTE  waitFloor2[4]; //2个侯梯楼层

   BYTE  powerSetp[8]; //停电顺序
   BYTE  fireSetp[8];  //消防顺序
   DWORD mode2_WaitFloorTime;  //分散待梯等待时间
	 
	 DWORD enableOrderTime[8][8];     //8台梯时间段设置
	 DWORD enableOrderFlag[8][8][2];  //8台梯8个时间段的禁呼设置
	
	 DWORD floorRunTime;  //电梯单层运行时间
	 DWORD runStopTime;   //电梯停靠时间	
	 DWORD runOrderTime;  //电梯登记优先时间	
//	 BYTE  groupAddress;  //群控板地址
} LIFT_FUNCCODE;	 
Extern_Lift LIFT_FUNCCODE  LP;

#define Password          (LP.password)
#define LiftFuntion       (LP.liftFuntion)
#define SetAddr           (LP.setAddr)
#define FirstFloor        (LP.firstFloor) 
#define MaxFloor          (LP.maxFloor)
#define EnUpOrder         (LP.enUpOrder)
#define EnDnOrder         (LP.enDnOrder)
#define EnUpOrderB        (LP.enUpOrderB)
#define EnDnOrderB        (LP.enDnOrderB)
#define NewFlag           (LP.newFlag)  
#define LiftNotMove       (LP.liftNotMove)
#define WaitFloorTime     (LP.waitFloorTime)
#define WaitStartTime     (LP.waitStartTime)
#define WaitEndTime       (LP.waitEndTime)
#define WaitFloor         (LP.waitFloor)
#define GroupFirstFloor   (LP.groupFirstFloor)
#define GroupMaxFloor     (LP.groupMaxFloor)

#define EnUpLift          (LP.enUpLift)
#define EnDnLift          (LP.enDnLift) 
#define EnUpGroup         (LP.enUpGroup)
#define EnDnGroup         (LP.enDnGroup)

#define WaitFloor8        (LP.waitFloor8) 
#define WaitFloor7        (LP.waitFloor7)
#define WaitFloor6        (LP.waitFloor6)
#define WaitFloor5        (LP.waitFloor5)
#define WaitFloor4        (LP.waitFloor4) 
#define WaitFloor3        (LP.waitFloor3) 
#define WaitFloor2        (LP.waitFloor2)

#define PowerSetp            (LP.powerSetp)
#define FireSetp             (LP.fireSetp)
#define Mode2_WaitFloorTime  (LP.mode2_WaitFloorTime)
//#define GroupAddress         (LP.groupAddress)
#define OrderTimeEnable      (LP.enableOrderTime)
#define OrderFlagEnable      (LP.enableOrderFlag)

#define FloorRunTime      (LP.floorRunTime)  //电梯单层运行时间
#define RunStopTime       (LP.runStopTime)   //电梯停靠时间	
#define RunOrderTime      (LP.runOrderTime)  //电梯登记优先时间	

//无需保存
Extern_Lift DWORD LiftWaitTimer[8];//等待时间
Extern_Lift DWORD LiftWaitFlag;    //回基站标志
Extern_Lift DWORD LiftNotMoveTimer[8];
Extern_Lift DWORD LiftOnline[8];
Extern_Lift DWORD LiftNowFloor[8];
Extern_Lift DWORD LiftABSFloor[8];
Extern_Lift DWORD LiftUpOrder[8][2];  //电梯的登记标志
Extern_Lift DWORD LiftDnOrder[8][2];
Extern_Lift DWORD LiftInOrder[8][2];
//Extern_Lift DWORD LiftUpOrderB[8][2]; //电梯B门的登记标志
//Extern_Lift DWORD LiftDnOrderB[8][2];
//Extern_Lift DWORD LiftInOrderB[8][2];
Extern_Lift DWORD LiftState[8];
Extern_Lift DWORD LiftRunFloor[8];
Extern_Lift DWORD LiftEnDecFloor[8];

Extern_Lift DWORD RunEnUpOrder[8][2]; //8台梯A门呼梯使能运行时使用
Extern_Lift DWORD RunEnDnOrder[8][2];
//Extern_Lift DWORD RunEnUpOrderB[8][2]; //8台梯B门呼梯使能运行时使用
//Extern_Lift DWORD RunEnDnOrderB[8][2];

//空位置
Extern_Lift DWORD GroupUpOrder[8][2], CanUpLed[8][2];//8组群控的呼梯标志
Extern_Lift DWORD GroupDnOrder[8][2], CanDnLed[8][2];
Extern_Lift DWORD UpOrder[8][2], UpOrderTemp[8][2], DoUpOrder[8][2], DoUpOrderOld[8][2]; //电梯的控制标志
Extern_Lift DWORD DnOrder[8][2], DnOrderTemp[8][2], DoDnOrder[8][2], DoDnOrderOld[8][2];
//Extern_Lift DWORD UpOrderSp[8][2], DnOrderSp[8][2];//专用

//Extern_Lift DWORD GroupUpOrderB[8][2], CanUpLedB[8][2]; //8组群控的呼梯标志
//Extern_Lift DWORD GroupDnOrderB[8][2], CanDnLedB[8][2];
//Extern_Lift DWORD UpOrderB[8][2], UpOrderTempB[8][2], DoUpOrderB[8][2], DoUpOrderOldB[8][2]; //电梯的控制标志
//Extern_Lift DWORD DnOrderB[8][2], DnOrderTempB[8][2], DoDnOrderB[8][2], DoDnOrderOldB[8][2];
//Extern_Lift DWORD UpOrderSpB[8][2], DnOrderSpB[8][2]; //专用

Extern_Lift BYTE CanUpLedTimer[8][72]; //发送次数
Extern_Lift BYTE CanDnLedTimer[8][72]; //发送次数
//Extern_Lift BYTE CanUpLedBTimer[8][72];//发送次数
//Extern_Lift BYTE CanDnLedBTimer[8][72];//发送次数

//电梯本层呼梯标志（用于确定同一楼层被按按钮的电梯优先）
//Extern_Lift DWORD LiftUpButton[8][2];
//Extern_Lift DWORD LiftDnButton[8][2];
//Extern_Lift DWORD LiftUpButtonB[8][2];
//Extern_Lift DWORD LiftDnButtonB[8][2];

//到站消号时把该电梯标记一下，用于保持最高最低楼层运算的正确
//当电梯的状态从停止到启动转变，或无方向信号时，清除标记
//Extern_Lift DWORD LiftGetUp[8][2];
//Extern_Lift DWORD LiftGetDn[8][2];
//Extern_Lift DWORD LiftGetUpB[8][2];
//Extern_Lift DWORD LiftGetDnB[8][2];

Extern_Lift volatile DWORD L[8];
Extern_Lift volatile DWORD T[8];
Extern_Lift volatile DWORD COut[8];
Extern_Lift volatile DWORD CIn[8];
Extern_Lift volatile DWORD C[8];
Extern_Lift volatile DWORD N[8];

Extern_Lift volatile DWORD FNum;  //楼层距离
Extern_Lift volatile DWORD FTime; //楼层时间

Extern_Lift volatile DWORD CNum;  //总呼登记数目
Extern_Lift volatile DWORD ONum;  //外呼登记数目
Extern_Lift volatile DWORD INum;  //内选登记数目
Extern_Lift volatile DWORD Do_Floor; //正在处理的楼层
Extern_Lift volatile DWORD Max; //最大楼层
Extern_Lift volatile DWORD Min; //最小楼层
Extern_Lift volatile DWORD BaseFloorGroup; //回基站运行状态
Extern_Lift volatile DWORD IncallNum[8];   //内选数目
Extern_Lift volatile DWORD BackFloor[8];   //回等待楼层

Extern_Lift volatile DWORD PowerFlag[8];   //断电数据标志
Extern_Lift volatile DWORD PowerRunSetp;   //断电处理步骤

Extern_Lift volatile DWORD NoOrderTimer;   //无呼梯定时器
Extern_Lift volatile DWORD StartupTime;    //启动时间
/*****************************************************************************/
/*****************************************************************************/
DWORD BitFun(DWORD *p, DWORD i, DWORD Value);
DWORD BitFunTwo(DWORD *p, DWORD *p1, DWORD i);

void InitData(DWORD x);
void LiftDistance(DWORD Lift, DWORD OrderUD, DWORD OrderFlr);
void LiftInit( void );
void LiftMain( void );
void LiftCancelWait(DWORD wf, DWORD n);
void LiftWait(DWORD n);
void WaitTimeControl(void);
void CheckNoOrder(void);
#endif /* end*/
/*****************************************************************************/



