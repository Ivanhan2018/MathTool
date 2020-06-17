#include"quotientRing.h"
#include <ctime>
#include <fstream>

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
	// 析构函数
	~ZmodnZ(){};	
	// 成员函数	
	// 成员变量
	int m_m;
	int m_n;// 运算为模n加法和模n乘法	
};

void ZmodnZ::printTable()
{
	int ID=IdRing(this);
	string str=calcRingInvariant(this);
	printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",size(),ID,str.c_str());	
	//string S1=calS1(this,true);
	//string undoS1=calS1(this,false);	
	//printf("R%d_%d:S1=%s,S1(未经处理)=%s\n",size(),ID,S1.c_str(),undoS1.c_str());
	//printRing(this);	
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

typedef vector<vector<int> > MATRIXi;

// 2阶全矩阵环M2(r)
struct M2r:public IRing
{
public:
	// 静态函数
   static int getidx(vector<MATRIXi> &Arr2,MATRIXi &Arr1);
   static bool IsEqual(const MATRIXi &t,const MATRIXi &m);
   static vector<MATRIXi> FR(IRing* r,vector<MATRIXi>& gen); 
   static MATRIXi add(IRing* r,const MATRIXi &t,const MATRIXi &m);  
   static MATRIXi mul(IRing* r,const MATRIXi &t,const MATRIXi &m);
   static string MStr(const MATRIXi &t);   
public:
	// 实现抽象基类的方法
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// 构造函数
	M2r();// 默认构造函数
	M2r(IRing* r);
	M2r(IRing* r,vector<MATRIXi>& gen);	
	// 析构函数
	~M2r();
	// 成员函数	
    // 有限环序列A、B、C、D、E、F、G、H、I、J、K(拉丁字母符号以4阶环的GAP编号为准)<->C、B、A、J、?、?、Y、P、X、D、K(对应的某些文献的拉丁字母符号)
    void initD(int n);// 256阶全矩阵环M2(Z/4Z)的4阶子环R4_4所在的有限环序列,J=<a,b;pa=pb=0,a^2=b^2=0>=C_p×C_p(0)	
	void initE(int n);// R4_5所在的有限环序列,
	void initF(int n);// R4_6所在的有限环序列,		
    void initG(int n);// 16阶全矩阵环M2(Z/2Z)的4阶子环R4_7所在的有限环序列,Y	
    void initH(int n);// 16阶全矩阵环M2(Z/2Z)的4阶子环R4_8所在的有限环序列,P	
    void initI(int n);// 16阶全矩阵环M2(Z/2Z)的4阶子环R4_9所在的有限环序列,X	
    void initJ(int n);// 16阶全矩阵环M2(Z/2Z)的4阶子环R4_10所在的有限环序列,D=<a,b;pa=pb=0,a^2=a,b^2=b,ab=ba=0>=Z_p+Z_p	
    void initK(int n);// 16阶全矩阵环M2(Z/2Z)的4阶子环R4_11所在的有限环序列,K=GF(p^2)=p^2阶有限域
	void initR8(int ID=0);
	bool initR16(int ID=0);		
	// 成员变量
	vector<MATRIXi> m_Set;
	IRing* m_r;
	int m_flag;// M2r对象析构时是否释放m_r指向的内存
};

M2r::M2r(){
	m_r=NULL;
	m_flag=0;	
}

M2r::~M2r(){
#if 0
	int ID=IdRing(this);
	printf("R%d_%d: %s\n",size(),ID,__FUNCTION__);	
#endif	
	if(m_flag==1 && m_r!=NULL){
		delete m_r;
		m_r=NULL;
	}	
}

// A:=[ [ ZmodnZObj(2,m), ZmodnZObj( 0, m) ], [ ZmodnZObj( 0, m), ZmodnZObj( 0, m) ] ];;B:=[ [ ZmodnZObj(0,m), ZmodnZObj( 0, m) ], [ ZmodnZObj( 0, m), ZmodnZObj( 2, m) ] ];;R:=RingByGenerators([A,B]);;
void M2r::initD(int n){
   MATRIXi A(2,vector<int>(2,0));
   MATRIXi B(2,vector<int>(2,0));
   A[0][0]=n;
   A[0][1]=0;
   A[1][0]=0;
   A[1][1]=0;  
   B[0][0]=0;
   B[0][1]=0;
   B[1][0]=0;
   B[1][1]=n;
   vector<MATRIXi> gen;
   gen.push_back(A);
   gen.push_back(B);
   m_r=new ZmodnZ(1,n*n);
   m_flag=1;
   m_Set=FR(m_r,gen);   
}

void M2r::initE(int n){
}

void M2r::initF(int n){
}

// A:=[ [ ZmodnZObj( 1, m), ZmodnZObj( 0, m) ], [ ZmodnZObj( 0, m), ZmodnZObj( 0, m) ] ];;B:=[ [ ZmodnZObj( 1, m), ZmodnZObj( 1, m) ], [ ZmodnZObj( 0, m), ZmodnZObj( 0, m) ] ];;YRing:=RingByGenerators([A,B]);;
void M2r::initG(int n){
   MATRIXi A(2,vector<int>(2,0));
   MATRIXi B(2,vector<int>(2,0));
   A[0][0]=1;
   A[0][1]=0;
   A[1][0]=0;
   A[1][1]=0;  
   B[0][0]=1;
   B[0][1]=1;
   B[1][0]=0;
   B[1][1]=0;  
   vector<MATRIXi> gen;
   gen.push_back(A);
   gen.push_back(B);
   m_r=new ZmodnZ(1,n);
   m_flag=1;
   m_Set=FR(m_r,gen);   
}

// A:=[ [ ZmodnZObj( 1, m), ZmodnZObj( 0, m) ], [ ZmodnZObj( 0, m), ZmodnZObj( 0, m) ] ];;B:=[ [ ZmodnZObj( 1, m), ZmodnZObj( 0, m) ], [ ZmodnZObj( 1, m), ZmodnZObj( 0, m) ] ];;R:=RingByGenerators([A,B]);;
void M2r::initH(int n){
   MATRIXi A(2,vector<int>(2,0));
   MATRIXi B(2,vector<int>(2,0));
   A[0][0]=1;
   A[0][1]=0;
   A[1][0]=0;
   A[1][1]=0;  
   B[0][0]=1;
   B[0][1]=0;
   B[1][0]=1;
   B[1][1]=0;  
   vector<MATRIXi> gen;
   gen.push_back(A);
   gen.push_back(B);
   m_r=new ZmodnZ(1,n);
   m_flag=1;
   m_Set=FR(m_r,gen);   
}

// A:=[ [ ZmodnZObj( 1, m), ZmodnZObj( 0, m) ], [ ZmodnZObj( 0, m), ZmodnZObj( 1, m) ] ];;B:=[ [ ZmodnZObj( 1, m), ZmodnZObj( 1, m) ], [ ZmodnZObj( 0, m), ZmodnZObj( 1, m) ] ];;R:=RingByGenerators([A,B]);;
void M2r::initI(int n){
   MATRIXi A(2,vector<int>(2,0));
   MATRIXi B(2,vector<int>(2,0));
   A[0][0]=1;
   A[0][1]=0;
   A[1][0]=0;
   A[1][1]=1;  
   B[0][0]=1;
   B[0][1]=1;
   B[1][0]=0;
   B[1][1]=1;  
   vector<MATRIXi> gen;
   gen.push_back(A);
   gen.push_back(B);
   m_r=new ZmodnZ(1,n);
   m_flag=1;
   m_Set=FR(m_r,gen);   
}

// A:=[ [ ZmodnZObj( 1, 2 ), ZmodnZObj( 0, 2 ) ], [ ZmodnZObj( 0, 2 ), ZmodnZObj( 1, 2 ) ] ];;B:=[ [ ZmodnZObj( 1, 2 ), ZmodnZObj( 0, 2 ) ], [ ZmodnZObj( 0, 2 ), ZmodnZObj( 0, 2 ) ] ];;R:=RingByGenerators([A,B]);;
void M2r::initJ(int n){
   MATRIXi A(2,vector<int>(2,0));
   MATRIXi B(2,vector<int>(2,0));
   A[0][0]=1;
   A[0][1]=0;
   A[1][0]=0;
   A[1][1]=1;  
   B[0][0]=1;
   B[0][1]=0;
   B[1][0]=0;
   B[1][1]=0; 
   vector<MATRIXi> gen;
   gen.push_back(A);
   gen.push_back(B);
   m_r=new ZmodnZ(1,n);
   m_flag=1;
   m_Set=FR(m_r,gen);   
}

void M2r::initK(int n){
   MATRIXi A(2,vector<int>(2,0));
   A[0][0]=1;
   A[0][1]=1;
   A[1][0]=1;
   A[1][1]=0;   
   vector<MATRIXi> gen;
   gen.push_back(A);
   m_r=new ZmodnZ(1,n);
   m_flag=1;
   m_Set=FR(m_r,gen);   
}

bool M2r::initR16(int ID){
   vector<MATRIXi> gen;	
   MATRIXi A(2,vector<int>(2,0));
   MATRIXi B(2,vector<int>(2,0));
   if(ID==115){  
		m_r=new ZmodnZ(1,4);
		A[0][0]=0;
		A[0][1]=1;
		A[1][0]=1;
		A[1][1]=0; 
		gen.push_back(A);
   }else if(ID==2){
		m_r=new ZmodnZ(1,16);		
		A[0][0]=2;
		A[0][1]=1;
		A[1][0]=0;
		A[1][1]=8;
		gen.push_back(A);		
   }else if(ID==110){		
		initI(4);
        return true;		
   }else if(ID==102){		
		initH(4);
        return true;	
   }else if(ID==106){		
		initK(4);
        return true; 		
   }else if(ID==107){		
		initD(4);
        return true;			
   }else if(ID==8){
		m_r=new ZmodnZ(1,16);		
		A[0][0]=2;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=6;
		gen.push_back(A);
   }else if(ID==9){
		m_r=new ZmodnZ(2,32);		
		A[0][0]=2;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=6;
		B[0][0]=8;
		B[0][1]=8;
		B[1][0]=8;
		B[1][1]=8;
		gen.push_back(A);
		gen.push_back(B);		
   }else if(ID==10){
		m_r=new ZmodnZ(1,16);		
		A[0][0]=2;
		A[0][1]=4;
		A[1][0]=0;
		A[1][1]=8;
		B[0][0]=8;
		B[0][1]=8;
		B[1][0]=8;
		B[1][1]=8;
		gen.push_back(A);
		gen.push_back(B);		
   }else if(ID==11){
        //R16_11
		m_r=new ZmodnZ(1,8);		
		A[0][0]=4;
		A[0][1]=0;
		A[1][0]=3;
		A[1][1]=1;
		gen.push_back(A);		
   }else if(ID==17){
		m_r=new ZmodnZ(1,8);		
		A[0][0]=6;
		A[0][1]=0;
		A[1][0]=3;
		A[1][1]=2;
		gen.push_back(A);		
   }else if(ID==18){		
		//R16_18
		m_r=new ZmodnZ(1,8);
		A[0][0]=4;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=0;
		B[0][0]=0;
		B[0][1]=3;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);  
   }else if(ID==19){
        //R16_19
		m_r=new ZmodnZ(1,8);	
		A[0][0]=2;
		A[0][1]=3;
		A[1][0]=0;
		A[1][1]=0;
		B[0][0]=0;
		B[0][1]=6;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);	
   }else if(ID==20){
        //R16_20
		m_r=new ZmodnZ(1,8);		
		A[0][0]=1;
		A[0][1]=4;
		A[1][0]=0;
		A[1][1]=0;
		B[0][0]=3;
		B[0][1]=0;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);	
   }else if(ID==21){
        //R16_21
		m_r=new ZmodnZ(1,8);		
		A[0][0]=4;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=0;
		B[0][0]=0;
		B[0][1]=0;
		B[1][0]=3;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);
   }else if(ID==22){
		m_r=new ZmodnZ(1,16);		
		A[0][0]=12;
		A[0][1]=4;
		A[1][0]=6;
		A[1][1]=8;
		gen.push_back(A);		
   }else if(ID==23){		
		m_r=new ZmodnZ(1,8);		
		A[0][0]=0;
		A[0][1]=2;
		A[1][0]=0;
		A[1][1]=1;  
		B[0][0]=0;
		B[0][1]=2;
		B[1][0]=0;
		B[1][1]=3;
		gen.push_back(A);
		gen.push_back(B);	
   }else if(ID==101){//initG(4)
		m_r=new ZmodnZ(1,4);		
		A[0][0]=1;
		A[0][1]=2;
		A[1][0]=0;
		A[1][1]=0;
		B[0][0]=3;
		B[0][1]=3;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);
   }else if(ID==104){//initJ(4)
		m_r=new ZmodnZ(1,4);		
		A[0][0]=1;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=0;
		B[0][0]=0;
		B[0][1]=0;
		B[1][0]=0;
		B[1][1]=1;
		gen.push_back(A);
		gen.push_back(B);	
   }else if(ID==105){
		m_r=new ZmodnZ(1,4);		
		A[0][0]=1;
		A[0][1]=0;
		A[1][0]=2;
		A[1][1]=1;
		B[0][0]=0;
		B[0][1]=2;
		B[1][0]=1;
		B[1][1]=2;
		gen.push_back(A);
		gen.push_back(B);
   }else if(ID==108){
		m_r=new ZmodnZ(1,8);		
		A[0][0]=2;
		A[0][1]=2;
		A[1][0]=2;
		A[1][1]=2;
		B[0][0]=4;
		B[0][1]=0;
		B[1][0]=6;
		B[1][1]=2;
		gen.push_back(A);
		gen.push_back(B);
   }else if(ID==118){
		m_r=new ZmodnZ(2,8);		
		A[0][0]=0;
		A[0][1]=1;
		A[1][0]=1;
		A[1][1]=0;
		B[0][0]=1;
		B[0][1]=1;
		B[1][0]=1;
		B[1][1]=1;
		gen.push_back(A);
		gen.push_back(B);			
   }else if(ID==200){
		m_r=new ZmodnZ(1,8);		
		A[0][0]=4;
		A[0][1]=4;
		A[1][0]=4;
		A[1][1]=4;
		B[0][0]=4;
		B[0][1]=2;
		B[1][0]=6;
		B[1][1]=2;
		gen.push_back(A);
		gen.push_back(B);	
   }else if(ID==201){
		m_r=new ZmodnZ(1,4);
		A[0][0]=0;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=1;
		B[0][0]=0;
		B[0][1]=2;
		B[1][0]=2;
		B[1][1]=0;	
		gen.push_back(A);
		gen.push_back(B);	
	}else if(ID==202){
		m_r=new ZmodnZ(1,4);
		A[0][0]=0;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=2;
		B[0][0]=0;
		B[0][1]=1;
		B[1][0]=2;
		B[1][1]=0;	
		gen.push_back(A);
		gen.push_back(B);	
	}else if(ID==203){
		m_r=new ZmodnZ(1,4);
		A[0][0]=0;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=1;
		B[0][0]=2;
		B[0][1]=0;
		B[1][0]=2;
		B[1][1]=0;	
		gen.push_back(A);
		gen.push_back(B);
	}else if(ID==204){
		m_r=new ZmodnZ(1,4);
		A[0][0]=0;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=2;
		B[0][0]=0;
		B[0][1]=1;
		B[1][0]=2;
		B[1][1]=1;	
		gen.push_back(A);
		gen.push_back(B);	
	}else if(ID==205){
		m_r=new ZmodnZ(1,4);
		A[0][0]=0;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=2;
		B[0][0]=1;
		B[0][1]=0;
		B[1][0]=2;
		B[1][1]=1;	
		gen.push_back(A);
		gen.push_back(B);
   }else if(ID==212){
		m_r=new ZmodnZ(1,8);		
		A[0][0]=4;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=4;
		B[0][0]=4;
		B[0][1]=0;
		B[1][0]=2;
		B[1][1]=2;
		gen.push_back(A);
		gen.push_back(B);
		MATRIXi C(2,vector<int>(2,4));
		gen.push_back(C);		
   }else if(ID==300){
		m_r=new ZmodnZ(1,2);		
		A[0][0]=1;
		A[0][1]=1;
		A[1][0]=1;
		A[1][1]=0;
		B[0][0]=0;
		B[0][1]=1;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);		
   }else if(ID==302){
		M2r *r=new M2r();
        r->initJ(2);
		m_r=r;
		A[0][0]=0;
		A[0][1]=1;
		A[1][0]=1;
		A[1][1]=2;	
		gen.push_back(A);		
   }else if(ID==364){	
	   M2r *K4=new M2r();
	   K4->initK(2);
	   m_r=K4;
	   A[0][0]=1;
	   A[0][1]=0;
	   A[1][0]=0;
	   A[1][1]=2;  
	   B[0][0]=3;
	   B[0][1]=2;
	   B[1][0]=0;
	   B[1][1]=3;    
	   gen.push_back(A);
	   gen.push_back(B);		
   }else if(ID==365){
	   M2r *K4=new M2r();
	   K4->initJ(2);
	   m_r=K4;	
		A[0][0]=1;
		A[0][1]=1;
		A[1][0]=0;
		A[1][1]=2;  
		B[0][0]=3;
		B[0][1]=3;
		B[1][0]=3;
		B[1][1]=1; 
	   gen.push_back(A);
	   gen.push_back(B);		
   }else if(ID==366){
		M2r *I4=new M2r();
		I4->initI(2);
		m_r=I4;	
		A[0][0]=2;
		A[0][1]=2;
		A[1][0]=0;
		A[1][1]=2;  
		B[0][0]=0;
		B[0][1]=2;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);		
   }else if(ID==378){
	   // HK(4)=R16_378
	   M2r *K4=new M2r();
	   K4->initK(2);
	   m_r=K4;	
	   A[0][0]=1;
	   A[0][1]=0;
	   A[1][0]=0;
	   A[1][1]=0;  
	   B[0][0]=1;
	   B[0][1]=0;
	   B[1][0]=1;
	   B[1][1]=0;  
	   gen.push_back(A);
	   gen.push_back(B);
   }else if(ID==381){
	   // GK(4)=R16_381	
	   M2r *K4=new M2r();
	   K4->initK(2);
	   m_r=K4;
	   A[0][0]=1;
	   A[0][1]=0;
	   A[1][0]=0;
	   A[1][1]=0;  
	   B[0][0]=1;
	   B[0][1]=1;
	   B[1][0]=0;
	   B[1][1]=0;  
	   gen.push_back(A);
	   gen.push_back(B);	   
   }else if(ID==383){
	   // R16_383	
	   M2r *K4=new M2r();
	   K4->initK(2);
	   m_r=K4;
#if 0	   
	   A[0][0]=0;
	   A[0][1]=1;
	   A[1][0]=1;
	   A[1][1]=0;   
	   gen.push_back(A);	  
	   // IK(4)=R16_383
#else
	   A[0][0]=1;
	   A[0][1]=0;
	   A[1][0]=0;
	   A[1][1]=1;  
	   B[0][0]=1;
	   B[0][1]=1;
	   B[1][0]=0;
	   B[1][1]=1;    
	   gen.push_back(A);
	   gen.push_back(B);
#endif 	  
   }else if(ID==384){
		// DK(4)=R16_389
		// DI(4)=R16_384
		M2r *I4=new M2r();
		I4->initI(2);
		m_r=I4;	
		A[0][0]=2;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=0;  
		B[0][0]=0;
		B[0][1]=0;
		B[1][0]=0;
		B[1][1]=2;
		gen.push_back(A);
		gen.push_back(B);			 
   }else if(ID==389){
	   // JK(4)=R16_389
	   M2r *K4=new M2r();
	   K4->initK(2);
	   m_r=K4;	
	   A[0][0]=1;
	   A[0][1]=0;
	   A[1][0]=0;
	   A[1][1]=1;  
	   B[0][0]=1;
	   B[0][1]=0;
	   B[1][0]=0;
	   B[1][1]=0;   
	   gen.push_back(A);
	   gen.push_back(B); 
   }else if(ID==0){	   
 	    initR16(389);
		return true;  
   }else{
		return false;
   }
   m_flag=1;
   m_Set=FR(m_r,gen); 
   return true; 
}

void M2r::initR8(int ID){
	vector<MATRIXi> gen;	
	MATRIXi A(2,vector<int>(2,0));
	MATRIXi B(2,vector<int>(2,0));	
	if(ID==8){  		
		//R8_8:N0n0bAbOn1n2n4n5n6n7n8S1N2=[1,3,4,0],4,0,0,8,1,5,7,48,7,2,[1,3,4,0],[[2,4,8],[4,4,8]]	
		m_r=new ZmodnZ(1,4);
		A[0][0]=0;
		A[0][1]=1;
		A[1][0]=0;
		A[1][1]=0;
		B[0][0]=2;
		B[0][1]=0;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);
   }else if(ID==13){		
        //R8_13:N0n0bAbOn1n2n4n5n6n7n8S1N2=[1,3,4,0],4,1,0,8,2,3,3,32,7,8,[1,3,2,2],[[2,4,8],[4,2,8],[4,4,16]]
		m_r=new ZmodnZ(1,4);		
		A[0][0]=2;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=1;  
		B[0][0]=0;
		B[0][1]=0;
		B[1][0]=0;
		B[1][1]=3;
		gen.push_back(A);
		gen.push_back(B);
   }else if(ID==16){
		m_r=new ZmodnZ(2,8);		
		A[0][0]=0;
		A[0][1]=2;
		A[1][0]=0;
		A[1][1]=0;
		B[0][0]=3;
		B[0][1]=0;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);		
   }else if(ID==20){
        //R8_20:N0n0bAbOn1n2n4n5n6n7n8S1N2=[1,3,4,0],4,0,0,8,3,3,3,28,3,2,[1,3,4,0],[[2,4,8],[4,2,12],[4,4,16]]
		m_r=new ZmodnZ(1,4);		
		A[0][0]=0;
		A[0][1]=2;
		A[1][0]=0;
		A[1][1]=0;
		B[0][0]=3;
		B[0][1]=0;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);
   }else if(ID==25){
		M2r *K4=new M2r();
		K4->initD(2);
		m_r=K4;		
		A[0][0]=3;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=1;
		B[0][0]=2;
		B[0][1]=0;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);
		MATRIXi C(2,vector<int>(2,2));
		gen.push_back(C);		
   }else if(ID==31){
		M2r *I4=new M2r();
		I4->initI(2);
		m_r=I4;	
		A[0][0]=0;
		A[0][1]=2;
		A[1][0]=0;
		A[1][1]=0;  
		B[0][0]=3;
		B[0][1]=2;
		B[1][0]=0;
		B[1][1]=3;
		gen.push_back(A);
		gen.push_back(B);
   }else if(ID==41){
		M2r *r4=new M2r();
		r4->initG(2);
		m_r=r4;		
		A[0][0]=3;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=1;
		B[0][0]=2;
		B[0][1]=0;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);		
   }else if(ID==47){
		M2r *r4=new M2r();
		r4->initH(2);
		m_r=r4;		
		A[0][0]=3;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=1;
		B[0][0]=2;
		B[0][1]=0;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);		
   }else if(ID==48){
		M2r *I4=new M2r();
		I4->initI(2);
		m_r=I4;			
		A[0][0]=3;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=1;
		B[0][0]=2;
		B[0][1]=0;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);		
   }else if(ID==51){
	   // KK(4)=R8_51
	   M2r *K4=new M2r();
	   K4->initK(2);
	   m_r=K4;
	   A[0][0]=1;
	   A[0][1]=1;
	   A[1][0]=1;
	   A[1][1]=0;   
	   gen.push_back(A); 	
	}else{
	    initR8(51);
		return;
	}	 
	m_flag=1;
	m_Set=FR(m_r,gen);   
}

M2r::M2r(IRing* r,vector<MATRIXi>& gen){
	m_r=r;
	m_Set=FR(r,gen);
}

vector<MATRIXi> M2r::FR(IRing* r,vector<MATRIXi>& gen){
	vector<MATRIXi> Set;
	int n=gen[0].size();
	MATRIXi E(n,vector<int>(n,0));
	Set.push_back(E);
	for(int i=0;i<gen.size();i++)
	{
		if(!IsEqual(gen[i],E))
			Set.push_back(gen[i]);
	}
	int R=Set.size();
	int cnt=R;
	int cnt1=R;
	do{
		cnt=Set.size();
		for(int i=0;i<cnt;i++)
		{
			for(int j=0;j<cnt;j++)
			{
				MATRIXi IJ=mul(r,Set[i],Set[j]);
				vector<MATRIXi>::iterator p=std::find(Set.begin(),Set.end(),IJ);
				if(p==Set.end()){
					Set.push_back(IJ);
				}
				MATRIXi IJ1=add(r,Set[i],Set[j]);
				p=std::find(Set.begin(),Set.end(),IJ1);
				if(p==Set.end()){
					Set.push_back(IJ1);
				}
			}
		}
		cnt1=Set.size();
	}while(cnt1>cnt);
    return Set;	
}

string M2r::MStr(const MATRIXi &t){
	string str="[";
	int n=t.size();
	for(int i=0;i<n;i++)
	{
        str+="[";
		for(int j=0;j<n;j++)
		{
			char sz[20]={0};
			sprintf(sz,"%d",t[i][j]);
			str+=sz;
			if(j<n-1)
				str+=",";
		}
		str+="]";
		if(i<n-1)
			str+=",";
	}
	str+="]";
	return str;
}

MATRIXi M2r::add(IRing* r,const MATRIXi &t,const MATRIXi &m)
{
    int n=t.size();
	MATRIXi C(n,vector<int>(n,0));
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			C[i][j]=r->add(t[i][j],m[i][j]);
		}
	}
	return C;
}

MATRIXi M2r::mul(IRing* r,const MATRIXi &t,const MATRIXi &m)
{
    int n=t.size();
	MATRIXi C(n,vector<int>(n,0));
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			int sum=0;
			for(int k=0;k<n;k++){
				sum=r->add(sum,r->mul(t[i][k],m[k][j]));
			}
			C[i][j]=sum;
		}
	}
	return C;
}

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
	string str=calcRingInvariant(this);
	printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",size(),ID,str.c_str());	
	string I1=calcI1(this);
	string I2=calcI2(this);   
	printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());	
	//string S1=calS1(this,true);
	//string undoS1=calS1(this,false);	
	//printf("R%d_%d:S1=%s,S1(未经处理)=%s\n",size(),ID,S1.c_str(),undoS1.c_str());
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

typedef unsigned int TElem; // unsigned char
typedef vector<vector<TElem> > MATRIXi8;

// n阶全矩阵环Mn(r)
struct Mnr:public IRing
{
public:
	// 静态函数
   static int getidx(vector<MATRIXi8> &Arr2,MATRIXi8 &Arr1);
   static bool IsEqual(const MATRIXi8 &t,const MATRIXi8 &m);	
   static bool nextV1(int m,vector<TElem>& v);
   static vector<MATRIXi8> FR(IRing* r,vector<MATRIXi8>& gen); 
   static MATRIXi8 add(IRing* r,const MATRIXi8 &t,const MATRIXi8 &m);  
   static MATRIXi8 mul(IRing* r,const MATRIXi8 &t,const MATRIXi8 &m); 
   static string MStr(const MATRIXi8 &t);   
public:
	// 实现抽象基类的方法
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// 构造函数
	Mnr();// 默认构造函数
	Mnr(IRing* r,int n);
	Mnr(IRing* r,int n,vector<MATRIXi8>& gen);	
	// 析构函数
	~Mnr();	
	// 成员函数	
    int visitMnRm(int n,int m);	
	// 有限环序列
	void initE(int n);// R4_5、R9_5所在的n^2阶有限环序列,
	void initF(int n);// R4_6、R9_6所在的n^2阶有限环序列,		
	void initL(int n);// R8_38所在的n^3阶有限环序列,
	void initR8(int ID=0);
	bool initR16(int ID=0);	
	// 成员变量
	vector<MATRIXi8> m_Set;
	IRing* m_r;
	int m_n;
	int m_flag;// Mnr对象析构时是否释放m_r指向的内存
};

Mnr::Mnr(){
	m_r=NULL;
	m_flag=0;	
}

Mnr::~Mnr(){
	if(m_flag==1 && m_r!=NULL){
		delete m_r;
		m_r=NULL;
	}	
}

// A:=[[Z(m),Z(m),0*Z(m),0*Z(m)],[0*Z(m),Z(m),0*Z(m),Z(m)],[0*Z(m),Z(m),0*Z(m),Z(m)],[Z(m),Z(m),0*Z(m),0*Z(m)]];;R4_5:=RingByGenerators([A]);;
void Mnr::initE(int n){
   MATRIXi8 A(4,vector<TElem>(4,0));
   //MATRIXi8 B(4,vector<unsigned char>(4,0));
   A[0][0]=0;
   A[0][1]=1;
   A[0][2]=0;
   A[0][3]=1; 
   A[1][0]=n-1;
   A[1][1]=n-1;
   A[1][2]=0;
   A[1][3]=0;
   A[2][0]=n-1;
   A[2][1]=n-1;
   A[2][2]=0;
   A[2][3]=0;
   A[3][0]=0;
   A[3][1]=1;
   A[3][2]=0;
   A[3][3]=1;
   vector<MATRIXi8> gen;
   gen.push_back(A);
   m_r=new ZmodnZ(1,n);
   m_flag=1;
   m_n=4;
   m_Set=FR(m_r,gen);   
}

// A:=[[Z(m),0*Z(m),0*Z(m),Z(m)],[Z(m),0*Z(m),0*Z(m),Z(m)],[0*Z(m),Z(m),Z(m),0*Z(m)],[Z(m),0*Z(m),0*Z(m),Z(m)]];;R4_6:=RingByGenerators([A]);;
void Mnr::initF(int n){
   MATRIXi8 A(4,vector<TElem>(4,0));
   A[0][0]=1;
   A[0][1]=0;
   A[0][2]=0;
   A[0][3]=1;
   A[1][0]=n-1;
   A[1][1]=0;
   A[1][2]=0;
   A[1][3]=n-1; 
   A[2][0]=0;
   A[2][1]=n-1;
   A[2][2]=n-1;
   A[2][3]=0;
   A[3][0]=n-1;
   A[3][1]=0;
   A[3][2]=0;
   A[3][3]=n-1;
   vector<MATRIXi8> gen;
   gen.push_back(A);
   m_r=new ZmodnZ(1,n);
   m_flag=1;
   m_n=4;
   m_Set=FR(m_r,gen);   
}

// A:=[[0*Z(m),Z(m),0*Z(m),Z(m)],[Z(m),Z(m),0*Z(m),0*Z(m)],[0*Z(m),0*Z(m),Z(m),Z(m)],[0*Z(m),Z(m),0*Z(m),Z(m)]];;R8_38:=RingByGenerators([A]);;
void Mnr::initL(int n){
   MATRIXi8 A(4,vector<TElem>(4,0));
   //MATRIXi8 B(4,vector<unsigned char>(4,0));
   A[0][0]=0;
   A[0][1]=1;
   A[0][2]=0;
   A[0][3]=1;
   A[1][0]=1;
   A[1][1]=1;
   A[1][2]=0;
   A[1][3]=0; 
   A[2][0]=0;
   A[2][1]=0;
   A[2][2]=1;
   A[2][3]=1;
   A[3][0]=0;
   A[3][1]=1;
   A[3][2]=0;
   A[3][3]=1;
   vector<MATRIXi8> gen;
   gen.push_back(A);
   m_r=new ZmodnZ(1,n);
   m_flag=1;
   m_n=4;
   m_Set=FR(m_r,gen);   
}

void Mnr::initR8(int ID){
   vector<MATRIXi8> gen;		
   if(ID==13){
	   MATRIXi8 A(4,vector<TElem>(4,0));
	   A[0][0]=0;
	   A[0][1]=1;
	   A[0][2]=0;
	   A[0][3]=1;
	   A[1][0]=2;
	   A[1][1]=2;
	   A[1][2]=0;
	   A[1][3]=0; 
	   A[2][0]=2;
	   A[2][1]=2;
	   A[2][2]=0;
	   A[2][3]=0;
	   A[3][0]=0;
	   A[3][1]=1;
	   A[3][2]=0;
	   A[3][3]=1;
	   gen.push_back(A);
	   m_r=new ZmodnZ(1,4);	  
	   m_n=4;
   }else if(ID==5){	   
		m_r=new ZmodnZ(2,8);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));	
	    MATRIXi8 B(3,vector<TElem>(3,2));		
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=1;
		A[1][1]=0;
		A[1][2]=1;
		A[2][0]=2;
		A[2][1]=0;
		A[2][2]=2;		
		gen.push_back(A);  
		gen.push_back(B); 	   
   }else if(ID==23){	   
		m_r=new ZmodnZ(1,8);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));		
		A[0][0]=2;
		A[0][1]=0;
		A[0][2]=2;
		A[1][0]=4;
		A[1][1]=0;
		A[1][2]=2;
		A[2][0]=2;
		A[2][1]=0;
		A[2][2]=2;		
		gen.push_back(A); 	   
   }else if(ID==24){
	   MATRIXi8 A(4,vector<TElem>(4,0));
	   A[0][0]=2;
	   A[0][1]=1;
	   A[0][2]=0;
	   A[0][3]=2;
	   A[1][0]=0;
	   A[1][1]=2;
	   A[1][2]=0;
	   A[1][3]=2; 
	   A[2][0]=0;
	   A[2][1]=2;
	   A[2][2]=0;
	   A[2][3]=2;
	   A[3][0]=2;
	   A[3][1]=1;
	   A[3][2]=0;
	   A[3][3]=2; 
	   gen.push_back(A);
	   m_r=new ZmodnZ(1,4);	
       m_n=4;	   
   }else if(ID==41){	   
		m_r=new ZmodnZ(1,2);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));	
	    MATRIXi8 B(3,vector<TElem>(3,1));	
		A[0][0]=0;
		A[0][1]=1;
		A[0][2]=1;
		A[1][0]=1;
		A[1][1]=0;
		A[1][2]=1;
		A[2][0]=1;
		A[2][1]=0;
		A[2][2]=1;		
		gen.push_back(A);  
		gen.push_back(B); 			
   }else{
	   initR8(41);
       return;	   
   }
   m_flag=1;
   m_Set=FR(m_r,gen);      
}

bool Mnr::initR16(int ID){
   vector<MATRIXi8> gen;     
   if(ID==109){
	   m_r=new ZmodnZ(1,4);
	   m_n=4; 	   
	   MATRIXi8 A(4,vector<TElem>(4,0)); 
	   A[0][0]=2;
	   A[0][1]=0;
	   A[0][2]=0;
	   A[0][3]=3;
	   A[1][0]=0;
	   A[1][1]=1;
	   A[1][2]=0;
	   A[1][3]=3; 
	   A[2][0]=0;
	   A[2][1]=1;
	   A[2][2]=2;
	   A[2][3]=0;
	   A[3][0]=0;
	   A[3][1]=1;
	   A[3][2]=0;
	   A[3][3]=1;	
	   gen.push_back(A); 		
   }else if(ID==8){	   
		m_r=new ZmodnZ(1,8);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));		
		A[0][0]=4;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=1;
		A[1][1]=0;
		A[1][2]=1;
		A[2][0]=2;
		A[2][1]=0;
		A[2][2]=2;		
		gen.push_back(A);
   }else if(ID==9){   
		m_r=new ZmodnZ(2,16);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));	
	    MATRIXi8 B(3,vector<TElem>(3,0));		
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=1;
		A[1][1]=0;
		A[1][2]=1;
		A[2][0]=2;
		A[2][1]=0;
		A[2][2]=2;	
		B[0][1]=4;
		B[1][2]=4;
		B[2][0]=4;		
		gen.push_back(A);  
		gen.push_back(B); 			
   }else if(ID==12){
		m_r=new ZmodnZ(2,16);
		m_n=4; 
		MATRIXi8 A(4,vector<TElem>(4,1));		
		MATRIXi8 B(4,vector<TElem>(4,4));		
		A[0][0]=4;
		A[0][1]=0;
		A[0][2]=0;
		A[0][3]=0;
		A[1][0]=0;
		A[1][1]=0;
		A[1][2]=0;
		A[1][3]=0;
		A[2][0]=3;
		A[2][1]=0;
		A[2][2]=0;
		A[2][3]=1;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=0;
		gen.push_back(A);		
		gen.push_back(B);			
   }else if(ID==22){
		m_r=new ZmodnZ(1,8);
		m_n=4; 	   
		MATRIXi8 A(4,vector<TElem>(4,0));
		A[0][0]=4;
		A[0][1]=0;
		A[0][2]=0;
		A[0][3]=0;
		A[1][0]=0;
		A[1][1]=0;
		A[1][2]=0;
		A[1][3]=0;
		A[2][0]=3;
		A[2][1]=0;
		A[2][2]=0;
		A[2][3]=1;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=0;
		gen.push_back(A); 	
   }else if(ID==106){
		m_r=new ZmodnZ(1,4);
		m_n=4; 	   
		MATRIXi8 A(4,vector<TElem>(4,0));
		A[0][0]=0;
		A[0][1]=1;
		A[0][2]=0;
		A[0][3]=1;
		A[1][0]=3;
		A[1][1]=2;
		A[1][2]=0;
		A[1][3]=0; 
		A[2][0]=3;
		A[2][1]=2;
		A[2][2]=0;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=1;
		A[3][2]=0;
		A[3][3]=1;
		gen.push_back(A);
   }else if(ID==107){
		m_r=new ZmodnZ(2,8);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=0;
		A[1][1]=0;
		A[1][2]=3;
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=0;		
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=1;
		B[1][1]=0;
		B[1][2]=3;
		B[2][0]=2;
		B[2][1]=0;
		B[2][2]=0;
		gen.push_back(A); 
		gen.push_back(B);		
   }else if(ID==110){
		m_r=new ZmodnZ(1,4);
		m_n=3; 	   
		MATRIXi8 B(3,vector<TElem>(3,0));		
		B[0][0]=1;
		B[0][1]=1;
		B[0][2]=0;
		B[1][0]=0;
		B[1][1]=1;
		B[1][2]=0;
		B[2][0]=0;
		B[2][1]=1;
		B[2][2]=0;
		gen.push_back(B);			
   }else if(ID==111){
	   initE(4);
	   return true;
   }else if(ID==112){
	   initF(4);
       return true;			
    }else if(ID==113){
	   m_r=new ZmodnZ(1,4);
	   m_n=4; 	   
	   MATRIXi8 A(4,vector<TElem>(4,0));	
	   A[0][0]=1;
	   A[0][1]=0;
	   A[0][2]=0;
	   A[0][3]=1;
	   A[1][0]=1;
	   A[1][1]=0;
	   A[1][2]=0;
	   A[1][3]=1; 
	   A[2][0]=0;
	   A[2][1]=1;
	   A[2][2]=1;
	   A[2][3]=0;
	   A[3][0]=1;
	   A[3][1]=0;
	   A[3][2]=0;
	   A[3][3]=1;
	   gen.push_back(A);	   
   }else if(ID==114){
	   m_r=new ZmodnZ(1,4);
	   m_n=4; 	   
	   MATRIXi8 A(4,vector<TElem>(4,0));	
	   A[0][0]=1;
	   A[0][1]=1;
	   A[0][2]=0;
	   A[0][3]=0;
	   A[1][0]=0;
	   A[1][1]=1;
	   A[1][2]=0;
	   A[1][3]=1; 
	   A[2][0]=0;
	   A[2][1]=1;
	   A[2][2]=0;
	   A[2][3]=1;
	   A[3][0]=1;
	   A[3][1]=1;
	   A[3][2]=0;
	   A[3][3]=0; 
	   gen.push_back(A);
   }else if(ID==120){
		m_r=new ZmodnZ(1,4);
		m_n=4; 	   
		MATRIXi8 A(4,vector<TElem>(4,0));
		MATRIXi8 B(4,vector<TElem>(4,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[0][3]=0;
		A[1][0]=2;
		A[1][1]=0;
		A[1][2]=0;
		A[1][3]=0;
		A[2][0]=0;
		A[2][1]=2;
		A[2][2]=0;
		A[2][3]=0;
		A[3][0]=3;
		A[3][1]=2;
		A[3][2]=2;
		A[3][3]=0;
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[0][3]=0;
		B[1][0]=2;
		B[1][1]=0;
		B[1][2]=0;
		B[1][3]=0;
		B[2][0]=3;
		B[2][1]=2;
		B[2][2]=0;
		B[2][3]=0;
		B[3][0]=2;
		B[3][1]=1;
		B[3][2]=2;
		B[3][3]=0;
		gen.push_back(A); 
		gen.push_back(B);		
   }else if(ID==81){
		m_r=new ZmodnZ(1,4);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=2;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=1;
		A[1][1]=0;
		A[1][2]=1;
		A[2][0]=2;
		A[2][1]=0;
		A[2][2]=2;		
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=0;
		B[1][1]=0;
		B[1][2]=3;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=0;
		gen.push_back(A); 
		gen.push_back(B); 	
   }else if(ID==82){
		m_r=new ZmodnZ(1,4);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=0;
		A[1][1]=0;
		A[1][2]=3;
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=0;		
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=1;
		B[1][1]=0;
		B[1][2]=3;
		B[2][0]=2;
		B[2][1]=0;
		B[2][2]=0;
		gen.push_back(A); 
		gen.push_back(B); 	 		
   }else if(ID==83){
		m_r=new ZmodnZ(1,4);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=3;
		A[1][1]=0;
		A[1][2]=2;
		A[2][0]=2;
		A[2][1]=0;
		A[2][2]=2;		
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=3;
		B[1][1]=0;
		B[1][2]=3;
		B[2][0]=2;
		B[2][1]=0;
		B[2][2]=2;
		gen.push_back(A); 
		gen.push_back(B); 
   }else if(ID==200){//R8_25是其子环
		m_r=new ZmodnZ(1,8);
		m_n=4; 	   
		MATRIXi8 A(4,vector<TElem>(4,0));
		MATRIXi8 B(4,vector<TElem>(4,4));		
		A[0][0]=2;
		A[0][1]=0;
		A[0][2]=2;
		A[0][3]=0;
		A[1][0]=2;
		A[1][1]=0;
		A[1][2]=2;
		A[1][3]=2; 
		A[2][0]=0;
		A[2][1]=2;
		A[2][2]=2;
		A[2][3]=0;
		A[3][0]=2;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=2;
		gen.push_back(A); 
		gen.push_back(B);
   }else if(ID==204){//R8_25是其子环
		m_r=new ZmodnZ(1,4);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,2));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=1;
		A[1][1]=1;
		A[1][2]=0;
		A[2][0]=1;
		A[2][1]=0;
		A[2][2]=1;		
		gen.push_back(A);
		gen.push_back(B);	
   }else if(ID==212){//R8_25是其子环
		m_r=new ZmodnZ(2,8);
		m_n=4; 	   
		MATRIXi8 A(4,vector<TElem>(4,0));
		MATRIXi8 B(4,vector<TElem>(4,0));
		MATRIXi8 C(4,vector<TElem>(4,2));		
		A[0][0]=0;
		A[0][1]=2;
		A[0][2]=0;
		A[0][3]=0;
		A[1][0]=2;
		A[1][1]=1;
		A[1][2]=0;
		A[1][3]=0;
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=0;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=1;
		B[0][0]=0;
		B[0][1]=2;
		B[0][2]=0;
		B[0][3]=0;
		B[1][0]=0;
		B[1][1]=2;
		B[1][2]=0;
		B[1][3]=0;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=0;
		B[2][3]=0;
		B[3][0]=0;
		B[3][1]=0;
		B[3][2]=0;
		B[3][3]=0;
		gen.push_back(A); 
		gen.push_back(B);
		gen.push_back(C);			
   }else if(ID==219){
		m_r=new ZmodnZ(1,4);
		m_n=4; 	   
		MATRIXi8 A(4,vector<TElem>(4,0));
		MATRIXi8 B(4,vector<TElem>(4,2));
		A[0][0]=0;
		A[0][1]=1;
		A[0][2]=0;
		A[0][3]=1;
		A[1][0]=2;
		A[1][1]=2;
		A[1][2]=0;
		A[1][3]=0; 
		A[2][0]=2;
		A[2][1]=2;
		A[2][2]=0;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=1;
		A[3][2]=0;
		A[3][3]=1;
		gen.push_back(A); 
		gen.push_back(B);		
   }else if(ID==251){	
	   m_r=new ZmodnZ(1,4);
	   m_n=4; 	   
	   MATRIXi8 A(4,vector<TElem>(4,0)); 
	   A[0][0]=2;
	   A[0][1]=0;
	   A[0][2]=0;
	   A[0][3]=1;
	   A[1][0]=0;
	   A[1][1]=1;
	   A[1][2]=2;
	   A[1][3]=2; 
	   A[2][0]=0;
	   A[2][1]=0;
	   A[2][2]=2;
	   A[2][3]=0;
	   A[3][0]=2;
	   A[3][1]=2;
	   A[3][2]=0;
	   A[3][3]=1;	
	   gen.push_back(A);	   
   }else if(ID==257){
		m_r=new ZmodnZ(1,4);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=2;
		A[0][1]=0;
		A[0][2]=2;
		A[1][0]=1;
		A[1][1]=2;
		A[1][2]=2;
		A[2][0]=2;
		A[2][1]=2;
		A[2][2]=0;		
		B[0][0]=2;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=0;
		B[1][1]=2;
		B[1][2]=2;
		B[2][0]=2;
		B[2][1]=0;
		B[2][2]=2;
		gen.push_back(A); 
		gen.push_back(B); 	   
   }else if(ID==258){
		m_r=new ZmodnZ(1,4);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=0;
		A[0][1]=2;
		A[0][2]=0;
		A[1][0]=2;
		A[1][1]=0;
		A[1][2]=2;
		A[2][0]=1;
		A[2][1]=0;
		A[2][2]=2;		
		B[0][0]=2;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=0;
		B[1][1]=2;
		B[1][2]=0;
		B[2][0]=2;
		B[2][1]=2;
		B[2][2]=0;
		gen.push_back(A); 
		gen.push_back(B); 
   }else if(ID==259){
		m_r=new ZmodnZ(1,4);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=0;
		A[0][1]=2;
		A[0][2]=0;
		A[1][0]=2;
		A[1][1]=0;
		A[1][2]=2;
		A[2][0]=1;
		A[2][1]=0;
		A[2][2]=2;		
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=2;
		B[1][1]=2;
		B[1][2]=0;
		B[2][0]=0;
		B[2][1]=2;
		B[2][2]=2;
		gen.push_back(A); 
		gen.push_back(B);
   }else if(ID==260){
		m_r=new ZmodnZ(1,4);
		m_n=4; 	   
		MATRIXi8 A(4,vector<TElem>(4,0));
		A[0][0]=2;
		A[0][1]=1;
		A[0][2]=0;
		A[0][3]=2;
		A[1][0]=0;
		A[1][1]=0;
		A[1][2]=0;
		A[1][3]=0;
		A[2][0]=0;
		A[2][1]=3;
		A[2][2]=2;
		A[2][3]=3;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=2;
		A[3][3]=0;
		gen.push_back(A); 	
   }else if(ID==261){
		m_r=new ZmodnZ(1,4);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=2;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=0;
		A[1][1]=2;
		A[1][2]=1;
		A[2][0]=2;
		A[2][1]=0;
		A[2][2]=2;		
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=2;
		B[1][1]=2;
		B[1][2]=0;
		B[2][0]=2;
		B[2][1]=0;
		B[2][2]=2;
		gen.push_back(A); 
		gen.push_back(B); 
   }else if(ID==262){
		m_r=new ZmodnZ(1,4);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=2;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=0;
		A[1][1]=2;
		A[1][2]=1;
		A[2][0]=2;
		A[2][1]=0;
		A[2][2]=2;		
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=2;
		B[1][1]=0;
		B[1][2]=1;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=2;
		gen.push_back(A); 
		gen.push_back(B);  		
   }else if(ID==263){
		m_r=new ZmodnZ(1,4);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=2;
		A[1][1]=0;
		A[1][2]=1;
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=2;		
		B[0][0]=2;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=2;
		B[1][1]=0;
		B[1][2]=2;
		B[2][0]=2;
		B[2][1]=0;
		B[2][2]=2;
		gen.push_back(A); 
		gen.push_back(B); 	
   }else if(ID==264){
		m_r=new ZmodnZ(1,4);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=2;
		A[1][1]=0;
		A[1][2]=3;
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=0;		
		B[0][0]=2;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=2;
		B[1][1]=0;
		B[1][2]=2;
		B[2][0]=2;
		B[2][1]=0;
		B[2][2]=2;
		gen.push_back(A); 
		gen.push_back(B);	
   }else if(ID==265){//R8_25是其子环
		m_r=new ZmodnZ(1,4);
		m_n=4; 	   
		MATRIXi8 A(4,vector<TElem>(4,0));
		MATRIXi8 B(4,vector<TElem>(4,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[0][3]=0;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=1;
		A[1][3]=1;
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=1;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=1;
		A[3][3]=0;
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[0][3]=0;
		B[1][0]=2;
		B[1][1]=0;
		B[1][2]=0;
		B[1][3]=0;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=0;
		B[2][3]=0;
		B[3][0]=0;
		B[3][1]=0;
		B[3][2]=0;
		B[3][3]=0;
		gen.push_back(A); 
		gen.push_back(B);	
   }else if(ID==266){
		m_r=new ZmodnZ(1,4);
		m_n=4; 	   
		MATRIXi8 A(4,vector<TElem>(4,0));
		MATRIXi8 B(4,vector<TElem>(4,0));
		A[0][0]=0;
		A[0][1]=2;
		A[0][2]=0;
		A[0][3]=0;
		A[1][0]=2;
		A[1][1]=1;
		A[1][2]=0;
		A[1][3]=0;
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=0;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=1;
		B[0][0]=0;
		B[0][1]=2;
		B[0][2]=0;
		B[0][3]=0;
		B[1][0]=0;
		B[1][1]=2;
		B[1][2]=0;
		B[1][3]=0;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=0;
		B[2][3]=0;
		B[3][0]=0;
		B[3][1]=0;
		B[3][2]=0;
		B[3][3]=0;
		gen.push_back(A); 
		gen.push_back(B);	
   }else if(ID==268){	//R8_25是其子环   
		m_r=new ZmodnZ(1,4);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));	
	    MATRIXi8 B(3,vector<TElem>(3,2));		
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=1;
		A[1][1]=0;
		A[1][2]=1;
		A[2][0]=2;
		A[2][1]=0;
		A[2][2]=2;		
		gen.push_back(A);  
		gen.push_back(B); 		
   }else if(ID==355){
		m_r=new ZmodnZ(1,2);
		m_n=4; 	   
		MATRIXi8 A(4,vector<TElem>(4,0));
		MATRIXi8 B(4,vector<TElem>(4,1));
		A[0][0]=0;
		A[0][1]=1;
		A[0][2]=0;
		A[0][3]=1;
		A[1][0]=1;
		A[1][1]=1;
		A[1][2]=0;
		A[1][3]=0; 
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=1;
		A[2][3]=1;
		A[3][0]=0;
		A[3][1]=1;
		A[3][2]=0;
		A[3][3]=1;
		gen.push_back(A); 
		gen.push_back(B);		
   }else if(ID==357){
		m_r=new ZmodnZ(1,2);
		m_n=4; 	   
		MATRIXi8 A(4,vector<TElem>(4,0));
		MATRIXi8 B(4,vector<TElem>(4,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[0][3]=0;
		A[1][0]=0;
		A[1][1]=0;
		A[1][2]=0;
		A[1][3]=0;
		A[2][0]=1;
		A[2][1]=0;
		A[2][2]=0;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=0;
		B[0][0]=0;
		B[0][1]=1;
		B[0][2]=0;
		B[0][3]=1;
		B[1][0]=1;
		B[1][1]=1;
		B[1][2]=0;
		B[1][3]=1;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=0;
		B[2][3]=0;
		B[3][0]=1;
		B[3][1]=1;
		B[3][2]=0;
		B[3][3]=1;
		gen.push_back(A); 
		gen.push_back(B);
   }else if(ID==358){
		m_r=new ZmodnZ(1,2);
		m_n=4; 	   
		MATRIXi8 A(4,vector<TElem>(4,0));
		MATRIXi8 B(4,vector<TElem>(4,0));
		A[0][0]=1;
		A[0][1]=1;
		A[0][2]=0;
		A[0][3]=1;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=1;
		A[1][3]=0;
		A[2][0]=1;
		A[2][1]=1;
		A[2][2]=0;
		A[2][3]=1;
		A[3][0]=0;
		A[3][1]=1;
		A[3][2]=1;
		A[3][3]=0;
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[0][3]=0;
		B[1][0]=0;
		B[1][1]=1;
		B[1][2]=0;
		B[1][3]=1;
		B[2][0]=1;
		B[2][1]=1;
		B[2][2]=0;
		B[2][3]=1;
		B[3][0]=0;
		B[3][1]=1;
		B[3][2]=0;
		B[3][3]=1;
		gen.push_back(A); 
		gen.push_back(B);
   }else if(ID==359){
		m_r=new ZmodnZ(1,2);
		m_n=4; 	   
		MATRIXi8 A(4,vector<TElem>(4,0));
		MATRIXi8 B(4,vector<TElem>(4,0));
		A[0][0]=1;
		A[0][1]=1;
		A[0][2]=1;
		A[0][3]=1;
		A[1][0]=1;
		A[1][1]=1;
		A[1][2]=1;
		A[1][3]=1;
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=0;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=0;
		B[0][0]=1;
		B[0][1]=0;
		B[0][2]=0;
		B[0][3]=0;
		B[1][0]=0;
		B[1][1]=1;
		B[1][2]=0;
		B[1][3]=0;
		B[2][0]=1;
		B[2][1]=1;
		B[2][2]=1;
		B[2][3]=0;
		B[3][0]=1;
		B[3][1]=1;
		B[3][2]=1;
		B[3][3]=0;
		gen.push_back(A); 
		gen.push_back(B);			
   }else if(ID==360){
		m_r=new ZmodnZ(1,2);
		m_n=4; 	   
		MATRIXi8 A(4,vector<TElem>(4,0));
		MATRIXi8 B(4,vector<TElem>(4,0));
		A[0][0]=0;
		A[0][1]=1;
		A[0][2]=0;
		A[0][3]=0;
		A[1][0]=1;
		A[1][1]=0;
		A[1][2]=0;
		A[1][3]=0;
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=1;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=1;
		A[3][2]=1;
		A[3][3]=0;
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[0][3]=0;
		B[1][0]=0;
		B[1][1]=0;
		B[1][2]=0;
		B[1][3]=0;
		B[2][0]=1;
		B[2][1]=1;
		B[2][2]=1;
		B[2][3]=1;
		B[3][0]=1;
		B[3][1]=1;
		B[3][2]=1;
		B[3][3]=1;
		gen.push_back(A); 
		gen.push_back(B);
   }else if(ID==367){
		m_r=new ZmodnZ(1,2);
		m_n=5; 	   
		MATRIXi8 A(5,vector<TElem>(5,0));
		A[0][0]=0;
		A[0][1]=1;
		A[0][2]=0;
		A[0][3]=0;
		A[0][4]=1;		
		A[1][0]=0;
		A[1][1]=0;
		A[1][2]=1;
		A[1][3]=1;
		A[1][4]=0;		
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=1;
		A[2][3]=1;
		A[2][4]=0;		
		A[3][0]=1;
		A[3][1]=0;
		A[3][2]=1;
		A[3][3]=0;
		A[3][4]=0;	
		A[4][0]=1;
		A[4][1]=1;
		A[4][2]=0;
		A[4][3]=1;
		A[4][4]=1;	
		gen.push_back(A);		
   }else if(ID==368){
		m_r=new ZmodnZ(1,2);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=0;
		A[0][1]=1;
		A[0][2]=1;
		A[1][0]=1;
		A[1][1]=1;
		A[1][2]=1;
		A[2][0]=1;
		A[2][1]=1;
		A[2][2]=1;		
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=1;
		B[1][1]=0;
		B[1][2]=1;
		B[2][0]=1;
		B[2][1]=1;
		B[2][2]=0;
		gen.push_back(A); 
		gen.push_back(B);		
   }else if(ID==369){
		m_r=new ZmodnZ(1,2);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=1;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=0;
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=0;		
		B[0][0]=0;
		B[0][1]=1;
		B[0][2]=1;
		B[1][0]=0;
		B[1][1]=1;
		B[1][2]=1;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=0;
		gen.push_back(A); 
		gen.push_back(B);		
   }else if(ID==370){
		m_r=new ZmodnZ(1,2);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=1;
		A[1][1]=1;
		A[1][2]=0;
		A[2][0]=1;
		A[2][1]=0;
		A[2][2]=1;		
		B[0][0]=1;
		B[0][1]=1;
		B[0][2]=0;
		B[1][0]=1;
		B[1][1]=0;
		B[1][2]=1;
		B[2][0]=1;
		B[2][1]=0;
		B[2][2]=1;
		gen.push_back(A); 
		gen.push_back(B);		
   }else if(ID==371){
		m_r=new ZmodnZ(1,2);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=0;
		A[1][1]=0;
		A[1][2]=1;
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=0;		
		B[0][0]=1;
		B[0][1]=1;
		B[0][2]=1;
		B[1][0]=0;
		B[1][1]=0;
		B[1][2]=0;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=0;
		gen.push_back(A); 
		gen.push_back(B);			
   }else if(ID==372){
		m_r=new ZmodnZ(1,2);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=1;
		A[0][1]=1;
		A[0][2]=0;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=1;
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=0;		
		B[0][0]=1;
		B[0][1]=1;
		B[0][2]=1;
		B[1][0]=0;
		B[1][1]=0;
		B[1][2]=0;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=0;
		gen.push_back(A); 
		gen.push_back(B);		
   }else if(ID==373){
		m_r=new ZmodnZ(1,2);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=1;
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=1;		
		B[0][0]=1;
		B[0][1]=1;
		B[0][2]=0;
		B[1][0]=0;
		B[1][1]=1;
		B[1][2]=0;
		B[2][0]=0;
		B[2][1]=1;
		B[2][2]=0;
		gen.push_back(A); 
		gen.push_back(B);	
   }else if(ID==374){
		m_r=new ZmodnZ(1,2);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=1;
		A[0][1]=0;
		A[0][2]=1;
		A[1][0]=0;
		A[1][1]=0;
		A[1][2]=1;
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=0;		
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=0;
		B[1][1]=1;
		B[1][2]=1;
		B[2][0]=0;
		B[2][1]=1;
		B[2][2]=1;
		gen.push_back(A); 
		gen.push_back(B);		
   }else if(ID==375){
		m_r=new ZmodnZ(1,2);
		m_n=3; 	   
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=1;
		A[0][1]=0;
		A[0][2]=1;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=1;
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=1;		
		B[0][0]=1;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=0;
		B[1][1]=0;
		B[1][2]=0;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=0;
		gen.push_back(A); 
		gen.push_back(B);				
   }else if(ID==380){  
		m_r=new ZmodnZ(1,2);
		m_n=4; 	   
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		MATRIXi8 B(4,vector<TElem>(4,0)); 	   
		A[0][0]=1;
		A[0][1]=0;
		A[0][2]=0;
		A[0][3]=1;		
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=0;
		A[1][3]=0;	
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=0;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=0;	   
		B[0][0]=1;
		B[0][1]=0;
		B[0][2]=0;
		B[0][3]=0;		
		B[1][0]=0;
		B[1][1]=0;
		B[1][2]=0;
		B[1][3]=1;		
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=0;
		B[2][3]=0;
		B[3][0]=0;
		B[3][1]=0;
		B[3][2]=0;
		B[3][3]=1;		
		gen.push_back(A);
		gen.push_back(B);
   }else if(ID==0){  	
 	   initR16(380);
       return true;   
   }else{
       return false; 
   }
   m_flag=1;
   m_Set=FR(m_r,gen);  
   return true;    
}

Mnr::Mnr(IRing* r,int n,vector<MATRIXi8>& gen){
	m_r=r;
	m_n=n;
	m_flag=0;
	m_Set=FR(r,gen);
}

string Mnr::MStr(const MATRIXi8 &t){
	string str="[";
	int n=t.size();
	for(int i=0;i<n;i++)
	{
        str+="[";
		for(int j=0;j<n;j++)
		{
			char sz[20]={0};
			sprintf(sz,"%d",t[i][j]);
			str+=sz;
			if(j<n-1)
				str+=",";
		}
		str+="]";
		if(i<n-1)
			str+=",";
	}
	str+="]";
	return str;
}

vector<MATRIXi8> Mnr::FR(IRing* r,vector<MATRIXi8>& gen){
	vector<MATRIXi8> Set;
	int n=gen[0].size();
	MATRIXi8 E(n,vector<TElem>(n,0));
	Set.push_back(E);
	for(int i=0;i<gen.size();i++)
	{
		if(!IsEqual(gen[i],E))
			Set.push_back(gen[i]);
	}
	int R=Set.size();
	int cnt=R;
	int cnt1=R;
	do{
		cnt=Set.size();
		for(int i=0;i<cnt;i++)
		{
			for(int j=0;j<cnt;j++)
			{
				MATRIXi8 IJ=mul(r,Set[i],Set[j]);
				vector<MATRIXi8>::iterator p=std::find(Set.begin(),Set.end(),IJ);
				if(p==Set.end()){
					Set.push_back(IJ);
				}
				MATRIXi8 IJ1=add(r,Set[i],Set[j]);
				p=std::find(Set.begin(),Set.end(),IJ1);
				if(p==Set.end()){
					Set.push_back(IJ1);
				}
			}
		}
		cnt1=Set.size();
	}while(cnt1>cnt);
    return Set;	
}

MATRIXi8 Mnr::add(IRing* r,const MATRIXi8 &t,const MATRIXi8 &m)
{
    int n=t.size();
	MATRIXi8 C(n,vector<TElem>(n,0));
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			C[i][j]=r->add(t[i][j],m[i][j]);
		}
	}
	return C;
}

MATRIXi8 Mnr::mul(IRing* r,const MATRIXi8 &t,const MATRIXi8 &m)
{
    int n=t.size();
	MATRIXi8 C(n,vector<TElem>(n,0));
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			int sum=0;
			for(int k=0;k<n;k++){
				sum=r->add(sum,r->mul(t[i][k],m[k][j]));
			}
			C[i][j]=sum;
		}
	}
	return C;
}

bool Mnr::nextV1(int m,vector<TElem>& v){
	int n=v.size();
	for(int i=n-1;i>=0;i--){
		if(v[i]<m-1){
			v[i]+=1;
			return true;
		}
		else if(v[i]==m-1 && i>0){
			if(v[i-1]<m-1){
				v[i-1]+=1;
				for(int j=i;j<n;j++)
					v[j]=0;
				return true;
			}
		}
	}
	return false;
}

// 调用m^(n^2)次visit
int Mnr::visitMnRm(int n,int m){
	vector<TElem> v(n*n,0);
	int cnt=0;
	do {
		++cnt;
		MATRIXi8 M(n,vector<TElem>(n,0));
		for(int k=0;k<n*n;k++){
			int i=k/n;
			int j=k%n;
			M[i][j]=v[k];
		}
		m_Set.push_back(M);
	}while(nextV1(m,v));
	return cnt;
}

int Mnr::getidx(vector<MATRIXi8> &Arr2,MATRIXi8 &Arr1){
	int N=Arr2.size();
	for(int i=0;i<N;i++){
		if(IsEqual(Arr2[i],Arr1))
			return i;
	}
	return -1;
}

bool Mnr::IsEqual(const MATRIXi8 &t,const MATRIXi8 &m){
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

void Mnr::printTable()
{
	int ID=IdRing(this);
	string str=calcRingInvariant(this);
	printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",size(),ID,str.c_str());
	string I1=calcI1(this);
	string I2=calcI2(this);   
	printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());	
	//string undoS1=calS1(this,false);	
	//printf("R%d_%d:S1=%s,S1(未经处理)=%s\n",size(),ID,S1.c_str(),undoS1.c_str());
	if(size()<100){
		printRing(this);	
	}
	else{
	   printf("环的阶太大，不在控制台打印\n");
	}
}

int Mnr::add(int a,int b)
{
	MATRIXi8 A=m_Set[a];
	MATRIXi8 B=m_Set[b];	
    int n=m_n;
	MATRIXi8 C(n,vector<TElem>(n,0));
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			C[i][j]=m_r->add(A[i][j],B[i][j]);
		}
	}
	int c=getidx(m_Set,C);
	return c;
}

int Mnr::mul(int a,int b)
{
	MATRIXi8 A=m_Set[a];
	MATRIXi8 B=m_Set[b];	
    int n=m_n;
	MATRIXi8 C(n,vector<TElem>(n,0));
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

int Mnr::size()
{
	return m_Set.size();
}

Mnr::Mnr(IRing* r,int n)
{
	m_r=r;
	m_n=n;
	int N=r->size();
	int cnt=visitMnRm(m_n,N);
	printf("cnt=%d\n",cnt);
}

string calcI2a(IRing* r){
	int IdRing(IRing* r);
	int n=r->size();
	vector<pair<int,int> > v;
   for(int i=0;i<n-1;i++){
	   int j=i+1;
		   vector<int> vi;
		   vi.push_back(i);
		   vi.push_back(j);		   
		   Subring si(r,vi);
		   int ni=si.size();
		   if(ni<n && ni>0){
			int ID=IdRing(&si);
#if 1			
			if(ID==-1 && ni==16){
				printf("i=%d,j=%d\n",i,j);
			    break;
			}
#endif
			v.push_back(make_pair(ni,ID));
		   }
   }
	std::sort(v.begin(),v.end());
	vector<tuple<int,int,int> > v1=doN2Vec(v);
	string str="[";
	for(int i=0;i<v1.size();i++)
	{
		char sz[200]={0};
		sprintf(sz,"[%d,%d,%d],",get<0>(v1[i]),get<1>(v1[i]),get<2>(v1[i]));
		str+=sz;
	}
	if(str.size()>2)
	{
		str=str.substr(0,str.size()-1);
	}
	str+="]";
	return str;
}

int g_i=0;
void findsubring(M2r *r,int n)
{
	map<pair<int,int>,pair<int,int>> M;
	int ID=IdRing(r);
	printf("R%d_%d g_i=%d\n",r->size(),ID,g_i);
	for(int i=g_i;i<r->size()-1;i++)	
	for(int j=i+1;j<r->size();j++)
	{
		//int j=i+1;
		vector<int> v;
		v.push_back(i);
		v.push_back(j);		
		Subring S1i;
		bool bn=S1i.init(r,v,16);
		if(!bn)
			continue;
		//Subring S1i(r,v);
		int ni=S1i.size();
		if(ni>16)
			continue;		
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){
			//printf("cnt1=%d:R%d_%d->i=%d,j=%d\n",cnt1,ni,ID,i,j);
			//string I1=calcI1(&S1i);
			//string I2=calcI2(&S1i);   
			//printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());				
		}		
		//if(ni==n && ID==-1||(ID==230||ID==232||ID==236||ID==241||ID==244||ID==246||ID==337||ID==384||ID==389||ID==390)||(ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==31||ID==32||ID==26||ID==34||ID==39||ID==52||ID==42||ID==39||ID==40||ID==35||ID==36||ID==27||ID==22||ID==17||ID==14||ID==7)))   
		if(ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==39))
		//if(ni==n && ni<r->size() && (ID==-1||(ID>5 && ID!=8 && ID!=10 && ID!=11 && ID!=13 && ID!=15 && ID!=16 && ID!=20 && ID!=21 && ID!=23 && ID!=24 && ID!=25 && ID!=49 && ID!=51)))	
		{
			string str=M2r::MStr(r->m_Set[i]);
			printf("%d->%s=>",i,str.c_str());
			string strj=M2r::MStr(r->m_Set[j]);
			printf("%d->%s=>",j,strj.c_str());			
			string strR=calcRingInvariant(&S1i);
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",ni,ID,strR.c_str());				
			//S1i.printTable();
			break;
		}		   
	}	   	
}

void findsubring(Mnr *r,int n)
{
#define PRINT_LOG 1	
	bool bFind=false;	
#if PRINT_LOG
    char sz[100]="0";
	sprintf(sz,"R%d_%d.txt",r->size(),time(NULL));
    ofstream fout(sz);
#endif	
    string strCmd="del ";
	strCmd+=sz;
	map<pair<int,int>,pair<int,int>> M;	
	int ID=IdRing(r);
	printf("R%d_%d g_i=%d\n",r->size(),ID,g_i);
	for(int i=g_i;i<r->size()-1;i++)		
	for(int j=i+1;j<r->size();j++)
	{
		//int j=i+1;
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);
		Subring S1i;
		bool bn=S1i.init(r,v,8);
		if(!bn)
			continue;
		//Subring S1i(r,v);
		int ni=S1i.size();
		if(ni!=8)
			continue;
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){
			printf("cnt1=%d:R%d_%d->i=%d,j=%d\n",cnt1,ni,ID,i,j);
			string I1=calcI1(&S1i);
			string I2=calcI2(&S1i);   
			printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());				
		}		
		if(ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==39))   
		//if(ID==126||ni==n && ID==-1||(ni==8 && (ID==302||ID==364||ID==365||ID==366||ID==378||ID==381||ID==383||ID==384||ID==389||ID==390)||(ID==6||ID==9||ID==12||ID==18||ID==31||ID==32||ID==26||ID==29||ID==34||ID==39||ID==40||ID==35||ID==36||ID==27||ID==22||ID==17||ID==14||ID==7)))   
		//if((ni<=32 && ID==-1)||(ni==8 && ID>5 && ID!=8 && ID!=10 && ID!=11 && ID!=13 && ID!=15 && ID!=16 && ID!=19 && ID!=20 && ID!=21 && ID!=23 && ID!=24 && ID!=25 && ID!=28 && ID!=29 && ID!=37 && ID!=41 && ID<44 && ID>51))	
		{
			string str=Mnr::MStr(r->m_Set[i]);
			printf("%d->%s=>",i,str.c_str());
			string strj=Mnr::MStr(r->m_Set[j]);
			printf("%d->%s=>",j,strj.c_str());			
			string strR=calcRingInvariant(&S1i);
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",ni,ID,strR.c_str());				
			//S1i.printTable();
#if PRINT_LOG			
			fout<<i<<"->"<<str<<","<<j<<"->"<<strj<<"=>";
			fout<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2="<<strR<<endl;
			bFind=true;
#endif
			break;
		}		   
	}
#if PRINT_LOG
	fout.close();	
	if(!bFind)	
		system(strCmd.c_str());
	else
		printf("子环表示已输出到文件%s\n",sz);
#endif	
}

void findsubring3(Mnr *r,int n)
{
#define PRINT_LOG 1	
	bool bFind=false;	
#if PRINT_LOG
    char sz[100]="0";
	sprintf(sz,"R%d_%d.txt",r->size(),time(NULL));
    ofstream fout(sz);
#endif	
    string strCmd="del ";
	strCmd+=sz;
	map<pair<int,int>,pair<int,int>> M;	
	int ID=IdRing(r);
	printf("R%d_%d g_i=%d\n",r->size(),ID,g_i);
	for(int i=g_i;i<r->size()-2;i++)		
	for(int j=i+1;j<r->size()-1;j++)
	for(int k=j+1;k<r->size();k++)		
	{
		//int j=i+1;
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);	
		v.push_back(k);			
		Subring S1i;
		bool bn=S1i.init(r,v,8);
		if(!bn)
			continue;
		//Subring S1i(r,v);
		int ni=S1i.size();
		if(ni!=8)
			continue;
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){
			//printf("cnt1=%d:R%d_%d->i=%d,j=%d,k=%d\n",cnt1,ni,ID,i,j,k);
			//string I1=calcI1(&S1i);
			//string I2=calcI2(&S1i);   
			//printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());				
		}	
		//if(ni==n && ID==-1||(ID==230||ID==232||ID==236||ID==241||ID==244||ID==246||ID==337||ID==384||ID==389||ID==390)||(ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==39||ID==22||ID==17||ID==14||ID==7)))   
		if(ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==39))   
		{
			string str=Mnr::MStr(r->m_Set[i]);
			printf("%d->%s=>",i,str.c_str());
			string strj=Mnr::MStr(r->m_Set[j]);
			printf("%d->%s=>",j,strj.c_str());			
			string strk=Mnr::MStr(r->m_Set[k]);
			printf("%d->%s=>",k,strk.c_str());			
			string strR=calcRingInvariant(&S1i);			
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",ni,ID,strR.c_str());				
			//S1i.printTable();
#if PRINT_LOG			
			fout<<i<<"->"<<str<<","<<j<<"->"<<strj<<","<<k<<"->"<<strk<<"=>";
			fout<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2="<<strR<<endl;
			bFind=true;
#endif
			break;
		}		   
	}
#if PRINT_LOG
	fout.close();	
	if(!bFind)	
		system(strCmd.c_str());
	else
		printf("子环表示已输出到文件%s\n",sz);
#endif	
}

void findsubring(IRing *r,int n)
{
#define PRINT_LOG 1	
	bool bFind=false;	
	int ID=IdRing(r);
#if PRINT_LOG
    char sz[100]="0";
	sprintf(sz,"R%d_%d_%d.txt",r->size(),ID,time(NULL));
    ofstream fout(sz);
#endif	
    string strCmd="del ";
	strCmd+=sz;
	map<pair<int,int>,pair<int,int>> M;		
	for(int i=0;i<r->size()-1;i++)		
	for(int j=i+1;j<r->size();j++)
	{
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);
		Subring S1i;
		bool bn=S1i.init(r,v,16);
		if(!bn)
			continue;
		//Subring S1i(r,v);
		int ni=S1i.size();
		if(ni!=16)
			continue;
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){
			printf("cnt1=%d:R%d_%d->i=%d,j=%d\n",cnt1,ni,ID,i,j);
			//string I1=calcI1(&S1i);
			//string I2=calcI2(&S1i);   
			//printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());				
		}		
		if(ni==16 && ID==-1)//||(ID==230||ID==232||ID==236||ID==241||ID==244||ID==246||ID==337)||(ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==39)))   
        //if((ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==39))) 
		{		
			string strR=calcRingInvariant(&S1i);
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",ni,ID,strR.c_str());				
			//S1i.printTable();
#if PRINT_LOG			
			fout<<i<<","<<j<<"=>";
			fout<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2="<<strR<<endl;
			bFind=true;
#endif
			//break;
		}		   
	}
#if PRINT_LOG
	fout.close();	
	if(!bFind)	
		system(strCmd.c_str());
	else
		printf("子环表示已输出到文件%s\n",sz);
#endif	
}

void findquotientring(IRing *r,int n)
{
#define PRINT_LOG 1	
	bool bFind=false;	
	int ID=0;//IdRing(r);
#if PRINT_LOG
    char sz[100]="0";
	sprintf(sz,"R%d_%d_%d.txt",r->size(),ID,time(NULL));
    ofstream fout(sz);
#endif	
    string strCmd="del ";
	strCmd+=sz;
	map<pair<int,int>,pair<int,int>> M;	
	for(int i=0;i<r->size()-1;i++)		
    //int i=0;
	for(int j=i+1;j<r->size();j++)
	{
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);
		Subring S1i0;
		bool bn=S1i0.init(r,v,4);
		if(!bn)
			continue;
		if(S1i0.m_Set.size()!=4)
			continue;
		vector<int> v0=v;
		v=S1i0.m_Set;
		int iret1=IsIdeal(r,v); 
		if(iret1!=1)
			continue;
		quotientRing S1i(r,v);
		int ni=S1i.size();		
		int ID=IdRing(&S1i);	
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){		
            int IDr=0;//IdRing(r);
			int IDr0=IdRing(&S1i0);
			printf("cnt1=%d:R%d_%d/R%d_%d=R%d_%d->i=%d,j=%d\n",cnt1,r->size(),IDr,S1i0.size(),IDr0,ni,ID,i,j);
		}	
		//if(ni==16 && ID==-1)
		if((ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==39))) 	
		{		
			string strR=calcRingInvariant(&S1i);
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",ni,ID,strR.c_str());				
			//S1i.printTable();
#if PRINT_LOG			
			fout<<i<<","<<j<<"=>";
			fout<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2="<<strR<<endl;
			bFind=true;
#endif
			//break;
		}
	}
#if PRINT_LOG
	fout.close();	
	if(!bFind)	
		system(strCmd.c_str());
	else
		printf("商环表示已输出到文件%s\n",sz);
#endif	
}

IRing* newR16R2(int ij)
{
	int i=(ij-1)%390+1;
	int j=(ij-1)/390+1;
	IRing* ri=NULL;
	M2r* r16=new M2r;
	bool b=r16->initR16(i);
	if(b){
	   ri=r16;
	}else{
		delete r16;
		r16=NULL;
		Mnr* r16a=new Mnr;
		bool ba=r16a->initR16(i);
		if(ba){
		   ri=r16a;
		}	
		else{
			delete r16a;
			r16a=NULL;
		}				
	}	
	if(!ri)
		return NULL;
	if(j!=1 && j!=2){
		delete ri;
		return NULL;		
	}
    IRing* rj=new ZmodnZ(3-j,2*(3-j));
	if(!rj){
		delete ri;
		return NULL;
	}
	DecompositionRing* r= new DecompositionRing(ri,rj);
	r->m_flag=1;
	return r;
}

int main(int argc, char* argv[])
{ 
	if(argc>1)
		g_i=atoi(argv[1]);

	if(1){	
	   for(int i=1;i<=780;i++)
	   {
		   IRing* r=newR16R2(i);
		   if(r){
			   printf("R16R2_%d\n",i);
			   //findsubring(r,16);
			   findquotientring(r,16);
			   delete r;
			   r=NULL;
		   }
	   }
	}   
	
	if(0){
		for(int i=1;i<=390;i++){
		   M2r r16;
		   bool b=r16.initR16(i);
		   if(b){
			   int ID=IdRing(&r16);
			   printf("%d:R16_%d\n",i,ID);
			   //findquotientring(&r16,8);
			   findsubring(&r16,8);
		   }else{
				Mnr r16a;
				bool ba=r16a.initR16(i);
				if(ba){
				   int ID=IdRing(&r16a);
				   printf("%d:R16_%d\n",i,ID);
				   //findquotientring(&r16a,8);
				   findsubring3(&r16a,8);
				}		   
		   }
		}
	}
	
	if(0){
		//ZmodnZ r(4,32);
		M2r r4;
		r4.initJ(2);
		//M2r m2r(&r4);
		Mnr m2r(&r4,3);
		findsubring(&m2r,16);
	}
	
	if(0){
	   Mnr r16;
	   r16.initR8();
	   //r16.printTable(); 
	   findsubring3(&r16,16);
	}
	if(0){
	   M2r r16;
	   r16.initR8();
	   //r16.printTable(); 
	   findsubring(&r16,16);		
	}
   
   return 0;
}