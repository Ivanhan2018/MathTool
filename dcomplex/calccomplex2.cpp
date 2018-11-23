//#include "dcomplex.h"
#include <stdio.h>
#include <stdlib.h>
#if defined(_WIN32)||defined(_WIN64)
#include<windows.h>
#else
#include <dlfcn.h>
#endif

struct DComplex
{
	double x;
	double y;
};

int main(int argc, char* argv[])
{
        if(argc<7)
        {
            printf("argc=%d<7.\n", argc);
            return -1;
        }
	typedef DComplex(*pR)(DComplex* x,DComplex* y);
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
        double x=atof(argv[3]);
        double y=atof(argv[4]);
        DComplex z={x,y};
        double x1=atof(argv[5]);
        double y1=atof(argv[6]);
        DComplex z1={x1,y1};
        DComplex w=R(&z,&z1);  
        printf("%s((%f,%f),(%f,%f))=(%f,%f)\n", argv[2],x,y,x1,y1,w.x,w.y);
#if defined(_WIN32)||defined(_WIN64)
	FreeLibrary(hLib);
#else
        dlclose(hLib);
#endif
	getchar();
	return 0;
}