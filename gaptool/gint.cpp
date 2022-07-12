#include"IRing.h"
#include<iostream>
#include<fstream>
#include<complex>
#include<set>
#include<vector>
#include<algorithm>
using namespace std;

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

typedef complex<int> GaussianInteger;

unsigned int Primes[]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,
	107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,
	337,347,349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,547,557,563,569,571,577,587,
	593,599,601,607,613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,769,773,787,797,809,811,821,823,827,829,839,853,
	857,859,863,877,881,883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997};

// 按范数和辐角主值从小到大排列顺序
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
	if(a.real()==0 && a.imag()!=0)
		str=Itos(a.imag())+"i";
	return str;
}

// 有限交换环Z[i]/(a)、mZ[i]/nZ[i]
struct ZimodnZ:public IRing
{
public:
	// 静态函数
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
	// 实现抽象基类的方法
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// 构造函数
	ZimodnZ(){};
	ZimodnZ(const GaussianInteger &a);
	// 成员函数	
	void initFR(const GaussianInteger &m,const GaussianInteger &n);
	void initFR(const GaussianInteger &m1,const GaussianInteger &m2,const GaussianInteger &n);		
	// 成员变量
	vector<GaussianInteger> m_Set;
	vector<GaussianInteger> m_gen;	
	GaussianInteger m_a;// 运算为模a加法和模a乘法
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
	S.clear();
	S.push_back(gMod(GaussianInteger(0,0),a));
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
					S.push_back(ij1);
				}				
			}
		}		
		cnt1=S.size();
	}
    return S;
}

void ZimodnZ::initFR(const GaussianInteger &m,const GaussianInteger &n)
{
	m_a=n;
	vector<GaussianInteger> v;
	v.push_back(m);
	m_Set=FRZi(n,v);
	m_gen=v;
}

void ZimodnZ::initFR(const GaussianInteger &m1,const GaussianInteger &m2,const GaussianInteger &n)
{
	m_a=n;
	vector<GaussianInteger> v;
	v.push_back(m1);
	v.push_back(m2);
	m_Set=FRZi(n,v);
	m_gen=v;
}

void ZimodnZ::printTable()
{
	int ID=IdRing(this);
	string str=calcRingInvariant(this);
	string str1=gStr(m_a);	
	string str2="";
	if(m_gen.size()>0){
		str2+="[";
		for(int i=0;i<m_gen.size();i++){
			str2+=gStr(m_gen[i]);
			if(i<m_gen.size()-1)
				str2+=",";
		}
		str2+="],";
	}	
	printf("Z[i]/(%s%s)=R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",str2.c_str(),str1.c_str(),size(),ID,str.c_str());	
	string I1=calcI1(this);
	string I2=calcI2(this);   
	printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());
	//printRing(this);	
}

int ZimodnZ::add(int a,int b)
{
	if(a<0||b<0)
		return -1;
	GaussianInteger C=gMod(m_Set[a]+m_Set[b],m_a);
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
	GaussianInteger C=gMod(m_Set[a]*m_Set[b],m_a);
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

// 利用欧几里得算法计算两个数的最大公约数
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
	高斯整数a+bi是素数当且仅当：
	1)a、b中有一个是零，另一个数的绝对值是形如4n+3的素数；
	2)a、b均不为零，而a^2+b^2为素数；
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

void findsubring2(ZimodnZ *r,int n){
	map<pair<int,int>,pair<int,int>> M;	
	set<string> S;	
	for(int i=0;i<r->size()-1;i++)
	for(int j=i+1;j<r->size();j++)
	{
		vector<int> v;
		v.push_back(i);
		v.push_back(j);	
		Subring S1i;		
		bool bn=S1i.init(r,v,n);
		if(!bn)
			continue;
		int ni=S1i.size();
		int ID=IdRing(&S1i);
		bool b=IsRing(&S1i);
		if(!b){
			continue;
		}
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){	
			string str=gStr(r->m_Set[i]);
			string strj=gStr(r->m_Set[j]);				
			printf("cnt1=%d:R%d_%d->i=%d,j=%d=>%s,%s\n",cnt1,ni,ID,i,j,str.c_str(),strj.c_str());
			if(ni==32 && ID>0){
				char sz1[128]={0};   
				sprintf(sz1,"R%d_%d.txt",ni,ID);
				writeTable(&S1i,sz1);                  
			}
		}		
		if(ID==-1)   
		{
			string strR=calcRingInvariant(&S1i);
			if(S.find(strR)==S.end()){				
				string str=gStr(r->m_Set[i]);
				string strj=gStr(r->m_Set[j]);
				printf("%d,%d=>%s,%s:R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",i,j,str.c_str(),strj.c_str(),ni,ID,strR.c_str());				
			}
			S.insert(strR);			
		}		   
	}			
}

string IMStr(ZimodnZ *r,int i){
	return gStr(r->m_Set[i]);
}

int main(int argc, char* argv[]){ 
	if(argc<3){
		printf("usage:gint m n\n");
		return 0;
	}	
    int m0=atoi(argv[1]);
    int n0=atoi(argv[2]);
    int m1=argc>3?atoi(argv[3]):0;
    int n1=argc>4?atoi(argv[4]):0;	
    int m2=argc>5?atoi(argv[5]):0;
    int n2=argc>6?atoi(argv[6]):0;
    int m3=argc>7?atoi(argv[7]):0;	
	if(argc==3||argc==4){	
		ZimodnZ r(GaussianInteger(m0,n0));
		char sz1[128]={0};   
		sprintf(sz1,"R%d_[%d.%d].txt",r.size(),m0,n0);
		writeTable(&r,sz1); 
		printf("\n写入文件%s\n",sz1);	
		findsubring2(&r,m1<=0?16:m1);
		if(m1==0){
			printf("\n");			
			for(int i=0;i<r.size();i++){
				string stri=IMStr(&r,i);		
				printf("i=%d=>%s\n",i,stri.c_str());	
			}	
		}		
		return 0;
	}
	if(argc==5||argc==6){	
		ZimodnZ r;
		r.initFR(GaussianInteger(m1,n1),GaussianInteger(m0,n0));
		char sz1[128]={0};   
		sprintf(sz1,"R%d_[%d.%d]_[%d.%d].txt",r.size(),m1,n1,m0,n0);
		writeTable(&r,sz1); 
		printf("\n写入文件%s\n",sz1);		
		findsubring2(&r,m2<=0?16:m2);
		if(m2==0){
			printf("\n");			
			for(int i=0;i<r.size();i++){
				string stri=IMStr(&r,i);		
				printf("i=%d=>%s\n",i,stri.c_str());	
			}	
		}		
		return 0;
	}
	if(argc==7||argc==8){	
		ZimodnZ r;
		r.initFR(GaussianInteger(m2,n2),GaussianInteger(m1,n1),GaussianInteger(m0,n0));
		char sz1[128]={0};   
		sprintf(sz1,"R%d_[%d.%d]_[%d.%d]_[%d.%d].txt",r.size(),m2,n2,m1,n1,m0,n0);
		writeTable(&r,sz1); 
		printf("\n写入文件%s\n",sz1);		
		findsubring2(&r,m3<=0?16:m3);
		if(m3==0){
			printf("\n");			
			for(int i=0;i<r.size();i++){
				string stri=IMStr(&r,i);		
				printf("i=%d=>%s\n",i,stri.c_str());	
			}	
		}		
		return 0;
	}	
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
	cout<<"范数不超过"<<Norm<<"的高斯整数共有"<<vgint.size()<<"个。"<<endl;
	sort(vgint.begin(), vgint.end(),lessGaussianInteger);
	for(int j=0;j<vgint.size();j++)
	{
		string strDes=ZimodnZ::IsPrime(vgint[j])?"是高斯素数":"不是高斯素数";
		int n=ZimodnZ::Norm(vgint[j]);
		cout<<"第"<<j+1<<"个高斯整数"<<vgint[j]<<"="<<gStr(vgint[j])<<"范数为"<<n<<strDes<<endl;
		if(n>0 && n!=10 && n!=26 && n!=34){
			ZimodnZ r(vgint[j]);
			r.printTable();
		}
	}
	system("pause");
	return 0;
}