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
	// ���ɴ��������� m �Ĳ���Լ����ʽ��
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
		GF(q)ΪGF(q^m)�Ļ���GF(q^m)ΪGF(q)������
		��ԭ����ʽ
		GF(q)�ϵ�m�β���Լ����ʽp(x)�����ܱ�p(x)����x^n-1����С������n=2^m-1�����p(x)ΪGF(q)�ϵ�m�α�ԭ����ʽ��
		��GF(q^m)�У��Ա�ԭԪΪ������С����ʽ��Ϊ����ı�ԭ����ʽ��
		g = x^4+x+1=10011=19��GF(2)�ϵ�4�α�ԭ����ʽ��
		f = x^4+x^3+x^2+x+1=11111=31�ǲ���Լ����ʽ����������������x^5-1����������Ǳ�ԭ����ʽ��
		����AES��f=x^8+x^4+x^3+x+1=100011011=283�ǷǱ�ԭ����Լ����ʽ,g=x^8+x^4+x^3+x^2+1=100011101=285�Ǳ�ԭ����ʽ
		f0=0,f1=1���ǷǱ�ԭ����Լ����ʽ
	*/
	bool isPrimitive() const;	
	bool isIrreducible() const;
	polyInF2 inverse (const polyInF2 &) const;

	// ��ӡ m �α�ԭ����ʽ
	static void listPrimitive(const int m);

	static string ToStr(const polyInF2 &);
	static unsigned long long ToUINT64(const polyInF2 &);

	friend ostream & operator << (ostream &, const polyInF2 &);
	friend istream & operator >> (istream &, polyInF2 &);
};


#endif