#include<iostream>
#include"IGroup.h"
using namespace std;

float g_S4[24][4][4]=
{ 
{ {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}},
{ {1,0,0,0},{0,1,0,0},{0,0,0,1},{0,0,1,0}},
{ {1,0,0,0},{0,0,1,0},{0,1,0,0},{0,0,0,1}},
{ {1,0,0,0},{0,0,0,1},{0,1,0,0},{0,0,1,0}},
{ {1,0,0,0},{0,0,1,0},{0,0,0,1},{0,1,0,0}},
{ {1,0,0,0},{0,0,0,1},{0,0,1,0},{0,1,0,0}},
{ {0,1,0,0},{1,0,0,0},{0,0,1,0},{0,0,0,1}},
{ {0,1,0,0},{1,0,0,0},{0,0,0,1},{0,0,1,0}},
{ {0,0,1,0},{1,0,0,0},{0,1,0,0},{0,0,0,1}},
{ {0,0,0,1},{1,0,0,0},{0,1,0,0},{0,0,1,0}},
{ {0,0,1,0},{1,0,0,0},{0,0,0,1},{0,1,0,0}},
{ {0,0,0,1},{1,0,0,0},{0,0,1,0},{0,1,0,0}},
{ {0,1,0,0},{0,0,1,0},{1,0,0,0},{0,0,0,1}},
{ {0,1,0,0},{0,0,0,1},{1,0,0,0},{0,0,1,0}},
{ {0,0,1,0},{0,1,0,0},{1,0,0,0},{0,0,0,1}},
{ {0,0,0,1},{0,1,0,0},{1,0,0,0},{0,0,1,0}},
{ {0,0,1,0},{0,0,0,1},{1,0,0,0},{0,1,0,0}},
{ {0,0,0,1},{0,0,1,0},{1,0,0,0},{0,1,0,0}},
{ {0,1,0,0},{0,0,1,0},{0,0,0,1},{1,0,0,0}},
{ {0,1,0,0},{0,0,0,1},{0,0,1,0},{1,0,0,0}},
{ {0,0,1,0},{0,1,0,0},{0,0,0,1},{1,0,0,0}},
{ {0,0,0,1},{0,1,0,0},{0,0,1,0},{1,0,0,0}},
{ {0,0,1,0},{0,0,0,1},{0,1,0,0},{1,0,0,0}},
{ {0,0,0,1},{0,0,1,0},{0,1,0,0},{1,0,0,0}},
};

typedef vector<vector<float> > MATRIX4;

// O(4,R)的n阶有限子群
struct O4R:public IGroup
{
public:
   //  静态函数
   static int getidx(vector<MATRIX4> &Arr2,MATRIX4 &Arr1);
   static bool IsEqual(const MATRIX4 &t,const MATRIX4 &m);
   static MATRIX4 mul(const MATRIX4 &t,const MATRIX4 &m);
   static MATRIX4 inv(const MATRIX4 &t);
   static vector<MATRIX4> Order(const MATRIX4 & m);   
public:
   // 实现抽象基类的方法
   virtual void printSet();
   virtual void printTable();     
   virtual int mul(int a,int b);
   virtual int size();
   virtual int inv(int a);
   // 构造函数
   O4R(float* set,int n);
   // 成员函数
   // 成员变量
   vector<MATRIX4> s_Arr;  
};

#include"windows.h"
int O4R::getidx(vector<MATRIX4> &Arr2,MATRIX4 &Arr1){
	char sz[1000]={0};
		sprintf(sz,"Arr1->[[%.0f,%.0f,%.0f,%.0f],[%.0f,%.0f,%.0f,%.0f],[%.0f,%.0f,%.0f,%.0f],[%.0f,%.0f,%.0f,%.0f]]\n" \
		,Arr1[0][0],Arr1[0][1],Arr1[0][2],Arr1[0][3] \
		,Arr1[1][0],Arr1[1][1],Arr1[1][2],Arr1[1][3] \
		,Arr1[2][0],Arr1[2][1],Arr1[2][2],Arr1[2][3] \
		,Arr1[3][0],Arr1[3][1],Arr1[3][2],Arr1[3][3] \
		);
	OutputDebugStringA(sz);
	int n=Arr2.size();
	for(int i=0;i<n;i++){
		if(IsEqual(Arr2[i],Arr1))
			return i;
	}
	return -1;
}

bool O4R::IsEqual(const MATRIX4 &t,const MATRIX4 &m){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(abs(t[i][j]-m[i][j])>0.01)
				return false;
		}
	}
	return true;
}

vector<MATRIX4> O4R::Order(const MATRIX4 & m){
	vector<MATRIX4> ret;
	MATRIX4 mi=m;
	MATRIX4 m0(4,vector<float>(4,0));
	for(int i=0;i<4;i++){
		m0[i][i]=1;
	}
    ret.push_back(mi);
	while(!IsEqual(mi,m0)){
		ret.push_back(mi);
		mi=mul(mi,m);
	}
	return ret;
}

MATRIX4 O4R::inv(const MATRIX4 &t){
	vector<MATRIX4> S1=Order(t);
	int ord=S1.size();
	MATRIX4 v=t;
	char sz[1000]={0};
		sprintf(sz,"v->[[%.0f,%.0f,%.0f,%.0f],[%.0f,%.0f,%.0f,%.0f],[%.0f,%.0f,%.0f,%.0f],[%.0f,%.0f,%.0f,%.0f]]\n" \
		,v[0][0],v[0][1],v[0][2],v[0][3] \
		,v[1][0],v[1][1],v[1][2],v[1][3] \
		,v[2][0],v[2][1],v[2][2],v[2][3] \
		,v[3][0],v[3][1],v[3][2],v[3][3] \
		);
	OutputDebugStringA(sz);
	return S1[ord-1];	
}

MATRIX4 O4R::mul(const MATRIX4 &t,const MATRIX4 &m){
	MATRIX4 c(4,vector<float>(4,0));
	int nt=t.size();
	int nm=m.size();
	if(nt!=4 || nm!=4)
		return c;
	for (int i=0;i<4;i++){
		for (int j=0;j<4;j++){ 
			c[i][j]=0.0;
			for(int l=0;l<4;l++)
				c[i][j]=c[i][j]+t[i][l]*m[l][j];
		}
	}
	return c;
}

O4R::O4R(float* set,int n){
	MATRIX4 v(4,vector<float>(4,0));
	for(int i=0;i<n;i++){
		for(int j=0;j<4;j++){
			for(int k=0;k<4;k++){
				v[j][k]=*(set+16*i+4*j+k);						
			}			
		}
		s_Arr.push_back(v);
	//char sz[1000]={0};
	//	sprintf(sz,"v->[[%.1f,%.1f,%.1f,%.1f],[%.1f,%.1f,%.1f,%.1f],[%.1f,%.1f,%.1f,%.1f],[%.1f,%.1f,%.1f,%.1f]]\n" \
	//	,v[0][0],v[0][1],v[0][2],v[0][3] \
	//	,v[1][0],v[1][1],v[1][2],v[1][3] \
	//	,v[2][0],v[2][1],v[2][2],v[2][3] \
	//	,v[3][0],v[3][1],v[3][2],v[3][3] \
	//	);
	//OutputDebugStringA(sz);
	}
}

void O4R::printSet(){
	printf("O(4,R)的%d阶子群:\n",size());
	for(int i=0;i<size();i++){
		int i1=inv(i);
		printf("%d=>%d->[[%.0f,%.0f,%.0f,%.0f],[%.0f,%.0f,%.0f,%.0f],[%.0f,%.0f,%.0f,%.0f],[%.0f,%.0f,%.0f,%.0f]]\n",i,i1 \
		,s_Arr[i1][0][0],s_Arr[i1][0][1],s_Arr[i1][0][2],s_Arr[i1][0][3] \
		,s_Arr[i1][1][0],s_Arr[i1][1][1],s_Arr[i1][1][2],s_Arr[i1][1][3] \
		,s_Arr[i1][2][0],s_Arr[i1][2][1],s_Arr[i1][2][2],s_Arr[i1][2][3] \
		,s_Arr[i1][3][0],s_Arr[i1][3][1],s_Arr[i1][3][2],s_Arr[i1][3][3] \
		);
	}
}

void O4R::printTable(){
   printf("O(4,R)的%d阶子群:\n",size());
   printGroup(this);
}

int O4R::mul(int a,int b){
	MATRIX4 A=s_Arr[a];
	MATRIX4 B=s_Arr[b];
	MATRIX4 C=mul(A,B);
	int idx=getidx(s_Arr,C);
	return idx;
}

int O4R::inv(int a){
	MATRIX4 v=inv(s_Arr[a]);
	int idx=getidx(s_Arr,v);
	return idx;
}

int O4R::size(){
	return s_Arr.size();	
}

int main(){
	O4R _S4(&g_S4[0][0][0],sizeof(g_S4)/sizeof(g_S4[0]));
	_S4.printSet();
	_S4.printTable();
	
	return 0;
}	