#include<iostream>
#include<iomanip> 
#include<complex>
#include"IGroup.h"
using namespace std;

float g_S3[6][2][2]=
{
	{{1,0},{0,1}},//I
	{{-1.f/2,sqrtf(3)/2},{-sqrtf(3)/2,-1.f/2}},//r
	{{-1.f/2,-sqrtf(3)/2},{sqrtf(3)/2,-1.f/2}},//r^2
	{{1,0},{0,-1}},//f
	{{-1.f/2,sqrtf(3)/2},{sqrtf(3)/2,1.f/2}},//fr
	{{-1.f/2,-sqrtf(3)/2},{-sqrtf(3)/2,1.f/2}},//fr^2
};

// S_3的2维不可约表示
float g_S3a[6][2][2]=
{
	{{1,0},{0,1}},
	{{-1,1},{0,1}},
	{{1,0},{1,-1}},
	{{0,-1},{-1,0}},
	{{-1,1},{-1,0}},
	{{0,-1},{1,-1}},
};	

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

typedef vector<vector<float> > MATRIXf;

// GL(n,R)的N阶有限子群
struct GLnR:public IGroup
{
public:
   //  静态函数
   static MATRIXf IdentityMat(int n);   
   static vector<MATRIXf> FG(vector<MATRIXf>& gen);   
   static int getidx(vector<MATRIXf> &Arr2,MATRIXf &Arr1);
   static bool IsEqual(const MATRIXf &t,const MATRIXf &m);
   static MATRIXf mul(const MATRIXf &t,const MATRIXf &m);
   static MATRIXf inv(const MATRIXf &t);
   static vector<MATRIXf> Order(const MATRIXf & m); 
   static MATRIXf TransposedMat(const MATRIXf &m);
   static bool IsOrthogonalMat(const MATRIXf &m);
   static float DeterminantMat(const MATRIXf &m);
   static float TraceMat(const MATRIXf &m);   
   static GLnR CyclicGroup(int n);
   static GLnR D2nGroup(int n);   
   static GLnR TestGroup();   
public:
   // 实现抽象基类的方法
   virtual void printSet();
   virtual void printTable();     
   virtual int mul(int a,int b);
   virtual int size();
   virtual int inv(int a);
   // 构造函数
   GLnR(){}// 默认构造函数
   GLnR(float* set,int n,int N);
   // 成员函数
   bool IsOrthogonalGroup()const;
   // 成员变量
   vector<MATRIXf> s_Arr;
   int m_n;  
};

MATRIXf GLnR::IdentityMat(int n){
	MATRIXf m0r(n,vector<float>(n,0));	
	for(int i=0;i<n;i++){
		m0r[i][i]=1;
	}
	return m0r;
}

vector<MATRIXf> GLnR::FG(vector<MATRIXf>& gen){
	vector<MATRIXf> S;
	int n=gen[0].size();
	MATRIXf m0=IdentityMat(n);
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
				MATRIXf IJ=mul(S[i],S[j]);
				//vector<MATRIXf>::iterator p=std::find(S.begin(),S.end(),IJ);// 请用下面的find_if语句，用这个会陷入死循环
				vector<MATRIXf>::iterator p=std::find_if(S.begin(),S.end(),[IJ](MATRIXf& m)->bool{return IsEqual(m,IJ);});
				if(p==S.end()){
					S.push_back(IJ);
				}
			}
		}
		cnt1=S.size();
	}while(cnt1>cnt);	
   return S;
}

GLnR GLnR::TestGroup(){
	MATRIXf a(3,vector<float>(3,0));
	MATRIXf b(3,vector<float>(3,0));
	MATRIXf c(3,vector<float>(3,0));	
	//{-1,0,0,0,-1,0,0,0,-1}
    a[0][0]=-1;
	a[0][1]=0;
    a[0][2]=0;
    a[1][0]=0;
	a[1][1]=-1;
    a[1][2]=0;
    a[2][0]=0;
	a[2][1]=0;
    a[2][2]=-1;	
	//{0,-1,0,1,0,0,0,0,-1}
    b[0][0]=0;
	b[0][1]=-1;
    b[0][2]=0;
    b[1][0]=1;
	b[1][1]=0;
    b[1][2]=0;
    b[2][0]=0;
	b[2][1]=0;
    b[2][2]=-1;	
	//{0,0,-1,0,-1,0,1,0,0}
    c[0][0]=0;
	c[0][1]=0;
    c[0][2]=-1;
    c[1][0]=0;
	c[1][1]=-1;
    c[1][2]=0;
    c[2][0]=1;
	c[2][1]=0;
    c[2][2]=0;		
	vector<MATRIXf> gen;
	gen.push_back(a);
	gen.push_back(b);
	//gen.push_back(c);	
	GLnR G;
	G.s_Arr=FG(gen);
    G.m_n=3;
    return G;
}

GLnR GLnR::D2nGroup(int n){
	MATRIXf a(2,vector<float>(2,0));
	MATRIXf b(2,vector<float>(2,0));	
    a[0][0]=cos(2*3.141592654/n);
	a[0][1]=-sin(2*3.141592654/n);
    a[1][0]=-a[0][1];
	a[1][1]=a[0][0];
    b[0][0]=-1;
	b[0][1]=0;
    b[1][0]=0;
	b[1][1]=1;	
	vector<MATRIXf> gen;
	gen.push_back(a);
	gen.push_back(b);
	GLnR G;
	G.s_Arr=FG(gen);
    G.m_n=2;
    return G;
}

/* 
SO(2,R)的子群圆群的元素可表示为复数cosθ+isinθ或矩阵{{cosθ,-sinθ},{sinθ,cosθ}},复数a+bi表示为矩阵{{a,-b},{b,a}},复数的乘法表示为矩阵的乘法
*/
GLnR GLnR::CyclicGroup(int n){
	MATRIXf m(2,vector<float>(2,0));
    m[0][0]=cos(2*3.141592654/n);
	m[1][1]=m[0][0];
    m[0][1]=-sin(2*3.141592654/n);
	m[1][0]=-m[0][1];
	GLnR Cn;
	Cn.s_Arr=Order(m);
    Cn.m_n=2;
    return Cn;
}

MATRIXf GLnR::TransposedMat(const MATRIXf &m){
	int nm=m.size();
	MATRIXf t(nm,vector<float>(nm,0));
	for(int i=0;i<nm;i++){
		for(int j=0;j<nm;j++){
			t[i][j]=m[j][i];
		}
	}
    return t;	
}

float GLnR::TraceMat(const MATRIXf &m){
	float Tr=0;
	int nm=m.size();
	for(int i=0;i<nm;i++){
		Tr+=m[i][i];
	}
    return Tr;	
}

// 实正交表示就是转置矩阵等于逆矩阵。
bool GLnR::IsOrthogonalMat(const MATRIXf &m){
	MATRIXf t=TransposedMat(m);
	MATRIXf m1=inv(m);
	bool IsO=IsEqual(t,m1);
	return IsO;	
}

/*
定理：设O(n)为n*n正交实矩阵组成的群，任何n阶的有限群同构于O(n)的一个子群。
任何有限群都是S_n的子群，而S_n又是O(n)的子群，任何有限群都是A_n的子群吗？
*/
bool GLnR::IsOrthogonalGroup()const{
    int N=s_Arr.size();
	for(int i=0;i<N;i++){
		if(!IsOrthogonalMat(s_Arr[i]))
			return false;
	}
	return true;
}

// 用全选主元高斯消去法求行列式的值
float GLnR::DeterminantMat(const MATRIXf &m){
    // 局部变量
    MATRIXf mtxA=m;	
	int n=m.size();
    int nIs, nJs;
    float q,d;
    float f = 1;
    float det = 1;

    // 选主元
    for(int k = 1;k<=n - 1;k++){
        q = 0;
        for(int i = k;i<=n;i++){
            for(int j = k;j<=n;j++){
                d = abs(mtxA[i-1][j-1]);
                if (d > q){
                    q = d;
                    nIs = i;
                    nJs = j;
                }
            }
        }//end for i

        // 求解失败
        if (q + 1 == 1) {
            return 0;
        }

        if (nIs != k) {
            f = -f;
            for(int j = k;j<=n;j++){
                d = mtxA[k-1][j-1];
                mtxA[k-1][j-1] = mtxA[nIs-1][j-1];
                mtxA[nIs-1][j-1] = d;
            }
        }

        // 调整
        if (nJs != k){
            f = -f;
            for(int i = k;i<=n;i++){
                d = mtxA[i-1][nJs-1];
                mtxA[i-1][nJs-1] = mtxA[i-1][k-1];
                mtxA[i-1][k-1] = d;
			}
        }

        // 计算行列式的值
        det = det * mtxA[k-1][k-1];
        
        // 调整方阵为上三角矩阵
        for(int i = k+1;i<=n;i++){
            d = mtxA[i-1][k-1] / mtxA[k-1][k-1];
            for(int j = k+1;j<=n;j++){
                mtxA[i-1][j-1] = mtxA[i-1][j-1] - d * mtxA[k-1][j-1];
            }
        }
	}//end for k

    // 计算行列式的值
    det = f * det * mtxA[n-1][n-1];

    // 求解成功
    return det;	
}

int GLnR::getidx(vector<MATRIXf> &Arr2,MATRIXf &Arr1){
	int N=Arr2.size();
	for(int i=0;i<N;i++){
		if(IsEqual(Arr2[i],Arr1))
			return i;
	}
	return -1;
}

bool GLnR::IsEqual(const MATRIXf &t,const MATRIXf &m){
	int nt=t.size();
	int nm=m.size();
	if(nt!=nm)
		return false;
	for(int i=0;i<nt;i++){
		for(int j=0;j<nt;j++){
			if(abs(t[i][j]-m[i][j])>0.01)
				return false;
		}
	}
	return true;
}

vector<MATRIXf> GLnR::Order(const MATRIXf & m){
	vector<MATRIXf> ret;
	int nm=m.size();
	MATRIXf mi=m;
	MATRIXf m0(nm,vector<float>(nm,0));
	for(int i=0;i<nm;i++){
		m0[i][i]=1;
	}
    ret.push_back(m0);
	while(!IsEqual(mi,m0)){
		ret.push_back(mi);
		mi=mul(mi,m);
	}
	return ret;
}

MATRIXf GLnR::inv(const MATRIXf &t){
	vector<MATRIXf> S1=Order(t);
	int ord=S1.size();
	return S1[ord-1];	
}

MATRIXf GLnR::mul(const MATRIXf &t,const MATRIXf &m){
	int nt=t.size();
	int nm=m.size();
	MATRIXf c(nm,vector<float>(nm,0));
	if(nt!=nm)
		return c;
	for (int i=0;i<nm;i++){
		for (int j=0;j<nm;j++){ 
			c[i][j]=0.0;
			for(int l=0;l<nm;l++)
				c[i][j]=c[i][j]+t[i][l]*m[l][j];
		}
	}
	return c;
}

GLnR::GLnR(float* set,int n,int N){
	m_n=n;
	MATRIXf v(n,vector<float>(n,0));
	for(int i=0;i<N;i++){
		for(int j=0;j<n;j++){
			for(int k=0;k<n;k++){
				v[j][k]=*(set+n*n*i+n*j+k);						
			}			
		}
		s_Arr.push_back(v);
	}
}

void GLnR::printSet(){
	bool bO=IsOrthogonalGroup();
	int ID=IdGroup(this);
	printf("GL(%d,R)的子群GAP[%d,%d]:bO=%d\n",m_n,size(),ID,bO);
	for(int i=0;i<size();i++){
		int i1=inv(i);
		float det=DeterminantMat(s_Arr[i]);
		float det1=DeterminantMat(s_Arr[i1]);
		float tr=TraceMat(s_Arr[i]);
		float tr1=TraceMat(s_Arr[i1]);		
		printf("%d->Det=%.0f,Tr=%.3f=>%d->Det=%.0f,Tr=%.3f\n",i,det,tr,i1,det1,tr1);
	}
}

void GLnR::printTable(){
   int ID=IdGroup(this);	
   printf("GL(%d,R)的子群GAP[%d,%d]:\n",m_n,size(),ID);   
   printGroup(this);
}

int GLnR::mul(int a,int b){
	MATRIXf A=s_Arr[a];
	MATRIXf B=s_Arr[b];
	MATRIXf C=mul(A,B);
	int idx=getidx(s_Arr,C);
	return idx;
}

int GLnR::inv(int a){
	MATRIXf v=inv(s_Arr[a]);
	int idx=getidx(s_Arr,v);
	return idx;
}

int GLnR::size(){
	return s_Arr.size();	
}

int main(){
	GLnR D3=GLnR::D2nGroup(3);
	D3.printSet();	
	D3.printTable();
	
	GLnR D24=GLnR::D2nGroup(24);
	D24.printSet();	
	D24.printTable();	
    return 0;	
	
	GLnR G=GLnR::TestGroup();
	G.printSet();	
	G.printTable();
    return 0;
	
	GLnR C4=GLnR::CyclicGroup(4);
	C4.printSet();	
	C4.printTable();
	
	GLnR C6=GLnR::CyclicGroup(6);
	C6.printSet();	
	C6.printTable();	
	
	GLnR _S3a(&g_S3a[0][0][0],2,sizeof(g_S3a)/sizeof(g_S3a[0]));
	_S3a.printSet();
	_S3a.printTable();
	
	vector<int> vA3a;
	for(int i=0;i<_S3a.size();i++){
		float det=GLnR::DeterminantMat(_S3a.s_Arr[i]);
		if(abs(det-1)<0.01){
			vA3a.push_back(i);
		}
	}
	vector<int> fgA3a=FG(&_S3a,vA3a);
	printSubGroup(&_S3a,fgA3a);
	
	GLnR _S3(&g_S3[0][0][0],2,sizeof(g_S3)/sizeof(g_S3[0]));
	_S3.printSet();
	_S3.printTable();
	
	vector<int> vA3;
	for(int i=0;i<_S3.size();i++){
		float det=GLnR::DeterminantMat(_S3.s_Arr[i]);
		if(abs(det-1)<0.01){
			vA3.push_back(i);
		}
	}
	vector<int> fgA3=FG(&_S3,vA3);
	printSubGroup(&_S3,fgA3);	
	
	GLnR _S4(&g_S4[0][0][0],4,sizeof(g_S4)/sizeof(g_S4[0]));
	_S4.printSet();
	_S4.printTable();
	
	vector<int> vA4;
	for(int i=0;i<_S4.size();i++){
		float det=GLnR::DeterminantMat(_S4.s_Arr[i]);
		if(abs(det-1)<0.01){
			vA4.push_back(i);
		}
	}
	vector<int> fgA4=FG(&_S4,vA4);
	printSubGroup(&_S4,fgA4);
	
	return 0;
}	