//#include "mathlib.h"
#include <stdio.h>
#include <dlfcn.h>

//定义函数接口
typedef void (*pconv)(float *xin, float *h, int lx, int lh,float *xout);//线性卷积


int main()
{
float a[4],b[3],xout[6];
a[0]=1;a[1]=2;a[2]=3;a[3]=4;
b[0]=1;b[1]=4;b[2]=5;
//动态加载so 
#if 1 
    void* handle = dlopen("/home/hanxiaohua/mathlib/libmathlib.so", RTLD_LAZY);
    if(!handle)
    {        
            printf("ERROR, Message(%s).\n", dlerror());
            return -1;
    }

    pconv g_Test = (pconv)dlsym(handle, "conv");
    char* szError = dlerror();
    if(szError != NULL)
    {
        printf("ERROR, Message(%s).\n", szError);
        dlclose(handle);
        return -1;
    }
    if(g_Test != NULL)
    {
        g_Test(a,b,4,3,xout);
    }
    dlclose(handle);
    g_Test = NULL;
    //return 0;
#else
conv(a,b,4,3,xout);
#endif
for (int i = 0; i <6; i++)
printf(" %f",xout[i]);
return 0;
}