#include <iostream>
using namespace std;
#include "zpoly.h"

int main(int argc, char* argv[])
{
#if 1
	 // 根据本原n次单位根计算n次分圆多项式
	 DComplex c3pr[]={{-0.5,0.866},{-0.5,-0.866}};
	 DComplex c4pr[]={{0,1},{0,-1}};
	 DComplex c5pr[]={{0.309017,0.951057},{-0.809017,0.587785},{-0.809017,-0.587785},{0.309017,-0.951057}};
         DComplex* cnpr[] ={&c3pr[0],&c4pr[0],&c5pr[0]};
         int nArr[]={3,4,5};
         int n1Arr[]={2,2,4};
         for(int i=0;i<3;i++)
         {
	   vector<DComplex> vc=PolyUtil::polyCmul(cnpr[i],n1Arr[i]);
	   vector<int> vz;
	   bool bret=PolyUtil::CPolyToZPoly(vc,vz);
           if(bret)
           {
            zpoly cn;
	    cn.set (vz);
	    cout << nArr[i]<<"次分圆多项式=";
 	    cn.print(); 
	    cout << "\n";              
           }            
         } 
#endif

#if 1
	int aa[3]={0,1,2};//0x^0+1x^1+2x^2
	int bb[2]={3,1};//3x^0+1x^1
	const int m=sizeof(aa)/sizeof(int);
	const int n=sizeof(bb)/sizeof(int);
	int k=0;
	int cc[m+n-1]={0};//0x^0+3x^1+7x^2+2x^3
	int ret=PolyUtil::polymul(aa,m,bb,n,cc,&k);

        vector<vector<int> > A(m),B(n),C(m+n-1);
        for(int i=0;i<m;i++)
        {
           vector<int> v(2);
           v[0]=aa[i];
           v[1]=i;
           A[i]=v;   
        }
        for(int i=0;i<n;i++)
        {
           vector<int> v(2);
           v[0]=bb[i];
           v[1]=i;
           B[i]=v;   
        }
        for(int i=0;i<m+n-1;i++)
        {
           vector<int> v(2);
           v[0]=cc[i];
           v[1]=i;
           C[i]=v;   
        }

	zpoly a, b, c1,c, d;
	a.set (A);
	b.set (B);
        c1.set (C);
	cout << "c1=a*b=";
	c1.print();
	cout << "\n";

#else
	const char* strA=" 7x^4   +  x^2";
	//const char* strB="6x^3 - 3x^2";
	const char* strB="- 3x^2+6x^3";

	zpoly a, b, c, d;
	a.set (strA);
	b.set (strB);
#endif

	cout << "a=";
 	a.print(); 
	cout << "\n";

	cout << "b=";      
 	b.print();
	cout << "\n";

	c = a.minus ( b ); // (7x^4 + x^2) - (6x^3 - 3x^2)
	cout << "c=a-b=";
	c.print();

	cout << "\n";

	c = a.times ( b ); // (7x^4 + x^2) * (6x^3 - 3x^2)
	cout << "c=a*b=";
	c.print();
	cout << "\n";

	cout << "\n";

	d = c.differentiate().differentiate();
	cout << "d=c''=";
	d.print();
	cout << "\n";

	cout << "c(2)=";
	cout << c.evaluate ( 2 ); //substitue x with 2
	cout << "\n";

	cin.get();

	return 0;
}