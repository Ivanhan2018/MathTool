#include"quotientRing.h"
#include<set>
#include"DecompositionRing.h"
#include"Rn.h"
#include"M2r.h"
#include"Mnr.h"
#include"PolynomialRing.h"
#include <ctime>
#include <fstream>

IRing* newR2(int i,int p=2)
{
	if(i==1)
	{
		ZmodnZ* r=new ZmodnZ(p,p*p);
		return r;
	}
	if(i==2)
	{
		ZmodnZ* r=new ZmodnZ(1,p);
		return r;
	}
	return NULL;
}

IRing* newR4(int i,int p=2)
{
	if(i==1)
	{
		ZmodnZ* r=new ZmodnZ(p*p,p*p*p*p);
		return r;
	}
	if(i==2)
	{
		ZmodnZ* r=new ZmodnZ(p,p*p*p);
		return r;
	}	
	if(i==3)
	{
		ZmodnZ* r=new ZmodnZ(1,p*p);
		return r;
	}
	if(i==4)
	{
		M2r* r=new M2r();
	    r->initD(p);
		return r;
	}	
	if(i==5)
	{
		Mnr* r=new Mnr();
	    r->initE(p);
		return r;
	}	
	if(i==6)
	{
		Mnr* r=new Mnr();
	    r->initF(p);
		return r;
	}	
	if(i==7)
	{
		M2r* r=new M2r();
	    r->initG(p);
		return r;
	}
	if(i==8)
	{
		M2r* r=new M2r();
	    r->initH(p);
		return r;
	}
	if(i==9)
	{
		M2r* r=new M2r();
	    r->initI(p);
		return r;
	}
	if(i==10)
	{
		M2r* r=new M2r();
	    r->initJ(p);
		return r;
	}
	if(i==11)
	{
		M2r* r=new M2r();
	    r->initK(p);
		return r;
	}	
	return NULL;
}

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

	char sz[200] = {0};
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

extern IRing* newRpppp(int ID,int p);

IRing* newRpppp(int ID,int p,int n=16){	
   //if(ID!=32)
	   //return NULL;
	//p=2;
	//if(p==3)return NULL;
	//int n=p*p*p*p;
	const CRingDataItem * pItem = Find(n,ID);
	if(pItem && pItem->m_n1==1 && pItem->m_n2==2){
		Mnr* r=new Mnr();   
		r->m_r=new ZmodnZ(1,p);
		r->m_n=pItem->m_n0; 
		vector<MATRIXi8> gen;		
		vector<string> vv=split(pItem->m_mstr,";");
		for(int i=0;i<vv.size();i++){
			MATRIXi8 A(r->m_n,vector<TElem>(r->m_n,0));
			vector<string> v=split(vv[i],",");
			for(int j=0;j<r->m_n;j++)
				for(int k=0;k<r->m_n;k++)
					A[j][k]=atoi(v[j*r->m_n+k].c_str());
			gen.push_back(A);
		}	
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
        return r;			
	}
	if(pItem && pItem->m_n1==1 && (pItem->m_n2==4||pItem->m_n2==8||pItem->m_n2==16)){
		Mnr* r=new Mnr();
		if(pItem->m_n2==4)		
			r->m_r=new ZmodnZ(1,p*p);
		else if(pItem->m_n2==8)
			r->m_r=new ZmodnZ(1,p*p*p);
		else if(pItem->m_n2==16)
			r->m_r=new ZmodnZ(1,p*p*p*p);		
		r->m_n=pItem->m_n0; 
		vector<MATRIXi8> gen;		
		vector<string> vv=split(pItem->m_mstr,";");
		for(int i=0;i<vv.size();i++){
			MATRIXi8 A(r->m_n,vector<TElem>(r->m_n,0));
			vector<string> v=split(vv[i],",");
			for(int j=0;j<r->m_n;j++)
				for(int k=0;k<r->m_n;k++){
					A[j][k]=atoi(v[j*r->m_n+k].c_str());
					if(A[j][k]==2)
						A[j][k]=p;
					else if(A[j][k]==4)
						A[j][k]=p*p;
					else if(A[j][k]==8)
						A[j][k]=p*p*p;					
					else{
						if(A[j][k]>1)
							A[j][k]=1;
					}
				}
			gen.push_back(A);
		}	
		r->m_flag=1;
		r->m_Set=Mnr::FR(r->m_r,gen); 
        return r;			
	}		
	return NULL;	
}

IRing* newRpppRp(int ID1,int ID2,int p){	
	IRing* ri=newRpppp(ID1,p,8);
	if(!ri)return NULL;
	IRing* rj=newR2(ID2,p);
	if(!rj)return NULL;		
	DecompositionRing* r= new DecompositionRing(ri,rj);
	r->m_flag=1;	   
	return r;
}

IRing* newRppRpp(int ID1,int ID2,int p){	
	IRing* ri=newR4(ID1,p);
	if(!ri)return NULL;
	IRing* rj=newR4(ID2,p);
	if(!rj)return NULL;		
	DecompositionRing* r= new DecompositionRing(ri,rj);
	r->m_flag=1;	   
	return r;
}

void testDR(IRing* r16,IRing* r81){
   if(!r16||!r81)
	   return;
	int ID16=IdRing(r16);
	int ID81=(r81->size()>81)?0:IdRing(r81);			
	#if 1
		printf("R%d_%d,R%d_%d\n",r16->size(),ID16,r81->size(),ID81);
	#else
		string str=calcRingInvariant(r16);
		printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",r16->size(),ID16,str.c_str());
		string I1=calcI1(r16);
		string I2=calcI2(r16);   
		printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());				
		str=calcRingInvariant(r81);
		printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",r81->size(),ID81,str.c_str());
		I1=calcI1(r81);
		I2=calcI2(r81);   
		printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());
	#endif	
	return;
}

int main(int argc, char* argv[]){ 
	int n1=273;
	int n2=390;
	int n=16;
	if(argc>1)
		n1=atoi(argv[1]);
	if(argc>2)
		n2=atoi(argv[2]);
	if(argc>3)
		n=atoi(argv[3]);	
	int ret=LoadData("RingData.csv");
	printf("ret=%d,环表示数据表中的记录条数=%d\n",ret,g_mapRingDataCache.size()); 
#ifdef TDR
	for(int i=1;i<=11;i++){
		for(int j=i;j<=11;j++){
			IRing* r16=newRppRpp(i,j,2);
			IRing* r81=newRppRpp(i,j,3);
			testDR(r16,r81);
			delete r16;
			delete r81;				
		}
	}
	for(int i=1;i<=52;i++){
		for(int j=1;j<=2;j++){		
			IRing* r16=newRpppRp(i,j,2);
			IRing* r81=newRpppRp(i,j,3);
			testDR(r16,r81);
			delete r16;
			delete r81;	
		}
	}		
	return 0;
#endif
	if(1){
		for(int i=n1;i<=n2;i++){
		   IRing* r16=newRpppp(i,2,n);
		   IRing* r81=newRpppp(i,3,n);		   
		   if(!r16||!r81)
			   continue;
			int ID16=IdRing(r16);
			int ID81=(r81->size()>81)?0:IdRing(r81);			
			#if 1
			    printf("Rpppp(%d,2)=R%d_%d,Rpppp(%d,3)=R%d_%d\n",i,r16->size(),ID16,i,r81->size(),ID81);
			#else
				string str=calcRingInvariant(r16);
				printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",r16->size(),ID16,str.c_str());
				string I1=calcI1(r16);
				string I2=calcI2(r16);   
				printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());				
				str=calcRingInvariant(r81);
				printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",r81->size(),ID81,str.c_str());
				I1=calcI1(r81);
				I2=calcI2(r81);   
				printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());
			#endif		
		}
	}
    return 0;
}