#ifndef POLYNOMIALRING_H
#define POLYNOMIALRING_H

#include"ZmodnZ.h"

typedef vector<int> Polynomial;// 用向量表示多项式

// r[x]/(a)、(m)r[x]/(n)r[x]
struct PolynomialRing:public IRing
{
public:
	// 静态函数
	static bool IsEqual(Polynomial& a,Polynomial& b);// 是否相等 
	static Polynomial add(Polynomial& a,Polynomial& b,int ord);// 加法	
	static Polynomial mul(Polynomial& a,Polynomial& b,int ord);// 乘法
	static Polynomial mod(Polynomial& a,Polynomial& b,int ord);// 取余	
	static bool is_zero(Polynomial& a);// 是否为零多项式	
	static Polynomial gcd(Polynomial& a,Polynomial& b,int ord);
	static bool nextV1(int m,vector<int>& v);
	static string Itos(int Num);
	static string sPoly(Polynomial& a);
	static vector<Polynomial> FR(Polynomial &a,vector<Polynomial> &v,int ord);
public:
	// 实现抽象基类的方法
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// 构造函数
	PolynomialRing();
	PolynomialRing(IRing *r,Polynomial& a);
	// 析构函数
	~PolynomialRing();
	// 成员函数	
	void initFR(IRing *r,Polynomial &m,Polynomial &n);
	void initFR(IRing *r,Polynomial &m1,Polynomial &m2,Polynomial &n);	
	int visitVnRm(int n,int m);
	// 成员变量
	vector<Polynomial> m_Set;
	Polynomial m_a;// 运算为模a加法和模a乘法
	vector<Polynomial> m_gen;
    IRing* m_r;
	int m_flag;// M2r对象析构时是否释放m_r指向的内存
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

// 将一个数字转换成字符串
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

// 调用m^n次visit
int PolynomialRing::visitVnRm(int n,int m){
	vector<int> v(n,0);
	int cnt=0;
	do {
		++cnt;
		Polynomial p=v;
		while( !p.empty() && !p.back() ) 
			p.pop_back(); 
		if (p.empty()) {// 补零
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
	printf("R%d_%d/(%s%s)=R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",m_r->size(),ID1,str2.c_str(),str1.c_str(),size(),ID,str.c_str());	
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

PolynomialRing::PolynomialRing(IRing *r,Polynomial& a)
{
	m_a=a;
	m_r=r;
	int N=r->size();
	unsigned int pOrd = a.size()-1;/*多项式最高次项次数*/	
	int cnt=visitVnRm(pOrd,N);
	printf("cnt=%d\n",cnt);
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
	while (c.size() && *(c.rbegin())==0) {// 清除高位无效零
		c.pop_back();
	}
	if (c.empty()) {// 补零
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
	if (mc.empty()) {// 补零
		mc.push_back(0);
	}	
	return mc;
}

Polynomial PolynomialRing::mod(Polynomial& a,Polynomial& b,int ord)
{  
	vector<int> m=a;/*被除数*/  
	vector<int> n=b;/*除数*/ 
	int coe;/*商系数*/
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
	if (m.empty()) {// 补零
		m.push_back(0);
	}	
	return m; 
}

#endif