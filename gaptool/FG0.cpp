#include"PermGroup.h"
#include <fstream>
#include <iostream>
#include <complex>
//#include <ctime>

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
		return 0;
	}
	
	string str=argv[1];
	if(str.find(",",0)!=string::npos){     
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
	Sn G;
	G.init(S);
	//G.printSet();	
	//G.printTable();	
	int ID=IdGroup(&G);
	string strG=calcGroupInvariant(&G);  
	printf("G%d_%d:%s\n",G.size(),ID,strG.c_str());	
	if(strN.substr(0,1)=="w"){	
		char sz[128]={0};   
		sprintf(sz,"G%d.%d.txt",G.size(),ID);
		writeTable(&G,sz);
	}
	
	return 0;
}
