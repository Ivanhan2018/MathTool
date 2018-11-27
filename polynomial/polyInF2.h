#ifndef POLY_IN_F2__H
#define POLY_IN_F2__H

#include<cstdio>
#include<cmath>
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;

class polyInF2;

ostream & operator << (ostream &, const polyInF2 &);
istream & operator >> (istream &, polyInF2 &);

class polyInF2 {
	// variables
private:
	vector<int>				coef;	
	static vector<polyInF2>		irreducible;

	// methods
private:
	inline void refresh();
	bool isPrimitive_step2() const;
	// 生成次数不高于 m 的不可约多项式表
	static void generateIrreducible(const int m);

public:
	// constructors
	polyInF2();	
	polyInF2(const polyInF2 &);
	polyInF2(const string &);

	// destructors
	~polyInF2();

	// overloaded operators
	polyInF2 & operator = (const polyInF2 &);
	bool operator == (const polyInF2 &) const;
	polyInF2 operator + (const polyInF2 &) const;
	polyInF2 operator - (const polyInF2 &) const;
	polyInF2 operator * (const polyInF2 &) const;
	polyInF2 operator / (const polyInF2 &) const;
	polyInF2 operator % (const polyInF2 &) const;	

	bool isZero() const;
	bool isOne() const;
	/*
		GF(q)为GF(q^m)的基域，GF(q^m)为GF(q)的扩域
		本原多项式
		GF(q)上的m次不可约多项式p(x)，若能被p(x)整除x^n-1的最小正整数n=2^m-1，则称p(x)为GF(q)上的m次本原多项式。
		在GF(q^m)中，以本原元为根的最小多项式称为该域的本原多项式。
		g = x^4+x+1=10011=19是GF(2)上的4次本原多项式。
		f = x^4+x^3+x^2+x+1=11111=31是不可约多项式，但由于它能整除x^5-1，因此它不是本原多项式。
		又如AES中f=x^8+x^4+x^3+x+1=100011011=283是非本原不可约多项式,g=x^8+x^4+x^3+x^2+1=100011101=285是本原多项式
		f0=0,f1=1都是非本原不可约多项式
	*/
	bool isPrimitive() const;	
	bool isIrreducible() const;
	polyInF2 inverse (const polyInF2 &) const;

	// 打印 m 次本原多项式
	static void listPrimitive(const int m);

	static string ToStr(const polyInF2 &);
	static unsigned long long ToUINT64(const polyInF2 &);

	friend ostream & operator << (ostream &, const polyInF2 &);
	friend istream & operator >> (istream &, polyInF2 &);
};


#endif