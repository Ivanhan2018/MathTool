#include<iostream>
#include"IGroup.h"
using namespace std;

// Sn
struct Sn:public IGroup
{
public:
   //  静态函数
   static vector<unsigned char> mul(const vector<unsigned char> &t,const vector<unsigned char> &m);  
   static vector<unsigned char> inv(const vector<unsigned char> &t);
   static vector<vector<unsigned char> > Order(const vector<unsigned char> & m);   
public:
   // 实现抽象基类的方法
   virtual void printSet();
   virtual void printTable();     
   virtual int mul(int a,int b);
   virtual int size();
   virtual int inv(int a);
   // 构造函数
   Sn(int n);
   // 成员函数
   // 成员变量
   vector<vector<unsigned char> > s_Arr;   
   int m_n;   
};

Sn::Sn(int n)
{
   m_n=n;
   vector<unsigned char> v(n);
   for(int i=0;i<n;i++)
		v[i]=i+1;
   do { 
     s_Arr.push_back(v);    
   }while( next_permutation(v.begin(),v.end()));
}

void Sn::printSet()
{
   printf("%d阶群S_%d:\n",size(),m_n);
   if(m_n<5)
   {
		for(int i=0;i<size();i++){
			int i1=inv(i);
			printf("%d=>%d\n",i,i1);
		}	   
   }
   else
   {
	   printf("群的阶太大，不在控制台打印\n");
   }
}

void Sn::printTable()
{
   if(m_n<5)
		printGroup(this);
}

int Sn::mul(int a,int b)
{
	vector<unsigned char> A=s_Arr[a];
	vector<unsigned char> B=s_Arr[b];
	vector<unsigned char> C=mul(A,B);
	int idx=getidx(s_Arr,C);
	return idx;
}

int Sn::inv(int a)
{
	vector<unsigned char> v=inv(s_Arr[a]);
	int idx=getidx(s_Arr,v);
	return idx;
}

int Sn::size()
{
	return s_Arr.size();	
}

vector<unsigned char> Sn::mul(const vector<unsigned char> &t,const vector<unsigned char> &m)
{
	int n=t.size();
	int nm=m.size();	
	vector<unsigned char> ret(n);
	if(n!=nm)
		return ret;
	for(int i=0;i<n;i++)
		ret[i]=m[t[i]-1];
	return ret;
}

vector<unsigned char> Sn::inv(const vector<unsigned char> &t)
{
	vector<vector<unsigned char> > S1=Order(t);
	int ord=S1.size();	
	return S1[ord-1];	
}

vector<vector<unsigned char> > Sn::Order(const vector<unsigned char> & m)
{
	vector<vector<unsigned char> > ret;
    int n=m.size();
	vector<unsigned char> mi=m;
	vector<unsigned char> m0(n);
	for(int i=0;i<n;i++)
	{
		m0[i]=i+1;
	}
	while(memcmp(&mi[0],&m0[0],sizeof(unsigned char)*n)!=0)
	{
		ret.push_back(mi);
		mi=mul(mi,m);
	}
	ret.push_back(mi);
	return ret;
}

int main(){
	
    Sn s9(9);
	s9.printSet();	
    s9.printTable();
	
	unsigned char s9a[]={4,5,6,9,3,2,7,1,8};
	unsigned char s9b[]={6,1,7,4,2,5,9,3,8};	
	vector<unsigned char> a(s9a,s9a+9);
	vector<unsigned char> b(s9b,s9b+9);	
	vector<int> A(1);
	A[0]=getidx(s9.s_Arr,a);
	vector<int> B(1);
	B[0]=getidx(s9.s_Arr,b);	
	vector<int> AB;
	AB.insert(AB.end(),A.begin(),A.end());
	AB.insert(AB.end(),B.begin(),B.end());	
	vector<int> fgA=FG(&s9,A);
	printSubGroup(&s9,fgA);
	vector<int> fgB=FG(&s9,B);
	printSubGroup(&s9,fgB);	
	vector<int> fgAB=FG(&s9,AB);
	printSubGroup(&s9,fgAB);
	
	return 0;
}
