#ifndef _GROUP_UTIL_H
#define _GROUP_UTIL_H

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
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
   
   // 群的最小生成元集、秩(eg:rank(Q_8)=2,rank(D_4)=2)
   static vector<int> rank(const vector<vector<int> > &A);

   static vector<char> lof(const char *fn);

   static vector<char> CharArrToNormal(const vector<char>& vec);

   static string CharArrToStr(const vector<char>& vec);

   static vector<vector<int> > atoTable(const char* strMtx);

   static vector<int> IsLegalMtx(const vector<vector<int> > &mtx);

   static vector<vector<int> > ABmul(const vector<vector<int> > &A,const vector<vector<int> > &B);

   static bool SaveTable(const char *fn,const vector<vector<int> > &A);

   static vector<int>  Factors(int n);

   // 未知n阶群的群元的阶
   static int getGnEOrder(const vector<vector<int> > &A,int a);

   static bool SaveGnEOrder(const char *srcfn,const char *Desfn,const char *DesGn=0);

   // 从路径名中分离出文件名
   static char * find_file_name(char *name);

   // 过滤掉文件名中的扩展名
   static void filter_ext_name(char *name);
};

#endif