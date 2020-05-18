#include "FiniteGroup.h"
#include "quotientGroup.h"
#include <fstream>
#include <complex>
using namespace std;

IGroup* newG24(int i){
    //int IDs[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	int IDs[]={4,1,5,6,7,8,9,10,2,11,12,13,14,15,3};
	int cnt=sizeof(IDs)/sizeof(IDs[0]);
	if(i<=0||i>cnt)
		return NULL;
	
	int iElem;
	int sum=0;
	char sz[40]={0};
	sprintf(sz,"G24_%d.txt",IDs[i-1]);
	ifstream fin(sz);
	int *g_G24Mul=new int[576];
	while( !fin.eof() && sum<24*24) {
		fin >> iElem;
		sum++;
		int i=(sum-1)/24;
		int j=(sum-1)%24;
		*(g_G24Mul+i*24+j)=iElem-1;
	}
	fin.close();
	
	FiniteGroup *g=new FiniteGroup(24,g_G24Mul,0);
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

int main(int argc, char* argv[])
{
    for(int i=1;i<=15;i++){
		IGroup* g=newG24(i);
		if(g){
			print(g);
			delete g;
			g=NULL;
		}
	}
	return 0;
}