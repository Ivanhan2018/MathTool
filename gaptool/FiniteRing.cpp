#define R32_DEL
#include<ctime>
#include<fstream>
#include<set>
#include"DecompositionRing.h"
#include"Rn.h"
#include"M2r.h"
#include"Mnr.h"
#include"PolynomialRing.h"
#include"quotientRing.h"

#ifndef Rn_H
int g_M2Add[2][2]={
	 {0,1},
	 {1,0}
};

int g_M2Mul[2][2]={
	 {0,0},
	 {0,0}
};

int g_F2Add[2][2]={
	 {0,1},
	 {1,0}
};
int g_F2Mul[2][2]={
	 {0,0},
	 {0,1}
};

int g_M3Add[3][3]={
	 {0,1,2},
	 {1,2,0},
	 {2,0,1}
};
int g_M3Mul[3][3]={
	 {0,0,0},
	 {0,0,0},
	 {0,0,0}
};

int g_F3Add[3][3]={
	 {0,1,2},
	 {1,2,0},
	 {2,0,1}
};

int g_F3Mul[3][3]={
	 {0,0,0},
	 {0,1,2},
	 {0,2,1}
};

int g_F3M2Add[6][6]={
	{0,1,2,3,4,5},
	{1,0,3,2,5,4},
	{2,3,4,5,0,1},
	{3,2,5,4,1,0},
	{4,5,0,1,2,3},
	{5,4,1,0,3,2},
};
int g_F3M2Mul[6][6]={
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,2,2,4,4},
	{0,0,2,2,4,4},
	{0,0,4,4,2,2},
	{0,0,4,4,2,2},
};

// 环R8_3的结构不变量n0,bA,bO,n1,n2,n4,n5,n6,n7,n8=8,1,1,4,2,1,3,20,3,8,[ 1, 2, 4, 8 ],[ 1, 2, 4, 4, 8, 8, 8, 8 ]
// R8_3:N0n0bAbOn1n2n4n5n6n7n8S1N2=[1,1,2,4],8,1,1,4,2,1,3,20,3,8,[1,1,2,4],[[2,8,4],[4,4,4],[4,8,8],[8,2,4],[8,4,8],[8,8,16]]
int g_Z8Mul[8][8]={
	{0,0,0,0,0,0,0,0},
	{0,1,2,3,4,5,6,7},
	{0,2,4,6,0,2,4,6},
	{0,3,6,1,4,7,2,5},
	{0,4,0,4,0,4,0,4},
	{0,5,2,7,4,1,6,3},
	{0,6,4,2,0,6,4,2},
	{0,7,6,5,4,3,2,1}
};

//C_4×C_2——>Z/4Z×F_2
//加法表数据有误
int g_Z4F2Add[8][8]={
	{0,1,2,3,4,5,6,7},
	{1,0,3,2,5,4,7,6},
	{2,3,4,5,6,7,0,1},
	{3,2,5,4,7,6,1,0},
	{4,5,6,7,2,3,0,1},
	{5,4,7,6,3,2,1,0},
	{6,7,0,1,2,3,4,5},
	{7,6,1,0,3,2,5,4},
};

// 环R8_14的结构不变量n0,bA,bO,n1,n2,n4,n5,n6,n7,n8=4,1,1,6,4,1,1,24,5,8,[ 1, 2, 2, 4, 4, 8 ],[ 1, 2, 2, 4, 4, 4, 4, 4 ]
// R8_14:N0n0bAbOn1n2n4n5n6n7n8S1N2=[1,3,4,0],4,1,1,6,4,1,1,24,5,8,[1,2,5,0],[[2,2,8],[2,4,10],[4,2,10],[4,4,12]]
int g_Z4F2Mul[8][8]={
	{0,0,0,0,0,0,0,0},
	{0,1,0,1,0,1,0,1},
	{0,0,2,2,4,4,6,6},
	{0,1,2,3,4,5,6,7},
	{0,0,4,4,0,0,4,4},
	{0,1,4,5,0,1,4,5},
	{0,0,6,6,4,4,2,2},
	{0,1,6,7,4,5,2,3},
};

//R8_52:N0n0bAbOn1n2n4n5n6n7n8S1N2=[1,7,0,0],2,1,1,1,2,0,0,15,0,8,[1,1,0,6],[[2,2,49]]
int g_F8Add[8][8]={
	{0,   1,   2,   3,   4,   5,   6,   7},  
	{1,   0,   4,   7,   2,   6,   5,   3},  
	{2,   4,   0,   5,   1,   3,   7,   6},  
	{3,   7,   5,   0,   6,   2,   4,   1},  
	{4,   2,   1,   6,   0,   7,   3,   5},  
	{5,   6,   3,   2,   7,   0,   1,   4},  
	{6,   5,   7,   4,   3,   1,   0,   2},  
	{7,   3,   6,   1,   5,   4,   2,   0}
};

int g_F8Mul[8][8]={ 
	{0,   0,   0,   0,   0,   0,   0,   0},  
	{0,   1,   2,   3,   4,   5,   6,   7}, 
	{0,   2,   3,   4,   5,   6,   7,   1},  
	{0,   3,   4,   5,   6,   7,   1,   2},  
	{0,   4,   5,   6,   7,   1,   2,   3},  
	{0,   5,   6,   7,   1,   2,   3,   4},  
	{0,   6,   7,   1,   2,   3,   4,   5},  
	{0,   7,   1,   2,   3,   4,   5,   6}
};
#endif

int g_M3M2_M6Add[6][6]={
	 {0,1,2,3,4,5},
	 {1,2,3,4,5,0},
	 {2,3,4,5,0,1},
	 {3,4,5,0,1,2},
	 {4,5,0,1,2,3},
	 {5,0,1,2,3,4}
}; 

int g_M3M2_M6Mul[6][6]={
	 {0,0,0,0,0,0},
	 {0,0,0,0,0,0},
	 {0,0,0,0,0,0},
	 {0,0,0,0,0,0},
	 {0,0,0,0,0,0},
	 {0,0,0,0,0,0}
};

//R6_2
int g_F3M2_R2Add[6][6]={
	 {0,1,2,3,4,5},
	 {1,2,3,4,5,0},
	 {2,3,4,5,0,1},
	 {3,4,5,0,1,2},
	 {4,5,0,1,2,3},
	 {5,0,1,2,3,4}
};

int g_F3M2_R2Mul[6][6]={
	 {0,0,0,0,0,0},
	 {0,2,4,0,2,4},
	 {0,4,2,0,4,2},
	 {0,0,0,0,0,0},
	 {0,2,4,0,2,4},
	 {0,4,2,0,4,2}
};

//R6_4
int g_F3F2_Z6Add[6][6]={
	 {0,1,2,3,4,5},
	 {1,2,3,4,5,0},
	 {2,3,4,5,0,1},
	 {3,4,5,0,1,2},
	 {4,5,0,1,2,3},
	 {5,0,1,2,3,4}
};

int g_F3F2_Z6Mul[6][6]={
	 {0,0,0,0,0,0},
	 {0,1,2,3,4,5},
	 {0,2,4,0,2,4},
	 {0,3,0,3,0,3},
	 {0,4,2,0,4,2},
	 {0,5,4,3,2,1}
};

//R6_4
int g_F3F2Add[6][6]={
	{0,1,2,3,4,5},
	{1,0,3,2,5,4},
	{2,3,4,5,0,1},
	{3,2,5,4,1,0},
	{4,5,0,1,2,3},
	{5,4,1,0,3,2},
};

int g_F3F2Mul[6][6]={
	{0,0,0,0,0,0},
	{0,1,0,1,0,1},
	{0,0,2,2,4,4},
	{0,1,2,3,4,5},
	{0,0,4,4,2,2},
	{0,1,4,5,2,3},
};

int g_C8Mul[8][8]={
	{0, 1, 2, 3, 4, 5, 6, 7},
	{1, 0, 3, 2, 5, 4, 7, 6},
	{2, 3, 1, 0, 6, 7, 5, 4},
	{3, 2, 0, 1, 7, 6, 4, 5},
	{4, 5, 6, 7, 2, 3, 1, 0},
	{5, 4, 7, 6, 3, 2, 0, 1},
	{6, 7, 5, 4, 1, 0, 3, 2},
	{7, 6, 4, 5, 0, 1, 2, 3}
};

int g_C8Mul_2[8][8]={
	{0,1,2,3,4,5,6,7},
	{1,2,3,4,5,6,7,0},
	{2,3,4,5,6,7,0,1},
	{3,4,5,6,7,0,1,2},
	{4,5,6,7,0,1,2,3},
	{5,6,7,0,1,2,3,4},
	{6,7,0,1,2,3,4,5},
	{7,0,1,2,3,4,5,6}
};

//int* g_C8Add=&g_C8Mul[0][0];//Error,与乘法表g_Z8Mul不相容
int* g_C8Add=&g_C8Mul_2[0][0];

// 环R8_2的结构不变量n0,bA,bO,n1,n2,n4,n5,n6,n7,n8=8,1,0,8,1,3,3,32,7,8,[ 1, 2, 4, 8 ],[ 1, 2, 4, 4, 8, 8, 8, 8 ]
// R8_2:N0n0bAbOn1n2n4n5n6n7n8S1N2=[1,1,2,4],8,1,0,8,1,3,3,32,7,8,[1,1,2,4],[[4,8,8],[8,4,8],[8,8,16]]
int g_C8R2Mul[8][8]={
	{0,0,0,0,0,0,0,0},
	{0,2,4,6,0,2,4,6},
	{0,4,0,4,0,4,0,4},
	{0,6,4,2,0,6,4,2},
	{0,0,0,0,0,0,0,0},
	{0,2,4,6,0,2,4,6},
	{0,4,0,4,0,4,0,4},
	{0,6,4,2,0,6,4,2}
};

// 环R8_4的结构不变量n0,bA,bO,n1,n2,n4,n5,n6,n7,n8=8,1,0,8,1,3,7,48,7,8,[ 1, 2, 4, 8 ],[ 1, 2, 4, 4, 8, 8, 8, 8 ]
// R8_4:N0n0bAbOn1n2n4n5n6n7n8S1N2=[1,1,2,4],8,1,0,8,1,3,7,48,7,8,[1,1,2,4],[[8,8,16]]
int g_C8R4Mul[8][8]={
	{0,0,0,0,0,0,0,0},
	{0,4,0,4,0,4,0,4},
	{0,0,0,0,0,0,0,0},
	{0,4,0,4,0,4,0,4},
	{0,0,0,0,0,0,0,0},
	{0,4,0,4,0,4,0,4},
	{0,0,0,0,0,0,0,0},
	{0,4,0,4,0,4,0,4},
};


int g_C2C4Mul[8][8]={
	{0, 1, 2, 3, 4, 5, 6, 7},
	{1, 0, 3, 2, 5, 4, 7, 6},
	{2, 3, 0, 1, 6, 7, 4, 5},
	{3, 2, 1, 0, 7, 6, 5, 4},
	{4, 5, 6, 7, 1, 0, 3, 2},
	{5, 4, 7, 6, 0, 1, 2, 3},
	{6, 7, 4, 5, 3, 2, 1, 0},
	{7, 6, 5, 4, 2, 3, 0, 1}
};

int g_C2C4Mul_2[8][8]={
	{0,1,2,3,4,5,6,7},
	{1,4,7,2,5,0,3,6},
	{2,7,4,1,6,3,0,5},
	{3,2,1,0,7,6,5,4},
	{4,5,6,7,0,1,2,3},
	{5,0,3,6,1,4,7,2},
	{6,3,0,5,2,7,4,1},
	{7,6,5,4,3,2,1,0}
};

//int* g_C2C4Add=&g_C2C4Mul[0][0];//Error,与乘法表g_Z4F2Mul不相容
int* g_C2C4Add=&g_C2C4Mul_2[0][0];


// C_2×C_2×C_2——>邢丹丹论文中的8阶环R_1
//加法表数据有误
int g_R8_C2C2C2_28_R1_Add[8][8]={
	{0,1,2,3,4,5,6,7},
	{1,0,3,2,5,4,7,6},
	{2,3,0,1,6,7,4,5},
	{3,2,0,1,7,6,5,4},
	{4,5,6,7,0,1,2,3},
	{5,4,7,6,1,0,3,2},
	{6,7,4,5,2,3,0,1},
	{7,6,5,4,3,2,1,0}
};

// 乘法表数据有误
int g_R8_C2C2C2_28_1Mul[8][8]={
	{0,0,0,0,0,0,0,0},
	{0,1,0,0,0,1,0,1},
	{0,0,2,2,0,0,2,2},
	{0,0,2,3,0,1,2,3},
	{0,0,0,0,4,4,4,4},
	{0,1,0,1,4,5,4,5},
	{0,0,2,2,4,4,6,6},
	{0,1,2,3,4,5,6,7}
};

//R={{{a,0,0},{b,a,0},{c,0,a}}|a,b,c∈Z/2Z}<=M_3(Z/2Z)是8阶交换幺环
//环R8_45的结构不变量n0,bA,bO,n1,n2,n4,n5,n6,n7,n8=2,1,1,4,2,3,3,24,3,8,[ 1, 2, 2, 2, 4, 8 ],[ 1, 2, 2, 2, 2, 4, 4, 4 ]
//R8_45:N0n0bAbOn1n2n4n5n6n7n8S1N2=[1,7,0,0],2,1,1,4,2,3,3,24,3,8,[1,4,3,0],[[2,2,40]]
int g_R8_C2C2C2_28_R1_Mul[8][8]={
	 {0,0,0,0,0,0,0,0},
	 {0,1,2,3,4,5,6,7},
	 {0,2,0,2,0,2,0,2},
	 {0,3,2,1,4,7,6,5},
	 {0,4,0,4,0,4,0,4},
	 {0,5,2,7,4,1,6,3},
	 {0,6,0,6,0,6,0,6},
	 {0,7,2,5,4,3,6,1}
};

int g_R8_C2C2C2_28_2Mul[8][8]={
	{0,0,0,0,0,0,0,0},
	{0,1,2,3,4,5,6,7},
	{0,2,0,2,0,2,0,2},
	{0,3,2,1,4,7,6,5},
	{0,4,0,4,0,4,0,4},
	{0,5,2,7,4,1,6,5},
	{0,6,0,6,0,6,0,6},
	{0,7,2,5,4,5,6,1}
};

int g_C2C2C2Mul[8][8]={
	{0, 1, 2, 3, 4, 5, 6, 7},
	{1, 0, 3, 2, 5, 4, 7, 6},
	{2, 3, 0, 1, 6, 7, 4, 5},
	{3, 2, 1, 0, 7, 6, 5, 4},
	{4, 5, 6, 7, 0, 1, 2, 3},
	{5, 4, 7, 6, 1, 0, 3, 2},
	{6, 7, 4, 5, 2, 3, 0, 1},
	{7, 6, 5, 4, 3, 2, 1, 0}
};
int* g_C2C2C2Add=&g_C2C2C2Mul[0][0];

// C_2×C_2×C_2——>邢丹丹论文中的8阶环R_3
// 乘法表数据有误
int g_R8_C2C2C2_28_R3_Mul[8][8]={
	 {0,0,0,0,0,0,0,0},
	 {0,1,2,3,4,5,6,7},
	 {0,2,0,2,0,2,0,2},
	 {0,3,2,1,4,7,6,5},
	 {0,4,0,4,3,7,3,7},//这一行后4个元素变了
	 {0,5,2,7,7,2,5,0},//这一行后4个元素变了
	 {0,6,0,6,3,5,3,5},//这一行后4个元素变了
	 {0,7,2,5,7,0,5,2}//这一行后4个元素变了
};

// C_2×C_2×C_2——>邢丹丹论文中的8阶环R_2
// 乘法表数据有误
int g_R8_C2C2C2_28_R2_Mul[8][8]={
	 {0,0,0,0,0,0,0,0},
	 {0,1,2,3,4,5,6,7},
	 {0,2,0,2,0,2,0,2},
	 {0,3,2,1,4,7,6,5},
	 {0,4,0,4,2,6,2,6},//这一行后4个元素变了
	 {0,5,2,7,6,3,4,1},//这一行后4个元素变了
	 {0,6,0,6,2,4,2,4},//这一行后4个元素变了
	 {0,7,2,5,4,1,4,3}//这一行后3个元素变了
};

//环R8_49的结构不变量n0,bA,bO,n1,n2,n4,n5,n6,n7,n8=2,0,1,6,6,1,1,26,5,2,[ 1, 2, 4, 4, 8 ],[ 1, 2, 2, 2, 2, 2, 2, 4 ]
//R8_49:N0n0bAbOn1n2n4n5n6n7n8S1N2=[1,7,0,0],2,0,1,6,6,1,1,26,5,2,[1,6,1,0],[[2,2,38]]
int g_R8_C2C2C2_28_3Mul[8][8]={
	{0,0,0,0,0,0,0,0},
	{0,1,0,1,4,5,4,5},
	{0,0,2,2,0,0,2,2},
	{0,1,2,3,4,5,6,7},
	{0,0,4,4,0,0,4,4},
	{0,1,4,5,4,5,0,1},
	{0,0,6,6,0,0,6,6},
	{0,1,6,7,4,5,2,3},
};

//C_2×C_2×C_2——>F_4×F_2
//R8_51:N0n0bAbOn1n2n4n5n6n7n8S1N2=[1,7,0,0],2,1,1,5,4,0,0,21,4,8,[1,3,2,2],[[2,2,43]]
int g_F4F2Add[8][8]={
	{0,1,2,3,4,5,6,7},
	{1,0,3,2,5,4,7,6},
	{2,3,0,1,6,7,4,5},
	{3,2,1,0,7,6,5,4},
	{4,5,6,7,0,1,2,3},
	{5,4,7,6,1,0,3,2},
	{6,7,4,5,2,3,0,1},
	{7,6,5,4,3,2,1,0},
};

int g_F4F2Mul[8][8]={
	{0,0,0,0,0,0,0,0},
	{0,1,0,1,0,1,0,1},
	{0,0,2,2,4,4,6,6},
	{0,1,2,3,4,5,6,7},
	{0,0,4,4,6,6,2,2},
	{0,1,4,5,6,7,2,3},
	{0,0,6,6,2,2,4,4},
	{0,1,6,7,2,3,4,5},
};


// 直接从凯莱表构造一个有限环
struct FiniteRing:public IRing
{
public:
	//  静态函数  
public:
	// 实现抽象基类的方法
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// 构造函数
	FiniteRing(int n,int* a,int* m,int delt);  
	// 成员变量  
	int m_n; 
	int* m_Add;
	int* m_Mul; 
	int m_delt; 
};

void FiniteRing::printTable()
{
	int ID=IdRing(this);
	string str=calcRingInvariant(this);
	printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",size(),ID,str.c_str());
	//printRing(this);	
}

int FiniteRing::add(int a,int b)
{
	int c=*(m_Add+a*m_n+b);
	return c-m_delt;
}

int FiniteRing::mul(int a,int b)
{
	int c=*(m_Mul+a*m_n+b);
	return c-m_delt;
}

int FiniteRing::size()
{
	return m_n;
}

FiniteRing::FiniteRing(int n,int* a,int* m,int delt)
{
	m_n=n;
	m_Add=a;
    m_Mul=m;
    m_delt=delt;	
}

IRing* newR2(int i)
{
	if(i==1)
	{
		ZmodnZ* r=new ZmodnZ(2,4);
		return r;
	}
	if(i==2)
	{
		ZmodnZ* r=new ZmodnZ(1,2);
		return r;
	}
	return NULL;
}

IRing* newR8(int i)
{
	if(i==1)
	{
		ZmodnZ* r=new ZmodnZ(8,64);
		return r;
	}
	if(i==2)
	{
		ZmodnZ* r=new ZmodnZ(2,16);
		return r;
	}	
	if(i==3)
	{
		ZmodnZ* r=new ZmodnZ(1,8);
		return r;
	}
	if(i==4)
	{
		ZmodnZ* r=new ZmodnZ(4,32);
		return r;
	}
	if(i==6)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_6Mul[0][0],0);
		return r;
	}
	if(i==8)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_8Mul[0][0],0);
		return r;
	}
	if(i==9)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_9Mul[0][0],0);
		return r;
	}
	if(i==10)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_10Mul[0][0],0);
		return r;
	}
	if(i==11)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_11Mul[0][0],0);
		return r;
	}
	if(i==12)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_12Mul[0][0],0);
		return r;
	}
	if(i==13)
	{
		Mnr* r=new Mnr();
		r->initR8(13);
		return r;
	}
	if(i==14)
	{
		FiniteRing* r=new FiniteRing(8,g_C2C4Add,&g_Z4F2Mul[0][0],0);
		return r;
	}
	if(i==15)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_15Mul[0][0],0);
		return r;
	}
	if(i==16)
	{
		ZmodnZ* r4=new ZmodnZ(2,8);
		ZmodnZ* r2=new ZmodnZ(2,4);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;
		return r;
	}
	if(i==18)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_18Mul[0][0],0);
		return r;
	}
	if(i==19)
	{
		M2r* r=new M2r();
		MATRIXi A(2,vector<int>(2,0));
		MATRIXi B(2,vector<int>(2,0));
		A[0][0]=2;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=2;
		B[0][0]=1;
		B[0][1]=1;
		B[1][0]=1;
		B[1][1]=1;
		vector<MATRIXi> gen;
		gen.push_back(A);
		gen.push_back(B);
		r->m_r=new ZmodnZ(1,4);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen);   
		return r;
	}
	if(i==20)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_20Mul[0][0],0);
		return r;
	}
	if(i==21)
	{
		M2r* r=new M2r();
		MATRIXi A(2,vector<int>(2,0));
		A[0][0]=1;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=3;
		vector<MATRIXi> gen;
		gen.push_back(A);
		r->m_r=new ZmodnZ(1,4);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen);   
		return r;
	}
	if(i==22)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_22Mul[0][0],0);
		return r;
	}
	if(i==23)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_23Mul[0][0],0);
		return r;
	}
	if(i==24)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_24Mul[0][0],0);
		return r;
	}
	if(i==28)
	{
		Mnr* r=new Mnr();
		MATRIXi8 A(4,vector<unsigned char>(4,0));
		MATRIXi8 B(4,vector<unsigned char>(4,0));
		A[0][0]=1;
		A[0][1]=0;
		A[0][2]=1;
		A[0][3]=0;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=0;
		A[1][3]=1; 
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=0;
		A[2][3]=1;
		A[3][0]=1;
		A[3][1]=0;
		A[3][2]=1;
		A[3][3]=0;
		B[0][0]=0;
		B[0][1]=1;
		B[0][2]=0;
		B[0][3]=1;
		B[1][0]=0;
		B[1][1]=1;
		B[1][2]=0;
		B[1][3]=1; 
		B[2][0]=0;
		B[2][1]=1;
		B[2][2]=0;
		B[2][3]=1;
		B[3][0]=0;
		B[3][1]=1;
		B[3][2]=0;
		B[3][3]=1;
		vector<MATRIXi8> gen;
		gen.push_back(A);
		gen.push_back(B);
		r->m_r=new ZmodnZ(1,2);
		r->m_flag=1;
		r->m_n=4;
		r->m_Set=Mnr::FR(r->m_r,gen);   
		return r;
	}
	if(i==30)
	{
		Mnr* r=new Mnr();
		MATRIXi8 A(4,vector<unsigned char>(4,0));
		MATRIXi8 B(4,vector<unsigned char>(4,0));
		MATRIXi8 C(4,vector<unsigned char>(4,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[0][3]=0;
		A[1][0]=1;
		A[1][1]=1;
		A[1][2]=0;
		A[1][3]=0; 
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=1;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=1;
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[0][3]=0;
		B[1][0]=1;
		B[1][1]=1;
		B[1][2]=0;
		B[1][3]=0; 
		B[2][0]=1;
		B[2][1]=0;
		B[2][2]=1;
		B[2][3]=0;
		B[3][0]=0;
		B[3][1]=0;
		B[3][2]=0;
		B[3][3]=1;
		C[0][0]=0;
		C[0][1]=0;
		C[0][2]=0;
		C[0][3]=0;
		C[1][0]=1;
		C[1][1]=1;
		C[1][2]=0;
		C[1][3]=0; 
		C[2][0]=0;
		C[2][1]=0;
		C[2][2]=1;
		C[2][3]=0;
		C[3][0]=1;
		C[3][1]=0;
		C[3][2]=0;
		C[3][3]=1;
		vector<MATRIXi8> gen;
		gen.push_back(A);
		gen.push_back(B);
		gen.push_back(C);
		r->m_r=new ZmodnZ(1,2);
		r->m_flag=1;
		r->m_n=4;
		r->m_Set=Mnr::FR(r->m_r,gen);   
		return r;
	}
	if(i==31)
	{
		FiniteRing* r=new FiniteRing(8,&g_F8Add[0][0],&g_R8_31Mul[0][0],0);
		return r;
	}
	if(i==32)
	{
		FiniteRing* r=new FiniteRing(8,&g_F8Add[0][0],&g_R8_32Mul[0][0],0);
		return r;
	}
	if(i==33)
	{
		Mnr* r=new Mnr();
		MATRIXi8 A(4,vector<unsigned char>(4,0));
		A[0][0]=1;
		A[0][1]=0;
		A[0][2]=0;
		A[0][3]=1;
		A[1][0]=1;
		A[1][1]=0;
		A[1][2]=0;
		A[1][3]=0; 
		A[2][0]=1;
		A[2][1]=1;
		A[2][2]=0;
		A[2][3]=1;
		A[3][0]=1;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=1;
		vector<MATRIXi8> gen;
		gen.push_back(A);
		r->m_r=new ZmodnZ(1,2);
		r->m_flag=1;
		r->m_n=4;
		r->m_Set=Mnr::FR(r->m_r,gen);   
		return r;
	}
	if(i==36)
	{
		FiniteRing* r=new FiniteRing(8,&g_F8Add[0][0],&g_R8_36Mul[0][0],0);
		return r;
	}
	if(i==37)
	{
		FiniteRing* r=new FiniteRing(8,&g_F8Add[0][0],&g_R8_37Mul[0][0],0);
		return r;
	}
	if(i==38)
	{
		Mnr* r=new Mnr();
		r->initL(2);
		return r;
	}
	if(i==39)
	{
		FiniteRing* r=new FiniteRing(8,&g_F8Add[0][0],&g_R8_39Mul[0][0],0);
		return r;
	}
	if(i==43)
	{
		Mnr* r=new Mnr();
		MATRIXi8 A(4,vector<unsigned char>(4,0));
		MATRIXi8 B(4,vector<unsigned char>(4,0));
		MATRIXi8 C(4,vector<unsigned char>(4,0));
		A[0][0]=0;
		A[0][1]=1;
		A[0][2]=0;
		A[0][3]=0;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=0;
		A[1][3]=0; 
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=1;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=1;
		B[0][0]=0;
		B[0][1]=1;
		B[0][2]=1;
		B[0][3]=1;
		B[1][0]=0;
		B[1][1]=1;
		B[1][2]=0;
		B[1][3]=0; 
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=1;
		B[2][3]=0;
		B[3][0]=0;
		B[3][1]=0;
		B[3][2]=0;
		B[3][3]=1;
		C[0][0]=0;
		C[0][1]=1;
		C[0][2]=1;
		C[0][3]=0;
		C[1][0]=0;
		C[1][1]=1;
		C[1][2]=0;
		C[1][3]=0; 
		C[2][0]=0;
		C[2][1]=0;
		C[2][2]=1;
		C[2][3]=0;
		C[3][0]=0;
		C[3][1]=0;
		C[3][2]=0;
		C[3][3]=1;
		vector<MATRIXi8> gen;
		gen.push_back(A);
		gen.push_back(B);
		gen.push_back(C);
		r->m_r=new ZmodnZ(1,2);
		r->m_flag=1;
		r->m_n=4;
		r->m_Set=Mnr::FR(r->m_r,gen);   
		return r;
	}
	if(i==44)
	{
		Mnr* r=new Mnr();
		MATRIXi8 A(3,vector<unsigned char>(3,0));
		MATRIXi8 B(3,vector<unsigned char>(3,0));
		A[0][0]=1;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=0; 
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=0;
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=1;
		B[1][1]=0;
		B[1][2]=0;
		B[2][0]=1;
		B[2][1]=0;
		B[2][2]=0;
		vector<MATRIXi8> gen;
		gen.push_back(A);
		gen.push_back(B);
		r->m_r=new ZmodnZ(1,2);
		r->m_flag=1;
		r->m_n=3;
		r->m_Set=Mnr::FR(r->m_r,gen);   
		return r;
	}
	if(i==45)
	{
		FiniteRing* r=new FiniteRing(8,&g_F4F2Add[0][0],&g_R8_C2C2C2_28_2Mul[0][0],0);
		return r;
	}
	if(i==46)
	{
		int f[] = {0,0,0,1};
		Polynomial vf(f,f+sizeof(f)/sizeof(f[0]));			
		ZmodnZ *r2_2=new ZmodnZ(1,2);	
		PolynomialRing* r=new PolynomialRing(r2_2,vf);
		r->m_flag=1;
		return r;
	}
	if(i==48)
	{
		int f[] = {0,1,0,1};
		Polynomial vf(f,f+sizeof(f)/sizeof(f[0]));			
		ZmodnZ *r2_2=new ZmodnZ(1,2);	
		PolynomialRing* r=new PolynomialRing(r2_2,vf);
		r->m_flag=1;
		return r;
	}
	if(i==49)
	{
		FiniteRing* r=new FiniteRing(8,&g_F4F2Add[0][0],&g_R8_C2C2C2_28_3Mul[0][0],0);
		return r;
	}	
	if(i==51)
	{
		FiniteRing* r=new FiniteRing(8,&g_F4F2Add[0][0],&g_F4F2Mul[0][0],0);
		return r;
	}	
	if(i==52)
	{
		FiniteRing* r=new FiniteRing(8,&g_F8Add[0][0],&g_F8Mul[0][0],0);
		return r;
	}	
	return NULL;
}	

IRing* newR8R2(int ij)
{
	int i=(ij-1)%52+1;
	int j=(ij-1)/52+1;
    IRing* ri=newR8(i);
	if(!ri)
		return NULL;
    IRing* rj=newR2(j);
	if(!rj){
		delete ri;
		return NULL;
	}
	DecompositionRing* r= new DecompositionRing(ri,rj);
	r->m_flag=1;
	return r;
}

void test1()
{
    int *R8Add[]={&g_M2Add[0][0],&g_F2Add[0][0],&g_M3Add[0][0],&g_F3Add[0][0],&g_M3M2_M6Add[0][0],&g_F3M2_R2Add[0][0],&g_F3M2Add[0][0],&g_F3F2Add[0][0],g_C8Add,g_C8Add,g_C8Add,g_C2C4Add,&g_F4F2Add[0][0],&g_F4F2Add[0][0],&g_F4F2Add[0][0],&g_F4F2Add[0][0],&g_F8Add[0][0]};
    int *R8Mul[]={&g_M2Mul[0][0],&g_F2Mul[0][0],&g_M3Mul[0][0],&g_F3Mul[0][0],&g_M3M2_M6Mul[0][0],&g_F3M2_R2Mul[0][0],&g_F3M2Mul[0][0],&g_F3F2Mul[0][0],&g_C8R2Mul[0][0],&g_Z8Mul[0][0],&g_C8R4Mul[0][0],&g_Z4F2Mul[0][0],&g_R8_C2C2C2_28_R1_Mul[0][0],&g_R8_C2C2C2_28_2Mul[0][0],&g_R8_C2C2C2_28_3Mul[0][0],&g_F4F2Mul[0][0],&g_F8Mul[0][0]};
	int nArr[]={2,2,3,3,6,6,6,6,8,8,8,8,8,8,8,8,8};
	int nDelt[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int nNum=sizeof(nArr)/sizeof(nArr[0]);
	for(int i=0;i<nNum;i++)
	{
		FiniteRing fr(nArr[i],R8Add[i],R8Mul[i],nDelt[i]);
		fr.printTable();
	}	
}

IRing* newR4(int i)
{
	if(i==1)
	{
		ZmodnZ* r=new ZmodnZ(4,16);
		return r;
	}
	if(i==2)
	{
		ZmodnZ* r=new ZmodnZ(2,8);
		return r;
	}	
	if(i==3)
	{
		ZmodnZ* r=new ZmodnZ(1,4);
		return r;
	}
	if(i==4)
	{
		M2r* r=new M2r();
	    r->initD(2);
		return r;
	}	
	if(i==5)
	{
		Mnr* r=new Mnr();
	    r->initE(2);
		return r;
	}	
	if(i==6)
	{
		Mnr* r=new Mnr();
	    r->initF(2);
		return r;
	}	
	if(i==7)
	{
		M2r* r=new M2r();
	    r->initG(2);
		return r;
	}
	if(i==8)
	{
		M2r* r=new M2r();
	    r->initH(2);
		return r;
	}
	if(i==9)
	{
		M2r* r=new M2r();
	    r->initI(2);
		return r;
	}
	if(i==10)
	{
		M2r* r=new M2r();
	    r->initJ(2);
		return r;
	}
	if(i==11)
	{
		M2r* r=new M2r();
	    r->initK(2);
		return r;
	}	
	return NULL;
}

IRing* newR4R4(int ij)
{
	int i=(ij-1)%11+1;
	int j=(ij-1)/11+1;
	if(i>j)
		return NULL;
    IRing* ri=newR4(i);
    IRing* rj=newR4(j);
	DecompositionRing* r= new DecompositionRing(ri,rj);
	r->m_flag=1;
	return r;
}

IRing* newR8R4(int ij)
{
	int i=(ij-1)%52+1;
	int j=(ij-1)/52+1;
    IRing* ri=newR8(i);
	if(!ri)
		return NULL;
    IRing* rj=newR4(j);
	if(!rj){
		delete ri;
		return NULL;
	}
	DecompositionRing* r= new DecompositionRing(ri,rj);
	r->m_flag=1;
	return r;
}

IRing* newR8R8(int ij)
{
	int i=(ij-1)%52+1;
	int j=(ij-1)/52+1;
	if(i>j)
		return NULL;	
    IRing* ri=newR8(i);
	if(!ri)
		return NULL;
    IRing* rj=newR8(j);
	if(!rj){
		delete ri;
		return NULL;
	}
	DecompositionRing* r= new DecompositionRing(ri,rj);
	r->m_flag=1;
	return r;
}

int test2()
{
   set<int> vID;
   int cnt=0;

   for(int i=1;i<=121;i++)
   {
	   IRing* r=newR4R4(i);
	   if(r){
		   int ID=IdRing(r);
		   printf("R4R4_%d=R%d_%d\n",i,r->size(),ID);
		   //r->printTable();
			if(vID.find(ID)!=vID.end()){
				//printf("Hit%d:i=%d,ID=%d\n",cnt++,i,ID);
			}
			vID.insert(ID);
			//string I1=calcI1(r);
			//string I2=calcI2(r);   
			//printf("R%d_%d:I1I2=%s,%s\n",r->size(),ID,I1.c_str(),I2.c_str());
		    delete r;
		    r=NULL;	
	   }	   
   }

   for(int i=1;i<=104;i++)
   {
	   IRing* r=newR8R2(i);
	   if(r){
		   int ID=IdRing(r);
		   printf("R8R2_%d=R%d_%d\n",i,r->size(),ID);
		   //r->printTable();
			if(vID.find(ID)!=vID.end()){
				//printf("Hit%d:i=%d,ID=%d\n",cnt++,i,ID);
			}
			vID.insert(ID);
			//string I1=calcI1(r);
			//string I2=calcI2(r);   
			//printf("R%d_%d:I1I2=%s,%s\n",r->size(),ID,I1.c_str(),I2.c_str());
		   delete r;
		   r=NULL;
	   }	   
   }
   printf("%d种16阶可分解环\n",vID.size());
   return 0;
}

void findsubring(IRing *r,int n)
{
#define PRINT_LOG 1	
	bool bFind=false;	
	int ID=IdRing(r);
#if PRINT_LOG
    char sz[100]="0";
	sprintf(sz,"R%d_%d_%d.txt",r->size(),ID,time(NULL));
    ofstream fout(sz);
#endif	
    string strCmd="del ";
	strCmd+=sz;
	map<pair<int,int>,pair<int,int>> M;		
	for(int i=0;i<r->size()-1;i++)		
	for(int j=i+1;j<r->size();j++)
	{
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);
		Subring S1i;
		bool bn=S1i.init(r,v,16);
		if(!bn)
			continue;
		//Subring S1i(r,v);
		int ni=S1i.size();
		if(ni!=16)
			continue;
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){
			printf("cnt1=%d:R%d_%d->i=%d,j=%d\n",cnt1,ni,ID,i,j);
			//string I1=calcI1(&S1i);
			//string I2=calcI2(&S1i);   
			//printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());				
		}		
		if(ni==n && ID==-1)//||(ID==230||ID==232||ID==236||ID==241||ID==244||ID==246||ID==337)||(ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==39)))   
        //if((ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==39))) 
		{		
			string strR=calcRingInvariant(&S1i);
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",ni,ID,strR.c_str());				
			//S1i.printTable();
#if PRINT_LOG			
			fout<<i<<","<<j<<"=>";
			fout<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2="<<strR<<endl;
			bFind=true;
#endif
			//break;
		}		   
	}
#if PRINT_LOG
	fout.close();	
	if(!bFind)	
		system(strCmd.c_str());
	else
		printf("子环表示已输出到文件%s\n",sz);
#endif	
}

void findsubring3(IRing *r,int n)
{
#define PRINT_LOG 1	
	bool bFind=false;	
	int ID=0;//IdRing(r);
#if PRINT_LOG
    char sz[100]="0";
	sprintf(sz,"R%d_%d_%d.txt",r->size(),ID,time(NULL));
    ofstream fout(sz);
#endif	
    string strCmd="del ";
	strCmd+=sz;
	set<string> S;	
	for(int i=0;i<r->size()-2;i++)		
	for(int j=i+1;j<r->size()-1;j++){
	for(int k=j+1;k<r->size();k++){		
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);
		v.push_back(k);		
		Subring S1i;
		bool bn=S1i.init(r,v,16);
		if(!bn)
			continue;
		//Subring S1i(r,v);
		int ni=S1i.size();
		if(ni!=16)
			continue;
		ID=IdRing(&S1i);
		if(ni==16 && ID==-1)
		{		
			string strR=calcRingInvariant(&S1i);
			if(S.find(strR)==S.end()){
				printf("%d,%d,%d=>R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",i,j,k,ni,ID,strR.c_str());
				//printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",ni,ID,strR.c_str());
	#if 0//PRINT_LOG			
				fout<<i<<","<<j<<","<<k<<"=>";
				fout<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2="<<strR<<endl;
				bFind=true;
	#endif				
			}
			S.insert(strR);						
			//break;
		}		   
	}
	}
#if PRINT_LOG
	fout.close();	
	if(!bFind)	
		system(strCmd.c_str());
	else
		printf("子环表示已输出到文件%s\n",sz);
#endif	
}

void findquotientring(IRing *r,int n)
{
#define PRINT_LOG 1	
	bool bFind=false;	
	int ID=IdRing(r);
#if PRINT_LOG
    char sz[100]="0";
	sprintf(sz,"R%d_%d_%d.txt",r->size(),ID,time(NULL));
    ofstream fout(sz);
#endif	
    string strCmd="del ";
	strCmd+=sz;
	map<pair<int,int>,pair<int,int>> M;	
	for(int i=0;i<r->size()-1;i++)		
    //int i=0;
	for(int j=i+1;j<r->size();j++)
	{
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);
		Subring S1i0;
		bool bn=S1i0.init(r,v,r->size()/n);
		if(!bn)
			continue;
		if(S1i0.m_Set.size()!=r->size()/n)
			continue;
		vector<int> v0=v;
		v=S1i0.m_Set;
		int iret1=IsIdeal(r,v); 
		if(iret1!=1)
			continue;
		quotientRing S1i(r,v);
		int ni=S1i.size();		
		int ID=IdRing(&S1i);	
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){		
            int IDr=0;//IdRing(r);
			int IDr0=IdRing(&S1i0);
			if(S1i0.size()==4 && IDr0==-1){
				Subring S1i00(r,S1i.m_I);
				printRing(&S1i00);
			}
			printf("cnt1=%d:R%d_%d/R%d_%d=R%d_%d->i=%d,j=%d\n",cnt1,r->size(),IDr,S1i0.size(),IDr0,ni,ID,i,j);
		}	
		if(ni==n && ((ni==16 && ID==-1)||(ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==39)))) 	
		{		
			string strR=calcRingInvariant(&S1i);
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",ni,ID,strR.c_str());				
			//S1i.printTable();
#if PRINT_LOG			
			fout<<i<<","<<j<<"=>";
			fout<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2="<<strR<<endl;
			bFind=true;
#endif
			//break;
		}
	}
#if PRINT_LOG
	fout.close();	
	if(!bFind)	
		system(strCmd.c_str());
	else
		printf("商环表示已输出到文件%s\n",sz);
#endif	
}

int g_i=1;
int g_a=572;
void testR8R4()
{
   //R8R4:1~572
   //R8R8:1~2400,2570~2704
   //R8R8:1~1520
   for(int i=g_i;i<=g_a;i++)
   {
	   IRing* r=newR8R4(i);
	   if(r){		   
		   printf("R8R4_%d\n",i);
		   //findsubring3(r,16);		   
		   findquotientring(r,16);
		   delete r;
		   r=NULL;
	   }	   
   }	
}

void testR8R8()
{
   //R8R4:1~572
   //R8R8:1~2400,2570~2704
   //R8R8:1~1520
   for(int i=g_i;i<=g_a;i++)
   {
	   IRing* r=newR8R8(i);
	   if(r){		   
		   printf("R8R8_%d\n",i);
		   //findsubring3(r,16);		   
		   findquotientring(r,16);
		   delete r;
		   r=NULL;
	   }	   
   }	
}

void testR4R4()
{
   for(int i=g_i;i<=g_a;i++)//1~121
   {
	   IRing* r=newR4R4(i);
	   if(r){
		   printf("R4R4_%d\n",i);
		   //findsubring(r,8);
		   findquotientring(r,8);
		   delete r;
		   r=NULL;
	   }	   
   }	
}

void testM2R4(int i)
{
   //for(int i=1;i<=11;i++)//1~121
   {
	   IRing* r4=newR4(i);
	   M2r* r=new M2r(r4);
	   if(r){
		   r->m_flag=1;
		   printf("M2R4_%d\n",i);
		   findquotientring(r,16);
		   delete r;
		   r=NULL;
	   }	   
   }	
}

int main(int argc, char* argv[])
{   
   if(argc>1)
	   g_i=atoi(argv[1]);
   if(argc>2)
	   g_a=atoi(argv[2]);   
   testR8R8();
   //system("pause");
   return 0;
}