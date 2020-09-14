#include"quotientRing.h"
#include<set>
#include"DecompositionRing.h"
#include"Rn.h"
#include"M2r.h"
#include"Mnr.h"
#include"PolynomialRing.h"
#include <ctime>
#include <fstream>

extern IRing* newRppp(int ID,int p=2,int sID=0);

string calcI2a(IRing* r){
	int IdRing(IRing* r);
	int n=r->size();
	vector<pair<int,int> > v;
   for(int i=0;i<n-1;i++){
	   int j=i+1;
		   vector<int> vi;
		   vi.push_back(i);
		   vi.push_back(j);		   
		   Subring si(r,vi);
		   int ni=si.size();
		   if(ni<n && ni>0){
			int ID=IdRing(&si);
#if 1			
			if(ID==-1 && ni==16){
				printf("i=%d,j=%d\n",i,j);
			    break;
			}
#endif
			v.push_back(make_pair(ni,ID));
		   }
   }
	std::sort(v.begin(),v.end());
	vector<tuple<int,int,int> > v1=doN2Vec(v);
	string str="[";
	for(int i=0;i<v1.size();i++)
	{
		char sz[200]={0};
		sprintf(sz,"[%d,%d,%d],",get<0>(v1[i]),get<1>(v1[i]),get<2>(v1[i]));
		str+=sz;
	}
	if(str.size()>2)
	{
		str=str.substr(0,str.size()-1);
	}
	str+="]";
	return str;
}

void findsubring(M2r *r,int n)
{
#define PRINT_LOG 1	
	bool bFind=false;	
#if PRINT_LOG
    char sz[100]="0";
	sprintf(sz,"R%d_%d.txt",r->size(),time(NULL));
    ofstream fout(sz);
#endif	
    string strCmd="del ";
	strCmd+=sz;	
	for(int i=0;i<r->size()-1;i++)
	for(int j=i+1;j<r->size();j++)
	{
		//int j=i+1;
		vector<int> v;
		v.push_back(i);
		v.push_back(j);		
		Subring S1i(r,v);
		int ni=S1i.size();
		if(n>0 && ni!=n && ni!=8)
			continue;
		int ID=IdRing(&S1i);
		if(n<r->size() && ni==n && ID==-1||(ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==36||ID==39)))   
		{
			string str=M2r::MStr(r->m_Set[i]);
			printf("%d->%s=>",i,str.c_str());
			string strj=M2r::MStr(r->m_Set[j]);
			printf("%d->%s=>",j,strj.c_str());			
			string strR=calcRingInvariant(&S1i);
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",ni,ID,strR.c_str());			
			//S1i.printTable();
#if PRINT_LOG			
			fout<<i<<"->"<<str<<","<<j<<"->"<<strj<<"=>";
			fout<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2N6="<<strR<<endl;
			bFind=true;
#endif			
			break;
		}		   
	}	
#if PRINT_LOG
	fout.close();	
	if(!bFind)	
		system(strCmd.c_str());
	else
		printf("子环表示已输出到文件%s\n",sz);
#endif	   	
}

void findsubring(Mnr *r,int n)
{
#define PRINT_LOG 1	
	bool bFind=false;	
#if PRINT_LOG
    char sz[100]="0";
	sprintf(sz,"R%d_%d.txt",r->size(),time(NULL));
    ofstream fout(sz);
#endif	
    string strCmd="del ";
	strCmd+=sz;
	for(int i=0;i<r->size()-1;i++)
	for(int j=i+1;j<r->size();j++)
	{
		//int j=i+1;
		vector<int> v;
		v.push_back(i);
		v.push_back(j);		
		Subring S1i(r,v);
		int ni=S1i.size();
		if(n>0 && ni!=n && ni!=8)
			continue;
		int ID=IdRing(&S1i);
		if(n<r->size() && ni==n && ID==386 || ID==3052 || ID==5552 || ID==-1||(ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==31||ID==32||ID==39)))   
		{
			string str=Mnr::MStr(r->m_Set[i]);
			printf("%d->%s=>",i,str.c_str());
			string strj=Mnr::MStr(r->m_Set[j]);
			printf("%d->%s=>",j,strj.c_str());			
			string strR=calcRingInvariant(&S1i);
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",ni,ID,strR.c_str());		
			if(ID==3052){
				string I1=calcI1(&S1i);
				string I2=calcI2(&S1i);   
				printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());
			}	
			//S1i.printTable();
#if PRINT_LOG			
			fout<<i<<"->"<<str<<","<<j<<"->"<<strj<<"=>";
			fout<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2N6="<<strR<<endl;
			bFind=true;
#endif
			break;
		}		   
	}
#if PRINT_LOG
	fout.close();	
	if(!bFind)	
		system(strCmd.c_str());
	else
		printf("子环表示已输出到文件%s\n",sz);
#endif	
}

IRing* newRppp(int ID,int p,int sID){	
   if(/* ID!=29 && ID!=46 ID!=26 && ID!=44ID!=36 && ID!=40*/ID!=7)
	   return NULL;
	//p=2;
	//if(p==3)return NULL;
   if(ID==1){
		return new ZmodnZ(p*p*p,p*p*p*p*p*p); 
   }else if(ID==2){
		return new ZmodnZ(p,p*p*p*p);	
   }else if(ID==3){ 
		return new ZmodnZ(1,p*p*p);
   }else if(ID==4){
		return new ZmodnZ(p*p,p*p*p*p*p);
   }else if(ID==5){	
		M2r* r=new M2r();   
		r->m_r=new ZmodnZ(1,p*p*p);
		vector<MATRIXi> gen;	
		MATRIXi A(2,vector<int>(2,0));
		MATRIXi B(2,vector<int>(2,0));		
		A[0][0]=0;
		A[0][1]=p;
		A[1][0]=0;
		A[1][1]=0;
		B[0][0]=p*p;
		B[0][1]=p;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen); 
        return r;	
   }else if(ID==7){
		ZmodnZ* rpp=new ZmodnZ(p*p,p*p*p*p);
		ZmodnZ* rp=new ZmodnZ(1,p);
		DecompositionRing* r= new DecompositionRing(rpp,rp);
		r->m_flag=1;	   
		return r;		
   }else if(ID==8){	
		M2r* r=new M2r();   
		r->m_r=new ZmodnZ(1,p*p);
		vector<MATRIXi> gen;	
		MATRIXi A(2,vector<int>(2,0));
		MATRIXi B(2,vector<int>(2,0));		
		A[0][0]=0;
		A[0][1]=1;
		A[1][0]=0;
		A[1][1]=0;
		B[0][0]=p;
		B[0][1]=0;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen); 
        return r;	
   }else if(ID==10){	
		M2r* r=new M2r();   
		r->m_r=new ZmodnZ(1,p*p);
		vector<MATRIXi> gen;	
		MATRIXi A(2,vector<int>(2,0));
		MATRIXi B(2,vector<int>(2,0));		
		A[0][0]=p;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=0;
		B[0][0]=p;
		B[0][1]=0;
		B[1][0]=1;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen); 
        return r;
   }else if(ID==11){	
		M2r* r=new M2r();   
		r->m_r=new ZmodnZ(1,p*p);
		vector<MATRIXi> gen;	
		MATRIXi A(2,vector<int>(2,0));
		MATRIXi B(2,vector<int>(2,0));		
		A[0][0]=p;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=p;
		B[0][0]=p;
		B[0][1]=1;
		B[1][0]=0;
		B[1][1]=p;
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen); 
        return r;		
   }else if(ID==13){	
		M2r* r=new M2r();   
		r->m_r=new ZmodnZ(1,p*p);
		vector<MATRIXi> gen;	
		MATRIXi A(2,vector<int>(2,0));	
		A[0][0]=p;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=1;  
		gen.push_back(A);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen);
#if 0
        for(int i=0;i<r->size();i++){
           	string str=M2r::MStr(r->m_Set[i]);
			printf("%d->%s=>",i,str.c_str()); 
		}		
#endif		
        return r;
   }else if(ID==14){
		ZmodnZ* rpp=new ZmodnZ(1,p*p);
		ZmodnZ* rp=new ZmodnZ(1,p);
		DecompositionRing* r= new DecompositionRing(rpp,rp);
		r->m_flag=1;	   
		return r;		
   }else if(ID==15){	
		M2r* r=new M2r();   
		r->m_r=new ZmodnZ(1,p*p);
		vector<MATRIXi> gen;	
		MATRIXi A(2,vector<int>(2,0));
		MATRIXi B(2,vector<int>(2,0));		
		A[0][0]=0;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=1;
		B[0][0]=0;
		B[0][1]=p;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen); 
        return r;	
   }else if(ID==16){	
		M2r* r=new M2r();   
		r->m_r=new ZmodnZ(1,p*p*p);
		vector<MATRIXi> gen;	
		MATRIXi A(2,vector<int>(2,0));
		MATRIXi B(2,vector<int>(2,0));		
		A[0][0]=p;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=0;
		B[0][0]=p*p;
		B[0][1]=p*p;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen); 
        return r;
   }else if(ID==17){
		ZmodnZ* rpp=new ZmodnZ(p,p*p*p);
		ZmodnZ* rp=new ZmodnZ(1,p);
		DecompositionRing* r= new DecompositionRing(rpp,rp);
		r->m_flag=1;	   
		return r;		
   }else if(ID==19){//不用下面的2阶矩阵表示Rppp',因为Rppp'(19,2)=R8_19，但Rppp'(19,3)!=R27_19(,Rppp'(19,3)=R27_13、R27_11)
/*
Rppp':
		M2r* r=new M2r();   
		r->m_r=new ZmodnZ(1,p*p);
		vector<MATRIXi> gen;	
		MATRIXi A(2,vector<int>(2,0));
		MATRIXi B(2,vector<int>(2,0));		
		A[0][0]=p;
		A[0][1]=p;
		A[1][0]=0;
		A[1][1]=p;
		B[0][0]=0;
		B[0][1]=1;
		B[1][0]=0;
		B[1][1]=p;
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen); 		
        return r;	
*/   
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p*p);
		r->m_n=3; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(3,vector<TElem>(3,0)); 
		MATRIXi8 B(3,vector<TElem>(3,0)); 	  					
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=p;
		A[1][1]=p;
		A[1][2]=1;
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=0;		
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=p;
		B[1][0]=0;
		B[1][1]=0;
		B[1][2]=1;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=p;		
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
        return r;		
   }else if(ID==20){	
		M2r* r=new M2r();   
		r->m_r=new ZmodnZ(1,p*p);
		vector<MATRIXi> gen;	
		MATRIXi A(2,vector<int>(2,0));
		MATRIXi B(2,vector<int>(2,0));		
		A[0][0]=0;
		A[0][1]=p;
		A[1][0]=0;
		A[1][1]=0;
		B[0][0]=1;
		B[0][1]=p;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen); 
        return r;	
   }else if(ID==21){	
		M2r* r=new M2r();   
		r->m_r=new ZmodnZ(1,p*p);
		vector<MATRIXi> gen;	
		MATRIXi A(2,vector<int>(2,0));		
		A[0][0]=1;
		A[0][1]=p;
		A[1][0]=p;
		A[1][1]=1;
		gen.push_back(A);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen); 
        return r;
   }else if(ID==23){	
		M2r* r=new M2r();   
		r->m_r=new ZmodnZ(1,p*p*p);
		vector<MATRIXi> gen;	
		MATRIXi A(2,vector<int>(2,0));		
		A[0][0]=p;
		A[0][1]=p;
		A[1][0]=0;
		A[1][1]=p;
		gen.push_back(A);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen); 
        return r;		
   }else if(ID==24){	
		M2r* r=new M2r();   
		r->m_r=new ZmodnZ(1,p*p);
		vector<MATRIXi> gen;	
		MATRIXi A(2,vector<int>(2,0));		
		A[0][0]=0;
		A[0][1]=1;
		A[1][0]=p;
		A[1][1]=0;
		gen.push_back(A);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen); 
        return r;
   }else if(ID==25){// 必须有3个生成元	
		M2r* r=new M2r();   
		r->m_r=new ZmodnZ(1,p*p);
		vector<MATRIXi> gen;	
		MATRIXi A(2,vector<int>(2,0));
		MATRIXi B(2,vector<int>(2,0));	
		MATRIXi C(2,vector<int>(2,p));		
		A[0][0]=p;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=0;
		B[0][0]=0;
		B[0][1]=0;
		B[1][0]=0;
		B[1][1]=p;
		gen.push_back(A);
		gen.push_back(B);
		gen.push_back(C);		
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen); 
#if 0
        for(int i=0;i<r->size();i++){
				for(int j=0;j<r->size();j++){
				vector<int> vi;
				vi.push_back(i);
				vi.push_back(j);		   
				Subring si(r,vi);
				int ni=si.size();
				if(ni==r->size()){		   
					string str=M2r::MStr(r->m_Set[i]);
					printf("%d->%s=>",i,str.c_str()); 
					string strj=M2r::MStr(r->m_Set[j]);
					printf("%d->%s=>",j,strj.c_str()); 	
					break;			
				}
			}	
		}		
#endif		
        return r;
   }else if(ID==26){
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		MATRIXi8 B(4,vector<TElem>(4,0)); 		
		A[0][0]=1;
		A[0][1]=1;
		A[0][2]=0;
		A[0][3]=0;		
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=0;
		A[1][3]=1;	
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=0;
		A[2][3]=1;
		A[3][0]=1;
		A[3][1]=1;
		A[3][2]=0;
		A[3][3]=0;	   
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=1;
		B[0][3]=1;		
		B[1][0]=1;
		B[1][1]=0;
		B[1][2]=1;
		B[1][3]=0;		
		B[2][0]=1;
		B[2][1]=0;
		B[2][2]=1;
		B[2][3]=0;
		B[3][0]=0;
		B[3][1]=0;
		B[3][2]=1;
		B[3][3]=1;		
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
		return r;	
   }else if(ID==27){
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		MATRIXi8 B(4,vector<TElem>(4,0)); 		
		A[3][2]=1;
		B[1][1]=1;
		B[3][0]=1;
		gen.push_back(A);
		gen.push_back(B);		
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 	
		return r;		
   }else if(ID==28){	
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=3; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(3,vector<TElem>(3,0)); 
		MATRIXi8 B(3,vector<TElem>(3,0)); 	  					
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=1;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=1;
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=1;		
		B[0][0]=0;
		B[0][1]=1;
		B[0][2]=1;
		B[1][0]=0;
		B[1][1]=1;
		B[1][2]=1;
		B[2][0]=0;
		B[2][1]=1;
		B[2][2]=1;		
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
        return r;			
   }else if(ID==29){//Rppp(29,2)=R8_29，但Rppp(29,3)=R27_49	
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=3; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(3,vector<TElem>(3,0)); 
		MATRIXi8 B(3,vector<TElem>(3,0)); 	  					
		A[0][0]=1;
		A[0][1]=0;
		A[0][2]=1;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=1;
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=1;		
		B[0][0]=1;
		B[0][1]=0;
		B[0][2]=1;
		B[1][0]=0;
		B[1][1]=0;
		B[1][2]=0;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=0;	
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
        return r;
   }else if(ID==30){// 必须有3个生成元
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		MATRIXi8 B(4,vector<TElem>(4,0)); 		
		MATRIXi8 C(4,vector<TElem>(4,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[0][3]=0;
		A[1][0]=1;
		A[1][1]=1;
		A[1][2]=0;
		A[1][3]=0; 
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=1;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=1;
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[0][3]=0;
		B[1][0]=1;
		B[1][1]=1;
		B[1][2]=0;
		B[1][3]=0; 
		B[2][0]=1;
		B[2][1]=0;
		B[2][2]=1;
		B[2][3]=0;
		B[3][0]=0;
		B[3][1]=0;
		B[3][2]=0;
		B[3][3]=1;
		C[0][0]=0;
		C[0][1]=0;
		C[0][2]=0;
		C[0][3]=0;
		C[1][0]=1;
		C[1][1]=1;
		C[1][2]=0;
		C[1][3]=0; 
		C[2][0]=0;
		C[2][1]=0;
		C[2][2]=1;
		C[2][3]=0;
		C[3][0]=1;
		C[3][1]=0;
		C[3][2]=0;
		C[3][3]=1;		
		gen.push_back(A);
		gen.push_back(B);
		gen.push_back(C);		
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 		
		return r;
   }else if(ID==32){
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		MATRIXi8 B(4,vector<TElem>(4,0)); 		
		A[0][3]=1;
		A[1][1]=1;
		A[1][2]=1;
		A[1][3]=1; 
		A[2][1]=1;
		A[2][2]=1;
		A[2][3]=1;
		B[0][3]=1;
		B[1][0]=1;
		B[2][0]=1;
		B[2][3]=1;
		gen.push_back(A);
		gen.push_back(B);		
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 	
		return r;		
   }else if(ID==33){	
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		A[0][0]=1;
		A[0][1]=0;
		A[0][2]=0;
		A[0][3]=1;
		A[1][0]=1;
		A[1][1]=0;
		A[1][2]=0;
		A[1][3]=0; 
		A[2][0]=1;
		A[2][1]=1;
		A[2][2]=0;
		A[2][3]=1;
		A[3][0]=1;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=1;
		gen.push_back(A);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
		return r;			
   }else if(ID==34){//Rppp(34,2)=R8_34，但Rppp(34,3)=R27_49	
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		MATRIXi8 B(4,vector<TElem>(4,1)); 		
		A[0][0]=1;
		A[0][1]=0;
		A[0][2]=0;
		A[0][3]=1;
		A[1][0]=1;
		A[1][1]=0;
		A[1][2]=0;
		A[1][3]=1; 
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=1;
		A[2][3]=0;
		A[3][0]=1;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=1;	
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
		return r;		
   }else if(ID==340){//Rppp(34,2)=R8_34，但Rppp(34,3)=R27_28	
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=3; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(3,vector<TElem>(3,0)); 
		MATRIXi8 B(3,vector<TElem>(3,0)); 	  					
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=1;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=1;
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=0;		
		B[0][0]=0;
		B[0][1]=1;
		B[0][2]=1;
		B[1][0]=0;
		B[1][1]=0;
		B[1][2]=0;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=0;		
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
        return r;	
   }else if(ID==35){	
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[0][3]=0;
		A[1][0]=0;
		A[1][1]=0;
		A[1][2]=0;
		A[1][3]=1; 
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=1;
		A[2][3]=0;
		A[3][0]=1;
		A[3][1]=0;
		A[3][2]=1;
		A[3][3]=1;	
		gen.push_back(A);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
		return r;	
   }else if(ID==36){
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		MATRIXi8 B(4,vector<TElem>(4,0)); 		
		A[2][2]=1;
		A[3][3]=1;		
		B[1][3]=1;
		B[2][0]=1;
		gen.push_back(A);
		gen.push_back(B);		
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 	
		return r;		
   }else if(ID==37){	
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=3; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(3,vector<TElem>(3,0)); 
		MATRIXi8 B(3,vector<TElem>(3,0)); 	  					
		A[0][0]=1;
		A[0][1]=1;
		A[0][2]=0;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=0;
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=0;		
		B[0][0]=0;
		B[0][1]=1;
		B[0][2]=1;
		B[1][0]=0;
		B[1][1]=0;
		B[1][2]=0;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=0;		
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
        return r;	
   }else if(ID==38){	
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		A[0][0]=0;
		A[0][1]=1;
		A[0][2]=0;
		A[0][3]=1;
		A[1][0]=1;
		A[1][1]=1;
		A[1][2]=0;
		A[1][3]=0; 
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=1;
		A[2][3]=1;
		A[3][0]=0;
		A[3][1]=1;
		A[3][2]=0;
		A[3][3]=1;	
		gen.push_back(A);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
		return r;	
   }else if(ID==39){	
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		MATRIXi8 B(4,vector<TElem>(4,0)); 	  					
		A[2][0]=1;
		A[2][3]=1;
		A[3][0]=1;
		B[1][0]=1;
		B[2][1]=1;
		B[3][0]=1;
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
        return r;		
   }else if(ID==40){//Rppp(40,2)=R8_40，但Rppp(40,3)=R27_38
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		MATRIXi8 B(4,vector<TElem>(4,0)); 		
		A[3][3]=1;
		B[1][2]=1;
		B[2][0]=1;
		B[2][2]=1;
		gen.push_back(A);
		gen.push_back(B);		
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 	
		return r;		
   }else if(ID==41){	
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		MATRIXi8 B(4,vector<TElem>(4,0)); 	  					
		A[0][0]=1;
		A[0][3]=1;		
		A[1][1]=1;
		B[0][0]=1;
		B[0][3]=1;
		B[1][3]=1;		
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
        return r;
   }else if(ID==42){	
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		A[0][0]=1;
		A[0][1]=1;
		A[0][2]=0;
		A[0][3]=1;
		A[1][0]=1;
		A[1][1]=0;
		A[1][2]=0;
		A[1][3]=1; 
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=1;
		A[2][3]=0;
		A[3][0]=1;
		A[3][1]=0;
		A[3][2]=1;
		A[3][3]=1;		
		gen.push_back(A);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
        return r;		
   }else if(ID==43){// 必须有3个生成元
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		MATRIXi8 B(4,vector<TElem>(4,0)); 		
		MATRIXi8 C(4,vector<TElem>(4,0));
		A[0][0]=0;
		A[0][1]=1;
		A[0][2]=0;
		A[0][3]=0;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=0;
		A[1][3]=0; 
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=1;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=1;
		B[0][0]=0;
		B[0][1]=1;
		B[0][2]=1;
		B[0][3]=1;
		B[1][0]=0;
		B[1][1]=1;
		B[1][2]=0;
		B[1][3]=0; 
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=1;
		B[2][3]=0;
		B[3][0]=0;
		B[3][1]=0;
		B[3][2]=0;
		B[3][3]=1;
		C[0][0]=0;
		C[0][1]=1;
		C[0][2]=1;
		C[0][3]=0;
		C[1][0]=0;
		C[1][1]=1;
		C[1][2]=0;
		C[1][3]=0; 
		C[2][0]=0;
		C[2][1]=0;
		C[2][2]=1;
		C[2][3]=0;
		C[3][0]=0;
		C[3][1]=0;
		C[3][2]=0;
		C[3][3]=1;	
		gen.push_back(A);
		gen.push_back(B);
		gen.push_back(C);		
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
#if 0
        for(int i=0;i<r->size();i++){
				for(int j=0;j<r->size();j++){
				vector<int> vi;
				vi.push_back(i);
				vi.push_back(j);		   
				Subring si(r,vi);
				int ni=si.size();
				if(ni==r->size()){		   
					string str=Mnr::MStr(r->m_Set[i]);
					printf("%d->%s=>",i,str.c_str()); 
					string strj=Mnr::MStr(r->m_Set[j]);
					printf("%d->%s=>",j,strj.c_str()); 	
					break;			
				}
			}	
		}		
#endif		
		return r;		
   }else if(ID==44){
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=3; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(3,vector<TElem>(3,0)); 
		MATRIXi8 B(3,vector<TElem>(3,0));
/*	
不用下面的3阶矩阵表示Rppp',因为Rppp'(44,2)=R8_44，但Rppp'(44,3)!=R27_44(,Rppp'(44,3)=R27_47	
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=0;
		A[1][1]=0;
		A[1][2]=1;
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=0;		
		B[0][0]=0;
		B[0][1]=1;
		B[0][2]=1;
		B[1][0]=0;
		B[1][1]=0;
		B[1][2]=0;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=0;		
		*/
		A[0][0]=1;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=0; 
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=0;
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=1;
		B[1][1]=0;
		B[1][2]=0;
		B[2][0]=1;
		B[2][1]=0;
		B[2][2]=0;		
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
        return r;					
   }else if(ID==45){//Rppp(45,2)=R8_45，但Rppp(45,3)=R27_49	
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=3; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(3,vector<TElem>(3,0)); 
		MATRIXi8 B(3,vector<TElem>(3,0)); 	  					
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=1;
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=1;		
		B[0][0]=1;
		B[0][1]=1;
		B[0][2]=1;
		B[1][0]=0;
		B[1][1]=1;
		B[1][2]=0;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=1;		
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
        return r;
   }else if(ID==46){//Rppp(46,2)=R8_46，但Rppp(46,3)=R27_48、R27_51
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=3; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(3,vector<TElem>(3,0)); 
#if 1
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=1;
		A[1][0]=1;
		A[1][1]=0;
		A[1][2]=0;
		A[2][0]=1;
		A[2][1]=1;
		A[2][2]=1;
#else		
		A[0][0]=1;
		A[0][1]=0;
		A[0][2]=1;
		A[1][0]=0;
		A[1][1]=0;
		A[1][2]=1;
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=0;
#endif		
		gen.push_back(A);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
        return r;		
   }else if(ID==47){	
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		MATRIXi8 B(4,vector<TElem>(4,0)); 	  					
		A[0][0]=1;
		A[1][3]=1;		
		A[3][3]=1;
		B[0][0]=1;
		B[0][3]=1;
		B[1][3]=1;	
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
        return r;	
   }else if(ID==48){	
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		MATRIXi8 B(4,vector<TElem>(4,0)); 	  					
		A[0][0]=1;
		A[1][1]=1;		
		A[1][3]=1;
		A[3][3]=1;		
		B[0][0]=1;
		B[0][3]=1;	
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
        return r;		
   }else if(ID==49){	
		M2r* r=new M2r();   
		r->m_r=new ZmodnZ(1,p);
		vector<MATRIXi> gen;	
		MATRIXi A(2,vector<int>(2,0));
		MATRIXi B(2,vector<int>(2,0));		
		A[0][0]=1;
		A[0][1]=1;
		A[1][0]=0;
		A[1][1]=1;
		B[0][0]=1;
		B[0][1]=0;
		B[1][0]=0;
		B[1][1]=0;
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen); 
        return r;	
   }else if(ID==50){	
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=3; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(3,vector<TElem>(3,0)); 
		MATRIXi8 B(3,vector<TElem>(3,0)); 	  					
		A[0][0]=1;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=0;
		A[1][1]=0;
		A[1][2]=0;
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=1;		
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=0;
		B[1][1]=1;
		B[1][2]=0;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=1;		
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
        return r;		
   }else if(ID==51){
        //p=2;	   
		M2r* r=new M2r();   
		//r->m_r=new ZmodnZ(1,p*p);
		M2r *r1=new M2r();
		r1->initK(p);
		r->m_r=r1;			
		vector<MATRIXi> gen;	
		MATRIXi A(2,vector<int>(2,0));	
		A[0][0]=1;
		A[0][1]=1;
		A[1][0]=1;
		A[1][1]=0;  	
		gen.push_back(A);		
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen);
#if 0
        for(int i=0;i<r->size();i++){
           	string str=M2r::MStr(r->m_Set[i]);
			printf("%d->%s=>",i,str.c_str()); 
		}		
#endif		
        return r;
   }else if(ID==52){	
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 
		A[0][0]=1;
		A[0][1]=1;
		A[0][2]=1;
		A[0][3]=1;
		A[1][0]=1;
		A[1][1]=0;
		A[1][2]=1;
		A[1][3]=1; 
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=1;
		A[2][3]=0;
		A[3][0]=1;
		A[3][1]=0;
		A[3][2]=1;
		A[3][3]=1;		
		gen.push_back(A);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
		return r;	
   }else if(ID==56){//Rppp(56,3)=R27_56,但Rppp(56,2)=R8_26
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;	   
		MATRIXi8 A(4,vector<TElem>(4,0));
		MATRIXi8 B(4,vector<TElem>(4,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[0][3]=1;
		A[1][0]=1;
		A[1][1]=0;
		A[1][2]=0;
		A[1][3]=1; 
		A[2][0]=1;
		A[2][1]=0;
		A[2][2]=0;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=0;
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[0][3]=0;
		B[1][0]=0;
		B[1][1]=0;
		B[1][2]=0;
		B[1][3]=1;
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=0;
		B[2][3]=0;
		B[3][0]=0;
		B[3][1]=0;
		B[3][2]=0;
		B[3][3]=0;	
		gen.push_back(A);
		gen.push_back(B);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen);		
		return r;		
   }else if(ID==59){//Rppp(59,3)=R27_59,但Rppp(59,2)=R4_6
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=4; 
		vector<MATRIXi8> gen;		
		MATRIXi8 A(4,vector<TElem>(4,0)); 		
		A[0][0]=0;
		A[0][1]=1;
		A[0][2]=1;
		A[0][3]=1;		
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=1;
		A[1][3]=1;	
		A[2][0]=1;
		A[2][1]=1;
		A[2][2]=1;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=1;
		A[3][3]=0;	   	
		gen.push_back(A);
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
		return r;			
   }else{
		return NULL;
   }		
}

IRing* newRpp(int ID,int p){
   if(ID==1){
		return new ZmodnZ(p*p,p*p*p*p); 
   }else if(ID==2){
		return new ZmodnZ(p,p*p*p);	
   }else if(ID==3){ 
		return new ZmodnZ(1,p*p);
   }else if(ID==4){
		M2r *r4=new M2r();
		r4->initD(p);
		return r4;	
   }else if(ID==5){
		Mnr *r4=new Mnr();
		r4->initE(p);	
		return r4;
   }else if(ID==6){
		Mnr *r4=new Mnr();
		r4->initF(p);	
		return r4;	
   }else if(ID==7){
		M2r *r4=new M2r();
		r4->initG(p);	
		return r4;	
   }else if(ID==8){
		M2r *r4=new M2r();
		r4->initH(p);	
		return r4;	
   }else if(ID==9){
		M2r *r4=new M2r();
		r4->initI(p);	
		return r4;	
   }else if(ID==10){
		M2r *r4=new M2r();
		r4->initJ(p);
		return r4;		
   }else if(ID==11){
		M2r *r4=new M2r();
		r4->initK(p);
		return r4;	
   }else{
		return NULL;
   }	
}

int main()
{ 
	if(1){
		for(int i=1;i<=59;i++){
		   IRing* r8=newRppp(i,2);
		   IRing* r27=newRppp(i,3);		   
		   if(!r8||!r27)
			   continue;
			int ID8=IdRing(r8);
			int ID27=IdRing(r27);			
			#if 0
			    printf("Rppp(%d,2)=R%d_%d,Rppp(%d,3)=R%d_%d\n",i,r8->size(),ID8,i,r27->size(),ID27);
			#else
				string str=calcRingInvariant(r8);
				printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",r8->size(),ID8,str.c_str());
				string I1=calcI1(r8);
				string I2=calcI2(r8);   
				printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());				
				str=calcRingInvariant(r27);
				printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",r27->size(),ID27,str.c_str());
				I1=calcI1(r27);
				I2=calcI2(r27);   
				printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());
			#endif		
		}
	}
    return 0;
}