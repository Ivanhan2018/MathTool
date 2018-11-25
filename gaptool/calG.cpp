#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include<algorithm>
using namespace std;

// 根据群的凯莱表分析其结构的小工具calG.exe
//#include <Windows.h>
//#pragma comment(lib,"user32.lib")

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


vector<char> lof(const char *fn)
{
	vector<char> ret;
	FILE *fp;
	int i=0;
	char ch=0;
	if((fp=fopen(fn, "rb"))!=NULL)
	{
		fseek(fp, 0, SEEK_SET); 
		while (ch!= EOF) 
		{
			i++; 
			ch = fgetc(fp);
			ret.push_back(ch);
		}; 
		fclose(fp);
	}
	if(ret.size()>0 && ret[ret.size()-1]==EOF)
		ret.pop_back();
	return ret; 
}

vector<char> CharArrToNormal(const vector<char>& vec)
{
	vector<char> ret;
	int n=vec.size();
	for(int i=0;i<n;i++)
	{
		if(vec[i]==32||vec[i]==13||vec[i]==10)
		{
			if(ret.size()>0 && ret[ret.size()-1]!=',')
				ret.push_back(',');
		}
		else
		{
			ret.push_back(vec[i]);
		}
	}
	return ret;
}

string CharArrToStr(const vector<char>& vec)
{
	string str;
	int n=vec.size();
	for(int i=0;i<n;i++)
	{
		str.push_back(vec[i]);
	}
	return str;
}

vector<vector<int> > atoTable(const char* strMtx)
{
	vector<vector<int> > vvMtx;
	vector<int> iAll;
	if(strMtx!=0)
	{
		vector<string> All=split(strMtx,",");
		for(int i=0;i<All.size();i++)
		{
			int iElem=atoi(All[i].c_str());
			iAll.push_back(iElem);
		}
	}
	int n=iAll.size();
	int n1=(int)sqrtf(n);
	if(n1*n1==n)
	{
		for(int i=0;i<n1;i++)
		{
			vector<int> iRow;
			for(int j=0;j<n1;j++)
			{
				int iElem=iAll[i*n1+j];
				iRow.push_back(iElem);
			}
			vvMtx.push_back(iRow);
		}
	}
	return vvMtx;
}

vector<int> IsLegalMtx(const vector<vector<int> > &mtx)
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


vector<vector<int> > ABmul(const vector<vector<int> > &A,const vector<vector<int> > &B)
{
	vector<vector<int> > C;
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

bool SaveTable(const char *fn,const vector<vector<int> > &A)
{
	vector<int> AFlag=IsLegalMtx(A);
	if(AFlag[0]==-1)
		return false;
	FILE *fp;
	if((fp=fopen(fn,"wb+"))!=NULL)
	{
		for(int i=0;i<AFlag[1];i++)
		{
			for(int j=0;j<AFlag[2];j++)
			{
				fprintf(fp,"%d ",A[i][j]);
			}
			fprintf(fp,"\r\n");
		}
		fclose(fp);
	}
	return true;
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
int getGnEOrder(const vector<vector<int> > &A,int a)
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

bool SaveGnEOrder(const char *srcfn,const char *Desfn,const char *DesGn=0)
{
	vector<char> A=lof(srcfn);
	string strA=CharArrToStr(CharArrToNormal(A));
	vector<vector<int> > vvA=atoTable(strA.c_str());

	vector<int> AFlag=IsLegalMtx(vvA);
	if(AFlag[0]==-1||AFlag[1]!=AFlag[2])
		return false;

	ofstream fout(Desfn);
	vector<int> vOrders=Factors(AFlag[1]);
	vector<int> vCounts(AFlag[1]+1);
	for(int i=0;i<AFlag[1];i++)
	{
		int ord=getGnEOrder(vvA,i);
		printf("G%dElementToOrder(%d)=%d\n",AFlag[1],i,ord);
		fout<<"G"<<AFlag[1]<<"ElementToOrder("<<i<<")="<<ord<<endl;
		vCounts[ord]=vCounts[ord]+1;
	}
	string strF;
	{
		char sz[200]={0};
		if(DesGn==0)
		{
			sprintf(sz,"G%d有",AFlag[1]);
                        fout<<"G"<<AFlag[1]<<"有"<<AFlag[1];
		}
		else
		{
			sprintf(sz,"%s有",DesGn);
                        fout<<DesGn<<"有";
		}
		strF=sz;
	}
	for(int i=0;i<vOrders.size();i++)
	{
		char sz[200]={0};
		sprintf(sz,"%d个%d阶元，",vCounts[vOrders[i]],vOrders[i]);
		strF+=sz;
                fout<<vCounts[vOrders[i]]<<"个"<<vOrders[i]<<"阶元";
                if(i<vOrders.size()-1)
                fout<<"，";
	}
	if(strF.size()>2)
	{
		strF=strF.substr(0,strF.size()-2);
	}
	printf("%s\n",strF.c_str());
	//fout<<strF.c_str()<<endl;// Linux下不能用sprintf格式化带中文的字符串??
	fout.close();
	return true;
}

// 从路径名中分离出文件名
char * find_file_name(char *name)
{
	char *name_start = NULL;
	int sep = '/';
	if (NULL == name) {
	    printf("the path name is NULL\n");
	    return NULL;
	}
	name_start = strrchr(name, sep);

	return (NULL == name_start)?name:(name_start + 1);
}

// 过滤掉文件名中的扩展名
void filter_ext_name(char *name)
{
	char *name_start = NULL;
	int sep = '.';
	if (NULL == name) {
	    printf("the path name is NULL\n");
	    return;
	}
	name_start = strrchr(name, sep);

        while(NULL != name_start && *name_start!='\0')
        {
            *name_start='\0';
            name_start++;
        }
}

int main(int argc, char **argv)
{
	char sz[100]={0};
	char sz1[100]={0};
	char sz2[100]={0};
	if(argc<2)
	{
		printf("请输入群A凯莱表文件名：");
		scanf("%s",sz);
	}
	else
		strcpy(sz,argv[1]);
	if(argc<3)
	{
#if defined(_WIN32)||defined(_WIN64)
		_splitpath(sz,0,0,sz1,0);
#else
                //sscanf(sz,"./%s.txt",sz1);
                char *fn=find_file_name(sz);
                strcpy(sz1,fn);
                printf("%s\n",sz1);
                filter_ext_name(sz1);
                printf("%s\n",sz1);
#endif
		string strDes=sz;
		string strFn=sz1;
		strFn+="_ElementToOrder";
		strcpy(sz2,sz1);
		strDes.replace(strDes.find(sz1,0),strlen(sz1),strFn.c_str());
		strcpy(sz1,strDes.c_str());
	}
	else
		strcpy(sz1,argv[2]);

	bool bret=SaveGnEOrder(sz,sz1,sz2);
	if(bret)
	{
		//MessageBoxA(NULL,"分析群的结构完毕！","提示",MB_OK);
                printf("分析群的结构完毕！\n");
	}

	//system("pause");
	return 0;
}