#ifndef M2R_H
#define M2R_H

#include"IRing.h"

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

#endif