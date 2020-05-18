#include"GL2Zn.h"

int g_D4[8][4]=
{
	{1,0,0,1},
	{0,1,6,0},
	{6,0,0,6},
	{0,6,1,0},
	{0,1,1,0},
	{6,0,0,1},
	{0,6,6,0},
	{1,0,0,6},
};

void test(int n)
{
    GL2Zn D8;
	D8.m_n=n;
	vector<TM2> gen;
#if 0	
	TM2 a(&g_D4[1][0],&g_D4[1][0]+4);
	TM2 b(&g_D4[4][0],&g_D4[4][0]+4);	
#else
	TM2 a(4);
	TM2 b(4);	
	a[0]=0;
	a[1]=1;
	a[2]=n-1;
	a[3]=0;
	b[0]=0;
	b[1]=1;
	b[2]=1;
	b[3]=0;		
#endif
	gen.push_back(a);
	gen.push_back(b);
	D8.s_Arr=GL2Zn::FG(gen,D8.m_n);
	D8.printSet();
	D8.printTable();	
}

//GAP[16,8]、GAP[24,8]、GAP[40,-1]、GAP[48,6]、GAP[32,19]、GAP[48,14]、GAP[48,6]
//GAP[120,-1]、GAP[40,-1]、GAP[96,-1]、GAP[56,-1]、GAP[96,-1]、
//GAP[160,-1]、GAP[96,-1]、GAP[72,-1]、GAP[48,6]、GAP[72,-1]、
void test1(int n)
{
    GL2Zn D8;
	D8.m_n=n;
	vector<TM2> gen;
	TM2 a(4);
	TM2 b(4);	
	a[0]=0;
	a[1]=1;
	a[2]=n-1;
	a[3]=0;
	b[0]=n-1;
	b[1]=n-1;
	b[2]=n-1;
	b[3]=0;		
	gen.push_back(a);
	gen.push_back(b);
	D8.s_Arr=GL2Zn::FG(gen,D8.m_n);
	D8.printSet();
	D8.printTable();	
}

//GAP[48,29]、GAP[96,-1]、GAP[240,-1]、GAP[288,-1]、GAP[672,-1]、GAP[768,-1]、GAP[1296,-1]
void test2(int n)
{
    GL2Zn D8;
	D8.m_n=n;
	vector<TM2> gen;
	TM2 a(4);
	TM2 b(4);	
	a[0]=0;
	a[1]=n-1;
	a[2]=n-1;
	a[3]=0;
	b[0]=n-1;
	b[1]=n-1;
	b[2]=n-1;
	b[3]=0;		
	gen.push_back(a);
	gen.push_back(b);
	D8.s_Arr=GL2Zn::FG(gen,D8.m_n);
	D8.printSet();
	D8.printTable();	
}

//GAP[24,3]、GAP[48,30]、GAP[120,-1]、GAP[144,-1]、GAP[336,-1]、GAP[384,-1]、GAP[648,-1]
void test3(int n)
{
    GL2Zn D8;
	D8.m_n=n;
	vector<TM2> gen;
	TM2 a(4);
	TM2 b(4);	
	a[0]=0;
	a[1]=n-1;
	a[2]=1;
	a[3]=0;
	b[0]=n-1;
	b[1]=1;
	b[2]=0;
	b[3]=n-1;		
	gen.push_back(a);
	gen.push_back(b);
	D8.s_Arr=GL2Zn::FG(gen,D8.m_n);
	D8.printSet();
	D8.printTable();	
}

int main(int argc,char *argv[])
{
    for(int i=3;i<10;i++)
	//for(int i=10;i<15;i++)
	//for(int i=15;i<20;i++)	
		test3(i);
    return 0;
}