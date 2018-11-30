// dpoly.h: interface for the dpoly class.
// 
//////////////////////////////////////////////////////////////////////

#if !defined(DPOLY_H)
#define DPOLY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cmath>
#include <cstring>
#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

// the node of polynomial
struct dpolynode
{
    double  cof;      // coefficient 
    int     deg;      // degree
};

class dpoly  
{
private:
    list<dpolynode> m_dpoly;	
public:
	dpoly();
	dpoly(string file);                     // initialization using file
	dpoly(double *cof,double *deg,int n);
	dpoly(const vector<double>& cof, const vector<double>& deg);
	dpoly(const vector<double>& cof);
	virtual ~dpoly();

    // overload
	dpoly& operator+( dpoly &right );	//Overload operator +
	dpoly& operator-( dpoly &right );	//Overload operator -
	dpoly& operator*( dpoly &right );	//Overload operator *
	dpoly& operator=( dpoly &right );	//Overload operator =

	void Print();
private:
	void ReadFromFile(string file);  
    void AddOneTerm(dpolynode term);   // add one term into m_dpoly
	
};

#endif // !defined(DPOLY_H)
