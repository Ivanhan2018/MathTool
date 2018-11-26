//  生成模n剩余类环Z/nZ的加法、乘法凯莱表Zn.exe
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

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

 int n=atoi(sz);
 char sz1[100]={0};
 sprintf(sz1,"Z%d.txt",n);

 FILE *fp=fopen(sz1,"w");
 // 生成加群凯莱表
 printf("[R%dAdd]\n",n);
 fprintf(fp,"[R%dAdd]\n",n);
 for(int i=0;i<n;i++)
 {
  for(int j=i;j<i+n;j++)
  {
   printf("%d ",j%n+1);
   fprintf(fp,"%d ",j%n+1);
  }
  printf("\n");
  fprintf(fp,"\n");
 }
 // 生成乘法凯莱表
 printf("[R%dMul]\n",n);
 fprintf(fp,"[R%dMul]\n",n);
 for(int i=0;i<n;i++)
 {
  for(int j=0;j<n;j++)
  {
   printf("%d ",(i*j)%n+1);
   fprintf(fp,"%d ",(i*j)%n+1);
  }
  printf("\n");
  fprintf(fp,"\n");
 }
 fclose(fp);

 return 0;
}