#ifndef FiniteGroup_H
#define FiniteGroup_H

#include "IGroup.h"

// ֱ�Ӵӿ�������һ������Ⱥ
struct FiniteGroup:public IGroup
{
public:
	//  ��̬����  
public:
	// ʵ�ֳ������ķ���
	virtual void printSet();
	virtual void printTable();     
	virtual int mul(int a,int b);
	virtual int size();
	virtual int inv(int a);
	// ���캯��
	FiniteGroup(int n,int* arr,int delt);
	// ��������
	~FiniteGroup();
	// ��Ա����
	vector<int> Order(int m);  
	// ��Ա����  
	int m_n; 
	int* m_Arr;
	int m_delt;
	int m_flag;// FiniteGroup��������ʱ�Ƿ��ͷ�m_Arrָ����ڴ�   
};

FiniteGroup::FiniteGroup(int n,int* arr,int delt)
{
	m_n=n;
	m_Arr=arr;
	m_delt=delt;
	m_flag=0;	
}

FiniteGroup::~FiniteGroup(){
	if(m_flag==1 && m_Arr!=NULL){
		delete[] m_Arr;
		m_Arr=NULL;
	}	
}

void FiniteGroup::printSet()
{
/*
	int ID=IdGroup(this);	
	printf("GAP[%d,%d]:\n",m_n,ID);
	for(int i=0;i<m_n;i++){
		int i1=inv(i);
		printf("%d->%d\n",i,i1);
	}
	*/
}

void FiniteGroup::printTable()
{
	int ID=IdGroup(this);
	string N0=calcN0(this);  
	string C1=calcC1(this);	
	string Nk=calcNk(this);	
	string S2=calcS2(this);
	string kKEZDCANS=calckKEZDCANS(this);
	printf("GAP[%d,%d]:\n",m_n,ID); 
	printf("N0C1Nk=%s,%s,%s\n",N0.c_str(),C1.c_str(),Nk.c_str());  
	printf("S2=%s\n",S2.c_str());
	printf("kKEZDCANS=%s\n",kKEZDCANS.c_str());
	string I1=calcI1(this);
	string I2=calcI2(this);   
	printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());   
	//printGroup(this);
}

int FiniteGroup::mul(int a,int b)
{
	int c=*(m_Arr+a*m_n+b);
	return c-m_delt;
}

int FiniteGroup::inv(int a)
{
	vector<int> S1=Order(a);
	int ord=S1.size();
	int v=S1[ord-1];	
	return v;
}

int FiniteGroup::size()
{
	return m_n;
}

vector<int> FiniteGroup::Order(int m)
{
	vector<int> ret;
	int mi=m;
	int m0=0;
    ret.push_back(m0);
	while(mi!=m0){
		ret.push_back(mi);
		mi=mul(mi,m);
	}
	return ret;
}

#endif
