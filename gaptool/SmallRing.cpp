//#include "stdafx.h"
#include "RingUtil.h"
#include <cstdio>
#include <map>
using namespace std;

struct IdxFg
{
	int n;
	int Id;
	char Fg[100];
  	char name[100];  
};

IdxFg g_ring_fg_data[]={ \
	//R2_1:=RingByGenerators([ZmodnZObj(2,4)]);
	{2,1,"1;4;2","M_2=M_2(M_2^+=C_2)"}, \
	//R2_2:=RingByGenerators([ZmodnZObj(1,2)]);
	{2,2,"1;2;1","GF(2)"}, \
};

int g_ring_fg_data_count=sizeof(g_ring_fg_data)/sizeof(IdxFg);

map<pair<int,int>,IdxFg> g_Mfg;

void init()
{
	for(int i=0;i<g_ring_fg_data_count;i++)
	{
	    pair<int,int> key=make_pair(g_ring_fg_data[i].n,g_ring_fg_data[i].Id);
            g_Mfg[key]=g_ring_fg_data[i];
	}
}

IdxFg SmallRingFg(int n,int Id)
{
	pair<int,int> key=make_pair(n,Id);
	map<pair<int,int>,IdxFg>::const_iterator ptr=g_Mfg.find(key);
	if(ptr!=g_Mfg.end())
	{
	    return ptr->second;
	}

	IdxFg ret;
	ret.n=n;
	ret.Id=Id;
	ret.Fg[0]='\0';
	ret.name[0]='\0';
	return ret;
}

int main(int argc, char* argv[])
{
	char szn[100]={0};
	char szId[100]={0};
	if(argc<2)
	{
		printf("请输入n：");
		scanf("%s",szn);
	}
	else
		strcpy(szn,argv[1]);

	if(argc<3)
	{
		printf("请输入Id：");
		scanf("%s",szId);
	}
	else
		strcpy(szId,argv[2]);

	int n=atoi(szn);
	int Id=atoi(szId);

	init();
	IdxFg idxfg=SmallRingFg(n,Id);
	if(idxfg.Fg[0]=='\0')
	{
		printf("Error\n");	   
		return -1;
	}
	char szfn[100]={0};
	sprintf(szfn,"SmallRing[%d,%d].txt",n,Id);
	bool bRet=RingUtil::FR(idxfg.Fg,szfn);

	//system("pause");
	return 0;
}


