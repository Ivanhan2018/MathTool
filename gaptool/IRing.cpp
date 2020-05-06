#ifndef IRING_H
#define IRING_H

#include<stdio.h>
#include<string.h>
#include<sstream>
#include<vector>
#include<map>
#include<iterator>
#include<algorithm>
using namespace std;

struct IRing
{
	virtual void printTable() = 0;
	virtual int add(int a,int b) = 0;
	virtual int mul(int a,int b) = 0;
	virtual int size() = 0; 	   
};

int IdRing(IRing* r)
{	
	return -1;
}

void printRing(IRing* r){
   int n=r->size();
   printf("[R%dAdd]\n",n);   
   for(int i=0;i<n;i++){
	   for(int j=0;j<n;j++){
		  int ij=r->add(i,j);
		  printf("%d ",ij+1);
	   } 
       printf("\n");   
   }
   printf("[R%dMul]\n",n);   
   for(int i=0;i<n;i++){
	   for(int j=0;j<n;j++){
		  int ij=r->mul(i,j);
		  printf("%d ",ij+1); 
	   } 
       printf("\n");	   
   }
}

// 一个环r的子环s
struct Subring:public IRing
{
public:
	// 静态函数
public:
	// 实现抽象基类的方法
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// 构造函数
	Subring(IRing* r,const vector<int>& gens);
	// 成员函数	
	// 成员变量
	vector<int> m_Set;
	IRing* m_r;
};

void Subring::printTable()
{
	int ID=IdRing(this);
	printf("R%d_%d:\n",size(),ID);
	printRing(this);	
}

int Subring::add(int a,int b)
{
	int ij=m_r->add(m_Set[a],m_Set[b]);
	vector<int>::iterator p=std::find(m_Set.begin(),m_Set.end(),ij);
	int IJ=-1;
	if(p!=m_Set.end()){
		IJ=p-m_Set.begin();
	}
	return IJ;
}

int Subring::mul(int a,int b)
{
	int ij=m_r->mul(m_Set[a],m_Set[b]);
	vector<int>::iterator p=std::find(m_Set.begin(),m_Set.end(),ij);
	int IJ=-1;
	if(p!=m_Set.end()){
		IJ=p-m_Set.begin();
	}
	return IJ;
}

int Subring::size()
{
	return m_Set.size();
}

Subring::Subring(IRing* r,const vector<int>& gens)
{
	m_r=r;
	int E=0;
	m_Set.push_back(E);
	for(int i=0;i<gens.size();i++)
	{
		if(gens[i]!=E)
			m_Set.push_back(gens[i]);
	}
	int R=m_Set.size();
	int cnt=R;
	int cnt1=R;
	do{
		cnt=m_Set.size();
		for(int i=0;i<cnt;i++)
		{
			for(int j=0;j<cnt;j++)
			{
				int IJ=m_r->mul(m_Set[i],m_Set[j]);
				vector<int>::iterator p=std::find(m_Set.begin(),m_Set.end(),IJ);
				if(p==m_Set.end()){
					m_Set.push_back(IJ);
				}
				int IJ1=m_r->add(m_Set[i],m_Set[j]);
				p=std::find(m_Set.begin(),m_Set.end(),IJ1);
				if(p==m_Set.end()){
					m_Set.push_back(IJ1);
				}
			}
		}
		cnt1=m_Set.size();
	}while(cnt1>cnt);
}

string itos(int i)
{
  stringstream s;
  s << i;
  return s.str();
}

vector<int>  DivisorsInt(int n){
	vector<int> ret;
	if(n<1)
		return ret;
	for(int i=1;i<=n;i++){
		if(n%i==0){
			ret.push_back(i);
		}
	}
	return ret;
}

string simplyS1(vector<int>& S1){
    int n=S1.size();
	vector<int> vOrders=DivisorsInt(n);
	vector<int> vCounts(n+1);
 	for(int i=0;i<n;i++)
	{
		int ord=S1[i];
		vCounts[ord]=vCounts[ord]+1;
	}
	string str="[";
	for(int i=0;i<vOrders.size();i++)
	{
		char sz[200]={0};
		sprintf(sz,"%d,",vCounts[vOrders[i]]);
		str+=sz;
	}
	if(str.size()>2)
	{
		str=str.substr(0,str.size()-1);
	}
	str+="]";
	return str;
}

string calS1(IRing* r,bool bDone){

   int n=r->size();
   vector<int> S1;
   for(int i=0;i<n;i++){
	   vector<int> v;
	   v.push_back(i);
	   Subring si(r,v);
	   S1.push_back(si.size());
   }
   if(bDone)
   {
	   string str=simplyS1(S1);
	   return str;
   }	
   string str="[";
   for(int i=0;i<n;i++)
   {
	   str+=itos(S1[i]);
	   if(i<n-1)
		   str+=",";   
   }	   
   str+="]";
   return str;
}

// 有限循环环mZ/nZ，这里限制m|n
struct ZmodnZ:public IRing
{
public:
	// 静态函数
public:
	// 实现抽象基类的方法
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// 构造函数
	ZmodnZ(int m,int n);
	// 成员函数	
	// 成员变量
	int m_m;
	int m_n;// 运算为模n加法和模n乘法	
};

void ZmodnZ::printTable()
{
	int ID=IdRing(this);
	string S1=calS1(this,true);
	string undoS1=calS1(this,false);	
	printf("R%d_%d:S1=%s,S1(未经处理)=%s\n",size(),ID,S1.c_str(),undoS1.c_str());
	printRing(this);	
}

int ZmodnZ::add(int a,int b)
{
	return ((m_m*a+m_m*b)%m_n)/m_m;
}

int ZmodnZ::mul(int a,int b)
{
	return ((m_m*a*m_m*b)%m_n)/m_m;	
}

int ZmodnZ::size()
{
	return m_n/m_m;
}

ZmodnZ::ZmodnZ(int m,int n)
{
	m_m=m;
	m_n=n;
}

typedef vector<vector<int> > MATRIXi;

// 2阶全矩阵环M2(r)
struct M2r:public IRing
{
public:
	// 静态函数
   static int getidx(vector<MATRIXi> &Arr2,MATRIXi &Arr1);
   static bool IsEqual(const MATRIXi &t,const MATRIXi &m);	
public:
	// 实现抽象基类的方法
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// 构造函数
	M2r(IRing* r);
	// 成员函数	
	// 成员变量
	vector<MATRIXi> m_Set;
	IRing* m_r;
};

int M2r::getidx(vector<MATRIXi> &Arr2,MATRIXi &Arr1){
	int N=Arr2.size();
	for(int i=0;i<N;i++){
		if(IsEqual(Arr2[i],Arr1))
			return i;
	}
	return -1;
}

bool M2r::IsEqual(const MATRIXi &t,const MATRIXi &m){
	int nt=t.size();
	int nm=m.size();
	if(nt!=nm)
		return false;
	for(int i=0;i<nt;i++){
		for(int j=0;j<nt;j++){
			if(t[i][j]!=m[i][j])
				return false;
		}
	}
	return true;
}

void M2r::printTable()
{
	int ID=IdRing(this);
	string S1=calS1(this,true);
	string undoS1=calS1(this,false);	
	printf("R%d_%d:S1=%s,S1(未经处理)=%s\n",size(),ID,S1.c_str(),undoS1.c_str());
	if(size()<100){
	printRing(this);	
	}
	else{
	   printf("环的阶太大，不在控制台打印\n");
	}
}

int M2r::add(int a,int b)
{
	MATRIXi A=m_Set[a];
	MATRIXi B=m_Set[b];	
    int n=2;
	MATRIXi C(n,vector<int>(n,0));
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			C[i][j]=m_r->add(A[i][j],B[i][j]);
		}
	}
	int c=getidx(m_Set,C);
	return c;
}

int M2r::mul(int a,int b)
{
	MATRIXi A=m_Set[a];
	MATRIXi B=m_Set[b];	
    int n=2;
	MATRIXi C(n,vector<int>(n,0));
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			int sum=0;
			for(int k=0;k<n;k++){
				sum=m_r->add(sum,m_r->mul(A[i][k],B[k][j]));
			}
			C[i][j]=sum;
		}
	}
	int c=getidx(m_Set,C);
	return c;
}

int M2r::size()
{
	return m_Set.size();
}

M2r::M2r(IRing* r)
{
	m_r=r;
	int N=r->size();
	int n=2;
	MATRIXi m(n,vector<int>(n,0));
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			for(int k=0;k<N;k++){
				for(int l=0;l<N;l++){
					m[0][0]=i;
					m[0][1]=j;
					m[1][0]=k;
					m[1][1]=l;
                    m_Set.push_back(m);					
				}
			}
		}
	}
}

int main()
{
   ZmodnZ r4_2(2,8);
   r4_2.printTable();  

   ZmodnZ r4_3(1,4);  
   r4_3.printTable();
   
   M2r m2r4_2(&r4_2);
   m2r4_2.printTable(); 

   M2r m2r4_3(&r4_3);
   m2r4_3.printTable(); 
   
   return 0;
}

#endif