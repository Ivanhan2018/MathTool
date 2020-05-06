#include"IRing.h"

// ����ѭ����mZ/nZ����������m|n
struct ZmodnZ:public IRing
{
public:
	// ��̬����
public:
	// ʵ�ֳ������ķ���
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// ���캯��
	ZmodnZ(int m,int n);
	// ��Ա����	
	// ��Ա����
	int m_m;
	int m_n;// ����Ϊģn�ӷ���ģn�˷�	
};

void ZmodnZ::printTable()
{
	int ID=IdRing(this);
	string str=calcRingInvariant(this);
	printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",size(),ID,str.c_str());	
	//string S1=calS1(this,true);
	//string undoS1=calS1(this,false);	
	//printf("R%d_%d:S1=%s,S1(δ������)=%s\n",size(),ID,S1.c_str(),undoS1.c_str());
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

typedef vector<vector<int> > MATRIXi;

// 2��ȫ����M2(r)
struct M2r:public IRing
{
public:
	// ��̬����
   static int getidx(vector<MATRIXi> &Arr2,MATRIXi &Arr1);
   static bool IsEqual(const MATRIXi &t,const MATRIXi &m);	
public:
	// ʵ�ֳ������ķ���
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// ���캯��
	M2r(IRing* r);
	// ��Ա����	
	// ��Ա����
	vector<MATRIXi> m_Set;
	IRing* m_r;
};

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
	//printf("R%d_%d:S1=%s,S1(δ������)=%s\n",size(),ID,S1.c_str(),undoS1.c_str());
	if(size()<100){
		//printRing(this);	
	}
	else{
	   printf("���Ľ�̫�󣬲��ڿ���̨��ӡ\n");
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

// n��ȫ����Mn(r)
struct Mnr:public IRing
{
public:
	// ��̬����
   static int getidx(vector<MATRIXi8> &Arr2,MATRIXi8 &Arr1);
   static bool IsEqual(const MATRIXi8 &t,const MATRIXi8 &m);	
   static bool nextV1(int m,vector<unsigned char>& v);
public:
	// ʵ�ֳ������ķ���
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// ���캯��
	Mnr(IRing* r,int n);
	// ��Ա����	
    int visitMnRm(int n,int m);	
	// ��Ա����
	vector<MATRIXi8> m_Set;
	IRing* m_r;
	int m_n;
};

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

// ����m^(n^2)��visit
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
	//printf("R%d_%d:S1=%s,S1(δ������)=%s\n",size(),ID,S1.c_str(),undoS1.c_str());
	if(size()<100){
		//printRing(this);	
	}
	else{
	   printf("���Ľ�̫�󣬲��ڿ���̨��ӡ\n");
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

int main()
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
   
   return 0;
}