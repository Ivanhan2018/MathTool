//  生成n阶Abel群AbelianGroup(L)及其对应的2n阶广义二面体群GeneralizedDihedralGroup(L)=AbelianGroup(L):C_2的凯莱表GD.exe，L=[n]时即为二面体群DihedralGroup(2n)，L=[n,n]时即为Y_n
#include <iostream>
#include <vector>
#include <string>
using namespace std;

std::vector<string> split( const std::string& str, const std::string& delims, unsigned int maxSplits = 0)
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

vector<int> IsLegalMtx(const vector<vector<int>> &mtx)
{
 vector<int> ret(3);
 int illegal=-1;
 ret[1]=mtx.size();
 if(ret[1]==0)
 {
  ret[0]=illegal;//不是合法矩阵
  return ret;
 }
 ret[2]=mtx[0].size();
 if(ret[2]==0)
 {
  ret[0]=illegal;//不是合法矩阵
  return ret;
 }
 for(int i=1;i<ret[1];i++)
 {
  if(mtx[i].size()!=ret[2])
  {
   ret[0]=illegal;//不是合法矩阵
   return ret;
  }
 }
 ret[0]=0;//是合法矩阵
 return ret;
}

vector<vector<int>> DirectProduct(const vector<vector<int>> &A,const vector<vector<int>> &B)
{
 vector<vector<int>> C;
 vector<int> AFlag=IsLegalMtx(A);
 if(AFlag[0]==-1)
  return C;
 vector<int> BFlag=IsLegalMtx(B);
 if(BFlag[0]==-1)
  return C;
 int nB=BFlag[1];
 int n=AFlag[1]*BFlag[1];

 for(int a=0;a<n;a++)
 {
  vector<int> iRow;
  for(int b=0;b<n;b++)
  {
   int aj=a%nB;
   int ai=a/nB;
   int bj=b%nB;
   int bi=b/nB;
   int i=A[ai][bi]-1;
   int j=B[aj][bj]-1;
   int c=nB*i+j+1;
   iRow.push_back(c);
  }
  C.push_back(iRow);
 }
 return C;
}

// 半直积群G:C2
vector<vector<int>> GC2(const vector<vector<int>> &A)
{
	vector<vector<int>> C;
	vector<int> AFlag=IsLegalMtx(A);
	if(AFlag[0]==-1)
		return C;
	
	vector<vector<int>> B;
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	vector<int> v2;
	v2.push_back(2);
	v2.push_back(1);
	B.push_back(v1);
	B.push_back(v2);

	vector<int> BFlag=IsLegalMtx(B);
	if(BFlag[0]==-1)
		return C;
	int nB=BFlag[1];
	int n=AFlag[1]*BFlag[1];

	for(int a=0;a<n;a++)
	{
		vector<int> iRow;
		for(int b=0;b<n;b++)
		{
			int aj=a%nB;
			int ai=a/nB;
			int bj=b%nB;
			int bi=b/nB;
			int bii=bi;
			//aj不是C_2={0,1}的单位元0
			if(aj==1)
			{
				for(int ii=0;ii<AFlag[1];ii++)
				{
					if(A[bi][ii]==1)
					{
						bii=ii;
						break;
					}
				}
			}
			int i=A[ai][bii]-1;
			int j=B[aj][bj]-1;
			int c=nB*i+j+1;
			iRow.push_back(c);
		}
		C.push_back(iRow);
	}
	return C;
}

vector<int>  Factors(int n)
{
 vector<int> ret;
 if(n<1)
  return ret;
 for(int i=1;i<=n;i++)
 {
  if(n%i==0)
  {
   ret.push_back(i);
  }
 }
 return ret;
}

// 未知n阶群的群元的阶
int getGnEOrder(const vector<vector<int>> &A,int a)
{
 vector<int> AFlag=IsLegalMtx(A);
 if(AFlag[0]==-1)
  return -1;
 int n=AFlag[1];
 if(a<0||a>=n)
  return -1;
 int t=0;
 for(int i=1;i<=n;i++)
 {
  t=A[t][a]-1;
  if(t==0)
   return i;
 }
 return -1;
}

//利用欧几里得算法计算两个数的最大公约数
int gcd(int a, int b)
{
	int temp;
	if(b == 0) return a;
	if(a == 0) return b;
	if(a%b == 0) return b;
	else return gcd(b, a%b);
}

int lcm(const vector<int>& v)
{
 int n=v.size();
 if(n<1)
  return 0;
 if(n==1)
  return v[0];
 int ret=v[0];
 for(int i=1;i<n;i++)
 {
  ret=(ret*v[i]/gcd(ret,v[i]));
 }
 return ret;
}

int Exponent(const vector<vector<int>> &A)
{
 vector<int> v;
 int n=A.size();
 vector<int> vOrders=Factors(n);
 vector<int> vCounts(n+1);
 for(int i=0;i<n;i++)
 {
  int ord=getGnEOrder(A,i);
  vCounts[ord]=vCounts[ord]+1;
 }
 for(int i=0;i<vOrders.size();i++)
 {
  if(vCounts[vOrders[i]]!=0)
	  v.push_back(vOrders[i]);
 }
 int n0=lcm(v);
 return n0;
}

string calN0(const vector<vector<int>> &A)
{
 int n=A.size();
 vector<int> vOrders=Factors(n);
 vector<int> vCounts(n+1);
 for(int i=0;i<n;i++)
 {
  int ord=getGnEOrder(A,i);
  vCounts[ord]=vCounts[ord]+1;
 }
 string str="[";
 for(int i=0;i<vOrders.size();i++)
 {
  char sz[40]={0};
  sprintf(sz,"%d",vCounts[vOrders[i]]);
  str+=sz;
  if(i<vOrders.size()-1)
	  str+=",";
 }
 str+="]";
 return str;
}

vector<vector<int> > CyclicGroup(int n)
{
	vector<vector<int> > ret(n,vector<int>(n,0));
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			ret[i][j]=(i+j)%n+1;
		}
	}
    return ret;	
}

vector<vector<int> > AbelianGroup(const vector<int>& L)
{
 	vector<vector<int> > ret;
	int n=L.size();
	if(n<1)
		return ret;
	ret=CyclicGroup(L[0]);
	for(int i=1;i<n;i++)
	{
		vector<vector<int> > Ci=CyclicGroup(L[i]);
		ret=DirectProduct(ret,Ci);	
	}
	return ret;	
}

int main(int argc, char **argv)
{
	char sz[100]={0};
	if(argc<2)
	{
		printf("请输入L：");
		scanf("%s",sz);
	}
	else
		strcpy(sz,argv[1]);
	
	 string str=sz;
	 vector<int> L;
	 vector<string> All=split(str,",");
	  for(int i=0;i<All.size();i++)
	  {
	   int a=atoi(All[i].c_str());
	   L.push_back(a);
	  }
	
	vector<vector<int> > A=AbelianGroup(L);
	vector<vector<int> > GD=GC2(A);
	
	int nA=A.size();
	string N0A=calN0(A);
	int n0A=Exponent(A);
    printf("%d阶Abel群AbelianGroup(%s)的不变量N0=%s,指数n0=%d\n",nA,sz,N0A.c_str(),n0A);
	
	char sz1[100]={0};
	sprintf(sz1,"G%d.txt",nA);
	// 生成AbelianGroup(L)的凯莱表
	FILE *fp=fopen(sz1,"w");
	for(int i=0;i<nA;i++)
	{
		for(int j=0;j<nA;j++)
		{
#ifdef PRINT_STDOUT
			printf("%d ",A[i][j]);
#endif
			fprintf(fp,"%d ",A[i][j]);
		}
#ifdef PRINT_STDOUT
			printf("\n");
#endif		
		fprintf(fp,"\n");
	}
	fclose(fp);
	
	int nGD=GD.size();
	string N0GD=calN0(GD);
	int n0GD=Exponent(GD);	
    printf("%d阶广义二面体群GeneralizedDihedralGroup(%s)的不变量N0=%s,指数n0=%d\n",nGD,sz,N0GD.c_str(),n0GD);
	
	char sz2[100]={0};
	sprintf(sz2,"GD%d.txt",nGD);	
	// 生成广义二面体群GeneralizedDihedralGroup(L)的凯莱表
	fp=fopen(sz2,"w");
	for(int i=0;i<nGD;i++)
	{
		for(int j=0;j<nGD;j++)
		{
#ifdef PRINT_STDOUT
			printf("%d ",GD[i][j]);
#endif
			fprintf(fp,"%d ",GD[i][j]);
		}
#ifdef PRINT_STDOUT
			printf("\n");
#endif		
		fprintf(fp,"\n");
	}
	fclose(fp);	

	system("pause");
	return 0;
}