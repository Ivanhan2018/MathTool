#include"FiniteGroup.h"
#include"DecompositionGroup.h"
using namespace std;

typedef IGroup*(*newGFunc)(int);
class grouptool
{
public:
	grouptool();
	~grouptool();
public:
    static int NumberSmallGroups(int n);
    static newGFunc newGn(int n);	
	static vector<int> Cn(int n);
    static IGroup* newCn(int n);
    static IGroup* newG8(int ID);	
    static IGroup* newG12(int ID);	
    static IGroup* newG16(int ID);
    static IGroup* newG24(int ID);
    static IGroup* newG32(int ID);	
    static IGroup* newG48(int ID);	
};

// 生成循环群C_n的凯莱表
vector<int> grouptool::Cn(int n){
 vector<int> v(n*n);	
 for(int i=0;i<n;i++){
  for(int j=0;j<n;j++){
	int ij=(i+j)%n;
    v[i*n+j]=ij;	
  }
 }
 return v;
}

int grouptool::NumberSmallGroups(int n){
    static map<int,int> M;
	if(M.size()==0){
		M.insert(make_pair(8,5));
		M.insert(make_pair(12,5));
		M.insert(make_pair(16,14));
		M.insert(make_pair(24,15));
		M.insert(make_pair(32,51));
		M.insert(make_pair(48,52));		
	}
	map<int,int>::const_iterator it=M.find(n);
	if(it!=M.end()){	
		return it->second;	
	}
	return 0;
}

newGFunc grouptool::newGn(int n){
    static map<int,newGFunc> M;
	if(M.size()==0){
		M.insert(make_pair(8,grouptool::newG8));
		M.insert(make_pair(12,grouptool::newG12));
		M.insert(make_pair(16,grouptool::newG16));
		M.insert(make_pair(24,grouptool::newG24));
		M.insert(make_pair(32,grouptool::newG32));
		M.insert(make_pair(48,grouptool::newG48));		
	}
	map<int,newGFunc>::const_iterator it=M.find(n);
	if(it!=M.end()){	
		return it->second;	
	}
	return NULL;
}

IGroup* grouptool::newCn(int n){
	static map<int,vector<int> > M;
	map<int,vector<int> >::const_iterator it=M.find(n);
	vector<int>* p=NULL;
	if(it!=M.end()){	
		p=const_cast<vector<int>*>(&(it->second));	
	}else{
		M[n]=Cn(n);
		p=&M[n];
	}
	FiniteGroup *fg=new FiniteGroup(n,&((*p)[0]),0);
	return fg;	
}

IGroup* grouptool::newG8(int ID){
	static int g_C8Mul_2[8][8]={
		{0,1,2,3,4,5,6,7},
		{1,2,3,4,5,6,7,0},
		{2,3,4,5,6,7,0,1},
		{3,4,5,6,7,0,1,2},
		{4,5,6,7,0,1,2,3},
		{5,6,7,0,1,2,3,4},
		{6,7,0,1,2,3,4,5},
		{7,0,1,2,3,4,5,6}
	};	
	static int g_C2C4Mul_2[8][8]={
		{0,1,2,3,4,5,6,7},
		{1,4,7,2,5,0,3,6},
		{2,7,4,1,6,3,0,5},
		{3,2,1,0,7,6,5,4},
		{4,5,6,7,0,1,2,3},
		{5,0,3,6,1,4,7,2},
		{6,3,0,5,2,7,4,1},
		{7,6,5,4,3,2,1,0}
	};	
	if(ID==1){
		FiniteGroup *fg=new FiniteGroup(8,&g_C8Mul_2[0][0],0);
		return fg;
	}		
	if(ID==2){
		FiniteGroup *fg=new FiniteGroup(8,&g_C2C4Mul_2[0][0],0);
		return fg;
	}	
    return NULL;	
}

IGroup* grouptool::newG12(int ID){
	static int G12_2[12][12]=
	{
		{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
		{2, 1, 4, 3, 6, 5, 8, 7, 10, 9, 12, 11},
		{3, 4, 5, 6, 1, 2, 9, 10, 11, 12, 7, 8},
		{4, 3, 6, 5, 2, 1, 10, 9, 12, 11, 8, 7},
		{5, 6, 1, 2, 3, 4, 11, 12, 7, 8, 9, 10},
		{6, 5, 2, 1, 4, 3, 12, 11, 8, 7, 10, 9},
		{7, 8, 9, 10, 11, 12, 2, 1, 4, 3, 6, 5},
		{8, 7, 10, 9, 12, 11, 1, 2, 3, 4, 5, 6},
		{9, 10, 11, 12, 7, 8, 4, 3, 6, 5, 2, 1},
		{10, 9, 12, 11, 8, 7, 3, 4, 5, 6, 1, 2},
		{11, 12, 7, 8, 9, 10, 6, 5, 2, 1, 4, 3},
		{12, 11, 8, 7, 10, 9, 5, 6, 1, 2, 3, 4}
	};
	static int G12_3[12][12]=
	{
		{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
		{2, 1, 4, 3, 8, 7, 6, 5, 11, 12, 9, 10},
		{3, 4, 1, 2, 6, 5, 8, 7, 12, 11, 10, 9},
		{4, 3, 2, 1, 7, 8, 5, 6, 10, 9, 12, 11},
		{5, 6, 7, 8, 9, 10, 11, 12, 1, 2, 3, 4},
		{6, 5, 8, 7, 12, 11, 10, 9, 3, 4, 1, 2},
		{7, 8, 5, 6, 10, 9, 12, 11, 4, 3, 2, 1},
		{8, 7, 6, 5, 11, 12, 9, 10, 2, 1, 4, 3},
		{9, 10, 11, 12, 1, 2, 3, 4, 5, 6, 7, 8},
		{10, 9, 12, 11, 4, 3, 2, 1, 7, 8, 5, 6},
		{11, 12, 9, 10, 2, 1, 4, 3, 8, 7, 6, 5},
		{12, 11, 10, 9, 3, 4, 1, 2, 6, 5, 8, 7}
	};
	static int G12_4[12][12]=
	{
		{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
		{2, 3, 1, 5, 6, 4, 9, 7, 8, 12, 10, 11},
		{3, 1, 2, 6, 4, 5, 8, 9, 7, 11, 12, 10},
		{4, 5, 6, 1, 2, 3, 10, 11, 12, 7, 8, 9},
		{5, 6, 4, 2, 3, 1, 12, 10, 11, 9, 7, 8},
		{6, 4, 5, 3, 1, 2, 11, 12, 10, 8, 9, 7},
		{7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6},
		{8, 9, 7, 11, 12, 10, 3, 1, 2, 6, 4, 5},
		{9, 7, 8, 12, 10, 11, 2, 3, 1, 5, 6, 4},
		{10, 11, 12, 7, 8, 9, 4, 5, 6, 1, 2, 3},
		{11, 12, 10, 8, 9, 7, 6, 4, 5, 3, 1, 2},
		{12, 10, 11, 9, 7, 8, 5, 6, 4, 2, 3, 1}
	};
	static int G12_5[12][12]=
	{
		{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
		{2, 3, 1, 5, 6, 4, 8, 9, 7, 11, 12, 10},
		{3, 1, 2, 6, 4, 5, 9, 7, 8, 12, 10, 11},
		{4, 5, 6, 1, 2, 3, 10, 11, 12, 7, 8, 9},
		{5, 6, 4, 2, 3, 1, 11, 12, 10, 8, 9, 7},
		{6, 4, 5, 3, 1, 2, 12, 10, 11, 9, 7, 8},
		{7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6},
		{8, 9, 7, 11, 12, 10, 2, 3, 1, 5, 6, 4},
		{9, 7, 8, 12, 10, 11, 3, 1, 2, 6, 4, 5},
		{10, 11, 12, 7, 8, 9, 4, 5, 6, 1, 2, 3},
		{11, 12, 10, 8, 9, 7, 5, 6, 4, 2, 3, 1},
		{12, 10, 11, 9, 7, 8, 6, 4, 5, 3, 1, 2}
	};	
	if(ID==2){
		FiniteGroup *fg=new FiniteGroup(12,&G12_2[0][0],1);
		return fg;
	}	
	if(ID==3){
		FiniteGroup *fg=new FiniteGroup(12,&G12_3[0][0],1);
		return fg;
	}		
	if(ID==4){
		FiniteGroup *fg=new FiniteGroup(12,&G12_4[0][0],1);
		return fg;
	}		
	if(ID==5){
		FiniteGroup *fg=new FiniteGroup(12,&G12_5[0][0],1);
		return fg;
	}	
	return NULL;	
}

IGroup* grouptool::newG16(int ID){
	static int g_M16Mul[16][16]={
		{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
		{1,0,8,9,10,11,7,6,2,3,4,5,15,14,13,12},
		{2,8,4,6,5,1,12,15,10,7,11,0,13,9,3,14},
		{3,9,7,0,12,14,8,2,6,1,15,13,4,11,5,10},
		{4,10,5,12,1,8,13,14,11,15,0,2,9,7,6,3},
		{5,11,1,13,8,10,9,3,0,14,2,4,7,15,12,6},
		{6,7,15,2,13,3,10,4,12,8,14,9,5,0,1,11},
		{7,6,12,8,14,9,4,10,15,2,13,3,11,1,0,5},
		{8,2,10,7,11,0,15,12,4,6,5,1,14,3,9,13},
		{9,3,6,1,15,13,2,8,7,0,12,14,10,5,11,4},
		{10,4,11,15,0,2,14,13,5,12,1,8,3,6,7,9},
		{11,5,0,14,2,4,3,9,1,13,8,10,6,12,15,7},
		{12,15,14,4,9,6,11,5,13,10,3,7,1,2,8,0},
		{13,14,3,5,7,12,0,1,9,11,6,15,8,4,10,2},
		{14,13,9,11,6,15,1,0,3,5,7,12,2,10,4,8},
		{15,12,13,10,3,7,5,11,14,4,9,6,0,8,2,1},
	};
	static int g_P16Mul[16][16]={
		{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
		{1,0,3,2,5,4,7,6,9,8,11,10,13,12,15,14},
		{2,3,1,0,6,7,5,4,10,11,9,8,14,15,13,12},
		{3,2,0,1,7,6,4,5,11,10,8,9,15,14,12,13},
		{4,5,6,7,0,1,2,3,14,15,13,12,11,10,8,9},
		{5,4,7,6,1,0,3,2,15,14,12,13,10,11,9,8},
		{6,7,5,4,2,3,1,0,13,12,15,14,8,9,10,11},
		{7,6,4,5,3,2,0,1,12,13,14,15,9,8,11,10},
		{8,9,10,11,15,14,12,13,0,1,2,3,6,7,5,4},
		{9,8,11,10,14,15,13,12,1,0,3,2,7,6,4,5},
		{10,11,9,8,12,13,14,15,2,3,1,0,5,4,7,6},
		{11,10,8,9,13,12,15,14,3,2,0,1,4,5,6,7},
		{12,13,14,15,10,11,9,8,7,6,4,5,0,1,2,3},
		{13,12,15,14,11,10,8,9,6,7,5,4,1,0,3,2},
		{14,15,13,12,9,8,11,10,4,5,6,7,2,3,1,0},
		{15,14,12,13,8,9,10,11,5,4,7,6,3,2,0,1},
	};	
	if(ID==6){
		FiniteGroup *fg=new FiniteGroup(16,&g_M16Mul[0][0],0);
		return fg;
	}
	if(ID==13){
		FiniteGroup *fg=new FiniteGroup(16,&g_P16Mul[0][0],0);
		return fg;
	}	
	return NULL;
}

IGroup* grouptool::newG24(int ID){
	if(ID==2){
		IGroup *g1=newG8(1);
		IGroup *g2=newCn(3);
		DecompositionGroup *dg=new DecompositionGroup(g1,g2);
		return dg;
	}	
	if(ID==9){
#if 1
		IGroup *g1=newG8(2);
		IGroup *g2=newCn(3);
#else
		IGroup *g1=newG12(2);
		IGroup *g2=newCn(2);
#endif
		DecompositionGroup *dg=new DecompositionGroup(g1,g2);
		return dg;
	}
	if(ID==13){
		IGroup *g1=newG12(3);
		IGroup *g2=newCn(2);
		DecompositionGroup *dg=new DecompositionGroup(g1,g2);
		return dg;
	}
	if(ID==14){
		IGroup *g1=newG12(4);
		IGroup *g2=newCn(2);
		DecompositionGroup *dg=new DecompositionGroup(g1,g2);
		return dg;
	}
	if(ID==15){
		IGroup *g1=newG12(5);
		IGroup *g2=newCn(2);
		DecompositionGroup *dg=new DecompositionGroup(g1,g2);
		return dg;
	}	
	return NULL;	
}

IGroup* grouptool::newG32(int ID){
	if(ID==37){
		IGroup *g1=newG16(6);
		IGroup *g2=newCn(2);
		DecompositionGroup *dg=new DecompositionGroup(g1,g2);
		return dg;
	}
	if(ID==48){
		IGroup *g1=newG16(13);
		IGroup *g2=newCn(2);
		DecompositionGroup *dg=new DecompositionGroup(g1,g2);
		return dg;
	}
	return NULL;	
}

IGroup* grouptool::newG48(int ID){
	if(ID==24){
		IGroup *g1=newG16(6);
		IGroup *g2=newCn(3);
		DecompositionGroup *dg=new DecompositionGroup(g1,g2);
		return dg;
	}
	if(ID==47){
		IGroup *g1=newG16(13);
		IGroup *g2=newCn(3);
		DecompositionGroup *dg=new DecompositionGroup(g1,g2);
		return dg;
	}
	return NULL;	
}

int main(){
	int ns[]={8,12,16,24,32,48};
	int cnt=sizeof(ns)/sizeof(ns[0]);
	for(int i=0;i<cnt;i++){
		int m=grouptool::NumberSmallGroups(ns[i]);
		newGFunc f=grouptool::newGn(ns[i]);
		if(m==0||f==NULL)
			continue;
		for(int j=1;j<=m;j++){
			IGroup *g=f(j);
			if(g){
				g->printSet();
				g->printTable();
				delete g;
				g=NULL;			
			}
		}
	}
	return 0;
}