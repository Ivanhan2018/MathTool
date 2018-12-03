//#include "stdafx.h"
#include "RingUtil.h"
#include <cstdio>
#include <map>
using namespace std;

#define USE_ARR2 //直接使用加法、乘法运算表表示有限环

#ifdef USE_ARR2

#include "Rn.h"

struct IdxArr2
{
	int n;
	int Id;
	int *RAdd;
  	int *RMul;
  	char name[100];   
};

IdxArr2 g_ring_arr2_data[]={ \
	{2,1,&g_M2Add[0][0],&g_M2Mul[0][0],"M_2"}, \
	{2,2,&g_F2Add[0][0],&g_F2Mul[0][0],"F_2"}, \
	{3,1,&g_M3Add[0][0],&g_M3Mul[0][0],"M_3"}, \
	{3,2,&g_F3Add[0][0],&g_F3Mul[0][0],"F_3"}, \
	{4,1,&g_M4Add[0][0],&g_M4Mul[0][0],"M_4"}, \
	{4,2,&g_B4Add[0][0],&g_B4Mul[0][0],"R4_2"}, \
	{4,3,&g_Z4Add[0][0],&g_Z4Mul[0][0],"Z/4Z"}, \
	{4,4,&g_M2M2Add[0][0],&g_M2M2Mul[0][0],"M_2×M_2"}, \
	{4,5,&g_R4_5_Add[0][0],&g_R4_5_Mul[0][0],"ring 22.Nu.2"}, \
	{4,6,&g_M2F2Add[0][0],&g_M2F2Mul[0][0],"M_2×F_2"}, \
	{4,7,&g_Y4Add[0][0],&g_Y4Mul[0][0],"ring 22.NC.2"}, \
	{4,8,&g_P4Add[0][0],&g_P4Mul[0][0],"ring 22.NC.1"}, \
	{4,9,&g_X4Add[0][0],&g_X4Mul[0][0],"ring 22.u.2"}, \
	{4,10,&g_F2F2Add[0][0],&g_F2F2Mul[0][0],"F_2×F_2"}, \
	{4,11,&g_F4Add[0][0],&g_F4Mul[0][0],"F_4"}, \
};

int g_ring_arr2_data_count=sizeof(g_ring_arr2_data)/sizeof(IdxArr2);

map<pair<int,int>,IdxArr2> g_Marr2;

void initArr2()
{
	for(int i=0;i<g_ring_arr2_data_count;i++)
	{
	    pair<int,int> key=make_pair(g_ring_arr2_data[i].n,g_ring_arr2_data[i].Id);
            g_Marr2[key]=g_ring_arr2_data[i];
	}
}

IdxArr2 SmallRingArr2(int n,int Id)
{
	pair<int,int> key=make_pair(n,Id);
	map<pair<int,int>,IdxArr2>::const_iterator ptr=g_Marr2.find(key);
	if(ptr!=g_Marr2.end())
	{
	    return ptr->second;
	}

	IdxArr2 ret;
	ret.n=n;
	ret.Id=Id;
	ret.RAdd=NULL;
	ret.RMul=NULL;
	ret.name[0]='\0';
	return ret;
}

#endif

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
	//R4_4:=DirectSum(SmallRing(2,1),SmallRing(2,1));
	{4,4,"2;4;2,0,0,0;0,0,0,2;2,0,0,2","M_2×M_2"}, \
	//R4_6:=DirectSum(SmallRing(2,1),SmallRing(2,2));
	{4,6,"1;4;2|1;2;1","M_2×F_2"}, \
	//R4_7:=ring 22.NC.2
	{4,7,"2;2;0,1,0,0;1,0,0,0;1,1,0,0","ring 22.NC.2"}, \
	//R4_8:=ring 22.NC.1
	{4,8,"2;2;0,0,1,0;1,0,0,0;1,0,1,0","ring 22.NC.1"}, \
	//R4_10:=DirectSum(SmallRing(2,2),SmallRing(2,2));
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

#ifdef USE_ARR2
	initArr2();
	IdxArr2 idxarr2=SmallRingArr2(n,Id);
	if(idxarr2.RAdd==NULL||idxarr2.RMul==NULL)
	{
		printf("Error\n");	   
		return -1;
	}
	char szfn[100]={0};
	sprintf(szfn,"R%d_%d.txt",n,Id);
        vector<vector<int> > A=RingUtil::Arr2ToVec2(idxarr2.RAdd,n);
        vector<vector<int> > B=RingUtil::Arr2ToVec2(idxarr2.RMul,n);
	bool bRet=RingUtil::SaveTable(szfn,A,B);
#else
	init();
	IdxFg idxfg=SmallRingFg(n,Id);
	if(idxfg.Fg[0]=='\0')
	{
		printf("Error\n");	   
		return -1;
	}
	char szfn[100]={0};
	sprintf(szfn,"R%d_%d.txt",n,Id);
	bool bRet=RingUtil::FR(idxfg.Fg,szfn);
#endif

	//system("pause");
	return 0;
}


