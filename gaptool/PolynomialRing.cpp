#include"ZmodnZ.h"
#include<vector> 
#include<iostream>
#include<fstream> 
#include<string> 
using namespace std;

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
	PolynomialRing(){};
	PolynomialRing(IRing *r,Polynomial& a);
	// ��Ա����	
	void initFR(IRing *r,Polynomial &m,Polynomial &n);
	void initFR(IRing *r,Polynomial &m1,Polynomial &m2,Polynomial &n);	
	int visitVnRm(int n,int m);
	// ��Ա����
	vector<Polynomial> m_Set;
	Polynomial m_a;// ����Ϊģa�ӷ���ģa�˷�
	vector<Polynomial> m_gen;
    IRing* m_r;	
};

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
	unsigned int pOrd = a.size()-1;/*����ʽ��ߴ������*/	
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

void test1(){
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
	int f6[] = {1,1,0,1};	
	Polynomial vf6(f6,f6+sizeof(f6)/sizeof(f6[0]));	
	int f7[] = {0,1,0,1};
	Polynomial vf7(f7,f7+sizeof(f7)/sizeof(f7[0]));
	int f8[] = {0,0,1,1};
	Polynomial vf8(f8,f8+sizeof(f8)/sizeof(f8[0]));
	int f9[] = {1,0,1,1};
	Polynomial vf9(f9,f9+sizeof(f9)/sizeof(f9[0]));	
	int f10[] = {0,1,1,1};
	Polynomial vf10(f10,f10+sizeof(f10)/sizeof(f10[0]));
	int f11[] = {1,1,1,1};
	Polynomial vf11(f11,f11+sizeof(f11)/sizeof(f11[0]));			
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
	
	PolynomialRing r32;
	r32.initFR(&r4_3,vf1,vf5);
	r32.printTable();
	PolynomialRing r64(&r4_3,vf5);
	r64.printTable();
	PolynomialRing r64_2(&r4_3,vf6);
	r64_2.printTable();	
	PolynomialRing r64_3(&r4_3,vf7);
	r64_3.printTable();	
	PolynomialRing r64_4(&r4_3,vf8);
	r64_4.printTable();	
	PolynomialRing r64_5(&r4_3,vf11);
	r64_5.printTable();	
	int m1[] = {0,1,1};
	Polynomial vm1(m1,m1+sizeof(m1)/sizeof(m1[0]));	
	int m2[] = {2};
	Polynomial vm2(m2,m2+sizeof(m2)/sizeof(m2[0]));	
	PolynomialRing r8_11;
	r8_11.initFR(&r4_3,vm1,vm2,vf8);
	r8_11.printTable();		
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
}

int main(){
    ZmodnZ r2_2(1,2);	
    ZmodnZ r4_3(1,4);
    ZmodnZ r8_3(1,8);	
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
	int f6[] = {1,1,0,1};
	Polynomial vf6(f6,f6+sizeof(f6)/sizeof(f6[0]));
	int f7[] = {0,1,0,1};
	Polynomial vf7(f7,f7+sizeof(f7)/sizeof(f7[0]));	
	int f8[] = {0,0,1,1};
	Polynomial vf8(f8,f8+sizeof(f8)/sizeof(f8[0]));		
	int f9[] = {1,0,1,1};
	Polynomial vf9(f9,f9+sizeof(f9)/sizeof(f9[0]));
	int f10[] = {0,1,1,1};
	Polynomial vf10(f10,f10+sizeof(f10)/sizeof(f10[0]));	
	int f11[] = {1,1,1,1};
	Polynomial vf11(f11,f11+sizeof(f11)/sizeof(f11[0]));	
	int f12[] = {0,0,0,1};
	Polynomial vf12(f12,f12+sizeof(f12)/sizeof(f12[0]));	
	int f13[] = {2,1,1};
	Polynomial vf13(f13,f3+sizeof(f13)/sizeof(f13[0]));	
	int f14[] = {0,0,1};
	Polynomial vf14(f14,f14+sizeof(f14)/sizeof(f14[0]));		
	//PolynomialRing r64(&r8_3,vf1);
	//r64.printTable();	
	int g1[] = {0,2};
	int g2[] = {2};	
	int g3[] = {4,6};
	int g4[] = {4,7};
	int g5[] = {6,1};
	int g6[] = {6,3};	
	int g7[] = {1,1,1};
	int g8[] = {1,1,1,1,1};
	Polynomial vg1(g1,g1+sizeof(g1)/sizeof(g1[0]));
	Polynomial vg2(g2,g2+sizeof(g2)/sizeof(g2[0]));	
	Polynomial vg3(g3,g3+sizeof(g3)/sizeof(g3[0]));
	Polynomial vg4(g4,g4+sizeof(g4)/sizeof(g4[0]));	
	Polynomial vg5(g5,g5+sizeof(g5)/sizeof(g5[0]));
	Polynomial vg6(g6,g6+sizeof(g6)/sizeof(g6[0]));	
	Polynomial vg7(g7,g7+sizeof(g7)/sizeof(g7[0]));
	Polynomial vg8(g8,g8+sizeof(g8)/sizeof(g8[0]));		
	//PolynomialRing r16_118;
	//r16_118.initFR(&r8_3,vg1,vg2,vf1);
	//r16_118.printTable();	
	//PolynomialRing r64(&r8_3,vf2);
	//PolynomialRing r16_119;
	//r16_119.initFR(&r8_3,vg1,vg2,vf2);
	//r16_119.printTable();
	//PolynomialRing r16_16;
	//r16_16.initFR(&r8_3,vg3,vg4,vf3);
	//r16_16.printTable();
	//PolynomialRing r16_17;
	//r16_17.initFR(&r8_3,vg5,vg6,vf3);
	//r16_17.printTable();	
	//r16_119.printTable();	
	//PolynomialRing r64(&r8_3,vf3);
	//PolynomialRing r64(&r8_3,vf4);
	//r64.printTable();	
	//PolynomialRing r64(&r8_3,vf14);
	//r64.printTable();
	int f15[] = {0,0,1,1,1,1};
	Polynomial vf15(f15,f15+sizeof(f15)/sizeof(f15[0]));	
	int f16[] = {1,0,0,0,0,1,1};
	Polynomial vf16(f16,f16+sizeof(f16)/sizeof(f16[0]));
	int f17[] = {1,0,0,0,1,1,1};
	Polynomial vf17(f17,f17+sizeof(f17)/sizeof(f17[0]));
	int f18[] = {1,1,0,0,1,1,1};
	Polynomial vf18(f18,f18+sizeof(f18)/sizeof(f18[0]));	
	int f19[] = {1,1,0,0,0,1,1};
	Polynomial vf19(f19,f19+sizeof(f19)/sizeof(f19[0]));
	int f20[] = {1,1,1,0,0,1,1};
	Polynomial vf20(f20,f20+sizeof(f20)/sizeof(f20[0]));	
	PolynomialRing r64(&r2_2,vf20);
	
	//PolynomialRing r16_377;
	//r16_377.initFR(&r2_2,vg7,vg8,vf15);
	//r16_377.printTable();		
if(1){
	//PolynomialRing r64;
	// for(int i=0;i<r64.size();i++)
	// {
		// string str=PolynomialRing::sPoly(r64.m_Set[i]);
		// printf("%d->%s\n",i,str.c_str());	   
	// }
	string undoS1=calS1(&r64,false);	
	printf("S1(δ������)=%s\n",undoS1.c_str());	
	for(int i=0;i<r64.size()-1;i++)
	//for(int j=i+1;j<r64.size();j++)
	{
		int j=i+1;
		vector<int> v;
		v.push_back(i);
		v.push_back(j);		
		Subring S1i(&r64,v);
		int ni=S1i.size();
		int ID=IdRing(&S1i);
		if(ni==16 && ID==-1||(ni==8 && (ID==6||ID==36||ID==9||ID==12||ID==18||ID==31||ID==32||ID==39)))   
		{
			string str=PolynomialRing::sPoly(r64.m_Set[i]);
			printf("%d->%s=>",i,str.c_str());
			string strj=PolynomialRing::sPoly(r64.m_Set[j]);
			printf("%d->%s=>",j,strj.c_str());			
			string strR=calcRingInvariant(&S1i);
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",ni,ID,strR.c_str());				
			S1i.printTable();
			break;
		}		   
	}
}	
	return 0;
}
