// 计算矩阵群的凯莱表的小工具FGMtx.exe
#include "GroupUtil.h"
#include<iostream>
#include<vector>
#include<string>
using namespace std;

template<class T>
void Brmul(T *a,T *b,int m,int n,int k,T *c)
{ 
	int i,j,l,u;
	for (i=0; i<=m-1; i++)
		for (j=0; j<=k-1; j++)
		{ 
			u=i*k+j; 
			c[u]=0.0;
			for(l=0; l<=n-1; l++)
				c[u]=c[u]+a[i*n+l]*b[l*k+j];
		}
		return;
}

int sqrti(int x)
{
	int ret=(int)(sqrtf(x));
	return ret;
}

int addInF4(int a,int b)
{
	if(a>=4||a<0)
		return -1;
	if(b>=4||b<0)
		return -1;
	static int g_Add[4][4]={
		{0,1,2,3},
		{1,0,3,2},
		{2,3,0,1},
		{3,2,1,0}
	};
	return g_Add[a][b];
}

int mulInF4(int a,int b)
{
	if(a>=4||a<0)
		return -1;
	if(b>=4||b<0)
		return -1;
	static int g_Mul[4][4]={
		{0,0,0,0},
		{0,1,2,3},
		{0,2,3,1},
		{0,3,1,2}
	};
	return g_Mul[a][b];
}

vector<int> Mul(int N,int n,vector<int> & a,vector<int> & b)
{
	vector<int> ij(N*N);
	if(n==4 && N==2)
	{
		ij[0]=addInF4(mulInF4(a[0],b[0]),mulInF4(a[1],b[2]));
		ij[1]=addInF4(mulInF4(a[0],b[1]),mulInF4(a[1],b[3]));
		ij[2]=addInF4(mulInF4(a[2],b[0]),mulInF4(a[3],b[2]));
		ij[3]=addInF4(mulInF4(a[2],b[1]),mulInF4(a[3],b[3]));
	}
	else
	{
		Brmul(&a[0],&b[0],N,N,N,&ij[0]);
		if(n>0)
		{
		    for(int m=0;m<N*N;m++)
		    {
			ij[m]=ij[m]%n;
		    }
		}
	}
        return ij;
}

vector<int> Identity(int N)
{
	vector<int> v(N*N);
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			v[i*N+j]=(i==j?1:0);
		}
	}
	return v;
}

bool IsEqual(const vector<int> & m,const vector<int> & n)
{
        int N2=m.size();
        if(N2!=n.size())
           return false;
  
	for(int i=0;i<N2;i++)
	{
	    if(m[i]!=n[i])
		return false;
	}

	return true;
}

int IsInFG(const vector<vector<int> >& FG,const vector<int> & m)
{
	for(int i=0;i<FG.size();i++)
	{
		if(IsEqual(FG[i],m))
			return i;
	}
	return -1;
}

vector<vector<int> > atoTable(vector<vector<int> >& ElemSet,int P=0)
{
	int m=ElemSet.size();//矩阵群的阶
	int N=sqrti(ElemSet[0].size());//N*N矩阵的阶
	vector<vector<int> > ret(m);
	for(int i=0;i<m;i++)
	{
		vector<int> I(m);
		for(int j=0;j<m;j++)
		{
			vector<int> ij=Mul(N,P,ElemSet[i],ElemSet[j]);
			int ij2=IsInFG(ElemSet,ij);
			I[j]=ij2+1;
		}
		ret[i]=I;
	}
	return ret;
}

// 根据矩阵群的生成元集合FG生成矩阵群
bool FGMtx(vector<vector<int> >& FG,int N,int n)
{
	vector<int> E=Identity(N);
	int bIn1=IsInFG(FG,E);
	if(bIn1==-1)
	{
           FG.insert(FG.begin(),E);
	}
	else if(bIn1>0)
	{
           FG.erase(FG.begin()+bIn1);
           FG.insert(FG.begin(),E);
	}
	int G=FG.size();
	int cnt=G;
	int cnt1=G;
	do{
		cnt=FG.size();
		for(int i=0;i<cnt;i++)
		{
			for(int j=0;j<cnt;j++)
			{
				vector<int> IJ=Mul(N,n,FG[i],FG[j]);
				int bIn=IsInFG(FG,IJ);
				if(bIn==-1)
				{
					FG.push_back(IJ);
				}
			}
		}
		cnt1=FG.size();
	}while(cnt1>cnt);

	return true;
}

int main(int argc, char **argv)
{
char sz[500]="SL;2;3; \
1,0,0,1; \
0,1,2,0; \
0,1,2,1; \
0,1,2,2; \
0,2,1,0; \
0,2,1,1; \
0,2,1,2; \
1,0,1,1; \
1,0,2,1; \
1,1,0,1; \
1,1,1,2; \
1,1,2,0; \
1,2,0,1; \
1,2,1,0; \
1,2,2,2; \
2,0,0,2; \
2,0,1,2; \
2,0,2,2; \
2,1,0,2; \
2,1,1,1; \
2,1,2,0; \
2,2,0,2; \
2,2,1,0; \
2,2,2,1 \
";
        //char sz1[500]="GL;2;3;0,1,2,0;0,1,2,1;2,2,0,1";
        //char sz1[500]="SL;2;3;0,1,2,0;0,1,2,1";//G24_3
        //char sz1[500]="O;2;3;0,1,2,0";//G8_3
        //char sz1[500]="SL;2;3;0,1,2,0";//SL(2,3)_4=G4_1
	char sz1[100]={0};
	if(argc<2)
	{
		printf("请输入群的矩阵表示：");
		scanf("%s",sz1);
	}
	else
		strcpy(sz1,argv[1]);

	vector<string> vM=GroupUtil::split(sz1,";");
	int M=vM.size();
	if(M<4)
	{
		printf("Error\n");	   
		return -1;
	}
	int N=atoi(vM[1].c_str());//N*N矩阵的阶
	int n=atoi(vM[2].c_str());//域F_n的阶
        vector<string> ElemSet(&vM[3],&vM[3]+(M-3));
	vector<vector<int> > vElemSet=GroupUtil::StrtoInt(ElemSet);
        bool bRet=FGMtx(vElemSet,N,n);
	vector<vector<int> > vvG=atoTable(vElemSet,n);
        char szOut[100]={0};
        if(argc>=3)
        {
           strcpy(szOut,argv[2]);
        }
        else 
        {
            sprintf(szOut,"%s(%d,%d)_%d",vM[0].c_str(),N,n,vvG.size());
        }
        string strOutFile=szOut;
        strOutFile+=".txt";
	bool bret=GroupUtil::SaveTable(strOutFile.c_str(),vvG);
	if(bret)
	{
		printf("计算矩阵群%s的凯莱表完毕！\n",szOut);
	}

	//system("pause");
	return 0;
}
