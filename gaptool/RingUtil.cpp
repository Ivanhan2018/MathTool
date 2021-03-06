﻿//#include "stdafx.h"
#include "RingUtil.h"

std::vector<string> RingUtil::split( const std::string& str, const std::string& delims, unsigned int maxSplits /*= 0*/)
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

vector<vector<int> > RingUtil::Add(int N,int n,const vector<vector<int> > & a,const vector<vector<int> > & b)
{
	vector<vector<int> > vv(N);
	for(int i=0;i<N;i++)
	{
		vector<int> v(N);
		for(int j=0;j<N;j++)
		{
			int iElem=(a[i][j]+b[i][j])%n;
			v[j]=iElem;
		}
		vv[i]=v;
	}
	return vv;
}

vector<vector<int> > RingUtil::Zero(int N)
{
	vector<vector<int> > vv(N);
	for(int i=0;i<N;i++)
	{
		vector<int> v(N);
		for(int j=0;j<N;j++)
		{
			v[j]=0;
		}
		vv[i]=v;
	}
	return vv;
}

bool RingUtil::Brmul(int p,int *a,int *b,int m,int n,int k,int *c)
{ 
	if(p<=0)
		return false;

	for (int i=0; i<=m-1; i++)
		for (int j=0; j<=k-1; j++)
		{ 
			int u=i*k+j;
			c[u]=0;
			for(int l=0; l<=n-1; l++)
			{
				c[u]=c[u]+a[i*n+l]*b[l*k+j];
				c[u]=c[u]%p;
			}
		}
	return true;
}

vector<vector<int> > RingUtil::Mul(int N,int n,const vector<vector<int> > & a,const vector<vector<int> > & b)
{
	vector<vector<int> > vv(N);
	for(int i=0;i<N;i++)
	{
		vector<int> v(N);
		vv[i]=v;
	}
	for (int i=0; i<N; i++)
		for (int j=0; j<N; j++)
		{ 
			vv[i][j]=0;
			for(int l=0; l<N; l++)
			{
				vv[i][j]=vv[i][j]+a[i][l]*b[l][j];
				vv[i][j]=vv[i][j]%n;
			}
		}
	return vv;
}

bool RingUtil::IsEqual(int N,const vector<vector<int> > & m,const vector<vector<int> > & n)
{
	for(int i=0;i<N;i++)
		for(int j=0;j<N;j++)
		{
			if(m[i][j]!=n[i][j])
				return false;
		}

		return true;
}

int RingUtil::IsInFR(int N,const vector<vector<vector<int> > > & FR,const vector<vector<int> > & m)
{
	for(int i=0;i<FR.size();i++)
	{
		if(IsEqual(N,FR[i],m))
			return i;
	}
	return -1;
}

bool RingUtil::FR(const char *szFR,const char *szfilename)
{
	vector<string> vMDP=split(szFR,"|");
	int MDP=vMDP.size();
	if(MDP>1)
	{
		vector<string> vfn(MDP);
		for(int i=0;i<MDP;i++)
		{
			char szfn[100]={0};
			sprintf(szfn,"%s%d",szfilename,i);
			vfn[i]=szfn;
			bool bRet=FR(vMDP[i].c_str(),szfn);
			if(!bRet)
				return false;
		}
		char szcmd[100]={0};
		sprintf(szcmd,"./DPR %s %s %s",vfn[0].c_str(),vfn[1].c_str(),szfilename);
		system(szcmd);
		char szcmd1[100]={0};
		sprintf(szcmd1,"rm %s %s",vfn[0].c_str(),vfn[1].c_str());
		system(szcmd1);
		return true;
	}

        // 判断是不是有限域上多项式环的商环表示
        string s=szFR;
        size_t loc = s.find_first_of("x", 0 );
        if(loc != string::npos )
	{
	   vector<string> vArg=split(szFR,",");
	   char szcmd[100]={0};
	   sprintf(szcmd,"./GRpf %s %s %s",vArg[0].c_str(),vArg[1].c_str(),vArg[2].c_str());
	   system(szcmd);
           return true;
	}

	vector<vector<vector<int> > > FR;
	vector<string> vM=split(szFR,";");
	int M=vM.size();
	if(M<3)
		return false;
	int N=atoi(vM[0].c_str());
	int n=atoi(vM[1].c_str());

	vector<vector<int> > E=Zero(N);
	FR.push_back(E);

	for(int k=2;k<M;k++)
	{
		vector<string> vM1=split(vM[k],",");
		int N2=vM1.size();
		if(N*N!=N2)
			return false;
		vector<vector<int> > vv(N);
		for(int i=0;i<N;i++)
		{
			vector<int> v(N);
			for(int j=0;j<N;j++)
			{
				int iElem=atoi(vM1[i*N+j].c_str());
				v[j]=iElem;
			}
			vv[i]=v;
		}
		if(!IsEqual(N,E,vv))
		{
			FR.push_back(vv);
		}
	}
	int R=FR.size();
	int cnt=R;
	int cnt1=R;
	do{
		cnt=FR.size();
		for(int i=0;i<cnt;i++)
		{
			for(int j=0;j<cnt;j++)
			{
				vector<vector<int> > IJ=Mul(N,n,FR[i],FR[j]);
				int bIn=IsInFR(N,FR,IJ);
				if(bIn==-1)
				{
					FR.push_back(IJ);
				}
				vector<vector<int> > IJ1=Add(N,n,FR[i],FR[j]);
				int bIn1=IsInFR(N,FR,IJ1);
				if(bIn1==-1)
				{
					FR.push_back(IJ1);
				}
			}
		}
		cnt1=FR.size();
	}while(cnt1>cnt);

	ofstream fout(szfilename);
	int m=FR.size();
	cout<<"[R"<<m<<"Add]"<<endl;
	fout<<"[R"<<m<<"Add]"<<endl;
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<m;j++)
		{
			vector<vector<int> > IJ=Add(N,n,FR[i],FR[j]);
			int bIn=IsInFR(N,FR,IJ);
			cout<<bIn+1<<" ";
			fout<<bIn+1<<" ";
		}
		cout<<endl;
		fout<<endl;
	}
	cout<<"[R"<<m<<"Mul]"<<endl;
	fout<<"[R"<<m<<"Mul]"<<endl;
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<m;j++)
		{
			vector<vector<int> > IJ=Mul(N,n,FR[i],FR[j]);
			int bIn=IsInFR(N,FR,IJ);
			cout<<bIn+1<<" ";
			fout<<bIn+1<<" ";
		}
		cout<<endl;
		fout<<endl;
	}
	cout<<szfilename<<"生成完毕！"<<endl;

	return true;
}

vector<int> RingUtil::IsLegalMtx(const vector<vector<int> > &mtx)
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

bool RingUtil::SaveTable(const char *fn,const vector<vector<int> > &A,const vector<vector<int> > &B)
{
	vector<int> AFlag=IsLegalMtx(A);
	if(AFlag[0]==-1)
		return false;
	vector<int> BFlag=IsLegalMtx(B);
	if(BFlag[0]==-1)
		return false;
	FILE *fp;
	if((fp=fopen(fn,"wb+"))!=NULL)
	{
                // 生成加群凯莱表
                printf("[R%dAdd]\n",AFlag[1]);
                fprintf(fp,"[R%dAdd]\n",AFlag[1]);
		for(int i=0;i<AFlag[1];i++)
		{
			for(int j=0;j<AFlag[2];j++)
			{
				printf("%d ",A[i][j]);
				fprintf(fp,"%d ",A[i][j]);
			}
			printf("\r\n");
			fprintf(fp,"\r\n");
		}
                // 生成乘法凯莱表
                printf("[R%dMul]\n",BFlag[1]);
                fprintf(fp,"[R%dMul]\n",BFlag[1]);
		for(int i=0;i<BFlag[1];i++)
		{
			for(int j=0;j<BFlag[2];j++)
			{
				printf("%d ",B[i][j]);
				fprintf(fp,"%d ",B[i][j]);
			}
			printf("\r\n");
			fprintf(fp,"\r\n");
		}
		fclose(fp);
	}
	return true;
}

vector<vector<int> > RingUtil::Arr2ToVec2(int *R,int N)
{
	vector<vector<int> > vv(N);
	for(int i=0;i<N;i++)
	{
		vector<int> v(N);
		vv[i]=v;
	}
	for (int i=0; i<N; i++)
		for (int j=0; j<N; j++)
		{ 
			vv[i][j]=*(R+i*N+j)+1;
		}
	return vv;
}