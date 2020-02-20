//  生成模n剩余类环Z/nZ的加法、乘法凯莱表Zn.exe
//  有限循环环的理想子环表示、Z／nZ的理想子环RingByGenerators([ZmodnZObj(m,n)])
/*
剩余类环Z/nZ的一般性质
①Z/nZ是交换环。Z/nZ是特征为n的有单位元的可换环。
②在Z/nZ中，所有左右零因子都是其零因子。
③Z/nZ是无零因子环的充分必要条件是n为素数。Z/nZ为整环的充分必要条件是n为素数。环Z/nZ是域<=>p为素数。设Z/nZ为无零因子环（n>1），那么加群中每一个非零元素的阶必相同。
④Z/nZ的加群的所有子群是循环子群。
⑤Z/nZ的所有理想都是主理想。所有循环子群（对加法）加上乘法都是Z/nZ的主理想。
⑥环Z/nZ有且只有T(n)个子环（其中T(n)表示n的正因子的个数），而且Z/nZ是一个n阶循环环，从而其加群、子环、理想是一致的。
例如：
T(12)=|{1,2,3,4,6,12}|=6
Z/12Z共有6个理想：<0>,<1>=<5>=<7>=<11>,<2>=<10>,<3>=<9>,<4>=<8>,<6>
⑦
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include<algorithm>
using namespace std;

vector<int> Order(int n,int m)
{
	vector<int> ret;
	int mi=m;
	int m0=0;
	while(mi!=0)
	{
		ret.push_back(mi);
		mi=(mi+m)%n;
	}
	ret.push_back(mi);
	std::sort(ret.begin(),ret.end());
	return ret;
}

int ToNormal(const vector<int> &v,int a)
{
	for(int i=0;i<v.size();i++)
	{
		if(v[i]==a)
			return i;
	}
	return -1;
}

int main(int argc, char **argv)
{
 char sz[100]={0};
 if(argc<2)
 {
  printf("请输入模n剩余类环Z/nZ的阶n：");
  scanf("%s",sz);
 }
 else
  strcpy(sz,argv[1]);

 char szm[100]={0};
 if(argc<3)
 {
  printf("请输入理想<m>的生成元m：");
  scanf("%s",szm);
 }
 else
  strcpy(szm,argv[2]);

 int n=atoi(sz);
 int m=atoi(szm);
 if(n<0||m>=n||m<0)
 {
   printf("Error input!\n");
   return 0;
 }
 char sz1[100]={0};
 sprintf(sz1,"Z%d_[%d].txt",n,m);

 vector<int> ZA=Order(n,m);
 int s=ZA.size();
 for(int i=0;i<s;i++)
 {
	 printf("%d ",ZA[i]);
 }
 printf("\n");

 FILE *fp=fopen(sz1,"w");
 // 生成加群凯莱表
 printf("[R%dAdd]\n",s);
 fprintf(fp,"[R%dAdd]\n",s);
 for(int i=0;i<s;i++)
 {
  for(int j=0;j<s;j++)
  {
	  int ij=(ZA[i]+ZA[j])%n;
	  int ij2=ToNormal(ZA,ij)+1;
	  printf("%d ",ij2);
	  fprintf(fp,"%d ",ij2);
  }
  printf("\n");
  fprintf(fp,"\n");
 }
 // 生成乘法凯莱表
 printf("[R%dMul]\n",s);
 fprintf(fp,"[R%dMul]\n",s);
 for(int i=0;i<s;i++)
 {
  for(int j=0;j<s;j++)
  {
	  int ij=(ZA[i]*ZA[j])%n;
	  int ij2=ToNormal(ZA,ij)+1;
	  printf("%d ",ij2);
	  fprintf(fp,"%d ",ij2);
  }
  printf("\n");
  fprintf(fp,"\n");
 }
 fclose(fp);

#ifdef WIN32
 system("pause");
#endif
 return 0;
}

