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

	cin.get();
	//system("pause");
	return 0;
}