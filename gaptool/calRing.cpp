#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 根据环的加法、乘法凯莱表分析其结构的小工具calR.exe

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

vector<char> lof2(const char *fn)
{
	vector<char> ret;
	FILE *fp;
	int i=0;
	char ch=0;
        int flag=0;//0:有效数据,1:注释开始,2:注释结束
	if((fp=fopen(fn, "rb"))!=NULL)
	{
		fseek(fp, 0, SEEK_SET); 
		while (ch!= EOF) 
		{
	           ch = fgetc(fp);
		   if (ch!='[' && ch!=']' && (flag==0||flag==2)) 
		   {
			i++; 
			ret.push_back(ch);
                        flag=0;
		   } 
                   else
                   {
                        if(ch=='[')
                        {
                          flag=1;
                        }
                        else if(ch==']')
                        {
                          flag=2;
                        }  
                   }                      
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

vector<vector<int> > atoTable(const char* strMtx,int k=1,int idx=0)
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
	int n=iAll.size()/k;
	int n1=(int)sqrtf(n);
	int r=iAll.size()%k;
        //printf("iAll.size()=%d,n=%d,n1=%d,r=%d\n",iAll.size(),n,n1,r);
	if(n1*n1==n)
	{
		for(int i=0;i<n1;i++)
		{
			vector<int> iRow;
			for(int j=0;j<n1;j++)
			{
				int iElem=iAll[i*n1+j+idx*n];
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

// 是否交换
bool IsAbelian(const vector<vector<int> > &A)
{
	vector<int> AFlag=IsLegalMtx(A);
	if(AFlag[0]==-1)
		return false;
	int n=AFlag[1];
	for (int i=0; i<n; i++)for (int j=0; j<n; j++){if(A[i][j]!=A[j][i])return false;}
	return true;
}

// 是否为幂等元
bool IsIdempotent(const vector<vector<int> > &A,int i2){
	vector<int> AFlag=IsLegalMtx(A);
	if(AFlag[0]==-1)
		return false;
	int n=AFlag[1];
	if(i2<0||i2>=n)
		return false;
	if(A[i2][i2]==i2+1)
		return true;
	return false;
}

// 是否为幺元
bool IsOne(const vector<vector<int> > &A,int i3)
{
	vector<int> AFlag=IsLegalMtx(A);
	if(AFlag[0]==-1)
		return false;
	int n=AFlag[1];
	if(i3<0||i3>=n)
		return false;
	for (int i=0; i<n; i++)
	{
		if(A[i3][i]!=i+1 || A[i][i3]!=i+1)
			return false;
	}
	return true;
}

// 是否有幺元,-1表示没有
int One(const vector<vector<int> > &A)
{
	vector<int> AFlag=IsLegalMtx(A);
	if(AFlag[0]==-1)
		return false;
	int n=AFlag[1];
	for (int i3=0; i3<n; i3++){if(IsOne(A,i3))return i3+1;}
	return -1;
}

// 是否有可逆元,-1表示没有
int Inv(const vector<vector<int> > &A,int i1,int iOne)
{
	vector<int> AFlag=IsLegalMtx(A);
	if(AFlag[0]==-1)
		return false;
	int n=AFlag[1];
	for (int i=0; i<n; i++){if(A[i1][i]==iOne && A[i][i1]==iOne)return i+1;}
	return -1;
}

// 不可逆元个数n1
int NoInvNum(const vector<vector<int> > &A)
{
	vector<int> AFlag=IsLegalMtx(A);
	if(AFlag[0]==-1)
		return -1;
	int n=AFlag[1];
	int iRet=0;
        int iOne=One(A);
        if(iOne==-1)
           return n;
	for(int i1=0;i1<n;i1++){if(Inv(A,i1,iOne)==-1)iRet++;}
	return iRet;
}

// 幂等元个数n2
int IdempotentNum(const vector<vector<int> > &A)
{
	vector<int> AFlag=IsLegalMtx(A);
	if(AFlag[0]==-1)
		return -1;
	int n=AFlag[1];
	int iRet=0;
	for(int i2=0;i2<n;i2++){if(IsIdempotent(A,i2))iRet++;}
	return iRet;
}

// 2次幂零元个数n4
int Nil2Num(const vector<vector<int> > &A)
{
	vector<int> AFlag=IsLegalMtx(A);
	if(AFlag[0]==-1)
		return -1;
	int n=AFlag[1];
	int iRet=0;
	for(int i4=1;i4<n;i4++){if(A[i4][i4]==1)iRet++;}
	return iRet;
}

// 2~3次幂零元个数n5
int Nil3Num(const vector<vector<int> > &A)
{
	vector<int> AFlag=IsLegalMtx(A);
	if(AFlag[0]==-1)
		return -1;
	int n=AFlag[1];
	int iRet=0;
	for(int i5=1;i5<n;i5++){int j=A[i5][i5]-1;if(A[j][i5]==1)iRet++;}
	return iRet;
}

// 零乘个数n6
int ZeroNum(const vector<vector<int> > &A)
{
	vector<int> AFlag=IsLegalMtx(A);
	if(AFlag[0]==-1)
		return -1;
	int n=AFlag[1];
	int iRet=0;
	for(int i=0;i<n;i++)for(int j=0;j<n;j++){if(A[i][j]==1)iRet++;}
	return iRet;
}

// 零因子个数n7
int ZeroFactorNum(const vector<vector<int> > &A)
{
	vector<int> AFlag=IsLegalMtx(A);
	if(AFlag[0]==-1)
		return -1;
	int n=AFlag[1];
	int iRet=0;
	for(int i=1;i<n;i++)
	{
		for(int j=1;j<n;j++)
		{
			if(A[i][j]==1)
			{
			    iRet++;
			    break;
			}
		}
	}
	return iRet;
}

bool IsInCenterOfG(const vector<vector<int> > &vvG,int j)
{
	int N=vvG.size();
	for(int i=0;i<N;i++)
	{
		int ij=vvG[i][j]-1;
		int ji=vvG[j][i]-1;
		if(ij==ji)
			continue;
		else
			return false;
	}
	return true;
}

// 定义：一个环R可以同每一个元交换的元作成一个子环，叫作环R的中心。
vector<int> CenterOfG(const vector<vector<int> > &vvG)
{
	vector<int> ret;
	int N=vvG.size();
	for(int i=0;i<N;i++)
	{
		if(IsInCenterOfG(vvG,i))
			ret.push_back(i);
		else
			continue;
	}
	return ret;
}

int ToNormal(const vector<int> &v,int a)
{
	for(int i=0;i<v.size();i++)
	{
		if(v[i]==a)
			return i;
	}
	return -1;
}

// N次置换
bool IsPerN(vector<int> A,int N)
{
	std::sort(A.begin(),A.end());//升序
	for(int i=0;i<N;i++)
		if(A[i]!=i+1)
			return false;
	return true;
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

// S_N中置换m的逆
vector<int> Inv(int N,const vector<int> & m)
{
	vector<vector<int> >vv=Order(N,m);
	int ord=vv.size();
	return vv[ord-1];
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

int IsInFR(const vector<int> &v,int m){
	for(int i=0;i<v.size();i++){
		if(v[i]==m){
			return i;
		}
	}
	return -1;
}

vector<int> FR(const vector<vector<int> > &vvA,const vector<vector<int> > &vvM,const vector<int> &v)
{
	vector<int> ret=v;	
	int cnt=0,cnt1=v.size();	
    while(cnt1!=cnt) {
	    cnt=ret.size();
		for(int i=0;i<cnt;i++){
			for(int j=0;j<cnt;j++){
			int IJ=vvA[ret[i]-1][ret[j]-1];
			int bIn=IsInFR(ret,IJ);
			if(bIn==-1){
			 ret.push_back(IJ);
			}
			int IJ1=vvM[ret[i]-1][ret[j]-1];			
			int bIn1=IsInFR(ret,IJ1);
			if(bIn1==-1){
			 ret.push_back(IJ1);
			}			
		}
		}
		cnt1=ret.size();
    }
	std::sort(ret.begin(),ret.end());
    return ret;
}

// 二维数组是群的凯莱表的充要条件
bool IsGroup(const vector<vector<int> > &vvA)
{
	vector<int> AFlag=IsLegalMtx(vvA);
	if(AFlag[0]==-1||AFlag[1]!=AFlag[2])
		return false;
	int N=AFlag[1];
	for(int i=0;i<AFlag[1];i++)
	{
		bool isPerN=IsPerN(vvA[i],AFlag[1]);
		if(!isPerN)
		 return false;
	}
	//E是乘法单位元
	vector<int> E;
	for(int i=0;i<N;i++)
	{
		E.push_back(i+1);
	}
	int bIn=IsInFG(N,vvA,E);
	if(bIn==-1)
	{
	   return false;
	} 
	//乘法封闭性
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			vector<int> IJ=Mul(N,vvA[i],vvA[j]);
			int bIn=IsInFG(N,vvA,IJ);
			if(bIn==-1)
			{
				return false;
			}
		}
	}
	//有乘法逆元
	for(int i=0;i<N;i++)
	{
		vector<int> inv=Inv(N,vvA[i]);
		int bIn=IsInFG(N,vvA,inv);
		if(bIn==-1)
		{
			return false;
		}
	}
	return true;
}

//0是Abel群
//1是群但不是Abel群
//2不是群
int IsAbelianGroup(const vector<vector<int> > &vvA)
{
 if(!IsGroup(vvA))
  return 2;
 int N=vvA.size();
 //乘法交换性
 for(int i=0;i<N;i++)
 {
  for(int j=i+1;j<N;j++)
  {
   vector<int> ij=Mul(N,vvA[i],vvA[j]);
   vector<int> ji=Mul(N,vvA[j],vvA[i]);
   if(memcmp(&ij[0],&ji[0],sizeof(int)*N)==0)
    continue;
   else
    return 1;
  }
 }
 return 0;
}

// 二维数组(A,M)是环的凯莱表的充要条件
bool IsRing(const vector<vector<int> > &vvA,const vector<vector<int> > &vvM)
{
	//Ⅰ加法的规律：<R,+>是交换群。
	if(IsAbelianGroup(vvA)!=0)
		return false;
	int N=vvA.size();	
	//零元
	for(int i=0;i<N;i++)
	{
		if(vvM[0][i]!=1||(vvM[i][0]!=1))
			return false;
	}
	//Ⅱ乘法满足结合律：<R ,·>是半群。
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			for(int k=0;k<N;k++)
			{
				int ij=vvM[i][j]-1;
				int ij_k=vvM[ij][k]-1;
				int jk=vvM[j][k]-1;
				int i_jk=vvM[i][jk]-1;
				if(ij_k!=i_jk)
					return false;
			}
		}
	}
	//Ⅲ分配律：乘运算对加运算可分配，即a·(b+c)=ab+ac,(a+b)·c=ac+bc。
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			for(int k=0;k<N;k++)
			{
				int JK=vvA[j][k]-1;
				int I_JK=vvM[i][JK]-1;
				int IJ=vvM[i][j]-1;
				int IK=vvM[i][k]-1;
				int IJ_IK=vvA[IJ][IK]-1;
				if(I_JK!=IJ_IK)
					return false;
				int ij=vvA[i][j]-1;
				int ij_k=vvM[ij][k]-1;
				int ik=vvM[i][k]-1;
				int jk=vvM[j][k]-1;
				int ik_jk=vvA[ik][jk]-1;
				if(ij_k!=ik_jk)
					return false;
			}
		}
	}
	return true;
}

//0是交换环
//1是非交换环
//2不是环
int IsAbelianRing(const vector<vector<int> > &vvA,const vector<vector<int> > &vvM)
{
 if(!IsRing(vvA,vvM))
  return 2;
 //乘法交换性
 bool bRet=IsAbelian(vvM);
 return bRet?0:1;
}

//0是幺环
//1是无幺环
//2不是环
int IsUnitaryRing(const vector<vector<int> > &vvA,const vector<vector<int> > &vvM)
{
 if(!IsRing(vvA,vvM))
  return 2;
 int bO=One(vvM);
 return bO>-1?0:1;
}

//0是域(无零因子交换幺环)
//1是有零因子交换幺环
//2是有零因子交换无幺环
//3是有零因子非交换幺环
//4是有零因子非交换无幺环，(不存在有限无零因子无幺环)
//-1不是环
int IsField(const vector<vector<int> > &vvA,const vector<vector<int> > &vvM)
{
 int iA=IsAbelianRing(vvA,vvM);
 if(iA==2)
	 return -1;
 int iO=IsUnitaryRing(vvA,vvM);
 int n1=NoInvNum(vvM);
 if(iA==0 && iO==0)
 {
     if(n1==1)
		 return 0;
	 return 1;
 }
 if(iA==0 && iO==1)
 {
	 return 2;
 }
 if(iA==1 && iO==0)
 {
	 return 3;
 }
 return 4;
}

const char *IsFieldRetInfo(int iRet)
{
	const char *szInfo[]={"0域","1有零因子交换幺环","2有零因子交换无幺环","3有零因子非交换幺环","4有零因子非交换无幺环"};
	if(iRet==-1)
		return "-1不是环";
	return szInfo[iRet];
}

//0是布尔环
//1是环但不是布尔环
//2不是环
int IsBooleanRing(const vector<vector<int> > &vvA,const vector<vector<int> > &vvM)
{
 if(!IsRing(vvA,vvM))
  return 2;
 //是否布尔环b2
 int n2=IdempotentNum(vvM);
 return n2==vvA.size()?0:1;
}

// 环的结构不变量bA,bO,n1,n2,n4,n5,n6,n7,n8
std::string calRInfo(const vector<vector<int> > &vvA1)
{
    // 分析乘法结构
	bool bA=IsAbelian(vvA1);
	int bO=One(vvA1)>-1;
    int n1=NoInvNum(vvA1);
	int n2=IdempotentNum(vvA1);
	int n4=Nil2Num(vvA1);
	int n5=Nil3Num(vvA1);
	int n6=ZeroNum(vvA1);
	int n7=ZeroFactorNum(vvA1);

    vector<int> ZA=CenterOfG(vvA1);
	int n8=ZA.size();
	
	char szInfo[100]={0};
	sprintf(szInfo,"%d,%d,%d,%d,%d,%d,%d,%d,%d",bA,bO,n1,n2,n4,n5,n6,n7,n8);
	return szInfo;
}

string itos(int a)
{
	char sz[20]={0};
	sprintf(sz,"%d",a);
	return sz;
}

string VStr(const vector<int> &v)
{
	string str="[";
    for(int i=0;i<v.size();i++){
		str+=itos(v[i]);
		if(i<v.size()-1)
          	str+=",";		
    }	
	str+="]";
	return str;
}

bool SaveGnEOrder(const char *srcfn,const char *Desfn,const char *DesGn=0)
{
	vector<char> A=lof2(srcfn);
	string strA=CharArrToStr(CharArrToNormal(A));
	vector<vector<int> > vvA=atoTable(strA.c_str(),2,0);
	vector<vector<int> > vvA1=atoTable(strA.c_str(),2,1);

	vector<int> AFlag=IsLegalMtx(vvA);
	if(AFlag[0]==-1||AFlag[1]!=AFlag[2])
		return false;

	ofstream fout(Desfn);

    //20200403新增代码开始
	int iRet=IsField(vvA,vvA1);
	const char *szInfo=IsFieldRetInfo(iRet);
	char sz1[20]={0};
	int iRet1=IsBooleanRing(vvA,vvA1);
	if(iRet1==0)
		strcpy(sz1,",布尔环");
	string str;
	if(iRet>-1)
	{
		str=calRInfo(vvA1).insert(0,",");
	}
	printf("R%d：%s%s%s\n",AFlag[1],szInfo,sz1,str.c_str());
	fout<<"R"<<AFlag[1]<<":"<<szInfo<<sz1<<str<<endl;
    //20200403新增代码结束	
	
	vector<int> vOrders=Factors(AFlag[1]);
	vector<int> vCounts(AFlag[1]+1);
	vector<int> vCounts1(AFlag[1]+1);	
	for(int i=0;i<AFlag[1];i++)
	{
		int ord=getGnEOrder(vvA,i);
		printf("G%dElementToOrder(%d)=%d\n",AFlag[1],i,ord);
		fout<<"G"<<AFlag[1]<<"ElementToOrder("<<i<<")="<<ord<<endl;
		vCounts[ord]=vCounts[ord]+1;
		
		vector<int> v(1,i+1);
		vector<int> vi=FR(vvA,vvA1,v);
		int ord1=vi.size();
		vCounts1[ord1]=vCounts1[ord1]+1;	
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
        fout<<endl;

	vector<int> v0;
	vector<int> v1;
	for(int i=0;i<vOrders.size();i++)
	{
		v0.push_back(vCounts[vOrders[i]]);
		v1.push_back(vCounts1[vOrders[i]]);
	}
	string N0=VStr(v0);//加法群同阶元的分布N0
	string S1=VStr(v1);//单个生成元子环的阶的分布S1
    int n0=0;
	for(int i=vOrders.size()-1;i>=0;i--)
	{
		if(vCounts[vOrders[i]]>0)
                {
                   n0=vOrders[i];
                   break;
                }
	}

        // 分析乘法结构
	bool bA=IsAbelian(vvA1);
	int bO=One(vvA1)>-1;
        int n1=NoInvNum(vvA1);
	int n2=IdempotentNum(vvA1);
	int n4=Nil2Num(vvA1);
	int n5=Nil3Num(vvA1);
	int n6=ZeroNum(vvA1);
	int n7=ZeroFactorNum(vvA1);

        vector<int> ZA=CenterOfG(vvA1);
	int n8=ZA.size();
	
	printf("环的结构不变量N0,n0,bA,bO,n1,n2,n4,n5,n6,n7,n8,S1=%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",N0.c_str(),n0,bA,bO,n1,n2,n4,n5,n6,n7,n8,S1.c_str());
        fout<<"环的结构不变量N0,n0,bA,bO,n1,n2,n4,n5,n6,n7,n8,S1="<<N0<<n0<<","<<bA<<","<<bO<<","<<n1<<","<<n2<<","<<n4<<","<<n5<<","<<n6<<","<<n7<<","<<n8<<S1<<endl;

        const char *szRet=(bA?"交换":"非交换");
	printf("%s,",szRet);
        fout<<szRet<<",";
        const char *szRet3=(bO?"幺环":"无幺环");
	printf("%s,",szRet3);
        fout<<szRet3<<",";
	printf("不可逆元个数n1=%d,",n1);
        fout<<"不可逆元个数n1="<<n1<<",";
	printf("幂等元个数n2=%d,",n2);
        fout<<"幂等元个数n2="<<n2<<",";
	printf("2次幂零元个数n4=%d,",n4);
        fout<<"2次幂零元个数n4="<<n4<<",";
	printf("2~3次幂零元个数n5=%d,",n5);
        fout<<"2~3次幂零元个数n5="<<n5<<",";
	printf("零乘个数n6=%d,",n6);
        fout<<"零乘个数n6="<<n6<<",";
	printf("零因子个数n7=%d,",n7);
        fout<<"零因子个数n7="<<n7<<",";
	printf("中心大小n8=%d,",n8);
        fout<<"中心大小n8="<<n8<<",";

	printf("\n");
        fout<<endl;

	for(int i=0;i<ZA.size();i++)
	{
		printf("%d ",ZA[i]);
		fout<<ZA[i]<<endl;
	}
	printf("\n");
	bool bIsNormal[]={false,true};
	const char * szArr[2]={"子环形式的中心凯莱表：","规范形式的中心凯莱表："};
	// 环的中心是非平凡子环才打印运算表
	if(n8>1 && n8<AFlag[1])
	{
		for(int k=0;k<2;k++)
		{
			printf("%s\n",szArr[k]);
			fout<<szArr[k]<<endl;
			printf("[R%dAdd]\n",n8);
			fout<<"[R"<<n8<<"Add]"<<endl;
			for(int i=0;i<ZA.size();i++)
			{
				for(int j=0;j<ZA.size();j++)
				{
					if(bIsNormal[k])
					{
						int ij=vvA[ZA[i]][ZA[j]]-1;
						int ij2=ToNormal(ZA,ij)+1;
						printf("%d ",ij2);
						fout<<ij2<<" ";
					}
					else
					{
						int ij=vvA[ZA[i]][ZA[j]];
						printf("%d ",ij);
						fout<<ij<<" ";
					}
				}
				printf("\n");
				fout<<endl;
			}
			printf("[R%dMul]\n",n8);
			fout<<"[R"<<n8<<"Mul]"<<endl;
			for(int i=0;i<ZA.size();i++)
			{
				for(int j=0;j<ZA.size();j++)
				{
					if(bIsNormal[k])
					{
						int ij=vvA1[ZA[i]][ZA[j]]-1;
						int ij2=ToNormal(ZA,ij)+1;
						printf("%d ",ij2);
						fout<<ij2<<" ";
					}
					else
					{
						int ij=vvA1[ZA[i]][ZA[j]];
						printf("%d ",ij);
						fout<<ij<<" ";
					}
				}
				printf("\n");
				fout<<endl;
			}
		}
	}

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
		printf("请输入环A加法、乘法凯莱表文件名：");
		scanf("%s",sz);
	}
	else
		strcpy(sz,argv[1]);
	if(argc<3)
	{
#if defined(_WIN32)||defined(_WIN64)
		_splitpath(sz,0,0,sz1,0);
#else
                char *fn=find_file_name(sz);
                strcpy(sz1,fn);
                //printf("%s\n",sz1);
                filter_ext_name(sz1);
                //printf("%s\n",sz1);
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
                printf("分析环的结构完毕！\n");
	}

	//system("pause");
	return 0;
}
