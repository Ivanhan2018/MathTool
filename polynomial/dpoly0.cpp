#include "dpoly.h"
/*
The polynomial is:
0  +1 x^1 +2.5 x^2
The polynomial is:
3  +1 x^1
The polynomial is:
0  +3 x^1 +8.5 x^2 +2.5 x^3
*/

int main(int argc, char* argv[])
{
	double a[]={0,1,2.5};
	vector<double> cof(a,a+3);
	dpoly pa(cof);
	pa.Print();

	double b[]={3,1};
	vector<double> cof2(b,b+2);
	dpoly pb(cof2);
	pb.Print();

	dpoly pc;
	pc=pa*pb;
	pc.Print();

	cin.get();
	return 0;
}

