#ifndef QUOTIENTGROUP_H
#define QUOTIENTGROUP_H

#include"IGroup.h"

// 商群G/N
struct quotientGroup:public IGroup
{
public:
	//  静态函数  
	static vector<vector<int> > quotientGN(const vector<vector<int> > &vvG,const vector<int> &vN);
	static vector<vector<int> > quotientTable(const vector<vector<int> > &vvG,const vector<vector<int> > &vvH,const vector<int> &vN);
public:
	// 实现抽象基类的方法
	virtual void printSet();
	virtual void printTable();     
	virtual int mul(int a,int b);
	virtual int size();
	virtual int inv(int a);
	// 构造函数
	quotientGroup();
	quotientGroup(IGroup *g,vector<int> &N);
	// 析构函数
	~quotientGroup();
	// 成员函数 
	template<class T> vector<vector<int> > getTable(T *obp,int n,int(T::*p)(int,int));	
	// 成员变量  
	IGroup *m_g;
	vector<int> m_N;	
	vector<vector<int> > m_Mul;
	int m_flag;// quotientGroup对象析构时是否释放m_r1指向的内存
};

quotientGroup::quotientGroup(){
	m_g=NULL;
	m_flag=0;	
}

quotientGroup::quotientGroup(IGroup *g,vector<int> &N)
{
    m_g=g;
	m_N=N;
	vector<vector<int> > Mul1=getTable(g,g->size(),&IGroup::mul);
	vector<vector<int> > H=quotientGN(Mul1,N);
	m_Mul=quotientTable(Mul1,H,N);
}

quotientGroup::~quotientGroup(){
	if(m_flag==1 && m_g!=NULL){
		delete m_g;
		m_g=NULL;
	}
}

template<class T>
vector<vector<int> > quotientGroup::getTable(T *obp,int n,int(T::*p)(int,int))
{
	vector<vector<int> > vv(n,vector<int>(n));
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
		{
			vv[i][j]=(obp->*p)(i,j);	
		}
	return vv;
}

vector<vector<int> > quotientGroup::quotientGN(const vector<vector<int> > &vvG,const vector<int> &vN)
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

vector<vector<int> > quotientGroup::quotientTable(const vector<vector<int> > &vvG,const vector<vector<int> > &vvH,const vector<int> &vN)
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

void quotientGroup::printSet()
{

}

void quotientGroup::printTable()
{
	int ID=IdGroup(this);
	printf("GAP[%d_%d]:\n",size(),ID);
	printGroup(this);
}

int quotientGroup::mul(int a,int b)
{
	return m_Mul[a][b];	
}

int quotientGroup::size()
{
	return m_Mul.size();
}

int quotientGroup::inv(int a)
{
	vector<int> S1=Order(this,a);
	int ord=S1.size();
	int v=S1[ord-1];	
	return v;
}

#endif