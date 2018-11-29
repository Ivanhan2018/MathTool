#ifndef POLY_UTIL__H
#define POLY_UTIL__H

#include<cmath>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;

struct DComplex
{
	double x;
	double y;
};

class PolyUtil
{
public:
        // 多项式a乘以多项式b得到多项式c，deg(a)=m-1,deg(b)=n-1,deg(c)=m+n-2=k
        static int polymul(int a[],int m,int b[],int n,int c[],int *k); 

        // 复数域多项式乘法
        static int polyCmul(DComplex a[],int m,DComplex b[],int n,DComplex c[],int *k); 
        static vector<DComplex> polyCmul(vector<DComplex> &a,vector<DComplex> &b);
        static vector<DComplex> polyCmul(DComplex cnpr[],int m);
        // 返回true，表示能转化成整系数多项式
        static bool CPolyToZPoly(const vector<DComplex> &c,vector<int> &z);
        // 复数乘法
        static DComplex DCMul(const DComplex &a,const DComplex &b);
        // 复数加法
        static DComplex DCAdd(const DComplex &a,const DComplex &b);
        // 复数的逆
        static DComplex DCAddInv(const DComplex &a);
        // 构造复数对象
        static DComplex DComplexObj(double x,double y);
        // 返回n个n次单位根
        static vector<DComplex>  CRoot(const DComplex &z,int n);

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