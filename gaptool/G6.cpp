#include"IGroup.h"
#include <iostream>
#include <functional>
using namespace std;

struct I2{
	int a,b;
};

// G6
struct G6:public IGroup
{
public:
   //  ��̬����
   static int I2ToInt(I2 a);
   static I2 mulInD6(I2 a,I2 b);   
   static I2 mulInC6(I2 a,I2 b);
   static int InvInC3(int a);
public:
   // ʵ�ֳ������ķ���
   virtual void printSet();
   virtual void printTable();     
   virtual int mul(int a,int b);
   virtual int size();
   virtual int inv(int a);
   // ���캯��
   G6(int ID);// Ĭ�Ϲ��캯��
   // ��Ա����
   // ��Ա����
   static I2 g_I2[6];
   int m_ID;
};

G6::G6(int ID){
	m_ID=ID;
}

int G6::mul(int a,int b){
	function<I2(I2,I2)> mulArr[]={mulInD6,mulInC6};
	int idx=m_ID-1;
	I2 cc=mulArr[idx](g_I2[a],g_I2[b]);
	int c=I2ToInt(cc);
	return c;
}

void G6::printSet(){
	int ID=IdGroup(this);
	printf("GAP[%d,%d]:\n",size(),ID);
	for(int i=0;i<size();i++){
		int i1=inv(i);
		printf("%d=>%d\n",i,i1);
	}
}

void G6::printTable(){
	printGroup(this);
}

int G6::inv(int a){
	vector<int> S1=Order(this,a);
	int ord=S1.size();	
	return S1[ord-1];
}

int G6::size(){
	return 6;	
}

// (I,J)�ı��Ϊn*(I-1)+(J-1)+1������1<=I<=m��1<=J<=n
// Ⱥ̬ͬ��:C_2->Aut(C_3)=C_2��3��ȺI,r,rr,2��ȺI,f
// ƽ��̬ͬ��1:I->I,r,rr,f->I,r,rr��Ӧֱ��
// ��ƽ��̬ͬ��2:I->I,r,rr,f->I,rr,r��Ӧ��ֱ��
I2 G6::g_I2[6]={
	{1,1},
	{1,2},
	{2,1},
	{2,2},
	{3,1},
	{3,2},
};

int G6::I2ToInt(I2 a){
	for(int i=0;i<6;i++){
		if(g_I2[i].a==a.a && g_I2[i].b==a.b)
			return i;
	}
	return -1;
}

// ֱ��ȺC_6
// ֱ���ĳ˷���Ϊ��(x1,y1)��(x2,y2)=(x1*x2,y1+y2)����Ȼ��Ԫ��(1,1)=1
I2 G6::mulInC6(I2 a,I2 b){
	I2 ret={(a.a+b.a-2)%3+1,(a.b+b.b-2)%2+1};
	return ret;
}

// ��2n��Ԫ�صĶ�����ȺD_nͬ����ѭ��ȺC_n��C_2�İ�ֱ����
// ���C_2�ķǵ�λԪ������C_n����Ԫ�ر�����棻����һ����ͬ������ΪC_n�ǽ���Ⱥ��
// ��ֱ��ȺC_3:C_2
// ��ֱ���ĳ˷���Ϊ��(x1,y1):(x2,y2)=(x1*(��(y1)(x2)),y1+y2)����Ȼ��Ԫ��(1,1)=1
// �����(y1)��ȺN��һ����ͬ����(��(y1)(x2))��ȺN�е�һ��ȺԪ������Ϊƽ��̬ͬʱ��(��(y1)(x2))=x2
I2 G6::mulInD6(I2 a,I2 b){
	I2 ret={(a.a+b.a-2)%3+1,(a.b+b.b-2)%2+1};
	/*
		(x1,y1):(x2,y2)
		=(x1*(��(y1)(x2)),y1+y2)=(x1*((x2)^-1),y1+y2),y1=2ʱ��
		=(x1*(��(y1)(x2)),y1+y2)=(x1*x2,y1+y2),y1=1ʱ��
	*/
	if(a.b==2){
		ret.a=(a.a+InvInC3(b.a)-2)%3+1;
	}
	return ret;
}

int G6::InvInC3(int a){
	if(a==1)
		return 1;
	if(a==2)
		return 3;
	if(a==3)
		return 2;
	return 0;
}

int main()
{
	G6 d6(1);
	d6.printSet();
	d6.printTable();
	
	G6 c6(2);
	c6.printSet();
	c6.printTable();
	
	return 0;
} 

