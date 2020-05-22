#include<iostream>
#include<vector>
#include<string>
using namespace std;

std::vector<string> split( const std::string& str, const std::string& delims = "\t\n ", unsigned int maxSplits = 0)
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

// S_N÷–÷√ªª≥À∑®m*n
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

vector<vector<int> > Order(const vector<int> & m)
{
	vector<vector<int> > ret;
    int n=m.size();
	vector<int> mi=m;
	vector<int> m0(n);
	for(int i=0;i<n;i++)
	{
		m0[i]=i+1;
	}
	ret.push_back(m0);
	while(memcmp(&mi[0],&m0[0],sizeof(int)*n)!=0)
	{
		ret.push_back(mi);
		mi=Mul(n,mi,m);
	}
	return ret;
}

vector<int> inv(const vector<int> &t)
{
	vector<vector<int> > S1=Order(t);
	int ord=S1.size();	
	return S1[ord-1];	
}

vector<int> split2(const string& str)
{
	vector<string> m=split(str,",");
	vector<int> ret(m.size());
	for(int i=0;i<m.size();i++)
	{
		ret[i]=atoi(m[i].c_str());
	}
	return ret;
}

int main(int argc, char **argv)
{
	char sz[100]={0};
	char sz1[10240]={0};
	if(argc<2)
	{
		printf("«Î ‰»Î÷√ªª¥Œ ˝N£∫");
		scanf("%s",sz);
	}
	else
		strcpy(sz,argv[1]);
	if(argc<3)
	{
		printf("«Î ‰»Î÷√ªªm£∫");
		scanf("%s",sz1);
	}
	else
		strcpy(sz1,argv[2]);

	int N=atoi(sz);
	vector<int> m=split2(sz1);
	vector<int> m1=inv(m);
	printf("÷√ªªm^-1£∫");
	for(int i=0;i<m1.size();i++)
	{
		printf("%d",m1[i]);
		if(i<m1.size()-1)
			printf(",");
	}
	printf("\n");

	system("pause");
	return 0;
}

