#include "IRing.h"

// “环表示数据表”结点
class CRingDataItem
{
public:
	CRingDataItem() 
	{
		m_n=0;
		m_ID=0;
		m_n0=0;
		m_n1=0;
		m_n2=0;
        m_mstr="";		
	}

	int m_n;
	int m_ID;	
	int m_n0;
	int m_n1;
	int m_n2;
	string m_mstr;
};

map<pair<int,int>,CRingDataItem> g_mapRingDataCache;

const CRingDataItem * Find(int n,int ID)
{
	map<pair<int,int>,CRingDataItem>::const_iterator it;
	it = g_mapRingDataCache.find(make_pair(n,ID));
	if( it != g_mapRingDataCache.end() )
		return &(it->second);
	return NULL;
}

// “环表示数据表”缓冲
int LoadData(char * pszFilePath)		//“从文件中读取数据”
{
	if( !g_mapRingDataCache.empty() )
		return 2;//2已经载入数据了

	FILE * fp =fopen(pszFilePath, "r");
	if( fp == NULL )
		return 1;//1打开文件失败

	char sz[240] = {0};
	CRingDataItem item;
	int n = 0;
	n = fscanf(fp, "%s", sz);
	while( n > 0 && !feof(fp) )
	{
		n = fscanf(fp, "%d,%d,%d,%d,%d,%s\n", &item.m_n, &item.m_ID, &item.m_n0,&item.m_n1, &item.m_n2, sz);
		if( n > 0 )
		{
			item.m_mstr = sz;
			g_mapRingDataCache.insert( make_pair(make_pair(item.m_n,item.m_ID),item));
		}
	}
	fclose(fp);
	return 0;//0成功
}

vector<int> FindRID(int n,int n0,int n1,int n2){
	map<pair<int,int>,CRingDataItem>::iterator it;
	vector<int> v;
	for( it = g_mapRingDataCache.begin(); it != g_mapRingDataCache.end(); ++it){
		if(it->second.m_n==n && it->second.m_n0==n0 && it->second.m_n1==n1 && it->second.m_n2==n2){
			v.push_back(it->second.m_ID);
		}
	}
	return v;
}

int main(int argc, char *argv[]){
    int ret=LoadData("RingData.csv");
	int iArr2[][4]={\
		{8,2,1,2}, \
		{8,3,1,2}, \
		{8,4,1,2}, \
		{8,2,1,4}, \
		{8,3,1,4}, \
		{8,4,1,4}, \
		{8,2,1,8}, \
		{8,3,1,8}, \
		{8,4,1,8}, \
		{16,2,1,2}, \
		{16,3,1,2}, \
		{16,4,1,2}, \
		{16,5,1,2}, \
		{16,6,1,2}, \
		{16,2,1,4}, \
		{16,3,1,4}, \
		{16,4,1,4}, \
		{16,2,1,8}, \
		{16,3,1,8}, \
		{16,4,1,8}, \
		{16,2,1,16}, \
		{27,2,1,3}, \
		{27,3,1,3}, \
		{27,4,1,3}, \
		{27,5,1,3}, \
		{27,2,1,9}, \
		{27,3,1,9}, \
		{27,4,1,9}, \
		{81,2,1,3}, \
		{81,3,1,3}, \
		{81,4,1,3}, \
		{81,5,1,3}, \
		{81,6,1,3}, \
		{81,2,1,9}, \
		{81,3,1,9}, \
		{81,4,1,9}, \
		{81,2,1,27}, \
		{81,3,1,27}, \
		{81,4,1,27}, \
		{81,2,1,81}, \
	};
    int cnt=sizeof(iArr2)/sizeof(iArr2[0]);
    for(int i=0;i<cnt;i++){
		int n=iArr2[i][0];
		int n0=iArr2[i][1];
		int n1=iArr2[i][2];
		int n2=iArr2[i][3];		
		vector<int> vID=FindRID(n,n0,n1,n2);
		printf("已找到%d种R%d(%d,%d,%d):\n",vID.size(),n,n0,n1,n2);
		for(auto it=vID.begin();it!=vID.end();it++){
			printf("%d,",*it);
		}
		printf("\n");
	}
	
	return 0;
}