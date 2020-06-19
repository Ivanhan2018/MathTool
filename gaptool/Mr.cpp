#define USE_MNR_UINT32
#include<iostream>
#include"ZmodnZ.h"
#include"M2r.h"
#include"Mnr.h"

int main(int argc, char* argv[])
{ 
    int n=2;
	int n1=1;
	int n2=8;
	int idx1=0;
	int idx2=1;	
	int idx3=2;		
	if(argc>1)
		n=atoi(argv[1]);
	if(argc>2)
		n1=atoi(argv[2]);
	if(argc>3)
		n2=atoi(argv[3]); 
	if(argc>4)
		idx1=atoi(argv[4]);
	if(argc>5)
		idx2=atoi(argv[5]);
	if(argc>6)
		idx3=atoi(argv[6]);
	IRing* r=NULL;	
	if(n1==4 && n2<=11){	
		r=M2r::newR4(n2);
	}else if(n2%n1==0){
		r=new ZmodnZ(n1,n2);
	}
	if(n>2||(n==2 && n1==4 && (n2==5||n2==6))){
		Mnr* R=new Mnr(r,n);
		R->m_flag=1;
		string str1=Mnr::MStr(R->m_Set[idx1]);
		printf("%d->%s\n",idx1,str1.c_str());
		string str2=Mnr::MStr(R->m_Set[idx2]);
		printf("%d->%s\n",idx2,str2.c_str());	
		string str3=Mnr::MStr(R->m_Set[idx3]);
		printf("%d->%s\n",idx3,str3.c_str());
		vector<int> v;
		v.push_back(idx1);		
		v.push_back(idx2);	
		v.push_back(idx3);			
		Subring S(R,v);		
		int ID=IdRing(&S);
		cout<<"}else if(ID=="<<ID<<"){//R"<<S.size()<<"_"<<ID<<endl;
		if(n1==4 && n2==4){
			cout<<"M2r *D4=new M2r();"<<endl;
			cout<<"D4->initD(2);"<<endl;
			cout<<"m_r=D4;"<<endl;	
		}else if(n1==4 && n2==5){
			cout<<"Mnr *E4=new Mnr();"<<endl;
			cout<<"E4->initE(2);"<<endl;
			cout<<"m_r=E4;"<<endl;				
		}else if(n1==4 && n2==6){
			cout<<"Mnr *F4=new Mnr();"<<endl;
			cout<<"F4->initF(2);"<<endl;
			cout<<"m_r=F4;"<<endl;	
		}else if(n1==4 && n2==7){
			cout<<"M2r *G4=new M2r();"<<endl;
			cout<<"G4->initG(2);"<<endl;
			cout<<"m_r=G4;"<<endl;			
		}else if(n1==4 && n2==8){
			cout<<"M2r *H4=new M2r();"<<endl;
			cout<<"H4->initH(2);"<<endl;
			cout<<"m_r=H4;"<<endl;			
		}else if(n1==4 && n2==9){
			cout<<"M2r *I4=new M2r();"<<endl;
			cout<<"I4->initI(2);"<<endl;
			cout<<"m_r=I4;"<<endl;
		}else if(n1==4 && n2==10){
			cout<<"M2r *J4=new M2r();"<<endl;
			cout<<"J4->initJ(2);"<<endl;
			cout<<"m_r=J4;"<<endl;	
		}else if(n1==4 && n2==11){
			cout<<"M2r *K4=new M2r();"<<endl;
			cout<<"K4->initK(2);"<<endl;
			cout<<"m_r=K4;"<<endl;			
		}else{	
			cout<<"m_r=new ZmodnZ("<<n1<<","<<n2<<");"<<endl;
		}
		cout<<"m_n="<<n<<";"<<endl;
		cout<<"MATRIXi8 A("<<n<<",vector<TElem>("<<n<<",0));"<<endl;
		cout<<"MATRIXi8 B("<<n<<",vector<TElem>("<<n<<",0));"<<endl;	
		cout<<"MATRIXi8 C("<<n<<",vector<TElem>("<<n<<",0));"<<endl;		
		for(int i=0;i<n;i++)  
			for(int j=0;j<n;j++) 
				cout<<"A["<<i<<"]["<<j<<"]="<<R->m_Set[idx1][i][j]<<";"<<endl;
		for(int i=0;i<n;i++)  
			for(int j=0;j<n;j++) 
				cout<<"B["<<i<<"]["<<j<<"]="<<R->m_Set[idx2][i][j]<<";"<<endl;
		for(int i=0;i<n;i++)  
			for(int j=0;j<n;j++) 
				cout<<"C["<<i<<"]["<<j<<"]="<<R->m_Set[idx3][i][j]<<";"<<endl;
		cout<<"gen.push_back(A);"<<endl;
		cout<<"gen.push_back(B);"<<endl;
		cout<<"gen.push_back(C);"<<endl;		
		delete R;
		R=NULL;
	}else{
		M2r* R=new M2r(r);
		R->m_flag=1;
		string str1=M2r::MStr(R->m_Set[idx1]);
		printf("%d->%s\n",idx1,str1.c_str());
		string str2=M2r::MStr(R->m_Set[idx2]);
		printf("%d->%s\n",idx2,str2.c_str());	
		string str3=M2r::MStr(R->m_Set[idx3]);
		printf("%d->%s\n",idx3,str3.c_str());
		vector<int> v;
		v.push_back(idx1);		
		v.push_back(idx2);	
		v.push_back(idx3);			
		Subring S(R,v);		
		int ID=IdRing(&S);
		cout<<"}else if(ID=="<<ID<<"){//R"<<S.size()<<"_"<<ID<<endl;		
		if(n1==4 && n2==4){
			cout<<"M2r *D4=new M2r();"<<endl;
			cout<<"D4->initD(2);"<<endl;
			cout<<"m_r=D4;"<<endl;	
		}else if(n1==4 && n2==7){
			cout<<"M2r *G4=new M2r();"<<endl;
			cout<<"G4->initG(2);"<<endl;
			cout<<"m_r=G4;"<<endl;			
		}else if(n1==4 && n2==8){
			cout<<"M2r *H4=new M2r();"<<endl;
			cout<<"H4->initH(2);"<<endl;
			cout<<"m_r=H4;"<<endl;			
		}else if(n1==4 && n2==9){
			cout<<"M2r *I4=new M2r();"<<endl;
			cout<<"I4->initI(2);"<<endl;
			cout<<"m_r=I4;"<<endl;
		}else if(n1==4 && n2==10){
			cout<<"M2r *J4=new M2r();"<<endl;
			cout<<"J4->initJ(2);"<<endl;
			cout<<"m_r=J4;"<<endl;	
		}else if(n1==4 && n2==11){
			cout<<"M2r *K4=new M2r();"<<endl;
			cout<<"K4->initK(2);"<<endl;
			cout<<"m_r=K4;"<<endl;				
		}else{	
			cout<<"m_r=new ZmodnZ("<<n1<<","<<n2<<");"<<endl;
		}
		for(int i=0;i<2;i++)  
			for(int j=0;j<2;j++) 
				cout<<"A["<<i<<"]["<<j<<"]="<<R->m_Set[idx1][i][j]<<";"<<endl;
		for(int i=0;i<2;i++)  
			for(int j=0;j<2;j++) 
				cout<<"B["<<i<<"]["<<j<<"]="<<R->m_Set[idx2][i][j]<<";"<<endl;
		for(int i=0;i<2;i++)  
			for(int j=0;j<2;j++) 
				cout<<"C["<<i<<"]["<<j<<"]="<<R->m_Set[idx3][i][j]<<";"<<endl;
		cout<<"gen.push_back(A);"<<endl;
		cout<<"gen.push_back(B);"<<endl;
		cout<<"gen.push_back(C);"<<endl;		
		delete R;
		R=NULL;		
	}
   
   return 0;
}