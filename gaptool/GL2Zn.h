#ifndef GL2ZN_H
#define GL2ZN_H

#include"IGroup.h"
#include<iostream>
#include<complex>
using namespace std;

/* ======================= Engin of function inverse_mod_p ======================= */

int mod( int n, int p )
{
register int raw_mod ;      /*  The value n % p computed by C's mod function.  */
raw_mod = n % p ;
if (raw_mod == 0)
    return( 0 ) ;
else if (n >= 0)             /*  mod is not 0.  n >= 0. */
    return( raw_mod ) ;
else
    return( raw_mod + p ) ;  /* mod is not 0.  n < 0. */
} 

int inverse_mod_p( int u, int p )
{
    int t1 = 0 ;
    int t3 = 0 ;
    int q  = 0 ;
    int u1 = 1 ;
    int u3 = u ;
    int v1 = 0 ;
    int v3 = p ;
    int inv_v = 0 ;

    while( v3 != 0)
    {
        q = (int)(u3 / v3) ;
        t1 = u1 - v1 * q ;
        t3 = u3 - v3 * q ;
        u1 = v1 ;
        u3 = v3 ;
        v1 = t1 ;
        v3 = t3 ;
    }
    inv_v = mod( u1, p ) ;
    if ( mod( u * inv_v, p ) != 1)return 0 ;
    return inv_v ;
}
/* ======================= End of function inverse_mod_p ======================= */

typedef int TM2Elem;
typedef vector<TM2Elem> TM2;

// GL(2,n)的子群
struct GL2Zn:public IGroup
{
public:
   //  静态函数
   static vector<TM2> FG(vector<TM2>& gen,int n); 
   static bool IsEqual(const TM2 &t,const TM2 &m);   
   static TM2 mul(const TM2 &t,const TM2 &m,int n);  
   static TM2 inv(const TM2 &t,int n);   
   static TM2Elem det(const TM2 &t,int n);  
public:
   // 实现抽象基类的方法
   virtual void printSet();
   virtual void printTable();     
   virtual int mul(int a,int b);
   virtual int size();
   virtual int inv(int a);
   // 构造函数
   GL2Zn(){}// 默认构造函数
   GL2Zn(int n);
   // 析构函数
   ~GL2Zn(){};   
   // 成员函数
   // 成员变量
   vector<TM2> s_Arr;   
   int m_n;   
};

vector<TM2> GL2Zn::FG(vector<TM2>& gen,int n){
	vector<TM2> S;
	TM2 m0(4);
	m0[0]=1;
	m0[1]=0;
	m0[2]=0;
	m0[3]=1;	
	S.push_back(m0);
	int R=gen.size();
	for(int i=0;i<R;i++){
		if(IsEqual(gen[i],m0))
			continue;
		S.push_back(gen[i]);
	}
	int cnt=R+1;
	int cnt1=R+1;
	do{
		cnt=S.size();
		for(int i=1;i<cnt;i++){
			for(int j=1;j<cnt;j++){
				TM2 IJ=mul(S[i],S[j],n);
				vector<TM2>::iterator p=std::find(S.begin(),S.end(),IJ);
				//vector<TM2>::iterator p=std::find_if(S.begin(),S.end(),[IJ](TM2& m)->bool{return IsEqual(m,IJ);});
				if(p==S.end()){
					S.push_back(IJ);
				}
			}
		}
		cnt1=S.size();
	}while(cnt1>cnt);	
   return S;
}

bool GL2Zn::IsEqual(const TM2 &t,const TM2 &m){
	int nt=t.size();
	int nm=m.size();
	if(nt!=nm)
		return false;
	for(int i=0;i<nt;i++){
			if(t[i]!=m[i])
				return false;
	}
	return true;
}

GL2Zn::GL2Zn(int n)
{
    m_n=n;
	TM2 v(4);
	v[0]=1;
	v[1]=0;
	v[2]=0;
	v[3]=1;
	s_Arr.push_back(v);
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)	
			for(int k=0;k<n;k++)
				for(int m=0;m<n;m++)
				{
					if(i==1 && j==0 && k==0 && m==1)
						continue;
					v[0]=i;
					v[1]=j;
					v[2]=k;
					v[3]=m;
				    int d=det(v,n);
					// 非奇异2阶方阵
					if(d!=0)
						s_Arr.push_back(v);						
				}
}

void GL2Zn::printSet()
{
	if(size()<100){
		int ID=IdGroup(this);	
		printf("GL(2,%d)的%d阶子群GAP[%d,%d]:\n",m_n,size(),size(),ID);
		for(int i=0;i<size();i++){
			TM2 v1=inv(s_Arr[i],m_n);
			int i1=getidx(s_Arr,v1);
			printf("%d->[[%d,%d],[%d,%d]]=>%d->[[%d,%d],[%d,%d]]\n",i,s_Arr[i][0],s_Arr[i][1],s_Arr[i][2],s_Arr[i][3],i1,v1[0],v1[1],v1[2],v1[3]);
		}
	}
}

void GL2Zn::printTable()
{
   int ID=IdGroup(this);	
   printf("GL(2,%d)的%d阶子群GAP[%d,%d]:\n",m_n,size(),size(),ID);
   if(size()<73)   
		printGroup(this);
}

int GL2Zn::mul(int a,int b)
{
	TM2 A=s_Arr[a];
	TM2 B=s_Arr[b];
	TM2 C=mul(A,B,m_n);
	int idx=getidx(s_Arr,C);
	return idx;
}

int GL2Zn::inv(int a)
{
	TM2 v=inv(s_Arr[a],m_n);
	int idx=getidx(s_Arr,v);
	return idx;
}

int GL2Zn::size()
{
	return s_Arr.size();	
}

TM2 GL2Zn::mul(const TM2 &t,const TM2 &m,int n)
{
	TM2 v(4);
	v[0]=t[0] * m[0]+t[1] * m[2];
	v[1]=t[0] * m[1]+t[1] * m[3];
	v[2]=t[2] * m[0]+t[3] * m[2];	
	v[3]=t[2] * m[1]+t[3] * m[3];
	if(n>0){
		for(int i=0;i<4;i++){
			v[i]=mod(v[i],n);
		}
	}	
	return v;
}

TM2Elem GL2Zn::det(const TM2 &t,int n)
{
	int ret=t[0]*t[3]-t[1]*t[2];
	if(n>0){
		ret=mod(ret,n);
	}
	return ret;
}

TM2 GL2Zn::inv(const TM2 &t,int n)
{
	TM2 v(4,0);
	int d=det(t,n);
	if(d==0)
		return v;
	int d1=inverse_mod_p(d,n);
	if(d1==0)
		return v;
	v[0]=t[3]*d1;
	v[1]=-t[1]*d1;
	v[2]=-t[2]*d1;
	v[3]=t[0]*d1;
	if(n>0){
		for(int i=0;i<4;i++){
			v[i]=mod(v[i],n);
		}
	}	
	return v;	
}

#endif

