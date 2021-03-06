﻿// 寻找F_2[x]上的不可约多项式，生成有限域GF(2^n)的加法、乘法凯莱表GF.exe
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include "GaloisField.h"
#include "GaloisFieldElement.h"
#include "GaloisFieldPolynomial.h"
using namespace std;

int Mod(int ret,unsigned int n)
{
	//assert(n>0);
	if(ret<0)
	{
		int ret1=ret+(-ret+1)*n;
		return ret1%n;
	}
	return ret%n;
}

bool FindrootInFp(vector<unsigned int>& v,int p)
{
        for(int i=0;i<p;i++)
	{
                int ret=0;
		for(int j=0;j<v.size();j++)
		{
                  ret+=v[j]*pow(i,v.size()-1-j);
		}
		if(Mod(ret,p)==0)
		{
	          return true;
		}
	}

	return false;
}

// 求F_2[x]中所有<=n次的不可约多项式
/*
例如：n=3时
ii=0
1 1 0 1 
ii=1
1 0 1 1 
ii=2
0 1 1 1 
即
x^3+x^2+1=0在F_2中无根
x^3+x+1=0在F_2中无根
x^2+x+1=0在F_2中无根
i=15:1 1 1 1 
i=14:1 1 1 0 
i=13:1 1 0 1 
i=12:1 1 0 0 
i=11:1 0 1 1 
i=10:1 0 1 0 
i=9:1 0 0 1 
i=8:1 0 0 0 
i=7:0 1 1 1 
i=6:0 1 1 0 
i=5:0 1 0 1 
i=4:0 1 0 0 
i=3:0 0 1 1 
i=2:0 0 1 0
*/
//#define USE_PRINT_ALL
vector<vector<unsigned int> > findIrreducible(int n)
{
   vector<vector<unsigned int> > vv;
   int ii=0;
   vector<unsigned int> v(n+1);
   int t=pow(2,n+1);
   for(int i=t-1;i>=2;i--)//注意这里的下界
   {
#ifdef USE_PRINT_ALL
     printf("\ni=%d:",i);
#endif
     for(int j=0;j<n+1;j++)
     { 
       v[j]=((i>>(n-j)) & 1);
#ifdef USE_PRINT_ALL
       printf("%d ",v[j]);
#endif
     }
     bool bFind=FindrootInFp(v,2);
     if(!bFind)
     {
       vv.push_back(v);
#ifndef USE_PRINT_ALL
       printf("ii=%d\n",ii);
#endif
       for(int j=0;j<n+1;j++)
       { 
#ifndef USE_PRINT_ALL
       printf("%d ",v[j]);
#endif
       }
       ii++;
#ifndef USE_PRINT_ALL
       printf("\n");
#endif         
     }     
   }

   return vv;
}

int main(int argc, char **argv)
{
	char szn[100]={0};
	if(argc<2)
	{
		printf("请输入n：");
		scanf("%s",szn);
	}
	else
		strcpy(szn,argv[1]);

	int n=atoi(szn);
	int m=pow(2,n);
	char sz1[100]={0};
	sprintf(sz1,"F%d.txt",m);

	vector<vector<unsigned int> > vv=findIrreducible(n); 
	galois::GaloisField gf(n,&vv[0][0]);

	FILE *fp=fopen(sz1,"w");
	// 生成加群凯莱表
	printf("[F%dAdd]\n",m);
	fprintf(fp,"[F%dAdd]\n",m);
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<m;j++)
		{
			printf("%d ",gf.add(i,j)+1);
			fprintf(fp,"%d ",gf.add(i,j)+1);
		}
		printf("\n");
		fprintf(fp,"\n");
	}
	// 生成乘法凯莱表
	printf("[F%dMul]\n",m);
	fprintf(fp,"[F%dMul]\n",m);
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<m;j++)
		{
			printf("%d ",gf.mul(i,j)+1);
			fprintf(fp,"%d ",gf.mul(i,j)+1);
		}
		printf("\n");
		fprintf(fp,"\n");
	}
	fclose(fp);

	return 0;
}