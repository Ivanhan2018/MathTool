#include "PolyUtil.h"

//   原始串
//   替换源串
//   替换目的串
string PolyUtil::stringReplace(const  string&   input,const   string&   find,const   string&   replaceWith)   
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
std::vector<string> PolyUtil::split( const std::string& str, const std::string& delims/* = "\t\n "*/, unsigned int maxSplits/* = 0*/)
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

bool isspace1(unsigned char c)
{
  return c==' ';
}

void PolyUtil::trimAllSpace(std::string &str)
{
	std::string::iterator destEnd=std::remove_if(str.begin(),str.end(),isspace1);
	str.resize(destEnd-str.begin());
}

vector<int> PolyUtil::CoefAndDeg(const string &str)
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

bool PolyUtil::parsePoly(const char* strA,vector<vector<int> >& result)
{
	if(strA!=0)
	{
		result.clear();
		string StrA=stringReplace(strA,"-","+-");
		vector<vector<int> > vMonomials;
		vector<string> strArr=split(StrA,"+");
		int nSize=strArr.size();
		for(int i=0;i<nSize;i++)
		{
			trimAllSpace(strArr[i]);
			vector<int> cds=CoefAndDeg(strArr[i]);
			result.push_back(cds);
		}
		return true;
	}
	return false;
}
