#ifndef DECOMPOSITIONGROUP_H
#define DECOMPOSITIONGROUP_H

// ���޿ɷֽ�Ⱥ
struct DecompositionGroup:public IGroup
{
public:
   //  ��̬����  
   static vector<vector<int> > getTable(IGroup *g);
   static vector<vector<int> > DirectProduct(const vector<vector<int>> &A,const vector<vector<int>> &B);   
public:
   // ʵ�ֳ������ķ���
   virtual void printSet();
   virtual void printTable();     
   virtual int mul(int a,int b);
   virtual int size();
   virtual int inv(int a);
   // ���캯��
   DecompositionGroup();   
   DecompositionGroup(IGroup *g1,IGroup *g2);
   // ��������
   ~DecompositionGroup();   
   // ��Ա����
   vector<int> Order(int m);  
   // ��Ա����  
   IGroup *m_g1;
   IGroup *m_g2; 
   vector<vector<int> > s_Arr;
   int m_flag;// DecompositionGroup��������ʱ�Ƿ��ͷ�m_g1��m_g2ָ����ڴ�   
};

DecompositionGroup::DecompositionGroup(){
	m_g1=NULL;
	m_g2=NULL;
	m_flag=0;	
}

DecompositionGroup::~DecompositionGroup(){
	if(m_flag==1 && m_g1!=NULL){
		delete m_g1;
		m_g1=NULL;
	}	
	if(m_flag==1 && m_g2!=NULL){
		delete m_g2;
		m_g2=NULL;
	}
}

vector<vector<int> > DecompositionGroup::getTable(IGroup *g)
{
	vector<vector<int> > vv(g->size(),vector<int>(g->size()));
	for(int i=0;i<g->size();i++)
			for(int j=0;j<g->size();j++)
			{
				vv[i][j]=g->mul(i,j);	
			}
	return vv;
}

vector<vector<int> > DecompositionGroup::DirectProduct(const vector<vector<int>> &A,const vector<vector<int>> &B)
{
	vector<vector<int> > C;
	int nA=A.size();
	int nB=B.size();
	int n=nA*nB;

	for(int a=0;a<n;a++)
	{
		vector<int> iRow;
		for(int b=0;b<n;b++)
		{
			int aj=a%nB;
			int ai=a/nB;
			int bj=b%nB;
			int bi=b/nB;
			int i=A[ai][bi];
			int j=B[aj][bj];
			int c=nB*i+j;
			iRow.push_back(c);
		}
		C.push_back(iRow);
	}
	return C;
}

DecompositionGroup::DecompositionGroup(IGroup *g1,IGroup *g2)
{
    m_g1=g1;
	m_g2=g2;
	vector<vector<int> > A=getTable(g1);
	vector<vector<int>> B=getTable(g2);	
	s_Arr=DirectProduct(A,B);
}

void DecompositionGroup::printSet()
{
   /*
   int ID=IdGroup(this);
   int ID1=IdGroup(m_g1);   
   int ID2=IdGroup(m_g2);   
   printf("GAP[%d,%d]=GAP[%d,%d]��GAP[%d,%d]:\n",size(),ID,m_g1->size(),ID1,m_g2->size(),ID2); 
	for(int i=0;i<size();i++){
		int i1=inv(i);
		printf("%d->%d\n",i,i1);
	}
	*/
}

void DecompositionGroup::printTable()
{
   int ID=IdGroup(this);
   int ID1=IdGroup(m_g1);   
   int ID2=IdGroup(m_g2);   
   printf("GAP[%d,%d]=GAP[%d,%d]��GAP[%d,%d]:\n",size(),ID,m_g1->size(),ID1,m_g2->size(),ID2);      
   //printGroup(this);
}

int DecompositionGroup::mul(int a,int b)
{
	int c=s_Arr[a][b];
	return c;
}

int DecompositionGroup::inv(int a)
{
	vector<int> S1=Order(a);
	int ord=S1.size();
	int v=S1[ord-1];	
	return v;
}

int DecompositionGroup::size()
{
	return s_Arr.size();
}

vector<int> DecompositionGroup::Order(int m)
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