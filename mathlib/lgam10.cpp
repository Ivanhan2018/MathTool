//#include "mathlib.h"
#include <stdio.h>
#if defined(_WIN32)||defined(_WIN64)
#include<windows.h>
#else
#include <dlfcn.h>
#endif

int main(int argc, char* argv[])
{
	typedef double(*pR)(double x);
	pR R;
#if defined(_WIN32)||defined(_WIN64)
	HINSTANCE hLib = LoadLibrary("MATHLIB72.DLL");
	if(!hLib) return 0;
	R=(pR)GetProcAddress(hLib,"_lgam1@8");//伽马函数
#else
        void* hLib = dlopen("./libmathlib.so", RTLD_LAZY);
        if(!hLib)
        {        
            printf("ERROR, Message(%s).\n", dlerror());
            return -1;
        }
        R=(pR)dlsym(hLib, "lgam1");
        char* szError = dlerror();
        if(szError != NULL)
        {
         printf("ERROR, Message(%s).\n", szError);
         dlclose(hLib);
         return -1;
        }
#endif

	printf("exit with pressing e:");
	do
	{
		double e,x;
		printf("计算实伽马函数Γ(x)，请输入x：\n");
		fflush(stdin);
		scanf("%lf",&x);
		fflush(stdin);
		printf("Γ(x=%f)=%f\n",x,R(x));
		printf("exit with pressing e:");
	}while(int c = getchar() != 'e')
#if defined(_WIN32)||defined(_WIN64)
	FreeLibrary(hLib);
#else
        dlclose(hLib);
#endif
	getchar();
	return 0;
}