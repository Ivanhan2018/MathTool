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

struct E8:public IGroup
{
public:
   static int s_Arr[][3];
public:
   virtual void printSet();
   virtual void printTable();     
   virtual int mul(int a,int b);
   virtual int size();
   virtual int inv(int a);
};

int E8::s_Arr[][3]={ \
	{0,0,0}, \
	{1,1,1}, \
	{0,0,1}, \
	{0,1,0}, \
	{0,1,1}, \
	{1,0,0}, \
	{1,0,1}, \
	{1,1,0}, \
};

void E8::printSet()
{
	//printf("GAP[8,5]=E_8:\n");
	int ID=IdGroup(this);	
	printf("E_8=GAP[%d,%d]:\n",size(),ID);   
	for(int i=0;i<8;i++){
		int i1=inv(i);
		int *pi1=s_Arr[i1];
		printf("%d->[%d,%d,%d]=>%d->[%d,%d,%d]\n",i,s_Arr[i][0],s_Arr[i][1],s_Arr[i][2],i1,pi1[0],pi1[1],pi1[2]);
	}
}

void E8::printTable()
{
   //printf("GAP[8,5]=E_8:\n");
   int ID=IdGroup(this);	
   printf("E_8=GAP[%d,%d]:\n",size(),ID);      
   printGroup(this);
}     

int E8::mul(int a,int b)
{
	int ret[]={0,0,0};
	int *A=s_Arr[a];
	int *B=s_Arr[b];
	for(int i=0;i<3;i++){	
		ret[i]=(A[i])^(B[i]);
		//ret[i]=((*(A+i))+(*(B+i)))%2;
	}
    int n=size();
	int idx=getidx(&s_Arr[0][0],n,3,&ret[0]);
	return idx;	
}

int E8::size()
{
	//int n=sizeof(s_Arr)/sizeof(s_Arr[0]);
	return 8;	
}

int E8::inv(int a)
{
	int ret[]={0,0,0};
	int *A=s_Arr[a];
	for(int i=0;i<3;i++){	
		ret[i]=(2-*(A+i))%2;
	}
    int n=size();
	int idx=getidx(&s_Arr[0][0],n,3,&ret[0]);
	return idx;
}

// GL(2,n)
struct GL2Zn:public IGroup
{
public:
   //  静态函数
   static vector<int> mul(const vector<int> &t,const vector<int> &m,int n);  
   static vector<int> inv(const vector<int> &t,int n);   
   static int det(const vector<int> &t,int n);  
public:
   // 实现抽象基类的方法
   virtual void printSet();
   virtual void printTable();     
   virtual int mul(int a,int b);
   virtual int size();
   virtual int inv(int a);
   // 构造函数
   GL2Zn(int n);
   // 成员函数
   // 成员变量
   vector<vector<int> > s_Arr;   
   int m_n;   
};

GL2Zn::GL2Zn(int n)
{
    m_n=n;
	vector<int> v(4);
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
	int ID=IdGroup(this);	
	printf("GL(2,%d)=GAP[%d,%d]:\n",m_n,size(),ID);
	for(int i=0;i<size();i++){
		vector<int> v1=inv(s_Arr[i],m_n);
		int i1=getidx(s_Arr,v1);
		printf("%d->[[%d,%d],[%d,%d]]=>%d->[[%d,%d],[%d,%d]]\n",i,s_Arr[i][0],s_Arr[i][1],s_Arr[i][2],s_Arr[i][3],i1,v1[0],v1[1],v1[2],v1[3]);
	}
}

void GL2Zn::printTable()
{
   int ID=IdGroup(this);	
   printf("GL(2,%d)=GAP[%d,%d]:\n",m_n,size(),ID);      
   printGroup(this);
}

int GL2Zn::mul(int a,int b)
{
	vector<int> A=s_Arr[a];
	vector<int> B=s_Arr[b];
	vector<int> C=mul(A,B,m_n);
	int idx=getidx(s_Arr,C);
	return idx;
}

int GL2Zn::inv(int a)
{
	vector<int> v=inv(s_Arr[a],m_n);
	int idx=getidx(s_Arr,v);
	return idx;
}

int GL2Zn::size()
{
	return s_Arr.size();	
}

vector<int> GL2Zn::mul(const vector<int> &t,const vector<int> &m,int n)
{
	vector<int> v(4);
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

int GL2Zn::det(const vector<int> &t,int n)
{
	int ret=t[0]*t[3]-t[1]*t[2];
	if(n>0){
		ret=mod(ret,n);
	}
	return ret;
}

vector<int> GL2Zn::inv(const vector<int> &t,int n)
{
	vector<int> v(4,0);
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

int main(){
	
    E8 e8;
	e8.printSet();	
    e8.printTable();
	
	GL2Zn gl2Z2(2);
	gl2Z2.printSet();
	gl2Z2.printTable();
	
	GL2Zn gl2Z3(3);
	gl2Z3.printSet();		
	gl2Z3.printTable();	
	
	return 0;
}