#include<set>
#include"DecompositionRing.h"
#include"Rn.h"
#include"M2r.h"
#include"Mnr.h"
#include"PolynomialRing.h"
#include <ctime>
#include <fstream>
#include <windows.h>

int g_i=0;
void fsubring(M2r *r,int n)
{
	map<pair<int,int>,pair<int,int>> M;
	printf("g_i=%d\n",g_i);
	for(int i=g_i;i<r->size()-1;i++)	
	for(int j=i+1;j<r->size();j++)
	{
		//int j=i+1;
		vector<int> v;
		v.push_back(i);
		v.push_back(j);		
		Subring S1i;
		bool bn=S1i.init(r,v,16);
		if(!bn)
			continue;
		//Subring S1i(r,v);
		int ni=S1i.size();
		if(ni>16)
			continue;		
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){
			printf("%s cnt1=%d:R%d_%d->i=%d,j=%d\n",__FUNCTION__,cnt1,ni,ID,i,j);
		}		
		if(ni==n && ID==-1||(ID==230||ID==232||ID==236||ID==241||ID==244||ID==246||ID==337)||(ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==39)))   
		{
			string str=M2r::MStr(r->m_Set[i]);
			printf("%d->%s=>",i,str.c_str());
			string strj=M2r::MStr(r->m_Set[j]);
			printf("%d->%s=>",j,strj.c_str());			
			string strR=calcRingInvariant(&S1i);
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",ni,ID,strR.c_str());				
			//S1i.printTable();
			//break;
		}	
	}	   	
}

void fsubring2(Mnr *r,int n)
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
	map<pair<int,int>,pair<int,int>> M;	
	printf("g_i=%d\n",g_i);
	for(int i=g_i;i<r->size()-1;i++)		
	for(int j=i+1;j<r->size();j++)
	{
		//int j=i+1;
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);
		Subring S1i;
		bool bn=S1i.init(r,v,16);
		if(!bn)
			continue;
		//Subring S1i(r,v);
		int ni=S1i.size();
		if(ni>16)
			continue;
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){
			printf("%s cnt1=%d:R%d_%d->i=%d,j=%d\n",__FUNCTION__,cnt1,ni,ID,i,j);
		}		
		if(ni==n && ID==-1||(ID==230||ID==232||ID==236||ID==241||ID==244||ID==246||ID==337)||(ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==39)))   
		{
			string str=Mnr::MStr(r->m_Set[i]);
			printf("%d->%s=>",i,str.c_str());
			string strj=Mnr::MStr(r->m_Set[j]);
			printf("%d->%s=>",j,strj.c_str());			
			string strR=calcRingInvariant(&S1i);
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",ni,ID,strR.c_str());				
			//S1i.printTable();
#if PRINT_LOG			
			fout<<i<<"->"<<str<<","<<j<<"->"<<strj<<"=>";
			fout<<__FUNCTION__<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2="<<strR<<endl;
			bFind=true;
#endif
			//break;
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

void fsubring3(Mnr *r,int n)
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
	map<pair<int,int>,pair<int,int>> M;	
	printf("g_i=%d\n",g_i);
	for(int i=g_i;i<r->size()-2;i++)		
	for(int j=i+1;j<r->size()-1;j++)
	for(int k=j+1;k<r->size();k++)		
	{
		//int j=i+1;
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);	
		v.push_back(k);			
		Subring S1i;
		bool bn=S1i.init(r,v,16);
		if(!bn)
			continue;
		//Subring S1i(r,v);
		int ni=S1i.size();
		if(ni>16)
			continue;
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){
			printf("%s cnt1=%d:R%d_%d->i=%d,j=%d,k=%d\n",__FUNCTION__,cnt1,ni,ID,i,j,k);
		}	
		if(ni==n && ID==-1||(ID==230||ID==232||ID==236||ID==241||ID==244||ID==246||ID==337)||(ni==8 && (ID==6||ID==9||ID==12||ID==18||ID==39)))   
		{
			string str=Mnr::MStr(r->m_Set[i]);
			printf("%d->%s=>",i,str.c_str());
			string strj=Mnr::MStr(r->m_Set[j]);
			printf("%d->%s=>",j,strj.c_str());			
			string strk=Mnr::MStr(r->m_Set[k]);
			printf("%d->%s=>",k,strk.c_str());			
			string strR=calcRingInvariant(&S1i);			
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2=%s\n",ni,ID,strR.c_str());				
			//S1i.printTable();
#if PRINT_LOG			
			fout<<i<<"->"<<str<<","<<j<<"->"<<strj<<","<<k<<"->"<<strk<<"=>";
			fout<<__FUNCTION__<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2="<<strR<<endl;
			bFind=true;
#endif
			//break;
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

DWORD WINAPI M3Z4ijk( void* lpParameter ){
	ZmodnZ r(1,4);
	Mnr m2r(&r,3);
	fsubring3(&m2r,16);
	ExitThread(0);
	return 0;
};

DWORD WINAPI M3Z4ij( void* lpParameter ){
	ZmodnZ r(1,4);
	Mnr m2r(&r,3);
	fsubring2(&m2r,16);
	ExitThread(0);
	return 0;
};

DWORD WINAPI M2I4ij( void* lpParameter ){
	M2r r;
	r.initI(2);
#if 0
	Mnr m2r(&r,2);
	fsubring2(&m2r,16);
#else
	M2r m2r(&r);
	fsubring(&m2r,16);
#endif
	ExitThread(0);
	return 0;
};

int main(int argc, char* argv[]){ 
	if(argc>1)
		g_i=atoi(argv[1]);

	PTHREAD_START_ROUTINE tfun[]={M3Z4ijk,M3Z4ij,M2I4ij};
	unsigned long threadID[]={0,0,0};
	HANDLE h_thread[]={0,0,0};//句柄数组
	DWORD maskArr[]={0x02,0x04,0x08};//i7-6500U CPU，绑定线程到指定的CPU核心（CPU 1~3）
	int cnt=sizeof(tfun)/sizeof(tfun[0]);
	for(int i=0;i<cnt;i++){
		h_thread[i]=CreateThread( NULL, 0, tfun[i], NULL, 0, &threadID[i] );
		if(NULL != h_thread[i])
		{
			printf("CreateThread %d OK,threadID=%d,h_thread=%d\n",i,threadID[i],h_thread[i]);
			SetThreadAffinityMask(h_thread[i],maskArr[i]);
		}
	}
	WaitForMultipleObjects(cnt,h_thread,TRUE,INFINITE);
	printf("main thread\n");
	for(int i=0;i<cnt;i++){
		CloseHandle(h_thread[i]);
	}
#ifdef _DEBUG
	system("pause");
#endif
	return 0;
}
