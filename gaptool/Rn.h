#ifndef Rn_H
#define Rn_H

/*
零环{0}是整环【一种约定而已】但不是域，是C_1的强抽象；1阶环{0}只包含一个元0，加法和乘法规定为0+0=0,0*0=0
规范：GAP4ID编号小的声明在前
*/

extern int g_M2Add[2][2];
extern int g_M2Mul[2][2];

extern int g_F2Add[2][2];
extern int g_F2Mul[2][2];

extern int g_M3Add[3][3];
extern int g_M3Mul[3][3];

extern int g_F3Add[3][3];
extern int g_F3Mul[3][3];

//4阶循环环有T(4)=|{1,2,4}|=3个:A、B、C

//R4_C=ring 4.Nu.1
extern int g_M4Add[4][4];
extern int g_M4Mul[4][4];

//R4_B=ring 4.Nu.2=Z/8Z的4阶子环{0,2,4,6},运算为模8加与模8乘
extern int g_B4Add[4][4];
extern int g_B4Mul[4][4];

//R4_A=ring 4.u.1的运算为模4加与模4乘
extern int g_Z4Add[4][4];
extern int g_Z4Mul[4][4];

//R4_J=ring 22.Nu.1=M_2×M_2
extern int g_M2M2Add[4][4];
extern int g_M2M2Mul[4][4];

//R4_5=ring 22.Nu.2
extern int g_R4_5_Add[4][4];
extern int g_R4_5_Mul[4][4];

//R4_6=ring 22.Nu.3=M_2×F_2
extern int g_M2F2Add[4][4];
extern int g_M2F2Mul[4][4];

//R4_Y=ring 22.NC.2={0={{0,0},{0,0}},1={{1,0},{0,0}},2={{1,1},{0,0}},3={{0,1},{0,0}}}，是16阶环M_2(Z_2)的一个子环
extern int g_Y4Add[4][4];
extern int g_Y4Mul[4][4];

//R4_P=ring 22.NC.1={0={{0,0},{0,0}},1={{1,0},{0,0}},2={{1,0},{1,0}},3={{0,0},{1,0}}}，是16阶环M_2(Z_2)的一个子环
extern int g_P4Add[4][4];
extern int g_P4Mul[4][4];

//R4_X=ring 22.u.2=F_2+uF_2={0={{0,0},{0,0}},1={{1,0},{0,1}},2={{0,1},{1,0}},3={{1,1},{1,1}}}
extern int g_X4Add[4][4];
extern int g_X4Mul[4][4];

//R4_D=ring 22.u.1={0=(0,0),3=(1,0),2=(0,1),1=(1,1)}={0={{0,0},{0,0}},1={{1,0},{0,1}},2={{0,0},{0,1}},3={{1,0},{0,0}}}
extern int g_F2F2Add[4][4];
extern int g_F2F2Mul[4][4];

//R4_K=ring 22.u.3={0={{0,0},{0,0}},1={{1,0},{0,1}},2={{1,1},{1,0}},3={{0,1},{1,1}}}
extern int g_F4Add[4][4];
extern int g_F4Mul[4][4];

#endif

