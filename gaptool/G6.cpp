#include"IGroup.h"
#include <iostream>
#include <functional>
using namespace std;

struct I2{
	int a,b;
};

// G6
struct G6:public IGroup
{
public:
   //  静态函数
   static int I2ToInt(I2 a);
   static I2 mulInD6(I2 a,I2 b);   
   static I2 mulInC6(I2 a,I2 b);
   static int InvInC3(int a);
public:
   // 实现抽象基类的方法
   virtual void printSet();
   virtual void printTable();     
   virtual int mul(int a,int b);
   virtual int size();
   virtual int inv(int a);
   // 构造函数
   G6(int ID);// 默认构造函数
   // 成员函数
   // 成员变量
   static I2 g_I2[6];
   int m_ID;
};

G6::G6(int ID){
	m_ID=ID;
}

int G6::mul(int a,int b){
	function<I2(I2,I2)> mulArr[]={mulInD6,mulInC6};
	int idx=m_ID-1;
	I2 cc=mulArr[idx](g_I2[a],g_I2[b]);
	int c=I2ToInt(cc);
	return c;
}

void G6::printSet(){
	int ID=IdGroup(this);
	printf("GAP[%d,%d]:\n",size(),ID);
	for(int i=0;i<size();i++){
		int i1=inv(i);
		printf("%d=>%d\n",i,i1);
	}
}

void G6::printTable(){
	printGroup(this);
}

int G6::inv(int a){
	vector<int> S1=Order(this,a);
	int ord=S1.size();	
	return S1[ord-1];
}

int G6::size(){
	return 6;	
}

// (I,J)的编号为n*(I-1)+(J-1)+1，这里1<=I<=m，1<=J<=n
// 群同态φ:C_2->Aut(C_3)=C_2，3阶群I,r,rr,2阶群I,f
// 平凡同态φ1:I->I,r,rr,f->I,r,rr对应直积
// 非平凡同态φ2:I->I,r,rr,f->I,rr,r对应半直积
I2 G6::g_I2[6]={
	{1,1},
	{1,2},
	{2,1},
	{2,2},
	{3,1},
	{3,2},
};

int G6::I2ToInt(I2 a){
	for(int i=0;i<6;i++){
		if(g_I2[i].a==a.a && g_I2[i].b==a.b)
			return i;
	}
	return -1;
}

// 直积群C_6
// 直积的乘法表为：(x1,y1)×(x2,y2)=(x1*x2,y1+y2)，显然幺元是(1,1)=1
I2 G6::mulInC6(I2 a,I2 b){
	I2 ret={(a.a+b.a-2)%3+1,(a.b+b.b-2)%2+1};
	return ret;
}

// 有2n个元素的二面体群D_n同构于循环群C_n和C_2的半直积。
// 这里，C_2的非单位元作用于C_n，将元素变成其逆；这是一个自同构，因为C_n是交换群。
// 半直积群C_3:C_2
// 半直积的乘法表为：(x1,y1):(x2,y2)=(x1*(φ(y1)(x2)),y1+y2)，显然幺元是(1,1)=1
// 这里φ(y1)是群N的一个自同构，(φ(y1)(x2))是群N中的一个群元，当φ为平凡同态时，(φ(y1)(x2))=x2
I2 G6::mulInD6(I2 a,I2 b){
	I2 ret={(a.a+b.a-2)%3+1,(a.b+b.b-2)%2+1};
	/*
		(x1,y1):(x2,y2)
		=(x1*(φ(y1)(x2)),y1+y2)=(x1*((x2)^-1),y1+y2),y1=2时；
		=(x1*(φ(y1)(x2)),y1+y2)=(x1*x2,y1+y2),y1=1时；
	*/
	if(a.b==2){
		ret.a=(a.a+InvInC3(b.a)-2)%3+1;
	}
	return ret;
}

int G6::InvInC3(int a){
	if(a==1)
		return 1;
	if(a==2)
		return 3;
	if(a==3)
		return 2;
	return 0;
}

int main()
{
	G6 d6(1);
	d6.printSet();
	d6.printTable();
	
	G6 c6(2);
	c6.printSet();
	c6.printTable();
	
	return 0;
} 

