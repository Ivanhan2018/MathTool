#include <iostream>
#include <cstring>
#include "PolyUtil.h"
using namespace std;

class zpoly
{
	//define private member functions
private:
	int m_p;//ÌØÕ÷p
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
		//assert(n>0);
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

};


