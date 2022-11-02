#include"IRing.h"
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
	int ID=IdRing(this);
	string str=calcRingInvariant(this);
	string Idem=V2S(calIdem(this));
	string N9=V2S(calN9(this));	
	printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6N5IdemN9=%s,%s,%s\n",size(),ID,str.c_str(),Idem.c_str(),N9.c_str());
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