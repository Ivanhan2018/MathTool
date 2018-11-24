//#include "mathlib.h"
#include <stdio.h>
#include <stdlib.h>
#if defined(_WIN32)||defined(_WIN64)
#include<windows.h>
#else
#include <dlfcn.h>
#endif
#include<vector>
#include<string>
using namespace std;

std::vector<string> split(const std::string& str, const std::string& delims, unsigned int maxSplits = 0)
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

string unsplit(vector<int>& iAll)
{
	string ret;
	for(int i=0;i<iAll.size();i++)
	{
	   char sz[20]={0};
           if(i<iAll.size()-1)
             sprintf(sz,"%d,",iAll[i]);
           else
             sprintf(sz,"%d",iAll[i]);
	   ret+=sz;
	}

	return ret;
}

int main2(int argc, char* argv[])
{
   typedef void(*pR2)(int *p,int n,int ascending);
   pR2 R2;
#if defined(_WIN32)||defined(_WIN64)
	HINSTANCE hLib = LoadLibrary(argv[1]);
	if(!hLib) return 0;
	R2=(pR2)GetProcAddress(hLib,argv[2]);
#else
        void* hLib = dlopen(argv[1], RTLD_LAZY);
        if(!hLib)
        {        
            printf("ERROR, Message(%s).\n", dlerror());
            return -1;
        }
        R2=(pR2)dlsym(hLib, argv[2]);
        char* szError = dlerror();
        if(szError != NULL)
        {
         printf("ERROR, Message(%s).\n", szError);
         dlclose(hLib);
         return -1;
        }
#endif
        string strArr=argv[3];
	vector<int> iAll;
	vector<string> All=split(strArr,",");
	for(int i=0;i<All.size();i++)
	{
	   int iElem=atoi(All[i].c_str());
	   iAll.push_back(iElem);
	}
        int ascending=atoi(argv[4]);//1表示升序，0表示降序
        R2(&iAll[0],iAll.size(),ascending);
        string strArr1=unsplit(iAll);
        printf("%s(%s,%d)=%s\n", argv[2],strArr.c_str(),ascending,strArr1.c_str());
#if defined(_WIN32)||defined(_WIN64)
	FreeLibrary(hLib);
#else
        dlclose(hLib);
#endif
	getchar();
	return 0;
}

int main(int argc, char* argv[])
{
        if(argc<4)
        {
            printf("argc=%d<4.\n", argc);
            return -1;
        }
        if(argc>=5)
        {
           int ret=main2(argc,argv);
           return ret;
        }
	typedef void(*pR)(int *p,int n);
	pR R;
#if defined(_WIN32)||defined(_WIN64)
	HINSTANCE hLib = LoadLibrary(argv[1]);
	if(!hLib) return 0;
	R=(pR)GetProcAddress(hLib,argv[2]);
#else
        void* hLib = dlopen(argv[1], RTLD_LAZY);
        if(!hLib)
        {        
            printf("ERROR, Message(%s).\n", dlerror());
            return -1;
        }
        R=(pR)dlsym(hLib, argv[2]);
        char* szError = dlerror();
        if(szError != NULL)
        {
         printf("ERROR, Message(%s).\n", szError);
         dlclose(hLib);
         return -1;
        }
#endif
        string strArr=argv[3];
	vector<int> iAll;
	vector<string> All=split(strArr,",");
	for(int i=0;i<All.size();i++)
	{
	   int iElem=atoi(All[i].c_str());
	   iAll.push_back(iElem);
	}
        R(&iAll[0],iAll.size());
        string strArr1=unsplit(iAll);
        printf("%s(%s)=%s\n", argv[2],strArr.c_str(),strArr1.c_str());
#if defined(_WIN32)||defined(_WIN64)
	FreeLibrary(hLib);
#else
        dlclose(hLib);
#endif
	getchar();
	return 0;
}