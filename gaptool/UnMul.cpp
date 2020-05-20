// 生成偶数阶Abel群U(n)=(Z/nZ)^*的凯莱表的小工具UnMulTable.exe
#include "FiniteGroup.h"
#include <iostream>
#include <complex>

/*
int gcd(int a, int b)
{
	//int temp;
	if(b == 0) return a;
	if(a == 0) return b;
	if(a%b == 0) return b;
	else return gcd(b, a%b);
}
*/

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

vector<int> V2ToV1(const vector<vector<int> > &vv)
{
	int n=vv.size();
	vector<int> ret(n*n);
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
		{
			ret[i*n+j]=vv[i][j];
		}
	return ret;
}

vector<vector<int> > UnMulTable(int n,const vector<int> &v)
{
	vector<vector<int>> ret(v.size());
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

void PrintUnMulTable(const vector<vector<int>> &vv) 
{
	vector<int>::const_iterator vi;
	vector<vector<int>>::const_iterator vvi;
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

	char sz[100]={0};
	if(argc<2)
	{
		printf("请输入正整数n：");
		scanf("%s",sz);
	}
	else
		strcpy(sz,argv[1]);

	int n=atoi(sz);
	if(n>=1)
	{
		vector<vector<int>> Un=UnMulTable(n,totient(n));
		vector<int> vUn=V2ToV1(Un);
		FiniteGroup g(Un.size(),&vUn[0],1);
		g.printTable();	
		PrintUnMulTable(Un);
	}

	//system("pause");
	return 0;
}
