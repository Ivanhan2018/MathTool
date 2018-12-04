// 生成有限环F_p[x]/(f)的加法、乘法凯莱表
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include "zpoly.h"
using namespace std;

int main(int argc, char **argv)
{
	char szp[100]={0};
	if(argc<2)
	{
		printf("请输入p：");
		scanf("%s",szp);
	}
	else
		strcpy(szp,argv[1]);

	char szf[100]={0};
	if(argc<3)
	{
		printf("请输入f：");
		scanf("%s",szf);
	}
	else
		strcpy(szf,argv[2]);
   
	int p=atoi(szp);
	bool bPrime=zpoly::IsPrime(p);
	if(p<0||!bPrime)
	{
		printf("%d不是正素数！\n",p);
		return -1;
	} 
	zpoly f(p);
	f.set((const char*)szf);
       
	int n=f.degree();
	int m=pow(p,n);
	char sz1[100]={0};
        if(argc>=4)
        {
           strcpy(sz1,argv[3]);
        }
        else 
        {
	   sprintf(sz1,"GR%d.txt",m);
        }


	//const char *szf=f.isIrreducible()?(f.isPrimitive()?"是本原多项式":"是非本原不可约多项式"):"是可约多项式";
        const char *szf1=f.isIrreducible()?"是不可约多项式":"是可约多项式";
	cout <<"f=";
	f.print();
	cout <<"="<<zpoly::ToStr(f).c_str()<<"="<<zpoly::ToUINT64(f,p)<<szf1<<endl;
	vector<vector<int> > gfadd=zpoly::makeAddTable(n,m,p);
	vector<vector<int> > gfmul=zpoly::makeMulTable(f,n,m,p);

	FILE *fp=fopen(sz1,"w");
	// 生成加群凯莱表
	printf("[R%dAdd]\n",m);
	fprintf(fp,"[R%dAdd]\n",m);
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
	printf("[R%dMul]\n",m);
	fprintf(fp,"[R%dMul]\n",m);
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