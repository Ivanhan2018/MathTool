//  生成循环零乘环Mn的加法、乘法凯莱表Mn.exe
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
  printf("请输入循环零乘环Mn的阶n：");
  scanf("%s",sz);
 }
 else
  strcpy(sz,argv[1]);

 int n=atoi(sz);
 char sz1[100]={0};
 sprintf(sz1,"M%d.txt",n);

 FILE *fp=fopen(sz1,"w");
 // 生成加群凯莱表
 printf("[M%dAdd]\n",n);
 fprintf(fp,"[M%dAdd]\n",n);
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
 printf("[M%dMul]\n",n);
 fprintf(fp,"[M%dMul]\n",n);
 for(int i=0;i<n;i++)
 {
  for(int j=0;j<n;j++)
  {
   printf("%d ",1);
   fprintf(fp,"%d ",1);
  }
  printf("\n");
  fprintf(fp,"\n");
 }
 fclose(fp);

 return 0;
}