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
	Subring(IRing* r,const vector<int>& gens);
	// 析构函数
	~Subring(){};	
	// 成员函数	
	// 成员变量
	vector<int> m_Set;
	IRing* m_r;
};

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
}

string itos(int i)
{
  stringstream s;
  s << i;
  return s.str();
}

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
	m_RingInvariant.insert(make_pair("2,1,0,8,1,7,7,40,7",31));
	m_RingInvariant.insert(make_pair("2,1,0,8,1,3,7,40,7",32));	
	m_RingInvariant.insert(make_pair("2,1,0,8,1,3,3,32,7",33));
	m_RingInvariant.insert(make_pair("[1,7,0,0],2,0,0,8,3,3,3,40,7,2,[1,5,2,0],[[2,2,24]]",34));	
	m_RingInvariant.insert(make_pair("2,1,0,8,2,3,3,32,7",35));	
	m_RingInvariant.insert(make_pair("2,0,0,8,5,3,3,32,7",36));	
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
	m_RingInvariant.insert(make_pair("16,1,0,16,1,3,7,80,15,16",2));
	m_RingInvariant.insert(make_pair("[1,1,2,4,8],16,1,1,8,2,3,3,48,7,16,[1,1,2,4,8],[[2,16,8],[4,8,8],[4,16,16],[8,4,8],[8,8,16],[8,16,32],[16,2,8],[16,4,16],[16,8,32],[16,16,64]]",3));//C16
	m_RingInvariant.insert(make_pair("[1,1,2,4,8],16,1,0,16,1,7,15,128,15,16,[1,1,2,4,8],[[8,16,32],[16,8,32],[16,16,64]]",4));//B16	
	m_RingInvariant.insert(make_pair("16,1,0,16,1,7,15,192,15,16",5));
	m_RingInvariant.insert(make_pair("8,1,1,12,4,1,3,60,11,16",6));
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
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,7,15,160,15,4,[1,3,8,4,0],[[4,4,96]]",121));	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,11,15,192,15,4,[1,3,8,4,0],[[4,4,64]]",122));	
	m_RingInvariant.insert(make_pair("[1,3,12,0,0],4,0,0,16,1,11,15,192,15,4,[1,3,12,0,0],[[4,4,64]]",123));	
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
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,0,16,4,3,3,96,15,16,[1,4,6,5,0],[[2,2,32],[2,4,40],[4,2,40],[4,4,48]]",228));	
	m_RingInvariant.insert(make_pair("[1,7,8,0,0],4,1,1,14,8,1,1,72,13,16,[1,4,11,0,0],[[2,2,40],[2,4,46],[4,2,46],[4,4,52]]",229));	
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
	m_RingInvariant.insert(make_pair("2,1,1,15,16,0,0,81,14,16",386));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,13,8,0,0,63,12,16,[1,7,2,6,0],[[2,2,193]]",387));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,9,4,0,0,45,8,16,[1,3,0,6,6],[[2,2,211]]",388));	
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,7,4,0,0,49,6,16,[1,3,8,4,0],[[2,2,207]]",389));
	m_RingInvariant.insert(make_pair("[1,15,0,0,0],2,1,1,1,2,0,0,31,0,16,[1,1,2,0,12],[[2,2,225]]",390));
	//R16
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[8,3,4]],[[2,1,3],[4,2,10],[4,4,3],[8,3,22],[8,16,12]]",11));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,1,8]],[[2,1,3],[4,1,10],[4,4,3],[8,1,44],[8,5,12]]",16));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12]]",17));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,2,8]],[[2,1,3],[4,1,10],[4,4,3],[8,2,44],[8,5,12]]",19));		
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[8,3,8]],[[2,1,3],[4,2,10],[4,4,3],[8,3,44],[8,16,12]]",20));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[8,1,4],[8,4,4]],[[2,1,3],[4,1,10],[4,4,3],[8,1,22],[8,4,22],[8,5,12]]",21));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4]],[[2,1,3],[4,1,10],[4,4,3],[8,5,12]]",22));		
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,3,6],[8,13,4],[8,21,2]],[[2,1,3],[4,3,15],[4,4,3],[8,13,34],[8,21,17]]",104));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,3,2],[8,21,2],[8,24,4]],[[2,1,3],[4,3,5],[4,4,3],[8,21,17],[8,24,22]]",105));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,3,2],[8,21,2]],[[2,1,3],[4,3,5],[4,4,3],[8,21,17]]",106));		
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4]],[[2,1,3],[4,2,10],[4,4,3],[8,16,12]]",109));		
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,3,2],[8,21,2]],[[2,1,3],[4,1,10],[4,3,5],[4,4,3],[8,11,12],[8,21,17]]",110));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[4,3,2],[8,13,2]],[[2,1,3],[4,2,10],[4,3,5],[4,4,3],[8,13,17],[8,16,12]]",113));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,2,4],[4,3,2],[8,21,2]],[[2,1,3],[4,2,10],[4,3,5],[4,4,3],[8,19,12],[8,21,17]]",115));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[4,1,4],[4,2,4],[8,23,4]],[[2,1,3],[4,1,10],[4,2,10],[4,4,3],[8,5,12],[8,16,12],[8,23,22]]",118));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,3,8]],[[2,1,7],[4,3,20],[4,4,21],[8,15,24],[8,20,24]]",201));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[8,24,8]],[[2,1,7],[4,4,21],[8,24,44]]",202));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,3,4],[8,13,4]],[[2,1,7],[4,3,10],[4,4,21],[8,13,34],[8,20,12]]",203));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,4],[4,6,2],[8,7,4]],[[2,1,3],[2,2,2],[4,1,10],[4,4,3],[4,6,8],[4,7,3],[8,5,12],[8,7,34],[8,29,9]]",209));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,3,2],[8,13,2],[8,21,2]],[[2,1,7],[4,3,5],[4,4,21],[8,13,17],[8,21,17]]",251));			
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,1,4],[4,6,2],[8,7,4]],[[2,1,3],[2,2,2],[4,1,10],[4,4,3],[4,6,8],[4,8,3],[8,5,12],[8,7,34],[8,34,9]]",255));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,3,4],[8,21,4]],[[2,1,7],[4,3,10],[4,4,21],[8,20,12],[8,21,34]]",265));		
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,7],[4,3,4],[8,21,4]],[[2,1,7],[4,3,10],[4,4,21],[8,15,12],[8,21,34]]",266));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,2,4],[4,6,2],[8,17,4]],[[2,1,3],[2,2,2],[4,2,10],[4,4,3],[4,6,8],[4,7,3],[8,16,12],[8,17,34],[8,29,9]]",215));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,2],[4,2,4],[4,6,2],[8,17,4]],[[2,1,3],[2,2,2],[4,2,10],[4,4,3],[4,6,8],[4,8,3],[8,16,12],[8,17,34],[8,34,9]]",256));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,3],[4,6,1],[4,9,2],[4,11,2],[8,42,2],[8,51,2]],[[2,1,1],[2,2,3],[4,6,4],[4,9,8],[4,10,3],[4,11,5],[8,42,17],[8,48,9],[8,51,17]]",302));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,5,4],[4,6,1],[4,9,1],[8,35,1],[8,38,2]],[[2,1,3],[2,2,1],[4,4,3],[4,5,10],[4,6,4],[4,9,4],[8,26,12],[8,35,13],[8,38,17]]",315));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,3],[4,6,3],[4,11,2],[8,42,2],[8,51,2]],[[2,1,1],[2,2,3],[4,6,12],[4,10,3],[4,11,5],[8,40,9],[8,42,17],[8,51,17]]",326));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,1],[4,6,1],[8,52,6]],[[2,1,1],[2,2,1],[4,6,4],[8,52,27]]",331));	
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
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,5],[2,2,4],[4,5,2],[4,9,4]],[[2,1,5],[2,2,4],[4,4,6],[4,5,5],[4,7,6],[4,8,6],[4,9,16],[8,28,12],[8,37,18],[8,44,18]]",368));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,5,4],[4,9,3],[8,46,4]],[[2,1,3],[2,2,1],[4,4,3],[4,5,10],[4,9,12],[8,26,12],[8,45,9],[8,46,34]]",377));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,11],[4,6,1],[4,9,2]],[[2,1,1],[2,2,11],[4,6,4],[4,7,6],[4,8,6],[4,9,8],[4,10,33],[8,41,6],[8,47,6],[8,48,9],[8,49,18],[8,50,6]]",380));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,9,3],[4,11,2]],[[2,1,3],[2,2,1],[4,4,3],[4,9,12],[4,11,5],[8,45,9]]",383));	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,1],[4,9,3],[8,33,4]],[[2,1,3],[2,2,1],[4,4,3],[4,9,12],[8,33,22],[8,45,9]]",385));
	m_I1I2.insert(make_pair("[[1,1,1],[2,2,7],[4,11,2],[8,51,6]],[[2,2,7],[4,10,18],[4,11,5],[8,50,3],[8,51,51]]",387));
	m_I1I2.insert(make_pair("[[1,1,1],[2,2,3],[8,52,6]],[[2,2,3],[4,10,3],[8,52,27]]",388));	
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
	//R32
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,31,31,1024,31,32",1000));//R8R4_1
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,768,31,32",1001));//R8R4_2
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,2,15,15,512,31,32",1002));//R8R4_3
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,15,512,31,32",1003));//R8R4_12
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,15,384,31,32",1004));//R8R4_13
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,2,7,7,256,31,32",1005));//R8R4_14
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,2,7,15,320,31,32",1006));//R8R4_23
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,2,3,15,240,31,32",1007));//R8R4_24
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,1,24,4,3,7,160,23,32",1008));//R8R4_25
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,15,31,768,31,32",1009));//R8R4_34
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,1,7,31,576,31,32",1010));//R8R4_35
	m_RingInvariant.insert(make_pair("[1,3,12,16,0,0],8,1,0,32,2,7,15,384,31,32",1011));//R8R4_36
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,15,15,512,31,32",2012));//R8R4_9
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,4,7,7,576,31,32",2013));//R8R4_10
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,7,448,31,32",2014));//R8R4_11
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,15,512,31,32",2015));//R8R4_15
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,7,15,384,31,32",2016));//R8R4_16
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,7,384,31,32",2017));//R8R4_17
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,7,7,320,31,8",2018));//R8R4_19
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,7,256,31,32",2019));//R8R4_20
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,4,3,3,288,31,32",2020));//R8R4_21
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,3,3,224,31,32",2021));//R8R4_22
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,7,7,320,31,8",2022));//R8R4_18
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,31,31,1024,31,32",2023));//R8R4_4
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,768,31,32",2024));//R8R4_5
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,15,320,31,32",2025));//R8R4_26
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,3,15,240,31,32",2026));//R8R4_27
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,4,3,7,240,31,32",2027));//R8R4_28
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,6,3,7,200,23,8",2028));//R8R4_29
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,6,3,7,200,31,8",2029));//R8R4_30
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,1,24,4,3,7,160,23,32",2030));//R8R4_31
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,1,28,8,1,3,180,27,32",2031));//R8R4_32
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,1,20,4,1,3,140,19,32",2032));//R8R4_33
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,15,15,768,31,32",2033));//R8R4_6
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,15,15,640,31,8",2034));//R8R4_7
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,15,15,640,31,8",2035));//R8R4_8
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,15,31,768,31,32",2036));//R8R4_37
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,1,7,31,576,31,32",2037));//R8R4_38
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,15,576,31,32",2038));//R8R4_39
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,7,15,480,31,8",2039));//R8R4_40
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,0,0,32,3,7,15,480,31,8",2040));//R8R4_41
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,7,15,384,31,32",2041));//R8R4_42
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,4,3,7,432,31,32",2042));//R8R4_43
	m_RingInvariant.insert(make_pair("[1,7,8,16,0,0],8,1,0,32,2,3,7,336,31,32",2043));//R8R4_44
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,31,31,1024,31,32",3044));//R8R4_45
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,768,31,32",3045));//R8R4_46
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,512,31,32",3046));//R8R4_47
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,576,31,32",3047));//R8R4_177
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,3,15,432,31,32",3048));//R8R4_178
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,4,3,7,288,31,32",3049));//R8R4_179
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,320,31,32",3050));//R8R4_201
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,576,31,32",3051));//R8R4_244
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,384,31,32",3052));//R8R4_245
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,31,31,640,31,32",3053));//R8R4_111
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,480,31,32",3054));//R8R4_112
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,768,31,32",3055));//R8R4_56
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,576,31,32",3056));//R8R4_57
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,384,31,32",3057));//R8R4_58
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,320,31,32",3058));//R8R4_113
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,15,384,31,32",3059));//R8R4_255
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,320,31,32",3060));//R8R4_232
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,15,15,448,31,8",3061));//R8R4_155
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,7,15,336,31,8",3062));//R8R4_156
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,6,7,7,224,31,8",3063));//R8R4_157
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,384,31,32",3064));//R8R4_221
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,288,31,32",3065));//R8R4_222
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,768,31,32",3066));//R8R4_67
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,576,31,32",3067));//R8R4_68
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,4,7,7,384,31,32",3068));//R8R4_69
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,24,4,7,7,192,23,32",3069));//R8R4_223
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,640,31,32",3070));//R8R4_122
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,480,31,32",3071));//R8R4_123
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,320,31,32",3072));//R8R4_124
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,7,31,640,31,8",3073));//R8R4_188
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,3,31,480,31,8",3074));//R8R4_189
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,3,15,320,31,8",3075));//R8R4_190
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,3,15,240,31,32",3076));//R8R4_233
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,768,31,8",3077));//R8R4_78
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8",3078));//R8R4_79
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,11,15,384,31,8",3079));//R8R4_80
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,15,15,448,31,8",3080));//R8R4_210
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,768,31,32",3081));//R8R4_166
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,576,31,32",3082));//R8R4_167
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,384,31,32",3083));//R8R4_168
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,15,15,512,31,32",3084));//R8R4_133
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,15,384,31,32",3085));//R8R4_134
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,4,7,7,256,31,32",3086));//R8R4_135
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,3,7,15,336,31,8",3087));//R8R4_211
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,768,31,8",3088));//R8R4_89
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8",3089));//R8R4_90
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,11,15,384,31,8",3090));//R8R4_91
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,6,7,7,224,23,8",3091));//R8R4_212
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,24,4,3,7,160,23,32",3092));//R8R4_234
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,2,7,7,256,31,32",3093));//R8R4_256
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,15,512,31,32",3094));//R8R4_254
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,768,31,32",3095));//R8R4_243
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,15,31,640,31,32",3096));//R8R4_199
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,1,7,31,480,31,32",3097));//R8R4_200
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,4,7,7,384,31,32",3098));//R8R4_144
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,23,31,768,31,8",3099));//R8R4_100
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,1,11,31,576,31,8",3100));//R8R4_101
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,0,0,32,2,11,15,384,31,8",3101));//R8R4_102
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,0,32,4,3,7,288,31,32",3102));//R8R4_145
	m_RingInvariant.insert(make_pair("[1,7,24,0,0,0],4,1,1,28,8,3,3,192,27,32",3103));//R8R4_146
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,31,31,1024,31,32",4104));//R8R4_48
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,768,31,32",4105));//R8R4_49
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,768,31,32",4106));//R8R4_50
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,640,31,8",4107));//R8R4_51
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,640,31,8",4108));//R8R4_52
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,512,31,32",4109));//R8R4_53
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,576,31,32",4110));//R8R4_54
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,448,31,32",4111));//R8R4_55
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,768,31,32",4112));//R8R4_59
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,576,31,32",4113));//R8R4_60
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32",4114));//R8R4_61
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8",4115));//R8R4_62
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8",4116));//R8R4_63
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32",4117));//R8R4_64
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,432,31,32",4118));//R8R4_65
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,336,31,32",4119));//R8R4_66
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,768,31,32",4120));//R8R4_70
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32",4121));//R8R4_71
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,576,31,32",4122));//R8R4_72
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,480,31,8",4123));//R8R4_73
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,480,31,8",4124));//R8R4_74
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,384,31,32",4125));//R8R4_75
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,3,3,432,31,32",4126));//R8R4_76
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,3,336,31,32",4127));//R8R4_77
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,768,31,8",4128));//R8R4_81
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8",4129));//R8R4_82
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,576,31,8",4130));//R8R4_83
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,11,15,480,31,2",4131));//R8R4_84
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,11,15,480,31,2",4132));//R8R4_85
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,384,31,8",4133));//R8R4_86
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,4,5,7,432,31,8",4134));//R8R4_87
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,5,7,336,31,8",4135));//R8R4_88
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,768,31,8",4136));//R8R4_92
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8",4137));//R8R4_93
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,576,31,8",4138));//R8R4_94
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,11,15,480,31,2",4139));//R8R4_95
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,11,15,480,31,2",4140));//R8R4_96
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,384,31,8",4141));//R8R4_97
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,4,5,7,432,31,8",4142));//R8R4_98
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,5,7,336,31,8",4143));//R8R4_99
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,768,31,8",4144));//R8R4_103
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8",4145));//R8R4_104
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,576,31,8",4146));//R8R4_105
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,11,15,480,31,2",4147));//R8R4_106
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,11,15,480,31,2",4148));//R8R4_107
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,384,31,8",4149));//R8R4_108
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,4,5,7,432,31,8",4150));//R8R4_109
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,5,7,336,31,8",4151));//R8R4_110
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,31,31,640,31,32",4152));//R8R4_114
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,480,31,32",4153));//R8R4_115
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,480,31,32",4154));//R8R4_116
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,400,31,8",4155));//R8R4_117
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,400,31,8",4156));//R8R4_118
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,320,31,32",4157));//R8R4_119
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,360,31,32",4158));//R8R4_120
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,280,31,32",4159));//R8R4_121
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,640,31,32",4160));//R8R4_125
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,480,31,32",4161));//R8R4_126
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,480,31,32",4162));//R8R4_127
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,400,31,8",4163));//R8R4_128
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,400,31,8",4164));//R8R4_129
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,320,31,32",4165));//R8R4_130
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,360,31,32",4166));//R8R4_131
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,280,31,32",4167));//R8R4_132
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,512,31,32",4168));//R8R4_136
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32",4169));//R8R4_137
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,384,31,32",4170));//R8R4_138
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,320,31,8",4171));//R8R4_139
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,320,31,8",4172));//R8R4_140
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,256,31,32",4173));//R8R4_141
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,3,3,288,31,32",4174));//R8R4_142
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,3,224,31,32",4175));//R8R4_143
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,384,31,32",4176));//R8R4_147
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,288,31,32",4177));//R8R4_148
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,3,3,288,31,32",4178));//R8R4_149
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,12,3,3,240,27,8",4179));//R8R4_150
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,12,3,3,240,31,8",4180));//R8R4_151
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,28,8,3,3,192,27,32",4181));//R8R4_152
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,30,16,1,1,216,29,32",4182));//R8R4_153
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,26,8,1,1,168,25,32",4183));//R8R4_154
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,448,31,8",4184));//R8R4_158
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,336,31,8",4185));//R8R4_159
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,336,31,8",4186));//R8R4_160
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,280,31,2",4187));//R8R4_161
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,280,31,2",4188));//R8R4_162
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,224,31,8",4189));//R8R4_163
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,12,3,3,252,31,8",4190));//R8R4_164
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,3,196,31,8",4191));//R8R4_165
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,768,31,32",4192));//R8R4_169
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,576,31,32",4193));//R8R4_170
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32",4194));//R8R4_171
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8",4195));//R8R4_172
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8",4196));//R8R4_173
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32",4197));//R8R4_174
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,432,31,32",4198));//R8R4_175
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,336,31,32",4199));//R8R4_176
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32",4200));//R8R4_180
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,15,432,31,32",4201));//R8R4_181
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,432,31,32",4202));//R8R4_182
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,360,31,8",4203));//R8R4_183
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,360,31,8",4204));//R8R4_184
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,288,31,32",4205));//R8R4_185
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,1,3,324,31,32",4206));//R8R4_186
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,1,3,252,31,32",4207));//R8R4_187
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,640,31,8",4208));//R8R4_191
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,480,31,8",4209));//R8R4_192
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,480,31,8",4210));//R8R4_193
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,3,15,400,31,2",4211));//R8R4_194
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,3,15,400,31,2",4212));//R8R4_195
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,320,31,8",4213));//R8R4_196
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,4,1,7,360,31,8",4214));//R8R4_197
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,1,7,280,31,8",4215));//R8R4_198
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,640,31,32",4216));//R8R4_202
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,480,31,32",4217));//R8R4_203
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,480,31,32",4218));//R8R4_204
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,400,31,8",4219));//R8R4_205
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,400,31,8",4220));//R8R4_206
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,320,31,32",4221));//R8R4_207
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,360,31,32",4222));//R8R4_208
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,280,31,32",4223));//R8R4_209
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,448,31,8",4224));//R8R4_213
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,336,31,8",4225));//R8R4_214
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,336,31,8",4226));//R8R4_215
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,280,23,2",4227));//R8R4_216
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,9,7,7,280,31,2",4228));//R8R4_217
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,224,23,8",4229));//R8R4_218
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,12,3,3,252,27,8",4230));//R8R4_219
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,3,196,19,8",4231));//R8R4_220
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,384,31,32",4232));//R8R4_224
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,288,31,32",4233));//R8R4_225
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,288,31,32",4234));//R8R4_226
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,240,23,8",4235));//R8R4_227
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,240,31,8",4236));//R8R4_228
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,24,4,7,7,192,23,32",4237));//R8R4_229
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,28,8,3,3,216,27,32",4238));//R8R4_230
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,20,4,3,3,168,19,32",4239));//R8R4_231
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,320,31,32",4240));//R8R4_235
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,15,240,31,32",4241));//R8R4_236
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,240,31,32",4242));//R8R4_237
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,200,23,8",4243));//R8R4_238
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,200,31,8",4244));//R8R4_239
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,24,4,3,7,160,23,32",4245));//R8R4_240
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,28,8,1,3,180,27,32",4246));//R8R4_241
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,20,4,1,3,140,19,32",4247));//R8R4_242
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,768,31,32",4248));//R8R4_246
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,576,31,32",4249));//R8R4_247
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32",4250));//R8R4_248
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8",4251));//R8R4_249
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8",4252));//R8R4_250
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32",4253));//R8R4_251
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,432,31,32",4254));//R8R4_252
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,336,31,32",4255));//R8R4_253
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,15,512,31,32",4256));//R8R4_257
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,15,384,31,32",4257));//R8R4_258
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,384,31,32",4258));//R8R4_259
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,7,320,31,8",4259));//R8R4_260
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,7,320,31,8",4260));//R8R4_261
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,256,31,32",4261));//R8R4_262
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,3,288,31,32",4262));//R8R4_263
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,3,224,31,32",4263));//R8R4_264
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,31,31,1024,31,32",4264));//R8R4_265
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,768,31,32",4265));//R8R4_266
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,512,31,32",4266));//R8R4_267
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,12,3,3,240,31,8",4267));//R8R4_509
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,384,31,32",4268));//R8R4_518
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,288,31,32",4269));//R8R4_519
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,28,8,3,3,192,27,32",4270));//R8R4_520
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,416,31,8",4271));//R8R4_529
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,312,31,8",4272));//R8R4_530
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,1,28,12,3,3,208,27,8",4273));//R8R4_531
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,3,3,432,31,32",4274));//R8R4_540
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,768,31,32",4275));//R8R4_276
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,576,31,32",4276));//R8R4_277
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32",4277));//R8R4_278
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,1,3,324,31,32",4278));//R8R4_541
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,30,16,1,1,216,29,32",4279));//R8R4_542
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,3,336,31,32",4280));//R8R4_551
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,1,3,252,31,32",4281));//R8R4_552
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,26,8,1,1,168,25,32",4282));//R8R4_553
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,3,240,31,32",4283));//R8R4_562
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,1,3,180,31,32",4284));//R8R4_563
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,18,4,1,1,120,17,32",4285));//R8R4_564
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,768,31,32",4286));//R8R4_287
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32",4287));//R8R4_288
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,384,31,32",4288));//R8R4_289
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,23,31,768,31,8",4289));//R8R4_298
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,11,31,576,31,8",4290));//R8R4_299
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,11,15,384,31,8",4291));//R8R4_300
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,640,31,8",4292));//R8R4_309
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8",4293));//R8R4_310
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,320,31,8",4294));//R8R4_311
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,15,15,576,31,4",4295));//R8R4_320
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,7,15,432,31,4",4296));//R8R4_321
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,10,7,7,288,23,4",4297));//R8R4_322
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,31,31,640,31,32",4298));//R8R4_331
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,480,31,32",4299));//R8R4_332
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,320,31,32",4300));//R8R4_333
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,31,640,31,32",4301));//R8R4_342
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,31,480,31,32",4302));//R8R4_343
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,320,31,32",4303));//R8R4_344
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,15,15,512,31,32",4304));//R8R4_353
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,1,7,15,384,31,32",4305));//R8R4_354
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,256,31,32",4306));//R8R4_355
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,640,31,8",4307));//R8R4_364
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,480,31,8",4308));//R8R4_365
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,320,31,8",4309));//R8R4_366
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,512,31,32",4310));//R8R4_375
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,384,31,32",4311));//R8R4_376
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,256,31,32",4312));//R8R4_377
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,15,15,512,31,4",4313));//R8R4_386
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,7,15,384,31,4",4314));//R8R4_387
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,10,7,7,256,31,4",4315));//R8R4_388
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,448,31,8",4316));//R8R4_397
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,336,31,8",4317));//R8R4_398
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,224,23,8",4318));//R8R4_399
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,576,31,32",4319));//R8R4_408
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,15,432,31,32",4320));//R8R4_409
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,288,31,32",4321));//R8R4_410
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,7,31,640,31,8",4322));//R8R4_419
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,1,3,31,480,31,8",4323));//R8R4_420
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,2,3,15,320,31,8",4324));//R8R4_421
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,7,7,576,31,32",4325));//R8R4_430
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,7,432,31,32",4326));//R8R4_431
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,8,3,3,288,31,32",4327));//R8R4_432
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,480,31,8",4328));//R8R4_441
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,360,31,8",4329));//R8R4_442
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,12,3,3,240,27,8",4330));//R8R4_443
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,7,448,31,32",4331));//R8R4_452
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,7,336,31,32",4332));//R8R4_453
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,4,3,3,224,31,32",4333));//R8R4_454
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,15,15,576,31,4",4334));//R8R4_463
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,5,7,15,432,31,4",4335));//R8R4_464
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,10,7,7,288,31,4",4336));//R8R4_465
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,15,15,448,31,8",4337));//R8R4_474
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,3,7,15,336,31,8",4338));//R8R4_475
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,224,31,8",4339));//R8R4_476
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,15,15,384,31,32",4340));//R8R4_485
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,288,31,32",4341));//R8R4_486
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,24,4,7,7,192,23,32",4342));//R8R4_487
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,7,15,320,31,32",4343));//R8R4_496
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,0,32,2,3,15,240,31,32",4344));//R8R4_497
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,1,1,24,4,3,7,160,23,32",4345));//R8R4_498
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,7,7,480,31,8",4346));//R8R4_507
	m_RingInvariant.insert(make_pair("[1,15,16,0,0,0],4,0,0,32,6,3,7,360,31,8",4347));//R8R4_508
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,31,31,1024,31,32",5348));//R8R4_268
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,15,31,768,31,32",5349));//R8R4_269
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,768,31,32",5350));//R8R4_270
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,640,31,8",5351));//R8R4_271
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,640,31,8",5352));//R8R4_272
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,512,31,32",5353));//R8R4_273
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,576,31,32",5354));//R8R4_274
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,7,448,31,32",5355));//R8R4_275
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,15,31,768,31,32",5356));//R8R4_279
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,7,31,576,31,32",5357));//R8R4_280
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,576,31,32",5358));//R8R4_281
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,480,31,8",5359));//R8R4_282
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,480,31,8",5360));//R8R4_283
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,384,31,32",5361));//R8R4_284
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,432,31,32",5362));//R8R4_285
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,7,336,31,32",5363));//R8R4_286
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,480,31,8",5364));//R8R4_293
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,480,31,8",5365));//R8R4_294
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,640,31,8",5366));//R8R4_367
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,480,31,8",5367));//R8R4_368
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,480,31,8",5368));//R8R4_369
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,400,31,2",5369));//R8R4_370
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,400,31,2",5370));//R8R4_371
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,320,31,8",5371));//R8R4_372
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8",5372));//R8R4_373
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,3,280,31,8",5373));//R8R4_374
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,384,31,32",5374));//R8R4_295
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,640,31,8",5375));//R8R4_312
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,480,31,8",5376));//R8R4_313
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,512,31,32",5377));//R8R4_378
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,384,31,32",5378));//R8R4_379
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,384,31,32",5379));//R8R4_380
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,320,31,8",5380));//R8R4_381
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,320,31,8",5381));//R8R4_382
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,256,31,32",5382));//R8R4_383
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,3,3,288,31,32",5383));//R8R4_384
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,3,224,31,32",5384));//R8R4_385
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,480,31,8",5385));//R8R4_314
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,400,31,2",5386));//R8R4_315
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,400,31,2",5387));//R8R4_316
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,15,15,512,31,4",5388));//R8R4_389
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,15,384,31,4",5389));//R8R4_390
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,384,31,4",5390));//R8R4_391
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,320,31,1",5391));//R8R4_392
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,320,31,1",5392));//R8R4_393
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,256,31,4",5393));//R8R4_394
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,20,3,3,288,31,4",5394));//R8R4_395
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,3,3,224,31,4",5395));//R8R4_396
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,320,31,8",5396));//R8R4_317
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8",5397));//R8R4_318
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,3,280,31,8",5398));//R8R4_319
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,448,31,8",5399));//R8R4_400
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,336,31,8",5400));//R8R4_401
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,336,31,8",5401));//R8R4_402
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,280,23,2",5402));//R8R4_403
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,280,31,2",5403));//R8R4_404
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,224,23,8",5404));//R8R4_405
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,252,27,8",5405));//R8R4_406
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,3,196,19,8",5406));//R8R4_407
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,3,3,432,31,32",5407));//R8R4_296
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,3,336,31,32",5408));//R8R4_297
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,768,31,32",5409));//R8R4_290
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,576,31,32",5410));//R8R4_411
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,15,432,31,32",5411));//R8R4_412
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,432,31,32",5412));//R8R4_413
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,360,31,8",5413));//R8R4_414
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,360,31,8",5414));//R8R4_415
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,288,31,32",5415));//R8R4_416
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,1,3,324,31,32",5416));//R8R4_417
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,1,3,252,31,32",5417));//R8R4_418
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,15,15,576,31,4",5418));//R8R4_323
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,15,432,31,4",5419));//R8R4_324
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,432,31,4",5420));//R8R4_325
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,7,31,640,31,8",5421));//R8R4_422
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,3,31,480,31,8",5422));//R8R4_423
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,3,15,480,31,8",5423));//R8R4_424
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,3,15,400,31,2",5424));//R8R4_425
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,3,15,400,31,2",5425));//R8R4_426
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,3,15,320,31,8",5426));//R8R4_427
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,4,1,7,360,31,8",5427));//R8R4_428
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,1,7,280,31,8",5428));//R8R4_429
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,360,23,1",5429));//R8R4_326
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,360,31,1",5430));//R8R4_327
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,288,23,4",5431));//R8R4_328
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,576,31,32",5432));//R8R4_433
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,432,31,32",5433));//R8R4_434
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,3,3,432,31,32",5434));//R8R4_435
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8",5435));//R8R4_436
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8",5436));//R8R4_437
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,3,3,288,31,32",5437));//R8R4_438
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,16,1,1,324,31,32",5438));//R8R4_439
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,1,1,252,31,32",5439));//R8R4_440
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,20,3,3,324,27,4",5440));//R8R4_329
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,3,3,252,19,4",5441));//R8R4_330
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,576,31,32",5442));//R8R4_291
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,480,31,8",5443));//R8R4_444
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,360,31,8",5444));//R8R4_445
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8",5445));//R8R4_446
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,300,27,2",5446));//R8R4_447
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,300,31,2",5447));//R8R4_448
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,240,27,8",5448));//R8R4_449
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,24,1,1,270,29,8",5449));//R8R4_450
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,1,1,210,25,8",5450));//R8R4_451
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,576,31,32",5451));//R8R4_292
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,23,31,768,31,8",5452));//R8R4_301
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,31,31,640,31,32",5453));//R8R4_334
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,7,448,31,32",5454));//R8R4_455
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,7,336,31,32",5455));//R8R4_456
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,3,336,31,32",5456));//R8R4_457
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,3,280,31,8",5457));//R8R4_458
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,3,280,31,8",5458));//R8R4_459
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,3,224,31,32",5459));//R8R4_460
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,1,1,252,31,32",5460));//R8R4_461
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,1,1,196,31,32",5461));//R8R4_462
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,15,31,480,31,32",5462));//R8R4_335
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,480,31,32",5463));//R8R4_336
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,400,31,8",5464));//R8R4_337
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,15,15,576,31,4",5465));//R8R4_466
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,5,7,15,432,31,4",5466));//R8R4_467
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,432,31,4",5467));//R8R4_468
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,360,31,1",5468));//R8R4_469
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,15,7,7,360,31,1",5469));//R8R4_470
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,7,7,288,31,4",5470));//R8R4_471
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,20,3,3,324,31,4",5471));//R8R4_472
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,10,3,3,252,31,4",5472));//R8R4_473
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,400,31,8",5473));//R8R4_338
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,320,31,32",5474));//R8R4_339
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,360,31,32",5475));//R8R4_340
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,15,15,448,31,8",5476));//R8R4_477
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,336,31,8",5477));//R8R4_478
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,336,31,8",5478));//R8R4_479
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,280,31,2",5479));//R8R4_480
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,9,7,7,280,31,2",5480));//R8R4_481
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,224,31,8",5481));//R8R4_482
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,252,31,8",5482));//R8R4_483
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,3,196,31,8",5483));//R8R4_484
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,7,280,31,32",5484));//R8R4_341
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,1,11,31,576,31,8",5485));//R8R4_302
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,11,15,576,31,8",5486));//R8R4_303
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,15,15,384,31,32",5487));//R8R4_488
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,288,31,32",5488));//R8R4_489
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,288,31,32",5489));//R8R4_490
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,240,23,8",5490));//R8R4_491
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,240,31,8",5491));//R8R4_492
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,24,4,7,7,192,23,32",5492));//R8R4_493
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,28,8,3,3,216,27,32",5493));//R8R4_494
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,20,4,3,3,168,19,32",5494));//R8R4_495
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,11,15,480,31,2",5495));//R8R4_304
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,15,31,640,31,32",5496));//R8R4_345
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,7,31,480,31,32",5497));//R8R4_346
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,320,31,32",5498));//R8R4_499
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,15,240,31,32",5499));//R8R4_500
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,240,31,32",5500));//R8R4_501
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,200,23,8",5501));//R8R4_502
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,200,31,8",5502));//R8R4_503
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,24,4,3,7,160,23,32",5503));//R8R4_504、R2_2/([0,0,1,1,1,1])=R32_5503
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,28,8,1,3,180,27,32",5504));//R8R4_505
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,20,4,1,3,140,19,32",5505));//R8R4_506、R2_2/([0,0,0,1,1,1])=R32_5505
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,480,31,32",5506));//R8R4_347
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,400,31,8",5507));//R8R4_348
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,15,400,31,8",5508));//R8R4_349
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,480,31,8",5509));//R8R4_510
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,360,31,8",5510));//R8R4_511
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,360,31,8",5511));//R8R4_512
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,300,31,2",5512));//R8R4_513
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,300,31,2",5513));//R8R4_514
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,240,31,8",5514));//R8R4_515
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,24,1,1,270,31,8",5515));//R8R4_516
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,1,1,210,31,8",5516));//R8R4_517
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,15,320,31,32",5517));//R8R4_350
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,360,31,32",5518));//R8R4_351
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,7,280,31,32",5519));//R8R4_352
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,7,7,384,31,32",5520));//R8R4_521
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,7,288,31,32",5521));//R8R4_522
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,3,3,288,31,32",5522));//R8R4_523
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,240,27,8",5523));//R8R4_524
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,240,31,8",5524));//R8R4_525
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,28,8,3,3,192,27,32",5525));//R8R4_526
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,30,16,1,1,216,29,32",5526));//R8R4_527
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,26,8,1,1,168,25,32",5527));//R8R4_528
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,11,15,480,31,2",5528));//R8R4_305
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,11,15,384,31,8",5529));//R8R4_306
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,4,5,7,432,31,8",5530));//R8R4_307
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,7,7,416,31,8",5531));//R8R4_532
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,3,7,312,31,8",5532));//R8R4_533
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,3,3,312,31,8",5533));//R8R4_534
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,260,27,2",5534));//R8R4_535
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,18,3,3,260,31,2",5535));//R8R4_536
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,28,12,3,3,208,27,8",5536));//R8R4_537
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,30,24,1,1,234,29,8",5537));//R8R4_538
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,1,26,12,1,1,182,25,8",5538));//R8R4_539
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,15,15,512,31,32",5539));//R8R4_356
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,1,7,15,384,31,32",5540));//R8R4_357
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,7,384,31,32",5541));//R8R4_358
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,3,3,432,31,32",5542));//R8R4_543
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,1,3,324,31,32",5543));//R8R4_544
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,16,1,1,324,31,32",5544));//R8R4_545
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,24,1,1,270,29,8",5545));//R8R4_546
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,24,1,1,270,31,8",5546));//R8R4_547
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,30,16,1,1,216,29,32",5547));//R8R4_548
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,31,32,0,0,243,30,32",5548));//R8R4_549
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,29,16,0,0,189,28,32",5549));//R8R4_550
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,7,320,31,8",5550));//R8R4_359
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,3,7,7,320,31,8",5551));//R8R4_360
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,7,7,256,31,32",5552));//R8R4_361
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,3,336,31,32",5553));//R8R4_554
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,1,3,252,31,32",5554));//R8R4_555
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,8,1,1,252,31,32",5555));//R8R4_556
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,1,1,210,25,8",5556));//R8R4_557
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,12,1,1,210,31,8",5557));//R8R4_558
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,26,8,1,1,168,25,32",5558));//R8R4_559
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,29,16,0,0,189,28,32",5559));//R8R4_560
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,23,8,0,0,147,22,32",5560));//R8R4_561
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,3,3,288,31,32",5561));//R8R4_362
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,3,224,31,32",5562));//R8R4_363
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,2,5,7,336,31,8",5563));//R8R4_308
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,3,3,240,31,32",5564));//R8R4_565
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,2,1,3,180,31,32",5565));//R8R4_566
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,0,32,4,1,1,180,31,32",5566));//R8R4_567
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,1,1,150,17,8",5567));//R8R4_568
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,0,0,32,6,1,1,150,31,8",5568));//R8R4_569
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,18,4,1,1,120,17,32",5569));//R8R4_570
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,25,8,0,0,135,24,32",5570));//R8R4_571
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,11,4,0,0,105,10,32",5571));//R8R4_572
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,16,2,3,7,112,15,32,[1,4,7,4,8,8],[[2,2,912]]",5000));//R2_2/([0,0,0,0,0,1])
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,24,4,3,3,144,23,32,[1,6,9,4,12,0],[[2,2,880]]",5001));//R2_2/([0,0,0,0,1,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,17,4,0,0,93,16,32,[1,3,2,2,12,12],[[2,2,931]]",5002));//R2_2/([0,1,1,1,1,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,20,4,3,3,120,19,32,[1,6,11,2,6,6],[[2,2,904]]",5003));//R2_2/([1,1,1,1,1,1])	
	m_RingInvariant.insert(make_pair("[1,31,0,0,0,0],2,1,1,1,2,0,0,63,0,32,[1,1,0,0,0,30],[[2,2,961]]",5004));//R2_2/([1,0,1,1,1,1])
	//R32	
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,3],[4,5,4],[4,6,2],[4,9,5],[8,35,2],[8,38,2],[8,46,6],[16,315,2],[16,348,2]],[[2,1,3],[2,2,3],[4,4,3],[4,5,10],[4,6,8],[4,9,20],[4,10,3],[8,26,12],[8,35,26],[8,38,17],[8,45,9],[8,46,51],[8,48,18],[16,315,53],[16,348,53],[16,377,36],[16,382,36],[16,384,27]]",5503));//R2_2/([0,0,1,1,1,1])=R32_5503
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,1],[2,2,3],[4,5,2],[4,6,1],[4,9,2],[4,11,2],[8,38,2],[8,42,2],[8,46,4],[8,51,2],[16,302,2],[16,325,4]],[[2,1,1],[2,2,3],[4,5,5],[4,6,4],[4,9,8],[4,10,3],[4,11,5],[8,38,17],[8,42,17],[8,46,34],[8,48,9],[8,51,17],[16,302,53],[16,325,70],[16,382,36]]",5505));//R2_2/([0,0,0,1,1,1])=R32_5505	
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
	char sz[1024]={0};
	sprintf(sz,"%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%s",strN0.c_str(),n0,bA,bO,n1,n2,n4,n5,n6,n7,n8,strS1.c_str(),strN2.c_str());	
	return sz;
}

int IdRing(IRing* r){	
   if(r->size()<=1)
	   return 1;
   string strRingInvariant=calcRingInvariant(r);
   RIDHelper idHelper;
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
   return vID[0];
}

//-----------------------------End of IdRing-------------------------------

#endif