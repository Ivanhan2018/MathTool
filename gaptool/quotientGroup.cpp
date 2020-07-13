#include"PermGroup.h"
#include"quotientGroup.h"
#include<stdio.h>
#include<stdlib.h>
#include<set>

string V2S(vector<unsigned char>& v){
   string str="[";
   int n=v.size();
   for(int i=0;i<n;i++)
   {
	   str+=itos(v[i]);
	   if(i<n-1)
		   str+=",";   
   }	   
   str+="]";
   return str;
}

set<string> gS;
set<std::pair<int,int>> gM;
int g_a=0;

void QFindquotientGroup1(int n,int m)
{	
	Sn sn(n);
	for(int i=g_a;i<m;i++){	
		vector<SnE> S;
		SnE vi=sn.s_Arr[i];
		S.push_back(vi);
		vector<int> v;
		v.push_back(0);
		v.push_back(i);
		Subgroup s;
		bool b=s.init(&sn,v,m/2);
		if(!b)
			continue;
		int ni=s.size();
		if(sn.size()/ni>100||ni==1)
			continue;
		int isN=IsNormalSubgroup(&sn,s.m_Set,true);
		if(isN!=1)
			continue;	
		quotientGroup G(&sn,s.m_Set);		
		//bool bG=IsGroup(&G);
		//if(!bG)
		//	continue;
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(G.size(),ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			int sID=IdGroup(&s);
			printf("%d->%s=>S_%d/GAP[%d,%d]=GAP[%d,%d]:\n",i,V2S(vi).c_str(),n,ni,sID,G.size(),ID);				
		}
		if(ID==-1){		
			string N0=calcN0(&G);  
			string C1=calcC1(&G);	
			string Nk=calcNk(&G);	
			string S2=calcS2(&G);
			string kKEZDCANS=calckKEZDCANS(&G);
			string strG=N0+C1+Nk+S2+kKEZDCANS;
			if(gS.find(strG)==gS.end()){			
				printf("N0C1Nk=%s,%s,%s\n",N0.c_str(),C1.c_str(),Nk.c_str());  
				printf("S2=%s\n",S2.c_str());
				printf("kKEZDCANS=%s\n",kKEZDCANS.c_str());			
			}
			gS.insert(strG);
		}
	}
}

void QFindquotientGroup2(int n,int m)
{	
	Sn sn(n);
	for(int i=g_a;i<m;i++)		
	for(int j=i+1;j<m;j++){	
		vector<SnE> S;
		SnE vi=sn.s_Arr[i];
		SnE vj=sn.s_Arr[j];
		S.push_back(vi);
		S.push_back(vj);
		vector<int> v;
		v.push_back(i);
		v.push_back(j);
		Subgroup s;
		bool b=s.init(&sn,v,m/2);
		if(!b)
			continue;
		int ni=s.size();
		if(sn.size()/ni>100)
			continue;
		int isN=IsNormalSubgroup(&sn,s.m_Set,true);
		if(isN!=1)
			continue;	
		quotientGroup G(&sn,s.m_Set);		
		//bool bG=IsGroup(&G);
		//if(!bG)
		//	continue;
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(G.size(),ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			int sID=IdGroup(&s);
			printf("%d,%d->%s,%s=>S_%d/GAP[%d,%d]=GAP[%d,%d]:\n",i,j,V2S(vi).c_str(),V2S(vj).c_str(),n,ni,sID,G.size(),ID);				
		}
		if(ID==-1){		
			string N0=calcN0(&G);  
			string C1=calcC1(&G);	
			string Nk=calcNk(&G);	
			string S2=calcS2(&G);
			string kKEZDCANS=calckKEZDCANS(&G);
			string strG=N0+C1+Nk+S2+kKEZDCANS;
			if(gS.find(strG)==gS.end()){			
				printf("N0C1Nk=%s,%s,%s\n",N0.c_str(),C1.c_str(),Nk.c_str());  
				printf("S2=%s\n",S2.c_str());
				printf("kKEZDCANS=%s\n",kKEZDCANS.c_str());			
			}
			gS.insert(strG);
		}
	}
}

void QFindquotientGroup3(int n,int m)
{
	Sn sn(n);
	for(int i=g_a;i<m;i++)		
	for(int j=i+1;j<m;j++){
	for(int k=j+1;k<m;k++){		
		vector<SnE> S;
		SnE vi=sn.s_Arr[i];
		SnE vj=sn.s_Arr[j];
		SnE vk=sn.s_Arr[k];
		S.push_back(vi);
		S.push_back(vj);
		S.push_back(vk);
		vector<int> v;
		v.push_back(i);
		v.push_back(j);
		Subgroup s;
		bool b=s.init(&sn,v,m/2);
		if(!b)
			continue;
		int ni=s.size();
		if(sn.size()/ni>100)
			continue;
		int isN=IsNormalSubgroup(&sn,s.m_Set,true);
		if(isN!=1)
			continue;	
		quotientGroup G(&sn,s.m_Set);
		//bool bG=IsGroup(&G);
		//if(!bG)
		//	continue;
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(G.size(),ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			int sID=IdGroup(&s);
			printf("%d,%d,%d->%s,%s,%s=>S_%d/GAP[%d,%d]=GAP[%d,%d]:\n",i,j,k,V2S(vi).c_str(),V2S(vj).c_str(),V2S(vk).c_str(),n,ni,sID,G.size(),ID);				
		}
		if(ID==-1){		
			string N0=calcN0(&G);  
			string C1=calcC1(&G);	
			string Nk=calcNk(&G);	
			string S2=calcS2(&G);
			string kKEZDCANS=calckKEZDCANS(&G); 
			string strG=N0+C1+Nk+S2+kKEZDCANS;
			if(gS.find(strG)==gS.end()){			
				printf("N0C1Nk=%s,%s,%s\n",N0.c_str(),C1.c_str(),Nk.c_str());  
				printf("S2=%s\n",S2.c_str());
				printf("kKEZDCANS=%s\n",kKEZDCANS.c_str());			
			}
			gS.insert(strG);
		}
	}
	}
}

int fact(int n)
{
	static int result[]={1,2,6,24,120,720,5040,40320,362880,362880};
    int idx=(n<1||n>10?0:n-1);
	return result[idx];
}

int main(int argc, char * argv[])
{
	char sz[100]={0};
	if(argc<2)
	{
		printf("Usage:  quotientGroup n [g_a] [r]\n");
		return 0;
	}
    int n=atoi(argv[1]);
	g_a=argc>2?atoi(argv[2]):0;		
	int r=argc>3?atoi(argv[3]):2;
	if(r!=2&&r!=3)
		r=1;
	if(n<1||n>10)
	{
		return 0;
	}
	int m=m=fact(n);
	printf("%d阶置换群S_%d的商群:\n",m,n);
	if(r==1)QFindquotientGroup1(n,m);	
	if(r==2)QFindquotientGroup2(n,m);
	if(r==3)QFindquotientGroup3(n,m);
#ifdef _DEBUG
	system("pause");
#endif	
	return 0;
}