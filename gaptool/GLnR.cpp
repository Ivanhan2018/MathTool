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

// S_3��2ά����Լ��ʾ
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

// GL(n,R)��N��������Ⱥ
struct GLnR:public IGroup
{
public:
   //  ��̬����
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
public:
   // ʵ�ֳ������ķ���
   virtual void printSet();
   virtual void printTable();     
   virtual int mul(int a,int b);
   virtual int size();
   virtual int inv(int a);
   // ���캯��
   GLnR(){}// Ĭ�Ϲ��캯��
   GLnR(float* set,int n,int N);
   // ��Ա����
   bool IsOrthogonalGroup()const;
   // ��Ա����
   vector<MATRIXf> s_Arr;
   int m_n;  
};

/* 
SO(2,R)����ȺԲȺ��Ԫ�ؿɱ�ʾΪ����cos��+isin�Ȼ����{{cos��,-sin��},{sin��,cos��}},����a+bi��ʾΪ����{{a,-b},{b,a}},�����ĳ˷���ʾΪ����ĳ˷�
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

// ʵ������ʾ����ת�þ�����������
bool GLnR::IsOrthogonalMat(const MATRIXf &m){
	MATRIXf t=TransposedMat(m);
	MATRIXf m1=inv(m);
	bool IsO=IsEqual(t,m1);
	return IsO;	
}

/*
������O(n)Ϊn*n����ʵ������ɵ�Ⱥ���κ�n�׵�����Ⱥͬ����O(n)��һ����Ⱥ��
�κ�����Ⱥ����S_n����Ⱥ����S_n����O(n)����Ⱥ���κ�����Ⱥ����A_n����Ⱥ��
*/
bool GLnR::IsOrthogonalGroup()const{
    int N=s_Arr.size();
	for(int i=0;i<N;i++){
		if(!IsOrthogonalMat(s_Arr[i]))
			return false;
	}
	return true;
}

// ��ȫѡ��Ԫ��˹��ȥ��������ʽ��ֵ
float GLnR::DeterminantMat(const MATRIXf &m){
    // �ֲ�����
    MATRIXf mtxA=m;	
	int n=m.size();
    int nIs, nJs;
    float q,d;
    float f = 1;
    float det = 1;

    // ѡ��Ԫ
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

        // ���ʧ��
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

        // ����
        if (nJs != k){
            f = -f;
            for(int i = k;i<=n;i++){
                d = mtxA[i-1][nJs-1];
                mtxA[i-1][nJs-1] = mtxA[i-1][k-1];
                mtxA[i-1][k-1] = d;
			}
        }

        // ��������ʽ��ֵ
        det = det * mtxA[k-1][k-1];
        
        // ��������Ϊ�����Ǿ���
        for(int i = k+1;i<=n;i++){
            d = mtxA[i-1][k-1] / mtxA[k-1][k-1];
            for(int j = k+1;j<=n;j++){
                mtxA[i-1][j-1] = mtxA[i-1][j-1] - d * mtxA[k-1][j-1];
            }
        }
	}//end for k

    // ��������ʽ��ֵ
    det = f * det * mtxA[n-1][n-1];

    // ���ɹ�
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
	printf("GL(%d,R)����ȺGAP[%d,%d]:bO=%d\n",m_n,size(),ID,bO);
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
   printf("GL(%d,R)����ȺGAP[%d,%d]:\n",m_n,size(),ID);   
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