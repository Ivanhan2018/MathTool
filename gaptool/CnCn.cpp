#include"ZmodnZ.h"
#include<fstream>

void writeTable(IRing* r,const char *path){
   ofstream fout(path);
   int n=r->size();
   fout<<"[R"<<n<<"Add]"<<endl;   
   for(int i=0;i<n;i++){
	   for(int j=0;j<n;j++){
		  int ij=r->add(i,j);
		  fout<<ij+1<<" ";
	   } 
       fout<<endl;   
   }
   fout<<"[R"<<n<<"Mul]"<<endl;   
   for(int i=0;i<n;i++){
	   for(int j=0;j<n;j++){
		  int ij=r->mul(i,j);
		  fout<<ij+1<<" "; 
	   } 
       fout<<endl;	   
   }	
}

// 加群为Cn×Cn的有限环
struct CnCn:public IRing
{
public:
	//  静态函数  
public:
	// 实现抽象基类的方法
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// 构造函数
	CnCn(int n,int id);  
	// 成员变量  	
	int m_n;
	int m_id;	
	vector<int> m_vA;
	vector<int> m_vM;
};

void CnCn::printTable()
{
	bool b1=IsRing(this);
	if(!b1){
		printf("不是环\n");
		return;
	}	
	int ID=IdRing(this);
	string str=calcRingInvariant(this);
	printf("R%d_%d_C%dC%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",size(),ID,this->m_n,this->m_n,this->m_id,str.c_str());
	string Q1=calcQ1(this);
	printf("Q1=%s\n",Q1.c_str());	
	char sz[100]={0};
	sprintf(sz,"C%dC%d_%d.txt",this->m_n,this->m_n,this->m_id);	
	writeTable(this,sz);
}

int CnCn::add(int a,int b)
{
	int c=m_vA[a*m_n*m_n+b];
	return c;
}

int CnCn::mul(int a,int b)
{
	int c=m_vM[a*m_n*m_n+b];
	return c;
}

int CnCn::size()
{
	return m_n*m_n;
}

CnCn::CnCn(int n,int id)
{
	m_n=n;
	m_id=id;
	ZmodnZ r(1,n);
	vector<int> A(n);
	vector<int> B(n);	
	for(int i=0;i<n;i++){
		A[i]=i;	
		B[i]=i;
	}
	map<pair<int,int>,int> mT;
	vector<pair<int,int> > vT(n*n);
	m_vA.resize(n*n*n*n);
	m_vM.resize(n*n*n*n);
	for(int i=0;i<n;i++)
	for(int j=0;j<n;j++)
	{
        int k=n*i+j;
		vT[k]=make_pair(A[i],B[j]);
		mT.insert(make_pair(make_pair(A[i],B[j]),k));
	}	
	for(int i=0;i<n*n;i++)
	for(int j=0;j<n*n;j++)
	{
		pair<int,int> a=vT[i];
		pair<int,int> b=vT[j];
		pair<int,int> s=make_pair(r.add(a.first,b.first),r.add(a.second,b.second));
		pair<int,int> m=make_pair(r.mul(a.first,b.first),r.mul(a.second,b.second));
		if(id==2){
			int k2=r.add(b.first,b.second);
			m=make_pair(r.mul(k2,a.first),r.mul(k2,a.second));			
		}
		m_vA[n*n*i+j]=mT[s];
		m_vM[n*n*i+j]=mT[m];		
	}	
}

int main(int argc, char **argv)
{
	char sz1[100]={0};
	if(argc<2)
	{
		printf("请输入n：");
		scanf("%s",sz1);
	}
	else
		strcpy(sz1,argv[1]);
		
	char sz2[100]={0};
	if(argc<3)
	{
		printf("请输入id：");
		scanf("%s",sz2);
	}
	else
		strcpy(sz2,argv[2]);	
	
	int n=atoi(sz1);
	int id=atoi(sz2);	
	CnCn r(n,id);
	r.printTable();

	//system("pause");
	return 0;
}
