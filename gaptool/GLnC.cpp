#include<iostream>
#include<iomanip> 
#include<complex>
#include"IGroup.h"
using namespace std;

float g_M16[16][8]={
 {1,0,0,1,0,0,0,0},//I=Y^2
 {-1,0,0,-1,0,0,0,0},//-I=X^4
 {0,1,0,0,0,0,1,0},//X
 {0,1,1,0,1,1,-1,-1},//Y
 {0,0,0,0,1,0,0,1},//X^2
 {0,0,-1,0,0,1,0,0},//X^3
 {1,0,-1,-1,-1,-1,0,1},//XY
 {-1,0,1,1,1,1,0,-1},//YX=-XY
 {0,-1,0,0,0,0,-1,0},//-X
 {0,-1,-1,0,-1,-1,1,1},//-Y
 {0,0,0,0,-1,0,0,-1},//-X^2
 {0,0,1,0,0,-1,0,0},//-X^3
 {-1,-1,1,1,0,1,1,0},//X^2Y=YX^2
 {1,1,0,-1,1,0,-1,-1},//X^3Y
 {-1,-1,0,1,-1,0,1,1},//YX^3=-X^3Y
 {1,1,-1,-1,0,-1,-1,0}//-X^2Y=-YX^2
};

float g_P16[16][8]={
 {1,0,0,1,0,0,0,0},//I
 {-1,0,0,-1,0,0,0,0},//-I
 {0,0,0,0,1,0,0,1},//iI
 {0,0,0,0,-1,0,0,-1},//-iI
 {0,1,1,0,0,0,0,0},//X
 {0,-1,-1,0,0,0,0,0},//-X
 {0,0,0,0,0,1,1,0},//iX
 {0,0,0,0,0,-1,-1,0},//-iX
 {0,0,0,0,0,-1,1,0},//Y
 {0,0,0,0,0,1,-1,0},//-Y
 {0,1,-1,0,0,0,0,0},//iY
 {0,-1,1,0,0,0,0,0},//-iY
 {1,0,0,-1,0,0,0,0},//Z
 {-1,0,0,1,0,0,0,0},//-Z
 {0,0,0,0,1,0,0,-1},//iZ
 {0,0,0,0,-1,0,0,1}//-iZ
};	

typedef vector<vector<float> > MATRIXf;
typedef pair<MATRIXf,MATRIXf> CMATRIXf;

// GL(n,C)的N阶有限子群
struct GLnC:public IGroup
{
public:
   //  静态函数
   static CMATRIXf IdentityMat(int n);
   static vector<CMATRIXf> FG(vector<CMATRIXf>& gen);
   static int getidx(vector<CMATRIXf> &Arr2,CMATRIXf &Arr1);
   static bool IsEqual(const MATRIXf &t,const MATRIXf &m);   
   static bool IsEqual(const CMATRIXf &t,const CMATRIXf &m);
   static MATRIXf add(const MATRIXf &t,const MATRIXf &m,int k=1);
   static MATRIXf sub(const MATRIXf &t,const MATRIXf &m);   
   static MATRIXf mul(const MATRIXf &t,const MATRIXf &m);   
   static CMATRIXf mul(const CMATRIXf &t,const CMATRIXf &m);   
   static CMATRIXf inv(const CMATRIXf &t);
   static vector<CMATRIXf> Order(const CMATRIXf & m); 
   static MATRIXf TransposedMat(const MATRIXf &m);   
   static CMATRIXf TransposedMat(const CMATRIXf &m);
   static CMATRIXf ConjugateMat(const CMATRIXf &m);  
   static bool IsUnitaryMat(const CMATRIXf &m);
   static bool IsOrthogonalMat(const CMATRIXf &m);   
   static float TraceMat(const MATRIXf &m);
   static complex<float> TraceMat(const CMATRIXf &m);   
   static GLnC CyclicGroup(int n);
   static GLnC QuaternionGroup(int n);
public:
   // 实现抽象基类的方法
   virtual void printSet();
   virtual void printTable();     
   virtual int mul(int a,int b);
   virtual int size();
   virtual int inv(int a);
   // 构造函数
   GLnC(){}// 默认构造函数
   GLnC(float* set,int n,int N);
   // 成员函数
   bool IsUnitaryGroup()const;  
   bool IsOrthogonalGroup()const;
   // 成员变量
   vector<CMATRIXf> s_Arr;
   int m_n;  
};

CMATRIXf GLnC::IdentityMat(int n){
	MATRIXf m0r(n,vector<float>(n,0));
	MATRIXf m0i(n,vector<float>(n,0));	
	for(int i=0;i<n;i++){
		m0r[i][i]=1;
	}
	CMATRIXf m0=make_pair(m0r,m0i);
	return m0;
}

vector<CMATRIXf> GLnC::FG(vector<CMATRIXf>& gen){
	vector<CMATRIXf> S;
	int n=gen[0].first.size();
	CMATRIXf m0=IdentityMat(n);
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
				CMATRIXf IJ=mul(S[i],S[j]);
				//vector<CMATRIXf>::iterator p=std::find(S.begin(),S.end(),IJ);// 请用下面的find_if语句，用这个会陷入死循环
				vector<CMATRIXf>::iterator p=std::find_if(S.begin(),S.end(),[IJ](CMATRIXf& m)->bool{return IsEqual(m,IJ);});
				if(p==S.end()){
					S.push_back(IJ);
				}
			}
		}
		cnt1=S.size();
	}while(cnt1>cnt);	
   return S;
}

/* 
GL(1,C)的子群圆群的元素可表示为复数cosθ+isinθ或1阶复矩阵{{cosθ+isinθ}}
*/
GLnC GLnC::CyclicGroup(int n){
	MATRIXf mr(1,vector<float>(1,0));
	MATRIXf mi(1,vector<float>(1,0));	
    mr[0][0]=cos(2*3.141592654/n);
    mi[0][0]=sin(2*3.141592654/n);
	CMATRIXf m=make_pair(mr,mi);
	GLnC Cn;
	Cn.s_Arr=Order(m);
    Cn.m_n=1;
    return Cn;
}

GLnC GLnC::QuaternionGroup(int n){
	int n4=n/4;
	vector<CMATRIXf> gen(2);	
	MATRIXf mr(2,vector<float>(2,0));
	MATRIXf mi(2,vector<float>(2,0));	
	// 2n次本原单位根对应的复矩阵
    mr[0][0]=cos(3.141592654/n4);
	mr[0][1]=0;
	mr[1][0]=0;		
    mr[1][1]=cos(3.141592654/n4);
    mi[0][0]=sin(3.141592654/n4);
	mi[0][1]=0;
	mi[1][0]=0;		
    mi[1][1]=-sin(3.141592654/n4);
	gen[0]=make_pair(mr,mi);
    mr[0][0]=0;
	mr[0][1]=-1;
	mr[1][0]=1;		
    mr[1][1]=0;
    mi[0][0]=0;
	mi[0][1]=0;
	mi[1][0]=0;		
    mi[1][1]=0;
	gen[1]=make_pair(mr,mi);
	GLnC Qn;
	Qn.s_Arr=FG(gen);
    Qn.m_n=2;	
	return Qn;
}

// 矩阵D(R)的转置D(R)^T
MATRIXf GLnC::TransposedMat(const MATRIXf &m){
	int nm=m.size();
	MATRIXf t(nm,vector<float>(nm,0));
	for(int i=0;i<nm;i++){
		for(int j=0;j<nm;j++){
			t[i][j]=m[j][i];
		}
	}
    return t;
}

// 矩阵D(R)的转置D(R)^T
CMATRIXf GLnC::TransposedMat(const CMATRIXf &m){
	MATRIXf tr=TransposedMat(m.first);
	MATRIXf ti=TransposedMat(m.second);
    return make_pair(tr,ti);	
}

float GLnC::TraceMat(const MATRIXf &m){
	float Tr=0;
	int nm=m.size();
	for(int i=0;i<nm;i++){
		Tr+=m[i][i];
	}
    return Tr;
}

complex<float> GLnC::TraceMat(const CMATRIXf &m){
	float r=TraceMat(m.first);
	float i=TraceMat(m.second);
    return complex<float>(r,i);	
}

// 矩阵D(R)的复共轭D(R)*
CMATRIXf GLnC::ConjugateMat(const CMATRIXf &m){
	int nm=m.first.size();
	MATRIXf ci(nm,vector<float>(nm,0));	
	ci=sub(ci,m.second);
	return make_pair(m.first,ci);
}

/*
酉矩阵，即逆阵是复共轭转置（或转置复共轭）：D(R)^*D(R)=I,detD(R)=±1，实正交矩阵的一种推广
1902年，L.Autonne给出酉矩阵(幺正矩阵)的概念，即逆矩阵的转置等于它的共轭矩阵；
同年他又得出结论：一个矩阵若满足实矩阵、正交矩阵、酉矩阵中的两个则另一个也成立。
*/
bool GLnC::IsUnitaryMat(const CMATRIXf &m){
	CMATRIXf t=ConjugateMat(TransposedMat(m));// 矩阵D(R)的转置复共轭D(R)^*
	CMATRIXf m1=inv(m);
	bool IsU=IsEqual(t,m1);
	return IsU;
}

// 酉群U(n)即由酉矩阵构成的线性群，U(n)是一个非单连通非半单紧的单Lie群。
bool GLnC::IsUnitaryGroup()const{
    int N=s_Arr.size();
	for(int i=0;i<N;i++){
		if(!IsUnitaryMat(s_Arr[i]))
			return false;
	}
	return true;
}

// 复正交矩阵：D(R)^TD(R)=I,detD(R)=±1,实正交矩阵的另一种推广
bool GLnC::IsOrthogonalMat(const CMATRIXf &m){
	CMATRIXf t=TransposedMat(m);
	CMATRIXf m1=inv(m);
	bool IsO=IsEqual(t,m1);
	return IsO;
}

bool GLnC::IsOrthogonalGroup()const{
    int N=s_Arr.size();
	for(int i=0;i<N;i++){
		if(!IsOrthogonalMat(s_Arr[i]))
			return false;
	}
	return true;
}

int GLnC::getidx(vector<CMATRIXf> &Arr2,CMATRIXf &Arr1){
	int N=Arr2.size();
	for(int i=0;i<N;i++){
		if(IsEqual(Arr2[i],Arr1))
			return i;
	}
	return -1;
}

bool GLnC::IsEqual(const MATRIXf &t,const MATRIXf &m){
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

bool GLnC::IsEqual(const CMATRIXf &t,const CMATRIXf &m){
	bool bE=IsEqual(t.first,m.first)&&IsEqual(t.second,m.second);
	return bE;
}

vector<CMATRIXf> GLnC::Order(const CMATRIXf & m){
	vector<CMATRIXf> ret;
	int nm=m.first.size();
	CMATRIXf mi=m;
	MATRIXf m0r(nm,vector<float>(nm,0));
	MATRIXf m0i(nm,vector<float>(nm,0));	
	for(int i=0;i<nm;i++){
		m0r[i][i]=1;
	}
	CMATRIXf m0=make_pair(m0r,m0i);
    ret.push_back(m0);
	while(!IsEqual(mi,m0)){
		ret.push_back(mi);
		mi=mul(mi,m);
	}
	return ret;
}

CMATRIXf GLnC::inv(const CMATRIXf &t){
	vector<CMATRIXf> S1=Order(t);
	int ord=S1.size();
	return S1[ord-1];	
}

/*
复矩阵乘法(A_1,A_2)×(B_1,B_2)=(A_1B_1-A_2B_2,A_1B_2+A_2B_1)=>二维实Banach代数中的乘法（复数乘法）：(a_1,a_2)×(b_1,b_2)=(a_1b_1-a_2b_2,a_1b_2+a_2b_1)。
*/
CMATRIXf GLnC::mul(const CMATRIXf &t,const CMATRIXf &m){
	int nt=t.first.size();
	int nm=m.first.size();
	MATRIXf cr(nm,vector<float>(nm,0));
	MATRIXf ci(nm,vector<float>(nm,0));	
	if(nt!=nm)
		return make_pair(cr,ci);
	cr=sub(mul(t.first,m.first),mul(t.second,m.second));
	ci=add(mul(t.first,m.second),mul(t.second,m.first));	
	return make_pair(cr,ci);
}

MATRIXf GLnC::add(const MATRIXf &t,const MATRIXf &m,int k){
	int nt=t.size();
	int nm=m.size();
	MATRIXf c(nm,vector<float>(nm,0));
	if(nt!=nm)
		return c;
	for (int i=0;i<nm;i++){
		for (int j=0;j<nm;j++){ 
			c[i][j]=t[i][j]+k*m[i][j];
		}
	}
	return c;
}

MATRIXf GLnC::sub(const MATRIXf &t,const MATRIXf &m){
	return add(t,m,-1);
}

MATRIXf GLnC::mul(const MATRIXf &t,const MATRIXf &m){
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

GLnC::GLnC(float* set,int n,int N){
	m_n=n;
	MATRIXf vr(n,vector<float>(n,0));
	MATRIXf vi(n,vector<float>(n,0));	
	for(int i=0;i<N;i++){
		for(int j=0;j<n;j++){
			for(int k=0;k<n;k++){
				vr[j][k]=*(set+n*n*2*i+n*j+k);
				vi[j][k]=*(set+n*n*2*i+n*n+n*j+k);				
			}			
		}
		s_Arr.push_back(make_pair(vr,vi));
	}
}

void GLnC::printSet(){
	bool bU=IsUnitaryGroup();
	bool bO=IsOrthogonalGroup();
	int ID=IdGroup(this);
	printf("GL(%d,C)的子群GAP[%d,%d]:bU=%d,bO=%d\n",m_n,size(),ID,bU,bO);
	for(int i=0;i<size();i++){
		int i1=inv(i);
		complex<float> tr=TraceMat(s_Arr[i]);
		complex<float> tr1=TraceMat(s_Arr[i1]);		
		cout<<setiosflags(ios::fixed)<<i<<",Tr="<<setprecision(3)<<tr<<"=>"<<i1<<",Tr="<<tr1<<endl;
	}
}

void GLnC::printTable(){
   int ID=IdGroup(this);	
   printf("GL(%d,C)的子群GAP[%d,%d]:\n",m_n,size(),ID);
   printGroup(this);
}

int GLnC::mul(int a,int b){
	CMATRIXf A=s_Arr[a];
	CMATRIXf B=s_Arr[b];
	CMATRIXf C=mul(A,B);
	int idx=getidx(s_Arr,C);
	return idx;
}

int GLnC::inv(int a){
	CMATRIXf v=inv(s_Arr[a]);
	int idx=getidx(s_Arr,v);
	return idx;
}

int GLnC::size(){
	return s_Arr.size();	
}

int main(){
	GLnC Q8=GLnC::QuaternionGroup(8);
	Q8.printSet();	
	Q8.printTable();	
	
	GLnC C4=GLnC::CyclicGroup(4);
	C4.printSet();	
	C4.printTable();
	
	GLnC C6=GLnC::CyclicGroup(6);
	C6.printSet();	
	C6.printTable();
	
	GLnC _M16(&g_M16[0][0],2,sizeof(g_M16)/sizeof(g_M16[0]));
	_M16.printSet();
	_M16.printTable();	
	
	GLnC _P16(&g_P16[0][0],2,sizeof(g_P16)/sizeof(g_P16[0]));
	_P16.printSet();
	_P16.printTable();		
	
	return 0;
}	