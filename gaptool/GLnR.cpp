#define PRINT_DETTR
#include"GLnR.h"

float g_S3[6][2][2]=
{
	{{1,0},{0,1}},//I
	{{-1.f/2,sqrtf(3)/2},{-sqrtf(3)/2,-1.f/2}},//r
	{{-1.f/2,-sqrtf(3)/2},{sqrtf(3)/2,-1.f/2}},//r^2
	{{1,0},{0,-1}},//f
	{{-1.f/2,sqrtf(3)/2},{sqrtf(3)/2,1.f/2}},//fr
	{{-1.f/2,-sqrtf(3)/2},{-sqrtf(3)/2,1.f/2}},//fr^2
};

// S_3的2维不可约表示
float g_S3a[6][2][2]=
{
	{{1,0},{0,1}},
	{{-1,1},{0,1}},
	{{1,0},{1,-1}},
	{{0,-1},{-1,0}},
	{{-1,1},{-1,0}},
	{{0,-1},{1,-1}},
};	

float g_S4[24][4][4]=
{ 
{ {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}},
{ {1,0,0,0},{0,1,0,0},{0,0,0,1},{0,0,1,0}},
{ {1,0,0,0},{0,0,1,0},{0,1,0,0},{0,0,0,1}},
{ {1,0,0,0},{0,0,0,1},{0,1,0,0},{0,0,1,0}},
{ {1,0,0,0},{0,0,1,0},{0,0,0,1},{0,1,0,0}},
{ {1,0,0,0},{0,0,0,1},{0,0,1,0},{0,1,0,0}},
{ {0,1,0,0},{1,0,0,0},{0,0,1,0},{0,0,0,1}},
{ {0,1,0,0},{1,0,0,0},{0,0,0,1},{0,0,1,0}},
{ {0,0,1,0},{1,0,0,0},{0,1,0,0},{0,0,0,1}},
{ {0,0,0,1},{1,0,0,0},{0,1,0,0},{0,0,1,0}},
{ {0,0,1,0},{1,0,0,0},{0,0,0,1},{0,1,0,0}},
{ {0,0,0,1},{1,0,0,0},{0,0,1,0},{0,1,0,0}},
{ {0,1,0,0},{0,0,1,0},{1,0,0,0},{0,0,0,1}},
{ {0,1,0,0},{0,0,0,1},{1,0,0,0},{0,0,1,0}},
{ {0,0,1,0},{0,1,0,0},{1,0,0,0},{0,0,0,1}},
{ {0,0,0,1},{0,1,0,0},{1,0,0,0},{0,0,1,0}},
{ {0,0,1,0},{0,0,0,1},{1,0,0,0},{0,1,0,0}},
{ {0,0,0,1},{0,0,1,0},{1,0,0,0},{0,1,0,0}},
{ {0,1,0,0},{0,0,1,0},{0,0,0,1},{1,0,0,0}},
{ {0,1,0,0},{0,0,0,1},{0,0,1,0},{1,0,0,0}},
{ {0,0,1,0},{0,1,0,0},{0,0,0,1},{1,0,0,0}},
{ {0,0,0,1},{0,1,0,0},{0,0,1,0},{1,0,0,0}},
{ {0,0,1,0},{0,0,0,1},{0,1,0,0},{1,0,0,0}},
{ {0,0,0,1},{0,0,1,0},{0,1,0,0},{1,0,0,0}},
};

int main(){
	if(0){
		GLnR D3=GLnR::D2nGroup(3);
		D3.printSet();	
		D3.printTable();
		
		GLnR D24=GLnR::D2nGroup(24);
		D24.printSet();	
		D24.printTable();	
		return 0;
		
		GLnR G=GLnR::TestGroup();
		G.printSet();	
		G.printTable();
		return 0;		
	}	
	
	GLnR C4=GLnR::CyclicGroup(4);
	C4.printSet();	
	C4.printTable();
	
	GLnR C6=GLnR::CyclicGroup(6);
	C6.printSet();	
	C6.printTable();	
	
	GLnR _S3a(&g_S3a[0][0][0],2,sizeof(g_S3a)/sizeof(g_S3a[0]));
	_S3a.printSet();
	_S3a.printTable();
	
	vector<int> vA3a;
	for(int i=0;i<_S3a.size();i++){
		float det=GLnR::DeterminantMat(_S3a.s_Arr[i]);
		if(abs(det-1)<0.01){
			vA3a.push_back(i);
		}
	}
	vector<int> fgA3a=FG(&_S3a,vA3a);
	printSubGroup(&_S3a,fgA3a);
	
	GLnR _S3(&g_S3[0][0][0],2,sizeof(g_S3)/sizeof(g_S3[0]));
	_S3.printSet();
	_S3.printTable();
	
	vector<int> vA3;
	for(int i=0;i<_S3.size();i++){
		float det=GLnR::DeterminantMat(_S3.s_Arr[i]);
		if(abs(det-1)<0.01){
			vA3.push_back(i);
		}
	}
	vector<int> fgA3=FG(&_S3,vA3);
	printSubGroup(&_S3,fgA3);	
	
	GLnR _S4(&g_S4[0][0][0],4,sizeof(g_S4)/sizeof(g_S4[0]));
	_S4.printSet();
	_S4.printTable();
	
	vector<int> vA4;
	for(int i=0;i<_S4.size();i++){
		float det=GLnR::DeterminantMat(_S4.s_Arr[i]);
		if(abs(det-1)<0.01){
			vA4.push_back(i);
		}
	}
	vector<int> fgA4=FG(&_S4,vA4);
	printSubGroup(&_S4,fgA4);
	
	return 0;
}	