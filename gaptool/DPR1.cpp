#include<stdlib.h>
#include<stdio.h>

int main(int argc, char* argv[]){
	if(argc<3){
		printf("Usage:  DPR1 n1 n2 [M2]\n");
		return 0;
	}
	int n1=atoi(argv[1]);
	int n2=atoi(argv[2]);
    char* str="M2";
    if(argc>3)	
		str=argv[3];
	int fun=4;
	if(argc>4){
		fun=atoi(argv[4]);
		if(fun<0||fun>4){
			fun=0;
		}	
	}
	char* str1="";
    if(argc>5)	
		str1=argv[5]; 
	char* str0="R32_";
    if(argc>6)	
		str0=argv[6];  	
	for(int i=n1;i<=n2;i++)	{
		char sz[1000]={0};
		sprintf(sz,"DPR0 %s%d.txt %s.txt %s%d%s.txt %d %s",str0,i,str,str0,i,str,fun,str1);
		printf("i=%d\n%s\n",i,sz);
		system(sz);
	} 
   return 0;
 }