#define R16_I1I2
#include"ZmodnZ.h"
#include"quotientRing.h"
#include<vector> 
#include<iostream>
#include<fstream> 
#include<string> 
#include<ctime>
using namespace std;

std::vector<string> split( const std::string& str, const std::string& delims, unsigned int maxSplits = 0)
{
	std::vector<string> ret;
	unsigned int numSplits = 0;
	// Use STL methods 
	size_t start, pos;
	start = 0;
	do 
	{
		pos = str.find_first_of(delims, start);
		if (pos == start)
		{
			// Do nothing
			start = pos + 1;
		}
		else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
		{
			// Copy the rest of the std::string
			ret.push_back( str.substr(start) );
			break;
		}
		else
		{
			// Copy up to delimiter
			ret.push_back( str.substr(start, pos - start) );
			start = pos + 1;
		}
		// parse up to next real data
		start = str.find_first_not_of(delims, start);
		++numSplits;
	} while (pos != std::string::npos);
	return ret;
}

// ����������ʾ���ݱ����
class CRingDataAItem
{
public:
	CRingDataAItem() 
	{
		m_n=0;
		m_ID=0;
		m_n0=0;
        m_vfvm="";		
	}

	int m_n;
	int m_ID;	
	int m_n0;
	string m_vfvm;
};

map<pair<int,int>,CRingDataAItem> g_mapRingDataACache;

const CRingDataAItem * Find(int n,int ID)
{
	map<pair<int,int>,CRingDataAItem>::const_iterator it;
	it = g_mapRingDataACache.find(make_pair(n,ID));
	if( it != g_mapRingDataACache.end() )
		return &(it->second);
	return NULL;
}

// ����������ʾ���ݱ�����
int LoadData(char * pszFilePath)		//�����ļ��ж�ȡ���ݡ�
{
	if( !g_mapRingDataACache.empty() )
		return 2;//2�Ѿ�����������

	FILE * fp =fopen(pszFilePath, "r");
	if( fp == NULL )
		return 1;//1���ļ�ʧ��

	char sz[200] = {0};
	CRingDataAItem item;
	int n = 0;
	n = fscanf(fp, "%s", sz);
	while( n > 0 && !feof(fp) )
	{
		n = fscanf(fp, "%d,%d,%d,%s\n", &item.m_n, &item.m_ID, &item.m_n0,sz);
		if( n > 0 )
		{
			item.m_vfvm = sz;
			g_mapRingDataACache.insert( make_pair(make_pair(item.m_n,item.m_ID),item));
		}
	}
	fclose(fp);
	return 0;//0�ɹ�
}

void printRing0(IRing* r,int ID){
   int n=r->size();
   printf("static int g_R%d_%dAdd[%d][%d]={\n",n,ID,n,n);   
   for(int i=0;i<n;i++){
	   printf("{"); 
	   for(int j=0;j<n;j++){
		  int ij=r->add(i,j);
		  printf("%d",ij);
		  if(j<n-1)
			printf(",");  
	   } 
       printf("},\n");   
   }
   printf("};\n");    
   printf("static int g_R%d_%dMul[%d][%d]={\n",n,ID,n,n);   
   for(int i=0;i<n;i++){
	   printf("{"); 
	   for(int j=0;j<n;j++){
		  int ij=r->mul(i,j);
		  printf("%d",ij);
		  if(j<n-1)
			printf(",");  
	   } 
       printf("},\n");   
   }
   printf("};\n"); 
}

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

typedef vector<int> Polynomial;// ��������ʾ����ʽ

// r[x]/(a)��(m)r[x]/(n)r[x]
struct PolynomialRing:public IRing
{
public:
	// ��̬����
	static bool IsEqual(Polynomial& a,Polynomial& b);// �Ƿ���� 
	static Polynomial add(Polynomial& a,Polynomial& b,int ord);// �ӷ�	
	static Polynomial mul(Polynomial& a,Polynomial& b,int ord);// �˷�
	static Polynomial mod(Polynomial& a,Polynomial& b,int ord);// ȡ��	
	static bool is_zero(Polynomial& a);// �Ƿ�Ϊ�����ʽ	
	static bool is_irr(Polynomial& a,int ord);// �Ƿ񲻿�Լ��Error���Բ�ͨ�� 
	static bool is_pri(Polynomial& a,int ord);// �Ƿ�ԭ��Error���Բ�ͨ��
	static Polynomial pow(Polynomial& a,unsigned int b,int ord);// ����ָ���㷨
	static Polynomial gcd(Polynomial& a,Polynomial& b,int ord);
	static vector<int> FactorsInt(int n);
	static bool nextV1(int m,vector<int>& v);
	static string Itos(int Num);
	static string sPoly(Polynomial& a);
	static vector<Polynomial> FR(Polynomial &a,vector<Polynomial> &v,int ord);
public:
	// ʵ�ֳ������ķ���
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// ���캯��
	PolynomialRing();
	PolynomialRing(IRing *r,Polynomial& a);
	// ��������
	~PolynomialRing();	
	// ��Ա����	
	void init(IRing *r,Polynomial& a);
	void initFR(IRing *r,vector<Polynomial> &m,Polynomial &n);	
	void initFR(IRing *r,Polynomial &m,Polynomial &n);
	void initFR(IRing *r,Polynomial &m1,Polynomial &m2,Polynomial &n);	
	bool init(int n,int ID);	
	int visitVnRm(int n,int m);
	bool initR4(int ID=0);	
	bool initR8(int ID=0);
	bool initR16(int ID=0);
	bool initR36(int ID=0);		
	// ��Ա����
	vector<Polynomial> m_Set;
	Polynomial m_a;// ����Ϊģa�ӷ���ģa�˷�
	vector<Polynomial> m_gen;
    IRing* m_r;	
	int m_flag;// PolynomialRing��������ʱ�Ƿ��ͷ�m_rָ����ڴ�	
};

PolynomialRing::PolynomialRing(){
	m_r=NULL;
	m_flag=0;	
}

PolynomialRing::~PolynomialRing(){
	if(m_flag==1 && m_r!=NULL){
		delete m_r;
		m_r=NULL;
	}
}

PolynomialRing::PolynomialRing(IRing *r,Polynomial& a){
	init(r,a);
}

void PolynomialRing::init(IRing *r,Polynomial& a){
	m_a=a;
	m_r=r;
	int N=r->size();
	unsigned int pOrd = a.size()-1;/*����ʽ��ߴ������*/	
	int cnt=visitVnRm(pOrd,N);
	//printf("cnt=%d\n",cnt);
}

bool PolynomialRing::init(int n,int ID){
	const CRingDataAItem * pItem = Find(n,ID);
	if(pItem){		
		vector<Polynomial> gen;		
		vector<string> vv=split(pItem->m_vfvm,";");
		for(int i=0;i<vv.size();i++){
			Polynomial vf;
			vector<string> v=split(vv[i],",");
			for(int i=0;i<v.size();i++){
				int vi=atoi(v[i].c_str());	
				vf.push_back(vi);
			}
			gen.push_back(vf);
		}
		Polynomial vf=gen[0];
		gen.erase(gen.begin());
		m_r=new ZmodnZ(1,pItem->m_n0); 
		m_flag=1;  
		initFR(m_r,gen,vf);
		return true;		
	}	
	return false;
}

bool PolynomialRing::initR4(int ID){
	bool bDone = init(4,ID);
	if(bDone){
		return true;		
	}
	Polynomial vf,vm1;
	if(ID==5){ 
		vm1.push_back(0);
		vm1.push_back(1);	
		vf.push_back(0);
		vf.push_back(0);
		vf.push_back(0);
		vf.push_back(1);
		m_r=new ZmodnZ(1,2); 
		m_flag=1;		
        initFR(m_r,vm1,vf);	
		return true;		
	}else if(ID==9){  
		//vf.push_back(1);
		vf.push_back(0);
		vf.push_back(0);
		vf.push_back(1);
	}else if(ID==10){
		vf.push_back(0);
		vf.push_back(1);
		vf.push_back(1);
	}else if(ID==11){
		vf.push_back(1);
		vf.push_back(1);
		vf.push_back(1);
	}else{
		return false;
	}
	m_r=new ZmodnZ(1,2); 
	m_flag=1;  
	init(m_r,vf);
	return true;
}

bool PolynomialRing::initR8(int ID){
	bool bDone = init(8,ID);
	if(bDone){
		return true;		
	}	
   Polynomial vf,vm1,vm2;
   if(ID==11){  
		vm1.push_back(0);
		vm1.push_back(1);
		vm1.push_back(1);
		vm2.push_back(2);
		vf.push_back(0);
		vf.push_back(0);
		vf.push_back(1);
		vf.push_back(1);
		m_r=new ZmodnZ(1,4);
		initFR(m_r,vm1,vm2,vf);
   }else if(ID==26){
		vm1.push_back(0);
		vm1.push_back(0);
		vm1.push_back(0);	
		vm1.push_back(1);
		vm1.push_back(1);
		vm2.push_back(0);
		vm2.push_back(1);
		vm2.push_back(0);
		vm2.push_back(0);
		vm2.push_back(1);		
		vf.push_back(0);
		vf.push_back(0);
		vf.push_back(1);
		vf.push_back(1);
		vf.push_back(1);
		vf.push_back(1);		
		m_r=new ZmodnZ(1,2);
		initFR(m_r,vm1,vm2,vf);	   
   }else if(ID==51){	
		vf.push_back(1);
		vf.push_back(0);
		vf.push_back(0);
		vf.push_back(1);
		m_r=new ZmodnZ(1,2);
		init(m_r,vf);
   }else if(ID==52){	
		vf.push_back(1);
		vf.push_back(0);
		vf.push_back(1);
		vf.push_back(1);
		m_r=new ZmodnZ(1,2);
		init(m_r,vf); 		
   }else{
	   return false;
   }
   m_flag=1;
   return true;
}
	
bool PolynomialRing::initR16(int ID){
	bool bDone = init(16,ID);
	if(bDone){
		return true;		
	}	
	Polynomial vf,vm1,vm2;
   if(ID==16){  
		vm1.push_back(4);
		vm1.push_back(6);
		vm2.push_back(4);
		vm2.push_back(7);
		vf.push_back(0);
		vf.push_back(0);
		vf.push_back(1);
		m_r=new ZmodnZ(1,8);
		initFR(m_r,vm1,vm2,vf);  
   }else if(ID==17){	
		vm1.push_back(6);
		vm1.push_back(1);
		vm2.push_back(6);
		vm2.push_back(3);  
		vf.push_back(0);
		vf.push_back(0);
		vf.push_back(1);
		m_r=new ZmodnZ(1,8);
		initFR(m_r,vm1,vm2,vf); 
   }else if(ID==92){	
		vf.push_back(0);
		vf.push_back(2);
		vf.push_back(1);
		m_r=new ZmodnZ(1,4);
		init(m_r,vf);		
   }else if(ID==98){	
		vm1.push_back(0);
		vm1.push_back(2);
		vm2.push_back(2);  
		vf.push_back(1);
		vf.push_back(0);
		vf.push_back(1);
		m_r=new ZmodnZ(1,8);
		initFR(m_r,vm1,vm2,vf); 
   }else if(ID==99){	
		vm1.push_back(0);
		vm1.push_back(2);
		vm2.push_back(2);  
		vf.push_back(1);
		vf.push_back(1);
		vf.push_back(1);
		m_r=new ZmodnZ(1,8);
		initFR(m_r,vm1,vm2,vf); 		
   }else if(ID==104){	
		vf.push_back(0);
		vf.push_back(1);
		vf.push_back(1);
		m_r=new ZmodnZ(1,4);
		init(m_r,vf); 
   }else if(ID==105){	
		vf.push_back(1);
		vf.push_back(0);
		vf.push_back(1);
		m_r=new ZmodnZ(1,4);
		init(m_r,vf); 
   }else if(ID==106){	
		vf.push_back(1);
		vf.push_back(1);
		vf.push_back(1);
		m_r=new ZmodnZ(1,4);
		init(m_r,vf);
   }else if(ID==110){	
		vf.push_back(0);
		vf.push_back(0);
		vf.push_back(1);
		m_r=new ZmodnZ(1,4);
		init(m_r,vf);  
   }else if(ID==302){	
		vm1.push_back(1);
		vm1.push_back(1);
		vm1.push_back(1);  
		vm2.push_back(1);
		vm2.push_back(1);
		vm2.push_back(1);
		vm2.push_back(1);
		vm2.push_back(1);   
		vf.push_back(0);	
		vf.push_back(1);
		vf.push_back(1);
		vf.push_back(0);
		vf.push_back(1);
		vf.push_back(1);		
		m_r=new ZmodnZ(1,2);
		initFR(m_r,vm1,vm2,vf); 
   }else if(ID==315){	
		vm1.push_back(0);
		vm1.push_back(1);
		vm1.push_back(1);  
		vm2.push_back(1);
		vm2.push_back(1);
		vm2.push_back(1);
		vm2.push_back(0);
		vm2.push_back(1);   
		vf.push_back(0);
		vf.push_back(0);
		vf.push_back(1);		
		vf.push_back(1);
		vf.push_back(1);
		vf.push_back(1);		
		m_r=new ZmodnZ(1,2);
		initFR(m_r,vm1,vm2,vf); 		
   }else if(ID==326){	
		vm1.push_back(0);
		vm1.push_back(1);
		vm1.push_back(1);  
		vm2.push_back(1);
		vm2.push_back(1);
		vm2.push_back(1);
		vm2.push_back(0);
		vm2.push_back(1);   
		vf.push_back(0);	
		vf.push_back(1);
		vf.push_back(1);
		vf.push_back(0);
		vf.push_back(1);
		vf.push_back(1);		
		m_r=new ZmodnZ(1,2);
		initFR(m_r,vm1,vm2,vf);
   }else if(ID==331){	
		vm1.push_back(0);
		vm1.push_back(1);
		vm1.push_back(1);  
		vm2.push_back(1);
		vm2.push_back(0);
		vm2.push_back(0);
		vm2.push_back(1);   
		vf.push_back(1);
		vf.push_back(0);
		vf.push_back(0);		
		vf.push_back(1);
		vf.push_back(1);
		vf.push_back(1);		
		m_r=new ZmodnZ(1,2);
		initFR(m_r,vm1,vm2,vf);  
   }else if(ID==361){
		vm1.push_back(0);
		vm1.push_back(0);
		vm1.push_back(0);  
		vm1.push_back(1);
		vm2.push_back(0);
		vm2.push_back(0);
		vm2.push_back(1);	   
		vf.push_back(0);
		vf.push_back(0);  
		vf.push_back(0);
		vf.push_back(0);
		vf.push_back(0);  
		vf.push_back(0);
		vf.push_back(1);
		m_r=new ZmodnZ(1,2);
		initFR(m_r,vm1,vm2,vf);  		
   }else if(ID==377){	
		vm1.push_back(1);
		vm1.push_back(1);
		vm1.push_back(1);  
		vm2.push_back(1);
		vm2.push_back(1);
		vm2.push_back(1);
		vm2.push_back(1);
		vm2.push_back(1);	
		vf.push_back(0);
		vf.push_back(0);
		vf.push_back(1);		
		vf.push_back(1);
		vf.push_back(1);
		vf.push_back(1);
		m_r=new ZmodnZ(1,2);
		initFR(m_r,vm1,vm2,vf); 
   }else if(ID==383){	
		vf.push_back(1);	
		vf.push_back(0);
		vf.push_back(1);
		vf.push_back(0);
		vf.push_back(1);		
		m_r=new ZmodnZ(1,2);
		init(m_r,vf);		
   }else if(ID==385){	
		vf.push_back(1);	
		vf.push_back(0);
		vf.push_back(0);
		vf.push_back(0);
		vf.push_back(1);		
		m_r=new ZmodnZ(1,2);
		init(m_r,vf);			
   }else if(ID==387){	
		vf.push_back(0);	
		vf.push_back(1);
		vf.push_back(0);
		vf.push_back(0);
		vf.push_back(1);		
		m_r=new ZmodnZ(1,2);
		init(m_r,vf);		
   }else if(ID==388){	
		vf.push_back(0);	
		vf.push_back(1);
		vf.push_back(0);
		vf.push_back(1);
		vf.push_back(1);		
		m_r=new ZmodnZ(1,2);
		init(m_r,vf); 
   }else if(ID==0){	
	   initR16(388);
	   return true; 		
   }else{
	   return false;
   }
   m_flag=1;
   return true;   
}	

bool PolynomialRing::initR36(int ID){
	bool bDone = init(36,ID);
	if(bDone){
		return true;		
	}		
	Polynomial vf,vm1,vm2;
   if(ID==1){  
   }else if(ID==99){	
		vf.push_back(1);
		vf.push_back(1);
		vf.push_back(1);
		m_r=new ZmodnZ(1,6);
		init(m_r,vf);	
   }else if(ID==119){	
		vf.push_back(1);
		vf.push_back(0);
		vf.push_back(1);
		m_r=new ZmodnZ(1,6);
		init(m_r,vf); 	
   }else{
	   return false;
   }
   m_flag=1;
   return true;    
}

void PolynomialRing::initFR(IRing *r,vector<Polynomial> &m,Polynomial &n)
{
    if(m.size()==0){
		init(r,n);
		return;
	}
	m_r=r;
	m_a=n;
	m_Set=FR(n,m,r->size());
	m_gen=m;
}

void PolynomialRing::initFR(IRing *r,Polynomial &m,Polynomial &n)
{
	m_r=r;
	m_a=n;
	vector<Polynomial> v;
	v.push_back(m);
	m_Set=FR(n,v,r->size());
	m_gen=v;
}

void PolynomialRing::initFR(IRing *r,Polynomial &m1,Polynomial &m2,Polynomial &n)
{
	m_r=r;
	m_a=n;
	vector<Polynomial> v;
	v.push_back(m1);
	v.push_back(m2);	
	m_Set=FR(n,v,r->size());
	m_gen=v;
}

vector<Polynomial> PolynomialRing::FR(Polynomial &a,vector<Polynomial> &v,int ord)
{
	vector<Polynomial> S;
	Polynomial p0;
	p0.push_back(0);
	S.push_back(p0);
	for(int i=0;i<v.size();i++)
	{
	    Polynomial r1=mod(v[i],a,ord);
		S.push_back(r1);
	}  		
	int cnt=0;
	int cnt1=S.size();
	while(cnt1>cnt){
		cnt=S.size();
		for(int i=0;i<S.size();i++){
			for(int j=0;j<S.size();j++){
				Polynomial ij=mod(mul(S[i],S[j],ord),a,ord);
				vector<Polynomial>::iterator p=std::find(S.begin(),S.end(),ij);
				if(p==S.end()){
					S.push_back(ij);
				}
				Polynomial ij1=mod(add(S[i],S[j],ord),a,ord);
				vector<Polynomial>::iterator p1=std::find(S.begin(),S.end(),ij1);
				if(p1==S.end()){
					S.push_back(ij1);
				}				
			}
		}		
		cnt1=S.size();
	}
    return S;
}

// ��һ������ת�����ַ���
string PolynomialRing::Itos(int Num)   
{
	char buffer[100]={0};
	sprintf(buffer,"%d",Num);
	string strBuf=buffer;

	return strBuf;
}

string PolynomialRing::sPoly(Polynomial& a)
{
	string str="[";
	int n=a.size();
	for(int i=0;i<n;i++)
	{
		str+=Itos(a[i]);
		if(i<n-1)
		{
			str+=",";
		}
	}
	str+="]";
	return str;
}

bool PolynomialRing::nextV1(int m,vector<int>& v){
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

// ����m^n��visit
int PolynomialRing::visitVnRm(int n,int m){
	vector<int> v(n,0);
	int cnt=0;
	do {
		++cnt;
		Polynomial p=v;
		while( !p.empty() && !p.back() ) 
			p.pop_back(); 
		if (p.empty()) {// ����
			p.push_back(0);
		}	
		m_Set.push_back(p);
	}while(nextV1(m,v));
	return cnt;
}

void PolynomialRing::printTable()
{
	int ID1=IdRing(m_r);
	int ID=IdRing(this);
	string str1=sPoly(m_a);
	string str2="";
	if(m_gen.size()>0){
		str2+="[";
		for(int i=0;i<m_gen.size();i++){
			str2+=sPoly(m_gen[i]);
			if(i<m_gen.size()-1)
				str2+=",";
		}
		str2+="],";
	}
	string str=calcRingInvariant(this);
	printf("R%d_%d/(%s%s)=R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",m_r->size(),ID1,str2.c_str(),str1.c_str(),size(),ID,str.c_str());	
	string I1=calcI1(this);
	string I2=calcI2(this);   
	printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());	
	//printRing(this);	
}

int PolynomialRing::add(int a,int b)
{
    Polynomial C=add(m_Set[a],m_Set[b],m_r->size());
	int c=-1;
	vector<Polynomial>::iterator p=std::find(m_Set.begin(),m_Set.end(),C);
	if(p!=m_Set.end()){
		c=p-m_Set.begin();
	}	
	return c;
}

int PolynomialRing::mul(int a,int b)
{
    Polynomial C1=mul(m_Set[a],m_Set[b],m_r->size());
    Polynomial C=mod(C1,m_a,m_r->size());
	int c=-1;
	vector<Polynomial>::iterator p=std::find(m_Set.begin(),m_Set.end(),C);
	if(p!=m_Set.end()){
		c=p-m_Set.begin();
	}	
	return c;
}

int PolynomialRing::size()
{
	return m_Set.size();
}

vector<int> PolynomialRing::FactorsInt(int n )
{  
	vector<int> a;  
	for( int i = 2 , sign ; i < n / 2 + 1 ; i++ )
	{   
		sign=0;   
		if( n % i == 0 )
		{    
			if( a.empty() )
			{     
				a.push_back( i );    
				continue;    
			}    
			for( unsigned int j = 0 ; j < a.size() ; j++ )
			{    
				if( i % a[j] == 0 )
				{      
					sign=1;     
					break;     
				}   
			}    
			if( sign == 0 ) 
				a.push_back( i );  
		}  
	}
	/*�Խ�n����Ψһ�ֽ⣬����洢������a*/  
	if( a.empty() ) 
		a.push_back( n );  
	return a;
}

Polynomial PolynomialRing::gcd(Polynomial& a,Polynomial& b,int ord)
{  
	Polynomial pa = a;
	Polynomial pb = b;
	Polynomial pc;  
	while(!is_zero(pb))
	{   
		pc = mod(pa,pb,ord);
		pa = pb;
		pb = pc;
	}  
	return pa; 
}

Polynomial PolynomialRing::pow(Polynomial& a,unsigned int b,int ord)
{
	unsigned int  sum =ord;/*sum���ڼ���p^n-1*/  
	vector<short> vBin;/*vBin���ڴ洢p^n-1�Ķ�����λ*/  
	Polynomial Q1;
	Q1.push_back(1);  
	Polynomial Qx;
	Qx.push_back(0);
	Qx.push_back(1);
	Polynomial qm = Q1;  
	for( unsigned int i = b ; i > 1 ; i-- ) 
		sum *= ord;  
	sum -= 1;
	for( ; sum != 0 ; )
	{   
		vBin.push_back( sum % 2 );   
		sum /= 2;  
	}
	/*��p^n-1��Ϊ�����Ʊ�ʾ����������vBin�������±�С��Ԫ�ش洢�����Ƶĵ�λ*/ 
	for( int i = vBin.size()-1 ; i >= 0 ; i-- )
	{ 
		qm=mod(mul(qm,qm,ord),a,ord);   
		if( vBin[i] )
		{    
			qm=mod(mul(qm,Qx,ord),a,ord);  
		}   
		if( is_zero(qm)) break; 
	}/*�˴��õ�����ָ���㷨*/  
	return qm;
}

bool PolynomialRing::is_irr(Polynomial& a,int ord)
{  
	Polynomial p=a;   
	unsigned int pOrd = a.size()-1;/*����ʽ��ߴ������*/  
	if(pOrd==1) 
		return true;  
	Polynomial Q1;
	Q1.push_back(1);
	//step 1�ж�f��x���Ƿ�����n������Ϊ�����Ĳ���Լ����ʽ֮��------------  
	if(!IsEqual(pow(p,pOrd,ord),Q1)) 
		return false;   //step 1����---------------------------------------------------------  
	//step 2�ж�f��x���Ƿ���һ����ʽ-------------------------------------  
	for(int i = 0,n,sum ; i < ord; i++ )
	{   
		n = i ;
		sum = a[0] ;   
		for(unsigned int j = 1 ; j<a.size() ; j++ )
		{    
			sum = (( sum + a[j] * n ) % ord + ord ) % ord;    
			n = n * i % ord;   
		}   
		if(sum == 0)
			return false;  
	}  
	//step 2����--------------------------------------------------------- 

	//step 3�ж�f��x�����Ƿ��д���С��n�Ĳ���Լ��ʽ--------------------  
	vector<int> vFac = FactorsInt( a.size()-1 );  
	for( unsigned int i = 0 ; i < vFac.size() ; i++ )
	{   
		if( !IsEqual(gcd(p,pow(p,(a.size()-1)/vFac[i],ord),ord),Q1))  
			return false;
	}  
	//step 3����---------------------------------------------------------  
	return true; 
} 

/*�ú����жԿ���ָ���㷨������һ���ĸĽ������û��ֱ�ӵ��ÿ���ָ���㷨��ֱ����д�˺���*/
bool PolynomialRing::is_pri(Polynomial& a,int ord)
{  
	Polynomial p=a;   
	unsigned int  sum = ord;/*sum���ڼ���p^n-1*/ 
	vector<short> vBin;/*vBin���ڴ洢(p^n-1)/t�Ķ�����λ*/  
	Polynomial Q1;
	Q1.push_back(1);
	Polynomial Qx;
	Qx.push_back(0);
	Qx.push_back(1);
	Polynomial qm = Q1; 
	for( unsigned int i = a.size()-1 ; i > 1 ; i-- ) 
		sum *= ord;  
	sum -= 1; 
	for( unsigned int i = 2 , j ; i < sum / 2 + 1 ; i++ )
	{   
		if( sum % i == 0 )
		{    
			j = sum / i;    
			for( ; j != 0 ; )
			{     
				vBin.push_back( j % 2 );     
				j /= 2;    
			}
			/*��(p^n-1)/t��Ϊ�����Ʊ�ʾ����������vBin�������±�С��Ԫ�ش洢�����Ƶĵ�λ*/    
			for( int k = vBin.size()-1 ; k >= 0 ; k-- )
			{     
				qm=mod(mul(qm,qm,ord),p,ord); 
				if( vBin[k] )
				{      
					qm=mod(mul(qm,Qx,ord),p,ord);     
				}   
			}    
			if( IsEqual(qm,Q1)) 
				return false;    
			vBin.clear();   
		}  
	}  
	return true; 
}

bool PolynomialRing::IsEqual(Polynomial& a,Polynomial& b)
{
    int na=a.size();
	int nb=b.size();
	if(na!=nb)
		return false;
	for(int i=0;i<na;i++)
		if(a[i]!=b[i])
			return false;
	return true;
}

bool PolynomialRing::is_zero(Polynomial& a)
{
	if(a.size()==0||(a.size()==1&&a[0]==0))
		return true;
	return false;
}

Polynomial PolynomialRing::add(Polynomial& a,Polynomial& b,int ord)
{  
	int m=a.size();
	int n=b.size();
	int k=(m>n?m:n);
	int ki=(m>n?n:m);
	vector<int> c(k);
	for(int i=0;i<ki;i++)
	{
		c[i]=(a[i]+b[i])%ord;
	}
	if(m>n)
	{
		memcpy(&c[n],&a[n],(m-n)*sizeof(int));
	}
	else if(m<n)
	{
		memcpy(&c[m],&b[m],(n-m)*sizeof(int));
	}
	while (c.size() && *(c.rbegin())==0) {// �����λ��Ч��
		c.pop_back();
	}
	if (c.empty()) {// ����
		c.push_back(0);
	}
	return c;
}

Polynomial PolynomialRing::mul(Polynomial& a,Polynomial& b,int ord)
{  
	vector<int> ma = a;  
	vector<int> mb = b;  
	vector<int> mc;
	mc.assign(ma.size()+mb.size()- 1,0); 
	for(unsigned int i=0;i<ma.size();i++) 
		for(unsigned int j=0;j<mb.size();j++) 
			mc[i+j]=((mc[i+j]+ma[i]*mb[j])%ord+ord)%ord; 
	while(!mc.empty() && !mc.back()) 
		mc.pop_back();
	if (mc.empty()) {// ����
		mc.push_back(0);
	}	
	return mc;
}

Polynomial PolynomialRing::mod(Polynomial& a,Polynomial& b,int ord)
{  
	vector<int> m=a;/*������*/  
	vector<int> n=b;/*����*/ 
	int coe;/*��ϵ��*/
	if( m.size() < n.size() ) 
		return a;
	else for( unsigned int i = 0 ; i < m.size() - n.size() + 1 ; i++ )
	{   
		for( int ma = m[m.size()-1-i] ; ; )
		{    
			if( !( ma % n[n.size()-1] ))
			{     
				coe = ma / n[n.size()-1];     
				break;    
			}    
			ma += ord;   
		}   
		for( unsigned int j=0;j<n.size();j++) 
			m[m.size()-1-i-j] = ( m[m.size()-1-i-j] - coe*n[n.size()-1-j] % ord + ord ) % ord;  
	}  
	while( !m.empty() && !m.back() ) 
		m.pop_back(); 
	if (m.empty()) {// ����
		m.push_back(0);
	}	
	return m; 
}

int g_i=0;

void checkring(IRing *r,int ID){
	if(r->size()==32||r->size()==64||r->size()==243){
		if(ID>0){
			char sz1[128]={0};   
			sprintf(sz1,"R%d_%d.txt",r->size(),ID);
			writeTable(r,sz1);  
		}else{
			string strR=calcRingInvariant(r);			
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",r->size(),ID,strR.c_str());	
		}			
	}
	if((r->size()==27||r->size()==81) && ID==-1){	
			string strR=calcRingInvariant(r);			
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",r->size(),ID,strR.c_str());	
	}	
}

void findsubring(PolynomialRing *r,int n){
#define PRINT_LOG 0	
	bool bFind=false;	
#if PRINT_LOG
    char sz[100]="0";
	sprintf(sz,"R%d_%d.txt",r->size(),time(NULL));
    ofstream fout(sz);
    string strCmd="del ";
	strCmd+=sz;	
#endif	
	map<pair<int,int>,pair<int,int>> M;
	int ID=(r->size()>32 && r->size()!=81)?0:IdRing(r);
	printf("R%d_%d g_i=%d\n",r->size(),ID,g_i);	
	checkring(r,ID);
	for(int i=g_i;i<r->size()-1;i++)
	for(int j=i+1;j<r->size();j++)
	{
		//int j=i+1;
		vector<int> v;
		v.push_back(i);
		v.push_back(j);		
		Subring S1i;
		bool bn=S1i.init(r,v,n);
		if(!bn)
			continue;
		//Subring S1i(r,v);
		int ni=S1i.size();
		if(ni>n)
			continue;
		bool b=IsRing(&S1i);
		if(!b){
			continue;
		}		
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){
			string str=PolynomialRing::sPoly(r->m_Set[i]);
			string strj=PolynomialRing::sPoly(r->m_Set[j]);			
			printf("cnt1=%d:R%d_%d->i=%d,j=%d=>%s,%s\n",cnt1,ni,ID,i,j,str.c_str(),strj.c_str());
			//string I1=calcI1(&S1i);
			//string I2=calcI2(&S1i);   
			//printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());		
			static int cnt=sizeof(IDs)/sizeof(IDs[0]);
		    static vector<int> vIDs(IDs,IDs+cnt);
			vector<int>::iterator p1=std::find(vIDs.begin(),vIDs.end(),ID);
			if(ni==16 && std::find(vIDs.begin(),vIDs.end(),ID)!=vIDs.end()){
				printRing0(&S1i,ID);
			}		
            if((ni==32||ni==81) && ID>0){
				char sz1[128]={0};   
				sprintf(sz1,"R%d_%d.txt",ni,ID);
				writeTable(&S1i,sz1);                  
			}			
		}
		if(ni==n && (ID==-1))  
		{
			string str=PolynomialRing::sPoly(r->m_Set[i]);
			printf("%d->%s=>",i,str.c_str());
			string strj=PolynomialRing::sPoly(r->m_Set[j]);
			printf("%d->%s=>",j,strj.c_str());			
			string strR=calcRingInvariant(&S1i);
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",ni,ID,strR.c_str());				
			//S1i.printTable();
#if PRINT_LOG			
			fout<<i<<"->"<<str<<","<<j<<"->"<<strj<<"=>";
			fout<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2N6="<<strR<<endl;
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
		printf("�ӻ���ʾ��������ļ�%s\n",sz);
#endif		
}

void findsubring3(PolynomialRing *r,int n){
#define PRINT_LOG 0	
	bool bFind=false;	
#if PRINT_LOG
    char sz[100]="0";
	sprintf(sz,"R%d_%d.txt",r->size(),time(NULL));
    ofstream fout(sz);
    string strCmd="del ";
	strCmd+=sz;	
#endif	
	map<pair<int,int>,pair<int,int>> M;
	int ID=(r->size()>32 && r->size()!=81)?0:IdRing(r);
	printf("R%d_%d g_i=%d\n",r->size(),ID,g_i);	
	checkring(r,ID);	
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
		bool bn=S1i.init(r,v,n);
		if(!bn)
			continue;
		//Subring S1i(r,v);
		int ni=S1i.size();
		if(ni>n)
			continue;	
		bool b=IsRing(&S1i);
		if(!b){
			continue;
		}		
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){
			string str=PolynomialRing::sPoly(r->m_Set[i]);
			string strj=PolynomialRing::sPoly(r->m_Set[j]);	
			string strk=PolynomialRing::sPoly(r->m_Set[k]);			
			printf("cnt1=%d:R%d_%d->i=%d,j=%d,k=%d=>%s,%s,%s\n",cnt1,ni,ID,i,j,k,str.c_str(),strj.c_str(),strk.c_str());
			static int cnt=sizeof(IDs)/sizeof(IDs[0]);
		    static vector<int> vIDs(IDs,IDs+cnt);
			vector<int>::iterator p1=std::find(vIDs.begin(),vIDs.end(),ID);
			if(ni==16 && std::find(vIDs.begin(),vIDs.end(),ID)!=vIDs.end()){
				printRing0(&S1i,ID);
			}	
            if((ni==32||ni==81) && ID>0){
				char sz1[128]={0};   
				sprintf(sz1,"R%d_%d.txt",ni,ID);
				writeTable(&S1i,sz1);                  
			}			
		}
		if(ni==n && (ID==-1))   
		{
			string str=PolynomialRing::sPoly(r->m_Set[i]);
			printf("%d->%s=>",i,str.c_str());
			string strj=PolynomialRing::sPoly(r->m_Set[j]);
			printf("%d->%s=>",j,strj.c_str());	
			string strk=PolynomialRing::sPoly(r->m_Set[k]);
			printf("%d->%s=>",k,strk.c_str());				
			string strR=calcRingInvariant(&S1i);
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",ni,ID,strR.c_str());				
			//S1i.printTable();
#if PRINT_LOG			
			fout<<i<<"->"<<str<<","<<j<<"->"<<strj<<","<<k<<"->"<<strk<<"=>";
			fout<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2N6="<<strR<<endl;
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
		printf("�ӻ���ʾ��������ļ�%s\n",sz);
#endif		
}

void findquotientring(PolynomialRing *r,int n)
{
#define PRINT_LOG 0	
	bool bFind=false;	
	int ID=(r->size()>32 && r->size()!=81 && r->size()!=64 && r->size()!=243)?0:IdRing(r);
	checkring(r,ID);	
#if PRINT_LOG
    char sz[100]="0";
	sprintf(sz,"R%d_%d_%d.txt",r->size(),ID,time(NULL));
    ofstream fout(sz);
    string strCmd="del ";
	strCmd+=sz;	
#endif	
	map<pair<int,int>,pair<int,int>> M;	
	for(int i=0;i<r->size()-1;i++)		
    //int i=0;
	for(int j=i+1;j<r->size();j++)
	{
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);
		Subring S1i0;
		bool bn=S1i0.init(r,v,r->size()/n);
		if(!bn)
			continue;
		if(S1i0.m_Set.size()!=r->size()/n)
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
            int IDr=IdRing(r);
			int IDr0=IdRing(&S1i0);
			printf("cnt1=%d:R%d_%d/R%d_%d=R%d_%d->i=%d,j=%d\n",cnt1,r->size(),IDr,S1i0.size(),IDr0,ni,ID,i,j);
			static int cnt=sizeof(IDs)/sizeof(IDs[0]);
		    static vector<int> vIDs(IDs,IDs+cnt);
			vector<int>::iterator p1=std::find(vIDs.begin(),vIDs.end(),ID);
			if(ni==16 && std::find(vIDs.begin(),vIDs.end(),ID)!=vIDs.end()){
				printRing0(&S1i,ID);
			}
            if((ni==32||ni==81) && ID>0){
				char sz1[128]={0};   
				sprintf(sz1,"R%d_%d.txt",ni,ID);
				writeTable(&S1i,sz1);                  
			}			
		}	
		if(ni==n && ID==-1)	
		{		
			string strR=calcRingInvariant(&S1i);
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",ni,ID,strR.c_str());				
			//S1i.printTable();
#if PRINT_LOG			
			fout<<i<<","<<j<<"=>";
			fout<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2N6="<<strR<<endl;
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
		printf("�̻���ʾ��������ļ�%s\n",sz);
#endif	
}

int testRingDataA(int argc, char* argv[]){ 
	if(argc<3){
		printf("usage:PolynomialRing n ID [g_i] [n0]\n");
		return 0;
	}	
	int n=atoi(argv[1]);
	int ID=atoi(argv[2]);
	const CRingDataAItem * pItem = Find(n,ID);
	if(!pItem){
		printf("û������R%d_%d�ı�ʾ���ݣ�\n",n,ID);
		return 0;
	}
	printf("m_vfvm=%s\n",pItem->m_vfvm.c_str());
	PolynomialRing *r=new PolynomialRing;
	bool b=r->init(n,ID);	
	if(b && r){
		int in=r->size();
		int iID=IdRing(r);
		printf("R%d_%d",in,iID);	
		if(in<=32 && iID<=0){
			string strR=calcRingInvariant(r);
			printf(":N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s",strR.c_str());
			string I1=calcI1(r);
			string I2=calcI2(r);			
			printf("\nI1I2=%s,%s",I1.c_str(),I2.c_str());				
		}
		if(in>=32 && in<=256){
			int fun=0;
			if(argc>3){
				fun=atoi(argv[3]);
				if(fun<0||fun>2){
					fun=0;
				}	
			}		
			if(argc>4){
				g_i=atoi(argv[4]);	
			}
			int n0=16;
			if(argc>5){
				int _n0=atoi(argv[5]);	
				if(_n0==27)
					n0=27;
				else if(_n0==81)
					n0=81;				
				else
					n0=32;		
			}					
			typedef void(*pF)(PolynomialRing *r,int n);
			pF Func[]={findsubring,findsubring3,findquotientring};
			Func[fun](r,n0);			
		}		
		printf("\n");	
/* 		for(int i=0;i<in;i++){		
			string stri=PolynomialRing::sPoly(r->m_Set[i]);		
			printf("i=%d=>%s\n",i,stri.c_str());	
		} */		
	}
	delete r;
	r=NULL;
	return 0;
}	

int main(int argc, char* argv[])
{ 
    // ����������ʾ�������õ��ļ��У��������
	int ret=LoadData("RingDataA.csv");
	printf("ret=%d,��������ʾ���ݱ��еļ�¼����=%d\n",ret,g_mapRingDataACache.size());
	return testRingDataA(argc,argv);

	return 0;
}
