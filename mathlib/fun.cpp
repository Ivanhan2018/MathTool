/*
g++ -o fun fun.cpp -L../lua -llua5.1 -ldl
计算定积分
./fun fsimp "fun.lua|fun" 1 2
计算级数
./fun fanx fun.lua 1
./fun fanx "fun.lua|fanx" 1
./fun fan fun.lua
./fun fan "fun.lua|fan"
./fun fsx fun.lua 1
./fun fsx "fun.lua|fsx" 1
./fun fsxy fun.lua 1 2
./fun fsxy "fun.lua|fsxy" 1 2
注意：以上lua脚本文件fun.lua不能是utf-8编码，否则fun运行会报错
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#if defined(_WIN32)||defined(_WIN64)
#include<windows.h>
#else
#include <dlfcn.h>
#endif
#include <string>
#include <vector>
using namespace std;

extern "C" 
{
 #include "../lua/lua.h"
 #include "../lua/lualib.h"
 #include "../lua/lauxlib.h"
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

/* the Lua interpreter */
lua_State  * L;

enum
{
   INVALID_FUNID=-1,
   FAN=0,
   FANX=1,
   FSX=2,
   FSIMP=3,
   FSXY=4,
   RK4=5,
   NUM_FUNID,
};

// 默认lua函数名
char *g_szluafun[NUM_FUNID]={"fan","fanx","fsx","fsimp","fsxy","rk4"};

int funid(char* arg)
{
        if(strcmp(arg,"fan")==0)
        {        
            return FAN;
        }
        if(strcmp(arg,"fanx")==0)
        {        
            return FANX;
        }
        if(strcmp(arg,"fsx")==0)
        {        
            return FSX;
        }
        if(strcmp(arg,"fsimp")==0)
        {        
            return FSIMP;
        }
        if(strcmp(arg,"fsxy")==0)
        {        
            return FSXY;
        }
        if(strcmp(arg,"rk4")==0)
        {        
            return RK4;
        }
        return INVALID_FUNID;
}

// g++版本有可能不支持C++ 11，故用全局变量保存lua函数名
double luafsimp(double x)
{
    double ret;
    //函数名
    lua_getglobal(L,g_szluafun[FSIMP]);
    //第一个参数压栈
    lua_pushnumber(L, x);
    //调用函数
    lua_call(L, 1, 1);
    //得到返回值
    ret=(double)lua_tonumber(L, -1);
    lua_pop(L, 1);
    return ret;
}

double luafanx(int n,double x)
{
    double ret;
    //函数名
    lua_getglobal(L,g_szluafun[FANX]);
    //第一个参数压栈
    lua_pushnumber(L, n);
    //第二个参数压栈
    lua_pushnumber(L, x);
    //调用函数
    lua_call(L, 2, 1);
    //得到返回值
    ret=(double)lua_tonumber(L, -1);
    lua_pop(L, 1);
    return ret;
}

double luafan(int n)
{
    double ret;
    //函数名
    lua_getglobal(L,g_szluafun[FAN]);
    //第一个参数压栈
    lua_pushnumber(L, n);
    //调用函数
    lua_call(L, 1, 1);
    //得到返回值
    ret=(double)lua_tonumber(L, -1);
    lua_pop(L, 1);
    return ret;
}

double luafsx(double x)
{
    double ret;
    //函数名
    lua_getglobal(L,g_szluafun[FSX]);
    //第一个参数压栈
    lua_pushnumber(L,x);
    //调用函数
    lua_call(L,1,1);
    //得到返回值
    ret=(double)lua_tonumber(L, -1);
    lua_pop(L,1);
    return ret;
}

double luafsxy(double x,double y)
{
    double ret;
    //函数名
    lua_getglobal(L,g_szluafun[FSXY]);
    //第一个参数压栈
    lua_pushnumber(L, x);
    //第二个参数压栈
    lua_pushnumber(L, y);
    //调用函数
    lua_call(L, 2, 1);
    //得到返回值
    ret=(double)lua_tonumber(L, -1);
    lua_pop(L, 1);
    return ret;
}

double luark4(double x,double y)
{
    double ret;
    //函数名
    lua_getglobal(L,g_szluafun[RK4]);
    //第一个参数压栈
    lua_pushnumber(L, x);
    //第二个参数压栈
    lua_pushnumber(L, y);
    //调用函数
    lua_call(L, 2, 1);
    //得到返回值
    ret=(double)lua_tonumber(L, -1);
    lua_pop(L, 1);
    return ret;
}

double fanx(double x)
{
 int i;
 double sum = 0;
 for (i = 1;i<=100; i++)
  sum+=luafanx(i,x);
 return sum;
}

double fan()
{
 int i;
 double sum = 0;
 for (i = 1;i<=100; i++)
  sum+=luafan(i);
 return sum;
}

double fsx(double x)
{
 return luafsx(x);
}

double fsxy(double x,double y)
{
 return luafsxy(x,y);
}

int main(int argc, char *argv[])
{
    if(argc<3)
    {
        printf("argc=%d<3.\n", argc);
        return -1;
    }
    char *fun=argv[1];
    string strluafile=argv[2];
    string strluafun=fun;
    vector<string> vstr=split(strluafile,"|");
    if(vstr.size()>1)
    {
       strluafile=vstr[0];
       strluafun=vstr[1];
    }
    int fid=funid(fun);
    printf("fid=%d,luafile=%s,luafun=%s\n",fid,strluafile.c_str(),strluafun.c_str());
    if(fid==INVALID_FUNID)
    {
        printf("无效的fid\n");
        return -1;
    }
    g_szluafun[fid]=(char *)strluafun.c_str();

    //initialize Lua创建一个指向Lua解释器的指针。
    L=lua_open();
    //load Lua base libraries函数加载Lua库
    luaL_openlibs(L);
    //run the script加载脚本
    luaL_dofile(L,strluafile.c_str());
    if(fid==FSIMP)//计算定积分
    {
      if(argc<5)
      {
          printf("argc=%d<5.\n", argc);
          lua_close(L);
          return -1;
      }
      double x=atof(argv[3]);
      double y=atof(argv[4]);
      typedef double(*pR)(double a,double b,double eps,double(*fsimpf)(double));
      void* handle = dlopen("./libmathlib.so", RTLD_LAZY);
      if(!handle)
      {        
            printf("ERROR, Message(%s).\n", dlerror());
            lua_close(L);
            return -1;
      }
      pR R;
      R=(pR)dlsym(handle,fun);
      char* szError = dlerror();
      if(szError != NULL)
      {
        printf("ERROR, Message(%s).\n", szError);
        dlclose(handle);
        lua_close(L);
        return -1;
      }
      if(R != NULL)
      {
        double eps=0.000001;
        double t=R(x,y,eps,luafsimp);
        printf("%e\n",t);
      }
      dlclose(handle);
    }
    else if(fid==RK4)
    {
      if(argc<6)
      {
          printf("argc=%d<6.\n", argc);
          lua_close(L);
          return -1;
      }
      double x=atof(argv[3]);
      double y=atof(argv[4]);
      double z=atof(argv[5]);
      typedef double(*pR)(double(*f)(double x, double y), double x0, double y0, double xn, double h);
      void* handle = dlopen("./libmathlib.so", RTLD_LAZY);
      if(!handle)
      {        
            printf("ERROR, Message(%s).\n", dlerror());
            lua_close(L);
            return -1;
      }
      pR R;
      R=(pR)dlsym(handle,fun);
      char* szError = dlerror();
      if(szError != NULL)
      {
        printf("ERROR, Message(%s).\n", szError);
        dlclose(handle);
        lua_close(L);
        return -1;
      }
      if(R != NULL)
      {
         double h=1e-2;
         double t=R(luark4,x,y,z,h);
         printf("%e\n",t);
      }
      dlclose(handle);
    }
    else if(fid==FAN)
    {        
            double t=fan();
            printf("%e\n",t);
    }
    else if(fid==FANX||fid==FSX)
    {        
            if(argc<4)
            {
               printf("argc=%d<4.\n", argc);
               lua_close(L);
               return -1;
            }
            double x=atof(argv[3]);
            double t=(fid==FANX?fanx(x):fsx(x));
            printf("%e\n",t);
    }
    else if(fid==FSXY)
    {        
            if(argc<5)
            {
               printf("argc=%d<5.\n", argc);
               lua_close(L);
               return -1;
            }
            double x=atof(argv[3]);
            double y=atof(argv[4]);
            double t=fsxy(x,y);
            printf("%e\n",t);
    }

    //cleanup Lua关闭 释放资源
    lua_close(L);
    getchar();
    return 0;
}