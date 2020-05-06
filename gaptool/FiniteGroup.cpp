#include"IGroup.h"
#include<iostream>
#include<complex>
using namespace std;

int G12_2[12][12]=
{
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 },
	{2, 1, 4, 3, 6, 5, 8, 7, 10, 9, 12, 11 },
	{3, 4, 5, 6, 1, 2, 9, 10, 11, 12, 7, 8 },
	{4, 3, 6, 5, 2, 1, 10, 9, 12, 11, 8, 7 },
	{5, 6, 1, 2, 3, 4, 11, 12, 7, 8, 9, 10 },
	{6, 5, 2, 1, 4, 3, 12, 11, 8, 7, 10, 9},
	{7, 8, 9, 10, 11, 12, 2, 1, 4, 3, 6, 5},
	{8, 7, 10, 9, 12, 11, 1, 2, 3, 4, 5, 6},
	{9, 10, 11, 12, 7, 8, 4, 3, 6, 5, 2, 1},
	{10, 9, 12, 11, 8, 7, 3, 4, 5, 6, 1, 2},
	{11, 12, 7, 8, 9, 10, 6, 5, 2, 1, 4, 3},
	{12, 11, 8, 7, 10, 9, 5, 6, 1, 2, 3, 4}
};

int G12_3[12][12]=
{
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 },
	{2, 1, 4, 3, 8, 7, 6, 5, 11, 12, 9, 10 },
	{3, 4, 1, 2, 6, 5, 8, 7, 12, 11, 10, 9 },
	{4, 3, 2, 1, 7, 8, 5, 6, 10, 9, 12, 11 },
	{5, 6, 7, 8, 9, 10, 11, 12, 1, 2, 3, 4 },
	{6, 5, 8, 7, 12, 11, 10, 9, 3, 4, 1, 2},
	{7, 8, 5, 6, 10, 9, 12, 11, 4, 3, 2, 1},
	{8, 7, 6, 5, 11, 12, 9, 10, 2, 1, 4, 3},
	{9, 10, 11, 12, 1, 2, 3, 4, 5, 6, 7, 8},
	{10, 9, 12, 11, 4, 3, 2, 1, 7, 8, 5, 6},
	{11, 12, 9, 10, 2, 1, 4, 3, 8, 7, 6, 5},
	{12, 11, 10, 9, 3, 4, 1, 2, 6, 5, 8, 7}
};

int G12_4[12][12]=
{
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 },
	{2, 3, 1, 5, 6, 4, 9, 7, 8, 12, 10, 11  },
	{3, 1, 2, 6, 4, 5, 8, 9, 7, 11, 12, 10 },
	{4, 5, 6, 1, 2, 3, 10, 11, 12, 7, 8, 9 },
	{5, 6, 4, 2, 3, 1, 12, 10, 11, 9, 7, 8 },
	{6, 4, 5, 3, 1, 2, 11, 12, 10, 8, 9, 7},
	{7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6},
	{8, 9, 7, 11, 12, 10, 3, 1, 2, 6, 4, 5},
	{9, 7, 8, 12, 10, 11, 2, 3, 1, 5, 6, 4},
	{10, 11, 12, 7, 8, 9, 4, 5, 6, 1, 2, 3},
	{11, 12, 10, 8, 9, 7, 6, 4, 5, 3, 1, 2},
	{12, 10, 11, 9, 7, 8, 5, 6, 4, 2, 3, 1}
};

int G12_5[12][12]=
{
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 },
	{2, 3, 1, 5, 6, 4, 8, 9, 7, 11, 12, 10 },
	{3, 1, 2, 6, 4, 5, 9, 7, 8, 12, 10, 11 },
	{4, 5, 6, 1, 2, 3, 10, 11, 12, 7, 8, 9 },
	{5, 6, 4, 2, 3, 1, 11, 12, 10, 8, 9, 7 },
	{6, 4, 5, 3, 1, 2, 12, 10, 11, 9, 7, 8},
	{7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6 },
	{8, 9, 7, 11, 12, 10, 2, 3, 1, 5, 6, 4},
	{9, 7, 8, 12, 10, 11, 3, 1, 2, 6, 4, 5},
	{10, 11, 12, 7, 8, 9, 4, 5, 6, 1, 2, 3},
	{11, 12, 10, 8, 9, 7, 5, 6, 4, 2, 3, 1},
	{12, 10, 11, 9, 7, 8, 6, 4, 5, 3, 1, 2}
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

int g_M16Mul[16][16]={
{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
{1,0,8,9,10,11,7,6,2,3,4,5,15,14,13,12},
{2,8,4,6,5,1,12,15,10,7,11,0,13,9,3,14},
{3,9,7,0,12,14,8,2,6,1,15,13,4,11,5,10},
{4,10,5,12,1,8,13,14,11,15,0,2,9,7,6,3},
{5,11,1,13,8,10,9,3,0,14,2,4,7,15,12,6},
{6,7,15,2,13,3,10,4,12,8,14,9,5,0,1,11},
{7,6,12,8,14,9,4,10,15,2,13,3,11,1,0,5},
{8,2,10,7,11,0,15,12,4,6,5,1,14,3,9,13},
{9,3,6,1,15,13,2,8,7,0,12,14,10,5,11,4},
{10,4,11,15,0,2,14,13,5,12,1,8,3,6,7,9},
{11,5,0,14,2,4,3,9,1,13,8,10,6,12,15,7},
{12,15,14,4,9,6,11,5,13,10,3,7,1,2,8,0},
{13,14,3,5,7,12,0,1,9,11,6,15,8,4,10,2},
{14,13,9,11,6,15,1,0,3,5,7,12,2,10,4,8},
{15,12,13,10,3,7,5,11,14,4,9,6,0,8,2,1},
};

int g_P16Mul[16][16]={
{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
{1,0,3,2,5,4,7,6,9,8,11,10,13,12,15,14},
{2,3,1,0,6,7,5,4,10,11,9,8,14,15,13,12},
{3,2,0,1,7,6,4,5,11,10,8,9,15,14,12,13},
{4,5,6,7,0,1,2,3,14,15,13,12,11,10,8,9},
{5,4,7,6,1,0,3,2,15,14,12,13,10,11,9,8},
{6,7,5,4,2,3,1,0,13,12,15,14,8,9,10,11},
{7,6,4,5,3,2,0,1,12,13,14,15,9,8,11,10},
{8,9,10,11,15,14,12,13,0,1,2,3,6,7,5,4},
{9,8,11,10,14,15,13,12,1,0,3,2,7,6,4,5},
{10,11,9,8,12,13,14,15,2,3,1,0,5,4,7,6},
{11,10,8,9,13,12,15,14,3,2,0,1,4,5,6,7},
{12,13,14,15,10,11,9,8,7,6,4,5,0,1,2,3},
{13,12,15,14,11,10,8,9,6,7,5,4,1,0,3,2},
{14,15,13,12,9,8,11,10,4,5,6,7,2,3,1,0},
{15,14,12,13,8,9,10,11,5,4,7,6,3,2,0,1},
};

// 生成循环群C_n的凯莱表
vector<int> Cn(int n)
{
 vector<int> v(n*n);	
 for(int i=0;i<n;i++)
 {
  for(int j=0;j<n;j++)
  {
	int ij=(i+j)%n;
    v[i*n+j]=ij;	
  }
 }
 return v;
}

// 直接从凯莱表构造一个有限群
struct FiniteGroup:public IGroup
{
public:
   //  静态函数  
public:
   // 实现抽象基类的方法
   virtual void printSet();
   virtual void printTable();     
   virtual int mul(int a,int b);
   virtual int size();
   virtual int inv(int a);
   // 构造函数
   FiniteGroup(int n,int* arr,int delt);
   // 成员函数
   vector<int> Order(int m);  
   // 成员变量  
  int m_n; 
  int* m_Arr;
  int m_delt;   
};

FiniteGroup::FiniteGroup(int n,int* arr,int delt)
{
    m_n=n;
	m_Arr=arr;
	m_delt=delt;	
}

void FiniteGroup::printSet()
{
/*
	int ID=IdGroup(this);	
	printf("GAP[%d,%d]:\n",m_n,ID);
	for(int i=0;i<m_n;i++){
		int i1=inv(i);
		printf("%d->%d\n",i,i1);
	}
	*/
}

void FiniteGroup::printTable()
{
   int ID=IdGroup(this);	
   printf("GAP[%d,%d]:\n",m_n,ID);      
   //printGroup(this);
}

int FiniteGroup::mul(int a,int b)
{
	int c=*(m_Arr+a*m_n+b);
	return c-m_delt;
}

int FiniteGroup::inv(int a)
{
	vector<int> S1=Order(a);
	int ord=S1.size();
	int v=S1[ord-1];	
	return v;
}

int FiniteGroup::size()
{
	return m_n;
}

vector<int> FiniteGroup::Order(int m)
{
	vector<int> ret;
	int mi=m;
	int m0=0;
    ret.push_back(m0);
	while(mi!=m0){
		ret.push_back(mi);
		mi=mul(mi,m);
	}
	return ret;
}

// 有限可分解群
struct DecompositionGroup:public IGroup
{
public:
   //  静态函数  
   static vector<vector<int> > getTable(IGroup *g);
   static vector<vector<int> > DirectProduct(const vector<vector<int>> &A,const vector<vector<int>> &B);   
public:
   // 实现抽象基类的方法
   virtual void printSet();
   virtual void printTable();     
   virtual int mul(int a,int b);
   virtual int size();
   virtual int inv(int a);
   // 构造函数
   DecompositionGroup(IGroup *g1,IGroup *g2);
   // 成员函数
   vector<int> Order(int m);  
   // 成员变量  
   IGroup *m_g1;
   IGroup *m_g2; 
   vector<vector<int> > s_Arr;  
};

vector<vector<int> > DecompositionGroup::getTable(IGroup *g)
{
	vector<vector<int> > vv(g->size(),vector<int>(g->size()));
	for(int i=0;i<g->size();i++)
			for(int j=0;j<g->size();j++)
			{
				vv[i][j]=g->mul(i,j);	
			}
	return vv;
}

vector<vector<int> > DecompositionGroup::DirectProduct(const vector<vector<int>> &A,const vector<vector<int>> &B)
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

DecompositionGroup::DecompositionGroup(IGroup *g1,IGroup *g2)
{
    m_g1=g1;
	m_g2=g2;
	vector<vector<int> > A=getTable(g1);
	vector<vector<int>> B=getTable(g2);	
	s_Arr=DirectProduct(A,B);
}

void DecompositionGroup::printSet()
{
   /*
   int ID=IdGroup(this);
   int ID1=IdGroup(m_g1);   
   int ID2=IdGroup(m_g2);   
   printf("GAP[%d,%d]=GAP[%d,%d]×GAP[%d,%d]:\n",size(),ID,m_g1->size(),ID1,m_g2->size(),ID2); 
	for(int i=0;i<size();i++){
		int i1=inv(i);
		printf("%d->%d\n",i,i1);
	}
	*/
}

void DecompositionGroup::printTable()
{
   int ID=IdGroup(this);
   int ID1=IdGroup(m_g1);   
   int ID2=IdGroup(m_g2);   
   printf("GAP[%d,%d]=GAP[%d,%d]×GAP[%d,%d]:\n",size(),ID,m_g1->size(),ID1,m_g2->size(),ID2);      
   //printGroup(this);
}

int DecompositionGroup::mul(int a,int b)
{
	int c=s_Arr[a][b];
	return c;
}

int DecompositionGroup::inv(int a)
{
	vector<int> S1=Order(a);
	int ord=S1.size();
	int v=S1[ord-1];	
	return v;
}

int DecompositionGroup::size()
{
	return s_Arr.size();
}

vector<int> DecompositionGroup::Order(int m)
{
	vector<int> ret;
	int mi=m;
	int m0=0;
    ret.push_back(m0);
	while(mi!=m0){
		ret.push_back(mi);
		mi=mul(mi,m);
	}
	return ret;
}

int main(){
	vector<int> vC2=Cn(2);
	FiniteGroup C2(2,&vC2[0],0);	
    int *G12[]={&G12_2[0][0],&G12_3[0][0],&G12_4[0][0],&G12_5[0][0]};
	int nArr[]={12,12,12,12};
	int nDelt[]={1,1,1,1};	
	int nNum=sizeof(nArr)/sizeof(nArr[0]);
	for(int i=0;i<nNum;i++)
	{
		FiniteGroup fg(nArr[i],G12[i],nDelt[i]);
		fg.printSet();
		fg.printTable();
		DecompositionGroup dg(&fg,&C2);
		dg.printSet();
		dg.printTable();
	}
	
	vector<int> vC3=Cn(3);
	FiniteGroup C3(3,&vC3[0],0);	
	FiniteGroup C8(8,&g_C8Mul_2[0][0],0);
	FiniteGroup C4C2(8,&g_C2C4Mul_2[0][0],0);	
	C8.printSet();
	C8.printTable();	
	C4C2.printSet();
	C4C2.printTable();
	DecompositionGroup C8C3(&C8,&C3);
	C8C3.printSet();
	C8C3.printTable();
	DecompositionGroup C4C2C3(&C4C2,&C3);
	C4C2C3.printSet();
	C4C2C3.printTable();
	
	FiniteGroup M16(16,&g_M16Mul[0][0],0);	
	FiniteGroup P16(16,&g_P16Mul[0][0],0);
	M16.printSet();
	M16.printTable();	
	P16.printSet();
	P16.printTable();
	DecompositionGroup M16C2(&M16,&C2);
	M16C2.printSet();
	M16C2.printTable();
	DecompositionGroup P16C2(&P16,&C2);
	P16C2.printSet();
	P16C2.printTable();	
	DecompositionGroup M16C3(&M16,&C3);
	M16C3.printSet();
	M16C3.printTable();
	DecompositionGroup P16C3(&P16,&C3);
	P16C3.printSet();
	P16C3.printTable();		
	
	return 0;
}