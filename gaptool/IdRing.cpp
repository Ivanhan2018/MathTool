#include "RingUtil.h"
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <map>
using namespace std;

struct Idx
{
	int n;
	int Id;
	char calR[100];//环的结构不变量n0,bA,bO,n1,n2,n4,n5,n6,n7
  	char name[100];      
};

Idx g_ring_data[]={ \
	{2,1,"2,1,0,2,1,1,1,4,1","M_2"}, \
	{2,2,"2,1,1,1,2,0,0,3,0","F_2"}, \
	{3,1,"3,1,0,3,1,2,2,9,2","M_3"}, \
	{3,2,"3,1,1,1,2,0,0,5,0","F_3"}, \
	{4,1,"4,1,0,4,1,3,3,16,3","M_4"}, \
	{4,2,"4,1,0,4,1,1,3,12,3","R4_2"}, \
	{4,3,"4,1,1,2,2,1,1,8,1","Z/4Z"}, \
	{4,4,"2,1,0,4,1,3,3,16,3","M_2×M_2"}, \
	{4,5,"2,1,0,4,1,1,3,12,3","ring 22.Nu.2"}, \
	{4,6,"2,1,0,4,2,1,1,12,3","M_2×F_2"}, \
	{4,7,"2,0,0,4,3,1,1,10,1","ring 22.NC.2"}, \
	{4,8,"2,0,0,4,3,1,1,10,3","ring 22.NC.1"}, \
	{4,9,"2,1,1,2,2,1,1,8,1","ring 22.u.2=F_2+uF_2"}, \
	{4,10,"2,1,1,3,4,0,0,9,2","4阶布尔环F_2×F_2=F_2[v]/(v(v+1))=F_2[i]"}, \
	{4,11,"2,1,1,1,2,0,0,7,0","F_4"}, \
	{5,1,"5,1,0,5,1,4,4,25,4","M_5"}, \
	{5,2,"5,1,1,1,2,0,0,9,0","F_5"}, \
	{6,1,"6,1,0,6,1,5,5,36,5","M_6=M_3×M_2"}, \
	{6,2,"6,1,0,6,2,1,1,20,5","F_3×M_2"}, \
	{6,3,"6,1,0,6,2,2,2,27,5","M_3×F_2"}, \
	{6,4,"6,1,1,4,4,0,0,15,3","Z/6Z=F_3×F_2"}, \
	{8,1,"8,1,0,8,1,7,7,64,7","M_8"}, \
	{8,2,"8,1,0,8,1,3,3,32,7","R8_2"}, \
	{8,3,"8,1,1,4,2,1,3,20,3","Z/8Z"}, \
	{8,4,"8,1,0,8,1,3,7,48,7","R8_4"}, \
	{8,5,"4,1,0,8,1,7,7,64,7","R4_1R2_1"}, \
	{8,7,"4,1,0,8,2,3,3,48,7","R4_1R2_2"}, \
	{8,13,"4,1,0,8,2,3,3,32,7","R4_3R2_1"}, \
	{8,14,"4,1,1,6,4,1,1,24,5","Z/4Z×F_2"}, \
	{8,16,"4,1,0,8,1,3,7,48,7","R4_2R2_1"}, \
	{8,17,"4,1,0,8,2,1,3,36,7","R4_2R2_2"}, \
	{8,25,"2,1,0,8,1,7,7,64,7","R4_4R2_1"}, \
	{8,26,"2,1,0,8,1,3,7,48,7","R4_5R2_1"}, \
	{8,27,"2,1,0,8,2,3,3,48,7","M_2×M_2×F_2"}, \
	{8,29,"2,0,0,8,3,3,3,40,7","R4_7R2_1"}, \
	{8,34,"2,0,0,8,3,3,3,40,7","R4_8R2_1"}, \
	{8,35,"2,1,0,8,2,3,3,32,7","R4_9R2_1"}, \
	{8,38,"2,1,0,8,2,1,3,36,7","R4_5R2_2"}, \
	{8,40,"2,1,0,8,4,1,1,36,7","F_2×F_2×M_2"}, \
	{8,41,"2,0,0,8,6,1,1,30,5","R4_7R2_2"}, \
	{8,42,"2,0,0,8,6,1,1,30,5","F_4×M_2"}, \
	{8,48,"2,1,1,6,4,1,1,24,5","R4_9R2_2=F_2[x]/(x^3+x^2)=F_2[x]/(x^3+x)=F_2[x,y]/(x^3+x^2,y)"}, \
	{8,50,"2,1,1,7,8,0,0,27,6","8阶布尔环F_2×F_2×F_2×F_2"}, \
	{8,51,"2,1,1,5,4,0,0,21,4","F_4×F_2"}, \
	{8,52,"2,1,1,1,2,0,0,15,0","F_8"}, \
};

int g_ring_data_count=sizeof(g_ring_data)/sizeof(Idx);

map<string,vector<Idx> > g_M;

void initM()
{
	for(int i=0;i<g_ring_data_count;i++)
	{
		vector<Idx> v;
		map<string,vector<Idx> >::const_iterator ptr=g_M.find(g_ring_data[i].calR);
		if(ptr!=g_M.end())
		{
			v=ptr->second; 
		}
		v.push_back(g_ring_data[i]);
		g_M[g_ring_data[i].calR]=v;
	}
}

vector<Idx> IdRing(const char* key)
{
	vector<Idx> ret;
	map<string,vector<Idx> >::const_iterator ptr=g_M.find(key);
	if(ptr!=g_M.end())
	{
		return ptr->second;
	}

	return ret;
}

int main(int argc, char **argv)
{
	char calR[100]={0};
	if(argc<2)
	{
		printf("请输入环的结构不变量calR=n0,bA,bO,n1,n2,n4,n5,n6,n7：");
		scanf("%s",calR);
	}
	else
		strcpy(calR,argv[1]);

	initM();    
	vector<Idx> v=IdRing((const char*)calR);
	int n=v.size();
	if(n<=0)
	{
		printf("Error\n");
	}
	else if(n==1)
	{
		printf("IdRing(%s)=[%d,%d],name=%s\n", calR,v[0].n,v[0].Id,v[0].name);
	}
	else
	{
		printf("IdRing(%s)有%d个环：\n", calR,n);
		for(int i=0;i<v.size();i++)
		{
			printf("IdRing(%s)=[%d,%d],name=%s\n", calR,v[i].n,v[i].Id,v[i].name);
		}
	}
	if(n>0)
	{
		vector<string> v=RingUtil::split(calR,",");
		int N=v.size();
		vector<int> vi(N);
		for(int j=0;j<N;j++)
		{
			int iElem=atoi(v[j].c_str());
			vi[j]=iElem;
		}
		if(N>=9)
		{
			printf("特征n0=%d,",vi[0]);
			const char *szRet=(vi[1]==1?"交换":"非交换");
			printf("%s,",szRet);
			const char *szRet3=(vi[2]==1?"幺环":"无幺环");
			printf("%s,",szRet3);
			printf("不可逆元个数n1=%d,",vi[3]);
			printf("幂等元个数n2=%d,",vi[4]);
			printf("2次幂零元个数n4=%d,",vi[5]);
			printf("2~3次幂零元个数n5=%d,",vi[6]);
			printf("零乘个数n6=%d,",vi[7]);
			printf("零因子个数n7=%d,",vi[8]);
			printf("\n");
		}
	}

	//system("pause");
	return 0;
}