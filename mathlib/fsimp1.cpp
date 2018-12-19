//C/C++宿主代码为：
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#if defined(_WIN32)||defined(_WIN64)
#include<windows.h>
#else
#include <dlfcn.h>
#endif

//g++ -o fsimp1 fsimp1.cpp -L../lua -llua5.1 -ldl
extern "C" 
{
 #include "../lua/lua.h"
 #include "../lua/lualib.h"
 #include "../lua/lauxlib.h"
}
/* the Lua interpreter */
lua_State  * L;

double luafun(double x)
{
    double ret;
    //函数名
    lua_getglobal(L, "fun");
    //第一个参数压栈
    lua_pushnumber(L, x);
    //调用函数
    lua_call(L, 1, 1);
    //得到返回值
    ret=(double)lua_tonumber(L, -1);
    lua_pop(L, 1);
    return ret;
}

// g++版本有可能不支持C++ 11，故用全局变量保存lua函数名
char g_szfun[200]="fun";
double luafun1(double x)
{
    double ret;
    //函数名
    lua_getglobal(L, g_szfun);
    //第一个参数压栈
    lua_pushnumber(L, x);
    //调用函数
    lua_call(L, 1, 1);
    //得到返回值
    ret=(double)lua_tonumber(L, -1);
    lua_pop(L, 1);
    return ret;
}

int main(int argc, char *argv[])
{
    int sum;
    double ret;
    //initialize Lua创建一个指向Lua解释器的指针。
    L=lua_open();
    //load Lua base libraries函数加载Lua库
    luaL_openlibs(L);
    //run the script加载脚本
    luaL_dofile(L,"fsimp1.lua");

    //计算定积分
    typedef double(*pR)(double a,double b,double eps,double(*fsimpf)(double));
    void* handle = dlopen("./libmathlib.so", RTLD_LAZY);
    if(!handle)
    {        
            printf("ERROR, Message(%s).\n", dlerror());
            return -1;
    }
    pR R;
    R=(pR)dlsym(handle, "fsimp");
    char* szError = dlerror();
    if(szError != NULL)
    {
        printf("ERROR, Message(%s).\n", szError);
        dlclose(handle);
        return -1;
    }
    if(R != NULL)
    {
        double a,b,eps,t;
        a=0.0;b=sqrt(2);eps=0.000001;
        t=R(a,b,eps,luafun1);
        printf("fsimp(a,b,eps,luafun)=%e\n",t);
	do{
          double a,b,eps,t;
          char sz[200];
          printf("调用fsimp计算某些定积分∫[a->b]f(x)dx，请输入a、b以及包含f(x)的lua脚本文件名、lua函数名,这里eps=0.000001：\n");
          fflush(stdin);
          scanf("%lf%lf%s%s",&a,&b,sz,g_szfun);
          fflush(stdin);
          eps=0.000001;
          luaL_dofile(L,sz);
          t=R(a,b,eps,luafun1);
          printf("fsimp(a,b,eps,luafun)=%e\n",t);
	  printf("exit with pressing e:");
        }while(getchar()!='e');
    }
    printf("\n");
    dlclose(handle);

    //cleanup Lua关闭 释放资源
    lua_close(L);
    getchar();
    return 0;
}