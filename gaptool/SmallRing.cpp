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
	{2,1,"1;4;2","M_2"}, \
	//R2_2:=RingByGenerators([ZmodnZObj(1,2)]);
	{2,2,"1;2;1","F_2"}, \
	//R3_1:=RingByGenerators([ZmodnZObj(3,9)]);
	{3,1,"1;9;3","M_3"}, \
	//R3_2:=RingByGenerators([ZmodnZObj(2,6)]);
	{3,2,"1;6;2","F_3"}, \
	//R4_1:=RingByGenerators([ZmodnZObj(4,16)]);
	{4,1,"1;16;4","M_4"}, \
	//R4_2:=RingByGenerators([ZmodnZObj(2,8)]);
	{4,2,"1;8;2","R4_2"}, \
	//R4_3:=RingByGenerators([ZmodnZObj(1,4)]);
	{4,3,"1;4;1","Z/4Z"}, \
	//R4_7:=ring 22.NC.2
	{4,7,"2;2;0,1,0,0;1,0,0,0;1,1,0,0","ring 22.NC.2"}, \
	//R4_8:=ring 22.NC.1
	{4,8,"2;2;0,0,1,0;1,0,0,0;1,0,1,0","ring 22.NC.1"}, \
	//R4_10:=DirectSum(GF(2),GF(2));
	{4,10,"2;2;1,0,0,0;1,0,0,1;0,0,0,1","4阶布尔环F_2×F_2=F_2[v]/(v(v+1))=F_2[i]"}, \
	//R5_1:=RingByGenerators([ZmodnZObj(5,25)]);
	{5,1,"1;25;5","M_5"}, \
	//R5_2:=RingByGenerators([ZmodnZObj(1,5)]);
	{5,2,"1;5;1","F_5"}, \
	//R6_1:=RingByGenerators([ZmodnZObj(6,36)]);
	{6,1,"1;36;6","M_6=M_3×M_2"}, \
	//R6_2:=RingByGenerators([ZmodnZObj(2,12)]);
	{6,2,"1;12;2","F_3×M_2"}, \
	//R6_3:=RingByGenerators([ZmodnZObj(3,18)]);
	{6,3,"1;18;3","M_3×F_2"}, \
	//R6_4:=RingByGenerators([ZmodnZObj(1,6)]);
	{6,4,"1;6;1","Z/6Z=F_3×F_2"}, \
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


