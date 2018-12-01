#ifndef _GROUP_UTIL_H
#define _GROUP_UTIL_H

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class GroupUtil
{
public:
   static std::vector<string> split( const std::string& str, const std::string& delims, unsigned int maxSplits = 0);
   
   // S_N中置换乘法m*n
   static vector<int> Mul(int N,const vector<int> & m,const vector<int> & n);

   static vector<vector<int> > Order(int N,const vector<int> & m);

   static int IsInFG(int N,const vector<vector<int> > FG,const vector<int> & m);

   // 有限生成置换群的小工具FG.exe
   static bool FG(const char *szFG,const char *szfilename);
};

#endif