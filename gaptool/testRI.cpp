#include<stdlib.h>
#include<stdio.h>
#include<string>
#include<vector>
using namespace std;

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

// 274,283,357
// 62,86
// 63,89
int main(int argc, char **argv){
	if(argc<2){
		printf("Usage:  testRI str [R16]\n");
		return 0;
	}
	char* str1=argv[1];
    char* str2="R16";
    if(argc>2)	
		str2=argv[2];

	vector<int> IDs;
	vector<string> vstr=split(str1,",");
	for(int i=0;i<vstr.size();i++)
	{
		int iID=atoi(vstr[i].c_str());
		IDs.push_back(iID);
	}	
	int cnt=IDs.size();
	const char *szCmd[]={"RI","RI2","S2","N89"};
	int count=sizeof(szCmd)/sizeof(szCmd[0]);
	for(int i=0;i<count;i++)	{
		printf("%s²»±äÁ¿:\n",szCmd[i]);		
		for(int j=0;j<cnt;j++)	{
			char sz[1000]={0};
			sprintf(sz,"%s R16%d.txt",szCmd[i],IDs[j]);
			system(sz);
		} 
	}

	
	return 0;
}
