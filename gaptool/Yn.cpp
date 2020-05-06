//  生成2*n*n阶群Y_n=(C_n×C_n):C_2的凯莱表Yn.exe
#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<vector<int> > ZnAddTable(int n)
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

int main(int argc, char **argv)
{
	char sz[100]={0};
	if(argc<2)
	{
		printf("请输入n：");
		scanf("%s",sz);
	}
	else
		strcpy(sz,argv[1]);

	int n=atoi(sz);
	char sz1[100]={0};
	sprintf(sz1,"Y%d.txt",n);
	
	vector<vector<int> > Cn=ZnAddTable(n);
	vector<vector<int> > Yn=GC2(DirectProduct(Cn,Cn));
	int m=Yn.size();
	string N0=calN0(Yn);
    printf("%d阶群Y_%d的不变量N0=%s\n",m,n,N0.c_str());
	
	// 生成Y_n的凯莱表
	FILE *fp=fopen(sz1,"w");
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<m;j++)
		{
#ifdef PRINT_STDOUT
			printf("%d ",Yn[i][j]);
#endif
			fprintf(fp,"%d ",Yn[i][j]);
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