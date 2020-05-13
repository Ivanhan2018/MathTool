#ifndef DECOMPOSITIONRING_H
#define DECOMPOSITIONRING_H

#include"ZmodnZ.h"

// 有限可分解环
struct DecompositionRing:public IRing
{
public:
	//  静态函数  
	static vector<vector<int> > DirectProduct(const vector<vector<int>> &A,const vector<vector<int>> &B);   
public:
	// 实现抽象基类的方法
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// 构造函数
	DecompositionRing();
	DecompositionRing(IRing *r1,IRing *r2);
	// 析构函数
	~DecompositionRing();
	// 成员函数 
	template<class T> vector<vector<int> > getTable(T *obp,int n,int(T::*p)(int,int));	
	void initF(int n);// R4_6、R9_6所在的有限环序列,
	// 成员变量  
	IRing *m_r1;
	IRing *m_r2;
	vector<vector<int> > m_Add;
	vector<vector<int> > m_Mul;
	int m_flag;// DecompositionRing对象析构时是否释放m_r1、m_r2指向的内存
};

DecompositionRing::DecompositionRing(){
	m_r1=NULL;
	m_r2=NULL;
	m_flag=0;	
}

DecompositionRing::~DecompositionRing(){
	if(m_flag==1 && m_r1!=NULL){
		delete m_r1;
		m_r1=NULL;
	}	
	if(m_flag==1 && m_r2!=NULL){
		delete m_r2;
		m_r2=NULL;
	}
}

void DecompositionRing::initF(int n){
	m_r1=new ZmodnZ(n,n*n);
	m_r2=new ZmodnZ(1,n); 
	vector<vector<int> > Add1=getTable(m_r1,m_r1->size(),&IRing::add);
	vector<vector<int> > Add2=getTable(m_r2,m_r2->size(),&IRing::add);
	m_Add=DirectProduct(Add1,Add2);
	vector<vector<int> > Mul1=getTable(m_r1,m_r1->size(),&IRing::mul);
	vector<vector<int> > Mul2=getTable(m_r2,m_r2->size(),&IRing::mul);
	m_Mul=DirectProduct(Mul1,Mul2);
}

template<class T>
vector<vector<int> > DecompositionRing::getTable(T *obp,int n,int(T::*p)(int,int))
{
	vector<vector<int> > vv(n,vector<int>(n));
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
		{
			vv[i][j]=(obp->*p)(i,j);	
		}
	return vv;
}

vector<vector<int> > DecompositionRing::DirectProduct(const vector<vector<int>> &A,const vector<vector<int>> &B)
{
	vector<vector<int> > C;
	int nA=A.size();
	int nB=B.size();
	int n=nA*nB;

	for(int a=0;a<n;a++)
	{
		vector<int> iRow;
		for(int b=0;b<n;b++)
		{
			int aj=a%nB;
			int ai=a/nB;
			int bj=b%nB;
			int bi=b/nB;
			int i=A[ai][bi];
			int j=B[aj][bj];
			int c=nB*i+j;
			iRow.push_back(c);
		}
		C.push_back(iRow);
	}
	return C;
}

DecompositionRing::DecompositionRing(IRing *r1,IRing *r2)
{
    m_r1=r1;
	m_r2=r2;
	vector<vector<int> > Add1=getTable(r1,r1->size(),&IRing::add);
	vector<vector<int> > Add2=getTable(r2,r2->size(),&IRing::add);
	m_Add=DirectProduct(Add1,Add2);
	vector<vector<int> > Mul1=getTable(r1,r1->size(),&IRing::mul);
	vector<vector<int> > Mul2=getTable(r2,r2->size(),&IRing::mul);
	m_Mul=DirectProduct(Mul1,Mul2);
}

void DecompositionRing::printTable()
{
	int ID=IdRing(this);
	string str=calcRingInvariant(this);
	printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",size(),ID,str.c_str());
	//printRing(this);	
}

int DecompositionRing::add(int a,int b)
{
	return m_Add[a][b];
}

int DecompositionRing::mul(int a,int b)
{
	return m_Mul[a][b];	
}

int DecompositionRing::size()
{
	return m_Add.size();
}

#endif