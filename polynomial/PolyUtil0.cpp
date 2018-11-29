#include <iostream>
#include <complex>
using namespace std;
#include "zpoly.h"

int main(int argc, char* argv[])
{


#if 1
   int N=4;
   complex<double> x(1,0);
   complex<double> y=std::pow(x,1.f/N);
   cout<<y<<endl;
   DComplex x0={1,0};
   vector<DComplex> C4=PolyUtil::CRoot(x0,N);
   for(int i=0;i<C4.size();i++)
   {
    complex<double> xi(C4[i].x,C4[i].y);
    complex<double> xiN=std::pow(*(complex<double>*)&xi,N);
    cout<<"第"<<i+1<<"个"<<N<<"次单位根="<< xi<<",模=" << std::abs(xi) << ","<<N<<"次幂=" << xiN << endl; 
   }
#endif

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

	c = a-b; // (7x^4 + x^2) - (6x^3 - 3x^2)
	cout << "c=a-b=";
	c.print();

	cout << "\n";

	c = a*b; // (7x^4 + x^2) * (6x^3 - 3x^2)
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
        
        // 多项式带余除法
#if 1
	const char* poly="x^0+2x^1+3x^2+4x^3";
	const char* div="x^0+2x^1+x^2";
	const char* quot="-5x^0+4x^1";
	const char* rem="6x^0+8x^1";

	zpoly polys[5];
	polys[0].set (poly);
	polys[1].set (div);
	polys[2].set (quot);
	polys[3].set (rem);
        polys[4]=(polys[1]*polys[2])+polys[3];
	cout << "polys[0]=";
	polys[0].print();
	cout << "\n"; 
	cout << "polys[1]=";
	polys[1].print();
	cout << "\n";
	cout << "polys[2]=";
	polys[2].print();
	cout << "\n";
	cout << "polys[3]=";
	polys[3].print();
	cout << "\n";
	cout << "polys[4]=";
	polys[4].print();
	cout << "\n"; 

        //pair<zpoly,zpoly> qr=zpoly::p_div(polys[0], polys[1]);
	cout << "q=";
	zpoly q=polys[0]/polys[1];
        q.print();
	//qr.first.print();
	cout << "\n";
	cout << "r=";
	zpoly r=polys[0]%polys[1];
        r.print();
	//qr.second.print();
	cout << "\n";     
#endif

	cin.get();

	return 0;
}