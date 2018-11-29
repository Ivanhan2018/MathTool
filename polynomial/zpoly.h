//-----------------------------zpoly.h---------------------------------
//-----------------------------------------------------------------------------
//
// A zpoly Class
//
// Author: Iamthwee 2008 (c)
//
// Improvements:
// Could use a dynamic array as opposed to a static one.
// Could improve the print function to tidy up the output.
// Suffers from a space complexity issue but achieves lexicographic sorting 
// very easily. I.e. prints terms in order of powers highest to lowest
// Could use operator overloading for code syntax candy lovers.
//
// Notes:
// Bugs may exist, not thorougly tested.
//
//------------------------------------------------------------------------------

#include <iostream>
#include "PolyUtil.h"
using namespace std;

class zpoly
{
	//define private member functions
private:
	int coef[100];  // array of coefficients
	// coef[0] would hold all coefficients of x^0
	// coef[1] would hold all x^1
	// coef[n] = x^n ...

	int deg;        // degree of polynomial (0 for the zero polynomial)

	//define public member functions
public:
	zpoly() //default constructor
	{
		for ( int i = 0; i < 100; i++ )
		{
			coef[i] = 0;
		}
	}
	void set ( int a , int b ) //setter function
	{
		//coef = new zpoly[b+1];
		coef[b] = a;
		deg = degree();
	}

	void set (const vector<vector<int> >& v) //setter function
	{
		for(int i=0;i<v.size();i++)
		{
                    set(v[i][0],v[i][1]);
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

	zpoly plus ( zpoly b )
	{
		zpoly a = *this; //a is the poly on the L.H.S
		zpoly c;

		for ( int i = 0; i <= a.deg; i++ ) c.coef[i] += a.coef[i];
		for ( int i = 0; i <= b.deg; i++ ) c.coef[i] += b.coef[i];
		c.deg = c.degree();

		return c;
	}

	zpoly minus ( zpoly b )
	{
		zpoly a = *this; //a is the poly on the L.H.S
		zpoly c;

		for ( int i = 0; i <= a.deg; i++ ) c.coef[i] += a.coef[i];
		for ( int i = 0; i <= b.deg; i++ ) c.coef[i] -= b.coef[i];
		c.deg = c.degree();

		return c;
	}

	zpoly times ( zpoly b )
	{
		zpoly a = *this; //a is the poly on the L.H.S
		zpoly c;

		for ( int i = 0; i <= a.deg; i++ )
			for ( int j = 0; j <= b.deg; j++ )
				c.coef[i+j] += ( a.coef[i] * b.coef[j] );
		c.deg = c.degree();
		return c;
	}
};


