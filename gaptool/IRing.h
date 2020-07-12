#ifndef IRING_H
#define IRING_H

#include<stdio.h>
#include<string.h>
#include<sstream>
#include<vector>
#include<map>
#include<tuple>
#include<iterator>
#include<algorithm>
#include<functional>
using namespace std;

struct IRing
{
	virtual ~IRing(){};	
	virtual void printTable() = 0;
	virtual int add(int a,int b) = 0;
	virtual int mul(int a,int b) = 0;
	virtual int size() = 0;	   
};

void printRing(IRing* r){
   int n=r->size();
   printf("[R%dAdd]\n",n);   
   for(int i=0;i<n;i++){
	   for(int j=0;j<n;j++){
		  int ij=r->add(i,j);
		  printf("%d ",ij+1);
	   } 
       printf("\n");   
   }
   printf("[R%dMul]\n",n);   
   for(int i=0;i<n;i++){
	   for(int j=0;j<n;j++){
		  int ij=r->mul(i,j);
		  printf("%d ",ij+1); 
	   } 
       printf("\n");	   
   }
}

// 一个环r的子环s
struct Subring:public IRing
{
public:
	// 静态函数
public:
	// 实现抽象基类的方法
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// 构造函数
	Subring();
	Subring(IRing* r,const vector<int>& gens);
	// 析构函数
	~Subring();	
	// 成员函数
    bool init(IRing* r,const vector<int>& gens,int N=0);//N>0时表示子环阶要小于等于N，否则初始化失败	
	// 成员变量
	vector<int> m_Set;
	IRing* m_r;
	int m_flag;// Subring对象析构时是否释放m_r指向的内存		
};

Subring::Subring(){
	
}

bool Subring::init(IRing* r,const vector<int>& gens,int N){
	m_r=r;
	int E=0;
	m_Set.push_back(E);
	for(int i=0;i<gens.size();i++)
	{
		if(gens[i]!=E)
			m_Set.push_back(gens[i]);
	}
	int R=m_Set.size();
	int cnt=R;
	int cnt1=R;
	do{
		cnt=m_Set.size();
		if(cnt==r->size())
			break;
		if(N>0 && cnt>N)
			return false;
		for(int i=0;i<cnt;i++)
		{
			for(int j=0;j<cnt;j++)
			{
				int IJ=m_r->mul(m_Set[i],m_Set[j]);
				vector<int>::iterator p=std::find(m_Set.begin(),m_Set.end(),IJ);
				if(p==m_Set.end()){
					m_Set.push_back(IJ);
				}
				int IJ1=m_r->add(m_Set[i],m_Set[j]);
				p=std::find(m_Set.begin(),m_Set.end(),IJ1);
				if(p==m_Set.end()){
					m_Set.push_back(IJ1);
				}
			}
		}
		cnt1=m_Set.size();
	}while(cnt1>cnt);
    return true;	
}

Subring::~Subring()
{
	if(m_flag==1 && m_r!=NULL){
		delete m_r;
		m_r=NULL;
	}
}

void Subring::printTable()
{
    int IdRing(IRing*);
	int ID=IdRing(this);
	printf("R%d_%d:\n",size(),ID);
	printRing(this);	
}

int Subring::add(int a,int b)
{
	int ij=m_r->add(m_Set[a],m_Set[b]);
	vector<int>::iterator p=std::find(m_Set.begin(),m_Set.end(),ij);
	int IJ=-1;
	if(p!=m_Set.end()){
		IJ=p-m_Set.begin();
	}
	return IJ;
}

int Subring::mul(int a,int b)
{
	int ij=m_r->mul(m_Set[a],m_Set[b]);
	vector<int>::iterator p=std::find(m_Set.begin(),m_Set.end(),ij);
	int IJ=-1;
	if(p!=m_Set.end()){
		IJ=p-m_Set.begin();
	}
	return IJ;
}

int Subring::size()
{
	return m_Set.size();
}

Subring::Subring(IRing* r,const vector<int>& gens)
{
	init(r,gens,0);
}

bool IsEqual(vector<int>& a,vector<int>& b){
    int na=a.size();
	int nb=b.size();
	if(na!=nb)
		return false;
	for(int i=0;i<na;i++)
		if(a[i]!=b[i])
			return false;
	return true;
}

// 判断环r是否真的是环
bool IsRing(IRing* r){
	int N=r->size();
	//Ⅰ加法的规律：<R,+>是交换群。
	for (int i=0; i<N; i++)
		for (int j=0; j<N; j++){
			if(r->add(i,j)!=r->add(j,i))
				return false;
			}		
	//零元
	for(int i=0;i<N;i++){
		if(r->mul(0,i)!=0||r->mul(i,0)!=0)
			return false;
	}
	//Ⅱ乘法满足结合律：<R ,·>是半群。
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			for(int k=0;k<N;k++){
				int ij=r->mul(i,j);
				int ij_k=r->mul(ij,k);
				int jk=r->mul(j,k);
				int i_jk=r->mul(i,jk);
				if(ij_k!=i_jk)
					return false;
			}
		}
	}
	//Ⅲ分配律：乘运算对加运算可分配，即a·(b+c)=ab+ac,(a+b)·c=ac+bc。
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			for(int k=0;k<N;k++){
				int JK=r->add(j,k);
				int I_JK=r->mul(i,JK);
				int IJ=r->mul(i,j);
				int IK=r->mul(i,k);
				int IJ_IK=r->add(IJ,IK);
				if(I_JK!=IJ_IK)
					return false;
				int ij=r->add(i,j);
				int ij_k=r->mul(ij,k);
				int ik=r->mul(i,k);
				int jk=r->mul(j,k);
				int ik_jk=r->add(ik,jk);
				if(ij_k!=ik_jk)
					return false;
			}
		}
	}
	return true;
}

/*
优化：
bool testFunc(){
	printf("enter testFunc\n");
	return true;
}
if(true||testFunc()){
	printf("Hit1\n");
}
printf("Hit2\n");
*/
// 判断集合I是否是环r的理想
int IsIdeal(IRing* r,vector<int> &I){
 //I是r的子环
 Subring s(r,I);
 bool bE=IsEqual(I,s.m_Set);
  if(!bE||!IsRing(&s)){
   return 0;// 子环也不是
  }
 //进一步判断是否是理想
 for(int i=0;i<r->size();i++){//任意纯量环元素c
  for(int j=0;j<I.size();j++){//任意向量模元素a
   int ca=r->mul(i,I[j]);
   int ac=r->mul(I[j],i);
	vector<int>::iterator p=std::find(I.begin(),I.end(),ca);
	if(p==I.end()){
		return 2;// 是子环但不是理想
	}   
	vector<int>::iterator p1=std::find(I.begin(),I.end(),ac);
	if(p1==I.end()){
		return 2;// 是子环但不是理想
	}
  }
 }
 return 1;//是理想
}

const char* IsIdealRetInfo(int iret){
	const char* sz[]={"不构成环","构成理想","构成非理想子环"};
	return sz[iret];
}

#ifndef IGROUP_H
string itos(int i)
{
  stringstream s;
  s << i;
  return s.str();
}

string V2S(vector<int>& v){
   string str="[";
   int n=v.size();
   for(int i=0;i<n;i++)
   {
	   str+=itos(v[i]);
	   if(i<n-1)
		   str+=",";   
   }	   
   str+="]";
   return str;
}
#endif

vector<int>  DivisorsInt(int n){
	vector<int> ret;
	if(n<1)
		return ret;
	for(int i=1;i<=n;i++){
		if(n%i==0){
			ret.push_back(i);
		}
	}
	return ret;
}

string simplyS1(vector<int>& S1){
    int n=S1.size();
	vector<int> vOrders=DivisorsInt(n);
	vector<int> vCounts(n+1);
 	for(int i=0;i<n;i++)
	{
		int ord=S1[i];
		vCounts[ord]=vCounts[ord]+1;
	}
	string str="[";
	for(int i=0;i<vOrders.size();i++)
	{
		char sz[200]={0};
		sprintf(sz,"%d,",vCounts[vOrders[i]]);
		str+=sz;
	}
	if(str.size()>2)
	{
		str=str.substr(0,str.size()-1);
	}
	str+="]";
	return str;
}

string calS1(IRing* r,bool bDone){

   int n=r->size();
   vector<int> S1;
   for(int i=0;i<n;i++){
	   vector<int> v;
	   v.push_back(i);
	   Subring si(r,v);
	   S1.push_back(si.size());
   }
   if(bDone)
   {
	   string str=simplyS1(S1);
	   return str;
   }	
   string str="[";
   for(int i=0;i<n;i++)
   {
	   str+=itos(S1[i]);
	   if(i<n-1)
		   str+=",";   
   }	   
   str+="]";
   return str;
}

vector<int> Order(IRing* r,int m){
	vector<int> ret;
	int mi=m;
	int m0=0;
    ret.push_back(m0);
	while(mi!=m0){
		ret.push_back(mi);
		mi=r->add(mi,m);
	}
	return ret;
}

string calcN0(IRing* r){
    int n=r->size();
	vector<int> vOrders=DivisorsInt(n);
	vector<int> vCounts(n+1);
 	for(int i=0;i<n;i++){
		vector<int> S1=Order(r,i);
		int ord=S1.size();
		vCounts[ord]=vCounts[ord]+1;
	}
	string strN0="[";
	for(int i=0;i<vOrders.size();i++){
		char sz[200]={0};
		sprintf(sz,"%d,",vCounts[vOrders[i]]);
		strN0+=sz;
	}
	if(strN0.size()>2){
		strN0=strN0.substr(0,strN0.size()-1);
	}
	strN0+="]";
	return strN0;
}

#ifndef IGROUP_H
//利用欧几里得算法计算两个数的最大公约数
int gcd(int a, int b)
{
	int temp;
	if(b == 0) return a;
	if(a == 0) return b;
	if(a%b == 0) return b;
	else return gcd(b, a%b);
}

int lcm(const vector<int>& v){
 int n=v.size();
 if(n<1)
  return 0;
 if(n==1)
  return v[0];
 int ret=v[0];
 for(int i=1;i<n;i++) {
  ret=(ret*v[i]/gcd(ret,v[i]));
 }
 return ret;
}
#endif

int calcn0(IRing* r){
	vector<int> v;
	int n=r->size();
	vector<int> vOrders=DivisorsInt(n);
	vector<int> vCounts(n+1);
	for(int i=0;i<n;i++){
		vector<int> S1=Order(r,i);
		int ord=S1.size();
		vCounts[ord]=vCounts[ord]+1;
	}
	for(int i=0;i<vOrders.size();i++){
	if(vCounts[vOrders[i]]!=0)
	  v.push_back(vOrders[i]);
	}
	int n0=lcm(v);
	return n0;
}

#ifndef IGROUP_H
vector<tuple<int,int,int> > doN2Vec(vector<pair<int,int> >& v){
	vector<tuple<int,int,int> > ret;
	int n=v.size();
	for(int i=0;i<n;i++){
		pair<int,int> vi=v[i];
		vector<tuple<int,int,int> >::const_iterator it=std::find_if(ret.begin(),ret.end(),[vi](tuple<int,int,int>& obj)->bool{if(get<0>(obj)==vi.first && get<1>(obj)==vi.second)return true;return false;});
		if(it==ret.end()){
			ret.push_back(make_tuple(vi.first,vi.second,1));
		}else{
			int cnt=get<2>(*it);
			ret[it-ret.begin()]=make_tuple(vi.first,vi.second,cnt+1);
		}
	}
	return ret;
}
#endif

string calcN2(IRing* r){
	int n=r->size();
	vector<pair<int,int> > v;
	for(int i=1;i<n;i++){
		for(int j=1;j<n;j++){
			if(r->mul(i,j)!=0){
				vector<int> S1i=Order(r,i);			
				int oi=S1i.size();
				vector<int> S1j=Order(r,j);			
				int oj=S1j.size();
				v.push_back(make_pair(oi,oj));
			}
		}
	}
	std::sort(v.begin(),v.end());
	vector<tuple<int,int,int> > v1=doN2Vec(v);
	string strN2="[";
	for(int i=0;i<v1.size();i++)
	{
		char sz[200]={0};
		sprintf(sz,"[%d,%d,%d],",get<0>(v1[i]),get<1>(v1[i]),get<2>(v1[i]));
		strN2+=sz;
	}
	if(strN2.size()>2)
	{
		strN2=strN2.substr(0,strN2.size()-1);
	}
	strN2+="]";
	return strN2;
}

string calcI1(IRing* r){
	int IdRing(IRing* r);
	int n=r->size();
	vector<pair<int,int> > v;
   for(int i=0;i<n;i++){
	   vector<int> vi;
	   vi.push_back(i);
	   Subring si(r,vi);
	   int ni=si.size();
	   if(ni<n && ni>0){
		int ID=IdRing(&si);
		v.push_back(make_pair(ni,ID));
	   }
   }
	std::sort(v.begin(),v.end());
	vector<tuple<int,int,int> > v1=doN2Vec(v);
	string str="[";
	for(int i=0;i<v1.size();i++)
	{
		char sz[200]={0};
		sprintf(sz,"[%d,%d,%d],",get<0>(v1[i]),get<1>(v1[i]),get<2>(v1[i]));
		str+=sz;
	}
	if(str.size()>2)
	{
		str=str.substr(0,str.size()-1);
	}
	str+="]";
	return str;
}

string calcI2(IRing* r){
	int IdRing(IRing* r);
	int n=r->size();
	vector<pair<int,int> > v;
   for(int i=0;i<n;i++){
	   for(int j=i+1;j<n;j++){
		   vector<int> vi;
		   vi.push_back(i);
		   vi.push_back(j);		   
		   Subring si(r,vi);
		   int ni=si.size();
		   if(ni<n && ni>0){
			int ID=IdRing(&si);
			v.push_back(make_pair(ni,ID));
		   }
	   }
   }
	std::sort(v.begin(),v.end());
	vector<tuple<int,int,int> > v1=doN2Vec(v);
	string str="[";
	for(int i=0;i<v1.size();i++)
	{
		char sz[200]={0};
		sprintf(sz,"[%d,%d,%d],",get<0>(v1[i]),get<1>(v1[i]),get<2>(v1[i]));
		str+=sz;
	}
	if(str.size()>2)
	{
		str=str.substr(0,str.size()-1);
	}
	str+="]";
	return str;
}

int ZeroNum(IRing* r){
    int n=r->size();
	int iRet=0;
	for(int i=0;i<n;i++)for(int j=0;j<n;j++){if(r->mul(i,j)==0)iRet++;}
	//printf("零乘个数n6=%d,",iRet);
	return iRet;
}

int ZeroFactorNum(IRing* r){
    int n=r->size();
	int iRet=0;
	for(int i=1;i<n;i++){
		for(int j=1;j<n;j++){
			if(r->mul(i,j)==0){
				iRet++;
			    break;
			}
		}
	}
	//printf("零因子个数n7=%d,",iRet);
	return iRet;
}

bool IsIdempotent(IRing* r,int i2){
    int n=r->size();	
	if(i2<0||i2>=n)
		return false;
	if(r->mul(i2,i2)==i2)
		return true;
	return false;
}

int IdempotentNum(IRing* r){
    int n=r->size();
	int iRet=0;
	for(int i2=0;i2<n;i2++){if(IsIdempotent(r,i2))iRet++;}
	//printf("幂等元个数n2=%d,",iRet);
	return iRet;
}

int Nil2Num(IRing* r){
    int n=r->size();
	int iRet=0;
	for(int i4=1;i4<n;i4++){if(r->mul(i4,i4)==0)iRet++;}
	//printf("2次幂零元个数n4=%d,",iRet);
	return iRet;
}

int Nil3Num(IRing* r){
    int n=r->size();
	int iRet=0;
	for(int i5=1;i5<n;i5++){int j=r->mul(i5,i5);if(r->mul(j,i5)==0)iRet++;}
	//printf("2~3次幂零元个数n5=%d,",iRet);
	return iRet;
}

// n2代表幂等元个数
// Idem代表环r的幂等元集合（N2已代表另外一个环不变量）
vector<int> calIdem(IRing* r){
    int n=r->size();
	vector<int> Idem;
	for(int i2=0;i2<n;i2++){
		if(IsIdempotent(r,i2))
			Idem.push_back(i2);
	}
	return Idem;
}

// n4,n5代表2次幂零元个数、2~3次幂零元个数
// n9=N9.size()代表幂零元个数
int IsNil(IRing* r,int i){
    int n=r->size();
	int iRet=i;
	if(iRet==0)
		return 1;// i=0是环r的1次幂零元
	for(int k=0;k<n;k++){// 最多n+1次幂零元
		iRet=r->mul(iRet,i);
		if(iRet==0)
			return k+2;// i是环r的k+2次幂零元
	}
	return 0;// i不是环r的幂零元
}

// N9代表环r的幂零元集合,处理过的N9代表[IdRing(N9),IdRing(r/N9)]或[IdRing(N9)]
vector<int> calN9(IRing* r){
    int n=r->size();
	vector<int> N9;
	for(int i9=0;i9<n;i9++){
		if(IsNil(r,i9)>0)
			N9.push_back(i9);
	}
	return N9;
}

bool IsAbelian(IRing* r){
    int n=r->size();
	for (int i=0; i<n; i++)for (int j=0; j<n; j++){if(r->mul(i,j)!=r->mul(j,i))return false;}
	return true;
}

bool IsOne(IRing* r,int i3){
    int n=r->size();
	if(i3<0||i3>=n)
		return false;
	for (int i=0; i<n; i++){
		if(r->mul(i3,i)!=i || r->mul(i,i3)!=i)
			return false;
	}
	return true;
}

int One(IRing* r){
    int n=r->size();
	for (int i3=0; i3<n; i3++){if(IsOne(r,i3))return i3;}
	return -1;
}

// 是否有可逆元,-1表示没有
int Inv(IRing* r,int i1,int iOne){
    int n=r->size();
	for (int i=0; i<n; i++){if(r->mul(i1,i)==iOne && r->mul(i,i1)==iOne)return i;}
	return -1;
}

// 不可逆元个数n1
int NoInvNum(IRing* r){
    int n=r->size();
	int iRet=0;
	int iOne=One(r);
	if(iOne==-1)
	   return n;
	for(int i1=0;i1<n;i1++){if(Inv(r,i1,iOne)==-1)iRet++;}
	return iRet;
}

bool IsInCenter(IRing* r,int j){
	int N=r->size();
	for(int i=0;i<N;i++){
		int ij=r->mul(i,j);
		int ji=r->mul(j,i);
		if(ij==ji)
			continue;
		else
			return false;
	}
	return true;
}

vector<int> Center(IRing* r){
	vector<int> ret;
	int N=r->size();
	for(int i=0;i<N;i++){
		if(IsInCenter(r,i))
			ret.push_back(i);
		else
			continue;
	}
	return ret;
}

//-----------------------------Begin of IdRing---------------------------------
class RIDHelper
{
public:
	RIDHelper();
	~RIDHelper();
private:
	multimap<string,int> m_RingInvariant;//根据环的结构不变量N0n0bAbOn1n2n4n5n6n7n8S1N2返回ID编号列表	
	multimap<string,int> m_I1I2;//根据环的结构不变量I1I2返回ID编号列表	
public:
	vector<int> IDFromRingInvariant(string& RingInvariant);	
	vector<int> IDFromI1I2(string& I1I2);	
	vector<string> I1I2FromID(int ID);	
};

RIDHelper::RIDHelper(){
	// N0,【n0,bA,bO,n1,n2,n4,n5,n6,n7】,n8,S1,N2
	//R1
	m_RingInvariant.insert(make_pair("1,0,1,1,0,0,1,0,1",1));	
	//R2
	m_RingInvariant.insert(make_pair("2,1,0,2,1,1,1,4,1",1));
	m_RingInvariant.insert(make_pair("2,1,1,1,2,0,0,3,0",2));	
	//R3
	m_RingInvariant.insert(make_pair("3,1,0,3,1,2,2,9,2",1));
	m_RingInvariant.insert(make_pair("3,1,1,1,2,0,0,5,0",2));		
	//R4
	m_RingInvariant.insert(make_pair("4,1,0,4,1,3,3,16,3",1));
	m_RingInvariant.insert(make_pair("4,1,0,4,1,1,3,12,3",2));
	m_RingInvariant.insert(make_pair("4,1,1,2,2,1,1,8,1",3));
	m_RingInvariant.insert(make_pair("2,1,0,4,1,3,3,16,3",4));	
	m_RingInvariant.insert(make_pair("2,1,0,4,1,1,3,12,3",5));
	m_RingInvariant.insert(make_pair("2,1,0,4,2,1,1,12,3",6));
	m_RingInvariant.insert(make_pair("2,0,0,4,3,1,1,10,1",7));
	m_RingInvariant.insert(make_pair("2,0,0,4,3,1,1,10,3",8));	
	m_RingInvariant.insert(make_pair("2,1,1,2,2,1,1,8,1",9));
	m_RingInvariant.insert(make_pair("2,1,1,3,4,0,0,9,2",10));
	m_RingInvariant.insert(make_pair("2,1,1,1,2,0,0,7,0",11));	
	//R5
	m_RingInvariant.insert(make_pair("5,1,0,5,1,4,4,25,4",1));
	m_RingInvariant.insert(make_pair("5,1,1,1,2,0,0,9,0",2));
	//R6
	m_RingInvariant.insert(make_pair("6,1,0,6,1,5,5,36,5",1));
	m_RingInvariant.insert(make_pair("6,1,0,6,2,1,1,20,5",2));
	m_RingInvariant.insert(make_pair("6,1,0,6,2,2,2,27,5",3));
	m_RingInvariant.insert(make_pair("6,1,1,4,4,0,0,15,3",4));
	//R7
	m_RingInvariant.insert(make_pair("1,0,7,1,6,6,49,6,7",1));
	m_RingInvariant.insert(make_pair("1,1,1,2,0,0,13,0,7",2));	
	//R8
	m_RingInvariant.insert(make_pair("8,1,0,8,1,7,7,64,7",1));
	m_RingInvariant.insert(make_pair("8,1,0,8,1,3,3,32,7",2));	
	m_RingInvariant.insert(make_pair("8,1,1,4,2,1,3,20,3,8,[1,1,2,4]",3));
	m_RingInvariant.insert(make_pair("8,1,0,8,1,3,7,48,7",4));	
	m_RingInvariant.insert(make_pair("4,1,0,8,1,7,7,64,7",5));
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,0,8,1,3,7,48,7,8,[1,1,6,0]",6));	
	m_RingInvariant.insert(make_pair("4,1,0,8,2,3,3,48,7",7));
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,0,0,8,1,5,7,48,7,2,[1,3,4,0],[[2,4,8],[4,4,8]]",8));	
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,0,0,8,1,5,7,48,7,2,[1,1,6,0]",9));
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,0,0,8,1,5,7,48,7,2,[1,3,4,0],[[4,2,8],[4,4,8]]",10));
	m_RingInvariant.insert(make_pair("4,1,0,8,1,7,7,40,7",11));
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,0,8,1,3,7,40,7,8,[1,1,6,0]",12));	
	m_RingInvariant.insert(make_pair("4,1,0,8,2,3,3,32,7",13));
	m_RingInvariant.insert(make_pair("4,1,1,6,4,1,1,24,5,8,[1,2,5,0]",14));	
	m_RingInvariant.insert(make_pair("4,0,0,8,3,3,3,28,7",15));	
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,0,8,1,3,7,48,7,8,[1,3,4,0]",16));	
	m_RingInvariant.insert(make_pair("4,1,0,8,2,1,3,36,7",17));
	m_RingInvariant.insert(make_pair("4,0,0,8,1,1,7,40,7",18));	
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,0,8,1,3,7,40,7,8,[1,3,4,0],[[2,4,8],[4,2,8],[4,4,8]]",19));
	m_RingInvariant.insert(make_pair("4,0,0,8,3,3,3,28,3",20));
	m_RingInvariant.insert(make_pair("4,1,1,4,2,3,3,24,3",21));
	m_RingInvariant.insert(make_pair("4,1,1,4,2,1,3,20,3",22));	
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,0,8,1,3,7,48,7,8,[1,3,0,4]",23));
	m_RingInvariant.insert(make_pair("4,1,0,8,1,3,3,32,7",24));	
	m_RingInvariant.insert(make_pair("2,1,0,8,1,7,7,64,7",25));
	m_RingInvariant.insert(make_pair("2,1,0,8,1,3,7,48,7",26));	
	m_RingInvariant.insert(make_pair("2,1,0,8,2,3,3,48,7",27));
	m_RingInvariant.insert(make_pair("2,0,0,8,1,5,7,48,7",28));	
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,0,0,8,3,3,3,40,7,2,[1,5,2,0],[[2,2,24]]",29));
	m_RingInvariant.insert(make_pair("2,0,0,8,5,3,3,36,3",30));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,0,8,1,7,7,40,7,8,[1,7,0,0],[[2,2,24]]",31));//I4:[[0,2],[0,0]],[[3,2],[0,3]]
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,0,8,1,3,7,40,7,8,[1,3,4,0],[[2,2,24]]",32));//I4:[[3,1],[0,0]],[[0,1],[0,3]]	
	m_RingInvariant.insert(make_pair("2,1,0,8,1,3,3,32,7",33));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,0,0,8,3,3,3,40,7,2,[1,5,2,0],[[2,2,24]]",34));	
	m_RingInvariant.insert(make_pair("2,1,0,8,2,3,3,32,7",35));	
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,0,0,8,5,3,3,32,7,1,[1,7,0,0],[[2,2,32]]",36));//J4:[[1,1],[1,1]],[[1,3],[2,0]]	
	m_RingInvariant.insert(make_pair("2,0,0,8,3,3,3,28,3",37));
	m_RingInvariant.insert(make_pair("2,1,0,8,2,1,3,36,7",38));	
	m_RingInvariant.insert(make_pair("2,0,0,8,1,1,7,40,7",39));
	m_RingInvariant.insert(make_pair("2,1,0,8,4,1,1,36,7",40));
	m_RingInvariant.insert(make_pair("2,0,0,8,6,1,1,30,5,2",41));
	m_RingInvariant.insert(make_pair("2,1,0,8,2,1,1,28,7,8",42));	
	m_RingInvariant.insert(make_pair("2,0,0,8,5,3,3,36,7",43));
	m_RingInvariant.insert(make_pair("2,0,0,8,3,3,3,28,7",44));	
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,1,4,2,3,3,24,3,8,[1,4,3,0],[[2,2,40]]",45));	
	m_RingInvariant.insert(make_pair("2,1,1,4,2,1,3,20,3",46));	
	m_RingInvariant.insert(make_pair("2,0,0,8,6,1,1,30,7",47));
	m_RingInvariant.insert(make_pair("2,1,1,6,4,1,1,24,5",48));	
	m_RingInvariant.insert(make_pair("2,0,1,6,6,1,1,26,5",49));
	m_RingInvariant.insert(make_pair("2,1,1,7,8,0,0,27,6",50));
	m_RingInvariant.insert(make_pair("2,1,1,5,4,0,0,21,4",51));
	m_RingInvariant.insert(make_pair("2,1,1,1,2,0,0,15,0",52));
	//R8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,6,2]],[[2,1,3],[2,2,2],[4,4,3],[4,6,8],[4,7,3]]",29));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,6,2]],[[2,1,3],[2,2,2],[4,4,3],[4,6,8],[4,8,3]]",34));	
	//R9
	m_RingInvariant.insert(make_pair("9,1,0,9,1,8,8,81,8,9",1));
	m_RingInvariant.insert(make_pair("9,1,1,3,2,2,2,21,2,9",2));
	m_RingInvariant.insert(make_pair("9,1,0,9,1,2,8,45,8,9",3));
	m_RingInvariant.insert(make_pair("3,1,0,9,1,8,8,81,8,9",4));	
	m_RingInvariant.insert(make_pair("3,1,0,9,1,2,8,45,8,9",5));
	m_RingInvariant.insert(make_pair("3,1,0,9,2,2,2,45,8,9",6));
	m_RingInvariant.insert(make_pair("3,0,0,9,4,2,2,33,2,1",7));
	m_RingInvariant.insert(make_pair("3,0,0,9,4,2,2,33,8,1",8));	
	m_RingInvariant.insert(make_pair("3,1,1,3,2,2,2,21,2,9",9));
	m_RingInvariant.insert(make_pair("3,1,1,5,4,0,0,25,4,9",10));
	m_RingInvariant.insert(make_pair("3,1,1,1,2,0,0,17,0,9",11));
	//R10
	m_RingInvariant.insert(make_pair("10,1,0,10,1,9,9,100,9,10",1));
	m_RingInvariant.insert(make_pair("10,1,0,10,2,1,1,36,9,10",2));
	m_RingInvariant.insert(make_pair("10,1,1,6,4,0,0,27,5,10",3));
	m_RingInvariant.insert(make_pair("10,1,0,10,2,4,4,75,9,10",4));	
	//R11
	m_RingInvariant.insert(make_pair("1,0,11,1,10,10,121,10,11",1));
	m_RingInvariant.insert(make_pair("1,1,1,2,0,0,21,0,11",2));	
	//R12
	m_RingInvariant.insert(make_pair("[1,1,2,2,2,4],12,1,0,12,1,11,11,144,11,12",1));
	m_RingInvariant.insert(make_pair("[1,1,2,2,2,4],12,1,0,12,2,1,3,60,11,12",2));
	m_RingInvariant.insert(make_pair("[1,1,2,2,2,4],12,1,0,12,2,5,5,72,11,12",3));
	m_RingInvariant.insert(make_pair("[1,1,2,2,2,4],12,1,0,12,2,3,3,80,11,12",4));	
	m_RingInvariant.insert(make_pair("[1,1,2,2,2,4],12,1,1,8,4,1,1,40,7,12",5));
	m_RingInvariant.insert(make_pair("[1,1,2,2,2,4],12,1,0,12,1,5,11,108,11,12",6));
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,1,0,12,1,11,11,144,11,12",7));
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,1,0,12,1,5,11,108,11,12",8));	
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,1,0,12,2,5,5,108,11,12",9));
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,0,0,12,3,5,5,90,11,3",10));
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,0,0,12,3,5,5,90,11,3",11));
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,1,0,12,2,5,5,72,11,12",12));
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,1,0,12,4,1,1,60,11,12",13));
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,1,1,10,8,0,0,45,9,12",14));	
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,0,0,12,6,1,1,50,11,3",15));
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,1,0,12,2,3,3,80,11,12",16));
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,1,0,12,2,1,3,60,11,12",17));
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,0,0,12,6,1,1,50,7,3",18));	
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,1,1,8,4,1,1,40,7,12",19));
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,1,0,12,4,2,2,81,11,12",20));
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,1,0,12,2,2,2,63,11,12",21));	
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,1,1,6,4,0,0,35,5,12",22));	
	//R13
	m_RingInvariant.insert(make_pair("1,0,13,1,12,12,169,12,13",1));
	m_RingInvariant.insert(make_pair("1,1,1,2,0,0,25,0,13",2));	
	//R14
	m_RingInvariant.insert(make_pair("14,1,0,14,1,13,13,196,13,14",1));
	m_RingInvariant.insert(make_pair("14,1,0,14,2,1,1,52,13,14",2));
	m_RingInvariant.insert(make_pair("14,1,1,8,4,0,0,39,7,14",3));
	m_RingInvariant.insert(make_pair("14,1,0,14,2,6,6,147,13,14",4));
	//R15
	m_RingInvariant.insert(make_pair("15,1,0,15,1,14,14,225,14,15",1));
	m_RingInvariant.insert(make_pair("15,1,1,7,4,0,0,45,6,15",2));
	m_RingInvariant.insert(make_pair("15,1,0,15,2,2,2,81,14,15",3));
	m_RingInvariant.insert(make_pair("15,1,0,15,2,4,4,125,14,15",4));	
	//R16
	m_RingInvariant.insert(make_pair("[1,1,2,4,8],16,1,0,16,1,15,15,256,15,16,[1,1,2,4,8],[]",1));//A16
	m_RingInvariant.insert(make_pair("[1,1,2,4,8],16,1,0,16,1,3,7,80,15,16,[1,1,2,4,8],[[4,16,16],[8,8,16],[8,16,32],[16,4,16],[16,8,32],[16,16,64]]",2));
	m_RingInvariant.insert(make_pair("[1,1,2,4,8],16,1,1,8,2,3,3,48,7,16,[1,1,2,4,8],[[2,16,8],[4,8,8],[4,16,16],[8,4,8],[8,8,16],[8,16,32],[16,2,8],[16,4,16],[16,8,32],[16,16,64]]",3));//C16
	m_RingInvariant.insert(make_pair("[1,1,2,4,8],16,1,0,16,1,7,15,128,15,16,[1,1,2,4,8],[[8,16,32],[16,8,32],[16,16,64]]",4));//B16	
	m_RingInvariant.insert(make_pair("[1,1,2,4,8],16,1,0,16,1,7,15,192,15,16,[1,1,2,4,8],[[16,16,64]]",5));
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,1,12,4,1,3,60,11,16,[1,2,3,10,0],[[2,2,4],[2,4,4],[2,8,20],[4,2,4],[4,4,16],[4,8,32],[8,2,20],[8,4,32],[8,8,64]]",6));
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,1,8,2,3,7,64,7,16,[1,3,4,4,4],[[2,8,24],[4,4,16],[4,8,32],[8,2,24],[8,4,32],[8,8,64]]",7));
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,7,7,128,15,16,[1,3,4,0,8],[[4,8,32],[8,4,32],[8,8,64]]",8));
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,7,15,192,15,16,[1,3,4,8,0],[[8,8,64]]",9));	
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,7,7,128,15,16,[1,3,4,8,0],[[4,8,32],[8,4,32],[8,8,64]]",10));
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,2,3,7,80,15,16,[1,3,4,4,4],[[2,8,16],[4,4,16],[4,8,32],[8,2,16],[8,4,32],[8,8,64]]",11));
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,15,15,256,15,16,[1,3,4,8,0],[]",12));
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,2,7,7,192,15,16,[1,2,3,6,4],[[2,2,4],[2,4,4],[2,8,8],[4,2,4],[4,4,4],[4,8,8],[8,2,8],[8,4,8],[8,8,16]]",13));
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,2,3,3,96,15,16,[1,2,3,6,4],[[2,2,4],[2,4,4],[2,8,8],[4,2,4],[4,4,4],[4,8,32],[8,2,8],[8,4,32],[8,8,64]]",14));
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,2,3,7,144,15,16,[1,2,3,6,4],[[2,2,4],[2,4,4],[2,8,8],[4,2,4],[4,4,4],[4,8,8],[8,2,8],[8,4,8],[8,8,64]]",15));	
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,15,15,160,15,16,[1,3,4,8,0],[[2,8,16],[4,8,16],[8,2,16],[8,4,16],[8,8,32]]",16));
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,7,7,128,15,16,[1,3,4,0,8],[[2,8,16],[4,8,16],[8,2,16],[8,4,16],[8,8,64]]",17));
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,0,0,16,1,11,15,192,15,4,[1,3,4,8,0],[[2,8,16],[4,8,16],[8,8,32]]",18));
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,0,0,16,1,7,7,128,15,4,[1,3,4,8,0],[[2,8,16],[4,8,16],[8,4,32],[8,8,64]]",19));	
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,0,0,16,3,3,7,72,7,4,[1,3,4,8,0],[[2,8,16],[4,4,16],[4,8,32],[8,2,24],[8,4,32],[8,8,64]]",20));	
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,0,0,16,1,11,15,192,15,4,[1,3,4,8,0],[[8,2,16],[8,4,16],[8,8,32]]",21));	
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,7,15,192,15,16,[1,3,4,0,8],[[8,8,64]]",22));	
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,0,0,16,3,3,7,72,15,4,[1,3,4,8,0],[[2,8,24],[4,4,16],[4,8,32],[8,2,16],[8,4,32],[8,8,64]]",23));	
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,7,7,128,15,16,[1,3,4,8,0],[[2,8,16],[4,8,16],[8,2,16],[8,4,16],[8,8,64]]",24));//[[3,1],[1,3]],[[0,4],[4,0]]	
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,0,0,16,1,7,7,128,15,4,[1,3,4,8,0],[[4,8,32],[8,2,16],[8,4,16],[8,8,64]]",25));//[[2,0],[1,0]],[[4,0],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,7,15,160,15,16,[1,3,4,8,0],[[2,8,16],[4,8,16],[8,2,16],[8,4,16],[8,8,32]]",26));//[[4,0],[0,4]],[[0,0],[7,4]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,7,15,128,15,16,[1,3,4,0,8],[[2,8,16],[4,8,16],[8,2,16],[8,4,16],[8,8,64]]",27));//[[0,4],[3,2]]	
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,7,15,192,15,16,[1,1,6,8,0],[[2,2,4],[2,4,4],[2,8,8],[4,2,4],[4,4,4],[4,8,8],[8,2,8],[8,4,8],[8,8,16]]",29));//R8_1×R8_2=R8R4_53的商环
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,3,7,112,15,16,[1,1,6,8,0],[[2,2,4],[2,4,4],[2,8,8],[4,2,4],[4,4,4],[4,8,24],[8,2,8],[8,4,24],[8,8,64]]",30));//R8_2×R8_2=R8R4_54的商环
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,7,15,160,15,16,[1,1,6,8,0],[[2,2,4],[2,4,4],[2,8,8],[4,2,4],[4,4,4],[4,8,8],[8,2,8],[8,4,8],[8,8,48]]",31));//R8_4×R8_2=R8R4_56的商环
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,1,8,2,3,7,56,7,16,[1,1,6,4,4],[[2,2,4],[2,4,4],[2,8,24],[4,2,4],[4,4,12],[4,8,32],[8,2,24],[8,4,32],[8,8,64]]",32));//Rpp(9,8)的商环、R8_3/([[4,0,1])的商环
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,0,0,16,1,3,15,160,15,4,[1,1,6,8,0],[[2,2,4],[2,4,4],[2,8,8],[4,2,4],[4,4,4],[4,8,8],[8,2,8],[8,4,8],[8,8,48]]",33));
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,3,15,112,15,16,[1,3,12,0,0],[[4,4,144]]",70));//R8_19×R8_19=R8R8_955的子环	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,7,15,160,15,16,[1,3,8,4,0],[[4,4,96]]",71));//[[0,0,0],[0,0,0],[1,0,2]],[[0,0,0],[1,0,0],[0,2,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,11,15,144,15,4,[1,3,8,4,0],[[4,4,112]]",72));//[[0,0,0],[0,0,0],[1,2,0]],[[0,0,0],[1,0,2],[0,0,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,3,15,160,15,4,[1,3,4,8,0],[[4,4,96]]",73));//[[0,0,0],[0,0,0],[3,0,2]],[[0,0,0],[1,0,0],[0,2,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,3,15,128,15,4,[1,3,4,8,0],[[4,4,128]]",74));//[[0,0,0],[0,0,0],[3,0,2]],[[0,0,0],[1,0,2],[0,2,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,3,15,144,15,16,[1,3,4,8,0],[[4,4,112]]",75));//[[0,0,0],[0,0,0],[3,0,2]],[[0,0,0],[1,2,0],[0,2,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,3,15,144,15,4,[1,3,4,8,0],[[4,4,112]]",76));//[[0,0,0],[0,0,0],[3,0,2]],[[0,0,0],[1,2,2],[0,2,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,7,15,144,15,4,[1,3,4,8,0],[[4,4,112]]",77));//[[0,0,0],[0,0,0],[3,2,0]],[[0,0,0],[1,0,2],[0,2,0]]	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,3,15,144,15,4,[1,3,0,12,0],[[4,4,112]]",78));//[[0,0,0],[0,2,2],[1,2,2]],[[0,0,0],[1,0,0],[0,2,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,3,15,144,15,16,[1,3,0,12,0],[[4,4,112]]",79));//[[0,0,0],[0,2,2],[1,2,2]],[[0,0,0],[1,0,2],[0,2,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,3,15,128,15,4,[1,3,0,12,0],[[4,4,128]]",80));//[[0,0,0],[0,0,2],[1,0,2]],[[0,0,0],[1,2,0],[0,2,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,8,4,0],[[4,4,96]]",81));	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,11,15,192,15,4,[1,3,8,4,0],[[4,4,64]]",82));	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,11,15,192,15,4,[1,3,12,0,0],[[4,4,64]]",83));	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,7,15,160,15,16,[1,3,4,8,0],[[4,4,96]]",84));//[[0,1,1,0],[1,1,2,1],[1,1,2,1],[1,0,1,1]],[[1,1,0,1],[0,2,2,0],[2,0,2,2],[3,1,2,3]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,4,8,0],[[4,4,96]]",85));//[[0,1,1],[0,2,2],[0,2,2]],[[0,1,3],[0,3,3],[0,3,3]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,11,15,144,15,4,[1,3,12,0,0],[[4,4,112]]",86));//[[2,3,0],[0,2,0],[0,2,0]],[[2,3,1],[0,2,0],[0,2,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,3,15,144,15,4,[1,3,8,4,0],[[4,4,112]]",87));//[[3,1,1,3],[3,1,1,3],[1,3,3,1],[3,1,1,3]],[[0,1,1,0],[0,1,1,0],[2,3,3,2],[0,1,1,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,7,15,144,15,4,[1,3,12,0,0],[[4,4,112]]",88));//[[0,0,0],[0,1,1],[0,1,1]],[[2,3,3],[0,2,0],[0,0,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,7,15,144,15,4,[1,3,8,4,0],[[4,4,112]]",89));//[[0,0,0],[0,0,0],[0,1,0]],[[0,0,0],[2,0,0],[1,0,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,3,15,128,15,4,[1,3,8,4,0],[[4,4,128]]",90));//[[0,0,0],[0,0,0],[0,1,2]],[[0,0,0],[0,2,0],[1,0,2]]	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,4,3,3,144,15,16,[1,3,6,6,0],[[4,4,112]]",91));//R8R8_690的子环
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,4,3,3,96,15,16,[1,3,6,6,0],[[2,4,16],[4,2,16],[4,4,128]]",92));//R8R8_690的子环
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,3,15,112,15,16,[1,3,0,12,0],[[4,4,144]]",93));//M2(R8_8)、R8R8_584的子环
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,3,15,112,15,4,[1,3,4,8,0],[[4,4,144]]",94));//M2(R8_8)、R8_18×R8_11=R8R8_538的子环
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,7,15,128,15,16,[1,3,4,8,0],[[4,4,128]]",95));//M2(R8_8)、R8_11×R8_6=R8R8_271的子环
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,5,5,7,88,7,1,[1,3,12,0,0],[[2,4,24],[4,2,24],[4,4,120]]",101));//G16
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,5,5,7,88,15,1,[1,3,12,0,0],[[2,4,24],[4,2,24],[4,4,120]]",102));//H16	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,3,15,144,15,16,[1,3,12,0,0],[[4,4,112]]",103));
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,1,12,4,3,3,64,11,16,[1,3,6,6,0],[[2,4,28],[4,2,28],[4,4,136]]",104));//J16	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,1,8,2,3,3,48,7,16,[1,3,2,6,4],[[2,4,32],[4,2,32],[4,4,144]]",105));
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,1,4,2,3,3,40,3,16,[1,3,2,2,8],[[2,4,36],[4,2,36],[4,4,144]]",106));//K16
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,15,15,256,15,16,[1,3,12,0,0],[]",107));//D16
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,12,0,0],[[4,4,96]]",108));	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,3,7,80,15,16,[1,3,4,0,8],[[2,4,16],[4,2,16],[4,4,144]]",109));
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,1,8,2,7,7,56,7,16,[1,3,6,2,4],[[2,4,32],[4,2,32],[4,4,136]]",110));//I16
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,7,15,128,15,16,[1,3,4,0,8],[[2,4,16],[4,2,16],[4,4,96]]",111));//E16
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,2,7,7,128,15,16,[1,3,6,2,4],[[2,4,16],[4,2,16],[4,4,96]]",112));//F16
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,2,3,7,96,15,16,[1,3,6,2,4],[[2,4,16],[4,2,16],[4,4,128]]",113));
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,7,7,128,15,16,[1,3,4,0,8],[[2,4,16],[4,2,16],[4,4,96]]",114));
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,1,8,2,3,7,56,7,16,[1,3,6,2,4],[[2,4,32],[4,2,32],[4,4,136]]",115));	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,7,15,192,15,16,[1,3,8,4,0],[[4,4,64]]",116));	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,7,15,192,15,16,[1,3,4,8,0],[[4,4,64]]",117));
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,7,15,128,15,16,[1,3,8,4,0],[[4,4,128]]",118));
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,3,15,112,15,16,[1,3,4,8,0],[[4,4,144]]",119));	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,15,15,160,15,16,[1,3,12,0,0],[[4,4,96]]",120));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,160,15,4,[1,5,10,0,0],[[2,2,16],[2,4,16],[4,2,32],[4,4,32]]",126));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,160,15,4,[1,5,10,0,0],[[2,2,16],[2,4,32],[4,2,16],[4,4,32]]",127));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,112,15,4,[1,3,12,0,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,64]]",128));//R8_18×R8_18=R8R8_902的子环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,128,15,4,[1,3,12,0,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,48]]",129));//M2(R8_12)的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,192,15,4,[1,5,10,0,0],[[2,2,16],[2,4,16],[4,2,16],[4,4,16]]",130));//R8R4_8的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,15,15,160,15,16,[1,7,8,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,24]]",131));//R8R4_11的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,160,15,16,[1,3,12,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,24]]",132));//R8R4_12的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,160,15,4,[1,1,14,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,24]]",133));//R8R4_18的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,7,128,15,16,[1,3,4,8,0],[[2,2,32],[2,4,32],[4,2,32],[4,4,32]]",134));//R8R4_24的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,5,10,0,0],[[2,2,16],[2,4,32],[4,2,16],[4,4,32]]",135));//R8R4_60的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,3,7,112,15,16,[1,3,4,8,0],[[2,2,32],[2,4,32],[4,2,32],[4,4,48]]",136));//R8R4_76的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,5,10,0,0],[[2,2,16],[2,4,16],[4,2,16],[4,4,48]]",137));//R8R4_80的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,144,15,16,[1,7,8,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,40]]",138));//R8R4_71的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,144,15,4,[1,5,10,0,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,32]]",139));//R8R4_70的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,144,15,16,[1,3,12,0,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,32]]",140));//R8R4_64的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,3,15,112,15,16,[1,3,8,4,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,64]]",147));//R8_19×R8_12=R8R8_591的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,3,15,112,15,16,[1,3,4,8,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,64]]",148));//R8_12×R8_12=R8R8_584的子环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,112,15,4,[1,3,4,8,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,64]]",149));//R8_18×R8_11=R8R8_538、M2(R8_9)的子环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,144,15,4,[1,3,12,0,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,32]]",150));//M2(R8_9)的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,128,15,4,[1,3,8,4,0],[[2,2,16],[2,4,16],[4,2,32],[4,4,64]]",151));//M2(R8_18)的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,192,15,4,[1,3,12,0,0],[[2,2,16],[2,4,16],[4,2,16],[4,4,16]]",152));//R8_9×R4_5的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,128,15,4,[1,3,4,8,0],[[2,2,16],[2,4,16],[4,2,32],[4,4,64]]",153));//R8_12×R8_10=R8R8_480的子环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,144,15,4,[1,7,8,0,0],[[2,4,32],[4,2,32],[4,4,48]]",154));//R8_10×R8_8=R8R8_374、M2(R8_6)的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,144,15,4,[1,7,0,8,0],[[2,4,32],[4,2,32],[4,4,48]]",155));//R8_10×R8_8=R8R8_374的子环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,12,0,0],[[2,2,16],[2,4,32],[4,2,16],[4,4,32]]",156));//R8_8×R4_5的商环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,144,15,4,[1,3,12,0,0],[[2,2,16],[2,4,32],[4,2,16],[4,4,48]]",157));//R8_10×R8_8=R8R8_376、M2(R8_12)的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,128,15,4,[1,3,8,4,0],[[2,2,16],[2,4,32],[4,2,16],[4,4,64]]",158));//M2(R8_18)的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,128,15,4,[1,3,4,8,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,48]]",159));//R8_39×R8_9=R8R8_455、M2(R8_9)的子环		
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,144,15,4,[1,3,8,4,0],[[2,2,16],[2,4,16],[4,2,32],[4,4,48]]",160));//R8_9×R8_10=R8R8_477的子环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,144,15,4,[1,3,12,0,0],[[2,2,16],[2,4,16],[4,2,32],[4,4,48]]",161));//M2(R8_12)的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,128,15,16,[1,3,12,0,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,48]]",162));//R8_12×R8_6=R8R8_272、M2(R8_12)的子环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,128,15,4,[1,3,4,8,0],[[2,2,16],[2,4,32],[4,2,16],[4,4,64]]",163));//R8_12×R8_8=R8R8_376的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,12,0,0],[[2,2,16],[2,4,16],[4,2,32],[4,4,32]]",164));//M2(R8_6)的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,144,15,4,[1,3,4,8,0],[[2,2,16],[2,4,32],[4,2,16],[4,4,48]]",165));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,12,0,0],[[2,2,16],[2,4,32],[4,2,16],[4,4,32]]",166));//M2(R8_6)的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,3,15,144,15,16,[1,3,4,8,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,32]]",167));//R8_6×R8_6=R8R8_266的子环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,144,15,4,[1,3,4,8,0],[[2,2,16],[2,4,16],[4,2,32],[4,4,48]]",168));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,5,5,7,80,15,2,[1,5,10,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,64]]",169));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,1,8,2,3,7,64,7,16,[1,3,6,6,0],[[2,2,16],[2,4,56],[4,2,56],[4,4,64]]",170));//R8R4_438=R8_22×R4_9的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,3,3,7,72,15,4,[1,3,8,4,0],[[2,2,16],[2,4,56],[4,2,48],[4,4,64]]",174));//R8R4_386=R8_22×R4_8的子环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,3,3,7,72,7,4,[1,3,8,4,0],[[2,2,16],[2,4,48],[4,2,56],[4,4,64]]",177));//R8R4_334=R8_22×R4_7的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,1,8,2,3,3,48,7,16,[1,3,2,6,4],[[2,2,32],[2,4,56],[4,2,56],[4,4,64]]",179));//R4_3/([[2,0,0,1])的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,15,15,160,15,16,[1,7,8,0,0],[[2,4,32],[4,2,32],[4,4,32]]",180));//R8R4_11的商环、R4_3/([[0,0,2],[0,1,1],[2]],[0,0,1,1])	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,128,15,16,[1,3,8,4,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,48]]",183));//R8R4_219=R8_11×R4_5、M2(R8_12)的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,144,15,4,[1,3,8,4,0],[[2,2,16],[2,4,16],[4,2,32],[4,4,48]]",184));//R8R4_218=R8_10×R4_5的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,8,4,0],[[2,2,16],[2,4,16],[4,2,32],[4,4,32]]",185));//R8R4_217=R8_9×R4_5的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,144,15,4,[1,3,8,4,0],[[2,2,16],[2,4,32],[4,2,16],[4,4,48]]",186));//R8R4_216=R8_8×R4_5的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,8,4,0],[[2,2,16],[2,4,32],[4,2,16],[4,4,32]]",187));//R8R4_216=R8_8×R4_5的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,3,15,144,15,16,[1,3,4,8,0],[[2,2,16],[2,4,16],[4,2,16],[4,4,64]]",188));//R8R4_214=R8_6×R4_5的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,3,15,144,15,16,[1,3,8,4,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,32]]",189));//R8R4_214=R8_6×R4_5的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,144,15,4,[1,3,8,4,0],[[2,2,16],[2,4,32],[4,2,16],[4,4,48]]",190));//R8R8_444=R8_28×R4_9的子环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,1,12,6,3,3,72,11,4,[1,5,8,2,0],[[2,2,24],[2,4,44],[4,2,52],[4,4,64]]",191));//R8R4_153=R8_49×R4_3的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,1,12,6,3,3,72,11,4,[1,5,8,2,0],[[2,2,24],[2,4,52],[4,2,44],[4,4,64]]",192));//R8R4_153=R8_49×R4_3的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,1,8,2,3,7,64,7,16,[1,3,6,2,4],[[2,2,16],[2,4,56],[4,2,56],[4,4,64]]",193));//R8R4_150=R8_46×R4_3的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,144,15,4,[1,3,12,0,0],[[2,2,16],[2,4,16],[4,2,16],[4,4,64]]",194));//R8R4_70=R8_18×R4_2的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,144,15,4,[1,3,4,8,0],[[2,2,16],[2,4,16],[4,2,16],[4,4,64]]",195));//R8R4_61=R8_9×R4_2的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,160,15,4,[1,3,4,8,0],[[2,2,16],[2,4,16],[4,2,16],[4,4,48]]",196));//R8R4_18=R8_18×R4_1的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,160,15,16,[1,3,8,4,0],[[2,2,16],[2,4,16],[4,2,16],[4,4,48]]",197));//R8R4_12=R8_12×R4_1的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,5,7,7,104,7,2,[1,7,8,0,0],[[2,4,32],[4,2,56],[4,4,64]]",198));//[[0,0,0],[0,0,0],[2,0,0]],[[0,0,0],[0,0,0],[2,0,1]],[[0,0,0],[0,0,0],[2,2,0]]
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,160,15,4,[1,7,8,0,0],[[2,4,32],[4,2,32],[4,4,32]]",199));//[[0,0],[0,2]],[[2,1],[0,0]],[[2,2],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,192,15,16,[1,7,0,8,0],[[4,4,64]]",200));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,5,7,7,96,15,2,[1,7,8,0,0],[[2,4,48],[4,2,48],[4,4,64]]",201));		
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,7,128,15,4,[1,7,0,8,0],[[2,4,32],[4,2,32],[4,4,64]]",202));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,3,7,7,112,15,4,[1,7,4,4,0],[[2,4,32],[4,2,48],[4,4,64]]",203));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,3,7,7,112,15,4,[1,7,4,4,0],[[2,4,48],[4,2,32],[4,4,64]]",204));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,1,8,2,7,7,80,7,16,[1,7,2,6,0],[[2,4,56],[4,2,56],[4,4,64]]",205));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,15,15,256,15,16,[1,7,8,0,0],[]",206));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,192,15,16,[1,3,8,4,0],[[2,2,16],[2,4,16],[4,2,16],[4,4,16]]",207));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,2,7,7,192,15,16,[1,4,7,4,0],[[2,2,16],[2,4,16],[4,2,16],[4,4,16]]",208));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,3,7,7,160,15,4,[1,5,6,4,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,24]]",209));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,2,7,7,128,15,16,[1,4,6,3,2],[[2,2,32],[2,4,32],[4,2,32],[4,4,32]]",210));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,2,3,3,112,15,16,[1,2,5,4,4],[[2,2,36],[2,4,36],[4,2,36],[4,4,36]]",211));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,192,15,16,[1,7,8,0,0],[[4,4,64]]",212));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,3,15,144,15,16,[1,3,8,4,0],[[2,2,16],[2,4,16],[4,2,16],[4,4,64]]",213));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,2,3,7,144,15,16,[1,4,7,4,0],[[2,2,16],[2,4,16],[4,2,16],[4,4,64]]",214));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,3,3,7,120,15,4,[1,5,6,4,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,64]]",215));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,2,3,7,96,15,16,[1,4,6,3,2],[[2,2,32],[2,4,32],[4,2,32],[4,4,64]]",216));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,4,1,3,108,15,16,[1,4,5,6,0],[[2,2,28],[2,4,28],[4,2,28],[4,4,64]]",217));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,2,1,3,84,15,16,[1,2,5,4,4],[[2,2,36],[2,4,36],[4,2,36],[4,4,64]]",218));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,2,7,7,128,15,16,[1,7,2,6,0],[[2,4,32],[4,2,32],[4,4,64]]",219));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,2,3,7,96,15,16,[1,3,6,2,4],[[2,2,16],[2,4,40],[4,2,40],[4,4,64]]",220));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,4,3,3,96,15,16,[1,4,7,4,0],[[2,2,16],[2,4,40],[4,2,40],[4,4,64]]",221));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,6,3,3,80,11,4,[1,5,8,2,0],[[2,2,24],[2,4,44],[4,2,44],[4,4,64]]",222));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,6,3,3,80,15,4,[1,5,8,2,0],[[2,2,24],[2,4,44],[4,2,44],[4,4,64]]",223));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,4,3,3,144,15,16,[1,4,5,6,0],[[2,2,28],[2,4,28],[4,2,28],[4,4,28]]",224));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,1,12,4,3,3,64,11,16,[1,4,6,5,0],[[2,2,32],[2,4,48],[4,2,48],[4,4,64]]",225));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,1,14,8,1,1,72,13,16,[1,4,11,0,0],[[2,2,28],[2,4,46],[4,2,46],[4,4,64]]",226));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,1,10,4,1,1,56,9,16,[1,2,7,2,4],[[2,2,36],[2,4,50],[4,2,50],[4,4,64]]",227));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,12,0,0],[[2,2,16],[2,4,16],[4,2,32],[4,4,32]]",228));//R8_10×R8_24的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,3,7,80,15,16,[1,3,4,0,8],[[2,2,16],[2,4,48],[4,2,48],[4,4,64]]",229));//R32_3012/R2_1=R16_229、M4(R16_179)的子环:[[0,2,0,0],[0,1,0,0],[0,0,0,0],[0,0,0,0]],[[0,7,0,0],[0,3,0,0],[0,0,0,0],[0,0,1,1]]		
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,192,15,16,[1,3,12,0,0],[[2,2,16],[2,4,16],[4,2,16],[4,4,16]]",230));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,192,15,4,[1,7,8,0,0],[[2,4,32],[4,4,32]]",231));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,192,15,4,[1,3,12,0,0],[[2,2,16],[2,4,16],[4,2,16],[4,4,16]]",232));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,192,15,4,[1,7,8,0,0],[[4,2,32],[4,4,32]]",233));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,15,15,160,15,16,[1,7,8,0,0],[[2,4,32],[4,2,32],[4,4,32]]",234));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,160,15,16,[1,3,12,0,0],[[2,2,16],[2,4,16],[4,2,16],[4,4,48]]",235));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,160,15,4,[1,3,12,0,0],[[2,2,16],[2,4,16],[4,2,16],[4,4,48]]",236));		
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,2,3,7,80,15,16,[1,3,6,4,2],[[2,2,16],[2,4,48],[4,2,48],[4,4,64]]",237));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,7,128,15,16,[1,7,0,8,0],[[2,4,32],[4,2,32],[4,4,64]]",238));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,2,3,7,144,15,16,[1,2,7,6,0],[[2,2,28],[2,4,28],[4,2,28],[4,4,28]]",239));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,2,5,7,144,15,4,[1,4,7,4,0],[[2,2,16],[2,4,40],[4,2,16],[4,4,40]]",240));		
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,2,5,7,144,15,4,[1,2,7,6,0],[[2,2,28],[2,4,28],[4,2,28],[4,4,28]]",241));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,2,5,7,144,15,4,[1,4,7,4,0],[[2,2,16],[2,4,16],[4,2,40],[4,4,40]]",242));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,2,7,7,120,15,16,[1,4,7,4,0],[[2,2,16],[2,4,40],[4,2,40],[4,4,40]]",243));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,2,3,7,120,15,16,[1,2,7,6,0],[[2,2,28],[2,4,28],[4,2,28],[4,4,52]]",244));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,6,3,3,84,15,4,[1,4,11,0,0],[[2,2,16],[2,4,52],[4,2,40],[4,4,64]]",245));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,2,1,7,120,15,4,[1,2,7,6,0],[[2,2,28],[2,4,28],[4,2,28],[4,4,52]]",246));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,6,3,3,84,11,4,[1,4,11,0,0],[[2,2,16],[2,4,40],[4,2,52],[4,4,64]]",247));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,1,12,4,1,3,60,11,16,[1,2,7,6,0],[[2,2,28],[2,4,52],[4,2,52],[4,4,64]]",248));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,2,3,7,144,15,16,[1,4,3,4,4],[[2,2,16],[2,4,16],[4,2,16],[4,4,64]]",249));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,2,3,3,96,15,16,[1,4,3,4,4],[[2,2,16],[2,4,40],[4,2,40],[4,4,64]]",250));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,2,7,7,96,15,16,[1,7,2,4,2],[[2,4,48],[4,2,48],[4,4,64]]",251));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,160,15,16,[1,7,8,0,0],[[2,4,32],[4,2,32],[4,4,32]]",252));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,2,3,7,120,15,16,[1,4,7,4,0],[[2,2,16],[2,4,40],[4,2,40],[4,4,40]]",253));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,1,12,4,3,3,72,11,16,[1,4,7,4,0],[[2,2,16],[2,4,52],[4,2,52],[4,4,64]]",254));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,3,7,7,160,15,4,[1,5,6,4,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,24]]",255));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,3,3,7,120,15,4,[1,5,6,4,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,64]]",256));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,128,15,16,[1,7,0,8,0],[[2,4,32],[4,2,32],[4,4,64]]",257));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,7,0,8,0],[[4,2,32],[4,4,64]]",258));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,7,0,8,0],[[2,4,32],[4,4,64]]",259));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,7,128,15,16,[1,7,0,0,8],[[2,4,32],[4,2,32],[4,4,64]]",260));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,144,15,4,[1,7,4,4,0],[[2,4,32],[4,2,32],[4,4,48]]",261));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,144,15,4,[1,7,4,4,0],[[2,4,32],[4,2,32],[4,4,48]]",262));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,7,4,4,0],[[4,2,32],[4,4,64]]",263));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,192,15,4,[1,7,4,4,0],[[4,2,32],[4,4,32]]",264));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,3,7,7,88,7,4,[1,7,4,4,0],[[2,4,48],[4,2,56],[4,4,64]]",265));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,3,7,7,88,15,4,[1,7,4,4,0],[[2,4,56],[4,2,48],[4,4,64]]",266));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,128,15,16,[1,7,8,0,0],[[2,4,32],[4,2,32],[4,4,64]]",267));//[[2,0,0,2],[0,0,2,0],[0,2,0,2],[2,0,0,2]],[[3,0,0,3],[0,1,1,1],[1,1,1,2],[3,0,0,3]]		
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,7,4,4,0],[[2,4,32],[4,4,64]]",268));//[[0,0,0],[3,0,3],[2,0,2]],[[2,2,2],[2,2,2],[2,2,2]]
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,192,15,4,[1,7,4,4,0],[[2,4,32],[4,4,32]]",269));//[[0,0,0,0],[2,0,0,0],[3,2,0,0],[2,1,2,0]],[[0,0,0,0],[0,0,0,0],[2,2,2,2],[2,2,2,2]]
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,160,15,16,[1,7,0,8,0],[[2,4,32],[4,2,32],[4,4,32]]",270));//[[0,0,0,0],[2,0,0,0],[1,0,2,2],[0,3,0,2]],[[2,2,0,0],[0,2,0,0],[1,0,0,0],[0,3,2,0]]
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,1,14,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,24]]",271));//R8R4_61的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,5,10,0,0],[[2,2,16],[2,4,16],[4,2,32],[4,4,32]]",272));//R8R4_62的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,144,15,16,[1,3,12,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,40]]",273));//R8R4_63的商环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,144,15,4,[1,1,14,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,40]]",274));//R8R4_91、R16_153×R4_2=R16R4_543的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,12,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,24]]",275));//R8R4_217的商环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,144,15,4,[1,3,12,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,40]]",276));//R8R4_226的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,3,7,112,15,16,[1,3,4,8,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,64]]",277));//R8R4_232的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,5,7,7,104,15,2,[1,7,8,0,0],[[2,4,56],[4,2,32],[4,4,64]]",278));//Rppp(43,4)的商环
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,7,15,144,15,4,[1,7,8,0,0],[[2,2,112]]",282));//R16_153×R4_5=R16R4_1713的商环
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,7,15,160,15,4,[1,7,8,0,0],[[2,2,96]]",283));		
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,1,12,6,3,3,72,11,4,[1,8,7,0,0],[[2,2,184]]",284));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,5,7,112,15,1,[1,9,6,0,0],[[2,2,144]]",285));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,1,7,15,144,15,16,[1,7,8,0,0],[[2,2,112]]",286));//R8_31×R4_5=R8R4_239的商环	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,1,3,7,112,15,16,[1,3,4,8,0],[[2,2,144]]",287));//R8_33×R4_5=R8R4_241的商环	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,7,15,144,15,4,[1,7,8,0,0],[[2,2,112]]",288));//M2(R8_39)的子环
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,3,3,7,72,15,4,[1,5,6,4,0],[[2,2,184]]",289));//M2(R8_46)、R8_46×R8_36=R8R8_1866的子环
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,3,15,112,15,4,[1,3,12,0,0],[[2,2,144]]",290));//R8_31×R8_39=R8R8_2007的子环
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,3,15,128,15,4,[1,3,12,0,0],[[2,2,128]]",291));//M2(R8_39)的子环
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,3,15,144,15,4,[1,3,12,0,0],[[2,2,112]]",292));//R8_28×R8_39=R8R8_2004、M2(R8_39)的子环
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,8,2,7,7,80,7,16,[1,8,7,0,0],[[2,2,176]]",293));//I4:[[0,0],[0,3]],[[0,0],[3,0]],[[1,3],[0,1]]
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,11,15,160,15,4,[1,11,4,0,0],[[2,2,96]]",294));//I4:[[1,2],[1,2]],[[1,2],[2,1]],[[2,2],[1,1]]
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,1,12,10,3,3,72,11,2,[1,12,3,0,0],[[2,2,184]]",295));//I4:[[0,0],[0,1]],[[1,3],[3,0]]
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,3,7,7,88,15,4,[1,9,6,0,0],[[2,2,168]]",296));//[[0,0,0,0],[1,1,1,1],[1,0,1,0],[0,0,0,1]],[[0,0,0,1],[0,0,0,1],[0,0,0,1],[0,0,0,0]]
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,1,12,10,3,3,84,11,2,[1,12,3,0,0],[[2,2,172]]",297));//[[0,0,0],[0,0,0],[0,0,1]],[[0,0,1],[0,0,1],[0,0,1]],[[1,0,0],[0,1,1],[0,0,0]]	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,9,3,3,84,11,1,[1,11,4,0,0],[[2,2,172]]",298));//[[0,0,0,0],[0,0,0,0],[0,0,1,0],[0,1,0,1]],[[0,0,0,0],[0,0,0,1],[1,0,0,0],[0,0,0,1]]
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,8,2,3,7,56,7,16,[1,4,7,4,0],[[2,2,200]]",299));//I4:[[2,0],[0,2]],[[1,1],[0,2]]
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,1,10,8,3,3,58,9,2,[1,10,5,0,0],[[2,2,198]]",300));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,1,15,15,256,15,16,[1,15,0,0,0],[]",301));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,10,4,1,1,56,9,16,[1,4,5,4,2],[[2,2,200]]",302));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,1,7,15,192,15,16,[1,7,8,0,0],[[2,2,64]]",303));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,1,3,15,144,15,16,[1,3,12,0,0],[[2,2,112]]",304));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,2,7,7,192,15,16,[1,8,7,0,0],[[2,2,64]]",305));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,2,3,7,144,15,16,[1,4,7,4,0],[[2,2,112]]",306));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,4,3,3,144,15,16,[1,6,9,0,0],[[2,2,112]]",307));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,3,7,7,160,15,4,[1,9,6,0,0],[[2,2,96]]",308));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,3,3,7,120,15,4,[1,5,6,4,0],[[2,2,136]]",309));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,6,3,3,120,15,4,[1,8,7,0,0],[[2,2,136]]",310));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,9,3,3,100,11,1,[1,11,4,0,0],[[2,2,156]]",311));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,9,3,3,100,15,1,[1,11,4,0,0],[[2,2,156]]",312));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,9,3,3,100,15,1,[1,11,4,0,0],[[2,2,156]]",313));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,2,7,7,128,15,16,[1,8,4,3,0],[[2,2,128]]",314));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,2,3,7,96,15,16,[1,4,6,3,2],[[2,2,160]]",315));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,4,3,3,96,15,16,[1,6,7,2,0],[[2,2,160]]",316));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,6,3,3,80,11,4,[1,8,6,1,0],[[2,2,176]]",317));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,6,3,3,80,15,4,[1,8,6,1,0],[[2,2,176]]",318));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,4,1,3,108,15,16,[1,4,5,6,0],[[2,2,148]]",319));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,8,1,1,108,15,16,[1,8,7,0,0],[[2,2,148]]",320));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,12,1,1,90,13,4,[1,12,3,0,0],[[2,2,166]]",321));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,12,1,1,90,15,4,[1,12,3,0,0],[[2,2,166]]",322));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,14,8,1,1,72,13,16,[1,8,7,0,0],[[2,2,184]]",323));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,2,3,3,112,15,16,[1,4,5,6,0],[[2,2,144]]",324));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,2,1,3,84,15,16,[1,2,5,4,4],[[2,2,172]]",325));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,4,1,1,84,15,16,[1,4,5,4,2],[[2,2,172]]",326));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,6,1,1,70,9,4,[1,6,3,6,0],[[2,2,186]]",327));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,6,1,1,70,15,4,[1,6,3,6,0],[[2,2,186]]",328));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,2,7,7,96,15,16,[1,8,4,3,0],[[2,2,160]]",329));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,6,3,3,104,15,4,[1,8,6,1,0],[[2,2,152]]",330));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,2,1,1,60,15,16,[1,2,1,6,6],[[2,2,196]]",331));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,12,4,3,3,72,11,16,[1,6,9,0,0],[[2,2,184]]",332));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,1,15,15,160,15,16,[1,15,0,0,0],[[2,2,96]]",333));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,1,7,15,160,15,16,[1,7,8,0,0],[[2,2,96]]",334));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,7,7,128,15,2,[1,11,4,0,0],[[2,2,128]]",335));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,3,7,7,112,15,4,[1,9,4,2,0],[[2,2,144]]",336));		
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,3,15,160,15,4,[1,3,12,0,0],[[2,2,96]]",337));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,2,7,7,120,15,16,[1,8,7,0,0],[[2,2,136]]",338));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,2,3,7,120,15,16,[1,4,7,4,0],[[2,2,136]]",339));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,10,3,3,96,15,2,[1,12,3,0,0],[[2,2,160]]",340));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,6,3,3,84,11,4,[1,8,7,0,0],[[2,2,172]]",341));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,2,1,7,120,15,4,[1,2,7,6,0],[[2,2,136]]",342));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,11,15,192,15,4,[1,11,4,0,0],[[2,2,64]]",343));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,2,5,7,144,15,4,[1,6,7,2,0],[[2,2,112]]",344));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,1,7,7,128,15,16,[1,7,0,8,0],[[2,2,128]]",345));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,2,3,3,96,15,16,[1,4,3,4,4],[[2,2,160]]",346));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,6,3,3,84,15,4,[1,8,7,0,0],[[2,2,172]]",347));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,2,3,7,80,15,16,[1,4,6,3,2],[[2,2,176]]",348));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,7,7,144,15,2,[1,11,4,0,0],[[2,2,112]]",349));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,10,3,3,108,11,2,[1,12,3,0,0],[[2,2,148]]",350));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,10,3,3,108,15,2,[1,12,3,0,0],[[2,2,148]]",351));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,3,7,7,112,15,4,[1,9,4,2,0],[[2,2,144]]",352));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,7,7,144,15,2,[1,11,4,0,0],[[2,2,112]]",353));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,3,7,7,160,15,4,[1,9,6,0,0],[[2,2,96]]",354));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,3,3,7,120,15,4,[1,5,6,4,0],[[2,2,136]]",355));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,6,3,3,120,15,4,[1,8,7,0,0],[[2,2,136]]",356));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,7,15,160,15,4,[1,7,8,0,0],[[2,2,96]]",357));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,7,7,96,15,2,[1,11,4,0,0],[[2,2,160]]",358));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,3,7,7,88,7,4,[1,9,6,0,0],[[2,2,168]]",359));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,3,3,7,72,7,4,[1,5,6,4,0],[[2,2,184]]",360));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,1,7,15,128,15,16,[1,7,8,0,0],[[2,2,128]]",361));//R2_2/([[0,0,0,1],[0,0,1]],[0,0,0,0,0,0,1])
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,1,3,15,112,15,16,[1,3,12,0,0],[[2,2,144]]",362));//R2_2/([[0,0,1,1,1],[0,1,0,0,1]],[0,1,1,0,1,0,1,1])
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,7,7,128,15,4,[1,7,0,8,0],[[2,2,128]]",363));//I4:[[3,3],[0,0]],[[3,1],[3,3]]
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,1,4,2,3,3,40,3,2,[1,4,11,0,0],[[2,2,216]]",364));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,9,3,3,84,15,1,[1,11,4,0,0],[[2,2,172]]",365));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,8,2,7,7,56,7,16,[1,8,7,0,0],[[2,2,200]]",366));		
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,1,3,7,80,15,16,[1,3,4,0,8],[[2,2,176]]",367));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,5,7,80,15,2,[1,9,6,0,0],[[2,2,176]]",368));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,9,3,3,92,15,1,[1,11,4,0,0],[[2,2,164]]",369));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,5,7,88,15,1,[1,9,6,0,0],[[2,2,168]]",370));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,1,12,6,3,3,72,11,4,[1,8,7,0,0],[[2,2,184]]",371));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,9,3,3,92,11,1,[1,11,4,0,0],[[2,2,164]]",372));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,5,7,88,7,1,[1,9,6,0,0],[[2,2,168]]",373));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,1,8,2,5,7,64,7,4,[1,6,7,2,0],[[2,2,192]]",374));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,5,7,112,15,1,[1,9,6,0,0],[[2,2,144]]",375));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,5,7,112,15,2,[1,9,6,0,0],[[2,2,144]]",376));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,8,2,3,7,64,7,16,[1,4,7,4,0],[[2,2,192]]",377));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,3,3,76,15,1,[1,7,8,0,0],[[2,2,180]]",378));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,11,15,144,15,4,[1,11,4,0,0],[[2,2,112]]",379));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,1,14,12,1,1,78,13,4,[1,12,3,0,0],[[2,2,178]]",380));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,3,3,76,3,1,[1,7,8,0,0],[[2,2,180]]",381));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,12,4,1,3,60,11,16,[1,4,5,6,0],[[2,2,196]]",382));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,4,2,3,3,40,3,16,[1,4,5,0,6],[[2,2,216]]",383));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,12,4,3,3,64,11,16,[1,6,7,2,0],[[2,2,192]]",384));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,8,2,3,3,48,7,16,[1,4,3,4,4],[[2,2,208]]",385));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,15,16,0,0,81,14,16,[1,15,0,0,0],[[2,2,175]]",386));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,13,8,0,0,63,12,16,[1,7,2,6,0],[[2,2,193]]",387));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,9,4,0,0,45,8,16,[1,3,0,6,6],[[2,2,211]]",388));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,7,4,0,0,49,6,16,[1,3,8,4,0],[[2,2,207]]",389));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,1,2,0,0,31,0,16,[1,1,2,0,12],[[2,2,225]]",390));
	//R16
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,5,8]],[[2,1,7],[4,4,21],[4,5,20],[8,26,24]]",288));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,5,8]],[[2,1,7],[4,4,9],[4,5,20],[8,28,36],[8,32,36],[8,39,12]]",282));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,5,8]],[[2,1,7],[4,4,21],[4,5,20],[8,26,24]]",357));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,5,8]],[[2,1,7],[4,4,9],[4,5,20],[8,26,24],[8,28,36],[8,32,12],[8,39,12]]",283));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,5,12],[8,16,12],[8,26,12]]",164));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,6,12],[8,9,12],[8,10,24],[8,12,12],[8,18,12],[8,26,12]]",228));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,5,12],[8,16,12],[8,26,12]]",166));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,6,12],[8,8,24],[8,9,12],[8,12,12],[8,18,12],[8,26,12]]",156));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,8],[4,5,4]],[[2,1,3],[4,1,20],[4,4,3],[4,5,10],[8,5,24],[8,26,12]]",232));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,8],[4,5,4]],[[2,1,3],[4,1,20],[4,4,3],[4,5,10],[8,5,24],[8,9,48],[8,26,12]]",152));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,8]],[[2,1,7],[4,1,20],[4,4,21],[8,5,24]]",234));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,8]],[[2,1,7],[4,1,20],[4,4,21],[8,5,24],[8,11,48]]",180));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,4],[4,6,2],[8,7,4]],[[2,1,3],[2,2,2],[4,1,10],[4,4,3],[4,6,8],[4,7,3],[8,5,12],[8,7,34],[8,29,9]]",209));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,4],[4,6,2],[8,7,4]],[[2,1,3],[2,2,2],[4,1,10],[4,4,3],[4,6,8],[4,8,3],[8,5,12],[8,7,34],[8,34,9]]",255));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,2,4],[4,6,2],[8,17,4]],[[2,1,3],[2,2,2],[4,2,10],[4,4,3],[4,6,8],[4,7,3],[8,16,12],[8,17,34],[8,29,9]]",215));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,2,4],[4,6,2],[8,17,4]],[[2,1,3],[2,2,2],[4,2,10],[4,4,3],[4,6,8],[4,8,3],[8,16,12],[8,17,34],[8,34,9]]",256));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,3],[4,6,3],[4,11,2],[8,42,2],[8,51,2]],[[2,1,1],[2,2,3],[4,6,12],[4,10,3],[4,11,5],[8,40,9],[8,42,17],[8,51,17]]",326));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,8],[4,6,4]],[[2,1,3],[2,2,8],[4,4,3],[4,6,16],[4,7,24],[4,10,12],[8,29,18],[8,41,24]]",311));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,8],[4,6,4]],[[2,1,3],[2,2,8],[4,4,3],[4,6,16],[4,7,9],[4,8,9],[4,10,12],[8,29,9],[8,34,9],[8,36,6],[8,41,12],[8,47,12]]",312));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,8],[4,6,4]],[[2,1,3],[2,2,8],[4,4,3],[4,6,16],[4,8,24],[4,10,12],[8,34,18],[8,47,24]]",313));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,6,2],[4,9,2],[8,35,2]],[[2,1,7],[2,2,2],[4,4,21],[4,6,8],[4,7,3],[4,9,8],[8,29,9],[8,35,26],[8,37,9]]",336));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,6,2],[4,9,2],[8,35,2]],[[2,1,7],[2,2,2],[4,4,21],[4,6,8],[4,8,3],[4,9,8],[8,34,9],[8,35,26],[8,44,9]]",352));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,4],[4,6,4]],[[2,1,7],[2,2,4],[4,4,21],[4,6,16],[4,7,18],[8,29,54]]",349));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,4],[4,6,4]],[[2,1,7],[2,2,4],[4,4,21],[4,6,16],[4,8,18],[8,34,54]]",353));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,6,6]],[[2,1,7],[2,2,2],[4,4,21],[4,6,24],[4,7,3],[8,27,18],[8,29,27]]",308));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,6,6]],[[2,1,7],[2,2,2],[4,4,21],[4,6,24],[4,8,3],[8,27,18],[8,34,27]]",354));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,5,4],[4,6,2],[8,38,4]],[[2,1,3],[2,2,2],[4,4,3],[4,5,10],[4,6,8],[4,7,3],[8,26,12],[8,29,9],[8,38,34]]",309));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,5,4],[4,6,2],[8,38,4]],[[2,1,3],[2,2,2],[4,4,3],[4,5,10],[4,6,8],[4,8,3],[8,26,12],[8,34,9],[8,38,34]]",355));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,6,7]],[[2,1,3],[2,2,5],[4,4,3],[4,6,28],[4,7,6],[4,10,6],[8,27,9],[8,29,18],[8,40,18],[8,41,6]]",310));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,6,7]],[[2,1,3],[2,2,5],[4,4,3],[4,6,28],[4,8,6],[4,10,6],[8,27,9],[8,34,18],[8,40,18],[8,47,6]]",356));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[2,2,4],[4,5,2],[4,6,4]],[[2,1,5],[2,2,4],[4,4,6],[4,5,5],[4,6,16],[4,8,18],[8,28,12],[8,34,18]]",285));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[2,2,4],[4,5,2],[4,6,4]],[[2,1,5],[2,2,4],[4,4,6],[4,5,5],[4,6,16],[4,7,18],[8,28,12],[8,29,18]]",375));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,6,2],[4,9,5]],[[2,1,3],[2,2,5],[4,4,3],[4,6,8],[4,7,3],[4,8,3],[4,9,20],[4,10,6],[8,34,9],[8,37,9],[8,45,9],[8,48,18],[8,49,6]]",284));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,6,2],[4,9,5]],[[2,1,3],[2,2,5],[4,4,3],[4,6,8],[4,7,3],[4,8,3],[4,9,20],[4,10,6],[8,29,9],[8,44,9],[8,45,9],[8,48,18],[8,49,6]]",371));	
#ifdef R16_I1I2
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[4,5,4],[8,23,4]],[[2,1,3],[4,2,10],[4,4,3],[4,5,10],[8,16,12],[8,23,22],[8,26,12]]",147));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,23,8]],[[2,1,3],[4,4,3],[4,5,10],[8,23,44],[8,26,12]]",165));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,23,8]],[[2,1,3],[4,4,3],[4,5,10],[8,23,44],[8,26,12]]",168));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[4,3,8],[4,5,2]],[[2,1,5],[4,3,20],[4,4,6],[4,5,5],[8,15,24],[8,20,24],[8,28,12]]",169));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,3,2],[8,21,2],[8,33,4]],[[2,1,3],[4,3,5],[4,4,3],[8,21,17],[8,33,22]]",179));		
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,6],[4,2,2],[4,5,6]],[[2,1,1],[4,1,15],[4,2,5],[4,5,15],[8,9,36],[8,12,36],[8,39,12]]",274));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,5,8]],[[2,1,7],[4,4,9],[4,5,20],[8,28,36],[8,32,36],[8,39,12]]",282));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,5,8]],[[2,1,7],[4,4,9],[4,5,20],[8,26,36],[8,31,12],[8,32,36]]",286));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,33,8]],[[2,1,3],[4,4,3],[4,5,10],[8,26,12],[8,33,44]]",287));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,5,4],[4,9,2],[8,46,4]],[[2,1,3],[2,2,2],[4,4,3],[4,5,10],[4,8,3],[4,9,8],[8,26,12],[8,44,9],[8,46,34]]",289));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,12]],[[2,1,3],[4,4,3],[4,5,30],[8,26,36]]",290));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[4,1,6],[4,2,2],[4,5,2]],[[2,1,5],[4,1,15],[4,2,5],[4,4,6],[4,5,5],[8,5,12],[8,9,12],[8,10,24],[8,11,12],[8,12,12],[8,28,12]]",126));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[4,1,6],[4,2,2],[4,5,2]],[[2,1,5],[4,1,15],[4,2,5],[4,4,6],[4,5,5],[8,5,12],[8,8,24],[8,9,12],[8,11,12],[8,12,12],[8,28,12]]",127));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,5,8]],[[2,1,7],[4,4,9],[4,5,20],[8,26,24],[8,28,36],[8,32,12],[8,39,12]]",283));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,5,4],[4,9,3],[8,46,4]],[[2,1,3],[2,2,1],[4,4,3],[4,5,10],[4,9,12],[8,26,12],[8,45,9],[8,46,34]]",377));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,2],[4,2,2],[4,5,2],[8,1,4],[8,4,4]],[[2,1,1],[4,1,5],[4,2,5],[4,5,5],[8,1,22],[8,4,22],[8,6,12]]",29));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,2],[4,2,2],[4,5,2],[8,2,8]],[[2,1,1],[4,1,5],[4,2,5],[4,5,5],[8,2,44],[8,6,12]]",30));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,2],[4,2,2],[4,5,2],[8,1,4],[8,4,4]],[[2,1,1],[4,1,5],[4,2,5],[4,5,5],[8,1,22],[8,4,22],[8,6,12]]",31));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,2],[4,2,2],[4,5,2],[8,3,4]],[[2,1,1],[4,1,5],[4,2,5],[4,5,5],[8,3,22],[8,12,12]]",32));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,12]],[[2,1,3],[4,2,30],[4,4,3],[8,16,36]]",70));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,5,4],[8,23,4]],[[2,1,3],[4,1,10],[4,4,3],[4,5,10],[8,5,12],[8,23,22],[8,26,12]]",190));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,23,8]],[[2,1,3],[4,4,3],[4,5,10],[8,23,44],[8,26,12]]",153));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[8,23,8]],[[2,1,7],[4,4,21],[8,23,44]]",155));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,23,8]],[[2,1,3],[4,4,3],[4,5,10],[8,23,44],[8,26,12]]",159));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,23,8]],[[2,1,3],[4,4,3],[4,5,10],[8,23,44],[8,26,12]]",163));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,8],[4,5,4]],[[2,1,3],[4,2,20],[4,4,3],[4,5,10],[8,16,24],[8,26,12]]",128));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,23,8]],[[2,1,3],[4,4,3],[4,5,10],[8,23,44],[8,26,12]]",148));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,23,8]],[[2,1,3],[4,4,3],[4,5,10],[8,23,44],[8,26,12]]",149));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,5,4],[8,23,4]],[[2,1,3],[4,1,10],[4,4,3],[4,5,10],[8,5,12],[8,23,22],[8,26,12]]",160));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,5,12],[8,16,12],[8,26,12]]",162));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,23,8]],[[2,1,3],[4,4,3],[4,5,10],[8,23,44],[8,26,12]]",167));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,5,4],[8,23,4]],[[2,1,3],[4,1,10],[4,4,3],[4,5,10],[8,5,12],[8,23,22],[8,26,12]]",183));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,2],[8,1,4]],[[2,1,1],[4,1,5],[8,1,22]]",1));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,2],[8,4,4]],[[2,1,1],[4,1,5],[8,4,22]]",2));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,2],[8,2,4]],[[2,1,1],[4,1,5],[8,2,22]]",3));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,2],[8,1,4]],[[2,1,1],[4,1,5],[8,1,22]]",4));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,2],[8,1,4]],[[2,1,1],[4,1,5],[8,1,22]]",5));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,1],[4,2,2],[4,6,1],[8,3,8],[8,17,2]],[[2,1,1],[2,2,1],[4,2,5],[4,6,4],[8,3,44],[8,17,17]]",6));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[8,3,4]],[[2,1,3],[4,2,10],[4,4,3],[8,3,22],[8,16,12]]",7));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12]]",8));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,4,8]],[[2,1,3],[4,1,10],[4,4,3],[8,4,44],[8,5,12]]",9));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,2,8]],[[2,1,3],[4,1,10],[4,4,3],[8,2,44],[8,5,12]]",10));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[8,3,4]],[[2,1,3],[4,2,10],[4,4,3],[8,3,22],[8,16,12]]",11));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,1,8]],[[2,1,3],[4,1,10],[4,4,3],[8,1,44],[8,5,12]]",12));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,1],[4,1,2],[4,6,1],[8,1,4],[8,7,2]],[[2,1,1],[2,2,1],[4,1,5],[4,6,4],[8,1,22],[8,7,17]]",13));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,1],[4,1,2],[4,6,1],[8,2,4],[8,7,2]],[[2,1,1],[2,2,1],[4,1,5],[4,6,4],[8,2,22],[8,7,17]]",14));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,1],[4,1,2],[4,6,1],[8,4,4],[8,7,2]],[[2,1,1],[2,2,1],[4,1,5],[4,6,4],[8,4,22],[8,7,17]]",15));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,1,8]],[[2,1,3],[4,1,10],[4,4,3],[8,1,44],[8,5,12]]",16));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12]]",17));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,1,4],[8,4,4]],[[2,1,3],[4,1,10],[4,4,3],[8,1,22],[8,4,22],[8,5,12]]",18));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,2,8]],[[2,1,3],[4,1,10],[4,4,3],[8,2,44],[8,5,12]]",19));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[8,3,8]],[[2,1,3],[4,2,10],[4,4,3],[8,3,44],[8,16,12]]",20));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,1,4],[8,4,4]],[[2,1,3],[4,1,10],[4,4,3],[8,1,22],[8,4,22],[8,5,12]]",21));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12]]",22));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[8,3,8]],[[2,1,3],[4,2,10],[4,4,3],[8,3,44],[8,16,12]]",23));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,2,8]],[[2,1,3],[4,1,10],[4,4,3],[8,2,44],[8,5,12]]",24));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,2,8]],[[2,1,3],[4,1,10],[4,4,3],[8,2,44],[8,5,12]]",25));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,4,8]],[[2,1,3],[4,1,10],[4,4,3],[8,4,44],[8,5,12]]",26));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12]]",27));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[8,23,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[8,5,12],[8,16,12],[8,23,22]]",71));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,8],[8,23,4]],[[2,1,3],[4,1,20],[4,4,3],[8,5,24],[8,23,22]]",72));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[8,23,8]],[[2,1,3],[4,2,10],[4,4,3],[8,16,12],[8,23,44]]",73));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[8,23,8]],[[2,1,3],[4,2,10],[4,4,3],[8,16,12],[8,23,44]]",74));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[8,23,8]],[[2,1,3],[4,2,10],[4,4,3],[8,16,12],[8,23,44]]",75));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[8,23,8]],[[2,1,3],[4,2,10],[4,4,3],[8,16,12],[8,23,44]]",76));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,23,8]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12],[8,23,44]]",77));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[8,23,12]],[[2,1,3],[4,4,3],[8,23,66]]",78));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[8,23,12]],[[2,1,3],[4,4,3],[8,23,66]]",79));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[8,23,12]],[[2,1,3],[4,4,3],[8,23,66]]",80));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[8,23,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[8,5,12],[8,16,12],[8,23,22]]",81));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,8],[8,23,4]],[[2,1,3],[4,1,20],[4,4,3],[8,5,24],[8,23,22]]",82));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,8],[4,2,4]],[[2,1,3],[4,1,20],[4,2,10],[4,4,3],[8,5,24],[8,16,12]]",83));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,23,8]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12],[8,23,44]]",84));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,23,8]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12],[8,23,44]]",85));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,8],[4,2,4]],[[2,1,3],[4,1,20],[4,2,10],[4,4,3],[8,5,24],[8,16,12]]",86));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,8],[8,23,4]],[[2,1,3],[4,2,20],[4,4,3],[8,16,24],[8,23,22]]",87));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,8]],[[2,1,3],[4,1,10],[4,2,20],[4,4,3],[8,5,12],[8,16,24]]",88));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[8,23,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[8,5,12],[8,16,12],[8,23,22]]",89));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,8],[8,23,4]],[[2,1,3],[4,2,20],[4,4,3],[8,16,24],[8,23,22]]",90));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[8,23,12]],[[2,1,3],[4,4,3],[8,23,66]]",93));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[8,23,8]],[[2,1,3],[4,2,10],[4,4,3],[8,16,12],[8,23,44]]",94));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,23,8]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12],[8,23,44]]",95));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,2],[4,2,2],[4,3,8]],[[2,1,3],[4,1,5],[4,2,5],[4,3,20],[4,4,3],[8,8,12],[8,20,24]]",101));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,2],[4,2,2],[4,3,8]],[[2,1,3],[4,1,5],[4,2,5],[4,3,20],[4,4,3],[8,10,12],[8,15,24]]",102));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,12]],[[2,1,3],[4,2,30],[4,4,3],[8,16,36]]",103));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,3,6],[8,13,4],[8,21,2]],[[2,1,3],[4,3,15],[4,4,3],[8,13,34],[8,21,17]]",104));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,3,2],[8,21,2],[8,24,4]],[[2,1,3],[4,3,5],[4,4,3],[8,21,17],[8,24,22]]",105));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,3,2],[8,21,2]],[[2,1,3],[4,3,5],[4,4,3],[8,21,17]]",106));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,12]],[[2,1,3],[4,1,30],[4,4,3],[8,5,36]]",107));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,8]],[[2,1,3],[4,1,10],[4,2,20],[4,4,3],[8,5,12],[8,16,24]]",108));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4]],[[2,1,3],[4,2,10],[4,4,3],[8,16,12]]",109));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,3,2],[8,21,2]],[[2,1,3],[4,1,10],[4,3,5],[4,4,3],[8,11,12],[8,21,17]]",110));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12]]",111));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,3,2],[8,13,2]],[[2,1,3],[4,1,10],[4,3,5],[4,4,3],[8,5,12],[8,13,17]]",112));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[4,3,2],[8,13,2]],[[2,1,3],[4,2,10],[4,3,5],[4,4,3],[8,13,17],[8,16,12]]",113));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12]]",114));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[4,3,2],[8,21,2]],[[2,1,3],[4,2,10],[4,3,5],[4,4,3],[8,19,12],[8,21,17]]",115));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[8,23,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[8,5,12],[8,16,12],[8,23,22]]",116));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,23,8]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12],[8,23,44]]",117));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[8,23,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[8,5,12],[8,16,12],[8,23,22]]",118));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[8,23,8]],[[2,1,3],[4,2,10],[4,4,3],[8,16,12],[8,23,44]]",119));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,12]],[[2,1,3],[4,1,30],[4,4,3],[8,5,36]]",120));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,8],[4,5,4]],[[2,1,3],[4,2,20],[4,4,3],[4,5,10],[8,16,24],[8,26,12]]",129));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[4,1,6],[4,2,2],[4,5,2]],[[2,1,5],[4,1,15],[4,2,5],[4,4,6],[4,5,5],[8,5,24],[8,6,12],[8,8,12],[8,9,12],[8,10,12],[8,28,12]]",130));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,8]],[[2,1,7],[4,1,20],[4,4,9],[8,5,36],[8,11,36],[8,31,12]]",131));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,5,12],[8,6,24],[8,12,24],[8,19,12],[8,32,12]]",132));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,2],[4,2,6],[4,5,6]],[[2,1,1],[4,1,5],[4,2,15],[4,5,15],[8,6,36],[8,18,36],[8,39,12]]",133));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,24,4],[8,33,4]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12],[8,24,22],[8,33,22]]",134));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[4,1,2],[4,2,6],[4,5,2]],[[2,1,5],[4,1,5],[4,2,15],[4,4,6],[4,5,5],[8,6,12],[8,8,24],[8,16,12],[8,18,12],[8,19,12],[8,28,12]]",135));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[8,24,4],[8,33,4]],[[2,1,3],[4,2,10],[4,4,3],[8,16,12],[8,24,22],[8,33,22]]",136));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[4,1,2],[4,2,6],[4,5,2]],[[2,1,5],[4,1,5],[4,2,15],[4,4,6],[4,5,5],[8,8,12],[8,10,12],[8,12,12],[8,16,24],[8,18,12],[8,28,12]]",137));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,2,8]],[[2,1,7],[4,2,20],[4,4,9],[8,16,36],[8,19,36],[8,31,12]]",138));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[4,1,2],[4,2,6],[4,5,2]],[[2,1,5],[4,1,5],[4,2,15],[4,4,6],[4,5,5],[8,8,12],[8,10,12],[8,12,12],[8,18,12],[8,19,24],[8,28,12]]",139));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,6,24],[8,11,12],[8,12,24],[8,19,12],[8,26,12]]",140));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,8],[4,5,4]],[[2,1,3],[4,1,20],[4,4,3],[4,5,10],[8,5,24],[8,26,12]]",150));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[4,5,4],[8,23,4]],[[2,1,3],[4,2,10],[4,4,3],[4,5,10],[8,16,12],[8,23,22],[8,26,12]]",151));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,8],[4,5,4]],[[2,1,3],[4,1,20],[4,4,3],[4,5,10],[8,5,24],[8,9,48],[8,26,12]]",152));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,4],[4,2,4]],[[2,1,7],[4,1,10],[4,2,10],[4,4,21],[8,5,12],[8,16,12]]",154));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,6,12],[8,8,24],[8,9,12],[8,12,12],[8,18,12],[8,26,12]]",156));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,5,12],[8,16,12],[8,26,12]]",157));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[4,5,4],[8,23,4]],[[2,1,3],[4,2,10],[4,4,3],[4,5,10],[8,16,12],[8,23,22],[8,26,12]]",158));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,5,12],[8,16,12],[8,26,12]]",161));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,5,12],[8,16,12],[8,26,12]]",164));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,5,12],[8,16,12],[8,26,12]]",166));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,3,2],[4,5,4],[8,21,2],[8,22,4]],[[2,1,3],[4,3,5],[4,4,3],[4,5,10],[8,21,17],[8,22,34],[8,26,12]]",170));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,3,4],[4,5,4],[8,22,4]],[[2,1,3],[4,3,10],[4,4,3],[4,5,10],[8,15,12],[8,22,34],[8,26,12]]",174));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,3,4],[4,5,4],[8,22,4]],[[2,1,3],[4,3,10],[4,4,3],[4,5,10],[8,20,12],[8,22,34],[8,26,12]]",177));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[4,5,4],[8,23,4]],[[2,1,3],[4,2,10],[4,4,3],[4,5,10],[8,16,12],[8,23,22],[8,26,12]]",184));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,5,4],[8,23,4]],[[2,1,3],[4,1,10],[4,4,3],[4,5,10],[8,5,12],[8,23,22],[8,26,12]]",185));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[4,5,4],[8,23,4]],[[2,1,3],[4,2,10],[4,4,3],[4,5,10],[8,16,12],[8,23,22],[8,26,12]]",186));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,5,4],[8,23,4]],[[2,1,3],[4,1,10],[4,4,3],[4,5,10],[8,5,12],[8,23,22],[8,26,12]]",187));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,23,8]],[[2,1,3],[4,4,3],[4,5,10],[8,23,44],[8,26,12]]",188));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[4,5,4],[8,23,4]],[[2,1,3],[4,2,10],[4,4,3],[4,5,10],[8,16,12],[8,23,22],[8,26,12]]",189));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,3,6],[4,6,2],[8,21,2]],[[2,1,3],[2,2,2],[4,3,15],[4,4,3],[4,6,8],[4,8,3],[8,14,24],[8,20,12],[8,21,17],[8,34,9]]",191));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,3,6],[4,6,2],[8,21,2]],[[2,1,3],[2,2,2],[4,3,15],[4,4,3],[4,6,8],[4,7,3],[8,14,24],[8,15,12],[8,21,17],[8,29,9]]",192));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,3,2],[4,5,4],[8,21,2]],[[2,1,3],[4,3,5],[4,4,3],[4,5,10],[8,21,17],[8,26,12]]",193));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,8],[4,5,4]],[[2,1,3],[4,2,20],[4,4,3],[4,5,10],[8,16,24],[8,26,12]]",194));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,23,8]],[[2,1,3],[4,4,3],[4,5,10],[8,23,44],[8,26,12]]",195));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,23,8]],[[2,1,3],[4,4,3],[4,5,10],[8,23,44],[8,26,12]]",196));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,5,4],[8,23,4]],[[2,1,3],[4,1,10],[4,4,3],[4,5,10],[8,5,12],[8,23,22],[8,26,12]]",197));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,3,8]],[[2,1,7],[4,3,20],[4,4,21],[8,20,72]]",198));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,4],[4,2,4]],[[2,1,7],[4,1,10],[4,2,10],[4,4,21],[8,8,24],[8,10,24],[8,11,12],[8,19,12]]",199));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[8,23,8]],[[2,1,7],[4,4,21],[8,23,44]]",200));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,3,8]],[[2,1,7],[4,3,20],[4,4,21],[8,15,24],[8,20,24]]",201));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[8,24,8]],[[2,1,7],[4,4,21],[8,24,44]]",202));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,3,4],[8,13,4]],[[2,1,7],[4,3,10],[4,4,21],[8,13,34],[8,20,12]]",203));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,3,4],[8,13,4]],[[2,1,7],[4,3,10],[4,4,21],[8,13,34],[8,15,12]]",204));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,3,2],[8,21,6]],[[2,1,7],[4,3,5],[4,4,21],[8,21,51]]",205));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,8]],[[2,1,7],[4,1,20],[4,4,21],[8,5,72]]",206));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,5,4],[8,23,4]],[[2,1,3],[4,1,10],[4,4,3],[4,5,10],[8,5,12],[8,23,22],[8,26,12]]",207));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,1,4],[4,6,3],[8,7,4]],[[2,1,3],[2,2,1],[4,1,10],[4,4,3],[4,6,12],[8,5,12],[8,7,34],[8,27,9]]",208));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,4],[4,6,2],[8,7,4]],[[2,1,3],[2,2,2],[4,1,10],[4,4,3],[4,6,8],[4,7,3],[8,5,12],[8,7,34],[8,29,9]]",209));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,1,4],[4,6,1],[4,9,1],[8,7,2],[8,35,1]],[[2,1,3],[2,2,1],[4,1,10],[4,4,3],[4,6,4],[4,9,4],[8,5,12],[8,7,17],[8,35,13]]",210));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,1],[4,1,2],[4,6,1],[4,11,2],[8,7,2],[8,42,2]],[[2,1,1],[2,2,1],[4,1,5],[4,6,4],[4,11,5],[8,7,17],[8,42,17]]",211));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,2,8]],[[2,1,7],[4,2,20],[4,4,21],[8,16,72]]",212));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[4,5,4],[8,23,4]],[[2,1,3],[4,2,10],[4,4,3],[4,5,10],[8,16,12],[8,23,22],[8,26,12]]",213));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,2,4],[4,6,3],[8,17,4]],[[2,1,3],[2,2,1],[4,2,10],[4,4,3],[4,6,12],[8,16,12],[8,17,34],[8,27,9]]",214));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,2,4],[4,6,2],[8,17,4]],[[2,1,3],[2,2,2],[4,2,10],[4,4,3],[4,6,8],[4,7,3],[8,16,12],[8,17,34],[8,29,9]]",215));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,2,4],[4,6,1],[4,9,1],[8,17,2],[8,35,1]],[[2,1,3],[2,2,1],[4,2,10],[4,4,3],[4,6,4],[4,9,4],[8,16,12],[8,17,17],[8,35,13]]",216));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,3],[4,2,2],[4,6,3],[8,17,6]],[[2,1,1],[2,2,3],[4,2,5],[4,6,12],[4,10,3],[8,17,51],[8,40,9]]",217));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,1],[4,2,2],[4,6,1],[4,11,2],[8,17,2],[8,42,2]],[[2,1,1],[2,2,1],[4,2,5],[4,6,4],[4,11,5],[8,17,17],[8,42,17]]",218));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,3,2],[8,13,6]],[[2,1,7],[4,3,5],[4,4,21],[8,13,51]]",219));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,3,2],[4,5,4],[8,13,2]],[[2,1,3],[4,3,5],[4,4,3],[4,5,10],[8,13,17],[8,26,12]]",220));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,3,4],[4,6,3],[8,13,4]],[[2,1,3],[2,2,1],[4,3,10],[4,4,3],[4,6,12],[8,13,34],[8,14,12],[8,27,9]]",221));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,3,6],[4,6,2],[8,13,2]],[[2,1,3],[2,2,2],[4,3,15],[4,4,3],[4,6,8],[4,7,3],[8,13,17],[8,14,24],[8,20,12],[8,29,9]]",222));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,3,6],[4,6,2],[8,13,2]],[[2,1,3],[2,2,2],[4,3,15],[4,4,3],[4,6,8],[4,8,3],[8,13,17],[8,14,24],[8,15,12],[8,34,9]]",223));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,3],[4,1,2],[4,6,3],[8,7,6]],[[2,1,1],[2,2,3],[4,1,5],[4,6,12],[4,10,3],[8,7,51],[8,40,9]]",224));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,3,4],[4,6,1],[4,9,1],[8,13,2],[8,21,2],[8,35,1]],[[2,1,3],[2,2,1],[4,3,10],[4,4,3],[4,6,4],[4,9,4],[8,13,17],[8,14,12],[8,21,17],[8,35,13]]",225));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,3],[4,3,8],[4,6,3]],[[2,1,1],[2,2,3],[4,3,20],[4,6,12],[4,10,3],[8,14,60],[8,40,9]]",226));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,1],[4,3,4],[4,6,1],[4,11,2],[8,42,2]],[[2,1,1],[2,2,1],[4,3,10],[4,6,4],[4,11,5],[8,14,12],[8,42,17]]",227));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,6,12],[8,9,12],[8,10,24],[8,12,12],[8,18,12],[8,26,12]]",228));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4]],[[2,1,3],[4,4,3],[4,5,10],[8,26,12]]",229));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,5,12],[8,6,48],[8,16,12],[8,26,12]]",230));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,4],[4,2,4]],[[2,1,7],[4,1,10],[4,2,10],[4,4,21],[8,5,12],[8,8,48],[8,16,12]]",231));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,8],[4,5,4]],[[2,1,3],[4,1,20],[4,4,3],[4,5,10],[8,5,24],[8,26,12]]",232));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,4],[4,2,4]],[[2,1,7],[4,1,10],[4,2,10],[4,4,21],[8,5,12],[8,10,48],[8,16,12]]",233));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,8]],[[2,1,7],[4,1,20],[4,4,21],[8,5,24]]",234));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,5,12],[8,12,48],[8,16,12],[8,26,12]]",235));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,8],[4,5,4]],[[2,1,3],[4,2,20],[4,4,3],[4,5,10],[8,16,24],[8,18,48],[8,26,12]]",236));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,3,2],[4,5,4],[8,13,2],[8,22,2]],[[2,1,3],[4,3,5],[4,4,3],[4,5,10],[8,13,17],[8,22,17],[8,26,12]]",237));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[8,24,8]],[[2,1,7],[4,4,21],[8,24,44]]",238));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,1],[4,1,2],[4,2,2],[4,5,2],[4,6,1],[8,7,2],[8,17,2],[8,38,2]],[[2,1,1],[2,2,1],[4,1,5],[4,2,5],[4,5,5],[4,6,4],[8,6,12],[8,7,17],[8,17,17],[8,38,17]]",239));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,1,2],[4,2,2],[4,6,3],[8,7,2],[8,17,2]],[[2,1,3],[2,2,1],[4,1,5],[4,2,5],[4,4,3],[4,6,12],[8,7,17],[8,8,12],[8,17,17],[8,27,9]]",240));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,1],[4,1,4],[4,5,2],[4,6,1],[8,7,4],[8,38,2]],[[2,1,1],[2,2,1],[4,1,10],[4,5,5],[4,6,4],[8,7,34],[8,9,12],[8,38,17]]",241));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,1,2],[4,2,2],[4,6,3],[8,7,2],[8,17,2]],[[2,1,3],[2,2,1],[4,1,5],[4,2,5],[4,4,3],[4,6,12],[8,7,17],[8,10,12],[8,17,17],[8,27,9]]",242));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,1,4],[4,6,3],[8,7,4]],[[2,1,3],[2,2,1],[4,1,10],[4,4,3],[4,6,12],[8,7,34],[8,11,12],[8,27,9]]",243));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,1],[4,1,2],[4,2,2],[4,5,2],[4,6,1],[8,7,2],[8,17,2],[8,38,2]],[[2,1,1],[2,2,1],[4,1,5],[4,2,5],[4,5,5],[4,6,4],[8,7,17],[8,12,12],[8,17,17],[8,38,17]]",244));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,3,8],[4,6,3]],[[2,1,3],[2,2,1],[4,3,20],[4,4,3],[4,6,12],[8,14,24],[8,15,24],[8,27,9]]",245));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,1],[4,2,4],[4,5,2],[4,6,1],[8,17,4],[8,38,2]],[[2,1,1],[2,2,1],[4,2,10],[4,5,5],[4,6,4],[8,17,34],[8,18,12],[8,38,17]]",246));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,3,8],[4,6,3]],[[2,1,3],[2,2,1],[4,3,20],[4,4,3],[4,6,12],[8,14,24],[8,20,24],[8,27,9]]",247));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,1],[4,3,4],[4,5,2],[4,6,1],[8,22,4],[8,38,2]],[[2,1,1],[2,2,1],[4,3,10],[4,5,5],[4,6,4],[8,14,12],[8,22,34],[8,38,17]]",248));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,6,3],[8,23,4]],[[2,1,3],[2,2,1],[4,4,3],[4,6,12],[8,23,22],[8,27,9]]",249));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,6,3],[8,24,4]],[[2,1,3],[2,2,1],[4,4,3],[4,6,12],[8,24,22],[8,27,9]]",250));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,3,2],[8,13,2],[8,21,2]],[[2,1,7],[4,3,5],[4,4,21],[8,13,17],[8,21,17]]",251));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,2,8]],[[2,1,7],[4,2,20],[4,4,21],[8,16,24],[8,19,48]]",252));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,2,4],[4,6,3],[8,17,4]],[[2,1,3],[2,2,1],[4,2,10],[4,4,3],[4,6,12],[8,17,34],[8,19,12],[8,27,9]]",253));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,3,4],[4,6,3],[8,21,4]],[[2,1,3],[2,2,1],[4,3,10],[4,4,3],[4,6,12],[8,14,12],[8,21,34],[8,27,9]]",254));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,4],[4,6,2],[8,7,4]],[[2,1,3],[2,2,2],[4,1,10],[4,4,3],[4,6,8],[4,8,3],[8,5,12],[8,7,34],[8,34,9]]",255));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,2,4],[4,6,2],[8,17,4]],[[2,1,3],[2,2,2],[4,2,10],[4,4,3],[4,6,8],[4,8,3],[8,16,12],[8,17,34],[8,34,9]]",256));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[8,23,8]],[[2,1,7],[4,4,21],[8,23,44]]",257));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[8,23,8]],[[2,1,7],[4,4,21],[8,23,44]]",258));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[8,23,8]],[[2,1,7],[4,4,21],[8,23,44]]",259));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7]],[[2,1,7],[4,4,21]]",260));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,4],[8,23,4]],[[2,1,7],[4,1,10],[4,4,21],[8,5,12],[8,23,22]]",261));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,2,4],[8,23,4]],[[2,1,7],[4,2,10],[4,4,21],[8,16,12],[8,23,22]]",262));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,2,4],[8,23,4]],[[2,1,7],[4,2,10],[4,4,21],[8,16,12],[8,23,22]]",263));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,4],[8,23,4]],[[2,1,7],[4,1,10],[4,4,21],[8,5,12],[8,23,22]]",264));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,3,4],[8,21,4]],[[2,1,7],[4,3,10],[4,4,21],[8,20,12],[8,21,34]]",265));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,3,4],[8,21,4]],[[2,1,7],[4,3,10],[4,4,21],[8,15,12],[8,21,34]]",266));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,2,8]],[[2,1,7],[4,2,20],[4,4,21],[8,16,24]]",267));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,2,4],[8,23,4]],[[2,1,7],[4,2,10],[4,4,21],[8,16,12],[8,23,22]]",268));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,4],[8,23,4]],[[2,1,7],[4,1,10],[4,4,21],[8,5,12],[8,23,22]]",269));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[8,23,8]],[[2,1,7],[4,4,21],[8,23,44]]",270));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,6],[4,2,2],[4,5,6]],[[2,1,1],[4,1,15],[4,2,5],[4,5,15],[8,6,24],[8,9,36],[8,12,12],[8,39,12]]",271));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[4,1,2],[4,2,6],[4,5,2]],[[2,1,5],[4,1,5],[4,2,15],[4,4,6],[4,5,5],[8,6,12],[8,10,24],[8,16,12],[8,18,12],[8,19,12],[8,28,12]]",272));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,6,24],[8,11,12],[8,12,24],[8,16,12],[8,32,12]]",273));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,6,24],[8,8,12],[8,9,12],[8,10,12],[8,18,12],[8,32,12]]",275));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,8,12],[8,9,12],[8,10,12],[8,12,24],[8,18,12],[8,32,12]]",276));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,24,8]],[[2,1,3],[4,4,3],[4,5,10],[8,24,44],[8,26,12]]",277));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,3,8]],[[2,1,7],[4,3,20],[4,4,21],[8,15,72]]",278));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,8]],[[2,1,7],[4,1,20],[4,4,21],[8,5,24],[8,11,48]]",180));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,6,2],[4,9,5]],[[2,1,3],[2,2,5],[4,4,3],[4,6,8],[4,7,3],[4,8,3],[4,9,20],[4,10,6],[8,34,9],[8,37,9],[8,45,9],[8,48,18],[8,49,6]]",284));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[2,2,4],[4,5,2],[4,6,4]],[[2,1,5],[2,2,4],[4,4,6],[4,5,5],[4,6,16],[4,8,18],[8,28,12],[8,34,18]]",285));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,5,8]],[[2,1,7],[4,4,21],[4,5,20],[8,26,24]]",288));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,12]],[[2,1,3],[4,4,3],[4,5,30],[8,26,36]]",291));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,12]],[[2,1,3],[4,4,3],[4,5,30],[8,26,36]]",292));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,1],[4,9,7]],[[2,1,7],[2,2,1],[4,4,21],[4,9,28],[8,45,63]]",293));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,11],[4,5,4]],[[2,1,11],[4,4,27],[4,5,10],[8,28,48],[8,31,12],[8,32,12]]",294));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,9],[4,9,3]],[[2,1,3],[2,2,9],[4,4,3],[4,7,12],[4,8,12],[4,9,12],[4,10,12],[8,36,12],[8,45,9],[8,49,24]]",295));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,9,6]],[[2,1,7],[2,2,2],[4,4,21],[4,8,3],[4,9,24],[8,44,27],[8,45,18]]",296));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,9],[4,9,3]],[[2,1,3],[2,2,9],[4,4,3],[4,7,18],[4,8,18],[4,9,12],[4,10,12],[8,45,9],[8,49,36]]",297));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,8],[4,6,2],[4,9,2]],[[2,1,3],[2,2,8],[4,4,3],[4,6,8],[4,7,12],[4,8,6],[4,9,8],[4,10,12],[8,29,9],[8,36,6],[8,37,9],[8,41,12],[8,49,12]]",298));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,5,4],[4,9,3],[8,46,4]],[[2,1,3],[2,2,1],[4,4,3],[4,5,10],[4,9,12],[8,32,12],[8,45,9],[8,46,34]]",299));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,7],[4,9,3],[4,11,2]],[[2,1,3],[2,2,7],[4,7,9],[4,8,9],[4,9,12],[4,10,9],[4,11,5],[8,49,18]]",300));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15]],[[2,1,15],[4,4,105]]",301));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,3],[4,6,1],[4,9,2],[4,11,2],[8,42,2],[8,51,2]],[[2,1,1],[2,2,3],[4,6,4],[4,9,8],[4,10,3],[4,11,5],[8,42,17],[8,48,9],[8,51,17]]",302));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,5,8]],[[2,1,7],[4,4,21],[4,5,20],[8,26,72]]",303));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,12]],[[2,1,3],[4,4,3],[4,5,30],[8,26,36]]",304));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,1],[4,6,7]],[[2,1,7],[2,2,1],[4,4,21],[4,6,28],[8,27,63]]",305));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,5,4],[4,6,3],[8,38,4]],[[2,1,3],[2,2,1],[4,4,3],[4,5,10],[4,6,12],[8,26,12],[8,27,9],[8,38,34]]",306));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,3],[4,6,9]],[[2,1,3],[2,2,3],[4,4,3],[4,6,36],[4,10,3],[8,27,27],[8,40,27]]",307));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,6,6]],[[2,1,7],[2,2,2],[4,4,21],[4,6,24],[4,7,3],[8,27,18],[8,29,27]]",308));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,5,4],[4,6,2],[8,38,4]],[[2,1,3],[2,2,2],[4,4,3],[4,5,10],[4,6,8],[4,7,3],[8,26,12],[8,29,9],[8,38,34]]",309));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,6,7]],[[2,1,3],[2,2,5],[4,4,3],[4,6,28],[4,7,6],[4,10,6],[8,27,9],[8,29,18],[8,40,18],[8,41,6]]",310));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,8],[4,6,4]],[[2,1,3],[2,2,8],[4,4,3],[4,6,16],[4,7,24],[4,10,12],[8,29,18],[8,41,24]]",311));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,8],[4,6,4]],[[2,1,3],[2,2,8],[4,4,3],[4,6,16],[4,7,9],[4,8,9],[4,10,12],[8,29,9],[8,34,9],[8,36,6],[8,41,12],[8,47,12]]",312));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,8],[4,6,4]],[[2,1,3],[2,2,8],[4,4,3],[4,6,16],[4,8,24],[4,10,12],[8,34,18],[8,47,24]]",313));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,1],[4,6,3],[4,9,1],[8,35,3]],[[2,1,7],[2,2,1],[4,4,21],[4,6,12],[4,9,4],[8,27,9],[8,35,39]]",314));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,5,4],[4,6,1],[4,9,1],[8,35,1],[8,38,2]],[[2,1,3],[2,2,1],[4,4,3],[4,5,10],[4,6,4],[4,9,4],[8,26,12],[8,35,13],[8,38,17]]",315));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,3],[4,6,5],[4,9,2],[8,35,2]],[[2,1,3],[2,2,3],[4,4,3],[4,6,20],[4,9,8],[4,10,3],[8,27,9],[8,35,26],[8,40,9],[8,48,9]]",316));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,6,3],[4,9,3],[8,35,1]],[[2,1,3],[2,2,5],[4,4,3],[4,6,12],[4,7,6],[4,9,12],[4,10,6],[8,29,9],[8,35,13],[8,37,9],[8,41,6],[8,48,18]]",317));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,6,3],[4,9,3],[8,35,1]],[[2,1,3],[2,2,5],[4,4,3],[4,6,12],[4,8,6],[4,9,12],[4,10,6],[8,34,9],[8,35,13],[8,44,9],[8,47,6],[8,48,18]]",318));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,3],[4,5,2],[4,6,3],[8,38,6]],[[2,1,1],[2,2,3],[4,5,5],[4,6,12],[4,10,3],[8,38,51],[8,40,9]]",319));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,7],[4,6,7]],[[2,1,1],[2,2,7],[4,6,28],[4,10,18],[8,40,54],[8,50,3]]",320));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,11],[4,6,3]],[[2,1,1],[2,2,11],[4,6,12],[4,7,12],[4,10,33],[8,40,9],[8,41,30],[8,50,6]]",321));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,11],[4,6,3]],[[2,1,1],[2,2,11],[4,6,12],[4,8,12],[4,10,33],[8,40,9],[8,47,30],[8,50,6]]",322));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,7],[4,6,3],[4,9,4]],[[2,1,1],[2,2,7],[4,6,12],[4,9,16],[4,10,18],[8,40,9],[8,48,45],[8,50,3]]",323));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,6,3],[4,11,2],[8,42,6]],[[2,1,3],[2,2,1],[4,4,3],[4,6,12],[4,11,5],[8,27,9],[8,42,51]]",324));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,1],[4,5,2],[4,6,1],[4,11,2],[8,38,2],[8,42,2]],[[2,1,1],[2,2,1],[4,5,5],[4,6,4],[4,11,5],[8,38,17],[8,42,17]]",325));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,3],[4,6,3],[4,11,2],[8,42,2],[8,51,2]],[[2,1,1],[2,2,3],[4,6,12],[4,10,3],[4,11,5],[8,40,9],[8,42,17],[8,51,17]]",326));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,5],[4,6,1],[4,11,2],[8,42,2],[8,51,4]],[[2,1,1],[2,2,5],[4,6,4],[4,7,6],[4,10,6],[4,11,5],[8,41,6],[8,42,17],[8,51,34]]",327));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,5],[4,6,1],[4,11,2],[8,42,2],[8,51,4]],[[2,1,1],[2,2,5],[4,6,4],[4,8,6],[4,10,6],[4,11,5],[8,42,17],[8,47,6],[8,51,34]]",328));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,1],[4,6,1],[4,9,3],[8,35,3]],[[2,1,7],[2,2,1],[4,4,21],[4,6,4],[4,9,12],[8,35,39],[8,45,9]]",329));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,6,5],[4,9,1],[8,35,1]],[[2,1,3],[2,2,5],[4,4,3],[4,6,20],[4,7,3],[4,8,3],[4,9,4],[4,10,6],[8,29,9],[8,34,9],[8,35,13],[8,40,18],[8,49,6]]",330));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,1],[4,6,1],[8,52,6]],[[2,1,1],[2,2,1],[4,6,4],[8,52,27]]",331));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,3],[4,6,3],[4,9,6]],[[2,1,3],[2,2,3],[4,4,3],[4,6,12],[4,9,24],[4,10,3],[8,27,9],[8,45,18],[8,48,27]]",332));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15]],[[2,1,15],[4,4,57],[8,31,48]]",333));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,5,8]],[[2,1,7],[4,4,21],[4,5,20],[8,26,24],[8,32,48]]",334));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,4],[4,6,4]],[[2,1,7],[2,2,4],[4,4,21],[4,6,16],[4,7,6],[4,8,6],[8,29,18],[8,34,18],[8,36,6]]",335));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,6,2],[4,9,2],[8,35,2]],[[2,1,7],[2,2,2],[4,4,21],[4,6,8],[4,7,3],[4,9,8],[8,29,9],[8,35,26],[8,37,9]]",336));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,12]],[[2,1,3],[4,4,3],[4,5,30],[8,26,36],[8,39,48]]",337));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,1],[4,6,7]],[[2,1,7],[2,2,1],[4,4,9],[4,6,28],[8,27,27],[8,31,12]]",338));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,5,4],[4,6,3],[8,38,4]],[[2,1,3],[2,2,1],[4,4,3],[4,5,10],[4,6,12],[8,27,9],[8,32,12],[8,38,34]]",339));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,9],[4,6,3]],[[2,1,3],[2,2,9],[4,4,3],[4,6,12],[4,7,12],[4,8,12],[4,10,12],[8,27,9],[8,36,12],[8,41,12],[8,47,12]]",340));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,6,3],[4,9,4]],[[2,1,3],[2,2,5],[4,4,3],[4,6,12],[4,7,6],[4,9,16],[4,10,6],[8,27,9],[8,37,18],[8,41,6],[8,48,18]]",341));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,1],[4,5,6],[4,6,1],[8,38,6]],[[2,1,1],[2,2,1],[4,5,15],[4,6,4],[8,38,51],[8,39,12]]",342));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,11],[4,5,4]],[[2,1,11],[4,4,39],[4,5,10],[8,26,12],[8,28,48]]",343));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[2,2,1],[4,5,2],[4,6,5],[8,38,2]],[[2,1,5],[2,2,1],[4,4,6],[4,5,5],[4,6,20],[8,27,18],[8,28,12],[8,38,17]]",344));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[8,33,8]],[[2,1,7],[4,4,21],[8,33,44]]",345));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,6,3],[8,33,4]],[[2,1,3],[2,2,1],[4,4,3],[4,6,12],[8,27,9],[8,33,22]]",346));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,6,3],[4,9,4]],[[2,1,3],[2,2,5],[4,4,3],[4,6,12],[4,8,6],[4,9,16],[4,10,6],[8,27,9],[8,44,18],[8,47,6],[8,48,18]]",347));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,5,4],[4,6,1],[4,9,1],[8,35,1],[8,46,2]],[[2,1,3],[2,2,1],[4,4,3],[4,5,10],[4,6,4],[4,9,4],[8,26,12],[8,35,13],[8,46,17]]",348));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,4],[4,6,4]],[[2,1,7],[2,2,4],[4,4,21],[4,6,16],[4,7,18],[8,29,54]]",349));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,9],[4,6,3]],[[2,1,3],[2,2,9],[4,4,3],[4,6,12],[4,7,36],[4,10,12],[8,27,9],[8,41,36]]",350));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,9],[4,6,3]],[[2,1,3],[2,2,9],[4,4,3],[4,6,12],[4,8,36],[4,10,12],[8,27,9],[8,47,36]]",351));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,6,2],[4,9,2],[8,35,2]],[[2,1,7],[2,2,2],[4,4,21],[4,6,8],[4,8,3],[4,9,8],[8,34,9],[8,35,26],[8,44,9]]",352));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,4],[4,6,4]],[[2,1,7],[2,2,4],[4,4,21],[4,6,16],[4,8,18],[8,34,54]]",353));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,6,6]],[[2,1,7],[2,2,2],[4,4,21],[4,6,24],[4,8,3],[8,27,18],[8,34,27]]",354));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,5,4],[4,6,2],[8,38,4]],[[2,1,3],[2,2,2],[4,4,3],[4,5,10],[4,6,8],[4,8,3],[8,26,12],[8,34,9],[8,38,34]]",355));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,6,7]],[[2,1,3],[2,2,5],[4,4,3],[4,6,28],[4,8,6],[4,10,6],[8,27,9],[8,34,18],[8,40,18],[8,47,6]]",356));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,5,8]],[[2,1,7],[4,4,21],[4,5,20],[8,26,24]]",357));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,4],[4,9,4]],[[2,1,7],[2,2,4],[4,4,21],[4,7,6],[4,8,6],[4,9,16],[8,36,6],[8,37,18],[8,44,18]]",358));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,9,6]],[[2,1,7],[2,2,2],[4,4,21],[4,7,3],[4,9,24],[8,37,27],[8,45,18]]",359));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,5,4],[4,9,2],[8,46,4]],[[2,1,3],[2,2,2],[4,4,3],[4,5,10],[4,7,3],[4,9,8],[8,26,12],[8,37,9],[8,46,34]]",360));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,5,8]],[[2,1,7],[4,4,21],[4,5,20],[8,26,24]]",361));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,12]],[[2,1,3],[4,4,3],[4,5,30],[8,26,36]]",362));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[8,33,8]],[[2,1,7],[4,4,21],[8,33,44]]",363));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,9,3],[4,11,8]],[[2,1,3],[2,2,1],[4,4,3],[4,9,12],[4,11,20],[8,45,9]]",364));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,8],[4,6,2],[4,9,2]],[[2,1,3],[2,2,8],[4,4,3],[4,6,8],[4,7,6],[4,8,12],[4,9,8],[4,10,12],[8,34,9],[8,36,6],[8,44,9],[8,47,12],[8,49,12]]",365));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,1],[4,9,7]],[[2,1,7],[2,2,1],[4,4,9],[4,9,28],[8,31,12],[8,45,27]]",366));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4]],[[2,1,3],[4,4,3],[4,5,10],[8,26,12]]",367));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[2,2,4],[4,5,2],[4,9,4]],[[2,1,5],[2,2,4],[4,4,6],[4,5,5],[4,7,6],[4,8,6],[4,9,16],[8,28,12],[8,37,18],[8,44,18]]",368));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,8],[4,6,2],[4,9,2]],[[2,1,3],[2,2,8],[4,4,3],[4,6,8],[4,7,3],[4,8,21],[4,9,8],[4,10,12],[8,29,9],[8,44,9],[8,47,12],[8,49,12]]",369));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[2,2,4],[4,5,2],[4,9,4]],[[2,1,5],[2,2,4],[4,4,6],[4,5,5],[4,8,18],[4,9,16],[8,28,12],[8,44,18]]",370));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,6,2],[4,9,5]],[[2,1,3],[2,2,5],[4,4,3],[4,6,8],[4,7,3],[4,8,3],[4,9,20],[4,10,6],[8,29,9],[8,44,9],[8,45,9],[8,48,18],[8,49,6]]",371));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,8],[4,6,2],[4,9,2]],[[2,1,3],[2,2,8],[4,4,3],[4,6,8],[4,7,21],[4,8,3],[4,9,8],[4,10,12],[8,34,9],[8,37,9],[8,41,12],[8,49,12]]",372));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[2,2,4],[4,5,2],[4,9,4]],[[2,1,5],[2,2,4],[4,4,6],[4,5,5],[4,7,18],[4,9,16],[8,28,12],[8,37,18]]",373));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[2,2,1],[4,5,2],[4,9,5],[8,46,2]],[[2,1,5],[2,2,1],[4,4,6],[4,5,5],[4,9,20],[8,28,12],[8,45,18],[8,46,17]]",374));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[2,2,4],[4,5,2],[4,6,4]],[[2,1,5],[2,2,4],[4,4,6],[4,5,5],[4,6,16],[4,7,18],[8,28,12],[8,29,18]]",375));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[2,2,4],[4,5,2],[4,6,4]],[[2,1,5],[2,2,4],[4,4,6],[4,5,5],[4,6,16],[4,7,6],[4,8,6],[8,28,12],[8,29,18],[8,34,18]]",376));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,4],[4,11,8]],[[2,1,3],[2,2,4],[4,4,3],[4,8,18],[4,11,20]]",378));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,11],[4,5,4]],[[2,1,11],[4,4,39],[4,5,10],[8,26,12]]",379));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,11],[4,6,1],[4,9,2]],[[2,1,1],[2,2,11],[4,6,4],[4,7,6],[4,8,6],[4,9,8],[4,10,33],[8,41,6],[8,47,6],[8,48,9],[8,49,18],[8,50,6]]",380));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,4],[4,11,8]],[[2,1,3],[2,2,4],[4,4,3],[4,7,18],[4,11,20]]",381));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,3],[4,5,2],[4,6,1],[4,9,2],[8,38,2],[8,46,4]],[[2,1,1],[2,2,3],[4,5,5],[4,6,4],[4,9,8],[4,10,3],[8,38,17],[8,46,34],[8,48,9]]",382));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,9,3],[4,11,2]],[[2,1,3],[2,2,1],[4,4,3],[4,9,12],[4,11,5],[8,45,9]]",383));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,3],[4,6,2],[4,9,5],[8,35,2]],[[2,1,3],[2,2,3],[4,4,3],[4,6,8],[4,9,20],[4,10,3],[8,35,26],[8,45,9],[8,48,18]]",384));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,9,3],[8,33,4]],[[2,1,3],[2,2,1],[4,4,3],[4,9,12],[8,33,22],[8,45,9]]",385));
	m_I1I2.insert(make_pair("[[1,1,1],[2,2,15]],[[2,2,15],[4,10,75],[8,50,30]]",386));
	m_I1I2.insert(make_pair("[[1,1,1],[2,2,7],[4,11,2],[8,51,6]],[[2,2,7],[4,10,18],[4,11,5],[8,50,3],[8,51,51]]",387));
	m_I1I2.insert(make_pair("[[1,1,1],[2,2,3],[8,52,6]],[[2,2,3],[4,10,3],[8,52,27]]",388));
	m_I1I2.insert(make_pair("[[1,1,1],[2,2,3],[4,11,8],[8,51,4]],[[2,2,3],[4,10,3],[4,11,20],[8,51,34]]",389));
	m_I1I2.insert(make_pair("[[1,1,1],[2,2,1],[4,11,2]],[[2,2,1],[4,11,5]]",390));
#endif	
	//R17
	m_RingInvariant.insert(make_pair("1,0,17,1,16,16,289,16,17",1));
	m_RingInvariant.insert(make_pair("1,1,1,2,0,0,33,0,17",2));	
	//R18
	m_RingInvariant.insert(make_pair("[1,1,2,2,6,6],18,1,0,18,1,17,17,324,17,18",1));
	m_RingInvariant.insert(make_pair("[1,1,2,2,6,6],18,1,0,18,2,5,5,84,17,18",2));
	m_RingInvariant.insert(make_pair("[1,1,2,2,6,6],18,1,0,18,1,5,17,180,17,18",3));
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,1,0,18,1,17,17,324,17,18",4));	
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,1,0,18,1,5,17,180,17,18",5));
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,1,0,18,2,5,5,180,17,18",6));
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,0,0,18,4,5,5,132,17,2,[1,1,8,8,0,0],[[3,3,48],[3,6,48],[6,3,48],[6,6,48]]",7));
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,0,0,18,4,5,5,132,17,2,[1,1,8,8,0,0],[[3,3,48],[3,6,48],[6,3,48],[6,6,48]]",8));	
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,1,0,18,2,5,5,84,17,18",9));
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,1,0,18,4,1,1,100,17,18",10));
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,1,0,18,2,1,1,68,17,18",11));
	m_RingInvariant.insert(make_pair("[1,1,2,2,6,6],18,1,0,18,2,8,8,243,17,18",12));
	m_RingInvariant.insert(make_pair("[1,1,2,2,6,6],18,1,1,12,4,2,2,63,11,18",13));
	m_RingInvariant.insert(make_pair("[1,1,2,2,6,6],18,1,0,18,2,2,8,135,17,18",14));	
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,1,0,18,2,8,8,243,17,18",15));
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,1,0,18,2,2,8,135,17,18",16));
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,1,0,18,4,2,2,135,17,18",17));
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,0,0,18,8,2,2,99,11,2",18));	
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,0,0,18,8,2,2,99,17,2",19));
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,1,1,12,4,2,2,63,11,18",20));
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,1,1,14,8,0,0,75,13,18",21));	
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,1,1,10,4,0,0,51,9,18",22));	
	//R18	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[3,1,2],[3,2,6],[6,1,2],[6,2,6]],[[2,1,1],[3,1,3],[3,2,9],[6,1,11],[6,2,33],[9,7,24]]",7));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[3,1,2],[3,2,6],[6,1,2],[6,2,6]],[[2,1,1],[3,1,3],[3,2,9],[6,1,11],[6,2,33],[9,8,24]]",8));		
	//R19
	m_RingInvariant.insert(make_pair("1,0,19,1,18,18,361,18,19",1));
	m_RingInvariant.insert(make_pair("1,1,1,2,0,0,37,0,19",2));	
	//R27	
	m_RingInvariant.insert(make_pair("[1,2,6,18],27,1,0,27,1,26,26,729,26,27",1));
	m_RingInvariant.insert(make_pair("[1,2,6,18],27,1,0,27,1,8,8,189,26,27",2));
	m_RingInvariant.insert(make_pair("[1,2,6,18],27,1,1,9,2,2,8,81,8,27",3));
	m_RingInvariant.insert(make_pair("[1,2,6,18],27,1,0,27,1,8,26,405,26,27",4));	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,26,26,729,26,27,[1,8,18,0],[]",5));//Rppp(5,3)、R9R3_1
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,2,8,8,405,26,27",7));//Rppp(7,3)=R9R3_12	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,14,26,405,26,3,[1,8,18,0],[[3,9,108],[9,9,216]]",8));//Rppp(8,3),R27_8有子环R3_1和R9_1，但不是可分解环		
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,1,15,4,2,2,105,14,27",9));//R9R3_13
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,14,26,405,26,3,[1,8,18,0],[[9,3,108],[9,9,216]]",10));//Rppp(10,3)	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,14,26,297,26,27,[1,8,18,0],[[3,9,108],[9,3,108],[9,9,216]]",11));//Rppp(11,3)	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,2,8,8,189,26,27,[1,8,6,12],[[3,9,108],[9,3,108],[9,9,324]]",13));//Rppp(13,3)、R9R3_2	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,4,8,8,153,26,3,[1,8,18,0],[[3,9,144],[9,3,108],[9,9,324]]",15));//Rppp(15,3)
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,8,26,405,26,27,[1,8,18,0],[[9,9,324]]",16));//Rppp(16,3),R9R3_3
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,2,2,8,225,26,27",17));//Rppp(17,3)=R9R3_14	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,8,26,297,26,3,[1,8,18,0],[[3,9,108],[9,3,108],[9,9,216]]",19));//Rppp(19,3)	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,4,8,8,153,8,3,[1,8,18,0],[[3,9,108],[9,3,144],[9,9,324]]",20));//Rppp(20,3)	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,1,9,2,8,8,117,8,27,[1,8,6,12],[[3,9,144],[9,3,144],[9,9,324]]",21));//Rppp(21,3)
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,8,26,405,26,27,[1,8,0,18],[[9,9,324]]",23));//Rppp(23,3)	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,8,8,189,26,27,[1,8,0,18],[[3,9,108],[9,3,108],[9,9,324]]",24));//Rppp(24,3)	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,1,26,26,729,26,27,[1,26,0,0],[]",25));//Rppp(25,3)、R9R3_4
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,4,8,8,297,26,3,[1,14,12,0],[[3,3,432]]",26));//Rppp(26,3)=R9R3_7
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,2,8,8,405,26,27,[1,10,16,0],[[3,3,324]]",27));//Rppp(27,3)、R9R3_6、R9R3_15	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,4,8,8,297,26,3,[1,14,12,0],[[3,3,432]]",28));//Rppp(28,3)=R9R3_8、Rppp(29,3)、Rppp(34,3)
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,10,8,8,261,8,1,[1,26,0,0],[[3,3,468]]",30));//Rppp(30,3)
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,2,2,8,225,26,27",33));//Rppp(33,3)=R9R3_16	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,2,8,8,189,26,27",35));//Rppp(35,3)=R9R3_9	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,10,8,8,189,26,1,[1,26,0,0],[[3,3,540]]",36));//Rppp(36,3)	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,4,8,8,153,8,3,[1,14,12,0],[[3,3,576]]",37));//Rppp(37,3)		
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,4,2,2,225,26,27,[1,8,14,4],[[3,3,504]]",38));//Rppp(38,3)=R9R3_10=R9R3_17	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,1,8,26,297,26,3,[1,8,18,0],[[3,3,432]]",39));//Rppp(39,3)		
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,8,2,2,165,14,3,[1,16,10,0],[[3,3,564]]",41));//Rppp(41,3)、R9R3_18	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,2,2,2,153,26,27",42));//Rppp(42,3)=R9R3_11
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,10,8,8,261,26,1,[1,26,0,0],[[3,3,468]]",43));//Rppp(43,3)	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,4,8,8,153,26,3,[1,14,12,0],[[3,3,576]]",44));//Rppp(44,3)		
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,8,2,2,165,26,3,[1,16,10,0],[[3,3,564]]",47));//Rppp(47,3)、Rppp(44,3)、R9R3_19		
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,15,4,2,2,105,14,27,[1,8,14,4],[[3,3,624]]",48));//Rppp(48,3)、R9R3_20、R3_2/([0,0,1,1])=R3_2/([0,0,2,1])	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,1,15,8,2,2,129,14,3,[1,16,10,0],[[3,3,600]]",49));//Rppp(29,3)、Rppp(49,3)=[[1,0],[0,0]],[[1,1],[0,1]]	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,19,8,0,0,125,18,27,[1,14,12,0],[[3,3,604]]",50));//Rppp(50,3)、R9R3_21、R3_2/([0,2,0,1])			
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,11,4,0,0,85,10,27,[1,6,8,12],[[3,3,644]]",51));//Rppp(51,3)、R9R3_22、R3_2/([1,1,0,1])	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,9,2,2,8,81,8,27",52));//Rppp(52,3)=R3_2/([1,0,0,1])=R3_2/([0,0,0,1])
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,1,8,26,405,26,27",56));//Rppp(56,3)=R9R3_5	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,1,2,0,0,53,0,27",59));//Rppp(59,3)=R3_2/([2,2,0,1])	
	//R27	
	m_I1I2.insert(make_pair("[[1,1,1],[3,1,8],[3,2,6],[9,6,12]],[[3,1,12],[3,2,9],[9,4,24],[9,6,90],[9,7,24]]",26));
	m_I1I2.insert(make_pair("[[1,1,1],[3,1,8],[3,2,2],[9,6,16]],[[3,1,12],[3,2,3],[9,4,24],[9,6,120]]",27));		
	m_I1I2.insert(make_pair("[[1,1,1],[3,1,8],[3,2,6],[9,6,12]],[[3,1,12],[3,2,9],[9,4,24],[9,6,90],[9,8,24]]",28));
	m_I1I2.insert(make_pair("[[1,1,1],[3,2,14],[9,10,12]],[[3,2,21],[9,10,162]]",50));	
#ifndef R32_DEL
	//R32
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,1,16,2,7,7,160,15,32,[1,3,4,8,8,8],[[2,16,48],[4,8,32],[4,16,64],[8,4,32],[8,8,64],[8,16,128],[16,2,48],[16,4,64],[16,8,128],[16,16,256]]",7));//[[1,8],[0,1]]	
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,1,31,31,1024,31,32,[1,3,4,8,16,0],[]",8));//[[0,4],[0,0]],[[32,8],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,0,0,32,1,23,31,768,31,8,[1,3,4,8,16,0],[[2,16,32],[4,16,32],[8,16,64],[16,16,128]]",9));//[[0,1],[0,0]],[[8,0],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,0,0,32,1,15,31,512,31,8,[1,3,4,8,16,0],[[2,16,32],[4,16,32],[8,16,64],[16,8,128],[16,16,256]]",10));//[[0,2],[0,0]],[[4,1],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,0,0,32,1,15,31,512,31,8,[1,3,4,8,16,0],[[8,16,128],[16,2,32],[16,4,32],[16,8,64],[16,16,256]]",11));//[[4,0],[1,0]],[[8,0],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,0,0,32,1,23,31,768,31,8,[1,3,4,8,16,0],[[16,2,32],[16,4,32],[16,8,64],[16,16,128]]",12));//[[8,0],[0,0]],[[8,0],[1,0]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,1,15,31,640,31,32,[1,3,4,8,16,0],[[2,16,32],[4,16,32],[8,16,64],[16,2,32],[16,4,32],[16,8,64],[16,16,128]]",13));//[[4,1],[0,4]],[[8,0],[0,8]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,1,31,31,640,31,32,[1,3,4,8,16,0],[[2,16,32],[4,16,32],[8,16,64],[16,2,32],[16,4,32],[16,8,64],[16,16,128]]",14));//[[8,0],[0,8]],[[8,1],[0,8]]	
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,2,7,7,192,31,32,[1,3,4,8,8,8],[[2,16,32],[4,8,32],[4,16,64],[8,4,32],[8,8,64],[8,16,128],[16,2,32],[16,4,64],[16,8,128],[16,16,256]]",15));//[[8,0],[0,3]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,0,0,32,3,7,7,176,31,8,[1,3,4,8,16,0],[[2,16,48],[4,8,32],[4,16,64],[8,4,32],[8,8,64],[8,16,128],[16,2,32],[16,4,64],[16,8,128],[16,16,256]]",16));//[[0,0],[0,1]],[[0,8],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,1,15,31,512,31,32,[1,3,4,8,16,0],[[8,16,128],[16,8,128],[16,16,256]]",17));//[[4,0],[0,0]],[[32,32],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,0,0,32,3,7,7,176,15,8,[1,3,4,8,16,0],[[2,16,32],[4,8,32],[4,16,64],[8,4,32],[8,8,64],[8,16,128],[16,2,48],[16,4,64],[16,8,128],[16,16,256]]",18));//[[1,4],[0,0]],[[0,8],[0,0]]
	/*---------------注意R8R4_12=R8_12×R4_1，但此处R8_2×R4_1也写成了R8R4_12(1*11+1=12,51*11+11=572,...)的形式----------------*/
	/*---------------begin of R32_1000~R32_1011----------------*/
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,31,31,1024,31,32,[1,3,12,16,0,0],[]",1000));//R8R4_1=R8_1×R4_1
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,15,512,31,32,[1,3,12,8,8,0],[[4,8,128],[8,4,128],[8,8,256]]",1003));//R8R4_12=R8_2×R4_1
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,2,7,15,320,31,32,[1,3,8,8,4,8],[[2,8,32],[4,4,64],[4,8,160],[8,2,32],[8,4,160],[8,8,256]]",1006));//R8R4_23=R8_3×R4_1
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,768,31,32,[1,3,12,16,0,0],[[8,8,256]]",1009));//R8R4_34=R8_4×R4_1
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,768,31,32,[1,3,8,12,8,0],[[4,4,64],[4,8,64],[8,4,64],[8,8,64]]",1001));//R8R4_2=R8_1×R4_2
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,15,384,31,32,[1,3,8,20,0,0],[[4,4,64],[4,8,160],[8,4,160],[8,8,256]]",1004));//R8R4_13=R8_2×R4_2
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,2,3,15,240,31,32,[1,3,12,4,4,8],[[2,8,32],[4,4,112],[4,8,176],[8,2,32],[8,4,176],[8,8,256]]",1007));//R8R4_24=R8_3×R4_2
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,31,576,31,32,[1,3,8,12,8,0],[[4,4,64],[4,8,64],[8,4,64],[8,8,256]]",1010));//R8R4_35=R8_4×R4_2
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,2,15,15,512,31,32,[1,3,6,10,4,8],[[2,4,16],[2,8,16],[4,2,16],[4,4,96],[4,8,112],[8,2,16],[8,4,112],[8,8,128]]",1002));//R8R4_3=R8_1×R4_3
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,2,7,7,256,31,32,[1,3,6,10,4,8],[[2,4,16],[2,8,16],[4,2,16],[4,4,96],[4,8,176],[8,2,16],[8,4,176],[8,8,256]]",1005));//R8R4_14=R8_2×R4_3
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,1,24,4,3,7,160,23,32,[1,3,6,10,12,0],[[2,4,16],[2,8,40],[4,2,16],[4,4,128],[4,8,184],[8,2,40],[8,4,184],[8,8,256]]",1008));//R8R4_25=R8_3×R4_3
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,2,7,15,384,31,32,[1,3,6,10,4,8],[[2,4,16],[2,8,16],[4,2,16],[4,4,96],[4,8,112],[8,2,16],[8,4,112],[8,8,256]]",1011));//R8R4_36=R8_4×R4_3
	/*---------------end of R32_1000~R32_1011----------------*/	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,1,16,2,3,15,144,15,32,[1,3,4,12,4,8],[[2,8,48],[4,4,144],[4,8,192],[8,2,48],[8,4,192],[8,8,256]]",1012));//R8_3/([[1,2]],[1,1,1])
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,1,16,2,3,15,176,15,32,[1,3,12,4,4,8],[[2,8,48],[4,4,112],[4,8,192],[8,2,48],[8,4,192],[8,8,256]]",1013));//R8_3/([[1,2]],[0,1,1])
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,1,16,2,7,15,160,15,32,[1,3,8,8,4,8],[[2,8,48],[4,4,128],[4,8,192],[8,2,48],[8,4,192],[8,8,256]]",1014));//R8_3/([[1,2]],[0,0,1])=R8_3/([[1,2]],[1,0,1])=R32_1014
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,15,320,31,32,[1,3,8,20,0,0],[[2,8,32],[4,4,128],[4,8,160],[8,2,32],[8,4,160],[8,8,192]]",1015));//[[2,0],[0,2]],[[1,1],[1,1]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,15,192,31,32,[1,3,8,4,0,16],[[2,8,32],[4,4,128],[4,8,192],[8,2,32],[8,4,192],[8,8,256]]",1016));//[[4,2],[5,6]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,5,7,15,240,15,2,[1,3,12,16,0,0],[[2,8,32],[4,4,96],[4,8,160],[8,2,48],[8,4,192],[8,8,256]]",1017));//[[1,0],[0,0]],[[2,2],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,11,23,448,31,2,[1,3,12,16,0,0],[[2,8,32],[4,4,96],[4,8,160],[8,4,96],[8,8,192]]",1018));//[[2,0],[0,0]],[[2,1],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,5,7,15,240,31,2,[1,3,12,16,0,0],[[2,8,48],[4,4,96],[4,8,192],[8,2,32],[8,4,160],[8,8,256]]",1019));//[[1,0],[0,0]],[[2,0],[2,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,11,23,448,31,2,[1,3,12,16,0,0],[[4,4,96],[4,8,96],[8,2,32],[8,4,160],[8,8,192]]",1020));//[[2,0],[0,0]],[[2,0],[1,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,23,288,31,32,[1,3,8,12,8,0],[[2,8,32],[4,4,128],[4,8,160],[8,2,32],[8,4,160],[8,8,224]]",1021));//[[2,0],[0,2]],[[2,1],[0,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,11,23,384,31,2,[1,3,12,16,0,0],[[2,8,32],[4,4,96],[4,8,160],[8,4,160],[8,8,192]]",1022));//[[0,0],[3,0]],[[4,0],[2,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,23,288,31,32,[1,3,8,12,8,0],[[2,8,32],[4,4,128],[4,8,160],[8,2,32],[8,4,160],[8,8,224]]",1023));//[[2,0],[6,2]],[[6,0],[5,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,15,384,31,32,[1,3,12,8,8,0],[[4,8,192],[8,4,192],[8,8,256]]",1024));//[[2,0],[0,6]],[[8,0],[8,12]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,15,384,31,32,[1,3,12,0,0,16],[[4,8,192],[8,4,192],[8,8,256]]",1025));//[[2,2],[0,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,23,31,768,31,8,[1,3,12,16,0,0],[[4,8,128],[8,8,128]]",1026));//[[4,0],[0,0]],[[0,2],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,23,31,768,31,8,[1,3,12,16,0,0],[[8,4,128],[8,8,128]]",1027));//[[4,0],[0,0]],[[8,0],[2,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,31,31,640,31,32,[1,3,12,16,0,0],[[4,8,128],[8,4,128],[8,8,128]]",1028));//[[4,0],[0,4]],[[8,2],[0,8]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,15,448,31,8,[1,3,12,16,0,0],[[4,8,192],[8,4,128],[8,8,256]]",1029));//[[0,4],[0,0]],[[0,0],[0,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,15,448,31,8,[1,3,12,16,0,0],[[4,8,128],[8,4,192],[8,8,256]]",1030));//[[0,4],[0,0]],[[2,8],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,512,31,32,[1,3,12,0,0,16],[[4,8,128],[8,4,128],[8,8,256]]",1031));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,7,15,288,31,8,[1,7,8,8,8,0],[[2,8,64],[4,4,64],[4,8,128],[8,2,96],[8,4,128],[8,8,256]]",2000));//[[4,0],[3,1]],[[0,0],[4,0]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,512,31,8,[1,7,8,0,16,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,256]]",2001));//[[0,6],[0,0]],[[6,7],[4,4]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,15,512,31,8,[1,7,8,0,16,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,256]]",2002));//[[4,0],[0,0]],[[4,7],[4,4]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,7,15,288,31,8,[1,7,8,8,8,0],[[2,8,96],[4,4,64],[4,8,128],[8,2,64],[8,4,128],[8,8,256]]",2003));//[[1,4],[0,0]],[[7,4],[4,4]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,5,7,15,256,31,4,[1,7,8,16,0,0],[[2,8,96],[4,4,64],[4,8,128],[8,2,96],[8,4,128],[8,8,256]]",2004));//[[5,4],[4,0]],[[7,0],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,1,16,2,7,15,224,15,32,[1,7,8,4,12,0],[[2,8,112],[4,4,64],[4,8,128],[8,2,112],[8,4,128],[8,8,256]]",2005));//[[4,4],[4,4]],[[5,0],[4,5]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,15,448,31,8,[1,7,8,8,8,0],[[2,8,64],[4,8,128],[8,4,128],[8,8,256]]",2006));
	/*---------------begin of R32_2012~R32_2043----------------*/
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,31,31,1024,31,32,[1,7,8,16,0,0],[]",2023));//R8R4_4=R8_1×R4_4
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,15,512,31,32,[1,7,8,16,0,0],[[4,8,128],[8,4,128],[8,8,256]]",2015));//R8R4_15=R8_2×R4_4
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,15,320,31,32,[1,7,8,4,12,0],[[2,8,64],[4,4,64],[4,8,128],[8,2,64],[8,4,128],[8,8,256]]",2025));//R8R4_26=R8_3×R4_4
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,768,31,32,[1,7,8,16,0,0],[[8,8,256]]",2036));//R8R4_37=R8_4×R4_4
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,768,31,32,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,64]]",2024));//R8R4_5=R8_1×R4_5
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,7,15,384,31,32,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,128],[8,2,32],[8,4,128],[8,8,256]]",2016));//R8R4_16=R8_2×R4_5
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,3,15,240,31,32,[1,3,8,8,4,8],[[2,2,16],[2,4,16],[2,8,80],[4,2,16],[4,4,64],[4,8,128],[8,2,80],[8,4,128],[8,8,256]]",2026));//R8R4_27=R8_3×R4_5
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,7,31,576,31,32,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,256]]",2037));//R8R4_38=R8_4×R4_5
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,15,15,768,31,32,[1,4,7,12,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,64]]",2033));//R8R4_6=R8_1×R4_6
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,7,384,31,32,[1,4,7,12,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,128],[8,2,32],[8,4,128],[8,8,256]]",2017));//R8R4_17=R8_2×R4_6
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,4,3,7,240,31,32,[1,4,7,12,8,0],[[2,2,16],[2,4,16],[2,8,80],[4,2,16],[4,4,64],[4,8,128],[8,2,80],[8,4,128],[8,8,256]]",2027));//R8R4_28=R8_3×R4_6
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,15,576,31,32,[1,4,7,12,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,256]]",2038));//R8R4_39=R8_4×R4_6
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,15,15,640,31,8,[1,5,6,12,8,0],[[2,2,24],[2,4,24],[2,8,48],[4,2,24],[4,4,24],[4,8,48],[8,2,48],[8,4,48],[8,8,96]]",2034));//R8R4_7=R8_1×R4_7
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,7,7,320,31,8,[1,5,6,12,8,0],[[2,2,24],[2,4,24],[2,8,48],[4,2,24],[4,4,24],[4,8,128],[8,2,48],[8,4,128],[8,8,256]]",2022));//R8R4_18=R8_2×R4_7
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,6,3,7,200,23,8,[1,5,6,16,4,0],[[2,2,24],[2,4,24],[2,8,88],[4,2,24],[4,4,64],[4,8,128],[8,2,88],[8,4,128],[8,8,256]]",2028));//R8R4_29=R8_3×R4_7
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,7,15,480,31,8,[1,5,6,12,8,0],[[2,2,24],[2,4,24],[2,8,48],[4,2,24],[4,4,24],[4,8,48],[8,2,48],[8,4,48],[8,8,256]]",2039));//R8R4_40=R8_4×R4_7
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,15,15,640,31,8,[1,5,6,12,8,0],[[2,2,24],[2,4,24],[2,8,48],[4,2,24],[4,4,24],[4,8,48],[8,2,48],[8,4,48],[8,8,96]]",2035));//R8R4_8=R8_1×R4_8
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,7,7,320,31,8,[1,5,6,12,8,0],[[2,2,24],[2,4,24],[2,8,48],[4,2,24],[4,4,24],[4,8,128],[8,2,48],[8,4,128],[8,8,256]]",2018));//R8R4_19=R8_2×R4_8
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,6,3,7,200,31,8,[1,5,6,16,4,0],[[2,2,24],[2,4,24],[2,8,88],[4,2,24],[4,4,64],[4,8,128],[8,2,88],[8,4,128],[8,8,256]]",2029));//R8R4_30=R8_3×R4_8
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,7,15,480,31,8,[1,5,6,12,8,0],[[2,2,24],[2,4,24],[2,8,48],[4,2,24],[4,4,24],[4,8,48],[8,2,48],[8,4,48],[8,8,256]]",2040));//R8R4_41=R8_4×R4_8
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,15,15,512,31,32,[1,4,6,11,6,4],[[2,2,32],[2,4,32],[2,8,64],[4,2,32],[4,4,32],[4,8,64],[8,2,64],[8,4,64],[8,8,128]]",2012));//R8R4_9=R8_1×R4_9
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,7,256,31,32,[1,4,6,11,6,4],[[2,2,32],[2,4,32],[2,8,64],[4,2,32],[4,4,32],[4,8,128],[8,2,64],[8,4,128],[8,8,256]]",2019));//R8R4_20=R8_2×R4_9
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,1,24,4,3,7,160,23,32,[1,4,6,11,10,0],[[2,2,32],[2,4,32],[2,8,96],[4,2,32],[4,4,64],[4,8,128],[8,2,96],[8,4,128],[8,8,256]]",2030));//R8R4_31=R8_3×R4_9
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,15,384,31,32,[1,4,6,11,6,4],[[2,2,32],[2,4,32],[2,8,64],[4,2,32],[4,4,32],[4,8,64],[8,2,64],[8,4,64],[8,8,256]]",2041));//R8R4_42=R8_4×R4_9
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,4,7,7,576,31,32,[1,4,5,10,12,0],[[2,2,28],[2,4,28],[2,8,56],[4,2,28],[4,4,28],[4,8,56],[8,2,56],[8,4,56],[8,8,112]]",2013));//R8R4_10=R8_1×R4_10
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,4,3,3,288,31,32,[1,4,5,10,12,0],[[2,2,28],[2,4,28],[2,8,56],[4,2,28],[4,4,28],[4,8,128],[8,2,56],[8,4,128],[8,8,256]]",2020));//R8R4_21=R8_2×R4_10
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,1,28,8,1,3,180,27,32,[1,4,5,22,0,0],[[2,2,28],[2,4,28],[2,8,92],[4,2,28],[4,4,64],[4,8,128],[8,2,92],[8,4,128],[8,8,256]]",2031));//R8R4_32=R8_3×R4_10
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,4,3,7,432,31,32,[1,4,5,10,12,0],[[2,2,28],[2,4,28],[2,8,56],[4,2,28],[4,4,28],[4,8,56],[8,2,56],[8,4,56],[8,8,256]]",2042));//R8R4_43=R8_4×R4_10
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,7,448,31,32,[1,2,5,8,8,8],[[2,2,36],[2,4,36],[2,8,72],[4,2,36],[4,4,36],[4,8,72],[8,2,72],[8,4,72],[8,8,144]]",2014));//R8R4_11=R8_1×R4_11
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,3,3,224,31,32,[1,2,5,8,8,8],[[2,2,36],[2,4,36],[2,8,72],[4,2,36],[4,4,36],[4,8,128],[8,2,72],[8,4,128],[8,8,256]]",2021));//R8R4_22=R8_2×R4_11
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,1,20,4,1,3,140,19,32,[1,2,5,12,4,8],[[2,2,36],[2,4,36],[2,8,100],[4,2,36],[4,4,64],[4,8,128],[8,2,100],[8,4,128],[8,8,256]]",2032));//R8R4_33=R8_3×R4_11
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,3,7,336,31,32,[1,2,5,8,8,8],[[2,2,36],[2,4,36],[2,8,72],[4,2,36],[4,4,36],[4,8,72],[8,2,72],[8,4,72],[8,8,256]]",2043));//R8R4_44=R8_4×R4_11	
	/*---------------end of R32_2012~R32_2043----------------*/	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,16,2,7,7,160,15,32,[1,7,2,14,0,8],[[2,4,144],[4,2,144],[4,4,576]]",3000));//R4_3/([[1,1,1]],[0,1,0,1])	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,7,192,31,32,[1,7,2,12,6,4],[[2,4,128],[4,2,128],[4,4,576]]",3001));//R4_3/([[0,0,1],[2,1]],[0,1,0,1])
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,5,11,15,288,31,2,[1,7,16,8,0,0],[[2,4,128],[4,2,96],[4,4,512]]",3002));//[[3,0],[0,0]],[[2,0],[3,2]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,1,16,2,11,15,192,15,8,[1,7,10,6,8,0],[[2,4,144],[4,2,144],[4,4,544]]",3003));//[[0,1],[1,0]],[[2,2],[0,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,1,24,6,7,7,200,23,8,[1,7,10,14,0,0],[[2,4,136],[4,2,136],[4,4,552]]",3004));//[[3,1],[0,0]],[[1,2],[2,1]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,5,11,15,288,31,2,[1,7,16,8,0,0],[[2,4,96],[4,2,128],[4,4,512]]",3005));//[[3,2],[1,0]],[[1,1],[1,1]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,1,16,2,7,7,160,15,8,[1,7,2,14,8,0],[[2,4,144],[4,2,144],[4,4,576]]",3006));//[[2,1],[3,2]],[[2,3],[3,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,7,160,31,32,[1,7,2,4,10,8],[[2,4,144],[4,2,144],[4,4,576]]",3007));//R4_3/([[2,0,1]],[0,1,1,1])	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,224,31,32,[1,7,10,4,6,4],[[2,4,128],[4,2,128],[4,4,544]]",3008));//R4_3/([[1,1,2]],[0,0,1,1])
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,16,2,15,15,192,15,32,[1,7,10,6,8,0],[[2,4,144],[4,2,144],[4,4,544]]",3009));//R4_3/([[1,0,2],[1,1,1]],[0,0,1,1])	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,16,2,7,15,192,15,32,[1,7,10,6,8,0],[[2,4,144],[4,2,144],[4,4,544]]",3010));//R4_3/([[0,0,1],[0,2]],[1,1,1,1])	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,15,288,31,32,[1,7,8,0,16,0],[[2,4,128],[4,2,128],[4,4,480]]",3011));//R4_3/([[1,0,1],[1,1]],[1,1,1,1])
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,15,192,31,32,[1,7,0,8,0,16],[[2,4,128],[4,2,128],[4,4,576]]",3012));//[[2,1,1,2],[0,2,1,2],[0,2,0,2],[2,1,0,2]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,15,320,31,32,[1,7,8,0,16,0],[[2,4,64],[4,2,64],[4,4,576]]",3013));//[[2,3,3,0],[0,0,2,2],[0,2,2,0],[0,2,3,0]],[[0,2,1,2],[0,2,0,0],[0,0,2,0],[0,2,2,2]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,16,2,7,15,160,15,32,[1,7,2,14,0,8],[[2,4,144],[4,2,144],[4,4,576]]",3014));//[[0,2,2,3],[3,1,3,3],[2,2,2,0],[3,2,3,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,224,31,32,[1,7,10,4,6,4],[[2,4,128],[4,2,128],[4,4,544]]",3015));//[[2,0,0,2],[1,0,3,1],[2,3,0,1],[2,0,0,2]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,16,8,0,0],[[4,4,512]]",3016));//KW1(2,1)	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,7,15,208,31,8,[1,7,12,4,8,0],[[2,4,144],[4,2,128],[4,4,544]]",3017));//[[0,0,0],[1,0,1],[1,1,0]],[[0,2,2],[0,2,2],[0,2,2]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,15,320,31,8,[1,7,8,0,16,0],[[2,4,64],[4,2,64],[4,4,576]]",3018));//[[0,1,1],[1,1,1],[1,1,1]],[[0,0,0],[2,2,2],[2,2,2]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,16,8,0,0],[[4,4,384]]",3019));//[[4,0,0],[2,0,2],[2,0,2]],[[0,0,0],[0,0,6],[0,0,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,23,288,31,32,[1,7,8,0,16,0],[[2,4,128],[4,2,128],[4,4,480]]",3020));//[[0,2,0],[2,0,0],[0,2,2]],[[3,1,2],[3,3,2],[1,2,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,5,11,15,352,31,2,[1,7,16,8,0,0],[[2,4,96],[4,2,96],[4,4,480]]",3021));//[[1,1,1,1],[1,1,1,1],[1,1,1,1],[1,1,1,1]],[[2,0,0,2],[2,0,0,2],[1,1,1,1],[2,0,0,2]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,15,15,208,15,8,[1,7,12,4,8,0],[[2,4,128],[4,2,144],[4,4,544]]",3022));//[[1,1,0],[0,1,0],[0,1,0]],[[0,2,2],[0,0,0],[0,0,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,5,11,15,272,15,2,[1,7,16,8,0,0],[[2,4,128],[4,2,112],[4,4,512]]",3023));//[[0,1,1],[0,0,0],[0,0,0]],[[1,2,0],[0,1,0],[0,1,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,5,11,15,272,31,2,[1,7,16,8,0,0],[[2,4,112],[4,2,128],[4,4,512]]",3024));//[[1,0,0],[0,1,0],[0,0,0]],[[0,0,0],[2,0,0],[1,0,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,15,15,208,31,8,[1,7,12,4,8,0],[[2,4,144],[4,2,128],[4,4,544]]",3025));//[[2,0,0],[0,2,0],[0,0,0]],[[1,0,0],[1,1,0],[1,0,0]]
	/*---------------begin of R32_3044~R32_3103----------------*/
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,31,31,1024,31,32,[1,7,24,0,0,0],[]",3044));//R8R4_45=R8_5×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,144]]",3055));//R8R4_56=R8_6×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,768,31,32,[1,4,15,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,144]]",3066));//R8R4_67=R8_7×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,7,20,4,0,0],[[2,4,64],[4,4,192]]",3077));//R8R4_78=R8_8×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,144]]",3088));//R8R4_89=R8_9×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,7,20,4,0,0],[[4,2,64],[4,4,192]]",3099));//R8R4_100=R8_10×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,31,31,640,31,32,[1,7,24,0,0,0],[[2,4,64],[4,2,64],[4,4,256]]",3053));//R8R4_111=R8_11×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,640,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,272]]",3070));//R8R4_122=R8_12×R4_1
	//m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,512,31,32,[1,7,10,6,8,0],[[2,4,64],[4,2,64],[4,4,384]]",3084));//R8R4_133=R8_13×R4_1
	//m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,4,7,7,384,31,32,[1,4,11,8,8,0],[[2,2,16],[2,4,96],[4,2,96],[4,4,432]]",3098));//R8R4_144=R8_14×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,15,15,448,31,8,[1,7,12,4,8,0],[[2,4,96],[4,2,64],[4,4,416]]",3061));//R8R4_155=R8_15×R4_1
	//m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,7,16,8,0,0],[[4,4,256]]",3081));//R8R4_166=R8_16×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,576,31,32,[1,4,11,12,4,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]]",3047));//R8R4_177=R8_17×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,640,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,272]]",3073));//R8R4_188=R8_18×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,640,31,32,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,256]]",3096));//R8R4_199=R8_19×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,15,15,448,31,8,[1,7,12,4,8,0],[[2,4,64],[4,2,96],[4,4,416]]",3080));//R8R4_210=R8_20×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,384,31,32,[1,7,10,4,6,4],[[2,4,96],[4,2,96],[4,4,448]]",3064));//R8R4_221=R8_21×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,320,31,32,[1,3,10,8,6,4],[[2,2,16],[2,4,112],[4,2,112],[4,4,464]]",3060));//R8R4_232=R8_22×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,7,8,16,0,0],[[4,4,256]]",3095));//R8R4_243=R8_23×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,15,512,31,32,[1,7,8,8,8,0],[[2,4,64],[4,2,64],[4,4,384]]",3094));//R8R4_254=R8_24×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,7,16,8,0,0],[[4,4,256]]",3045));//R8R4_46=R8_5×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]]",3056));//R8R4_57=R8_6×R4_2
	//m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,576,31,32,[1,4,11,12,4,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]]",3067));//R8R4_68=R8_7×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,7,20,4,0,0],[[2,4,64],[4,4,384]]",3078));//R8R4_79=R8_8×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]]",3089));//R8R4_90=R8_9×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,7,20,4,0,0],[[4,2,64],[4,4,384]]",3100));//R8R4_101=R8_10×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,480,31,32,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,416]]",3054));//R8R4_112=R8_11×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,480,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3071));//R8R4_123=R8_12×R4_2
	//m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,7,10,6,8,0],[[2,4,64],[4,2,64],[4,4,512]]",3085));//R8R4_134=R8_13×R4_2
	//m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,4,3,7,288,31,32,[1,4,11,8,8,0],[[2,2,16],[2,4,96],[4,2,96],[4,4,528]]",3102));//R8R4_145=R8_14×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,7,15,336,31,8,[1,7,12,4,8,0],[[2,4,96],[4,2,64],[4,4,528]]",3062));//R8R4_156=R8_15×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,7,24,0,0,0],[[4,4,448]]",3082));//R8R4_167=R8_16×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,3,15,432,31,32,[1,4,15,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,480]]",3048));//R8R4_178=R8_17×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,480,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3074));//R8R4_189=R8_18×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,480,31,32,[1,7,24,0,0,0],[[2,4,64],[4,2,64],[4,4,416]]",3097));//R8R4_200=R8_19×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,7,15,336,31,8,[1,7,12,4,8,0],[[2,4,64],[4,2,96],[4,4,528]]",3087));//R8R4_211=R8_20×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,288,31,32,[1,7,10,4,6,4],[[2,4,96],[4,2,96],[4,4,544]]",3065));//R8R4_222=R8_21×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,3,15,240,31,32,[1,3,10,8,6,4],[[2,2,16],[2,4,112],[4,2,112],[4,4,544]]",3076));//R8R4_233=R8_22×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,7,8,16,0,0],[[4,4,448]]",3051));//R8R4_244=R8_23×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,15,384,31,32,[1,7,8,8,8,0],[[2,4,64],[4,2,64],[4,4,512]]",3059));//R8R4_255=R8_24×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,512,31,32,[1,7,10,6,8,0],[[2,4,64],[4,2,64],[4,4,384]]",3046));//R8R4_47=R8_5×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,3,14,2,12,0],[[2,2,16],[2,4,96],[4,2,96],[4,4,432]]",3057));//R8R4_58=R8_6×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,4,7,7,384,31,32,[1,4,11,8,8,0],[[2,2,16],[2,4,96],[4,2,96],[4,4,432]]",3068));//R8R4_69=R8_7×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,11,15,384,31,8,[1,7,10,6,8,0],[[2,4,112],[4,2,64],[4,4,464]]",3079));//R8R4_80=R8_8×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,11,15,384,31,8,[1,3,14,2,12,0],[[2,2,16],[2,4,96],[4,2,96],[4,4,432]]",3090));//R8R4_91=R8_9×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,11,15,384,31,8,[1,7,10,6,8,0],[[2,4,64],[4,2,112],[4,4,464]]",3101));//R8R4_102=R8_10×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,320,31,32,[1,7,10,6,8,0],[[2,4,112],[4,2,112],[4,4,480]]",3058));//R8R4_113=R8_11×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,320,31,32,[1,3,14,2,12,0],[[2,2,16],[2,4,96],[4,2,96],[4,4,496]]",3072));//R8R4_124=R8_12×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,4,7,7,256,31,32,[1,7,6,16,2,0],[[2,4,112],[4,2,112],[4,4,544]]",3086));//R8R4_135=R8_13×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,28,8,3,3,192,27,32,[1,4,15,12,0,0],[[2,2,16],[2,4,132],[4,2,132],[4,4,552]]",3103));//R8R4_146=R8_14×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,6,7,7,224,31,8,[1,7,10,14,0,0],[[2,4,136],[4,2,112],[4,4,552]]",3063));//R8R4_157=R8_15×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,7,10,6,8,0],[[2,4,64],[4,2,64],[4,4,512]]",3083));//R8R4_168=R8_16×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,4,3,7,288,31,32,[1,4,11,8,8,0],[[2,2,16],[2,4,96],[4,2,96],[4,4,528]]",3049));//R8R4_179=R8_17×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,3,15,320,31,8,[1,3,14,2,12,0],[[2,2,16],[2,4,96],[4,2,96],[4,4,496]]",3075));//R8R4_190=R8_18×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,320,31,32,[1,7,10,6,8,0],[[2,4,112],[4,2,112],[4,4,480]]",3050));//R8R4_201=R8_19×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,6,7,7,224,23,8,[1,7,10,14,0,0],[[2,4,112],[4,2,136],[4,4,552]]",3091));//R8R4_212=R8_20×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,24,4,7,7,192,23,32,[1,7,6,16,2,0],[[2,4,136],[4,2,136],[4,4,560]]",3069));//R8R4_223=R8_21×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,24,4,3,7,160,23,32,[1,3,10,8,10,0],[[2,2,16],[2,4,144],[4,2,144],[4,4,560]]",3092));//R8R4_234=R8_22×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,7,2,14,0,8],[[2,4,64],[4,2,64],[4,4,512]]",3052));//R8R4_245=R8_23×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,7,256,31,32,[1,7,2,14,0,8],[[2,4,112],[4,2,112],[4,4,544]]",3093));//R8R4_256=R8_24×R4_3	
	/*---------------end of R32_3044~R32_3103----------------*/	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,23,416,31,4,[1,15,8,8,0,0],[[2,4,192],[4,2,192],[4,4,224]]",4000));//[[0,2],[1,0]],[[0,0],[3,0]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,15,15,384,31,4,[1,15,8,8,0,0],[[2,4,192],[4,2,192],[4,4,256]]",4001));//[[3,2],[1,0]],[[0,2],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,15,0,16,0,0],[[2,4,128],[4,4,256]]",4002));//[[2,0,0],[1,0,1],[2,0,2]],[[2,2,2],[2,2,2],[2,2,2]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,352,31,8,[1,15,4,8,4,0],[[2,4,224],[4,2,192],[4,4,256]]",4003));//[[2,2,2],[2,2,2],[2,2,2]],[[0,0,0],[3,1,2],[3,2,1]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,15,448,31,8,[1,15,0,16,0,0],[[2,4,128],[4,2,192],[4,4,256]]",4004));//[[2,1,0,2],[0,2,0,2],[0,2,0,2],[2,1,0,2]],[[2,2,2,2],[2,2,2,2],[2,2,2,2],[2,2,2,2]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,15,512,31,8,[1,15,0,0,16,0],[[2,4,128],[4,2,128],[4,4,256]]",4005));//[[0,0,0,0],[0,0,0,0],[2,2,2,2],[2,2,2,2]],[[2,2,2,2],[0,2,2,2],[1,0,2,2],[0,3,0,2]]
	/*---------------begin of R32_4104~R32_4347----------------*/
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,31,31,1024,31,32,[1,15,16,0,0,0],[]",4104));//R8R4_48=R8_5×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,7,16,8,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,64]]",4105));//R8R4_49=R8_5×R4_5	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,768,31,32,[1,8,15,8,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,64]]",4106));//R8R4_50=R8_5×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,640,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4107));//R8R4_51=R8_5×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,640,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4108));//R8R4_52=R8_5×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,512,31,32,[1,8,12,7,4,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]]",4109));//R8R4_53=R8_5×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,576,31,32,[1,6,13,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4110));//R8R4_54=R8_5×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,448,31,32,[1,4,9,10,8,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,144]]",4111));//R8R4_55=R8_5×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,7,24,0,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,64]]",4112));//R8R4_59=R8_6×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4113));//R8R4_60=R8_6×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32,[1,4,15,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4114));//R8R4_61=R8_6×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8,[1,5,14,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]]",4115));//R8R4_62=R8_6×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8,[1,5,14,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]]",4116));//R8R4_63=R8_6×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,4,14,7,6,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,160]]",4117));//R8R4_64=R8_6×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,432,31,32,[1,4,9,18,0,0],[[2,2,148],[2,4,148],[4,2,148],[4,4,148]]",4118));//R8R4_65=R8_6×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,336,31,32,[1,2,9,8,12,0],[[2,2,172],[2,4,172],[4,2,172],[4,4,172]]",4119));//R8R4_66=R8_6×R4_11
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,768,31,32,[1,8,15,8,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,64]]",4120));//R8R4_70=R8_7×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32,[1,4,11,12,4,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4121));//R8R4_71=R8_7×R4_5
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,576,31,32,[1,6,13,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4122));//R8R4_72=R8_7×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,480,31,8,[1,8,11,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]]",4123));//R8R4_73=R8_7×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,480,31,8,[1,8,11,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]]",4124));//R8R4_74=R8_7×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,384,31,32,[1,6,11,10,4,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,160]]",4125));//R8R4_75=R8_7×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,3,3,432,31,32,[1,8,9,14,0,0],[[2,2,148],[2,4,148],[4,2,148],[4,4,148]]",4126));//R8R4_76=R8_7×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,3,336,31,32,[1,4,7,10,6,4],[[2,2,172],[2,4,172],[4,2,172],[4,4,172]]",4127));//R8R4_77=R8_7×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,15,16,0,0,0],[[2,4,128],[4,4,128]]",4128));//R8R4_81=R8_8×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,64],[4,4,160]]",4129));//R8R4_82=R8_8×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,576,31,8,[1,8,15,8,0,0],[[2,2,64],[2,4,160],[4,2,64],[4,4,160]]",4130));//R8R4_83=R8_8×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,11,15,480,31,2,[1,9,14,8,0,0],[[2,2,96],[2,4,176],[4,2,96],[4,4,176]]",4131));//R8R4_84=R8_8×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,11,15,480,31,2,[1,9,14,8,0,0],[[2,2,96],[2,4,176],[4,2,96],[4,4,176]]",4132));//R8R4_85=R8_8×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,384,31,8,[1,8,12,7,4,0],[[2,2,128],[2,4,192],[4,2,128],[4,4,192]]",4133));//R8R4_86=R8_8×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,4,5,7,432,31,8,[1,6,13,12,0,0],[[2,2,112],[2,4,184],[4,2,112],[4,4,184]]",4134));//R8R4_87=R8_8×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,5,7,336,31,8,[1,4,9,10,8,0],[[2,2,144],[2,4,200],[4,2,144],[4,4,200]]",4135));//R8R4_88=R8_8×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,64]]",4136));//R8R4_92=R8_9×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4137));//R8R4_93=R8_9×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,576,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4138));//R8R4_94=R8_9×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,11,15,480,31,2,[1,5,14,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]]",4139));//R8R4_95=R8_9×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,11,15,480,31,2,[1,5,14,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]]",4140));//R8R4_96=R8_9×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,384,31,8,[1,4,14,7,6,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,160]]",4141));//R8R4_97=R8_9×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,4,5,7,432,31,8,[1,4,9,18,0,0],[[2,2,148],[2,4,148],[4,2,148],[4,4,148]]",4142));//R8R4_98=R8_9×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,5,7,336,31,8,[1,2,9,8,12,0],[[2,2,172],[2,4,172],[4,2,172],[4,4,172]]",4143));//R8R4_99=R8_9×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,15,16,0,0,0],[[4,2,128],[4,4,128]]",4144));//R8R4_103=R8_10×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,160]]",4145));//R8R4_104=R8_10×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,576,31,8,[1,8,15,8,0,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,160]]",4146));//R8R4_105=R8_10×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,11,15,480,31,2,[1,9,14,8,0,0],[[2,2,96],[2,4,96],[4,2,176],[4,4,176]]",4147));//R8R4_106=R8_10×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,11,15,480,31,2,[1,9,14,8,0,0],[[2,2,96],[2,4,96],[4,2,176],[4,4,176]]",4148));//R8R4_107=R8_10×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,384,31,8,[1,8,12,7,4,0],[[2,2,128],[2,4,128],[4,2,192],[4,4,192]]",4149));//R8R4_108=R8_10×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,4,5,7,432,31,8,[1,6,13,12,0,0],[[2,2,112],[2,4,112],[4,2,184],[4,4,184]]",4150));//R8R4_109=R8_10×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,5,7,336,31,8,[1,4,9,10,8,0],[[2,2,144],[2,4,144],[4,2,200],[4,4,200]]",4151));//R8R4_110=R8_10×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,31,31,640,31,32,[1,15,16,0,0,0],[[2,4,128],[4,2,128],[4,4,128]]",4152));//R8R4_114=R8_11×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,640,31,32,[1,7,24,0,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,192]]",4160));//R8R4_125=R8_12×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,480,31,32,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]]",4153));//R8R4_115=R8_11×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,480,31,32,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4161));//R8R4_126=R8_12×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,480,31,32,[1,8,15,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]]",4154));//R8R4_116=R8_11×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,480,31,32,[1,4,15,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4162));//R8R4_127=R8_12×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,400,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,176]]",4155));//R8R4_117=R8_11×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,400,31,8,[1,5,14,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,216]]",4163));//R8R4_128=R8_12×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,400,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,176]]",4156));//R8R4_118=R8_11×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,400,31,8,[1,5,14,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,216]]",4164));//R8R4_129=R8_12×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,320,31,32,[1,8,12,7,4,0],[[2,2,128],[2,4,192],[4,2,192],[4,4,192]]",4157));//R8R4_119=R8_11×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,320,31,32,[1,4,14,7,6,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,224]]",4165));//R8R4_130=R8_12×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,360,31,32,[1,6,13,12,0,0],[[2,2,112],[2,4,184],[4,2,184],[4,4,184]]",4158));//R8R4_120=R8_11×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,360,31,32,[1,4,9,18,0,0],[[2,2,148],[2,4,148],[4,2,148],[4,4,220]]",4166));//R8R4_131=R8_12×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,280,31,32,[1,4,9,10,8,0],[[2,2,144],[2,4,200],[4,2,200],[4,4,200]]",4159));//R8R4_121=R8_11×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,280,31,32,[1,2,9,8,12,0],[[2,2,172],[2,4,172],[4,2,172],[4,4,228]]",4167));//R8R4_132=R8_12×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,512,31,32,[1,15,2,14,0,0],[[2,4,128],[4,2,128],[4,4,256]]",4168));//R8R4_136=R8_13×R4_4
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,384,31,32,[1,8,11,12,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,256]]",4176));//R8R4_147=R8_14×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,448,31,8,[1,15,4,12,0,0],[[2,4,192],[4,2,128],[4,4,256]]",4184));//R8R4_158=R8_15×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,15,16,0,0,0],[[4,4,256]]",4192));//R8R4_169=R8_16×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,7,10,6,8,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,256]]",4169));//R8R4_137=R8_13×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,288,31,32,[1,4,11,8,8,0],[[2,2,112],[2,4,184],[4,2,184],[4,4,256]]",4177));//R8R4_148=R8_14×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,336,31,8,[1,7,12,4,8,0],[[2,2,64],[2,4,208],[4,2,160],[4,4,256]]",4185));//R8R4_159=R8_15×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,7,16,8,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,256]]",4193));//R8R4_170=R8_16×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,384,31,32,[1,8,11,12,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,256]]",4170));//R8R4_138=R8_13×R4_6
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,3,3,288,31,32,[1,6,17,8,0,0],[[2,2,112],[2,4,184],[4,2,184],[4,4,256]]",4178));//R8R4_149=R8_14×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,336,31,8,[1,8,15,8,0,0],[[2,2,64],[2,4,208],[4,2,160],[4,4,256]]",4186));//R8R4_160=R8_15×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32,[1,8,15,8,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,256]]",4194));//R8R4_171=R8_16×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,320,31,8,[1,9,12,10,0,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,256]]",4171));//R8R4_139=R8_13×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,12,3,3,240,27,8,[1,8,19,4,0,0],[[2,2,136],[2,4,196],[4,2,196],[4,4,256]]",4179));//R8R4_150=R8_14×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,280,31,2,[1,9,18,4,0,0],[[2,2,96],[2,4,216],[4,2,176],[4,4,256]]",4187));//R8R4_161=R8_15×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]]",4195));//R8R4_172=R8_16×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,320,31,8,[1,9,12,10,0,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,256]]",4172));//R8R4_140=R8_13×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,12,3,3,240,31,8,[1,8,19,4,0,0],[[2,2,136],[2,4,196],[4,2,196],[4,4,256]]",4180));//R8R4_151=R8_14×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,280,31,2,[1,9,18,4,0,0],[[2,2,96],[2,4,216],[4,2,176],[4,4,256]]",4188));//R8R4_162=R8_15×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]]",4196));//R8R4_173=R8_16×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,256,31,32,[1,8,8,13,2,0],[[2,2,128],[2,4,192],[4,2,192],[4,4,256]]",4173));//R8R4_141=R8_13×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,28,8,3,3,192,27,32,[1,6,15,10,0,0],[[2,2,160],[2,4,208],[4,2,208],[4,4,256]]",4181));//R8R4_152=R8_14×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,224,31,8,[1,8,12,11,0,0],[[2,2,128],[2,4,224],[4,2,192],[4,4,256]]",4189));//R8R4_163=R8_15×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,8,12,7,4,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,256]]",4197));//R8R4_174=R8_16×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,3,3,288,31,32,[1,6,17,8,0,0],[[2,2,112],[2,4,184],[4,2,184],[4,4,256]]",4174));//R8R4_142=R8_13×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,30,16,1,1,216,29,32,[1,8,23,0,0,0],[[2,2,148],[2,4,202],[4,2,202],[4,4,256]]",4182));//R8R4_153=R8_14×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,12,3,3,252,31,8,[1,6,25,0,0,0],[[2,2,112],[2,4,220],[4,2,184],[4,4,256]]",4190));//R8R4_164=R8_15×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,432,31,32,[1,6,13,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,256]]",4198));//R8R4_175=R8_16×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,3,224,31,32,[1,4,9,10,4,4],[[2,2,144],[2,4,200],[4,2,200],[4,4,256]]",4175));//R8R4_143=R8_13×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,26,8,1,1,168,25,32,[1,4,13,4,10,0],[[2,2,172],[2,4,214],[4,2,214],[4,4,256]]",4183));//R8R4_154=R8_14×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,3,196,31,8,[1,4,13,6,8,0],[[2,2,144],[2,4,228],[4,2,200],[4,4,256]]",4191));//R8R4_165=R8_15×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,336,31,32,[1,4,9,10,8,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,256]]",4199));//R8R4_176=R8_16×R4_11
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32,[1,8,15,8,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,256]]",4200));//R8R4_180=R8_17×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,640,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,192]]",4208));//R8R4_191=R8_18×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,640,31,32,[1,15,16,0,0,0],[[2,4,128],[4,2,128],[4,4,128]]",4216));//R8R4_202=R8_19×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,448,31,8,[1,15,4,12,0,0],[[2,4,128],[4,2,192],[4,4,256]]",4224));//R8R4_213=R8_20×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,15,432,31,32,[1,4,11,12,4,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,256]]",4201));//R8R4_181=R8_17×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,480,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4209));//R8R4_192=R8_18×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,480,31,32,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]]",4217));//R8R4_203=R8_19×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,336,31,8,[1,7,12,4,8,0],[[2,2,64],[2,4,160],[4,2,208],[4,4,256]]",4225));//R8R4_214=R8_20×R4_5
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,432,31,32,[1,6,13,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,256]]",4202));//R8R4_182=R8_17×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,480,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4210));//R8R4_193=R8_18×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,480,31,32,[1,8,15,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]]",4218));//R8R4_204=R8_19×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,336,31,8,[1,8,15,8,0,0],[[2,2,64],[2,4,160],[4,2,208],[4,4,256]]",4226));//R8R4_215=R8_20×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,360,31,8,[1,8,11,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,256]]",4203));//R8R4_183=R8_17×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,3,15,400,31,2,[1,5,14,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,216]]",4211));//R8R4_194=R8_18×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,400,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,176]]",4219));//R8R4_205=R8_19×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,280,23,2,[1,9,18,4,0,0],[[2,2,96],[2,4,176],[4,2,216],[4,4,256]]",4227));//R8R4_216=R8_20×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,360,31,8,[1,8,11,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,256]]",4204));//R8R4_184=R8_17×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,3,15,400,31,2,[1,5,14,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,216]]",4212));//R8R4_195=R8_18×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,400,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,176]]",4220));//R8R4_206=R8_19×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,280,31,2,[1,9,18,4,0,0],[[2,2,96],[2,4,176],[4,2,216],[4,4,256]]",4228));//R8R4_217=R8_20×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,288,31,32,[1,6,11,10,4,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,256]]",4205));//R8R4_185=R8_17×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,320,31,8,[1,4,14,7,6,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,224]]",4213));//R8R4_196=R8_18×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,320,31,32,[1,8,12,7,4,0],[[2,2,128],[2,4,192],[4,2,192],[4,4,192]]",4221));//R8R4_207=R8_19×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,224,23,8,[1,8,12,11,0,0],[[2,2,128],[2,4,192],[4,2,224],[4,4,256]]",4229));//R8R4_218=R8_20×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,1,3,324,31,32,[1,8,9,14,0,0],[[2,2,148],[2,4,148],[4,2,148],[4,4,256]]",4206));//R8R4_186=R8_17×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,4,1,7,360,31,8,[1,4,9,18,0,0],[[2,2,148],[2,4,148],[4,2,148],[4,4,220]]",4214));//R8R4_197=R8_18×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,360,31,32,[1,6,13,12,0,0],[[2,2,112],[2,4,184],[4,2,184],[4,4,184]]",4222));//R8R4_208=R8_19×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,12,3,3,252,27,8,[1,6,25,0,0,0],[[2,2,112],[2,4,184],[4,2,220],[4,4,256]]",4230));//R8R4_219=R8_20×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,1,3,252,31,32,[1,4,7,10,6,4],[[2,2,172],[2,4,172],[4,2,172],[4,4,256]]",4207));//R8R4_187=R8_17×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,1,7,280,31,8,[1,2,9,8,12,0],[[2,2,172],[2,4,172],[4,2,172],[4,4,228]]",4215));//R8R4_198=R8_18×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,280,31,32,[1,4,9,10,8,0],[[2,2,144],[2,4,200],[4,2,200],[4,4,200]]",4223));//R8R4_209=R8_19×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,3,196,19,8,[1,4,13,6,8,0],[[2,2,144],[2,4,200],[4,2,228],[4,4,256]]",4231));//R8R4_220=R8_20×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,384,31,32,[1,15,2,8,6,0],[[2,4,192],[4,2,192],[4,4,256]]",4232));//R8R4_224=R8_21×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,320,31,32,[1,7,10,8,6,0],[[2,2,64],[2,4,192],[4,2,192],[4,4,256]]",4240));//R8R4_235=R8_22×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,15,0,16,0,0],[[4,4,256]]",4248));//R8R4_246=R8_23×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,15,512,31,32,[1,15,0,16,0,0],[[2,4,128],[4,2,128],[4,4,256]]",4256));//R8R4_257=R8_24×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,288,31,32,[1,7,10,4,6,4],[[2,2,64],[2,4,208],[4,2,208],[4,4,256]]",4233));//R8R4_225=R8_21×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,15,240,31,32,[1,3,14,4,6,4],[[2,2,112],[2,4,208],[4,2,208],[4,4,256]]",4241));//R8R4_236=R8_22×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,7,8,16,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,256]]",4249));//R8R4_247=R8_23×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,15,384,31,32,[1,7,8,16,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,256]]",4257));//R8R4_258=R8_24×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,288,31,32,[1,8,11,8,4,0],[[2,2,64],[2,4,208],[4,2,208],[4,4,256]]",4234));//R8R4_226=R8_21×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,240,31,32,[1,4,11,12,4,0],[[2,2,112],[2,4,208],[4,2,208],[4,4,256]]",4242));//R8R4_237=R8_22×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32,[1,8,7,8,8,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,256]]",4250));//R8R4_248=R8_23×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,384,31,32,[1,8,7,8,8,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,256]]",4258));//R8R4_259=R8_24×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,240,23,8,[1,9,12,8,2,0],[[2,2,96],[2,4,216],[4,2,216],[4,4,256]]",4235));//R8R4_227=R8_21×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,200,23,8,[1,5,12,12,2,0],[[2,2,136],[2,4,216],[4,2,216],[4,4,256]]",4243));//R8R4_238=R8_22×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8,[1,9,6,8,8,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]]",4251));//R8R4_249=R8_23×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,7,320,31,8,[1,9,6,8,8,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,256]]",4259));//R8R4_260=R8_24×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,240,31,8,[1,9,12,8,2,0],[[2,2,96],[2,4,216],[4,2,216],[4,4,256]]",4236));//R8R4_228=R8_21×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,200,31,8,[1,5,12,12,2,0],[[2,2,136],[2,4,216],[4,2,216],[4,4,256]]",4244));//R8R4_239=R8_22×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8,[1,9,6,8,8,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]]",4252));//R8R4_250=R8_23×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,7,320,31,8,[1,9,6,8,8,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,256]]",4260));//R8R4_261=R8_24×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,24,4,7,7,192,23,32,[1,8,8,13,2,0],[[2,2,128],[2,4,224],[4,2,224],[4,4,256]]",4237));//R8R4_229=R8_21×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,24,4,3,7,160,23,32,[1,4,10,13,4,0],[[2,2,160],[2,4,224],[4,2,224],[4,4,256]]",4245));//R8R4_240=R8_22×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,8,4,11,4,4],[[2,2,128],[2,4,128],[4,2,128],[4,4,256]]",4253));//R8R4_251=R8_23×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,256,31,32,[1,8,4,11,4,4],[[2,2,128],[2,4,192],[4,2,192],[4,4,256]]",4261));//R8R4_262=R8_24×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,28,8,3,3,216,27,32,[1,6,17,8,0,0],[[2,2,112],[2,4,220],[4,2,220],[4,4,256]]",4238));//R8R4_230=R8_21×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,28,8,1,3,180,27,32,[1,4,13,14,0,0],[[2,2,148],[2,4,220],[4,2,220],[4,4,256]]",4246));//R8R4_241=R8_22×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,432,31,32,[1,6,9,4,12,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,256]]",4254));//R8R4_252=R8_23×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,3,288,31,32,[1,6,9,4,12,0],[[2,2,112],[2,4,184],[4,2,184],[4,4,256]]",4262));//R8R4_263=R8_24×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,20,4,3,3,168,19,32,[1,4,9,10,4,4],[[2,2,144],[2,4,228],[4,2,228],[4,4,256]]",4239));//R8R4_231=R8_21×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,20,4,1,3,140,19,32,[1,2,9,8,8,4],[[2,2,172],[2,4,228],[4,2,228],[4,4,256]]",4247));//R8R4_242=R8_22×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,336,31,32,[1,4,5,10,4,8],[[2,2,144],[2,4,144],[4,2,144],[4,4,256]]",4255));//R8R4_253=R8_23×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,3,224,31,32,[1,4,5,10,4,8],[[2,2,144],[2,4,200],[4,2,200],[4,4,256]]",4263));//R8R4_264=R8_24×R4_11
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,31,31,1024,31,32,[1,15,16,0,0,0],[]",4264));//R8R4_265=R8_25×R4_1
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,15,16,0,0,0],[[4,4,256]]",4265));//R8R4_266=R8_25×R4_2
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,512,31,32,[1,15,2,14,0,0],[[2,4,128],[4,2,128],[4,4,256]]",4266));//R8R4_267=R8_25×R4_3
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,12,3,3,240,31,8,[1,8,19,4,0,0],[[2,2,136],[2,4,196],[4,2,196],[4,4,256]]",4267));//R8R4_509=R8_47×R4_3
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,384,31,32,[1,6,11,10,4,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,160]]",4268));//R8R4_518=R8_48×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,416,31,8,[1,8,10,11,2,0],[[2,2,152],[2,4,152],[4,2,152],[4,4,152]]",4271));//R8R4_529=R8_49×R4_1
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,288,31,32,[1,6,11,10,4,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,256]]",4269));//R8R4_519=R8_48×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,312,31,8,[1,8,10,11,2,0],[[2,2,152],[2,4,152],[4,2,152],[4,4,256]]",4272));//R8R4_530=R8_49×R4_2
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,28,8,3,3,192,27,32,[1,6,15,10,0,0],[[2,2,160],[2,4,208],[4,2,208],[4,4,256]]",4270));//R8R4_520=R8_48×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,1,28,12,3,3,208,27,8,[1,8,18,5,0,0],[[2,2,152],[2,4,204],[4,2,204],[4,4,256]]",4273));//R8R4_531=R8_49×R4_3
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,3,3,432,31,32,[1,8,9,14,0,0],[[2,2,148],[2,4,148],[4,2,148],[4,4,148]]",4274));//R8R4_540=R8_50×R4_1
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,7,16,8,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,64]]",4275));//R8R4_276=R8_26×R4_1
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,7,16,8,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,256]]",4276));//R8R4_277=R8_26×R4_2
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,7,10,6,8,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,256]]",4277));//R8R4_278=R8_26×R4_3
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,1,3,324,31,32,[1,8,9,14,0,0],[[2,2,148],[2,4,148],[4,2,148],[4,4,256]]",4278));//R8R4_541=R8_50×R4_2
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,30,16,1,1,216,29,32,[1,8,23,0,0,0],[[2,2,148],[2,4,202],[4,2,202],[4,4,256]]",4279));//R8R4_542=R8_50×R4_3
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,3,336,31,32,[1,4,7,10,6,4],[[2,2,172],[2,4,172],[4,2,172],[4,4,172]]",4280));//R8R4_551=R8_51×R4_1
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,1,3,252,31,32,[1,4,7,10,6,4],[[2,2,172],[2,4,172],[4,2,172],[4,4,256]]",4281));//R8R4_552=R8_51×R4_2
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,26,8,1,1,168,25,32,[1,4,13,4,10,0],[[2,2,172],[2,4,214],[4,2,214],[4,4,256]]",4282));//R8R4_553=R8_51×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,3,240,31,32,[1,2,3,8,6,12],[[2,2,196],[2,4,196],[4,2,196],[4,4,196]]",4283));//R8R4_562=R8_52×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,1,3,180,31,32,[1,2,3,8,6,12],[[2,2,196],[2,4,196],[4,2,196],[4,4,256]]",4283));//R8R4_563=R8_52×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,18,4,1,1,120,17,32,[1,2,5,6,6,12],[[2,2,196],[2,4,226],[4,2,226],[4,4,256]]",4285));//R8R4_564=R8_52×R4_3
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,768,31,32,[1,8,15,8,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,64]]",4286));//R8R4_287=R8_27×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,64]]",4289));//R8R4_298=R8_28×R4_1
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32,[1,8,15,8,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,256]]",4287));//R8R4_288=R8_27×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,256]]",4290));//R8R4_299=R8_28×R4_2
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,384,31,32,[1,8,11,12,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,256]]",4288));//R8R4_289=R8_27×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,384,31,8,[1,11,6,10,4,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,256]]",4291));//R8R4_300=R8_28×R4_3
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,640,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4292));//R8R4_309=R8_29×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,15,15,576,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4295));//R8R4_320=R8_30×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,31,31,640,31,32,[1,15,16,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4298));//R8R4_331=R8_31×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,640,31,32,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4301));//R8R4_342=R8_32×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,15,512,31,32,[1,7,8,8,8,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]]",4304));//R8R4_353=R8_33×R4_1
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,640,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4307));//R8R4_364=R8_34×R4_1
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,512,31,32,[1,8,12,7,4,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]]",4310));//R8R4_375=R8_35×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,15,15,512,31,4,[1,11,12,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]]",4313));//R8R4_386=R8_36×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,448,31,8,[1,9,12,6,4,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,144]]",4316));//R8R4_397=R8_37×R4_1
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32,[1,4,11,12,4,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4319));//R8R4_408=R8_38×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,640,31,8,[1,3,16,12,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4322));//R8R4_419=R8_39×R4_1
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,576,31,32,[1,6,13,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4325));//R8R4_430=R8_40×R4_1
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,480,31,8,[1,8,11,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]]",4328));//R8R4_441=R8_41×R4_1
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,448,31,32,[1,4,9,10,8,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,144]]",4331));//R8R4_452=R8_42×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,15,15,576,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4334));//R8R4_463=R8_43×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,448,31,8,[1,9,12,6,4,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,144]]",4337));//R8R4_474=R8_44×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,384,31,32,[1,8,12,5,6,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,160]]",4340));//R8R4_485=R8_45×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,320,31,32,[1,4,10,9,4,4],[[2,2,176],[2,4,176],[4,2,176],[4,4,176]]",4343));//R8R4_496=R8_46×R4_1
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,480,31,8,[1,8,11,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]]",4346));//R8R4_507=R8_47×R4_1
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]]",4293));//R8R4_310=R8_29×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,7,15,432,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,256]]",4296));//R8R4_321=R8_30×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,480,31,32,[1,15,16,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]]",4299));//R8R4_332=R8_31×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,480,31,32,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]]",4302));//R8R4_343=R8_32×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,15,384,31,32,[1,7,8,8,8,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,256]]",4305));//R8R4_354=R8_33×R4_2
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]]",4308));//R8R4_365=R8_34×R4_2
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,8,12,7,4,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,256]]",4311));//R8R4_376=R8_35×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,7,15,384,31,4,[1,11,12,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,256]]",4314));//R8R4_387=R8_36×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,336,31,8,[1,9,12,6,4,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,256]]",4317));//R8R4_398=R8_37×R4_2
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,15,432,31,32,[1,4,11,12,4,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,256]]",4320));//R8R4_409=R8_38×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,480,31,8,[1,3,16,12,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]]",4323));//R8R4_420=R8_39×R4_2
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,432,31,32,[1,6,13,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,256]]",4326));//R8R4_431=R8_40×R4_2
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,360,31,8,[1,8,11,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,256]]",4329));//R8R4_442=R8_41×R4_2
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,336,31,32,[1,4,9,10,8,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,256]]",4332));//R8R4_453=R8_42×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,7,15,432,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,256]]",4335));//R8R4_464=R8_43×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,336,31,8,[1,9,12,6,4,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,256]]",4338));//R8R4_475=R8_44×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,288,31,32,[1,8,12,5,6,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,256]]",4341));//R8R4_486=R8_45×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,15,240,31,32,[1,4,10,9,4,4],[[2,2,176],[2,4,176],[4,2,176],[4,4,256]]",4344));//R8R4_497=R8_46×R4_2
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,360,31,8,[1,8,11,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,256]]",4347));//R8R4_508=R8_47×R4_2
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,320,31,8,[1,9,12,10,0,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,256]]",4294));//R8R4_311=R8_29×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,10,7,7,288,23,4,[1,11,14,6,0,0],[[2,2,112],[2,4,184],[4,2,184],[4,4,256]]",4297));//R8R4_322=R8_30×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,320,31,32,[1,15,2,14,0,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,256]]",4300));//R8R4_333=R8_31×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,320,31,32,[1,7,10,6,8,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,256]]",4303));//R8R4_344=R8_32×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,256,31,32,[1,7,2,14,0,8],[[2,2,128],[2,4,192],[4,2,192],[4,4,256]]",4306));//R8R4_355=R8_33×R4_3
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,320,31,8,[1,9,12,10,0,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,256]]",4309));//R8R4_366=R8_34×R4_3
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,256,31,32,[1,8,8,13,2,0],[[2,2,128],[2,4,192],[4,2,192],[4,4,256]]",4312));//R8R4_377=R8_35×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,10,7,7,256,31,4,[1,11,14,6,0,0],[[2,2,128],[2,4,192],[4,2,192],[4,4,256]]",4315));//R8R4_388=R8_36×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,224,23,8,[1,9,10,12,0,0],[[2,2,144],[2,4,200],[4,2,200],[4,4,256]]",4318));//R8R4_399=R8_37×R4_3
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,288,31,32,[1,4,11,8,8,0],[[2,2,112],[2,4,184],[4,2,184],[4,4,256]]",4321));//R8R4_410=R8_38×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,320,31,8,[1,3,14,2,12,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,256]]",4324));//R8R4_421=R8_39×R4_3
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,3,3,288,31,32,[1,6,17,8,0,0],[[2,2,112],[2,4,184],[4,2,184],[4,4,256]]",4327));//R8R4_432=R8_40×R4_3
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,12,3,3,240,27,8,[1,8,19,4,0,0],[[2,2,136],[2,4,196],[4,2,196],[4,4,256]]",4330));//R8R4_443=R8_41×R4_3
	//m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,3,224,31,32,[1,4,9,10,4,4],[[2,2,144],[2,4,200],[4,2,200],[4,4,256]]",4333));//R8R4_454=R8_42×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,10,7,7,288,31,4,[1,11,14,6,0,0],[[2,2,112],[2,4,184],[4,2,184],[4,4,256]]",4336));//R8R4_465=R8_43×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,224,31,8,[1,9,10,12,0,0],[[2,2,144],[2,4,200],[4,2,200],[4,4,256]]",4339));//R8R4_476=R8_44×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,24,4,7,7,192,23,32,[1,8,8,15,0,0],[[2,2,160],[2,4,208],[4,2,208],[4,4,256]]",4342));//R8R4_487=R8_45×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,24,4,3,7,160,23,32,[1,4,10,7,10,0],[[2,2,176],[2,4,216],[4,2,216],[4,4,256]]",4345));//R8R4_498=R8_46×R4_3
	/*---------------end of R32_4104~R32_4347----------------*/	
	/*---------------begin of R32_5348~R32_5571----------------*/
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,31,31,1024,31,32,[1,31,0,0,0,0],[]",5348));//R8R4_268=R8_25×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,15,31,768,31,32,[1,15,16,0,0,0],[[2,2,256]]",5356));//R8R4_279=R8_26×R4_4
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,15,31,768,31,32,[1,15,16,0,0,0],[[2,2,256]]",5349));//R8R4_269=R8_25×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,7,31,576,31,32,[1,7,24,0,0,0],[[2,2,448]]",5357));//R8R4_280=R8_26×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,768,31,32,[1,16,15,0,0,0],[[2,2,256]]",5350));//R8R4_270=R8_25×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,576,31,32,[1,8,15,8,0,0],[[2,2,448]]",5358));//R8R4_281=R8_26×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,640,31,8,[1,17,14,0,0,0],[[2,2,384]]",5351));//R8R4_271=R8_25×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,480,31,8,[1,9,14,8,0,0],[[2,2,544]]",5359));//R8R4_282=R8_26×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,640,31,8,[1,17,14,0,0,0],[[2,2,384]]",5352));//R8R4_272=R8_25×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,480,31,8,[1,9,14,8,0,0],[[2,2,544]]",5360));//R8R4_283=R8_26×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,512,31,32,[1,16,8,7,0,0],[[2,2,512]]",5353));//R8R4_273=R8_25×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,384,31,32,[1,8,12,7,4,0],[[2,2,640]]",5361));//R8R4_284=R8_26×R4_9	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,576,31,32,[1,10,21,0,0,0],[[2,2,448]]",5354));//R8R4_274=R8_25×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,432,31,32,[1,6,13,12,0,0],[[2,2,592]]",5362));//R8R4_285=R8_26×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,7,448,31,32,[1,8,9,14,0,0],[[2,2,576]]",5355));//R8R4_275=R8_25×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,7,336,31,32,[1,4,9,10,8,0],[[2,2,688]]",5363));//R8R4_286=R8_26×R4_11	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,480,31,8,[1,12,19,0,0,0],[[2,2,544]]",5364));//R8R4_293=R8_27×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,480,31,8,[1,12,19,0,0,0],[[2,2,544]]",5365));//R8R4_294=R8_27×R4_8	
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,640,31,8,[1,17,14,0,0,0],[[2,2,384]]",5366));//R8R4_367=R8_34×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,480,31,8,[1,9,14,8,0,0],[[2,2,544]]",5367));//R8R4_368=R8_34×R4_5
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,480,31,8,[1,12,19,0,0,0],[[2,2,544]]",5368));//R8R4_369=R8_34×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,400,31,2,[1,15,16,0,0,0],[[2,2,624]]",5369));//R8R4_370=R8_34×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,400,31,2,[1,15,16,0,0,0],[[2,2,624]]",5370));//R8R4_371=R8_34×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,320,31,8,[1,12,14,5,0,0],[[2,2,704]]",5371));//R8R4_372=R8_34×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8,[1,14,17,0,0,0],[[2,2,664]]",5372));//R8R4_373=R8_34×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,3,280,31,8,[1,8,9,10,4,0],[[2,2,744]]",5373));//R8R4_374=R8_34×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,384,31,32,[1,10,15,6,0,0],[[2,2,640]]",5374));//R8R4_295=R8_27×R4_9
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,640,31,8,[1,17,14,0,0,0],[[2,2,384]]",5375));//R8R4_312=R8_29×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,480,31,8,[1,9,14,8,0,0],[[2,2,544]]",5376));//R8R4_313=R8_29×R4_5
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,512,31,32,[1,16,8,7,0,0],[[2,2,512]]",5377));//R8R4_378=R8_35×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,384,31,32,[1,8,12,7,4,0],[[2,2,640]]",5378));//R8R4_379=R8_35×R4_5
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,384,31,32,[1,10,15,6,0,0],[[2,2,640]]",5379));//R8R4_380=R8_35×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,320,31,8,[1,12,14,5,0,0],[[2,2,704]]",5380));//R8R4_381=R8_35×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,320,31,8,[1,12,14,5,0,0],[[2,2,704]]",5381));//R8R4_382=R8_35×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,256,31,32,[1,10,12,9,0,0],[[2,2,768]]",5382));//R8R4_383=R8_35×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,3,3,288,31,32,[1,10,17,4,0,0],[[2,2,736]]",5383));//R8R4_384=R8_35×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,3,224,31,32,[1,6,9,10,4,2],[[2,2,800]]",5384));//R8R4_385=R8_35×R4_11	
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,480,31,8,[1,12,19,0,0,0],[[2,2,544]]",5385));//R8R4_314=R8_29×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,400,31,2,[1,15,16,0,0,0],[[2,2,624]]",5386));//R8R4_315=R8_29×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,400,31,2,[1,15,16,0,0,0],[[2,2,624]]",5387));//R8R4_316=R8_29×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,15,15,512,31,4,[1,19,12,0,0,0],[[2,2,512]]",5388));//R8R4_389=R8_36×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,15,384,31,4,[1,11,12,8,0,0],[[2,2,640]]",5389));//R8R4_390=R8_36×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,384,31,4,[1,16,15,0,0,0],[[2,2,640]]",5390));//R8R4_391=R8_36×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,320,31,1,[1,21,10,0,0,0],[[2,2,704]]",5391));//R8R4_392=R8_36×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,320,31,1,[1,21,10,0,0,0],[[2,2,704]]",5392));//R8R4_393=R8_36×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,256,31,4,[1,16,12,3,0,0],[[2,2,768]]",5393));//R8R4_394=R8_36×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,20,3,3,288,31,4,[1,22,9,0,0,0],[[2,2,736]]",5394));//R8R4_395=R8_36×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,3,3,224,31,4,[1,12,5,14,0,0],[[2,2,800]]",5395));//R8R4_396=R8_36×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,320,31,8,[1,12,14,5,0,0],[[2,2,704]]",5396));//R8R4_317=R8_29×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8,[1,14,17,0,0,0],[[2,2,664]]",5397));//R8R4_318=R8_29×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,3,280,31,8,[1,8,9,10,4,0],[[2,2,744]]",5398));//R8R4_319=R8_29×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,448,31,8,[1,17,8,6,0,0],[[2,2,576]]",5399));//R8R4_400=R8_37×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,336,31,8,[1,9,12,6,4,0],[[2,2,688]]",5400));//R8R4_401=R8_37×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,336,31,8,[1,12,15,4,0,0],[[2,2,688]]",5401));//R8R4_402=R8_37×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,280,23,2,[1,15,14,2,0,0],[[2,2,744]]",5402));//R8R4_403=R8_37×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,280,31,2,[1,15,14,2,0,0],[[2,2,744]]",5403));//R8R4_404=R8_37×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,224,23,8,[1,12,14,5,0,0],[[2,2,800]]",5404));//R8R4_405=R8_37×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,252,27,8,[1,14,17,0,0,0],[[2,2,772]]",5405));//R8R4_406=R8_37×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,3,196,19,8,[1,8,9,10,4,0],[[2,2,828]]",5406));//R8R4_407=R8_37×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,3,3,432,31,32,[1,10,21,0,0,0],[[2,2,592]]",5407));//R8R4_296=R8_27×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,3,336,31,32,[1,6,11,8,6,0],[[2,2,688]]",5408));//R8R4_297=R8_27×R4_11	
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,768,31,32,[1,16,15,0,0,0],[[2,2,256]]",5409));//R8R4_290=R8_27×R4_4
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,576,31,32,[1,8,15,8,0,0],[[2,2,448]]",5410));//R8R4_411=R8_38×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,15,432,31,32,[1,4,15,12,0,0],[[2,2,592]]",5411));//R8R4_412=R8_38×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,432,31,32,[1,6,13,12,0,0],[[2,2,592]]",5412));//R8R4_413=R8_38×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,360,31,8,[1,8,11,12,0,0],[[2,2,664]]",5413));//R8R4_414=R8_38×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,360,31,8,[1,8,11,12,0,0],[[2,2,664]]",5414));//R8R4_415=R8_38×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,288,31,32,[1,6,11,10,4,0],[[2,2,736]]",5415));//R8R4_416=R8_38×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,1,3,324,31,32,[1,8,9,14,0,0],[[2,2,700]]",5416));//R8R4_417=R8_38×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,1,3,252,31,32,[1,4,7,10,6,4],[[2,2,772]]",5417));//R8R4_418=R8_38×R4_11	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,15,15,576,31,4,[1,19,12,0,0,0],[[2,2,448]]",5418));//R8R4_323=R8_30×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,15,432,31,4,[1,11,12,8,0,0],[[2,2,592]]",5419));//R8R4_324=R8_30×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,432,31,4,[1,16,15,0,0,0],[[2,2,592]]",5420));//R8R4_325=R8_30×R4_6	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,640,31,8,[1,7,24,0,0,0],[[2,2,384]]",5421));//R8R4_422=R8_39×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,3,31,480,31,8,[1,3,28,0,0,0],[[2,2,544]]",5422));//R8R4_423=R8_39×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,3,15,480,31,8,[1,4,15,12,0,0],[[2,2,544]]",5423));//R8R4_424=R8_39×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,3,15,400,31,2,[1,5,14,12,0,0],[[2,2,624]]",5424));//R8R4_425=R8_39×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,3,15,400,31,2,[1,5,14,12,0,0],[[2,2,624]]",5425));//R8R4_426=R8_39×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,3,15,320,31,8,[1,4,14,7,6,0],[[2,2,704]]",5426));//R8R4_427=R8_39×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,4,1,7,360,31,8,[1,4,9,18,0,0],[[2,2,664]]",5427));//R8R4_428=R8_39×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,1,7,280,31,8,[1,2,9,8,12,0],[[2,2,744]]",5428));//R8R4_429=R8_39×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,360,23,1,[1,21,10,0,0,0],[[2,2,664]]",5429));//R8R4_326=R8_30×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,360,31,1,[1,21,10,0,0,0],[[2,2,664]]",5430));//R8R4_327=R8_30×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,288,23,4,[1,16,12,3,0,0],[[2,2,736]]",5431));//R8R4_328=R8_30×R4_9
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,576,31,32,[1,10,21,0,0,0],[[2,2,448]]",5432));//R8R4_433=R8_40×R4_4
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,480,31,8,[1,12,19,0,0,0],[[2,2,544]]",5443));//R8R4_444=R8_41×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,432,31,32,[1,6,13,12,0,0],[[2,2,592]]",5433));//R8R4_434=R8_40×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,360,31,8,[1,8,11,12,0,0],[[2,2,664]]",5444));//R8R4_445=R8_41×R4_5
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,3,3,432,31,32,[1,10,21,0,0,0],[[2,2,592]]",5434));//R8R4_435=R8_40×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8,[1,14,17,0,0,0],[[2,2,664]]",5445));//R8R4_446=R8_41×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8,[1,14,17,0,0,0],[[2,2,664]]",5435));//R8R4_436=R8_40×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,300,27,2,[1,20,11,0,0,0],[[2,2,724]]",5446));//R8R4_447=R8_41×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8,[1,14,17,0,0,0],[[2,2,664]]",5436));//R8R4_437=R8_40×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,300,31,2,[1,20,11,0,0,0],[[2,2,724]]",5447));//R8R4_448=R8_41×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,3,3,288,31,32,[1,10,17,4,0,0],[[2,2,736]]",5437));//R8R4_438=R8_40×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,240,27,8,[1,14,15,2,0,0],[[2,2,784]]",5448));//R8R4_449=R8_41×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,16,1,1,324,31,32,[1,16,15,0,0,0],[[2,2,700]]",5438));//R8R4_439=R8_40×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,24,1,1,270,29,8,[1,24,7,0,0,0],[[2,2,754]]",5449));//R8R4_450=R8_41×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,1,1,252,31,32,[1,8,9,8,6,0],[[2,2,772]]",5439));//R8R4_440=R8_40×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,1,1,210,25,8,[1,12,5,12,2,0],[[2,2,814]]",5450));//R8R4_451=R8_41×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,20,3,3,324,27,4,[1,22,9,0,0,0],[[2,2,700]]",5440));//R8R4_329=R8_30×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,3,3,252,19,4,[1,12,5,14,0,0],[[2,2,772]]",5441));//R8R4_330=R8_30×R4_11
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,576,31,32,[1,8,15,8,0,0],[[2,2,448]]",5442));//R8R4_291=R8_27×R4_5
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,576,31,32,[1,10,21,0,0,0],[[2,2,448]]",5451));//R8R4_292=R8_27×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,23,31,768,31,8,[1,23,8,0,0,0],[[2,2,256]]",5452));//R8R4_301=R8_28×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,31,31,640,31,32,[1,31,0,0,0,0],[[2,2,384]]",5453));//R8R4_334=R8_31×R4_4	
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,7,448,31,32,[1,8,9,14,0,0],[[2,2,576]]",5454));//R8R4_455=R8_42×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,15,15,576,31,4,[1,19,12,0,0,0],[[2,2,448]]",5465));//R8R4_466=R8_43×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,7,336,31,32,[1,4,9,10,8,0],[[2,2,688]]",5455));//R8R4_456=R8_42×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,15,432,31,4,[1,11,12,8,0,0],[[2,2,592]]",5466));//R8R4_467=R8_43×R4_5
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,3,336,31,32,[1,6,11,8,6,0],[[2,2,688]]",5456));//R8R4_457=R8_42×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,432,31,4,[1,16,15,0,0,0],[[2,2,592]]",5467));//R8R4_468=R8_43×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,3,280,31,8,[1,8,9,10,4,0],[[2,2,744]]",5457));//R8R4_458=R8_42×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,360,31,1,[1,21,10,0,0,0],[[2,2,664]]",5468));//R8R4_469=R8_43×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,3,280,31,8,[1,8,9,10,4,0],[[2,2,744]]",5458));//R8R4_459=R8_42×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,360,31,1,[1,21,10,0,0,0],[[2,2,664]]",5469));//R8R4_470=R8_43×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,3,224,31,32,[1,6,9,10,4,2],[[2,2,800]]",5459));//R8R4_460=R8_42×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,288,31,4,[1,16,12,3,0,0],[[2,2,736]]",5470));//R8R4_471=R8_43×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,1,1,252,31,32,[1,8,9,8,6,0],[[2,2,772]]",5460));//R8R4_461=R8_42×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,20,3,3,324,31,4,[1,22,9,0,0,0],[[2,2,700]]",5471));//R8R4_472=R8_43×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,1,1,196,31,32,[1,4,11,12,4,0],[[2,2,828]]",5461));//R8R4_462=R8_42×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,3,3,252,31,4,[1,12,5,14,0,0],[[2,2,772]]",5472));//R8R4_473=R8_43×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,15,31,480,31,32,[1,15,16,0,0,0],[[2,2,544]]",5462));//R8R4_335=R8_31×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,480,31,32,[1,16,15,0,0,0],[[2,2,544]]",5463));//R8R4_336=R8_31×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,400,31,8,[1,17,14,0,0,0],[[2,2,624]]",5464));//R8R4_337=R8_31×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,400,31,8,[1,17,14,0,0,0],[[2,2,624]]",5473));//R8R4_338=R8_31×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,320,31,32,[1,16,8,7,0,0],[[2,2,704]]",5474));//R8R4_339=R8_31×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,360,31,32,[1,10,21,0,0,0],[[2,2,664]]",5475));//R8R4_340=R8_31×R4_10	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,448,31,8,[1,17,8,6,0,0],[[2,2,576]]",5476));//R8R4_477=R8_44×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,384,31,32,[1,16,6,9,0,0],[[2,2,640]]",5487));//R8R4_488=R8_45×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,336,31,8,[1,9,12,6,4,0],[[2,2,688]]",5477));//R8R4_478=R8_44×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,288,31,32,[1,8,12,5,6,0],[[2,2,736]]",5488));//R8R4_489=R8_45×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,336,31,8,[1,12,15,4,0,0],[[2,2,688]]",5478));//R8R4_479=R8_44×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,288,31,32,[1,10,15,6,0,0],[[2,2,736]]",5489));//R8R4_490=R8_45×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,280,31,2,[1,15,14,2,0,0],[[2,2,744]]",5479));//R8R4_480=R8_44×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,240,23,8,[1,12,16,3,0,0],[[2,2,784]]",5490));//R8R4_491=R8_45×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,280,31,2,[1,15,14,2,0,0],[[2,2,744]]",5480));//R8R4_481=R8_44×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,240,31,8,[1,12,16,3,0,0],[[2,2,784]]",5491));//R8R4_492=R8_45×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,224,31,8,[1,12,14,5,0,0],[[2,2,800]]",5481));//R8R4_482=R8_44×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,24,4,7,7,192,23,32,[1,10,15,6,0,0],[[2,2,832]]",5492));//R8R4_493=R8_45×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,252,31,8,[1,14,17,0,0,0],[[2,2,772]]",5482));//R8R4_483=R8_44×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,28,8,3,3,216,27,32,[1,10,21,0,0,0],[[2,2,808]]",5493));//R8R4_494=R8_45×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,3,196,31,8,[1,8,9,10,4,0],[[2,2,828]]",5483));//R8R4_484=R8_44×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,20,4,3,3,168,19,32,[1,6,11,8,6,0],[[2,2,856]]",5494));//R8R4_495=R8_45×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,7,280,31,32,[1,8,9,14,0,0],[[2,2,744]]",5484));//R8R4_341=R8_31×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,11,31,576,31,8,[1,11,20,0,0,0],[[2,2,448]]",5485));//R8R4_302=R8_28×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,11,15,576,31,8,[1,12,15,4,0,0],[[2,2,448]]",5486));//R8R4_303=R8_28×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,11,15,480,31,2,[1,13,14,4,0,0],[[2,2,544]]",5495));//R8R4_304=R8_28×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,15,31,640,31,32,[1,15,16,0,0,0],[[2,2,384]]",5496));//R8R4_345=R8_32×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,7,31,480,31,32,[1,7,24,0,0,0],[[2,2,544]]",5497));//R8R4_346=R8_32×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,480,31,32,[1,8,15,8,0,0],[[2,2,544]]",5506));//R8R4_347=R8_32×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,400,31,8,[1,9,14,8,0,0],[[2,2,624]]",5507));//R8R4_348=R8_32×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,400,31,8,[1,9,14,8,0,0],[[2,2,624]]",5508));//R8R4_349=R8_32×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,320,31,32,[1,8,12,5,6,0],[[2,2,704]]",5498));//R8R4_499=R8_46×R4_4
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,480,31,8,[1,12,19,0,0,0],[[2,2,544]]",5509));//R8R4_510=R8_47×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,15,240,31,32,[1,4,14,5,4,4],[[2,2,784]]",5499));//R8R4_500=R8_46×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,360,31,8,[1,8,11,12,0,0],[[2,2,664]]",5510));//R8R4_511=R8_47×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,240,31,32,[1,6,11,10,4,0],[[2,2,784]]",5500));//R8R4_501=R8_46×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8,[1,14,17,0,0,0],[[2,2,664]]",5511));//R8R4_512=R8_47×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,200,23,8,[1,8,10,11,2,0],[[2,2,824]]",5501));//R8R4_502=R8_46×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,300,31,2,[1,20,11,0,0,0],[[2,2,724]]",5512));//R8R4_513=R8_47×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,200,31,8,[1,8,10,11,2,0],[[2,2,824]]",5502));//R8R4_503=R8_46×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,300,31,2,[1,20,11,0,0,0],[[2,2,724]]",5513));//R8R4_514=R8_47×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,24,4,3,7,160,23,32,[1,6,11,10,4,0],[[2,2,864]]",5503));//R8R4_504=R8_46×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,240,31,8,[1,14,15,2,0,0],[[2,2,784]]",5514));//R8R4_515=R8_47×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,28,8,1,3,180,27,32,[1,8,9,14,0,0],[[2,2,844]]",5504));//R8R4_505=R8_46×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,24,1,1,270,31,8,[1,24,7,0,0,0],[[2,2,754]]",5515));//R8R4_516=R8_47×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,20,4,1,3,140,19,32,[1,4,7,10,6,4],[[2,2,884]]",5505));//R8R4_506=R8_46×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,1,1,210,31,8,[1,12,5,12,2,0],[[2,2,814]]",5516));//R8R4_517=R8_47×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,320,31,32,[1,8,12,7,4,0],[[2,2,704]]",5517));//R8R4_350=R8_32×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,360,31,32,[1,6,13,12,0,0],[[2,2,664]]",5518));//R8R4_351=R8_32×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,7,280,31,32,[1,4,9,10,8,0],[[2,2,744]]",5519));//R8R4_352=R8_32×R4_11	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,11,15,480,31,2,[1,13,14,4,0,0],[[2,2,544]]",5528));//R8R4_305=R8_28×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,11,15,384,31,8,[1,12,10,7,2,0],[[2,2,640]]",5529));//R8R4_306=R8_28×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,4,5,7,432,31,8,[1,8,17,6,0,0],[[2,2,592]]",5530));//R8R4_307=R8_28×R4_10
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,384,31,32,[1,10,15,6,0,0],[[2,2,640]]",5520));//R8R4_521=R8_48×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,416,31,8,[1,12,16,3,0,0],[[2,2,608]]",5531));//R8R4_532=R8_49×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,288,31,32,[1,6,11,10,4,0],[[2,2,736]]",5521));//R8R4_522=R8_48×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,312,31,8,[1,8,10,11,2,0],[[2,2,712]]",5532));//R8R4_533=R8_49×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,3,3,288,31,32,[1,10,17,4,0,0],[[2,2,736]]",5522));//R8R4_523=R8_48×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,312,31,8,[1,14,15,2,0,0],[[2,2,712]]",5533));//R8R4_534=R8_49×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,240,27,8,[1,14,15,2,0,0],[[2,2,784]]",5523));//R8R4_524=R8_48×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,260,27,2,[1,20,10,1,0,0],[[2,2,764]]",5534));//R8R4_535=R8_49×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,240,31,8,[1,14,15,2,0,0],[[2,2,784]]",5524));//R8R4_525=R8_48×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,260,31,2,[1,20,10,1,0,0],[[2,2,764]]",5535));//R8R4_536=R8_49×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,28,8,3,3,192,27,32,[1,10,17,4,0,0],[[2,2,832]]",5525));//R8R4_526=R8_48×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,28,12,3,3,208,27,8,[1,14,15,2,0,0],[[2,2,816]]",5536));//R8R4_537=R8_49×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,30,16,1,1,216,29,32,[1,16,15,0,0,0],[[2,2,808]]",5526));//R8R4_527=R8_48×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,30,24,1,1,234,29,8,[1,24,7,0,0,0],[[2,2,790]]",5537));//R8R4_538=R8_49×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,26,8,1,1,168,25,32,[1,8,9,8,6,0],[[2,2,856]]",5527));//R8R4_528=R8_48×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,26,12,1,1,182,25,8,[1,12,5,12,2,0],[[2,2,842]]",5538));//R8R4_539=R8_49×R4_11	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,15,15,512,31,32,[1,15,0,16,0,0],[[2,2,512]]",5539));//R8R4_356=R8_33×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,7,15,384,31,32,[1,7,8,16,0,0],[[2,2,640]]",5540));//R8R4_357=R8_33×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,7,384,31,32,[1,8,7,8,8,0],[[2,2,640]]",5541));//R8R4_358=R8_33×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,7,320,31,8,[1,9,6,8,8,0],[[2,2,704]]",5550));//R8R4_359=R8_33×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,7,320,31,8,[1,9,6,8,8,0],[[2,2,704]]",5551));//R8R4_360=R8_33×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,7,256,31,32,[1,8,4,11,4,4],[[2,2,768]]",5552));//R8R4_361=R8_33×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,3,288,31,32,[1,6,9,4,12,0],[[2,2,736]]",5561));//R8R4_362=R8_33×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,3,224,31,32,[1,4,5,10,4,8],[[2,2,800]]",5562));//R8R4_363=R8_33×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,5,7,336,31,8,[1,6,9,12,4,0],[[2,2,688]]",5563));//R8R4_308=R8_28×R4_11
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,3,3,432,31,32,[1,10,21,0,0,0],[[2,2,592]]",5542));//R8R4_543=R8_50×R4_4
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,3,336,31,32,[1,6,11,8,6,0],[[2,2,688]]",5553));//R8R4_554=R8_51×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,3,240,31,32,[1,4,3,6,18,0],[[2,2,784]]",5564));//R8R4_565=R8_52×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,1,3,324,31,32,[1,8,9,14,0,0],[[2,2,700]]",5543));//R8R4_544=R8_50×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,1,3,252,31,32,[1,4,7,10,6,4],[[2,2,772]]",5554));//R8R4_555=R8_51×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,1,3,180,31,32,[1,2,3,8,6,12],[[2,2,844]]",5565));//R8R4_566=R8_52×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,16,1,1,324,31,32,[1,16,15,0,0,0],[[2,2,700]]",5544));//R8R4_545=R8_50×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,1,1,252,31,32,[1,8,9,8,6,0],[[2,2,772]]",5555));//R8R4_556=R8_51×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,1,1,180,31,32,[1,4,3,6,12,6],[[2,2,844]]",5566));//R8R4_567=R8_52×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,24,1,1,270,29,8,[1,24,7,0,0,0],[[2,2,754]]",5545));//R8R4_546=R8_50×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,1,1,210,25,8,[1,12,5,12,2,0],[[2,2,814]]",5556));//R8R4_557=R8_51×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,1,1,150,17,8,[1,6,1,6,18,0],[[2,2,874]]",5567));//R8R4_568=R8_52×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,24,1,1,270,31,8,[1,24,7,0,0,0],[[2,2,754]]",5546));//R8R4_547=R8_50×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,1,1,210,31,8,[1,12,5,12,2,0],[[2,2,814]]",5557));//R8R4_558=R8_51×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,1,1,150,31,8,[1,6,1,6,18,0],[[2,2,874]]",5568));//R8R4_569=R8_52×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,30,16,1,1,216,29,32,[1,16,15,0,0,0],[[2,2,808]]",5547));//R8R4_548=R8_50×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,26,8,1,1,168,25,32,[1,8,9,8,6,0],[[2,2,856]]",5558));//R8R4_559=R8_51×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,18,4,1,1,120,17,32,[1,4,3,6,12,6],[[2,2,904]]",5569));//R8R4_570=R8_52×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,31,32,0,0,243,30,32,[1,31,0,0,0,0],[[2,2,781]]",5548));//R8R4_549=R8_50×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,29,16,0,0,189,28,32,[1,15,2,14,0,0],[[2,2,835]]",5559));//R8R4_560=R8_51×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,25,8,0,0,135,24,32,[1,7,0,6,18,0],[[2,2,889]]",5570));//R8R4_571=R8_52×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,29,16,0,0,189,28,32,[1,15,2,14,0,0],[[2,2,835]]",5549));//R8R4_550=R8_50×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,23,8,0,0,147,22,32,[1,7,8,16,0,0],[[2,2,877]]",5560));//R8R4_561=R8_51×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,11,4,0,0,105,10,32,[1,3,2,8,6,12],[[2,2,919]]",5571));//R8R4_572=R8_52×R4_11	
	/*---------------end of R32_5348~R32_5571----------------*/		
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,16,2,3,7,112,15,32,[1,4,7,4,8,8],[[2,2,912]]",5000));//R2_2/([0,0,0,0,0,1])=R2_2/([1,1,0,0,1,1])=R32_5000
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,24,4,3,3,144,23,32,[1,6,9,4,12,0],[[2,2,880]]",5001));//R2_2/([0,0,0,0,1,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,17,4,0,0,93,16,32,[1,3,2,2,12,12],[[2,2,931]]",5002));//R2_2/([0,1,1,1,1,1])=R2_2/([1,0,0,0,0,1])=R32_5002	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,20,4,3,3,120,19,32,[1,6,11,2,6,6],[[2,2,904]]",5003));//R2_2/([1,1,1,1,1,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,1,2,0,0,63,0,32,[1,1,0,0,0,30],[[2,2,961]]",5004));//R2_2/([1,0,1,1,1,1])=R2_2/([1,1,1,1,0,1])=R2_2/([1,1,1,0,1,1])=R32_5004
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,1,1,124,31,32,[1,2,3,2,12,12],[[2,2,900]]",5005));//R2_2/([[0,0,0,0,1,1]],[1,1,0,0,0,1,1])
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,7,160,31,32,[1,8,6,5,6,6],[[2,2,864]]",5006));//R2_2/([[0,0,0,1,1]],[1,0,0,0,0,0,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,7,15,192,31,32,[1,7,8,0,0,16],[[2,2,832]]",5007));//R2_2/([[0,1]],[0,0,0,0,0,0,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,16,2,7,15,160,15,32,[1,8,15,8,0,0],[[2,2,864]]",5008));//R2_2/([[1,0,0,1],[1,0,1]],[0,0,0,0,0,0,1])		
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,13,5,7,264,31,1,[1,17,14,0,0,0],[[2,2,760]]",5009));//[[0,1,0,1],[0,0,0,0],[0,0,0,1],[0,1,0,1]],[[0,0,0,1],[0,0,0,1],[0,0,0,1],[0,0,0,1]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,26,16,3,3,174,25,4,[1,18,11,2,0,0],[[2,2,850]]",5010));//[[1,1,1,1],[1,1,1,1],[1,1,1,1],[1,1,1,1]],[[0,1,0,1],[0,0,0,0],[0,1,0,0],[0,1,0,1]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,24,10,5,7,208,23,2,[1,14,15,2,0,0],[[2,2,816]]",5011));//[[0,1,0,1],[0,0,0,0],[0,1,0,1],[0,1,0,1]],[[1,0,1,0],[1,1,1,1],[1,1,1,0],[1,0,1,0]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,24,10,5,7,192,23,4,[1,14,15,2,0,0],[[2,2,832]]",5012));//[[0,0,0,1],[0,0,0,1],[0,0,0,1],[0,0,0,1]],[[0,1,0,1],[0,0,0,0],[1,1,1,0],[0,1,0,1]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,28,18,3,3,236,27,2,[1,20,11,0,0,0],[[2,2,788]]",5013));//[[0,1,0,1],[0,0,0,0],[1,1,1,0],[0,1,0,1]],[[0,1,0,0],[0,0,0,1],[0,0,0,0],[0,1,0,0]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,20,10,3,3,180,19,2,[1,12,11,8,0,0],[[2,2,844]]",5014));//[[0,1,0,1],[0,1,0,1],[0,1,0,1],[0,1,0,1]],[[1,0,1,0],[1,1,1,1],[0,0,0,1],[1,0,1,0]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,7,240,31,32,[1,4,7,4,8,8],[[2,2,784]]",5015));//R2_2/([[0,1]],[0,0,0,0,0,1,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,24,4,3,7,192,23,32,[1,6,13,12,0,0],[[2,2,832]]",5016));//R2_2/([[1,0,1],[1,0,1,1]],[0,0,0,0,0,1,1])
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,7,192,31,32,[1,8,4,11,4,4],[[2,2,832]]",5017));//R2_2/([[1,1]],[0,0,0,0,1,0,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,16,2,7,7,160,15,32,[1,8,7,8,8,0],[[2,2,864]]",5018));//R2_2/([[1,0,0,0,1,1],[1,1,0,0,0,1]],[0,0,0,0,0,1,1])
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,13,5,7,264,23,1,[1,17,14,0,0,0],[[2,2,760]]",5019));//[[0,0,0,0],[0,0,0,0],[1,1,1,1],[0,0,0,0]],[[1,1,1,0],[1,1,1,0],[1,0,1,1],[1,1,1,0]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,240,15,8,[1,9,14,8,0,0],[[2,2,784]]",5020));//[[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1]],[[0,0,0,0,1,0],[0,0,0,1,0,0],[1,0,0,0,1,1],[0,0,0,1,0,0],[1,0,0,1,0,1],[0,1,0,1,0,1]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,8,7,7,232,31,4,[1,14,12,5,0,0],[[2,2,792]]",5021));//J4:[[1,1],[1,1]],[[1,3],[3,3]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,16,2,3,15,144,15,32,[1,4,15,12,0,0],[[2,2,880]]",5022));//R2_2/([[0,0,0,1],[0,1,1]],[0,1,1,0,1,0,1,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,19,23,416,31,4,[1,19,4,8,0,0],[[2,2,608]]",5023));//I4:[[0,2],[0,0]],[[3,3],[3,3]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,16,2,7,7,160,15,8,[1,8,7,8,8,0],[[2,2,864]]",5024));//I4:[[1,1],[3,1]],[[3,3],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,15,15,384,31,4,[1,19,8,4,0,0],[[2,2,640]]",5025));//I4:[[2,0],[0,0]],[[3,3],[3,3]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,24,10,7,7,208,23,4,[1,16,15,0,0,0],[[2,2,816]]",5026));//I4:[[1,0],[0,0]],[[3,3],[3,1]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,24,6,7,7,200,23,8,[1,12,15,4,0,0],[[2,2,824]]",5027));//I4:[[0,0],[0,1]],[[2,0],[3,3]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,15,336,31,32,[1,4,15,12,0,0],[[2,2,688]]",5028));//R2_2/([[0,0,0,0,1,1,1],[1,0,0,1]],[0,1,1,0,1,0,1,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,512,31,8,[1,7,24,0,0,0],[[2,2,512]]",5029));//KW(5,1):[[0,2],[2,0]],[[2,2],[0,0]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,11,15,288,31,2,[1,15,12,4,0,0],[[2,2,736]]",5030));//R4(9,1):[[0,0],[2,2]],[[2,1],[1,2]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,15,240,15,2,[1,11,12,8,0,0],[[2,2,784]]",5031));//[[0,1,0,0],[1,0,0,0],[0,1,1,0],[0,1,1,0]],[[0,0,1,1],[0,0,1,1],[0,0,1,1],[0,0,1,1]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,11,15,272,15,2,[1,15,16,0,0,0],[[2,2,752]]",5032));//[[1,0,0,0],[0,1,0,0],[1,1,1,0],[1,1,1,0]],[[1,0,1,1],[0,1,1,1],[1,0,0,1],[1,0,0,1]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,11,15,240,15,2,[1,13,14,4,0,0],[[2,2,784]]",5033));//[[1,0,1,1],[0,1,1,1],[1,0,0,1],[1,0,0,1]],[[1,1,0,0],[1,1,0,0],[1,0,0,0],[1,0,0,0]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,11,15,336,15,1,[1,19,12,0,0,0],[[2,2,688]]",5034));
	//R32	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,4],[4,1,8],[4,6,4],[8,7,8]],[[2,1,7],[2,2,4],[4,1,20],[4,4,21],[4,6,16],[4,7,18],[8,5,72],[8,7,68],[8,29,54],[16,209,216]]",4295));//R8R4_320=R8_30×R4_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,4],[4,1,8],[4,6,4],[8,7,8]],[[2,1,7],[2,2,4],[4,1,20],[4,4,21],[4,6,16],[4,8,18],[8,5,72],[8,7,68],[8,34,54],[16,255,216]]",4334));//R8R4_463=R8_43×R4_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,1,8],[4,6,2],[4,9,2],[8,7,4],[8,35,2],[16,210,4]],[[2,1,7],[2,2,2],[4,1,20],[4,4,21],[4,6,8],[4,7,3],[4,9,8],[8,5,72],[8,7,34],[8,29,9],[8,35,26],[8,37,9],[16,209,36],[16,210,106],[16,336,27]]",4316));//R8R4_397=R8_37×R4_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,1,8],[4,6,2],[4,9,2],[8,7,4],[8,35,2],[16,210,4]],[[2,1,7],[2,2,2],[4,1,20],[4,4,21],[4,6,8],[4,8,3],[4,9,8],[8,5,72],[8,7,34],[8,34,9],[8,35,26],[8,44,9],[16,210,106],[16,255,36],[16,352,27]]",4337));//R8R4_474=R8_44×R4_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,1,8],[4,6,6],[8,7,8]],[[2,1,7],[2,2,2],[4,1,20],[4,4,21],[4,6,24],[4,8,3],[8,5,72],[8,7,68],[8,27,18],[8,34,27],[16,208,72],[16,255,72],[16,354,18]]",4108));//R8R4_52=R8_5×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,1,8],[4,6,6],[8,7,8]],[[2,1,7],[2,2,2],[4,1,20],[4,4,21],[4,6,24],[4,7,3],[8,5,72],[8,7,68],[8,27,18],[8,29,27],[16,208,72],[16,209,72],[16,308,18]]",4107));//R8R4_51=R8_5×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,1,4],[4,6,7],[8,7,12]],[[2,1,3],[2,2,5],[4,1,10],[4,4,3],[4,6,28],[4,8,6],[4,10,6],[8,5,12],[8,7,102],[8,27,9],[8,34,18],[8,40,18],[8,47,6],[16,208,36],[16,224,72],[16,255,72],[16,356,18]]",4124));//R8R4_74=R8_7×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,1,4],[4,6,7],[8,7,12]],[[2,1,3],[2,2,5],[4,1,10],[4,4,3],[4,6,28],[4,7,6],[4,10,6],[8,5,12],[8,7,102],[8,27,9],[8,29,18],[8,40,18],[8,41,6],[16,208,36],[16,209,72],[16,224,72],[16,310,18]]",4123));//R8R4_73=R8_7×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,4],[4,2,8],[4,6,4],[8,17,8]],[[2,1,7],[2,2,4],[4,2,20],[4,4,21],[4,6,16],[4,7,18],[8,16,72],[8,17,68],[8,29,54],[16,215,216]]",4296));//R8R4_321=R8_30×R4_2
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,4],[4,2,8],[4,6,4],[8,17,8]],[[2,1,7],[2,2,4],[4,2,20],[4,4,21],[4,6,16],[4,8,18],[8,16,72],[8,17,68],[8,34,54],[16,256,216]]",4335));//R8R4_464=R8_43×R4_2
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,2,8],[4,6,6],[8,17,8]],[[2,1,7],[2,2,2],[4,2,20],[4,4,21],[4,6,24],[4,8,3],[8,16,72],[8,17,68],[8,27,18],[8,34,27],[16,214,72],[16,256,72],[16,354,18]]",4196));//R8R4_173=R8_16×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,2,8],[4,6,6],[8,17,8]],[[2,1,7],[2,2,2],[4,2,20],[4,4,21],[4,6,24],[4,7,3],[8,16,72],[8,17,68],[8,27,18],[8,29,27],[16,214,72],[16,215,72],[16,308,18]]",4195));//R8R4_172=R8_16×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,2,8],[4,6,2],[4,9,2],[8,17,4],[8,35,2],[16,216,4]],[[2,1,7],[2,2,2],[4,2,20],[4,4,21],[4,6,8],[4,8,3],[4,9,8],[8,16,72],[8,17,34],[8,34,9],[8,35,26],[8,44,9],[16,216,106],[16,256,36],[16,352,27]]",4338));//R8R4_475=R8_44×R4_2
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,2,8],[4,6,2],[4,9,2],[8,17,4],[8,35,2],[16,216,4]],[[2,1,7],[2,2,2],[4,2,20],[4,4,21],[4,6,8],[4,7,3],[4,9,8],[8,16,72],[8,17,34],[8,29,9],[8,35,26],[8,37,9],[16,215,36],[16,216,106],[16,336,27]]",4317));//R8R4_398=R8_37×R4_2
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,2,4],[4,6,7],[8,17,12]],[[2,1,3],[2,2,5],[4,2,10],[4,4,3],[4,6,28],[4,8,6],[4,10,6],[8,16,12],[8,17,102],[8,27,9],[8,34,18],[8,40,18],[8,47,6],[16,214,36],[16,217,72],[16,256,72],[16,356,18]]",4204));//R8R4_184=R8_17×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,2,4],[4,6,7],[8,17,12]],[[2,1,3],[2,2,5],[4,2,10],[4,4,3],[4,6,28],[4,7,6],[4,10,6],[8,16,12],[8,17,102],[8,27,9],[8,29,18],[8,40,18],[8,41,6],[16,214,36],[16,215,72],[16,217,72],[16,310,18]]",4203));//R8R4_183=R8_17×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,3,6],[4,6,6],[8,13,10]],[[2,1,7],[2,2,2],[4,3,15],[4,4,21],[4,6,24],[4,8,3],[8,13,85],[8,14,24],[8,15,12],[8,27,18],[8,34,27],[16,204,36],[16,219,36],[16,221,72],[16,223,24],[16,354,18]]",4172));//R8R4_140=R8_13×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,3,6],[4,6,6],[8,13,10]],[[2,1,7],[2,2,2],[4,3,15],[4,4,21],[4,6,24],[4,7,3],[8,13,85],[8,14,24],[8,20,12],[8,27,18],[8,29,27],[16,203,36],[16,219,36],[16,221,72],[16,222,24],[16,308,18]]",4171));//R8R4_139=R8_13×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[2,2,4],[4,6,12]],[[2,1,15],[2,2,4],[4,4,105],[4,6,48],[4,8,18],[8,27,36],[8,34,162],[16,354,108]]",5465));//R8R4_466=R8_43×R4_4
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[2,2,4],[4,6,12]],[[2,1,15],[2,2,4],[4,4,105],[4,6,48],[4,7,18],[8,27,36],[8,29,162],[16,308,108]]",5418));//R8R4_323=R8_30×R4_4
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[2,2,2],[4,6,6],[4,9,2],[8,35,6]],[[2,1,15],[2,2,2],[4,4,105],[4,6,24],[4,8,3],[4,9,8],[8,27,18],[8,34,27],[8,35,78],[8,44,9],[16,314,54],[16,352,81],[16,354,18]]",5476));//R8R4_477=R8_44×R4_4
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[2,2,2],[4,6,6],[4,9,2],[8,35,6]],[[2,1,15],[2,2,2],[4,4,105],[4,6,24],[4,7,3],[4,9,8],[8,27,18],[8,29,27],[8,35,78],[8,37,9],[16,308,18],[16,314,54],[16,336,81]]",5399));//R8R4_400=R8_37×R4_4
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[2,2,2],[4,6,14]],[[2,1,15],[2,2,2],[4,4,105],[4,6,56],[4,8,3],[8,27,126],[8,34,63],[16,354,126]]",5352));//R8R4_272=R8_25×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[2,2,2],[4,6,14]],[[2,1,15],[2,2,2],[4,4,105],[4,6,56],[4,7,3],[8,27,126],[8,29,63],[16,308,126]]",5351));//R8R4_271=R8_25×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,5],[4,6,19]],[[2,1,7],[2,2,5],[4,4,21],[4,6,76],[4,8,6],[4,10,6],[8,27,99],[8,34,54],[8,40,54],[8,47,6],[16,307,36],[16,354,36],[16,356,54]]",5365));//R8R4_294=R8_27×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,5],[4,6,19]],[[2,1,7],[2,2,5],[4,4,21],[4,6,76],[4,7,6],[4,10,6],[8,27,99],[8,29,54],[8,40,54],[8,41,6],[16,307,36],[16,308,36],[16,310,54]]",5364));//R8R4_293=R8_27×R4_7
#endif
	//R36
	m_RingInvariant.insert(make_pair("[1,3,8,0,24,0,0,0,0],6,1,1,18,4,2,2,147,17,36",99));		
	m_RingInvariant.insert(make_pair("[1,3,8,0,24,0,0,0,0],6,1,1,20,4,1,1,136,19,36",119));	
}

RIDHelper::~RIDHelper(){
	m_RingInvariant.clear();	
}

vector<int>  RIDHelper::IDFromRingInvariant(string& RingInvariant){
	std::multimap<string,int>::iterator it;
	std::pair<std::multimap<string,int>::iterator, std::multimap<string,int>::iterator> pa;
	vector<int> v;
#if 1
	for( it = m_RingInvariant.begin(); it != m_RingInvariant.end(); ++it){
		string::size_type idx=RingInvariant.find(it->first);
		if(idx!=string::npos)
			v.push_back(it->second);
	}
#else
	pa = m_RingInvariant.equal_range(RingInvariant);	
	for( it = pa.first; it != m_RingInvariant.end() && it != pa.second; ++it){
		v.push_back(it->second);
	}
#endif
	return v;
}

vector<int>  RIDHelper::IDFromI1I2(string& I1I2){
	std::multimap<string,int>::iterator it;
	std::pair<std::multimap<string,int>::iterator, std::multimap<string,int>::iterator> pa;
	vector<int> v;
	pa = m_I1I2.equal_range(I1I2);	
	for( it = pa.first; it != m_I1I2.end() && it != pa.second; ++it){
		v.push_back(it->second);
	}
	return v;
}

vector<string> RIDHelper::I1I2FromID(int ID){
	std::multimap<string,int>::iterator it;
	vector<string> v;
	for( it = m_I1I2.begin(); it != m_I1I2.end(); ++it){
		if(it->second==ID)
			v.push_back(it->first);
	}
    return v;	
}

string calcRingInvariant(IRing* r){
	string strN0=calcN0(r);
	int n0=calcn0(r); 
	bool bA=IsAbelian(r);
	int n3=One(r);
	bool bO=(n3>-1);
	int n1=NoInvNum(r);
	int n2=IdempotentNum(r);
	int n4=Nil2Num(r);
	int n5=Nil3Num(r);
	int n6=ZeroNum(r);
	int n7=ZeroFactorNum(r);
    vector<int> ZA=Center(r);
	int n8=ZA.size();
	string strS1=calS1(r,true);
	string strN2=calcN2(r);
    // N0n0bAbOn1n2n4n5n6n7n8S1N2
	char sz[2048]={0};
	sprintf(sz,"%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%s",strN0.c_str(),n0,bA,bO,n1,n2,n4,n5,n6,n7,n8,strS1.c_str(),strN2.c_str());	
	//int len=strlen(sz);//Rpp(12,3):1576>1024
	return sz;
}

int IdRing(IRing* r){	
   if(r->size()<=1)
	   return 1;
   string strRingInvariant=calcRingInvariant(r);
   static RIDHelper idHelper;
   vector<int> vID=idHelper.IDFromRingInvariant(strRingInvariant);
   if(vID.size()<=0)	   
	   return -1;//没有RingInvariant数据
   if(vID.size()>1){   
	    string strI1I2=calcI1(r)+","+calcI2(r);
		vector<int> vID2=idHelper.IDFromI1I2(strI1I2);	
		vector<int> vID02;
		set_intersection(vID.begin(),vID.end(),vID2.begin(),vID2.end(),back_inserter(vID02)); 
		if(vID02.size()!=1){
			#if 1
			 printf("[");
			 for(int i=0;i<vID.size();i++){
				 printf("%d,",vID[i]);
			 }
			 printf("]\n");
			#endif	
		}			
        if(vID02.size()>1){		
			return 0;//ID不确定，还需要新的环不变量确定编号
	   }
	   else if(vID02.size()<=0){
		   printf("出错了，环不变量数据I1I2遗漏或有误！\n");
		   return 0;
	   }
	   return vID02[0];
   }   
#ifdef R16_I1I2
   if(r->size()==16){
		static int IDs[]={274,282,286,287,289,290,126,127,283,377,29,30,31,32,70,190,153,155,159,163,165,128,147,148,149,160,162,167,168,169,183,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,93,94,95,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,129,130,131,132,133,134,135,136,137,138,139,140,150,151,152,154,156,157,158,161,164,166,170,174,177,179,184,185,186,187,188,189,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,272,273,275,276,277,278,180,284,285,288,291,292,293,294,295,296,297,298,299,300,301,302,303,304,305,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,335,336,337,338,339,340,341,342,343,344,345,346,347,348,349,350,351,352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,378,379,380,381,382,383,384,385,386,387,388,389,390};
		static int cnt=sizeof(IDs)/sizeof(IDs[0]);
		vector<int> vIDs(IDs,IDs+cnt);
		vector<int>::iterator p1=std::find(vIDs.begin(),vIDs.end(),vID[0]);
		if(p1!=vIDs.end()){
			string strI1I2=calcI1(r)+","+calcI2(r);
			vector<string> vI1I2=idHelper.I1I2FromID(vID[0]);
			vector<string>::iterator p=std::find(vI1I2.begin(),vI1I2.end(),strI1I2);
			if(p==vI1I2.end()){
				printf("出错了，环不变量数据RingInvariant=%s,I1I2=%s与ID=%d不匹配！\n",strRingInvariant.c_str(),strI1I2.c_str(),vID[0]);
			}
		}
   }	   
#endif   
   return vID[0];
}

//R16
static int IDs[]={-1,28,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,91,92,96,97,98,99,100,121,122,123,124,125,141,142,143,144,145,146,171,172,173,175,176,178,181,182,279,280,281};

bool isR16ID(int i){
	if(i==6||i==13||i==14||i==15||i==29||i==30||i==31||i==32||i==70||i==126||i==127||i==128||i==130||i==131|i==132||i==133||i==134||i==135||i==136||i==137||i==138||i==139||i==140||i==147||i==148||i==152||i==153||i==155||i==156||i==160||i==163||i==165||i==167||i==168||i==169||i==179||i==190||i==228||i==229||i==239||i==241||i==244||i==246||i==248||i==271||i==272||i==273||i==274||i==275||i==276||i==277||i==278||i==180||i==282||i==283||i==286||i==287||i==290||i==377)
		return true;
	return false;
}

//-----------------------------End of IdRing-------------------------------

#endif