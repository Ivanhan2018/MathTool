#include"DecompositionRing.h"
//#include"Rn.h"
#include"M2r.h"
#include"Mnr.h"
#include"PolynomialRing.h"

// n2�����ݵ�Ԫ����
// Idem����r���ݵ�Ԫ���ϣ�N2�Ѵ�������һ������������
vector<int> calIdem(IRing* r){
    int n=r->size();
	vector<int> Idem;
	for(int i2=0;i2<n;i2++){
		if(IsIdempotent(r,i2))
			Idem.push_back(i2);
	}
	return Idem;
}

// n4,n5����2������Ԫ������2~3������Ԫ����
// n9=N9.size()��������Ԫ����
int IsNil(IRing* r,int i){
    int n=r->size();
	int iRet=i;
	if(iRet==0)
		return 1;// i=0�ǻ�r��1������Ԫ
	for(int k=0;k<n;k++){// ���n+1������Ԫ
		iRet=r->mul(iRet,i);
		if(iRet==0)
			return k+2;// i�ǻ�r��k+2������Ԫ
	}
	return 0;// i���ǻ�r������Ԫ
}

// N9����r������Ԫ����,�������N9����[IdRing(N9),IdRing(r/N9)]��[IdRing(N9)]
vector<int> calN9(IRing* r){
    int n=r->size();
	vector<int> N9;
	for(int i9=0;i9<n;i9++){
		if(IsNil(r,i9)>0)
			N9.push_back(i9);
	}
	return N9;
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

// �жϼ���I�Ƿ��ǻ�r������
int IsIdeal(IRing* r,vector<int> &I){
 //I��r���ӻ�
 Subring s(r,I);
 bool bE=IsEqual(I,s.m_Set);
  if(!bE){
   return 0;// �ӻ�Ҳ����
  }
 //��һ���ж��Ƿ�������
 for(int i=0;i<r->size();i++){//���ⴿ����Ԫ��c
  for(int j=0;j<I.size();j++){//��������ģԪ��a
   int ca=r->mul(i,I[j]);
   int ac=r->mul(I[j],i);
	vector<int>::iterator p=std::find(I.begin(),I.end(),ca);
	if(p==I.end()){
		return 2;// ���ӻ�����������
	}   
	vector<int>::iterator p1=std::find(I.begin(),I.end(),ac);
	if(p1==I.end()){
		return 2;// ���ӻ�����������
	}
  }
 }
 return 1;//������
}

const char* IsIdealRetInfo(int iret){
	const char* sz[]={"�����ɻ�","��������","���ɷ������ӻ�"};
	return sz[iret];
}

// �̻�R/I
struct quotientRing:public IRing
{
public:
	//  ��̬����  
	static vector<vector<int> > quotientGN(const vector<vector<int> > &vvG,const vector<int> &vN);
	static vector<vector<int> > quotientTable(const vector<vector<int> > &vvG,const vector<vector<int> > &vvH,const vector<int> &vN);
public:
	// ʵ�ֳ������ķ���
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// ���캯��
	quotientRing();
	quotientRing(IRing *r1,vector<int> &I);
	// ��������
	~quotientRing();
	// ��Ա���� 
	template<class T> vector<vector<int> > getTable(T *obp,int n,int(T::*p)(int,int));	
	// ��Ա����  
	IRing *m_r1;
	vector<int> m_I;	
	vector<vector<int> > m_Add;
	vector<vector<int> > m_Mul;
	int m_flag;// quotientRing��������ʱ�Ƿ��ͷ�m_r1ָ����ڴ�
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
			//�ж�I�е�Ԫ���Ƿ���ret��
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
	printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2IdemN9=%s,%s,%s\n",size(),ID,str.c_str(),Idem.c_str(),N9.c_str());
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

void print(IRing* zn[],int cnt)
{
	for(int i=0;i<cnt;i++)
	{	
		zn[i]->printTable();
		vector<int> vIdem=calIdem(zn[i]);
		string strIdem=V2S(vIdem);		
		vector<int> vN9=calN9(zn[i]);
		//string strN9=V2S(vN9);
		int iret1=IsIdeal(zn[i],vIdem); 
		int iret2=IsIdeal(zn[i],vN9);	
 		Subring Idem(zn[i],vN9);
		//Idem.printTable();      		
		Subring N9(zn[i],vN9);
		//N9.printTable(); 	
		quotientRing znmodN9(zn[i],vN9);
		//znmodN9.printTable(); 
        char szN9[1024]={0};
		sprintf(szN9,"[R%d_%d,R%d_%d]",N9.size(),IdRing(&N9),znmodN9.size(),IdRing(&znmodN9));		
		printf("IdemN9=%s,%s\n",strIdem.c_str(),szN9);
		printf("�ݵ�Ԫ����%s,����Ԫ����%s\n",IsIdealRetInfo(iret1),IsIdealRetInfo(iret2));
	}	
}

// ������Ԫ�����У�����Ԫ��������Ԫ��
// ����Ԫһ����������(n9<=n7+1)�����ڲ�������Ԫ�������ӣ�����Z/6Z��~2��~3��~4��
// n7(Z/24Z)=24-��(24)-1=24-8-1=15
// Z/nZ��������Ԫ���ҽ���n��ƽ�����ӡ�N9(Z/6Z)=[0],N7(Z/6Z)=[2,3,4],N9(Z/8Z)=[0,2,4,6],N9(Z/9Z)=[0,3,6]
// ������������R�ϵ�����һ������A����A^2=A����R���������붼Ϊ�ݵ����롣
// Z/2Z��һ��2�׵�������������������
// �������뻷����R�ϵ�����һ������A����A^n=0����R���������붼Ϊ�������롣
// R4_2=2Z/8Z=N9(Z/8Z)=[0,2,4,6]��һ��4�׵�����Ԫ�����������뻷��
void test1()
{
	ZmodnZ zn[]={ZmodnZ(1,6),ZmodnZ(1,8),ZmodnZ(1,9),ZmodnZ(1,12),ZmodnZ(1,24)};
	int cnt=sizeof(zn)/sizeof(zn[0]);
	vector<IRing*> v(cnt);
	for(int i=0;i<cnt;i++)
	{
		v[i]=&zn[i];
	}	
	print(&v[0],cnt);	
}

void test2()
{
    ZmodnZ r4_3(1,4);
    ZmodnZ r8_3(1,8);	
	int f1[] = {1,0,1};
	Polynomial vf1(f1,f1+sizeof(f1)/sizeof(f1[0]));
	int f2[] = {1,1,1};
	Polynomial vf2(f2,f2+sizeof(f2)/sizeof(f2[0]));
	int f3[] = {0,0,1};
	Polynomial vf3(f3,f3+sizeof(f3)/sizeof(f3[0]));
	int f4[] = {0,1,1};
	Polynomial vf4(f4,f4+sizeof(f4)/sizeof(f4[0]));
	int f5[] = {1,0,0,1};
	Polynomial vf5(f5,f5+sizeof(f5)/sizeof(f5[0]));
	int f6[] = {1,1,0,1};
	Polynomial vf6(f6,f6+sizeof(f6)/sizeof(f6[0]));
	int f7[] = {0,1,0,1};
	Polynomial vf7(f7,f7+sizeof(f7)/sizeof(f7[0]));	
	int f8[] = {0,0,1,1};
	Polynomial vf8(f8,f8+sizeof(f8)/sizeof(f8[0]));		
	int f9[] = {1,0,1,1};
	Polynomial vf9(f9,f9+sizeof(f9)/sizeof(f9[0]));
	int f10[] = {0,1,1,1};
	Polynomial vf10(f10,f10+sizeof(f10)/sizeof(f10[0]));	
	int f11[] = {1,1,1,1};
	Polynomial vf11(f11,f11+sizeof(f11)/sizeof(f11[0]));	
	int f12[] = {0,0,0,1};
	Polynomial vf12(f12,f12+sizeof(f12)/sizeof(f12[0]));	
	int f13[] = {2,1,1};
	Polynomial vf13(f13,f3+sizeof(f13)/sizeof(f13[0]));	
	int f14[] = {0,0,1};
	Polynomial vf14(f14,f14+sizeof(f14)/sizeof(f14[0]));	
	int g1[] = {0,2};
	int g2[] = {2};	
	int g3[] = {4,6};
	int g4[] = {4,7};
	int g5[] = {6,1};
	int g6[] = {6,3};	
	Polynomial vg1(g1,g1+sizeof(g1)/sizeof(g1[0]));
	Polynomial vg2(g2,g2+sizeof(g2)/sizeof(g2[0]));	
	Polynomial vg3(g3,g3+sizeof(g3)/sizeof(g3[0]));
	Polynomial vg4(g4,g4+sizeof(g4)/sizeof(g4[0]));	
	Polynomial vg5(g5,g5+sizeof(g5)/sizeof(g5[0]));
	Polynomial vg6(g6,g6+sizeof(g6)/sizeof(g6[0]));		
	PolynomialRing r16_118;
	r16_118.initFR(&r8_3,vg1,vg2,vf1);	
	PolynomialRing r64(&r8_3,vf2);
	PolynomialRing r16_119;
	r16_119.initFR(&r8_3,vg1,vg2,vf2);
	PolynomialRing r16_16;
	r16_16.initFR(&r8_3,vg3,vg4,vf3);
	PolynomialRing r16_17;
	r16_17.initFR(&r8_3,vg5,vg6,vf3);		
	IRing* zn[]={&r16_16,&r16_17,&r16_118,&r16_119,&r64};
	int cnt=sizeof(zn)/sizeof(zn[0]);
	print(zn,cnt);
}

void test3()
{
	M2r r4_7;
	r4_7.initG(2);
	M2r r4_8;
	r4_8.initH(2);
	IRing* zn[]={&r4_7,&r4_8};
	int cnt=sizeof(zn)/sizeof(zn[0]);
	print(zn,cnt);	
}

int main()
{
	test1();
	test2();
	test3();
	
	return 0;
}