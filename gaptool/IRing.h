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
vector<pair<int,int> > doN1Vec(vector<int>& v){
	vector<pair<int,int> > ret;
	int n=v.size();
	for(int i=0;i<n;i++){
		int vi=v[i];
		vector<pair<int,int> >::const_iterator it=std::find_if(ret.begin(),ret.end(),[vi](pair<int,int>& obj)->bool{if(vi==obj.first)return true;return false;});
		if(it==ret.end()){
			ret.push_back(make_pair(vi,1));
		}else{
			int cnt=(*it).second;
			ret[it-ret.begin()]=make_pair(vi,cnt+1);
		}
	}
	return ret;
}

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
	   Subring si;
	   //bool b=si.init(r,vi,n-1);
	   bool b=si.init(r,vi,n/2+1);
	   if(!b)
		   continue;
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
		   Subring si;
		   //bool b=si.init(r,vi,n-1);
		   bool b=si.init(r,vi,n/2+1);
		   if(!b)
			   continue;
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

string calcI3(IRing* r){
	int IdRing(IRing* r);
	int n=r->size();
	vector<pair<int,int> > v;
   for(int i=0;i<n-2;i++){
	for(int j=i+1;j<n-1;j++){
	   for(int k=j+1;k<n;k++){
		   vector<int> vi;
		   vi.push_back(i);
		   vi.push_back(j);
		   vi.push_back(k);		   
		   Subring si;
		   //bool b=si.init(r,vi,n-1);
		   bool b=si.init(r,vi,n/2+1);
		   if(!b)
			   continue;
		   int ni=si.size();
		   if(ni<n && ni>0){
			int ID=IdRing(&si);
#if 1			
			if(ID==-1){
				printf("i=%d,j=%d,k=%d\n",i,j,k);
			    break;
			}
#endif
			v.push_back(make_pair(ni,ID));
		   }
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

string calcI4(IRing* r){
	int IdRing(IRing* r);
	int n=r->size();
	vector<pair<int,int> > v;
   for(int t=0;t<n-3;t++){	
   for(int i=t+1;i<n-2;i++){
	for(int j=i+1;j<n-1;j++){
	   for(int k=j+1;k<n;k++){
		   vector<int> vi;
		   vi.push_back(t);		   
		   vi.push_back(i);
		   vi.push_back(j);
		   vi.push_back(k);		   
		   Subring si;
		   //bool b=si.init(r,vi,n-1);
		   bool b=si.init(r,vi,n/2+1);
		   if(!b)
			   continue;
		   int ni=si.size();
		   if(ni<n && ni>0){
			int ID=IdRing(&si);
#if 1			
			if(ID==-1){
				printf("t=%d,i=%d,j=%d,k=%d\n",t,i,j,k);
			    break;
			}
#endif
			v.push_back(make_pair(ni,ID));
		   }
   }
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

// 零乘个数的分布N6
string calcN6(IRing* r){
    int n=r->size();	
	vector<int> v;
	for(int i=0;i<n;i++){
		int I=0;
		for(int j=0;j<n;j++){
			if(r->mul(i,j)==0)
				I++;
		}
		v.push_back(I);
	}
	std::sort(v.begin(),v.end());
	vector<pair<int,int> > v1=doN1Vec(v);
	string str="[";
	for(int i=0;i<v1.size();i++){
		char sz[200]={0};
		sprintf(sz,"[%d,%d],",v1[i].first,v1[i].second);
		str+=sz;
	}
	if(str.size()>2){
		str=str.substr(0,str.size()-1);
	}
	str+="]";
	return str;
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
	multimap<string,int> m_RingInvariant;//根据环的结构不变量N0n0bAbOn1n2n4n5n6n7n8S1N2N6返回ID编号列表	
	multimap<string,int> m_I1I2;//根据环的结构不变量I1I2返回ID编号列表	
public:	
	int LoadData(char * pszFilePath);		//“从文件中读取环结构不变量数据”
	vector<int> IDFromRingInvariant(string& RingInvariant);	
	vector<int> IDFromI1I2(string& I1I2);	
	vector<string> I1I2FromID(int ID);	
	vector<int> IDFromN0n0bAbO(string& N0n0bAbO);	
};

int RIDHelper::LoadData(char * pszFilePath){
	FILE * fp =fopen(pszFilePath, "r");
	if( fp == NULL )
		return 1;//1打开文件失败

	char sz[400] = {0};
	pair<string,int> item;
	int n = 0;
	n = fscanf(fp, "%s", sz);
	while( n > 0 && !feof(fp) )
	{
		n = fscanf(fp, "%d,%s\n", &item.second, sz);
		if( n > 0 )
		{
			item.first = sz;
			m_RingInvariant.insert(item);
		}
	}
	fclose(fp);
	return 0;//0成功
}

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
	m_RingInvariant.insert(make_pair("[1,1,2,4],8,1,0,8,1,7,7,64,7",1));
	m_RingInvariant.insert(make_pair("[1,1,2,4],8,1,0,8,1,3,3,32,7",2));	
	m_RingInvariant.insert(make_pair("[1,1,2,4],8,1,1,4,2,1,3,20,3,8,[1,1,2,4]",3));
	m_RingInvariant.insert(make_pair("[1,1,2,4],8,1,0,8,1,3,7,48,7",4));	
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,0,8,1,7,7,64,7",5));
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,0,8,1,3,7,48,7,8,[1,1,6,0]",6));	
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,0,8,2,3,3,48,7",7));
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,0,0,8,1,5,7,48,7,2,[1,3,4,0],[[2,4,8],[4,4,8]]",8));	
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,0,0,8,1,5,7,48,7,2,[1,1,6,0]",9));
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,0,0,8,1,5,7,48,7,2,[1,3,4,0],[[4,2,8],[4,4,8]]",10));
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,0,8,1,7,7,40,7",11));
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,0,8,1,3,7,40,7,8,[1,1,6,0]",12));	
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,0,8,2,3,3,32,7",13));
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,1,6,4,1,1,24,5,8,[1,2,5,0]",14));	
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,0,0,8,3,3,3,28,7",15));	
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,0,8,1,3,7,48,7,8,[1,3,4,0]",16));	
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,0,8,2,1,3,36,7",17));
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,0,0,8,1,1,7,40,7",18));	
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,0,8,1,3,7,40,7,8,[1,3,4,0],[[2,4,8],[4,2,8],[4,4,8]]",19));
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,0,0,8,3,3,3,28,3",20));
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,1,4,2,3,3,24,3",21));
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,1,4,2,1,3,20,3",22));	
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,0,8,1,3,7,48,7,8,[1,3,0,4]",23));
	m_RingInvariant.insert(make_pair("[1,3,4,0],4,1,0,8,1,3,3,32,7",24));	
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,0,8,1,7,7,64,7",25));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,0,8,1,3,7,48,7",26));	
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,0,8,2,3,3,48,7",27));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,0,0,8,1,5,7,48,7",28));	
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,0,0,8,3,3,3,40,7,2,[1,5,2,0],[[2,2,24]],[[2,4],[8,4]]",29));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,0,0,8,5,3,3,36,3",30));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,0,8,1,7,7,40,7,8,[1,7,0,0],[[2,2,24]]",31));//I4:[[0,2],[0,0]],[[3,2],[0,3]]
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,0,8,1,3,7,40,7,8,[1,3,4,0],[[2,2,24]]",32));//I4:[[3,1],[0,0]],[[0,1],[0,3]]	
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,0,8,1,3,3,32,7",33));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,0,0,8,3,3,3,40,7,2,[1,5,2,0],[[2,2,24]],[[4,6],[8,2]]",34));	
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,0,8,2,3,3,32,7",35));	
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,0,0,8,5,3,3,32,7,1,[1,7,0,0],[[2,2,32]]",36));//J4:[[1,1],[1,1]],[[1,3],[2,0]]	
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,0,0,8,3,3,3,28,3",37));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,0,8,2,1,3,36,7",38));	
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,0,0,8,1,1,7,40,7",39));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,0,8,4,1,1,36,7",40));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,0,0,8,6,1,1,30,5,2",41));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,0,8,2,1,1,28,7,8",42));	
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,0,0,8,5,3,3,36,7",43));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,0,0,8,3,3,3,28,7",44));	
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,1,4,2,3,3,24,3,8,[1,4,3,0],[[2,2,40]]",45));	
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,1,4,2,1,3,20,3",46));	
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,0,0,8,6,1,1,30,7",47));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,1,6,4,1,1,24,5",48));	
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,0,1,6,6,1,1,26,5",49));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,1,7,8,0,0,27,6",50));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,1,5,4,0,0,21,4",51));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,1,1,1,2,0,0,15,0",52));
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
#if 0
	m_RingInvariant.insert(make_pair("[1,1,2,4,8],16,1,0,16,1,15,15,256,15,16,[1,1,2,4,8],[],[[16,16]]",1));//A16
	m_RingInvariant.insert(make_pair("[1,1,2,4,8],16,1,0,16,1,3,7,80,15,16,[1,1,2,4,8],[[4,16,16],[8,8,16],[8,16,32],[16,4,16],[16,8,32],[16,16,64]],[[2,8],[4,4],[8,2],[16,2]]",2));
	m_RingInvariant.insert(make_pair("[1,1,2,4,8],16,1,1,8,2,3,3,48,7,16,[1,1,2,4,8],[[2,16,8],[4,8,8],[4,16,16],[8,4,8],[8,8,16],[8,16,32],[16,2,8],[16,4,16],[16,8,32],[16,16,64]],[[1,8],[2,4],[4,2],[8,1],[16,1]]",3));//C16
	m_RingInvariant.insert(make_pair("[1,1,2,4,8],16,1,0,16,1,7,15,128,15,16,[1,1,2,4,8],[[8,16,32],[16,8,32],[16,16,64]],[[4,8],[8,4],[16,4]]",4));//B16	
	m_RingInvariant.insert(make_pair("[1,1,2,4,8],16,1,0,16,1,7,15,192,15,16,[1,1,2,4,8],[[16,16,64]],[[8,8],[16,8]]",5));
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,1,12,4,1,3,60,11,16,[1,2,3,10,0],[[2,2,4],[2,4,4],[2,8,20],[4,2,4],[4,4,16],[4,8,32],[8,2,20],[8,4,32],[8,8,64]],[[1,4],[2,6],[4,3],[8,2],[16,1]]",6));
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
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,0,0,16,3,3,7,72,7,4,[1,3,4,8,0],[[2,8,16],[4,4,16],[4,8,32],[8,2,24],[8,4,32],[8,8,64]],[[1,8],[4,4],[8,2],[16,2]]",20));	
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,0,0,16,1,11,15,192,15,4,[1,3,4,8,0],[[8,2,16],[8,4,16],[8,8,32]],[[8,8],[16,8]]",21));	
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,7,15,192,15,16,[1,3,4,0,8],[[8,8,64]],[[8,8],[16,8]]",22));	
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,0,0,16,3,3,7,72,15,4,[1,3,4,8,0],[[2,8,24],[4,4,16],[4,8,32],[8,2,16],[8,4,32],[8,8,64]],[[2,8],[4,4],[8,3],[16,1]]",23));	
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,7,7,128,15,16,[1,3,4,8,0],[[2,8,16],[4,8,16],[8,2,16],[8,4,16],[8,8,64]],[[4,8],[8,4],[16,4]]",24));//[[3,1],[1,3]],[[0,4],[4,0]]	
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,0,0,16,1,7,7,128,15,4,[1,3,4,8,0],[[4,8,32],[8,2,16],[8,4,16],[8,8,64]],[[4,8],[8,4],[16,4]]",25));//[[2,0],[1,0]],[[4,0],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,7,15,160,15,16,[1,3,4,8,0],[[2,8,16],[4,8,16],[8,2,16],[8,4,16],[8,8,32]],[[8,12],[16,4]]",26));//[[4,0],[0,4]],[[0,0],[7,4]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,7,15,128,15,16,[1,3,4,0,8],[[2,8,16],[4,8,16],[8,2,16],[8,4,16],[8,8,64]],[[4,8],[8,4],[16,4]]",27));//[[0,4],[3,2]]	
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,0,0,16,1,11,15,192,15,4,[1,1,6,8,0],[[2,2,4],[2,4,4],[2,8,8],[4,2,4],[4,4,4],[4,8,8],[8,2,8],[8,4,8],[8,8,16]],[[8,8],[16,8]]",28));//R32_1040的商环
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,7,15,192,15,16,[1,1,6,8,0],[[2,2,4],[2,4,4],[2,8,8],[4,2,4],[4,4,4],[4,8,8],[8,2,8],[8,4,8],[8,8,16]],[[8,8],[16,8]]",29));//R8_1×R8_2=R8R4_53的商环
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,3,7,112,15,16,[1,1,6,8,0],[[2,2,4],[2,4,4],[2,8,8],[4,2,4],[4,4,4],[4,8,24],[8,2,8],[8,4,24],[8,8,64]],[[4,8],[8,6],[16,2]]",30));//R8_2×R8_2=R8R4_54的商环
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,0,16,1,7,15,160,15,16,[1,1,6,8,0],[[2,2,4],[2,4,4],[2,8,8],[4,2,4],[4,4,4],[4,8,8],[8,2,8],[8,4,8],[8,8,48]],[[8,12],[16,4]]",31));//R8_4×R8_2=R8R4_56的商环
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,1,1,8,2,3,7,56,7,16,[1,1,6,4,4],[[2,2,4],[2,4,4],[2,8,24],[4,2,4],[4,4,12],[4,8,32],[8,2,24],[8,4,32],[8,8,64]],[[1,8],[4,6],[8,1],[16,1]]",32));//Rpp(9,8)的商环、R8_3/([[4,0,1])的商环
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,0,0,16,1,3,15,160,15,4,[1,1,6,8,0],[[2,2,4],[2,4,4],[2,8,8],[4,2,4],[4,4,4],[4,8,8],[8,2,8],[8,4,8],[8,8,48]],[[8,12],[16,4]]",33));//R32_2085的商环
	m_RingInvariant.insert(make_pair("[1,3,4,8,0],8,0,0,16,1,3,7,112,15,4,[1,1,6,8,0],[[2,2,4],[2,4,4],[2,8,8],[4,2,4],[4,4,4],[4,8,24],[8,2,8],[8,4,24],[8,8,64]],[[4,8],[8,6],[16,2]]",34));//R32_2117的商环
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,12,0,0],[[4,4,96]],[[8,12],[16,4]]",67));
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,8,4,0],[[4,4,96]],[[4,8],[16,8]]",68));
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,4,8,0],[[4,4,96]],[[4,8],[16,8]]",69));	
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
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,8,4,0],[[4,4,96]],[[8,12],[16,4]]",81));	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,11,15,192,15,4,[1,3,8,4,0],[[4,4,64]]",82));	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,11,15,192,15,4,[1,3,12,0,0],[[4,4,64]]",83));	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,7,15,160,15,16,[1,3,4,8,0],[[4,4,96]]",84));//[[0,1,1,0],[1,1,2,1],[1,1,2,1],[1,0,1,1]],[[1,1,0,1],[0,2,2,0],[2,0,2,2],[3,1,2,3]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,4,8,0],[[4,4,96]],[[8,12],[16,4]]",85));//[[0,1,1],[0,2,2],[0,2,2]],[[0,1,3],[0,3,3],[0,3,3]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,11,15,144,15,4,[1,3,12,0,0],[[4,4,112]]",86));//[[2,3,0],[0,2,0],[0,2,0]],[[2,3,1],[0,2,0],[0,2,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,3,15,144,15,4,[1,3,8,4,0],[[4,4,112]]",87));//[[3,1,1,3],[3,1,1,3],[1,3,3,1],[3,1,1,3]],[[0,1,1,0],[0,1,1,0],[2,3,3,2],[0,1,1,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,7,15,144,15,4,[1,3,12,0,0],[[4,4,112]]",88));//[[0,0,0],[0,1,1],[0,1,1]],[[2,3,3],[0,2,0],[0,0,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,7,15,144,15,4,[1,3,8,4,0],[[4,4,112]]",89));//[[0,0,0],[0,0,0],[0,1,0]],[[0,0,0],[2,0,0],[1,0,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,3,15,128,15,4,[1,3,8,4,0],[[4,4,128]]",90));//[[0,0,0],[0,0,0],[0,1,2]],[[0,0,0],[0,2,0],[1,0,2]]	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,7,7,128,15,16,[1,3,4,0,8],[[2,4,16],[4,2,16],[4,4,96]],[[4,8],[8,4],[16,4]]",91));
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,1,8,2,3,7,56,7,16,[1,3,6,2,4],[[2,4,32],[4,2,32],[4,4,136]],[[1,8],[4,6],[8,1],[16,1]]",92));	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,3,15,112,15,16,[1,3,0,12,0],[[4,4,144]]",93));//M2(R8_8)、R8R8_584的子环
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,3,15,112,15,4,[1,3,4,8,0],[[4,4,144]]",94));//M2(R8_8)、R8_18×R8_11=R8R8_538的子环
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,7,15,128,15,16,[1,3,4,8,0],[[4,4,128]]",95));//M2(R8_8)、R8_11×R8_6=R8R8_271的子环
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,7,15,192,15,16,[1,3,8,4,0],[[4,4,64]]",96));	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,7,15,192,15,16,[1,3,4,8,0],[[4,4,64]]",97));
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,7,15,128,15,16,[1,3,8,4,0],[[4,4,128]]",98));
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,3,15,112,15,16,[1,3,4,8,0],[[4,4,144]]",99));	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,15,15,160,15,16,[1,3,12,0,0],[[4,4,96]]",100));		
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,5,5,7,88,7,1,[1,3,12,0,0],[[2,4,24],[4,2,24],[4,4,120]]",101));//G16
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,5,5,7,88,15,1,[1,3,12,0,0],[[2,4,24],[4,2,24],[4,4,120]]",102));//H16	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,3,15,144,15,16,[1,3,12,0,0],[[4,4,112]]",103));
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,1,12,4,3,3,64,11,16,[1,3,6,6,0],[[2,4,28],[4,2,28],[4,4,136]]",104));//J16	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,1,8,2,3,3,48,7,16,[1,3,2,6,4],[[2,4,32],[4,2,32],[4,4,144]]",105));
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,1,4,2,3,3,40,3,16,[1,3,2,2,8],[[2,4,36],[4,2,36],[4,4,144]]",106));//K16
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,15,15,256,15,16,[1,3,12,0,0],[]",107));//D16
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,12,0,0],[[4,4,96]],[[4,8],[16,8]]",108));	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,3,7,80,15,16,[1,3,4,0,8],[[2,4,16],[4,2,16],[4,4,144]]",109));
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,1,8,2,7,7,56,7,16,[1,3,6,2,4],[[2,4,32],[4,2,32],[4,4,136]]",110));//I16
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,1,7,15,128,15,16,[1,3,4,0,8],[[2,4,16],[4,2,16],[4,4,96]]",111));//E16
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,2,7,7,128,15,16,[1,3,6,2,4],[[2,4,16],[4,2,16],[4,4,96]]",112));//F16
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,1,0,16,2,3,7,96,15,16,[1,3,6,2,4],[[2,4,16],[4,2,16],[4,4,128]]",113));
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,144,15,4,[1,3,12,0,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,32]],[[8,14],[16,2]]",118));//R32_3229的商环		
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,1,8,2,5,7,64,7,4,[1,5,4,6,0],[[2,2,16],[2,4,56],[4,2,56],[4,4,64]],[[1,8],[4,4],[8,3],[16,1]]",119));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,7,128,15,4,[1,3,4,8,0],[[2,2,32],[2,4,32],[4,2,32],[4,4,32]],[[4,8],[8,4],[16,4]]",120));//R32_4766的商环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,7,112,15,4,[1,3,4,8,0],[[2,2,32],[2,4,32],[4,2,32],[4,4,48]],[[4,8],[8,6],[16,2]]",121));//R32_3627的商环		
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,7,112,15,4,[1,3,4,8,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,64]],[[4,8],[8,6],[16,2]]",122));//R32_4728的商环		
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,1,8,2,1,7,56,7,4,[1,1,8,6,0],[[2,2,24],[2,4,56],[4,2,56],[4,4,64]]",123));//R32_4668的商环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,1,8,2,3,7,56,7,16,[1,3,6,6,0],[[2,2,24],[2,4,56],[4,2,56],[4,4,64]]",124));//R32_4667的商环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,144,15,4,[1,1,14,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,40]]",125));//R8R4_91、R16_153×R4_2=R16R4_543的商环	
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
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,144,15,4,[1,1,14,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,40]]",141));//R32_3183的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,144,15,4,[1,5,10,0,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,32]]",142));//R32_3187的商环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,160,15,4,[1,3,12,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,24]]",143));//R32_3230的商环		
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,160,15,4,[1,7,8,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,24]]",144));//R32_4483的商环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,144,15,4,[1,7,8,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,40]]",145));//R32_3277的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,144,15,4,[1,3,12,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,40]]",146));//R32_3322的商环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,3,15,112,15,16,[1,3,8,4,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,64]]",147));//R8_19×R8_12=R8R8_591的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,3,15,112,15,16,[1,3,4,8,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,64]]",148));//R8_12×R8_12=R8R8_584的子环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,112,15,4,[1,3,4,8,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,64]]",149));//R8_18×R8_11=R8R8_538、M2(R8_9)的子环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,11,15,144,15,4,[1,3,12,0,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,32]],[[4,4],[8,8],[16,4]]",150));//M2(R8_9)的子环
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
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,1,14,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,24]]",171));//R8R4_61的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,5,10,0,0],[[2,2,16],[2,4,16],[4,2,32],[4,4,32]]",172));//R8R4_62的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,7,15,144,15,16,[1,3,12,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,40]]",173));//R8R4_63的商环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,3,3,7,72,15,4,[1,3,8,4,0],[[2,2,16],[2,4,56],[4,2,48],[4,4,64]]",174));//R8R4_386=R8_22×R4_8的子环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,12,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,24]]",175));//R8R4_217的商环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,7,15,144,15,4,[1,3,12,0,0],[[2,2,24],[2,4,24],[4,2,24],[4,4,40]]",176));//R8R4_226的商环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,3,3,7,72,7,4,[1,3,8,4,0],[[2,2,16],[2,4,48],[4,2,56],[4,4,64]]",177));//R8R4_334=R8_22×R4_7的子环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,5,7,7,104,15,2,[1,7,8,0,0],[[2,4,56],[4,2,32],[4,4,64]]",178));//Rppp(43,4)的商环	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,1,8,2,3,3,48,7,16,[1,3,2,6,4],[[2,2,32],[2,4,56],[4,2,56],[4,4,64]]",179));//R4_3/([[2,0,0,1])的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,15,15,160,15,16,[1,7,8,0,0],[[2,4,32],[4,2,32],[4,4,32]]",180));//R8R4_11的商环、R4_3/([[0,0,2],[0,1,1],[2]],[0,0,1,1])
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,1,3,7,112,15,16,[1,3,4,8,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,64]]",181));//R8R4_232的商环
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,0,0,16,1,3,15,144,15,4,[1,3,12,0,0],[[2,2,16],[2,4,32],[4,2,32],[4,4,32]]",182));//R32_3327的商环	
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
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,3,15,144,15,4,[1,3,12,0,0],[[2,2,112]],[[8,14],[16,2]]",275));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,11,15,144,15,4,[1,11,4,0,0],[[2,2,112]],[[8,14],[16,2]]",276));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,3,7,112,15,4,[1,3,4,8,0],[[2,2,144]]",277));//R32_5161的商环
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,9,7,7,136,7,1,[1,15,0,0,0],[[2,2,120]]",278));//[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,1,0,0]],[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,1,0,1]],[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,1,1,0]],[[0,0,0,0],[0,0,0,0],[0,0,0,0],[1,0,0,0]]	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,9,7,7,112,15,1,[1,15,0,0,0],[[2,2,144]]",279));//R32_5129的商环		
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,7,7,104,15,2,[1,11,4,0,0],[[2,2,152]]",280));//R32_5121的商环		
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,7,7,104,7,2,[1,11,4,0,0],[[2,2,152]]",281));//R32_5034的商环	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,7,15,144,15,4,[1,7,8,0,0],[[2,2,112]]",282));//R16_153×R4_5=R16R4_1713的商环
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,7,15,160,15,4,[1,7,8,0,0],[[2,2,96]]",283));		
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,1,12,6,3,3,72,11,4,[1,8,7,0,0],[[2,2,184]],[[1,4],[2,4],[4,3],[8,4],[16,1]]",284));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,5,7,112,15,1,[1,9,6,0,0],[[2,2,144]],[[4,6],[8,9],[16,1]]",285));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,1,7,15,144,15,16,[1,7,8,0,0],[[2,2,112]]",286));//R8_31×R4_5=R8R4_239的商环	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,0,16,1,3,7,112,15,16,[1,3,4,8,0],[[2,2,144]]",287));//R8_33×R4_5=R8R4_241的商环	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,7,15,144,15,4,[1,7,8,0,0],[[2,2,112]]",288));//M2(R8_39)的子环
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,3,3,7,72,15,4,[1,5,6,4,0],[[2,2,184]]",289));//M2(R8_46)、R8_46×R8_36=R8R8_1866的子环
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,3,15,112,15,4,[1,3,12,0,0],[[2,2,144]]",290));//R8_31×R8_39=R8R8_2007的子环
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,3,15,128,15,4,[1,3,12,0,0],[[2,2,128]]",291));//M2(R8_39)的子环
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,3,15,144,15,4,[1,3,12,0,0],[[2,2,112]],[[4,4],[8,8],[16,4]]",292));//R8_28×R8_39=R8R8_2004、M2(R8_39)的子环
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
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,3,7,7,160,15,4,[1,9,6,0,0],[[2,2,96]],[[4,8],[16,8]]",308));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,3,3,7,120,15,4,[1,5,6,4,0],[[2,2,136]],[[2,4],[4,4],[8,4],[16,4]]",309));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,6,3,3,120,15,4,[1,8,7,0,0],[[2,2,136]],[[2,4],[4,4],[8,4],[16,4]]",310));
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
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,3,7,7,160,15,4,[1,9,6,0,0],[[2,2,96]],[[8,12],[16,4]]",354));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,3,3,7,120,15,4,[1,5,6,4,0],[[2,2,136]],[[4,6],[8,8],[16,2]]",355));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,6,3,3,120,15,4,[1,8,7,0,0],[[2,2,136]],[[4,6],[8,8],[16,2]]",356));
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
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,1,12,6,3,3,72,11,4,[1,8,7,0,0],[[2,2,184]],[[1,4],[2,2],[4,6],[8,3],[16,1]]",371));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,9,3,3,92,11,1,[1,11,4,0,0],[[2,2,164]]",372));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,5,7,88,7,1,[1,9,6,0,0],[[2,2,168]]",373));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,1,8,2,5,7,64,7,4,[1,6,7,2,0],[[2,2,192]]",374));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,5,7,112,15,1,[1,9,6,0,0],[[2,2,144]],[[2,8],[8,4],[16,4]]",375));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,5,7,112,15,2,[1,9,6,0,0],[[2,2,144]]",376));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,8,2,3,7,64,7,16,[1,4,7,4,0],[[2,2,192]]",377));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,5,3,3,76,15,1,[1,7,8,0,0],[[2,2,180]]",378));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,0,0,16,1,11,15,144,15,4,[1,11,4,0,0],[[2,2,112]],[[4,4],[8,8],[16,4]]",379));
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
#endif
	//R16
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,8],[4,5,4]],[[2,1,3],[4,1,20],[4,4,3],[4,5,10],[8,9,48],[8,11,24],[8,26,12]]",118));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,8],[4,5,4]],[[2,1,3],[4,1,20],[4,4,3],[4,5,10],[8,5,24],[8,26,12]]",150));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,12]],[[2,1,3],[4,4,3],[4,5,30],[8,26,12],[8,32,24],[8,39,48]]",275));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,12]],[[2,1,3],[4,4,3],[4,5,30],[8,26,36]]",292));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,11],[4,5,4]],[[2,1,11],[4,4,15],[4,5,10],[8,26,12],[8,28,48],[8,31,24]]",276));		
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,11],[4,5,4]],[[2,1,11],[4,4,39],[4,5,10],[8,26,12]]",379));	
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
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[4,3,2],[4,5,2],[8,21,4],[8,22,2]],[[2,1,5],[4,3,5],[4,4,6],[4,5,5],[8,21,34],[8,22,17],[8,28,12]]",119));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,2],[4,2,2],[4,5,2],[8,2,8]],[[2,1,1],[4,1,5],[4,2,5],[4,5,5],[8,2,44],[8,6,12]]",34));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,24,4],[8,33,4]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12],[8,24,22],[8,33,22]]",120));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[8,24,4],[8,33,4]],[[2,1,3],[4,2,10],[4,4,3],[8,16,12],[8,24,22],[8,33,22]]",121));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,24,8]],[[2,1,3],[4,4,3],[4,5,10],[8,24,44],[8,26,12]]",122));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,3,2],[4,5,6],[8,22,6]],[[2,1,1],[4,3,5],[4,5,15],[8,22,51],[8,39,12]]",123));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,3,2],[4,5,4],[8,21,2],[8,22,4]],[[2,1,3],[4,3,5],[4,4,3],[4,5,10],[8,21,17],[8,22,34],[8,32,12]]",124));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,33,8]],[[2,1,3],[4,4,3],[4,5,10],[8,26,12],[8,33,44]]",277));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,8]],[[2,1,7],[2,2,8],[4,4,21],[4,7,84]]",278));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,8]],[[2,1,7],[2,2,8],[4,4,21],[4,7,36],[4,8,12],[8,36,36]]",279));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,4],[4,9,4]],[[2,1,7],[2,2,4],[4,4,21],[4,8,18],[4,9,16],[8,44,54]]",280));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,4],[4,9,4]],[[2,1,7],[2,2,4],[4,4,21],[4,7,18],[4,9,16],[8,37,54]]",281));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,2],[4,2,2],[4,5,2],[8,1,8]],[[2,1,1],[4,1,5],[4,2,5],[4,5,5],[8,1,44],[8,6,12]]",28));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,2],[4,2,2],[4,5,2],[8,4,8]],[[2,1,1],[4,1,5],[4,2,5],[4,5,5],[8,4,44],[8,6,12]]",33));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,8],[4,5,4]],[[2,1,3],[4,2,20],[4,4,3],[4,5,10],[8,16,12],[8,18,48],[8,19,12],[8,32,12]]",146));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,8],[4,5,4]],[[2,1,3],[4,2,20],[4,4,3],[4,5,10],[8,18,48],[8,19,24],[8,26,12]]",182));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,2],[4,2,6],[4,5,6]],[[2,1,1],[4,1,5],[4,2,15],[4,5,15],[8,6,12],[8,12,24],[8,18,36],[8,39,12]]",141));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[4,1,6],[4,2,2],[4,5,2]],[[2,1,5],[4,1,15],[4,2,5],[4,4,6],[4,5,5],[8,6,12],[8,8,12],[8,9,12],[8,10,12],[8,11,24],[8,28,12]]",142));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,8],[4,5,4]],[[2,1,3],[4,1,20],[4,4,3],[4,5,10],[8,5,12],[8,9,48],[8,11,12],[8,32,12]]",143));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,4],[4,2,4]],[[2,1,7],[4,1,10],[4,2,10],[4,4,9],[8,5,12],[8,8,24],[8,10,24],[8,19,12],[8,31,12]]",144));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,4],[4,2,4]],[[2,1,7],[4,1,10],[4,2,10],[4,4,9],[8,8,24],[8,10,24],[8,11,12],[8,16,12],[8,31,12]]",145));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[4,5,4],[8,23,4]],[[2,1,3],[4,2,10],[4,4,3],[4,5,10],[8,16,12],[8,23,22],[8,26,12]]",147));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,23,8]],[[2,1,3],[4,4,3],[4,5,10],[8,23,44],[8,26,12]]",165));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,23,8]],[[2,1,3],[4,4,3],[4,5,10],[8,23,44],[8,26,12]]",168));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[4,3,8],[4,5,2]],[[2,1,5],[4,3,20],[4,4,6],[4,5,5],[8,15,24],[8,20,24],[8,28,12]]",169));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,3,2],[8,21,2],[8,33,4]],[[2,1,3],[4,3,5],[4,4,3],[8,21,17],[8,33,22]]",179));		
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,6],[4,2,2],[4,5,6]],[[2,1,1],[4,1,15],[4,2,5],[4,5,15],[8,9,36],[8,12,36],[8,39,12]]",125));
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
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[8,23,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[8,5,12],[8,16,12],[8,23,22]]",68));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[8,23,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[8,5,12],[8,16,12],[8,23,22]]",81));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,8],[8,23,4]],[[2,1,3],[4,1,20],[4,4,3],[8,5,24],[8,23,22]]",82));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,8],[4,2,4]],[[2,1,3],[4,1,20],[4,2,10],[4,4,3],[8,5,24],[8,16,12]]",83));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,23,8]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12],[8,23,44]]",84));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,23,8]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12],[8,23,44]]",69));	
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
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,8]],[[2,1,3],[4,1,10],[4,2,20],[4,4,3],[8,5,12],[8,16,24]]",67));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,8]],[[2,1,3],[4,1,10],[4,2,20],[4,4,3],[8,5,12],[8,16,24]]",108));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4]],[[2,1,3],[4,2,10],[4,4,3],[8,16,12]]",109));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,3,2],[8,21,2]],[[2,1,3],[4,1,10],[4,3,5],[4,4,3],[8,11,12],[8,21,17]]",110));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12]]",111));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,3,2],[8,13,2]],[[2,1,3],[4,1,10],[4,3,5],[4,4,3],[8,5,12],[8,13,17]]",112));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[4,3,2],[8,13,2]],[[2,1,3],[4,2,10],[4,3,5],[4,4,3],[8,13,17],[8,16,12]]",113));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12]]",91));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[4,3,2],[8,21,2]],[[2,1,3],[4,2,10],[4,3,5],[4,4,3],[8,19,12],[8,21,17]]",92));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[8,23,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[8,5,12],[8,16,12],[8,23,22]]",96));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,23,8]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12],[8,23,44]]",97));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[8,23,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[8,5,12],[8,16,12],[8,23,22]]",98));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[8,23,8]],[[2,1,3],[4,2,10],[4,4,3],[8,16,12],[8,23,44]]",99));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,12]],[[2,1,3],[4,1,30],[4,4,3],[8,5,36]]",100));
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
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[4,1,6],[4,2,2],[4,5,6]],[[2,1,1],[4,1,15],[4,2,5],[4,5,15],[8,6,24],[8,9,36],[8,12,12],[8,39,12]]",171));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[4,1,2],[4,2,6],[4,5,2]],[[2,1,5],[4,1,5],[4,2,15],[4,4,6],[4,5,5],[8,6,12],[8,10,24],[8,16,12],[8,18,12],[8,19,12],[8,28,12]]",172));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,6,24],[8,11,12],[8,12,24],[8,16,12],[8,32,12]]",173));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,6,24],[8,8,12],[8,9,12],[8,10,12],[8,18,12],[8,32,12]]",175));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[4,5,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[8,8,12],[8,9,12],[8,10,12],[8,12,24],[8,18,12],[8,32,12]]",176));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,4],[8,24,8]],[[2,1,3],[4,4,3],[4,5,10],[8,24,44],[8,26,12]]",181));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,3,8]],[[2,1,7],[4,3,20],[4,4,21],[8,15,72]]",178));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,8]],[[2,1,7],[4,1,20],[4,4,21],[8,5,24],[8,11,48]]",180));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,6,2],[4,9,5]],[[2,1,3],[2,2,5],[4,4,3],[4,6,8],[4,7,3],[4,8,3],[4,9,20],[4,10,6],[8,34,9],[8,37,9],[8,45,9],[8,48,18],[8,49,6]]",284));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[2,2,4],[4,5,2],[4,6,4]],[[2,1,5],[2,2,4],[4,4,6],[4,5,5],[4,6,16],[4,8,18],[8,28,12],[8,34,18]]",285));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,5,8]],[[2,1,7],[4,4,21],[4,5,20],[8,26,24]]",288));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,5,12]],[[2,1,3],[4,4,3],[4,5,30],[8,26,36]]",291));
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
#if 0	
	m_RingInvariant.insert(make_pair("[1,2,6,18],27,1,0,27,1,26,26,729,26,27",1));//Rppp(1,3)
	m_RingInvariant.insert(make_pair("[1,2,6,18],27,1,0,27,1,8,8,189,26,27",2));//Rppp(2,3)
	m_RingInvariant.insert(make_pair("[1,2,6,18],27,1,1,9,2,2,8,81,8,27",3));//Rppp(3,3)
	m_RingInvariant.insert(make_pair("[1,2,6,18],27,1,0,27,1,8,26,405,26,27",4));//Rppp(4,3)	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,26,26,729,26,27,[1,8,18,0],[]",5));//Rppp(5,3)、R9R3_1
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,14,26,405,26,3,[1,2,24,0],[[3,3,36],[3,9,72],[9,3,72],[9,9,144]],[[9,18],[27,9]]",6));//rank=2,R81_107的商环	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,2,8,8,405,26,27",7));//Rppp(7,3)=R9R3_12	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,14,26,405,26,3,[1,8,18,0],[[3,9,108],[9,9,216]]",8));//Rppp(8,3),R27_8有子环R3_1和R9_1，但不是可分解环	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,8,26,405,26,27,[1,2,24,0],[[3,3,36],[3,9,72],[9,3,72],[9,9,144]],[[9,18],[27,9]]",9));//rank=2,R81_211的商环	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,14,26,405,26,3,[1,8,18,0],[[9,3,108],[9,9,216]]",10));//Rppp(10,3)	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,14,26,297,26,27,[1,8,18,0],[[3,9,108],[9,3,108],[9,9,216]]",11));//Rppp(11,3)
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,14,26,297,26,27,[1,2,24,0],[[3,3,36],[3,9,72],[9,3,72],[9,9,252]],[[9,24],[27,3]]",12));//rank=2,R81_108的商环	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,2,8,8,189,26,27,[1,8,6,12],[[3,9,108],[9,3,108],[9,9,324]]",13));//Rppp(13,3)、R9R3_2
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,1,15,4,2,2,105,14,27",14));//Rppp(14,3)=R9R3_13	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,4,8,8,153,26,3,[1,8,18,0],[[3,9,144],[9,3,108],[9,9,324]]",15));//Rppp(15,3)
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,8,26,405,26,27,[1,8,18,0],[[9,9,324]]",16));//Rppp(16,3),R9R3_3
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,2,2,8,225,26,27",17));//Rppp(17,3)=R9R3_14	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,26,26,297,26,3,[1,8,18,0],[[3,9,108],[9,3,108],[9,9,216]],[[9,24],[27,3]]",18));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,8,26,297,26,3,[1,8,18,0],[[3,9,108],[9,3,108],[9,9,216]]",19));//Rppp(19,3)	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,4,8,8,153,8,3,[1,8,18,0],[[3,9,108],[9,3,144],[9,9,324]]",20));//Rppp(20,3)	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,1,9,2,8,8,117,8,27,[1,8,6,12],[[3,9,144],[9,3,144],[9,9,324]]",21));//Rppp(21,3)
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,1,9,2,2,8,81,8,27,[1,2,12,12],[[3,3,36],[3,9,144],[9,3,144],[9,9,324]],[[1,18],[3,6],[9,2],[27,1]]",22));//rank=1,R81_102的商环	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,8,26,405,26,27,[1,8,0,18],[[9,9,324]]",23));//Rppp(23,3)	
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,8,8,189,26,27,[1,8,0,18],[[3,9,108],[9,3,108],[9,9,324]]",24));//Rppp(24,3)	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,1,26,26,729,26,27,[1,26,0,0],[]",25));//Rppp(25,3)、R9R3_4
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,4,8,8,297,26,3,[1,14,12,0],[[3,3,432]],[[3,18],[27,9]]",26));//Rppp(26,3)=R9R3_7
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,2,8,8,405,26,27,[1,10,16,0],[[3,3,324]],[[9,18],[27,9]]",27));//Rppp(27,3)、R9R3_6、R9R3_15	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,4,8,8,297,26,3,[1,14,12,0],[[3,3,432]],[[9,24],[27,3]]",28));//Rppp(28,3)=R9R3_8、Rppp(29,3)、Rppp(34,3)
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,1,2,26,297,26,27,[1,2,24,0],[[3,3,432]],[[9,24],[27,3]]",29));//rank=2,R81_329的商环	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,10,8,8,261,8,1,[1,26,0,0],[[3,3,468]]",30));//Rppp(30,3)
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,1,14,26,297,26,27,[1,14,12,0],[[3,3,432]],[[9,24],[27,3]]",31));//M2(R9_9)的子环		
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,2,2,8,225,26,27",33));//Rppp(33,3)=R9R3_16	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,1,2,26,297,26,3,[1,2,24,0],[[3,3,432]],[[9,24],[27,3]]",34));//rank=2,R81_381的商环		
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,2,8,8,189,26,27",35));//Rppp(35,3)=R9R3_9	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,10,8,8,189,26,1,[1,26,0,0],[[3,3,540]]",36));//Rppp(36,3)	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,4,8,8,153,8,3,[1,14,12,0],[[3,3,576]]",37));//Rppp(37,3)		
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,4,2,2,225,26,27,[1,8,14,4],[[3,3,504]]",38));//Rppp(38,3)=R9R3_10=R9R3_17	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,1,8,26,297,26,3,[1,8,18,0],[[3,3,432]]",39));//Rppp(39,3)		
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,8,2,2,165,14,3,[1,16,10,0],[[3,3,564]]",41));//Rppp(41,3)、R9R3_18	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,2,2,2,153,26,27",42));//Rppp(42,3)=R9R3_11
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,10,8,8,261,26,1,[1,26,0,0],[[3,3,468]]",43));//Rppp(43,3)	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,4,8,8,153,26,3,[1,14,12,0],[[3,3,576]]",44));//Rppp(44,3)	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,9,2,8,8,117,8,27,[1,10,16,0],[[3,3,612]],[[1,18],[9,8],[27,1]]",45));//Rppp(45,3)=R3_2/([[0,0,0,0,0,1],[1,0,0,0,1]],[0,0,0,0,0,0,1])		
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,15,4,2,2,105,14,27,[1,8,14,4],[[3,3,624]]",46));//rank=1,Rppp(46,3)、R9R3_20、R3_2/([0,0,1,1])=R3_2/([0,0,2,1])	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,8,2,2,165,26,3,[1,16,10,0],[[3,3,564]]",47));//Rppp(47,3)、Rppp(44,3)、R9R3_19	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,1,14,26,405,26,3,[1,14,12,0],[[3,3,324]],[[9,18],[27,9]]",48));//rank=2,R81_304的子环	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,1,15,8,2,2,129,14,3,[1,16,10,0],[[3,3,600]]",49));//Rppp(29,3)、Rppp(49,3)=[[1,0],[0,0]],[[1,1],[0,1]]	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,19,8,0,0,125,18,27,[1,14,12,0],[[3,3,604]],[[1,8],[3,12],[9,6],[27,1]]",50));//Rppp(50,3)、R9R3_21、R3_2/([0,2,0,1])			
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,11,4,0,0,85,10,27,[1,6,8,12],[[3,3,644]]",51));//Rppp(51,3)、R9R3_22、R3_2/([1,1,0,1])	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,9,2,2,8,81,8,27",52));//Rppp(52,3)=R3_2/([1,0,0,1])=R3_2/([0,0,0,1])
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,8,26,297,26,3,[1,2,24,0],[[3,3,36],[3,9,72],[9,3,72],[9,9,252]],[[9,24],[27,3]]",53));//rank=2,R81_214的商环
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,2,26,297,26,27,[1,2,24,0],[[3,3,36],[3,9,72],[9,3,72],[9,9,252]],[[9,24],[27,3]]",54));//rank=2,R81_215的商环
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,2,26,297,26,3,[1,2,24,0],[[3,3,36],[3,9,72],[9,3,72],[9,9,252]],[[9,24],[27,3]]",55));//rank=2,R81_217的商环	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,1,8,26,405,26,27",56));//Rppp(56,3)=R9R3_5
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,1,8,8,189,26,27,[1,8,0,18],[[3,3,540]],[[3,18],[9,6],[27,3]]",57));//Rppp(57,3)=R3_2/([[0,0,0,1,1],[0,1,0,0,1]],[0,0,0,0,1,1])	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,1,26,26,297,26,3,[1,26,0,0],[[3,3,432]],[[9,24],[27,3]]",58));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,1,2,0,0,53,0,27",59));//Rppp(59,3)=R3_2/([2,2,0,1])		
#else
	m_RingInvariant.insert(make_pair("[1,2,6,18],27,1,0,27,1,26,26,729,26,27,[1,2,6,18],[],[[27,27]]",1));
	m_RingInvariant.insert(make_pair("[1,2,6,18],27,1,0,27,1,8,8,189,26,27,[1,2,6,18],[[9,27,108],[27,9,108],[27,27,324]],[[3,18],[9,6],[27,3]]",2));
	m_RingInvariant.insert(make_pair("[1,2,6,18],27,1,1,9,2,2,8,81,8,27,[1,2,6,18],[[3,27,36],[9,9,36],[9,27,108],[27,3,36],[27,9,108],[27,27,324]],[[1,18],[3,6],[9,2],[27,1]]",3));
	m_RingInvariant.insert(make_pair("[1,2,6,18],27,1,0,27,1,8,26,405,26,27,[1,2,6,18],[[27,27,324]],[[9,18],[27,9]]",4));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,26,26,729,26,27,[1,8,18,0],[],[[27,27]]",5));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,14,26,405,26,3,[1,2,24,0],[[3,3,36],[3,9,72],[9,3,72],[9,9,144]],[[9,18],[27,9]]",6));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,2,8,8,405,26,27,[1,4,10,12],[[3,3,36],[3,9,72],[9,3,72],[9,9,144]],[[9,18],[27,9]]",7));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,14,26,405,26,3,[1,8,18,0],[[3,9,108],[9,9,216]],[[9,18],[27,9]]",8));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,8,26,405,26,27,[1,2,24,0],[[3,3,36],[3,9,72],[9,3,72],[9,9,144]],[[9,18],[27,9]]",9));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,14,26,405,26,3,[1,8,18,0],[[9,3,108],[9,9,216]],[[9,18],[27,9]]",10));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,14,26,297,26,27,[1,8,18,0],[[3,9,108],[9,3,108],[9,9,216]],[[9,24],[27,3]]",11));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,14,26,297,26,27,[1,2,24,0],[[3,3,36],[3,9,72],[9,3,72],[9,9,252]],[[9,24],[27,3]]",12));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,2,8,8,189,26,27,[1,8,6,12],[[3,9,108],[9,3,108],[9,9,324]],[[3,18],[9,6],[27,3]]",13));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,1,15,4,2,2,105,14,27,[1,4,16,6],[[3,3,36],[3,9,132],[9,3,132],[9,9,324]],[[1,12],[3,10],[9,4],[27,1]]",14));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,4,8,8,153,26,3,[1,8,18,0],[[3,9,144],[9,3,108],[9,9,324]],[[3,18],[9,8],[27,1]]",15));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,8,26,405,26,27,[1,8,18,0],[[9,9,324]],[[9,18],[27,9]]",16));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,2,2,8,225,26,27,[1,4,10,12],[[3,3,36],[3,9,72],[9,3,72],[9,9,324]],[[3,12],[9,12],[27,3]]",17));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,26,26,297,26,3,[1,8,18,0],[[3,9,108],[9,3,108],[9,9,216]],[[9,24],[27,3]]",18));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,8,26,297,26,3,[1,8,18,0],[[3,9,108],[9,3,108],[9,9,216]],[[9,24],[27,3]]",19));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,4,8,8,153,8,3,[1,8,18,0],[[3,9,108],[9,3,144],[9,9,324]],[[1,18],[9,6],[27,3]]",20));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,1,9,2,8,8,117,8,27,[1,8,6,12],[[3,9,144],[9,3,144],[9,9,324]],[[1,18],[9,8],[27,1]]",21));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,1,9,2,2,8,81,8,27,[1,2,12,12],[[3,3,36],[3,9,144],[9,3,144],[9,9,324]],[[1,18],[3,6],[9,2],[27,1]]",22));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,8,26,405,26,27,[1,8,0,18],[[9,9,324]],[[9,18],[27,9]]",23));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,8,8,189,26,27,[1,8,0,18],[[3,9,108],[9,3,108],[9,9,324]],[[3,18],[9,6],[27,3]]",24));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,1,26,26,729,26,27,[1,26,0,0],[],[[27,27]]",25));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,4,8,8,297,26,3,[1,14,12,0],[[3,3,432]],[[3,18],[27,9]]",26));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,2,8,8,405,26,27,[1,10,16,0],[[3,3,324]],[[9,18],[27,9]]",27));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,4,8,8,297,26,3,[1,14,12,0],[[3,3,432]],[[9,24],[27,3]]",28));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,1,2,26,297,26,27,[1,2,24,0],[[3,3,432]],[[9,24],[27,3]]",29));	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,10,8,8,261,8,1,[1,26,0,0],[[3,3,468]],[[1,18],[27,9]]",30));	
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,1,14,26,297,26,27,[1,14,12,0],[[3,3,432]],[[9,24],[27,3]]",31));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,2,2,8,225,26,27,[1,4,10,12],[[3,3,504]],[[3,12],[9,12],[27,3]]",33));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,1,2,26,297,26,3,[1,2,24,0],[[3,3,432]],[[9,24],[27,3]]",34));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,2,8,8,189,26,27,[1,10,8,8],[[3,3,540]],[[3,18],[9,6],[27,3]]",35));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,10,8,8,189,26,1,[1,26,0,0],[[3,3,540]],[[3,18],[9,6],[27,3]]",36));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,4,8,8,153,8,3,[1,14,12,0],[[3,3,576]],[[1,18],[9,6],[27,3]]",37));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,4,2,2,225,26,27,[1,8,14,4],[[3,3,504]],[[3,12],[9,12],[27,3]]",38));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,1,8,26,297,26,3,[1,8,18,0],[[3,3,432]],[[9,24],[27,3]]",39));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,8,2,2,165,14,3,[1,16,10,0],[[3,3,564]],[[1,12],[3,6],[9,6],[27,3]]",41));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,2,2,2,153,26,27,[1,4,10,12],[[3,3,576]],[[3,24],[27,3]]",42));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,10,8,8,261,26,1,[1,26,0,0],[[3,3,468]],[[9,26],[27,1]]",43));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,4,8,8,153,26,3,[1,14,12,0],[[3,3,576]],[[3,18],[9,8],[27,1]]",44));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,9,2,8,8,117,8,27,[1,10,16,0],[[3,3,612]],[[1,18],[9,8],[27,1]]",45));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,15,4,2,2,105,14,27,[1,8,14,4],[[3,3,624]],[[1,12],[3,10],[9,4],[27,1]]",46));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,8,2,2,165,26,3,[1,16,10,0],[[3,3,564]],[[3,16],[9,10],[27,1]]",47));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,1,14,26,405,26,3,[1,14,12,0],[[3,3,324]],[[9,18],[27,9]]",48));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,1,15,8,2,2,129,14,3,[1,16,10,0],[[3,3,600]],[[1,12],[3,6],[9,8],[27,1]]",49));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,19,8,0,0,125,18,27,[1,14,12,0],[[3,3,604]],[[1,8],[3,12],[9,6],[27,1]]",50));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,11,4,0,0,85,10,27,[1,6,8,12],[[3,3,644]],[[1,16],[3,8],[9,2],[27,1]]",51));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,9,2,2,8,81,8,27,[1,4,10,12],[[3,3,648]],[[1,18],[3,6],[9,2],[27,1]]",52));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,8,26,297,26,3,[1,2,24,0],[[3,3,36],[3,9,72],[9,3,72],[9,9,252]],[[9,24],[27,3]]",53));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,1,0,27,1,2,26,297,26,27,[1,2,24,0],[[3,3,36],[3,9,72],[9,3,72],[9,9,252]],[[9,24],[27,3]]",54));
	m_RingInvariant.insert(make_pair("[1,8,18,0],9,0,0,27,1,2,26,297,26,3,[1,2,24,0],[[3,3,36],[3,9,72],[9,3,72],[9,9,252]],[[9,24],[27,3]]",55));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,1,8,26,405,26,27,[1,8,18,0],[[3,3,324]],[[9,18],[27,9]]",56));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,0,27,1,8,8,189,26,27,[1,8,0,18],[[3,3,540]],[[3,18],[9,6],[27,3]]",57));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,0,0,27,1,26,26,297,26,3,[1,26,0,0],[[3,3,432]],[[9,24],[27,3]]",58));
	m_RingInvariant.insert(make_pair("[1,26,0,0],3,1,1,1,2,0,0,53,0,27,[1,2,0,24],[[3,3,676]],[[1,26],[27,1]]",59));
#endif
	//R81
	m_RingInvariant.insert(make_pair("[1,2,6,18,54],81,1,0,81,1,80,80,6561,80,81,[1,2,6,18,54],[],[[81,81]]",1));//A81
	m_RingInvariant.insert(make_pair("[1,2,6,18,54],81,1,0,81,1,26,80,1701,80,81,[1,2,6,18,54],[[27,81,972],[81,27,972],[81,81,2916]],[[9,54],[27,18],[81,9]]",2));//B81
	m_RingInvariant.insert(make_pair("[1,2,6,18,54],81,1,1,27,2,8,8,297,26,81,[1,2,6,18,54],[[3,81,108],[9,27,108],[9,81,324],[27,9,108],[27,27,324],[27,81,972],[81,3,108],[81,9,324],[81,27,972],[81,81,2916]],[[1,54],[3,18],[9,6],[27,2],[81,1]]",3));//C81
	m_RingInvariant.insert(make_pair("[1,8,18,54,0],27,0,0,81,4,8,26,621,26,9,[1,8,18,54,0],[[3,27,324],[9,9,324],[9,27,972],[27,3,432],[27,9,972],[27,27,2916]],[[1,54],[9,18],[27,6],[81,3]]",10));
	m_RingInvariant.insert(make_pair("[1,8,18,54,0],27,1,0,81,1,26,80,1701,80,81,[1,8,18,0,54],[[3,27,324],[9,27,648],[27,3,324],[27,9,648],[27,27,2916]],[[9,54],[27,18],[81,9]]",11));	
	m_RingInvariant.insert(make_pair("[1,8,18,54,0],27,1,0,81,2,8,26,729,80,81,[1,8,18,18,36],[[3,27,324],[9,9,324],[9,27,972],[27,3,324],[27,9,972],[27,27,2916]],[[3,54],[9,18],[27,6],[81,3]]",12));	
	m_RingInvariant.insert(make_pair("[1,8,18,54,0],27,1,0,81,1,80,80,6561,80,81,[1,8,18,54,0],[],[[81,81]]",13));
	m_RingInvariant.insert(make_pair("[1,8,18,54,0],27,1,0,81,1,26,26,1701,80,81,[1,8,18,54,0],[[9,27,972],[27,9,972],[27,27,2916]],[[9,54],[27,18],[81,9]]",14));
	m_RingInvariant.insert(make_pair("[1,8,18,54,0],27,1,0,81,1,26,80,3645,80,81,[1,8,18,54,0],[[27,27,2916]],[[27,54],[81,27]]",15));
	m_RingInvariant.insert(make_pair("[1,8,18,54,0],27,1,0,81,2,26,26,3645,80,81,[1,4,10,30,36],[[3,3,36],[3,9,72],[3,27,216],[9,3,72],[9,9,144],[9,27,432],[27,3,216],[27,9,432],[27,27,1296]],[[27,54],[81,27]]",16));
	m_RingInvariant.insert(make_pair("[1,8,18,54,0],27,1,0,81,2,8,8,945,80,81,[1,4,10,30,36],[[3,3,36],[3,9,72],[3,27,216],[9,3,72],[9,9,144],[9,27,972],[27,3,216],[27,9,972],[27,27,2916]],[[3,36],[9,30],[27,12],[81,3]]",17));
	m_RingInvariant.insert(make_pair("[1,8,18,54,0],27,1,1,45,4,2,8,405,44,81,[1,4,10,48,18],[[3,3,36],[3,9,72],[3,27,396],[9,3,72],[9,9,324],[9,27,972],[27,3,396],[27,9,972],[27,27,2916]],[[1,36],[3,30],[9,10],[27,4],[81,1]]",18));
	m_RingInvariant.insert(make_pair("[1,8,18,54,0],27,1,0,81,2,8,26,2025,80,81,[1,4,10,30,36],[[3,3,36],[3,9,72],[3,27,216],[9,3,72],[9,9,144],[9,27,432],[27,3,216],[27,9,432],[27,27,2916]],[[9,36],[27,36],[81,9]]",19));
	m_RingInvariant.insert(make_pair("[1,8,18,54,0],27,1,0,81,1,26,80,3645,80,81,[1,2,24,54,0],[[3,3,36],[3,9,72],[3,27,216],[9,3,72],[9,9,144],[9,27,432],[27,3,216],[27,9,432],[27,27,1296]],[[27,54],[81,27]]",20));
	m_RingInvariant.insert(make_pair("[1,8,18,54,0],27,1,0,81,1,8,26,1377,80,81,[1,2,24,54,0],[[3,3,36],[3,9,72],[3,27,216],[9,3,72],[9,9,144],[9,27,756],[27,3,216],[27,9,756],[27,27,2916]],[[9,54],[27,24],[81,3]]",21));
	m_RingInvariant.insert(make_pair("[1,8,18,54,0],27,1,0,81,1,44,80,2673,80,81,[1,2,24,54,0],[[3,3,36],[3,9,72],[3,27,216],[9,3,72],[9,9,144],[9,27,432],[27,3,216],[27,9,432],[27,27,2268]],[[27,72],[81,9]]",22));
	m_RingInvariant.insert(make_pair("[1,8,18,54,0],27,1,0,81,1,8,80,2673,80,81,[1,2,24,54,0],[[3,3,36],[3,9,72],[3,27,216],[9,3,72],[9,9,144],[9,27,432],[27,3,216],[27,9,432],[27,27,2268]],[[27,72],[81,9]]",23));
	m_RingInvariant.insert(make_pair("[1,8,18,54,0],27,1,0,81,1,26,26,1701,80,81,[1,8,18,54,0],[[3,27,324],[9,27,648],[27,3,324],[27,9,648],[27,27,2916]],[[9,54],[27,18],[81,9]]",24));	

	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,1,44,80,2673,80,81,[1,8,36,36,0],[[9,9,3888]],[[27,72],[81,9]]",90));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,26,80,2025,80,9,[1,8,18,54,0],[[9,9,4536]],[[9,36],[27,36],[81,9]]",91));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,1,26,80,1701,80,81,[1,8,18,54,0],[[9,9,4860]],[[9,54],[27,18],[81,9]]",92));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,26,80,1701,80,9,[1,8,18,54,0],[[9,9,4860]],[[9,54],[27,18],[81,9]]",93));	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,54,18,0],[[9,9,3888]],[[9,54],[81,27]]",94));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,26,80,2025,80,9,[1,8,72,0,0],[[9,9,4536]],[[9,36],[27,36],[81,9]]",95));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,54,18,0],[[9,9,3888]],[[27,72],[81,9]]",96));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,8,80,1701,80,9,[1,8,54,18,0],[[9,9,4860]],[[9,54],[27,18],[81,9]]",97));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,44,80,2025,80,9,[1,8,36,36,0],[[9,9,4536]],[[9,36],[27,36],[81,9]]",98));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,8,80,2025,80,9,[1,8,54,18,0],[[9,9,4536]],[[9,36],[27,36],[81,9]]",99));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,1,9,2,8,8,225,8,81,[1,8,6,12,54],[[3,9,576],[9,3,576],[9,9,5184]],[[1,72],[9,8],[81,1]]",100));//[[1,1],[7,6]]=K81	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,1,45,4,8,8,441,44,81,[1,8,18,36,18],[[3,9,504],[9,3,504],[9,9,5112]],[[1,36],[3,24],[9,16],[27,4],[81,1]]",101));//[[5,8],[3,1]]=J81
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,1,27,2,8,8,297,26,81,[1,8,6,30,36],[[3,9,540],[9,3,540],[9,9,5184]],[[1,54],[3,18],[9,6],[27,2],[81,1]]",102));//[[5,6],[1,8]]
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,1,27,2,14,26,405,26,81,[1,8,24,12,36],[[3,9,540],[9,3,540],[9,9,5076]],[[1,54],[9,24],[27,2],[81,1]]",103));//I81
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,10,14,26,945,26,1,[1,8,72,0,0],[[3,9,432],[9,3,432],[9,9,4752]],[[1,54],[9,18],[81,9]]",104));//G81	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,10,14,26,945,80,1,[1,8,72,0,0],[[3,9,432],[9,3,432],[9,9,4752]],[[9,72],[27,8],[81,1]]",105));//H81	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,2,8,26,945,80,81,[1,8,24,12,36],[[3,9,324],[9,3,324],[9,9,4968]],[[3,36],[9,30],[27,12],[81,3]]",106));	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,44,80,3645,80,9,[1,8,36,36,0],[[9,9,2916]],[[27,54],[81,27]]",107));	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,44,80,2025,80,9,[1,8,54,18,0],[[9,9,4536]],[[9,36],[27,36],[81,9]]",108));	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,1,26,80,1701,80,81,[1,8,36,36,0],[[9,9,4860]],[[9,54],[27,18],[81,9]]",109));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,1,80,80,6561,80,81,[1,8,72,0,0],[],[[81,81]]",110));//R9_1×R9_1=R81_110=D81
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,18,54,0],[[9,9,3888]],[[9,54],[81,27]]",111));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,72,0,0],[[9,9,3888]],[[9,54],[81,27]]",112));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,8,80,1701,80,9,[1,8,36,36,0],[[9,9,4860]],[[9,54],[27,18],[81,9]]",113));	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,1,26,80,3645,80,81,[1,8,18,54,0],[[9,9,2916]],[[27,54],[81,27]]",114));	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,8,80,2025,80,9,[1,8,36,36,0],[[9,9,4536]],[[9,36],[27,36],[81,9]]",115));	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,8,80,1377,80,9,[1,8,18,54,0],[[9,9,5184]],[[9,72],[81,9]]",116));	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,8,80,1701,80,9,[1,8,18,54,0],[[9,9,4860]],[[9,54],[27,18],[81,9]]",117));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,44,80,3645,80,9,[1,8,54,18,0],[[9,9,2916]],[[27,54],[81,27]]",118));	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,26,80,2025,80,9,[1,8,36,36,0],[[9,9,4536]],[[9,36],[27,36],[81,9]]",119));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,1,26,80,3645,80,81,[1,8,36,36,0],[[9,9,2916]],[[27,54],[81,27]]",120));//R9_1×R9_3=R81_120
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,36,36,0],[[9,9,3888]],[[9,54],[81,27]]",121));	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,26,80,1701,80,9,[1,8,36,36,0],[[9,9,4860]],[[9,54],[27,18],[81,9]]",122));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,26,80,2025,80,9,[1,8,54,18,0],[[9,9,4536]],[[9,36],[27,36],[81,9]]",123));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,1,8,80,1377,80,81,[1,8,18,54,0],[[9,9,5184]],[[9,72],[81,9]]",124));	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,2,26,26,1701,80,81,[1,8,24,12,36],[[3,9,324],[9,3,324],[9,9,4212]],[[9,54],[27,18],[81,9]]",125));//F81
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,1,8,80,2025,80,81,[1,8,54,18,0],[[9,9,4536]],[[9,36],[27,36],[81,9]]",126));	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,1,26,80,1701,80,81,[1,8,18,0,54],[[3,9,324],[9,3,324],[9,9,4212]],[[9,54],[27,18],[81,9]]",127));//E81	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,1,44,80,2673,80,81,[1,8,54,18,0],[[9,9,3888]],[[27,72],[81,9]]",128));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,1,8,80,2673,80,81,[1,8,18,54,0],[[9,9,3888]],[[27,72],[81,9]]",129));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,1,8,80,2025,80,81,[1,8,18,54,0],[[9,9,4536]],[[9,36],[27,36],[81,9]]",130));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,1,8,80,2025,80,81,[1,8,36,36,0],[[9,9,4536]],[[9,36],[27,36],[81,9]]",131));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,80,80,2673,80,9,[1,8,72,0,0],[[9,9,3888]],[[27,72],[81,9]]",132));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,8,80,2673,80,9,[1,8,18,54,0],[[9,9,3888]],[[27,72],[81,9]]",133));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,8,80,2025,80,9,[1,8,18,54,0],[[9,9,4536]],[[9,36],[27,36],[81,9]]",134));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,1,8,80,1377,80,81,[1,8,36,36,0],[[9,9,5184]],[[9,72],[81,9]]",135));	
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,1,0,81,1,8,80,2025,80,81,[1,8,0,72,0],[[9,9,4536]],[[9,36],[27,36],[81,9]]",136));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,36,36,0],[[9,9,3888]],[[27,72],[81,9]]",137));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,72,0,0],[[9,9,3888]],[[27,72],[81,9]]",138));
	m_RingInvariant.insert(make_pair("[1,8,72,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,18,54,0],[[9,9,3888]],[[27,72],[81,9]]",139));
	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,26,80,1701,80,81,[1,8,36,36,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,2592]],[[9,54],[27,18],[81,9]]",162));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,8,80,2025,80,81,[1,8,18,54,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,2268]],[[9,36],[27,36],[81,9]]",163));		
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,8,80,2673,80,9,[1,8,18,54,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,2268]],[[27,72],[81,9]]",164));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2025,80,9,[1,8,54,18,0],[[3,3,324],[3,9,648],[9,3,972],[9,9,2592]],[[9,36],[27,36],[81,9]]",165));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,44,80,3645,80,9,[1,26,18,36,0],[[3,9,972],[9,9,1944]],[[27,54],[81,27]]",166));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,36,36,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,2268]],[[27,72],[81,9]]",167));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,8,80,2025,80,9,[1,8,54,18,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,2268]],[[9,36],[27,36],[81,9]]",168));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2025,80,9,[1,26,18,36,0],[[3,9,972],[9,3,972],[9,9,2592]],[[9,36],[27,36],[81,9]]",169));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,54,18,0],[[3,3,324],[3,9,648],[9,3,972],[9,9,1944]],[[27,72],[81,9]]",170));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,8,80,1701,80,9,[1,8,54,18,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,2592]],[[9,54],[27,18],[81,9]]",171));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2025,80,9,[1,26,0,54,0],[[3,9,972],[9,3,972],[9,9,2592]],[[9,36],[27,36],[81,9]]",172));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,8,80,1377,80,9,[1,8,36,36,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,2916]],[[9,72],[81,9]]",173));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2025,80,9,[1,8,36,36,0],[[3,3,324],[3,9,648],[9,3,972],[9,9,2592]],[[9,36],[27,36],[81,9]]",174));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2025,80,9,[1,8,36,36,0],[[3,3,324],[3,9,972],[9,3,648],[9,9,2592]],[[9,36],[27,36],[81,9]]",175));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,44,80,2025,80,9,[1,8,54,18,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,2268]],[[9,36],[27,36],[81,9]]",176));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2025,80,9,[1,8,36,36,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,2268]],[[9,36],[27,36],[81,9]]",177));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,1701,80,9,[1,8,36,36,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,2592]],[[9,54],[27,18],[81,9]]",178));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,26,80,1701,80,81,[1,8,54,18,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,2592]],[[9,54],[27,18],[81,9]]",179));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,36,36,0],[[3,3,324],[3,9,648],[9,3,972],[9,9,1944]],[[27,72],[81,9]]",180));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,26,18,36,0],[[3,9,972],[9,9,2916]],[[27,72],[81,9]]",181));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,8,80,2025,80,9,[1,8,54,18,0],[[3,3,324],[3,9,648],[9,3,972],[9,9,2592]],[[9,36],[27,36],[81,9]]",182));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,8,80,2025,80,9,[1,8,36,36,0],[[3,3,324],[3,9,648],[9,3,972],[9,9,2592]],[[9,36],[27,36],[81,9]]",183));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,72,0,0],[[3,3,324],[3,9,648],[9,3,972],[9,9,1944]],[[27,72],[81,9]]",184));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,44,80,2673,80,81,[1,14,66,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,1728]],[[27,72],[81,9]]",185));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,8,80,2673,80,81,[1,2,78,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,1728]],[[27,72],[81,9]]",186));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,72,0,0],[[3,3,324],[3,9,972],[9,3,648],[9,9,1944]],[[27,72],[81,9]]",187));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,26,80,2349,80,81,[1,8,72,0,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,1944]],[[27,78],[81,3]]",188));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,14,66,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,1728]],[[27,72],[81,9]]",189));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,2,78,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,1728]],[[27,72],[81,9]]",190));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,26,80,2349,80,81,[1,2,78,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,2052]],[[27,78],[81,3]]",191));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,26,80,2349,80,81,[1,14,66,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,2052]],[[27,78],[81,3]]",192));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,44,80,2673,80,81,[1,8,54,18,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,2268]],[[27,72],[81,9]]",193));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,8,80,2673,80,81,[1,8,18,54,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,2268]],[[27,72],[81,9]]",194));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,8,80,2025,80,81,[1,8,18,54,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,2916]],[[9,36],[27,36],[81,9]]",195));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,8,80,2025,80,81,[1,8,54,18,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,2916]],[[9,36],[27,36],[81,9]]",196));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2349,80,9,[1,8,72,0,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,1944]],[[27,78],[81,3]]",197));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,44,80,2349,80,9,[1,8,72,0,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,1944]],[[27,78],[81,3]]",198));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,8,80,2349,80,9,[1,8,72,0,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,1944]],[[27,78],[81,3]]",199));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,26,1701,80,9,[1,26,0,54,0],[[3,9,972],[9,3,972],[9,9,2916]],[[9,54],[27,18],[81,9]]",200));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,4,26,26,1377,80,9,[1,26,18,36,0],[[3,9,1296],[9,3,972],[9,9,2916]],[[9,54],[27,24],[81,3]]",201));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,10,26,26,1053,80,3,[1,26,54,0,0],[[3,9,1296],[9,3,1296],[9,9,2916]],[[3,54],[27,24],[81,3]]",202));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,4,26,26,1377,80,9,[1,26,18,36,0],[[3,9,972],[9,3,1296],[9,9,2916]],[[3,54],[27,18],[81,9]]",203));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,1,45,8,8,8,585,44,9,[1,14,36,30,0],[[3,3,432],[3,9,1368],[9,3,1260],[9,9,2916]],[[1,36],[3,12],[9,24],[27,8],[81,1]]",204));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,1,45,8,8,8,585,44,9,[1,14,36,30,0],[[3,3,432],[3,9,1260],[9,3,1368],[9,9,2916]],[[1,36],[3,18],[9,16],[27,10],[81,1]]",205));		
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,8,8,8,693,44,9,[1,14,36,30,0],[[3,3,432],[3,9,1260],[9,3,1260],[9,9,2916]],[[1,36],[3,12],[9,24],[27,6],[81,3]]",206));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,8,8,8,765,44,9,[1,10,52,18,0],[[3,3,324],[3,9,1188],[9,3,1368],[9,9,2916]],[[1,36],[3,18],[9,12],[27,12],[81,3]]",207));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,44,80,2025,80,9,[1,26,18,36,0],[[3,9,972],[9,3,972],[9,9,2592]],[[9,36],[27,36],[81,9]]",208));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,1701,80,9,[1,26,0,54,0],[[3,9,972],[9,3,972],[9,9,2916]],[[9,54],[27,18],[81,9]]",209));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,4,8,26,621,26,9,[1,8,36,36,0],[[3,3,324],[3,9,1296],[9,3,1404],[9,9,2916]],[[1,54],[9,18],[27,6],[81,3]]",210));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,26,80,3645,80,81,[1,8,36,36,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,1296]],[[27,54],[81,27]]",211));//R9_1×R9_5=R81_211
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,44,80,3645,80,9,[1,8,54,18,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,1296]],[[27,54],[81,27]]",212));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,36,36,0],[[3,3,324],[3,9,972],[9,3,648],[9,9,1944]],[[9,54],[81,27]]",213));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2025,80,9,[1,8,54,18,0],[[3,3,324],[3,9,972],[9,3,648],[9,9,2592]],[[9,36],[27,36],[81,9]]",214));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,8,80,1701,80,9,[1,8,36,36,0],[[3,3,324],[3,9,972],[9,3,648],[9,9,2916]],[[9,54],[27,18],[81,9]]",215));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,44,80,3645,80,9,[1,26,18,36,0],[[9,3,972],[9,9,1944]],[[27,54],[81,27]]",216));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,8,80,2025,80,9,[1,8,54,18,0],[[3,3,324],[3,9,972],[9,3,648],[9,9,2592]],[[9,36],[27,36],[81,9]]",217));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,8,80,2025,80,81,[1,8,36,36,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,2916]],[[9,36],[27,36],[81,9]]",218));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,26,18,36,0],[[9,3,972],[9,9,2916]],[[9,54],[81,27]]",219));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,8,80,2025,80,9,[1,8,36,36,0],[[3,3,324],[3,9,972],[9,3,648],[9,9,2592]],[[9,36],[27,36],[81,9]]",220));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,8,80,2025,80,9,[1,8,54,18,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,2916]],[[9,36],[27,36],[81,9]]",221));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,26,0,54,0],[[9,3,972],[9,9,2916]],[[9,54],[81,27]]",222));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,2,14,26,2025,80,9,[1,10,34,36,0],[[3,3,324],[3,9,1188],[9,3,648],[9,9,2376]],[[9,36],[27,36],[81,9]]",223));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,54,18,0],[[3,3,324],[3,9,972],[9,3,648],[9,9,1944]],[[9,54],[81,27]]",224));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,4,8,26,1485,80,9,[1,14,30,36,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,2916]],[[3,36],[9,18],[27,18],[81,9]]",225));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,4,26,26,2673,80,9,[1,14,30,36,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,1728]],[[9,54],[81,27]]",226));//R9_1×R9_7=R81_226
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,1,27,2,26,26,837,26,81,[1,26,6,48,0],[[3,9,1404],[9,3,1404],[9,9,2916]],[[1,54],[27,26],[81,1]]",227));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,80,80,6561,80,81,[1,26,54,0,0],[],[[81,81]]",227));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,2,26,26,3645,80,81,[1,10,34,36,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,1296]],[[27,54],[81,27]]",228));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,4,26,26,2673,80,9,[1,14,30,36,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,1728]],[[27,72],[81,9]]",229));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,2,26,26,1701,80,81,[1,10,26,20,24],[[3,3,540],[3,9,1080],[9,3,1080],[9,9,2160]],[[9,54],[27,18],[81,9]]",230));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,4,8,8,2025,80,81,[1,8,20,40,12],[[3,3,504],[3,9,1008],[9,3,1008],[9,9,2016]],[[9,36],[27,36],[81,9]]",231));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,2,8,8,1377,80,81,[1,4,16,24,36],[[3,3,576],[3,9,1152],[9,3,1152],[9,9,2304]],[[9,72],[81,9]]",232));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,26,80,3645,80,81,[1,26,54,0,0],[[9,9,2916]],[[27,54],[81,27]]",233));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,2,8,26,2025,80,81,[1,10,34,36,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,2916]],[[9,36],[27,36],[81,9]]",234));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,4,8,26,1485,80,9,[1,14,30,36,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,2916]],[[9,48],[27,30],[81,3]]",235));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,2,8,26,945,80,81,[1,10,26,20,24],[[3,3,540],[3,9,1080],[9,3,1080],[9,9,2916]],[[3,36],[9,30],[27,12],[81,3]]",236));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,4,2,8,1125,80,81,[1,8,20,40,12],[[3,3,504],[3,9,1008],[9,3,1008],[9,9,2916]],[[3,24],[9,36],[27,18],[81,3]]",237));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,2,2,8,765,80,81,[1,4,16,24,36],[[3,3,576],[3,9,1152],[9,3,1152],[9,9,2916]],[[3,48],[9,24],[27,6],[81,3]]",238));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,2,26,26,1701,80,81,[1,26,6,48,0],[[3,9,972],[9,3,972],[9,9,2916]],[[9,54],[27,18],[81,9]]",239));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,2,8,26,945,80,81,[1,8,24,12,36],[[3,3,324],[3,9,1188],[9,3,1188],[9,9,2916]],[[3,36],[9,30],[27,12],[81,3]]",240));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,4,8,8,945,80,81,[1,10,28,30,12],[[3,3,324],[3,9,1188],[9,3,1188],[9,9,2916]],[[3,36],[9,30],[27,12],[81,3]]",241));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,8,8,8,693,80,9,[1,14,36,30,0],[[3,3,432],[3,9,1260],[9,3,1260],[9,9,2916]],[[3,48],[9,22],[27,10],[81,1]]",242));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,1,45,4,8,8,441,44,81,[1,10,20,38,12],[[3,3,540],[3,9,1332],[9,3,1332],[9,9,2916]],[[1,36],[3,24],[9,16],[27,4],[81,1]]",243));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,1,57,8,2,2,525,56,81,[1,8,38,34,0],[[3,3,504],[3,9,1308],[9,3,1308],[9,9,2916]],[[1,24],[3,32],[9,18],[27,6],[81,1]]",244));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,1,33,4,2,2,357,32,81,[1,4,22,18,36],[[3,3,576],[3,9,1356],[9,3,1356],[9,9,2916]],[[1,48],[3,16],[9,14],[27,2],[81,1]]",245));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,44,80,3645,80,9,[1,8,72,0,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,1296]],[[27,54],[81,27]]",246));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,44,80,3645,80,9,[1,26,54,0,0],[[3,9,972],[9,9,1944]],[[27,54],[81,27]]",247));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,26,80,3645,80,81,[1,8,72,0,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,1296]],[[27,54],[81,27]]",248));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,44,80,3645,80,9,[1,26,54,0,0],[[9,3,972],[9,9,1944]],[[27,54],[81,27]]",249));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,44,80,2673,80,81,[1,26,54,0,0],[[3,9,972],[9,3,972],[9,9,1944]],[[27,72],[81,9]]",250));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,44,80,2673,80,81,[1,8,72,0,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,2268]],[[27,72],[81,9]]",251));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,80,80,2673,80,9,[1,26,54,0,0],[[3,9,972],[9,3,972],[9,9,1944]],[[27,72],[81,9]]",252));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,26,54,0,0],[[3,9,972],[9,3,972],[9,9,1944]],[[27,72],[81,9]]",253));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,2,26,26,1053,80,81,[1,26,6,24,24],[[3,9,1296],[9,3,1296],[9,9,2916]],[[3,54],[27,24],[81,3]]",254));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,2,8,26,729,80,81,[1,8,24,24,24],[[3,3,324],[3,9,1296],[9,3,1296],[9,9,2916]],[[3,54],[9,18],[27,6],[81,3]]",255));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,26,80,3645,80,81,[1,26,0,54,0],[[9,9,2916]],[[27,54],[81,27]]",256));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,26,26,1701,80,81,[1,26,0,54,0],[[3,9,972],[9,3,972],[9,9,2916]],[[9,54],[27,18],[81,9]]",257));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,72,0,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,2268]],[[27,72],[81,9]]",258));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,8,80,2673,80,81,[1,8,72,0,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,2268]],[[27,72],[81,9]]",259));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,8,80,2673,80,9,[1,8,72,0,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,2268]],[[27,72],[81,9]]",260));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,2,14,26,2025,80,9,[1,4,28,48,0],[[3,3,504],[3,9,1008],[9,3,1008],[9,9,2016]],[[9,36],[27,36],[81,9]]",261));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,2,8,26,2025,80,81,[1,4,28,48,0],[[3,3,504],[3,9,1008],[9,3,1008],[9,9,2016]],[[9,36],[27,36],[81,9]]",262));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,2,14,26,2025,80,9,[1,10,34,36,0],[[3,3,324],[3,9,648],[9,3,1188],[9,9,2376]],[[9,36],[27,36],[81,9]]",263));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,2,14,26,1485,80,81,[1,10,34,36,0],[[3,3,324],[3,9,1188],[9,3,1188],[9,9,2376]],[[9,48],[27,30],[81,3]]",264));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,2,14,26,1485,80,81,[1,4,28,48,0],[[3,3,504],[3,9,1008],[9,3,1008],[9,9,2556]],[[9,48],[27,30],[81,3]]",265));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,8,8,8,765,80,9,[1,10,52,18,0],[[3,3,324],[3,9,1368],[9,3,1188],[9,9,2916]],[[3,36],[9,34],[27,10],[81,1]]",266));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,2,26,26,1485,80,9,[1,10,34,36,0],[[3,3,324],[3,9,1188],[9,3,1188],[9,9,2376]],[[9,48],[27,30],[81,3]]",267));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,2,8,26,1485,80,9,[1,10,34,36,0],[[3,3,324],[3,9,1188],[9,3,1188],[9,9,2376]],[[9,48],[27,30],[81,3]]",268));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,1,45,4,8,8,585,44,81,[1,10,28,30,12],[[3,3,324],[3,9,1368],[9,3,1368],[9,9,2916]],[[1,36],[3,18],[9,16],[27,10],[81,1]]",269));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,1,45,4,2,8,405,44,81,[1,4,22,42,12],[[3,3,504],[3,9,1368],[9,3,1368],[9,9,2916]],[[1,36],[3,30],[9,10],[27,4],[81,1]]",270));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,2,8,26,2025,80,81,[1,10,16,18,36],[[3,3,324],[3,9,648],[9,3,648],[9,9,2916]],[[9,36],[27,36],[81,9]]",271));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,2,8,8,945,80,81,[1,10,16,18,36],[[3,3,324],[3,9,1188],[9,3,1188],[9,9,2916]],[[3,36],[9,30],[27,12],[81,3]]",272));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,2,8,26,1485,80,9,[1,4,28,48,0],[[3,3,504],[3,9,1008],[9,3,1008],[9,9,2556]],[[9,48],[27,30],[81,3]]",273));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,2,2,26,1485,80,81,[1,4,28,48,0],[[3,3,504],[3,9,1008],[9,3,1008],[9,9,2556]],[[9,48],[27,30],[81,3]]",274));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,2,2,26,1485,80,9,[1,4,28,48,0],[[3,3,504],[3,9,1008],[9,3,1008],[9,9,2556]],[[9,48],[27,30],[81,3]]",275));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,8,26,1377,80,81,[1,8,18,54,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,2916]],[[9,54],[27,24],[81,3]]",276));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2349,80,9,[1,14,66,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,2052]],[[27,78],[81,3]]",277));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2349,80,9,[1,2,78,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,2052]],[[27,78],[81,3]]",278));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,44,80,2349,80,9,[1,14,66,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,2052]],[[27,78],[81,3]]",279));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,8,80,2349,80,9,[1,2,78,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,2052]],[[27,78],[81,3]]",280));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2349,80,9,[1,8,72,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,2052]],[[27,78],[81,3]]",281));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,14,66,0,0],[[3,3,324],[3,9,648],[9,3,972],[9,9,1944]],[[27,72],[81,9]]",282));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,8,72,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,1728]],[[27,72],[81,9]]",283));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,14,66,0,0],[[3,3,324],[3,9,972],[9,3,648],[9,9,1944]],[[27,72],[81,9]]",284));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,44,80,2349,80,9,[1,26,54,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,2052]],[[27,78],[81,3]]",285));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2349,80,9,[1,26,54,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,2052]],[[27,78],[81,3]]",286));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2349,80,9,[1,14,66,0,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,1944]],[[27,78],[81,3]]",287));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,44,80,2349,80,9,[1,14,66,0,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,1944]],[[27,78],[81,3]]",288));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2673,80,9,[1,14,66,0,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,2268]],[[27,72],[81,9]]",289));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,44,80,2349,80,9,[1,8,72,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,2052]],[[27,78],[81,3]]",290));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,8,80,2349,80,9,[1,8,72,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,2052]],[[27,78],[81,3]]",291));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,8,80,2025,80,81,[1,8,36,36,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,2268]],[[9,36],[27,36],[81,9]]",292));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,8,26,1377,80,81,[1,8,18,54,0],[[3,3,540],[3,9,1080],[9,3,1080],[9,9,2484]],[[9,54],[27,24],[81,3]]",293));	
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,44,80,3645,80,9,[1,14,66,0,0],[[3,3,324],[3,9,648],[9,3,648],[9,9,1296]],[[27,54],[81,27]]",294));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,8,80,2673,80,9,[1,2,78,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,1728]],[[27,72],[81,9]]",295));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,1,0,81,1,26,26,1701,80,81,[1,8,18,54,0],[[3,3,540],[3,9,1080],[9,3,1080],[9,9,2160]],[[9,54],[27,18],[81,9]]",296));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,80,80,2673,80,9,[1,26,54,0,0],[[3,3,432],[3,9,864],[9,3,864],[9,9,1728]],[[27,72],[81,9]]",297));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,8,80,1377,80,9,[1,8,18,54,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,2916]],[[9,72],[81,9]]",298));
	m_RingInvariant.insert(make_pair("[1,26,54,0,0],9,0,0,81,1,26,80,2025,80,9,[1,8,54,18,0],[[3,3,324],[3,9,972],[9,3,972],[9,9,2268]],[[9,36],[27,36],[81,9]]",299));	
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,1,33,14,8,8,417,32,3,[1,34,46,0,0],[[3,3,6144]],[[1,48],[9,32],[81,1]]",300));//M2(Z/3Z)=R3_2:[[0,0],[0,1]],[[1,1],[1,1]]
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,1,49,8,0,0,425,48,81,[1,14,18,36,12],[[3,3,6136]],[[1,32],[3,32],[9,12],[27,4],[81,1]]",301));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,1,27,2,14,26,405,26,81,[1,16,40,24,0],[[3,3,6156]],[[1,54],[9,24],[27,2],[81,1]]",302));	
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,16,8,8,981,80,1,[1,38,42,0,0],[[3,3,5580]],[[3,48],[9,6],[27,26],[81,1]]",303));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,10,14,26,1377,80,1,[1,32,48,0,0],[[3,3,5184]],[[9,48],[27,32],[81,1]]",304));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,1,45,4,8,8,441,44,81,[1,14,34,24,8],[[3,3,6120]],[[1,36],[3,24],[9,16],[27,4],[81,1]]",305));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,1,29,4,0,0,265,28,81,[1,6,2,24,48],[[3,3,6296]],[[1,52],[3,26],[27,2],[81,1]]",306));	
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,1,27,2,8,8,297,26,81,[1,10,16,18,36],[[3,3,6264]],[[1,54],[3,18],[9,6],[27,2],[81,1]]",307));	
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,1,1,2,0,0,161,0,81,[1,2,6,0,72],[[3,3,6400]],[[1,80],[81,1]]",308));	
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,10,14,26,1377,80,1,[1,32,48,0,0],[[3,3,5184]],[[3,54],[27,18],[81,9]]",309));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,1,45,8,8,8,585,44,9,[1,22,46,12,0],[[3,3,5976]],[[1,36],[3,12],[9,24],[27,8],[81,1]]",310));	
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,1,44,80,2025,80,9,[1,44,36,0,0],[[3,3,4536]],[[9,36],[27,36],[81,9]]",311));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,10,14,26,1377,80,3,[1,32,48,0,0],[[3,3,5184]],[[9,72],[81,9]]",312));	
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,10,14,26,945,26,1,[1,32,48,0,0],[[3,3,5616]],[[1,54],[9,18],[81,9]]",313));	
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,16,8,8,1089,44,1,[1,38,42,0,0],[[3,3,5472]],[[1,36],[9,36],[81,9]]",314));	
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,10,8,8,801,8,1,[1,26,54,0,0],[[3,3,5760]],[[1,72],[81,9]]",315));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,4,26,26,2673,80,9,[1,32,48,0,0],[[3,3,3888]],[[9,54],[81,27]]",316));	
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,1,26,80,2673,80,9,[1,26,54,0,0],[[3,3,3888]],[[9,54],[81,27]]",317));	
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,1,9,2,8,8,225,8,81,[1,10,22,0,48],[[3,3,6336]],[[1,72],[9,8],[81,1]]",318));	
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,1,26,80,1701,80,9,[1,26,54,0,0],[[3,3,4860]],[[9,54],[27,18],[81,9]]",319));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,1,57,16,2,2,645,56,9,[1,32,44,4,0],[[3,3,5916]],[[1,24],[3,24],[9,22],[27,10],[81,1]]",320));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,16,2,2,825,80,9,[1,32,44,4,0],[[3,3,5736]],[[3,32],[9,36],[27,12],[81,1]]",321));	
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,1,80,80,6561,80,81,[1,80,0,0,0],[],[[81,81]]",322));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,1,26,80,3645,80,81,[1,26,54,0,0],[[3,3,2916]],[[27,54],[81,27]]",323));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,2,26,26,3645,80,81,[1,28,52,0,0],[[3,3,2916]],[[27,54],[81,27]]",324));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,4,26,26,2673,80,9,[1,32,48,0,0],[[3,3,3888]],[[27,72],[81,9]]",325));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,2,26,26,1701,80,81,[1,28,20,32,0],[[3,3,4860]],[[9,54],[27,18],[81,9]]",326));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,4,8,8,2025,80,81,[1,14,50,16,0],[[3,3,4536]],[[9,36],[27,36],[81,9]]",327));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,2,8,8,1377,80,81,[1,10,22,48,0],[[3,3,5184]],[[9,72],[81,9]]",328));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,1,8,80,2025,80,81,[1,8,72,0,0],[[3,3,4536]],[[9,36],[27,36],[81,9]]",329));//R9_5×R9_5=R81_329
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,2,8,26,2025,80,81,[1,10,34,36,0],[[3,3,4536]],[[9,36],[27,36],[81,9]]",330));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,4,8,26,1485,80,9,[1,14,30,36,0],[[3,3,5076]],[[3,36],[9,18],[27,18],[81,9]]",331));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,4,8,26,1485,80,9,[1,14,30,36,0],[[3,3,5076]],[[9,48],[27,30],[81,3]]",332));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,2,8,26,945,80,81,[1,10,26,20,24],[[3,3,5616]],[[3,36],[9,30],[27,12],[81,3]]",333));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,4,2,8,1125,80,81,[1,8,20,40,12],[[3,3,5436]],[[3,24],[9,36],[27,18],[81,3]]",334));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,2,2,8,765,80,81,[1,4,16,24,36],[[3,3,5796]],[[3,48],[9,24],[27,6],[81,3]]",335));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,1,17,4,0,0,289,16,81,[1,6,26,24,24],[[3,3,6272]],[[1,64],[9,16],[81,1]]",336));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,8,8,8,1485,80,9,[1,22,46,12,0],[[3,3,5076]],[[3,36],[9,18],[27,18],[81,9]]",337));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,8,8,8,1485,80,9,[1,22,46,12,0],[[3,3,5076]],[[9,48],[27,30],[81,3]]",338));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,4,8,8,945,80,81,[1,14,34,24,8],[[3,3,5616]],[[3,36],[9,30],[27,12],[81,3]]",339));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,8,2,2,1125,80,81,[1,16,40,24,0],[[3,3,5436]],[[3,24],[9,36],[27,18],[81,3]]",340));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,4,2,2,765,80,81,[1,8,20,28,24],[[3,3,5796]],[[3,48],[9,24],[27,6],[81,3]]",341));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,16,8,8,1089,80,1,[1,38,42,0,0],[[3,3,5472]],[[3,48],[9,6],[27,24],[81,3]]",342));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,8,8,8,693,44,9,[1,22,38,20,0],[[3,3,5868]],[[1,36],[3,12],[9,24],[27,6],[81,3]]",343));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,16,2,2,825,56,9,[1,32,44,4,0],[[3,3,5736]],[[1,24],[3,24],[9,18],[27,12],[81,3]]",344));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,8,2,2,561,32,9,[1,16,16,48,0],[[3,3,6000]],[[1,48],[9,30],[81,3]]",345));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,16,8,8,1089,80,1,[1,38,42,0,0],[[3,3,5472]],[[9,64],[27,16],[81,1]]",346));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,8,8,8,693,80,9,[1,22,38,20,0],[[3,3,5868]],[[3,48],[9,22],[27,10],[81,1]]",347));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,8,2,2,561,80,9,[1,16,16,48,0],[[3,3,6000]],[[3,64],[9,8],[27,8],[81,1]]",348));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,1,57,8,2,2,525,56,81,[1,16,40,24,0],[[3,3,6036]],[[1,24],[3,32],[9,18],[27,6],[81,1]]",349));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,1,33,4,2,2,357,32,81,[1,8,20,28,24],[[3,3,6204]],[[1,48],[3,16],[9,14],[27,2],[81,1]]",350));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,1,65,16,0,0,625,64,81,[1,30,50,0,0],[[3,3,5936]],[[1,16],[3,32],[9,24],[27,8],[81,1]]",351));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,1,8,80,1701,80,9,[1,8,72,0,0],[[3,3,4860]],[[9,54],[27,18],[81,9]]",352));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,1,8,80,2673,80,81,[1,8,72,0,0],[[3,3,3888]],[[27,72],[81,9]]",353));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,10,26,26,2349,80,3,[1,44,36,0,0],[[3,3,4212]],[[3,54],[81,27]]",354));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,1,44,80,2673,80,81,[1,44,36,0,0],[[3,3,3888]],[[27,72],[81,9]]",355));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,10,26,26,1701,80,3,[1,44,36,0,0],[[3,3,4860]],[[9,54],[27,18],[81,9]]",356));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,4,26,26,1377,80,9,[1,32,24,24,0],[[3,3,5184]],[[3,54],[27,18],[81,9]]",357));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,1,26,80,2673,80,9,[1,26,54,0,0],[[3,3,3888]],[[27,72],[81,9]]",358));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,10,26,26,2349,80,3,[1,44,36,0,0],[[3,3,4212]],[[27,78],[81,3]]",359));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,4,26,26,1377,80,9,[1,32,24,24,0],[[3,3,5184]],[[9,54],[27,24],[81,3]]",360));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,2,26,26,1053,80,81,[1,28,20,32,0],[[3,3,5508]],[[3,54],[27,24],[81,3]]",361));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,1,44,80,3645,80,9,[1,44,36,0,0],[[3,3,2916]],[[27,54],[81,27]]",362));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,8,8,8,1161,80,9,[1,22,38,20,0],[[3,3,5400]],[[3,36],[9,18],[27,24],[81,3]]",363));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,2,8,26,729,80,81,[1,10,26,20,24],[[3,3,5832]],[[3,54],[9,18],[27,6],[81,3]]",364));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,1,26,26,1701,80,81,[1,26,0,54,0],[[3,3,4860]],[[9,54],[27,18],[81,9]]",365));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,1,80,80,2673,80,9,[1,80,0,0,0],[[3,3,3888]],[[27,72],[81,9]]",366));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,2,2,2,477,80,81,[1,4,4,24,48],[[3,3,6084]],[[3,78],[81,3]]",367));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,2,2,26,1485,80,81,[1,4,28,48,0],[[3,3,5076]],[[9,48],[27,30],[81,3]]",368));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,20,8,8,1305,44,3,[1,46,34,0,0],[[3,3,5256]],[[1,36],[3,18],[27,18],[81,9]]",369));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,2,14,26,1485,80,81,[1,16,40,24,0],[[3,3,5076]],[[9,48],[27,30],[81,3]]",370));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,20,8,8,945,80,3,[1,46,34,0,0],[[3,3,5616]],[[3,36],[9,30],[27,12],[81,3]]",371));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,8,8,8,765,44,9,[1,22,46,12,0],[[3,3,5796]],[[1,36],[3,18],[9,12],[27,12],[81,3]]",372));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,2,8,26,1485,80,9,[1,10,34,36,0],[[3,3,5076]],[[9,48],[27,30],[81,3]]",373));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,20,8,8,1305,80,3,[1,46,34,0,0],[[3,3,5256]],[[9,52],[27,28],[81,1]]",374));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,8,8,8,765,80,9,[1,22,46,12,0],[[3,3,5796]],[[3,36],[9,34],[27,10],[81,1]]",375));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,1,45,4,8,8,585,44,81,[1,14,50,16,0],[[3,3,5976]],[[1,36],[3,18],[9,16],[27,10],[81,1]]",376));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,2,14,26,2025,80,9,[1,16,40,24,0],[[3,3,4536]],[[9,36],[27,36],[81,9]]",377));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,1,45,4,2,8,405,44,81,[1,8,20,40,12],[[3,3,6156]],[[1,36],[3,30],[9,10],[27,4],[81,1]]",378));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,2,8,8,945,80,81,[1,10,16,18,36],[[3,3,5616]],[[3,36],[9,30],[27,12],[81,3]]",379));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,2,26,26,1485,80,9,[1,28,52,0,0],[[3,3,5076]],[[9,48],[27,30],[81,3]]",380));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,1,8,80,2025,80,9,[1,8,72,0,0],[[3,3,4536]],[[9,36],[27,36],[81,9]]",381));	
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,1,8,80,1377,80,9,[1,8,72,0,0],[[3,3,5184]],[[9,72],[81,9]]",382));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,1,8,80,1377,80,81,[1,8,72,0,0],[[3,3,5184]],[[9,72],[81,9]]",383));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,16,8,8,981,44,1,[1,38,42,0,0],[[3,3,5580]],[[1,36],[3,18],[27,24],[81,3]]",384));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,1,8,80,2673,80,9,[1,8,72,0,0],[[3,3,3888]],[[27,72],[81,9]]",385));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,1,26,80,2025,80,9,[1,26,54,0,0],[[3,3,4536]],[[9,36],[27,36],[81,9]]",386));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,1,26,80,1701,80,81,[1,26,54,0,0],[[3,3,4860]],[[9,54],[27,18],[81,9]]",387));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,2,2,26,1485,80,9,[1,4,28,48,0],[[3,3,5076]],[[9,48],[27,30],[81,3]]",388));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,1,8,26,1377,80,81,[1,8,18,54,0],[[3,3,5184]],[[9,54],[27,24],[81,3]]",389));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,1,26,80,2349,80,9,[1,26,54,0,0],[[3,3,4212]],[[27,78],[81,3]]",390));	
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,1,44,80,2349,80,9,[1,44,36,0,0],[[3,3,4212]],[[27,78],[81,3]]",391));
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,0,0,81,1,8,80,2349,80,9,[1,8,72,0,0],[[3,3,4212]],[[27,78],[81,3]]",392));	
	m_RingInvariant.insert(make_pair("[1,80,0,0,0],3,1,0,81,1,26,80,2349,80,81,[1,26,54,0,0],[[3,3,4212]],[[27,78],[81,3]]",393));
#ifndef R32_DEL
	//R32
	m_RingInvariant.insert(make_pair("[1,1,2,4,8,16],32,1,0,32,1,31,31,1024,31,32,[1,1,2,4,8,16],[],[[32,32]]",1));//[[0,0],[1,0]]
	m_RingInvariant.insert(make_pair("[1,1,2,4,8,16],32,1,1,16,2,3,7,112,15,32,[1,1,2,4,8,16],[[2,32,16],[4,16,16],[4,32,32],[8,8,16],[8,16,32],[8,32,64],[16,4,16],[16,8,32],[16,16,64],[16,32,128],[32,2,16],[32,4,32],[32,8,64],[32,16,128],[32,32,256]],[[1,16],[2,8],[4,4],[8,2],[16,1],[32,1]]",2));//[[0,0],[0,1]]	
	m_RingInvariant.insert(make_pair("[1,1,2,4,8,16],32,1,0,32,1,7,15,192,31,32,[1,1,2,4,8,16],[[4,32,32],[8,16,32],[8,32,64],[16,8,32],[16,16,64],[16,32,128],[32,4,32],[32,8,64],[32,16,128],[32,32,256]],[[2,16],[4,8],[8,4],[16,2],[32,2]]",3));//[[0,0],[1,2]]
	m_RingInvariant.insert(make_pair("[1,1,2,4,8,16],32,1,0,32,1,7,15,320,31,32,[1,1,2,4,8,16],[[8,32,64],[16,16,64],[16,32,128],[32,8,64],[32,16,128],[32,32,256]],[[4,16],[8,8],[16,4],[32,4]]",4));//[[0,0],[1,4]]
	m_RingInvariant.insert(make_pair("[1,1,2,4,8,16],32,1,0,32,1,15,31,512,31,32,[1,1,2,4,8,16],[[16,32,128],[32,16,128],[32,32,256]],[[8,16],[16,8],[32,8]]",5));//[[0,0],[1,8]]
	m_RingInvariant.insert(make_pair("[1,1,2,4,8,16],32,1,0,32,1,15,31,768,31,32,[1,1,2,4,8,16],[[32,32,256]],[[16,16],[32,16]]",6));//[[0,0],[1,16]]	
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,1,16,2,7,7,160,15,32,[1,3,4,8,8,8],[[2,16,48],[4,8,32],[4,16,64],[8,4,32],[8,8,64],[8,16,128],[16,2,48],[16,4,64],[16,8,128],[16,16,256]],[[1,16],[4,8],[8,4],[16,3],[32,1]]",7));//[[1,8],[0,1]]	
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,1,31,31,1024,31,32,[1,3,4,8,16,0],[],[[32,32]]",8));//R16_1×R2_1=R32_8=[[0,4],[0,0]],[[32,8],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,0,0,32,1,23,31,768,31,8,[1,3,4,8,16,0],[[2,16,32],[4,16,32],[8,16,64],[16,16,128]],[[16,16],[32,16]]",9));//[[0,1],[0,0]],[[8,0],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,0,0,32,1,15,31,512,31,8,[1,3,4,8,16,0],[[2,16,32],[4,16,32],[8,16,64],[16,8,128],[16,16,256]],[[8,16],[16,8],[32,8]]",10));//[[0,2],[0,0]],[[4,1],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,0,0,32,1,15,31,512,31,8,[1,3,4,8,16,0],[[8,16,128],[16,2,32],[16,4,32],[16,8,64],[16,16,256]],[[8,16],[16,8],[32,8]]",11));//[[4,0],[1,0]],[[8,0],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,0,0,32,1,23,31,768,31,8,[1,3,4,8,16,0],[[16,2,32],[16,4,32],[16,8,64],[16,16,128]],[[16,16],[32,16]]",12));//[[8,0],[0,0]],[[8,0],[1,0]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,1,15,31,640,31,32,[1,3,4,8,16,0],[[2,16,32],[4,16,32],[8,16,64],[16,2,32],[16,4,32],[16,8,64],[16,16,128]]",13));//[[4,1],[0,4]],[[8,0],[0,8]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,1,31,31,640,31,32,[1,3,4,8,16,0],[[2,16,32],[4,16,32],[8,16,64],[16,2,32],[16,4,32],[16,8,64],[16,16,128]]",14));//[[8,0],[0,8]],[[8,1],[0,8]]	
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,2,7,7,192,31,32,[1,3,4,8,8,8],[[2,16,32],[4,8,32],[4,16,64],[8,4,32],[8,8,64],[8,16,128],[16,2,32],[16,4,64],[16,8,128],[16,16,256]],[[2,16],[4,8],[8,4],[16,2],[32,2]]",15));//R16_3×R2_1=R32_15=[[8,0],[0,3]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,0,0,32,3,7,7,176,31,8,[1,3,4,8,16,0],[[2,16,48],[4,8,32],[4,16,64],[8,4,32],[8,8,64],[8,16,128],[16,2,32],[16,4,64],[16,8,128],[16,16,256]],[[2,16],[4,8],[8,4],[16,3],[32,1]]",16));//[[0,0],[0,1]],[[0,8],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,1,15,31,512,31,32,[1,3,4,8,16,0],[[8,16,128],[16,8,128],[16,16,256]],[[8,16],[16,8],[32,8]]",17));//R16_4×R2_1=R32_17=[[4,0],[0,0]],[[32,32],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,0,0,32,3,7,7,176,15,8,[1,3,4,8,16,0],[[2,16,32],[4,8,32],[4,16,64],[8,4,32],[8,8,64],[8,16,128],[16,2,48],[16,4,64],[16,8,128],[16,16,256]],[[1,16],[4,8],[8,4],[16,2],[32,2]]",18));//[[1,4],[0,0]],[[0,8],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,0,0,32,1,7,15,320,31,8,[1,3,4,8,16,0],[[2,16,32],[4,16,32],[8,8,64],[8,16,128],[16,4,64],[16,8,128],[16,16,256]],[[4,16],[8,8],[16,4],[32,4]]",19));//[[2,1],[0,8]],[[4,6],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,1,7,15,320,31,32,[1,3,4,8,0,16],[[2,16,32],[4,16,32],[8,8,64],[8,16,128],[16,2,32],[16,4,32],[16,8,128],[16,16,256]]",20));//[[4,10],[0,0]],[[6,1],[0,12]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,1,7,15,320,31,32,[1,3,4,8,16,0],[[2,16,32],[4,16,32],[8,8,64],[8,16,128],[16,2,32],[16,4,32],[16,8,128],[16,16,256]]",21));//[[2,1],[0,8]],[[8,0],[0,8]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,0,0,32,1,7,15,320,31,8,[1,3,4,8,16,0],[[4,16,64],[8,8,64],[8,16,128],[16,2,32],[16,4,32],[16,8,128],[16,16,256]]",22));//[[2,1],[0,8]],[[12,6],[0,8]]
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,1,15,15,512,31,32,[1,3,4,8,0,16],[[2,16,32],[4,16,32],[8,16,64],[16,2,32],[16,4,32],[16,8,64],[16,16,256]],[[8,16],[16,8],[32,8]]",23));//[[4,12],[4,12]],[[5,9],[7,11]]	
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,2,15,15,768,31,32,[1,2,3,6,12,8],[[2,2,4],[2,4,4],[2,8,8],[2,16,16],[4,2,4],[4,4,4],[4,8,8],[4,16,16],[8,2,8],[8,4,8],[8,8,16],[8,16,32],[16,2,16],[16,4,16],[16,8,32],[16,16,64]]",24));//R16_1×R2_2
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,1,7,15,320,31,32,[1,3,4,8,16,0],[[4,16,64],[8,8,64],[8,16,128],[16,4,64],[16,8,128],[16,16,256]],[[4,16],[8,8],[16,4],[32,4]]",25));//R16_2×R2_1
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,2,3,7,240,31,32,[1,2,3,6,12,8],[[2,2,4],[2,4,4],[2,8,8],[2,16,16],[4,2,4],[4,4,4],[4,8,8],[4,16,64],[8,2,8],[8,4,8],[8,8,64],[8,16,128],[16,2,16],[16,4,64],[16,8,128],[16,16,256]],[[2,8],[4,12],[8,6],[16,4],[32,2]]",26));//R16_2×R2_2
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,1,7,15,320,31,32,[1,3,4,8,0,16],[[4,16,64],[8,8,64],[8,16,128],[16,4,64],[16,8,128],[16,16,256]],[[4,16],[8,8],[16,4],[32,4]]",27));
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,1,24,4,3,3,144,23,32,[1,2,3,6,20,0],[[2,2,4],[2,4,4],[2,8,8],[2,16,40],[4,2,4],[4,4,4],[4,8,32],[4,16,64],[8,2,8],[8,4,32],[8,8,64],[8,16,128],[16,2,40],[16,4,64],[16,8,128],[16,16,256]],[[1,8],[2,12],[4,6],[8,3],[16,2],[32,1]]",28));//R16_3×R2_2
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,2,7,15,384,31,32,[1,2,3,6,12,8],[[2,2,4],[2,4,4],[2,8,8],[2,16,16],[4,2,4],[4,4,4],[4,8,8],[4,16,16],[8,2,8],[8,4,8],[8,8,16],[8,16,128],[16,2,16],[16,4,16],[16,8,128],[16,16,256]]",29));//R16_4×R2_2
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,1,15,31,768,31,32,[1,3,4,8,16,0],[[16,16,256]],[[16,16],[32,16]]",30));//R16_5×R2_1
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,2,7,15,576,31,32,[1,2,3,6,12,8],[[2,2,4],[2,4,4],[2,8,8],[2,16,16],[4,2,4],[4,4,4],[4,8,8],[4,16,16],[8,2,8],[8,4,8],[8,8,16],[8,16,32],[16,2,16],[16,4,16],[16,8,32],[16,16,256]],[[8,8],[16,16],[32,8]]",31));//R16_5×R2_2
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,1,15,31,512,31,32,[1,3,4,8,16,0],[[2,16,32],[4,16,32],[8,16,64],[16,2,32],[16,4,32],[16,8,64],[16,16,256]],[[8,16],[16,8],[32,8]]",32));//[[0,0],[6,8]],[[4,0],[1,0]]	
	m_RingInvariant.insert(make_pair("[1,3,4,8,16,0],16,1,0,32,1,7,15,288,31,32,[1,1,6,8,16,0],[[2,2,4],[2,4,4],[2,8,8],[2,16,16],[4,2,4],[4,4,4],[4,8,8],[4,16,48],[8,2,8],[8,4,8],[8,8,48],[8,16,128],[16,2,16],[16,4,48],[16,8,128],[16,16,256]],[[4,16],[8,8],[16,6],[32,2]]",33));//R32_25×R4_2的商环
	/*---------------注意R8R4_12=R8_12×R4_1，但此处R8_2×R4_1也写成了R8R4_12(1*11+1=12,51*11+11=572,...)的形式----------------*/
	/*---------------begin of R32_1000~R32_1011----------------*/
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,31,31,1024,31,32,[1,3,12,16,0,0],[],[[32,32]]",1000));//R8R4_1=R8_1×R4_1
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,15,512,31,32,[1,3,12,8,8,0],[[4,8,128],[8,4,128],[8,8,256]],[[8,16],[16,8],[32,8]]",1003));//R8R4_12=R8_2×R4_1
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,2,7,15,320,31,32,[1,3,8,8,4,8],[[2,8,32],[4,4,64],[4,8,160],[8,2,32],[8,4,160],[8,8,256]],[[4,16],[8,8],[16,4],[32,4]]",1006));//R8R4_23=R8_3×R4_1
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,768,31,32,[1,3,12,16,0,0],[[8,8,256]],[[16,16],[32,16]]",1009));//R8R4_34=R8_4×R4_1
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,768,31,32,[1,3,8,12,8,0],[[4,4,64],[4,8,64],[8,4,64],[8,8,64]],[[16,16],[32,16]]",1001));//R8R4_2=R8_1×R4_2
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,15,384,31,32,[1,3,8,20,0,0],[[4,4,64],[4,8,160],[8,4,160],[8,8,256]],[[4,8],[8,12],[16,8],[32,4]]",1004));//R8R4_13=R8_2×R4_2
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,2,3,15,240,31,32,[1,3,12,4,4,8],[[2,8,32],[4,4,112],[4,8,176],[8,2,32],[8,4,176],[8,8,256]],[[2,8],[4,12],[8,6],[16,4],[32,2]]",1007));//R8R4_24=R8_3×R4_2=R8_3:[[3,0],[4,6]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,31,576,31,32,[1,3,8,12,8,0],[[4,4,64],[4,8,64],[8,4,64],[8,8,256]],[[8,8],[16,16],[32,8]]",1010));//R8R4_35=R8_4×R4_2
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,2,15,15,512,31,32,[1,3,6,10,4,8],[[2,4,16],[2,8,16],[4,2,16],[4,4,96],[4,8,112],[8,2,16],[8,4,112],[8,8,128]],[[8,16],[16,8],[32,8]]",1002));//R8R4_3=R8_1×R4_3
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,2,7,7,256,31,32,[1,3,6,10,4,8],[[2,4,16],[2,8,16],[4,2,16],[4,4,96],[4,8,176],[8,2,16],[8,4,176],[8,8,256]],[[2,8],[4,8],[8,10],[16,4],[32,2]]",1005));//R8R4_14=R8_2×R4_3
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,1,24,4,3,7,160,23,32,[1,3,6,10,12,0],[[2,4,16],[2,8,40],[4,2,16],[4,4,128],[4,8,184],[8,2,40],[8,4,184],[8,8,256]],[[1,8],[2,8],[4,8],[8,5],[16,2],[32,1]]",1008));//R8R4_25=R8_3×R4_3
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,2,7,15,384,31,32,[1,3,6,10,4,8],[[2,4,16],[2,8,16],[4,2,16],[4,4,96],[4,8,112],[8,2,16],[8,4,112],[8,8,256]],[[4,8],[8,12],[16,8],[32,4]]",1011));//R8R4_36=R8_4×R4_3
	/*---------------end of R32_1000~R32_1011----------------*/	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,1,16,2,3,15,144,15,32,[1,3,4,12,4,8],[[2,8,48],[4,4,144],[4,8,192],[8,2,48],[8,4,192],[8,8,256]],[[1,16],[4,12],[16,3],[32,1]]",1012));//R8_3/([[1,2]],[1,1,1])=R8_3:[[3,2],[6,5]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,1,16,2,3,15,176,15,32,[1,3,12,4,4,8],[[2,8,48],[4,4,112],[4,8,192],[8,2,48],[8,4,192],[8,8,256]],[[1,16],[4,4],[8,8],[16,3],[32,1]]",1013));//R8_3/([[1,2]],[0,1,1])=R8_3:[[1,4],[4,7]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,1,16,2,7,15,160,15,32,[1,3,8,8,4,8],[[2,8,48],[4,4,128],[4,8,192],[8,2,48],[8,4,192],[8,8,256]],[[1,16],[4,8],[8,4],[16,3],[32,1]]",1014));//R8_3/([[1,2]],[0,0,1])=R8_3/([[1,2]],[1,0,1])=R32_1014=R8_3:[[1,6],[0,5]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,15,320,31,32,[1,3,8,20,0,0],[[2,8,32],[4,4,128],[4,8,160],[8,2,32],[8,4,160],[8,8,192]],[[4,8],[8,16],[16,6],[32,2]]",1015));//[[2,0],[0,2]],[[1,1],[1,1]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,15,192,31,32,[1,3,8,4,0,16],[[2,8,32],[4,4,128],[4,8,192],[8,2,32],[8,4,192],[8,8,256]],[[2,16],[4,8],[8,4],[16,2],[32,2]]",1016));//[[4,2],[5,6]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,5,7,15,240,15,2,[1,3,12,16,0,0],[[2,8,32],[4,4,96],[4,8,160],[8,2,48],[8,4,192],[8,8,256]],[[1,16],[4,8],[16,4],[32,4]]",1017));//[[1,0],[0,0]],[[2,2],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,11,23,448,31,2,[1,3,12,16,0,0],[[2,8,32],[4,4,96],[4,8,160],[8,4,96],[8,8,192]],[[4,16],[16,8],[32,8]]",1018));//[[2,0],[0,0]],[[2,1],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,5,7,15,240,31,2,[1,3,12,16,0,0],[[2,8,48],[4,4,96],[4,8,192],[8,2,32],[8,4,160],[8,8,256]],[[4,16],[8,12],[16,3],[32,1]]",1019));//[[1,0],[0,0]],[[2,0],[2,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,11,23,448,31,2,[1,3,12,16,0,0],[[4,4,96],[4,8,96],[8,2,32],[8,4,160],[8,8,192]],[[8,16],[16,12],[32,4]]",1020));//[[2,0],[0,0]],[[2,0],[1,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,23,288,31,32,[1,3,8,12,8,0],[[2,8,32],[4,4,128],[4,8,160],[8,2,32],[8,4,160],[8,8,224]],[[4,16],[8,8],[16,6],[32,2]]",1021));//[[2,0],[0,2]],[[2,1],[0,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,11,23,384,31,2,[1,3,12,16,0,0],[[2,8,32],[4,4,96],[4,8,160],[8,4,160],[8,8,192]],[[4,16],[16,12],[32,4]]",1022));//[[0,0],[3,0]],[[4,0],[2,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,23,288,31,32,[1,3,8,12,8,0],[[2,8,32],[4,4,128],[4,8,160],[8,2,32],[8,4,160],[8,8,224]],[[4,16],[8,8],[16,6],[32,2]]",1023));//[[2,0],[6,2]],[[6,0],[5,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,15,384,31,32,[1,3,12,8,8,0],[[4,8,192],[8,4,192],[8,8,256]],[[4,16],[16,12],[32,4]]",1024));//[[2,0],[0,6]],[[8,0],[8,12]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,15,384,31,32,[1,3,12,0,0,16],[[4,8,192],[8,4,192],[8,8,256]],[[4,16],[16,12],[32,4]]",1025));//[[2,2],[0,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,23,31,768,31,8,[1,3,12,16,0,0],[[4,8,128],[8,8,128]],[[16,16],[32,16]]",1026));//[[4,0],[0,0]],[[0,2],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,23,31,768,31,8,[1,3,12,16,0,0],[[8,4,128],[8,8,128]],[[16,16],[32,16]]",1027));//[[4,0],[0,0]],[[8,0],[2,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,31,31,640,31,32,[1,3,12,16,0,0],[[4,8,128],[8,4,128],[8,8,128]],[[16,24],[32,8]]",1028));//[[4,0],[0,4]],[[8,2],[0,8]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,15,448,31,8,[1,3,12,16,0,0],[[4,8,192],[8,4,128],[8,8,256]]",1029));//[[0,4],[0,0]],[[0,0],[0,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,15,448,31,8,[1,3,12,16,0,0],[[4,8,128],[8,4,192],[8,8,256]],[[4,16],[16,8],[32,8]]",1030));//[[0,4],[0,0]],[[2,8],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,512,31,32,[1,3,12,0,0,16],[[4,8,128],[8,4,128],[8,8,256]],[[8,16],[16,8],[32,8]]",1031));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,640,31,32,[1,3,12,16,0,0],[[4,8,128],[8,4,128],[8,8,128]],[[16,24],[32,8]]",1032));//[[4,10],[0,0]],[[4,0],[0,4]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,15,384,31,8,[1,3,4,24,0,0],[[4,4,64],[4,8,128],[8,4,192],[8,8,256]],[[4,16],[16,12],[32,4]]",1033));//R16_30:[[0,0],[0,1]],[[0,0],[2,8]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,15,384,31,8,[1,3,4,24,0,0],[[4,4,64],[4,8,192],[8,4,128],[8,8,256]],[[8,24],[16,4],[32,4]]",1034));//R16_30:[[0,0],[0,1]],[[0,2],[0,8]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,12,0,16,0],[[4,8,128],[8,8,256]],[[16,24],[32,8]]",1035));//R16_30:[[0,0],[0,2]],[[0,0],[1,8]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,15,448,31,8,[1,3,12,0,16,0],[[4,8,128],[8,4,192],[8,8,256]],[[4,16],[16,8],[32,8]]",1036));//R16_30:[[0,0],[0,2]],[[0,0],[9,1]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,12,0,16,0],[[8,4,128],[8,8,256]],[[8,16],[32,16]]",1037));//R16_30:[[0,0],[0,2]],[[0,1],[0,8]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,23,31,768,31,8,[1,3,12,8,8,0],[[8,4,128],[8,8,128]],[[16,16],[32,16]]",1038));//R16_31:[[0,0],[3,0]],[[0,0],[3,5]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,576,31,8,[1,3,8,20,0,0],[[4,4,64],[4,8,128],[8,4,64],[8,8,192]],[[8,8],[16,16],[32,8]]",1039));//R16_31:[[0,0],[3,1]],[[0,0],[3,4]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,8,12,8,0],[[4,4,64],[4,8,128],[8,4,64],[8,8,128]],[[8,16],[32,16]]",1040));//R16_31:[[0,0],[3,1]],[[0,0],[4,1]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,8,20,0,0],[[4,4,64],[4,8,128],[8,4,64],[8,8,128]],[[8,16],[32,16]]",1041));//R16_29:[[0,0],[3,3]],[[0,0],[3,4]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,4,16,8,0],[[4,4,64],[4,8,128],[8,4,64],[8,8,128]],[[8,16],[32,16]]",1042));//R16_29:[[0,0],[3,4]],[[0,0],[8,1]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,12,8,8,0],[[8,4,128],[8,8,256]],[[8,16],[32,16]]",1043));//R16_26:[[0,0],[3,12]],[[0,0],[9,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,576,31,8,[1,3,12,8,8,0],[[4,8,128],[8,4,128],[8,8,192]]",1044));//R16_26:[[0,0],[3,12]],[[0,0],[9,1]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,12,8,8,0],[[4,8,128],[8,8,256]],[[16,24],[32,8]]",1045));//R16_26:[[0,0],[3,12]],[[0,2],[0,0]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,8,12,8,0],[[4,4,64],[4,8,64],[8,4,128],[8,8,128]],[[16,24],[32,8]]",1046));//R16_31:	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,576,31,8,[1,3,8,20,0,0],[[4,4,64],[4,8,64],[8,4,128],[8,8,192]],[[8,8],[16,16],[32,8]]",1047));//R16_31:[[0,0],[0,5]],[[0,1],[0,1]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,23,31,768,31,8,[1,3,8,20,0,0],[[4,4,64],[4,8,64],[8,4,64],[8,8,64]],[[16,16],[32,16]]",1048));//R16_28:[[0,2],[4,4]],[[0,2],[4,6]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,640,31,32,[1,3,8,12,8,0],[[4,4,64],[4,8,64],[8,4,64],[8,8,192]]",1049));//R16_28:
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,23,31,768,31,8,[1,3,4,24,0,0],[[4,4,64],[4,8,64],[8,4,64],[8,8,64]],[[16,16],[32,16]]",1050));//R16_28:[[0,2],[2,6]],[[0,2],[7,1]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,8,20,0,0],[[4,4,64],[4,8,64],[8,4,64],[8,8,256]],[[8,8],[16,16],[32,8]]",1051));//R16_33:[[0,0],[0,1]],[[0,0],[3,3]]或[[0,0,0],[0,0,0],[0,2,1]],[[0,0,0],[0,0,0],[0,3,3]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,640,31,8,[1,3,4,24,0,0],[[4,4,64],[4,8,64],[8,4,64],[8,8,192]],[[16,24],[32,8]]",1052));//R16_33:[[0,0],[0,3]],[[0,0],[7,1]]或[[0,0,0],[0,0,0],[0,3,3]],[[0,0,0],[0,0,0],[0,4,9]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,8,12,8,0],[[4,4,64],[4,8,128],[8,4,64],[8,8,192]],[[8,8],[16,16],[32,8]]",1053));//R16_33:[[0,0],[0,1]],[[0,0],[5,3]]或[[0,0,0],[0,0,0],[0,2,1]],[[0,0,0],[0,0,0],[0,5,3]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,768,31,32,[1,3,12,0,16,0],[[8,8,256]],[[16,16],[32,16]]",1054));//R16_31:[[0,0],[0,10]],[[0,0],[4,1]]或[[0,0,0],[0,0,0],[0,2,8]],[[0,0,0],[0,0,0],[0,4,1]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,8,12,8,0],[[4,4,64],[4,8,64],[8,4,128],[8,8,192]],[[8,8],[16,16],[32,8]]",1055));//R16_33:[[0,0],[0,1]],[[0,3],[0,3]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,15,512,31,8,[1,3,12,8,8,0],[[4,8,128],[8,4,128],[8,8,256]],[[8,16],[16,8],[32,8]]",1056));//R16_25:[[0,0],[0,1]],[[0,0],[2,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,640,31,32,[1,3,4,24,0,0],[[4,4,64],[4,8,64],[8,4,64],[8,8,192]],[[16,24],[32,8]]",1057));//R16_31:[[0,0],[0,5]],[[0,0],[8,1]]或[[0,0,0],[0,0,0],[0,3,9]],[[0,0,0],[0,0,0],[0,4,4]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,4,16,8,0],[[4,4,64],[4,8,128],[8,4,64],[8,8,256]],[[8,16],[16,8],[32,8]]",1058));//R16_31:[[0,0],[1,4]],[[0,0],[8,1]]或[[0,0,0],[0,0,0],[0,3,9]],[[0,0,0],[0,0,0],[0,4,5]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,768,31,32,[1,3,4,24,0,0],[[4,4,64],[4,8,64],[8,4,64],[8,8,64]],[[16,16],[32,16]]",1059));//R16_29:[[0,0,0],[0,0,0],[0,6,4]],[[0,0,0],[0,0,0],[0,7,13]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,15,320,31,32,[1,3,4,16,8,0],[[4,4,64],[4,8,192],[8,4,192],[8,8,256]],[[4,16],[8,8],[16,4],[32,4]]",1060));//R16_30:[[0,0,0],[0,0,0],[0,6,8]],[[0,0,0],[0,0,0],[0,8,1]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,23,31,768,31,8,[1,3,12,8,8,0],[[4,8,128],[8,8,128]],[[16,16],[32,16]]",1061));//R16_33:[[0,13],[0,2]],[[0,13],[9,7]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,512,31,32,[1,3,12,0,16,0],[[4,8,128],[8,4,128],[8,8,256]],[[8,16],[16,8],[32,8]]",1062));//R8_2:[[0,1],[6,0]],[[2,0],[0,2]]		
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,8,20,0,0],[[4,4,64],[4,8,64],[8,4,128],[8,8,128]],[[16,24],[32,8]]",1063));//R16_29:[[0,0],[0,7]],[[0,1],[0,1]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,23,288,31,8,[1,3,8,12,8,0],[[2,8,32],[4,4,128],[4,8,160],[8,2,32],[8,4,160],[8,8,224]],[[4,16],[8,8],[16,6],[32,2]]",1064));//R8_3:[[0,1],[4,4]],[[2,0],[0,6]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,15,320,31,8,[1,3,8,20,0,0],[[2,8,32],[4,4,128],[4,8,160],[8,2,32],[8,4,160],[8,8,192]],[[4,8],[8,16],[16,6],[32,2]]",1065));//R8_3:[[0,0],[1,2]],[[2,0],[0,6]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,11,23,384,31,2,[1,3,12,16,0,0],[[4,4,96],[4,8,160],[8,2,32],[8,4,160],[8,8,192]],[[8,24],[16,4],[32,4]]",1066));//R8_3:[[0,0],[1,2]],[[2,4],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,23,288,31,8,[1,3,8,12,8,0],[[2,8,32],[4,4,128],[4,8,160],[8,2,32],[8,4,160],[8,8,224]],[[4,16],[8,8],[16,6],[32,2]]",1067));//R8_3:[[0,0],[5,0]],[[2,0],[0,6]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,15,512,31,32,[1,3,12,0,0,16],[[4,8,128],[8,4,128],[8,8,256]],[[8,16],[16,8],[32,8]]",1068));//R8_3:[[0,0,0],[0,0,1],[2,0,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,31,256,31,32,[1,3,8,4,16,0],[[2,8,32],[4,4,128],[4,8,160],[8,2,32],[8,4,160],[8,8,256]],[[4,24],[16,6],[32,2]]",1069));//R8_3:[[0,1],[4,2]],[[2,0],[0,2]]
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,256,31,8,[1,3,8,4,16,0],[[2,8,32],[4,4,128],[4,8,160],[8,2,32],[8,4,160],[8,8,256]],[[4,24],[16,6],[32,2]]",1070));//R8_3:	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,15,448,31,32,[1,3,4,16,8,0],[[2,4,16],[2,8,16],[4,2,16],[4,4,96],[4,8,112],[8,2,16],[8,4,112],[8,8,192]],[[8,16],[16,12],[32,4]]",1071));//R8_2×R8_4的商环
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,15,512,31,32,[1,3,4,16,8,0],[[2,4,16],[2,8,16],[4,2,16],[4,4,96],[4,8,112],[8,2,16],[8,4,112],[8,8,128]],[[8,16],[16,8],[32,8]]",1072));//R16_13×R16_14的商环
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,15,384,31,8,[1,3,8,12,8,0],[[4,4,64],[4,8,160],[8,4,160],[8,8,256]],[[4,8],[8,12],[16,8],[32,4]]",1073));//R16_30:
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,1,16,2,3,7,112,15,32,[1,3,4,4,12,8],[[2,4,16],[2,8,48],[4,2,16],[4,4,144],[4,8,192],[8,2,48],[8,4,192],[8,8,256]]",1074));//Z[i]/(4+4i)
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,15,384,31,8,[1,3,8,20,0,0],[[4,4,64],[4,8,160],[8,4,160],[8,8,256]],[[4,8],[8,12],[16,8],[32,4]]",1075));//R16_34:
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,15,320,31,8,[1,3,4,16,8,0],[[4,4,64],[4,8,192],[8,4,192],[8,8,256]],[[4,16],[8,8],[16,4],[32,4]]",1076));//R16_34:
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,23,31,576,31,8,[1,3,12,8,8,0],[[4,8,128],[8,4,128],[8,8,192]],[[8,8],[16,16],[32,8]]",1077));//R16_34:	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,15,384,31,8,[1,3,12,8,8,0],[[4,8,192],[8,4,192],[8,8,256]],[[4,16],[16,12],[32,4]]",1078));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,23,288,31,32,[1,3,4,16,8,0],[[2,4,16],[2,8,16],[4,2,16],[4,4,96],[4,8,176],[8,2,16],[8,4,176],[8,8,224]],[[4,16],[8,8],[16,6],[32,2]]",1079));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,15,448,31,8,[1,3,12,0,16,0],[[4,8,192],[8,4,128],[8,8,256]],[[8,16],[16,12],[32,4]]",1080));//R16_34:
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,4,16,8,0],[[4,4,64],[4,8,64],[8,4,128],[8,8,256]],[[8,16],[16,8],[32,8]]",1081));//R16_31:
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,576,31,8,[1,3,4,16,8,0],[[4,4,64],[4,8,64],[8,4,128],[8,8,192]],[[8,8],[16,16],[32,8]]",1082));	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,4,8,16,0],[[4,4,64],[4,8,64],[8,4,64],[8,8,256]],[[8,8],[16,16],[32,8]]",1083));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,4,16,8,0],[[4,4,64],[4,8,64],[8,4,128],[8,8,128]],[[16,24],[32,8]]",1084));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,4,16,8,0],[[4,4,64],[4,8,64],[8,4,128],[8,8,192]],[[8,8],[16,16],[32,8]]",1085));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,31,576,31,32,[1,3,4,8,16,0],[[4,4,64],[4,8,64],[8,4,64],[8,8,256]],[[8,8],[16,16],[32,8]]",1086));	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,31,576,31,32,[1,3,8,12,8,0],[[4,4,64],[4,8,128],[8,4,128],[8,8,128]],[[8,8],[16,16],[32,8]]",1087));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,640,31,32,[1,3,12,0,16,0],[[4,8,128],[8,4,128],[8,8,128]],[[16,24],[32,8]]",1088));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,15,384,31,8,[1,3,8,4,16,0],[[4,4,64],[4,8,160],[8,4,160],[8,8,256]],[[4,8],[8,12],[16,8],[32,4]]",1089));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,15,448,31,8,[1,3,4,16,8,0],[[4,4,64],[4,8,128],[8,4,128],[8,8,256]],[[8,16],[16,12],[32,4]]",1090));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,31,576,31,32,[1,3,4,8,16,0],[[4,4,64],[4,8,128],[8,4,128],[8,8,128]],[[8,8],[16,16],[32,8]]",1091));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,23,31,576,31,8,[1,3,12,16,0,0],[[4,8,128],[8,4,128],[8,8,192]],[[8,8],[16,16],[32,8]]",1092));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,23,31,576,31,8,[1,3,4,24,0,0],[[4,4,64],[4,8,128],[8,4,128],[8,8,128]],[[8,8],[16,16],[32,8]]",1093));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,576,31,8,[1,3,4,16,8,0],[[4,4,64],[4,8,128],[8,4,64],[8,8,192]],[[8,8],[16,16],[32,8]]",1094));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,23,31,576,31,8,[1,3,8,20,0,0],[[4,4,64],[4,8,128],[8,4,128],[8,8,128]],[[8,8],[16,16],[32,8]]",1095));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,8,20,0,0],[[4,4,64],[4,8,128],[8,4,128],[8,8,192]],[[8,16],[16,8],[32,8]]",1096));	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,512,31,32,[1,3,8,12,8,0],[[4,4,64],[4,8,128],[8,4,128],[8,8,192]],[[8,16],[16,8],[32,8]]",1097));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,8,4,16,0],[[4,4,64],[4,8,128],[8,4,64],[8,8,192]],[[8,8],[16,16],[32,8]]",1098));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,8,4,16,0],[[4,4,64],[4,8,128],[8,4,128],[8,8,192]],[[8,16],[16,8],[32,8]]",1099));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,8,4,16,0],[[4,4,64],[4,8,64],[8,4,128],[8,8,256]],[[8,16],[16,8],[32,8]]",1100));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,8,12,8,0],[[4,4,64],[4,8,128],[8,4,64],[8,8,256]],[[8,16],[16,8],[32,8]]",1101));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,31,448,31,32,[1,3,8,12,8,0],[[4,4,64],[4,8,128],[8,4,128],[8,8,256]],[[8,24],[32,8]]",1102));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,8,12,8,0],[[4,4,64],[4,8,64],[8,4,128],[8,8,256]],[[8,16],[16,8],[32,8]]",1103));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,576,31,8,[1,3,12,0,16,0],[[4,8,128],[8,4,128],[8,8,192]],[[8,8],[16,16],[32,8]]",1104));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,15,31,576,31,8,[1,3,8,12,8,0],[[4,4,64],[4,8,64],[8,4,128],[8,8,192]],[[8,8],[16,16],[32,8]]",1105));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,8,4,16,0],[[4,4,64],[4,8,64],[8,4,64],[8,8,256]],[[8,8],[16,16],[32,8]]",1106));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,8,4,16,0],[[4,4,64],[4,8,64],[8,4,128],[8,8,192]],[[8,8],[16,16],[32,8]]",1107));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,4,16,8,0],[[4,4,64],[4,8,128],[8,4,64],[8,8,192]],[[8,8],[16,16],[32,8]]",1108));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,640,31,8,[1,3,8,4,16,0],[[4,4,64],[4,8,64],[8,4,64],[8,8,192]],[[16,24],[32,8]]",1109));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,8,4,16,0],[[4,4,64],[4,8,128],[8,4,64],[8,8,256]],[[8,16],[16,8],[32,8]]",1110));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,4,8,16,0],[[4,4,64],[4,8,128],[8,4,128],[8,8,192]],[[8,16],[16,8],[32,8]]",1111));	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,7,31,448,31,8,[1,3,4,24,0,0],[[4,4,64],[4,8,128],[8,4,128],[8,8,256]],[[8,24],[32,8]]",1112));	
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,0,0,32,1,11,23,384,31,2,[1,3,4,24,0,0],[[4,4,96],[4,8,160],[8,2,32],[8,4,160],[8,8,192]],[[8,24],[16,4],[32,4]]",1113));
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,512,31,32,[1,3,12,16,0,0],[[4,8,128],[8,4,128],[8,8,256]],[[8,16],[16,8],[32,8]]",1114));	
	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,7,15,288,31,8,[1,7,8,8,8,0],[[2,8,64],[4,4,64],[4,8,128],[8,2,96],[8,4,128],[8,8,256]]",2000));//R16_20×R2_1=R32_2000=[[4,0],[3,1]],[[0,0],[4,0]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,512,31,8,[1,7,8,0,16,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,256]]",2001));//[[0,6],[0,0]],[[6,7],[4,4]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,15,512,31,8,[1,7,8,0,16,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,256]]",2002));//[[4,0],[0,0]],[[4,7],[4,4]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,7,15,288,31,8,[1,7,8,8,8,0],[[2,8,96],[4,4,64],[4,8,128],[8,2,64],[8,4,128],[8,8,256]]",2003));//R16_23×R2_1=R32_2003=[[1,4],[0,0]],[[7,4],[4,4]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,5,7,15,256,31,4,[1,7,8,16,0,0],[[2,8,96],[4,4,64],[4,8,128],[8,2,96],[8,4,128],[8,8,256]]",2004));//[[5,4],[4,0]],[[7,0],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,1,16,2,7,15,224,15,32,[1,7,8,4,12,0],[[2,8,112],[4,4,64],[4,8,128],[8,2,112],[8,4,128],[8,8,256]]",2005));//[[4,4],[4,4]],[[5,0],[4,5]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,15,448,31,8,[1,7,8,8,8,0],[[2,8,64],[4,8,128],[8,4,128],[8,8,256]]",2006));//R16_7:[[0,0,0],[0,0,0],[0,1,12]],[[0,0,0],[0,0,0],[0,2,3]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,15,512,31,32,[1,7,8,0,16,0],[[4,8,128],[8,4,128],[8,8,256]]",2007));//R16_8×R2_1=R32_2007=[[2,0],[0,6]],[[0,8],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,23,31,640,31,8,[1,7,8,16,0,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,128]]",2008));	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,15,448,31,8,[1,7,8,8,8,0],[[4,8,128],[8,2,64],[8,4,128],[8,8,256]]",2009));//R16_30:[[0,0],[0,1]],[[0,0],[10,0]]	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,15,384,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,96],[8,2,32],[8,4,128],[8,8,256]]",2010));//R16_30:[[0,0],[0,1]],[[0,0],[10,8]]		
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,7,8,0,16,0],[[8,2,64],[8,4,64],[8,8,256]]",2011));//R16_31:[[0,0],[3,0]],[[0,0],[4,1]]
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
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,7,7,320,31,8,[1,5,6,12,8,0],[[2,2,24],[2,4,24],[2,8,48],[4,2,24],[4,4,24],[4,8,128],[8,2,48],[8,4,128],[8,8,256]]",2018));//R8R4_19=R8_2×R4_8	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,6,3,7,200,23,8,[1,5,6,16,4,0],[[2,2,24],[2,4,24],[2,8,88],[4,2,24],[4,4,64],[4,8,128],[8,2,88],[8,4,128],[8,8,256]]",2028));//R8R4_29=R8_3×R4_7
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,7,15,480,31,8,[1,5,6,12,8,0],[[2,2,24],[2,4,24],[2,8,48],[4,2,24],[4,4,24],[4,8,48],[8,2,48],[8,4,48],[8,8,256]]",2039));//R8R4_40=R8_4×R4_7
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,15,15,640,31,8,[1,5,6,12,8,0],[[2,2,24],[2,4,24],[2,8,48],[4,2,24],[4,4,24],[4,8,48],[8,2,48],[8,4,48],[8,8,96]]",2035));//R8R4_8=R8_1×R4_8
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
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,12,8,8,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,32],[8,4,32],[8,8,256]]",2044));//R16_31:[[0,0],[1,4]],[[0,0],[1,5]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,12,8,8,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,32],[8,4,32],[8,8,128]]",2045));//R16_29:[[0,0],[3,3]],[[0,0],[4,5]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,32],[8,4,32],[8,8,128]]",2046));//R16_29:[[0,0],[3,4]],[[0,0],[8,5]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,512,31,32,[1,7,8,0,16,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,256]]",2047));//R16_27×R2_1=R32_2047=R16_27:[[0,0],[3,4]],[[0,0],[3,8]]	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,7,8,8,8,0],[[8,2,64],[8,4,64],[8,8,256]]",2048));//R16_26:[[0,0],[3,12]],[[0,0],[8,0]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,576,31,8,[1,7,8,8,8,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,192]]",2049));//R16_26:[[0,0],[3,12]],[[0,0],[8,1]]	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,7,8,8,8,0],[[2,8,64],[4,8,64],[8,8,256]]",2050));//R16_26:[[0,0],[3,12]],[[0,8],[0,0]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,7,384,31,32,[1,4,7,4,8,8],[[2,2,16],[2,4,16],[2,8,80],[4,2,16],[4,4,16],[4,8,80],[8,2,80],[8,4,80],[8,8,256]],[[4,8],[8,12],[16,8],[32,4]]",2051));//R16_17×R2_2=R32_2051=R16_17×R4_3的商环=R16_6:[[1,14],[9,8]]	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,15,15,480,31,32,[1,4,7,12,8,0],[[2,2,16],[2,4,16],[2,8,80],[4,2,16],[4,4,16],[4,8,80],[8,2,80],[8,4,80],[8,8,160]]",2052));//R16_16×R2_2=R32_2052=R16_16×R4_3的商环	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,768,31,32,[1,3,12,16,0,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,64]]",2053));//R16_29×R2_1=R32_2053=R16_29×R4_2的商环	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,640,31,32,[1,3,12,16,0,0],[[2,2,24],[2,4,24],[2,8,48],[4,2,24],[4,4,24],[4,8,48],[8,2,48],[8,4,48],[8,8,96]]",2054));//R16_29×R4_2的商环
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,7,15,448,31,32,[1,3,12,16,0,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,96],[8,2,32],[8,4,96],[8,8,256]]",2055));//R16_30×R2_1=R32_2055=R16_30×R4_2的商环	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,7,15,416,31,32,[1,3,12,16,0,0],[[2,2,24],[2,4,24],[2,8,48],[4,2,24],[4,4,24],[4,8,80],[8,2,48],[8,4,80],[8,8,256]]",2056));//R16_30×R4_2的商环
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,15,256,31,32,[1,7,8,4,8,4],[[2,8,96],[4,4,64],[4,8,128],[8,2,96],[8,4,128],[8,8,256]]",2057));//R16_7×R2_1
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,1,24,4,3,7,192,23,32,[1,4,7,12,8,0],[[2,2,16],[2,4,16],[2,8,104],[4,2,16],[4,4,64],[4,8,128],[8,2,104],[8,4,128],[8,8,256]]",2058));//R16_7×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,7,384,31,32,[1,4,7,4,8,8],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,128],[8,2,32],[8,4,128],[8,8,256]]",2059));//R16_8×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,31,31,640,31,32,[1,7,8,16,0,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,128]]",2060));//R16_16×R2_1
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,15,512,31,32,[1,7,8,0,16,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,256]]",2061));//R16_17×R2_1
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,23,31,768,31,8,[1,7,8,16,0,0],[[2,8,64],[4,8,64],[8,8,128]]",2062));//R16_18×R2_1
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,2,11,15,576,31,8,[1,4,7,12,8,0],[[2,2,16],[2,4,16],[2,8,80],[4,2,16],[4,4,16],[4,8,80],[8,2,32],[8,4,32],[8,8,160]]",2063));//R16_18×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,15,512,31,8,[1,7,8,16,0,0],[[2,8,64],[4,8,64],[8,4,128],[8,8,256]]",2064));//R16_19×R2_1
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,2,7,7,384,31,8,[1,4,7,12,8,0],[[2,2,16],[2,4,16],[2,8,80],[4,2,16],[4,4,16],[4,8,80],[8,2,32],[8,4,128],[8,8,256]]",2065));//R16_19×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,6,3,7,216,23,8,[1,4,7,20,0,0],[[2,2,16],[2,4,16],[2,8,80],[4,2,16],[4,4,64],[4,8,128],[8,2,104],[8,4,128],[8,8,256]]",2066));//R16_20×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,23,31,768,31,8,[1,7,8,16,0,0],[[8,2,64],[8,4,64],[8,8,128]]",2067));//R16_21×R2_1
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,2,11,15,576,31,8,[1,4,7,12,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,80],[8,4,80],[8,8,160]]",2068));//R16_21×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,768,31,32,[1,7,8,0,16,0],[[8,8,256]]",2069));//R16_22×R2_1
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,15,576,31,32,[1,4,7,4,8,8],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,256]]",2070));//R16_22×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,6,3,7,216,31,8,[1,4,7,20,0,0],[[2,2,16],[2,4,16],[2,8,104],[4,2,16],[4,4,64],[4,8,128],[8,2,80],[8,4,128],[8,8,256]]",2071));//R16_23×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,15,512,31,32,[1,7,8,16,0,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,256]]",2072));//R16_24×R2_1
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,7,384,31,32,[1,4,7,12,8,0],[[2,2,16],[2,4,16],[2,8,80],[4,2,16],[4,4,16],[4,8,80],[8,2,80],[8,4,80],[8,8,256]]",2073));//R16_24×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,15,512,31,8,[1,7,8,16,0,0],[[4,8,128],[8,2,64],[8,4,64],[8,8,256]]",2074));//R16_25×R2_1
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,2,7,7,384,31,8,[1,4,7,12,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,128],[8,2,80],[8,4,80],[8,8,256]]",2075));//R16_25×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,640,31,32,[1,7,8,16,0,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,128]]",2076));//R16_26×R2_1
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,15,480,31,32,[1,4,7,12,8,0],[[2,2,16],[2,4,16],[2,8,80],[4,2,16],[4,4,16],[4,8,80],[8,2,80],[8,4,80],[8,8,160]]",2077));//R16_26×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,15,384,31,32,[1,4,7,4,8,8],[[2,2,16],[2,4,16],[2,8,80],[4,2,16],[4,4,16],[4,8,80],[8,2,80],[8,4,80],[8,8,256]]",2078));//R16_27×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,15,576,31,32,[1,2,7,14,8,0],[[2,2,28],[2,4,28],[2,8,56],[4,2,28],[4,4,28],[4,8,56],[8,2,56],[8,4,56],[8,8,112]]",2079));//R16_29×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,3,7,336,31,32,[1,2,7,14,8,0],[[2,2,28],[2,4,28],[2,8,56],[4,2,28],[4,4,28],[4,8,104],[8,2,56],[8,4,104],[8,8,256]]",2080));//R16_30×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,640,31,32,[1,3,12,16,0,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,192]]",2081));//R16_31×R2_1
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,15,480,31,32,[1,2,7,14,8,0],[[2,2,28],[2,4,28],[2,8,56],[4,2,28],[4,4,28],[4,8,56],[8,2,56],[8,4,56],[8,8,208]]",2082));//R16_31×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,15,224,31,32,[1,3,12,4,8,4],[[2,2,16],[2,4,16],[2,8,96],[4,2,16],[4,4,48],[4,8,128],[8,2,96],[8,4,128],[8,8,256]]",2083));//R16_32×R2_1
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,1,24,4,3,7,168,23,32,[1,2,7,14,8,0],[[2,2,28],[2,4,28],[2,8,104],[4,2,28],[4,4,52],[4,8,128],[8,2,104],[8,4,128],[8,8,256]]",2084));//R16_32×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,12,8,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,64],[8,4,64],[8,8,256]]",2085));//R16_31:[[0,0],[0,1]],[[0,1],[0,4]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,23,31,768,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,64]]",2086));//R16_28×R2_1=R32_2086
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,12,8,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,64],[8,4,64],[8,8,128]]",2087));//R16_28:
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,640,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,192]]",2088));//R16_33×R2_1=R32_2088
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,256]]",2089));//R16_33:[[0,0],[0,3]],[[0,0],[9,1]]或[[0,0,0],[0,0,0],[0,2,9]],[[0,0,0],[0,0,0],[0,3,3]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,32],[8,4,32],[8,8,192]]",2090));//R16_33:[[0,0],[0,5]],[[0,0],[9,1]]或[[0,0,0],[0,0,0],[0,2,9]],[[0,0,0],[0,0,0],[0,5,3]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,2,11,15,576,31,8,[1,2,7,14,8,0],[[2,2,28],[2,4,28],[2,8,56],[4,2,28],[4,4,28],[4,8,56],[8,2,56],[8,4,56],[8,8,112]]",2091));//R16_28×R2_2	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,2,3,15,480,31,8,[1,2,7,14,8,0],[[2,2,28],[2,4,28],[2,8,56],[4,2,28],[4,4,28],[4,8,56],[8,2,56],[8,4,56],[8,8,208]]",2092));//R16_33×R2_2
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,7,15,208,15,8,[1,3,12,8,8,0],[[2,2,16],[2,4,16],[2,8,96],[4,2,16],[4,4,48],[4,8,128],[8,2,112],[8,4,128],[8,8,256]]",2093));//R16_32:[[0,0],[0,1]],[[0,0],[13,2]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,7,15,240,15,8,[1,7,8,8,8,0],[[2,8,96],[4,4,64],[4,8,128],[8,2,112],[8,4,128],[8,8,256]]",2094));//R16_7:[[0,0],[4,14]],[[0,0],[7,8]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,23,31,768,31,8,[1,7,8,8,8,0],[[2,8,64],[4,8,64],[8,8,128]]",2095));//R16_7:[[0,0],[4,15]],[[0,0],[5,0]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,32],[8,4,32],[8,8,128]]",2096));//R16_31:[[0,0,0],[0,0,0],[0,0,11]],[[0,0,0],[0,0,0],[0,5,0]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,576,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,32],[8,4,32],[8,8,192]]",2097));//R16_31:[[0,0],[1,4]],[[0,0],[8,5]]或[[0,0,0],[0,0,0],[0,2,9]],[[0,0,0],[0,0,0],[0,5,4]]	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,23,31,768,31,8,[1,7,8,8,8,0],[[8,2,64],[8,4,64],[8,8,128]]",2098));//R16_31:[[0,0],[0,5]],[[0,0],[9,0]]或[[0,0,0],[0,0,0],[0,3,8]],[[0,0,0],[0,0,0],[0,4,5]]	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,15,512,31,8,[1,7,8,0,16,0],[[2,8,64],[4,8,64],[8,4,128],[8,8,256]]",2099));//R16_7:[[0,0,0],[0,0,0],[0,1,12]],[[0,0,0],[0,0,0],[0,2,4]]	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,7,8,0,16,0],[[2,8,64],[4,8,64],[8,8,256]]",2100));//R16_24:[[0,1],[4,3]],[[0,1],[4,6]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,64],[8,4,64],[8,8,128]]",2101));//R16_29:[[0,13],[0,2]],[[0,13],[0,9]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,23,31,576,31,8,[1,7,8,8,8,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,192]]",2102));//R16_29:[[0,2],[11,0]],[[0,3],[0,4]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,640,31,8,[1,3,8,4,16,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,192]]",2103));//R16_33:[[0,3],[14,6]],[[0,4],[1,1]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,23,31,768,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,64]]",2104));//R16_28:[[0,6],[0,15]],[[0,6],[2,3]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,576,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,64],[8,4,64],[8,8,192]]",2105));//R16_31:[[2,7],[2,4]],[[2,7],[2,13]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,15,512,31,8,[1,7,8,0,16,0],[[4,8,128],[8,2,64],[8,4,64],[8,8,256]]",2106));//R16_25:[[0,0],[0,1]],[[0,0],[1,2]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,15,384,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,128],[8,2,64],[8,4,96],[8,8,256]]",2107));//R16_30:[[0,0],[0,1]],[[0,10],[0,8]]	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,15,512,31,8,[1,7,8,16,0,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,256]]",2108));//R8_3:[[4,0],[0,0]],[[4,0],[0,4]],[[4,0],[1,2]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,640,31,32,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,192]]",2109));//R16_31:[[0,7],[13,0]],[[0,9],[9,0]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,576,31,8,[1,3,12,16,0,0],[[2,2,24],[2,4,24],[2,8,48],[4,2,24],[4,4,24],[4,8,48],[8,2,48],[8,4,48],[8,8,160]]",2110));//R16_33×R8_33的商环
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,23,31,768,31,8,[1,5,10,16,0,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,64]]",2111));//R16_28×R16_13的商环
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,576,31,32,[1,3,12,16,0,0],[[2,2,24],[2,4,24],[2,8,48],[4,2,24],[4,4,24],[4,8,48],[8,2,48],[8,4,48],[8,8,160]]",2112));//R16_15×R16_16的商环
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,576,31,32,[1,3,12,16,0,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,64],[8,4,64],[8,8,128]]",2113));//R16_16×R16_17的商环
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,32],[8,4,32],[8,8,128]]",2114));//R16_17×R16_18的商环
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,5,7,15,272,15,4,[1,7,8,16,0,0],[[2,8,64],[4,4,64],[4,8,128],[8,2,112],[8,4,128],[8,8,256]],[[1,16],[8,8],[16,4],[32,4]]",2115));//R16_6:	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,1,24,6,3,7,184,23,8,[1,5,6,16,4,0],[[2,2,24],[2,4,24],[2,8,104],[4,2,24],[4,4,64],[4,8,128],[8,2,88],[8,4,128],[8,8,256]],[[1,8],[2,4],[4,10],[8,6],[16,3],[32,1]]",2116));//R16_6:	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,15,384,31,8,[1,3,12,8,8,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,128],[8,2,32],[8,4,96],[8,8,256]],[[8,24],[16,4],[32,4]]",2117));//R16_30:	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,64],[8,4,64],[8,8,192]],[[8,8],[16,16],[32,8]]",2118));//R16_30:
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,15,384,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,128],[8,2,32],[8,4,128],[8,8,256]],[[4,8],[8,12],[16,8],[32,4]]",2119));//R16_34:	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,15,384,31,8,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,96],[8,2,32],[8,4,128],[8,8,256]],[[4,8],[8,12],[16,8],[32,4]]",2120));//R16_34:
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,15,384,31,8,[1,3,12,8,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,96],[8,2,64],[8,4,128],[8,8,256]],[[4,16],[16,12],[32,4]]",2121));//R16_34:	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,15,384,31,8,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,128],[8,2,64],[8,4,96],[8,8,256]],[[4,8],[8,12],[16,8],[32,4]]",2122));//R16_34:	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,15,448,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,96],[8,2,32],[8,4,96],[8,8,256]],[[8,16],[16,12],[32,4]]",2123));//R16_34×R2_1	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,2,3,7,336,31,8,[1,2,7,14,8,0],[[2,2,28],[2,4,28],[2,8,56],[4,2,28],[4,4,28],[4,8,104],[8,2,56],[8,4,104],[8,8,256]],[[4,8],[8,14],[16,8],[32,2]]",2124));//R16_34×R2_2	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,15,448,31,8,[1,7,8,8,8,0],[[2,8,64],[4,8,128],[8,2,64],[8,4,64],[8,8,256]],[[8,16],[16,12],[32,4]]",2125));//R16_34:	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,15,384,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,96],[8,2,64],[8,4,96],[8,8,256]],[[4,8],[8,12],[16,8],[32,4]]",2126));//R16_34:
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,15,448,31,8,[1,7,8,8,8,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,128],[8,8,256]],[[4,16],[16,8],[32,8]]",2127));//R16_34:	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,15,416,31,8,[1,3,12,16,0,0],[[2,2,24],[2,4,24],[2,8,48],[4,2,24],[4,4,24],[4,8,80],[8,2,48],[8,4,80],[8,8,256]],[[8,16],[16,14],[32,2]]",2128));//R16_34×R16_34的商环
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,15,448,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,64],[8,4,64],[8,8,256]],[[8,16],[16,12],[32,4]]",2129));//R16_33×R16_34的商环
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,15,512,31,32,[1,7,8,0,0,16],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,256]],[[8,16],[16,8],[32,8]]",2130));//R32_1008:[[0,1],[30,16]]
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,15,384,31,32,[1,7,8,0,0,16],[[2,8,64],[4,8,128],[8,2,64],[8,4,128],[8,8,256]],[[4,16],[16,12],[32,4]]",2131));//R32_1008:[[0,2],[29,24]]	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,15,384,31,32,[1,7,8,16,0,0],[[2,8,64],[4,8,128],[8,2,64],[8,4,128],[8,8,256]],[[4,16],[16,12],[32,4]]",2132));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,15,384,31,8,[1,7,8,16,0,0],[[2,8,64],[4,8,128],[8,2,64],[8,4,128],[8,8,256]],[[4,16],[16,12],[32,4]]",2133));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,1,16,2,7,15,192,15,32,[1,3,12,4,12,0],[[2,2,16],[2,4,16],[2,8,112],[4,2,16],[4,4,48],[4,8,128],[8,2,112],[8,4,128],[8,8,256]],[[1,16],[8,12],[16,3],[32,1]]",2134));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,64],[8,4,64],[8,8,192]],[[8,16],[16,8],[32,8]]",2135));//R16_31:	
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,7,31,576,31,32,[1,3,12,0,16,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,256]],[[8,8],[16,16],[32,8]]",2136));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,576,31,8,[1,3,12,8,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,64],[8,4,64],[8,8,192]],[[8,8],[16,16],[32,8]]",2137));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,12,0,16,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,256]],[[8,8],[16,16],[32,8]]",2138));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,12,8,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,64],[8,4,64],[8,8,192]],[[8,8],[16,16],[32,8]]",2139));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,7,31,576,31,32,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,64],[8,4,64],[8,8,128]],[[8,8],[16,16],[32,8]]",2140));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,640,31,32,[1,7,8,0,16,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,128]],[[16,24],[32,8]]",2141));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,7,31,576,31,32,[1,3,12,0,16,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,64],[8,4,64],[8,8,128]],[[8,8],[16,16],[32,8]]",2142));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,15,384,31,8,[1,7,8,0,16,0],[[2,8,64],[4,8,128],[8,2,64],[8,4,128],[8,8,256]],[[4,16],[16,12],[32,4]]",2143));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,15,320,31,8,[1,3,12,0,16,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,128],[8,2,64],[8,4,128],[8,8,256]],[[4,16],[8,8],[16,4],[32,4]]",2144));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,15,384,31,32,[1,7,8,0,16,0],[[2,8,64],[4,8,128],[8,2,64],[8,4,128],[8,8,256]],[[4,16],[16,12],[32,4]]",2145));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,23,31,576,31,8,[1,7,8,16,0,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,192]],[[8,8],[16,16],[32,8]]",2146));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,23,31,576,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,64],[8,4,64],[8,8,128]],[[8,8],[16,16],[32,8]]",2147));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,23,31,576,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,64],[8,4,64],[8,8,128]],[[8,8],[16,16],[32,8]]",2148));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,512,31,32,[1,7,8,16,0,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,256]],[[8,16],[16,8],[32,8]]",2149));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,576,31,8,[1,3,12,8,8,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,32],[8,4,32],[8,8,192]],[[8,8],[16,16],[32,8]]",2150));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,32],[8,4,32],[8,8,256]],[[8,16],[16,8],[32,8]]",2151));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,7,31,448,31,32,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,64],[8,4,64],[8,8,256]],[[8,24],[32,8]]",2152));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,8,4,16,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,32],[8,4,32],[8,8,192]],[[8,8],[16,16],[32,8]]",2153));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,8,4,16,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,64],[8,4,64],[8,8,192]],[[8,16],[16,8],[32,8]]",2154));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,512,31,32,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,64],[8,4,64],[8,8,192]],[[8,16],[16,8],[32,8]]",2155));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,8,4,16,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,32],[8,4,32],[8,8,256]],[[8,16],[16,8],[32,8]]",2156));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,8,4,16,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,64],[8,4,64],[8,8,256]],[[8,16],[16,8],[32,8]]",2157));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,576,31,8,[1,7,8,0,16,0],[[2,8,64],[4,8,64],[8,2,64],[8,4,64],[8,8,192]],[[8,8],[16,16],[32,8]]",2158));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,64],[8,4,64],[8,8,256]],[[8,16],[16,8],[32,8]]",2159));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,640,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,64],[8,4,64],[8,8,128]],[[16,24],[32,8]]",2160));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,576,31,8,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,64],[8,4,64],[8,8,192]],[[8,8],[16,16],[32,8]]",2161));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,7,31,448,31,32,[1,3,12,0,16,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,64],[8,4,64],[8,8,256]],[[8,24],[32,8]]",2162));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,8,4,16,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,32],[8,4,32],[8,8,256]],[[8,8],[16,16],[32,8]]",2163));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,8,4,16,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,32],[8,2,64],[8,4,64],[8,8,192]],[[8,8],[16,16],[32,8]]",2164));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,576,31,8,[1,3,12,8,8,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,32],[8,4,32],[8,8,192]],[[8,8],[16,16],[32,8]]",2165));		
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,31,512,31,8,[1,3,12,0,16,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,64],[8,4,64],[8,8,192]],[[8,16],[16,8],[32,8]]",2166));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,15,31,576,31,8,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,32],[8,4,32],[8,8,192]],[[8,8],[16,16],[32,8]]",2167));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,1,7,15,448,31,8,[1,3,12,0,16,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,96],[8,2,32],[8,4,96],[8,8,256]],[[8,16],[16,12],[32,4]]",2168));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,7,15,448,31,32,[1,3,12,0,16,0],[[2,2,16],[2,4,16],[2,8,32],[4,2,16],[4,4,16],[4,8,96],[8,2,32],[8,4,96],[8,8,256]],[[8,16],[16,12],[32,4]]",2169));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,7,15,384,31,32,[1,3,8,12,8,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,96],[8,2,64],[8,4,96],[8,8,256]],[[4,8],[8,12],[16,8],[32,4]]",2170));
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,512,31,32,[1,3,12,16,0,0],[[2,2,16],[2,4,16],[2,8,64],[4,2,16],[4,4,16],[4,8,64],[8,2,64],[8,4,64],[8,8,192]],[[8,16],[16,8],[32,8]]",2171));
	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,16,2,7,7,160,15,32,[1,7,2,14,0,8],[[2,4,144],[4,2,144],[4,4,576]]",3000));//R4_3/([[1,1,1]],[0,1,0,1])	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,7,192,31,32,[1,7,2,12,6,4],[[2,4,128],[4,2,128],[4,4,576]]",3001));//R16_105×R2_1=R32_3001=R4_3/([[0,0,1],[2,1]],[0,1,0,1])
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,5,11,15,288,31,2,[1,7,16,8,0,0],[[2,4,128],[4,2,96],[4,4,512]]",3002));//[[3,0],[0,0]],[[2,0],[3,2]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,1,16,2,11,15,192,15,8,[1,7,10,6,8,0],[[2,4,144],[4,2,144],[4,4,544]]",3003));//[[0,1],[1,0]],[[2,2],[0,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,1,24,6,7,7,200,23,8,[1,7,10,14,0,0],[[2,4,136],[4,2,136],[4,4,552]]",3004));//[[3,1],[0,0]],[[1,2],[2,1]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,5,11,15,288,31,2,[1,7,16,8,0,0],[[2,4,96],[4,2,128],[4,4,512]]",3005));//[[3,2],[1,0]],[[1,1],[1,1]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,1,16,2,7,7,160,15,8,[1,7,2,14,8,0],[[2,4,144],[4,2,144],[4,4,576]]",3006));//[[2,1],[3,2]],[[2,3],[3,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,7,160,31,32,[1,7,2,4,10,8],[[2,4,144],[4,2,144],[4,4,576]],[[2,24],[8,6],[32,2]]",3007));//R16_106×R2_1=R32_3007=R4_3/([[2,0,1]],[0,1,1,1])=R4_3:[[0,1,2],[1,3,3],[0,2,2]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,224,31,32,[1,7,10,4,6,4],[[2,4,128],[4,2,128],[4,4,544]],[[2,16],[8,12],[16,2],[32,2]]",3008));//R16_110×R2_1=R32_3008=R4_3/([[1,1,2]],[0,0,1,1])
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,16,2,15,15,192,15,32,[1,7,10,6,8,0],[[2,4,144],[4,2,144],[4,4,544]]",3009));//R4_3/([[1,0,2],[1,1,1]],[0,0,1,1])	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,16,2,7,15,192,15,32,[1,7,10,6,8,0],[[2,4,144],[4,2,144],[4,4,544]]",3010));//R4_3/([[0,0,1],[0,2]],[1,1,1,1])	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,15,288,31,32,[1,7,8,0,16,0],[[2,4,128],[4,2,128],[4,4,480]]",3011));//R4_3/([[1,0,1],[1,1]],[1,1,1,1])
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,15,192,31,32,[1,7,0,8,0,16],[[2,4,128],[4,2,128],[4,4,576]],[[2,16],[4,8],[8,4],[16,2],[32,2]]",3012));//[[2,1,1,2],[0,2,1,2],[0,2,0,2],[2,1,0,2]]=R4_3:[[0,1,0],[2,1,3],[0,3,3]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,15,320,31,32,[1,7,8,0,16,0],[[2,4,64],[4,2,64],[4,4,576]]",3013));//R16_109×R2_1=R32_3013=[[2,3,3,0],[0,0,2,2],[0,2,2,0],[0,2,3,0]],[[0,2,1,2],[0,2,0,0],[0,0,2,0],[0,2,2,2]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,16,2,7,15,160,15,32,[1,7,2,14,0,8],[[2,4,144],[4,2,144],[4,4,576]]",3014));//[[0,2,2,3],[3,1,3,3],[2,2,2,0],[3,2,3,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,224,31,32,[1,7,10,4,6,4],[[2,4,128],[4,2,128],[4,4,544]],[[2,16],[8,12],[16,2],[32,2]]",3015));//R16_92×R2_1=R32_3015=[[2,0,0,2],[1,0,3,1],[2,3,0,1],[2,0,0,2]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,16,8,0,0],[[4,4,512]]",3016));//R16_90×R2_1=R32_3016=KW1(2,1)	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,7,15,208,31,8,[1,7,12,4,8,0],[[2,4,144],[4,2,128],[4,4,544]]",3017));//[[0,0,0],[1,0,1],[1,1,0]],[[0,2,2],[0,2,2],[0,2,2]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,15,320,31,8,[1,7,8,0,16,0],[[2,4,64],[4,2,64],[4,4,576]]",3018));//[[0,1,1],[1,1,1],[1,1,1]],[[0,0,0],[2,2,2],[2,2,2]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,16,8,0,0],[[4,4,384]],[[8,16],[32,16]]",3019));//R16_81×R2_1=R32_3019=[[4,0,0],[2,0,2],[2,0,2]],[[0,0,0],[0,0,6],[0,0,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,23,288,31,32,[1,7,8,0,16,0],[[2,4,128],[4,2,128],[4,4,480]],[[4,16],[8,8],[16,6],[32,2]]",3020));//[[0,2,0],[2,0,0],[0,2,2]],[[3,1,2],[3,3,2],[1,2,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,5,11,15,352,31,2,[1,7,16,8,0,0],[[2,4,96],[4,2,96],[4,4,480]]",3021));//R16_101×R2_1=R32_3021=[[1,1,1,1],[1,1,1,1],[1,1,1,1],[1,1,1,1]],[[2,0,0,2],[2,0,0,2],[1,1,1,1],[2,0,0,2]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,15,15,208,15,8,[1,7,12,4,8,0],[[2,4,128],[4,2,144],[4,4,544]]",3022));//[[1,1,0],[0,1,0],[0,1,0]],[[0,2,2],[0,0,0],[0,0,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,5,11,15,272,15,2,[1,7,16,8,0,0],[[2,4,128],[4,2,112],[4,4,512]]",3023));//[[0,1,1],[0,0,0],[0,0,0]],[[1,2,0],[0,1,0],[0,1,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,5,11,15,272,31,2,[1,7,16,8,0,0],[[2,4,112],[4,2,128],[4,4,512]]",3024));//[[1,0,0],[0,1,0],[0,0,0]],[[0,0,0],[2,0,0],[1,0,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,15,15,208,31,8,[1,7,12,4,8,0],[[2,4,144],[4,2,128],[4,4,544]]",3025));//[[2,0,0],[0,2,0],[0,0,0]],[[1,0,0],[1,1,0],[1,0,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,8,16,0,0],[[4,4,384]]",3026));//R16_85×R2_1=R32_3026=R8_9:[[1,2],[7,4]],[[1,4],[3,2]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,7,16,8,0,0],[[4,4,256]]",3027));//R16_82×R2_1=R32_3027=R8_9:[[1,2],[7,4]],[[2,1],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,0,24,0,0],[[4,4,512]]",3028));//R16_80×R2_1=R32_3028=R8_9:[[1,2],[7,4]],[[1,3],[0,2]]		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,7,8,16,0,0],[[4,4,448]]",3029));//R16_77×R2_1=R32_3029=R8_9:[[1,2],[7,4]],[[1,3],[1,3]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,8,16,0,0],[[4,4,512]],[[8,16],[16,8],[32,8]]",3030));//R8_9:[[1,2],[7,4]],[[2,0],[1,3]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,7,0,24,0,0],[[4,4,448]]",3031));//R16_78×R2_1=R32_3031=R8_9:[[1,2],[7,4]],[[2,1],[0,2]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,8,16,0,0],[[4,4,512]]",3032));//R16_74×R2_1=R32_3032=R8_9:[[1,2],[7,4]],[[2,1],[1,3]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,640,31,8,[1,7,0,24,0,0],[[4,4,384]]",3033));//R8_9:[[1,2],[7,4]],[[2,1],[2,1]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,7,16,8,0,0],[[4,4,448]]",3034));//R16_72×R2_1=R32_3034=R8_8:[[1,2],[7,5]],[[1,5],[4,4]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,16,8,0,0],[[4,4,512]]",3035));//R8_6:[[1,1],[7,0]],[[1,1],[7,3]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,448,31,32,[1,7,0,24,0,0],[[4,4,576]],[[8,24],[32,8]]",3036));//R16_93×R2_1=R32_3036=R8_6:[[1,1],[7,0]],[[1,2],[0,3]]		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,448,31,32,[1,7,8,16,0,0],[[4,4,576]]",3037));//R16_99×R2_1=R32_3037=R8_6:[[1,1],[7,0]],[[2,1],[3,3]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,7,16,8,0,0],[[4,4,448]]",3038));//R16_89×R2_1=R32_3038=R8_8:[[0,0],[7,7]],[[0,1],[0,2]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,7,8,16,0,0],[[4,4,448]]",3039));//R16_76×R2_1=R32_3039=R8_8:[[0,0],[7,7]],[[0,1],[0,4]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,4,15,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3040));//R16_108×R2_2=R32_3040=R8_17:[[2,2],[6,6]],[[2,3],[0,1]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,4,11,12,4,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,528]]",3041));//R16_98×R2_2=R32_3041=R8_17:[[2,2],[6,6]],[[2,4],[0,3]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,3,15,336,31,32,[1,4,7,12,8,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,576]]",3042));//R16_99×R2_2=R32_3042=R8_17:[[2,2],[6,0]],[[2,4],[0,3]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,8,16,0,0],[[4,4,576]]",3043));//R16_94×R2_1=R32_3043=R8_18:[[2,3],[1,4]],[[2,4],[0,0]]
	/*---------------begin of R32_3044~R32_3103----------------*/		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,31,31,1024,31,32,[1,7,24,0,0,0],[]",3044));//R8R4_45=R8_5×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,7,16,8,0,0],[[4,4,256]],[[16,16],[32,16]]",3045));//R8R4_46=R8_5×R4_2、R8R4_166=R8_16×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,512,31,32,[1,7,10,6,8,0],[[2,4,64],[4,2,64],[4,4,384]],[[8,16],[16,8],[32,8]]",3046));//R8R4_47=R8_5×R4_3、R8R4_133=R8_13×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,576,31,32,[1,4,11,12,4,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]],[[8,8],[16,16],[32,8]]",3047));//R8R4_177=R8_17×R4_1、R8R4_68=R8_7×R4_2	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,3,15,432,31,32,[1,4,15,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,480]]",3048));//R8R4_178=R8_17×R4_2	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,4,3,7,288,31,32,[1,4,11,8,8,0],[[2,2,16],[2,4,96],[4,2,96],[4,4,528]],[[2,4],[4,10],[8,10],[16,6],[32,2]]",3049));//R8R4_179=R8_17×R4_3、R8R4_145=R8_14×R4_2	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,320,31,32,[1,7,10,6,8,0],[[2,4,112],[4,2,112],[4,4,480]]",3050));//R8R4_201=R8_19×R4_3	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,7,8,16,0,0],[[4,4,448]]",3051));//R8R4_244=R8_23×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,7,2,14,0,8],[[2,4,64],[4,2,64],[4,4,512]],[[4,8],[8,12],[16,8],[32,4]]",3052));//R8R4_245=R8_23×R4_3	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,31,31,640,31,32,[1,7,24,0,0,0],[[2,4,64],[4,2,64],[4,4,256]]",3053));//R8R4_111=R8_11×R4_1	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,480,31,32,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,416]]",3054));//R8R4_112=R8_11×R4_2	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,144]]",3055));//R8R4_56=R8_6×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]]",3056));//R8R4_57=R8_6×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,3,14,2,12,0],[[2,2,16],[2,4,96],[4,2,96],[4,4,432]]",3057));//R8R4_58=R8_6×R4_3	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,320,31,32,[1,7,10,6,8,0],[[2,4,112],[4,2,112],[4,4,480]],[[4,12],[8,10],[16,8],[32,2]]",3058));//R8R4_113=R8_11×R4_3	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,15,384,31,32,[1,7,8,8,8,0],[[2,4,64],[4,2,64],[4,4,512]]",3059));//R8R4_255=R8_24×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,320,31,32,[1,3,10,8,6,4],[[2,2,16],[2,4,112],[4,2,112],[4,4,464]],[[4,16],[8,8],[16,4],[32,4]]",3060));//R8R4_232=R8_22×R4_1	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,15,15,448,31,8,[1,7,12,4,8,0],[[2,4,96],[4,2,64],[4,4,416]]",3061));//R8R4_155=R8_15×R4_1	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,7,15,336,31,8,[1,7,12,4,8,0],[[2,4,96],[4,2,64],[4,4,528]]",3062));//R8R4_156=R8_15×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,6,7,7,224,31,8,[1,7,10,14,0,0],[[2,4,136],[4,2,112],[4,4,552]]",3063));//R8R4_157=R8_15×R4_3	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,384,31,32,[1,7,10,4,6,4],[[2,4,96],[4,2,96],[4,4,448]],[[4,16],[16,12],[32,4]]",3064));//R8R4_221=R8_21×R4_1	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,288,31,32,[1,7,10,4,6,4],[[2,4,96],[4,2,96],[4,4,544]]",3065));//R8R4_222=R8_21×R4_2	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,768,31,32,[1,4,15,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,144]]",3066));//R8R4_67=R8_7×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,384]],[[8,12],[16,16],[32,4]]",3067));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,4,7,7,384,31,32,[1,4,11,8,8,0],[[2,2,16],[2,4,96],[4,2,96],[4,4,432]],[[4,8],[8,12],[16,8],[32,4]]",3068));//R8R4_69=R8_7×R4_3、R8R4_144=R8_14×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,24,4,7,7,192,23,32,[1,7,6,16,2,0],[[2,4,136],[4,2,136],[4,4,560]]",3069));//R8R4_223=R8_21×R4_3	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,640,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,272]]",3070));//R8R4_122=R8_12×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,480,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3071));//R8R4_123=R8_12×R4_2	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,320,31,32,[1,3,14,2,12,0],[[2,2,16],[2,4,96],[4,2,96],[4,4,496]]",3072));//R8R4_124=R8_12×R4_3	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,640,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,272]]",3073));//R8R4_188=R8_18×R4_1	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,480,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3074));//R8R4_189=R8_18×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,3,15,320,31,8,[1,3,14,2,12,0],[[2,2,16],[2,4,96],[4,2,96],[4,4,496]],[[4,12],[8,10],[16,8],[32,2]]",3075));//R8R4_190=R8_18×R4_3	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,3,15,240,31,32,[1,3,10,8,6,4],[[2,2,16],[2,4,112],[4,2,112],[4,4,544]],[[2,8],[4,12],[8,6],[16,4],[32,2]]",3076));//R8R4_233=R8_22×R4_2	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,7,20,4,0,0],[[2,4,64],[4,4,192]]",3077));//R8R4_78=R8_8×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,7,20,4,0,0],[[2,4,64],[4,4,384]]",3078));//R8R4_79=R8_8×R4_2	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,11,15,384,31,8,[1,7,10,6,8,0],[[2,4,112],[4,2,64],[4,4,464]]",3079));//R8R4_80=R8_8×R4_3	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,15,15,448,31,8,[1,7,12,4,8,0],[[2,4,64],[4,2,96],[4,4,416]]",3080));//R8R4_210=R8_20×R4_1	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]],[[8,8],[16,20],[32,4]]",3081));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,7,24,0,0,0],[[4,4,448]]",3082));//R8R4_167=R8_16×R4_2	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,7,10,6,8,0],[[2,4,64],[4,2,64],[4,4,512]],[[4,8],[8,12],[16,8],[32,4]]",3083));//R8R4_168=R8_16×R4_3、R8R4_134=R8_13×R4_2	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]],[[8,8],[16,20],[32,4]]",3084));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,7,8,16,0,0],[[2,4,64],[4,2,96],[4,4,384]],[[8,12],[16,16],[32,4]]",3085));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,4,7,7,256,31,32,[1,7,6,16,2,0],[[2,4,112],[4,2,112],[4,4,544]]",3086));//R8R4_135=R8_13×R4_3	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,7,15,336,31,8,[1,7,12,4,8,0],[[2,4,64],[4,2,96],[4,4,528]]",3087));//R8R4_211=R8_20×R4_2	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,144]]",3088));//R8R4_89=R8_9×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]]",3089));//R8R4_90=R8_9×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,11,15,384,31,8,[1,3,14,2,12,0],[[2,2,16],[2,4,96],[4,2,96],[4,4,432]],[[4,8],[8,12],[16,8],[32,4]]",3090));//R8R4_91=R8_9×R4_3	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,6,7,7,224,23,8,[1,7,10,14,0,0],[[2,4,112],[4,2,136],[4,4,552]]",3091));//R8R4_212=R8_20×R4_3	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,24,4,3,7,160,23,32,[1,3,10,8,10,0],[[2,2,16],[2,4,144],[4,2,144],[4,4,560]]",3092));//R8R4_234=R8_22×R4_3
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,7,256,31,32,[1,7,2,14,0,8],[[2,4,112],[4,2,112],[4,4,544]],[[2,8],[4,8],[8,10],[16,4],[32,2]]",3093));//R8R4_256=R8_24×R4_3=R4_3:[[0,0,2],[3,3,0],[0,1,0]]		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,15,512,31,32,[1,7,8,8,8,0],[[2,4,64],[4,2,64],[4,4,384]]",3094));//R8R4_254=R8_24×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,7,8,16,0,0],[[4,4,256]]",3095));//R8R4_243=R8_23×R4_1	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,640,31,32,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,256]]",3096));//R8R4_199=R8_19×R4_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,480,31,32,[1,7,24,0,0,0],[[2,4,64],[4,2,64],[4,4,416]]",3097));//R8R4_200=R8_19×R4_2	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,400]],[[8,12],[16,16],[32,4]]",3098));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,7,20,4,0,0],[[4,2,64],[4,4,192]]",3099));//R8R4_100=R8_10×R4_1	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,7,20,4,0,0],[[4,2,64],[4,4,384]]",3100));//R8R4_101=R8_10×R4_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,11,15,384,31,8,[1,7,10,6,8,0],[[2,4,64],[4,2,112],[4,4,464]]",3101));//R8R4_102=R8_10×R4_3	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,368]],[[8,12],[16,16],[32,4]]",3102));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,28,8,3,3,192,27,32,[1,4,15,12,0,0],[[2,2,16],[2,4,132],[4,2,132],[4,4,552]]",3103));//R8R4_146=R8_14×R4_3
	/*---------------end of R32_3044~R32_3103----------------*/	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,0,24,0,0],[[4,4,576]],[[8,24],[32,8]]",3104));//R8_18:[[2,3],[1,4]],[[2,5],[3,0]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,512,31,32,[1,7,8,16,0,0],[[4,4,512]]",3105));//R16_95×R2_1=R32_3105=R8_18:[[2,3],[1,4]],[[2,6],[2,2]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,16,8,0,0],[[4,4,576]]",3106));//R8_18:[[2,3],[1,4]],[[3,1],[3,1]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,7,16,8,0,0],[[4,2,64],[4,4,192]]",3107));//R8_10:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,7,16,8,0,0],[[4,2,64],[4,4,320]]",3108));//R8_10:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,7,20,4,0,0],[[4,2,64],[4,4,320]]",3109));//R8_10:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,7,16,8,0,0],[[4,2,96],[4,4,288]]",3110));//R8_10:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,7,12,12,0,0],[[4,2,64],[4,4,320]]",3111));//R8_10:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,7,24,0,0,0],[[4,2,96],[4,4,288]]",3112));//R8_10:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,7,12,12,0,0],[[4,2,96],[4,4,288]]",3113));//R8_10:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,12,12,0,0],[[4,2,64],[4,4,320]]",3114));//R8_10:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,12,12,0,0],[[4,2,64],[4,4,448]]",3115));//R8_10:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,20,4,0,0],[[4,2,64],[4,4,384]]",3116));//R8_10:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,7,20,4,0,0],[[4,2,64],[4,4,320]]",3117));//R8_10:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,4,20,0,0],[[4,2,64],[4,4,448]]",3118));//R8_10:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,15,512,31,8,[1,7,8,0,16,0],[[2,4,64],[4,2,64],[4,4,384]]",3119));//R4_3:[[1,1,0],[1,0,1],[1,0,1]],[[0,0,0],[0,2,2],[0,2,2]]		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,9,11,15,288,31,1,[1,7,24,0,0,0],[[2,4,128],[4,2,96],[4,4,512]]",3120));//R8_14:[[0,1],[2,7]],[[0,1],[6,0]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,10,5,7,264,23,2,[1,4,23,4,0,0],[[2,2,16],[2,4,120],[4,2,120],[4,4,504]]",3121));//R16_101×R2_2=R32_3121=R8_14:[[0,1],[2,7]],[[0,1],[6,1]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,7,20,4,0,0],[[2,4,64],[4,2,96],[4,4,352]]",3122));//R8_11:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]]",3123));//R16_140:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,7,16,8,0,0],[[2,4,64],[4,4,192]]",3124));//R8_11:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,640,31,32,[1,7,8,16,0,0],[[4,4,384]]",3125));//R16_84×R2_1=R32_3125=R16_127:[[0,2],[0,8]],[[0,2],[1,2]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,31,31,640,31,32,[1,7,24,0,0,0],[[4,4,384]]",3126));//R16_100×R2_1=R32_3126=R16_127:[[0,2],[0,8]],[[0,2],[1,5]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,7,24,0,0,0],[[4,4,448]]",3127));//R16_86×R2_1=R32_3127=R16_127:[[0,2],[0,8]],[[0,2],[2,3]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,7,0,24,0,0],[[4,4,448]]",3128));//R16_79×R2_1=R32_3128=R8_9:[[0,6],[3,2]],[[0,6],[6,1]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,24,0,0,0],[[4,4,384]]",3129));//R16_108×R2_1=R32_3129=R4_2:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,10,5,7,264,31,2,[1,4,23,4,0,0],[[2,2,16],[2,4,120],[4,2,120],[4,4,504]]",3130));//R16_102×R2_2=R32_3130=R8_14:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,7,15,208,15,8,[1,7,12,4,8,0],[[2,4,128],[4,2,144],[4,4,544]]",3131));//R4_3:[[0,0,0,0],[0,0,0,0],[0,2,1,3],[0,1,0,3]],[[0,0,0,0],[0,0,0,0],[0,2,2,2],[0,2,0,2]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,8,0,16,0],[[2,4,64],[4,2,64],[4,4,384]]",3132));//R4_3:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,7,7,176,15,8,[1,7,4,12,8,0],[[2,4,128],[4,2,144],[4,4,576]]",3133));//R4_3:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,448,31,32,[1,7,24,0,0,0],[[4,4,576]]",3134));//R16_70×R2_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,3,15,336,31,32,[1,4,15,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,576]],[[4,12],[8,12],[16,4],[32,4]]",3135));//R16_70×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,640,31,32,[1,7,16,8,0,0],[[4,4,384]]",3136));//R16_71×R2_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,480,31,32,[1,4,11,12,4,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3137));//R16_71×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,11,15,432,31,8,[1,4,11,12,4,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,480]]",3138));//R16_72×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,640,31,8,[1,7,8,16,0,0],[[4,4,384]]",3139));//R16_73×R2_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,3,15,480,31,8,[1,4,7,12,8,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]],[[8,12],[16,16],[32,4]]",3140));//R16_73×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,3,15,384,31,8,[1,4,7,12,8,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,528]],[[4,8],[8,12],[16,8],[32,4]]",3141));//R16_74×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,3,15,432,31,32,[1,4,7,12,8,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,480]]",3142));//R16_75×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,3,15,432,31,8,[1,4,7,12,8,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,480]],[[4,4],[8,12],[16,12],[32,4]]",3143));//R16_76×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,7,15,432,31,8,[1,4,7,12,8,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,480]]",3144));//R16_77×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,3,15,432,31,8,[1,4,3,12,12,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,480]]",3145));//R16_78×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,3,15,432,31,32,[1,4,3,12,12,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,480]]",3146));//R16_79×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,3,15,384,31,8,[1,4,3,12,12,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,528]],[[4,8],[8,12],[16,8],[32,4]]",3147));//R16_80×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,4,11,12,4,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3148));//R16_81×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,11,15,576,31,8,[1,4,11,12,4,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]]",3149));//R16_82×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,7,24,0,0,0],[[4,4,256]]",3150));//R16_83×R2_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,11,15,576,31,8,[1,4,15,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]]",3151));//R16_83×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,480,31,32,[1,4,7,12,8,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3152));//R16_84×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,4,7,12,8,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3153));//R16_85×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,11,15,432,31,8,[1,4,15,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,480]],[[4,4],[8,12],[16,12],[32,4]]",3154));//R16_86×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,7,16,8,0,0],[[4,4,448]]",3155));//R16_87×R2_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,3,15,432,31,8,[1,4,11,12,4,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,480]]",3156));//R16_87×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,7,24,0,0,0],[[4,4,448]]",3157));//R16_88×R2_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,7,15,432,31,8,[1,4,15,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,480]],[[4,4],[8,12],[16,12],[32,4]]",3158));//R16_88×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,7,15,432,31,8,[1,4,11,12,4,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,480]]",3159));//R16_89×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,3,15,384,31,8,[1,4,11,12,4,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,528]]",3160));//R16_90×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,3,15,336,31,32,[1,4,3,12,12,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,576]],[[4,12],[8,12],[16,4],[32,4]]",3161));//R16_93×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,3,15,336,31,8,[1,4,7,12,8,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,576]],[[4,12],[8,12],[16,4],[32,4]]",3162));//R16_94×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,4,7,12,8,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,528]]",3163));//R16_95×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,24,4,3,3,144,23,32,[1,4,7,8,12,0],[[2,2,16],[2,4,144],[4,2,144],[4,4,576]]",3164));//R16_105×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,20,4,3,3,120,19,32,[1,4,7,4,8,8],[[2,2,16],[2,4,156],[4,2,156],[4,4,576]],[[1,12],[2,12],[4,3],[8,3],[16,1],[32,1]]",3165));//R16_106×R2_2=R8_14:[[0,2],[3,7]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,3,7,240,31,32,[1,4,7,4,8,8],[[2,2,16],[2,4,96],[4,2,96],[4,4,576]]",3166));//R16_109×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,24,4,7,7,168,23,32,[1,4,11,8,8,0],[[2,2,16],[2,4,144],[4,2,144],[4,4,552]]",3167));//R16_110×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,512,31,32,[1,7,8,0,16,0],[[2,4,64],[4,2,64],[4,4,384]]",3168));//R16_111×R2_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,4,7,4,8,8],[[2,2,16],[2,4,96],[4,2,96],[4,4,432]]",3169));//R16_111×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,15,512,31,32,[1,7,8,0,16,0],[[2,4,64],[4,2,64],[4,4,384]]",3170));//R16_91×R2_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,7,384,31,32,[1,4,7,4,8,8],[[2,2,16],[2,4,96],[4,2,96],[4,4,432]]",3171));//R16_91×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,384,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3172));//R16_140:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,24,4,3,7,168,23,32,[1,4,11,8,8,0],[[2,2,16],[2,4,144],[4,2,144],[4,4,552]]",3173));//R16_92×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,576,31,32,[1,4,7,12,8,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]]",3174));//R16_97×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,512,31,32,[1,7,16,8,0,0],[[4,4,512]]",3175));//R16_98×R2_1
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,480,31,32,[1,4,15,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3176));//R16_100×R2_2
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,7,7,176,31,8,[1,7,4,12,8,0],[[2,4,144],[4,2,128],[4,4,576]]",3177));//R8_14:[[1,2],[3,4]],[[1,6],[6,4]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,16,2,7,7,160,15,32,[1,7,2,14,8,0],[[2,4,144],[4,2,144],[4,4,576]]",3178));//R8_14:[[1,2],[3,5]],[[1,6],[6,5]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,9,11,15,288,31,1,[1,7,24,0,0,0],[[2,4,96],[4,2,128],[4,4,512]]",3179));//R8_14:[[1,2],[1,2]],[[1,3],[1,1]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,240]]",3180));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,7,20,4,0,0],[[2,4,96],[4,2,64],[4,4,352]]",3181));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,384]]",3182));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]]",3183));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]]",3184));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,7,12,12,0,0],[[4,2,64],[4,4,384]]",3185));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]]",3186));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,416]]",3187));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,304]]",3188));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,304]]",3189));//R16_140:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]]",3190));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,400]]",3191));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,240]]",3192));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3193));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]]",3194));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,7,8,16,0,0],[[4,4,256]]",3195));//R16_97×R2_1=R32_3195
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,448,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,464]]",3196));//R16_140:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,7,12,12,0,0],[[2,4,64],[4,2,96],[4,4,384]]",3197));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,512,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]]",3198));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,8,16,0,0],[[2,4,96],[4,2,96],[4,4,416]]",3199));//R16_140:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,4,20,0,0],[[4,2,64],[4,4,448]]",3200));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,16,8,0,0],[[4,2,64],[4,4,512]]",3201));//R16_140:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,12,12,0,0],[[2,4,64],[4,2,96],[4,4,416]]",3202));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,464]]",3203));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]]",3204));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,416,31,32,[1,7,16,8,0,0],[[2,4,96],[4,2,96],[4,4,416]]",3205));//R16_140:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,464]]",3206));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]]",3207));//R16_140:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,304]]",3208));//R16_140:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,240]]",3209));//R16_140:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]]",3210));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]]",3211));//R16_141:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,464]]",3212));//R16_141:[[0,0],[0,1]],[[0,0],[0,2]],[[0,0],[1,3]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,464]]",3213));//R16_141:[[0,0],[0,1]],[[0,0],[0,3]],[[0,0],[3,2]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3214));//R16_141:[[0,0],[0,1]],[[0,0],[0,2]],[[0,0],[3,3]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3215));//R16_141:[[0,0],[0,1]],[[0,0],[0,3]],[[0,0],[5,2]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,400]]",3216));//R16_141:[[0,0],[0,1]],[[0,0],[0,2]],[[0,0],[7,3]]		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,304]]",3217));//R16_141:[[0,0],[0,1]],[[0,0],[0,2]],[[0,3],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]]",3218));//R16_141:[[0,0],[0,1]],[[0,0],[0,3]],[[0,1],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,24,0,0,0],[[4,4,512]]",3219));//R16_141:[[0,5],[0,3]],[[0,5],[1,0]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]]",3220));//R16_141:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,5,11,15,352,31,2,[1,7,16,8,0,0],[[2,4,96],[4,2,96],[4,4,480]]",3221));//R16_102×R2_1=R32_3221	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]]",3222));//R16_141:[[0,0],[0,1]],[[0,0],[0,2]],[[0,5],[0,3]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,400]]",3223));//R16_141:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3224));//R16_141:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,480,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3225));//R16_141:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,400]]",3226));//R16_141:[[0,0],[0,1]],[[0,0],[0,2]],[[0,3],[0,3]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,464]]",3227));//R16_141:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,400]]",3228));//R16_141:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,416]],[[8,16],[16,12],[32,4]]",3229));//R16_141:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,384]]",3230));//R16_141:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,416]]",3231));//R16_142:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]]",3232));//R16_142:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,480,31,8,[1,7,20,4,0,0],[[2,4,96],[4,2,64],[4,4,384]]",3233));//R16_142:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3234));//R16_142:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,7,20,4,0,0],[[2,4,96],[4,2,64],[4,4,384]]",3235));//R16_142:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,416]]",3236));//R16_142:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,240]]",3237));//R16_143:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]]",3238));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,320]]",3239));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]]",3240));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]]",3241));//R16_143:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,384]]",3242));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]]",3243));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,256]]",3244));//R16_143:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,304]]",3245));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]]",3246));//R16_143:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,272]]",3247));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]],[[8,8],[16,16],[32,8]]",3248));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,240]]",3249));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]],[[8,12],[16,16],[32,4]]",3250));//R16_176:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,7,8,16,0,0],[[4,4,448]]",3251));//R16_75×R2_1=R32_3251	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,320]]",3252));//R16_143:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,7,16,8,0,0],[[4,2,64],[4,4,448]]",3253));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]]",3254));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,384]]]",3255));//R16_143:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,304]]",3256));//R16_143:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,12,12,0,0],[[4,2,64],[4,4,384]]",3257));//R16_143:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,512,31,32,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,384]]",3258));//R16_143:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,384]]",3259));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]]",3260));//R16_143:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,4,20,0,0],[[2,4,96],[4,2,64],[4,4,416]]",3261));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,304]]",3262));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,7,8,16,0,0],[[2,4,96],[4,2,64],[4,4,416]]",3263));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]]",3264));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,320]]",3265));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,16,8,0,0],[[2,4,96],[4,4,352]]",3266));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,7,16,8,0,0],[[2,4,96],[4,2,64],[4,4,384]]",3267));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,416]]",3268));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]]",3269));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,12,12,0,0],[[4,2,64],[4,4,512]]",3270));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,12,12,0,0],[[4,2,64],[4,4,384]]",3271));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,16,8,0,0],[[4,2,64],[4,4,448]]",3272));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,7,24,0,0,0],[[2,4,96],[4,4,288]]",3273));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,304]]",3274));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]]",3275));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]]",3276));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,20,4,0,0],[[2,4,64],[4,2,64],[4,4,448]]",3277));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,7,16,8,0,0],[[2,4,96],[4,2,64],[4,4,416]]",3278));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,16,8,0,0],[[2,4,96],[4,2,64],[4,4,352]]",3279));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,448]]",3280));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,16,8,0,0],[[2,4,96],[4,2,64],[4,4,384]]",3281));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,96],[4,4,448]]",3282));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]]",3283));//R16_139:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]]",3284));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,96],[4,4,416]]",3285));//R16_139:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,24,0,0,0],[[2,4,64],[4,2,64],[4,4,320]]",3286));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,16,8,0,0],[[2,4,96],[4,2,64],[4,4,416]]",3287));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,16,8,0,0],[[2,4,96],[4,4,416]]",3288));//R16_127:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,20,4,0,0],[[2,4,64],[4,4,384]]",3289));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,16,8,0,0],[[2,4,64],[4,4,448]]",3290));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,12,12,0,0],[[2,4,96],[4,4,480]]",3291));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,16,8,0,0],[[2,4,96],[4,4,416]]",3292));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,16,8,0,0],[[2,4,64],[4,4,384]]",3293));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]]",3294));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]]",3295));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,12,12,0,0],[[2,4,64],[4,4,448]]",3296));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,12,12,0,0],[[2,4,64],[4,4,320]]",3297));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,7,16,8,0,0],[[2,4,64],[4,4,320]]",3298));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,20,4,0,0],[[2,4,64],[4,4,448]]",3299));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,7,20,4,0,0],[[2,4,64],[4,4,320]]",3300));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,20,4,0,0],[[2,4,64],[4,2,64],[4,4,416]]",3301));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,7,20,4,0,0],[[2,4,64],[4,2,64],[4,4,448]]",3302));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,7,20,4,0,0],[[2,4,64],[4,2,64],[4,4,416]]",3303));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,7,24,0,0,0],[[2,4,64],[4,2,64],[4,4,416]]",3304));//R16_145:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,8,16,0,0],[[2,4,96],[4,4,416]]",3305));//R16_156:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,16,8,0,0],[[4,2,64],[4,4,320]]",3306));//R16_28:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,384]]",3307));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,12,12,0,0],[[4,2,64],[4,4,448]]",3308));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,7,4,20,0,0],[[4,2,64],[4,4,384]]",3309));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]]",3310));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]]",3311));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,496]]",3312));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,384,31,32,[1,7,8,16,0,0],[[2,4,96],[4,2,96],[4,4,448]]",3313));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,448]]",3314));//R16_140:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]]",3315));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]]",3316));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,20,4,0,0],[[2,4,96],[4,2,64],[4,4,352]]",3317));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,240]]",3318));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]]",3319));//R16_140:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,7,16,8,0,0],[[2,4,96],[4,2,96],[4,4,416]]",3320));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]]",3321));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,496]]",3322));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,304]]",3323));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,272]]",3324));//R16_140:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]]",3325));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]]",3326));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,7,8,16,0,0],[[2,4,96],[4,2,96],[4,4,448]]",3327));//R16_140:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,480]]",3328));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,496]]",3329));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]]",3330));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]]",3331));//R16_140:			
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]]",3332));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,464]]",3333));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]]",3334));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]]",3335));//R16_146:			
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,448,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,464]]",3336));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]]",3337));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,416]]",3338));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]]",3339));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,3,28,0,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]]",3340));//R16_145:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,24,0,0,0],[[2,4,96],[4,2,64],[4,4,352]]",3341));//R16_144:			
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,24,0,0,0],[[2,4,64],[4,2,64],[4,4,384]]",3342));//R16_144:[[0,0],[0,1]],[[0,0],[0,2]],[[0,0],[1,3]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]]",3343));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,7,20,4,0,0],[[4,2,64],[4,4,448]]",3344));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,20,4,0,0],[[4,2,64],[4,4,448]]",3345));//R16_145:			
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]]",3346));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,384,31,4,[1,7,16,8,0,0],[[2,4,96],[4,2,64],[4,4,480]]",3347));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,304]]",3348));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]]",3349));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]]",3350));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]]",3351));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,384]]",3352));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,3,8,20,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]]",3353));//R16_146:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]]",3354));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]]",3355));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,464]]",3356));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]]",3357));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,416]]",3358));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,464]]",3359));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,20,4,0,0],[[4,2,64],[4,4,512]]",3360));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,7,8,16,0,0],[[4,2,64],[4,4,448]]",3361));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,528]]",3362));//R16_146:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,12,12,0,0],[[4,2,64],[4,4,512]]",3363));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,480]]",3364));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]]",3365));//R16_145:			
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,480]]",3366));//R16_145:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,240]]",3367));//R16_144:			
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,320]]",3368));//R16_144:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,20,4,0,0],[[2,4,64],[4,2,64],[4,4,384]]",3369));//R16_144:			
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,20,4,0,0],[[4,2,64],[4,4,384]]",3370));//R16_144:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,7,20,4,0,0],[[2,4,64],[4,2,64],[4,4,320]]",3371));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,304]]",3372));//R16_144:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]]",3373));//R16_144:			
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]]",3374));//R16_144:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,20,4,0,0],[[2,4,64],[4,2,64],[4,4,320]]",3375));//R16_144:[[0,0],[0,1]],[[0,0],[0,2]],[[0,0],[3,3]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]]",3376));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,12,12,0,0],[[4,2,64],[4,4,512]]",3377));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]]",3378));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]]",3379));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,8,16,0,0],[[2,4,96],[4,2,64],[4,4,448]]",3380));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]]",3381));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]]",3382));//R16_144:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,272]]",3383));//R16_144:			
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]]",3384));//R16_144:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]]",3385));//R16_144:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,384]]",3386));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]]",3387));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,480]]",3388));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,16,8,0,0],[[4,2,64],[4,4,512]]",3389));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]]",3390));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,480]]",3391));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,384]]",3392));//R16_144:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,16,8,0,0],[[4,2,64],[4,4,448]]",3393));//R16_144:			
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,20,4,0,0],[[2,4,96],[4,2,64],[4,4,416]]",3394));//R16_144:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,20,4,0,0],[[2,4,64],[4,2,64],[4,4,384]]",3395));//R16_144:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,7,20,4,0,0],[[2,4,96],[4,2,64],[4,4,416]]",3396));//R16_144:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,304]]",3397));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]]",3398));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]]",3399));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,7,12,12,0,0],[[4,2,64],[4,4,448]]",3400));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]]",3401));//R16_144:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]]",3402));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3403));//R16_125:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,272]]",3404));//R16_172:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,256]]",3405));//R16_272:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,272]]",3406));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,272]]",3407));//R16_172:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,8,16,0,0],[[2,4,96],[4,2,64],[4,4,352]]",3408));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,7,8,16,0,0],[[4,2,64],[4,4,384]]",3409));//R16_172:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,4,20,0,0],[[4,2,64],[4,4,448]]",3410));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]]",3411));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,464]]",3412));//R16_125:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,400]]",3413));//R16_125:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3414));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,240]]",3415));//R8_6:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]]",3416));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]]",3417));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,16,8,0,0],[[2,4,96],[4,2,64],[4,4,352]]",3418));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,272]]",3419));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]]",3420));//R16_172:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]]",3421));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]]",3422));//R16_125:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,464]]",3423));//R16_125:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,8,20,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,496]]",3424));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]]",3425));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,352]]",3426));//R16_272:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]]",3427));//R16_172:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,304]]",3428));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,464]]",3429));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,304]]",3430));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,16,8,0,0],[[4,2,96],[4,4,416]]",3431));//R16_272:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]]",3432));//R16_172:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]]",3433));//R16_125:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,272]]",3434));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,8,16,0,0],[[2,4,96],[4,2,64],[4,4,352]]",3435));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,304]]",3436));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]]",3437));//R16_146:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,480,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,400]]",3438));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]]",3439));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,496]]",3440));//R16_146:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]]",3441));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3442));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]]",3443));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,496]]",3444));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,12,12,0,0],[[2,4,64],[4,4,448]]",3445));//R8_8:[[0,0],[0,1]],[[0,0],[0,2]],[[0,4],[2,0]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,20,4,0,0],[[2,4,64],[4,4,384]]",3446));//R8_8:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,12,12,0,0],[[2,4,64],[4,4,512]]",3447));//R8_8:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,12,12,0,0],[[2,4,64],[4,4,448]]",3448));//R8_8:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,4,20,0,0],[[2,4,64],[4,4,512]]",3449));//R8_8:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,400]]",3450));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,464]]",3451));//R16_125:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,384]]",3452));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,416]]",3453));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]]",3454));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]]",3455));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,480,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,400]]",3456));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,7,20,4,0,0],[[2,4,64],[4,4,320]]",3457));//R8_8:[[0,0],[0,1]],[[0,0],[0,2]],[[0,4],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,7,16,8,0,0],[[2,4,64],[4,4,320]]",3458));//R8_8:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,7,12,12,0,0],[[2,4,64],[4,4,320]]",3459));//R8_8:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,7,4,20,0,0],[[2,4,64],[4,4,448]]",3460));//R8_8:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,7,8,16,0,0],[[2,4,64],[4,4,384]]",3461));//R8_8:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,4,20,0,0],[[2,4,64],[4,4,448]]",3462));//R8_8:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,7,12,12,0,0],[[2,4,64],[4,4,448]]",3463));//R8_8:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,8,16,0,0],[[2,4,64],[4,4,384]]",3464));//R8_8:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,4,20,0,0],[[2,4,64],[4,4,448]]",3465));//R8_8:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,7,20,4,0,0],[[2,4,64],[4,2,64],[4,4,256]]",3466));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,24,0,0,0],[[2,4,64],[4,2,96],[4,4,352]]",3467));//R16_144:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,96],[4,4,352]]",3468));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,20,4,0,0],[[2,4,64],[4,2,64],[4,4,384]]",3469));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,16,8,0,0],[[2,4,64],[4,4,320]]",3470));//R8_8:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,9,11,15,336,15,1,[1,7,24,0,0,0],[[2,4,96],[4,2,112],[4,4,480]]",3471));//R4_3:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,464]]",3472));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]]",3473));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]]",3474));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,400]]",3475));//R16_125:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3476));//R16_125:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,464]]",3477));//R16_146:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]]",3478));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,272]]",3479));//R8_9:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]]",3480));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,12,12,0,0],[[2,4,64],[4,4,384]]",3481));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,496]]",3482));//R16_125:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]]",3483));//R16_125:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,496]]",3484));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]]",3485));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]]",3486));//R16_146:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]]",3487));//R16_146:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]]",3488));//R16_146:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,496]]",3489));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]]",3490));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,496]]",3491));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]]",3492));//R16_125:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,448,31,32,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]]",3493));//R16_132:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]]",3494));//R16_132:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,304]]",3495));//R16_132:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]]",3496));//R16_146:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]]",3497));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,448,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,464]]",3498));//R16_146:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]]",3499));//R16_146:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,496]]",3500));//R16_146:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,528]]",3501));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,496]]",3502));//R16_146:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,496]]",3503));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,416]]",3504));//R16_146:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]]",3505));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,400]]",3506));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,8,16,0,0],[[2,4,96],[4,2,64],[4,4,416]]",3507));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]]",3508));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]]",3509));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]]",3510));//R16_176:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]]",3511));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,8,16,0,0],[[4,2,64],[4,4,448]]",3512));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,12,12,0,0],[[4,2,64],[4,4,448]]",3513));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,448]]",3514));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,496]]",3515));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]]",3516));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]]",3517));//R16_176:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,320]]",3518));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,3,28,0,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,272]]",3519));//R16_127:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]]",3520));//R16_127:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,272]]",3521));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,304]]",3522));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]]",3523));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,16,8,0,0],[[4,2,96],[4,4,352]]",3524));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,4,20,0,0],[[4,2,64],[4,4,512]]",3525));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]]",3526));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]]",3527));//R16_126:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,12,12,0,0],[[4,2,96],[4,4,480]]",3528));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,384]]",3529));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,16,8,0,0],[[4,2,64],[4,4,448]]",3530));//R16_126:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,416]]",3531));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,400]]",3532));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,384]]",3533));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,400]]",3534));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,400]]",3535));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,12,12,0,0],[[4,2,96],[4,4,416]]",3536));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,8,16,0,0],[[4,2,96],[4,4,480]]",3537));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,20,4,0,0],[[2,4,96],[4,2,64],[4,4,352]]",3538));//R16_144:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,416]]",3539));//R16_144:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,352]]",3540));//R16_144:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,384]]",3541));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,96],[4,4,352]]",3542));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,384]]",3543));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,16,8,0,0],[[4,2,96],[4,4,352]]",3544));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,448]]",3545));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,416,31,32,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,480]]",3546));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,480,31,8,[1,7,24,0,0,0],[[2,4,64],[4,2,64],[4,4,416]]",3547));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,416]]",3548));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,7,20,4,0,0],[[2,4,64],[4,2,64],[4,4,416]]",3549));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,20,4,0,0],[[4,2,64],[4,4,448]]",3550));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]]",3551));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]]",3552));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]]",3553));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,496]],[[8,24],[16,4],[32,4]]",3554));//R16_182:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,448,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]],[[8,16],[16,12],[32,4]]",3555));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,448,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]],[[8,16],[16,12],[32,4]]",3556));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,400]],[[8,12],[16,16],[32,4]]",3557));//R16_182:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,448,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]],[[8,16],[16,12],[32,4]]",3558));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,400]],[[8,12],[16,16],[32,4]]",3559));//R16_125:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,272]],[[8,8],[16,16],[32,8]]",3560));//R16_131:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,512,31,32,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,384]],[[8,16],[16,8],[32,8]]",3561));//R16_131:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,12,12,0,0],[[2,4,64],[4,4,512]],[[8,16],[16,12],[32,4]]",3562));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]],[[8,20],[16,8],[32,4]]",3563));//R16_125:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,8,16,0,0],[[2,4,64],[4,4,448]],[[8,8],[16,20],[32,4]]",3564));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,96],[4,4,384]],[[8,12],[16,16],[32,4]]",3565));//R16_139:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,16,8,0,0],[[2,4,96],[4,2,96],[4,4,416]],[[8,20],[16,8],[32,4]]",3566));//R16_139:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3567));//R16_139:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,20,4,0,0],[[2,4,64],[4,2,96],[4,4,384]],[[8,12],[16,16],[32,4]]",3568));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,9,11,15,336,31,1,[1,7,24,0,0,0],[[2,4,112],[4,2,96],[4,4,480]],[[8,24],[16,7],[32,1]]",3569));//R16_169:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,20,4,0,0],[[2,4,96],[4,2,64],[4,4,352]],[[8,16],[16,8],[32,8]]",3570));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,96],[4,4,352]],[[8,8],[16,20],[32,4]]",3571));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,7,20,4,0,0],[[2,4,64],[4,2,96],[4,4,416]],[[8,16],[16,12],[32,4]]",3572));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,384]],[[8,8],[16,20],[32,4]]",3573));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3574));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]],[[8,16],[16,12],[32,4]]",3575));//R8_12:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,640,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,272]],[[16,24],[32,8]]",3576));//R16_133:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]],[[8,16],[16,8],[32,8]]",3577));//R16_133:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]],[[8,8],[16,16],[32,8]]",3578));//R16_133:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]],[[8,16],[16,8],[32,8]]",3579));//R16_171:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,272]],[[8,8],[16,16],[32,8]]",3580));//R16_171:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,256]],[[16,24],[32,8]]",3581));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,8,16,0,0],[[4,2,96],[4,4,416]],[[8,16],[16,8],[32,8]]",3582));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]],[[8,16],[16,8],[32,8]]",3583));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]],[[8,16],[16,8],[32,8]]",3584));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,96],[4,4,352]],[[8,8],[16,20],[32,4]]",3585));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]],[[8,16],[16,8],[32,8]]",3586));//R16_173:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,24],[32,8]]",3587));//R16_173:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,400]],[[8,12],[16,16],[32,4]]",3588));//R16_173:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]],[[8,16],[16,8],[32,8]]",3589));//R16_173:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3590));//R16_173:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3591));//R16_173:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3592));//R16_173:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,480]],[[8,20],[16,8],[32,4]]",3593));//R16_173:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,496]],[[8,20],[16,8],[32,4]]",3594));//R16_173:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,384,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3595));//R16_173:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,496]],[[8,24],[16,4],[32,4]]",3596));//R16_173:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,400]],[[8,12],[16,16],[32,4]]",3597));//R16_173:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]],[[8,16],[16,12],[32,4]]",3598));//R16_173:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]],[[8,20],[16,8],[32,4]]",3599));//R16_173:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,8,16,0,0],[[2,4,96],[4,2,64],[4,4,448]],[[8,20],[16,8],[32,4]]",3600));//R16_173:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,384,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3601));//R16_173:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,384,31,32,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3602));//R16_173:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,3,28,0,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,144]],[[16,16],[32,16]]",3603));//R16_130:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,496]],[[8,24],[16,4],[32,4]]",3604));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]],[[8,16],[16,12],[32,4]]",3605));//R8_18:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]],[[8,12],[16,16],[32,4]]",3606));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,416]],[[8,12],[16,16],[32,4]]",3607));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3608));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,416]],[[8,12],[16,16],[32,4]]",3609));//R16_176:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,8,16,0,0],[[2,4,64],[4,2,96],[4,4,416]],[[8,16],[16,12],[32,4]]",3610));//R16_176:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,96],[4,4,416]],[[8,16],[16,12],[32,4]]",3611));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,416]],[[8,12],[16,16],[32,4]]",3612));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]],[[8,16],[16,12],[32,4]]",3613));//R16_176:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,464]],[[8,16],[16,12],[32,4]]",3614));//R16_176:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,464]],[[8,16],[16,12],[32,4]]",3615));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]],[[8,16],[16,12],[32,4]]",3616));//R16_176:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3617));//R16_176:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3618));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,464]],[[8,20],[16,8],[32,4]]",3619));//R16_176:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3620));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]],[[8,16],[16,8],[32,8]]",3621));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,304]],[[8,8],[16,16],[32,8]]",3622));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]],[[8,16],[16,8],[32,8]]",3623));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,496]],[[8,24],[16,4],[32,4]]",3624));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,480]],[[8,20],[16,8],[32,4]]",3625));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,464]],[[8,20],[16,8],[32,4]]",3626));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,15,384,31,8,[1,7,8,0,16,0],[[2,4,64],[4,2,64],[4,4,512]],[[8,24],[16,4],[32,4]]",3627));//R16_136:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,15,384,31,8,[1,7,8,8,8,0],[[2,4,64],[4,2,64],[4,4,512]],[[4,8],[8,12],[16,8],[32,4]]",3628));//R16_121:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,15,384,31,8,[1,7,8,0,16,0],[[2,4,64],[4,2,64],[4,4,512]],[[4,16],[16,12],[32,4]]",3629));//R16_121:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,15,448,31,8,[1,7,8,8,8,0],[[2,4,64],[4,2,64],[4,4,448]],[[4,16],[16,8],[32,8]]",3630));//R16_121:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,496]],[[8,24],[16,4],[32,4]]",3631));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]],[[8,20],[16,8],[32,4]]",3632));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,304]],[[8,8],[16,16],[32,8]]",3633));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,16,8,0,0],[[4,2,64],[4,4,384]],[[8,8],[16,16],[32,8]]",3634));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,8,16,0,0],[[4,2,96],[4,4,352]],[[8,8],[16,16],[32,8]]",3635));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]],[[8,8],[16,16],[32,8]]",3636));//R16_133:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]",3637));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,496]],[[8,24],[16,4],[32,4]]",3638));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,8,16,0,0],[[4,2,64],[4,4,384]],[[8,8],[16,16],[32,8]]",3639));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,240]],[[16,24],[32,8]]",3640));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3641));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,480]],[[8,20],[16,8],[32,4]]",3642));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,7,4,20,0,0],[[2,4,64],[4,2,64],[4,4,512]],[[8,24],[16,4],[32,4]]",3643));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,7,8,16,0,0],[[2,4,96],[4,2,64],[4,4,480]],[[8,24],[16,4],[32,4]]",3644));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,7,8,16,0,0],[[2,4,96],[4,2,64],[4,4,480]],[[8,24],[16,4],[32,4]]",3645));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,15,512,31,8,[1,7,8,8,8,0],[[2,4,64],[4,2,64],[4,4,384]],[[8,16],[16,8],[32,8]]",3646));//R16_120:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,15,448,31,8,[1,7,8,0,16,0],[[2,4,64],[4,2,64],[4,4,448]],[[4,16],[16,8],[32,8]]",3647));//R16_120:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,15,448,31,8,[1,7,8,8,8,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3648));//R16_120:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]],[[8,16],[16,8],[32,8]]",3649));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]],[[8,16],[16,12],[32,4]]",3650));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,400]],[[8,12],[16,16],[32,4]]",3651));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]],[[16,28],[32,4]]",3652));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,480,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]],[[8,12],[16,16],[32,4]]",3653));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]],[[16,28],[32,4]]",3654));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3655));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,12,12,0,0],[[4,2,96],[4,4,352]],[[8,8],[16,16],[32,8]]",3656));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]],[[8,24],[32,8]]",3657));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]],[[8,16],[16,8],[32,8]]",3658));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,464]],[[8,16],[16,12],[32,4]]",3659));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,7,4,20,0,0],[[4,2,64],[4,4,448]],[[8,16],[16,8],[32,8]]",3660));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,12,12,0,0],[[4,2,96],[4,4,480]],[[8,24],[32,8]]",3661));//R16_146:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,7,4,20,0,0],[[2,4,64],[4,2,96],[4,4,384]],[[8,12],[16,16],[32,4]]",3662));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,7,20,4,0,0],[[2,4,64],[4,2,96],[4,4,384]],[[8,12],[16,16],[32,4]]",3663));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,96],[4,4,448]],[[8,20],[16,8],[32,4]]",3664));//R16_172:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,96],[4,4,448]],[[8,20],[16,8],[32,4]]",3665));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,20,4,0,0],[[2,4,64],[4,2,96],[4,4,352]],[[8,8],[16,20],[32,4]]",3666));//R16_172:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,576,31,32,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,320]],[[16,28],[32,4]]",3667));//R16_172:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,20,4,0,0],[[2,4,64],[4,2,96],[4,4,352]],[[8,8],[16,20],[32,4]]",3668));//R16_172:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,7,20,4,0,0],[[2,4,96],[4,2,64],[4,4,384]],[[8,12],[16,16],[32,4]]",3669));//R16_172:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,480,31,32,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,416]],[[8,12],[16,16],[32,4]]",3670));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,640,31,32,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,256]],[[16,24],[32,8]]",3671));//R16_132:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,320]],[[8,8],[16,16],[32,8]]",3672));//R16_132:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,16,8,0,0],[[2,4,64],[4,4,448]],[[8,16],[16,8],[32,8]]",3673));//R16_134:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,16,8,0,0],[[2,4,64],[4,4,448]],[[8,8],[16,20],[32,4]]",3674));//R16_134:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]],[[8,16],[16,8],[32,8]]",3675));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,12,12,0,0],[[4,2,96],[4,4,416]],[[8,16],[16,8],[32,8]]",3676));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]],[[8,16],[16,8],[32,8]]",3677));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,16,8,0,0],[[4,2,96],[4,4,416]],[[8,16],[16,8],[32,8]]",3678));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]],[[8,16],[16,12],[32,4]]",3679));//R16_172:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,96],[4,4,416]],[[8,16],[16,12],[32,4]]",3680));//R16_172:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]],[[8,16],[16,12],[32,4]]",3681));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,16],[16,12],[32,4]]",3682));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,16],[16,12],[32,4]]",3683));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]],[[8,16],[16,12],[32,4]]",3684));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,480]],[[8,24],[16,4],[32,4]]",3685));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,480]],[[8,20],[16,8],[32,4]]",3686));//R16_172:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,7,4,20,0,0],[[2,4,96],[4,2,64],[4,4,480]],[[8,24],[16,4],[32,4]]",3687));//R16_172:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,496]],[[8,20],[16,8],[32,4]]",3688));//R16_172:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,8,16,0,0],[[4,2,64],[4,4,512]],[[8,24],[32,8]]",3689));//R16_176:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,384,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3690));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,7,16,8,0,0],[[4,2,64],[4,4,320]],[[16,24],[32,8]]",3691));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,12,12,0,0],[[4,2,96],[4,4,416]],[[8,16],[16,8],[32,8]]",3692));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,8,16,0,0],[[4,2,96],[4,4,416]],[[8,16],[16,8],[32,8]]",3693));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3694));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,416,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3695));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,448]],[[8,20],[16,8],[32,4]]",3696));//R16_176:		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,384]],[[8,16],[16,8],[32,8]]",3697));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]],[[8,24],[32,8]]",3698));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3699));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,7,24,0,0,0],[[2,4,64],[4,2,64],[4,4,256]],[[16,24],[32,8]]",3700));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,576,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]],[[16,28],[32,4]]",3701));//R16_156:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]],[[8,8],[16,20],[32,4]]",3702));//R16_156:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]],[[8,16],[16,12],[32,4]]",3703));//R16_156:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,496]],[[8,20],[16,8],[32,4]]",3704));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,496]],[[8,24],[16,4],[32,4]]",3705));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,96],[4,4,448]],[[8,20],[16,8],[32,4]]",3706));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,12,12,0,0],[[2,4,64],[4,2,96],[4,4,416]],[[8,16],[16,12],[32,4]]",3707));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,496]],[[8,24],[16,4],[32,4]]",3708));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,12,12,0,0],[[4,2,64],[4,4,448]],[[8,16],[16,8],[32,8]]",3709));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,8,16,0,0],[[4,2,64],[4,4,448]],[[8,16],[16,8],[32,8]]",3710));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,240]],[[16,24],[32,8]]",3711));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,7,16,8,0,0],[[2,4,96],[4,4,288]],[[8,16],[32,16]]",3712));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,448,31,8,[1,7,16,8,0,0],[[2,4,96],[4,2,96],[4,4,384]],[[8,16],[16,12],[32,4]]",3713));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,31,31,448,31,32,[1,7,24,0,0,0],[[2,4,96],[4,2,96],[4,4,384]],[[8,16],[16,12],[32,4]]",3714));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]],[[8,16],[16,8],[32,8]]",3715));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]],[[8,16],[16,8],[32,8]]",3716));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,16],[16,12],[32,4]]",3717));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,16],[16,12],[32,4]]",3718));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,16],[16,12],[32,4]]",3719));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]],[[8,8],[16,20],[32,4]]",3720));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]],[[8,8],[16,20],[32,4]]",3721));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]],[[8,8],[16,20],[32,4]]",3722));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,7,16,8,0,0],[[2,4,96],[4,2,96],[4,4,384]],[[8,16],[16,12],[32,4]]",3723));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,7,20,4,0,0],[[2,4,64],[4,4,448]],[[8,8],[16,20],[32,4]]",3724));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,7,20,4,0,0],[[2,4,64],[4,2,64],[4,4,384]],[[8,8],[16,20],[32,4]]",3725));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,384]],[[8,8],[16,20],[32,4]]",3726));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,16,8,0,0],[[2,4,64],[4,4,448]],[[8,8],[16,20],[32,4]]",3727));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,304]],[[8,8],[16,16],[32,8]]",3728));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,304]],[[8,8],[16,16],[32,8]]",3729));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]],[[8,8],[16,20],[32,4]]",3730));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]],[[8,8],[16,20],[32,4]]",3731));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,7,24,0,0,0],[[2,4,64],[4,2,64],[4,4,384]],[[8,8],[16,20],[32,4]]",3732));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,7,12,12,0,0],[[2,4,64],[4,4,384]],[[8,8],[16,16],[32,8]]",3733));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,7,24,0,0,0],[[2,4,64],[4,2,64],[4,4,320]],[[16,28],[32,4]]",3734));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3735));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,20,4,0,0],[[2,4,64],[4,4,448]],[[8,8],[16,20],[32,4]]",3736));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]],[[8,8],[16,16],[32,8]]",3737));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]],[[8,20],[16,8],[32,4]]",3738));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,400]],[[8,12],[16,16],[32,4]]",3739));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,368]],[[8,12],[16,16],[32,4]]",3740));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,384,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3741));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]],[[8,20],[16,8],[32,4]]",3742));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]],[[8,16],[16,12],[32,4]]",3743));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]],[[8,20],[16,8],[32,4]]",3744));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,384,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3745));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,304]],[[8,8],[16,16],[32,8]]",3746));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]],[[8,8],[16,16],[32,8]]",3747));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]],[[8,8],[16,16],[32,8]]",3748));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,12,12,0,0],[[2,4,64],[4,4,384]],[[8,8],[16,16],[32,8]]",3749));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]],[[8,8],[16,16],[32,8]]",3750));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,304]],[[8,8],[16,16],[32,8]]",3751));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,368]],[[8,16],[16,8],[32,8]]",3752));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,304]],[[8,8],[16,16],[32,8]]",3753));		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,304]],[[8,8],[16,16],[32,8]]",3754));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,304]],[[8,8],[16,16],[32,8]]",3755));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,416,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,480]],[[8,20],[16,8],[32,4]]",3756));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,416]],[[8,12],[16,16],[32,4]]",3757));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3758));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,448,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3759));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,384]],[[8,16],[16,8],[32,8]]",3760));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3761));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,7,8,16,0,0],[[2,4,96],[4,2,64],[4,4,448]],[[8,20],[16,8],[32,4]]",3762));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,7,20,4,0,0],[[2,4,96],[4,2,64],[4,4,416]],[[8,16],[16,12],[32,4]]",3763));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,416,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,448]],[[8,20],[16,8],[32,4]]",3764));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3765));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,480]],[[8,20],[16,8],[32,4]]",3766));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,480]],[[8,20],[16,8],[32,4]]",3767));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,7,4,20,0,0],[[2,4,64],[4,2,64],[4,4,512]],[[8,24],[16,4],[32,4]]",3768));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,416]],[[8,24],[32,8]]",3769));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,448]],[[8,20],[16,8],[32,4]]",3770));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,400]],[[8,12],[16,16],[32,4]]",3771));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,416,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3772));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3773));		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,496]],[[8,24],[16,4],[32,4]]",3774));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,400]],[[8,12],[16,16],[32,4]]",3775));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]],[[8,16],[16,12],[32,4]]",3776));//R16_125:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,24],[32,8]]",3777));//R16_139:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,496]],[[8,24],[16,4],[32,4]]",3778));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,496]],[[8,20],[16,8],[32,4]]",3779));//R16_125:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,352]],[[8,16],[16,8],[32,8]]",3780));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]],[[8,16],[16,8],[32,8]]",3781));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,24],[32,8]]",3782));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,24],[32,8]]",3783));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]],[[8,8],[16,20],[32,4]]",3784));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]],[[8,8],[16,20],[32,4]]",3785));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,16],[16,12],[32,4]]",3786));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,16],[16,12],[32,4]]",3787));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,496]],[[8,24],[16,4],[32,4]]",3788));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,352,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,496]],[[8,28],[32,4]]",3789));		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,400]],[[8,12],[16,16],[32,4]]",3790));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3791));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,352,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,496]],[[8,28],[32,4]]",3792));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,416,31,8,[1,7,20,4,0,0],[[2,4,64],[4,2,96],[4,4,448]],[[8,20],[16,8],[32,4]]",3793));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,16],[16,12],[32,4]]",3794));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,352,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,496]],[[8,28],[32,4]]",3795));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,12,12,0,0],[[4,2,64],[4,4,448]],[[8,16],[16,8],[32,8]]",3796));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,304]],[[8,8],[16,16],[32,8]]",3797));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]],[[8,16],[16,8],[32,8]]",3798));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]],[[8,8],[16,16],[32,8]]",3799));		
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,16,8,0,0],[[2,4,96],[4,4,352]],[[8,8],[16,16],[32,8]]",3800));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]],[[8,16],[16,8],[32,8]]",3801));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,24],[32,8]]",3802));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,24],[32,8]]",3803));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,400]],[[8,12],[16,16],[32,4]]",3804));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,480,31,8,[1,7,20,4,0,0],[[2,4,64],[4,2,96],[4,4,384]],[[8,12],[16,16],[32,4]]",3805));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3806));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]],[[8,16],[16,8],[32,8]]",3807));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,20,4,0,0],[[4,2,64],[4,4,448]],[[8,16],[16,8],[32,8]]",3808));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,400]],[[8,12],[16,16],[32,4]]",3809));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,400]],[[8,12],[16,16],[32,4]]",3810));//R16_126:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,7,20,4,0,0],[[2,4,64],[4,2,64],[4,4,480]],[[8,20],[16,8],[32,4]]",3811));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,7,16,8,0,0],[[2,4,96],[4,2,96],[4,4,416]],[[8,20],[16,8],[32,4]]",3812));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,416]],[[8,16],[16,12],[32,4]]",3813));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,12,12,0,0],[[2,4,64],[4,4,448]],[[8,8],[16,20],[32,4]]",3814));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,368]],[[8,12],[16,16],[32,4]]",3815));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,464]],[[8,20],[16,8],[32,4]]",3816));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,480]],[[8,20],[16,8],[32,4]]",3817));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,7,12,12,0,0],[[2,4,96],[4,2,96],[4,4,416]],[[8,20],[16,8],[32,4]]",3818));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,8,16,0,0],[[2,4,96],[4,2,64],[4,4,416]],[[8,24],[32,8]]",3819));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,448,31,8,[1,7,20,4,0,0],[[2,4,96],[4,2,64],[4,4,416]],[[8,16],[16,12],[32,4]]",3820));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,400]],[[8,12],[16,16],[32,4]]",3821));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,24,0,0,0],[[2,4,96],[4,2,96],[4,4,384]],[[8,16],[16,12],[32,4]]",3822));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,16,8,0,0],[[2,4,96],[4,2,96],[4,4,384]],[[8,16],[16,12],[32,4]]",3823));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,7,24,0,0,0],[[2,4,64],[4,2,64],[4,4,416]],[[8,12],[16,16],[32,4]]",3824));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,96],[4,4,384]],[[8,12],[16,16],[32,4]]",3825));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,7,24,0,0,0],[[2,4,96],[4,2,96],[4,4,416]],[[8,20],[16,8],[32,4]]",3826));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,7,16,8,0,0],[[2,4,96],[4,2,64],[4,4,448]],[[8,20],[16,8],[32,4]]",3827));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,448,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,16],[16,12],[32,4]]",3828));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,496]],[[8,24],[16,4],[32,4]]",3829));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,464]],[[8,20],[16,8],[32,4]]",3830));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3831));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,528]],[[8,24],[16,4],[32,4]]",3832));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,96],[4,4,384]],[[8,16],[16,12],[32,4]]",3833));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,7,8,16,0,0],[[2,4,96],[4,2,64],[4,4,384]],[[8,12],[16,16],[32,4]]",3834));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,7,4,20,0,0],[[2,4,96],[4,2,96],[4,4,448]],[[8,24],[16,4],[32,4]]",3835));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,7,8,16,0,0],[[2,4,96],[4,2,64],[4,4,384]],[[8,12],[16,16],[32,4]]",3836));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,8,16,0,0],[[2,4,64],[4,2,96],[4,4,448]],[[8,20],[16,8],[32,4]]",3837));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3838));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,96],[4,4,384]],[[8,12],[16,16],[32,4]]",3839));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,384,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,96],[4,4,480]],[[8,24],[16,4],[32,4]]",3840));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,7,16,8,0,0],[[2,4,96],[4,2,96],[4,4,448]],[[8,24],[16,4],[32,4]]",3841));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,7,16,8,0,0],[[2,4,96],[4,2,64],[4,4,480]],[[8,24],[16,4],[32,4]]",3842));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,16,8,0,0],[[2,4,96],[4,2,64],[4,4,448]],[[8,20],[16,8],[32,4]]",3843));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]],[[8,16],[16,12],[32,4]]",3844));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3845));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,8,16,0,0],[[2,4,64],[4,2,96],[4,4,448]],[[8,20],[16,8],[32,4]]",3846));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,7,20,4,0,0],[[2,4,64],[4,2,96],[4,4,416]],[[8,16],[16,12],[32,4]]",3847));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,384,31,8,[1,7,16,8,0,0],[[2,4,96],[4,2,96],[4,4,448]],[[8,24],[16,4],[32,4]]",3848));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,464]],[[8,20],[16,8],[32,4]]",3849));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]],[[8,16],[16,12],[32,4]]",3850));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,96],[4,4,416]],[[8,16],[16,12],[32,4]]",3851));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,16,2,3,7,112,15,32,[1,3,6,2,12,8],[[2,2,16],[2,4,160],[4,2,160],[4,4,576]],[[1,16],[2,8],[4,4],[8,2],[16,1],[32,1]]",3852));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,7,16,8,0,0],[[4,2,96],[4,4,480]],[[8,24],[32,8]]",3853));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3854));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,20,4,0,0],[[2,4,64],[4,2,96],[4,4,448]],[[8,20],[16,8],[32,4]]",3855));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,496]],[[8,20],[16,8],[32,4]]",3856));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,7,16,8,0,0],[[2,4,96],[4,2,96],[4,4,416]],[[8,20],[16,8],[32,4]]",3857));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,96],[4,4,416]],[[8,16],[16,12],[32,4]]",3858));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,464]],[[8,16],[16,12],[32,4]]",3859));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,480,31,32,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]],[[8,12],[16,16],[32,4]]",3860));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,480,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,400]],[[8,12],[16,16],[32,4]]",3861));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]],[[8,20],[16,8],[32,4]]",3862));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3863));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3864));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,496]],[[8,24],[16,4],[32,4]]",3865));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3866));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,8,[1,3,4,24,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3867));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]],[[8,20],[16,8],[32,4]]",3868));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,24,0,0,0],[[2,4,96],[4,4,352]],[[8,8],[16,16],[32,8]]",3869));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,336]],[[8,8],[16,16],[32,8]]",3870));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,272]],[[8,8],[16,16],[32,8]]",3871));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,480,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]],[[8,12],[16,16],[32,4]]",3872));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,464]],[[8,20],[16,8],[32,4]]",3873));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,448,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,16],[16,12],[32,4]]",3874));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,12,12,0,0],[[2,4,96],[4,4,416]],[[8,8],[16,20],[32,4]]",3875));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3876));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,4,20,0,0],[[2,4,96],[4,2,64],[4,4,416]],[[8,24],[32,8]]",3877));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,384]],[[8,16],[16,8],[32,8]]",3878));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,24],[32,8]]",3879));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3880));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,416,31,8,[1,7,20,4,0,0],[[2,4,96],[4,2,64],[4,4,448]],[[8,20],[16,8],[32,4]]",3881));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,480,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,96],[4,4,384]],[[8,12],[16,16],[32,4]]",3882));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,8,20,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]],[[8,16],[16,12],[32,4]]",3883));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,7,20,4,0,0],[[2,4,64],[4,2,64],[4,4,384]],[[8,16],[16,8],[32,8]]",3884));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,480,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,416]],[[8,12],[16,16],[32,4]]",3885));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]],[[8,16],[16,8],[32,8]]",3886));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3887));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,416]],[[8,12],[16,16],[32,4]]",3888));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,448]],[[8,20],[16,8],[32,4]]",3889));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,7,8,16,0,0],[[2,4,96],[4,2,96],[4,4,448]],[[8,24],[16,4],[32,4]]",3890));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,416,31,8,[1,7,24,0,0,0],[[2,4,96],[4,2,96],[4,4,416]],[[8,20],[16,8],[32,4]]",3891));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,16,8,0,0],[[2,4,96],[4,2,64],[4,4,416]],[[8,24],[32,8]]",3892));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,7,20,4,0,0],[[2,4,96],[4,2,96],[4,4,416]],[[8,20],[16,8],[32,4]]",3893));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,368]],[[8,12],[16,16],[32,4]]",3894));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,368]],[[8,12],[16,16],[32,4]]",3895));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,496]],[[8,24],[16,4],[32,4]]",3896));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]],[[8,16],[16,12],[32,4]]",3897));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,336]],[[8,8],[16,20],[32,4]]",3898));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,432]],[[8,12],[16,16],[32,4]]",3899));//R16_118:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,64],[4,4,384]],[[8,12],[16,16],[32,4]]",3900));//R16_118:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,96],[4,4,384]],[[8,12],[16,16],[32,4]]",3901));//R16_118:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]],[[8,16],[16,12],[32,4]]",3902));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,16],[16,12],[32,4]]",3903));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3904));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,4,20,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3905));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,4,20,0,0],[[2,4,96],[4,2,64],[4,4,448]],[[8,20],[16,8],[32,4]]",3906));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,96],[4,4,480]],[[8,24],[16,4],[32,4]]",3907));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3908));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,416,31,4,[1,7,20,4,0,0],[[2,4,96],[4,2,64],[4,4,448]],[[8,20],[16,8],[32,4]]",3909));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,464]],[[8,20],[16,8],[32,4]]",3910));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,7,4,20,0,0],[[2,4,64],[4,2,96],[4,4,480]],[[8,24],[16,4],[32,4]]",3911));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,7,16,8,0,0],[[2,4,64],[4,2,96],[4,4,480]],[[8,24],[16,4],[32,4]]",3912));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,96],[4,4,448]],[[8,20],[16,8],[32,4]]",3913));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,16],[16,12],[32,4]]",3914));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,368]],[[8,12],[16,16],[32,4]]",3915));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,400]],[[8,12],[16,16],[32,4]]",3916));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,7,16,8,0,0],[[2,4,96],[4,2,64],[4,4,416]],[[8,16],[16,12],[32,4]]",3917));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,96],[4,4,384]],[[8,12],[16,16],[32,4]]",3918));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,384,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3919));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,24],[32,8]]",3920));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,416,31,8,[1,7,16,8,0,0],[[2,4,96],[4,2,96],[4,4,416]],[[8,20],[16,8],[32,4]]",3921));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,7,4,20,0,0],[[2,4,96],[4,2,64],[4,4,480]],[[8,24],[16,4],[32,4]]",3922));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,20,4,0,0],[[2,4,64],[4,2,96],[4,4,416]],[[8,16],[16,12],[32,4]]",3923));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,448,31,8,[1,7,20,4,0,0],[[2,4,96],[4,2,96],[4,4,384]],[[8,16],[16,12],[32,4]]",3924));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3925));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,31,31,448,31,32,[1,7,24,0,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3926));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,416,31,8,[1,7,20,4,0,0],[[2,4,64],[4,2,64],[4,4,480]],[[8,20],[16,8],[32,4]]",3927));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,448,31,32,[1,7,16,8,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3928));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,416,31,32,[1,7,8,16,0,0],[[2,4,96],[4,2,96],[4,4,416]],[[8,20],[16,8],[32,4]]",3929));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,8,16,0,0],[[2,4,96],[4,2,64],[4,4,416]],[[8,16],[16,12],[32,4]]",3930));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,8,16,0,0],[[2,4,64],[4,2,96],[4,4,448]],[[8,20],[16,8],[32,4]]",3931));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3932));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,7,4,20,0,0],[[2,4,64],[4,2,96],[4,4,480]],[[8,24],[16,4],[32,4]]",3933));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,7,8,16,0,0],[[2,4,64],[4,2,96],[4,4,480]],[[8,24],[16,4],[32,4]]",3934));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,7,8,16,0,0],[[2,4,64],[4,2,96],[4,4,480]],[[8,24],[16,4],[32,4]]",3935));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,384,31,8,[1,7,16,8,0,0],[[2,4,96],[4,2,96],[4,4,448]],[[8,24],[16,4],[32,4]]",3936));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,384,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3937));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,384,31,32,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,512]],[[8,24],[16,4],[32,4]]",3938));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,7,4,20,0,0],[[2,4,96],[4,2,64],[4,4,448]],[[8,20],[16,8],[32,4]]",3939));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,496]],[[8,24],[16,4],[32,4]]",3940));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,7,4,20,0,0],[[2,4,96],[4,2,64],[4,4,480]],[[8,24],[16,4],[32,4]]",3941));	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,480,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,368]],[[8,12],[16,16],[32,4]]",3942));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]],[[8,16],[16,12],[32,4]]",3943));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,432]],[[8,20],[16,8],[32,4]]",3944));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,496]],[[8,20],[16,8],[32,4]]",3945));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,496]],[[8,20],[16,8],[32,4]]",3946));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3947));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,4,20,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3948));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,7,12,12,0,0],[[2,4,64],[4,2,96],[4,4,448]],[[8,20],[16,8],[32,4]]",3949));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]],[[8,16],[16,8],[32,8]]",3950));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,7,4,20,0,0],[[2,4,96],[4,2,96],[4,4,448]],[[8,24],[16,4],[32,4]]",3951));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,7,4,20,0,0],[[2,4,96],[4,2,96],[4,4,448]],[[8,24],[16,4],[32,4]]",3952));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,3,12,16,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,368]],[[8,12],[16,16],[32,4]]",3953));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,368]],[[8,12],[16,16],[32,4]]",3954));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,384,31,8,[1,7,12,12,0,0],[[2,4,96],[4,2,96],[4,4,448]],[[8,24],[16,4],[32,4]]",3955));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,7,16,8,0,0],[[2,4,64],[4,2,96],[4,4,448]],[[8,20],[16,8],[32,4]]",3956));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]],[[8,16],[16,12],[32,4]]",3957));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,448,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,24],[32,8]]",3958));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,24,4,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]],[[8,8],[16,20],[32,4]]",3959));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,448,31,4,[1,3,8,20,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,432]],[[8,16],[16,12],[32,4]]",3960));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,496]],[[8,20],[16,8],[32,4]]",3961));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,464]],[[8,20],[16,8],[32,4]]",3962));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,416,31,4,[1,7,24,0,0,0],[[2,4,64],[4,2,96],[4,4,448]],[[8,20],[16,8],[32,4]]",3963));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,384,31,4,[1,7,24,0,0,0],[[2,4,96],[4,2,96],[4,4,448]],[[8,24],[16,4],[32,4]]",3964));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]],[[8,8],[16,20],[32,4]]",3965));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,7,8,16,0,0],[[2,4,96],[4,2,96],[4,4,448]],[[8,24],[16,4],[32,4]]",3966));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,8,16,0,0],[[2,4,96],[4,2,96],[4,4,384]],[[8,16],[16,12],[32,4]]",3967));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,352,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,496]],[[8,28],[32,4]]",3968));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,7,20,4,0,0],[[2,4,96],[4,2,96],[4,4,384]],[[8,16],[16,12],[32,4]]",3969));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3970));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]],[[8,24],[32,8]]",3971));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,12,12,0,0],[[2,4,96],[4,2,96],[4,4,384]],[[8,16],[16,12],[32,4]]",3972));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,4,20,0,0],[[2,4,64],[4,2,96],[4,4,416]],[[8,16],[16,12],[32,4]]",3973));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,480,31,32,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,368]],[[8,12],[16,16],[32,4]]",3974));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]],[[8,16],[16,12],[32,4]]",3975));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,448,31,8,[1,7,20,4,0,0],[[2,4,64],[4,2,96],[4,4,416]],[[8,16],[16,12],[32,4]]",3976));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,3,28,0,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3977));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,416,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,96],[4,4,416]],[[8,20],[16,8],[32,4]]",3978));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",3979));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,464]],[[8,16],[16,12],[32,4]]",3980));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]],[[8,8],[16,20],[32,4]]",3981));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,4,20,0,0],[[2,4,96],[4,2,96],[4,4,416]],[[8,20],[16,8],[32,4]]",3982));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,12,16,0,0],[[2,2,16],[2,4,48],[4,2,48],[4,4,400]],[[8,8],[16,20],[32,4]]",3983));//R16_176:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,368]],[[8,8],[16,20],[32,4]]",3984));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,240]],[[8,16],[32,16]]",3985));//R16_130:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,448,31,32,[1,7,0,24,0,0],[[4,4,576]],[[8,24],[32,8]]",3986));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,8,16,0,0],[[4,2,64],[4,4,512]],[[8,24],[32,8]]",3987));//R16_146:
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,448,31,8,[1,7,24,0,0,0],[[2,4,96],[4,2,96],[4,4,384]],[[8,16],[16,12],[32,4]]",3988));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,480,31,4,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,368]],[[8,12],[16,16],[32,4]]",3989));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,496]],[[8,24],[16,4],[32,4]]",3990));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,448,31,4,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,16],[16,12],[32,4]]",3991));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,8,[1,3,8,20,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,496]],[[8,24],[16,4],[32,4]]",3992));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,416]],[[8,12],[16,16],[32,4]]",3993));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,352,31,4,[1,7,12,12,0,0],[[2,4,96],[4,2,96],[4,4,480]],[[8,28],[32,4]]",3994));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,7,12,12,0,0],[[2,4,64],[4,2,96],[4,4,416]],[[8,16],[16,12],[32,4]]",3995));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,352,31,8,[1,7,8,16,0,0],[[2,4,96],[4,2,96],[4,4,480]],[[8,28],[32,4]]",3996));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,352,31,4,[1,7,8,16,0,0],[[2,4,96],[4,2,96],[4,4,480]],[[8,28],[32,4]]",3997));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,480,31,32,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,416]],[[8,12],[16,16],[32,4]]",3998));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,8,16,0,0],[[2,4,64],[4,2,64],[4,4,448]],[[8,16],[16,12],[32,4]]",3999));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,416,31,4,[1,7,16,8,0,0],[[2,4,96],[4,2,64],[4,4,448]],[[8,20],[16,8],[32,4]]",7000));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,480,31,8,[1,7,16,8,0,0],[[2,4,96],[4,2,64],[4,4,384]],[[8,12],[16,16],[32,4]]",7001));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,448,31,8,[1,7,16,8,0,0],[[2,4,96],[4,2,64],[4,4,416]],[[8,16],[16,12],[32,4]]",7002));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,7,12,12,0,0],[[2,4,96],[4,4,480]],[[8,16],[16,12],[32,4]]",7003));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]],[[8,16],[16,12],[32,4]]",7004));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,24],[32,8]]",7005));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,3,20,8,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,400]],[[8,16],[16,12],[32,4]]",7006));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,4,24,0,0],[[2,2,16],[2,4,80],[4,2,80],[4,4,464]],[[8,24],[16,4],[32,4]]",7007));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,3,16,12,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]],[[8,16],[16,12],[32,4]]",7008));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,3,24,4,0,0],[[2,2,16],[2,4,80],[4,2,48],[4,4,432]],[[8,16],[16,12],[32,4]]",7009));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,3,20,8,0,0],[[2,2,16],[2,4,48],[4,2,80],[4,4,496]],[[8,24],[16,4],[32,4]]",7010));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,4,20,0,0],[[2,4,64],[4,2,96],[4,4,448]],[[8,20],[16,8],[32,4]]",7011));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,4,20,0,0],[[2,4,64],[4,2,96],[4,4,448]],[[8,20],[16,8],[32,4]]",7012));
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,15,320,31,32,[1,7,8,0,0,16],[[2,4,64],[4,2,64],[4,4,576]],[[4,16],[8,8],[16,4],[32,4]]",7013));//R32_3000:[[0,0],[1,5]]
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,15,384,31,32,[1,7,8,0,0,16],[[2,4,64],[4,2,64],[4,4,512]],[[4,16],[16,12],[32,4]]",7014));//R32_3003:[[0,4],[14,25]]
		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,3,20,8,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6000));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,160]],[[8,12],[16,16],[32,4]]",6001));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,240]],[[8,24],[16,4],[32,4]]",6002));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,12,12,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,240]],[[8,20],[16,8],[32,4]]",6003));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,16,12,0,0],[[2,2,128],[2,4,160],[4,2,160],[4,4,192]],[[8,24],[16,4],[32,4]]",6004));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,16,12,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,192]],[[8,16],[16,12],[32,4]]",6005));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,16,12,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,208]],[[8,20],[16,8],[32,4]]",6006));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,3,24,4,0,0],[[2,2,112],[2,4,176],[4,2,176],[4,4,176]],[[8,24],[16,4],[32,4]]",6007));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,384,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,144],[4,2,176],[4,4,208]],[[8,24],[16,4],[32,4]]",6008));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,448,31,32,[1,3,20,8,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,176]],[[8,16],[16,12],[32,4]]",6009));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,160],[4,2,192],[4,4,192]],[[8,20],[16,8],[32,4]]",6010));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,416,31,4,[1,15,8,8,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6011));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,24,0,0,0],[[2,2,112],[2,4,144],[4,2,176],[4,4,176]],[[8,20],[16,8],[32,4]]",6012));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,208]],[[8,20],[16,8],[32,4]]",6013));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,224]],[[8,24],[32,8]]",6014));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,24,0,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,208]],[[8,16],[16,12],[32,4]]",6015));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,192],[4,2,160],[4,4,192]],[[8,24],[16,4],[32,4]]",6016));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,192]],[[8,16],[16,12],[32,4]]",6017));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,112],[4,2,176],[4,4,176]],[[8,16],[16,12],[32,4]]",6018));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,15,8,8,0,0],[[2,4,192],[4,2,192],[4,4,192]],[[8,16],[16,12],[32,4]]",6019));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,20,8,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,160]],[[8,24],[32,8]]",6020));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,7,12,12,0,0],[[2,2,112],[2,4,144],[4,2,176],[4,4,208]],[[8,24],[16,4],[32,4]]",6021));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,16,12,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,224]],[[8,20],[16,8],[32,4]]",6022));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,16,12,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,208]],[[8,20],[16,8],[32,4]]",6023));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,3,24,4,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,208]],[[8,24],[16,4],[32,4]]",6024));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,3,28,0,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]],[[8,8],[16,16],[32,8]]",6025));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,128],[4,4,224]],[[8,16],[16,12],[32,4]]",6026));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,3,24,4,0,0],[[2,2,112],[2,4,176],[4,2,144],[4,4,208]],[[8,24],[16,4],[32,4]]",6027));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,24,0,0,0],[[2,2,112],[2,4,176],[4,2,144],[4,4,176]],[[8,20],[16,8],[32,4]]",6028));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,15,8,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]],[[8,8],[16,20],[32,4]]",6029));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,416,31,32,[1,7,16,8,0,0],[[2,2,128],[2,4,160],[4,2,160],[4,4,160]],[[8,20],[16,8],[32,4]]",6030));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,384,31,8,[1,15,0,16,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,224]],[[8,24],[16,4],[32,4]]",6031));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,11,4,16,0,0],[[2,2,64],[2,4,192],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6032));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,11,8,12,0,0],[[2,2,112],[2,4,176],[4,2,112],[4,4,208]],[[8,20],[16,8],[32,4]]",6033));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,384,31,4,[1,11,8,12,0,0],[[2,2,112],[2,4,176],[4,2,144],[4,4,208]],[[8,24],[16,4],[32,4]]",6034));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]],[[8,8],[16,20],[32,4]]",6035));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,16,8,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,144]],[[8,8],[16,20],[32,4]]",6036));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,160]],[[8,16],[16,12],[32,4]]",6037));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,3,24,4,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,176]],[[8,20],[16,8],[32,4]]",6038));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,24,4,0,0],[[2,2,112],[2,4,112],[4,2,176],[4,4,208]],[[8,20],[16,8],[32,4]]",6039));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,16,12,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,176]],[[8,20],[16,8],[32,4]]",6040));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,16,12,0,0],[[2,2,112],[2,4,144],[4,2,176],[4,4,176]],[[8,20],[16,8],[32,4]]",6041));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,3,20,8,0,0],[[2,2,128],[2,4,160],[4,2,160],[4,4,192]],[[8,24],[16,4],[32,4]]",6042));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,416,31,8,[1,11,16,4,0,0],[[2,2,112],[2,4,144],[4,2,176],[4,4,176]],[[8,20],[16,8],[32,4]]",6043));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,7,8,16,0,0],[[2,2,96],[2,4,128],[4,2,96],[4,4,192]],[[8,8],[16,20],[32,4]]",6044));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,224]],[[8,8],[16,20],[32,4]]",6045));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,16,12,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6046));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,192]],[[8,16],[16,12],[32,4]]",6047));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,11,8,12,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,208]],[[8,16],[16,12],[32,4]]",6048));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,24,4,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,176]],[[8,20],[16,8],[32,4]]",6049));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]],[[8,16],[16,8],[32,8]]",6050));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,16,8,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,192]],[[8,20],[16,8],[32,4]]",6051));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,3,24,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,144]],[[8,8],[16,20],[32,4]]",6052));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,160],[4,2,160],[4,4,192]],[[8,24],[16,4],[32,4]]",6053));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,144]],[[8,8],[16,20],[32,4]]",6054));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,7,12,12,0,0],[[2,2,112],[2,4,144],[4,2,176],[4,4,208]],[[8,24],[16,4],[32,4]]",6055));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,544,31,8,[1,11,20,0,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,144]],[[16,30],[32,2]]",6056));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,544,31,8,[1,11,20,0,0,0],[[2,2,96],[2,4,128],[4,2,128],[4,4,128]],[[16,30],[32,2]]",6057));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]],[[8,12],[16,16],[32,4]]",6058));//R16_118:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,480,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]],[[8,12],[16,16],[32,4]]",6059));//R16_118:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,176],[4,4,208]],[[8,20],[16,8],[32,4]]",6060));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6061));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,176],[4,2,144],[4,4,208]],[[8,24],[16,4],[32,4]]",6062));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,7,24,0,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,192]],[[8,20],[16,8],[32,4]]",6063));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6064));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,11,16,4,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,208]],[[8,20],[16,8],[32,4]]",6065));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]],[[8,12],[16,16],[32,4]]",6066));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,416,31,4,[1,11,16,4,0,0],[[2,2,112],[2,4,176],[4,2,144],[4,4,176]],[[8,20],[16,8],[32,4]]",6067));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,384,31,4,[1,11,16,4,0,0],[[2,2,112],[2,4,176],[4,2,176],[4,4,176]],[[8,24],[16,4],[32,4]]",6068));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,240]],[[8,20],[16,8],[32,4]]",6069));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,384,31,8,[1,7,16,8,0,0],[[2,2,112],[2,4,176],[4,2,176],[4,4,176]],[[8,24],[16,4],[32,4]]",6070));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,24,0,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,208]],[[8,16],[16,12],[32,4]]",6071));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,11,8,12,0,0],[[2,2,64],[2,4,160],[4,2,192],[4,4,192]],[[8,20],[16,8],[32,4]]",6072));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,416,31,32,[1,15,0,16,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6073));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,192],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6074));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,384,31,8,[1,11,16,4,0,0],[[2,2,112],[2,4,176],[4,2,176],[4,4,176]],[[8,24],[16,4],[32,4]]",6075));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,11,8,12,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,176]],[[8,16],[16,12],[32,4]]",6076));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,11,4,16,0,0],[[2,2,112],[2,4,176],[4,2,144],[4,4,208]],[[8,24],[16,4],[32,4]]",6077));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,12,12,0,0],[[2,2,96],[2,4,160],[4,2,128],[4,4,224]],[[8,20],[16,8],[32,4]]",6078));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,192]],[[8,20],[16,8],[32,4]]",6079));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6080));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,416,31,8,[1,15,8,8,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6081));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,192],[4,4,192]],[[8,24],[16,4],[32,4]]",6082));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,416,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,192],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6083));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,416,31,32,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6084));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,160]],[[8,16],[16,12],[32,4]]",6085));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,384,31,32,[1,7,16,8,0,0],[[2,2,64],[2,4,192],[4,2,192],[4,4,192]],[[8,24],[16,4],[32,4]]",6086));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,16,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,240]],[[8,16],[16,12],[32,4]]",6087));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,224]],[[8,20],[16,8],[32,4]]",6088));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,24,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,208]],[[8,16],[16,12],[32,4]]",6089));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,224]],[[8,16],[16,12],[32,4]]",6090));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6091));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,224]],[[8,16],[16,12],[32,4]]",6092));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,128],[4,2,160],[4,4,192]],[[8,16],[16,12],[32,4]]",6093));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,128],[4,2,160],[4,4,224]],[[8,20],[16,8],[32,4]]",6094));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,3,20,8,0,0],[[2,2,112],[2,4,144],[4,2,176],[4,4,208]],[[8,24],[16,4],[32,4]]",6095));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,24,4,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,176]],[[8,16],[16,12],[32,4]]",6096));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,7,12,12,0,0],[[2,2,96],[2,4,192],[4,2,160],[4,4,192]],[[8,24],[16,4],[32,4]]",6097));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,128]],[[8,8],[16,16],[32,8]]",6098));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,7,12,12,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,240]],[[8,20],[16,8],[32,4]]",6099));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,160]],[[8,16],[16,8],[32,8]]",6100));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,3,24,4,0,0],[[2,2,112],[2,4,144],[4,2,176],[4,4,176]],[[8,20],[16,8],[32,4]]",6101));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,224]],[[8,20],[16,8],[32,4]]",6102));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,128],[4,2,160],[4,4,224]],[[8,20],[16,8],[32,4]]",6103));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,384,31,8,[1,15,8,8,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,224]],[[8,24],[16,4],[32,4]]",6104));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,176],[4,2,176],[4,4,176]],[[8,24],[16,4],[32,4]]",6105));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,24,4,0,0],[[2,2,112],[2,4,176],[4,2,144],[4,4,176]],[[8,20],[16,8],[32,4]]",6106));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,352,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,160],[4,2,160],[4,4,224]],[[8,28],[32,4]]",6107));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,384,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,192],[4,2,160],[4,4,192]],[[8,24],[16,4],[32,4]]",6108));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,7,12,12,0,0],[[2,2,112],[2,4,176],[4,2,144],[4,4,176]],[[8,20],[16,8],[32,4]]",6109));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,128],[4,2,96],[4,4,192]],[[8,8],[16,20],[32,4]]",6110));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,3,24,4,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,144]],[[8,16],[16,12],[32,4]]",6111));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,224]],[[8,16],[16,12],[32,4]]",6112));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,3,16,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]],[[8,8],[16,16],[32,8]]",6113));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,512,31,32,[1,7,24,0,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]],[[8,16],[16,8],[32,8]]",6114));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,256]],[[8,8],[16,20],[32,4]]",6115));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,24,0,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,144]],[[8,8],[16,20],[32,4]]",6116));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,480,31,8,[1,3,24,4,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]],[[8,12],[16,16],[32,4]]",6117));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,3,24,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,208]],[[8,16],[16,12],[32,4]]",6118));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,24,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,240]],[[8,20],[16,8],[32,4]]",6119));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,3,20,8,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]],[[8,12],[16,16],[32,4]]",6120));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,128],[4,2,160],[4,4,192]],[[8,16],[16,12],[32,4]]",6121));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,480,31,8,[1,15,16,0,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,192]],[[8,12],[16,16],[32,4]]",6122));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,416,31,8,[1,15,16,0,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6123));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,3,28,0,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,144]],[[8,16],[16,12],[32,4]]",6124));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,7,24,0,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6125));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,3,28,0,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,208]],[[8,24],[16,4],[32,4]]",6126));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,15,8,8,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6127));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,192]],[[8,16],[16,12],[32,4]]",6128));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,24,4,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,208]],[[8,20],[16,8],[32,4]]",6129));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,128],[4,4,192]],[[8,16],[16,12],[32,4]]",6130));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,28,0,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,144]],[[8,16],[16,8],[32,8]]",6131));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,448,31,8,[1,7,24,0,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,160]],[[8,16],[16,12],[32,4]]",6132));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,192],[4,2,96],[4,4,192]],[[8,24],[32,8]]",6133));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6134));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,192]],[[8,16],[16,12],[32,4]]",6135));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6136));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,28,0,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,208]],[[8,16],[16,12],[32,4]]",6137));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6138));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,16,2,3,7,112,15,32,[1,3,6,6,8,8],[[2,2,176],[2,4,240],[4,2,240],[4,4,256]],[[1,16],[2,8],[4,4],[8,2],[16,1],[32,1]]",6139));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,480,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]],[[8,12],[16,16],[32,4]]",6140));//R16_142:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,160],[4,2,128],[4,4,192]],[[8,12],[16,16],[32,4]]",6141));//R16_142:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,128],[4,2,160],[4,4,160]],[[8,12],[16,16],[32,4]]",6142));//R16_142:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,15,384,31,8,[1,15,0,16,0,0],[[2,4,192],[4,2,192],[4,4,256]],[[4,16],[16,12],[32,4]]",6143));//R16_181:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]],[[8,8],[16,20],[32,4]]",6144));//R16_146:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,224]],[[8,16],[16,12],[32,4]]",6145));//R16_146:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,224]],[[8,16],[16,12],[32,4]]",6146));//R16_146:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,192]],[[8,16],[16,12],[32,4]]",6147));//R16_146:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,24,0,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,192]],[[8,16],[16,12],[32,4]]",6148));//R16_146:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,224]],[[8,24],[32,8]]",6149));//R16_146:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,3,20,8,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,160]],[[8,24],[32,8]]",6150));//R16_146:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,31,31,448,31,32,[1,15,16,0,0,0],[[2,4,192],[4,2,192],[4,4,192]],[[8,16],[16,12],[32,4]]",6151));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,448,31,8,[1,15,16,0,0,0],[[2,4,192],[4,2,192],[4,4,192]],[[8,16],[16,12],[32,4]]",6152));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,384,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,192],[4,2,192],[4,4,192]],[[8,24],[16,4],[32,4]]",6153));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,416,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,144],[4,2,176],[4,4,176]],[[8,20],[16,8],[32,4]]",6154));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,384,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,176],[4,2,144],[4,4,208]],[[8,24],[16,4],[32,4]]",6155));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,384,31,8,[1,3,20,8,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,224]],[[8,24],[16,4],[32,4]]",6156));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,384,31,32,[1,7,16,8,0,0],[[2,2,128],[2,4,160],[4,2,160],[4,4,192]],[[8,24],[16,4],[32,4]]",6157));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,176]],[[8,16],[16,12],[32,4]]",6158));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,384,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,192],[4,2,160],[4,4,192]],[[8,24],[16,4],[32,4]]",6159));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,352,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,160],[4,2,160],[4,4,224]],[[8,28],[32,4]]",6160));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,192],[4,2,160],[4,4,192]],[[8,24],[16,4],[32,4]]",6161));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,176],[4,2,144],[4,4,208]],[[8,24],[16,4],[32,4]]",6162));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,384,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,176],[4,2,176],[4,4,176]],[[8,24],[16,4],[32,4]]",6163));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,384,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,192],[4,2,192],[4,4,192]],[[8,24],[16,4],[32,4]]",6164));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,416,31,8,[1,11,16,4,0,0],[[2,2,112],[2,4,176],[4,2,144],[4,4,176]],[[8,20],[16,8],[32,4]]",6165));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,128],[4,4,160]],[[8,8],[16,20],[32,4]]",6166));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,160]],[[8,16],[16,12],[32,4]]",6167));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,160]],[[8,12],[16,16],[32,4]]",6168));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6169));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,7,12,12,0,0],[[2,2,96],[2,4,160],[4,2,192],[4,4,192]],[[8,24],[16,4],[32,4]]",6170));//R16_140:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,384,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,192],[4,4,192]],[[8,24],[16,4],[32,4]]",6171));//R16_140:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6172));//R16_140:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,416,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,192],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6173));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,448,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,192]],[[8,16],[16,12],[32,4]]",6174));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,480,31,8,[1,15,8,8,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,192]],[[8,12],[16,16],[32,4]]",6175));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,160],[4,2,128],[4,4,160]],[[8,12],[16,16],[32,4]]",6176));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,128]],[[16,28],[32,4]]",6177));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,3,16,12,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,224]],[[8,24],[16,4],[32,4]]",6178));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,16,12,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,240]],[[8,20],[16,8],[32,4]]",6179));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,12,16,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,224]],[[8,24],[16,4],[32,4]]",6180));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,12,16,0,0],[[2,2,112],[2,4,144],[4,2,176],[4,4,208]],[[8,24],[16,4],[32,4]]",6181));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,7,8,16,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6182));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,128],[4,4,192]],[[8,12],[16,16],[32,4]]",6183));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,128],[4,2,160],[4,4,224]],[[8,20],[16,8],[32,4]]",6184));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,11,8,12,0,0],[[2,2,64],[2,4,160],[4,2,128],[4,4,192]],[[8,12],[16,16],[32,4]]",6185));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,12,12,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,144]],[[8,16],[16,8],[32,8]]",6186));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,160]],[[8,12],[16,16],[32,4]]",6187));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,160]],[[8,16],[16,12],[32,4]]",6188));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,128],[4,4,160]],[[8,12],[16,16],[32,4]]",6189));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,16,8,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,160]],[[8,16],[16,12],[32,4]]",6190));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",6191));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,192]],[[8,12],[16,16],[32,4]]",6192));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,11,8,12,0,0],[[2,2,112],[2,4,176],[4,2,144],[4,4,176]],[[8,20],[16,8],[32,4]]",6193));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,208]],[[8,20],[16,8],[32,4]]",6194));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,11,4,16,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,208]],[[8,20],[16,8],[32,4]]",6195));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,384,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,144],[4,2,176],[4,4,208]],[[8,24],[16,4],[32,4]]",6196));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,11,4,16,0,0],[[2,2,112],[2,4,144],[4,2,176],[4,4,208]],[[8,24],[16,4],[32,4]]",6197));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,384,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,160],[4,2,160],[4,4,192]],[[8,24],[16,4],[32,4]]",6198));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,24,0,0,0],[[2,2,64],[2,4,160],[4,2,128],[4,4,192]],[[8,12],[16,16],[32,4]]",6199));

	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,23,416,31,4,[1,15,8,8,0,0],[[2,4,192],[4,2,192],[4,4,224]]",4000));//[[0,2],[1,0]],[[0,0],[3,0]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,15,15,384,31,4,[1,15,8,8,0,0],[[2,4,192],[4,2,192],[4,4,256]]",4001));//R16_201×R2_1=R32_4001=[[3,2],[1,0]],[[0,2],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,15,0,16,0,0],[[2,4,128],[4,4,256]]",4002));//R16_259×R2_1=R32_4002=[[2,0,0],[1,0,1],[2,0,2]],[[2,2,2],[2,2,2],[2,2,2]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,352,31,8,[1,15,4,8,4,0],[[2,4,224],[4,2,192],[4,4,256]]",4003));//R16_266×R2_1=R32_4003=[[2,2,2],[2,2,2],[2,2,2]],[[0,0,0],[3,1,2],[3,2,1]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,15,448,31,8,[1,15,0,16,0,0],[[2,4,128],[4,2,192],[4,4,256]]",4004));//[[2,1,0,2],[0,2,0,2],[0,2,0,2],[2,1,0,2]],[[2,2,2,2],[2,2,2,2],[2,2,2,2],[2,2,2,2]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,15,512,31,8,[1,15,0,0,16,0],[[2,4,128],[4,2,128],[4,4,256]],[[8,16],[16,8],[32,8]]",4005));//[[0,0,0,0],[0,0,0,0],[2,2,2,2],[2,2,2,2]],[[2,2,2,2],[0,2,2,2],[1,0,2,2],[0,3,0,2]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,1,16,2,11,15,176,15,8,[1,11,6,14,0,0],[[2,2,112],[2,4,240],[4,2,240],[4,4,256]]",4006));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,128]]",4007));//R16_185×R2_1=R32_4007=R8_6:[[0,0],[0,1]],[[0,1],[2,0]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,128]]",4008));//R16_187×R2_1=R32_4008=R8_6:[[0,0],[0,1]],[[0,2],[1,0]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,8,16,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,256]]",4009));//R16_153×R2_1=R32_4009=R8_9:[[0,0],[0,1]],[[0,1],[3,0]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,8,16,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,256]],[[8,16],[16,8],[32,8]]",4010));//R16_163×R2_1=R32_4010=R8_9:[[0,0],[0,1]],[[0,2],[1,0]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,7,8,16,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,256]]",4011));//R16_195×R2_1=R32_4011=R8_9:[[0,0],[0,1]],[[1,0],[3,2]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,8,16,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,192]]",4012));//R16_159×R2_1=R32_4012=R8_9:[[0,0],[0,1]],[[1,1],[1,2]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,192]]",4013));//R8_9:[[0,0],[0,1]],[[1,1],[2,2]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,7,8,16,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,192]]",4014));//R16_165×R2_1=R32_4014=R8_9:[[0,0],[0,1]],[[2,0],[1,0]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,192]]",4015));//R16_190×R2_1=R32_4015=R8_9:[[0,0],[0,1]],[[2,0],[1,2]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,15,0,16,0,0],[[4,2,128],[4,4,256]]",4016));//R16_258×R2_1=R32_4016=R8_9:[[1,2],[7,4]],[[1,3],[3,0]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,7,8,16,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,128]]",4017));//R16_167×R2_1=R32_4017=R8_9:[[1,2],[7,4]],[[1,5],[1,0]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,448,31,32,[1,7,8,16,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,256]]",4018));//R16_148×R2_1=R32_4018=R8_6:[[1,1],[7,0]],[[1,4],[0,1]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,15,0,16,0,0],[[2,4,128],[4,2,128],[4,4,256]]",4019));//R8_6:[[1,1],[7,0]],[[1,4],[3,0]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,15,8,8,0,0],[[2,4,128],[4,2,128],[4,4,256]]",4020));//R8_6:[[0,0],[7,7]],[[0,1],[0,0]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,256]]",4021));//R16_158×R2_1=R32_4021=R8_6:[[0,0],[7,7]],[[0,1],[0,1]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,192]]",4022));//R8_6:[[0,0],[7,7]],[[0,1],[1,0]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,192]]",4023));//R8_6:[[0,0],[7,7]],[[0,1],[1,1]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,256]],[[8,16],[16,8],[32,8]]",4024));//R16_151×R2_1=R32_4024=R8_6:[[0,0],[7,7]],[[1,0],[0,0]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,15,8,8,0,0],[[2,4,128],[4,2,128],[4,4,192]]",4025));//R16_261×R2_1=R32_4025=R8_11:[[0,0],[7,7]],[[0,2],[0,0]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,256]]",4026));//R8_18:[[2,3],[1,6]],[[3,2],[0,2]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,15,0,16,0,0],[[2,4,128],[4,2,128],[4,4,192]]",4027));//R16_155×R2_1=R32_4027=R8_18:[[2,3],[1,7]],[[2,6],[1,3]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,15,8,8,0,0],[[4,2,192],[4,4,192]]",4028));//R8_10:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,15,8,8,0,0],[[4,2,128],[4,4,128]]",4029));//R16_264×R2_1=R32_4029=R8_10:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,320,31,32,[1,15,2,8,6,0],[[2,4,224],[4,2,224],[4,4,256]]",4030));//R16_205×R2_1=R32_4030=R4_3:[[2,0,0],[2,0,0],[0,0,0]],[[3,2,0],[2,3,0],[0,0,2]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,16,2,15,15,288,15,32,[1,15,2,14,0,0],[[2,4,240],[4,2,240],[4,4,256]]",4031));//R4_3:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,192]]",4032));//R16_157×R2_1=R32_4032=R16_157:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,15,8,8,0,0],[[2,4,128],[4,2,192],[4,4,192]]",4033));//R8_11:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,264,23,2,[1,9,18,4,0,0],[[2,2,96],[2,4,176],[4,2,232],[4,4,256]]",4034));//R8_14:[[0,1],[2,7]],[[0,1],[3,2]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,248,23,2,[1,9,18,4,0,0],[[2,2,96],[2,4,208],[4,2,216],[4,4,256]]",4035));//R8_14:[[0,1],[2,7]],[[1,0],[0,4]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,352,31,8,[1,15,4,8,4,0],[[2,4,192],[4,2,224],[4,4,256]]",4036));//R16_265×R2_1=R32_4036=R8_21:[[0,1],[3,1]],[[0,2],[0,6]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,15,448,31,8,[1,15,0,16,0,0],[[2,4,192],[4,2,128],[4,4,256]]",4037));//R8_24:[[1,7],[8,4]],[[1,7],[9,5]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,8,16,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,256]]",4038));//R16_149×R2_1=R32_4038=R16_127:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,288,31,8,[1,7,12,8,4,0],[[2,2,64],[2,4,224],[4,2,192],[4,4,256]]",4039));//R16_174×R2_1=R32_4039=R16_177:[[0,2],[3,11]],[[0,3],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,288,31,8,[1,7,12,8,4,0],[[2,2,64],[2,4,192],[4,2,224],[4,4,256]]",4040));//R16_177×R2_1=R32_4040=R16_174:[[0,0],[5,10]],[[0,0],[6,4]]		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,7,176,15,8,[1,7,4,12,8,0],[[2,2,128],[2,4,224],[4,2,240],[4,4,256]]",4041));//R16_179:[[0,0],[8,0]],[[0,0],[8,6]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,7,15,240,15,2,[1,7,16,8,0,0],[[2,2,96],[2,4,192],[4,2,240],[4,4,256]]",4042));//R16_174:[[0,0],[5,10]],[[0,0],[5,11]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,7,15,256,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,224],[4,2,224],[4,4,256]]",4043));//R16_174:[[0,0],[5,10]],[[0,0],[11,9]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,23,448,31,2,[1,7,16,8,0,0],[[2,2,96],[2,4,192],[4,2,96],[4,4,192]]",4044));//R16_174:[[0,0],[5,11]],[[0,0],[8,0]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,8,7,8,8,0],[[2,2,64],[2,4,160],[4,2,64],[4,4,256]],[[8,12],[16,16],[32,4]]",4045));//R16_259×R2_2=R32_4045=R16_191:[[0,4],[10,6]],[[0,4],[10,13]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,11,15,272,15,2,[1,11,12,8,0,0],[[2,2,64],[2,4,192],[4,2,240],[4,4,256]],[[1,16],[8,8],[16,4],[32,4]]",4046));//R16_191:[[0,0],[3,15]],[[0,0],[6,4]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,1,24,6,7,7,224,23,8,[1,9,12,10,0,0],[[2,2,96],[2,4,232],[4,2,216],[4,4,256]]",4047));//R16_191:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,11,15,320,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,192],[4,2,192],[4,4,256]]",4048));//R16_169×R2_1=R32_4048=R16_191:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,15,384,31,8,[1,7,8,16,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,256]]",4049));//R16_181:[[0,0],[2,7]],[[0,0],[2,8]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,15,384,31,8,[1,7,8,16,0,0],[[2,2,64],[2,4,192],[4,2,128],[4,4,256]]",4050));//R16_181:[[0,0],[2,7]],[[0,0],[2,11]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,15,320,31,32,[1,7,8,16,0,0],[[2,2,64],[2,4,192],[4,2,192],[4,4,256]]",4051));//R16_181:[[0,0],[2,7]],[[0,0],[3,2]]		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,1,28,12,3,3,216,27,8,[1,8,19,4,0,0],[[2,2,136],[2,4,196],[4,2,220],[4,4,256]]",4052));//R16_191×R2_2=R32_4052=R16_191×R4_3的商环
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,15,432,31,32,[1,4,11,12,4,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,160]]",4053));//R16_189×R2_2=R32_4053=R16_189×R4_3的商环	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,432,31,8,[1,4,11,12,4,0],[[2,2,112],[2,4,160],[4,2,112],[4,4,208]]",4054));//R16_190×R2_2=R32_4054=R16_190×R4_3的商环
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,640,31,2,[1,7,24,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4055));//R8_9×R8_8的商环	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,264,31,2,[1,9,18,4,0,0],[[2,2,96],[2,4,232],[4,2,176],[4,4,256]]",4056));//R8_14:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,248,31,2,[1,9,18,4,0,0],[[2,2,96],[2,4,216],[4,2,208],[4,4,256]]",4057));//R8_14:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,15,15,320,31,4,[1,15,8,8,0,0],[[2,4,224],[4,2,224],[4,4,256]]",4058));//R8_21:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,15,8,8,0,0],[[2,4,192],[4,4,192]]",4059));//R8_15:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,15,8,8,0,0],[[2,4,128],[4,2,128],[4,4,192]]",4060));//R8_15:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,11,15,320,31,2,[1,11,12,8,0,0],[[2,2,64],[2,4,160],[4,2,224],[4,4,256]]",4061));//R16_169:[[0,0,0],[0,0,0],[0,0,1]],[[0,0,0],[0,0,0],[0,2,5]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,2,[1,7,16,8,0,0],[[2,2,96],[2,4,192],[4,2,96],[4,4,224]]",4062));//R16_169:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,23,448,31,2,[1,11,4,16,0,0],[[2,2,64],[2,4,192],[4,2,64],[4,4,256]]",4063));//R16_169:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,15,512,31,32,[1,15,0,0,16,0],[[2,4,128],[4,2,128],[4,4,256]]",4064));//R16_260×R2_1=R32_4064=R8_24:[[0,0,4],[1,4,4],[3,4,0]],[[0,0,4],[1,4,4],[3,4,3]]		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,11,20,0,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,128]]",4065));//R16_126×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,480,31,8,[1,6,15,10,0,0],[[2,2,112],[2,4,112],[4,2,160],[4,4,160]],[[8,12],[16,16],[32,4]]",4066));//R16_126×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,11,20,0,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,128]]",4067));//R16_127×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,480,31,8,[1,6,15,10,0,0],[[2,2,112],[2,4,160],[4,2,112],[4,4,160]]",4068));//R16_127×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,256]]",4069));//R16_128×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,336,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,256]],[[4,12],[8,12],[16,4],[32,4]]",4070));//R16_128×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,192]]",4071));//R16_129×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,384,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,208]],[[4,8],[8,12],[16,8],[32,4]]",4072));//R16_129×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,11,20,0,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,64]]",4073));//R16_130×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,576,31,8,[1,6,15,10,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]],[[8,8],[16,16],[32,8]]",4074));//R16_130×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,480,31,32,[1,8,15,8,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]],[[8,12],[16,16],[32,4]]",4075));//R16_131×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,640,31,32,[1,7,24,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4076));//R16_132×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,480,31,32,[1,4,15,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]],[[8,12],[16,16],[32,4]]",4077));//R16_132×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,640,31,8,[1,3,28,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4078));//R16_133×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,480,31,8,[1,2,15,14,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]],[[8,12],[16,16],[32,4]]",4079));//R16_133×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,15,512,31,32,[1,7,8,16,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]]",4080));//R16_134×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,384,31,32,[1,4,7,12,8,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,160]],[[4,8],[8,12],[16,8],[32,4]]",4081));//R16_134×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,11,20,0,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,128]],[[16,24],[32,8]]",4082));//R16_135×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,6,15,10,0,0],[[2,2,112],[2,4,160],[4,2,112],[4,4,160]],[[8,12],[16,16],[32,4]]",4083));//R16_135×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,15,448,31,32,[1,7,8,16,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,192]]",4084));//R16_136×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,336,31,32,[1,4,7,12,8,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,208]]",4085));//R16_136×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,11,20,0,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,192]]",4086));//R16_137×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,6,15,10,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4087));//R16_137×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,576,31,32,[1,15,16,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,160]]",4088));//R16_138×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,432,31,32,[1,8,15,8,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,184]]",4089));//R16_138×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,11,20,0,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,128]]",4090));//R16_139×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,432,31,8,[1,6,15,10,0,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,160]]",4091));//R16_139×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,576,31,32,[1,7,24,0,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,128]]",4092));//R16_140×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,432,31,32,[1,4,15,12,0,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,160]]",4093));//R16_140×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,448,31,32,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,256]]",4094));//R16_147×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,15,336,31,32,[1,4,11,12,4,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,256]]",4095));//R16_147×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,15,336,31,32,[1,4,7,12,8,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,256]]",4096));//R16_148×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,336,31,8,[1,4,7,12,8,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,256]]",4097));//R16_149×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,128]],[[8,8],[16,16],[32,8]]",4098));//R16_150×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,432,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,160]],[[4,4],[8,12],[16,12],[32,4]]",4099));//R16_150×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,384,31,8,[1,4,11,12,4,0],[[2,2,112],[2,4,112],[4,2,160],[4,4,256]]",4100));//R16_151×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,384,31,8,[1,4,7,12,8,0],[[2,2,112],[2,4,112],[4,2,160],[4,4,256]]",4101));//R16_153×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,15,16,0,0,0],[[2,4,128],[4,2,128],[4,4,192]]",4102));//R16_154×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,432,31,8,[1,8,15,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,208]]",4103));//R16_154×R2_2
			
	/*---------------begin of R32_4104~R32_4345(242-46)----------------*/
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,192]],[[8,12],[16,16],[32,4]]",4120));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,192]],[[8,12],[16,16],[32,4]]",4122));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,15,8,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,192]],[[8,12],[16,16],[32,4]]",4176));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,224]],[[8,20],[16,8],[32,4]]",4178));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,224]],[[8,24],[16,4],[32,4]]",4200));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,28,0,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,208]],[[8,24],[16,4],[32,4]]",4202));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,20,8,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,208]],[[8,24],[16,4],[32,4]]",4264));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,416,31,32,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,224]],[[8,20],[16,8],[32,4]]",4265));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,192],[4,4,192]],[[8,20],[16,8],[32,4]]",4266));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,192]],[[8,16],[16,12],[32,4]]",4267));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,20,8,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",4268));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,7,24,0,0,0],[[2,2,112],[2,4,112],[4,2,176],[4,4,176]],[[8,16],[16,12],[32,4]]",4269));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,16,8,0,0],[[2,2,112],[2,4,112],[4,2,176],[4,4,176]],[[8,16],[16,12],[32,4]]",4270));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,28,0,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",4274));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,20,8,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",4275));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]],[[8,16],[32,16]]",4276));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,128]],[[16,24],[32,8]]",4277));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,160]],[[8,16],[16,8],[32,8]]",4278));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,64],[4,2,192],[4,4,192]],[[8,16],[16,8],[32,8]]",4279));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,160]],[[8,8],[16,16],[32,8]]",4280));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,28,0,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,144]],[[8,24],[32,8]]",4281));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,20,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]],[[8,16],[16,8],[32,8]]",4282));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,12,12,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,160]],[[8,16],[16,8],[32,8]]",4286));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,16,12,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,224]],[[8,24],[16,4],[32,4]]",4287));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,15,320,31,8,[1,7,8,16,0,0],[[2,2,64],[2,4,192],[4,2,192],[4,4,256]],[[4,16],[8,8],[16,4],[32,4]]",4288));//R16_122:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,144]],[[8,8],[16,20],[32,4]]",4292));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,144]],[[8,12],[16,16],[32,4]]",4307));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,448,31,8,[1,15,16,0,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,160]],[[8,16],[16,12],[32,4]]",4310));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,448,31,8,[1,11,20,0,0,0],[[2,2,112],[2,4,176],[4,2,112],[4,4,176]],[[8,16],[16,12],[32,4]]",4319));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,416,31,32,[1,7,24,0,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,224]],[[8,20],[16,8],[32,4]]",4325));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,11,20,0,0,0],[[2,2,64],[2,4,160],[4,2,128],[4,4,160]],[[8,8],[16,20],[32,4]]",4328));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,416,31,32,[1,15,16,0,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",4331));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,15,16,0,0,0],[[2,4,192],[4,2,128],[4,4,192]],[[8,8],[16,20],[32,4]]",4293));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,160]],[[8,8],[16,20],[32,4]]",4308));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,12,12,0,0],[[2,2,112],[2,4,144],[4,2,176],[4,4,176]],[[8,20],[16,8],[32,4]]",4311));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,208]],[[8,16],[16,12],[32,4]]",4320));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,12,12,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,208]],[[8,20],[16,8],[32,4]]",4326));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,160],[4,2,160],[4,4,160]],[[8,20],[16,8],[32,4]]",4329));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,7,12,12,0,0],[[2,2,96],[2,4,160],[4,2,192],[4,4,192]],[[8,24],[16,4],[32,4]]",4332));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,7,12,12,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,224]],[[8,24],[16,4],[32,4]]",4294));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,176],[4,4,176]],[[8,16],[16,12],[32,4]]",4309));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,128],[4,2,160],[4,4,160]],[[8,12],[16,16],[32,4]]",4312));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,3,16,12,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,144]],[[8,12],[16,16],[32,4]]",4321));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,160],[4,2,160],[4,4,160]],[[8,20],[16,8],[32,4]]",4327));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,144]],[[8,8],[16,20],[32,4]]",4330));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,208]],[[8,16],[16,12],[32,4]]",4333));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,31,31,1024,31,32,[1,15,16,0,0,0],[],[[32,32]]",4104));//R8R4_48=R8_5×R4_4、R8R4_265=R8_25×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,7,16,8,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,64]],[[16,16],[32,16]]",4105));//R8R4_49=R8_5×R4_5、R8R4_276=R8_26×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,768,31,32,[1,8,15,8,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,64]],[[16,16],[32,16]]",4106));//R8R4_50=R8_5×R4_6、R8R4_70=R8_7×R4_4、R8R4_287=R8_27×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,640,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]],[[8,16],[32,16]]",4107));//R8R4_51=R8_5×R4_7、R8R4_309=R8_29×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,640,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]],[[16,24],[32,8]]",4108));//R8R4_52=R8_5×R4_8、R8R4_364=R8_34×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,512,31,32,[1,8,12,7,4,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]],[[8,16],[16,8],[32,8]]",4109));//R8R4_53=R8_5×R4_9、R8R4_375=R8_35×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,576,31,32,[1,6,13,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]],[[8,8],[16,16],[32,8]]",4110));//R8R4_54=R8_5×R4_10、R8R4_72=R8_7×R4_6、R8R4_430=R8_40×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,448,31,32,[1,4,9,10,8,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,144]],[[8,24],[32,8]]",4111));//R8R4_55=R8_5×R4_11、R8R4_452=R8_42×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,7,24,0,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,64]]",4112));//R8R4_59=R8_6×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4113));//R8R4_60=R8_6×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32,[1,4,15,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4114));//R8R4_61=R8_6×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8,[1,5,14,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]]",4115));//R8R4_62=R8_6×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8,[1,5,14,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]]",4116));//R8R4_63=R8_6×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,4,14,7,6,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,160]]",4117));//R8R4_64=R8_6×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,432,31,32,[1,4,9,18,0,0],[[2,2,148],[2,4,148],[4,2,148],[4,4,148]]",4118));//R8R4_65=R8_6×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,336,31,32,[1,2,9,8,12,0],[[2,2,172],[2,4,172],[4,2,172],[4,4,172]]",4119));//R8R4_66=R8_6×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32,[1,4,11,12,4,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]],[[8,8],[16,16],[32,8]]",4121));//R8R4_71=R8_7×R4_5、R8R4_408=R8_38×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,480,31,8,[1,8,11,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]],[[4,8],[8,8],[16,8],[32,8]]",4123));//R8R4_73=R8_7×R4_7、R8R4_441=R8_41×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,480,31,8,[1,8,11,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]],[[8,12],[16,16],[32,4]]",4124));//R8R4_74=R8_7×R4_8、R8R4_507=R8_47×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,384,31,32,[1,6,11,10,4,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,160]],[[4,8],[8,12],[16,8],[32,4]]",4125));//R8R4_75=R8_7×R4_9、R8R4_518=R8_48×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,3,3,432,31,32,[1,8,9,14,0,0],[[2,2,148],[2,4,148],[4,2,148],[4,4,148]],[[4,4],[8,12],[16,12],[32,4]]",4126));//R8R4_76=R8_7×R4_10、R8R4_540=R8_50×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,3,336,31,32,[1,4,7,10,6,4],[[2,2,172],[2,4,172],[4,2,172],[4,4,172]],[[4,12],[8,12],[16,4],[32,4]]",4127));//R8R4_77=R8_7×R4_11、R8R4_551=R8_51×R4_1
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
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,512,31,32,[1,15,2,14,0,0],[[2,4,128],[4,2,128],[4,4,256]],[[8,16],[16,8],[32,8]]",4168));//R8R4_136=R8_13×R4_4、R8R4_267=R8_25×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,448,31,8,[1,15,4,12,0,0],[[2,4,192],[4,2,128],[4,4,256]]",4184));//R8R4_158=R8_15×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,768,31,32,[1,15,16,0,0,0],[[4,4,256]],[[16,16],[32,16]]",4192));//R8R4_169=R8_16×R4_4、R8R4_266=R8_25×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,7,10,6,8,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,256]],[[4,8],[8,12],[16,8],[32,4]]",4169));//R8R4_137=R8_13×R4_5、R8R4_278=R8_26×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,288,31,32,[1,4,11,8,8,0],[[2,2,112],[2,4,184],[4,2,184],[4,4,256]],[[2,4],[4,10],[8,10],[16,6],[32,2]]",4177));//R8R4_148=R8_14×R4_5、R8R4_410=R8_38×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,336,31,8,[1,7,12,4,8,0],[[2,2,64],[2,4,208],[4,2,160],[4,4,256]]",4185));//R8R4_159=R8_15×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,7,16,8,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,256]],[[8,8],[16,16],[32,8]]",4193));//R8R4_170=R8_16×R4_5、R8R4_277=R8_26×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,384,31,32,[1,8,11,12,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,256]],[[4,8],[8,12],[16,8],[32,4]]",4170));//R8R4_138=R8_13×R4_6、R8R4_147=R8_14×R4_4、R8R4_289=R8_27×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,336,31,8,[1,8,15,8,0,0],[[2,2,64],[2,4,208],[4,2,160],[4,4,256]]",4186));//R8R4_160=R8_15×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32,[1,8,15,8,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,256]],[[8,8],[16,16],[32,8]]",4194));//R8R4_171=R8_16×R4_6、R8R4_180=R8_17×R4_4、R8R4_288=R8_27×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,320,31,8,[1,9,12,10,0,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,256]],[[2,8],[4,4],[8,12],[16,4],[32,4]]",4171));//R8R4_139=R8_13×R4_7、R8R4_311=R8_29×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,12,3,3,240,27,8,[1,8,19,4,0,0],[[2,2,136],[2,4,196],[4,2,196],[4,4,256]],[[1,4],[2,6],[4,8],[8,8],[16,4],[32,2]]",4179));//R8R4_150=R8_14×R4_7、R8R4_443=R8_41×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,280,31,2,[1,9,18,4,0,0],[[2,2,96],[2,4,216],[4,2,176],[4,4,256]]",4187));//R8R4_161=R8_15×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]],[[4,8],[8,8],[16,8],[32,8]]",4195));//R8R4_172=R8_16×R4_7、R8R4_310=R8_29×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,320,31,8,[1,9,12,10,0,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,256]],[[4,12],[8,10],[16,8],[32,2]]",4172));//R8R4_140=R8_13×R4_8、R8R4_366=R8_34×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,12,3,3,240,31,8,[1,8,19,4,0,0],[[2,2,136],[2,4,196],[4,2,196],[4,4,256]],[[2,6],[4,11],[8,9],[16,5],[32,1]]",4180));//R8R4_151=R8_14×R4_8、R8R4_509=R8_47×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,280,31,2,[1,9,18,4,0,0],[[2,2,96],[2,4,216],[4,2,176],[4,4,256]]",4188));//R8R4_162=R8_15×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]],[[8,12],[16,16],[32,4]]",4196));//R8R4_173=R8_16×R4_8、R8R4_365=R8_34×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,256,31,32,[1,8,8,13,2,0],[[2,2,128],[2,4,192],[4,2,192],[4,4,256]],[[2,8],[4,8],[8,10],[16,4],[32,2]]",4173));//R8R4_141=R8_13×R4_9、R8R4_377=R8_35×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,28,8,3,3,192,27,32,[1,6,15,10,0,0],[[2,2,160],[2,4,208],[4,2,208],[4,4,256]],[[1,4],[2,8],[4,9],[8,7],[16,3],[32,1]]",4181));//R8R4_152=R8_14×R4_9、R8R4_520=R8_48×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,224,31,8,[1,8,12,11,0,0],[[2,2,128],[2,4,224],[4,2,192],[4,4,256]]",4189));//R8R4_163=R8_15×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,8,12,7,4,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,256]],[[4,8],[8,12],[16,8],[32,4]]",4197));//R8R4_174=R8_16×R4_9、R8R4_376=R8_35×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,3,3,288,31,32,[1,6,17,8,0,0],[[2,2,112],[2,4,184],[4,2,184],[4,4,256]],[[2,4],[4,10],[8,10],[16,6],[32,2]]",4174));//R8R4_142=R8_13×R4_10、R8R4_149=R8_14×R4_6、R8R4_432=R8_40×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,30,16,1,1,216,29,32,[1,8,23,0,0,0],[[2,2,148],[2,4,202],[4,2,202],[4,4,256]],[[1,2],[2,7],[4,10],[8,8],[16,4],[32,1]]",4182));//R8R4_153=R8_14×R4_10、R8R4_542=R8_50×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,12,3,3,252,31,8,[1,6,25,0,0,0],[[2,2,112],[2,4,220],[4,2,184],[4,4,256]]",4190));//R8R4_164=R8_15×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,432,31,32,[1,6,13,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,256]],[[4,4],[8,12],[16,12],[32,4]]",4198));//R8R4_175=R8_16×R4_10、R8R4_182=R8_17×R4_6、R8R4_431=R8_40×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,3,224,31,32,[1,4,9,10,4,4],[[2,2,144],[2,4,200],[4,2,200],[4,4,256]],[[2,12],[4,6],[8,10],[16,2],[32,2]]",4175));//R8R4_143=R8_13×R4_11、R8R4_454=R8_42×R4_3=R8_14:[[7,7],[5,0]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,26,8,1,1,168,25,32,[1,4,13,4,10,0],[[2,2,172],[2,4,214],[4,2,214],[4,4,256]],[[1,6],[2,9],[4,8],[8,6],[16,2],[32,1]]",4183));//R8R4_154=R8_14×R4_11、R8R4_553=R8_51×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,3,196,31,8,[1,4,13,6,8,0],[[2,2,144],[2,4,228],[4,2,200],[4,4,256]]",4191));//R8R4_165=R8_15×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,336,31,32,[1,4,9,10,8,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,256]],[[4,12],[8,12],[16,4],[32,4]]",4199));//R8R4_176=R8_16×R4_11、R8R4_453=R8_42×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,640,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,192]]",4208));//R8R4_191=R8_18×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,640,31,32,[1,15,16,0,0,0],[[2,4,128],[4,2,128],[4,4,128]]",4216));//R8R4_202=R8_19×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,448,31,8,[1,15,4,12,0,0],[[2,4,128],[4,2,192],[4,4,256]]",4224));//R8R4_213=R8_20×R4_4
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,15,432,31,32,[1,4,11,12,4,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,256]],[[4,4],[8,12],[16,12],[32,4]]",4201));//R8R4_181=R8_17×R4_5、R8R4_409=R8_38×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,480,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4209));//R8R4_192=R8_18×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,480,31,32,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]]",4217));//R8R4_203=R8_19×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,336,31,8,[1,7,12,4,8,0],[[2,2,64],[2,4,160],[4,2,208],[4,4,256]]",4225));//R8R4_214=R8_20×R4_5
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,480,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4210));//R8R4_193=R8_18×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,480,31,32,[1,8,15,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]]",4218));//R8R4_204=R8_19×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,336,31,8,[1,8,15,8,0,0],[[2,2,64],[2,4,160],[4,2,208],[4,4,256]]",4226));//R8R4_215=R8_20×R4_6
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,360,31,8,[1,8,11,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,256]],[[2,4],[4,8],[8,8],[16,8],[32,4]]",4203));//R8R4_183=R8_17×R4_7、R8R4_442=R8_41×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,3,15,400,31,2,[1,5,14,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,216]]",4211));//R8R4_194=R8_18×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,400,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,176]]",4219));//R8R4_205=R8_19×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,280,23,2,[1,9,18,4,0,0],[[2,2,96],[2,4,176],[4,2,216],[4,4,256]]",4227));//R8R4_216=R8_20×R4_7
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,360,31,8,[1,8,11,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,256]],[[4,6],[8,14],[16,10],[32,2]]",4204));//R8R4_184=R8_17×R4_8、R8R4_508=R8_47×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,3,15,400,31,2,[1,5,14,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,216]]",4212));//R8R4_195=R8_18×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,400,31,8,[1,9,14,8,0,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,176]]",4220));//R8R4_206=R8_19×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,280,31,2,[1,9,18,4,0,0],[[2,2,96],[2,4,176],[4,2,216],[4,4,256]]",4228));//R8R4_217=R8_20×R4_8
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,288,31,32,[1,6,11,10,4,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,256]],[[2,4],[4,10],[8,10],[16,6],[32,2]]",4205));//R8R4_185=R8_17×R4_9、R8R4_519=R8_48×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,320,31,8,[1,4,14,7,6,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,224]]",4213));//R8R4_196=R8_18×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,320,31,32,[1,8,12,7,4,0],[[2,2,128],[2,4,192],[4,2,192],[4,4,192]]",4221));//R8R4_207=R8_19×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,224,23,8,[1,8,12,11,0,0],[[2,2,128],[2,4,192],[4,2,224],[4,4,256]]",4229));//R8R4_218=R8_20×R4_9
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,1,3,324,31,32,[1,8,9,14,0,0],[[2,2,148],[2,4,148],[4,2,148],[4,4,256]],[[2,2],[4,8],[8,12],[16,8],[32,2]]",4206));//R8R4_186=R8_17×R4_10、R8R4_541=R8_50×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,4,1,7,360,31,8,[1,4,9,18,0,0],[[2,2,148],[2,4,148],[4,2,148],[4,4,220]]",4214));//R8R4_197=R8_18×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,360,31,32,[1,6,13,12,0,0],[[2,2,112],[2,4,184],[4,2,184],[4,4,184]]",4222));//R8R4_208=R8_19×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,12,3,3,252,27,8,[1,6,25,0,0,0],[[2,2,112],[2,4,184],[4,2,220],[4,4,256]]",4230));//R8R4_219=R8_20×R4_10
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,1,3,252,31,32,[1,4,7,10,6,4],[[2,2,172],[2,4,172],[4,2,172],[4,4,256]],[[2,6],[4,12],[8,8],[16,4],[32,2]]",4207));//R8R4_187=R8_17×R4_11、R8R4_552=R8_51×R4_2
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
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,336,31,32,[1,4,5,10,4,8],[[2,2,144],[2,4,144],[4,2,144],[4,4,256]],[[4,12],[8,12],[16,4],[32,4]]",4255));//R8R4_253=R8_23×R4_11
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,3,224,31,32,[1,4,5,10,4,8],[[2,2,144],[2,4,200],[4,2,200],[4,4,256]],[[2,12],[4,6],[8,10],[16,2],[32,2]]",4263));//R8R4_264=R8_24×R4_11=R8_14:[[7,7],[7,2]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,416,31,8,[1,8,10,11,2,0],[[2,2,152],[2,4,152],[4,2,152],[4,4,152]]",4271));//R8R4_529=R8_49×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,312,31,8,[1,8,10,11,2,0],[[2,2,152],[2,4,152],[4,2,152],[4,4,256]]",4272));//R8R4_530=R8_49×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,1,28,12,3,3,208,27,8,[1,8,18,5,0,0],[[2,2,152],[2,4,204],[4,2,204],[4,4,256]]",4273));//R8R4_531=R8_49×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,3,240,31,32,[1,2,3,8,6,12],[[2,2,196],[2,4,196],[4,2,196],[4,4,196]]",4283));//R8R4_562=R8_52×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,1,3,180,31,32,[1,2,3,8,6,12],[[2,2,196],[2,4,196],[4,2,196],[4,4,256]]",4283));//R8R4_563=R8_52×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,18,4,1,1,120,17,32,[1,2,5,6,6,12],[[2,2,196],[2,4,226],[4,2,226],[4,4,256]]",4285));//R8R4_564=R8_52×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,64]]",4289));//R8R4_298=R8_28×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,256]]",4290));//R8R4_299=R8_28×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,384,31,8,[1,11,6,10,4,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,256]]",4291));//R8R4_300=R8_28×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,15,15,576,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4295));//R8R4_320=R8_30×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,31,31,640,31,32,[1,15,16,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4298));//R8R4_331=R8_31×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,640,31,32,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4301));//R8R4_342=R8_32×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,15,512,31,32,[1,7,8,8,8,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]]",4304));//R8R4_353=R8_33×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,15,15,512,31,4,[1,11,12,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]]",4313));//R8R4_386=R8_36×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,448,31,8,[1,9,12,6,4,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,144]]",4316));//R8R4_397=R8_37×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,640,31,8,[1,3,16,12,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4322));//R8R4_419=R8_39×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,15,15,576,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4334));//R8R4_463=R8_43×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,448,31,8,[1,9,12,6,4,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,144]]",4337));//R8R4_474=R8_44×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,384,31,32,[1,8,12,5,6,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,160]]",4340));//R8R4_485=R8_45×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,320,31,32,[1,4,10,9,4,4],[[2,2,176],[2,4,176],[4,2,176],[4,4,176]]",4343));//R8R4_496=R8_46×R4_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,7,15,432,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,256]]",4296));//R8R4_321=R8_30×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,480,31,32,[1,15,16,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]]",4299));//R8R4_332=R8_31×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,480,31,32,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]]",4302));//R8R4_343=R8_32×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,15,384,31,32,[1,7,8,8,8,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,256]]",4305));//R8R4_354=R8_33×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,7,15,384,31,4,[1,11,12,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,256]]",4314));//R8R4_387=R8_36×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,336,31,8,[1,9,12,6,4,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,256]]",4317));//R8R4_398=R8_37×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,480,31,8,[1,3,16,12,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]]",4323));//R8R4_420=R8_39×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,7,15,432,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,256]]",4335));//R8R4_464=R8_43×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,336,31,8,[1,9,12,6,4,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,256]]",4338));//R8R4_475=R8_44×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,288,31,32,[1,8,12,5,6,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,256]]",4341));//R8R4_486=R8_45×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,15,240,31,32,[1,4,10,9,4,4],[[2,2,176],[2,4,176],[4,2,176],[4,4,256]]",4344));//R8R4_497=R8_46×R4_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,10,7,7,288,23,4,[1,11,14,6,0,0],[[2,2,112],[2,4,184],[4,2,184],[4,4,256]]",4297));//R8R4_322=R8_30×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,320,31,32,[1,15,2,14,0,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,256]]",4300));//R8R4_333=R8_31×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,320,31,32,[1,7,10,6,8,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,256]]",4303));//R8R4_344=R8_32×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,256,31,32,[1,7,2,14,0,8],[[2,2,128],[2,4,192],[4,2,192],[4,4,256]]",4306));//R8R4_355=R8_33×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,10,7,7,256,31,4,[1,11,14,6,0,0],[[2,2,128],[2,4,192],[4,2,192],[4,4,256]]",4315));//R8R4_388=R8_36×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,224,23,8,[1,9,10,12,0,0],[[2,2,144],[2,4,200],[4,2,200],[4,4,256]]",4318));//R8R4_399=R8_37×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,320,31,8,[1,3,14,2,12,0],[[2,2,96],[2,4,176],[4,2,176],[4,4,256]]",4324));//R8R4_421=R8_39×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,10,7,7,288,31,4,[1,11,14,6,0,0],[[2,2,112],[2,4,184],[4,2,184],[4,4,256]]",4336));//R8R4_465=R8_43×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,224,31,8,[1,9,10,12,0,0],[[2,2,144],[2,4,200],[4,2,200],[4,4,256]]",4339));//R8R4_476=R8_44×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,24,4,7,7,192,23,32,[1,8,8,15,0,0],[[2,2,160],[2,4,208],[4,2,208],[4,4,256]]",4342));//R8R4_487=R8_45×R4_3
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,24,4,3,7,160,23,32,[1,4,10,7,10,0],[[2,2,176],[2,4,216],[4,2,216],[4,4,256]]",4345));//R8R4_498=R8_46×R4_3
	/*---------------end of R32_4104~R32_4345----------------*/		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,128]],[[16,28],[32,4]]",4346));//R16_118×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,432,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,160]],[[8,14],[16,16],[32,2]]",4347));//R16_118×R2_2	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,432,31,8,[1,8,7,8,8,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,208]]",4348));//R16_155×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,128]]",4349));//R16_156×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,160],[4,2,112],[4,4,160]]",4350));//R16_156×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,432,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,160],[4,2,112],[4,4,208]]",4351));//R16_157×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,384,31,8,[1,4,11,12,4,0],[[2,2,112],[2,4,160],[4,2,112],[4,4,256]]",4352));//R16_158×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,384,31,8,[1,4,7,12,8,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,208]]",4353));//R16_159×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,192]]",4354));//R16_160×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,432,31,8,[1,4,11,12,4,0],[[2,2,112],[2,4,112],[4,2,160],[4,4,208]]",4355));//R16_160×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,192]]",4356));//R16_161×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,432,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,112],[4,2,160],[4,4,208]]",4357));//R16_161×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,512,31,32,[1,7,24,0,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,192]]",4358));//R16_162×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,4,15,12,0,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,208]]",4359));//R16_162×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,384,31,8,[1,4,7,12,8,0],[[2,2,112],[2,4,160],[4,2,112],[4,4,256]]",4360));//R16_163×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,128]]",4361));//R16_164×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,112],[4,2,160],[4,4,160]]",4362));//R16_164×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,432,31,8,[1,4,7,12,8,0],[[2,2,112],[2,4,160],[4,2,112],[4,4,208]]",4363));//R16_165×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,128]]",4364));//R16_166×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,160],[4,2,112],[4,4,160]]",4365));//R16_166×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,15,432,31,32,[1,4,7,12,8,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,160]]",4366));//R16_167×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,7,8,16,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,192]]",4367));//R16_168×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,432,31,8,[1,4,7,12,8,0],[[2,2,112],[2,4,112],[4,2,160],[4,4,208]]",4368));//R16_168×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,10,5,7,240,31,4,[1,6,23,2,0,0],[[2,2,112],[2,4,208],[4,2,208],[4,4,256]]",4369));//R16_169×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,256,31,32,[1,7,10,8,6,0],[[2,2,64],[2,4,224],[4,2,224],[4,4,256]]",4370));//R16_170×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,24,4,3,7,192,23,32,[1,4,11,16,0,0],[[2,2,112],[2,4,232],[4,2,232],[4,4,256]]",4371));//R16_170×R2_2	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,216,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,232],[4,2,208],[4,4,256]]",4372));//R16_174×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,216,23,8,[1,4,15,12,0,0],[[2,2,112],[2,4,208],[4,2,232],[4,4,256]]",4373));//R16_177×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,192,31,32,[1,7,2,12,6,4],[[2,2,128],[2,4,224],[4,2,224],[4,4,256]]",4374));//R16_179×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,24,4,3,3,144,23,32,[1,4,7,8,12,0],[[2,2,160],[2,4,232],[4,2,232],[4,4,256]]",4375));//R16_179×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,512,31,32,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,192]]",4376));//R16_183×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,4,11,12,4,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,208]]",4377));//R16_183×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,192]]",4378));//R16_184×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,432,31,8,[1,4,11,12,4,0],[[2,2,112],[2,4,112],[4,2,160],[4,4,208]]",4379));//R16_184×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,4,11,12,4,0],[[2,2,112],[2,4,112],[4,2,160],[4,4,160]]",4380));//R16_185×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,192]]",4381));//R16_186×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,432,31,8,[1,4,11,12,4,0],[[2,2,112],[2,4,160],[4,2,112],[4,4,208]]",4382));//R16_186×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,4,11,12,4,0],[[2,2,112],[2,4,160],[4,2,112],[4,4,160]]",4383));//R16_187×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,15,432,31,32,[1,4,7,12,8,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,256]]",4384));//R16_188×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,128]]",4385));//R16_189×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,288,31,8,[1,9,12,8,2,0],[[2,2,96],[2,4,176],[4,2,208],[4,4,256]]",4386));//R16_191×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,288,31,8,[1,9,12,8,2,0],[[2,2,96],[2,4,208],[4,2,176],[4,4,256]]",4387));//R16_192×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,1,28,12,3,3,216,27,8,[1,8,19,4,0,0],[[2,2,136],[2,4,220],[4,2,196],[4,4,256]]",4388));//R16_192×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,256,31,32,[1,7,10,4,6,4],[[2,2,64],[2,4,224],[4,2,224],[4,4,256]]",4389));//R16_193×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,24,4,3,7,192,23,32,[1,4,11,8,8,0],[[2,2,112],[2,4,232],[4,2,232],[4,4,256]]",4390));//R16_193×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,256]]",4391));//R16_194×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,432,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,256]]",4392));//R16_194×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,432,31,8,[1,4,7,12,8,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,256]]",4393));//R16_195×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,640,31,8,[1,7,8,16,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,192]]",4394));//R16_196×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,480,31,8,[1,4,7,12,8,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4395));//R16_196×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,640,31,32,[1,7,16,8,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,192]]",4396));//R16_197×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,480,31,32,[1,4,11,12,4,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4397));//R16_197×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,15,15,416,31,4,[1,15,8,8,0,0],[[2,4,128],[4,2,224],[4,4,256]]",4398));//R16_198×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,10,7,7,312,23,4,[1,8,23,0,0,0],[[2,2,64],[2,4,160],[4,2,232],[4,4,256]]",4399));//R16_198×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,15,16,0,0,0],[[2,4,128],[4,2,128],[4,4,128]]",4400));//R16_199×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,480,31,8,[1,8,15,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]]",4401));//R16_199×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,10,7,7,288,31,4,[1,8,23,0,0,0],[[2,2,64],[2,4,208],[4,2,208],[4,4,256]]",4402));//R16_201×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,15,512,31,8,[1,15,0,16,0,0],[[2,4,128],[4,2,128],[4,4,256]]",4403));//R16_202×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,7,384,31,8,[1,8,7,8,8,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,256]]",4404));//R16_202×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,24,4,7,7,240,23,32,[1,8,11,12,0,0],[[2,2,64],[2,4,232],[4,2,232],[4,4,256]]",4405));//R16_205×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,128]]",4406));//R16_228×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,112],[4,2,160],[4,4,160]]",4407));//R16_228×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,15,320,31,32,[1,7,8,0,16,0],[[2,2,64],[2,4,192],[4,2,192],[4,4,256]]",4408));//R16_229×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,240,31,32,[1,4,7,4,8,8],[[2,2,112],[2,4,208],[4,2,208],[4,4,256]]",4409));//R16_229×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,512,31,32,[1,15,0,16,0,0],[[2,4,128],[4,2,128],[4,4,256]]",4410));//R16_257×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,8,7,8,8,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,256]]",4411));//R16_257×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,8,7,8,8,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,256]]",4412));//R16_258×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,384,31,32,[1,8,7,0,8,8],[[2,2,64],[2,4,160],[4,2,160],[4,4,256]]",4413));//R16_260×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,432,31,8,[1,8,11,8,4,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,208]]",4414));//R16_261×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,15,8,8,0,0],[[2,4,128],[4,2,128],[4,4,192]]",4415));//R16_262×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,432,31,8,[1,8,11,8,4,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,208]]",4416));//R16_262×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,15,8,8,0,0],[[4,2,128],[4,4,256]]",4417));//R16_263×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,8,11,8,4,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,256]]",4418));//R16_263×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,576,31,8,[1,8,11,8,4,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,160]]",4419));//R16_264×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,264,23,8,[1,8,15,8,0,0],[[2,2,64],[2,4,208],[4,2,232],[4,4,256]]",4420));//R16_265×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,264,31,8,[1,8,15,8,0,0],[[2,2,64],[2,4,232],[4,2,208],[4,4,256]]",4421));//R16_266×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,512,31,32,[1,15,16,0,0,0],[[2,4,128],[4,2,128],[4,4,256]]",4422));//R16_267×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32,[1,8,15,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,256]]",4423));//R16_267×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,15,8,8,0,0],[[2,4,128],[4,4,256]]",4424));//R16_268×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,8,11,8,4,0],[[2,2,64],[2,4,160],[4,2,64],[4,4,256]]",4425));//R16_268×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,15,8,8,0,0],[[2,4,128],[4,4,128]]",4426));//R16_269×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,576,31,8,[1,8,11,8,4,0],[[2,2,64],[2,4,160],[4,2,64],[4,4,160]]",4427));//R16_269×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,640,31,32,[1,15,0,16,0,0],[[2,4,128],[4,2,128],[4,4,128]]",4428));//R16_270×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,480,31,32,[1,8,7,8,8,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]]",4429));//R16_270×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,3,28,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4430));//R16_171×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,2,15,14,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]]",4431));//R16_171×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,11,20,0,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,128]]",4432));//R16_172×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,6,15,10,0,0],[[2,2,112],[2,4,112],[4,2,160],[4,4,160]]",4433));//R16_172×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,576,31,32,[1,7,24,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,160]]",4434));//R16_173×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,432,31,32,[1,4,15,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,184]]",4435));//R16_173×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,3,28,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,160]]",4436));//R16_125×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,432,31,8,[1,2,15,14,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,184]]",4437));//R16_125×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,24,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4438));//R16_175×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,480,31,8,[1,4,15,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]]",4439));//R16_175×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,7,24,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,160]]",4440));//R16_176×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,15,432,31,8,[1,4,15,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,184]]",4441));//R16_176×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,15,448,31,32,[1,7,8,16,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,256]]",4442));//R16_181×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,336,31,32,[1,4,7,12,8,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,256]]",4443));//R16_181×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,15,15,416,31,4,[1,15,8,8,0,0],[[2,4,224],[4,2,128],[4,4,256]]",4444));//R16_178×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,10,7,7,312,31,4,[1,8,23,0,0,0],[[2,2,64],[2,4,232],[4,2,160],[4,4,256]]",4445));//R16_178×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,224]]",4446));//R16_140:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,128]]",4447));//R16_140:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,160]]",4448));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,576,31,32,[1,7,8,16,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,256]]",4449));//R16_188×R2_1=R32_4449
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]]",4450));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,128],[4,4,160]]",4451));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,31,31,640,31,32,[1,15,16,0,0,0],[[2,4,128],[4,2,128],[4,4,128]]",4452));//R16_234×R2_1=R32_4452	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,192],[4,4,192]]",4453));//R16_140:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,480,31,32,[1,8,15,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]]",4454));//R16_234×R2_2=R32_4454
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,192]]",4455));//R16_140:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4456));//R16_140:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,128],[4,4,160]]",4457));//R16_140:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,160]]",4458));//R16_140:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,15,8,8,0,0],[[2,4,192],[4,2,128],[4,4,192]]",4459));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,224]]",4460));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,192]]",4461));//R16_140:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,192],[4,2,128],[4,4,192]]",4462));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,3,24,4,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4463));//R16_141:[[0,0],[0,1]],[[0,0],[0,3]],[[0,0],[2,2]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,192]]",4464));//R16_141:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,3,28,0,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]]",4465));//R16_141:[[0,0],[0,1]],[[0,0],[0,2]],[[0,0],[6,3]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,192]]",4466));//R16_141:[[0,0],[0,1]],[[0,0],[0,3]],[[0,0],[6,2]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,224]]",4467));//R16_141:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,3,24,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]]",4468));//R16_141:[[0,0],[0,1]],[[0,0],[0,2]],[[0,0],[11,3]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,3,28,0,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4469));//R16_141:[[0,0],[0,1]],[[0,0],[0,3]],[[0,0],[11,2]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,224]]",4470));//R16_141:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,128],[4,2,96],[4,4,128]]",4471));//R16_141:[[0,0],[0,1]],[[0,0],[0,2]],[[0,6],[0,0]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,128],[4,4,160]]",4472));//R16_142:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,160]]",4473));//R16_142:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,192]]",4474));//R16_142:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,192]]",4475));//R16_142:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,160]]",4476));//R16_142:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]]",4477));//R16_142:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,128],[4,4,160]]",4478));//R16_142:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,128],[4,2,192],[4,4,192]]",4479));//R16_142:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,160],[4,2,128],[4,4,192]]",4480));//R16_142:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4481));//R16_143:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,144]]",4482));//R16_143:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]]",4483));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4484));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,144]]",4485));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]]",4486));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,160]]",4487));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,128]]",4488));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,160]]",4489));//R16_143:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,16,12,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,160]]",4490));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,192]]",4491));//R16_143:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,20,8,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,144]]",4492));//R16_143:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,160]]",4493));//R16_139:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,160],[4,2,128],[4,4,160]]",4494));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]]",4495));//R16_139:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,160]]",4496));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,160]]",4497));//R16_139:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,31,31,640,31,32,[1,15,16,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4498));//R16_131×R2_1=R32_4498
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,3,28,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,160]]",4499));//R16_141×R2_1=R32_4499
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,432,31,8,[1,2,15,14,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,184]]",4500));//R16_141×R2_2=R32_4500		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,11,20,0,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,128]]",4501));//R16_142×R2_1=R32_4501
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,432,31,8,[1,6,15,10,0,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,160]]",4502));//R16_142×R2_2=R32_4502
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,7,24,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4503));//R16_143×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,480,31,8,[1,4,15,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]]",4504));//R16_143×R2_2	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,15,16,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4505));//R16_144×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,480,31,8,[1,8,15,8,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,136]]",4506));//R16_144×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,15,16,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,160]]",4507));//R16_145×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,432,31,8,[1,8,15,8,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,184]]",4508));//R16_145×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,160],[4,2,64],[4,4,160]]",4509));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,160],[4,2,64],[4,4,160]]",4510));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4511));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,192]]",4512));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,192],[4,2,64],[4,4,192]]",4513));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,128]]",4514));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,160]]",4515));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,11,8,12,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,192]]",4516));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,11,8,12,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,256]]",4517));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,128],[4,2,96],[4,4,128]]",4518));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,192]]",4519));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,15,16,0,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,160]]",4520));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,11,16,4,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4521));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,15,8,8,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,192]]",4522));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,11,16,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]]",4523));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,7,24,0,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,192]]",4524));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,15,8,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]]",4525));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,192]]",4526));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,192],[4,2,64],[4,4,192]]",4527));//R16_156:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,24,0,0,0],[[2,2,64],[2,4,160],[4,2,64],[4,4,160]]",4528));//R16_156:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,192]]",4529));//R16_156:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,15,192,31,32,[1,7,8,0,0,16],[[2,2,128],[2,4,224],[4,2,224],[4,4,256]],[[2,16],[4,8],[8,4],[16,2],[32,2]]",4530));//R8_22:[[4,6],[5,4]]		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,11,15,272,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,192],[4,2,192],[4,4,256]]",4531));//R8_22:[[0,1],[5,6]],[[0,4],[0,2]]
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,224]]",4532));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,192]]",4533));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,224]]",4534));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,192],[4,2,128],[4,4,192]]",4535));//R16_140:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,768,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,64]]",4536));//R16_232×R2_1=R32_4536
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,224]]",4537));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,576,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4538));//R16_232×R2_2=R32_4538
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]]",4539));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,224]]",4540));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,15,8,8,0,0],[[2,4,128],[4,2,192],[4,4,192]]",4541));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,416,31,32,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,224]]",4542));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]]",4543));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,160]]",4544));//R16_140:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,256]]",4545));//R16_140:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,128],[4,4,192]]",4546));//R16_140:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,7,24,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,160]]",4547));//R16_146×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,432,31,8,[1,4,15,12,0,0],[[2,2,136],[2,4,136],[4,2,136],[4,4,184]]",4548));//R16_146×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,128]]",4549));//R16_182×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,432,31,8,[1,4,15,12,0,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,160]]",4550));//R16_182×R2_2	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,192]]",4551));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,11,20,0,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,144]]",4552));//R16_145:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,11,20,0,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4553));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,224]]",4554));//R16_146:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,416,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,240]]",4555));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,128]]",4556));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,24,0,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,160]]",4557));//R16_145:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4558));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,11,16,4,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4559));//R16_144:[[0,0],[0,1]],[[0,0],[0,2]],[[0,0],[6,3]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,11,20,0,0,0],[[2,2,112],[2,4,176],[4,2,112],[4,4,176]]",4560));//R16_145:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,160]]",4561));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,11,16,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,144]]",4562));//R16_144:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,11,16,4,0,0],[[2,2,112],[2,4,176],[4,2,112],[4,4,176]]",4563));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,16,8,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,192]]",4564));//R16_146:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,12,12,0,0],[[2,2,112],[2,4,176],[4,2,112],[4,4,208]]",4565));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,224]]",4566));//R16_144:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,11,8,12,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,160]]",4567));//R16_146:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,192]]",4568));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,160]]",4569));//R16_156:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,11,8,12,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,128]]",4570));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,128]]",4571));//R16_172:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,20,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,224]]",4572));//R16_274:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,3,24,4,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4573));//R16_274:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4574));//R16_274:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,11,4,16,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,160]]",4575));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]]",4576));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,128]]",4577));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,11,16,4,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4578));//R8_6:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,11,8,12,0,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,224]]",4579));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,416,31,8,[1,3,16,12,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,224]]",4580));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,160]]",4581));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,11,8,12,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,192]]",4582));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]]",4583));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,480,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]]",4584));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]]",4585));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,480,31,4,[1,3,28,0,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4586));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,11,16,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,144]]",4587));//R16_131:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,15,8,8,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,160]]",4588));//R16_131:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,11,8,12,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,224]]",4589));//R16_131:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,160]]",4590));//R16_131:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,11,8,12,0,0],[[2,2,112],[2,4,176],[4,2,112],[4,4,176]]",4591));//R16_131:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,16,8,0,0],[[2,2,112],[2,4,176],[4,2,112],[4,4,176]]",4592));//R16_146:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,20,8,0,0],[[2,2,112],[2,4,176],[4,2,112],[4,4,208]]",4593));//R16_146:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,224]]",4594));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,16,8,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,144]]",4595));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,176],[4,2,112],[4,4,176]]",4596));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,16,12,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,192]]",4597));//R16_146:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,224]]",4598));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,192]]",4599));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,7,16,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4600));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,192]]",4601));//R16_146:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,1,24,6,7,7,224,23,8,[1,9,12,10,0,0],[[2,2,96],[2,4,216],[4,2,232],[4,4,256]]",4602));//R8_14:[[5,4],[0,0]],[[6,0],[1,7]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,160]]",4603));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,192]]",4604));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,192]]",4605));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,128],[4,4,160]]",4606));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,64],[4,4,224]]",4607));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,7,24,0,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]]",4608));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,128],[4,4,192]]",4609));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,192],[4,4,192]]",4610));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,128],[4,2,96],[4,4,192]]",4611));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]]",4612));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,3,24,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,176]]",4613));//R16_141:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,176]]",4614));//R16_141:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,24,0,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]]",4615));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,11,16,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,144]]",4616));//R16_144:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,11,16,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,144]]",4617));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]]",4618));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,192]]",4619));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,448,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,192]]",4620));//R16_146:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,176]]",4621));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,480,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,176]]",4622));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,304,15,8,[1,15,4,12,0,0],[[2,4,224],[4,2,240],[4,4,256]]",4623));//R4_3:[[0,0,0,0],[0,0,0,0],[0,0,1,0],[2,3,0,3]],[[0,0,0,0],[0,0,0,0],[0,0,1,0],[2,3,2,1]]	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,448,31,32,[1,3,20,8,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,144]]",4624));//R16_132:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,3,16,12,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]]",4625));//R16_132:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4626));//R16_132:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,512,31,32,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]]",4627));//R16_132:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,7,24,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]]",4628));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,256]]",4629));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,224]]",4630));//R16_146:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,240]]",4631));//R16_146:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,448,31,4,[1,3,24,4,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,240]]",4632));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,448,31,4,[1,3,28,0,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,240]]",4633));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,192]]",4634));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,256]]",4635));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]]",4636));//R16_146:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,240]]",4637));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,256,23,2,[1,11,12,8,0,0],[[2,2,112],[2,4,216],[4,2,184],[4,4,256]]",4638));//R16_192:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,240,15,8,[1,7,12,4,8,0],[[2,2,64],[2,4,224],[4,2,240],[4,4,256]]",4639));//R16_193:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,2,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,192],[4,4,224]]",4640));//R16_169:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,23,448,31,2,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,192],[4,4,192]]",4641));//R16_169:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,240,15,8,[1,7,12,12,0,0],[[2,2,64],[2,4,224],[4,2,240],[4,4,256]]",4642));//R16_170:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,15,16,0,0,0],[[2,4,128],[4,2,192],[4,4,192]]",4643));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,11,20,0,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,224]]",4644));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,11,20,0,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,160]]",4645));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,64],[4,2,192],[4,4,192]]",4646));//R16_126:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]]",4647));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,64],[4,2,192],[4,4,192]]",4648));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]]",4649));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]]",4650));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,192]]",4651));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,224]]",4652));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,256]]",4653));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,7,15,240,31,2,[1,7,16,8,0,0],[[2,2,96],[2,4,240],[4,2,192],[4,4,256]]",4654));//R8_22:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,16,2,7,15,224,15,32,[1,7,10,6,8,0],[[2,2,64],[2,4,240],[4,2,240],[4,4,256]]",4655));//R8_22:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,544,31,32,[1,7,24,0,0,0],[[2,2,96],[2,4,128],[4,2,128],[4,4,128]]",4656));//R8_12×R8_12的商环
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,640,31,2,[1,11,20,0,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,128]]",4657));//R8_8×R8_8的商环
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,640,31,2,[1,11,20,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]]",4658));//R8_9×R8_9的商环	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,640,31,2,[1,11,20,0,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,128]]",4659));//R8_10×R8_10的商环	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,576,31,8,[1,11,20,0,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,128]]",4660));//R8_10×R8_11的商环	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,31,31,544,31,32,[1,15,16,0,0,0],[[2,2,96],[2,4,128],[4,2,128],[4,4,128]]",4661));//R8_11×R8_11的商环
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,544,31,32,[1,7,24,0,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,144]]",4662));//R8_11×R8_12的商环
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]]",4663));//R16_28×R16_129的商环	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,3,28,0,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4664));//R16_29×R16_146的商环		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,544,31,8,[1,7,24,0,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,144]]",4665));//R16_146×R16_346的商环	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,544,31,2,[1,11,20,0,0,0],[[2,2,96],[2,4,128],[4,2,128],[4,4,128]]",4666));//R16_246×R16_246的商环
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,16,2,7,15,160,15,32,[1,7,10,14,0,0],[[2,2,128],[2,4,240],[4,2,240],[4,4,256]]",4667));//R8_22:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,1,16,2,3,15,144,15,8,[1,3,14,14,0,0],[[2,2,144],[2,4,240],[4,2,240],[4,4,256]]",4668));//R8_22:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,16,2,3,15,144,15,32,[1,3,14,6,8,0],[[2,2,144],[2,4,240],[4,2,240],[4,4,256]]",4669));//R8_22:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,23,288,31,32,[1,7,16,8,0,0],[[2,2,128],[2,4,192],[4,2,192],[4,4,224]]",4670));//R8_22:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,320,31,4,[1,7,24,0,0,0],[[2,2,128],[2,4,192],[4,2,192],[4,4,192]]",4671));//R8_22:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,352,31,2,[1,7,16,8,0,0],[[2,2,96],[2,4,192],[4,2,160],[4,4,224]]",4672));//R8_22:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,23,384,31,2,[1,7,16,8,0,0],[[2,2,96],[2,4,192],[4,2,160],[4,4,192]]",4673));//R8_22:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,23,288,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,192],[4,2,192],[4,4,224]]",4674));//R8_22:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,352,31,2,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,192],[4,4,224]]",4675));//R8_22:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,23,384,31,2,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,192],[4,4,192]]",4676));//R8_22:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,3,15,208,15,2,[1,3,16,12,0,0],[[2,2,96],[2,4,224],[4,2,240],[4,4,256]]",4677));//R16_123:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,3,15,208,31,2,[1,3,16,12,0,0],[[2,2,96],[2,4,240],[4,2,224],[4,4,256]]",4678));//R16_123:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,208,15,8,[1,7,12,12,0,0],[[2,2,96],[2,4,224],[4,2,240],[4,4,256]]",4679));//R16_124:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,208,31,8,[1,7,12,12,0,0],[[2,2,96],[2,4,240],[4,2,224],[4,4,256]]",4680));//R16_124:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,224]]",4681));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,224]]",4682));//R16_126:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,20,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,224]]",4683));//R16_126:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,8,[1,3,28,0,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]]",4684));//R8_9×R8_19的商环		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,544,31,32,[1,15,16,0,0,0],[[2,2,96],[2,4,128],[4,2,128],[4,4,128]]",4685));//R8_19×R8_19的商环	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,224,31,8,[1,3,14,8,6,0],[[2,2,96],[2,4,224],[4,2,224],[4,4,256]]",4686));//R16_123×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,1,24,4,1,7,168,23,8,[1,2,11,18,0,0],[[2,2,136],[2,4,232],[4,2,232],[4,4,256]]",4687));//R16_123×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,224,31,32,[1,7,10,8,6,0],[[2,2,96],[2,4,224],[4,2,224],[4,4,256]]",4688));//R16_124×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,24,4,3,7,168,23,32,[1,4,11,16,0,0],[[2,2,136],[2,4,232],[4,2,232],[4,4,256]]",4689));//R16_124×R2_2	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,11,15,320,31,2,[1,11,12,8,0,0],[[2,2,64],[2,4,224],[4,2,160],[4,4,256]],[[4,8],[8,14],[16,9],[32,1]]",4690));//R16_192:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,11,15,256,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,224],[4,2,224],[4,4,256]],[[2,16],[8,6],[16,9],[32,1]]",4691));//R16_192:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,224]],[[8,16],[16,8],[32,8]]",4692));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,192],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",4693));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,224]],[[8,24],[16,4],[32,4]]",4694));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,384,31,8,[1,3,28,0,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,208]],[[8,24],[16,4],[32,4]]",4695));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]],[[8,12],[16,16],[32,4]]",4696));//R16_125:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,160]],[[8,16],[16,8],[32,8]]",4697));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,160]],[[8,16],[16,12],[32,4]]",4698));//R16_127:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,160],[4,2,128],[4,4,160]],[[8,8],[16,20],[32,4]]",4699));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,160],[4,2,128],[4,4,160]],[[8,8],[16,20],[32,4]]",4700));//R16_127:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,192],[4,2,64],[4,4,192]],[[8,8],[16,20],[32,4]]",4701));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,192],[4,2,128],[4,4,192]],[[8,16],[16,12],[32,4]]",4702));//R16_127:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,15,320,31,32,[1,7,8,16,0,0],[[2,2,128],[2,4,192],[4,2,192],[4,4,192]],[[4,8],[8,16],[16,6],[32,2]]",4703));//R8_22:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,15,320,31,8,[1,7,8,16,0,0],[[2,2,128],[2,4,192],[4,2,192],[4,4,192]],[[4,8],[8,16],[16,6],[32,2]]",4704));//R8_22:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,23,288,31,32,[1,7,16,8,0,0],[[2,2,128],[2,4,192],[4,2,192],[4,4,224]],[[4,16],[8,8],[16,6],[32,2]]",4705));//R8_22:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,23,288,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,192],[4,2,192],[4,4,224]],[[4,16],[8,8],[16,6],[32,2]]",4706));//R8_22:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,16,2,3,15,176,15,32,[1,3,14,6,8,0],[[2,2,112],[2,4,240],[4,2,240],[4,4,256]],[[1,16],[4,4],[8,8],[16,3],[32,1]]",4707));//R8_22:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,15,15,352,31,2,[1,15,16,0,0,0],[[2,4,192],[4,2,224],[4,4,256]],[[2,16],[16,12],[32,4]]",4708));//R4_3:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,16,8,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,144]],[[8,16],[16,8],[32,8]]",4709));//R16_173:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,3,20,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]],[[8,16],[16,8],[32,8]]",4710));//R16_173:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,24,0,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,192]],[[8,16],[16,12],[32,4]]",4711));//R16_173:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]],[[8,12],[16,16],[32,4]]",4712));//R16_173:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,3,24,4,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,192]],[[8,20],[16,8],[32,4]]",4713));//R16_173:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,3,20,8,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,208]],[[8,24],[16,4],[32,4]]",4714));//R16_173:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,7,24,0,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,192]],[[8,12],[16,16],[32,4]]",4715));//R16_173:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,176],[4,2,112],[4,4,208]],[[8,20],[16,8],[32,4]]",4716));//R16_173:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,240]],[[8,20],[16,8],[32,4]]",4717));//R16_173:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]],[[8,12],[16,16],[32,4]]",4718));//R16_173:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,3,28,0,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]],[[8,12],[16,16],[32,4]]",4719));//R16_173:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,176]],[[8,12],[16,16],[32,4]]",4720));//R16_173:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,3,28,0,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,176]],[[8,12],[16,16],[32,4]]",4721));//R16_173:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,192]],[[8,16],[16,8],[32,8]]",4722));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,3,28,0,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,176]],[[8,12],[16,16],[32,4]]",4723));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,3,16,12,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,144]],[[8,24],[32,8]]",4724));//R16_131:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,7,15,272,15,4,[1,7,16,8,0,0],[[2,2,64],[2,4,192],[4,2,240],[4,4,256]],[[1,16],[8,8],[16,4],[32,4]]",4725));//R16_177:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,11,15,272,31,2,[1,11,12,8,0,0],[[2,2,64],[2,4,240],[4,2,192],[4,4,256]],[[4,16],[8,8],[16,7],[32,1]]",4726));//R16_179:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,7,176,31,8,[1,7,4,12,8,0],[[2,2,128],[2,4,240],[4,2,224],[4,4,256]],[[2,16],[4,8],[8,4],[16,3],[32,1]]",4727));//R16_179:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,15,384,31,8,[1,7,8,16,0,0],[[2,2,64],[2,4,128],[4,2,192],[4,4,256]],[[4,16],[16,12],[32,4]]",4728));//R16_181:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,176]],[[8,12],[16,16],[32,4]]",4729));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,176]],[[8,12],[16,16],[32,4]]",4730));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,192]],[[8,16],[16,12],[32,4]]",4731));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,160]],[[8,8],[16,16],[32,8]]",4732));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,3,28,0,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]],[[8,12],[16,16],[32,4]]",4733));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]],[[8,12],[16,16],[32,4]]",4734));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,64],[4,4,160]],[[8,8],[16,16],[32,8]]",4735));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,3,28,0,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,176]],[[8,12],[16,16],[32,4]]",4736));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,128],[4,2,96],[4,4,128]],[[8,8],[16,16],[32,8]]",4737));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,176]],[[8,16],[16,12],[32,4]]",4738));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,224]],[[8,16],[16,8],[32,8]]",4739));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,224]],[[8,16],[16,8],[32,8]]",4740));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,224]],[[8,20],[16,8],[32,4]]",4741));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,3,28,0,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,208]],[[8,24],[16,4],[32,4]]",4742));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,144]],[[8,8],[16,20],[32,4]]",4743));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,128],[4,2,160],[4,4,160]],[[8,12],[16,16],[32,4]]",4744));//R16_176:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]],[[8,12],[16,16],[32,4]]",4745));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,11,8,12,0,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,160]],[[8,8],[16,16],[32,8]]",4746));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,384,31,4,[1,3,20,8,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,208]],[[8,24],[16,4],[32,4]]",4747));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,192]],[[8,20],[16,8],[32,4]]",4748));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,160]],[[8,16],[16,8],[32,8]]",4749));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,192]],[[8,12],[16,16],[32,4]]",4750));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,192]],[[8,20],[16,8],[32,4]]",4751));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,4,[1,3,28,0,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,208]],[[8,24],[16,4],[32,4]]",4752));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,224]],[[8,20],[16,8],[32,4]]",4753));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,3,28,0,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]],[[8,12],[16,16],[32,4]]",4754));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,176],[4,2,112],[4,4,208]],[[8,20],[16,8],[32,4]]",4755));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,20,8,0,0],[[2,2,112],[2,4,176],[4,2,112],[4,4,208]],[[8,20],[16,8],[32,4]]",4756));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,224]],[[8,16],[16,12],[32,4]]",4757));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,8,[1,3,20,8,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,160]],[[8,24],[32,8]]",4758));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,15,384,31,8,[1,7,8,8,8,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,224]],[[4,8],[8,12],[16,8],[32,4]]",4759));//R16_136:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,15,448,31,8,[1,7,8,16,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,192]],[[8,16],[16,12],[32,4]]",4760));//R16_121×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,7,336,31,8,[1,4,7,12,8,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,208]],[[4,8],[8,14],[16,8],[32,2]]",4761));//R16_121×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,15,448,31,8,[1,7,8,16,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,256]],[[8,16],[16,12],[32,4]]",4762));//R16_122×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,7,336,31,8,[1,4,7,12,8,0],[[2,2,112],[2,4,160],[4,2,160],[4,4,256]],[[4,8],[8,14],[16,8],[32,2]]",4763));//R16_122×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,15,384,31,8,[1,7,8,8,8,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,256]],[[4,8],[8,12],[16,8],[32,4]]",4764));//R16_121:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,15,384,31,8,[1,7,8,16,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,224]],[[4,16],[16,12],[32,4]]",4765));//R16_121:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,15,448,31,8,[1,7,8,8,8,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,160]],[[4,16],[16,8],[32,8]]",4766));//R16_121:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,15,448,31,8,[1,7,8,8,8,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,160]],[[8,16],[16,12],[32,4]]",4767));//R16_121:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,15,384,31,8,[1,7,8,8,8,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,224]],[[4,8],[8,12],[16,8],[32,4]]",4768));//R16_121:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,15,384,31,8,[1,7,8,16,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,224]],[[8,24],[16,4],[32,4]]",4769));//R16_121:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,15,448,31,8,[1,15,0,0,16,0],[[2,4,128],[4,2,192],[4,4,256]],[[4,16],[16,8],[32,8]]",4770));//R16_121:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,15,448,31,8,[1,7,8,16,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,160]],[[4,16],[16,8],[32,8]]",4771));//R16_120:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,15,512,31,8,[1,7,8,8,8,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]],[[8,16],[16,8],[32,8]]",4772));//R16_120:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,15,448,31,8,[1,7,8,16,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,160]],[[8,16],[16,12],[32,4]]",4773));//R16_120:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,192]],[[8,16],[16,8],[32,8]]",4774));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]],[[8,12],[16,16],[32,4]]",4775));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,192]],[[8,12],[16,16],[32,4]]",4776));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,224]],[[8,16],[16,8],[32,8]]",4777));//R16_182:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,15,448,31,8,[1,15,0,0,16,0],[[2,4,192],[4,2,128],[4,4,256]],[[8,16],[16,12],[32,4]]",4778));//R16_120:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,15,384,31,8,[1,15,0,0,16,0],[[2,4,192],[4,2,192],[4,4,256]],[[4,16],[16,12],[32,4]]",4779));//R16_120:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,3,24,4,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]],[[8,8],[16,16],[32,8]]",4780));//R16_133:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,576,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]],[[8,8],[16,16],[32,8]]",4781));//R16_133:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,240]],[[8,20],[16,8],[32,4]]",4782));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,12,12,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,240]],[[8,20],[16,8],[32,4]]",4783));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,64],[4,4,224]],[[8,8],[16,20],[32,4]]",4784));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,128],[4,2,96],[4,4,192]],[[8,8],[16,20],[32,4]]",4785));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,128],[4,2,96],[4,4,192]],[[8,8],[16,20],[32,4]]",4786));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,3,20,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]],[[8,16],[16,8],[32,8]]",4787));//R16_176:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,128]],[[8,8],[16,16],[32,8]]",4788));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]],[[8,16],[16,8],[32,8]]",4789));//R16_146:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,224]],[[8,16],[16,12],[32,4]]",4790));//R16_146:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,384,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,224]],[[8,24],[16,4],[32,4]]",4791));//R16_146:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,192],[4,4,192]],[[8,24],[32,8]]",4792));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,11,8,12,0,0],[[2,2,64],[2,4,64],[4,2,192],[4,4,192]],[[8,16],[16,8],[32,8]]",4793));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,224]],[[8,16],[16,8],[32,8]]",4794));//R16_172:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,11,16,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,176]],[[8,12],[16,16],[32,4]]",4795));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,11,8,12,0,0],[[2,2,112],[2,4,112],[4,2,176],[4,4,208]],[[8,20],[16,8],[32,4]]",4796));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,12,12,0,0],[[2,2,112],[2,4,112],[4,2,176],[4,4,208]],[[8,20],[16,8],[32,4]]",4797));//R16_172:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,160]],[[8,16],[16,8],[32,8]]",4798));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,160]],[[8,16],[16,8],[32,8]]",4799));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,64],[4,2,192],[4,4,192]],[[8,16],[16,8],[32,8]]",4800));//R16_172:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,128],[4,2,192],[4,4,192]],[[8,16],[16,12],[32,4]]",4801));//R16_172:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,15,8,8,0,0],[[2,4,192],[4,2,128],[4,4,192]],[[8,8],[16,20],[32,4]]",4802));//R16_172:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]],[[8,8],[16,20],[32,4]]",4803));//R16_172:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]],[[8,12],[16,16],[32,4]]",4804));//R16_172:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,480,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,128],[4,2,160],[4,4,160]],[[8,12],[16,16],[32,4]]",4805));//R16_172:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,128],[4,2,160],[4,4,160]],[[8,12],[16,16],[32,4]]",4806));//R16_172:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,7,20,4,0,0],[[2,2,96],[2,4,128],[4,2,160],[4,4,160]],[[8,12],[16,16],[32,4]]",4807));//R16_172:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,24,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,144]],[[8,16],[16,8],[32,8]]",4808));//R16_171:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,3,28,0,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]],[[8,8],[16,16],[32,8]]",4809));//R16_171:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,3,24,4,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]],[[8,8],[16,16],[32,8]]",4810));//R16_171:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]],[[8,16],[16,8],[32,8]]",4811));//R16_171:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,15,416,31,8,[1,3,12,16,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,176]],[[8,16],[16,14],[32,2]]",4812));//R16_121×R16_121的商环:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,15,416,31,8,[1,7,8,16,0,0],[[2,2,96],[2,4,128],[4,2,128],[4,4,256]],[[8,16],[16,14],[32,2]]",4813));//R16_121×R16_122的商环:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,11,8,12,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,192]],[[8,16],[16,8],[32,8]]",4814));//R16_130:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,15,512,31,8,[1,7,8,16,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]],[[8,16],[16,8],[32,8]]",4815));//R16_120×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,7,7,384,31,8,[1,4,7,12,8,0],[[2,2,160],[2,4,160],[4,2,160],[4,4,160]],[[4,8],[8,12],[16,8],[32,4]]",4816));//R16_120×R2_2	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,128]],[[8,16],[32,16]]",4817));//R16_130:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,15,448,31,8,[1,3,12,16,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,144]],[[8,16],[16,12],[32,4]]",4818));//R16_120×R16_34的商环
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,15,416,31,8,[1,7,8,16,0,0],[[2,2,128],[2,4,160],[4,2,160],[4,4,160]],[[8,16],[16,14],[32,2]]",4819));//R16_121×R16_34的商环
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,384,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,224]],[[8,24],[16,4],[32,4]]",4820));//R16_172:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,15,8,8,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,160]],[[8,16],[16,8],[32,8]]",4821));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,11,16,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,144]],[[8,16],[16,8],[32,8]]",4822));//R16_144:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,11,8,12,0,0],[[2,2,112],[2,4,176],[4,2,112],[4,4,176]],[[8,24],[32,8]]",4823));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,16,8,0,0],[[2,2,128],[2,4,160],[4,2,128],[4,4,160]],[[8,24],[32,8]]",4824));//R16_144:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,11,16,4,0,0],[[2,2,112],[2,4,176],[4,2,112],[4,4,176]],[[8,24],[32,8]]",4825));//R16_144:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,11,8,12,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,144]],[[8,16],[16,8],[32,8]]",4826));//R16_144:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,11,8,12,0,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,224]],[[8,16],[16,8],[32,8]]",4827));//R16_144:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,24,0,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]],[[8,16],[16,8],[32,8]]",4828));//R16_144:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,128],[4,2,96],[4,4,128]],[[8,8],[16,16],[32,8]]",4829));//R16_156:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,24,0,0,0],[[2,2,64],[2,4,160],[4,2,128],[4,4,160]],[[8,8],[16,20],[32,4]]",4830));//R16_156:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]],[[8,16],[16,8],[32,8]]",4831));//R16_125:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,11,16,4,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]],[[8,12],[16,16],[32,4]]",4832));//R16_125:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,224]],[[8,8],[16,20],[32,4]]",4833));//R16_125:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,512,31,8,[1,11,16,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,144]],[[8,8],[16,20],[32,4]]",4834));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,448,31,8,[1,11,20,0,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,176]],[[8,16],[16,12],[32,4]]",4835));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,31,31,448,31,32,[1,15,16,0,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,160]],[[8,16],[16,12],[32,4]]",4836));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,128]],[[16,24],[32,8]]",4837));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,128]],[[16,24],[32,8]]",4838));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,192]],[[8,8],[16,20],[32,4]]",4839));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,192]],[[8,8],[16,20],[32,4]]",4840));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,8,[1,7,24,0,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,160]],[[8,16],[16,8],[32,8]]",4841));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,24,0,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,144]],[[8,16],[16,8],[32,8]]",4842));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,11,16,4,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,176]],[[8,16],[16,12],[32,4]]",4843));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,192]],[[8,16],[16,8],[32,8]]",4844));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,7,24,0,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,176]],[[8,16],[16,12],[32,4]]",4845));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,176]],[[8,16],[16,12],[32,4]]",4846));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,448,31,8,[1,15,8,8,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,160]],[[8,16],[16,12],[32,4]]",4847));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,144]],[[8,8],[16,20],[32,4]]",4848));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,640,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]],[[16,24],[32,8]]",4849));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,640,31,8,[1,15,8,8,0,0],[[2,4,128],[4,2,128],[4,4,128]],[[16,24],[32,8]]",4850));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,240]],[[8,16],[16,12],[32,4]]",4851));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,7,24,0,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,160]],[[8,12],[16,16],[32,4]]",4852));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,11,16,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,176]],[[8,12],[16,16],[32,4]]",4853));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,24,0,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,176]],[[8,16],[16,12],[32,4]]",4854));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,11,8,12,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,192]],[[8,12],[16,16],[32,4]]",4855));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,192],[4,4,192]],[[8,16],[16,12],[32,4]]",4856));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,192]],[[8,12],[16,16],[32,4]]",4857));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,192]],[[8,12],[16,16],[32,4]]",4858));
 	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",4859));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,128],[4,4,192]],[[8,16],[16,12],[32,4]]",4860));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,480,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,128],[4,4,160]],[[8,12],[16,16],[32,4]]",4861));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,160],[4,2,128],[4,4,192]],[[8,16],[16,12],[32,4]]",4862));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",4863));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",4864));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,192],[4,4,192]],[[8,24],[16,4],[32,4]]",4865));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,384,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,192],[4,4,192]],[[8,24],[16,4],[32,4]]",4866));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",4867));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,24,4,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,160]],[[8,16],[16,12],[32,4]]",4868));
 	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,160],[4,2,160],[4,4,160]],[[8,20],[16,8],[32,4]]",4869));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,7,16,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]],[[8,12],[16,16],[32,4]]",4870));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,11,8,12,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,176]],[[8,12],[16,16],[32,4]]",4871));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,192]],[[8,16],[16,12],[32,4]]",4872));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,384,31,8,[1,3,20,8,0,0],[[2,2,128],[2,4,160],[4,2,160],[4,4,192]],[[8,24],[16,4],[32,4]]",4873));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,24,0,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,160]],[[8,16],[16,8],[32,8]]",4874));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,128],[4,2,160],[4,4,160]],[[8,12],[16,16],[32,4]]",4875));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]],[[8,8],[16,16],[32,8]]",4876));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,28,0,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]],[[8,8],[16,16],[32,8]]",4877));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,64],[4,2,160],[4,4,160]],[[8,8],[16,16],[32,8]]",4878));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,64],[4,2,128],[4,4,192]],[[8,8],[16,16],[32,8]]",4879));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,11,8,12,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,256]],[[8,8],[16,16],[32,8]]",4880));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,3,24,4,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]],[[8,8],[16,16],[32,8]]",4881));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,160],[4,2,64],[4,4,160]],[[8,8],[16,16],[32,8]]",4882));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,160]],[[8,16],[16,8],[32,8]]",4883));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,144]],[[8,16],[16,8],[32,8]]",4884));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,15,8,8,0,0],[[2,4,192],[4,2,128],[4,4,192]],[[8,8],[16,20],[32,4]]",4885));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,192],[4,2,128],[4,4,192]],[[8,16],[16,12],[32,4]]",4886));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,160],[4,2,64],[4,4,224]],[[8,8],[16,20],[32,4]]",4887));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,128],[4,4,128]],[[8,8],[16,16],[32,8]]",4888));
 	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,192]],[[8,8],[16,16],[32,8]]",4889));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,160],[4,2,64],[4,4,160]],[[8,8],[16,16],[32,8]]",4890));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,15,8,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]],[[8,8],[16,20],[32,4]]",4891));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,11,8,12,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,144]],[[8,8],[16,20],[32,4]]",4892));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,112]],[[8,8],[16,16],[32,8]]",4893));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,8,[1,15,0,16,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,192]],[[8,12],[16,16],[32,4]]",4894));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,11,8,12,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,208]],[[8,16],[16,12],[32,4]]",4895));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,11,4,16,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,208]],[[8,12],[16,16],[32,4]]",4896));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,11,8,12,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,192]],[[8,12],[16,16],[32,4]]",4897));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,11,8,12,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,192]],[[8,16],[16,12],[32,4]]",4898));
 	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,11,8,12,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,160]],[[8,12],[16,16],[32,4]]",4899));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,11,4,16,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]],[[8,12],[16,16],[32,4]]",4900));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,11,8,12,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,144]],[[8,12],[16,16],[32,4]]",4901));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,208]],[[8,16],[16,12],[32,4]]",4902));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,11,8,12,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,208]],[[8,20],[16,8],[32,4]]",4903));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,15,8,8,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,160]],[[8,16],[16,12],[32,4]]",4904));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,15,0,16,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",4905));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,512,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,160],[4,2,64],[4,4,224]],[[8,8],[16,20],[32,4]]",4906));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,224]],[[8,16],[16,12],[32,4]]",4907));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,3,24,4,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,128]],[[8,8],[16,20],[32,4]]",4908));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,144]],[[8,8],[16,20],[32,4]]",4909));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,3,28,0,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,144]],[[8,8],[16,20],[32,4]]",4910));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,1,24,6,7,7,200,23,8,[1,9,10,12,0,0],[[2,2,144],[2,4,224],[4,2,200],[4,4,256]],[[1,8],[2,4],[4,8],[8,7],[16,4],[32,1]]",4911));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,1,24,6,7,7,200,23,8,[1,9,10,12,0,0],[[2,2,144],[2,4,200],[4,2,224],[4,4,256]],[[1,8],[2,4],[4,8],[8,7],[16,4],[32,1]]",4912));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,15,384,31,32,[1,15,0,16,0,0],[[2,4,192],[4,2,192],[4,4,256]],[[4,16],[16,12],[32,4]]",4913));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,256,31,8,[1,11,6,8,6,0],[[2,2,64],[2,4,224],[4,2,224],[4,4,256]],[[2,16],[8,8],[16,6],[32,2]]",4914));//R16_119×R2_1
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,1,24,4,5,7,192,23,8,[1,6,11,14,0,0],[[2,2,112],[2,4,232],[4,2,232],[4,4,256]],[[1,8],[2,8],[4,4],[8,7],[16,4],[32,1]]",4915));//R16_119×R2_2
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,7,15,224,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,224],[4,2,224],[4,4,256]],[[2,16],[8,10],[16,5],[32,1]]",4916));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,11,15,240,15,2,[1,11,8,12,0,0],[[2,2,64],[2,4,224],[4,2,240],[4,4,256]],[[1,16],[8,8],[16,6],[32,2]]",4917));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,11,15,240,31,2,[1,11,8,12,0,0],[[2,2,64],[2,4,240],[4,2,224],[4,4,256]],[[2,16],[8,8],[16,7],[32,1]]",4918));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,15,15,336,15,4,[1,15,8,8,0,0],[[2,4,192],[4,2,240],[4,4,256]],[[1,16],[16,12],[32,4]]",4919));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,15,15,336,31,4,[1,15,8,8,0,0],[[2,4,240],[4,2,192],[4,4,256]],[[4,16],[16,15],[32,1]]",4920));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,256]],[[8,16],[16,8],[32,8]]",4921));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,192]],[[8,12],[16,16],[32,4]]",4922));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,11,12,8,0,0],[[2,2,64],[2,4,64],[4,2,192],[4,4,192]],[[8,16],[16,8],[32,8]]",4923));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,160],[4,4,160]],[[8,16],[16,8],[32,8]]",4924));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,128]],[[8,8],[16,16],[32,8]]",4925));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,11,16,4,0,0],[[2,2,64],[2,4,128],[4,2,192],[4,4,192]],[[8,24],[32,8]]",4926));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,192]],[[8,16],[16,8],[32,8]]",4927));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,224]],[[8,24],[32,8]]",4928));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,192],[4,4,192]],[[8,24],[32,8]]",4929));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,192],[4,4,192]],[[8,24],[32,8]]",4930));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,192]],[[8,16],[16,8],[32,8]]",4931));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,224]],[[8,24],[32,8]]",4932));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,192]],[[8,16],[16,8],[32,8]]",4933));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,224]],[[8,24],[32,8]]",4934));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]],[[8,16],[16,8],[32,8]]",4935));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,11,4,16,0,0],[[2,2,64],[2,4,64],[4,2,64],[4,4,256]],[[8,8],[16,16],[32,8]]",4936));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,128],[4,2,96],[4,4,128]],[[8,8],[16,16],[32,8]]",4937));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,576,31,4,[1,11,8,12,0,0],[[2,2,64],[2,4,128],[4,2,64],[4,4,192]],[[8,8],[16,16],[32,8]]",4938));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,232,23,2,[1,11,12,8,0,0],[[2,2,128],[2,4,192],[4,2,216],[4,4,256]],[[1,8],[4,12],[8,6],[16,4],[32,2]]",4939));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,7,8,16,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,224]],[[8,16],[16,8],[32,8]]",4940));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,640,31,8,[1,3,24,4,0,0],[[2,2,96],[2,4,96],[4,2,96],[4,4,96]],[[16,24],[32,8]]",4941));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,224]],[[8,20],[16,8],[32,4]]",4942));//R16_176:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,256]],[[8,24],[32,8]]",4943));//R16_146:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,15,15,352,31,2,[1,15,16,0,0,0],[[2,4,224],[4,2,192],[4,4,256]],[[4,16],[16,14],[32,2]]",4944));//R8_14:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,3,24,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]],[[8,12],[16,16],[32,4]]",4945));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,3,28,0,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,176]],[[8,12],[16,16],[32,4]]",4946));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,4,[1,3,24,4,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,192]],[[8,12],[16,16],[32,4]]",4947));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,20,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,224]],[[8,20],[16,8],[32,4]]",4948));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,576,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,96],[4,2,128],[4,4,128]],[[8,8],[16,16],[32,8]]",4949));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,28,0,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,176]],[[8,16],[16,12],[32,4]]",4950));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,208]],[[8,20],[16,8],[32,4]]",4951));//R16_125:
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,24,4,0,0],[[2,2,96],[2,4,160],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",4952));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,16,8,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,176]],[[8,16],[16,12],[32,4]]",4953));//R16_125:	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,11,15,288,31,2,[1,11,20,0,0,0],[[2,2,64],[2,4,192],[4,2,224],[4,4,256]],[[2,16],[8,4],[16,10],[32,2]]",4954));//R16_119:		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,144]],[[8,16],[16,8],[32,8]]",4955));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,3,16,12,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,160]],[[8,16],[16,8],[32,8]]",4956));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,24,4,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,160]],[[8,16],[16,8],[32,8]]",4957));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,20,8,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,160]],[[8,16],[16,8],[32,8]]",4958));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,8,16,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,224]],[[8,24],[32,8]]",4959));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,3,20,8,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,144]],[[8,24],[32,8]]",4960));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,16,8,0,0],[[2,2,112],[2,4,176],[4,2,112],[4,4,176]],[[8,24],[32,8]]",4961));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,512,31,4,[1,3,16,12,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]],[[8,8],[16,20],[32,4]]",4962));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,24,4,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]],[[8,8],[16,20],[32,4]]",4963));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,512,31,4,[1,3,20,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,160]],[[8,8],[16,20],[32,4]]",4964));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,3,16,12,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,160]],[[8,16],[16,12],[32,4]]",4965));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,448,31,8,[1,11,8,12,0,0],[[2,2,64],[2,4,160],[4,2,128],[4,4,224]],[[8,16],[16,12],[32,4]]",4966));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,12,12,0,0],[[2,2,96],[2,4,128],[4,2,96],[4,4,256]],[[8,16],[16,12],[32,4]]",4967));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,8,16,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,224]],[[8,16],[16,12],[32,4]]",4968));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,16,8,0,0],[[2,2,112],[2,4,112],[4,2,176],[4,4,176]],[[8,16],[16,12],[32,4]]",4969));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,176]],[[8,16],[16,12],[32,4]]",4970));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,112],[4,2,176],[4,4,176]],[[8,16],[16,12],[32,4]]",4971));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,416,31,8,[1,7,16,8,0,0],[[2,2,128],[2,4,160],[4,2,160],[4,4,160]],[[8,20],[16,8],[32,4]]",4972));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,448,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,176],[4,2,112],[4,4,176]],[[8,16],[16,12],[32,4]]",4973));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,20,4,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,176]],[[8,16],[16,12],[32,4]]",4974));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,11,12,8,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,176]],[[8,16],[16,12],[32,4]]",4975));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,224]],[[8,16],[16,12],[32,4]]",4976));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,128],[4,4,192]],[[8,16],[16,12],[32,4]]",4977));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,4,[1,7,20,4,0,0],[[2,2,96],[2,4,160],[4,2,96],[4,4,224]],[[8,16],[16,12],[32,4]]",4978));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,416,31,8,[1,3,12,16,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,208]],[[8,20],[16,8],[32,4]]",4979));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,416,31,8,[1,3,16,12,0,0],[[2,2,112],[2,4,176],[4,2,144],[4,4,176]],[[8,20],[16,8],[32,4]]",4980));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,16,8,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,208]],[[8,20],[16,8],[32,4]]",4981));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,416,31,8,[1,3,16,12,0,0],[[2,2,112],[2,4,144],[4,2,176],[4,4,176]],[[8,20],[16,8],[32,4]]",4982));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,448,31,4,[1,3,12,16,0,0],[[2,2,112],[2,4,112],[4,2,112],[4,4,240]],[[8,16],[16,12],[32,4]]",4983));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,448,31,4,[1,3,16,12,0,0],[[2,2,112],[2,4,112],[4,2,144],[4,4,208]],[[8,16],[16,12],[32,4]]",4984));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,384,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,144],[4,2,144],[4,4,240]],[[8,24],[16,4],[32,4]]",4985));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,448,31,4,[1,3,16,12,0,0],[[2,2,112],[2,4,144],[4,2,112],[4,4,208]],[[8,16],[16,12],[32,4]]",4986));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,3,20,8,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,176]],[[8,20],[16,8],[32,4]]",4987));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,19,31,512,31,4,[1,15,8,8,0,0],[[2,4,128],[4,2,192],[4,4,192]],[[8,16],[16,8],[32,8]]",4988));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,8,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,224]],[[8,20],[16,8],[32,4]]",4989));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,192],[4,4,192]],[[8,20],[16,8],[32,4]]",4990));		
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,448,31,4,[1,7,16,8,0,0],[[2,2,64],[2,4,160],[4,2,160],[4,4,192]],[[8,16],[16,12],[32,4]]",4991));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,384,31,8,[1,3,20,8,0,0],[[2,2,144],[2,4,144],[4,2,144],[4,4,208]],[[8,24],[16,4],[32,4]]",4992));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,416,31,4,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",4993));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,15,31,480,31,4,[1,7,24,0,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,192]],[[8,12],[16,16],[32,4]]",4994));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,448,31,8,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,176],[4,4,176]],[[8,16],[16,12],[32,4]]",4995));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,480,31,8,[1,7,16,8,0,0],[[2,2,96],[2,4,96],[4,2,160],[4,4,192]],[[8,12],[16,16],[32,4]]",4996));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,7,16,8,0,0],[[2,2,128],[2,4,128],[4,2,160],[4,4,192]],[[8,20],[16,8],[32,4]]",4997));	
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,416,31,4,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,176],[4,4,208]],[[8,20],[16,8],[32,4]]",4998));
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,416,31,4,[1,3,20,8,0,0],[[2,2,112],[2,4,112],[4,2,176],[4,4,208]],[[8,20],[16,8],[32,4]]",4999));	
	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,16,2,3,7,112,15,32,[1,4,7,4,8,8],[[2,2,912]]",5000));//R2_2/([0,0,0,0,0,1])=R2_2/([1,1,0,0,1,1])=R32_5000
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,24,4,3,3,144,23,32,[1,6,9,4,12,0],[[2,2,880]]",5001));//R16_385×R2_2=R32_5001=R2_2/([0,0,0,0,1,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,17,4,0,0,93,16,32,[1,3,2,2,12,12],[[2,2,931]]",5002));//R16_390×R2_2=R32_5002=R2_2/([0,1,1,1,1,1])=R2_2/([1,0,0,0,0,1])=R32_5002	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,20,4,3,3,120,19,32,[1,6,11,2,6,6],[[2,2,904]]",5003));//R16_383×R2_2=R32_5003=R2_2/([1,1,1,1,1,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,1,2,0,0,63,0,32,[1,1,0,0,0,30],[[2,2,961]]",5004));//R2_2/([1,0,1,1,1,1])=R2_2/([1,1,1,1,0,1])=R2_2/([1,1,1,0,1,1])=R32_5004
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,1,1,124,31,32,[1,2,3,2,12,12],[[2,2,900]]",5005));//R16_390×R2_1=R32_5005=R2_2/([[0,0,0,0,1,1]],[1,1,0,0,0,1,1])
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,7,160,31,32,[1,8,6,5,6,6],[[2,2,864]]",5006));//R16_383×R2_1=R32_5006=R2_2/([[0,0,0,1,1]],[1,0,0,0,0,0,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,7,15,192,31,32,[1,7,8,0,0,16],[[2,2,832]],[[2,16],[4,8],[8,4],[16,2],[32,2]]",5007));//R2_2/([[0,1]],[0,0,0,0,0,0,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,16,2,7,15,160,15,32,[1,8,15,8,0,0],[[2,2,864]]",5008));//R2_2/([[1,0,0,1],[1,0,1]],[0,0,0,0,0,0,1])		
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,13,5,7,264,31,1,[1,17,14,0,0,0],[[2,2,760]]",5009));//[[0,1,0,1],[0,0,0,0],[0,0,0,1],[0,1,0,1]],[[0,0,0,1],[0,0,0,1],[0,0,0,1],[0,0,0,1]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,26,16,3,3,174,25,4,[1,18,11,2,0,0],[[2,2,850]]",5010));//R16_300×R2_2=R32_5010=[[1,1,1,1],[1,1,1,1],[1,1,1,1],[1,1,1,1]],[[0,1,0,1],[0,0,0,0],[0,1,0,0],[0,1,0,1]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,24,10,5,7,208,23,2,[1,14,15,2,0,0],[[2,2,816]]",5011));//[[0,1,0,1],[0,0,0,0],[0,1,0,1],[0,1,0,1]],[[1,0,1,0],[1,1,1,1],[1,1,1,0],[1,0,1,0]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,24,10,5,7,192,23,4,[1,14,15,2,0,0],[[2,2,832]]",5012));//[[0,0,0,1],[0,0,0,1],[0,0,0,1],[0,0,0,1]],[[0,1,0,1],[0,0,0,0],[1,1,1,0],[0,1,0,1]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,28,18,3,3,236,27,2,[1,20,11,0,0,0],[[2,2,788]]",5013));//[[0,1,0,1],[0,0,0,0],[1,1,1,0],[0,1,0,1]],[[0,1,0,0],[0,0,0,1],[0,0,0,0],[0,1,0,0]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,20,10,3,3,180,19,2,[1,12,11,8,0,0],[[2,2,844]]",5014));//[[0,1,0,1],[0,1,0,1],[0,1,0,1],[0,1,0,1]],[[1,0,1,0],[1,1,1,1],[0,0,0,1],[1,0,1,0]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,7,240,31,32,[1,4,7,4,8,8],[[2,2,784]],[[2,8],[4,12],[8,6],[16,4],[32,2]]",5015));//R16_367×R2_2=R32_5015=R2_2/([[0,1]],[0,0,0,0,0,1,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,24,4,3,7,192,23,32,[1,6,13,12,0,0],[[2,2,832]]",5016));//R16_377×R2_2=R32_5016=R2_2/([[1,0,1],[1,0,1,1]],[0,0,0,0,0,1,1])
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,7,192,31,32,[1,8,4,11,4,4],[[2,2,832]]",5017));//R16_385×R2_1=R32_5017=R2_2/([[1,1]],[0,0,0,0,1,0,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,16,2,7,7,160,15,32,[1,8,7,8,8,0],[[2,2,864]]",5018));//R2_2/([[1,0,0,0,1,1],[1,1,0,0,0,1]],[0,0,0,0,0,1,1])
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,13,5,7,264,23,1,[1,17,14,0,0,0],[[2,2,760]],[[1,8],[4,8],[8,12],[32,4]]",5019));//[[0,0,0,0],[0,0,0,0],[1,1,1,1],[0,0,0,0]],[[1,1,1,0],[1,1,1,0],[1,0,1,1],[1,1,1,0]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,240,15,8,[1,9,14,8,0,0],[[2,2,784]]",5020));//[[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1]],[[0,0,0,0,1,0],[0,0,0,1,0,0],[1,0,0,0,1,1],[0,0,0,1,0,0],[1,0,0,1,0,1],[0,1,0,1,0,1]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,8,7,7,232,31,4,[1,14,12,5,0,0],[[2,2,792]]",5021));//R16_300×R2_1=R32_5021=J4:[[1,1],[1,1]],[[1,3],[3,3]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,16,2,3,15,144,15,32,[1,4,15,12,0,0],[[2,2,880]]",5022));//R2_2/([[0,0,0,1],[0,1,1]],[0,1,1,0,1,0,1,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,19,23,416,31,4,[1,19,4,8,0,0],[[2,2,608]]",5023));//I4:[[0,2],[0,0]],[[3,3],[3,3]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,16,2,7,7,160,15,8,[1,8,7,8,8,0],[[2,2,864]]",5024));//I4:[[1,1],[3,1]],[[3,3],[0,0]]	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,15,15,384,31,4,[1,19,8,4,0,0],[[2,2,640]]",5025));//R16_358×R2_1=R32_5025=I4:[[2,0],[0,0]],[[3,3],[3,3]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,24,10,7,7,208,23,4,[1,16,15,0,0,0],[[2,2,816]]",5026));//I4:[[1,0],[0,0]],[[3,3],[3,1]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,24,6,7,7,200,23,8,[1,12,15,4,0,0],[[2,2,824]]",5027));//I4:[[0,0],[0,1]],[[2,0],[3,3]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,15,336,31,32,[1,4,15,12,0,0],[[2,2,688]]",5028));//R16_362×R2_2=R32_5028=R2_2/([[0,0,0,0,1,1,1],[1,0,0,1]],[0,1,1,0,1,0,1,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,512,31,8,[1,7,24,0,0,0],[[2,2,512]],[[8,16],[16,8],[32,8]]",5029));//R16_291×R2_1=R32_5029=KW(5,1):[[0,2],[2,0]],[[2,2],[0,0]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,11,15,288,31,2,[1,15,12,4,0,0],[[2,2,736]]",5030));//R4(9,1):[[0,0],[2,2]],[[2,1],[1,2]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,15,240,15,2,[1,11,12,8,0,0],[[2,2,784]]",5031));//[[0,1,0,0],[1,0,0,0],[0,1,1,0],[0,1,1,0]],[[0,0,1,1],[0,0,1,1],[0,0,1,1],[0,0,1,1]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,11,15,272,15,2,[1,15,16,0,0,0],[[2,2,752]]",5032));//[[1,0,0,0],[0,1,0,0],[1,1,1,0],[1,1,1,0]],[[1,0,1,1],[0,1,1,1],[1,0,0,1],[1,0,0,1]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,11,15,240,15,2,[1,13,14,4,0,0],[[2,2,784]]",5033));//[[1,0,1,1],[0,1,1,1],[1,0,0,1],[1,0,0,1]],[[1,1,0,0],[1,1,0,0],[1,0,0,0],[1,0,0,0]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,11,15,336,15,1,[1,19,12,0,0,0],[[2,2,688]]",5034));
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,31,512,31,8,[1,15,16,0,0,0],[[2,2,512]],[[8,16],[16,8],[32,8]]",5035));//R8_9:[[0,0],[0,1]],[[1,1],[1,0]]	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,28,18,3,3,220,27,2,[1,20,11,0,0,0],[[2,2,804]]",5036));//R4_10:[[0,0],[0,1]],[[1,2],[3,2]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,11,15,448,31,4,[1,15,16,0,0,0],[[2,2,576]]",5037));//R16_376×R2_1=R32_5037=R8_34:[[0,0],[0,2]],[[0,0],[2,1]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,11,15,352,31,2,[1,15,12,4,0,0],[[2,2,672]]",5038));//R16_370×R2_1=R32_5038=R8_35:[[0,0],[0,2]],[[0,0],[1,1]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,344,23,1,[1,21,10,0,0,0],[[2,2,680]]",5039));//R4_7:[[0,0],[0,1]],[[0,0],[0,2]],[[1,0],[2,0]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,11,15,416,31,1,[1,19,12,0,0,0],[[2,2,608]]",5040));//R4_7:[[0,0],[0,1]],[[0,0],[0,2]],[[3,0],[1,0]]	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,264,23,2,[1,15,16,0,0,0],[[2,2,760]]",5041));//R16_284:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,5,7,264,23,2,[1,14,15,2,0,0],[[2,2,760]]",5042));//R16_373×R2_2=R32_5042=R16_284:		
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,256,31,2,[1,15,14,2,0,0],[[2,2,768]]",5043));//R16_284:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,11,15,320,31,2,[1,15,12,4,0,0],[[2,2,704]]",5044));//R16_284:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,3,3,228,19,2,[1,12,11,8,0,0],[[2,2,796]]",5045));//R16_381×R2_2=R32_5045=R8_51:[[0,0],[0,1]],[[0,0],[4,0]]	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,7,304,31,2,[1,11,12,8,0,0],[[2,2,720]]",5046));//R16_378×R2_1=R32_5046=R8_42:[[0,0],[0,1]],[[0,0],[2,0]]	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,31,576,31,8,[1,15,16,0,0,0],[[2,2,448]]",5047));//R16_282×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,7,15,432,31,8,[1,8,15,8,0,0],[[2,2,592]]",5048));//R16_282×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,31,640,31,8,[1,15,16,0,0,0],[[2,2,384]]",5049));//R16_283×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,7,15,480,31,8,[1,8,15,8,0,0],[[2,2,544]]",5050));//R16_283×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,288,31,8,[1,12,14,5,0,0],[[2,2,736]]",5051));//R16_284×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,28,12,3,3,216,27,8,[1,14,17,0,0,0],[[2,2,808]]",5052));//R16_284×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,11,15,448,31,2,[1,15,16,0,0,0],[[2,2,576]]",5053));//R16_285×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,5,7,336,31,2,[1,14,15,2,0,0],[[2,2,688]]",5054));//R16_285×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,15,31,576,31,32,[1,15,16,0,0,0],[[2,2,448]]",5055));//R16_286×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,432,31,32,[1,8,15,8,0,0],[[2,2,592]]",5056));//R16_286×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,7,15,448,31,32,[1,7,8,16,0,0],[[2,2,576]]",5057));//R16_287×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,7,336,31,32,[1,4,7,12,8,0],[[2,2,688]]",5058));//R16_287×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,31,576,31,8,[1,15,16,0,0,0],[[2,2,448]]",5059));//R16_288×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,7,15,432,31,8,[1,8,15,8,0,0],[[2,2,592]]",5060));//R16_288×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,288,31,8,[1,9,12,6,4,0],[[2,2,736]]",5061));//R16_289×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,216,31,8,[1,8,11,12,0,0],[[2,2,808]]",5062));//R16_289×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,448,31,8,[1,7,24,0,0,0],[[2,2,576]],[[8,24],[32,8]]",5063));//R16_290×R2_1=R8_39:[[2,0],[7,4],[[2,6],[5,4]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,3,15,336,31,8,[1,4,15,12,0,0],[[2,2,688]]",5064));//R16_290×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,3,15,384,31,8,[1,4,15,12,0,0],[[2,2,640]]",5065));//R16_291×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,576,31,8,[1,7,24,0,0,0],[[2,2,448]],[[8,8],[16,16],[32,8]]",5066));//R16_292×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,3,15,432,31,8,[1,4,15,12,0,0],[[2,2,592]],[[4,4],[8,12],[16,12],[32,4]]",5067));//R16_292×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,24,4,7,7,240,23,32,[1,10,21,0,0,0],[[2,2,784]]",5068));//R16_293×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,23,31,640,31,8,[1,23,8,0,0,0],[[2,2,384]]",5069));//R16_294×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,11,15,480,31,8,[1,12,15,4,0,0],[[2,2,544]]",5070));//R16_294×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,28,20,3,3,216,27,4,[1,22,9,0,0,0],[[2,2,808]]",5071));//R16_295×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,352,31,8,[1,17,8,6,0,0],[[2,2,672]]",5072));//R16_296×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,264,31,8,[1,12,19,0,0,0],[[2,2,760]]",5073));//R16_296×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,336,31,4,[1,16,12,3,0,0],[[2,2,688]]",5074));//R16_297×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,28,20,3,3,252,27,4,[1,22,9,0,0,0],[[2,2,772]]",5075));//R16_297×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,336,31,2,[1,15,14,2,0,0],[[2,2,688]]",5076));//R16_298×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,252,27,2,[1,20,11,0,0,0],[[2,2,772]]",5077));//R16_298×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,224,31,32,[1,8,12,7,4,0],[[2,2,800]]",5078));//R16_299×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,24,4,3,7,168,23,32,[1,6,13,12,0,0],[[2,2,856]]",5079));//R16_299×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,31,640,31,8,[1,15,16,0,0,0],[[2,2,384]]",5080));//R16_357×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,7,15,480,31,8,[1,8,15,8,0,0],[[2,2,544]]",5081));//R16_357×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,288,31,4,[1,16,15,0,0,0],[[2,2,736]]",5082));//R16_358×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,352,31,8,[1,17,8,6,0,0],[[2,2,672]]",5083));//R16_359×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,264,23,8,[1,12,19,0,0,0],[[2,2,760]]",5084));//R16_359×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,288,31,8,[1,9,12,6,4,0],[[2,2,736]]",5085));//R16_360×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,216,23,8,[1,8,11,12,0,0],[[2,2,808]]",5086));//R16_360×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,15,31,512,31,32,[1,15,16,0,0,0],[[2,2,512]]",5087));//R16_361×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,384,31,32,[1,8,15,8,0,0],[[2,2,640]]",5088));//R16_361×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,7,31,448,31,32,[1,7,24,0,0,0],[[2,2,576]]",5089));//R16_362×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,15,512,31,8,[1,15,0,16,0,0],[[2,2,512]]",5090));//R16_363×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,7,7,384,31,8,[1,8,7,8,8,0],[[2,2,640]]",5091));//R16_363×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,7,7,160,31,4,[1,8,12,11,0,0],[[2,2,864]]",5092));//R16_364×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,20,4,3,3,120,19,4,[1,6,17,8,0,0],[[2,2,904]]",5093));//R16_364×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,336,31,2,[1,15,14,2,0,0],[[2,2,688]]",5094));//R16_365×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,252,31,2,[1,20,11,0,0,0],[[2,2,772]]",5095));//R16_365×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,224,31,32,[1,16,8,7,0,0],[[2,2,800]]",5096));//R16_366×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,24,4,7,7,168,23,32,[1,10,21,0,0,0],[[2,2,856]]",5097));//R16_366×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,7,15,320,31,32,[1,7,8,0,16,0],[[2,2,704]]",5098));//R16_367×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,11,15,320,31,4,[1,15,12,4,0,0],[[2,2,704]]",5099));//R16_368×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,5,7,240,31,4,[1,14,15,2,0,0],[[2,2,784]]",5100));//R16_368×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,368,31,2,[1,15,14,2,0,0],[[2,2,656]]",5101));//R16_369×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,276,31,2,[1,20,11,0,0,0],[[2,2,748]]",5102));//R16_369×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,5,7,264,31,2,[1,14,15,2,0,0],[[2,2,760]]",5103));//R16_370×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,288,31,8,[1,12,14,5,0,0],[[2,2,736]]",5104));//R16_371×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,28,12,3,3,216,27,8,[1,14,17,0,0,0],[[2,2,808]]",5105));//R16_371×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,368,31,2,[1,15,14,2,0,0],[[2,2,656]]",5106));//R16_372×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,276,27,2,[1,20,11,0,0,0],[[2,2,748]]",5107));//R16_372×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,11,15,256,31,8,[1,12,10,7,2,0],[[2,2,768]]",5108));//R16_374×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,24,4,5,7,192,23,8,[1,8,17,6,0,0],[[2,2,832]]",5109));//R16_374×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,11,15,448,31,2,[1,15,16,0,0,0],[[2,2,576]]",5110));//R16_375×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,5,7,336,31,2,[1,14,15,2,0,0],[[2,2,688]]",5111));//R16_375×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,5,7,336,31,4,[1,14,15,2,0,0],[[2,2,688]]",5112));//R16_376×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,256,31,32,[1,8,12,7,4,0],[[2,2,768]]",5113));//R16_377×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,3,3,228,31,2,[1,12,11,8,0,0],[[2,2,796]]",5114));//R16_378×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,23,31,576,31,8,[1,23,8,0,0,0],[[2,2,448]],[[8,8],[16,16],[32,8]]",5115));//R16_379×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,11,15,432,31,8,[1,12,15,4,0,0],[[2,2,592]],[[4,4],[8,12],[16,12],[32,4]]",5116));//R16_379×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,15,448,31,8,[1,15,0,16,0,0],[[2,2,576]]",5117));//R8_33:[[0,0],[1,4]],[[0,0],[1,5]]	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,15,384,31,8,[1,15,0,16,0,0],[[2,2,640]]",5118));//R8_33:[[0,1],[7,0]],[[0,1],[7,2]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,15,15,384,31,32,[1,15,0,16,0,0],[[2,2,640]]",5119));//R8_33:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,19,31,640,31,4,[1,19,12,0,0,0],[[2,2,384]]",5120));//R16_127:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,15,15,416,31,4,[1,19,8,4,0,0],[[2,2,608]]",5121));//R16_280×R2_1=R32_5121
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,15,15,320,31,4,[1,19,12,0,0,0],[[2,2,704]]",5122));//R8_45:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,15,15,336,15,4,[1,19,12,0,0,0],[[2,2,688]]",5123));//R8_45:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,15,240,31,2,[1,11,12,8,0,0],[[2,2,784]]",5124));//R8_46:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,11,23,448,31,2,[1,11,12,8,0,0],[[2,2,576]]",5125));//[[0,0,0,0],[0,0,1,0],[1,0,0,0],[0,0,0,0]],[[0,0,0,0],[0,0,1,0],[1,0,0,0],[0,1,0,0]]	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,13,7,7,328,31,1,[1,19,12,0,0,0],[[2,2,696]]",5126));//R8_49:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,11,15,336,31,1,[1,19,12,0,0,0],[[2,2,688]]",5127));//R8_44:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,512,31,4,[1,7,24,0,0,0],[[2,2,512]]",5128));//R8_39:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,15,15,448,31,2,[1,23,8,0,0,0],[[2,2,576]]",5129));//R16_279×R2_1=R32_5129
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,11,15,384,31,2,[1,19,12,0,0,0],[[2,2,640]]",5130));//R8_36:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,15,256,31,4,[1,11,12,8,0,0],[[2,2,768]]",5131));//R8_46:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,248,23,2,[1,15,16,0,0,0],[[2,2,776]]",5132));//R8_49:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,15,272,15,4,[1,11,12,8,0,0],[[2,2,752]]",5133));//R16_360:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,11,31,512,31,4,[1,11,20,0,0,0],[[2,2,512]]",5134));//R8_28:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,304,15,8,[1,17,14,0,0,0],[[2,2,720]]",5135));//R16_293:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,312,23,1,[1,21,10,0,0,0],[[2,2,712]]",5136));//R16_297:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,31,576,31,4,[1,15,16,0,0,0],[[2,2,448]]",5137));//R8_28:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,11,15,352,31,2,[1,15,12,4,0,0],[[2,2,672]]",5138));//R16_373×R2_1=R32_5138	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,23,31,512,31,8,[1,23,8,0,0,0],[[2,2,512]]",5139));//R16_131:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,11,31,512,31,8,[1,11,20,0,0,0],[[2,2,512]]",5140));//R16_146:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,31,448,31,8,[1,15,16,0,0,0],[[2,2,576]]",5141));//R16_131:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,7,7,336,31,2,[1,24,7,0,0,0],[[2,2,688]]",5142));//R16_279×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,312,31,4,[1,16,15,0,0,0],[[2,2,712]]",5143));//R16_280×R2_2	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,312,23,4,[1,16,15,0,0,0],[[2,2,712]]",5144));//R16_281×R2_2	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,3,31,416,31,4,[1,3,28,0,0,0],[[2,2,608]]",5145));//R16_282:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,7,304,31,2,[1,11,12,8,0,0],[[2,2,720]]",5146));//R16_381×R2_1=R32_5146	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,11,31,480,31,4,[1,11,20,0,0,0],[[2,2,544]]",5147));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,31,480,31,4,[1,15,16,0,0,0],[[2,2,544]]",5148));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,31,512,31,4,[1,15,16,0,0,0],[[2,2,512]]",5149));//R16_282:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,19,31,576,31,4,[1,19,12,0,0,0],[[2,2,448]]",5150));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,11,31,448,31,8,[1,11,20,0,0,0],[[2,2,576]]",5151));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,11,31,416,31,4,[1,11,20,0,0,0],[[2,2,608]]",5152));//R16_282:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,15,368,31,2,[1,11,12,8,0,0],[[2,2,656]]",5153));//[[0,0,0,0],[0,0,1,0],[1,0,0,0],[0,0,0,0]],[[0,0,0,0],[0,0,1,0],[1,0,0,0],[0,1,0,1]]	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,15,15,544,31,2,[1,23,8,0,0,0],[[2,2,480]]",5154));//R16_278×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,7,7,408,23,2,[1,24,7,0,0,0],[[2,2,616]]",5155));//R16_278×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,256,23,2,[1,15,14,2,0,0],[[2,2,768]]",5156));//[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,1,1]],[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,1,0,0]],[[0,0,0,0],[0,1,0,0],[0,1,1,0],[1,0,0,1]]	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,15,352,31,4,[1,11,12,8,0,0],[[2,2,672]]",5157));//[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,1,1,1]],[[0,0,0,0],[0,0,0,0],[0,0,0,0],[1,0,0,0]],[[0,0,0,0],[0,0,0,1],[1,0,0,1],[0,0,0,0]]
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,15,448,31,8,[1,7,8,16,0,0],[[2,2,576]]",5158));//R16_277×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,3,7,336,31,8,[1,4,7,12,8,0],[[2,2,688]]",5159));//R16_277×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,7,15,320,31,32,[1,7,8,16,0,0],[[2,2,704]]",5160));//R16_287:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,15,384,31,8,[1,7,8,16,0,0],[[2,2,640]]",5161));//R16_287:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,384,31,4,[1,7,24,0,0,0],[[2,2,640]]",5162));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,11,31,384,31,4,[1,11,20,0,0,0],[[2,2,640]]",5163));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,3,31,384,31,4,[1,3,28,0,0,0],[[2,2,640]]",5164));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,31,448,31,4,[1,15,16,0,0,0],[[2,2,576]]",5165));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,11,31,448,31,4,[1,11,20,0,0,0],[[2,2,576]]",5166));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,416,31,8,[1,7,24,0,0,0],[[2,2,608]]",5167));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,448,31,4,[1,7,24,0,0,0],[[2,2,576]]",5168));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,31,416,31,4,[1,15,16,0,0,0],[[2,2,608]]",5169));//R16_282:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,288,31,4,[1,16,12,3,0,0],[[2,2,736]]",5170));//R16_295×R2_1=R32_5170
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,11,31,480,31,8,[1,11,20,0,0,0],[[2,2,544]]",5171));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,480,31,8,[1,7,24,0,0,0],[[2,2,544]]",5172));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,416,31,4,[1,7,24,0,0,0],[[2,2,608]]",5173));//R16_282:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,320,31,32,[1,16,8,7,0,0],[[2,2,704]]",5174));//R16_293×R2_1=R32_5174	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,11,31,416,31,2,[1,11,20,0,0,0],[[2,2,608]]",5175));//[[0,0,0,0],[0,0,1,0],[1,0,0,0],[1,0,1,0]],[[0,0,0,0],[0,0,1,0],[1,1,0,1],[0,0,1,0]]		
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,240,31,8,[1,9,14,8,0,0],[[2,2,784]]",5176));//R16_377:[[0,0],[0,1]],[[0,1],[0,2]]	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,15,224,31,4,[1,11,12,8,0,0],[[2,2,800]],[[2,16],[8,10],[16,5],[32,1]]",5177));//[[0,0,0,1],[1,1,1,1],[1,0,0,1],[1,1,1,0]],[[0,0,1,0],[0,0,0,0],[0,0,0,0],[0,1,0,0]]	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,384,31,8,[1,7,24,0,0,0],[[2,2,640]],[[8,24],[16,4],[32,4]]",5178));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,3,31,384,31,8,[1,3,28,0,0,0],[[2,2,640]],[[8,24],[16,4],[32,4]]",5179));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,31,416,31,8,[1,15,16,0,0,0],[[2,2,608]],[[8,20],[16,8],[32,4]]",5180));//R16_282:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,11,31,416,31,8,[1,11,20,0,0,0],[[2,2,608]],[[8,20],[16,8],[32,4]]",5181));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,19,31,512,31,4,[1,19,12,0,0,0],[[2,2,512]],[[8,16],[16,8],[32,8]]",5182));//R16_282:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,31,384,31,8,[1,15,16,0,0,0],[[2,2,640]],[[8,24],[16,4],[32,4]]",5183));//R16_282:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,15,320,31,8,[1,7,8,16,0,0],[[2,2,704]],[[4,16],[8,8],[16,4],[32,4]]",5184));//R16_277:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,15,15,352,31,2,[1,23,8,0,0,0],[[2,2,672]],[[4,16],[16,14],[32,2]]",5185));//R8_44:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,31,480,31,8,[1,15,16,0,0,0],[[2,2,544]],[[8,12],[16,16],[32,4]]",5186));//R16_282:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,31,384,31,4,[1,15,16,0,0,0],[[2,2,640]],[[8,24],[16,4],[32,4]]",5187));//R16_282:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,11,31,384,31,8,[1,11,20,0,0,0],[[2,2,640]],[[8,24],[16,4],[32,4]]",5188));//R16_282:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,19,31,480,31,4,[1,19,12,0,0,0],[[2,2,544]],[[8,12],[16,16],[32,4]]",5189));//R16_282:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,3,31,416,31,8,[1,3,28,0,0,0],[[2,2,608]],[[8,20],[16,8],[32,4]]",5190));//R16_282:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,19,31,512,31,4,[1,19,12,0,0,0],[[2,2,512]],[[8,8],[16,20],[32,4]]",5191));//R8_45:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,16,2,11,15,224,15,8,[1,12,15,4,0,0],[[2,2,800]],[[1,16],[8,8],[16,7],[32,1]]",5192));//R8_45:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,16,2,11,15,192,15,8,[1,12,15,4,0,0],[[2,2,832]],[[1,16],[8,12],[16,3],[32,1]]",5193));//R8_45:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,23,31,416,31,8,[1,23,8,0,0,0],[[2,2,608]],[[8,20],[16,8],[32,4]]",5194));	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,23,31,448,31,8,[1,23,8,0,0,0],[[2,2,576]],[[8,16],[16,12],[32,4]]",5195));
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,23,31,480,31,8,[1,23,8,0,0,0],[[2,2,544]],[[8,12],[16,16],[32,4]]",5196));	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,31,31,448,31,32,[1,31,0,0,0,0],[[2,2,576]],[[8,16],[16,12],[32,4]]",5197));	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,264,31,2,[1,15,16,0,0,0],[[2,2,760]],[[2,8],[4,6],[8,10],[16,7],[32,1]]",5198));
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,19,31,448,31,4,[1,19,12,0,0,0],[[2,2,576]],[[8,16],[16,12],[32,4]]",5199));	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,19,31,416,31,4,[1,19,12,0,0,0],[[2,2,608]],[[8,20],[16,8],[32,4]]",5200));	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,19,31,384,31,4,[1,19,12,0,0,0],[[2,2,640]],[[8,24],[16,4],[32,4]]",5201));	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,23,288,31,8,[1,15,8,8,0,0],[[2,2,736]],[[4,16],[8,8],[16,6],[32,2]]",5202));	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,15,320,31,8,[1,7,8,16,0,0],[[2,2,704]],[[4,8],[8,16],[16,6],[32,2]]",5203));	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,3,31,448,31,4,[1,3,28,0,0,0],[[2,2,576]],[[8,16],[16,12],[32,4]]",5204));
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,7,176,15,8,[1,9,6,8,8,0],[[2,2,848]],[[1,16],[4,8],[8,4],[16,2],[32,2]]",5205));//R4_9:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,16,2,11,15,176,15,8,[1,12,15,4,0,0],[[2,2,848]],[[1,16],[4,4],[8,8],[16,3],[32,1]]",5206));
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,3,31,480,31,4,[1,3,28,0,0,0],[[2,2,544]],[[8,12],[16,16],[32,4]]",5207));//R16_275:
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,480,31,4,[1,7,24,0,0,0],[[2,2,544]],[[8,12],[16,16],[32,4]]",5208));//R16_275:	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,15,23,288,31,32,[1,15,8,8,0,0],[[2,2,736]],[[4,16],[8,8],[16,6],[32,2]]",5209));	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,7,23,288,31,32,[1,7,16,8,0,0],[[2,2,736]],[[4,16],[8,8],[16,6],[32,2]]",5210));	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,23,288,31,8,[1,7,16,8,0,0],[[2,2,736]],[[4,16],[8,8],[16,6],[32,2]]",5211));	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,3,31,352,31,4,[1,3,28,0,0,0],[[2,2,672]],[[8,28],[32,4]]",5212));
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,11,15,272,31,2,[1,15,16,0,0,0],[[2,2,752]],[[4,16],[8,8],[16,7],[32,1]]",5213));	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,352,31,8,[1,7,24,0,0,0],[[2,2,672]],[[8,28],[32,4]]",5214));	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,23,31,576,31,8,[1,23,8,0,0,0],[[2,2,448]],[[16,28],[32,4]]",5215));//R16_276×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,11,15,432,31,8,[1,12,15,4,0,0],[[2,2,592]],[[8,14],[16,16],[32,2]]",5216));//R16_276×R2_2
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,11,15,240,31,2,[1,13,14,4,0,0],[[2,2,784]],[[2,16],[8,8],[16,7],[32,1]]",5217));
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,11,31,352,31,8,[1,11,20,0,0,0],[[2,2,672]],[[8,28],[32,4]]",5218));	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,15,31,352,31,4,[1,15,16,0,0,0],[[2,2,672]],[[8,28],[32,4]]",5219));	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,352,31,4,[1,7,24,0,0,0],[[2,2,672]],[[8,28],[32,4]]",5220));		
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,15,15,416,31,4,[1,19,8,4,0,0],[[2,2,608]]",5221));//R16_281×R2_1=R32_5221
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,7,31,416,31,32,[1,7,24,0,0,0],[[2,2,608]],[[8,20],[16,8],[32,4]]",5222));
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,7,31,384,31,32,[1,7,24,0,0,0],[[2,2,640]],[[8,24],[16,4],[32,4]]",5223));
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,248,31,1,[1,21,10,0,0,0],[[2,2,776]],[[2,8],[4,4],[8,15],[16,4],[32,1]]",5224));
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,13,7,7,296,31,1,[1,19,12,0,0,0],[[2,2,728]],[[4,12],[8,11],[16,8],[32,1]]",5225));
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,15,31,448,31,32,[1,15,16,0,0,0],[[2,2,576]],[[8,16],[16,12],[32,4]]",5226));	
	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,576,31,8,[1,7,24,0,0,0],[[2,2,448]],[[16,28],[32,4]]",5266));//R16_275×R2_1
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,3,15,432,31,8,[1,4,15,12,0,0],[[2,2,592]],[[8,14],[16,16],[32,2]]",5267));//R16_275×R2_2	
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
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,480,31,8,[1,9,14,8,0,0],[[2,2,544]]",5367));//R8R4_368=R8_34×R4_5
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,480,31,8,[1,12,19,0,0,0],[[2,2,544]]",5368));//R8R4_369=R8_34×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,400,31,2,[1,15,16,0,0,0],[[2,2,624]]",5369));//R8R4_370=R8_34×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,400,31,2,[1,15,16,0,0,0],[[2,2,624]]",5370));//R8R4_371=R8_34×R4_8
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,320,31,8,[1,12,14,5,0,0],[[2,2,704]]",5371));//R8R4_372=R8_34×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8,[1,14,17,0,0,0],[[2,2,664]]",5372));//R8R4_373=R8_34×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,3,280,31,8,[1,8,9,10,4,0],[[2,2,744]]",5373));//R8R4_374=R8_34×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,384,31,32,[1,10,15,6,0,0],[[2,2,640]]",5374));//R8R4_295=R8_27×R4_9
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,640,31,8,[1,17,14,0,0,0],[[2,2,384]]",5375));//R8R4_312=R8_29×R4_4
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,480,31,8,[1,9,14,8,0,0],[[2,2,544]]",5376));//R8R4_313=R8_29×R4_5
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,512,31,32,[1,16,8,7,0,0],[[2,2,512]]",5377));//R8R4_378=R8_35×R4_4
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,384,31,32,[1,8,12,7,4,0],[[2,2,640]]",5378));//R8R4_379=R8_35×R4_5
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,384,31,32,[1,10,15,6,0,0],[[2,2,640]]",5379));//R8R4_380=R8_35×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,320,31,8,[1,12,14,5,0,0],[[2,2,704]]",5380));//R8R4_381=R8_35×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,320,31,8,[1,12,14,5,0,0],[[2,2,704]]",5381));//R8R4_382=R8_35×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,256,31,32,[1,10,12,9,0,0],[[2,2,768]]",5382));//R8R4_383=R8_35×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,3,3,288,31,32,[1,10,17,4,0,0],[[2,2,736]]",5383));//R8R4_384=R8_35×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,3,224,31,32,[1,6,9,10,4,2],[[2,2,800]]",5384));//R8R4_385=R8_35×R4_11	
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,480,31,8,[1,12,19,0,0,0],[[2,2,544]]",5385));//R8R4_314=R8_29×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,400,31,2,[1,15,16,0,0,0],[[2,2,624]]",5386));//R8R4_315=R8_29×R4_7
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,400,31,2,[1,15,16,0,0,0],[[2,2,624]]",5387));//R8R4_316=R8_29×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,15,15,512,31,4,[1,19,12,0,0,0],[[2,2,512]]",5388));//R8R4_389=R8_36×R4_4
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,15,384,31,4,[1,11,12,8,0,0],[[2,2,640]]",5389));//R8R4_390=R8_36×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,384,31,4,[1,16,15,0,0,0],[[2,2,640]]",5390));//R8R4_391=R8_36×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,320,31,1,[1,21,10,0,0,0],[[2,2,704]]",5391));//R8R4_392=R8_36×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,320,31,1,[1,21,10,0,0,0],[[2,2,704]]",5392));//R8R4_393=R8_36×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,256,31,4,[1,16,12,3,0,0],[[2,2,768]]",5393));//R8R4_394=R8_36×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,20,3,3,288,31,4,[1,22,9,0,0,0],[[2,2,736]]",5394));//R8R4_395=R8_36×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,3,3,224,31,4,[1,12,5,14,0,0],[[2,2,800]]",5395));//R8R4_396=R8_36×R4_11
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,320,31,8,[1,12,14,5,0,0],[[2,2,704]]",5396));//R8R4_317=R8_29×R4_9
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
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,432,31,32,[1,6,13,12,0,0],[[2,2,592]]",5412));//R8R4_413=R8_38×R4_6
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
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,432,31,32,[1,6,13,12,0,0],[[2,2,592]]",5433));//R8R4_434=R8_40×R4_5
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,360,31,8,[1,8,11,12,0,0],[[2,2,664]]",5444));//R8R4_445=R8_41×R4_5
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,3,3,432,31,32,[1,10,21,0,0,0],[[2,2,592]]",5434));//R8R4_435=R8_40×R4_6
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8,[1,14,17,0,0,0],[[2,2,664]]",5445));//R8R4_446=R8_41×R4_6
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8,[1,14,17,0,0,0],[[2,2,664]]",5435));//R8R4_436=R8_40×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,300,27,2,[1,20,11,0,0,0],[[2,2,724]]",5446));//R8R4_447=R8_41×R4_7
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8,[1,14,17,0,0,0],[[2,2,664]]",5436));//R8R4_437=R8_40×R4_8
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,300,31,2,[1,20,11,0,0,0],[[2,2,724]]",5447));//R8R4_448=R8_41×R4_8
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,3,3,288,31,32,[1,10,17,4,0,0],[[2,2,736]]",5437));//R8R4_438=R8_40×R4_9
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
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,7,336,31,32,[1,4,9,10,8,0],[[2,2,688]]",5455));//R8R4_456=R8_42×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,15,432,31,4,[1,11,12,8,0,0],[[2,2,592]]",5466));//R8R4_467=R8_43×R4_5
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,3,336,31,32,[1,6,11,8,6,0],[[2,2,688]]",5456));//R8R4_457=R8_42×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,432,31,4,[1,16,15,0,0,0],[[2,2,592]]",5467));//R8R4_468=R8_43×R4_6
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,3,280,31,8,[1,8,9,10,4,0],[[2,2,744]]",5457));//R8R4_458=R8_42×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,360,31,1,[1,21,10,0,0,0],[[2,2,664]]",5468));//R8R4_469=R8_43×R4_7
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,3,280,31,8,[1,8,9,10,4,0],[[2,2,744]]",5458));//R8R4_459=R8_42×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,360,31,1,[1,21,10,0,0,0],[[2,2,664]]",5469));//R8R4_470=R8_43×R4_8
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,3,224,31,32,[1,6,9,10,4,2],[[2,2,800]]",5459));//R8R4_460=R8_42×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,288,31,4,[1,16,12,3,0,0],[[2,2,736]]",5470));//R8R4_471=R8_43×R4_9
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,1,1,252,31,32,[1,8,9,8,6,0],[[2,2,772]]",5460));//R8R4_461=R8_42×R4_10
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
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,360,31,8,[1,8,11,12,0,0],[[2,2,664]]",5510));//R8R4_511=R8_47×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,240,31,32,[1,6,11,10,4,0],[[2,2,784]]",5500));//R8R4_501=R8_46×R4_6
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8,[1,14,17,0,0,0],[[2,2,664]]",5511));//R8R4_512=R8_47×R4_6
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
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,288,31,32,[1,6,11,10,4,0],[[2,2,736]]",5521));//R8R4_522=R8_48×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,312,31,8,[1,8,10,11,2,0],[[2,2,712]]",5532));//R8R4_533=R8_49×R4_5
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,3,3,288,31,32,[1,10,17,4,0,0],[[2,2,736]]",5522));//R8R4_523=R8_48×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,312,31,8,[1,14,15,2,0,0],[[2,2,712]]",5533));//R8R4_534=R8_49×R4_6
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,240,27,8,[1,14,15,2,0,0],[[2,2,784]]",5523));//R8R4_524=R8_48×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,260,27,2,[1,20,10,1,0,0],[[2,2,764]]",5534));//R8R4_535=R8_49×R4_7
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,240,31,8,[1,14,15,2,0,0],[[2,2,784]]",5524));//R8R4_525=R8_48×R4_8
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
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,1,3,324,31,32,[1,8,9,14,0,0],[[2,2,700]]",5543));//R8R4_544=R8_50×R4_5
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,1,3,252,31,32,[1,4,7,10,6,4],[[2,2,772]]",5554));//R8R4_555=R8_51×R4_5
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,1,3,180,31,32,[1,2,3,8,6,12],[[2,2,844]]",5565));//R8R4_566=R8_52×R4_5
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,16,1,1,324,31,32,[1,16,15,0,0,0],[[2,2,700]]",5544));//R8R4_545=R8_50×R4_6
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,1,1,252,31,32,[1,8,9,8,6,0],[[2,2,772]]",5555));//R8R4_556=R8_51×R4_6
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,1,1,180,31,32,[1,4,3,6,12,6],[[2,2,844]]",5566));//R8R4_567=R8_52×R4_6
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,24,1,1,270,29,8,[1,24,7,0,0,0],[[2,2,754]]",5545));//R8R4_546=R8_50×R4_7
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,1,1,210,25,8,[1,12,5,12,2,0],[[2,2,814]]",5556));//R8R4_557=R8_51×R4_7
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,1,1,150,17,8,[1,6,1,6,18,0],[[2,2,874]]",5567));//R8R4_568=R8_52×R4_7
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,24,1,1,270,31,8,[1,24,7,0,0,0],[[2,2,754]]",5546));//R8R4_547=R8_50×R4_8
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,1,1,210,31,8,[1,12,5,12,2,0],[[2,2,814]]",5557));//R8R4_558=R8_51×R4_8
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,1,1,150,31,8,[1,6,1,6,18,0],[[2,2,874]]",5568));//R8R4_569=R8_52×R4_8
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,30,16,1,1,216,29,32,[1,16,15,0,0,0],[[2,2,808]]",5547));//R8R4_548=R8_50×R4_9
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,26,8,1,1,168,25,32,[1,8,9,8,6,0],[[2,2,856]]",5558));//R8R4_559=R8_51×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,18,4,1,1,120,17,32,[1,4,3,6,12,6],[[2,2,904]]",5569));//R8R4_570=R8_52×R4_9
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,31,32,0,0,243,30,32,[1,31,0,0,0,0],[[2,2,781]]",5548));//R8R4_549=R8_50×R4_10
	//m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,29,16,0,0,189,28,32,[1,15,2,14,0,0],[[2,2,835]]",5559));//R8R4_560=R8_51×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,25,8,0,0,135,24,32,[1,7,0,6,18,0],[[2,2,889]]",5570));//R8R4_571=R8_52×R4_10
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,29,16,0,0,189,28,32,[1,15,2,14,0,0],[[2,2,835]]",5549));//R8R4_550=R8_50×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,23,8,0,0,147,22,32,[1,7,8,16,0,0],[[2,2,877]]",5560));//R8R4_561=R8_51×R4_11
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,11,4,0,0,105,10,32,[1,3,2,8,6,12],[[2,2,919]]",5571));//R8R4_572=R8_52×R4_11	
	/*---------------end of R32_5348~R32_5571----------------*/			
	//R32
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[8,23,24]],[[2,1,7],[4,4,21],[8,23,132],[16,93,192],[16,200,144]]",3036));//R16_93×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[8,23,24]],[[2,1,7],[4,4,21],[8,23,132],[16,200,144]]",3986));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,16],[4,5,8]],[[2,1,7],[4,1,40],[4,4,21],[4,5,20],[8,5,144],[8,26,72],[16,150,192]]",4098));//R16_150×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,16],[4,5,8]],[[2,1,7],[4,1,40],[4,4,21],[4,5,20],[8,5,144],[8,26,72]]",6098));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[2,2,4],[4,6,4],[4,9,4],[8,35,4]],[[2,1,15],[2,2,4],[4,4,105],[4,6,16],[4,8,18],[4,9,16],[8,34,54],[8,35,52],[8,44,54],[16,352,162]]",5121));//R16_280×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[2,2,4],[4,6,4],[4,9,4],[8,35,4]],[[2,1,15],[2,2,4],[4,4,105],[4,6,16],[4,7,18],[4,9,16],[8,29,54],[8,35,52],[8,37,54],[16,336,162]]",5221));//R16_281×R2_1	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,5,8],[4,6,2],[4,9,2],[8,35,2],[8,46,4],[16,348,4]],[[2,1,7],[2,2,2],[4,4,21],[4,5,20],[4,6,8],[4,8,3],[4,9,8],[8,26,72],[8,34,9],[8,35,26],[8,44,9],[8,46,34],[16,289,36],[16,348,106],[16,352,27]]",5061));//R16_289×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,5,8],[4,6,2],[4,9,2],[8,35,2],[8,46,4],[16,348,4]],[[2,1,7],[2,2,2],[4,4,21],[4,5,20],[4,6,8],[4,7,3],[4,9,8],[8,26,72],[8,29,9],[8,35,26],[8,37,9],[8,46,34],[16,336,27],[16,348,106],[16,360,36]]",5085));//R16_360×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,11],[4,6,7],[4,9,10]],[[2,1,3],[2,2,11],[4,4,3],[4,6,28],[4,7,6],[4,8,6],[4,9,40],[4,10,33],[8,27,9],[8,34,18],[8,37,18],[8,40,18],[8,41,6],[8,45,18],[8,47,6],[8,48,99],[8,49,18],[8,50,6],[16,284,54],[16,323,18],[16,332,18],[16,341,18],[16,356,18],[16,380,6]]",5052));//R16_284×R2_2
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,11],[4,6,7],[4,9,10]],[[2,1,3],[2,2,11],[4,4,3],[4,6,28],[4,7,6],[4,8,6],[4,9,40],[4,10,33],[8,27,9],[8,29,18],[8,40,18],[8,41,6],[8,44,18],[8,45,18],[8,47,6],[8,48,99],[8,49,18],[8,50,6],[16,310,18],[16,323,18],[16,332,18],[16,347,18],[16,371,54],[16,380,6]]",5105));//R16_371×R2_2
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,5],[4,6,9],[4,9,5],[8,35,5]],[[2,1,7],[2,2,5],[4,4,21],[4,6,36],[4,7,3],[4,8,3],[4,9,20],[4,10,6],[8,27,18],[8,29,27],[8,34,9],[8,35,65],[8,40,18],[8,44,9],[8,45,9],[8,48,18],[8,49,6],[16,308,18],[16,316,54],[16,329,27],[16,330,18],[16,352,27],[16,371,18]]",5104));//R16_371×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,5],[4,6,9],[4,9,5],[8,35,5]],[[2,1,7],[2,2,5],[4,4,21],[4,6,36],[4,7,3],[4,8,3],[4,9,20],[4,10,6],[8,27,18],[8,29,9],[8,34,27],[8,35,65],[8,37,9],[8,40,18],[8,45,9],[8,48,18],[8,49,6],[16,284,18],[16,316,54],[16,329,27],[16,330,18],[16,336,27],[16,354,18]]",5051));//R16_284×R2_1	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[2,2,2],[4,6,2],[4,9,6],[8,35,6]],[[2,1,15],[2,2,2],[4,4,105],[4,6,8],[4,8,3],[4,9,24],[8,34,9],[8,35,78],[8,44,27],[8,45,18],[16,296,18],[16,329,54],[16,352,81]]",5072));//R16_296×R2_1	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[2,2,2],[4,6,2],[4,9,6],[8,35,6]],[[2,1,15],[2,2,2],[4,4,105],[4,6,8],[4,7,3],[4,9,24],[8,29,9],[8,35,78],[8,37,27],[8,45,18],[16,329,54],[16,336,81],[16,359,18]]",5083));//R16_359×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,8],[4,6,12],[4,9,2],[8,35,2]],[[2,1,7],[2,2,8],[4,4,21],[4,6,48],[4,7,12],[4,8,6],[4,9,8],[4,10,12],[8,27,18],[8,29,54],[8,34,18],[8,35,26],[8,36,6],[8,37,9],[8,40,36],[8,41,12],[8,49,12],[16,298,12],[16,308,18],[16,310,36],[16,330,36],[16,335,18],[16,336,27]]",5076));//R16_298×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,8],[4,6,12],[4,9,2],[8,35,2]],[[2,1,7],[2,2,8],[4,4,21],[4,6,48],[4,7,6],[4,8,12],[4,9,8],[4,10,12],[8,27,18],[8,29,18],[8,34,54],[8,35,26],[8,36,6],[8,40,36],[8,44,9],[8,47,12],[8,49,12],[16,330,36],[16,335,18],[16,352,27],[16,354,18],[16,356,36],[16,365,12]]",5094));//R16_365×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,1],[4,5,8],[4,6,7],[8,38,8]],[[2,1,7],[2,2,1],[4,4,9],[4,5,20],[4,6,28],[8,26,24],[8,27,27],[8,28,36],[8,32,12],[8,38,68],[8,39,12],[16,306,72],[16,339,36],[16,342,36],[16,344,108]]",5050));//R16_283×R2_2
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,1],[4,5,8],[4,6,7],[8,38,8]],[[2,1,7],[2,2,1],[4,4,21],[4,5,20],[4,6,28],[8,26,24],[8,27,63],[8,38,68],[16,306,72],[16,357,48]]",5081));//R16_357×R2_2	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[4,5,16]],[[2,1,15],[4,4,57],[4,5,40],[8,26,144],[8,28,144],[8,32,48],[8,39,48]]",5049));//R16_283×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[4,5,16]],[[2,1,15],[4,4,105],[4,5,40],[8,26,144],[16,357,192]]",5080));//R16_357×R2_1	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,11],[2,2,4],[4,5,4],[4,6,12]],[[2,1,11],[2,2,4],[4,4,39],[4,5,10],[4,6,48],[4,8,18],[8,26,12],[8,27,36],[8,28,48],[8,34,90],[16,285,72],[16,354,36]]",5053));//R16_285×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,11],[2,2,4],[4,5,4],[4,6,12]],[[2,1,11],[2,2,4],[4,4,39],[4,5,10],[4,6,48],[4,7,18],[8,26,12],[8,27,36],[8,28,48],[8,29,90],[16,308,36],[16,375,72]]",5110));//R16_375×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,8],[4,6,12],[4,9,2],[8,35,2]],[[2,1,7],[2,2,8],[4,4,21],[4,6,48],[4,7,3],[4,8,21],[4,9,8],[4,10,12],[8,27,18],[8,29,27],[8,34,63],[8,35,26],[8,40,36],[8,44,9],[8,47,12],[8,49,12],[16,308,18],[16,330,36],[16,352,27],[16,356,36],[16,369,12]]",5101));//R16_369×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,8],[4,6,12],[4,9,2],[8,35,2]],[[2,1,7],[2,2,8],[4,4,21],[4,6,48],[4,7,21],[4,8,3],[4,9,8],[4,10,12],[8,27,18],[8,29,63],[8,34,27],[8,35,26],[8,37,9],[8,40,36],[8,41,12],[8,49,12],[16,310,36],[16,330,36],[16,336,27],[16,354,18],[16,372,12]]",5106));//R16_372×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,1],[4,5,8],[4,6,7],[8,38,8]],[[2,1,7],[2,2,1],[4,4,9],[4,5,20],[4,6,28],[8,27,27],[8,28,36],[8,32,36],[8,38,68],[8,39,12],[16,339,108],[16,342,36],[16,344,108]]",5048));//R16_282×R2_2
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,1],[4,5,8],[4,6,7],[8,38,8]],[[2,1,7],[2,2,1],[4,4,21],[4,5,20],[4,6,28],[8,26,24],[8,27,63],[8,38,68],[16,288,48],[16,306,72]]",5060));//R16_288×R2_2	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[4,5,16]],[[2,1,15],[4,4,105],[4,5,40],[8,26,144],[16,288,192]]",5059));//R16_288×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[4,5,16]],[[2,1,15],[4,4,57],[4,5,40],[8,26,48],[8,28,144],[8,32,144],[8,39,48]]",5047));//R16_282×R2_1	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[2,2,9],[4,5,2],[4,6,13],[8,38,2]],[[2,1,5],[2,2,9],[4,4,6],[4,5,5],[4,6,52],[4,7,36],[4,10,12],[8,27,18],[8,28,12],[8,29,36],[8,38,17],[8,40,36],[8,41,36],[16,310,36],[16,344,36],[16,375,72]]",5111));//R16_375×R2_2
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[2,2,9],[4,5,2],[4,6,13],[8,38,2]],[[2,1,5],[2,2,9],[4,4,6],[4,5,5],[4,6,52],[4,8,36],[4,10,12],[8,27,18],[8,28,12],[8,34,36],[8,38,17],[8,40,36],[8,47,36],[16,285,72],[16,344,36],[16,356,36]]",5054));//R16_285×R2_2	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,1,4],[4,2,4],[4,5,4],[4,6,3],[8,7,4],[8,17,4],[8,38,4]],[[2,1,3],[2,2,1],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[4,6,12],[8,6,12],[8,7,34],[8,9,12],[8,10,24],[8,12,12],[8,17,34],[8,18,12],[8,26,12],[8,27,9],[8,38,34],[16,239,36],[16,241,36],[16,242,72],[16,244,36],[16,246,36],[16,306,36]]",4407));//R16_228×R2_2
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,1,4],[4,2,4],[4,5,4],[4,6,3],[8,7,4],[8,17,4],[8,38,4]],[[2,1,3],[2,2,1],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[4,6,12],[8,5,12],[8,7,34],[8,16,12],[8,17,34],[8,26,12],[8,27,9],[8,38,34],[16,164,48],[16,208,36],[16,214,36],[16,306,36]]",4362));//R16_164×R2_2	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,8],[4,2,8],[4,5,8]],[[2,1,7],[4,1,20],[4,2,20],[4,4,21],[4,5,20],[8,5,72],[8,16,72],[8,26,72],[16,164,192]]",4361));//R16_164×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,8],[4,2,8],[4,5,8]],[[2,1,7],[4,1,20],[4,2,20],[4,4,21],[4,5,20],[8,5,24],[8,6,48],[8,9,48],[8,10,96],[8,12,48],[8,16,24],[8,18,48],[8,26,72]]",4406));//R16_228×R2_1	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,1,4],[4,2,4],[4,5,4],[4,6,3],[8,7,4],[8,17,4],[8,38,4]],[[2,1,3],[2,2,1],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[4,6,12],[8,6,12],[8,7,34],[8,8,24],[8,9,12],[8,12,12],[8,17,34],[8,18,12],[8,26,12],[8,27,9],[8,38,34],[16,239,36],[16,240,72],[16,241,36],[16,244,36],[16,246,36],[16,306,36]]",4350));//R16_156×R2_2
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,1,4],[4,2,4],[4,5,4],[4,6,3],[8,7,4],[8,17,4],[8,38,4]],[[2,1,3],[2,2,1],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[4,6,12],[8,5,12],[8,7,34],[8,16,12],[8,17,34],[8,26,12],[8,27,9],[8,38,34],[16,166,48],[16,208,36],[16,214,36],[16,306,36]]",4365));//R16_166×R2_2	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,8],[4,2,8],[4,5,8]],[[2,1,7],[4,1,20],[4,2,20],[4,4,21],[4,5,20],[8,5,24],[8,6,48],[8,8,96],[8,9,48],[8,12,48],[8,16,24],[8,18,48],[8,26,72]]",4349));//R16_156×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,8],[4,2,8],[4,5,8]],[[2,1,7],[4,1,20],[4,2,20],[4,4,21],[4,5,20],[8,5,72],[8,16,72],[8,26,72],[16,166,192]]",4364));//R16_166×R2_1	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,4],[4,6,4],[4,11,8],[8,42,8]],[[2,1,7],[2,2,4],[4,4,21],[4,6,16],[4,8,18],[4,11,20],[8,34,54],[8,42,68],[16,378,72]]",5046));//R16_378×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,4],[4,6,4],[4,11,8],[8,42,8]],[[2,1,7],[2,2,4],[4,4,21],[4,6,16],[4,7,18],[4,11,20],[8,29,54],[8,42,68],[16,381,72]]",5146));//R16_381×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,11],[2,2,4],[4,5,4],[4,6,4],[4,9,4],[8,35,4]],[[2,1,11],[2,2,4],[4,4,39],[4,5,10],[4,6,16],[4,7,18],[4,9,16],[8,26,12],[8,28,48],[8,29,54],[8,35,52],[8,37,18],[16,336,54],[16,373,36]]",5138));//R16_373×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,11],[2,2,4],[4,5,4],[4,6,4],[4,9,4],[8,35,4]],[[2,1,11],[2,2,4],[4,4,39],[4,5,10],[4,6,16],[4,8,18],[4,9,16],[8,26,12],[8,28,48],[8,34,54],[8,35,52],[8,44,18],[16,352,54],[16,370,36]]",5038));//R16_370×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,9],[4,6,9],[4,9,3],[8,35,3]],[[2,1,7],[2,2,9],[4,4,21],[4,6,36],[4,7,12],[4,8,12],[4,9,12],[4,10,12],[8,29,36],[8,34,36],[8,35,39],[8,36,12],[8,40,36],[8,45,9],[8,49,24],[16,295,12],[16,329,27],[16,330,72],[16,335,36]]",5170));//R16_295×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,9],[4,6,7],[4,9,5],[8,35,3]],[[2,1,7],[2,2,9],[4,4,21],[4,6,28],[4,8,36],[4,9,20],[4,10,12],[8,27,9],[8,34,54],[8,35,39],[8,44,54],[8,47,36],[8,48,36],[16,314,27],[16,318,108]]",5470));//R8R4_471=R8_43×R4_9
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[2,2,1],[4,6,1],[4,9,7],[8,35,7]],[[2,1,15],[2,2,1],[4,4,105],[4,6,4],[4,9,28],[8,35,91],[8,45,63],[16,329,189]]",5174));//R16_293×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[2,2,1],[4,6,7],[4,9,1],[8,35,7]],[[2,1,15],[2,2,1],[4,4,57],[4,6,28],[4,9,4],[8,27,27],[8,31,48],[8,35,91],[16,314,81],[16,338,36]]",5474));//R8R4_339=R8_31×R4_9	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,1],[4,1,8],[4,6,7],[8,7,8]],[[2,1,7],[2,2,1],[4,1,20],[4,4,21],[4,6,28],[8,5,24],[8,7,68],[8,27,63],[16,208,72],[16,234,48]]",4454));//R16_234×R2_2
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,1],[4,1,8],[4,6,7],[8,7,8]],[[2,1,7],[2,2,1],[4,1,20],[4,4,21],[4,6,28],[8,5,24],[8,7,68],[8,11,48],[8,27,63],[16,208,72],[16,243,144]]",4154));//R8R4_116=R8_11×R4_6
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[4,1,16]],[[2,1,15],[4,1,40],[4,4,105],[8,5,144],[16,234,192]]",4452));//R16_234×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[4,1,16]],[[2,1,15],[4,1,40],[4,4,105],[8,5,144],[8,11,192]]",4152));//R8R4_114=R8_11×R4_4
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,1,8],[4,5,4],[4,6,3],[8,7,8],[8,38,4]],[[2,1,3],[2,2,1],[4,1,20],[4,4,3],[4,5,10],[4,6,12],[8,5,24],[8,7,68],[8,26,12],[8,27,9],[8,38,34],[16,208,72],[16,232,48],[16,306,36]]",4538));//R16_232×R2_2
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,1,8],[4,5,4],[4,6,3],[8,7,8],[8,38,4]],[[2,1,3],[2,2,1],[4,1,20],[4,4,3],[4,5,10],[4,6,12],[8,5,24],[8,7,68],[8,9,48],[8,26,12],[8,27,9],[8,38,34],[16,208,72],[16,241,144],[16,306,36]]",4138));//R8R4_94=R8_9×R4_6
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,16],[4,5,8]],[[2,1,7],[4,1,40],[4,4,21],[4,5,20],[8,5,144],[8,26,72],[16,232,192]]",4536));//R16_232×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,16],[4,5,8]],[[2,1,7],[4,1,40],[4,4,21],[4,5,20],[8,5,144],[8,9,192],[8,26,72]]",4136));//R8R4_92=R8_9×R4_4	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,5,8],[8,23,16]],[[2,1,7],[4,4,21],[4,5,20],[8,23,88],[8,26,72],[16,188,192],[16,200,96]]",4249));//R16_188×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,5,8],[8,23,16]],[[2,1,7],[4,4,21],[4,5,20],[8,23,88],[8,26,72],[16,200,96]]",4249));//R8R4_247=R8_23×R4_5	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[4,1,16]],[[2,1,15],[4,1,40],[4,4,57],[8,5,192],[8,11,144],[8,31,48]]",4498));//R16_131×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[4,1,16]],[[2,1,15],[4,1,40],[4,4,57],[8,5,192],[8,31,48],[16,234,144]]",4298));//R8R4_331=R8_31×R4_1	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,4],[4,2,4],[4,3,8],[8,13,8]],[[2,1,7],[4,1,10],[4,2,10],[4,3,20],[4,4,21],[8,5,12],[8,8,48],[8,13,68],[8,16,12],[8,20,24],[16,101,48],[16,203,72]]",3021));//R16_101×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,4],[4,2,4],[4,3,8],[8,13,8]],[[2,1,7],[4,1,10],[4,2,10],[4,3,20],[4,4,21],[8,5,12],[8,10,48],[8,13,68],[8,15,24],[8,16,12],[16,102,48],[16,204,72]]",3221));//R16_102×R2_1	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,2,8],[8,23,16]],[[2,1,7],[4,2,20],[4,4,21],[8,16,72],[8,23,88],[16,75,192],[16,200,96]]",3251));//R16_75×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,2,8],[8,23,16]],[[2,1,7],[4,2,20],[4,4,21],[8,16,72],[8,23,88],[16,200,96]]",3051));//R8R4_244=R8_23×R4_2	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,8],[8,23,16]],[[2,1,7],[4,1,20],[4,4,21],[8,5,72],[8,23,88],[16,97,192],[16,200,96]]",3195));//R16_97×R2_1
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,1,8],[8,23,16]],[[2,1,7],[4,1,20],[4,4,21],[8,5,72],[8,23,88],[16,200,96]]",3095));//R8R4_243=R8_23×R4_1	
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
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,4],[4,5,8],[4,6,4],[8,38,8]],[[2,1,7],[2,2,4],[4,4,21],[4,5,20],[4,6,16],[4,8,18],[8,26,72],[8,34,54],[8,38,68],[16,355,216]]",5466));//R8R4_467=R8_43×R4_5
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,4],[4,5,8],[4,6,4],[8,38,8]],[[2,1,7],[2,2,4],[4,4,21],[4,5,20],[4,6,16],[4,7,18],[8,26,72],[8,29,54],[8,38,68],[16,309,216]]",5419));//R8R4_324=R8_30×R4_5
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,5,8],[4,6,2],[4,9,2],[8,35,2],[8,38,4],[16,315,4]],[[2,1,7],[2,2,2],[4,4,21],[4,5,20],[4,6,8],[4,8,3],[4,9,8],[8,26,72],[8,34,9],[8,35,26],[8,38,34],[8,44,9],[16,315,106],[16,352,27],[16,355,36]]",5477));//R8R4_478=R8_44×R4_5
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,5,8],[4,6,2],[4,9,2],[8,35,2],[8,38,4],[16,315,4]],[[2,1,7],[2,2,2],[4,4,21],[4,5,20],[4,6,8],[4,7,3],[4,9,8],[8,26,72],[8,29,9],[8,35,26],[8,37,9],[8,38,34],[16,309,36],[16,315,106],[16,336,27]]",5400));//R8R4_401=R8_37×R4_5
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,5,4],[4,6,7],[8,38,12]],[[2,1,3],[2,2,5],[4,4,3],[4,5,10],[4,6,28],[4,8,6],[4,10,6],[8,26,12],[8,27,9],[8,34,18],[8,38,102],[8,40,18],[8,47,6],[16,306,36],[16,319,72],[16,355,72],[16,356,18]]",5414));//R8R4_415=R8_38×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,5,4],[4,6,7],[8,38,12]],[[2,1,3],[2,2,5],[4,4,3],[4,5,10],[4,6,28],[4,7,6],[4,10,6],[8,26,12],[8,27,9],[8,29,18],[8,38,102],[8,40,18],[8,41,6],[16,306,36],[16,309,72],[16,310,18],[16,319,72]]",5413));//R8R4_414=R8_38×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,5,8],[4,6,6],[8,38,8]],[[2,1,7],[2,2,2],[4,4,21],[4,5,20],[4,6,24],[4,8,3],[8,26,72],[8,27,18],[8,34,27],[8,38,68],[16,306,72],[16,354,18],[16,355,72]]",5360));//R8R4_283=R8_26×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,5,8],[4,6,6],[8,38,8]],[[2,1,7],[2,2,2],[4,4,21],[4,5,20],[4,6,24],[4,7,3],[8,26,72],[8,27,18],[8,29,27],[8,38,68],[16,306,72],[16,308,18],[16,309,72]]",5359));//R8R4_282=R8_26×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,9],[4,6,15]],[[2,1,7],[2,2,9],[4,4,21],[4,6,60],[4,8,36],[4,10,12],[8,27,63],[8,34,108],[8,40,36],[8,47,36],[16,356,108]]",5467));//R8R4_468=R8_43×R4_6
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,9],[4,6,15]],[[2,1,7],[2,2,9],[4,4,21],[4,6,60],[4,7,36],[4,10,12],[8,27,63],[8,29,108],[8,40,36],[8,41,36],[16,310,108]]",5420));//R8R4_325=R8_30×R4_6	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,5],[4,6,11],[4,9,4],[8,35,4]],[[2,1,7],[2,2,5],[4,4,21],[4,6,44],[4,8,6],[4,9,16],[4,10,6],[8,27,63],[8,34,18],[8,35,52],[8,40,18],[8,44,18],[8,47,6],[8,48,18],[16,316,54],[16,347,18],[16,352,54],[16,356,18]]",5478));//R8R4_479=R8_44×R4_6
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,5],[4,6,11],[4,9,4],[8,35,4]],[[2,1,7],[2,2,5],[4,4,21],[4,6,44],[4,7,6],[4,9,16],[4,10,6],[8,27,63],[8,29,18],[8,35,52],[8,37,18],[8,40,18],[8,41,6],[8,48,18],[16,310,18],[16,316,54],[16,336,54],[16,341,18]]",5401));//R8R4_402=R8_37×R4_6
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,4],[4,6,2],[8,1,8],[8,7,4],[16,13,8]],[[2,1,3],[2,2,2],[4,1,10],[4,4,3],[4,6,8],[4,7,3],[8,1,44],[8,5,12],[8,7,34],[8,29,9],[16,12,48],[16,13,140],[16,209,36]]",2034));//R8R4_7=R8_1×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,4],[4,6,2],[8,1,8],[8,7,4],[16,13,8]],[[2,1,3],[2,2,2],[4,1,10],[4,4,3],[4,6,8],[4,8,3],[8,1,44],[8,5,12],[8,7,34],[8,34,9],[16,12,48],[16,13,140],[16,255,36]]",2035));//R8R4_8=R8_1×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,11],[4,6,17]],[[2,1,3],[2,2,11],[4,4,3],[4,6,68],[4,7,12],[4,10,33],[8,27,27],[8,29,36],[8,40,117],[8,41,30],[8,50,6],[16,307,18],[16,310,90],[16,320,18],[16,321,6]]",5397));//R8R4_318=R8_29×R4_10
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,11],[4,6,17]],[[2,1,3],[2,2,11],[4,4,3],[4,6,68],[4,8,12],[4,10,33],[8,27,27],[8,34,36],[8,40,117],[8,47,30],[8,50,6],[16,307,18],[16,320,18],[16,322,6],[16,356,90]]",5372));//R8R4_373=R8_34×R4_10
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,4],[4,6,2],[8,4,8],[8,7,4],[16,15,8]],[[2,1,3],[2,2,2],[4,1,10],[4,4,3],[4,6,8],[4,7,3],[8,4,44],[8,5,12],[8,7,34],[8,29,9],[16,9,48],[16,15,140],[16,209,36]]",2039));//R8R4_40=R8_4×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,4],[4,6,2],[8,4,8],[8,7,4],[16,15,8]],[[2,1,3],[2,2,2],[4,1,10],[4,4,3],[4,6,8],[4,8,3],[8,4,44],[8,5,12],[8,7,34],[8,34,9],[16,9,48],[16,15,140],[16,255,36]]",2040));//R8R4_41=R8_4×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,4],[4,2,4],[4,5,4],[4,6,2],[8,7,4],[8,17,4],[8,38,4]],[[2,1,3],[2,2,2],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[4,6,8],[4,7,3],[8,5,12],[8,6,48],[8,7,34],[8,16,12],[8,17,34],[8,26,12],[8,29,9],[8,38,34],[16,209,36],[16,215,36],[16,239,72],[16,309,36]]",4115));//R8R4_62=R8_6×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,4],[4,2,4],[4,5,4],[4,6,2],[8,7,4],[8,17,4],[8,38,4]],[[2,1,3],[2,2,2],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[4,6,8],[4,8,3],[8,5,12],[8,6,48],[8,7,34],[8,16,12],[8,17,34],[8,26,12],[8,34,9],[8,38,34],[16,239,72],[16,255,36],[16,256,36],[16,355,36]]",4116));//R8R4_63=R8_6×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,1,4],[4,2,4],[4,6,6],[8,7,4],[8,17,4]],[[2,1,7],[2,2,2],[4,1,10],[4,2,10],[4,4,21],[4,6,24],[4,7,3],[8,5,12],[8,7,34],[8,8,48],[8,16,12],[8,17,34],[8,27,18],[8,29,27],[16,209,36],[16,215,36],[16,240,72],[16,308,18]]",4131));//R8R4_84=R8_8×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,1,4],[4,2,4],[4,6,6],[8,7,4],[8,17,4]],[[2,1,7],[2,2,2],[4,1,10],[4,2,10],[4,4,21],[4,6,24],[4,8,3],[8,5,12],[8,7,34],[8,8,48],[8,16,12],[8,17,34],[8,27,18],[8,34,27],[16,240,72],[16,255,36],[16,256,36],[16,354,18]]",4132));//R8R4_85=R8_8×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,8],[4,5,4],[4,6,2],[8,7,8],[8,38,4]],[[2,1,3],[2,2,2],[4,1,20],[4,4,3],[4,5,10],[4,6,8],[4,7,3],[8,5,24],[8,7,68],[8,9,48],[8,26,12],[8,29,9],[8,38,34],[16,209,72],[16,241,72],[16,309,36]]",4139));//R8R4_95=R8_9×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,8],[4,5,4],[4,6,2],[8,7,8],[8,38,4]],[[2,1,3],[2,2,2],[4,1,20],[4,4,3],[4,5,10],[4,6,8],[4,8,3],[8,5,24],[8,7,68],[8,9,48],[8,26,12],[8,34,9],[8,38,34],[16,241,72],[16,255,72],[16,355,36]]",4140));//R8R4_96=R8_9×R4_8	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,1,4],[4,2,4],[4,6,6],[8,7,4],[8,17,4]],[[2,1,7],[2,2,2],[4,1,10],[4,2,10],[4,4,21],[4,6,24],[4,7,3],[8,5,12],[8,7,34],[8,10,48],[8,16,12],[8,17,34],[8,27,18],[8,29,27],[16,209,36],[16,215,36],[16,242,72],[16,308,18]]",4147));//R8R4_106=R8_10×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,1,4],[4,2,4],[4,6,6],[8,7,4],[8,17,4]],[[2,1,7],[2,2,2],[4,1,10],[4,2,10],[4,4,21],[4,6,24],[4,8,3],[8,5,12],[8,7,34],[8,10,48],[8,16,12],[8,17,34],[8,27,18],[8,34,27],[16,242,72],[16,255,36],[16,256,36],[16,354,18]]",4148));//R8R4_107=R8_10×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,1,8],[4,6,6],[8,7,8]],[[2,1,7],[2,2,2],[4,1,20],[4,4,21],[4,6,24],[4,7,3],[8,5,24],[8,7,68],[8,11,48],[8,27,18],[8,29,27],[16,209,72],[16,243,72],[16,308,18]]",4155));//R8R4_117=R8_11×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,1,8],[4,6,6],[8,7,8]],[[2,1,7],[2,2,2],[4,1,20],[4,4,21],[4,6,24],[4,8,3],[8,5,24],[8,7,68],[8,11,48],[8,27,18],[8,34,27],[16,243,72],[16,255,72],[16,354,18]]",4156));//R8R4_118=R8_11×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,4],[4,2,4],[4,5,4],[4,6,2],[8,7,4],[8,17,4],[8,38,4]],[[2,1,3],[2,2,2],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[4,6,8],[4,7,3],[8,5,12],[8,7,34],[8,12,48],[8,16,12],[8,17,34],[8,26,12],[8,29,9],[8,38,34],[16,209,36],[16,215,36],[16,244,72],[16,309,36]]",4163));//R8R4_128=R8_12×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,4],[4,2,4],[4,5,4],[4,6,2],[8,7,4],[8,17,4],[8,38,4]],[[2,1,3],[2,2,2],[4,1,10],[4,2,10],[4,4,3],[4,5,10],[4,6,8],[4,8,3],[8,5,12],[8,7,34],[8,12,48],[8,16,12],[8,17,34],[8,26,12],[8,34,9],[8,38,34],[16,244,72],[16,255,36],[16,256,36],[16,355,36]]",4164));//R8R4_129=R8_12×R4_8	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,3,12],[4,6,6],[8,13,4]],[[2,1,7],[2,2,2],[4,3,30],[4,4,21],[4,6,24],[4,7,3],[8,13,34],[8,14,48],[8,15,36],[8,20,24],[8,27,18],[8,29,27],[16,201,24],[16,204,36],[16,222,48],[16,245,48],[16,308,18]]",4187));//R8R4_161=R8_15×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,3,12],[4,6,6],[8,13,4]],[[2,1,7],[2,2,2],[4,3,30],[4,4,21],[4,6,24],[4,8,3],[8,13,34],[8,14,48],[8,15,84],[8,27,18],[8,34,27],[16,204,36],[16,223,48],[16,245,48],[16,354,18]]",4188));//R8R4_162=R8_15×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,2,8],[4,5,4],[4,6,2],[8,17,8],[8,38,4]],[[2,1,3],[2,2,2],[4,2,20],[4,4,3],[4,5,10],[4,6,8],[4,7,3],[8,16,24],[8,17,68],[8,18,48],[8,26,12],[8,29,9],[8,38,34],[16,215,72],[16,246,72],[16,309,36]]",4211));//R8R4_194=R8_18×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,2,8],[4,5,4],[4,6,2],[8,17,8],[8,38,4]],[[2,1,3],[2,2,2],[4,2,20],[4,4,3],[4,5,10],[4,6,8],[4,8,3],[8,16,24],[8,17,68],[8,18,48],[8,26,12],[8,34,9],[8,38,34],[16,246,72],[16,256,72],[16,355,36]]",4212));//R8R4_195=R8_18×R4_8	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,2,8],[4,6,6],[8,17,8]],[[2,1,7],[2,2,2],[4,2,20],[4,4,21],[4,6,24],[4,7,3],[8,16,24],[8,17,68],[8,19,48],[8,27,18],[8,29,27],[16,215,72],[16,253,72],[16,308,18]]",4219));//R8R4_205=R8_19×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,2,8],[4,6,6],[8,17,8]],[[2,1,7],[2,2,2],[4,2,20],[4,4,21],[4,6,24],[4,8,3],[8,16,24],[8,17,68],[8,19,48],[8,27,18],[8,34,27],[16,253,72],[16,256,72],[16,354,18]]",4220));//R8R4_206=R8_19×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,6,6],[8,23,8],[16,249,8]],[[2,1,7],[2,2,2],[4,4,21],[4,6,24],[4,7,3],[8,23,44],[8,27,18],[8,29,27],[16,200,48],[16,249,140],[16,308,18]]",4251));//R8R4_249=R8_23×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,6,6],[8,23,8],[16,249,8]],[[2,1,7],[2,2,2],[4,4,21],[4,6,24],[4,8,3],[8,23,44],[8,27,18],[8,34,27],[16,200,48],[16,249,140],[16,354,18]]",4252));//R8R4_250=R8_23×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,6,6],[8,24,8],[16,250,8]],[[2,1,7],[2,2,2],[4,4,21],[4,6,24],[4,7,3],[8,24,44],[8,27,18],[8,29,27],[16,238,48],[16,250,140],[16,308,18]]",4259));//R8R4_260=R8_24×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,6,6],[8,24,8],[16,250,8]],[[2,1,7],[2,2,2],[4,4,21],[4,6,24],[4,8,3],[8,24,44],[8,27,18],[8,34,27],[16,238,48],[16,250,140],[16,354,18]]",4260));//R8R4_261=R8_24×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,11],[2,2,2],[4,5,4],[4,6,10],[8,38,4]],[[2,1,11],[2,2,2],[4,4,39],[4,5,10],[4,6,40],[4,7,3],[8,26,12],[8,27,36],[8,28,48],[8,29,45],[8,38,34],[16,308,36],[16,309,36],[16,344,72]]",5495));//R8R4_304=R8_28×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,11],[2,2,2],[4,5,4],[4,6,10],[8,38,4]],[[2,1,11],[2,2,2],[4,4,39],[4,5,10],[4,6,40],[4,8,3],[8,26,12],[8,27,36],[8,28,48],[8,34,45],[8,38,34],[16,344,72],[16,354,36],[16,355,36]]",5528));//R8R4_305=R8_28×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[2,2,2],[4,6,14]],[[2,1,15],[2,2,2],[4,4,57],[4,6,56],[4,7,3],[8,27,54],[8,29,63],[8,31,48],[16,308,54],[16,338,72]]",5464));//R8R4_337=R8_31×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,15],[2,2,2],[4,6,14]],[[2,1,15],[2,2,2],[4,4,57],[4,6,56],[4,8,3],[8,27,54],[8,31,48],[8,34,63],[16,338,72],[16,354,54]]",5473));//R8R4_338=R8_31×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,5,8],[4,6,6],[8,38,8]],[[2,1,7],[2,2,2],[4,4,21],[4,5,20],[4,6,24],[4,7,3],[8,26,24],[8,27,18],[8,29,27],[8,32,48],[8,38,68],[16,308,18],[16,309,72],[16,339,72]]",5507));//R8R4_348=R8_32×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,5,8],[4,6,6],[8,38,8]],[[2,1,7],[2,2,2],[4,4,21],[4,5,20],[4,6,24],[4,8,3],[8,26,24],[8,27,18],[8,32,48],[8,34,27],[8,38,68],[16,339,72],[16,354,18],[16,355,72]]",5508));//R8R4_349=R8_32×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,6,6],[8,33,8],[16,346,8]],[[2,1,7],[2,2,2],[4,4,21],[4,6,24],[4,7,3],[8,27,18],[8,29,27],[8,33,44],[16,308,18],[16,345,48],[16,346,140]]",5550));//R8R4_359=R8_33×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,2],[4,6,6],[8,33,8],[16,346,8]],[[2,1,7],[2,2,2],[4,4,21],[4,6,24],[4,8,3],[8,27,18],[8,33,44],[8,34,27],[16,345,48],[16,346,140],[16,354,18]]",5551));//R8R4_360=R8_33×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,14],[4,6,10]],[[2,1,7],[2,2,14],[4,4,21],[4,6,40],[4,7,45],[4,8,18],[4,10,24],[8,27,18],[8,29,45],[8,34,18],[8,36,42],[8,41,48],[8,47,24],[16,308,18],[16,311,24],[16,312,24],[16,335,18],[16,340,24]]",5391));//R8R4_392=R8_36×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,14],[4,6,10]],[[2,1,7],[2,2,14],[4,4,21],[4,6,40],[4,7,18],[4,8,45],[4,10,24],[8,27,18],[8,29,18],[8,34,45],[8,36,42],[8,41,24],[8,47,48],[16,312,24],[16,313,24],[16,335,18],[16,340,24],[16,354,18]]",5392));//R8R4_393=R8_36×R4_8	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,5,12],[4,6,2],[8,38,12]],[[2,1,3],[2,2,2],[4,4,3],[4,5,30],[4,6,8],[4,7,3],[8,26,36],[8,29,9],[8,38,102],[8,39,48],[16,309,108],[16,342,72]]",5424));//R8R4_425=R8_39×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,5,12],[4,6,2],[8,38,12]],[[2,1,3],[2,2,2],[4,4,3],[4,5,30],[4,6,8],[4,8,3],[8,26,36],[8,34,9],[8,38,102],[8,39,48],[16,342,72],[16,355,108]]",5425));//R8R4_426=R8_39×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,17],[4,6,11]],[[2,1,3],[2,2,17],[4,4,3],[4,6,44],[4,7,18],[4,8,18],[4,10,60],[8,27,9],[8,29,18],[8,34,18],[8,36,12],[8,40,36],[8,41,54],[8,47,54],[8,50,12],[16,310,18],[16,312,36],[16,321,12],[16,322,12],[16,340,12],[16,356,18]]",5512));//R8R4_513=R8_47×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,17],[4,6,11]],[[2,1,3],[2,2,17],[4,4,3],[4,6,44],[4,8,48],[4,10,60],[8,27,9],[8,34,36],[8,40,36],[8,47,120],[8,50,12],[16,313,36],[16,322,24],[16,356,36]]",5513));//R8R4_514=R8_47×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,8],[4,6,8],[4,9,6],[8,35,2]],[[2,1,7],[2,2,8],[4,4,21],[4,6,32],[4,7,9],[4,8,9],[4,9,24],[4,10,12],[8,27,18],[8,29,9],[8,34,27],[8,35,26],[8,36,6],[8,37,27],[8,41,12],[8,44,18],[8,47,12],[8,48,36],[16,312,12],[16,318,36],[16,336,27],[16,341,36],[16,354,18],[16,358,18]]",5403));//R8R4_404=R8_37×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,8],[4,6,8],[4,9,6],[8,35,2]],[[2,1,7],[2,2,8],[4,4,21],[4,6,32],[4,7,9],[4,8,9],[4,9,24],[4,10,12],[8,27,18],[8,29,27],[8,34,9],[8,35,26],[8,36,6],[8,37,18],[8,41,12],[8,44,27],[8,47,12],[8,48,36],[16,308,18],[16,312,12],[16,317,36],[16,347,36],[16,352,27],[16,358,18]]",5479));//R8R4_480=R8_44×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,8],[4,6,8],[4,9,6],[8,35,2]],[[2,1,7],[2,2,8],[4,4,21],[4,6,32],[4,8,24],[4,9,24],[4,10,12],[8,27,18],[8,34,36],[8,35,26],[8,44,63],[8,47,24],[8,48,36],[16,313,12],[16,318,36],[16,347,36],[16,352,27],[16,354,18]]",5480));//R8R4_481=R8_44×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,14],[4,6,10]],[[2,1,7],[2,2,14],[4,4,21],[4,6,40],[4,7,54],[4,8,15],[4,10,24],[8,27,18],[8,29,54],[8,34,27],[8,36,36],[8,41,72],[8,47,24],[16,312,72],[16,354,18]]",5430));//R8R4_327=R8_30×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,14],[4,6,10]],[[2,1,7],[2,2,14],[4,4,21],[4,6,40],[4,7,15],[4,8,54],[4,10,24],[8,27,18],[8,29,27],[8,34,54],[8,36,36],[8,41,24],[8,47,72],[16,308,18],[16,312,72]]",5468));//R8R4_469=R8_43×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,14],[4,6,10]],[[2,1,7],[2,2,14],[4,4,21],[4,6,40],[4,8,105],[4,10,24],[8,27,18],[8,34,81],[8,47,96],[16,313,72],[16,354,18]]",5469));//R8R4_470=R8_43×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,6,7],[4,11,2],[8,42,6],[8,51,4],[16,326,4]],[[2,1,3],[2,2,5],[4,4,3],[4,6,28],[4,8,6],[4,10,6],[4,11,5],[8,27,9],[8,34,18],[8,40,18],[8,42,51],[8,47,6],[8,51,34],[16,324,36],[16,326,106],[16,328,36],[16,356,18]]",5373));//R8R4_374=R8_34×R4_11
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,5],[4,6,7],[4,11,2],[8,42,6],[8,51,4],[16,326,4]],[[2,1,3],[2,2,5],[4,4,3],[4,6,28],[4,7,6],[4,10,6],[4,11,5],[8,27,9],[8,29,18],[8,40,18],[8,41,6],[8,42,51],[8,51,34],[16,310,18],[16,324,36],[16,326,106],[16,327,36]]",5398));//R8R4_319=R8_29×R4_11
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,5],[4,6,11],[4,9,3],[8,35,5]],[[2,1,7],[2,2,5],[4,4,21],[4,6,44],[4,8,6],[4,9,12],[4,10,6],[8,27,27],[8,34,36],[8,35,65],[8,40,18],[8,44,9],[8,47,6],[8,48,18],[16,314,27],[16,316,54],[16,318,18],[16,352,27],[16,354,18],[16,356,18]]",5381));//R8R4_382=R8_35×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,5],[4,6,11],[4,9,3],[8,35,5]],[[2,1,7],[2,2,5],[4,4,21],[4,6,44],[4,7,6],[4,9,12],[4,10,6],[8,27,27],[8,29,36],[8,35,65],[8,37,9],[8,40,18],[8,41,6],[8,48,18],[16,308,18],[16,310,18],[16,314,27],[16,316,54],[16,317,18],[16,336,27]]",5380));//R8R4_381=R8_35×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,8],[4,6,16]],[[2,1,7],[2,2,8],[4,4,21],[4,6,64],[4,7,24],[4,10,12],[8,27,36],[8,29,108],[8,40,36],[8,41,24],[16,308,36],[16,310,72],[16,311,12]]",5386));//R8R4_315=R8_29×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,8],[4,6,16]],[[2,1,7],[2,2,8],[4,4,21],[4,6,64],[4,7,9],[4,8,9],[4,10,12],[8,27,36],[8,29,45],[8,34,45],[8,36,6],[8,40,36],[8,41,12],[8,47,12],[16,308,18],[16,310,36],[16,312,12],[16,335,18],[16,354,18],[16,356,36]]",5369));//R8R4_370=R8_34×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[2,2,8],[4,6,16]],[[2,1,7],[2,2,8],[4,4,21],[4,6,64],[4,8,24],[4,10,12],[8,27,36],[8,34,108],[8,40,36],[8,47,24],[16,313,12],[16,354,36],[16,356,72]]",5370));//R8R4_371=R8_34×R4_8
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,4],[4,6,2],[8,2,8],[8,7,4],[16,14,8]],[[2,1,3],[2,2,2],[4,1,10],[4,4,3],[4,6,8],[4,7,3],[8,2,44],[8,5,12],[8,7,34],[8,29,9],[16,10,48],[16,14,140],[16,209,36]]",2022));//R8R4_18=R8_2×R4_7
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,4],[4,6,2],[8,2,8],[8,7,4],[16,14,8]],[[2,1,3],[2,2,2],[4,1,10],[4,4,3],[4,6,8],[4,8,3],[8,2,44],[8,5,12],[8,7,34],[8,34,9],[16,10,48],[16,14,140],[16,255,36]]",2018));//R8R4_19=R8_2×R4_8
#endif
	//R36
	m_RingInvariant.insert(make_pair("[1,3,8,0,24,0,0,0,0],6,1,1,18,4,2,2,147,17,36",99));		
	m_RingInvariant.insert(make_pair("[1,3,8,0,24,0,0,0,0],6,1,1,20,4,1,1,136,19,36",119));	
	
	int cnt0=m_RingInvariant.size();
	int iret=LoadData("RingInvariant.csv");	
	int cnt1=m_RingInvariant.size();
    printf("iret=%d,%d-%d=%d\n",iret,cnt1,cnt0,cnt1-cnt0);	
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

vector<int> RIDHelper::IDFromN0n0bAbO(string& N0n0bAbO){
	std::multimap<string,int>::iterator it;
	std::pair<std::multimap<string,int>::iterator, std::multimap<string,int>::iterator> pa;
	vector<int> v;
	for( it = m_RingInvariant.begin(); it != m_RingInvariant.end(); ++it){
		string::size_type idx=it->first.find(N0n0bAbO);
		if(idx!=string::npos)
			v.push_back(it->second);
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
	string strN6=calcN6(r);	
    // N0n0bAbOn1n2n4n5n6n7n8S1N2N6
	char sz[2048]={0};
	sprintf(sz,"%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%s,%s",strN0.c_str(),n0,bA,bO,n1,n2,n4,n5,n6,n7,n8,strS1.c_str(),strN2.c_str(),strN6.c_str());	
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
		// 调用set_intersection之前要加上sort，否则计算的来的交集有可能不准。例如R32_2022和R32_2018
		std::sort(vID.begin(),vID.end());
		std::sort(vID2.begin(),vID2.end());
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
		static int IDs[]={34,67,68,69,118,119,120,121,122,123,124,125,275,276,277,278,279,280,281,282,286,287,289,290,126,127,283,377,28,33,146,182,29,30,31,32,70,190,153,155,159,163,165,128,147,148,149,160,162,167,168,169,183,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,93,94,95,101,102,103,104,105,106,107,108,109,110,111,112,113,91,92,96,97,98,99,100,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,150,151,152,154,156,157,158,161,164,166,170,174,177,179,184,185,186,187,188,189,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,171,172,173,175,176,181,178,180,284,285,288,291,292,293,294,295,296,297,298,299,300,301,302,303,304,305,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,335,336,337,338,339,340,341,342,343,344,345,346,347,348,349,350,351,352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,378,379,380,381,382,383,384,385,386,387,388,389,390};
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
static int IDs[]={-1,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,114,115,116,117,271,272,273,274};

bool isR16ID(int i){
	if(i==6||i==13||i==14||i==15||i==28||i==29||i==30||i==31||i==32||i==33||i==34||i==118||i==119||i==120||i==121||i==122||i==123||i==124||i==125||i==126||i==127||i==128||i==130||i==131|i==132||i==133||i==134||i==135||i==136||i==137||i==138||i==139||i==140||i==141||i==142||i==143||i==144||i==145||i==146||i==156||i==165||i==169||i==171||i==172||i==173||i==175||i==176||i==178||i==179||i==181||i==182||i==228||i==229||i==239||i==241||i==244||i==246||i==248||i==275||i==276||i==277||i==279||i==281||i==282||i==283||i==286||i==287)
		return true;
	return false;
}

//-----------------------------End of IdRing-------------------------------

#endif