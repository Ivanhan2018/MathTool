#include <cstdio>
#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
using namespace std;

int Mod(int ret,unsigned int n)
{
	//assert(n>0);
	if(ret<0)
	{
		int ret1=ret+(-ret+1)*n;
		return ret1%n;
	}
	return ret%n;
}

// 多项式a乘以多项式b得到多项式c，deg(a)=m-1,deg(b)=n-1,deg(c)=m+n-2=k
int polymul(int a[],int m,int b[],int n,int c[],int *k) 
{ 
 int i,j; 
 if(k)
  *k=m+n-2;
 for(i=0;i<m+n-2;i++) 
  c[i]=0; 
 for(i=0;i<m;i++) 
  for(j=0;j<n;j++) 
   c[i+j]=a[i]*b[j]+c[i+j];
 return 0;
} 

// 清除高位无效零、补零
void doPoly(vector<int>& c)
{
	while(c.size() && *(c.rbegin())==0)
	{
		c.pop_back();
	}
	if(c.empty())
	{
		c.push_back(0);
	}	
}

vector<int> polyadd(vector<int>& a,vector<int>& b,int m)
{
	int na=a.size();
	int nb=b.size();
	int k=(na>nb?na:nb);
	int ki=(na>nb?nb:na);	
	vector<int> c(k);
	for(int i=0;i<ki;i++) 
	  c[i]=Mod(a[i]+b[i],m); 
	if(na>nb)
	{
      memcpy(&c[nb],&a[nb],(na-nb)*sizeof(int));
	}
	else if(na<nb)
	{
      memcpy(&c[na],&b[na],(nb-na)*sizeof(int));
	}
    doPoly(c);
	return c;
}

vector<int> polymul(vector<int>& a,vector<int>& b,int m)
{
	int na=a.size();
	int nb=b.size();
	vector<int> c(na+nb-1);
	int k=0;
	int ret=polymul(&a[0],na,&b[0],nb,&c[0],&k);
	for_each(c.begin(),c.end(),[&](int& i)->void{i=Mod(i,m);});
	doPoly(c);
	return c;
}

vector<int> polymod(vector<int>& a,vector<int>& b,int m)
{
    vector<int> A=a,B=b;
	int coe;
	int na=A.size();
	int nb=B.size();
	if(na<nb)
		return A;
	else
	{
		for(int i=0;i<na-nb+1;i++)
		{
			for(int j=A[na-1-i];;) 
			{
				if(!(j%B[nb-1]))
				{
					coe=j/B[nb-1];
					break;
				}
				j+=2;
			} 
			for(int k=0;k<nb;k++) 
			{
				A[na-1-i-k]=Mod(A[na-1-i-k]-coe*B[nb-1-k],m);
			} 			
		}					
	}
	doPoly(A);
	return A;
}

//将一个数字转换成字符串
string NumberTostring(long Num)   
{
	char buffer[100]={0};
	sprintf(buffer,"%d",Num);
	string strBuf=buffer;

	return strBuf;
}

string sPoly(int *a,int n)
{
	string str="[";
	for(int i=0;i<n;i++)
	{
		str+=NumberTostring(a[i]);
		if(i<n-1)
		{
			str+=",";
		}
	}
	str+="]";
	return str;
}

int getIdx(vector<vector<int> >& Set,vector<int>& a)
{
	int n=Set.size();
	int na=a.size();
	for(int i=0;i<n;i++)
	{
        int ni=Set[i].size();
		if(ni!=na)
			continue;
		bool bEqual=true;
		for(int j=0;j<na;j++)
		{
		  if(Set[i][j]!=a[j])
		  {
			bEqual=false;
			break;
		  }
		}
		if(bEqual)
		{
			return i;
		}
	}		
	return -1;
}

int powi(int a,int n)
{
	int ret=1;
	for(int i=0;i<n;i++)
		ret*=a;
	return ret;
}

void printSet(vector<vector<int> >& Set)
{
    int n=Set.size();
	for(int i=0;i<n;i++)
	{
        vector<int> vi=Set[i];
		string si=sPoly(&vi[0],vi.size());
		cout<<i<<"->"<<si<<endl;
	}
}

vector<vector<int> > getSet(int d,int n){
	int q=powi(n,d);
	vector<vector<int> > ret;
	for(int i=0;i<q;i++)
	{
		vector<int> v;
		int num=i;
		while(num)
		{
			v.push_back(num%n);
			num/=n;
		}
		int r=d-v.size();
		for(int i=0;i<r;i++)
		{
			v.push_back(0);
		}
		doPoly(v);
        ret.push_back(v);		
	}
	return ret;
}

int PolygonVal(vector<int>& m,int x)
{
    int ret=0;
	int n=m.size();
	for(int i=0;i<n;i++)
	{
		ret+=m[i]*pow(x,i);
	}	
	return ret;
}

vector<int> Findroot(vector<int>& m,int n)
{
	vector<int> ret;
	for(int i=0;i<n;i++)
	{
		if(Mod(PolygonVal(m,i),n)==0)
			ret.push_back(i);
	}
	return ret;
}

vector<vector<int> > getIrreduciblePolynomialsSet(int d,int n,bool Irr){
	vector<vector<int> > ret0=getSet(d+1,n);
	vector<vector<int> > ret;	
	int n0=ret0.size();
	for(int i=0;i<n0;i++)
	{
		if(ret0[i].size()==d+1 && ret0[i][ret0[i].size()-1]==1)// 只考虑最高项系数为1的d次多项式
		{
            vector<int> v=Findroot(ret0[i],n);
			if((v.size()==0 && Irr)||(v.size()>0 && !Irr))
			{				
				ret.push_back(ret0[i]);
			}
		}
	}
	return ret;
}

// 有限环Z/(n)[x]/(m)中的多项式集合
vector<vector<int> > getSet(vector<int>& m,int n){
	int d=m.size()-1;//多项式m的次数
	vector<vector<int> > ret=getSet(d,n);
	return ret;
}

// 有限环Z/(n)[x]/(a,m)中的多项式集合
vector<vector<int> > getFRSet(vector<int>& a,vector<int>& m,int n){
	vector<vector<int> > S;
	vector<int> I;
	I.push_back(0);
	S.push_back(I);
	vector<int> am=polymod(a,m,n);
	int ia=getIdx(S,am);
	if(ia==-1)
		S.push_back(am);
	int cnt=0;
	int cnt1=S.size();
	while(cnt1>cnt)
	{
		cnt=S.size();
		for(int i=0;i<cnt;i++)
			for(int j=0;j<cnt;j++)
			{
				vector<int> ij=polymul(S[i],S[j],n);
				vector<int> ij1=polymod(ij,m,n);			
				int IJ=getIdx(S,ij1);
                if(IJ==-1)
                   	S.push_back(ij1);
				vector<int> ij2=polyadd(S[i],S[j],n);			
				int IJ2=getIdx(S,ij2);
                if(IJ2==-1)
                   	S.push_back(ij2);                				
			}
		cnt1=S.size();
	}
	return S;
}

// 加法运算表
vector<vector<int> > AddTable(vector<vector<int> >& Set,int n){
	int N=Set.size();
	vector<vector<int> > ret(N,vector<int>(N,0));
	for(int i=0;i<N;i++)
		for(int j=0;j<N;j++)
		{
			vector<int> ij=polyadd(Set[i],Set[j],n);
			int IJ=getIdx(Set,ij);
			ret[i][j]=IJ+1;
		}		
	return ret;
}

// 乘法运算表
vector<vector<int> > MulTable(vector<vector<int> >& Set,vector<int>& m,int n){
	int N=Set.size();
	vector<vector<int> > ret(N,vector<int>(N,0));
	for(int i=0;i<N;i++)
		for(int j=0;j<N;j++)
		{
			vector<int> ij=polymul(Set[i],Set[j],n);
			vector<int> ij1=polymod(ij,m,n);			
			int IJ=getIdx(Set,ij1);
			ret[i][j]=IJ+1;
		}		
	return ret;
}

void PrintRingTable(vector<vector<int> >& A,vector<vector<int> >& M){
    int n=A.size();
	printf("[R%dAdd]\n",n);
	for(int i=0;i<n;i++){
	    for(int j=0;j<n;j++){
			printf("%d ",A[i][j]);
		}
		printf("\n");
	}	
    n=M.size();	
	printf("[R%dMul]\n",n);
	for(int i=0;i<n;i++){
	    for(int j=0;j<n;j++){
			printf("%d ",M[i][j]);
		}
		printf("\n");
	}
}



int main(int argc, char **argv)
{
/* 	int a[3]={0,1,2};//0x^0+1x^1+2x^2
	int b[2]={3,1};//3x^0+1x^1
	const int m=sizeof(a)/sizeof(int);
	const int n=sizeof(b)/sizeof(int);
	int k=0;
	int c[m+n-1]={0};//0x^0+3x^1+7x^2+2x^3
	int ret=polymul(a,m,b,n,c,&k);
	cout<<sPoly(a,m)<<"*"<<sPoly(b,n)<<"="<<sPoly(c,m+n-1)<<endl;  */
	{
		int prim_poly[] = {1,1,0,1};
		int d1=sizeof(prim_poly)/sizeof(prim_poly[0]);
		vector<int> f1(prim_poly,prim_poly+d1);
		vector<vector<int> > vPoly=getSet(f1,2);
		cout<<sPoly(prim_poly,d1)<<"在Z/(2)上无根，GF(2,3)=Z/(2)/(x^3+x+1):"<<endl;	
		printSet(vPoly);	
		vector<vector<int> > A=AddTable(vPoly,2);
		vector<vector<int> > M=MulTable(vPoly,f1,2);
		PrintRingTable(A,M);
		cout<<endl;
	}
	{
		int prim_poly[] = {1,1,1};
		int d1=sizeof(prim_poly)/sizeof(prim_poly[0]);
		vector<int> f1(prim_poly,prim_poly+d1);
		vector<vector<int> > vPoly=getSet(f1,2);
		cout<<sPoly(prim_poly,d1)<<"在Z/(2)上无根，GF(2,2)=Z[ω]/(2)=Z/(2)[x]/(x^2+x+1):"<<endl;	
		printSet(vPoly);
		vector<vector<int> > A=AddTable(vPoly,2);
		vector<vector<int> > M=MulTable(vPoly,f1,2);
		PrintRingTable(A,M);
		cout<<endl;
	}
	{
		int prim_poly[] = {1,0,1};
		int d1=sizeof(prim_poly)/sizeof(prim_poly[0]);
		vector<int> f1(prim_poly,prim_poly+d1);
		vector<vector<int> > vPoly=getSet(f1,2);
		cout<<sPoly(prim_poly,d1)<<"在Z/(2)上有根，Z[i]/(2)=Z/(2)[x]/(x^2+1):"<<endl;	
		printSet(vPoly);
		vector<vector<int> > A=AddTable(vPoly,2);
		vector<vector<int> > M=MulTable(vPoly,f1,2);
		PrintRingTable(A,M);
		cout<<endl;
	}
    for(int n=2;n<7;n++){
		for(int d=2;d<4;d++)		
		{
			vector<vector<int> > IrrSet=getIrreduciblePolynomialsSet(d,n,true);
			cout<<"Z/("<<n<<")[x]上"<<d<<"次不可约多项式的个数："<<IrrSet.size()<<endl;
			if(IrrSet.size()<10)
			{
				printSet(IrrSet);
			}
			vector<vector<int> > Irr0Set=getIrreduciblePolynomialsSet(d,n,false);
			cout<<"Z/("<<n<<")[x]上"<<d<<"次可约多项式的个数："<<Irr0Set.size()<<endl;
			if(Irr0Set.size()<10)
			{
				printSet(Irr0Set);
			}				
		}
	}
	{
        int n=4;
		int poly1[] = {2,0,1};
		int d1=sizeof(poly1)/sizeof(poly1[0]);
		vector<int> f1(poly1,poly1+d1);		
		vector<vector<int> > vPoly=getSet(f1,n);
		cout<<"16阶环(Z/4Z)[x]/(x^2+2):"<<endl;	
		printSet(vPoly);
		vector<vector<int> > A=AddTable(vPoly,n);
		vector<vector<int> > M=MulTable(vPoly,f1,n);
		PrintRingTable(A,M);
		cout<<endl;
		int poly2[] = {0,1};
		int d2=sizeof(poly2)/sizeof(poly2[0]);
		vector<int> f2(poly2,poly2+d2);		
		vector<vector<int> > vPoly2=getFRSet(f2,f1,n);
		cout<<"8阶环(Z/4Z)[x]/(x,x^2+2):"<<endl;	
		printSet(vPoly2);
		vector<vector<int> > A2=AddTable(vPoly2,n);
		vector<vector<int> > M2=MulTable(vPoly2,f1,n);
		PrintRingTable(A2,M2);
		cout<<endl;
	}	
	return 0;
}