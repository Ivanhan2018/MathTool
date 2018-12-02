//#include "stdafx.h"
#include "GroupUtil.h"

std::vector<string> GroupUtil::split( const std::string& str, const std::string& delims, unsigned int maxSplits /*= 0*/)
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
vector<int> GroupUtil::Mul(int N,const vector<int> & m,const vector<int> & n)
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

vector<vector<int> > GroupUtil::Order(int N,const vector<int> & m)
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

int GroupUtil::IsInFG(int N,const vector<vector<int> > FG,const vector<int> & m)
{
	for(int i=0;i<FG.size();i++)
	{
		if(memcmp(&m[0],&FG[i][0],sizeof(int)*N)==0)
			return i;
	}
	return -1;
}

bool GroupUtil::FG(const char *szFG,const char *szfilename)
{
	vector<vector<int> > FG;
	vector<string> vM=split(szFG,";");
	int M=vM.size();
	for(int i=0;i<M;i++)
	{
		vector<string> vM1=split(vM[i],",");
		int N=vM1.size();
		vector<int> viM1(N);
		for(int i=0;i<N;i++)
		{
			viM1[i]=atoi(vM1[i].c_str());
		}
		FG.push_back(viM1);
	}
	int N=FG[0].size();
	vector<int> E;
	for(int i=0;i<N;i++)
	{
		E.push_back(i+1);
	}
	int bIn=IsInFG(N,FG,E);
	if(bIn==-1)
	{
	        FG.insert(FG.begin(),E);
	} 
	int R=FG.size();
	int cnt=R;
	int cnt1=R;
	do{
		cnt=FG.size();
		for(int i=0;i<cnt;i++)
		{
			for(int j=0;j<cnt;j++)
			{
				vector<int> IJ=Mul(N,FG[i],FG[j]);
				int bIn=IsInFG(N,FG,IJ);
				if(bIn==-1)
				{
					FG.push_back(IJ);
				}
			}
		}
		cnt1=FG.size();
	}while(cnt1>cnt);

	ofstream fout(szfilename);
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
	cout<<szfilename<<"生成完毕！"<<endl;

	return true;
}