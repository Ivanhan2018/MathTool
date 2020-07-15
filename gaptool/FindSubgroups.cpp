#include "FiniteGroup.h"
#include "quotientGroup.h"
#include <fstream>
#include <set>

IGroup* newG(int n,const char* sz){
	int iElem;
	int sum=0;
	ifstream fin(sz);
	if(!fin)
		return NULL;	
	int *g_GMul=new int[n*n];
	while( !fin.eof() && sum<n*n) {
		fin >> iElem;
		sum++;
		int i=(sum-1)/n;
		int j=(sum-1)%n;
		*(g_GMul+i*n+j)=iElem-1;
	}
	fin.close();
	
	FiniteGroup *g=new FiniteGroup(n,g_GMul,0);
	g->m_flag=1;
	return g;
}

void print(IGroup* g,bool More){
	int ID=IdGroup(g);
	printf("GAP[%d,%d]:\n",g->size(),ID); 
	if(More){
		string N0=calcN0(g);   
		string S2=calcS2(g);
		string kKEZDCANS=calckKEZDCANS(g);	
		printf("N0=%s\n",N0.c_str());  
		printf("S2=%s\n",S2.c_str());
		printf("kKEZDCANS=%s\n",kKEZDCANS.c_str());
		string I1=calcI1(g);
		string I2=calcI2(g);
		printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());

		vector<int> z=CenterOfG(g);
		vector<int> d=commutatorOfG(g);	
		Subgroup Z(g,z);
		Subgroup D(g,d);
		int ZID=IdGroup(&Z);
		int DID=IdGroup(&D);	
		printf("Center(GAP[%d,%d])=GAP[%d,%d]\n",g->size(),ID,Z.size(),ZID);	
		printf("DerivedSubgroup(GAP[%d,%d])=GAP[%d,%d]\n",g->size(),ID,D.size(),DID);
		
		quotientGroup PZ(g,z);
		quotientGroup PD(g,d);
		int PZID=IdGroup(&PZ);
		int PDID=IdGroup(&PD);
		printf("PZ(GAP[%d,%d])=GAP[%d,%d]\n",g->size(),ID,PZ.size(),PZID);	
		printf("PD(GAP[%d,%d])=GAP[%d,%d]\n",g->size(),ID,PD.size(),PDID);	
		//printf("\n");
	}
}

set<string> gS;
set<std::pair<int,int>> gM;
int g_a=0;
void fsg2(IGroup *g,int N)
{	
    int m=g->size();
	for(int i=g_a;i<m;i++)		
	for(int j=i+1;j<m;j++){	
		vector<int> v;
		v.push_back(i);
		v.push_back(j);
		Subgroup G;
		bool b=G.init(g,v,N);
		if(!b)
			continue;
		int ni=G.size();
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(ni,ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			printf("%d,%d=>GAP[%d,%d]:\n",i,j,G.size(),ID);				
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

void fsg3(IGroup *g,int N)
{	
    int m=g->size();
	for(int i=g_a;i<m;i++)		
	for(int j=i+1;j<m;j++){	
	for(int k=j+1;k<m;k++){	
		vector<int> v;
		v.push_back(i);
		v.push_back(j);
		v.push_back(k);		
		Subgroup G;
		bool b=G.init(g,v,N);
		if(!b)
			continue;
		int ni=G.size();
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(ni,ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			printf("%d,%d,%d=>GAP[%d,%d]:\n",i,j,k,G.size(),ID);				
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

void fqg1(IGroup *g,int N)
{	
    int m=g->size();
	for(int i=g_a;i<m;i++){	
		vector<int> v;
		v.push_back(i);
		Subgroup s;
		bool b=s.init(g,v,m/2);
		if(!b)
			continue;
		int ni=s.size();
		if(m/ni>N||ni==1)
			continue;
		int isN=IsNormalSubgroup(g,s.m_Set,true);
		if(isN!=1)
			continue;	
		quotientGroup G(g,s.m_Set);		
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(G.size(),ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			int sID=IdGroup(&s);
			printf("%d=>G/GAP[%d,%d]=GAP[%d,%d]:\n",i,ni,sID,G.size(),ID);				
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

void fqg2(IGroup *g,int N)
{	
    int m=g->size();
	for(int i=g_a;i<m;i++)		
	for(int j=i+1;j<m;j++){	
		vector<int> v;
		v.push_back(i);
		v.push_back(j);		
		Subgroup s;
		bool b=s.init(g,v,m/2);
		if(!b)
			continue;
		int ni=s.size();
		if(m/ni>N)
			continue;
		int isN=IsNormalSubgroup(g,s.m_Set,true);
		if(isN!=1)
			continue;	
		quotientGroup G(g,s.m_Set);		
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(G.size(),ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			int sID=IdGroup(&s);
			printf("%d,%d=>G/GAP[%d,%d]=GAP[%d,%d]:\n",i,j,ni,sID,G.size(),ID);				
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

void fqg3(IGroup *g,int N)
{	
    int m=g->size();
	for(int i=g_a;i<m;i++)		
	for(int j=i+1;j<m;j++){
	for(int k=j+1;k<m;k++){		
		vector<int> v;
		v.push_back(i);
		v.push_back(j);
		v.push_back(k);		
		Subgroup s;
		bool b=s.init(g,v,m/2);
		if(!b)
			continue;
		int ni=s.size();
		if(m/ni>N)
			continue;
		int isN=IsNormalSubgroup(g,s.m_Set,true);
		if(isN!=1)
			continue;	
		quotientGroup G(g,s.m_Set);
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(G.size(),ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			int sID=IdGroup(&s);
			printf("%d,%d,%d=>G/GAP[%d,%d]=GAP[%d,%d]:\n",i,j,k,ni,sID,G.size(),ID);				
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

void findfunc(IGroup *g,int func,int N)
{
	typedef void(*pF)(IGroup *g,int N);
	pF Func[]={fsg2,fsg3,fqg1,fqg2,fqg3};
	int cnt=sizeof(Func)/sizeof(Func[0]);
	if(func<0||func>=cnt)
		func=0;	
	Func[func](g,N);
}

int main(int argc, char* argv[])
{
	if(argc<3)
	{
		printf("Usage:  FindSubgroups fin n [func] [g_a] [N]\n");
		return 0;
	}
    int n=atoi(argv[2]);
	int func=argc>3?atoi(argv[3]):0;	
	g_a=argc>4?atoi(argv[4]):0;
	int N=argc>5?atoi(argv[5]):72;	
	IGroup* g=newG(n<0?-n:n,argv[1]);
	if(g){
		print(g,n<0);
		findfunc(g,func,N);
		delete g;
		g=NULL;
	}

	return 0;
}