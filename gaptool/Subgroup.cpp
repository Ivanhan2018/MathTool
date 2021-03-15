#include"PermGroup.h"
#include<stdio.h>
#include<stdlib.h>
#include<set>

string V2S(vector<unsigned char>& v){
   string str="";
   int n=v.size();
   for(int i=0;i<n;i++)
   {
	   str+=itos(v[i]);
	   if(i<n-1)
		   str+=",";   
   }	   
   str+="";
   return str;
}

set<string> gS;
set<std::pair<int,int>> gM;
int g_a=0;
void FindPermGroup2(int n,int m)
{	
	for(int i=g_a;i<m;i++)		
	for(int j=i+1;j<m;j++){	
		vector<SnE> S;
		SnE vi=Sn::getSnE(n,i);
		SnE vj=Sn::getSnE(n,j);
		S.push_back(vi);
		S.push_back(vj);
		Sn G;
		bool b=G.init(S,100);
		if(!b)
			continue;
		int ni=G.size();
		//bool bG=IsGroup(&G);
		//if(!bG)
		//	continue;
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(ni,ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			printf("G%d_%d:%d,%d->%s;%s\n",G.size(),ID,i,j,V2S(vi).c_str(),V2S(vj).c_str());				
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

void FindPermGroup3(int n,int m)
{
	for(int i=g_a;i<m;i++)		
	for(int j=i+1;j<m;j++){
	for(int k=j+1;k<m;k++){		
		vector<SnE> S;
		SnE vi=Sn::getSnE(n,i);
		SnE vj=Sn::getSnE(n,j);
		SnE vk=Sn::getSnE(n,k);
		S.push_back(vi);
		S.push_back(vj);
		S.push_back(vk);
		Sn G;
		bool b=G.init(S,100);
		if(!b)
			continue;
		int ni=G.size();
		//bool bG=IsGroup(&G);
		//if(!bG)
		//	continue;
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(ni,ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			printf("G%d_%d:%d,%d,%d->%s;%s;%s\n",G.size(),ID,i,j,k,V2S(vi).c_str(),V2S(vj).c_str(),V2S(vk).c_str());				
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

// 优化后
void QFindPermGroup2(int n,int m)
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
		Subgroup G;
		bool b=G.init(&sn,v,100);
		if(!b)
			continue;
		int ni=G.size();
		//bool bG=IsGroup(&G);
		//if(!bG)
		//	continue;
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(ni,ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			printf("G%d_%d:%d,%d->%s;%s\n",G.size(),ID,i,j,V2S(vi).c_str(),V2S(vj).c_str());				
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

void QFindPermGroup3(int n,int m)
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
		Subgroup G;
		bool b=G.init(&sn,v,100);
		if(!b)
			continue;
		int ni=G.size();
		//bool bG=IsGroup(&G);
		//if(!bG)
		//	continue;
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(ni,ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			printf("G%d_%d:%d,%d,%d->%s;%s;%s\n",G.size(),ID,i,j,k,V2S(vi).c_str(),V2S(vj).c_str(),V2S(vk).c_str());				
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
		printf("Usage:  SubGroup n [g_a] [m] [r]\n");
		return 0;
	}
    int n=atoi(argv[1]);
	g_a=argc>2?atoi(argv[2]):0;		
	int m=argc>3?atoi(argv[3]):0;
	int r=argc>4?3:2;
	if(m==0 && (n<1||n>10))
	{
		return 0;
	}
	if(m==0){
		m=fact(n);
		printf("%d阶置换群S_%d的子群:\n",m,n);
		if(r==2)QFindPermGroup2(n,m);
		if(r==3)QFindPermGroup3(n,m);
	}else{
		printf("置换群S_%d的子群:\n",n);
		if(r==2)FindPermGroup2(n,m);
		if(r==3)FindPermGroup3(n,m);
	}
#ifdef _DEBUG
	system("pause");
#endif	
	return 0;
}