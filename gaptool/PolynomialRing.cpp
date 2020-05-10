#include"ZmodnZ.h"
#include<vector> 
#include<iostream>
#include<fstream> 
#include<string> 
using namespace std;

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
	static bool is_irr(Polynomial& a,int ord);// 是否不可约，Error测试不通过 
	static bool is_pri(Polynomial& a,int ord);// 是否本原，Error测试不通过
	static Polynomial pow(Polynomial& a,unsigned int b,int ord);// 快速指数算法
	static Polynomial gcd(Polynomial& a,Polynomial& b,int ord);
	static vector<int> FactorsInt(int n);
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
	PolynomialRing(){};
	PolynomialRing(IRing *r,Polynomial& a);
	// 成员函数	
	void initFR(IRing *r,Polynomial &m,Polynomial &n);
	int visitVnRm(int n,int m);
	// 成员变量
	vector<Polynomial> m_Set;
	Polynomial m_a;// 运算为模a加法和模a乘法
    IRing* m_r;	
};

void PolynomialRing::initFR(IRing *r,Polynomial &m,Polynomial &n)
{
	m_r=r;
	m_a=n;
	vector<Polynomial> v;
	v.push_back(m);
	m_Set=FR(n,v,r->size());
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
	string str=calcRingInvariant(this);
	printf("R%d_%d/(%s)=R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",m_r->size(),ID1,str1.c_str(),size(),ID,str.c_str());	
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
	/*对阶n进行唯一分解，结果存储于向量a*/  
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
	unsigned int  sum =ord;/*sum用于计算p^n-1*/  
	vector<short> vBin;/*vBin用于存储p^n-1的二进制位*/  
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
	/*将p^n-1化为二进制表示，存入向量vBin，向量下标小的元素存储二进制的低位*/ 
	for( int i = vBin.size()-1 ; i >= 0 ; i-- )
	{ 
		qm=mod(mul(qm,qm,ord),a,ord);   
		if( vBin[i] )
		{    
			qm=mod(mul(qm,Qx,ord),a,ord);  
		}   
		if( is_zero(qm)) break; 
	}/*此处用到快速指数算法*/  
	return qm;
}

bool PolynomialRing::is_irr(Polynomial& a,int ord)
{  
	Polynomial p=a;   
	unsigned int pOrd = a.size()-1;/*多项式最高次项次数*/  
	if(pOrd==1) 
		return true;  
	Polynomial Q1;
	Q1.push_back(1);
	//step 1判断f（x）是否是以n的因子为次数的不可约多项式之积------------  
	if(!IsEqual(pow(p,pOrd,ord),Q1)) 
		return false;   //step 1结束---------------------------------------------------------  
	//step 2判断f（x）是否有一次因式-------------------------------------  
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
	//step 2结束--------------------------------------------------------- 

	//step 3判断f（x）中是否含有次数小于n的不可约因式--------------------  
	vector<int> vFac = FactorsInt( a.size()-1 );  
	for( unsigned int i = 0 ; i < vFac.size() ; i++ )
	{   
		if( !IsEqual(gcd(p,pow(p,(a.size()-1)/vFac[i],ord),ord),Q1))  
			return false;
	}  
	//step 3结束---------------------------------------------------------  
	return true; 
} 

/*该函数中对快速指数算法进行了一定的改进，因此没有直接调用快速指数算法而直接重写了函数*/
bool PolynomialRing::is_pri(Polynomial& a,int ord)
{  
	Polynomial p=a;   
	unsigned int  sum = ord;/*sum用于计算p^n-1*/ 
	vector<short> vBin;/*vBin用于存储(p^n-1)/t的二进制位*/  
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
			/*将(p^n-1)/t化为二进制表示，存入向量vBin，向量下标小的元素存储二进制的低位*/    
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

int main(){
	int f1[] = {1,0,1};
	Polynomial vf1(f1,f1+sizeof(f1)/sizeof(f1[0]));
	int f2[] = {1,1,1};
	Polynomial vf2(f2,f2+sizeof(f2)/sizeof(f2[0]));
	int f3[] = {0,0,1};
	Polynomial vf3(f3,f3+sizeof(f3)/sizeof(f3[0]));
	int f4[] = {0,1,1};
	Polynomial vf4(f4,f4+sizeof(f4)/sizeof(f4[0]));
	int f5[] = {1,0,0,1};
	Polynomial vf5(f5,f5+sizeof(f5)/sizeof(f5[0]));
    ZmodnZ r2_2(1,2);	
    ZmodnZ r3_2(1,3);
    ZmodnZ r4_3(1,4);
    ZmodnZ r8_3(1,8);
	PolynomialRing r4_9(&r2_2,vf1);
	r4_9.printTable();
	PolynomialRing r4_11(&r2_2,vf2);
	r4_11.printTable();
	PolynomialRing r4_9a(&r2_2,vf3);
	r4_9a.printTable();
	PolynomialRing r4_10(&r2_2,vf4);
	r4_10.printTable();
	PolynomialRing r8_51(&r2_2,vf5);
	r8_51.printTable();
	PolynomialRing r9_11(&r3_2,vf1);
	r9_11.printTable();
	PolynomialRing r9_9(&r3_2,vf2);
	r9_9.printTable();
	PolynomialRing r9_9a(&r3_2,vf3);
	r9_9a.printTable();
	PolynomialRing r9_10(&r3_2,vf4);
	r9_10.printTable();
	PolynomialRing r16_105(&r4_3,vf1);
	r16_105.printTable();
	PolynomialRing r16_106(&r4_3,vf2);
	r16_106.printTable();
	PolynomialRing r16_110(&r4_3,vf3);
	r16_110.printTable();
	PolynomialRing r16_104(&r4_3,vf4);
	r16_104.printTable();
	{
		PolynomialRing r64_5;
		r64_5.initFR(&r4_3,vf1,vf5);
		r64_5.printTable();
		PolynomialRing r64_1;
		r64_1.initFR(&r8_3,vf1,vf5);
		r64_1.printTable();
		PolynomialRing r64_2;
		r64_2.initFR(&r8_3,vf2,vf5);
		r64_2.printTable();
		PolynomialRing r64_3;
		r64_3.initFR(&r8_3,vf3,vf5);
		r64_3.printTable();
		PolynomialRing r64_4;
		r64_4.initFR(&r8_3,vf4,vf5);
		r64_4.printTable();
	}
	{
		PolynomialRing r64_5(&r4_3,vf5);
		r64_5.printTable();
		PolynomialRing r64_1(&r8_3,vf1);
		r64_1.printTable();
		PolynomialRing r64_2(&r8_3,vf2);
		r64_2.printTable();
		PolynomialRing r64_3(&r8_3,vf3);
		r64_3.printTable();
		PolynomialRing r64_4(&r8_3,vf4);
		r64_4.printTable();
	}
	return 0;
}
