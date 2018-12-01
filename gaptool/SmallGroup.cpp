//#include "stdafx.h"
#include "GroupUtil.h"
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

IdxFg g_group_fg_data[]={ \
	{6,1,"2,1,3;3,2,1;1,3,2;2,3,1;3,1,2","S_3=D_3"}, \
	{6,2,"2,3,4,5,6,1","C_6"}, \
};

int g_group_fg_data_count=sizeof(g_group_fg_data)/sizeof(IdxFg);

map<pair<int,int>,IdxFg> g_Mfg;

void init()
{
	for(int i=0;i<g_group_fg_data_count;i++)
	{
	    pair<int,int> key=make_pair(g_group_fg_data[i].n,g_group_fg_data[i].Id);
            g_Mfg[key]=g_group_fg_data[i];
	}
}

IdxFg SmallGroupFg(int n,int Id)
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
	IdxFg idxfg=SmallGroupFg(n,Id);
	if(idxfg.Fg[0]=='\0')
	{
		printf("Error\n");	   
		return -1;
	}
	char szfn[100]={0};
	sprintf(szfn,"GAP4[%d,%d].txt",n,Id);
	bool bRet=GroupUtil::FG(idxfg.Fg,szfn);

	//system("pause");
	return 0;
}


