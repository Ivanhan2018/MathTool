#include<stdlib.h>
#include<stdio.h>

int main(int argc, char* argv[]){
	if(argc<3){
#ifdef IDR		
		printf("Usage:  IDR n1 n2 [M2]\n");
#else
		printf("Usage:  IDR n1 n2 [M2]\n");
#endif	
		return 0;
	}
	int n1=atoi(argv[1]);
	int n2=atoi(argv[2]);
    char* str="M2";
    if(argc>3)	
		str=argv[3];
	char* str1="";
    if(argc>4)	
		str1=argv[4];  	
	for(int i=n1;i<=n2;i++)	{
		char sz[1000]={0};
#ifdef IDR
		sprintf(sz,"IdRing R32_%d%s.txt",i,str);
#else
		sprintf(sz,"QR16 R32_%d%s.txt %s",i,str,str1);
#endif
		printf("i=%d\n%s\n",i,sz);
		system(sz);
	} 
   return 0;
 }