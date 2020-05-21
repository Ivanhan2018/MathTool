#include "FiniteGroup.h"
#include "quotientGroup.h"
#include <fstream>
#include <complex>
using namespace std;

bool usecomma=false;

IGroup* newG(int n,int i,int* IDs,int cnt){
	if(i<=0||i>cnt)
		return NULL;
	
	int iElem;
	int sum=0;
	char sz[40]={0};
//#ifdef USECOMMA
if(usecomma)
	sprintf(sz,"G%d.%d.txt",n,IDs[i-1]);
//#else
else
	sprintf(sz,"G%d_%d.txt",n,IDs[i-1]);
//#endif
	ifstream fin(sz);
	if(!fin)
		return NULL;
	int *g_GMul=new int[n*n];
	while(!fin.eof() && sum<n*n) {
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

void print(IGroup* g){
	int ID=IdGroup(g);
	string N0=calcN0(g);   
	string S2=calcS2(g);
	string kKEZDCANS=calckKEZDCANS(g);
	printf("GAP[%d,%d]:\n",g->size(),ID); 
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
	printf("\n");
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

int main(int argc, char* argv[])
{
    //if(strspn(argv[0],"Gnm")==0)//Error
	if(string(argv[0]).find("Gnm",0)!=string::npos)	
		usecomma=true;
	
	char sz[100]={0};
	if(argc<3)
	{
		printf("Usage:  Group arg1 arg2 [arg3]\n");
		printf("eg16:Group 16 14 1,2,6,7,3,8,9,10,11,4,12,13,14,5\n");
		printf("eg18:Group 18 5 3,1,4,5,2\n");		
		printf("eg24:Group 24 15 4,1,5,6,7,8,9,10,2,11,12,13,14,15,3\n");
		return 0;
	}
	
    int n=atoi(argv[1]);
    int m=atoi(argv[2]);
	if(n<1||m<1)
	{
		return 0;
	}
	vector<int> IDs(m);
	for(int i=0;i<m;i++)
		IDs[i]=i+1;
	if(argc>3)
	{
		vector<string> v=split(argv[3],",");
		if(v.size()!=m)
			return 0;
		for(int i=0;i<m;i++)
			IDs[i]=atoi(v[i].c_str());
	}	
    for(int i=1;i<=m;i++){
		IGroup* g=newG(n,i,&IDs[0],m);
		if(g){
			print(g);
			delete g;
			g=NULL;
		}
	}
	return 0;
}