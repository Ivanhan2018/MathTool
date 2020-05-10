#include"IRing.h"
#include<iostream>
#include<complex>
#include<vector>
#include<algorithm>
using namespace std;

typedef complex<int> GaussianInteger;

unsigned int Primes[]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,
	107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,
	337,347,349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,547,557,563,569,571,577,587,
	593,599,601,607,613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,769,773,787,797,809,811,821,823,827,829,839,853,
	857,859,863,877,881,883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997};

// �������ͷ�����ֵ��С��������˳��
bool lessGaussianInteger(const GaussianInteger &m,const GaussianInteger &n) 
{
	int norm1=m.real()*m.real()+m.imag()*m.imag();
	int norm2=n.real()*n.real()+n.imag()*n.imag();
	float arg1=atan2f(m.imag(),m.real());
	float arg2=atan2f(n.imag(),n.real());
	float pi=atan2f(0,-1);
	if(arg1<0)
		arg1+=2*pi;
	if(arg2<0)
		arg2+=2*pi;
	if(norm1!=norm2)
		return norm1<norm2;
	else
        return arg1<arg2;
}

string Itos(int a)
{
	char sz[20]={0};
	sprintf(sz,"%d",a);
	return sz;
}

string gStr(GaussianInteger& a)
{
    string str;
	if(a.real()!=0 && a.imag()>0)
		str=Itos(a.real())+"+"+Itos(a.imag())+"i";
	if(a.real()!=0 && a.imag()<0)
		str=Itos(a.real())+Itos(a.imag())+"i";
	if(a.imag()==0)
		str=Itos(a.real());
	if(a.imag()==0 && a.imag()!=0)
		str=Itos(a.imag())+"i";
	return str;
}

// ���޽�����Z[i]/(a)��mZ[i]/nZ[i]
struct ZimodnZ:public IRing
{
public:
	// ��̬����
	static int gcd(int a, int b);
	static GaussianInteger Div(const GaussianInteger &a,const GaussianInteger &b);
	static bool divide(const GaussianInteger& a, const GaussianInteger& b,GaussianInteger& q, GaussianInteger& r);
	static GaussianInteger gMod(const GaussianInteger &a,const GaussianInteger &b);
	static int Norm(const GaussianInteger& a);
	static GaussianInteger gcd(const GaussianInteger& a, const GaussianInteger& b);
	static GaussianInteger simple_gcd(const GaussianInteger& a, const GaussianInteger& b);
	static GaussianInteger extended_gcd(const GaussianInteger& a, const GaussianInteger& b,GaussianInteger& x, GaussianInteger& y);
	static bool IsPrime(unsigned int N);
	static bool IsPrime(const GaussianInteger& a);
	static vector<GaussianInteger> FRZi(const GaussianInteger &a,const vector<GaussianInteger> &v);
public:
	// ʵ�ֳ������ķ���
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// ���캯��
	ZimodnZ(){};
	ZimodnZ(const GaussianInteger &a);
	// ��Ա����	
	void initmZinZi(const GaussianInteger &m,const GaussianInteger &n);
	// ��Ա����
	vector<GaussianInteger> m_Set;
	GaussianInteger m_a;// ����Ϊģa�ӷ���ģa�˷�
};

ZimodnZ::ZimodnZ(const GaussianInteger &a)
{
	m_a=a;
	int num=Norm(a);
	int nb=abs(gcd(a.real(),a.imag()));
	int na=num/nb;
	for(int i=0;i<na;i++)
	{
		for(int j=0;j<nb;j++)
		{
            GaussianInteger ij=gMod(GaussianInteger(i,j),a);
			m_Set.push_back(ij);
		}
	}
}

vector<GaussianInteger> ZimodnZ::FRZi(const GaussianInteger &a,const vector<GaussianInteger> &v)
{
	vector<GaussianInteger> S;	
	S.push_back(GaussianInteger(0,0));
	for(int i=0;i<v.size();i++)
	{
	    GaussianInteger r1=gMod(v[i],a);
		S.push_back(r1);
	}  		
	int cnt=0;
	int cnt1=S.size();
	while(cnt1>cnt){
		cnt=S.size();
		for(int i=0;i<S.size();i++){
			for(int j=0;j<S.size();j++){
				GaussianInteger ij=gMod(S[i]*S[j],a);
				vector<GaussianInteger>::iterator p=std::find(S.begin(),S.end(),ij);
				if(p==S.end()){
					S.push_back(ij);
				}
				GaussianInteger ij1=gMod(S[i]+S[j],a);
				vector<GaussianInteger>::iterator p1=std::find(S.begin(),S.end(),ij1);
				if(p1==S.end()){
					S.push_back(ij);
				}				
			}
		}		
		cnt1=S.size();
	}
    return S;
}

void ZimodnZ::initmZinZi(const GaussianInteger &m,const GaussianInteger &n)
{
	m_a=n;
	vector<GaussianInteger> v;
	v.push_back(m);
	m_Set=FRZi(n,v);
}

void ZimodnZ::printTable()
{
	int ID=IdRing(this);
	string str=calcRingInvariant(this);
	printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",size(),ID,str.c_str());	
	//string I1=calcI1(this);
	//string I2=calcI2(this);   
	//printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());
	//printRing(this);	
}

int ZimodnZ::add(int a,int b)
{
	if(a<0||b<0)
		return -1;
	GaussianInteger C=gMod(GaussianInteger(m_Set[a])+GaussianInteger(m_Set[b]),m_a);
	vector<GaussianInteger>::iterator p=std::find(m_Set.begin(),m_Set.end(),C);
	int c=-1;
	if(p!=m_Set.end()){
		c=p-m_Set.begin();
	}
	return c;
}

int ZimodnZ::mul(int a,int b)
{
	if(a<0||b<0)
		return -1;
	GaussianInteger C=gMod(GaussianInteger(m_Set[a])*GaussianInteger(m_Set[b]),m_a);
	vector<GaussianInteger>::iterator p=std::find(m_Set.begin(),m_Set.end(),C);
	int c=-1;
	if(p!=m_Set.end()){
		c=p-m_Set.begin();
	}
	return c;
}

int ZimodnZ::size()
{
	return m_Set.size();
}

// ����ŷ������㷨���������������Լ��
int ZimodnZ::gcd(int a, int b)
{
	if(b == 0) return a;
	if(a == 0) return b;
	if(a%b == 0) return b;
	else return gcd(b, a%b);
}

GaussianInteger ZimodnZ::Div(const GaussianInteger &a,const GaussianInteger &b)
{
	std::complex<float> af(a.real(),a.imag());
	std::complex<float> bf(b.real(),b.imag());
	std::complex<float> cf=af/bf;
	// int cr=cf.real()>0?(int)(cf.real()+0.5):(int)(cf.real()-0.5);
	// int ci=cf.imag()>0?(int)(cf.imag()+0.5):(int)(cf.imag()-0.5);
	int cr=floor(cf.real()+0.5);
	int ci=floor(cf.imag()+0.5); 
	GaussianInteger c(cr,ci);
	return c;
}

bool ZimodnZ::divide(const GaussianInteger& a, const GaussianInteger& b,GaussianInteger& q, GaussianInteger& r)
{
  q=Div(a,b);
  r = a - q*b;
  bool bret=(r==GaussianInteger(0,0));
  return bret;
}

GaussianInteger ZimodnZ::gMod(const GaussianInteger &a,const GaussianInteger &b)
{
  GaussianInteger q=Div(a,b);
  GaussianInteger r=a-q*b;
  return r;
}

int ZimodnZ::Norm(const GaussianInteger& a)
{
 return (a.real()*a.real()+a.imag()*a.imag());
}

GaussianInteger ZimodnZ::gcd(const GaussianInteger& a, const GaussianInteger& b)
{
 GaussianInteger x = a, y = b;
 if(Norm(x)<Norm(y) )
 {
  std::swap(x,y);
 }
 while ( y != GaussianInteger(0,0) ) {
  GaussianInteger q,r;
  bool ret=divide(x,y,q,r);
  x = y;
  y = r;
 }
 return x;
}

GaussianInteger ZimodnZ::simple_gcd(const GaussianInteger& a, const GaussianInteger& b)
{
  GaussianInteger aa = a, bb = b;
  if(Norm(aa)<Norm(bb) )
  {
      std::swap(aa,bb);
   }
  while(bb != GaussianInteger(0,0))
   {
      GaussianInteger qq, rr;
      bool bret=divide (aa, bb, qq, rr);
      aa = bb;
      bb = rr;
   }
  return aa;
}

GaussianInteger ZimodnZ::extended_gcd(const GaussianInteger& a, const GaussianInteger& b,GaussianInteger& x, GaussianInteger& y)
{
 GaussianInteger aa = a, bb = b;
 bool swapped = false;
 if(Norm(aa)<Norm(bb) )
 {
  std::swap(aa,bb);
  swapped = true;
 }
 GaussianInteger xx = 0, lx = 1, yy = 1, ly = 0;
 do
 {
  GaussianInteger qq, rr;
  bool bret=divide (aa, bb, qq, rr);
  aa = bb; bb = rr;
  GaussianInteger tx = lx - qq*xx;
  lx = xx; xx = tx;
  GaussianInteger ty = ly - qq*yy;
  ly = yy; yy = ty;
 }while (bb != GaussianInteger(0,0));
 x = lx;
 y = ly;
 if (swapped)
 {
  std::swap(x,y);
 }
 return aa;
}

bool ZimodnZ::IsPrime(unsigned int N)
{
	if(N==0||N==1)
		return false;
	int up=sqrt((float)N);
	for(int i=2;i<=up;i++)
	{
		if(N%i==0)
			return false;
	}
	return true;
}

/*
	��˹����a+bi���������ҽ�����
	1)a��b����һ�����㣬��һ�����ľ���ֵ������4n+3��������
	2)a��b����Ϊ�㣬��a^2+b^2Ϊ������
*/
bool ZimodnZ::IsPrime(const GaussianInteger& a)
{
	int r=abs(a.real());
	int i=abs(a.imag());		
	if(r==0 && (i+1)%4==0)
		return true;
	if(i==0 && (r+1)%4==0)
		return true;
	if(r*i>0 && IsPrime(Norm(a)))
		return true;
	return false;
}

int main(void)
{
	//GaussianInteger v[]={GaussianInteger(0,-1),GaussianInteger(3,1)};
	//int nv=sizeof(v)/sizeof(v[0]);
	//for(int i=0;i<nv;i++)
	//{
	//	ZimodnZ r(v[i]);
	//	r.printTable();
	//}
	//system("pause");
	//return 0;
	if(0)
	{
		ZimodnZ r32(GaussianInteger(4,4));
		int ID=IdRing(&r32);
		string str=calcRingInvariant(&r32);
		printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",r32.size(),ID,str.c_str());	
		string S1=calS1(&r32,true);
		string undoS1=calS1(&r32,false);	
		printf("R%d_%d:S1=%s,S1(δ������)=%s\n",r32.size(),ID,S1.c_str(),undoS1.c_str());
		string I1=calcI1(&r32);
		string I2=calcI2(&r32);   
		printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());
	}
	if(1)
	{
		ZimodnZ r32(GaussianInteger(8,8));
		int ID=IdRing(&r32);
		string str=calcRingInvariant(&r32);
		printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",r32.size(),ID,str.c_str());	
		string S1=calS1(&r32,true);
		string undoS1=calS1(&r32,false);	
		printf("R%d_%d:S1=%s,S1(δ������)=%s\n",r32.size(),ID,S1.c_str(),undoS1.c_str());
		string I1=calcI1(&r32);
		string I2=calcI2(&r32);   
		printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());
	}
	system("pause");
	return 0;

	int Norm=25;
	int n=sqrt((float)Norm);
	vector<GaussianInteger> vgint;
	for(int i=-n;i<=n;i++)
	for(int j=-n;j<=n;j++)
	{
		GaussianInteger a(i,j);  
		vgint.push_back(a);
	}
	cout<<"����������"<<Norm<<"�ĸ�˹��������"<<vgint.size()<<"����"<<endl;
	sort(vgint.begin(), vgint.end(),lessGaussianInteger);
	for(int j=0;j<vgint.size();j++)
	{
		string strDes=ZimodnZ::IsPrime(vgint[j])?"�Ǹ�˹����":"���Ǹ�˹����";
		int n=ZimodnZ::Norm(vgint[j]);
		cout<<"��"<<j+1<<"����˹����"<<vgint[j]<<"="<<gStr(vgint[j])<<"����Ϊ"<<n<<strDes<<endl;
		if(n>0 && n!=10 && n!=26 && n!=34){
			ZimodnZ r(vgint[j]);
			r.printTable();
		}
	}
	system("pause");
	return 0;
}