#define USE_MNR_UINT32
#include<iostream>
#include<set>
#include"ZmodnZ.h"
#include"M2r.h"
#include"Mnr.h"

int g_i=0;
void findsubringR8R16(IRing *r)
{
	set<pair<int,int>> M;
	set<string> S;	
	int ID=IdRing(r);
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
		if(ni!=16 && ni!=8)
			continue;
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

int main(int argc, char* argv[])
{ 
    int n=2;
	int n1=1;
	int n2=8;		
	if(argc>1)
		n=atoi(argv[1]);
	if(argc>2)
		n1=atoi(argv[2]);
	if(argc>3)
		n2=atoi(argv[3]); 
	if(argc>4)
		g_i=atoi(argv[4]);
	IRing* r=NULL;	
	if(n1==4 && n2<=11){	
		r=M2r::newR4(n2);
	}else if(n2%n1==0){
		r=new ZmodnZ(n1,n2);
	}
	if(n>2){
		Mnr* R=new Mnr(r,n);
		R->m_flag=1;
		findsubringR8R16(R);		
		delete R;
		R=NULL;
	}else{
		M2r* R=new M2r(r);
		R->m_flag=1;
		findsubringR8R16(R);		
		delete R;
		R=NULL;		
	}
   
   return 0;
}