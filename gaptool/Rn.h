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

//M_6=M_3×M_2=ring 6.Nu.1即6阶零乘环，同构于Z/36Z的6阶子环{0,6,12,18,24,30},运算为模36加与模36乘
extern int g_M6Add[6][6];
extern int g_M6Mul[6][6];

//F_3×M_2=ring 6.Nu.2同构于Z/12Z的6阶子环{0,2,4,6,8,10},运算为模12加与模12乘，非零零因子个数：5（[2],[4],[6],[8],[10]）
extern int g_F3M2Add[6][6];
extern int g_F3M2Mul[6][6];

//M_3×F_2=ring 6.Nu.3同构于Z/18Z的6阶子环{0,3,6,9,12,15},运算为模18加与模18乘,非零零因子个数：5（[3],[6],[9],[12],[15]）
extern int g_M3F2Add[6][6];
extern int g_M3F2Mul[6][6];

//Z/6Z=F_3×F_2=ring 6.u.1,运算为模6加与模6乘,非零零因子个数：3（[[2],[3],[4]]）
extern int g_Z6Add[6][6];
extern int g_Z6Mul[6][6];

/*
8阶环分类定理：
n0=8的8阶环有4个：R8_1~R8_4
n0=4的8阶环有20个：R8_5~R8_24
n0=2的8阶环有28个：R8_25~R8_52
There are 52 (if p=2) or 53 (if p>2) rings of size p3, as follows:
Four rings over Zp3.
Twenty (or twenty-one if p>2) rings over Zp2+Zp.
Twenty-eight rings over Zp+Zp+Zp.
*/
//R8_1=M_8=Z_64的一个8阶子环={[0],[8],[16],[24],[32],[40],[48],[56]},运算为模64加与模64乘,零乘环
extern int g_M8Add[8][8];
extern int g_M8Mul[8][8];

//R8_2=Z_16的一个8阶子环={[0],[2],[4],[6],[8],[10],[12],[14]},运算为模16加与模16乘,非零零因子个数：7（[2],[4],[6],[8],[10],[12],[14]）
//g_R8_2Add同g_Z8Add
extern int g_R8_2Mul[8][8];

//R8_3=Z/8Z,运算为模8加与模8乘,非零零因子个数：3（[[2],[4],[6]]）
extern int g_Z8Add[8][8];
extern int g_Z8Mul[8][8];

//R8_4=Z_32的一个8阶子环={[0],[4],[8],[12],[16],[20],[24],[28]},运算为模32加与模32乘，非零零因子个数：7（[4],[8],[12],[16],[20],[24],[28]）
//g_R8_4Add同g_Z8Add
extern int g_R8_4Mul[8][8];

//R8_6
//g_R8_6Add同g_R8_24Add
extern int g_R8_6Mul[8][8];

//R8_8
//g_R8_8Add同g_R8_24Add
extern int g_R8_8Mul[8][8];

//R8_9
//g_R8_9Add同g_R8_24Add
extern int g_R8_9Mul[8][8];

//R8_10
//g_R8_10Add同g_R8_24Add
extern int g_R8_10Mul[8][8];

//R8_11
//g_R8_11Add同g_R8_24Add
extern int g_R8_11Mul[8][8];

//R8_12
//g_R8_12Add同g_R8_24Add
extern int g_R8_12Mul[8][8];

//R8_14=Z/4Z×F_2
extern int g_Z4F2Add[8][8];
extern int g_Z4F2Mul[8][8];

//R8_15
//g_R8_15Add同g_R8_24Add
extern int g_R8_15Mul[8][8];

//R8_18
//g_R8_18Add同g_R8_24Add
extern int g_R8_18Mul[8][8];

//R8_19
extern int g_R8_19Add[8][8];
extern int g_R8_19Mul[8][8];

//R8_20
//g_R8_20Add同g_R8_24Add
extern int g_R8_20Mul[8][8];


//R8_21
extern int g_R8_21Add[8][8];
extern int g_R8_21Mul[8][8];

//R8_22
//g_R8_22Add同g_R8_24Add
extern int g_R8_22Mul[8][8];

//R8_23
//g_R8_23Add同g_R8_24Add
extern int g_R8_23Mul[8][8];

//R8_24
extern int g_R8_24Add[8][8];
extern int g_R8_24Mul[8][8];

//R8_28
extern int g_R8_28Add[8][8];
extern int g_R8_28Mul[8][8];

//R8_30
extern int g_R8_30Add[8][8];
extern int g_R8_30Mul[8][8];

//R8_31
//g_R8_31Add同g_F8Add;
extern int g_R8_31Mul[8][8];

//R8_32
//g_R8_32Add同g_F8Add;
extern int g_R8_32Mul[8][8];

//R8_33
extern int g_R8_33Add[8][8];
extern int g_R8_33Mul[8][8];

//R8_36
//g_R8_36Add同g_F8Add;
extern int g_R8_36Mul[8][8];

//R8_37
//g_R8_37Add同g_F8Add;
extern int g_R8_37Mul[8][8];

//R8_39
//g_R8_39Add同g_F8Add;
extern int g_R8_39Mul[8][8];

//R8_43
extern int g_R8_43Add[8][8];
extern int g_R8_43Mul[8][8];

//R8_44
extern int g_R8_44Add[8][8];
extern int g_R8_44Mul[8][8];

//R8_45={{{a,0,0},{b,a,0},{c,0,a}}|a,b,c∈Z/2Z}<=M_3(Z/2Z)=邢丹丹论文中的8阶环R_1，8阶交换幺环，非零零因子个数：3（2,4,6）
//g_R8_45Add同g_F8Add;
extern int g_R8_45Mul[8][8];

//R8_46=F_2[x]/(x^3)=F_2[x]/(x^3+x^2+x+1)
extern int g_R8_46Add[8][8];
extern int g_R8_46Mul[8][8];

//R8_47
extern int g_R8_47Add[8][8];
extern int g_R8_47Mul[8][8];

//R8_49={{{a,c},{0,b}}|a,b,c∈Z/2Z}<=M_2(Z/2Z),8阶非交换幺环,非零零因子个数：5（1,2,4,5,6）
//g_R8_49Add同g_F8Add;
extern int g_R8_49Mul[8][8];

//R8_51
extern int g_F4F2Add[8][8];
extern int g_F4F2Mul[8][8];

//R8_52
extern int g_F8Add[8][8];
extern int g_F8Mul[8][8];

extern int* g_C2C4Add;
extern int g_R8_C2C2C2_28_2Mul[8][8];
extern int g_R8_C2C2C2_28_3Mul[8][8];

extern int g_R9_5Add[9][9];
extern int g_R9_5Mul[9][9];

#endif

