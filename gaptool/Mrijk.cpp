#define R16_I1I2
#define USE_MNR_UINT32
#include<iostream>
#include<set>
#include"ZmodnZ.h"
#include"M2r.h"
#include"Mnr.h"

int g_i=0;
void findsubring1R8R16(IRing *r)
{
	set<pair<int,int>> M;
	set<string> S;	
	int ID=0;//IdRing(r);
	printf("R%d_%d g_i=%d\n",r->size(),ID,g_i);
	for(int i=g_i;i<r->size();i++)			
	{
		vector<int> v;
		v.push_back(i);				
		Subring S1i;
		bool bn=S1i.init(r,v,16);
		if(!bn)
			continue;
		int ni=S1i.size();
		//if(ni!=16 && ni!=8)
			//continue;
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(ni,ID));
		int cnt1=M.size();
		if(cnt1>cnt){		
			printf("cnt1=%d:R%d_%d->i=%d\n",cnt1,ni,ID,i);			
		}	
		if((ni==16 && ID==-1)||(ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==39)))   
		{					
			string strR=calcRingInvariant(&S1i);
			if(S.find(strR)==S.end()){		
				printf("i=%d->R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",i,ni,ID,strR.c_str());
			}
			S.insert(strR);	
		}		   
	}
}

void findsubring2R8R16(IRing *r)
{
	set<pair<int,int>> M;
	set<string> S;	
	int ID=0;//IdRing(r);
	printf("R%d_%d g_i=%d\n",r->size(),ID,g_i);
	for(int i=g_i;i<r->size()-1;i++)		
	for(int j=i+1;j<r->size();j++)	
	{
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);			
		Subring S1i;
		bool bn=S1i.init(r,v,16);
		if(!bn)
			continue;
		int ni=S1i.size();
		//if(ni!=16 && ni!=8)
			//continue;
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(ni,ID));
		int cnt1=M.size();
		if(cnt1>cnt){		
			printf("cnt1=%d:R%d_%d->i=%d,j=%d\n",cnt1,ni,ID,i,j);			
		}	
		if((ni==16 && ID==-1)||(ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==39)))   
		{					
			string strR=calcRingInvariant(&S1i);
			if(S.find(strR)==S.end()){		
				printf("i=%d,j=%d->R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",i,j,ni,ID,strR.c_str());
			}
			S.insert(strR);	
		}		   
	}
}

void findsubring3R8R16(IRing *r)
{
	set<pair<int,int>> M;
	set<string> S;	
	int ID=0;//IdRing(r);
	printf("R%d_%d g_i=%d\n",r->size(),ID,g_i);
	for(int i=g_i;i<r->size()-2;i++)		
	for(int j=i+1;j<r->size()-1;j++)
	for(int k=j+1;k<r->size();k++)		
	{
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);	
		v.push_back(k);			
		Subring S1i;
		bool bn=S1i.init(r,v,16);
		if(!bn)
			continue;
		int ni=S1i.size();
		//if(ni!=16 && ni!=8)
			//continue;
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(ni,ID));
		int cnt1=M.size();
		if(cnt1>cnt){		
			printf("cnt1=%d:R%d_%d->i=%d,j=%d,k=%d\n",cnt1,ni,ID,i,j,k);			
		}	
		if((ni==16 && ID==-1)||(ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==39)))   
		{					
			string strR=calcRingInvariant(&S1i);
			if(S.find(strR)==S.end()){		
				printf("i=%d,j=%d,k=%d->R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",i,j,k,ni,ID,strR.c_str());
			}
			S.insert(strR);	
		}		   
	}
}

void findsubringR8R16(IRing *r,int ijk=1)
{
	if(ijk>3||ijk<1)
		ijk=1;
	typedef void(*pF)(IRing *r);
	pF Func[]={findsubring1R8R16,findsubring2R8R16,findsubring3R8R16};
	Func[ijk-1](r);
}

int main(int argc, char* argv[])
{ 
    int n=2;
	int n1=1;
	int n2=8;
	int ijk=1;	
	if(argc>1)
		n=atoi(argv[1]);
	if(argc>2)
		n1=atoi(argv[2]);
	if(argc>3)
		n2=atoi(argv[3]); 
	if(argc>4)
		g_i=atoi(argv[4]);
	if(argc>5)
		ijk=atoi(argv[5]);		
	IRing* r=NULL;	
	if(n1==4 && n2<=11){	
		r=M2r::newR4(n2);
	}else if(n2%n1==0){
		r=new ZmodnZ(n1,n2);
	}
	if(n>2||n==1){
		Mnr* R=new Mnr(r,n);
		R->m_flag=1;
		findsubringR8R16(R,ijk);		
		delete R;
		R=NULL;
	}else{
		M2r* R=new M2r(r);
		R->m_flag=1;
		findsubringR8R16(R,ijk);		
		delete R;
		R=NULL;		
	}
   
   return 0;
}