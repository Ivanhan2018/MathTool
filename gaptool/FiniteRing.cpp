//  生成有限环的加法、乘法凯莱表FiniteRing.exe
// 解析一个多项式的系数和指数
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;

class StringUtil
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
	static bool parsePoly(const char* strA,vector<vector<int>>& result);

};

//   原始串
//   替换源串
//   替换目的串
string StringUtil::stringReplace(const  string&   input,const   string&   find,const   string&   replaceWith)   
{   
	string   strOut(input);
	int   curPos   =   0;   

	int   pos;
	while((pos=strOut.find(find,curPos))!=-1)   
	{   
		strOut.replace(pos,find.size(),replaceWith);             //   一次替换   
		curPos=pos+replaceWith.size();                     //   防止循环替换!!   
	}   

	return   strOut;   
}   

/** Returns a std::stringVector that contains all the substd::strings delimited
   by the characters in the passed <code>delims</code> argument.
   @param 
   delims A list of delimiter characters to split by
   @param 
   maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
   parameters is > 0, the splitting process will stop after this many splits, left to right.
   */
std::vector<string> StringUtil::split( const std::string& str, const std::string& delims/* = "\t\n "*/, unsigned int maxSplits/* = 0*/)
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

void StringUtil::trimAllSpace(std::string &str)
{
	std::string::iterator destEnd=std::remove_if(str.begin(),str.end(),isspace);
	str.resize(destEnd-str.begin());
}

vector<int> StringUtil::CoefAndDeg(const string &str)
{
	string s=str;
	vector<int> ret;
	int Coef=0;
	int Deg=0;
	unsigned int loc = s.find( "x", 0 );
	if(loc != string::npos)
	{
		unsigned int loc1 = s.find( "x^", 0 );
		if(loc1 == string::npos)
		{
			Deg=1;
		}
		else
		{
	        string strR=s.substr(loc1+2,s.length()-loc1-2);
			Deg=atoi(strR.c_str());
		}
		if(loc==0)
		{
			Coef=1;
		}
		else
		{
			if(loc==1 && s[0]=='-')
		    {
			   Coef=-1;
		    }
			else
			{
			   	string strL=s.substr(0,loc1+1);
			    Coef=atoi(strL.c_str());
			}
		}
		ret.push_back(Coef);
		ret.push_back(Deg);
	}
	else
	{
		ret.push_back(atoi(s.c_str()));
		ret.push_back(0);
	}
	return ret;
}

bool StringUtil::parsePoly(const char* strA,vector<vector<int>>& result)
{
	if(strA!=0)
	{
		result.clear();
		string StrA=StringUtil::stringReplace(strA,"-","+-");
		vector<vector<int>> vMonomials;
		vector<string> strArr=StringUtil::split(StrA,"+");
		int nSize=strArr.size();
		for(int i=0;i<nSize;i++)
		{
			StringUtil::trimAllSpace(strArr[i]);
			vector<int> cds=StringUtil::CoefAndDeg(strArr[i]);
			result.push_back(cds);
		}
		return true;
	}
	return false;
}

// 多项式a乘以多项式b得到多项式c，deg(a)=m-1,deg(b)=n-1,deg(c)=m+n-2=k
int polymul(int a[],int m,int b[],int n,int c[],int *k) 
{ 
 int i,j; 
 if(k)
  *k=m+n-2;
 for(i=0;i<m+n-2;i++) 
  c[i]=0; 
 for(i=0;i<m;i++) 
  for(j=0;j<n;j++) 
   c[i+j]=a[i]*b[j]+c[i+j];
 return 0;
} 

//将一个数字转换成字符串
string NumberTostring(long Num)   
{
	char buffer[100]={0};
	sprintf(buffer,"%d",Num);
	string strBuf=buffer;

	return strBuf;
}

string sPoly(int a[],int n)
{
	string str="[";
	for(int i=0;i<n;i++)
	{
		str+=NumberTostring(a[i]);
		if(i<n-1)
		{
			str+=",";
		}
	}
	str+="]";
	return str;
}

int indexofmax1(int value)
{
	int tmp=1;
	int count=0;
	for(int i=0;i<sizeof(int)*8;++i)
	{
		if((value&tmp))
			count=i;
		tmp=tmp*2;
	}
	return count;
}
void polynomialtostring(int value)
{
	int tmp=1;
	int flag=0;
	int c=indexofmax1(value);
	for(int i=0;i<sizeof(int)*8;++i)
	{
		if((value&tmp))
		{
			if(i==0)
			{
				cout<<"1";
			}else if(i==1)
			{
				cout<<"x";
			}else
			{
				cout<<"x^"<<i;
			}
			flag=1;
			if(i<c)
				cout<<"+";
		}
		tmp=tmp*2;
	}
	if(flag==0)
		cout<<"0";
}
int powofvalue(int value)
{
	return 1<<(value);
}
int divide(int m,int b,int &remainvalue)
{
	int mindex=indexofmax1(m);
	int vindex=indexofmax1(b);
	if(mindex<vindex)
	{
		remainvalue=m;
		return 0;
	}
	int c=mindex-vindex;
	int tmp=b;
	tmp=tmp<<c;
	m=m^tmp;
	return powofvalue(c)|divide(m,b,remainvalue);
}
int Tx(int ax,int q,int bx)
{
	//cout<<endl;
	//cout<<ax<<"\t"<<bx<<"\t";
	int tmp=1;
	int value=0;
	for(int i=0;i<sizeof(int)*8;++i)
	{
		if((q&tmp))
		{
			value=value^((bx<<i));
		}
		tmp=tmp*2;
	}
	//cout<<ax<<"\t"<<value<<"\t";
	//cout<<endl;
	return ax^(value);
}
int extent_gcd(int m,int b,int &x,int &y)
{
	int a1=1,a2=0,a3=m;
	int b1=0,b2=1,b3=b;
	int remainvalue=0;
	while(1)
	{
/*
		polynomialtostring(a1);
		cout<<" ";
		polynomialtostring(a2);
		cout<<" ";
		polynomialtostring(a3);
		cout<<" ";
		polynomialtostring(b1);
		cout<<" ";
		polynomialtostring(b2);
		cout<<" ";
		polynomialtostring(b3);
		cout<<" ";
		*/
		if(b3==0)
			return a3;
		if(b3==1)
			return b3;
		int q=divide(a3,b3,remainvalue);
		int t1=Tx(a1,q,b1);
		int t2=Tx(a2,q,b2);
		int t3=remainvalue;
		cout<<"t1="<<t1<<endl;
		cout<<"t2="<<t2<<endl;
		a1=b1;a2=b2;a3=b3;
		b1=t1;b2=t2;b3=t3;
		x=b2;y=b3;
		//polynomialtostring(q);
		//cout<<endl;
	}
}

void test1()
{
	int m=283,b=83,x=0,y=0;
	//cout<<"中间结果如下:"<<endl;
	//cout<<"a1 a2 a3 b1 b2 b3 q"<<endl;
	int r1=0;
	int q1=divide(m,b,r1);
	int ret=extent_gcd(m,b,x,y);
	cout<<b<<","<<m<<"->"<<q1<<","<<r1<<"->"<<ret<<","<<x<<","<<y<<endl;
	cout<<"多项式(";polynomialtostring(b);cout<<")mod(";polynomialtostring(m);cout<<")的乘法逆元是(";polynomialtostring(x);cout<<")"<<endl;	
}

int main(int argc, char **argv)
{
	int a[3]={0,1,2};//0x^0+1x^1+2x^2
	int b[2]={3,1};//3x^0+1x^1
	const int m=sizeof(a)/sizeof(int);
	const int n=sizeof(b)/sizeof(int);
	int k=0;
	int c[m+n-1]={0};//0x^0+3x^1+7x^2+2x^3
	int ret=polymul(a,m,b,n,c,&k);
	cout<<sPoly(a,m)<<"*"<<sPoly(b,n)<<"="<<sPoly(c,m+n-1)<<endl;
	
	const char* strA=" 7x^4   +  x^2";
	vector<vector<int> > vMonomialsA;
	bool bretA=StringUtil::parsePoly(strA,vMonomialsA);

	const char* strB="6x^3 - 3x^2";
	vector<vector<int> > vMonomialsB;
	bool bretB=StringUtil::parsePoly(strB,vMonomialsB);
	
    test1();
	//system("pause");
	return 0;
}