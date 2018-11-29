#include <iostream>
using namespace std;
#include "zpoly.h"

int main(int argc, char* argv[])
{
	const char* strA=" 7x^4   +  x^2";
	//const char* strB="6x^3 - 3x^2";
	const char* strB="- 3x^2+6x^3";

	zpoly a, b, c, d;
	a.set (strA);
	b.set (strB);

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