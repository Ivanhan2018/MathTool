#include"ZmodnZ.h"

void R6()
{
    ZmodnZ r6_1(6,36);
	r6_1.printTable();
    ZmodnZ r6_2(2,12);
	r6_2.printTable();	
    ZmodnZ r6_3(1,6);
	r6_3.printTable();	
    ZmodnZ r6_4(3,18);
	r6_4.printTable();	
}

void R10()
{
    ZmodnZ r10_1(10,100);
	r10_1.printTable();
    ZmodnZ r10_2(2,20);
	r10_2.printTable();	
    ZmodnZ r10_3(1,10);
	r10_3.printTable();	
    ZmodnZ r10_4(5,50);
	r10_4.printTable();	
}

void R14()
{
    ZmodnZ r14_1(14,196);
	r14_1.printTable();
    ZmodnZ r14_2(2,28);
	r14_2.printTable();	
    ZmodnZ r14_3(1,14);
	r14_3.printTable();	
    ZmodnZ r14_4(7,98);
	r14_4.printTable();	
}

void R15()
{
    ZmodnZ r15_1(15,225);
	r15_1.printTable();
    ZmodnZ r15_2(3,45);
	r15_2.printTable();	
    ZmodnZ r15_3(1,15);
	r15_3.printTable();	
    ZmodnZ r15_4(5,75);
	r15_4.printTable();	
}

int main()
{ 
    R6();
    R10();	
    R14();	
    R15();		
	return 0;
}