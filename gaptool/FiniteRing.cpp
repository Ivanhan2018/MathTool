#define PARSE_RING_FILE
#include"FiniteRing.h"

IRing* newR27(int i){
#ifdef PARSE_RING_FILE	
	char sz[100]={0};
	sprintf(sz,"R27_%d.txt",i);
	FiniteRing* r=newRing(sz);
	return r;	
#endif
	return NULL;
}

int testR16R81(){
   set<int> vID2;
   set<int> vID;   
   int cnt=0;
	for(int i=1;i<=11;i++){
		IRing* ri2=newR4(i);
		int IDi2=IdRing(ri2);		
		IRing* ri=newR4(i,3);
		int IDi=IdRing(ri);
		for(int j=i;j<=11;j++){
			IRing* rj2=newR4(j);
			int IDj2=IdRing(rj2);			
			DecompositionRing* r2= new DecompositionRing(ri2,rj2);
			r2->m_flag=1;			
			IRing* rj=newR4(j,3);
			int IDj=IdRing(rj);			
			DecompositionRing* r= new DecompositionRing(ri,rj);
			r->m_flag=1;
			int ID2=IdRing(r2);
			vID2.insert(ID2);			
			int ID=IdRing(r);
			vID.insert(ID);
			printf("R4_%d×R4_%d=R16_%d,R9_%d×R9_%d=R81_%d\n",IDi2,IDj2,ID2,IDi,IDj,ID);
			//delete r;
			//r=NULL;
		}
	}
    for(int i=1;i<=59;i++){
		IRing* ri2=newR8(i);		
		IRing* ri=newR27(i);
		for(int j=1;j<=2;j++){
			if(ri2){
				IRing* rj=new ZmodnZ(3-j,2*(3-j));
				DecompositionRing* r= new DecompositionRing(ri2,rj);
				r->m_flag=1;
				int ID=IdRing(r);
				vID2.insert(ID);
				printf("R8_%d×R2_%d=R16_%d,",i,j,ID);
				//delete r;
				//r=NULL;
			}
			if(ri){			
				IRing* rj=newR2(j,3);
				DecompositionRing* r= new DecompositionRing(ri,rj);
				r->m_flag=1;
				int ID=IdRing(r);
				vID.insert(ID);
				printf("R27_%d×R3_%d=R81_%d\n",i,j,ID);
				//delete r;
				//r=NULL;
			}else{
				printf("\n");
			}
		}
	}
    printf("%d种16阶可分解环\n",vID2.size());
	for(auto it=vID2.begin();it!=vID2.end();it++){
		printf("%d,",*it);
	}
	printf("\n");	
    printf("%d种81阶可分解环\n",vID.size());
	for(auto it=vID.begin();it!=vID.end();it++){
		printf("%d,",*it);
	}
	printf("\n");   
   return 0;
}

int R81_iR3_j(int argc, char* argv[]){
	int i0=1;
	int j0=1;
	if(argc>1)
	   i0=atoi(argv[1]);
    if(argc>2)
	   j0=atoi(argv[2]);     
	int fun=4;
	if(argc>3){
		fun=atoi(argv[3]);
		if(fun<0||fun>4){
			fun=0;
		}	
	}   
	char* str1="81";
    if(argc>4)	
		str1=argv[4];   
	int J=0;	
 	for(int i=i0;i<=495;i++){
		for(int j=(J==0?j0:1);j<=2;j++){
			J=1;
			char sz[1000]={0};
			sprintf(sz,"DPR0 R81_%d.txt R3_%d.txt R81_%d%R3_%d.txt %d %s",i,j,i,j,fun,str1);
			printf("i=%d,j=%d\n",i,j);
			system(sz);
		} 
	}
	return 0;
}

int R27_iR9_j(int argc, char* argv[]){
	int i0=1;
	int j0=1;
	if(argc>1)
	   i0=atoi(argv[1]);
    if(argc>2)
	   j0=atoi(argv[2]);     
	int fun=4;
	if(argc>3){
		fun=atoi(argv[3]);
		if(fun<0||fun>4){
			fun=0;
		}	
	}   
	char* str1="81";
    if(argc>4)	
		str1=argv[4];   
	int J=0;	
 	for(int i=i0;i<=59;i++){
		for(int j=(J==0?j0:1);j<=11;j++){
			J=1;
			char sz[1000]={0};
			sprintf(sz,"DPR0 R27_%d.txt R9%d.txt R27_%d%R9_%d.txt %d %s",i,j,i,j,fun,str1);
			printf("i=%d,j=%d\n",i,j);
			system(sz);
		} 
	}
	return 0;
}

int R27_iR27_j(int argc, char* argv[]){
	int i0=1;
	int j0=1;
	if(argc>1)
	   i0=atoi(argv[1]);
    if(argc>2)
	   j0=atoi(argv[2]);     
	int fun=4;
	if(argc>3){
		fun=atoi(argv[3]);
		if(fun<0||fun>4){
			fun=0;
		}	
	}   
	char* str1="81";
    if(argc>4)	
		str1=argv[4];   
	int J=0;	
 	for(int i=i0;i<=59;i++){
		for(int j=(J==0?j0:1);j<=59;j++){
			J=1;
			char sz[1000]={0};
			sprintf(sz,"DPR0 R27_%d.txt R27_%d.txt R27_%d%R27_%d.txt %d %s",i,j,i,j,fun,str1);
			printf("i=%d,j=%d\n",i,j);
			system(sz);
		} 
	}
	return 0;
}

int R8_iR8_j(int argc, char* argv[]){
	int i0=1;
	int j0=1;
	if(argc>1)
	   i0=atoi(argv[1]);
    if(argc>2)
	   j0=atoi(argv[2]);     
	int fun=4;
	if(argc>3){
		fun=atoi(argv[3]);
		if(fun<0||fun>4){
			fun=0;
		}	
	}   
	char* str1="";
    if(argc>4)	
		str1=argv[4];   
	int J=0;	
 	for(int i=i0;i<=52;i++){
		for(int j=(J==0?j0:1);j<=52;j++){
			J=1;
			char sz[1000]={0};
			sprintf(sz,"DPR0 R8%d.txt R8%d.txt R8_%d%R8_%d.txt %d %s",i,j,i,j,fun,str1);
			printf("i=%d,j=%d\n",i,j);
			system(sz);
		} 
	}
	return 0;
}

int R16_iR8_j(int argc, char* argv[]){
	int i0=1;
	int j0=1;
	if(argc>1)
	   i0=atoi(argv[1]);
    if(argc>2)
	   j0=atoi(argv[2]);     
	int fun=4;
	if(argc>3){
		fun=atoi(argv[3]);
		if(fun<0||fun>4){
			fun=0;
		}	
	}   
	char* str1="";
    if(argc>4)	
		str1=argv[4];   
	int J=0;	
 	for(int i=i0;i<=390;i++){
		for(int j=(J==0?j0:1);j<=52;j++){
			J=1;
			char sz[1000]={0};
			sprintf(sz,"DPR0 R16%d.txt R8%d.txt R16_%d%R8_%d.txt %d %s",i,j,i,j,fun,str1);
			printf("i=%d,j=%d\n",i,j);
			system(sz);
		} 
	}
	return 0;
}

int R16_iR4_j(int argc, char* argv[]){
	int i0=1;
	int j0=1;
	if(argc>1)
	   i0=atoi(argv[1]);
    if(argc>2)
	   j0=atoi(argv[2]);     
	int fun=4;
	if(argc>3){
		fun=atoi(argv[3]);
		if(fun<0||fun>4){
			fun=0;
		}	
	}   
	char* str1="";
    if(argc>4)	
		str1=argv[4];   
	int J=0;	
 	for(int i=i0;i<=390;i++){
		for(int j=(J==0?j0:1);j<=11;j++){
			J=1;
			char sz[1000]={0};
			sprintf(sz,"DPR0 R16%d.txt R4%d.txt R16_%d%R4_%d.txt %d %s",i,j,i,j,fun,str1);
			printf("i=%d,j=%d\n",i,j);
			system(sz);
		} 
	}
	return 0;
}

int R16_iR2_j(int argc, char* argv[]){
	int i0=1;
	int j0=1;
	if(argc>1)
	   i0=atoi(argv[1]);
    if(argc>2)
	   j0=atoi(argv[2]);     
	int fun=4;
	if(argc>3){
		fun=atoi(argv[3]);
		if(fun<0||fun>4){
			fun=0;
		}	
	}   
	char* str1="";
    if(argc>4)	
		str1=argv[4];   
	int J=0;	
 	for(int i=i0;i<=390;i++){
		for(int j=(J==0?j0:1);j<=2;j++){
			J=1;
			char sz[1000]={0};
			sprintf(sz,"DPR0 R16%d.txt R2_%d.txt R16_%d%R2_%d.txt %d %s",i,j,i,j,fun,str1);
			printf("i=%d,j=%d\n",i,j);
			system(sz);
		} 
	}
	return 0;
}

int R8_iR4_j(int argc, char* argv[]){
	int i0=1;
	int j0=1;
	if(argc>1)
	   i0=atoi(argv[1]);
    if(argc>2)
	   j0=atoi(argv[2]);     
	int fun=4;
	if(argc>3){
		fun=atoi(argv[3]);
		if(fun<0||fun>4){
			fun=0;
		}	
	}   
	char* str1="";
    if(argc>4)	
		str1=argv[4];   
	int J=0;	
 	for(int i=i0;i<=52;i++){
		for(int j=(J==0?j0:1);j<=11;j++){
			J=1;
			char sz[1000]={0};
			sprintf(sz,"DPR0 R8%d.txt R4%d.txt R8_%d%R4_%d.txt %d %s",i,j,i,j,fun,str1);
			printf("i=%d,j=%d\n",i,j);
			system(sz);
		} 
	}
	return 0;
}

int main(int argc, char* argv[]){   
#ifdef R16R8	
	return R16_iR8_j(argc,argv);
#elif defined(R16R4)
	return R16_iR4_j(argc,argv);
#elif defined(R16R2)
	return R16_iR2_j(argc,argv);	
#elif defined(R8R4)
	return R8_iR4_j(argc,argv);	
#elif defined(R8R8)
	return R8_iR8_j(argc,argv);
#elif defined(R81R3)
	return R81_iR3_j(argc,argv);	
#elif defined(R27R27)
	return R27_iR27_j(argc,argv);	
#elif defined(R27R9)
	return R27_iR9_j(argc,argv);	
#endif
	testR16R81();
	return 0;
}