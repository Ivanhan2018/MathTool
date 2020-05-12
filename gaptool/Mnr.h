#ifndef MNR_H
#define MNR_H

#include"IRing.h"

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

#endif