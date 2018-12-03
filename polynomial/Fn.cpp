// 寻找F_p[x]上的不可约多项式，生成有限域F_n(n=p^m)的加法、乘法凯莱表Fn.exe
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include "zpoly.h"
using namespace std;

vector<int>  Factors(int n)
{
	vector<int> ret;
	if(n<1)
		return ret;
	for(int i=1;i<=n;i++)
	{
		if(n%i==0)
		{
			ret.push_back(i);
		}
	}
	return ret;
}

// 尝试把整数n分解为n=p^m
vector<int>  Factors2(int n)
{
     vector<int> ret(2);
     vector<int> v=Factors(n);
     for(int i=0;i<v.size();i++)
     {
	bool bPrime=zpoly::IsPrime(v[i]);
	if(bPrime)
        {
            int m=(int)(log((double)n)/log((double)v[i])+0.5);
            if(pow(v[i],m)==n)
            {
               ret[0]=v[i];
               ret[1]=m;
               return ret;
            }
        }
     }
     ret[0]=0;
     ret[1]=0;
     return ret;  
}

int main(int argc, char **argv)
{
	char szn[100]={0};
	if(argc<2)
	{
		printf("请输入n=p^m：");
		scanf("%s",szn);
	}
	else
		strcpy(szn,argv[1]);

	int m=atoi(szn);
        vector<int> v=Factors2(m);  
	if(v[0]==0||v[1]==0)
	{
		printf("有限域F_%d不存在！\n",m);
		return -1;
	}        
	int n=v[0];
	int p=v[1];
	char sz1[100]={0};
	sprintf(sz1,"F%d.txt",m);
	vector<vector<int> > vv=zpoly::findIrreducible(n,p); 
	zpoly f(p);
	f.set(vv[0]);
	//const char *szf=f.isIrreducible()?(f.isPrimitive()?"是本原多项式":"是非本原不可约多项式"):"是可约多项式";
	cout <<"f=";
	f.print();
	cout <<"="<<zpoly::ToStr(f).c_str()<<"="<<zpoly::ToUINT64(f,p)<<endl;
	vector<vector<int> > gfadd=zpoly::makeAddTable(n,m,p);
	vector<vector<int> > gfmul=zpoly::makeMulTable(f,n,m,p);

	FILE *fp=fopen(sz1,"w");
	// 生成加群凯莱表
	printf("[F%dAdd]\n",m);
	fprintf(fp,"[F%dAdd]\n",m);
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<m;j++)
		{
			printf("%d ",gfadd[i][j]+1);
			fprintf(fp,"%d ",gfadd[i][j]+1);
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
			printf("%d ",gfmul[i][j]+1);
			fprintf(fp,"%d ",gfmul[i][j]+1);
		}
		printf("\n");
		fprintf(fp,"\n");
	}
	fclose(fp);

	return 0;
}