/*****************************************************************************
  全局变量
******************************************************************************/
#ifndef __MAIN_H 
#define __MAIN_H
/*****************************************************************************/
#ifdef MAIN_Ex
   #define Extern_MAIN         //定义变量
#else
   #define Extern_MAIN extern  //声明变量
#endif
/*****************************************************************************/
int main (void);
void Check_NoMove(void);
void Check_DelInOrder(void);	 
void Check_Online(void);
void SetStd(void);
void TimeSetEnOrder(void);
void CheckNoLiftEnable(void);	 
#endif /* end*/
/*****************************************************************************/



