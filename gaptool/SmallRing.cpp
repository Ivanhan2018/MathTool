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
	{6,1,&g_M6Add[0][0],&g_M6Mul[0][0],"M_6"}, \
	{6,2,&g_F3M2Add[0][0],&g_F3M2Mul[0][0],"F_3×M_2"}, \
	{6,3,&g_M3F2Add[0][0],&g_M3F2Mul[0][0],"M_3×F_2"}, \
	{6,4,&g_Z6Add[0][0],&g_Z6Mul[0][0],"Z/6Z=F_3×F_2"}, \
	{8,1,&g_M8Add[0][0],&g_M8Mul[0][0],"M_8"}, \
	{8,2,&g_Z8Add[0][0],&g_R8_2Mul[0][0],"R8_2"}, \
	{8,3,&g_Z8Add[0][0],&g_Z8Mul[0][0],"Z/8Z"}, \
	{8,4,&g_Z8Add[0][0],&g_R8_4Mul[0][0],"R8_4"}, \
	{8,6,&g_R8_24Add[0][0],&g_R8_6Mul[0][0],"R8_6"}, \
	{8,8,&g_R8_24Add[0][0],&g_R8_8Mul[0][0],"R8_8"}, \
	{8,9,&g_R8_24Add[0][0],&g_R8_9Mul[0][0],"R8_9"}, \
	{8,10,&g_R8_24Add[0][0],&g_R8_10Mul[0][0],"R8_10"}, \
	{8,11,&g_R8_24Add[0][0],&g_R8_11Mul[0][0],"R8_11"}, \
	{8,12,&g_R8_24Add[0][0],&g_R8_12Mul[0][0],"R8_12"}, \
	{8,14,&g_Z4F2Add[0][0],&g_Z4F2Mul[0][0],"Z/4Z×F_2"}, \
	{8,15,&g_R8_24Add[0][0],&g_R8_15Mul[0][0],"R8_15"}, \
	{8,18,&g_R8_24Add[0][0],&g_R8_18Mul[0][0],"R8_18"}, \
	{8,20,&g_R8_24Add[0][0],&g_R8_20Mul[0][0],"R8_20"}, \
	{8,22,&g_R8_24Add[0][0],&g_R8_22Mul[0][0],"R8_22"}, \
	{8,23,&g_R8_24Add[0][0],&g_R8_23Mul[0][0],"R8_23"}, \
	{8,24,&g_R8_24Add[0][0],&g_R8_24Mul[0][0],"R8_24"}, \
	{8,31,&g_F8Add[0][0],&g_R8_31Mul[0][0],"R8_31"}, \
	{8,32,&g_F8Add[0][0],&g_R8_32Mul[0][0],"R8_32"}, \
	{8,36,&g_F8Add[0][0],&g_R8_36Mul[0][0],"R8_36"}, \
	{8,37,&g_F8Add[0][0],&g_R8_37Mul[0][0],"R8_37"}, \
	{8,39,&g_F8Add[0][0],&g_R8_39Mul[0][0],"R8_39"}, \
	{8,45,&g_F8Add[0][0],&g_R8_45Mul[0][0],"R8_45"}, \
	{8,49,&g_F8Add[0][0],&g_R8_49Mul[0][0],"R8_49"}, \
	{8,52,&g_F8Add[0][0],&g_F8Mul[0][0],"F_8"}, \
	{9,5,&g_R9_5Add[0][0],&g_R9_5Mul[0][0],"R9_5"}, \
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
	char Fg[140];
  	char name[60];  
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
	//R4_9:=F_2[x]/(x^2);
	{4,9,"2,x^2,R4_9.txt","R4_9"}, \
	//R4_10:=DirectSum(SmallRing(2,2),SmallRing(2,2));
	{4,10,"2;2;1,0,0,0;1,0,0,1;0,0,0,1","4阶布尔环F_2×F_2=F_2[v]/(v(v+1))=F_2[i]"}, \
	//R4_11:=F_2[x]/(x^2+x+1);
	{4,11,"2,x^2+x^1+x^0,R4_11.txt","F_4"}, \
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
	//R8_1:=RingByGenerators([ZmodnZObj(8,64)]);
	{8,1,"1;64;8","M_8"}, \
	//R8_2:=RingByGenerators([ZmodnZObj(2,16)]);
	{8,2,"1;16;2","R8_2"}, \
	//R8_3:=RingByGenerators([ZmodnZObj(1,8)]);
	{8,3,"1;8;1","Z/8Z"}, \
	//R8_4:=RingByGenerators([ZmodnZObj(4,32)]);
	{8,4,"1;32;4","R8_4"}, \
	//R8_14:=DirectSum(SmallRing(4,3),SmallRing(2,2));
	{8,14,"1;4;1|1;2;1","Z/4Z×F_2"}, \
	//R8_19
	{8,19,"2;4;2,0,0,2;1,1,1,1","R8_19"}, \
	//R8_21
	{8,21,"2;4;1,0,0,3","R8_21"}, \
	//R8_28
	{8,28,"4;2;1,0,1,0,0,1,0,1,0,1,0,1,1,0,1,0;0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1","R8_28"}, \
	//R8_30
	{8,30,"4;2;0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1;0,0,0,0,1,1,0,0,1,0,1,0,0,0,0,1;0,0,0,0,1,1,0,0,0,0,1,0,1,0,0,1","R8_30"}, \
	//R8_33
	{8,33,"4;2;1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,1","R8_33"}, \
	//R8_43
	{8,43,"4;2;0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1;0,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1;0,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1","R8_43"}, \
	//R8_44
	{8,44,"3;2;1,0,0,0,1,0,0,0,0;0,0,0,1,0,0,1,0,0","R8_44"}, \
	//R8_46
	{8,46,"2,x^3,R8_46.txt","R8_46"}, \
	//R8_48:=DirectSum(SmallRing(4,9),SmallRing(2,2));
	{8,48,"2,x^3+x^1,R8_48.txt","R8_48"}, \
	//R8_51
	{8,51,"2,x^3+x^0,R8_51.txt","F4×F2"}, \
	//R8_52:=F_8
	{8,52,"2,x^3+x^1+x^0,R8_52.txt","F_8"}, \
	//R9_1:=RingByGenerators([ZmodnZObj(9,81)]);
	{9,1,"1;81;9","M_9"}, \
	//R9_2:=RingByGenerators([ZmodnZObj(1,9)]);
	{9,2,"1;9;1","Z/9Z"}, \
	//R9_3:=RingByGenerators([ZmodnZObj(3,27)]);
	{9,3,"1;27;3","R9_3"}, \
        //R9_4:=M_3+M_3
	{9,4,"1;9;3|1;9;3","M_3×M_3"}, \
        //R9_6:=M_3+F_3
	{9,6,"1;9;3|1;6;2","M_3×F_3"}, \
	//R9_7
	{9,7,"2;3;1,0,0,0;1,1,0,0","R9_7"}, \
	//R9_8
	{9,8,"2;3;1,0,0,0;1,0,1,0","R9_8"}, \
	//R9_9:=F_3[x]/(x^2);
	{9,9,"3,x^2,R9_9.txt","R9_9"}, \
        //R9_10:=F_3+F_3
	{9,10,"1;6;2|1;6;2","F_3×F_3"}, \
	//F_9:=F_3[x]/(x^2+x^0);
	{9,11,"3,x^2+x^0,F_9.txt","F_9"}, \
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
                int iFlag=0;// 非0表示继续查找
                if(argc==4) 
                {
         	    char szFlag[100]={0};
		    strcpy(szFlag,argv[3]);    
    	            iFlag=atoi(szFlag);                               
                }   
                
                if(iFlag)
                {
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
                    return 0;
                }
                else
                {
		   printf("Error\n");	   
		   return -1;
                }
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


