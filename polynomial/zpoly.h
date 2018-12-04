#include <iostream>
#include <cstring>
#include "PolyUtil.h"
using namespace std;

class zpoly
{
	//define private member functions
private:
	int m_p;//特征p
	int coef[100];  // array of coefficients
	// coef[0] would hold all coefficients of x^0
	// coef[1] would hold all x^1
	// coef[n] = x^n ...

	int deg;        // degree of polynomial (0 for the zero polynomial)

	//define public member functions
public:
	zpoly(int p=0) //default constructor
	{
		for ( int i = 0; i < 100; i++ )
		{
			coef[i] = 0;
		}
		m_p=p;
	}
	void setp(int p)
	{
		m_p=p;
	}
	void set ( int a , int b ) //setter function
	{
		int a1=a;
		if(m_p>0)
		{
			a1=Mod(a1,m_p);
		}
		coef[b] = a1;
		deg = degree();
	}

	void set (const vector<vector<int> >& vv) //setter function
	{
		for(int i=0;i<vv.size();i++)
		{
			set(vv[i][0],vv[i][1]);
		}
	}

	void set (const vector<int>& v) //setter function
	{
		for(int i=0;i<v.size();i++)
		{
			set(v[i],i);
		}
	}

	bool set (const char* str) //setter function
	{
		vector<vector<int> > v;
		bool bret=PolyUtil::parsePoly(str,v);
		if(bret)
		{
			set(v);
		}
		return bret;
	}

	int degree()
	{
		int d = 0;
		for ( int i = 0; i < 100; i++ )
			if ( coef[i] != 0 ) d = i;
		return d;
	}

        vector<int> vcoef()
	{
		vector<int> v;
		deg = degree();
		for ( int i = 0; i <= deg; i++ )
		   v.push_back(coef[i]);
		return v;
	}

        bool isIrreducible()
        {
                vector<int> v=vcoef();
                bool bFind=zpoly::FindrootInFp(v,m_p);
	        return !bFind; 
        }

	void print()
	{
		for ( int i = 99; i >= 0; i-- ) {
			if ( coef[i] != 0 ) {
				cout << coef[i] << "x^" << i << " ";
			}
		}
	}

	// use Horner's method to compute and return the polynomial evaluated at x
	int evaluate ( int x )
	{
		int p = 0;
		for ( int i = deg; i >= 0; i-- )
			p = coef[i] + ( x * p );
		if(m_p>0)
		{
			p=Mod(p,m_p);
		}
		return p;
	}

	// differentiate this polynomial and return it
	zpoly differentiate()
	{
		if ( deg == 0 )  {
			zpoly t;
			t.set ( 0, 0 );
			return t;
		}
		zpoly deriv;// = new zpoly ( 0, deg - 1 );
		deriv.deg = deg - 1;
		for ( int i = 0; i < deg; i++ )
			deriv.coef[i] = ( i + 1 ) * coef[i + 1];
		return deriv;
	}

	static int Mod(int ret,unsigned int n)
	{
		if(ret<0)
		{
			int ret1=ret+(-ret+1)*n;
			return ret1%n;
		}
		return ret%n;
	}

	zpoly operator + (const zpoly & b) const
	{
		zpoly a = *this; //a is the poly on the L.H.S
		zpoly c(m_p);

		for ( int i = 0; i <= a.deg; i++ ) c.coef[i] += a.coef[i];
		for ( int i = 0; i <= b.deg; i++ ) c.coef[i] += b.coef[i];
		c.deg = c.degree();
		if(m_p>0)
		{
			for ( int i = 0; i <= c.deg; i++ )
				c.coef[i]=Mod(c.coef[i],m_p);
		}

		return c;
	}

	zpoly operator - (const zpoly & b) const
	{
		zpoly a = *this; //a is the poly on the L.H.S
		zpoly c(m_p);

		for ( int i = 0; i <= a.deg; i++ ) c.coef[i] += a.coef[i];
		for ( int i = 0; i <= b.deg; i++ ) c.coef[i] -= b.coef[i];
		c.deg = c.degree();
		if(m_p>0)
		{
			for ( int i = 0; i <= c.deg; i++ )
				c.coef[i]=Mod(c.coef[i],m_p);
		}

		return c;
	}

	zpoly operator * (const zpoly & b) const
	{
		zpoly a = *this; //a is the poly on the L.H.S
		zpoly c(m_p);

		for ( int i = 0; i <= a.deg; i++ )
			for ( int j = 0; j <= b.deg; j++ )
				c.coef[i+j] += ( a.coef[i] * b.coef[j] );
		c.deg = c.degree();
		if(m_p>0)
		{
			for ( int i = 0; i <= c.deg; i++ )
				c.coef[i]=Mod(c.coef[i],m_p);
		}
		return c;
	}

	zpoly operator / (const zpoly & b) const
	{
		zpoly a = *this; //a is the poly on the L.H.S
		pair<zpoly,zpoly> qr=p_div(a, b,m_p);
		zpoly c = qr.first;
		return c;
	}

	zpoly operator % (const zpoly & b) const
	{
		zpoly a = *this; //a is the poly on the L.H.S
		pair<zpoly,zpoly> qr=p_div(a, b,m_p);
		zpoly c = qr.second;
		return c;
	}

	/* p: poly;  d: divisor;  r: remainder; returns quotient */
	static pair<zpoly,zpoly> p_div(zpoly p, zpoly d,int m_p=0)
	{        
		zpoly q(m_p);
		zpoly r=p;       
		int i, j;        
		int power = p.degree() - d.degree();        
		if (power < 0) 
			return pair<zpoly,zpoly>(q,r);   
		for (i = p.degree(); i >= d.degree(); i--) 
		{                
			int ratio = r.coef[i] / d.coef[d.degree()]; 
			q.coef[i - d.degree()] = ratio;            
			r.coef[i] = 0;                 
			for (j = 0; j < d.degree(); j++)                        
				r.coef[i - d.degree() + j] -= d.coef[j] * ratio;        
		}
		q.deg = q.degree();
		r.deg = r.degree();
		if(m_p>0)
		{
			for ( int i = 0; i <= q.deg; i++ )
				q.coef[i]=Mod(q.coef[i],m_p);
			for ( int i = 0; i <= r.deg; i++ )
				r.coef[i]=Mod(r.coef[i],m_p);
		}
		return pair<zpoly,zpoly>(q,r);
	} 

	static bool IsPrime(int i)
	{
		if(i<0)
			return IsPrime(-i);
		if(i==1||i==0)
			return false;
		if(i==2||i==3)
			return true;
		for(int j=2;j<=sqrt((float)i);j++)
		{
			if(i%j)
				return true;
		}
		return false;
	}

	static vector<vector<int> > makeAddTable(int n,int m,int p)
	{
		vector<vector<int> > vv;
		for(int i=0;i<m;i++)
		{
			vector<int> v;
			for(int j=0;j<m;j++)
			{
				zpoly fi=Int2poly(n,i,p);
				zpoly fj=Int2poly(n,j,p);
				zpoly fij=fi+fj;
				int ij=ToUINT64(fij,p);
				v.push_back(ij);
			}
			vv.push_back(v);
		}
		return vv;
	}

	static vector<vector<int> > makeMulTable(zpoly &f,int n,int m,int p)
	{
		vector<vector<int> > vv;
		for(int i=0;i<m;i++)
		{
			vector<int> v;
			for(int j=0;j<m;j++)
			{
				zpoly fi=Int2poly(n,i,p);
				zpoly fj=Int2poly(n,j,p);
				zpoly fij=(fi*fj)%f;
				int ij=ToUINT64(fij,p);
				v.push_back(ij);
			}
			vv.push_back(v);
		}
		return vv;
	}

#define USE_PRINT_ALL_POLY 0
#define USE_PRINT_IRR_POLY 1
	// 求F_p[x]中所有<=n次的不可约多项式
	static vector<vector<int> > findIrreducible(int n,int p)
	{
		vector<vector<int> > vv;
		int ii=0;
		int t=pow(p,n+1);
		for(int i=t-1;i>=2;i--)//注意这里的下界
		{
#if USE_PRINT_ALL_POLY
			printf("\ni=%d:",i);
#endif
			vector<int> v=zpoly::Int2Vec(n,i,p);
			for(int j=0;j<v.size();j++)
			{ 
#if USE_PRINT_ALL_POLY
				printf("%d ",v[j]);
#endif
			}     
			bool bFind=FindrootInFp(v,p);
			if(!bFind)
			{
				vv.push_back(v);
#if USE_PRINT_IRR_POLY
				printf("ii=%d\n",ii);
#endif
				for(int j=0;j<n+1;j++)
				{ 
#if USE_PRINT_IRR_POLY
					printf("%d ",v[j]);
#endif
				}
#if USE_PRINT_IRR_POLY
				zpoly c(p);
				c.set(v);
				string str=ToStr(c);
				printf("\n%s",str.c_str());
#endif
				ii++;
#if USE_PRINT_IRR_POLY
				printf("\n");
#endif         
			}     
		}

		return vv;
	}
#undef USE_PRINT_ALL_POLY
#undef USE_PRINT_IRR_POLY

	// v[n]…v[0]表示p进制数
	static bool FindrootInFp(vector<int>& v,int p)
	{
		for(int i=0;i<p;i++)
		{
			int ret=0;
			for(int j=0;j<v.size();j++)
			{
				ret+=v[j]*pow(i,j);
			}
			if(Mod(ret,p)==0)
			{
				return true;
			}
		}

		return false;
	}

	// v[n]…v[0]表示p进制数
	static vector<int> Int2Vec(int n,int a,int p)
	{
		//printf("a=%d\n",a);
		vector<int> v(n+1);
		for(int j=0;j<=n;j++)
		{ 
			int t=pow(p,j);
			v[j]=(a/t)%p;
			//printf("v[%d]=%d\n",j,v[j]);
		}
		return v;
	}

	static zpoly Int2poly(int n,int a,int p)
	{
		vector<int> v=Int2Vec(n,a,p);
		zpoly c(p);
		c.set(v);
		return c;     
	}

	static string ToStr(const zpoly &f)
	{
		string str;
		for(int i=f.deg;i>=0;i--)
		{
			char sz[40]={0};
			sprintf(sz,"%dx^%d ",f.coef[i],i);
			str+=sz;
		}
		return str;
	}

	static unsigned long long ToUINT64(const zpoly &f,int m_p)
	{
		unsigned long long lret=0;
		for(int i=0;i<=f.deg;i++)
		{
			unsigned long long ll=(unsigned long long)(pow((double)m_p,i)+0.5);
			lret+=f.coef[i]*ll;
		}
		return lret;
	}
};


