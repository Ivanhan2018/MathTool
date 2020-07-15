#include"FiniteGroup.h"
#include"DecompositionGroup.h"
#include"GLnR.h"
#include"GLnC.h"
#include"GL2Zn.h"
#include"PermGroup.h"

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
	static IGroup* newT1(int n);
	static IGroup* newT2(int n);	
	static IGroup* newT3(int n);	
    static IGroup* newCn(int n);
    static IGroup* newG6(int ID);	
    static IGroup* newG8(int ID);	
    static IGroup* newG12(int ID);	
    static IGroup* newG16(int ID);
    static IGroup* newG24(int ID);
    static IGroup* newG32(int ID);
    static IGroup* newG40(int ID);	
    static IGroup* newG48(int ID);	
    static IGroup* newG72(int ID);	
};

IGroup* grouptool::newT1(int n){
    GL2Zn *t1=new GL2Zn();
	t1->m_n=n;
	vector<TM2> gen;
	TM2 a(4);
	TM2 b(4);	
	a[0]=0;
	a[1]=1;
	a[2]=n-1;
	a[3]=0;
	b[0]=n-1;
	b[1]=n-1;
	b[2]=n-1;
	b[3]=0;		
	gen.push_back(a);
	gen.push_back(b);
	t1->s_Arr=GL2Zn::FG(gen,n);
	return t1;	
}

IGroup* grouptool::newT2(int n){
    GL2Zn *t=new GL2Zn();
	t->m_n=n;
	vector<TM2> gen;
	TM2 a(4);
	TM2 b(4);	
	a[0]=0;
	a[1]=n-1;
	a[2]=n-1;
	a[3]=0;
	b[0]=n-1;
	b[1]=n-1;
	b[2]=n-1;
	b[3]=0;	
	gen.push_back(a);
	gen.push_back(b);
	t->s_Arr=GL2Zn::FG(gen,n);
	return t;	
}

IGroup* grouptool::newT3(int n){
    GL2Zn *t=new GL2Zn();
	t->m_n=n;
	vector<TM2> gen;
	TM2 a(4);
	TM2 b(4);	
	a[0]=0;
	a[1]=n-1;
	a[2]=1;
	a[3]=0;
	b[0]=n-1;
	b[1]=1;
	b[2]=0;
	b[3]=n-1;
	gen.push_back(a);
	gen.push_back(b);
	t->s_Arr=GL2Zn::FG(gen,n);
	return t;	
}

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
		M.insert(make_pair(6,2));		
		M.insert(make_pair(8,5));
		M.insert(make_pair(12,5));
		M.insert(make_pair(16,14));
		M.insert(make_pair(24,15));
		M.insert(make_pair(32,51));
		M.insert(make_pair(40,52));		
		M.insert(make_pair(48,52));	
		M.insert(make_pair(72,50));		
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
		M.insert(make_pair(6,grouptool::newG6));		
		M.insert(make_pair(8,grouptool::newG8));
		M.insert(make_pair(12,grouptool::newG12));
		M.insert(make_pair(16,grouptool::newG16));
		M.insert(make_pair(24,grouptool::newG24));
		M.insert(make_pair(32,grouptool::newG32));
		M.insert(make_pair(40,grouptool::newG40));		
		M.insert(make_pair(48,grouptool::newG48));	
		M.insert(make_pair(72,grouptool::newG72));		
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

IGroup* grouptool::newG6(int ID){
	if(ID==1){
		GLnR *fg=new GLnR();
		*fg=GLnR::D2nGroup(3);
		return fg;
	}		
	if(ID==2){
		GLnR *fg=new GLnR();
		*fg=GLnR::CyclicGroup(6);
		return fg;
	}	
    return NULL;	
}

IGroup* grouptool::newG8(int ID){
/* 	static int g_C8Mul_2[8][8]={
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
	};	 */
	if(ID==1){
#if 1
		IGroup *fg=newCn(8);
#else
		FiniteGroup *fg=new FiniteGroup(8,&g_C8Mul_2[0][0],0);
#endif
		return fg;
	}		
	if(ID==2){
#if 1
		IGroup *g1=newCn(4);
		IGroup *g2=newCn(2);
		DecompositionGroup *fg=new DecompositionGroup(g1,g2);
#else
		FiniteGroup *fg=new FiniteGroup(8,&g_C2C4Mul_2[0][0],0);
#endif
		return fg;
	}
	if(ID==3){
		GLnR *fg=new GLnR();
		*fg=GLnR::D2nGroup(4);
		return fg;
	}	
	if(ID==4){
		GLnC *fg=new GLnC();		
		*fg=GLnC::QuaternionGroup(8);
		return fg;
	}	
    return NULL;	
}

IGroup* grouptool::newG12(int ID){
	/* static int G12_2[12][12]=
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
	}; */	
	if(ID==1){
		GL2Zn *t=new GL2Zn();
		t->m_n=8;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=0;
		a[1]=1;
		a[2]=7;
		a[3]=0;
		b[0]=5;
		b[1]=3;
		b[2]=3;
		b[3]=2;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==2){
		//FiniteGroup *fg=new FiniteGroup(12,&G12_2[0][0],1);
		//return fg;
		GL2Zn *t=new GL2Zn();
		t->m_n=8;
		vector<TM2> gen;
		TM2 a(4);	
		a[0]=7;
		a[1]=7;
		a[2]=7;
		a[3]=0;
		gen.push_back(a);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;		
	}	
	if(ID==3){
		//FiniteGroup *fg=new FiniteGroup(12,&G12_3[0][0],1);
		//return fg;
		/*		
		GL2Zn *t=new GL2Zn();
		t->m_n=4;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=2;
		a[1]=1;
		a[2]=1;
		a[3]=1;
		b[0]=1;
		b[1]=1;
		b[2]=1;
		b[3]=2;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;	
		*/	
		Sn *sn=new Sn(4);
		Subgroup *an=new Subgroup(sn,sn->EvenSet(),1);
		return an;  		
	}		
	if(ID==4){
		//FiniteGroup *fg=new FiniteGroup(12,&G12_4[0][0],1);
		//return fg;
		GL2Zn *t=new GL2Zn();
		t->m_n=8;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=7;
		a[1]=0;
		a[2]=1;
		a[3]=1;
		b[0]=1;
		b[1]=7;
		b[2]=0;
		b[3]=7;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;      		
	}		
	if(ID==5){
		//FiniteGroup *fg=new FiniteGroup(12,&G12_5[0][0],1);
		//return fg;
		GL2Zn *t=new GL2Zn();
		t->m_n=8;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=7;
		a[1]=0;
		a[2]=0;
		a[3]=7;
		b[0]=2;
		b[1]=1;
		b[2]=1;
		b[3]=1;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;		
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
	if(ID==1){
		GL2Zn *t=new GL2Zn();
		t->m_n=14;
		vector<TM2> gen;
		TM2 a(4);	
		a[0]=11;
		a[1]=12;
		a[2]=12;
		a[3]=13;	
		gen.push_back(a);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==4){
		GL2Zn *t=new GL2Zn();
		t->m_n=12;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=0;
		a[1]=1;
		a[2]=11;
		a[3]=0;
		b[0]=1;
		b[1]=8;
		b[2]=8;
		b[3]=5;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}
	if(ID==5){
		GL2Zn *t=new GL2Zn();
		t->m_n=15;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=14;
		a[1]=0;
		a[2]=0;
		a[3]=14;
		b[0]=7;
		b[1]=10;
		b[2]=10;
		b[3]=12;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==6){
		FiniteGroup *fg=new FiniteGroup(16,&g_M16Mul[0][0],0);
		return fg;
	}
	if(ID==7){
		GL2Zn *t=new GL2Zn();
		t->m_n=14;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=13;
		a[1]=0;
		a[2]=1;
		a[3]=1;
		b[0]=5;
		b[1]=6;
		b[2]=3;
		b[3]=9;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==8){
		return newT1(3);
	}	
	if(ID==9){
		GL2Zn *t=new GL2Zn();
		t->m_n=14;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=0;
		a[1]=1;
		a[2]=13;
		a[3]=0;
		b[0]=13;
		b[1]=8;
		b[2]=8;
		b[3]=5;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==13){
		FiniteGroup *fg=new FiniteGroup(16,&g_P16Mul[0][0],0);
		return fg;
	}	
	return NULL;
}

IGroup* grouptool::newG24(int ID){
	if(ID==1){
		GL2Zn *t=new GL2Zn();
		t->m_n=6;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=2;
		a[1]=1;
		a[2]=1;
		a[3]=1;
		b[0]=0;
		b[1]=5;
		b[2]=5;
		b[3]=4;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}		
	if(ID==2){
		IGroup *g1=newG8(1);
		IGroup *g2=newCn(3);
		DecompositionGroup *dg=new DecompositionGroup(g1,g2);
		return dg;
	}
	if(ID==3){
		return newT3(3);
	}
	if(ID==4){
		GL2Zn *t=new GL2Zn();
		t->m_n=14;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=0;
		a[1]=1;
		a[2]=13;
		a[3]=0;
		b[0]=5;
		b[1]=3;
		b[2]=3;
		b[3]=2;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}
	if(ID==5){
		GL2Zn *t=new GL2Zn();
		t->m_n=17;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=0;
		a[1]=1;
		a[2]=16;
		a[3]=0;
		b[0]=14;
		b[1]=15;
		b[2]=15;
		b[3]=16;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==6){
		GL2Zn *t=new GL2Zn();
		t->m_n=14;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=13;
		a[1]=0;
		a[2]=1;
		a[3]=1;
		b[0]=2;
		b[1]=11;
		b[2]=1;
		b[3]=12;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==7){
		GL2Zn *t=new GL2Zn();
		t->m_n=12;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=0;
		a[1]=1;
		a[2]=11;
		a[3]=0;
		b[0]=5;
		b[1]=3;
		b[2]=3;
		b[3]=2;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==8){
		return newT1(4);
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
	if(ID==10){
		GL2Zn *t=new GL2Zn();
		t->m_n=6;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=2;
		a[1]=1;
		a[2]=1;
		a[3]=1;
		b[0]=4;
		b[1]=5;
		b[2]=3;
		b[3]=5;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==11){
		GL2Zn *t=new GL2Zn();
		t->m_n=6;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=2;
		a[1]=1;
		a[2]=1;
		a[3]=1;
		b[0]=1;
		b[1]=1;
		b[2]=1;
		b[3]=2;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==12){
		Sn *sn=new Sn(4);
		return sn;
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
	if(ID==9){
		GL2Zn *t=new GL2Zn();
		t->m_n=12;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=0;
		a[1]=1;
		a[2]=11;
		a[3]=0;
		b[0]=9;
		b[1]=10;
		b[2]=10;
		b[3]=11;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==19){
		return newT1(7);
	}	
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

IGroup* grouptool::newG40(int ID){
	if(ID==2){
		GL2Zn *t=new GL2Zn();
		t->m_n=15;
		vector<TM2> gen;
		TM2 a(4);	
		a[0]=14;
		a[1]=14;
		a[2]=14;
		a[3]=0;	
		gen.push_back(a);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==4){
		GLnC *fg=new GLnC();		
		*fg=GLnC::QuaternionGroup(40);
		return fg;
	}	
	if(ID==5){
		return newT1(5);
	}
	if(ID==6){
		GL2Zn *t=new GL2Zn();
		t->m_n=15;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=14;
		a[1]=0;
		a[2]=1;
		a[3]=1;
		b[0]=1;
		b[1]=14;
		b[2]=0;
		b[3]=14;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==7){
		GL2Zn *t=new GL2Zn();
		t->m_n=15;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=0;
		a[1]=1;
		a[2]=14;
		a[3]=0;
		b[0]=5;
		b[1]=3;
		b[2]=3;
		b[3]=2;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==8){
		return newT1(11);
	}
	if(ID==9){
		GL2Zn *t=new GL2Zn();
		t->m_n=15;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=14;
		a[1]=0;
		a[2]=0;
		a[3]=14;
		b[0]=2;
		b[1]=1;
		b[2]=1;
		b[3]=1;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	return NULL;
}

IGroup* grouptool::newG48(int ID){
	if(ID==2){
		GL2Zn *t=new GL2Zn();
		t->m_n=14;
		vector<TM2> gen;
		TM2 a(4);	
		a[0]=13;
		a[1]=13;
		a[2]=13;
		a[3]=0;	
		gen.push_back(a);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}
	if(ID==4){
		GL2Zn *t=new GL2Zn();
		t->m_n=6;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=5;
		a[1]=5;
		a[2]=5;
		a[3]=0;
		b[0]=0;
		b[1]=5;
		b[2]=5;
		b[3]=4;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==6){
		return newT1(6);//或newT1(9)、newT1(18)
	}	
	if(ID==7){
		GLnR *fg=new GLnR();
		*fg=GLnR::D2nGroup(24);
		return fg;
	}
	if(ID==8){
		GL2Zn *t=new GL2Zn();
		t->m_n=14;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=0;
		a[1]=1;
		a[2]=13;
		a[3]=0;
		b[0]=2;
		b[1]=1;
		b[2]=1;
		b[3]=1;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==13){
		GL2Zn *t=new GL2Zn();
		t->m_n=12;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=0;
		a[1]=1;
		a[2]=11;
		a[3]=0;
		b[0]=2;
		b[1]=1;
		b[2]=1;
		b[3]=1;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==14){
		return newT1(8);
	}
	if(ID==17){
		GL2Zn *t=new GL2Zn();
		t->m_n=6;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=0;
		a[1]=5;
		a[2]=5;
		a[3]=0;
		b[0]=2;
		b[1]=1;
		b[2]=1;
		b[3]=1;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==23){
		GL2Zn *t=new GL2Zn();
		t->m_n=12;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=11;
		a[1]=11;
		a[2]=11;
		a[3]=0;
		b[0]=11;
		b[1]=0;
		b[2]=0;
		b[3]=11;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==24){
		IGroup *g1=newG16(6);
		IGroup *g2=newCn(3);
		DecompositionGroup *dg=new DecompositionGroup(g1,g2);
		return dg;
	}
	if(ID==26){
		GL2Zn *t=new GL2Zn();
		t->m_n=6;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=5;
		a[1]=5;
		a[2]=5;
		a[3]=0;
		b[0]=1;
		b[1]=1;
		b[2]=1;
		b[3]=2;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==29){
		return newT2(3);
	}
	if(ID==30){
		return newT3(4);
	}	
	if(ID==32){
		GL2Zn *t=new GL2Zn();
		t->m_n=6;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=0;
		a[1]=5;
		a[2]=1;
		a[3]=0;
		b[0]=1;
		b[1]=4;
		b[2]=0;
		b[3]=1;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==47){
		IGroup *g1=newG16(13);
		IGroup *g2=newCn(3);
		DecompositionGroup *dg=new DecompositionGroup(g1,g2);
		return dg;
	}
	if(ID==48){
		GL2Zn *t=new GL2Zn();
		t->m_n=4;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=0;
		a[1]=3;
		a[2]=3;
		a[3]=0;
		b[0]=3;
		b[1]=1;
		b[2]=2;
		b[3]=3;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	if(ID==49){
		GL2Zn *t=new GL2Zn();
		t->m_n=4;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=3;
		a[1]=3;
		a[2]=3;
		a[3]=0;
		b[0]=0;
		b[1]=3;
		b[2]=3;
		b[3]=3;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	return NULL;	
}

IGroup* grouptool::newG72(int ID){
	if(ID==5){
		return newT1(17);
	}
	if(ID==8){
		return newT1(19);
	}
	if(ID==25){
		GL2Zn *t=new GL2Zn();
		t->m_n=6;
		vector<TM2> gen;
		TM2 a(4);
		TM2 b(4);	
		a[0]=0;
		a[1]=1;
		a[2]=5;
		a[3]=1;
		b[0]=1;
		b[1]=1;
		b[2]=5;
		b[3]=0;	
		gen.push_back(a);
		gen.push_back(b);
		t->s_Arr=GL2Zn::FG(gen,t->m_n);
		return t;
	}	
	return NULL;	
}

int main(int argc,char *argv[]){
	if(0){
		GLnC Q40=GLnC::QuaternionGroup(40);
		Q40.printSet();	
		Q40.printTable();

		GLnR D40=GLnR::D2nGroup(20);
		D40.printSet();	
		D40.printTable();		
		return 0;
	}
	if(argc>2){
		newGFunc f=grouptool::newGn(atoi(argv[1]));
		if(f){
			IGroup *g=f(atoi(argv[2]));
			if(g){
				printGroup(g);
				delete g;
				g=NULL;			
			}
			return 0;	
		}		
	}	
	int ns[]={6,8,12,16,24,32,40,48};
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