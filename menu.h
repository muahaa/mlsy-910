#ifndef __Menu_H
#define __Menu_H

#ifdef Menu_Ex
   #define Extern_Menu           //定义变量
#else
   #define Extern_Menu extern    //声明变量
#endif
/*--------------------------------------------------------------------------------------------------*/
#define ReadKeyExit()  (FIO2PIN&(1<<13))
#define ReadKeyDn()    (FIO2PIN&(1<<12))
#define ReadKeyUp()    (FIO2PIN&(1<<11))
#define ReadKeyEnter() (FIO2PIN&(1<<10))

Extern_Menu DWORD  Menu100msDelay;
Extern_Menu DWORD  NowKeyValue;  //当前按钮值
Extern_Menu DWORD  OldKeyValue;  //之前按钮值
Extern_Menu DWORD  Modify;       //参数修改
Extern_Menu DWORD  ModifyID;     //参数数据位
Extern_Menu DWORD  Modified;     //参数已经修改标志
Extern_Menu DWORD  ModifyData;   //临时修改数据
Extern_Menu DWORD  BitDispData[12];	 //临时位数据
Extern_Menu DWORD  CurMenuIndex; //当前状态索引号
Extern_Menu DWORD  OldMenuIndex; //当前状态索引号
Extern_Menu DWORD  PasswordOK[3];//密码有效标志
Extern_Menu DWORD  DispOn500ms;  //闪嗦时间
Extern_Menu DWORD  PasswordTime; //密码有效时间
Extern_Menu DWORD  BackLightTime;//背光亮时间 

Extern_Menu DWORD  NowKeyTimer;  //当前按钮值
Extern_Menu DWORD  UpKeyTime;    //
Extern_Menu DWORD  DnKeyTime;    //
Extern_Menu DWORD  KeyTime;      //背光亮时间

void copy(DWORD *P1, const BYTE *P2 , DWORD N);
void Menu_Ini(void);
void MenuMain(void);
void DispState(void);
void DispState2(void);
void MenuChar(DWORD *MData, DWORD *MRam, DWORD Max, DWORD Min, const BYTE *PData, DWORD M);
void GroupMenuData(DWORD *MData, DWORD *MRam, DWORD Ram_N, DWORD Pre, DWORD Max, DWORD Min, DWORD Gn);
void GroupMenuData_Byte(BYTE *MData, DWORD *MRam, DWORD Ram_N, DWORD Pre, DWORD Max, DWORD Min, DWORD Gn);
void ChangeCharData(DWORD data, DWORD *P, DWORD N);
DWORD ChangeLongData(DWORD *P, DWORD N);
#define D_Hz   1
#define D_KHz  2
#define D_A	   3
#define D_V	   4
#define D_KW   5
#define D_H	   6
#define D_Min  7
#define D_S	   8
#define D_mS   9
#define D_BFH  10
#define D_MS   11
#define D_M	   12
#define D_mm   13
#define D_Du   14
#define D_Jiao 15
#define D_RPM  16
#define D_JSD  17
#define D_PS   18
#define D_PPR  19
void DispData(DWORD *P, DWORD *Ram_P, DWORD Ram_N);
void MenuData(DWORD *MData, DWORD *MRam, DWORD Ram_N, DWORD Pre, DWORD Max, DWORD Min);
void DispData2For3(DWORD data1, DWORD data2, DWORD data3, DWORD *Ram_P, DWORD DT);
void MenuData2For3(volatile DWORD *MData1, volatile DWORD *MData2, volatile DWORD *MData3, DWORD *MRam, DWORD DT);
void MenuSetTime( DWORD *T);
void MenuEnOrder(DWORD *MData, DWORD *MRam);
//void DispBitData(DWORD *PBit, DWORD n, const BYTE *PTab);
//void MenuBitData(DWORD *BitData, DWORD n, const BYTE *PTab, DWORD Max_n);
void DispBit32(DWORD *BitData);
void MenuBit32(DWORD *BitData);
void DispCallData(DWORD *BitData);
void MenuCallBit(DWORD *BitData);
void DispCallDataTwo(DWORD *BitData, DWORD *BitDataB);
void MenuCallBitTwo(DWORD *BitData, DWORD *BitDataB);
void DispUpDnData(DWORD *BitData1, DWORD *BitData2);
void FunPassword(DWORD i); 
void FunFactoryPara(void);

//Extern_Menu DWORD  Send_Setp;    //
//void Window(void);
/*--------------------------------------------------------------------------------------------------*/
#endif

