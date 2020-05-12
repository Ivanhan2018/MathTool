#include"IRing.h"

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
	template<class T> vector<vector<int> > getTable(T *obp,int n,int(T::*p)(int,int));
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
	void initR16(int ID=0);		
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

void M2r::initR16(int ID){
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
   }else if(ID==302){
		M2r *r=new M2r();
        r->initJ(2);
		m_r=r;
		A[0][0]=0;
		A[0][1]=1;
		A[1][0]=1;
		A[1][1]=2;	
		gen.push_back(A);	
   }else{
	    initR16(115);
		return;
   }	   
   m_flag=1;
   m_Set=FR(m_r,gen);   
}

void M2r::initR8(int ID){
   MATRIXi A(2,vector<int>(2,0));
   A[0][0]=0;
   A[0][1]=1;
   A[1][0]=1;
   A[1][1]=0;  
   vector<MATRIXi> gen;
   gen.push_back(A);
   m_r=new ZmodnZ(1,4);
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

typedef vector<vector<unsigned char> > MATRIXi8;

// n阶全矩阵环Mn(r)
struct Mnr:public IRing
{
public:
	// 静态函数
   static int getidx(vector<MATRIXi8> &Arr2,MATRIXi8 &Arr1);
   static bool IsEqual(const MATRIXi8 &t,const MATRIXi8 &m);	
   static bool nextV1(int m,vector<unsigned char>& v);
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
	void initR16(int ID=0);	
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
   MATRIXi8 A(4,vector<unsigned char>(4,0));
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
   MATRIXi8 A(4,vector<unsigned char>(4,0));
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
   MATRIXi8 A(4,vector<unsigned char>(4,0));
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
   MATRIXi8 A(4,vector<unsigned char>(4,0));	
   if(ID==13){
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
   }else if(ID==24){
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
   }else{
	   initR8(13);
       return;	   
   }
   vector<MATRIXi8> gen;
   gen.push_back(A);
   m_r=new ZmodnZ(1,4);
   m_flag=1;
   m_n=4;
   m_Set=FR(m_r,gen);      
}

void Mnr::initR16(int ID){
   MATRIXi8 A(4,vector<unsigned char>(4,0));	
   if(ID==113){
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
   }else if(ID==114){
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
   }else{
	   initR16(113);
       return;  
   }
   vector<MATRIXi8> gen;
   gen.push_back(A);
   m_r=new ZmodnZ(1,4);
   m_flag=1;
   m_n=4;
   m_Set=FR(m_r,gen);      
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
	MATRIXi8 E(n,vector<unsigned char>(n,0));
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
	MATRIXi8 C(n,vector<unsigned char>(n,0));
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
	MATRIXi8 C(n,vector<unsigned char>(n,0));
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

bool Mnr::nextV1(int m,vector<unsigned char>& v){
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
	vector<unsigned char> v(n*n,0);
	int cnt=0;
	do {
		++cnt;
		MATRIXi8 M(n,vector<unsigned char>(n,0));
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
	MATRIXi8 C(n,vector<unsigned char>(n,0));
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
	MATRIXi8 C(n,vector<unsigned char>(n,0));
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

void test1()
{
   ZmodnZ r2_1(2,4);
   r2_1.printTable();  

   ZmodnZ r2_2(1,2);  
   r2_2.printTable();

   ZmodnZ r4_2(2,8);
   r4_2.printTable();  

   ZmodnZ r4_3(1,4);  
   r4_3.printTable();
   
   M2r m2r4_2(&r4_2);
   m2r4_2.printTable(); 

   M2r m2r4_3(&r4_3);
   m2r4_3.printTable(); 

   vector<int> v;
   v.push_back(2);
   Subring r2_1a(&r4_3,v);
   r2_1a.printTable();
   
   Mnr m2r2_1(&r2_1,2);
   m2r2_1.printTable(); 

   Mnr m3r2_1(&r2_1,3);
   m3r2_1.printTable();   
   
   Mnr m2r2_2(&r2_2,2);
   m2r2_2.printTable(); 

   Mnr m3r2_2(&r2_2,3);
   m3r2_2.printTable();     
}

int test2()
{
   ZmodnZ r2_1(2,4);  
   M2r G4;
   G4.initG(2);
   M2r H4;
   H4.initH(2);   
   DecompositionRing r8_29(&G4,&r2_1);
   DecompositionRing r8_34(&H4,&r2_1); 
   string I1=calcI1(&r8_29);
   string I2=calcI2(&r8_29);   
   printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());
   r8_29.printTable();
   I1=calcI1(&r8_34);
   I2=calcI2(&r8_34);   
   printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());  
   r8_34.printTable();
   
   //int IDs[]={115,201,202,203,204,205};
   int IDs[]={201};
   for(int i=0;i<sizeof(IDs)/sizeof(IDs[0]);i++){
	   M2r R16;
	   R16.initR16(IDs[i]);
	   R16.printTable();
	   for(int j=0;j<R16.size();j++){				
			string str=M2r::MStr(R16.m_Set[j]);
			printf("%d->%s\n",j+1,str.c_str());
		}	   
   }
   return 0;
   int nIDs[]={113,114};
   for(int i=0;i<sizeof(nIDs)/sizeof(nIDs[0]);i++){
	   Mnr R16;
	   R16.initR16(nIDs[i]);
	   R16.printTable();
   } 
   {
	   Mnr R16;
	   R16.initE(4);
	   R16.printTable();
   }  
   {
	   Mnr R16;
	   R16.initF(4);
	   R16.printTable();
   }    
   return 0;

   test1();
   for(int i=2;i<10;i++)
   {
	   if(i!=4)
		   continue;
	   // A、B、C系列的有限环是1阶矩阵表示
	   ZmodnZ A(i*i,i*i*i*i);
	   printf("A%d=",i*i);
	   A.printTable();	
	   ZmodnZ B(i,i*i*i);
	   printf("B%d=",i*i);
	   B.printTable();	
	   ZmodnZ C(1,i*i);
	   printf("C%d=",i*i);
	   C.printTable();

	   // D、G、H、I、J、K系列的有限环是2阶矩阵表示
	   M2r D;
	   D.initD(i);
	   printf("D%d=",i*i);
	   D.printTable();

	   // E、F系列的有限环是4阶矩阵表示
	   Mnr E;
	   E.initE(i);
	   printf("E%d=",i*i);
	   E.printTable();	 
	   Mnr F;
	   F.initF(i);
	   printf("F%d=",i*i);
	   F.printTable();	
	   DecompositionRing Fa;
	   Fa.initF(i);
	   printf("F%d=",i*i);
	   Fa.printTable(); 

	   M2r G;
	   G.initG(i);
	   printf("G%d=",i*i);
	   G.printTable();	   
	   M2r H;
	   printf("H%d=",i*i);	   
	   H.initH(i);
	   H.printTable(); 
	   M2r I;
	   printf("I%d=",i*i);	   
	   I.initI(i);
	   I.printTable(); 
	   M2r J;
	   printf("J%d=",i*i);	   
	   J.initJ(i);
	   J.printTable(); 	
	   M2r K;
	   printf("K%d=",i*i);	   
	   K.initK(i);
	   K.printTable();	  
   }	
   return 0;
}

int main()
{ 
   return 0;
}