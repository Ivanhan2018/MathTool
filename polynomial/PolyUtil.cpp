#include "PolyUtil.h"

void ontrt(double x,double y,int n,double *u,double *v)
{ 
	int k;
	double r,q,t;
	if(n<1) 
		return;
	q=atan2(y,x);
	r=sqrt(x*x+y*y);
	if(r+1.0!=1.0)
	{ 
		r=(1.0/n)*log(r); 
		r=exp(r);
	}
	for(k=0; k<=n-1; k++)
	{ 
		t=(2.0*k*3.1415926+q)/n;
		u[k]=r*cos(t); 
		v[k]=r*sin(t);
	}
	return;
}

vector<DComplex>  PolyUtil::CRoot(const DComplex &z,int n)
{
	vector<DComplex> ret;
	vector<double> u(n);
	vector<double> v(n);
	ontrt(z.x,z.y,n,&u[0],&v[0]);
	for(int i=0;i<n;i++)
	{
		DComplex uv={u[i],v[i]};
		ret.push_back(uv);
	}

	return ret;
}

// 多项式a乘以多项式b得到多项式c，deg(a)=m-1,deg(b)=n-1,deg(c)=m+n-2=k
int PolyUtil::polymul(int a[],int m,int b[],int n,int c[],int *k) 
{ 
 int i,j; 
 *k=m+n-2;
 for(i=0;i<m+n-2;i++) 
  c[i]=0; 
 for(i=0;i<m;i++) 
  for(j=0;j<n;j++) 
   c[i+j]=a[i]*b[j]+c[i+j];
 return 0;
}

void ocmul(double a,double b,double c,double d,double *e,double *f)
{ 
    double p,q,s;
    p=a*c; q=b*d; s=(a+b)*(c+d);
    *e=p-q; *f=s-p-q;
    return;
}

DComplex PolyUtil::DCMul(const DComplex &a,const DComplex &b)
{
	DComplex c;
	ocmul(a.x,a.y,b.x,b.y,&c.x,&c.y);
	return c;
}

DComplex PolyUtil::DCAdd(const DComplex &a,const DComplex &b)
{
	DComplex c={a.x+b.x,a.y+b.y};
	return c;
}

DComplex PolyUtil::DCAddInv(const DComplex &a)
{
	DComplex c={-a.x,-a.y};
	return c;
}

DComplex PolyUtil::DComplexObj(double x,double y) 
{
	DComplex c={x,y};
	return c;
}

// 复数域多项式乘法
int PolyUtil::polyCmul(DComplex a[],int m,DComplex b[],int n,DComplex c[],int *k) 
{ 
 int i,j; 
 *k=m+n-2;
 for(i=0;i<m+n-2;i++) 
 {
    c[i].x=0;
	c[i].y=0;
 }
 for(i=0;i<m;i++) 
  for(j=0;j<n;j++) 
   c[i+j]=DCAdd(DCMul(a[i],b[j]),c[i+j]);
 return 0;
}

vector<DComplex> PolyUtil::polyCmul(vector<DComplex> &a,vector<DComplex> &b) 
{ 
 vector<DComplex> c(a.size()+b.size()-1);
 int k=0;
 int ret=polyCmul(&a[0],a.size(),&b[0],b.size(),&c[0],&k);
 return c;
}

vector<DComplex> PolyUtil::polyCmul(DComplex cnpr[],int m) 
{
	vector<vector<DComplex> > arr;
	for(int i=0;i<m;i++)
	{
	     vector<DComplex> a;
	     a.push_back(DCAddInv(cnpr[i]));
	     a.push_back(DComplexObj(1,0));
	     arr.push_back(a);
	}
	vector<DComplex> ret;
	int n=arr.size();
	if(n==0)
		return ret;
	if(n==1)
		return arr[0];
	ret=arr[0];
	for(int i=1;i<n;i++)
	{ 
		vector<DComplex> retMul=polyCmul(ret,arr[i]);
		ret=retMul;
	}
	return ret;
}

// 返回true，表示能转化成整系数多项式
bool PolyUtil::CPolyToZPoly(const vector<DComplex> &c,vector<int> &z)
{
	z.clear();
	int n=c.size();
	for(int i=0;i<n;i++)
	{
	    int ix=floor(c[i].x+0.5);
		int iy=floor(c[i].y+0.5);
		if(fabs(c[i].x-ix)<0.01 && iy==0 && fabs(c[i].y-iy)<0.01)
		{
		   z.push_back(ix);
		}
		else
		{
		    return false;
		}
	}
	return true;
}

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
