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
	m_RingInvariant.insert(make_pair("4,1,0,8,1,3,7,40,7",12));	
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
	m_RingInvariant.insert(make_pair("1,0,12,1,11,11,144,11,12",1));
	m_RingInvariant.insert(make_pair("1,0,12,2,1,3,60,11,12",2));
	m_RingInvariant.insert(make_pair("1,0,12,2,5,5,72,11,12",3));
	m_RingInvariant.insert(make_pair("1,0,12,2,3,3,80,11,12",4));	
	m_RingInvariant.insert(make_pair("1,1,8,4,1,1,40,7,12",5));
	m_RingInvariant.insert(make_pair("1,0,12,1,5,11,108,11,12",6));
	m_RingInvariant.insert(make_pair("1,0,12,1,11,11,144,11,12",7));
	m_RingInvariant.insert(make_pair("1,0,12,1,5,11,108,11,12",8));	
	m_RingInvariant.insert(make_pair("1,0,12,2,5,5,108,11,12",9));
	m_RingInvariant.insert(make_pair("0,0,12,3,5,5,90,11,3",10));
	m_RingInvariant.insert(make_pair("0,0,12,3,5,5,90,11,3",11));
	m_RingInvariant.insert(make_pair("1,0,12,2,5,5,72,11,12",12));
	m_RingInvariant.insert(make_pair("1,0,12,4,1,1,60,11,12",13));
	m_RingInvariant.insert(make_pair("1,1,10,8,0,0,45,9,12",14));	
	m_RingInvariant.insert(make_pair("0,0,12,6,1,1,50,11,3",15));
	m_RingInvariant.insert(make_pair("1,0,12,2,3,3,80,11,12",16));
	m_RingInvariant.insert(make_pair("1,0,12,2,1,3,60,11,12",17));
	m_RingInvariant.insert(make_pair("0,0,12,6,1,1,50,7,3",18));	
	m_RingInvariant.insert(make_pair("1,1,8,4,1,1,40,7,12",19));
	m_RingInvariant.insert(make_pair("1,0,12,4,2,2,81,11,12",20));
	m_RingInvariant.insert(make_pair("1,0,12,2,2,2,63,11,12",21));	
	m_RingInvariant.insert(make_pair("1,1,6,4,0,0,35,5,12",22));	
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
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,8],[4,6,4]],[[2,1,3],[2,2,8],[4,4,3],[4,6,16],[4,7,9],[4,8,9],[4,10,12],[8,29,9],[8,34,9],[8,36,6],[8,41,12],[8,47,12]]",312));
	m_I1I2.insert(make_pair("[[1,1,1],[2,1,3],[2,2,8],[4,6,4]],[[2,1,3],[2,2,8],[4,4,3],[4,6,16],[4,8,24],[4,10,12],[8,34,18],[8,47,24]]",313));	
	//R17
	m_RingInvariant.insert(make_pair("1,0,17,1,16,16,289,16,17",1));
	m_RingInvariant.insert(make_pair("1,1,1,2,0,0,33,0,17",2));	
	//R18
	m_RingInvariant.insert(make_pair("1,0,18,1,17,17,324,17,18",1));
	m_RingInvariant.insert(make_pair("1,0,18,2,5,5,84,17,18",2));
	m_RingInvariant.insert(make_pair("1,0,18,1,5,17,180,17,18",3));
	m_RingInvariant.insert(make_pair("1,0,18,1,17,17,324,17,18",4));	
	m_RingInvariant.insert(make_pair("1,0,18,1,5,17,180,17,18",5));
	m_RingInvariant.insert(make_pair("1,0,18,2,5,5,180,17,18",6));
	m_RingInvariant.insert(make_pair("0,0,18,4,5,5,132,17,2",7));
	m_RingInvariant.insert(make_pair("0,0,18,4,5,5,132,17,2",8));	
	m_RingInvariant.insert(make_pair("1,0,18,2,5,5,84,17,18",9));
	m_RingInvariant.insert(make_pair("1,0,18,4,1,1,100,17,18",10));
	m_RingInvariant.insert(make_pair("1,0,18,2,1,1,68,17,18",11));
	m_RingInvariant.insert(make_pair("1,0,18,2,8,8,243,17,18",12));
	m_RingInvariant.insert(make_pair("1,1,12,4,2,2,63,11,18",13));
	m_RingInvariant.insert(make_pair("1,0,18,2,2,8,135,17,18",14));	
	m_RingInvariant.insert(make_pair("1,0,18,2,8,8,243,17,18",15));
	m_RingInvariant.insert(make_pair("1,0,18,2,2,8,135,17,18",16));
	m_RingInvariant.insert(make_pair("1,0,18,4,2,2,135,17,18",17));
	m_RingInvariant.insert(make_pair("0,0,18,8,2,2,99,11,2",18));	
	m_RingInvariant.insert(make_pair("0,0,18,8,2,2,99,17,2",19));
	m_RingInvariant.insert(make_pair("1,1,12,4,2,2,63,11,18",20));
	m_RingInvariant.insert(make_pair("1,1,14,8,0,0,75,13,18",21));	
	m_RingInvariant.insert(make_pair("1,1,10,4,0,0,51,9,18",22));	
	//R19
	m_RingInvariant.insert(make_pair("1,0,19,1,18,18,361,18,19",1));
	m_RingInvariant.insert(make_pair("1,1,1,2,0,0,37,0,19",2));	
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