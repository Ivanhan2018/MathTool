#ifndef _RING_UTIL_H
#define _RING_UTIL_H

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class RingUtil
{
public:
   static std::vector<string> split( const std::string& str, const std::string& delims, unsigned int maxSplits = 0);
   
   // M_N(Z/nZ)中零元矩阵
   static vector<vector<int> > Zero(int N);
   // M_N(Z/nZ)中矩阵加法a*b
   static vector<vector<int> > Add(int N,int n,const vector<vector<int> > & a,const vector<vector<int> > & b);
   // M_N(Z/nZ)中矩阵乘法a*b
   static vector<vector<int> > Mul(int N,int n,const vector<vector<int> > & a,const vector<vector<int> > & b);
   static bool Brmul(int p,int *a,int *b,int m,int n,int k,int *c);
   static bool IsEqual(int N,const vector<vector<int> > & m,const vector<vector<int> > & n);
   static int IsInFR(int N,const vector<vector<vector<int> > > & FR,const vector<vector<int> > & m);

   // 有限生成矩阵环M_N(Z/nZ)的小工具FR.exe
   static bool FR(const char *szFR,const char *szfilename);
};

#endif