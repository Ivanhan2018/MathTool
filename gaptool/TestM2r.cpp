#include"IRing.h"
#include<stdio.h>
#include<stdlib.h>

// 16阶环M_2(Z_2)
struct M2Z2:public IRing
{
public:
    // 类型
	struct M2
	{
		int a,b,c,d;
	};
	// 静态函数
	static M2 Add(const M2 &m,const M2 &n);
	static M2 Mul(const M2 &m,const M2 &n);
public:
	// 实现抽象基类的方法
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// 构造函数
	M2Z2(){};
	// 成员函数	
	void init(int ID);
	void init(M2* p,int cnt);
	int M2ElemToInt(const M2 &m);	
	// 成员变量
	vector<M2> g_M2Elem;
};

int M2Z2::M2ElemToInt(const M2Z2::M2 &m)
{
	for(int i=0;i<size();i++)
	{
		if(g_M2Elem[i].a==m.a && g_M2Elem[i].b==m.b && g_M2Elem[i].c==m.c && g_M2Elem[i].d==m.d)
			return i;
	}
	return -1;
}

M2Z2::M2 M2Z2::Add(const M2Z2::M2 &m,const M2Z2::M2 &n)
{
	M2 ret;
	ret.a=(m.a+n.a)%2;
	ret.b=(m.b+n.b)%2;
	ret.c=(m.c+n.c)%2;
	ret.d=(m.d+n.d)%2;
	return ret;
}

M2Z2::M2 M2Z2::Mul(const M2Z2::M2 &m,const M2Z2::M2 &n)
{
	M2 ret;
	ret.a=(m.a*n.a+m.b*n.c)%2;
	ret.b=(m.a*n.b+m.b*n.d)%2;
	ret.c=(m.c*n.a+m.d*n.c)%2;
	ret.d=(m.c*n.b+m.d*n.d)%2;
	return ret;
}

void M2Z2::printTable()
{
	int ID=IdRing(this);
	string str=calcRingInvariant(this);
	printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",size(),ID,str.c_str());	
	string I1=calcI1(this);
	string I2=calcI2(this);   
	printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());	
}

int M2Z2::add(int a,int b)
{
	M2 ma=g_M2Elem[a];
	M2 mb=g_M2Elem[b];

	M2 mc=Add(ma,mb);
	int c=M2ElemToInt(mc);

	return c;
}

int M2Z2::mul(int a,int b)
{
	M2 ma=g_M2Elem[a];
	M2 mb=g_M2Elem[b];

	M2 mc=Mul(ma,mb);
	int c=M2ElemToInt(mc);

	return c;
}

int M2Z2::size()
{
	return g_M2Elem.size();
}

void M2Z2::init(int ID)
{
    if(ID==0){
        M2 M2Elem[16]={
			{0,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,1,0,0},
			{0,0,1,0},			
			{1,0,1,0},
			{1,1,1,0},
			{0,1,1,0},	
			{0,0,0,1},
			{1,0,0,1},
			{1,1,0,1},
			{0,1,0,1},
			{0,0,1,1},			
			{1,0,1,1},
			{1,1,1,1},
			{0,1,1,1}			
        };   
        init(&M2Elem[0],16);		
		return;
	}else if(ID==7){
		//Y
        M2 M2Elem[4]={
			{0,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,1,0,0}			
        };   
        init(&M2Elem[0],4);		
		return;
	}else if(ID==8){
		//P
        M2 M2Elem[4]={
			{0,0,0,0},
			{1,0,0,0},
			{1,0,1,0},
			{0,0,1,0}
        }; 
        init(&M2Elem[0],4);		
		return;			
	}else if(ID==9){
		//X
        M2 M2Elem[4]={
			{0,0,0,0},
			{1,0,0,1},
			{0,1,1,0},
			{1,1,1,1}
        }; 
        init(&M2Elem[0],4);		
		return;		
	}else if(ID==10){
		//D
        M2 M2Elem[4]={
			{0,0,0,0},
			{1,0,0,1},
			{0,0,0,1},
			{1,0,0,0}
        }; 
        init(&M2Elem[0],4);		
		return;
	}else if(ID==11){
		//K
        M2 M2Elem[4]={
			{0,0,0,0},
			{1,0,0,1},
			{1,1,1,0},
			{0,1,1,1}
        }; 
        init(&M2Elem[0],4);		
		return;
	}
	else{		
		init(0);
		return;
	}
}

void M2Z2::init(M2Z2::M2* p,int cnt)
{
	for(int i=0;i<cnt;i++)
	{
		M2 m2={*((int *)p+4*i),*((int *)p+4*i+1),*((int *)p+4*i+2),*((int *)p+4*i+3)};
		g_M2Elem.push_back(m2);	
	}
}

int main()
{
	M2Z2 r16;
	r16.init(0);
	r16.printTable();
	
	M2Z2 r4_7;
	r4_7.init(7);
	r4_7.printTable();

	return 0;
}
