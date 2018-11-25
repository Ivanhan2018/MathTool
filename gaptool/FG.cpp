#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include<algorithm>
using namespace std;

// 有限生成置换群的小工具FG.exe

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

// S_N中置换乘法m*n
vector<int> Mul(int N,const vector<int> & m,const vector<int> & n)
{
	vector<int> tArr(N);
	vector<int> aArr(N);
	vector<int> taArr(N);
	memcpy(&tArr[0],&m[0],sizeof(tArr[0])*N);
	memcpy(&aArr[0],&n[0],sizeof(aArr[0])*N);
	for(int i=0;i<N;i++)
		taArr[i]=aArr[tArr[i]-1];
	vector<int> ret(N);
	memcpy(&ret[0],&taArr[0],sizeof(taArr[0])*N);
	return ret;
}

vector<vector<int> > Order(int N,const vector<int> & m)
{
	vector<vector<int> > ret;
	vector<int> mi=m;
	vector<int> m0(N);
	for(int i=0;i<N;i++)
	{
		m0[i]=i+1;
	}
	while(memcmp(&mi[0],&m0[0],sizeof(int)*N)!=0)
	{
		ret.push_back(mi);
		mi=Mul(N,mi,m);
	}
	ret.push_back(mi);
	return ret;
}

int IsInFG(int N,const vector<vector<int> > FG,const vector<int> & m)
{
	for(int i=0;i<FG.size();i++)
	{
		if(memcmp(&m[0],&FG[i][0],sizeof(int)*N)==0)
			return i;
	}
	return -1;
}

int main(int argc, char* argv[])
{
	char sz[100]={0};
	char sz1[100]={0};
	if(argc<2)
	{
		printf("请输入有限生成置换群A的生成元文件名：");
		scanf("%s",sz);
	}
	else
		strcpy(sz,argv[1]);
	strcpy(sz1,sz);
	strcat(sz1,".txt");

	ifstream fin(sz);
	if( !fin ) {
		cerr << "Error opening input stream" << endl;
	        //system("pause");
		return -1;
	}
	else
	{
		string strLine;
		fin >> strLine;

		vector<string> vN=split(strLine,",");
	        int N=vN.size();

		vector<vector<int> > FG;
		vector<int> E;
		for(int i=0;i<N;i++)
		{
			E.push_back(i+1);
		}
		FG.push_back(E);

		while(strLine!="")
		{
			strLine="";
		    fin >> strLine;

			vector<string> vN1=split(strLine,",");
			int N1=vN1.size();
			if(N1==N)
			{
				vector<int> viN1(N);
				for(int i=0;i<N;i++)
				{
					viN1[i]=atoi(vN1[i].c_str());
				}
				if(memcmp(&E[0],&viN1[0],sizeof(int)*N)!=0)
				{
					FG.push_back(viN1);
				}
			}
		}

		int R=FG.size()-1;
		int cnt=R+1;
		int cnt1=R+1;
		do{
			cnt=FG.size();
			for(int i=1;i<cnt;i++)
			{
				for(int j=1;j<cnt;j++)
				{
					vector<int> IJ=Mul(N,FG[i],FG[j]);
					int bIn=IsInFG(N,FG,IJ);
					if(bIn==-1)
					{
						//cout<<FG.size()<<"="<<i<<"*"<<j<<endl;
						FG.push_back(IJ);
					}
				}
			}
			cnt1=FG.size();
		}while(cnt1>cnt);

		ofstream fout(sz1);
		int n=FG.size();
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<n;j++)
			{
				vector<int> IJ=Mul(N,FG[i],FG[j]);
				int bIn=IsInFG(N,FG,IJ);
				fout<<bIn+1<<" ";
			}
			fout<<endl;
		}
		cout<<FG.size()<<"阶群的凯莱表生成完毕！"<<endl;
	}

	//system("pause");
	return 0;
}
