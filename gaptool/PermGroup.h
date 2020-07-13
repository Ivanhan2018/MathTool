#ifndef PERMGROUP_H
#define PERMGROUP_H

#include"IGroup.h"

typedef unsigned char TSnE;
typedef vector<TSnE> SnE;

// Sn
struct Sn:public IGroup
{
public:
   //  ��̬����
   static vector<SnE> FG(const vector<SnE> & gen);  
   static SnE mul(const SnE &t,const SnE &m);  
   static SnE inv(const SnE &t);
   static vector<SnE> Order(const SnE & m); 
   static SnE getSnE(int n,int idx);   
public:
   // ʵ�ֳ������ķ���
   virtual void printSet();
   virtual void printTable();     
   virtual int mul(int a,int b);
   virtual int size();
   virtual int inv(int a);
   // ���캯��
   Sn(){};
   Sn(int n);
   Sn(int n,TSnE* a,TSnE* b);   
   // ��Ա����
   void init(const vector<SnE> & gen);
   bool init(const vector<SnE> & gen,int N);
   // ��Ա����
   vector<SnE> s_Arr; 
   vector<SnE> m_gen;
   int m_n;   
};

vector<SnE> Sn::FG(const vector<SnE> & gen)
{
	int N=gen[0].size();
	vector<SnE> S;
	SnE E;
	for(int i=0;i<N;i++)
	{
		E.push_back(i+1);
	}
	S.push_back(E);
	for(int i=0;i<gen.size();i++)
	{
		if(memcmp(&E[0],&gen[i][0],sizeof(TSnE)*N)!=0)
		{
			S.push_back(gen[i]);
		}
	}	
	int R=S.size();
	int cnt=R;
	int cnt1=R;
	do{
		cnt=S.size();
		for(int i=1;i<cnt;i++)
		{
			for(int j=1;j<cnt;j++)
			{
				SnE IJ=mul(S[i],S[j]);
				int bIn=getidx(S,IJ);
				if(bIn==-1)
				{
					S.push_back(IJ);
				}
			}
		}
		cnt1=S.size();
	}while(cnt1>cnt);	
	return S;
}

Sn::Sn(int n)
{
   m_n=n;
   SnE v(n);
   for(int i=0;i<n;i++)
		v[i]=i+1;
   do { 
     s_Arr.push_back(v);    
   }while( next_permutation(v.begin(),v.end()));
}

SnE Sn::getSnE(int n,int idx)
{
	SnE v(n);
	for(int i=0;i<n;i++)
		v[i]=i+1;
	if(idx<=0){
		return v;
	}
	int cnt=-1;
	do { 
		cnt++;
		if(cnt==idx)
			return v; 		 
	}while( next_permutation(v.begin(),v.end()));	
	return v;
}

Sn::Sn(int n,TSnE* a,TSnE* b)
{
	SnE sa(a,a+n);
	SnE sb(b,b+n);	
	m_gen.push_back(sa);
	m_gen.push_back(sb);
	s_Arr=FG(m_gen);
	m_n=n;
}

void Sn::init(const vector<SnE> & gen)
{	
	m_gen=gen;
	s_Arr=FG(m_gen);
	m_n=gen[0].size();
}

bool Sn::init(const vector<SnE> & gen,int N)
{
	m_gen=gen;
	m_n=gen[0].size();
	int n=gen[0].size();
	SnE E;
	for(int i=0;i<n;i++)
	{
		E.push_back(i+1);
	}
	s_Arr.push_back(E);
	for(int i=0;i<gen.size();i++)
	{
		if(memcmp(&E[0],&gen[i][0],sizeof(TSnE)*n)!=0)
		{
			s_Arr.push_back(gen[i]);
		}
	}	
	int R=s_Arr.size();
	int cnt=R;
	int cnt1=R;
	do{
		cnt=s_Arr.size();
		if(N>0 && cnt>N)
			return false;
		for(int i=1;i<cnt;i++)
		{
			for(int j=1;j<cnt;j++)
			{
				SnE IJ=mul(s_Arr[i],s_Arr[j]);
				int bIn=getidx(s_Arr,IJ);
				if(bIn==-1)
				{
					s_Arr.push_back(IJ);
				}
			}
		}
		cnt1=s_Arr.size();
	}while(cnt1>cnt);	
	return true;
}

void Sn::printSet()
{	
   int ID=IdGroup(this);
   printf("GAP[%d,%d]:\n",size(),ID);
#ifdef PRINT_SET   
   if(size()<73)
   {
		for(int i=0;i<size();i++){
			int i1=inv(i);
			printf("%d=>%d\n",i,i1);
		}	   
   }
   else
   {
	   printf("Ⱥ�Ľ�̫�󣬲��ڿ���̨��ӡ\n");
   }
#endif
}

void Sn::printTable()
{
   //if(size()<73)
		//printGroup(this);
}

int Sn::mul(int a,int b)
{
	SnE A=s_Arr[a];
	SnE B=s_Arr[b];
	SnE C=mul(A,B);
	int idx=getidx(s_Arr,C);
	return idx;
}

int Sn::inv(int a)
{
	SnE v=inv(s_Arr[a]);
	int idx=getidx(s_Arr,v);
	return idx;
}

int Sn::size()
{
	return s_Arr.size();	
}

SnE Sn::mul(const SnE &t,const SnE &m)
{
	int n=t.size();
	int nm=m.size();	
	SnE ret(n);
	if(n!=nm)
		return ret;
	for(int i=0;i<n;i++)
		ret[i]=m[t[i]-1];
	return ret;
}

SnE Sn::inv(const SnE &t)
{
	vector<SnE> S1=Order(t);
	int ord=S1.size();	
	return S1[ord-1];	
}

vector<SnE> Sn::Order(const SnE & m)
{
	vector<SnE> ret;
    int n=m.size();
	SnE mi=m;
	SnE m0(n);
	for(int i=0;i<n;i++)
	{
		m0[i]=i+1;
	}
	ret.push_back(m0);
	while(memcmp(&mi[0],&m0[0],sizeof(TSnE)*n)!=0)
	{
		ret.push_back(mi);
		mi=mul(mi,m);
	}	
	return ret;
}

#endif