#ifndef IGROUP_H
#define IGROUP_H

#include<cstdio>
#include<cstring>
#include<string>
#include<vector>
#include<map>
#include<tuple>
#include<iterator>
#include<algorithm>
using namespace std;

// 位操作
enum IsGroupEnum
{
	IsGroup_None=0,//不是群
	IsGroup_bG=0x00000001,//是群
	IsGroup_bC=0x00000002,//循环群
	IsGroup_bA=0x00000004,//交换群
	IsGroup_bN=0x00000008,//幂零群
	IsGroup_bS=0x00000010,//可解群
};

struct IGroup
{
public:
   virtual ~IGroup(){};	
   virtual void printSet()=0; 
   virtual void printTable()=0;    
   virtual int mul(int a,int b)=0;
   virtual int size()=0;
   virtual int inv(int a)=0;	
};

void printGroup(IGroup* g){
   int n=g->size();  
   for(int i=0;i<n;i++){
	   for(int j=0;j<n;j++){
		  int ij=g->mul(i,j);
		  printf("%d ",ij+1); 
	   }
       printf("\n");
   }
}

// FG、printSubGroup只与IGroup有关，可以放到IGroup.h里面去
int getidx(vector<vector<unsigned char> > &Arr2,vector<unsigned char> &Arr1){
	int n=Arr2.size();
	int m=Arr1.size();	
	for(int i=0;i<n;i++){
		int ni=Arr2[i].size();
		if(ni!=m)
			return -1;
		if(memcmp(&Arr2[i][0],&Arr1[0],m*sizeof(unsigned char))==0){
			return i;	
		}
	}
	return -1;	
}

vector<int> FG(IGroup* g,const vector<int>& gen){
	vector<int> S;
	S.push_back(0);
	int R=gen.size();
	for(int i=0;i<R;i++){
		if(gen[i]==0)
			continue;
		S.push_back(gen[i]);
	}
	int cnt=R+1;
	int cnt1=R+1;
	do{
		cnt=S.size();
		for(int i=1;i<cnt;i++){
			for(int j=1;j<cnt;j++){
				int IJ=g->mul(S[i],S[j]);
				vector<int>::iterator p=std::find(S.begin(),S.end(),IJ);
				if(p==S.end()){
					S.push_back(IJ);
				}
			}
		}
		cnt1=S.size();
	}while(cnt1>cnt);	
   return S;
}

string itos(int i)
{
  char sz[40]={0};
  sprintf(sz,"%d",i);
  return sz;
}

string V2S(vector<int>& v){
   string str="";
   int n=v.size();
   for(int i=0;i<n;i++)
   {
	   str+=itos(v[i]);
	   if(i<n-1)
		   str+=",";   
   }	   
   str+="";
   return str;
}

// 一个群g的子群s
struct Subgroup:public IGroup
{
public:
	// 静态函数
public:
	// 实现抽象基类的方法
	virtual void printSet(); 
	virtual void printTable();    
	virtual int mul(int a,int b);
	virtual int size();
	virtual int inv(int a); 
	// 构造函数
	Subgroup();	
	Subgroup(IGroup* g,const vector<int>& gens,int flag=0);
	// 析构函数
	~Subgroup();		
	// 成员函数
    bool init(IGroup* g,const vector<int>& gens,int N=0,int flag=0);//N>0时表示子群阶要小于等于N，否则初始化失败
	// 成员变量
	vector<int> m_Set;
	IGroup* m_g;
    int m_flag;// Subgroup对象析构时是否释放m_g指向的内存 	
};

Subgroup::Subgroup(){
	m_g=NULL;
	m_flag=0;
}

Subgroup::~Subgroup(){
	if(m_flag==1 && m_g!=NULL){
		delete m_g;
		m_g=NULL;
	}
}

bool Subgroup::init(IGroup* g,const vector<int>& gens,int N,int flag){
	m_g=g;
	m_flag=flag;
	m_Set.push_back(0);
	int R=gens.size();
	for(int i=0;i<R;i++){
		if(gens[i]==0)
			continue;
		m_Set.push_back(gens[i]);
	}
	int cnt=R+1;
	int cnt1=R+1;
	do{
		cnt=m_Set.size();
		if(N>0 && cnt>N)
			return false;
		if(cnt==g->size())
			break;
		for(int i=1;i<cnt;i++){
			for(int j=1;j<cnt;j++){
				int IJ=g->mul(m_Set[i],m_Set[j]);
				vector<int>::iterator p=std::find(m_Set.begin(),m_Set.end(),IJ);
				if(p==m_Set.end()){
					m_Set.push_back(IJ);
				}
			}
		}
		cnt1=m_Set.size();
	}while(cnt1>cnt);	
   return true;
}


void Subgroup::printSet()
{
    string str=V2S(m_Set);
	printf("%s\n",str.c_str());
}

void Subgroup::printTable()
{
    int IdGroup(IGroup*);
	int ID=IdGroup(this);
	printf("GAP[%d,%d]:\n",size(),ID);
	//printGroup(this);
}

int Subgroup::inv(int a)
{
	int A1=m_g->inv(m_Set[a]);
	vector<int>::iterator p=std::find(m_Set.begin(),m_Set.end(),A1);
	int a1=-1;
	if(p!=m_Set.end()){
		a1=p-m_Set.begin();
	}
	return a1;
}

int Subgroup::mul(int a,int b)
{
	int ij=m_g->mul(m_Set[a],m_Set[b]);
	vector<int>::iterator p=std::find(m_Set.begin(),m_Set.end(),ij);
	int IJ=-1;
	if(p!=m_Set.end()){
		IJ=p-m_Set.begin();
	}
	return IJ;
}

int Subgroup::size()
{
	return m_Set.size();
}

Subgroup::Subgroup(IGroup* g,const vector<int>& gens,int flag)
{
	//m_g=g;
	//m_Set=FG(g,gens);
	init(g,gens,0,flag);
}

// 判断集合S是否是群g的正规子群
int IsNormalSubgroup(IGroup* g,vector<int> &S,bool bSet){	
	vector<int>* I;
	Subgroup s;
	//S不是g的生成元集
	if(!bSet){
		bool b=s.init(g,S,S.size());
		if(!b){
			return 0;// 子群也不是
		}
		I=&s.m_Set;
	}else{
		I=&S;
	}
	//进一步判断是否是正规子群
	for(int i=0;i<g->size();i++){
		for(int j=0;j<(*I).size();j++){
			int gh=g->mul(i,(*I)[j]);
			int g1=g->inv(i);
			int ghg1=g->mul(gh,g1);  
			vector<int>::iterator p=std::find((*I).begin(),(*I).end(),ghg1);
			if(p==(*I).end()){
				return 2;// 是子群但不是正规子群
			} 
		}
	}
	return 1;//是正规子群
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

string calcI1(IGroup* g){
	int IdGroup(IGroup* g);
	int n=g->size();
	vector<pair<int,int> > v;
   for(int i=0;i<n;i++){
	   vector<int> vi;
	   vi.push_back(i);
	   Subgroup si(g,vi);
	   int ni=si.size();
	   if(ni<n && ni>0){
		int ID=IdGroup(&si);
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

string calcI2(IGroup* g){
	int IdGroup(IGroup* g);
	int n=g->size();
	vector<pair<int,int> > v;
   for(int i=0;i<n;i++){
	   for(int j=i+1;j<n;j++){
		   vector<int> vi;
		   vi.push_back(i);
		   vi.push_back(j);		   
		   Subgroup si(g,vi);
		   int ni=si.size();
		   if(ni<n && ni>0){
			int ID=IdGroup(&si);
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

string calcC1(IGroup* g){
	int IdGroup(IGroup* g);
	bool IsConjugacy(IGroup* g,int a,int b);
	int n=g->size();
	vector<int> v;
   for(int i=0;i<n;i++){
	   int i2=g->mul(i,i);
	   int i3=g->mul(i2,i);
	   int I1=IsConjugacy(g,i,i3);
	   v.push_back(I1);
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

int calk(vector<vector<int> >& vv,int i){
    int k=vv.size();
 	for(int j=0;j<k;j++)
	{
		int kj=vv[j].size();
		vector<int>::iterator p=std::find(vv[j].begin(),vv[j].end(),i);
		if(p!=vv[j].end()){
			return kj;
		}
	}
	return 0;
}

string calcNk(IGroup* g){
	int IdGroup(IGroup* g);
	vector<vector<int> > ConjugacyClasses(IGroup* g);
	vector<int> Order(IGroup* g,int m);
	int n=g->size();
	vector<pair<int,int> > v;
	vector<vector<int> > vv=ConjugacyClasses(g);
	for(int i=0;i<n;i++){
		vector<int> S1=Order(g,i);
		int I1=S1.size();
		int I2=calk(vv,i);
		v.push_back(make_pair(I1,I2));
	}
	std::sort(v.begin(),v.end());
	vector<tuple<int,int,int> > v1=doN2Vec(v);
	string str="[";
	for(int i=0;i<v1.size();i++){
		char sz[200]={0};
		sprintf(sz,"[%d,%d,%d],",get<0>(v1[i]),get<1>(v1[i]),get<2>(v1[i]));
		str+=sz;
	}
	if(str.size()>2){
		str=str.substr(0,str.size()-1);
	}
	str+="]";
	return str;
}

// IdGroup只与IGroup有关，可以放到IGroup.h里面去
//-----------------------------Begin of IdGroup---------------------------------
class IDHelper
{
public:
	IDHelper();
	~IDHelper();
private:
	multimap<string,int> m_N0;
	multimap<string,int> m_S2;	
	multimap<string,int> m_kKEZDCANS;	
public:
	vector<int> IDFromN0C1Nk(string& N0,string& C1Nk);//根据N0C1Nk返回ID编号列表
	vector<int> IDFromS2(string& S2);//根据S2返回ID编号列表	
	vector<int> IDFromkKEZDCANS(string& kKEZDCANS);//根据kKEZDCANS返回ID编号列表		
};

IDHelper::IDHelper(){
	//G2
	m_N0.insert(make_pair("1,1",1));	
	//G3
	m_N0.insert(make_pair("1,2",1));	
	//G4
	m_N0.insert(make_pair("1,1,2",1));
	m_N0.insert(make_pair("1,3,0",2));
	//G5
	m_N0.insert(make_pair("1,4",1));	
	//G6
	m_N0.insert(make_pair("1,3,2,0",1));
	m_N0.insert(make_pair("1,1,2,2",2));
	//G7
	m_N0.insert(make_pair("1,6,[[0,6],[1,1]],[[1,1,1],[7,1,6]]",1));	
	//G8
	m_N0.insert(make_pair("1,1,2,4",1));
	m_N0.insert(make_pair("1,3,4,0",2));
	m_N0.insert(make_pair("1,5,2,0",3));
	m_N0.insert(make_pair("1,1,6,0",4));
	m_N0.insert(make_pair("1,7,0,0",5));
	//G9
	m_N0.insert(make_pair("1,2,6",1));
	m_N0.insert(make_pair("1,8,0",2));
	//G10
	m_N0.insert(make_pair("1,5,4,0",1));
	m_N0.insert(make_pair("1,1,4,4",2));	
	//G12
	m_N0.insert(make_pair("1,1,2,6,2,0",1));
	m_N0.insert(make_pair("1,1,2,2,2,4",2));
	m_N0.insert(make_pair("1,3,8,0,0,0",3));
	m_N0.insert(make_pair("1,7,2,0,2,0",4));
	m_N0.insert(make_pair("1,3,2,0,6,0",5));
	//G14
	m_N0.insert(make_pair("1,7,6,0,[[0,6],[1,8]],[[1,1,1],[2,7,7],[7,2,6]]",1));
	m_N0.insert(make_pair("1,1,6,6,[[0,12],[1,2]],[[1,1,1],[2,1,1],[7,1,6],[14,1,6]]",2));	
	//G15	
	m_N0.insert(make_pair("1,2,4,8",1));	
	//G16
	m_N0.insert(make_pair("1,1,2,4,8",1));
	m_N0.insert(make_pair("1,3,12,0,0",2));	
	m_N0.insert(make_pair("1,7,8,0,0",3));
	m_N0.insert(make_pair("1,3,12,0,0",4));
	m_N0.insert(make_pair("1,3,4,8,0",5));	
	m_N0.insert(make_pair("1,3,4,8,0",6));
	m_N0.insert(make_pair("1,9,2,4,0",7));	
	m_N0.insert(make_pair("1,5,6,4,0",8));
	m_N0.insert(make_pair("1,1,10,4,0",9));
	m_N0.insert(make_pair("1,7,8,0,0",10));
	m_N0.insert(make_pair("1,11,4,0,0",11));	
	m_N0.insert(make_pair("1,3,12,0,0",12));
	m_N0.insert(make_pair("1,7,8,0,0",13));
	m_N0.insert(make_pair("1,15,0,0,0",14));
	//G16	
	m_S2.insert(make_pair("0,1,5,22,92",1));
	m_S2.insert(make_pair("0,3,33,36,48",2));	
	m_S2.insert(make_pair("0,7,41,24,48",3));
	m_S2.insert(make_pair("0,3,33,36,48",4));
	m_S2.insert(make_pair("0,3,13,56,48",5));	
	m_S2.insert(make_pair("0,3,13,56,48",6));
	m_S2.insert(make_pair("0,9,17,46,48",7));	
	m_S2.insert(make_pair("0,5,21,46,48",8));
	m_S2.insert(make_pair("0,1,25,46,48",9));
	m_S2.insert(make_pair("0,7,41,72,0",10));
	m_S2.insert(make_pair("0,11,49,60,0",11));	
	m_S2.insert(make_pair("0,3,33,84,0",12));
	m_S2.insert(make_pair("0,7,29,84,0",13));
	m_S2.insert(make_pair("0,15,105,0,0",14));
	//G16	
	m_kKEZDCANS.insert(make_pair("16,[16,0,0,0,0],4,16,1,0,1,1,1",2));	
	m_kKEZDCANS.insert(make_pair("10,[4,6,0,0,0],4,4,2,0,0,1,1",4));
	m_kKEZDCANS.insert(make_pair("16,[16,0,0,0,0],8,16,1,0,1,1,1",5));
	m_kKEZDCANS.insert(make_pair("10,[4,6,0,0,0],8,4,2,0,0,1,1",6));
	//G18
	m_N0.insert(make_pair("1,9,2,0,6,0",1));
	m_N0.insert(make_pair("1,1,2,2,6,6",2));
	m_N0.insert(make_pair("1,3,8,6,0,0",3));
	m_N0.insert(make_pair("1,9,8,0,0,0",4));
	m_N0.insert(make_pair("1,1,8,8,0,0",5));
	//G20
	m_N0.insert(make_pair("1,1,10,4,4,0",1));
	m_N0.insert(make_pair("1,1,2,4,4,8",2));
	m_N0.insert(make_pair("1,5,10,4,0,0",3));
	m_N0.insert(make_pair("1,11,0,4,4,0",4));
	m_N0.insert(make_pair("1,3,0,4,12,0",5));	
	//G21
	m_N0.insert(make_pair("1,14,6,0,[[0,20],[1,1]],[[1,1,1],[3,7,14],[7,3,6]]",1));
	m_N0.insert(make_pair("1,2,6,12,[[0,20],[1,1]],[[1,1,1],[3,1,2],[7,1,6],[21,1,12]]",2));	
	//G24
	m_N0.insert(make_pair("1,1,2,2,2,12,4,0",1));
	m_N0.insert(make_pair("1,1,2,2,2,4,4,8",2));	
	m_N0.insert(make_pair("1,1,8,6,8,0,0,0",3));
	m_N0.insert(make_pair("1,1,2,14,2,0,4,0",4));
	m_N0.insert(make_pair("1,7,2,8,2,0,4,0",5));	
	m_N0.insert(make_pair("1,13,2,2,2,0,4,0",6));
	m_N0.insert(make_pair("1,3,2,12,6,0,0,0",7));	
	m_N0.insert(make_pair("1,9,2,6,6,0,0,0",8));
	m_N0.insert(make_pair("1,3,2,4,6,0,8,0",9));
	m_N0.insert(make_pair("1,5,2,2,10,0,4,0",10));
	m_N0.insert(make_pair("1,1,2,6,2,0,12,0",11));	
	m_N0.insert(make_pair("1,9,8,6,0,0,0,0",12));
	m_N0.insert(make_pair("1,7,8,0,8,0,0,0",13));
	m_N0.insert(make_pair("1,15,2,0,6,0,0,0",14));	
	m_N0.insert(make_pair("1,7,2,0,14,0,0,0",15));		
	//G24	
	m_S2.insert(make_pair("0,1,3,5,11,66,46,144",1));
	m_S2.insert(make_pair("0,1,3,5,11,22,46,188",2));	
	m_S2.insert(make_pair("0,1,12,15,44,12,0,192",3));
	m_S2.insert(make_pair("0,1,3,35,11,36,118,72",4));
	m_S2.insert(make_pair("0,7,3,29,29,36,100,72",5));	
	m_S2.insert(make_pair("0,13,3,23,47,36,82,72",6));
	m_S2.insert(make_pair("0,3,3,33,33,36,96,72",7));	
	m_S2.insert(make_pair("0,9,3,27,51,36,78,72",8));
	m_S2.insert(make_pair("0,3,3,13,33,12,116,96",9));
	m_S2.insert(make_pair("0,5,3,11,55,12,94,96",10));
	m_S2.insert(make_pair("0,1,3,15,11,12,138,96",11));	
	m_S2.insert(make_pair("0,9,12,27,36,36,48,108",12));
	m_S2.insert(make_pair("0,7,12,21,44,0,48,144",13));
	m_S2.insert(make_pair("0,15,3,57,69,0,132,0",14));
	m_S2.insert(make_pair("0,7,3,21,77,0,168,0",15));
	//G27
	m_N0.insert(make_pair("1,2,6,18,[[0,26],[1,1]],[[1,1,1],[3,1,2],[9,1,6],[27,1,18]]",1));
	m_N0.insert(make_pair("1,8,18,0,[[0,26],[1,1]],[[1,1,1],[3,1,8],[9,1,18]]",2));
	m_N0.insert(make_pair("1,26,0,0,[[0,26],[1,1]],[[1,1,1],[3,1,2],[3,3,24]]",3));	
	m_N0.insert(make_pair("1,8,18,0,[[0,26],[1,1]],[[1,1,1],[3,1,2],[3,3,6],[9,3,18]]",4));
	m_N0.insert(make_pair("1,26,0,0,[[0,26],[1,1]],[[1,1,1],[3,1,26]]",5));	
	//G30
	m_N0.insert(make_pair("1,3,2,4,0,12,8,0",1));
	m_N0.insert(make_pair("1,5,2,4,10,0,8,0",2));
	m_N0.insert(make_pair("1,15,2,4,0,0,8,0",3));	
	m_N0.insert(make_pair("1,1,2,4,2,4,8,8",4));	
	//G32
	m_N0.insert(make_pair("1,1,2,4,8,16",1));
	m_N0.insert(make_pair("1,7,24,0,0,0",2));	
	m_N0.insert(make_pair("1,3,12,16,0,0",3));
	m_N0.insert(make_pair("1,3,12,16,0,0,[[0,28],[1,4]],[[1,1,1],[2,1,3],[4,1,4],[4,2,8],[8,2,16]]",4));
	m_N0.insert(make_pair("1,7,8,16,0,0",5));	
	m_N0.insert(make_pair("1,11,20,0,0,0",6));
	m_N0.insert(make_pair("1,11,4,16,0,0",7));
	m_N0.insert(make_pair("1,3,12,16,0,0",8));	
	m_N0.insert(make_pair("1,11,12,8,0,0",9));
	m_N0.insert(make_pair("1,3,20,8,0,0,[[0,16],[1,16]],[[1,1,1],[2,1,3],[4,2,4],[4,4,16],[8,2,8]]",10));	
	m_N0.insert(make_pair("1,7,16,8,0,0",11));	
	m_N0.insert(make_pair("1,3,12,16,0,0,[[0,24],[1,8]],[[1,1,1],[2,1,3],[4,1,4],[4,2,8],[8,2,16]]",12));
	m_N0.insert(make_pair("1,3,20,8,0,0,[[0,16],[1,16]],[[1,1,1],[2,1,3],[4,2,4],[4,4,16],[8,2,8]]",13));	
	m_N0.insert(make_pair("1,3,20,8,0,0,[[0,24],[1,8]],[[1,1,1],[2,1,3],[4,2,4],[4,4,16],[8,2,8]]",14));	
	m_N0.insert(make_pair("1,3,4,24,0,0",15));
	m_N0.insert(make_pair("1,3,4,8,16,0",16));
	m_N0.insert(make_pair("1,3,4,8,16,0",17));
	m_N0.insert(make_pair("1,17,2,4,8,0",18));	
	m_N0.insert(make_pair("1,9,10,4,8,0",19));
	m_N0.insert(make_pair("1,1,18,4,8,0",20));	
	m_N0.insert(make_pair("1,7,24,0,0,0",21));
	m_N0.insert(make_pair("1,15,16,0,0,0",22));	
	m_N0.insert(make_pair("1,7,24,0,0,0",23));
	m_N0.insert(make_pair("1,7,24,0,0,0",24));	
	m_N0.insert(make_pair("1,11,20,0,0,0",25));	
	m_N0.insert(make_pair("1,3,28,0,0,0",26));
	m_N0.insert(make_pair("1,19,12,0,0,0",27));
	m_N0.insert(make_pair("1,15,16,0,0,0",28));	
	m_N0.insert(make_pair("1,7,24,0,0,0",29));
	m_N0.insert(make_pair("1,11,20,0,0,0,[[0,8],[1,24]],[[1,1,1],[2,1,3],[2,2,4],[2,4,4],[4,2,8],[4,4,12]]",30));	
	m_N0.insert(make_pair("1,11,20,0,0,0,[[0,8],[1,24]],[[1,1,1],[2,1,3],[2,4,8],[4,2,12],[4,4,8]]",31));	
	m_N0.insert(make_pair("1,3,28,0,0,0",32));
	m_N0.insert(make_pair("1,7,24,0,0,0",33));	
	m_N0.insert(make_pair("1,19,12,0,0,0",34));
	m_N0.insert(make_pair("1,3,28,0,0,0",35));	
	m_N0.insert(make_pair("1,7,8,16,0,0",36));	
	m_N0.insert(make_pair("1,7,8,16,0,0",37));
	m_N0.insert(make_pair("1,7,8,16,0,0",38));	
	m_N0.insert(make_pair("1,19,4,8,0,0",39));
	m_N0.insert(make_pair("1,11,12,8,0,0",40));	
	m_N0.insert(make_pair("1,3,20,8,0,0",41));
	m_N0.insert(make_pair("1,11,12,8,0,0",42));	
	m_N0.insert(make_pair("1,15,8,8,0,0",43));
	m_N0.insert(make_pair("1,7,16,8,0,0",44));	
	m_N0.insert(make_pair("1,15,16,0,0,0",45));	
	m_N0.insert(make_pair("1,23,8,0,0,0",46));
	m_N0.insert(make_pair("1,7,24,0,0,0",47));
	m_N0.insert(make_pair("1,15,16,0,0,0",48));	
	m_N0.insert(make_pair("1,19,12,0,0,0",49));
	m_N0.insert(make_pair("1,11,20,0,0,0",50));	
	m_N0.insert(make_pair("1,31,0,0,0,0",51));	
	//G32
	m_S2.insert(make_pair("0,1,5,22,92,376",1));
	m_S2.insert(make_pair("0,7,81,216,0,192",2));	
	m_S2.insert(make_pair("0,3,33,124,144,192",3));
	m_S2.insert(make_pair("0,3,33,124,144,192",4));
	m_S2.insert(make_pair("0,7,41,160,96,192",5));	
	m_S2.insert(make_pair("0,11,89,108,96,192",6));
	m_S2.insert(make_pair("0,11,49,148,96,192",7));	
	m_S2.insert(make_pair("0,3,33,172,96,192",8));	
	m_S2.insert(make_pair("0,11,69,128,96,192",9));
	m_S2.insert(make_pair("0,3,53,152,96,192",10));	
	m_S2.insert(make_pair("0,7,49,152,96,192",11));	
	m_S2.insert(make_pair("0,3,33,124,144,192",12));
	m_S2.insert(make_pair("0,3,53,104,144,192",13));	
	m_S2.insert(make_pair("0,3,53,104,144,192",14));	
	m_S2.insert(make_pair("0,3,13,144,144,192",15));
	m_S2.insert(make_pair("0,3,13,56,232,192",16));
	m_S2.insert(make_pair("0,3,13,56,232,192",17));
	m_S2.insert(make_pair("0,17,29,70,188,192",18));	
	m_S2.insert(make_pair("0,9,37,70,188,192",19));
	m_S2.insert(make_pair("0,1,45,70,188,192",20));	
	m_S2.insert(make_pair("0,7,81,216,192,0",21));
	m_S2.insert(make_pair("0,15,145,144,192,0",22));	
	m_S2.insert(make_pair("0,7,81,216,192,0",23));
	m_S2.insert(make_pair("0,7,81,120,288,0",24));	
	m_S2.insert(make_pair("0,11,89,204,192,0",25));	
	m_S2.insert(make_pair("0,3,73,132,288,0",26));
	m_S2.insert(make_pair("0,19,153,180,144,0",27));
	m_S2.insert(make_pair("0,15,97,240,144,0",28));
	m_S2.insert(make_pair("0,7,81,168,240,0",29));
	m_S2.insert(make_pair("0,11,89,156,240,0",30));	
	m_S2.insert(make_pair("0,11,89,156,240,0",31));	
	m_S2.insert(make_pair("0,3,73,84,336,0",32));
	m_S2.insert(make_pair("0,7,81,72,336,0",33));	
	m_S2.insert(make_pair("0,19,105,324,48,0",34));
	m_S2.insert(make_pair("0,3,73,180,240,0",35));	
	m_S2.insert(make_pair("0,7,41,160,288,0",36));	
	m_S2.insert(make_pair("0,7,41,160,288,0",37));
	m_S2.insert(make_pair("0,7,29,172,288,0",38));	
	m_S2.insert(make_pair("0,19,85,152,240,0",39));
	m_S2.insert(make_pair("0,11,69,176,240,0",40));	
	m_S2.insert(make_pair("0,3,53,200,240,0",41));
	m_S2.insert(make_pair("0,11,45,200,240,0",42));	
	m_S2.insert(make_pair("0,15,65,176,240,0",43));	
	m_S2.insert(make_pair("0,7,49,200,240,0",44));	
	m_S2.insert(make_pair("0,15,145,336,0,0",45));	
	m_S2.insert(make_pair("0,23,209,264,0,0",46));
	m_S2.insert(make_pair("0,7,81,408,0,0",47));
	m_S2.insert(make_pair("0,15,97,384,0,0",48));	
	m_S2.insert(make_pair("0,19,129,348,0,0",49));
	m_S2.insert(make_pair("0,11,65,420,0,0",50));	
	m_S2.insert(make_pair("0,31,465,0,0,0",51));
	//G32
	m_kKEZDCANS.insert(make_pair("32,[32,0,0,0,0,0],8,32,1,0,1,1,1",3));	
	m_kKEZDCANS.insert(make_pair("20,[8,12,0,0,0,0],8,8,2,0,0,1,1",4));	
	m_kKEZDCANS.insert(make_pair("20,[8,12,0,0,0,0],8,8,2,0,0,1,1",12));	
	m_kKEZDCANS.insert(make_pair("14,[4,6,4,0,0,0],8,4,4,0,0,1,1",13));	
	m_kKEZDCANS.insert(make_pair("14,[4,6,4,0,0,0],8,4,4,0,0,1,1",14));
	m_kKEZDCANS.insert(make_pair("32,[32,0,0,0,0,0],16,32,1,0,1,1,1",16));	
	m_kKEZDCANS.insert(make_pair("20,[8,12,0,0,0,0],16,8,2,0,0,1,1",17));
	m_kKEZDCANS.insert(make_pair("32,[32,0,0,0,0,0],4,32,1,0,1,1,1",21));	
	m_kKEZDCANS.insert(make_pair("20,[8,12,0,0,0,0],4,8,2,0,0,1,1",23));	
	m_kKEZDCANS.insert(make_pair("20,[8,12,0,0,0,0],4,8,2,0,0,1,1",24));	
	m_kKEZDCANS.insert(make_pair("32,[32,0,0,0,0,0],8,32,1,0,1,1,1",36));	
	m_kKEZDCANS.insert(make_pair("20,[8,12,0,0,0,0],8,8,2,0,0,1,1",37));
	m_kKEZDCANS.insert(make_pair("20,[8,12,0,0,0,0],4,8,2,0,0,1,1",47));	
	//G36
	m_N0.insert(make_pair("1,1,2,18,2,6,0,6,0",1));
	m_N0.insert(make_pair("1,1,2,2,2,6,4,6,12",2));	
	m_N0.insert(make_pair("1,3,2,0,6,24,0,0,0",3));
	m_N0.insert(make_pair("1,19,2,0,2,6,0,6,0",4));
	m_N0.insert(make_pair("1,3,2,0,6,6,0,18,0",5));	
	m_N0.insert(make_pair("1,1,8,6,8,0,12,0,0",6));
	m_N0.insert(make_pair("1,1,8,18,8,0,0,0,0",7));	
	m_N0.insert(make_pair("1,1,8,2,8,0,16,0,0",8));
	m_N0.insert(make_pair("1,9,8,18,0,0,0,0,0",9));
	m_N0.insert(make_pair("1,15,8,0,12,0,0,0,0",10));
	m_N0.insert(make_pair("1,3,26,0,6,0,0,0,0",11));	
	m_N0.insert(make_pair("1,7,8,0,20,0,0,0,0",12));
	m_N0.insert(make_pair("1,19,8,0,8,0,0,0,0",13));
	m_N0.insert(make_pair("1,3,8,0,24,0,0,0,0",14));
	//G37
	m_N0.insert(make_pair("1,36,[[0,36],[1,1]],[[1,1,1],[37,1,36]]",1));
	//G40	
	m_N0.insert(make_pair("1,1,2,4,20,4,8,0",1));
	m_N0.insert(make_pair("1,1,2,4,4,4,8,16",2));
	m_N0.insert(make_pair("1,1,10,4,20,4,0,0",3));
	m_N0.insert(make_pair("1,1,22,4,0,4,8,0",4));
	m_N0.insert(make_pair("1,11,12,4,0,4,8,0",5));
	m_N0.insert(make_pair("1,21,2,4,0,4,8,0",6));
	m_N0.insert(make_pair("1,3,20,4,0,12,0,0",7));
	m_N0.insert(make_pair("1,13,10,4,0,12,0,0",8));
	m_N0.insert(make_pair("1,3,4,4,0,12,16,0",9));
	m_N0.insert(make_pair("1,5,2,4,0,20,8,0",10));
	m_N0.insert(make_pair("1,1,6,4,0,4,24,0",11));
	m_N0.insert(make_pair("1,11,20,4,0,4,0,0",12));
	m_N0.insert(make_pair("1,23,0,4,0,12,0,0",13));
	m_N0.insert(make_pair("1,7,0,4,0,28,0,0",14));	
	//G45
	m_N0.insert(make_pair("1,2,4,6,8,24",1));
	m_N0.insert(make_pair("1,8,4,0,32,0",2));	
	//G48
	m_N0.insert(make_pair("1,1,2,2,2,4,4,24,8,0",1));
	m_N0.insert(make_pair("1,1,2,2,2,4,4,8,8,16",2));
	m_N0.insert(make_pair("1,3,32,12,0,0,0,0,0,0",3));	
	m_N0.insert(make_pair("1,7,2,8,2,16,4,0,8,0",4));	
	m_N0.insert(make_pair("1,13,2,14,2,4,4,0,8,0",6));
	m_N0.insert(make_pair("1,25,2,2,2,4,4,0,8,0",7));	
	m_N0.insert(make_pair("1,1,2,26,2,4,4,0,8,0",8));
	m_N0.insert(make_pair("1,3,2,4,6,24,8,0,0,0",9));	
	m_N0.insert(make_pair("1,3,2,4,6,24,8,0,0,0",10));
	m_N0.insert(make_pair("1,3,2,28,6,0,8,0,0,0,[[0,44],[1,4]],[[1,1,1],[2,1,3],[3,2,2],[4,1,4],[4,3,24],[6,2,6],[12,2,8]]",11));	
	m_N0.insert(make_pair("1,3,2,28,6,0,8,0,0,0,[[0,40],[1,8]],[[1,1,1],[2,1,3],[3,2,2],[4,2,4],[4,6,24],[6,2,6],[12,2,8]]",13));
	m_N0.insert(make_pair("1,15,2,16,6,0,8,0,0,0",14));	
	m_N0.insert(make_pair("1,17,2,2,10,12,4,0,0,0",15));
	m_N0.insert(make_pair("1,13,2,6,2,12,12,0,0,0,[[0,16],[1,32]],[[1,1,1],[2,1,1],[2,12,12],[3,2,2],[4,2,2],[4,4,4],[6,2,2],[8,6,12],[12,4,12]]",17));	
	m_N0.insert(make_pair("1,3,2,12,6,0,24,0,0,0,[[0,44],[1,4]],[[1,1,1],[2,1,3],[3,1,2],[4,1,12],[6,1,6],[12,1,24]]",20));	
	m_N0.insert(make_pair("1,7,2,8,14,0,16,0,0,0",21));
	m_N0.insert(make_pair("1,3,2,12,6,0,24,0,0,0,[[0,40],[1,8]],[[1,1,1],[2,1,3],[3,1,2],[4,2,12],[6,1,6],[12,2,24]]",22));	
	m_N0.insert(make_pair("1,3,2,4,6,8,8,0,16,0",23));
	m_N0.insert(make_pair("1,3,2,4,6,8,8,0,16,0",24));	
	m_N0.insert(make_pair("1,9,2,2,18,4,4,0,8,0",25));	
	m_N0.insert(make_pair("1,5,2,6,10,4,12,0,8,0",26));
	m_N0.insert(make_pair("1,1,2,10,2,4,20,0,8,0",27));
	m_N0.insert(make_pair("1,1,8,18,8,12,0,0,0,0",28));	
	m_N0.insert(make_pair("1,13,8,6,8,12,0,0,0,0",29));	
	m_N0.insert(make_pair("1,7,8,24,8,0,0,0,0,0",30));
	m_N0.insert(make_pair("1,7,8,8,8,0,16,0,0,0",31));
	m_N0.insert(make_pair("1,3,8,12,24,0,0,0,0,0",32));
	m_N0.insert(make_pair("1,7,8,8,8,0,16,0,0,0",33));
	m_N0.insert(make_pair("1,3,2,28,6,0,8,0,0,0",34));	
	m_N0.insert(make_pair("1,15,2,16,6,0,8,0,0,0",35));	
	m_N0.insert(make_pair("1,27,2,4,6,0,8,0,0,0",36));	
	m_N0.insert(make_pair("1,15,2,16,6,0,8,0,0,0",37));
	m_N0.insert(make_pair("1,23,2,8,10,0,4,0,0,0",38));	
	m_N0.insert(make_pair("1,7,2,24,2,0,12,0,0,0",40));	
	m_N0.insert(make_pair("1,7,2,24,14,0,0,0,0,0",42));	
	m_N0.insert(make_pair("1,19,2,12,14,0,0,0,0,0",43));	
	m_N0.insert(make_pair("1,7,2,8,14,0,16,0,0,0",44));		
	m_N0.insert(make_pair("1,11,2,4,22,0,8,0,0,0",45));	
	m_N0.insert(make_pair("1,3,2,12,6,0,24,0,0,0",46));
	m_N0.insert(make_pair("1,7,2,8,14,0,16,0,0,0",47));
	m_N0.insert(make_pair("1,19,8,12,8,0,0,0,0,0",48));	
	m_N0.insert(make_pair("1,15,8,0,24,0,0,0,0,0",49));
	m_N0.insert(make_pair("1,15,32,0,0,0,0,0,0,0",50));		
	m_N0.insert(make_pair("1,31,2,0,14,0,0,0,0,0",51));	
	m_N0.insert(make_pair("1,15,2,0,30,0,0,0,0,0",52));		
	//G48
	m_S2.insert(make_pair("0,1,3,5,11,22,46,276,188,576",1));
	m_S2.insert(make_pair("0,1,3,5,11,22,46,92,188,760",2));
	m_S2.insert(make_pair("0,3,48,33,0,36,192,48,0,768",3));	
	m_S2.insert(make_pair("0,7,3,29,29,124,100,144,404,288",4));	
	m_S2.insert(make_pair("0,13,3,53,47,94,154,144,332,288",6));
	m_S2.insert(make_pair("0,25,3,41,83,94,118,144,332,288",7));	
	m_S2.insert(make_pair("0,1,3,65,11,94,190,144,332,288",8));
	m_S2.insert(make_pair("0,3,3,13,33,144,116,144,384,288",9));	
	m_S2.insert(make_pair("0,3,3,13,33,144,116,144,384,288",10));
	m_S2.insert(make_pair("0,3,3,73,33,84,260,144,240,288",11));	
	m_S2.insert(make_pair("0,3,3,73,33,84,260,144,240,288",13));
	m_S2.insert(make_pair("0,15,3,97,69,48,296,144,168,288",14));	
	m_S2.insert(make_pair("0,17,3,29,91,114,130,144,312,288",15));
	m_S2.insert(make_pair("0,13,3,33,47,114,174,144,312,288",17));	
	m_S2.insert(make_pair("0,3,3,33,33,36,300,48,288,384",20));	
	m_S2.insert(make_pair("0,7,3,41,77,24,352,48,192,384",21));
	m_S2.insert(make_pair("0,3,3,33,33,36,300,48,288,384",22));	
	m_S2.insert(make_pair("0,3,3,13,33,56,116,48,472,384",23));
	m_S2.insert(make_pair("0,3,3,13,33,56,116,48,472,384",24));	
	m_S2.insert(make_pair("0,9,3,17,99,46,142,48,380,384",25));	
	m_S2.insert(make_pair("0,5,3,21,55,46,186,48,380,384",26));
	m_S2.insert(make_pair("0,1,3,25,11,46,230,48,380,384",27));
	m_S2.insert(make_pair("0,1,12,45,44,114,144,144,192,432",28));
	m_S2.insert(make_pair("0,13,12,33,116,114,72,144,192,432",29));	
	m_S2.insert(make_pair("0,7,12,81,44,72,192,144,144,432",30));	
	m_S2.insert(make_pair("0,7,12,41,44,72,232,0,144,576",31));
	m_S2.insert(make_pair("0,3,12,33,132,84,96,0,192,576",32));
	m_S2.insert(make_pair("0,7,12,29,44,84,184,0,192,576",33));
	m_S2.insert(make_pair("0,3,3,73,33,228,260,0,528,0",34));	
	m_S2.insert(make_pair("0,15,3,97,69,192,296,0,456,0",35));	
	m_S2.insert(make_pair("0,27,3,121,105,156,332,0,384,0",36));	
	m_S2.insert(make_pair("0,15,3,61,69,228,224,0,528,0",37));
	m_S2.insert(make_pair("0,23,3,125,109,156,328,0,384,0",38));	
	m_S2.insert(make_pair("0,7,3,69,29,228,264,0,528,0",40));	
	m_S2.insert(make_pair("0,7,3,81,77,216,312,0,432,0",42));	
	m_S2.insert(make_pair("0,19,3,105,113,180,348,0,360,0",43));	
	m_S2.insert(make_pair("0,7,3,41,77,72,352,0,576,0",44));		
	m_S2.insert(make_pair("0,11,3,49,121,60,404,0,480,0",45));	
	m_S2.insert(make_pair("0,3,3,33,33,84,300,0,672,0",46));
	m_S2.insert(make_pair("0,7,3,29,77,84,256,0,672,0",47));
	m_S2.insert(make_pair("0,19,12,105,116,180,120,0,360,216",48));	
	m_S2.insert(make_pair("0,15,12,105,132,0,144,0,432,288",49));
	m_S2.insert(make_pair("0,15,48,105,0,0,960,0,0,0",50));		
	m_S2.insert(make_pair("0,31,3,273,149,0,672,0,0,0",51));	
	m_S2.insert(make_pair("0,15,3,105,165,0,840,0,0,0",52));
	//G48	
	m_kKEZDCANS.insert(make_pair("12,[2,3,0,4,2,0,1,0,0,0],24,2,12,0,0,0,1",17));	
	m_kKEZDCANS.insert(make_pair("48,[48,0,0,0,0,0,0,0,0,0],24,48,1,0,1,1,1",23));	
	m_kKEZDCANS.insert(make_pair("30,[12,18,0,0,0,0,0,0,0,0],24,12,2,0,0,1,1",24));
	//G60	
	m_N0.insert(make_pair("1,1,2,6,4,2,4,0,8,24,8,0",1));
	m_N0.insert(make_pair("1,1,2,10,4,2,4,20,8,0,8,0",2));	
	m_N0.insert(make_pair("1,1,2,30,4,2,4,0,8,0,8,0",3));
	m_N0.insert(make_pair("1,1,2,2,4,2,4,4,8,8,8,16",4));
	m_N0.insert(make_pair("1,15,20,0,24,0,0,0,0,0,0,0",5));	
	m_N0.insert(make_pair("1,5,2,10,4,10,0,20,8,0,0,0",6));
	m_N0.insert(make_pair("1,5,2,30,4,10,0,0,8,0,0,0",7));	
	m_N0.insert(make_pair("1,23,2,0,4,10,12,0,8,0,0,0",8));
	m_N0.insert(make_pair("1,3,8,0,4,0,12,0,32,0,0,0",9));
	m_N0.insert(make_pair("1,11,2,0,4,22,4,0,8,0,8,0",10));
	m_N0.insert(make_pair("1,7,2,0,4,2,28,0,8,0,8,0",11));	
	m_N0.insert(make_pair("1,31,2,0,4,2,4,0,8,0,8,0",12));
	m_N0.insert(make_pair("1,3,2,0,4,6,12,0,8,0,24,0",13));	
	//G72	
	m_N0.insert(make_pair("1,1,2,2,2,36,6,4,6,0,12,0",1));
	m_N0.insert(make_pair("1,1,2,2,2,4,6,4,6,8,12,24",2));
	m_N0.insert(make_pair("1,1,2,6,2,0,24,12,24,0,0,0",3));
	m_N0.insert(make_pair("1,1,2,38,2,0,6,4,6,0,12,0",4));
	m_N0.insert(make_pair("1,19,2,20,2,0,6,4,6,0,12,0",5));
	m_N0.insert(make_pair("1,37,2,2,2,0,6,4,6,0,12,0",6));
	m_N0.insert(make_pair("1,3,2,36,6,0,6,0,18,0,0,0",7));
	m_N0.insert(make_pair("1,21,2,18,6,0,6,0,18,0,0,0",8));
	m_N0.insert(make_pair("1,3,2,4,6,0,6,8,18,0,24,0",9));
	m_N0.insert(make_pair("1,5,2,2,10,0,6,4,30,0,12,0",10));
	m_N0.insert(make_pair("1,1,2,6,2,0,6,12,6,0,36,0",11));
	m_N0.insert(make_pair("1,1,8,2,8,12,0,16,0,24,0,0",12));
	m_N0.insert(make_pair("1,1,8,2,8,36,0,16,0,0,0,0",13));
	m_N0.insert(make_pair("1,1,8,2,8,4,0,16,0,32,0,0",14));
	m_N0.insert(make_pair("1,21,2,18,6,0,24,0,0,0,0,0",15));
	m_N0.insert(make_pair("1,7,2,0,14,0,24,0,24,0,0,0",16));
	m_N0.insert(make_pair("1,39,2,0,6,0,6,0,18,0,0,0",17));
	m_N0.insert(make_pair("1,7,2,0,14,0,6,0,42,0,0,0",18));
	m_N0.insert(make_pair("1,1,8,18,8,36,0,0,0,0,0,0",19));
	m_N0.insert(make_pair("1,7,8,24,20,0,0,12,0,0,0,0",20));
	m_N0.insert(make_pair("1,19,8,12,8,0,0,24,0,0,0,0",21));
	m_N0.insert(make_pair("1,13,8,18,32,0,0,0,0,0,0,0",22));
	m_N0.insert(make_pair("1,25,8,6,20,0,0,12,0,0,0,0",23));
	m_N0.insert(make_pair("1,1,8,30,8,0,0,24,0,0,0,0",24));
	m_N0.insert(make_pair("1,1,26,6,26,0,0,12,0,0,0,0",25));
	m_N0.insert(make_pair("1,1,8,14,8,0,0,40,0,0,0,0",26));
	m_N0.insert(make_pair("1,7,8,8,20,0,0,28,0,0,0,0",27));
	m_N0.insert(make_pair("1,13,8,2,32,0,0,16,0,0,0,0",28));
	m_N0.insert(make_pair("1,3,8,12,24,0,0,24,0,0,0,0",29));
	m_N0.insert(make_pair("1,9,8,6,36,0,0,12,0,0,0,0",30));
	m_N0.insert(make_pair("1,1,8,38,8,0,0,16,0,0,0,0",31));
	m_N0.insert(make_pair("1,19,8,20,8,0,0,16,0,0,0,0",32));
	m_N0.insert(make_pair("1,37,8,2,8,0,0,16,0,0,0,0",33));
	m_N0.insert(make_pair("1,3,8,36,24,0,0,0,0,0,0,0",34));
	m_N0.insert(make_pair("1,21,8,18,24,0,0,0,0,0,0,0,[[0,32],[1,40]],[[1,1,1],[2,1,1],[2,2,2],[2,18,18],[3,2,8],[4,18,18],[6,2,24]]",35));
	m_N0.insert(make_pair("1,3,8,4,24,0,0,32,0,0,0,0",36));
	m_N0.insert(make_pair("1,5,8,2,40,0,0,16,0,0,0,0",37));
	m_N0.insert(make_pair("1,1,8,6,8,0,0,48,0,0,0,0",38));
	m_N0.insert(make_pair("1,9,8,18,0,36,0,0,0,0,0,0",39));
	m_N0.insert(make_pair("1,21,8,18,24,0,0,0,0,0,0,0,[[0,32],[1,40]],[[1,1,1],[2,6,12],[2,9,9],[3,4,8],[4,18,18],[6,12,24]]",40));
	m_N0.insert(make_pair("1,9,8,54,0,0,0,0,0,0,0,0",41));
	m_N0.insert(make_pair("1,9,26,6,18,0,0,12,0,0,0,0",42));
	m_N0.insert(make_pair("1,21,26,18,6,0,0,0,0,0,0,0",43));
	m_N0.insert(make_pair("1,15,26,0,30,0,0,0,0,0,0,0",44));
	m_N0.insert(make_pair("1,19,8,36,8,0,0,0,0,0,0,0",45));
	m_N0.insert(make_pair("1,31,8,0,32,0,0,0,0,0,0,0",46));
	m_N0.insert(make_pair("1,7,26,0,38,0,0,0,0,0,0,0",47));
	m_N0.insert(make_pair("1,15,8,0,48,0,0,0,0,0,0,0",48));
	m_N0.insert(make_pair("1,39,8,0,24,0,0,0,0,0,0,0",49));
	m_N0.insert(make_pair("1,7,8,0,56,0,0,0,0,0,0,0",50));
	//G90	
	m_N0.insert(make_pair("1,9,2,4,0,6,36,8,0,0,24,0",1));
	m_N0.insert(make_pair("1,5,2,4,10,6,0,8,30,0,24,0",2));
	m_N0.insert(make_pair("1,45,2,4,0,6,0,8,0,0,24,0",3));
	m_N0.insert(make_pair("1,1,2,4,2,6,4,8,6,8,24,24",4));
	m_N0.insert(make_pair("1,5,8,4,40,0,0,32,0,0,0,0",5));
	m_N0.insert(make_pair("1,3,8,4,6,0,12,32,0,24,0,0",6));
	m_N0.insert(make_pair("1,15,8,4,30,0,0,32,0,0,0,0",7));
	m_N0.insert(make_pair("1,9,8,4,0,0,36,32,0,0,0,0",8));
	m_N0.insert(make_pair("1,45,8,4,0,0,0,32,0,0,0,0",9));
	m_N0.insert(make_pair("1,1,8,4,8,0,4,32,0,32,0,0",10));	
	//G120	
	m_N0.insert(make_pair("1,1,2,2,4,2,12,4,4,8,8,0,8,48,16,0",1));
	m_N0.insert(make_pair("1,1,2,2,4,2,20,4,4,8,8,40,8,0,16,0",2));
	m_N0.insert(make_pair("1,1,2,2,4,2,60,4,4,8,8,0,8,0,16,0",3));
	m_N0.insert(make_pair("1,1,2,2,4,2,4,4,4,8,8,8,8,16,16,32",4));
	m_N0.insert(make_pair("1,1,20,30,24,20,0,24,0,0,0,0,0,0,0,0",5));
	m_N0.insert(make_pair("1,1,2,10,4,2,20,4,20,8,0,40,8,0,0,0",6));
	m_N0.insert(make_pair("1,1,2,10,4,2,60,4,20,8,0,0,8,0,0,0",7));
	m_N0.insert(make_pair("1,11,2,36,4,22,0,4,0,8,24,0,8,0,0,0",8));
	m_N0.insert(make_pair("1,7,2,40,4,2,0,28,20,8,0,0,8,0,0,0",9));
	m_N0.insert(make_pair("1,31,2,16,4,2,0,4,20,8,24,0,8,0,0,0",10));
	m_N0.insert(make_pair("1,17,2,30,4,22,0,28,0,8,0,0,8,0,0,0",11));
	m_N0.insert(make_pair("1,41,2,6,4,22,0,4,0,8,24,0,8,0,0,0",12));
	m_N0.insert(make_pair("1,37,2,10,4,2,0,28,20,8,0,0,8,0,0,0",13));
	m_N0.insert(make_pair("1,1,2,46,4,2,0,4,20,8,24,0,8,0,0,0",14));
	m_N0.insert(make_pair("1,1,8,6,4,8,0,4,0,32,24,0,32,0,0,0",15));
	m_N0.insert(make_pair("1,1,2,22,4,2,0,4,44,8,8,0,8,0,16,0",16));
	m_N0.insert(make_pair("1,11,2,12,4,22,0,4,24,8,8,0,8,0,16,0",17));
	m_N0.insert(make_pair("1,21,2,2,4,42,0,4,4,8,8,0,8,0,16,0",18));
	m_N0.insert(make_pair("1,3,2,20,4,6,0,12,40,8,0,0,24,0,0,0",19));
	m_N0.insert(make_pair("1,13,2,10,4,26,0,12,20,8,0,0,24,0,0,0",20));
	m_N0.insert(make_pair("1,1,2,14,4,2,0,4,4,8,56,0,8,0,16,0",21));
	m_N0.insert(make_pair("1,7,2,8,4,2,0,28,4,8,32,0,8,0,16,0",22));
	m_N0.insert(make_pair("1,13,2,2,4,2,0,52,4,8,8,0,8,0,16,0",23));
	m_N0.insert(make_pair("1,3,2,12,4,6,0,12,0,8,48,0,24,0,0,0",24));
	m_N0.insert(make_pair("1,9,2,6,4,6,0,36,0,8,24,0,24,0,0,0",25));
	m_N0.insert(make_pair("1,1,2,62,4,2,0,4,4,8,8,0,8,0,16,0",26));
	m_N0.insert(make_pair("1,31,2,32,4,2,0,4,4,8,8,0,8,0,16,0",27));
	m_N0.insert(make_pair("1,61,2,2,4,2,0,4,4,8,8,0,8,0,16,0",28));
	m_N0.insert(make_pair("1,3,2,60,4,6,0,12,0,8,0,0,24,0,0,0",29));
	m_N0.insert(make_pair("1,33,2,30,4,6,0,12,0,8,0,0,24,0,0,0",30));
	m_N0.insert(make_pair("1,3,2,4,4,6,0,12,8,8,16,0,24,0,32,0",31));
	m_N0.insert(make_pair("1,5,2,2,4,10,0,20,4,8,8,0,40,0,16,0",32));
	m_N0.insert(make_pair("1,1,2,6,4,2,0,4,12,8,24,0,8,0,48,0",33));
	m_N0.insert(make_pair("1,25,20,30,24,20,0,0,0,0,0,0,0,0,0,0",34));
	m_N0.insert(make_pair("1,31,20,0,24,20,0,24,0,0,0,0,0,0,0,0",35));
	m_N0.insert(make_pair("1,23,2,40,4,10,0,12,20,8,0,0,0,0,0,0",36));
	m_N0.insert(make_pair("1,9,8,6,4,0,0,36,0,32,24,0,0,0,0,0",37));
	m_N0.insert(make_pair("1,33,8,30,4,0,0,12,0,32,0,0,0,0,0,0",38));
	m_N0.insert(make_pair("1,23,8,0,4,40,0,12,0,32,0,0,0,0,0,0",39));
	m_N0.insert(make_pair("1,11,2,20,4,22,0,4,40,8,0,0,8,0,0,0",40));
	m_N0.insert(make_pair("1,11,2,60,4,22,0,4,0,8,0,0,8,0,0,0",41));
	m_N0.insert(make_pair("1,47,2,0,4,22,0,28,0,8,0,0,8,0,0,0",42));
	m_N0.insert(make_pair("1,7,8,0,4,8,0,28,0,32,0,0,32,0,0,0",43));
	m_N0.insert(make_pair("1,23,2,0,4,46,0,12,0,8,0,0,24,0,0,0",44));
	m_N0.insert(make_pair("1,15,2,0,4,6,0,60,0,8,0,0,24,0,0,0",45));
	m_N0.insert(make_pair("1,63,2,0,4,6,0,12,0,8,0,0,24,0,0,0",46));
	m_N0.insert(make_pair("1,7,2,0,4,14,0,28,0,8,0,0,56,0,0,0",47));	
	//G180
	m_N0.insert(make_pair("1,1,2,18,4,2,6,4,0,8,6,72,8,0,24,0,24,0",1));
	m_N0.insert(make_pair("1,1,2,10,4,2,6,4,20,8,6,0,8,60,24,0,24,0",2));
	m_N0.insert(make_pair("1,1,2,90,4,2,6,4,0,8,6,0,8,0,24,0,24,0",3));
	m_N0.insert(make_pair("1,1,2,2,4,2,6,4,4,8,6,8,8,12,24,16,24,48",4));
	m_N0.insert(make_pair("1,5,2,10,4,10,6,0,20,8,30,0,0,60,24,0,0,0",5));
	m_N0.insert(make_pair("1,5,2,90,4,10,6,0,0,8,30,0,0,0,24,0,0,0",6));
	m_N0.insert(make_pair("1,59,2,0,4,10,6,36,0,8,30,0,0,0,24,0,0,0",7));
	m_N0.insert(make_pair("1,3,2,0,4,6,24,12,0,8,0,0,24,0,96,0,0,0",8));
	m_N0.insert(make_pair("1,11,2,0,4,22,6,4,0,8,66,0,8,0,24,0,24,0",9));
	m_N0.insert(make_pair("1,19,2,0,4,2,6,76,0,8,6,0,8,0,24,0,24,0",10));
	m_N0.insert(make_pair("1,91,2,0,4,2,6,4,0,8,6,0,8,0,24,0,24,0",11));
	m_N0.insert(make_pair("1,3,2,0,4,6,6,12,0,8,18,0,24,0,24,0,72,0",12));
	m_N0.insert(make_pair("1,1,8,10,4,8,0,4,80,32,0,0,32,0,0,0,0,0",13));
	m_N0.insert(make_pair("1,1,8,6,4,8,0,4,12,32,0,24,32,0,0,48,0,0",14));
	m_N0.insert(make_pair("1,1,8,30,4,8,0,4,60,32,0,0,32,0,0,0,0,0",15));
	m_N0.insert(make_pair("1,1,8,18,4,8,0,4,0,32,0,72,32,0,0,0,0,0",16));
	m_N0.insert(make_pair("1,1,8,90,4,8,0,4,0,32,0,0,32,0,0,0,0,0",17));
	m_N0.insert(make_pair("1,1,8,2,4,8,0,4,16,32,0,8,32,0,0,64,0,0",18));
	m_N0.insert(make_pair("1,15,62,0,24,30,0,0,0,48,0,0,0,0,0,0,0,0",19));
	m_N0.insert(make_pair("1,5,8,10,4,40,0,0,80,32,0,0,0,0,0,0,0,0",20));
	m_N0.insert(make_pair("1,5,8,30,4,40,0,0,60,32,0,0,0,0,0,0,0,0",21));
	m_N0.insert(make_pair("1,5,8,90,4,40,0,0,0,32,0,0,0,0,0,0,0,0",22));
	m_N0.insert(make_pair("1,9,8,18,4,0,0,36,0,32,0,72,0,0,0,0,0,0",23));
	m_N0.insert(make_pair("1,9,8,90,4,0,0,36,0,32,0,0,0,0,0,0,0,0",24));
	m_N0.insert(make_pair("1,45,8,90,4,0,0,0,0,32,0,0,0,0,0,0,0,0",25));
	m_N0.insert(make_pair("1,23,8,0,4,76,0,12,0,32,0,0,24,0,0,0,0,0",26));
	m_N0.insert(make_pair("1,59,8,0,4,40,0,36,0,32,0,0,0,0,0,0,0,0",27));
	m_N0.insert(make_pair("1,15,8,0,4,12,0,60,0,32,0,0,48,0,0,0,0,0",28));
	m_N0.insert(make_pair("1,63,8,0,4,36,0,12,0,32,0,0,24,0,0,0,0,0",29));
	m_N0.insert(make_pair("1,39,8,0,4,60,0,36,0,32,0,0,0,0,0,0,0,0",30));
	m_N0.insert(make_pair("1,3,26,0,4,6,0,12,0,104,0,0,24,0,0,0,0,0",31));
	m_N0.insert(make_pair("1,11,8,0,4,88,0,4,0,32,0,0,32,0,0,0,0,0",32));
	m_N0.insert(make_pair("1,7,8,0,4,20,0,28,0,32,0,0,80,0,0,0,0,0",33));
	m_N0.insert(make_pair("1,31,8,0,4,68,0,4,0,32,0,0,32,0,0,0,0,0",34));
	m_N0.insert(make_pair("1,19,8,0,4,8,0,76,0,32,0,0,32,0,0,0,0,0",35));
	m_N0.insert(make_pair("1,91,8,0,4,8,0,4,0,32,0,0,32,0,0,0,0,0",36));
	m_N0.insert(make_pair("1,3,8,0,4,24,0,12,0,32,0,0,96,0,0,0,0,0",37));	
}	

IDHelper::~IDHelper(){
	m_N0.clear();
	m_S2.clear();	
	m_kKEZDCANS.clear();	
}

vector<int>  IDHelper::IDFromN0C1Nk(string& N0,string& C1Nk){
	std::multimap<string,int>::iterator it;
	std::pair<std::multimap<string,int>::iterator, std::multimap<string,int>::iterator> pa;
    vector<int> v;
	string strN0C1Nk=N0+","+C1Nk;
	pa = m_N0.equal_range(strN0C1Nk);		
	for( it = pa.first; it != m_N0.end() && it != pa.second; ++it){
		v.push_back(it->second);
	}
    if(v.size()<=0){
		pa = m_N0.equal_range(N0);		
		for( it = pa.first; it != m_N0.end() && it != pa.second; ++it){
			v.push_back(it->second);
		}
	}
	return v;
}

vector<int>  IDHelper::IDFromS2(string& S2){
	std::multimap<string,int>::iterator it;
	std::pair<std::multimap<string,int>::iterator, std::multimap<string,int>::iterator> pa;
	pa = m_S2.equal_range(S2);	
    vector<int> v;
	for( it = pa.first; it != m_S2.end() && it != pa.second; ++it){
		v.push_back(it->second);
	}
	return v;
}

vector<int>  IDHelper::IDFromkKEZDCANS(string& kKEZDCANS){
	std::multimap<string,int>::iterator it;
	std::pair<std::multimap<string,int>::iterator, std::multimap<string,int>::iterator> pa;
	pa = m_kKEZDCANS.equal_range(kKEZDCANS);	
    vector<int> v;
	for( it = pa.first; it != m_kKEZDCANS.end() && it != pa.second; ++it){
		v.push_back(it->second);
	}
	return v;
}

// 未知n阶群的群元的阶
int getGnEOrder(const vector<vector<int>> &A,int a){
	int n=A.size();
	if(a<0||a>=n)
		return -1;
	int t=0;
	for(int i=1;i<=n;i++)
	{
		t=A[t][a]-1;
		if(t==0)
			return i;
		if(t<0)
			return -1;
	}
	return -1;
}

vector<int>  Factors(int n){
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

string calcN0(vector<vector<int> >& A){
    int n=A.size();
	vector<int> vOrders=Factors(n);
	vector<int> vCounts(n+1);
 	for(int i=0;i<n;i++)
	{
		int ord=getGnEOrder(A,i);
		if(ord<1)
			return "";
		vCounts[ord]=vCounts[ord]+1;
	}
	string strN0="";
	for(int i=0;i<vOrders.size();i++)
	{
		char sz[200]={0};
		sprintf(sz,"%d,",vCounts[vOrders[i]]);
		strN0+=sz;
	}
	if(strN0.size()>2)
	{
		strN0=strN0.substr(0,strN0.size()-1);
	}
	return strN0;
}

// 两个元素生成的子群的阶的分布
string calcS2(IGroup* g){
	int n=g->size();
	vector<int> vOrders=Factors(n);
	vector<int> vCounts(n+1);
	vector<int> gen(2);
 	for(int i=0;i<n;i++)
	{
		for(int j=i+1;j<n;j++)
		{
			gen[0]=i;
			gen[1]=j;
			vector<int> vij=FG(g,gen);
			int ord=vij.size();
			vCounts[ord]=vCounts[ord]+1;
		}
	}
	string str="";
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
	return str;
}

//-----------------------------Begin of calckKEZDCANS---------------------------------
// 判断群g是否真的是群（没有利用群的正则表示，仅仅根据抽象群的定义）
bool IsGroup(IGroup* g){
	int N=g->size();
	// 0是乘法单位元
	// 乘法封闭性
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			int ij=g->mul(i,j);
			if(ij<0||ij>=N){
				return false;
			}
		}
	}
	// 有乘法逆元
	for(int i=0;i<N;i++){
		int i1=g->inv(i);
		if(i1<0||i1>=N){
			return false;
		}
	}
	// 结合律（利用群的正则表示判断时没有加这一条件）
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			for(int k=0;k<N;k++){
				int ij_k=g->mul(g->mul(i,j),k);
				int i_jk=g->mul(i,g->mul(j,k));
				if(ij_k!=i_jk){
					return false;
				}
			}
		}
	}
	return true;
}

vector<int> Order(IGroup* g,int m){
	vector<int> ret;
	int mi=m;
	int m0=0;
    ret.push_back(m0);
	while(mi!=m0){
		ret.push_back(mi);
		mi=g->mul(mi,m);
	}
	return ret;
}

string calcN0(IGroup* g){
    int n=g->size();
	vector<int> vOrders=Factors(n);
	vector<int> vCounts(n+1);
 	for(int i=0;i<n;i++)
	{
		vector<int> S1=Order(g,i);
		int ord=S1.size();		
		if(ord<1)
			return "";
		vCounts[ord]=vCounts[ord]+1;
	}
	string strN0="";
	for(int i=0;i<vOrders.size();i++)
	{
		char sz[200]={0};
		sprintf(sz,"%d,",vCounts[vOrders[i]]);
		strN0+=sz;
	}
	if(strN0.size()>2)
	{
		strN0=strN0.substr(0,strN0.size()-1);
	}
	return strN0;
}

bool IsCyclic(IGroup* g){
	if(!IsGroup(g))
		return false;
	int N=g->size();
	for(int i=0;i<N;i++){
		vector<int> S1=Order(g,i);
		int ord=S1.size();
		if(ord==N){
			return true;
		}
	}
	return false;
}

bool IsAbelian(IGroup* g){
	if(!IsGroup(g))
		return false;
	int N=g->size();
	// 乘法交换性
	for(int i=0;i<N;i++){
		for(int j=i+1;j<N;j++){
			int ij=g->mul(i,j);
			int ji=g->mul(j,i);
			if(ij==ji)
				continue;
			else
				return false;
		}
	}
	return true;
}

// 降中心列：G,G'=[G,G],G''=[G',G],……
vector<int> commutatorOfGn(IGroup* g,const vector<int> &vGn){
	vector<int> ret;
	int M=vGn.size();
	int N=g->size();
	for(int k=0;k<M;k++){
		int i=vGn[k];
		int I=g->inv(i);
		for(int j=0;j<N;j++){
			int ij=g->mul(i,j);
			int J=g->inv(j);
			int IJ=g->mul(I,J);
			int ijIJ=g->mul(ij,IJ);
			ret.push_back(ijIJ);
		}
	}
	sort(ret.begin(),ret.end());
	ret.erase(unique(ret.begin(),ret.end()),ret.end());
	return ret;
}

int IsNilpotentGroup(IGroup* g){
	if(!IsGroup(g))
		return false;
	int N=g->size();
	if(N==1)
		return true;
	vector<int> vGn;
	for(int i=0;i<N;i++){
		vGn.push_back(i);
	}
	for(int i=0;i<N;i++){
		int n=vGn.size();
		vGn=commutatorOfGn(g,vGn);
		int n1=vGn.size();
		if(n1==1)
			return true;
		if(n1>1 && n==n1)
			return false;
	}
	return false;
}

int Inv(const vector<vector<int>> &vvG,int j){
	int N=vvG.size();
	for(int i=0;i<N;i++)
	{
		int ij=vvG[i][j];
		if(ij==0)
			return i;
	}
	return -1;
}

vector<int> commutatorOfG(const vector<vector<int>> &vvG){
	vector<int> ret;
	int N=vvG.size();
	for(int i=0;i<N;i++)
	{
		int I=Inv(vvG,i);
		for(int j=0;j<N;j++)
		{
			int ij=vvG[i][j];
			int J=Inv(vvG,j);
			int IJ=vvG[I][J];
			int ijIJ=vvG[ij][IJ];
			ret.push_back(ijIJ);
		}
	}
	sort(ret.begin(),ret.end());
	ret.erase(unique(ret.begin(),ret.end()),ret.end());
	return ret;
}

vector<vector<int> > getTable(IGroup *g,int delt)
{
	vector<vector<int> > vv(g->size(),vector<int>(g->size()));
	for(int i=0;i<g->size();i++)
		for(int j=0;j<g->size();j++)
		{
			vv[i][j]=g->mul(i,j)+delt;	
		}
	return vv;
}

vector<int> commutatorOfG(IGroup* g){
	vector<int> ret;
	int N=g->size();
	for(int i=0;i<N;i++){
		int I=g->inv(i);
		for(int j=0;j<N;j++){
			int ij=g->mul(i,j);
			int J=g->inv(j);
			int IJ=g->mul(I,J);
			int ijIJ=g->mul(ij,IJ);
			ret.push_back(ijIJ);
		}
	}
	sort(ret.begin(),ret.end());
	ret.erase(unique(ret.begin(),ret.end()),ret.end());
	return ret;
}

int ToNormal(const vector<int> &v,int a){
	for(int i=0;i<v.size();i++)
	{
		if(v[i]==a)
			return i;
	}
	return -1;
}

// 群G的换位子群或导群G'
vector<vector<int> > DG(const vector<vector<int>> &vvG){
	vector<int> DA=commutatorOfG(vvG);
	int n=DA.size();
	vector<vector<int> > ret(n);
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			int ij=vvG[DA[i]][DA[j]];
			int ij2=ToNormal(DA,ij);
			ret[i].push_back(ij2);
		}
	}
	return ret;
}

int IsSolvableGroup(IGroup* g){
	if(!IsGroup(g))
		return false;
	int N=g->size();
	vector<vector<int> > vvG=getTable(g,0);
	for(int i=0;i<N;i++)
	{
		int n=vvG.size();
		vvG=DG(vvG);
		int n1=vvG.size();
		if(n1==1)
			return true;
		if(n1>1 && n==n1)
			return false;
	}
	return true;
}

bool IsInCenterOfG(IGroup* g,int j){
	int N=g->size();
	for(int i=0;i<N;i++){
		int ij=g->mul(i,j);
		int ji=g->mul(j,i);
		if(ij==ji)
			continue;
		else
			return false;
	}
	return true;
}

vector<int> CenterOfG(IGroup* g){
	vector<int> ret;
	int N=g->size();
	for(int i=0;i<N;i++){
		if(IsInCenterOfG(g,i))
			ret.push_back(i);
		else
			continue;
	}
	return ret;
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

int lcm(const vector<int>& v)
{
 int n=v.size();
 if(n<1)
  return 0;
 if(n==1)
  return v[0];
 int ret=v[0];
 for(int i=1;i<n;i++)
 {
  ret=(ret*v[i]/gcd(ret,v[i]));
 }
 return ret;
}

int Exponent(IGroup* g)
{
 vector<vector<int>> A=getTable(g,1);
 vector<int> v;
 int n=A.size();
 vector<int> vOrders=Factors(n);
 vector<int> vCounts(n+1);
 for(int i=0;i<n;i++)
 {
  int ord=getGnEOrder(A,i);
  vCounts[ord]=vCounts[ord]+1;
 }
 for(int i=0;i<vOrders.size();i++)
 {
  if(vCounts[vOrders[i]]!=0)
	  v.push_back(vOrders[i]);
 }
 int n0=lcm(v);
 return n0;
}

// 定义：给定群G，对G中的两个元a，b，若存在G中的某个元g，使得g^-1ag=b，就称元素a，b在G中共轭，记为a~b。
bool IsConjugacy(IGroup* g,int a,int b){
    int N=g->size();
    for(int i=0;i<N;i++){
        int ai=g->mul(a,i);
        int ib=g->mul(i,b);
        if(ai==ib)
            return true;
    }
    return false;
}

vector<int> ConjugacyClasses(IGroup* g,int a){
    vector<int> ret;
    int N=g->size();
    for(int i=0;i<N;i++){
        bool bCon=IsConjugacy(g,a,i);
        if(bCon)
            ret.push_back(i);
    }
    return ret;
}

vector<vector<int> > ConjugacyClasses(IGroup* g){
    vector<vector<int> > vv;
    int N=g->size();
    for(int i=0;i<N;i++){
        vector<int> v=ConjugacyClasses(g,i);
        vv.push_back(v);
    }
    sort(vv.begin(),vv.end());
    vv.erase(unique(vv.begin(),vv.end()),vv.end());
    return vv;
}

// G表示有限群，k=Con(G)表示G的共轭类个数
int calk(IGroup* g)
{
    if(!IsGroup(g))
        return 0;
    vector<vector<int> > vv=ConjugacyClasses(g);
    int k=vv.size();
    return k;
}

// G表示有限群，K表示G的共轭类长度的分布
string calK(IGroup* g)
{
    if(!IsGroup(g))
        return "";
    vector<vector<int> > vv=ConjugacyClasses(g);
    int k=vv.size();
    int n=g->size();
	vector<int> vOrders=Factors(n);
	vector<int> vCounts(n+1);
 	for(int i=0;i<k;i++)
	{
		int ord=vv[i].size();
		vCounts[ord]=vCounts[ord]+1;
	}
	string strK="";
	for(int i=0;i<vOrders.size();i++)
	{
		char sz[200]={0};
		sprintf(sz,"%d,",vCounts[vOrders[i]]);
		strK+=sz;
	}
	if(strK.size()>2)
	{
		strK=strK.substr(0,strK.size()-1);
	}
	return strK;
}

string calckKEZDCANS(IGroup* g){
    int k=calk(g);
	string K=calK(g);
	int E=Exponent(g);
	vector<int> vZ=CenterOfG(g);
	int Z=vZ.size();
	vector<int> vD=commutatorOfG(g);
	int D=vD.size();
	bool bC=IsCyclic(g);
	bool bA=IsAbelian(g);
	bool bN=IsNilpotentGroup(g);
	bool bS=IsSolvableGroup(g);
	char sz[1024]={0};
	sprintf(sz,"%d,[%s],%d,%d,%d,%d,%d,%d,%d",k,K.c_str(),E,Z,D,bC,bA,bN,bS);	
	return sz;
}

// N0C1NkkKEZDCANS
string calcGroupInvariant(IGroup* g){
	char sz[1024]={0};
	string N0=calcN0(g);  
	string C1=calcC1(g);	
	string Nk=calcNk(g);	
	//string S2=calcS2(g);
	string kKEZDCANS=calckKEZDCANS(g); 
	sprintf(sz,"%s,%s,%s;%s",N0.c_str(),C1.c_str(),Nk.c_str(),kKEZDCANS.c_str());
	//sprintf(sz,"%s,%s,%s;%s;%s",N0.c_str(),C1.c_str(),Nk.c_str(),S2.c_str(),kKEZDCANS.c_str());  
	return sz;
}

//-----------------------------End of calckKEZDCANS---------------------------------

int IdGroup(IGroup* g){
   int n=g->size();
   if(n<=1)
	  return 1;  
   string strN0=calcN0(g);
   string strC1=calcC1(g);  
   string strNk=calcNk(g);
   string strC1Nk=strC1+","+strNk;   
   IDHelper idHelper;
   vector<int> vID=idHelper.IDFromN0C1Nk(strN0,strC1Nk);
   if(vID.size()<=0)
	   return -1;//没有N0数据
   if(vID.size()>1){ // 
       string strS2=calcS2(g);
	   vector<int> vID2=idHelper.IDFromS2(strS2);
	   vector<int> vID02;
	   set_intersection(vID.begin(),vID.end(),vID2.begin(),vID2.end(),back_inserter(vID02));
	   if(vID02.size()>1){
		   string strkKEZDCANS=calckKEZDCANS(g);
		   vector<int> vID3=idHelper.IDFromkKEZDCANS(strkKEZDCANS);
		   vector<int> vID023;
		   set_intersection(vID02.begin(),vID02.end(),vID3.begin(),vID3.end(),back_inserter(vID023));		   
		   if(vID023.size()>1){
		#if 1
			if(vID023.size()>1){
				 printf("[");
				 for(int i=0;i<vID023.size();i++){
					 printf("%d,",vID023[i]);
				 }
				 printf("]\n");
			}
		#endif
				return 0;//ID不确定，还需要N0、S2、kKEZDCANS以外的群不变量确定编号
		   }
		   else if(vID023.size()<=0){
		#if 1
				 printf("[");
				 for(int i=0;i<vID02.size();i++){
					 printf("%d,",vID02[i]);
				 }
				 printf("]\n");
		#endif
			   printf("出错了，群不变量数据kKEZDCANS有误！\n");
			   return 0;
		   }
		   return vID023[0];
		}
		else if(vID02.size()<=0){
		#if 1
				 printf("[");
				 for(int i=0;i<vID.size();i++){
					 printf("%d,",vID[i]);
				 }
				 printf("]\n");
		#endif
			printf("出错了，群不变量数据S2有误！\n");
			return 0;
		}
	   return vID02[0];
   }   
   return vID[0];   
}

//-----------------------------End of IdGroup-------------------------------

void printSubGroup(IGroup* g,vector<int>& S){
   int n=S.size();
   printf("%d阶子群[",n);
   for(int i=0;i<n;i++){
		printf("%d",S[i]);
		if(i<n-1){
			printf(",");			
		}
   }
   printf("]:\n");	
    for(int i=0;i<n;i++){
	   for(int j=0;j<n;j++){
		int ij=g->mul(S[i],S[j]);
		vector<int>::iterator p=std::find(S.begin(),S.end(),ij);
		int IJ=-1;
		if(p!=S.end()){
			IJ=p-S.begin();
		}		  
		printf("%d ",IJ+1);
	   }
       printf("\n");
   }
}

int getidx(vector<vector<float> > &Arr2,vector<float> &Arr1){
	int n=Arr2.size();
	int m=Arr1.size();	
	for(int i=0;i<n;i++){
		int ni=Arr2[i].size();
		if(ni!=m)
			return -1;
		bool isE=true;
		for(int j=0;j<m;j++){
			if(abs(Arr2[i][j]-Arr1[j])>0.01){
				isE=false;
				break;
			}
		}
		if(isE)
			return i;
	}
	return -1;	
}

int getidx(int *Arr2,int n,int m,int *Arr1){
	for(int i=0;i<n;i++){
		if(memcmp(Arr2+m*i,Arr1,m*sizeof(int))==0){
			return i;	
		}
	}
	return -1;	
}

int getidx(vector<vector<int> > &Arr2,vector<int> &Arr1){
	int n=Arr2.size();
	int m=Arr1.size();	
	for(int i=0;i<n;i++){
		int ni=Arr2[i].size();
		if(ni!=m)
			return -1;
		if(memcmp(&Arr2[i][0],&Arr1[0],m*sizeof(int))==0){
			return i;	
		}
	}
	return -1;	
}

#endif