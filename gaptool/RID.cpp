#include<stdio.h>
#include<stdlib.h>

// 判断一个文件是否存在
bool FileExists(const char *str){
	FILE *fp;
	if((fp=fopen(str,"r"))!=NULL){
		fclose(fp);
		return true;
	}
	return false;
}

int main(int argc, char *argv[]){
	if(argc<3){
		printf("Usage:  RID n1 n2 [R32_]\n");
		return 0;
	}
	int n1=atoi(argv[1]);
	int n2=atoi(argv[2]);
    char* str="R32_";
    if(argc>3)	
		str=argv[3];
	int cnt=0;
	for(int i=n1;i<=n2;i++)	{
		char sz[100]={0};
		sprintf(sz,"%s%d.txt",str,i);
		bool b=FileExists(sz);
		if(!b){
			cnt++;
			printf("%d ",i);
		}
	} 
   printf("\ncnt=%d\n",cnt);	
   return 0;
}