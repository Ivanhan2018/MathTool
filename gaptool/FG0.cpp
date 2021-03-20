#include"PermGroup.h"
#include"quotientGroup.h"
#include <fstream>
#include <iostream>
#include <complex>
#include <ctime>
#include<set>

std::vector<string> split( const std::string& str, const std::string& delims, unsigned int maxSplits = 0)
{
	std::vector<string> ret;
	unsigned int numSplits = 0;
	// Use STL methods 
	size_t start, pos;
	start = 0;
	do 
	{
		pos = str.find_first_of(delims, start);
		if (pos == start)
		{
			// Do nothing
			start = pos + 1;
		}
		else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
		{
			// Copy the rest of the std::string
			ret.push_back( str.substr(start) );
			break;
		}
		else
		{
			// Copy up to delimiter
			ret.push_back( str.substr(start, pos - start) );
			start = pos + 1;
		}
		// parse up to next real data
		start = str.find_first_not_of(delims, start);
		++numSplits;
	} while (pos != std::string::npos);
	return ret;
}

// “置换表示数据表”结点
class CPermGroupDataItem
{
public:
	CPermGroupDataItem() 
	{
		m_n=0;
		m_ID=0;
        m_str="";		
	}

	int m_n;
	int m_ID;	
	string m_str;
};

map<pair<int,int>,CPermGroupDataItem> g_mapPermGroupDataCache;

const CPermGroupDataItem * Find(int n,int ID)
{
	map<pair<int,int>,CPermGroupDataItem>::const_iterator it;
	it = g_mapPermGroupDataCache.find(make_pair(n,ID));
	if( it != g_mapPermGroupDataCache.end() )
		return &(it->second);
	return NULL;
}

// “置换表示数据表”缓冲
int LoadData(char * pszFilePath)		//“从文件中读取数据”
{
	if( !g_mapPermGroupDataCache.empty() )
		return 2;//2已经载入数据了

	FILE * fp =fopen(pszFilePath, "r");
	if( fp == NULL )
		return 1;//1打开文件失败

	char sz[240] = {0};
	CPermGroupDataItem item;
	int n = 0;
	n = fscanf(fp, "%s", sz);
	while( n > 0 && !feof(fp) )
	{
		n = fscanf(fp, "%d,%d,%s\n", &item.m_n, &item.m_ID, sz);
		if( n > 0 )
		{
			item.m_str = sz;
			g_mapPermGroupDataCache.insert( make_pair(make_pair(item.m_n,item.m_ID),item));
		}
	}
	fclose(fp);
	return 0;//0成功
}

set<string> gS;
set<std::pair<int,int>> gM;
int g_a=0;
void QFindPermGroup2(Sn *sn,int n0)
{	
    int m=sn->size();
	for(int i=g_a;i<m;i++)		
	for(int j=i+1;j<m;j++){	
		vector<SnE> S;
		SnE vi=sn->s_Arr[i];
		SnE vj=sn->s_Arr[j];
		S.push_back(vi);
		S.push_back(vj);
		vector<int> v;
		v.push_back(i);
		v.push_back(j);
		Subgroup G;
		bool b=G.init(sn,v,n0);
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

void QFindPermGroup3(Sn *sn,int n0)
{	
    int m=sn->size();
	for(int i=g_a;i<m;i++)		
	for(int j=i+1;j<m;j++){
	for(int k=j+1;k<m;k++){		
		vector<SnE> S;
		SnE vi=sn->s_Arr[i];
		SnE vj=sn->s_Arr[j];
		SnE vk=sn->s_Arr[k];
		S.push_back(vi);
		S.push_back(vj);
		S.push_back(vk);
		vector<int> v;
		v.push_back(i);
		v.push_back(j);
		Subgroup G;
		bool b=G.init(sn,v,n0);
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

void QFindquotientGroup1(Sn *sn,int n0)
{	
    int m=sn->size();
	for(int i=g_a;i<m;i++){	
		vector<SnE> S;
		SnE vi=sn->s_Arr[i];
		S.push_back(vi);
		vector<int> v;
		//v.push_back(0);
		v.push_back(i);
		Subgroup s;
		bool b=s.init(sn,v,m/2);
		if(!b)
			continue;
		int ni=s.size();
		if(sn->size()/ni>n0||ni==1)
			continue;
		int isN=IsNormalSubgroup(sn,s.m_Set,true);
		if(isN!=1)
			continue;	
		quotientGroup G(sn,s.m_Set);		
		//bool bG=IsGroup(&G);
		//if(!bG)
		//	continue;
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(G.size(),ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			int sID=IdGroup(&s);
			printf("G%d_%d/G%d_%d=G%d_%d:%d->%s\n",m,0,ni,sID,G.size(),ID,i,V2S(vi).c_str());				
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

void QFindquotientGroup2(Sn *sn,int n0)
{	
    int m=sn->size();
	for(int i=g_a;i<m;i++)		
	for(int j=i+1;j<m;j++){	
		vector<SnE> S;
		SnE vi=sn->s_Arr[i];
		SnE vj=sn->s_Arr[j];
		S.push_back(vi);
		S.push_back(vj);
		vector<int> v;
		v.push_back(i);
		v.push_back(j);
		Subgroup s;
		bool b=s.init(sn,v,m/2);
		if(!b)
			continue;
		int ni=s.size();
		if(sn->size()/ni>n0)
			continue;
		int isN=IsNormalSubgroup(sn,s.m_Set,true);
		if(isN!=1)
			continue;	
		quotientGroup G(sn,s.m_Set);		
		//bool bG=IsGroup(&G);
		//if(!bG)
		//	continue;
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(G.size(),ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			int sID=IdGroup(&s);
			printf("G%d_%d/G%d_%d=G%d_%d:%d,%d->%s,%s\n",m,0,ni,sID,G.size(),ID,i,j,V2S(vi).c_str(),V2S(vj).c_str());				
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

void QFindquotientGroup3(Sn *sn,int n0)
{	
    int m=sn->size();
	for(int i=g_a;i<m;i++)		
	for(int j=i+1;j<m;j++){
	for(int k=j+1;k<m;k++){		
		vector<SnE> S;
		SnE vi=sn->s_Arr[i];
		SnE vj=sn->s_Arr[j];
		SnE vk=sn->s_Arr[k];
		S.push_back(vi);
		S.push_back(vj);
		S.push_back(vk);
		vector<int> v;
		v.push_back(i);
		v.push_back(j);
		v.push_back(k);
		Subgroup s;
		bool b=s.init(sn,v,m/2);
		if(!b)
			continue;
		int ni=s.size();
		if(sn->size()/ni>n0)
			continue;
		int isN=IsNormalSubgroup(sn,s.m_Set,true);
		if(isN!=1)
			continue;	
		quotientGroup G(sn,s.m_Set);
		//bool bG=IsGroup(&G);
		//if(!bG)
		//	continue;
		int ID=IdGroup(&G);
		int cnt=gM.size();
		gM.insert(make_pair(G.size(),ID));
		int cnt1=gM.size();
		if(cnt1>cnt){
			int sID=IdGroup(&s);
			printf("G%d_%d/G%d_%d=G%d_%d:%d,%d,%d->%s,%s,%s\n",m,0,ni,sID,G.size(),ID,i,j,k,V2S(vi).c_str(),V2S(vj).c_str(),V2S(vk).c_str());				
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

vector<SnE> readGens(const char *path){
    vector<SnE> S;
	ifstream fin(path);
	if(fin){
		string strLine;
		fin >> strLine;
		vector<string> vN=split(strLine,",");
		int N=vN.size();
		while(strLine!=""){
			strLine="";
		    fin >> strLine;

			vector<string> vN1=split(strLine,",");
			int N1=vN1.size();
			if(N1==N){
				SnE viN1(N);
				for(int i=0;i<N;i++){
					viN1[i]=atoi(vN1[i].c_str());
				}
				S.push_back(viN1);
			}
		}
	}
	return S;
}

void writeTable(IGroup* g,const char *path){
	ofstream fout(path);
	int n=g->size();  
	for(int i=0;i<n;i++){
	   for(int j=0;j<n;j++){
		  int ij=g->mul(i,j);
		  fout<<ij+1<<" ";
	   }
	   fout<<endl;
	}
}

void test1(){
    Sn s9(9);
	s9.printSet();	
    s9.printTable();
	
	unsigned char s9a[]={4,5,6,9,3,2,7,1,8};
	unsigned char s9b[]={6,1,7,4,2,5,9,3,8};	
	SnE a(s9a,s9a+9);
	SnE b(s9b,s9b+9);	
	vector<int> A(1);
	A[0]=getidx(s9.s_Arr,a);
	vector<int> B(1);
	B[0]=getidx(s9.s_Arr,b);	
	vector<int> AB;
	AB.insert(AB.end(),A.begin(),A.end());
	AB.insert(AB.end(),B.begin(),B.end());	
	vector<int> fgA=FG(&s9,A);
	printSubGroup(&s9,fgA);
	vector<int> fgB=FG(&s9,B);
	printSubGroup(&s9,fgB);	
	vector<int> fgAB=FG(&s9,AB);
	printSubGroup(&s9,fgAB);	
}

void test2(){
	unsigned char s9a[]={4,5,6,9,3,2,7,1,8};
	unsigned char s9b[]={6,1,7,4,2,5,9,3,8};
    Sn M9(9,s9a,s9b);	
	M9.printSet();	
    M9.printTable();
}

int oldFG(int argc, char* argv[])
{
	char sz[100]={0};
	char sz1[100]={0};
	if(argc<2)
	{
		printf("请输入有限生成置换群A的生成元文件名：");
		scanf("%s",sz);
	}
	else
		strcpy(sz,argv[1]);

    if(strspn(sz,"Set")==0)
	{
	   	string strDes=sz;
		strDes.replace(strDes.find("Set",0),strlen("Set"),"");
		strcpy(sz1,strDes.c_str());
	}
	else
	{
		strcpy(sz1,sz);
		strcat(sz1,".txt");	
	}

	vector<SnE> gen=readGens(sz);
    Sn G;
	G.init(gen);
	G.printSet();	
    G.printTable();	
	
	writeTable(&G,sz1);

    if(argc<2)
		system("pause");	
	
	return 0;
}

int GetRand(int a,int b){
	if(a>=b)
		return a;
	int iRet=rand()%(b-a+1)+a;
	return iRet;
}

int fact(int n){
	static int result[]={1,2,6,24,120,720,5040,40320,362880,362880};
    int idx=(n<1||n>10?0:n-1);
	return result[idx];
}

int main(int argc, char* argv[])
{
    //if(strspn(argv[0],"FG")==0)
	if(string(argv[0]).find("FG",0)!=string::npos)
		return oldFG(argc,argv);

    // 将置换表示数据配置到文件中
	int ret=LoadData("PermGroupData.csv");
	printf("ret=%d,置换表示数据表中的记录条数=%d\n",ret,g_mapPermGroupDataCache.size());

	int N=2;	
	if(argc<N)
	{
		printf("Usage:  PermGroup arg1|n ID [w]\n");
		printf("eg:PermGroup 4,5,6,9,3,2,7,1,8;6,1,7,4,2,5,9,3,8\n");
		printf("eg:PermGroup 72 41\n");	
		printf("eg:PermGroup S10\n");		
		return 0;
	}
	
	int bSn=0;
	string str=argv[1];
	if(str.find(",",0)!=string::npos){   
	}else if(str.substr(0,1)=="S"){
		str.erase(str.begin());
		bSn=atoi(str.c_str());
		string mstr=argv[2];
		if(bSn>0 && mstr.substr(0,1)=="n"){
			srand(time(NULL));
			int N1=fact(bSn);
			if(N1<=0)
				N1=10000;	
			int cnt=GetRand(1,3);
			if(mstr.size()>1){
				int cnt1=atoi(mstr.substr(1,1).c_str());
				if(cnt1>0 && cnt1<6){
					cnt=cnt1;
				}
			}
			str="";
			for(int i=0;i<cnt;i++){
				int ii=GetRand(1,N1-1);
				SnE vi=Sn::getSnE(bSn,ii);		
				str+=V2S(vi);
				if(i<cnt-1)
					str+=";";
			}
			printf("str=%s\n",str.c_str());
			N=3;
			bSn=0;
		}else if(bSn>0 && mstr.substr(0,1)=="N"){
			mstr=mstr.substr(1,mstr.size()-1);
			vector<string> vs=split(mstr,";");		
			int cnt=vs.size();
			str="";
			for(int i=0;i<cnt;i++){
				SnE vi=Sn::getSnE(bSn,atoi(vs[i].c_str()));
				str+=V2S(vi);
				if(i<cnt-1)
					str+=";";
			}
			printf("str=%s\n",str.c_str());
			N=3;
			bSn=0;			
		}
	}else{
		int n=atoi(argv[1]);
		int ID=atoi(argv[2]);
		const CPermGroupDataItem * pItem = Find(n,ID);
		if(!pItem){
			printf("没有配置G%d_%d的表示数据！\n",n,ID);
			return 0;
		}
		str=pItem->m_str;
		printf("str=%s\n",str.c_str());
		N=3;
	}
	
	string strN="";
	if(argc>N)
		strN=argv[N];	
	
	Sn *G=new Sn();
	if(bSn>0){
		G->init(bSn);
	}else{
		vector<SnE> S;
		vector<string> vN=split(str,";");
		for(int i=0;i<vN.size();i++)
		{
			vector<string> vNi=split(vN[i],",");
			int N=vNi.size();
			SnE viN1(N);
			for(int j=0;j<N;j++){
				viN1[j]=atoi(vNi[j].c_str());
			}
			S.push_back(viN1);		
		}		
		G->init(S);
	}
	//G.printSet();	
	//G.printTable();	
	int ID=G->size()>120?0:IdGroup(G);  
	printf("G%d_%d:\n",G->size(),ID);
	if(strN.substr(0,1)=="r"){	
		string strG=calcGroupInvariant(G);	
		printf("%s\n",strG.c_str());
	}	
	if(strN.substr(0,1)=="w"){	
		char sz[128]={0};   
		sprintf(sz,"G%d.%d.txt",G->size(),ID);
		writeTable(G,sz);
		printf("写入完毕！\n");
	}
	
	int fun=0;
	if(argc>N){
		fun=atoi(strN.c_str());
		if(fun<-1||fun>4){
			fun=0;
		}	
	}	
	int n0=120;
	if(argc>N+1){
		n0=atoi(argv[N+1]);
	}	
	g_a=argc>(N+2)?atoi(argv[N+2]):0;	
	
	typedef void(*pF)(Sn *g,int n0);
	pF Func[]={QFindPermGroup2,QFindPermGroup3,QFindquotientGroup1,QFindquotientGroup2,QFindquotientGroup3};
	Func[fun](G,n0);
    
    delete G;
    G=NULL;	
	
	return 0;
}
