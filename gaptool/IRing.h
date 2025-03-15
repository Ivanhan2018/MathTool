#ifndef IRING_H
#define IRING_H

#include<stdio.h>
#include<string.h>
#include<sstream>
#include<set>
#include<vector>
#include<map>
#include<tuple>
#include<iterator>
#include<algorithm>
#include<functional>
using namespace std;

//warning: ISO C++ forbids converting a string constant to 'char*' [-Wwrite-strings]
//#pragma GCC diagnostic ignored "-Wwrite-strings"
//或者g++ -Wno-write-strings -o IRing.exe IRing.cpp

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
	m_r=NULL;
	m_flag=0;
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
	if(N>0 && m_Set.size()>N)
		return false;	
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

bool IsEqual(const vector<int>& a,const vector<int>& b){
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

// 判断子环I是否是环r的左理想
int IsLeftIdeal(IRing* r,vector<int> &I){
 //进一步判断是否是左理想
 for(int i=0;i<r->size();i++){//任意纯量环元素c
  for(int j=0;j<I.size();j++){//任意向量模元素a
   int ca=r->mul(i,I[j]);
	vector<int>::iterator p=std::find(I.begin(),I.end(),ca);
	if(p==I.end()){
		return 2;// 是子环但不是左理想
	}   
  }
 }
 return 1;//是左理想
}

// 判断子环I是否是环r的右理想
int IsRightIdeal(IRing* r,vector<int> &I){
 //进一步判断是否是右理想
 for(int i=0;i<r->size();i++){//任意纯量环元素c
  for(int j=0;j<I.size();j++){//任意向量模元素a
   int ac=r->mul(I[j],i);  
	vector<int>::iterator p1=std::find(I.begin(),I.end(),ac);
	if(p1==I.end()){
		return 2;// 是子环但不是右理想
	}
  }
 }
 return 1;//是右理想
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

string V2S(vector<pair<int,int> >& v1){
	string str="[";
	for(int i=0;i<v1.size();i++){
		char sz[20]={0};
		sprintf(sz,"[%d,%d],",v1[i].first,v1[i].second);
		str+=sz;
	}
	if(str.size()>2){
		str=str.substr(0,str.size()-1);
	}
	str+="]";
	return str;
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
		char sz[20]={0};
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
		char sz[20]={0};
		sprintf(sz,"%d,",vCounts[vOrders[i]]);
		strN0+=sz;
	}
	if(strN0.size()>2){
		strN0=strN0.substr(0,strN0.size()-1);
	}
	strN0+="]";
	return strN0;
}

// 加法生成元
vector<pair<int,int> > calcAddGen(IRing* r){
    int n=r->size();
	vector<pair<int,vector<int> > > vS1;	
 	for(int i=0;i<n;i++){
		vector<int> S1=Order(r,i);
		std::sort(S1.begin(),S1.end());
		int ord=S1.size();
		vector<pair<int,vector<int> > >::const_iterator it=std::find_if(vS1.begin(),vS1.end(),[S1](const pair<int,vector<int> >& obj)->bool{return IsEqual(obj.second,S1);});
		if(it==vS1.end()){
			vS1.push_back(make_pair(i,S1));	
			//printf("i=%d,S1=%s\n",i,V2S(S1).c_str());
		}		
	}
	std::sort(vS1.begin(),vS1.end(),[](const pair<int,vector<int> >& a,const pair<int,vector<int> >& b)->bool{
		                return a.second.size()>b.second.size();
				});	
				
	int m=vS1.size();
	vector<pair<int,int> > ret;
	vector<int> vSet;	
	if(m<=0)
		return ret;
	ret.push_back(make_pair(vS1[0].first,vS1[0].second.size()));		
	vSet.insert(vSet.end(),vS1[0].second.begin(),vS1[0].second.end());
	int cnt=vSet.size();
	int cnt1=cnt;
	int N=vS1[0].second.size();
	if(cnt==n)
		return ret;	
	for(int k=1;k<m;k++)
	{		
		if(N*vS1[k].second.size()>n)
			continue;
		vector<int>::iterator p=std::find(vSet.begin(),vSet.end(),vS1[k].first);
		if(p==vSet.end()){
			ret.push_back(make_pair(vS1[k].first,vS1[k].second.size()));
			vSet.insert(vSet.end(),vS1[k].second.begin(),vS1[k].second.end());
			vSet.erase(unique(vSet.begin(), vSet.end()), vSet.end());			
		}else{	
			continue;
		}
		N*=vS1[k].second.size();		
		do{
			cnt=vSet.size();
			if(cnt==n)
				return ret;
			for(int i=0;i<cnt;i++)
			{
				for(int j=0;j<cnt;j++)
				{
					int IJ1=r->add(vSet[i],vSet[j]);
					vector<int>::iterator p=std::find(vSet.begin(),vSet.end(),IJ1);
					if(p==vSet.end()){
						vSet.push_back(IJ1);
					}
				}
			}
			cnt1=vSet.size();
		}while(cnt1>cnt);		   
		if(cnt==n||cnt1==n)
			return ret;
	}
	return ret;
}

// 加法群元的坐标表示
void calcCoordinate(IRing* r,const vector<pair<int,int> >& gen){
	int m=gen.size();
	if(m==1){
		for(int i=0;i<gen[0].second;i++){
			vector<int> Si=Order(r,gen[0].first);
			int ai=Si[i];
			printf("(%d)=%d\n",i,ai);		
		}
		return;		
	}	
	if(m==2){
		for(int i=0;i<gen[0].second;i++){
			vector<int> Si=Order(r,gen[0].first);
			int ai=Si[i];
			for(int j=0;j<gen[1].second;j++){
				vector<int> Sj=Order(r,gen[1].first);
				int aj=Sj[j];
				int aij=r->add(ai,aj);
				printf("(%d,%d)=%d\n",i,j,aij);	
			}	
		}
		return;		
	}
	if(m==3){
		for(int i=0;i<gen[0].second;i++){
			vector<int> Si=Order(r,gen[0].first);
			int ai=Si[i];
			for(int j=0;j<gen[1].second;j++){
				vector<int> Sj=Order(r,gen[1].first);
				int aj=Sj[j];
				int aij=r->add(ai,aj);
				for(int k=0;k<gen[2].second;k++){
					vector<int> Sk=Order(r,gen[2].first);
					int ak=Sk[k];
					int aijk=r->add(aij,ak);
					printf("(%d,%d,%d)=%d\n",i,j,k,aijk);	
				}
			}	
		}
		return;		
	}
	if(m==4){
		for(int i=0;i<gen[0].second;i++){
			vector<int> Si=Order(r,gen[0].first);
			int ai=Si[i];
			for(int j=0;j<gen[1].second;j++){
				vector<int> Sj=Order(r,gen[1].first);
				int aj=Sj[j];
				int aij=r->add(ai,aj);
				for(int k=0;k<gen[2].second;k++){
					vector<int> Sk=Order(r,gen[2].first);
					int ak=Sk[k];
					int aijk=r->add(aij,ak);
					for(int t=0;t<gen[3].second;t++){
						vector<int> St=Order(r,gen[3].first);
						int at=St[t];
						int aijkt=r->add(aijk,at);
						printf("(%d,%d,%d,%d)=%d\n",i,j,k,t,aijkt);	
					}	
				}
			}	
		}
		return;		
	}	
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
		vector<pair<int,int> >::const_iterator it=std::find_if(ret.begin(),ret.end(),[vi](const pair<int,int>& obj)->bool{if(vi==obj.first)return true;return false;});
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
		vector<tuple<int,int,int> >::const_iterator it=std::find_if(ret.begin(),ret.end(),[vi](const tuple<int,int,int>& obj)->bool{if(get<0>(obj)==vi.first && get<1>(obj)==vi.second)return true;return false;});
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

vector<tuple<int,int,int,int> > doN3Vec(vector<tuple<int,int,int> >& v){
	vector<tuple<int,int,int,int> > ret;
	int n=v.size();
	for(int i=0;i<n;i++){
		tuple<int,int,int> vi=v[i];
		vector<tuple<int,int,int,int> >::const_iterator it=std::find_if(ret.begin(),ret.end(),[vi](const tuple<int,int,int,int>& obj)->bool{if(get<0>(obj)==get<0>(vi) && get<1>(obj)==get<1>(vi) && get<2>(obj)==get<2>(vi))return true;return false;});
		if(it==ret.end()){
			ret.push_back(make_tuple(get<0>(vi),get<1>(vi),get<2>(vi),1));
		}else{
			int cnt=get<3>(*it);
			ret[it-ret.begin()]=make_tuple(get<0>(vi),get<1>(vi),get<2>(vi),cnt+1);
		}
	}
	return ret;
}

bool IsConjugacy(IRing* r,int a,int b){
    int N=r->size();
    for(int i=0;i<N;i++){
        int ai=r->add(a,i);
        int ib=r->add(i,b);
        if(ai==ib)
            return true;
    }
    return false;
}
string calcC2(IRing* r){
	int n=r->size();
	vector<pair<int,int> > v;
	for(int i=1;i<n;i++){
		int I1=0;
		int I2=0;		
		for(int j=1;j<n;j++){
			if(r->mul(i,j)!=0){
				bool bC=IsConjugacy(r,i,j);	
				if(bC)
					I1++;
				else
					I2++;				
			}
		}
		v.push_back(make_pair(I1,I2));
	}
	std::sort(v.begin(),v.end());
	vector<tuple<int,int,int> > v1=doN2Vec(v);
	string str="[";
	for(int i=0;i<v1.size();i++)
	{
		char sz[20]={0};
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

string calcN3(IRing* r){
	int IsNil(IRing* r,int i);	
	int n=r->size();
	vector<pair<int,int> > v;
	for(int i=1;i<n;i++){
		vector<int> S1=Order(r,i);			
		int o=S1.size();
		int ni=IsNil(r,i);					
		v.push_back(make_pair(o,ni));
	}
	vector<tuple<int,int,int> > v1=doN2Vec(v);
	std::sort(v1.begin(),v1.end(),[](const tuple<int,int,int>& a,const tuple<int,int,int>& b)->bool{
					if(get<0>(a)!=get<0>(b))
						return get<0>(a)<get<0>(b);				
					return get<1>(a)<get<1>(b);	
				});	
	string str="[";
	for(int i=0;i<v1.size();i++)
	{
		char sz[20]={0};
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
		char sz[20]={0};
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

// 零因子阶的分布N1
string calcN1(IRing* r){
	int IsNil(IRing* r,int i);
	int n=r->size();
	vector<pair<int,int> > v;
	for(int i=1;i<n;i++){
		for(int j=1;j<n;j++){
			int ij=r->mul(i,j);
			if(ij==0){
				vector<int> S1=Order(r,i);			
				int o=S1.size();
				int ni=IsNil(r,i);
				v.push_back(make_pair(o,ni));
				break;
			}
		}
	}
	vector<tuple<int,int,int> > v1=doN2Vec(v);
	std::sort(v1.begin(),v1.end(),[](const tuple<int,int,int>& a,const tuple<int,int,int>& b)->bool{
					if(get<0>(a)!=get<0>(b))
						return get<0>(a)<get<0>(b);
					return get<1>(a)<get<1>(b);	
				});	
	string str="[";
	for(int i=0;i<v1.size();i++)
	{
		char sz[20]={0};
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
		char sz[20]={0};
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
		char sz[20]={0};
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
		char sz[20]={0};
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
		char sz[20]={0};
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

bool IsRingGen(IRing* r,vector<int>& v){
	int s=r->size();				
	Subring S1i0;
	bool b=S1i0.init(r,v,s);
	if(!b)
	   return false;
	if(S1i0.m_Set.size()!=s)
		return false;
	if(v.size()>=5){
		string str=V2S(v);	
		printf("RingGen:%s\n",str.c_str());
	}
	return true;
}

int Rank(IRing* r){
	int n=r->size();
	if(n<4)
		return 1;
	int s=n;
	std::vector<std::vector<int> > sets;
	std::vector<int> indexs;
	for (int i = 0; i < n; i++)
		indexs.push_back(i);
	
	for(int num=1;num<=n;num++){
		std::vector<int> elements;
		elements.resize(indexs.size());
		for (int i = 0; i < num; i++)
		{
			elements.at(i) = 1;
		}

		do
		{	
			std::vector<int> currentCombination;
			for (size_t i = 0; i < elements.size(); ++i)
			{
				if (elements[i])
				{
					currentCombination.push_back(indexs[i]);
				}
			}		
			bool bGen=IsRingGen(r,currentCombination);
			if(bGen)
				return num;		
		} while (prev_permutation(elements.begin(), elements.end()));
	}
	return 0;
}

int Rank4(IRing* r){
	int n=r->size();
	if(n<4)
		return 1;	
	for(int i=1;i<n;i++){
	   vector<int> vi;
	   vi.push_back(i);	   
	   Subring si(r,vi);
	   si.m_flag=0;
	   int ni=si.size();
	   if(ni==n){
			return 1;
	   }
   }
	for(int i=1;i<n-1;i++){
		for(int j=i+1;j<n;j++){		
		   vector<int> vi;
		   vi.push_back(i);	
		   vi.push_back(j);	   
		   Subring si(r,vi);
		   si.m_flag=0;
		   int ni=si.size();
		   if(ni==n){
				return 2;
		   }
	   }  
	}
	for(int i=1;i<n-2;i++){
		for(int j=i+1;j<n-1;j++){	
			for(int k=j+1;k<n;k++){		
			   vector<int> vi;
			   vi.push_back(i);	
			   vi.push_back(j);
			   vi.push_back(k);		   
			   Subring si(r,vi);
			   si.m_flag=0;
			   int ni=si.size();
			   if(ni==n){
					return 3;
			   }
		   } 
		}
	}
	return 4;//rank>=4  
}

int Rank5(IRing* r){
	int n=r->size();
	if(n<4)
		return 1;	
	for(int i=1;i<n;i++){
	   vector<int> vi;
	   vi.push_back(i);	   
	   Subring si(r,vi);
	   si.m_flag=0;
	   int ni=si.size();
	   if(ni==n){
			return 1;
	   }
   }
	for(int i=1;i<n-1;i++){
		for(int j=i+1;j<n;j++){		
		   vector<int> vi;
		   vi.push_back(i);	
		   vi.push_back(j);	   
		   Subring si(r,vi);
		   si.m_flag=0;
		   int ni=si.size();
		   if(ni==n){
				return 2;
		   }
	   }  
	}
	for(int i=1;i<n-2;i++){
		for(int j=i+1;j<n-1;j++){	
			for(int k=j+1;k<n;k++){		
			   vector<int> vi;
			   vi.push_back(i);	
			   vi.push_back(j);
			   vi.push_back(k);		   
			   Subring si(r,vi);
			   si.m_flag=0;
			   int ni=si.size();
			   if(ni==n){
					return 3;
			   }
		   } 
		}
	}
	for(int i=1;i<n-3;i++){
		for(int j=i+1;j<n-2;j++){	
			for(int k=j+1;k<n-1;k++){		
				for(int m=k+1;m<n;m++){	
				   vector<int> vi;
				   vi.push_back(i);	
				   vi.push_back(j);
				   vi.push_back(k);	
				   vi.push_back(m);  
				   Subring si(r,vi);
				   si.m_flag=0;
				   int ni=si.size();
				   if(ni==n){
						return 4;
				   }
				}
		   } 
		}
	}	
	return 5;//rank>=5,例如R32_5348=R16_301×R2_1、R64_1039=R16_301×R4_4、R64_1040=R16_301×R4_6  
}

int ZeroNum(IRing* r){
    int n=r->size();
	int iRet=0;
	for(int i=0;i<n;i++)for(int j=0;j<n;j++){if(r->mul(i,j)==0)iRet++;}
	//printf("零乘个数n6=%d,",iRet);
	return iRet;
}

// 非零乘加法阶的分布N5
string calcN5(IRing* r){
    int n=r->size();	
	vector<int> v;
	for(int i=1;i<n;i++){
		for(int j=1;j<n;j++){
			int ij=r->mul(i,j);
			if(ij!=0){
				vector<int> S1=Order(r,ij);			
				int o=S1.size();				
				v.push_back(o);
			}
		}
	}	
	std::sort(v.begin(),v.end());
	vector<pair<int,int> > v1=doN1Vec(v);
	string str="[";
	for(int i=0;i<v1.size();i++){
		char sz[20]={0};
		sprintf(sz,"[%d,%d],",v1[i].first,v1[i].second);
		str+=sz;
	}
	if(str.size()>2){
		str=str.substr(0,str.size()-1);
	}
	str+="]";
	return str;
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
		char sz[20]={0};
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

// 2次幂零元阶的分布
string calcN4(IRing* r){
	int n=r->size();
	vector<int> v;
	for(int i4=1;i4<n;i4++){
		if(r->mul(i4,i4)==0){
			vector<int> S1=Order(r,i4);			
			int o=S1.size();				
			v.push_back(o);			
		}
	}	
	std::sort(v.begin(),v.end());	
	vector<pair<int,int> > v1=doN1Vec(v);
	string str="[";
	for(int i=0;i<v1.size();i++){
		char sz[20]={0};
		sprintf(sz,"[%d,%d],",v1[i].first,v1[i].second);
		str+=sz;
	}
	if(str.size()>2){
		str=str.substr(0,str.size()-1);
	}
	str+="]";
	return str;
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

//>1表示幂等指数，0表示幂零环
int IsIdem(IRing* r,int i2){
    int n=r->size();	
	if(i2<0||i2>=n)
		return 0;
	int m=i2;
	for(int i=2;i<n+2;i++){
		int mi=r->mul(m,i2);
		if(mi==i2)
			return i;
		if(mi==0)
			return 0;
		m=mi;
	}
	return 1;
}

// 幂等指数分布N7
string calcN7(IRing* r){
    int n=r->size();
	vector<int> v;
	for(int i2=1;i2<n;i2++){
		int I=IsIdem(r,i2);
		if(I>=2)		
			v.push_back(I);
	}
	std::sort(v.begin(),v.end());
	vector<pair<int,int> > v1=doN1Vec(v);
	string str="[";
	for(int i=0;i<v1.size();i++){
		char sz[20]={0};
		sprintf(sz,"[%d,%d],",v1[i].first,v1[i].second);
		str+=sz;
	}
	if(str.size()>2){
		str=str.substr(0,str.size()-1);
	}
	str+="]";
	return str;	
}

vector<int> IdealProduct(IRing* r,const vector<int>& A,const vector<int>& B){
    int nA=A.size();	
    int nB=B.size();
    set<int> S;	
	for (int i=0; i<nA; i++){
		int I=0;
		for (int j=0; j<nB; j++){
			int ij=r->mul(A[i],B[j]);
			I=r->add(I,ij);
			S.insert(ij);
			S.insert(I);
		}
		S.insert(I);
	}
	vector<int> V;
	V.assign(S.begin(),S.end());	
	int cnt=0,cnt1=0;
	do{
		cnt=V.size();
		if(cnt==1)
			break;
		for(int i=0;i<cnt;i++){
			for(int j=0;j<cnt;j++){
				int IJ=r->add(V[i],V[j]);
				vector<int>::iterator p=std::find(V.begin(),V.end(),IJ);
				if(p==V.end()){
					V.push_back(IJ);
				}
			}
		}
		cnt1=V.size();
	}while(cnt1!=cnt);
	if(V.size()>1){
		std::sort(V.begin(),V.end());
	}
	return V;
}

//>0表示幂零环的幂零指数，0表示非幂零环
int IsNilpotent(IRing* r){
	int n=r->size();
	vector<int> S0;
	for(int i=0;i<n;i++){
		S0.push_back(i);
	}
	vector<int> S=S0;	
	int cnt=S.size();	
	if(cnt==1)
		return 1;
	int iret=1;	
	bool bE=false;
	do{
		vector<int> S1=IdealProduct(r,S,S0);
		iret++;		
		bE=IsEqual(S1,S);
		S=S1;
		if(S.size()==1)
			return iret; 	
	}while(!bE);
	//string str=V2S(S);
	//printf("iret=%d,cnt=%d,str=%s\n",iret,S.size(),str.c_str());
    return 0;
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

bool IsOneL(IRing* r,int i3){
    int n=r->size();
	if(i3<0||i3>=n)
		return false;
	for (int i=0; i<n; i++){
		if(r->mul(i3,i)!=i)
			return false;
	}
	return true;
}

bool IsOneR(IRing* r,int i3){
    int n=r->size();
	if(i3<0||i3>=n)
		return false;
	for (int i=0; i<n; i++){
		if(r->mul(i,i3)!=i)
			return false;
	}
	return true;
}

// 0-不是单位元 1-单位元 2-左单位元 3-右单位元
int IsOneEx(IRing* r,int i3){
	bool bL=IsOneL(r,i3);
	bool bR=IsOneR(r,i3);	
	if(bL && bR)
		return 1;
	if(bL)
		return 2;
	if(bR)
		return 3;		
	return 0;
}

/*
非交换环R4_7{0,a,b,a+b}，两个左单位元：a,a+b，没有右单位元。a^2=a,a·b=b,b·a=0,b^2=0
非交换环R4_8{0,a,b,a+b}，两个右单位元：a,a+b，没有左单位元。a^2=a,a·b=0,b·a=b,b^2=0
*/
// bLR：1-单位元个数bO、2-左单位元个数bL、3-右单位元个数bR
int OneExNum(IRing* r,int bLR){
    int n=r->size();
	int iRet=0;
	for(int i1=0;i1<n;i1++){if(IsOneEx(r,i1)==bLR)iRet++;}
	return iRet;
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

// 20220828:新增幺环不变量m0：R16Y=R16_52(m0=0),R16X=R16_105(m0=1),R27_22(m0=0),R27_32(m0=1)
int calcm0(IRing* r){
    int n=r->size();
	int e=One(r);
	if(e==-1)
		return -1;
	//int e2=r->add(e,e);
	int e2=0;
	int p=2;
	for(;p<=n;p++){
		if(n%p==0)
			break;
	}
	for(int i=0;i<p;i++){
		e2=r->add(e2,e);	
	}
 	for(int i=1;i<n;i++){
		int i2=r->mul(i,i);	
        if(i2!=e2){
			continue;
		}
		return 1;
	}
	return 0;
}

#ifndef QUOTIENTRING_H
#define QUOTIENTRING_H

// 商环R/I
struct quotientRing:public IRing
{
public:
	//  静态函数  
	static vector<vector<int> > quotientGN(const vector<vector<int> > &vvG,const vector<int> &vN);
	static vector<vector<int> > quotientTable(const vector<vector<int> > &vvG,const vector<vector<int> > &vvH,const vector<int> &vN);
public:
	// 实现抽象基类的方法
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// 构造函数
	quotientRing();
	quotientRing(IRing *r1,vector<int> &I);
	// 析构函数
	~quotientRing();
	// 成员函数 
	template<class T> vector<vector<int> > getTable(T *obp,int n,int(T::*p)(int,int));	
	// 成员变量  
	IRing *m_r1;
	vector<int> m_I;	
	vector<vector<int> > m_Add;
	vector<vector<int> > m_Mul;
	int m_flag;// quotientRing对象析构时是否释放m_r1指向的内存
};

quotientRing::quotientRing(){
	m_r1=NULL;
	m_flag=0;	
}

quotientRing::quotientRing(IRing *r1,vector<int> &I)
{
    m_r1=r1;
	m_I=I;
	vector<vector<int> > Add1=getTable(r1,r1->size(),&IRing::add);
	vector<vector<int> > H=quotientGN(Add1,I);
	m_Add=quotientTable(Add1,H,I);
	vector<vector<int> > Mul1=getTable(r1,r1->size(),&IRing::mul);
	m_Mul=quotientTable(Mul1,H,I);
}

quotientRing::~quotientRing(){
	if(m_flag==1 && m_r1!=NULL){
		delete m_r1;
		m_r1=NULL;
	}
}

template<class T>
vector<vector<int> > quotientRing::getTable(T *obp,int n,int(T::*p)(int,int))
{
	vector<vector<int> > vv(n,vector<int>(n));
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
		{
			vv[i][j]=(obp->*p)(i,j);	
		}
	return vv;
}

vector<vector<int> > quotientRing::quotientGN(const vector<vector<int> > &vvG,const vector<int> &vN)
{
	vector<vector<int> > ret;
	int G=vvG.size();
	int N=vN.size();
	for(int i=0;i<G;i++)
	{
		vector<int> I;
		for(int j=0;j<N;j++)
		{
			int ij=vvG[i][vN[j]];
			I.push_back(ij);
		}
		bool bNew=true;
		for(int k=0;k<ret.size();k++)
		{
			//判断I中的元素是否在ret中
			vector<int>::iterator p;
			p=std::find(ret[k].begin(),ret[k].end(),I[0]);
			if(p!=ret[k].end())
			{
				bNew=false;
				break;
			}
		}
		if(bNew)
		{
			ret.push_back(I);
		}
	}
	return ret;
}

vector<vector<int> > quotientRing::quotientTable(const vector<vector<int> > &vvG,const vector<vector<int> > &vvH,const vector<int> &vN)
{
	int G=vvG.size();
	int H=vvH.size();
	vector<vector<int> > ret(H);
	for(int i=0;i<H;i++)
	{
		vector<int> I(H);
		for(int j=0;j<H;j++)
		{
			int ij=vvG[vvH[i][0]][vvH[j][0]];
			int IJ=-1;
			for(int k=0;k<H;k++)
			{
				vector<int>::const_iterator p;
				p=std::find(vvH[k].begin(),vvH[k].end(),ij);
				if(p!=vvH[k].end())
				{
					IJ=k;
					break;
				}
			}
			I[j]=IJ;
		}
		ret[i]=I;
	}
	return ret;
}

void quotientRing::printTable()
{
	printRing(this);
}

int quotientRing::add(int a,int b)
{
	return m_Add[a][b];
}

int quotientRing::mul(int a,int b)
{
	return m_Mul[a][b];	
}

int quotientRing::size()
{
	return m_Add.size();
}

#endif

//b8N8N9
string calcb8N8N9(IRing* r){
	int IdRing(IRing* r);				
	vector<int> ZA=Center(r);
	int b8=IsIdeal(r,ZA);
	char szN8[1024]={0};			
	Subring N8(r,ZA); 	
	if(b8==1){
		quotientRing rmodN8(r,ZA);
		if(N8.size()==1||N8.size()==r->size())				
			sprintf(szN8,"[%d,%d]",N8.size(),rmodN8.size());
		else
			sprintf(szN8,"[R%d_%d,R%d_%d]",N8.size(),IdRing(&N8),rmodN8.size(),IdRing(&rmodN8));
	}else{
		if(N8.size()==1||N8.size()==r->size())	
			sprintf(szN8,"[%d]",N8.size());	
		else
			sprintf(szN8,"[R%d_%d]",N8.size(),IdRing(&N8));	
	}			
	vector<int> vN9=calN9(r);   		
	Subring N9(r,vN9); 	
	quotientRing rmodN9(r,vN9); 
	char szN9[1024]={0};
	if(N9.size()==1||N9.size()==r->size())				
		sprintf(szN9,"[%d,%d]",N9.size(),r->size()/N9.size());
	else
		sprintf(szN9,"[R%d_%d,R%d_%d]",N9.size(),IdRing(&N9),rmodN9.size(),IdRing(&rmodN9));
    char sz[2048]={0};	
	sprintf(sz,"%d,%s,%s",b8,szN8,szN9);
	return sz;
}
			
string calcQ1(IRing* r){
	int IdRing(IRing* r);
	int n=r->size();
	int p=2;
	for(;p<=n;p++){
		if(n%p==0)
			break;
	}
	if(p==n){
		return "[]";
	}
	vector<int> v1; 
	for(int i=1;i<n;i++){
	   vector<int> v;
	   v.push_back(i);
	   Subring S1i0;
	   bool b=S1i0.init(r,v,p);
	   if(!b)
		   continue;
		if(S1i0.m_Set.size()!=p)
			continue;
		vector<int> v0=v;
		v=S1i0.m_Set;
		int iret1=IsIdeal(r,v); 
		if(iret1!=1)
			continue;
		quotientRing S1i(r,v);
		bool b1=IsRing(&S1i);
		if(!b1){
			continue;
		}			
		int ID=IdRing(&S1i);
		auto it=std::find(v1.begin(),v1.end(),ID);
		if(it==v1.end()){
			v1.push_back(ID);
		}
	}
	std::sort(v1.begin(),v1.end());	
	int cnt=v1.size();  
	string str="[";
	for(int i=0;i<cnt;i++){
	   str+=itos(v1[i]);
	   if(i<cnt-1)
		   str+=",";   
	}	   
	str+="]";
	return str;
}

string calcQ2(IRing* r){
	int IdRing(IRing* r);
	int n=r->size();
	int p=2;
	for(;p<=n;p++){
		if(n%p==0)
			break;
	}
	if(p==n||p*p==n){
		return "[]";
	}
	vector<int> v1; 
	for(int i=0;i<n;i++){
		for(int j=i+1;j<n;j++){
			vector<int> v;
			v.push_back(i);		
			v.push_back(j);
			Subring S1i0;
			bool b=S1i0.init(r,v,p*p);
			if(!b)
			   continue;
			if(S1i0.m_Set.size()!=p*p)
				continue;
			vector<int> v0=v;
			v=S1i0.m_Set;
			int iret1=IsIdeal(r,v); 
			if(iret1!=1)
				continue;
			quotientRing S1i(r,v);
			bool b1=IsRing(&S1i);
			if(!b1){
				continue;
			}			
			int ID=IdRing(&S1i);
			auto it=std::find(v1.begin(),v1.end(),ID);
			if(it==v1.end()){
				v1.push_back(ID);
			}
		}
	}
	std::sort(v1.begin(),v1.end());	
	int cnt=v1.size();  
	string str="[";
	for(int i=0;i<cnt;i++){
	   str+=itos(v1[i]);
	   if(i<cnt-1)
		   str+=",";   
	}	   
	str+="]";
	return str;
}

string calcH2(IRing* r){
	int IdRing(IRing* r);
	int n=r->size();
	int p=2;
	for(;p<=n;p++){
		if(n%p==0)
			break;
	}
	if(p==n||p*p==n){
		return "[]";
	}
	vector<pair<int,int> > v1;
	for(int i=0;i<n;i++){
		for(int j=i+1;j<n;j++){
			vector<int> v;
			v.push_back(i);		
			v.push_back(j);
			Subring S1i0;
			bool b=S1i0.init(r,v,p*p);
			if(!b)
			   continue;
			if(S1i0.m_Set.size()!=p*p)
				continue;
			vector<int> v0=v;
			v=S1i0.m_Set;
			int iret1=IsIdeal(r,v); 
			if(iret1==0)
				continue;
			if(iret1==2){
				int iret1L=IsLeftIdeal(r,v);
				int iret1R=IsRightIdeal(r,v);			
				if(iret1L==1 && iret1R==2)
					iret1=3;//是左理想
				else if(iret1L==2 && iret1R==1)
					iret1=4;//是右理想		
				else{
					iret1=2;//既不是左理想，也不是右理想	
				}				
			}
			int ID=IdRing(&S1i0);
			auto it=std::find(v1.begin(),v1.end(),make_pair(ID,iret1));
			if(it==v1.end()){
				v1.push_back(make_pair(ID,iret1));
			}
	}
	}
	std::sort(v1.begin(),v1.end());	 
	string str=V2S(v1);
	return str;
}

string calcH1(IRing* r){
	int IdRing(IRing* r);
	int n=r->size();
	int p=2;
	for(;p<=n;p++){
		if(n%p==0)
			break;
	}
	if(p==n){
		return "[]";
	}
	vector<pair<int,int> > v1; 	
	for(int i=0;i<n;i++){
		for(int j=i+1;j<n;j++){
			vector<int> v;
			v.push_back(i);		
			v.push_back(j);
			Subring S1i0;
			bool b=S1i0.init(r,v,n/p);
			if(!b)
			   continue;
			if(S1i0.m_Set.size()!=n/p)
				continue;
			vector<int> v0=v;
			v=S1i0.m_Set;
			int iret1=IsIdeal(r,v);
			if(iret1==0)
				continue;	
			if(iret1==2){
				int iret1L=IsLeftIdeal(r,v);
				int iret1R=IsRightIdeal(r,v);			
				if(iret1L==1 && iret1R==2)
					iret1=3;//是左理想
				else if(iret1L==2 && iret1R==1)
					iret1=4;//是右理想		
				else{
					iret1=2;//既不是左理想，也不是右理想	
				}				
			}
			int ID=IdRing(&S1i0);
			auto it=std::find(v1.begin(),v1.end(),make_pair(ID,iret1));
			if(it==v1.end()){
				v1.push_back(make_pair(ID,iret1));
			}
		}
	}
	std::sort(v1.begin(),v1.end());	 
	string str=V2S(v1);
	return str;
}

string calcS2(IRing* r){
	int IdRing(IRing* r);
	int n=r->size();	
	vector<vector<int> > vv;
	for(int i=0;i<n;i++){
		for(int j=i+1;j<n;j++){
			vector<int> v;
			v.push_back(i);		
			v.push_back(j);
			Subring S1i0;
			bool bn=S1i0.init(r,v,n/2+1);
			if(!bn)
				continue;
			if(S1i0.m_Set.size()==1||S1i0.m_Set.size()==r->size())
				continue;
			vector<int> v0=v;
			v=S1i0.m_Set;
			int iret1=IsIdeal(r,v);
			if(iret1!=1)
				continue;			
			std::sort(v.begin(),v.end());
			auto it=std::find_if(vv.begin(),vv.end(),[v](const vector<int>& obj)->bool{
						return IsEqual(obj,const_cast<vector<int>&>(v));
				}); 
			if(it==vv.end()){
				vv.push_back(v);
			}			
		}
	}
	std::sort(vv.begin(),vv.end(),[](const vector<int>& a,const vector<int>& b)->bool{
		                return a.size()>b.size();
				});
	vector<std::vector<int> > vv1;				
	int cnt=vv.size();				
	for(int i=0;i<cnt;i++){
		    vector<int> v=vv[i];
			auto it=std::find_if(vv1.begin(),vv1.end(),[v](const vector<int>& obj)->bool{
						vector<int> v1;
						set_intersection(obj.begin(),obj.end(),v.begin(),v.end(),back_inserter(v1)); 
						if(v1.size()==v.size() && v.size()<=obj.size())
							return true;
						return false;						
				}); 
			if(it==vv1.end()){
				vv1.push_back(v);
			}	
	}	
	vector<pair<int,int> > v;
	int cnt1=vv1.size();  
	for(int i=0;i<cnt1;i++){
		Subring S1i0(r,vv1[i]);
		int ni=S1i0.size();			
		int IDi=IdRing(&S1i0);
        v.push_back(make_pair(ni,IDi));		
	}	
	std::sort(v.begin(),v.end());
	vector<tuple<int,int,int> > v1=doN2Vec(v);
	string str="[";
	for(int i=0;i<v1.size();i++)
	{
		char sz[20]={0};
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

string RandSubRing(IRing* r){
	int IdRing(IRing* r);
	int n=r->size();
	int p=2;
	for(;p<=n;p++){
		if(n%p==0)
			break;
	}
	if(p==n){
		return "[]";
	}
	vector<int> v;
	int m=rand()%2+3;
    for(int i=0;i<m;i++){
		int a=rand()%n;
		auto it=std::find(v.begin(),v.end(),a);
		if(it==v.end()){
			v.push_back(a);
		}		
	}				
	Subring S1i0;
	bool b=S1i0.init(r,v,n/p);
	if(!b)
	   return "[]";
	if(S1i0.m_Set.size()!=n/p)
		return "[]";
	vector<int> v0=v;
	v=S1i0.m_Set;
	int iret1=IsIdeal(r,v);
	if(iret1==0)
		return "[]";		
	int ID=IdRing(&S1i0);
	int rk=Rank(&S1i0);
	string str="["+itos(ID)+","+itos(iret1)+","+itos(rk)+"]";
	return str;
}

string RandSubRing(IRing* r,int s,int m){
	int IdRing(IRing* r);
	int n=r->size();
	if(n%s!=0||s>=n||s<2){
		return "[]";
	}
	vector<int> v;
    for(int i=0;i<m;i++){
		int a=rand()%n;
		auto it=std::find(v.begin(),v.end(),a);
		if(it==v.end()){
			v.push_back(a);
		}		
	}				
	Subring S1i0;
	bool b=S1i0.init(r,v,s);
	if(!b)
	   return "[]";
	if(S1i0.m_Set.size()!=s)
		return "[]";
	vector<int> v0=v;
	v=S1i0.m_Set;
	int iret1=IsIdeal(r,v);
	if(iret1==0)
		return "[]";		
	int ID=IdRing(&S1i0);
	int rk=Rank(&S1i0);
	string str="["+itos(ID)+","+itos(iret1)+","+itos(rk)+"]";
	return str;
}

//-----------------------------Begin of IdRing---------------------------------
class RIDHelper
{
public:
	RIDHelper();
	~RIDHelper();
private:
	multimap<string,int> m_RingInvariant;//根据环的结构不变量N0n0bAbOn1n2n4n5n6n7n8S1N2N6N5返回ID编号列表	
	multimap<string,int> m_I1I2;//根据环的结构不变量I1I2返回ID编号列表		
	map<pair<int,int>,string> m_Str[6];//idx=0:秩、idx=1:RI2不变量、idx=2:RI3不变量、idx=3:bN不变量、idx=4:Q1不变量、idx=5:m0不变量	
public:	
	int LoadData(char * pszFilePath,int idx);		//“从文件中读取环结构不变量数据”
	int LoadStr(char * pszFilePath,int n,int idx);	
	vector<int> IDFromRingInvariant(string& RingInvariant);	
	vector<int> IDFromI1I2(string& I1I2);	
	vector<string> I1I2FromID(int ID);			
	vector<int> IDFromN0n0bAbO(string& N0n0bAbO);
	string StrFromID(int n,int ID,int idx);	
};

int RIDHelper::LoadData(char * pszFilePath,int idx){
	FILE * fp =fopen(pszFilePath, "r");
	if( fp == NULL )
		return 1;//1打开文件失败

	char sz[2048] = {0};
	pair<string,int> item;
	int n = 0;
	n = fscanf(fp, "%s", sz);
	multimap<string,int> *p=(idx==0?(&m_RingInvariant):(&m_I1I2));
	while( n > 0 && !feof(fp) )
	{
		n = fscanf(fp, "%d,%s\n", &item.second, sz);
		if( n > 0 )
		{
			item.first = sz;
			(*p).insert(item);
		}
	}
	fclose(fp);
	return 0;//0成功
}

int RIDHelper::LoadStr(char * pszFilePath,int n,int idx){
	FILE * fp =fopen(pszFilePath, "r");
	if( fp == NULL )
		return 1;//1打开文件失败

	char sz[2048] = {0};//RI2R81.csv中的不变量数据超出400字节了
	pair<pair<int,int>,string> item;
	int n0 = 0;
	n0 = fscanf(fp, "%s", sz);
	map<pair<int,int>,string> *p=&m_Str[idx];
	while( n0 > 0 && !feof(fp) )
	{
        int id=0;
		n0 = fscanf(fp, "%d,%s\n",&id,sz);
		if( n0 > 0 )
		{
			item.first = make_pair(n,id);
			item.second=sz;			
			(*p).insert(item);
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
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,0,0,12,3,5,5,90,11,3,[1,3,2,0,6,0],[[2,2,6],[2,6,12],[6,2,12],[6,6,24]],[[3,6],[12,6]]",10));
	m_RingInvariant.insert(make_pair("[1,3,2,0,6,0],6,0,0,12,3,5,5,90,11,3,[1,3,2,0,6,0],[[2,2,6],[2,6,12],[6,2,12],[6,6,24]],[[6,9],[12,3]]",11));
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
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,0,0,18,4,5,5,132,17,2,[1,1,8,8,0,0],[[3,3,48],[3,6,48],[6,3,48],[6,6,48]],[[2,12],[18,6]]",7));
	m_RingInvariant.insert(make_pair("[1,1,8,8,0,0],6,0,0,18,4,5,5,132,17,2,[1,1,8,8,0,0],[[3,3,48],[3,6,48],[6,3,48],[6,6,48]],[[6,16],[18,2]]",8));	
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
	
	int cnt0=m_RingInvariant.size();
	int iret=LoadData("RingInvariant.csv",0);	
	int cnt1=m_RingInvariant.size();			
	iret=LoadStr("Q1R16.csv",16,4);	
	iret=LoadStr("Q1R32.csv",32,4);	
	iret=LoadStr("Q1R27.csv",27,4);	
	iret=LoadStr("Q1R81.csv",81,4);	
	iret=LoadStr("Q1R125.csv",125,4);		
	iret=LoadStr("Q1R243.csv",243,4);	
	int q1cnt=m_Str[4].size();
    //printf("q1cnt=%d\n",q1cnt);
	iret=LoadStr("m0R16.csv",16,5);	
	iret=LoadStr("m0R27.csv",27,5);
	iret=LoadStr("m0R81.csv",81,5);
	iret=LoadStr("m0R125.csv",125,5);	
 	int m0cnt=m_Str[5].size();
 	iret=LoadStr("RI2R16.csv",16,1);	
	iret=LoadStr("RI2R27.csv",27,1);	
	iret=LoadStr("RI2R32.csv",32,1);
	iret=LoadStr("RI2R81.csv",81,1);	
	int ri2cnt=m_Str[1].size();
    //printf("ri2cnt=%d\n",ri2cnt);	
 	iret=LoadStr("RI3R16.csv",16,2);	
	iret=LoadStr("RI3R27.csv",27,2);	
	iret=LoadStr("RI3R32.csv",32,2);
	iret=LoadStr("RI3R81.csv",81,2);
	int ri3cnt=m_Str[2].size();	
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

string RIDHelper::StrFromID(int n,int ID,int idx){
	std::map<pair<int,int>,string>::iterator it;
	it=m_Str[idx].find(make_pair(n,ID));
	if(it!=m_Str[idx].end()){
		return it->second;
	}
	return "";
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
	string strN5=calcN5(r);	
    // N0n0bAbOn1n2n4n5n6n7n8S1N2N6N5
	char sz[2048]={0};
	sprintf(sz,"%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%s,%s,%s",strN0.c_str(),n0,bA,bO,n1,n2,n4,n5,n6,n7,n8,strS1.c_str(),strN2.c_str(),strN6.c_str(),strN5.c_str());	
	//int len=strlen(sz);//Rpp(12,3):1576>1024
	if(bO && (r->size()==32||r->size()==243)){
		printf("%d阶幺环，RI=%s\n",r->size(),sz);
	}	
	return sz;
}

string calcRingInvariant2(IRing* r){
	int bN=IsNilpotent(r);	
	int bL=OneExNum(r,2);
	int bR=OneExNum(r,3);
	string strC2=calcC2(r);
	string strN3=calcN3(r);
	string strN4=calcN4(r);
	string strN7=calcN7(r);	
	string strN1=calcN1(r);	
    // bNbLbRC2N3N4N7N1
	char sz[2048]={0};
	sprintf(sz,"%d,%d,%d,%s,%s,%s,%s,%s",bN,bL,bR,strC2.c_str(),strN3.c_str(),strN4.c_str(),strN7.c_str(),strN1.c_str());	
	return sz;
}

string calcRingInvariant3(IRing* r){
	string strb8N8N9=calcb8N8N9(r);	
	string strQ2=calcQ2(r);
    // b8N8N9Q2
	char sz[2048]={0};
	sprintf(sz,"%s,%s",strb8N8N9.c_str(),strQ2.c_str());	
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
		string Q1=calcQ1(r);
		vector<int> vID02;
		for(int k=0;k<vID.size();k++){
			string Q10=idHelper.StrFromID(r->size(),vID[k],4);
			if(Q10==Q1){		
				vID02.push_back(vID[k]);
			}
		}			
        if(vID02.size()>1){		
			int im0=calcm0(r);
			string m0=itos(im0);
			vector<int> vID03;
			for(int k=0;k<vID02.size();k++){
				string m00=idHelper.StrFromID(r->size(),vID02[k],5);
				if(m00==m0){		
					vID03.push_back(vID[k]);
				}
			}
			if(vID03.size()==1){	
				printf("R%d_%d,Q1=%s,m0=%d\n",r->size(),vID03[0],Q1.c_str(),im0);	
				return vID03[0];
			}
			//R81_281和R81_516的中心不一样
			string sRI3=calcRingInvariant3(r);
			vector<int> vID04;
			for(int k=0;k<vID02.size();k++){
				string sRI30=idHelper.StrFromID(r->size(),vID02[k],2);	
				if(sRI3==sRI30){		
					vID04.push_back(vID02[k]);
				}
			}		
			if(vID04.size()==1){
				printf("R%d_%d,Q1=%s,m0=%d,RI3=%s\n",r->size(),vID04[0],Q1.c_str(),im0,sRI3.c_str());				
				return vID04[0];	
			}	
			printf("R%d_0(%d),Q1=%s,m0=%d,RI3=%s,RI=%s\n",r->size(),vID02[0],Q1.c_str(),im0,sRI3.c_str(),strRingInvariant.c_str());	
			return 0;//ID不确定，还需要新的环不变量确定编号
/* 			string strI1I2=calcI1(r)+","+calcI2(r);
			vector<int> vID2=idHelper.IDFromI1I2(strI1I2);	
			vector<int> vID023;
			// 调用set_intersection之前要加上sort，否则计算的来的交集有可能不准。例如R32_2022和R32_2018
			std::sort(vID02.begin(),vID02.end());
			std::sort(vID2.begin(),vID2.end());
			set_intersection(vID02.begin(),vID02.end(),vID2.begin(),vID2.end(),back_inserter(vID023)); 
			if(vID023.size()!=1){
				#if 1
				 printf("[");
				 for(int i=0;i<vID.size();i++){
					 printf("%d,",vID[i]);
				 }
				 printf("]\n");
				#endif	
			}			
            if(vID023.size()>1){
				return 0;//ID不确定，还需要新的环不变量确定编号
			}
			else if(vID023.size()<=0){
			   printf("出错了，环不变量数据I1I2遗漏或有误！I1I2=%s\n",strI1I2.c_str());
			   return 0;
			}
			return vID023[0]; */		
	   }
	   else if(vID02.size()<=0){
			#if 1
			 printf("[");
			 for(int i=0;i<vID.size();i++){
				 printf("%d,",vID[i]);
			 }
			 printf("]\n");
			#endif			   
		    printf("出错了，%d阶环不变量数据Q1有误！Q1=%s\n",r->size(),Q1.c_str());
			return 0;
	   }
	   return vID02[0];
   }  
   if(r->size()==16||r->size()==27||r->size()==81||r->size()==32||r->size()==243){
		string Q1=calcQ1(r);
		string Q10=idHelper.StrFromID(r->size(),vID[0],4);
		if(Q10!="" && Q10!=Q1){			
			printf("出错了，%d阶环的Q1=%s与ID=%d,Q1=%s不匹配！\n",r->size(),Q1.c_str(),vID[0],Q10.c_str());
		}
   }   
   // if(/* r->size()==16||r->size()==27|| */r->size()==81||r->size()==125){
		// int im0=calcm0(r);
		// string m0=itos(im0);
		// string m00=idHelper.StrFromID(r->size(),vID[0],5);
		// if(m00!="" && m00!=m0){			
			// printf("出错了，%d阶环的m0=%s与ID=%d,m0=%s不匹配！\n",r->size(),m0.c_str(),vID[0],m00.c_str());
		// }
   // }      
    /*if(r->size()==16||r->size()==27||r->size()==81){
		string sRI2=calcRingInvariant2(r);
		string sRI20=idHelper.StrFromID(r->size(),vID[0],1);		
		if(sRI20!="" && sRI2.find(sRI20)==string::npos){			
			printf("出错了，%d阶环的RI2=%s与ID=%d,RI2=%s不匹配！\n",r->size(),sRI2.c_str(),vID[0],sRI20.c_str());
		}
   }*/
   if(r->size()==16||r->size()==27||r->size()==81){
		string sRI3=calcRingInvariant3(r);
		string sRI30=idHelper.StrFromID(r->size(),vID[0],2);		
		if(sRI30!="" && sRI3.find(sRI30)==string::npos){			
			printf("出错了，%d阶环的RI3=%s与ID=%d,RI3=%s不匹配！\n",r->size(),sRI3.c_str(),vID[0],sRI30.c_str());
		}	
        else{
			if(r->size()==81 && (vID[0]==507||vID[0]==366||vID[0]==347))printf("R%d_%d,RI3=%s\n",r->size(),vID[0],sRI3.c_str());
		}		
   }     
   return vID[0];
}

//R16
static int IDs[]={-1,35,36,37,51};

bool isR16ID(int i){
	if(i==6||i==13||i==14||i==15||i==28||i==29||i==30||i==31||i==32||i==33||i==34||i==118||i==119||i==120||i==121||i==122||i==123||i==124||i==125||i==126||i==127||i==128||i==130||i==131||i==134||i==135||i==136||i==137||i==138||i==139||i==140||i==141||i==142||i==143||i==144||i==145||i==146||i==156||i==171||i==172||i==173||i==175||i==176||i==178||i==179||i==181||i==182||i==228||i==239||i==241||i==244||i==246||i==248||i==275||i==276||i==282||i==286)
		return true;
	return false;
}

//-----------------------------End of IdRing-------------------------------

#endif