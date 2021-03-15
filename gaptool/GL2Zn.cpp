#include"GL2Zn.h"
#include<set>
#include<fstream>

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

// “群表示数据表”结点
class CGroupDataItem
{
public:
	CGroupDataItem() 
	{
		m_n=0;
		m_ID=0;
		m_n0=0;
		m_n1=0;
        m_mstr="";		
	}

	int m_n;
	int m_ID;	
	int m_n0;
	int m_n1;
	string m_mstr;
};

map<pair<int,int>,CGroupDataItem> g_mapGroupDataCache;

const CGroupDataItem * Find(int n,int ID)
{
	map<pair<int,int>,CGroupDataItem>::const_iterator it;
	it = g_mapGroupDataCache.find(make_pair(n,ID));
	if( it != g_mapGroupDataCache.end() )
		return &(it->second);
	return NULL;
}

// “群表示数据表”缓冲
int LoadData(char * pszFilePath)		//“从文件中读取数据”
{
	if( !g_mapGroupDataCache.empty() )
		return 2;//2已经载入数据了

	FILE * fp =fopen(pszFilePath, "r");
	if( fp == NULL )
		return 1;//1打开文件失败

	char sz[240] = {0};
	CGroupDataItem item;
	int n = 0;
	n = fscanf(fp, "%s", sz);
	while( n > 0 && !feof(fp) )
	{
		n = fscanf(fp, "%d,%d,%d,%d,%s\n", &item.m_n, &item.m_ID, &item.m_n0, &item.m_n1, sz);
		if( n > 0 )
		{
			item.m_mstr = sz;
			g_mapGroupDataCache.insert( make_pair(make_pair(item.m_n,item.m_ID),item));
		}
	}
	fclose(fp);
	return 0;//0成功
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

set<string> gS;
set<std::pair<int,int>> gM;
int g_a=0;
void findgroup2(GL2Zn *g,int N)
{	
    int m=g->size();
	for(int i=g_a;i<m;i++)		
	for(int j=i+1;j<m;j++){	
		TM2 vi=g->s_Arr[i];
		TM2 vj=g->s_Arr[j];
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
			printf("G%d_%d:%d,%d->%s;%s\n",G.size(),ID,i,j,V2S(vi).c_str(),V2S(vj).c_str());				
		}
		if(ni<m && ID==-1){		
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

void findgroup3(GL2Zn *g,int N)
{	
    int m=g->size();
	for(int i=g_a;i<m;i++)		
	for(int j=i+1;j<m;j++){	
	for(int k=j+1;k<m;k++){
		TM2 vi=g->s_Arr[i];
		TM2 vj=g->s_Arr[j];
		TM2 vk=g->s_Arr[k];		
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
			printf("G%d_%d:%d,%d,%d->%s;%s;%s\n",G.size(),ID,i,j,k,V2S(vi).c_str(),V2S(vj).c_str(),V2S(vk).c_str());				
		}
		if(ni<m && ID==-1){		
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

int g_D4[8][4]=
{
	{1,0,0,1},
	{0,1,6,0},
	{6,0,0,6},
	{0,6,1,0},
	{0,1,1,0},
	{6,0,0,1},
	{0,6,6,0},
	{1,0,0,6},
};

void test(int n)
{
    GL2Zn D8;
	D8.m_n=n;
	vector<TM2> gen;
#if 0	
	TM2 a(&g_D4[1][0],&g_D4[1][0]+4);
	TM2 b(&g_D4[4][0],&g_D4[4][0]+4);	
#else
	TM2 a(4);
	TM2 b(4);	
	a[0]=0;
	a[1]=1;
	a[2]=n-1;
	a[3]=0;
	b[0]=0;
	b[1]=1;
	b[2]=1;
	b[3]=0;		
#endif
	gen.push_back(a);
	gen.push_back(b);
	D8.s_Arr=GL2Zn::FG(gen,D8.m_n);
	D8.printSet();
	D8.printTable();	
}

//GAP[16,8]、GAP[24,8]、GAP[40,5]、GAP[48,6]、GAP[32,19]、GAP[48,14]、GAP[48,6]
//GAP[120,27]、GAP[40,8]、GAP[96,-1]、GAP[56,-1]、GAP[96,-1]、
//GAP[160,-1]、GAP[96,-1]、GAP[72,5]、GAP[48,6]、GAP[72,8]、
void test1(int n)
{
    GL2Zn D8;
	D8.m_n=n;
	vector<TM2> gen;
	TM2 a(4);
	TM2 b(4);	
	a[0]=0;
	a[1]=1;
	a[2]=n-1;
	a[3]=0;
	b[0]=n-1;
	b[1]=n-1;
	b[2]=n-1;
	b[3]=0;		
	gen.push_back(a);
	gen.push_back(b);
	D8.s_Arr=GL2Zn::FG(gen,D8.m_n);
	D8.printSet();
	D8.printTable();
    findgroup2(&D8,120);	
}

//GAP[48,29]、GAP[96,-1]、GAP[240,-1]、GAP[288,-1]、GAP[672,-1]、GAP[768,-1]、GAP[1296,-1]
void test2(int n)
{
    GL2Zn D8;
	D8.m_n=n;
	vector<TM2> gen;
	TM2 a(4);
	TM2 b(4);	
	a[0]=0;
	a[1]=n-1;
	a[2]=n-1;
	a[3]=0;
	b[0]=n-1;
	b[1]=n-1;
	b[2]=n-1;
	b[3]=0;		
	gen.push_back(a);
	gen.push_back(b);
	D8.s_Arr=GL2Zn::FG(gen,D8.m_n);
	D8.printSet();
	D8.printTable();
    findgroup2(&D8,120);		
}

//GAP[24,3]、GAP[48,30]、GAP[120,5]、GAP[144,-1]、GAP[336,-1]、GAP[384,-1]、GAP[648,-1]
void test3(int n)
{
    GL2Zn D8;
	D8.m_n=n;
	vector<TM2> gen;
	TM2 a(4);
	TM2 b(4);	
	a[0]=0;
	a[1]=n-1;
	a[2]=1;
	a[3]=0;
	b[0]=n-1;
	b[1]=1;
	b[2]=0;
	b[3]=n-1;		
	gen.push_back(a);
	gen.push_back(b);
	D8.s_Arr=GL2Zn::FG(gen,D8.m_n);
	D8.printSet();
	D8.printTable();
    findgroup2(&D8,120);		
}

//GAP[6,1]、GAP[24,3]、GAP[48,30]、GAP[120,5]、GAP[144,-1]
void SL2Zn(int n)
{
    GL2Zn g(n,1);
	g.printSet();
	g.printTable();
    findgroup2(&g,120);		
}

int main(int argc,char *argv[]){
    // 将群表示数据配置到文件中
	int ret=LoadData("GroupData.csv");
	printf("ret=%d,群表示数据表中的记录条数=%d\n",ret,g_mapGroupDataCache.size());
	
	if(argc<3){
		printf("usage:GL2Zn n ID\n");
		return 0;
	}	
	int n=atoi(argv[1]);
	int ID=atoi(argv[2]);
	const CGroupDataItem * pItem = Find(n,ID);
	if(!pItem){
		printf("没有配置G%d_%d的表示数据！\n",n,ID);
		return 0;
	}
	if(pItem->m_n0!=2){
		printf("暂不处理%d阶矩阵表示！\n",pItem->m_n0);
		return 0;		
	}
	
	vector<TM2> gen;		
	vector<string> vv=split(pItem->m_mstr,";");
	for(int i=0;i<vv.size();i++){
		TM2 A(4);
		vector<string> v=split(vv[i],",");
		A[0]=atoi(v[0].c_str());
		A[1]=atoi(v[1].c_str());
		A[2]=atoi(v[2].c_str());
		A[3]=atoi(v[3].c_str());
		gen.push_back(A);
	}	
    GL2Zn g;
	g.m_n=pItem->m_n1;
	g.s_Arr=GL2Zn::FG(gen,g.m_n);
	
	int ID1=IdGroup(&g);
	string strG=calcGroupInvariant(&g);  
	printf("G%d_%d:%s\n",g.size(),ID1,strG.c_str());	
	
	int fun=0;
	string str="";
	if(argc>3){
		str=argv[3];
		fun=atoi(str.c_str());
		if(fun<-1||fun>1){
			fun=0;
		}	
	}	
	int n0=120;
	if(argc>4){
		n0=atoi(argv[4]);
	}	
	g_a=argc>5?atoi(argv[5]):0;
	
	if(str.substr(0,1)=="w"){	
		char sz[128]={0};   
		sprintf(sz,"G%d.%d.txt",g.size(),ID1);
		writeTable(&g,sz);
	}		
	typedef void(*pF)(GL2Zn *g,int N);
	pF Func[]={findgroup2,findgroup3};
	Func[fun](&g,n0);
	
	//int n=argc>2?atoi(argv[2]):12;
	//test1(19);
    //for(int i=3;i<10;i++)
	//for(int i=10;i<15;i++)
	//for(int i=2;i<7;i++)	
		;//SL2Zn(i);
	//SL2Zn(n);
    return 0;
}