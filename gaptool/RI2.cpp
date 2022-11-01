#define PARSE_RING_FILE
#define QF
#define R16_I1I2
#define USE_MNR_UINT32
#include<iostream>
#include<set>
#if 1
#include"FiniteRing.h"
#else
#include"ZmodnZ.h"
#include"M2r.h"
#include"Mnr.h"
#endif

#ifndef PARSE_RING_FILE
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
#endif

IRing* newR(int n,int ID){
#ifdef PARSE_RING_FILE	
    const char *szArr[]={"_","","."};
	for(int i=0;i<3;i++){
		char sz[100]={0};
		sprintf(sz,"R%d%s%d.txt",n,szArr[i],ID);
		FiniteRing* r=newRing(sz);
		if(r && r->size()==n)
			return r;
		else 
			continue;	
	}
#endif
	return NULL;
}

int main(int argc, char* argv[]){ 
    int n=81;
	int n1=1;
	int n2=514;
	if(argc>1)
		n=atoi(argv[1]);
	if(argc>2)
		n1=atoi(argv[2]);
	if(argc>3)
		n2=atoi(argv[3]); 		
	for(int i=n1;i<=n2;i++){	
		IRing* r=newR(n,i);
		if(r){
			int ID=IdRing(r);
#if defined(RI3)
			string str=calcRingInvariant3(r);
#elif defined(RI)
			string str=calcRingInvariant(r);				
#elif defined(N89)
			string str=calcb8N8N9(r);	
#elif defined(Q1_)
			string str=calcQ1(r);		
#else	
			string str=calcRingInvariant2(r);
#endif		
			printf("%d,%s\n",ID,str.c_str());	
            delete r;
            r=NULL;			
		}
	}
   return 0;
}