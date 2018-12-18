#include <stdio.h>
#include <stdlib.h>
#if defined(_WIN32)||defined(_WIN64)
#include<windows.h>
#else
#include <dlfcn.h>
#endif

//g++ -o luatest luatest.cpp -L../lua/ -llua5.1
//extern "C" _declspec(dllexport)void luaL_openlibs(lua_State *L) 
//extern "C" _declspec(dllexport)lua_State *lua_open (void);
//Since the static Lua libraries are written in C, you must import them as such: 
extern "C" 
{
 #include "../lua/lua.h"
 #include "../lua/lualib.h"
 #include "../lua/lauxlib.h"
}
/* the Lua interpreter */
lua_State* L;
int main(int argc, char *argv[])
{
 /* initialize Lua */
 L = lua_open();
 /* load Lua base libraries */
 luaL_openlibs(L);
 /* run the script */
 luaL_dofile(L, "test.lua");
 /* cleanup Lua */
 lua_close(L);
 /* pause */
 printf( "Press enter to exit..." );
 getchar();
 return 0;
}