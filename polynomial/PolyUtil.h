#ifndef POLY_UTIL__H
#define POLY_UTIL__H

#include<vector>
#include<string>
#include<algorithm>
using namespace std;

class PolyUtil
{
public:
	static string stringReplace(const  string&   input,const   string&   find,const   string&   replaceWith);

	// 根据所给字符对字符串进行分割
	static std::vector<string> split( const std::string& str, const std::string& delims = "\t\n ", unsigned int maxSplits = 0);

	// 去掉所有空格
	static void trimAllSpace(std::string &str);

	// 得到单项式的系数和次数
	static vector<int> CoefAndDeg(const string &str);

	// 解析多项式
	static bool parsePoly(const char* strA,vector<vector<int> >& result);
};

#endif