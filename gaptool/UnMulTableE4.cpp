// 生成偶数阶Abel群U(m+ni)=(Z[i]/(m+ni))^*的凯莱表的小工具UnMulTableE4.exe
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <complex>
#include <vector>
#include <iostream>
using namespace std;

std::complex<long> Div(const std::complex<long> &a,const std::complex<long> &b)
{
 std::complex<float> af(a.real(),a.imag());
 std::complex<float> bf(b.real(),b.imag());
 std::complex<float> cf=af/bf;
 // long cr=cf.real()>0?(long)(cf.real()+0.5):(long)(cf.real()-0.5);
 // long ci=cf.imag()>0?(long)(cf.imag()+0.5):(long)(cf.imag()-0.5);
 long cr=floor(cf.real()+0.5);
 long ci=floor(cf.imag()+0.5); 
 std::complex<long> c(cr,ci);
 return c;
}

bool divide(const std::complex<long>& a, const std::complex<long>& b,std::complex<long>& q, std::complex<long>& r)
{
  //long qr = floor((a/b).real() + 0.5);
  //long qi = floor((a/b).imag() + 0.5);
  //q = std::complex<long>(qr,qi);
  q=Div(a,b);
  r = a - q*b;
  bool bret=(r==std::complex<long>(0,0));
  return bret;
}

std::complex<long> gMod(const std::complex<long> &a,const std::complex<long> &b)
{
  std::complex<long> q=Div(a,b);
  std::complex<long> r=a-q*b;
  return r;
}

long Norm(const std::complex<long>& a)
{
 return (a.real()*a.real()+a.imag()*a.imag());
}

std::complex<long> gcd(const std::complex<long>& a, const std::complex<long>& b)
{
 std::complex<long> x = a, y = b;
 //if(abs(x)<abs(y) )
 if(Norm(x)<Norm(y) )
 {
  std::swap(x,y);
 }
 while ( y != std::complex<long>(0,0) ) {
  std::complex<long> q,r;
  bool ret=divide(x,y,q,r);
  x = y;
  y = r;
 }
 return x;
}

std::complex<long> simple_gcd(const std::complex<long>& a, const std::complex<long>& b)
{
  std::complex<long> aa = a, bb = b;
  //if ( abs(aa) < abs(bb) )
  if(Norm(aa)<Norm(bb) )
  {
      std::swap(aa,bb);
   }
  //while ( abs(bb) != 0)
  while(bb != std::complex<long>(0,0))
   {
      std::complex<long> qq, rr;
      bool bret=divide (aa, bb, qq, rr);
      aa = bb;
      bb = rr;
   }
  return aa;
}

std::complex<long> extended_gcd(const std::complex<long>& a, const std::complex<long>& b,std::complex<long>& x, std::complex<long>& y)
{
 std::complex<long> aa = a, bb = b;
 bool swapped = false;
 //if( abs(aa) < abs(bb) )
 if(Norm(aa)<Norm(bb) )
 {
  std::swap(aa,bb);
  swapped = true;
 }
 std::complex<long> xx = 0, lx = 1, yy = 1, ly = 0;
 do
 {
  std::complex<long> qq, rr;
  bool bret=divide (aa, bb, qq, rr);
  aa = bb; bb = rr;
  std::complex<long> tx = lx - qq*xx;
  lx = xx; xx = tx;
  std::complex<long> ty = ly - qq*yy;
  ly = yy; yy = ty;
 }while (bb != std::complex<long>(0,0));
 x = lx;
 y = ly;
 if (swapped)
 {
  std::swap(x,y);
 }
 return aa;
}

// a helper function not currently being used (although it’s handy,
// for instance, in checking the order of a power of a character).
// harnesses the euclidean algorithm to return the gcd of two numbers.
//利用欧几里得算法计算两个数的最大公约数
int gcd(int a, int b)
{
	//int temp;
	if(b == 0) return a;
	if(a == 0) return b;
	if(a%b == 0) return b;
	else return gcd(b, a%b);
}

vector<int> totient(int num)
{
	vector<int> ret;
	if(num==1)
	{
		vector<int> ret1(1);
		ret1[0]=1;
		return ret1;
	}
	for(int i=1;i<=num-1;i++)
	{
		if(gcd(num,i)==1)
		{
			ret.push_back(i);
		}
	}
	return ret;
}

vector<vector<int> > UnMulTable(int n,const vector<int> &v)
{
	vector<vector<int> > ret(v.size());
	for(int i=0;i<v.size();i++)
	{
		vector<int> I(v.size());
		for(int j=0;j<v.size();j++)
		{
			int ij=(v[i]*v[j])%n;
			int index=-1;
			for(int k=0;k<v.size();k++)
			{
				if(v[k]==ij)
				{
					index=k;
					break;
				}
			}
			I[j]=index+1;
		}
		ret[i]=I;
	}
	return ret;
}

vector<std::complex<long> > totientE4(const std::complex<long>& z)
{
	vector<std::complex<long> > ret;
	long num=Norm(z);
	long nb=gcd(z.real(),z.imag());
	long na=num/nb;
	if(num==1)
	{
		vector<std::complex<long> > ret1(1);
		ret1[0]=std::complex<long>(1,0);
		return ret1;
	}
	for(int i=0;i<na;i++)
	{
		for(int j=0;j<nb;j++)
		{
            std::complex<long> ij=std::complex<long>(i,j);
			std::complex<long> ij1=simple_gcd(ij,z);
	        long num1=Norm(ij1);			
			if(num1==1)
			{
				ret.push_back(ij);
			}
		}
	}
	return ret;
}

vector<vector<int> > UnMulTableE4(const std::complex<long>& z,const vector<std::complex<long> > &v1)
{
	vector<std::complex<long> > v(v1.size());
	for(int j=0;j<v.size();j++)
	{
		v[j]=gMod(v1[j],z);
	}	
	vector<vector<int> > ret(v.size());
	for(int i=0;i<v.size();i++)
	{
		vector<int> I(v.size());
		for(int j=0;j<v.size();j++)
		{
			std::complex<long> ij=gMod(v[i]*v[j],z);
			int index=-1;
			for(int k=0;k<v.size();k++)
			{
				if(v[k]==ij)
				{
					index=k;
					break;
				}
			}
			I[j]=index+1;
		}
		ret[i]=I;
	}
	return ret;
}

string itos(long a)
{
	char sz[20]={0};
	sprintf(sz,"%ld",a);
	return sz;
}

string gStr(const std::complex<long>& z)
{
	string s;
	
	if(z.imag()==0)
		s=itos(z.real());
	else if(z.imag()>0)
	{
		if(z.real()!=0)
			s=itos(z.real())+"+"+itos(z.imag())+"i";
		else
		{
			if(z.imag()==1)
				s="i";
			else if(z.imag()==1)
				s="-i";
			else
				s=itos(z.imag())+"i";
		}
	}
	else
	{
		if(z.real()!=0)
			s=itos(z.real())+itos(z.imag())+"i";
		else
		{
			if(z.imag()==1)
				s="i";
			else if(z.imag()==1)
				s="-i";
			else
				s=itos(z.imag())+"i";
		}
	}
	return s;
}

string UnSetStr(const vector<std::complex<long> > &v1) 
{
	string str;
	for(int i=0;i<v1.size();i++)
	{
		str+=gStr(v1[i]);
		if(i<v1.size()-1)
			str+=",";
	} 
	return str;
}

void PrintUnMulTable(const vector<vector<int> > &vv) 
{
	vector<int>::const_iterator vi;
	vector<vector<int> >::const_iterator vvi;
	for(vvi=vv.begin(); vvi!=vv.end(); vvi++)
	{
		for(vi=(*vvi).begin(); vi!=(*vvi).end(); vi++)
		{
			cout << (*vi)<<" ";
		}
		cout << endl;
	} 
	return;
}

int main(int argc, char **argv)
{
	if( argc < 3 )
	{
		cout<<"Usage:  UnMulTableE4 arg1 arg2"<<endl;
		return 0;
	}
    int a=atoi(argv[1]);
	int b=atoi(argv[2]);
	std::complex<long> z=std::complex<long>(a,b);
	int n=Norm(z);
	if(n>=1)
	{
        vector<std::complex<long> > UnSet=totientE4(z);
		cout<<"模"<<gStr(z)<<"的既约剩余系:"<<UnSetStr(UnSet)<<endl;
		cout<<"Φ("<<gStr(z)<<")="<<UnSet.size()<<endl;
		vector<vector<int> > Un=UnMulTableE4(z,UnSet);
		PrintUnMulTable(Un);
	}

	return 0;
}
