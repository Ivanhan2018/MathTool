//-----------------------------zpoly0.cpp-------------------------------
#include <iostream>
using namespace std;
#include "zpoly.h"


int main()
{
	zpoly a, b, c, d;
	a.set ( 7, 4 ); //7x^4
	a.set ( 1, 2 ); //x^2

	b.set ( 6, 3 ); //6x^3
	b.set ( -3, 2 ); //-3x^2

	c = a-b; // (7x^4 + x^2) - (6x^3 - 3x^2)

	c.print();

	cout << "\n";

	c = a*b; // (7x^4 + x^2) * (6x^3 - 3x^2)
	c.print();

	cout << "\n";

	d = c.differentiate().differentiate();
	d.print();

	cout << "\n";

	cout << c.evaluate ( 2 ); //substitue x with 2
	cout << "\n";

    // F_p[x]上多项式带余除法
#if 1
	const char* poly="x^0+2x^1+1x^2+2x^3";
	const char* div="x^0+2x^1+x^2";
	const char* quot="2x^1";
	const char* rem="1x^0";
	//const char* quot="-3x^0+2x^1";
	//const char* rem="4x^0+6x^1";

	zpoly polys[5];
	for(int i=0;i<5;i++)
	   polys[i].setp (3);
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

        //pair<zpoly,zpoly> qr=zpoly::p_div(polys[0], polys[1],3);
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
	//system("pause");
	return 0;
}