#include"GL2Zn.h"
#include<set>

set<string> gS;
set<std::pair<int,int>> gM;
int g_a=0;
void findgroup2(GL2Zn *g,int N)
{	
    int m=g->size();
	for(int i=g_a;i<m;i++)		
	for(int j=i+1;j<m;j++){	
		TM2 vi=g->s_Arr[i];
		TM2 vj=g->s_Arr[j];
		vector<int> v;
		v.push_back(i);
		v.push_back(j);
		Subgroup G;
		bool b=G.init(g,v,N);
		if(!b)
			continue;
		int ni=G.size();
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(ni,ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			printf("%d,%d->%s,%s=>GAP[%d,%d]:\n",i,j,V2S(vi).c_str(),V2S(vj).c_str(),G.size(),ID);				
		}
		if(ID==-1){		
			string N0=calcN0(&G);  
			string C1=calcC1(&G);	
			string Nk=calcNk(&G);	
			string S2=calcS2(&G);
			string kKEZDCANS=calckKEZDCANS(&G);
			string strG=N0+C1+Nk+S2+kKEZDCANS;
			if(gS.find(strG)==gS.end()){			
				printf("N0C1Nk=%s,%s,%s\n",N0.c_str(),C1.c_str(),Nk.c_str());  
				printf("S2=%s\n",S2.c_str());
				printf("kKEZDCANS=%s\n",kKEZDCANS.c_str());			
			}
			gS.insert(strG);
		}
	}
}

void findgroup3(GL2Zn *g,int N)
{	
    int m=g->size();
	for(int i=g_a;i<m;i++)		
	for(int j=i+1;j<m;j++){	
	for(int k=j+1;k<m;k++){
		TM2 vi=g->s_Arr[i];
		TM2 vj=g->s_Arr[j];
		TM2 vk=g->s_Arr[k];		
		vector<int> v;
		v.push_back(i);
		v.push_back(j);
		v.push_back(k);		
		Subgroup G;
		bool b=G.init(g,v,N);
		if(!b)
			continue;
		int ni=G.size();
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(ni,ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			printf("%d,%d,%d->%s,%s,%s=>GAP[%d,%d]:\n",i,j,k,V2S(vi).c_str(),V2S(vj).c_str(),V2S(vk).c_str(),G.size(),ID);				
		}
		if(ID==-1){		
			string N0=calcN0(&G);  
			string C1=calcC1(&G);	
			string Nk=calcNk(&G);	
			string S2=calcS2(&G);
			string kKEZDCANS=calckKEZDCANS(&G);
			string strG=N0+C1+Nk+S2+kKEZDCANS;
			if(gS.find(strG)==gS.end()){			
				printf("N0C1Nk=%s,%s,%s\n",N0.c_str(),C1.c_str(),Nk.c_str());  
				printf("S2=%s\n",S2.c_str());
				printf("kKEZDCANS=%s\n",kKEZDCANS.c_str());			
			}
			gS.insert(strG);
		}
	}
	}
}

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

//GAP[16,8]、GAP[24,8]、GAP[40,5]、GAP[48,6]、GAP[32,19]、GAP[48,14]、GAP[48,6]
//GAP[120,-1]、GAP[40,8]、GAP[96,-1]、GAP[56,-1]、GAP[96,-1]、
//GAP[160,-1]、GAP[96,-1]、GAP[72,5]、GAP[48,6]、GAP[72,8]、
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
    findgroup2(&D8,72);	
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
    findgroup2(&D8,72);		
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
    findgroup2(&D8,72);		
}

int main(int argc,char *argv[])
{
	g_a=argc>1?atoi(argv[1]):0;
	int n=argc>2?atoi(argv[2]):12;
	//test1(19);
    //for(int i=3;i<10;i++)
	//for(int i=10;i<15;i++)
	//for(int i=2;i<20;i++)	
		//test3(i);
	test2(n);
    return 0;
}