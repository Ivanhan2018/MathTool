#ifndef FINITERING_H
#define FINITERING_H

//#include"IRing.h"
#define R16_I1I2
//#define R32_DEL
#include<ctime>
#include<fstream>
#include<set>
#include"DecompositionRing.h"
#include"Rn.h"
#include"M2r.h"
#include"Mnr.h"
#include"PolynomialRing.h"
#include"quotientRing.h"

// 直接从凯莱表构造一个有限环
struct FiniteRing:public IRing
{
public:
	//  静态函数  
public:
	// 实现抽象基类的方法
	virtual void printTable();
	virtual int add(int a,int b);
	virtual int mul(int a,int b);
	virtual int size(); 
	// 构造函数
	FiniteRing(int n,int* a,int* m,int delt);
	// 析构函数
	~FiniteRing();	
	// 成员变量  
	int m_n; 
	int* m_Add;
	int* m_Mul; 
	int m_delt; 
	int m_flag;// FiniteRing对象析构时是否释放m_Add、m_Mul指向的内存	
};

void FiniteRing::printTable()
{
	int ID=IdRing(this);
	string str=calcRingInvariant(this);
	printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",size(),ID,str.c_str());
	//printRing(this);	
}

int FiniteRing::add(int a,int b)
{
	int c=*(m_Add+a*m_n+b);
	return c-m_delt;
}

int FiniteRing::mul(int a,int b)
{
	int c=*(m_Mul+a*m_n+b);
	return c-m_delt;
}

int FiniteRing::size()
{
	return m_n;
}

FiniteRing::FiniteRing(int n,int* a,int* m,int delt)
{
	m_n=n;
	m_Add=a;
    m_Mul=m;
    m_delt=delt;	
}

FiniteRing::~FiniteRing(){
	if(m_flag==1 && m_Add!=NULL){
		delete m_Add;
		m_Add=NULL;
	}	
	if(m_flag==1 && m_Mul!=NULL){
		delete m_Mul;
		m_Mul=NULL;
	}		
}

void printRing0(IRing* r,int ID){
   int n=r->size();
   printf("static int g_R%d_%dAdd[%d][%d]={\n",n,ID,n,n);   
   for(int i=0;i<n;i++){
	   printf("{"); 
	   for(int j=0;j<n;j++){
		  int ij=r->add(i,j);
		  printf("%d",ij);
		  if(j<n-1)
			printf(",");  
	   } 
       printf("},\n");   
   }
   printf("};\n");    
   printf("static int g_R%d_%dMul[%d][%d]={\n",n,ID,n,n);   
   for(int i=0;i<n;i++){
	   printf("{"); 
	   for(int j=0;j<n;j++){
		  int ij=r->mul(i,j);
		  printf("%d",ij);
		  if(j<n-1)
			printf(",");  
	   } 
       printf("},\n");   
   }
   printf("};\n"); 
}

void writeTable(IRing* r,const char *path){
   ofstream fout(path);
   int n=r->size();
   fout<<"[R"<<n<<"Add]"<<endl;   
   for(int i=0;i<n;i++){
	   for(int j=0;j<n;j++){
		  int ij=r->add(i,j);
		  fout<<ij+1<<" ";
	   } 
       fout<<endl;   
   }
   fout<<"[R"<<n<<"Mul]"<<endl;   
   for(int i=0;i<n;i++){
	   for(int j=0;j<n;j++){
		  int ij=r->mul(i,j);
		  fout<<ij+1<<" "; 
	   } 
       fout<<endl;	   
   }	
}

#ifdef PARSE_RING_FILE

vector<char> lof(const char *fn)
{
	vector<char> ret;
	FILE *fp;
	int i=0;
	char ch=0;
	if((fp=fopen(fn, "rb"))!=NULL)
	{
		fseek(fp, 0, SEEK_SET); 
		while (ch!= EOF) 
		{
			i++; 
			ch = fgetc(fp);
			ret.push_back(ch);
		}; 
		fclose(fp);
	}
	if(ret.size()>0 && ret[ret.size()-1]==EOF)
		ret.pop_back();
	return ret; 
}

vector<char> lof2(const char *fn)
{
	vector<char> ret;
	FILE *fp;
	int i=0;
	char ch=0;
        int flag=0;//0:有效数据,1:注释开始,2:注释结束
	if((fp=fopen(fn, "rb"))!=NULL)
	{
		fseek(fp, 0, SEEK_SET); 
		while (ch!= EOF) 
		{
	           ch = fgetc(fp);
		   if (ch!='[' && ch!=']' && (flag==0||flag==2)) 
		   {
			i++; 
			ret.push_back(ch);
                        flag=0;
		   } 
                   else
                   {
                        if(ch=='[')
                        {
                          flag=1;
                        }
                        else if(ch==']')
                        {
                          flag=2;
                        }  
                   }                      
		}; 
		fclose(fp);
	}
	if(ret.size()>0 && ret[ret.size()-1]==EOF)
		ret.pop_back();
	return ret;
}

vector<char> CharArrToNormal(const vector<char>& vec)
{
	vector<char> ret;
	int n=vec.size();
	for(int i=0;i<n;i++)
	{
		if(vec[i]==32||vec[i]==13||vec[i]==10)
		{
			if(ret.size()>0 && ret[ret.size()-1]!=',')
				ret.push_back(',');
		}
		else
		{
			ret.push_back(vec[i]);
		}
	}
	return ret;
}

string CharArrToStr(const vector<char>& vec)
{
	string str;
	int n=vec.size();
	for(int i=0;i<n;i++)
	{
		str.push_back(vec[i]);
	}
	return str;
}

std::vector<string> split( const std::string& str, const std::string& delims, unsigned int maxSplits = 0)
{
	std::vector<string> ret;
	unsigned int numSplits = 0;
	// Use STL methods 
	size_t start, pos;
	start = 0;
	do 
	{
		pos = str.find_first_of(delims, start);
		if (pos == start)
		{
			// Do nothing
			start = pos + 1;
		}
		else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
		{
			// Copy the rest of the std::string
			ret.push_back( str.substr(start) );
			break;
		}
		else
		{
			// Copy up to delimiter
			ret.push_back( str.substr(start, pos - start) );
			start = pos + 1;
		}
		// parse up to next real data
		start = str.find_first_not_of(delims, start);
		++numSplits;
	} while (pos != std::string::npos);
	return ret;
}

vector<int> atoV1(const char* strMtx,int k=1,int idx=0)
{
	vector<int> vMtx;
	vector<int> iAll;
	if(strMtx!=0)
	{
		vector<string> All=split(strMtx,",");
		for(int i=0;i<All.size();i++)
		{
			int iElem=atoi(All[i].c_str());
			iAll.push_back(iElem);
		}
	}
	int n=iAll.size()/k;
	int n1=(int)sqrtf(n);
	if(n1*n1==n)
	{
		for(int k=0;k<n;k++)
		{
			int i=k/n1;
			int j=k%n1;
			int iElem=iAll[i*n1+j+idx*n];
			vMtx.push_back(iElem);
		}
	}
	return vMtx;
}

int IsLegalTable(const vector<int> &mtx)
{
	vector<int> ret(3);
	int illegal=-1;
	int n=mtx.size();
	int n1=(int)sqrtf(n);
	bool bLegal=(n1*n1==n);
	return bLegal?n1:0;
}

FiniteRing* newRing(const char* szFileName){
	vector<char> A=lof2(szFileName);
	string strA=CharArrToStr(CharArrToNormal(A));
	vector<int> vA=atoV1(strA.c_str(),2,0);
	vector<int> vM=atoV1(strA.c_str(),2,1);

	int n=IsLegalTable(vA);
	if(n==0)
		return 0;
	int *Add=new int[n*n];
	int *Mul=new int[n*n];
    memcpy(Add,&vA[0],sizeof(int)*n*n);
    memcpy(Mul,&vM[0],sizeof(int)*n*n);	
	FiniteRing* r=new FiniteRing(n,Add,Mul,1);
	r->m_flag=1;
	return r;
}

#endif

IRing* newR2(int i)
{
	if(i==1)
	{
		ZmodnZ* r=new ZmodnZ(2,4);
		return r;
	}
	if(i==2)
	{
		ZmodnZ* r=new ZmodnZ(1,2);
		return r;
	}
	return NULL;
}

IRing* newR4(int i,int p=2)
{
	if(i==1)
	{
		ZmodnZ* r=new ZmodnZ(p*p,p*p*p*p);
		return r;
	}
	if(i==2)
	{
		ZmodnZ* r=new ZmodnZ(p,p*p*p);
		return r;
	}	
	if(i==3)
	{
		ZmodnZ* r=new ZmodnZ(1,p*p);
		return r;
	}
	if(i==4)
	{
		M2r* r=new M2r();
	    r->initD(p);
		return r;
	}	
	if(i==5)
	{
		Mnr* r=new Mnr();
	    r->initE(p);
		return r;
	}	
	if(i==6)
	{
		Mnr* r=new Mnr();
	    r->initF(p);
		return r;
	}	
	if(i==7)
	{
		M2r* r=new M2r();
	    r->initG(p);
		return r;
	}
	if(i==8)
	{
		M2r* r=new M2r();
	    r->initH(p);
		return r;
	}
	if(i==9)
	{
		M2r* r=new M2r();
	    r->initI(p);
		return r;
	}
	if(i==10)
	{
		M2r* r=new M2r();
	    r->initJ(p);
		return r;
	}
	if(i==11)
	{
		M2r* r=new M2r();
	    r->initK(p);
		return r;
	}	
	return NULL;
}

IRing* newR8(int i)
{
	if(i==1)
	{
		ZmodnZ* r=new ZmodnZ(8,64);
		return r;
	}
	if(i==2)
	{
		ZmodnZ* r=new ZmodnZ(2,16);
		return r;
	}	
	if(i==3)
	{
		ZmodnZ* r=new ZmodnZ(1,8);
		return r;
	}
	if(i==4)
	{
		ZmodnZ* r=new ZmodnZ(4,32);
		return r;
	}
	if(i==5)//R8_5:=DirectSum(SmallRing(4,1),SmallRing(2,1));;
	{
		ZmodnZ* r4=new ZmodnZ(4,16);
		ZmodnZ* r2=new ZmodnZ(2,4);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;
		return r;
	}	
	if(i==6)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_6Mul[0][0],0);
		return r;
	}
	if(i==7)//R8_7:=DirectSum(SmallRing(4,1),SmallRing(2,2));;
	{
		ZmodnZ* r4=new ZmodnZ(4,16);
		ZmodnZ* r2=new ZmodnZ(1,2);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;
		return r;
	}	
	if(i==8)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_8Mul[0][0],0);
		return r;
	}
	if(i==9)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_9Mul[0][0],0);
		return r;
	}
	if(i==10)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_10Mul[0][0],0);
		return r;
	}
	if(i==11)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_11Mul[0][0],0);
		return r;
	}
	if(i==12)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_12Mul[0][0],0);
		return r;
	}
	if(i==13)
	{
		Mnr* r=new Mnr();
		r->initR8(13);
		return r;
	}
	if(i==14)//R8_14:=DirectSum(SmallRing(4,3),SmallRing(2,2));;R8_14:=DirectSum(ZmodnZ(2),ZmodnZ(4));;
	{
		//不是环FiniteRing* r=new FiniteRing(8,g_C2C4Add,&g_Z4F2Mul[0][0],0);
		ZmodnZ* r4=new ZmodnZ(1,4);
		ZmodnZ* r2=new ZmodnZ(1,2);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;		
		return r;
	}
	if(i==15)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_15Mul[0][0],0);
		return r;
	}
	if(i==16)//R8_16:=DirectSum(SmallRing(4,2),SmallRing(2,1));;
	{
		ZmodnZ* r4=new ZmodnZ(2,8);
		ZmodnZ* r2=new ZmodnZ(2,4);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;
		return r;
	}
	if(i==17)//R8_17:=DirectSum(SmallRing(4,2),SmallRing(2,2));;
	{
		ZmodnZ* r4=new ZmodnZ(2,8);
		ZmodnZ* r2=new ZmodnZ(1,2);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;
		return r;
	}	
	if(i==18)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_18Mul[0][0],0);
		return r;
	}
	if(i==19)
	{
		M2r* r=new M2r();
		MATRIXi A(2,vector<int>(2,0));
		MATRIXi B(2,vector<int>(2,0));
		A[0][0]=2;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=2;
		B[0][0]=1;
		B[0][1]=1;
		B[1][0]=1;
		B[1][1]=1;
		vector<MATRIXi> gen;
		gen.push_back(A);
		gen.push_back(B);
		r->m_r=new ZmodnZ(1,4);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen);   
		return r;
	}
	if(i==20)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_20Mul[0][0],0);
		return r;
	}
	if(i==21)
	{
		M2r* r=new M2r();
		MATRIXi A(2,vector<int>(2,0));
		A[0][0]=1;
		A[0][1]=0;
		A[1][0]=0;
		A[1][1]=3;
		vector<MATRIXi> gen;
		gen.push_back(A);
		r->m_r=new ZmodnZ(1,4);
		r->m_flag=1;
		r->m_Set=M2r::FR(r->m_r,gen);   
		return r;
	}
	if(i==22)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_22Mul[0][0],0);
		return r;
	}
	if(i==23)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_23Mul[0][0],0);
		return r;
	}
	if(i==24)
	{
		FiniteRing* r=new FiniteRing(8,&g_R8_24Add[0][0],&g_R8_24Mul[0][0],0);
		return r;
	}
	if(i==25)//R8_25:=DirectSum(SmallRing(4,4),SmallRing(2,1));;
	{
		M2r* r4=new M2r;
		r4->initD(2);
		ZmodnZ* r2=new ZmodnZ(2,4);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;
		return r;
	}	
	if(i==26)//R8_26:=DirectSum(SmallRing(4,5),SmallRing(2,1));;
	{
		Mnr* r4=new Mnr;
		r4->initE(2);
		ZmodnZ* r2=new ZmodnZ(2,4);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;
		return r;
	}	
	if(i==27)//R8_27:=DirectSum(SmallRing(4,6),SmallRing(2,1));;
	{
		Mnr* r4=new Mnr;
		r4->initF(2);
		ZmodnZ* r2=new ZmodnZ(2,4);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;
		return r;
	}	
	if(i==28)
	{
		Mnr* r=new Mnr();
		MATRIXi8 A(4,vector<TElem>(4,0));
		MATRIXi8 B(4,vector<TElem>(4,0));
		A[0][0]=1;
		A[0][1]=0;
		A[0][2]=1;
		A[0][3]=0;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=0;
		A[1][3]=1; 
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=0;
		A[2][3]=1;
		A[3][0]=1;
		A[3][1]=0;
		A[3][2]=1;
		A[3][3]=0;
		B[0][0]=0;
		B[0][1]=1;
		B[0][2]=0;
		B[0][3]=1;
		B[1][0]=0;
		B[1][1]=1;
		B[1][2]=0;
		B[1][3]=1; 
		B[2][0]=0;
		B[2][1]=1;
		B[2][2]=0;
		B[2][3]=1;
		B[3][0]=0;
		B[3][1]=1;
		B[3][2]=0;
		B[3][3]=1;
		vector<MATRIXi8> gen;
		gen.push_back(A);
		gen.push_back(B);
		r->m_r=new ZmodnZ(1,2);
		r->m_flag=1;
		r->m_n=4;
		r->m_Set=Mnr::FR(r->m_r,gen);   
		return r;
	}
	if(i==29)//R8_29:=DirectSum(SmallRing(4,7),SmallRing(2,1));;
	{
		M2r* r4=new M2r;
		r4->initG(2);
		ZmodnZ* r2=new ZmodnZ(2,4);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;
		return r;
	}	
	if(i==30)
	{
		Mnr* r=new Mnr();
		MATRIXi8 A(4,vector<TElem>(4,0));
		MATRIXi8 B(4,vector<TElem>(4,0));
		MATRIXi8 C(4,vector<TElem>(4,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[0][3]=0;
		A[1][0]=1;
		A[1][1]=1;
		A[1][2]=0;
		A[1][3]=0; 
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=1;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=1;
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[0][3]=0;
		B[1][0]=1;
		B[1][1]=1;
		B[1][2]=0;
		B[1][3]=0; 
		B[2][0]=1;
		B[2][1]=0;
		B[2][2]=1;
		B[2][3]=0;
		B[3][0]=0;
		B[3][1]=0;
		B[3][2]=0;
		B[3][3]=1;
		C[0][0]=0;
		C[0][1]=0;
		C[0][2]=0;
		C[0][3]=0;
		C[1][0]=1;
		C[1][1]=1;
		C[1][2]=0;
		C[1][3]=0; 
		C[2][0]=0;
		C[2][1]=0;
		C[2][2]=1;
		C[2][3]=0;
		C[3][0]=1;
		C[3][1]=0;
		C[3][2]=0;
		C[3][3]=1;
		vector<MATRIXi8> gen;
		gen.push_back(A);
		gen.push_back(B);
		gen.push_back(C);
		r->m_r=new ZmodnZ(1,2);
		r->m_flag=1;
		r->m_n=4;
		r->m_Set=Mnr::FR(r->m_r,gen);   
		return r;
	}
	if(i==31)
	{
		FiniteRing* r=new FiniteRing(8,&g_F8Add[0][0],&g_R8_31Mul[0][0],0);
		return r;
	}
	if(i==32)
	{
		FiniteRing* r=new FiniteRing(8,&g_F8Add[0][0],&g_R8_32Mul[0][0],0);
		return r;
	}
	if(i==33)
	{
		Mnr* r=new Mnr();
		MATRIXi8 A(4,vector<TElem>(4,0));
		A[0][0]=1;
		A[0][1]=0;
		A[0][2]=0;
		A[0][3]=1;
		A[1][0]=1;
		A[1][1]=0;
		A[1][2]=0;
		A[1][3]=0; 
		A[2][0]=1;
		A[2][1]=1;
		A[2][2]=0;
		A[2][3]=1;
		A[3][0]=1;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=1;
		vector<MATRIXi8> gen;
		gen.push_back(A);
		r->m_r=new ZmodnZ(1,2);
		r->m_flag=1;
		r->m_n=4;
		r->m_Set=Mnr::FR(r->m_r,gen);   
		return r;
	}
	if(i==34)//R8_34:=DirectSum(SmallRing(4,8),SmallRing(2,1));;
	{
		M2r* r4=new M2r;
		r4->initH(2);
		ZmodnZ* r2=new ZmodnZ(2,4);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;
		return r;
	}	
	if(i==35)//R8_35:=DirectSum(SmallRing(4,9),SmallRing(2,1));;
	{
		M2r* r4=new M2r;
		r4->initI(2);
		ZmodnZ* r2=new ZmodnZ(2,4);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;
		return r;
	}	
	if(i==36)
	{
		FiniteRing* r=new FiniteRing(8,&g_F8Add[0][0],&g_R8_36Mul[0][0],0);
		return r;
	}
	if(i==37)
	{
		FiniteRing* r=new FiniteRing(8,&g_F8Add[0][0],&g_R8_37Mul[0][0],0);
		return r;
	}
	if(i==38)//R8_38:=DirectSum(SmallRing(4,5),SmallRing(2,2));;
	{
		Mnr* r=new Mnr();
		r->initL(2);
		return r;
	}
	if(i==39)
	{
		FiniteRing* r=new FiniteRing(8,&g_F8Add[0][0],&g_R8_39Mul[0][0],0);
		return r;
	}
	if(i==40)//R8_40:=DirectSum(SmallRing(4,10),SmallRing(2,1));;
	{
		M2r* r4=new M2r;
		r4->initJ(2);
		ZmodnZ* r2=new ZmodnZ(2,4);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;
		return r;
	}
	if(i==41)//R8_41:=DirectSum(SmallRing(4,7),SmallRing(2,2));;
	{
		M2r* r4=new M2r;
		r4->initG(2);
		ZmodnZ* r2=new ZmodnZ(1,2);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;
		return r;
	}	
	if(i==42)//R8_42:=DirectSum(SmallRing(4,11),SmallRing(2,1));;
	{
		M2r* r4=new M2r;
		r4->initK(2);
		ZmodnZ* r2=new ZmodnZ(2,4);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;
		return r;
	}	
	if(i==43)
	{
		Mnr* r=new Mnr();
		MATRIXi8 A(4,vector<TElem>(4,0));
		MATRIXi8 B(4,vector<TElem>(4,0));
		MATRIXi8 C(4,vector<TElem>(4,0));
		A[0][0]=0;
		A[0][1]=1;
		A[0][2]=0;
		A[0][3]=0;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=0;
		A[1][3]=0; 
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=1;
		A[2][3]=0;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=0;
		A[3][3]=1;
		B[0][0]=0;
		B[0][1]=1;
		B[0][2]=1;
		B[0][3]=1;
		B[1][0]=0;
		B[1][1]=1;
		B[1][2]=0;
		B[1][3]=0; 
		B[2][0]=0;
		B[2][1]=0;
		B[2][2]=1;
		B[2][3]=0;
		B[3][0]=0;
		B[3][1]=0;
		B[3][2]=0;
		B[3][3]=1;
		C[0][0]=0;
		C[0][1]=1;
		C[0][2]=1;
		C[0][3]=0;
		C[1][0]=0;
		C[1][1]=1;
		C[1][2]=0;
		C[1][3]=0; 
		C[2][0]=0;
		C[2][1]=0;
		C[2][2]=1;
		C[2][3]=0;
		C[3][0]=0;
		C[3][1]=0;
		C[3][2]=0;
		C[3][3]=1;
		vector<MATRIXi8> gen;
		gen.push_back(A);
		gen.push_back(B);
		gen.push_back(C);
		r->m_r=new ZmodnZ(1,2);
		r->m_flag=1;
		r->m_n=4;
		r->m_Set=Mnr::FR(r->m_r,gen);   
		return r;
	}
	if(i==44)
	{
		Mnr* r=new Mnr();
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=1;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=0; 
		A[2][0]=0;
		A[2][1]=0;
		A[2][2]=0;
		B[0][0]=0;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=1;
		B[1][1]=0;
		B[1][2]=0;
		B[2][0]=1;
		B[2][1]=0;
		B[2][2]=0;
		vector<MATRIXi8> gen;
		gen.push_back(A);
		gen.push_back(B);
		r->m_r=new ZmodnZ(1,2);
		r->m_flag=1;
		r->m_n=3;
		r->m_Set=Mnr::FR(r->m_r,gen);   
		return r;
	}
	if(i==45)
	{
		//不是环FiniteRing* r=new FiniteRing(8,&g_F4F2Add[0][0],&g_R8_C2C2C2_28_2Mul[0][0],0);
		Mnr* r=new Mnr();
		MATRIXi8 A(3,vector<TElem>(3,0));
		MATRIXi8 B(3,vector<TElem>(3,0));
		A[0][0]=0;
		A[0][1]=0;
		A[0][2]=0;
		A[1][0]=1;
		A[1][1]=0;
		A[1][2]=0;
		A[2][0]=1;
		A[2][1]=0;
		A[2][2]=0;
		B[0][0]=1;
		B[0][1]=0;
		B[0][2]=0;
		B[1][0]=1;
		B[1][1]=0;
		B[1][2]=1;
		B[2][0]=1;
		B[2][1]=1;
		B[2][2]=0;  
		vector<MATRIXi8> gen;
		gen.push_back(A);
		gen.push_back(B);		
		r->m_r=new ZmodnZ(1,2);
		r->m_flag=1;
		r->m_n=3;
		r->m_Set=Mnr::FR(r->m_r,gen);   
		return r;
	}
	if(i==46)
	{
		int f[] = {0,0,0,1};
		Polynomial vf(f,f+sizeof(f)/sizeof(f[0]));			
		ZmodnZ *r2_2=new ZmodnZ(1,2);	
		PolynomialRing* r=new PolynomialRing(r2_2,vf);
		r->m_flag=1;
		return r;
	}
	if(i==47)//R8_47:=DirectSum(SmallRing(4,8),SmallRing(2,2));;
	{
		M2r* r4=new M2r;
		r4->initH(2);
		ZmodnZ* r2=new ZmodnZ(1,2);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;
		return r;
	}	
	if(i==48)//R8_48:=DirectSum(SmallRing(4,9),SmallRing(2,2));;R8_48=F_2[x]/(x^3+x^2)=F_2[x]/(x^3+x)
	{
		int f[] = {0,1,0,1};
		Polynomial vf(f,f+sizeof(f)/sizeof(f[0]));			
		ZmodnZ *r2_2=new ZmodnZ(1,2);	
		PolynomialRing* r=new PolynomialRing(r2_2,vf);
		r->m_flag=1;
		return r;
	}
	if(i==49)
	{
		FiniteRing* r=new FiniteRing(8,&g_F4F2Add[0][0],&g_R8_C2C2C2_28_3Mul[0][0],0);
		return r;
	}	
	if(i==50)//R8_50:=DirectSum(SmallRing(4,10),SmallRing(2,2));;
	{
		M2r* r4=new M2r;
		r4->initJ(2);
		ZmodnZ* r2=new ZmodnZ(1,2);
		DecompositionRing* r= new DecompositionRing(r4,r2);
		r->m_flag=1;
		return r;
	}	
	if(i==51)//R8_51:=DirectSum(SmallRing(4,11),SmallRing(2,2));;R8_51=F_2[x]/(x^3+x^2+x)=F_2[x]/(x^3+1)
	{
		FiniteRing* r=new FiniteRing(8,&g_F4F2Add[0][0],&g_F4F2Mul[0][0],0);
		return r;
	}	
	if(i==52)//R8_52=F_2[x]/(x^3+x+1)=F_2[x]/(x^3+x^2+1)
	{
		//不是环FiniteRing* r=new FiniteRing(8,&g_F8Add[0][0],&g_F8Mul[0][0],0);
		Mnr* r=new Mnr();
		MATRIXi8 A(4,vector<TElem>(4,0));
		A[0][0]=0;
		A[0][1]=1;
		A[0][2]=1;
		A[0][3]=0;
		A[1][0]=0;
		A[1][1]=1;
		A[1][2]=1;
		A[1][3]=0; 
		A[2][0]=0;
		A[2][1]=1;
		A[2][2]=0;
		A[2][3]=1;
		A[3][0]=0;
		A[3][1]=0;
		A[3][2]=1;
		A[3][3]=0; 
		vector<MATRIXi8> gen;
		gen.push_back(A);
		r->m_r=new ZmodnZ(1,2);
		r->m_flag=1;
		r->m_n=4;
		r->m_Set=Mnr::FR(r->m_r,gen);   
		return r;
	}	
	return NULL;
}

IRing* newR8R2(int ij)
{
	int i=(ij-1)%52+1;
	int j=(ij-1)/52+1;
    IRing* ri=newR8(i);
	if(!ri)
		return NULL;
    IRing* rj=newR2(j);
	if(!rj){
		delete ri;
		return NULL;
	}
	DecompositionRing* r= new DecompositionRing(ri,rj);
	r->m_flag=1;
	return r;
}

IRing* newR4R4(int ij)
{
	int i=(ij-1)%11+1;
	int j=(ij-1)/11+1;
	if(i>j)
		return NULL;
    IRing* ri=newR4(i);
    IRing* rj=newR4(j);
	DecompositionRing* r= new DecompositionRing(ri,rj);
	r->m_flag=1;
	return r;
}

IRing* newR8R4(int ij)
{
	int i=(ij-1)%52+1;
	int j=(ij-1)/52+1;
    IRing* ri=newR8(i);
	if(!ri)
		return NULL;
    IRing* rj=newR4(j);
	if(!rj){
		delete ri;
		return NULL;
	}
	DecompositionRing* r= new DecompositionRing(ri,rj);
	r->m_flag=1;
	return r;
}

IRing* newR8R8(int ij)
{
	int i=(ij-1)%52+1;
	int j=(ij-1)/52+1;
	if(i>j)
		return NULL;	
    IRing* ri=newR8(i);
	if(!ri)
		return NULL;
    IRing* rj=newR8(j);
	if(!rj){
		delete ri;
		return NULL;
	}
	DecompositionRing* r= new DecompositionRing(ri,rj);
	r->m_flag=1;
	return r;
}

string IMStr(IRing *r,int i)
{
	M2r *r1=dynamic_cast<M2r *>(r);
	if(r1)
		return M2r::MStr(r1->m_Set[i]);
	Mnr *r2=dynamic_cast<Mnr *>(r);
	if(r2)
		return Mnr::MStr(r2->m_Set[i]);
	return "";
}

void checkring(IRing *r,int ID){
	if(r->size()==32||r->size()==64||r->size()==243){
		if(ID>0){
			char sz1[128]={0};   
			sprintf(sz1,"R%d_%d.txt",r->size(),ID);
			writeTable(r,sz1);  
		}else{
			string strR=calcRingInvariant(r);			
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",r->size(),ID,strR.c_str());	
		}			
	}
	if((r->size()==27||r->size()==81) && ID==-1){	
			string strR=calcRingInvariant(r);			
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",r->size(),ID,strR.c_str());	
	}	
}

//int g_i=0;
void findsubring1(IRing *r,int n)
{
	set<pair<int,int>> M;
	set<string> S;	
	int ID=(r->size()>32 && r->size()!=81)?0:IdRing(r);
	//srand(time(NULL));
	//g_i=rand()%r->size();
	//printf("R%d_%d g_i=%d\n",r->size(),ID,g_i);
	checkring(r,ID);
	int i0=0;
#ifdef QF
	i0=1;
#endif	
	for(int i=i0;i<r->size();i++)			
	{
		vector<int> v;
		v.push_back(i);				
		Subring S1i;
		bool bn=S1i.init(r,v,n);
		if(!bn)
			continue;
		int ni=S1i.size();
#ifdef QF
		if(ni<16)
			continue;
#endif
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(ni,ID));
		int cnt1=M.size();
		if(cnt1>cnt){		
			printf("cnt1=%d:R%d_%d->i=%d=>%s\n",cnt1,ni,ID,i,IMStr(r,i).c_str());	
            if((ni==32||ni==81) && ID>0){
				char sz1[128]={0};   
				sprintf(sz1,"R%d_%d.txt",ni,ID);
				writeTable(&S1i,sz1);                  
			}			
		}
		if(ID==-1) 
		{					
			string strR=calcRingInvariant(&S1i);
			if(S.find(strR)==S.end()){		
				printf("i=%d=>%s->R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",i,IMStr(r,i).c_str(),ni,ID,strR.c_str());
			}
			S.insert(strR);	
		}		   
	}
}

void findsubring2(IRing *r,int n)
{
#define PRINT_LOG 0	
	bool bFind=false;	
	int ID=(r->size()>32 && r->size()!=81)?0:IdRing(r);
#if PRINT_LOG
    char sz[100]="0";
	sprintf(sz,"R%d_%d_%d.txt",r->size(),ID,time(NULL));
    ofstream fout(sz);
    string strCmd="del ";
	strCmd+=sz;	
#endif	
	map<pair<int,int>,pair<int,int>> M;	
	set<string> S;		
	//srand(time(NULL));
	//g_i=rand()%r->size();
	printf("R%d_%d\n",r->size(),ID);
	checkring(r,ID);	
	int i0=0;
#ifdef QF
	i0=1;
#endif	
	for(int i=i0;i<r->size()-1;i++)	
	for(int j=i+1;j<r->size();j++)
	{
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);
		Subring S1i;
		bool bn=S1i.init(r,v,n);
		if(!bn)
			continue;
		//Subring S1i(r,v);
		int ni=S1i.size();
#ifdef QF
		if(ni<16)
			continue;
#endif
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){
			string str=IMStr(r,i);
			string strj=IMStr(r,j);				
			printf("cnt1=%d:R%d_%d->i=%d,j=%d=>%s,%s\n",cnt1,ni,ID,i,j,str.c_str(),strj.c_str());
            if((ni==32||ni==81) && ID>0){
				char sz1[128]={0};   
				sprintf(sz1,"R%d_%d.txt",ni,ID);
				writeTable(&S1i,sz1);                  
			}				
		}	
		if(ID==-1)  
		{		
			string strR=calcRingInvariant(&S1i);
			if(S.find(strR)==S.end()){			
				printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",ni,ID,strR.c_str());				
				//S1i.printTable();
#if PRINT_LOG			
				fout<<i<<","<<j<<"=>";
				fout<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2N6="<<strR<<endl;
				bFind=true;
#endif
			}
			S.insert(strR);
			//break;
		}		   
	}
#if PRINT_LOG
	fout.close();	
	if(!bFind)	
		system(strCmd.c_str());
	else
		printf("子环表示已输出到文件%s\n",sz);
#endif	
}

void findsubring3(IRing *r,int n)
{
#define PRINT_LOG 0	
	bool bFind=false;	
#if PRINT_LOG
    char sz[100]="0";
	sprintf(sz,"R%d_%d.txt",r->size(),time(NULL));
    ofstream fout(sz);
    string strCmd="del ";
	strCmd+=sz;	
#endif	
	map<pair<int,int>,pair<int,int>> M;	
	set<string> S;	
	int ID=(r->size()>32 && r->size()!=81)?0:IdRing(r);
	printf("R%d_%d\n",r->size(),ID);
	checkring(r,ID);	
	int i0=0;
#ifdef QF
	i0=1;
#endif	
	for(int i=i0;i<r->size()-2;i++)	
	for(int j=i+1;j<r->size()-1;j++)
	for(int k=j+1;k<r->size();k++)		
	{
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);	
		v.push_back(k);			
		Subring S1i;
		bool bn=S1i.init(r,v,n);
		if(!bn)
			continue;
		int ni=S1i.size();
#ifdef QF
		if(ni<16)
			continue;
#endif
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){			
			string str=IMStr(r,i);
			string strj=IMStr(r,j);
			string strk=IMStr(r,k);				
			printf("cnt1=%d:R%d_%d->i=%d,j=%d,k=%d=>%s,%s,%s\n",cnt1,ni,ID,i,j,k,str.c_str(),strj.c_str(),strk.c_str());			
            if((ni==32||ni==81) && ID>0){
				char sz1[128]={0};   
				sprintf(sz1,"R%d_%d.txt",ni,ID);
				writeTable(&S1i,sz1);                  
			}			
		}	
		if(ID==-1)   
		{					
			string strR=calcRingInvariant(&S1i);
			if(S.find(strR)==S.end()){				
				printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",ni,ID,strR.c_str());				
#if 0//PRINT_LOG			
				fout<<i<<","<<j<<","<<k<<"=>";
				fout<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2N6="<<strR<<endl;
				bFind=true;
#endif
		}
			S.insert(strR);
			//break;
		}		   
	}
#if PRINT_LOG
	fout.close();	
	if(!bFind)	
		system(strCmd.c_str());
	else
		printf("子环表示已输出到文件%s\n",sz);
#endif	
}

void findsubring4(IRing *r,int n)
{
#define PRINT_LOG 0	
	bool bFind=false;	
#if PRINT_LOG
    char sz[100]="0";
	sprintf(sz,"R%d_%d.txt",r->size(),time(NULL));
    ofstream fout(sz);
    string strCmd="del ";
	strCmd+=sz;	
#endif	
	map<pair<int,int>,pair<int,int>> M;	
	set<string> S;		
	int ID=(r->size()>32 && r->size()!=81)?0:IdRing(r);
	printf("R%d_%d\n",r->size(),ID);
	checkring(r,ID);
	int i0=0;
#ifdef QF
	i0=1;
#endif	
	for(int t=i0;t<r->size()-3;t++)
	for(int i=t+1;i<r->size()-2;i++)		
	for(int j=i+1;j<r->size()-1;j++)
	for(int k=j+1;k<r->size();k++)		
	{
		vector<int> v;
		v.push_back(t);			
		v.push_back(i);		
		v.push_back(j);	
		v.push_back(k);			
		Subring S1i;
		bool bn=S1i.init(r,v,n);
		if(!bn)
			continue;
		int ni=S1i.size();
#ifdef QF
		if(ni<16)
			continue;
#endif
		int ID=IdRing(&S1i);
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){
#if 1
			printf("cnt1=%d:R%d_%d->t=%d,i=%d,j=%d,k=%d\n",cnt1,ni,ID,t,i,j,k);	
#else
			string strt=IMStr(r,t);		
			string str=IMStr(r,i);
			string strj=IMStr(r,j);
			string strk=IMStr(r,k);		
			printf("cnt1=%d:R%d_%d->t=%d,i=%d,j=%d,k=%d=>%s,%s,%s,%s\n",cnt1,ni,ID,t,i,j,k,strt.c_str(),str.c_str(),strj.c_str(),strk.c_str());	
#endif
            if((ni==32||ni==81) && ID>0){
				char sz1[128]={0};   
				sprintf(sz1,"R%d_%d.txt",ni,ID);
				writeTable(&S1i,sz1);                  
			}			
		}	
		if(ID==-1)   
		{					
			string strR=calcRingInvariant(&S1i);
			if(S.find(strR)==S.end()){			
				printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",ni,ID,strR.c_str());				
#if 0//PRINT_LOG			
				fout<<t<<","<<i<<","<<j<<","<<k<<"=>";
				fout<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2N6="<<strR<<endl;
				bFind=true;
#endif
		}
			S.insert(strR);
			//break;
		}		   
	}
#if PRINT_LOG
	fout.close();	
	if(!bFind)	
		system(strCmd.c_str());
	else
		printf("子环表示已输出到文件%s\n",sz);
#endif	
}

void findquotientring(IRing *r,int n)
{
#define PRINT_LOG 0	
	bool bFind=false;	
	int ID=(r->size()>32 && r->size()!=81 && r->size()!=64 && r->size()!=243)?0:IdRing(r);
	printf("R%d_%d\n",r->size(),ID);
	checkring(r,ID);	
#if PRINT_LOG
    char sz[100]="0";
	sprintf(sz,"R%d_%d_%d.txt",r->size(),ID,time(NULL));
    ofstream fout(sz);
    string strCmd="del ";
	strCmd+=sz;	
#endif	
	map<pair<int,int>,pair<int,int>> M;	
	set<string> S;	
	for(int i=0;i<r->size()-1;i++)		
    //int i=0;
	for(int j=i+1;j<r->size();j++)
	{
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);
		Subring S1i0;
		bool bn=S1i0.init(r,v,r->size()/n);
		if(!bn)
			continue;
		if(S1i0.m_Set.size()!=r->size()/n)
			continue;
		vector<int> v0=v;
		v=S1i0.m_Set;
		int iret1=IsIdeal(r,v); 
		if(iret1!=1)
			continue;
		quotientRing S1i(r,v);
		int ni=S1i.size();	
		int IDr=ID;
		bool b=IsRing(&S1i);
		if(!b){
			continue;
		}			
		int ID=IdRing(&S1i);	
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){		
			int IDr0=IdRing(&S1i0);
			printf("cnt1=%d:R%d_%d/R%d_%d=R%d_%d->i=%d,j=%d\n",cnt1,r->size(),IDr,S1i0.size(),IDr0,ni,ID,i,j);			
			static int cnt=sizeof(IDs)/sizeof(IDs[0]);
		    static vector<int> vIDs(IDs,IDs+cnt);
			vector<int>::iterator p1=std::find(vIDs.begin(),vIDs.end(),ID);
			if((ni==32||ni==27) && ID==-1 || (ni==16 && std::find(vIDs.begin(),vIDs.end(),ID)!=vIDs.end())){
				printRing0(&S1i,ID);
			}	
            if((ni==32||ni==81) && ID>0){
				char sz1[128]={0};   
				sprintf(sz1,"R%d_%d.txt",ni,ID);
				writeTable(&S1i,sz1);                  
			}			
		}	
		if(ID==-1) 	
		{		
			string strR=calcRingInvariant(&S1i);
			if(S.find(strR)==S.end()){				
				printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",ni,ID,strR.c_str());				
				//S1i.printTable();
#if PRINT_LOG			
				fout<<i<<","<<j<<"=>";
				fout<<"R"<<ni<<"_"<<ID<<":N0n0bAbOn1n2n4n5n6n7n8S1N2N6="<<strR<<endl;
				bFind=true;
#endif
		}
			S.insert(strR);
			//break;
		}
	}
#if PRINT_LOG
	fout.close();	
	if(!bFind)	
		system(strCmd.c_str());
	else
		printf("商环表示已输出到文件%s\n",sz);
#endif	
}

void findquotientring3(IRing *r,int n)
{
	int ID=(r->size()>32 && r->size()!=81 && r->size()!=64 && r->size()!=243)?0:IdRing(r);
	printf("R%d_%d\n",r->size(),ID);
	checkring(r,ID);	
	map<pair<int,int>,pair<int,int>> M;
	set<string> S;	
	for(int i=0;i<r->size()-2;i++)		
	for(int j=i+1;j<r->size()-1;j++)
	for(int k=j+1;k<r->size();k++)		
	{
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);
		v.push_back(k);		
		Subring S1i0;
		bool bn=S1i0.init(r,v,r->size()/n);
		if(!bn)
			continue;
		if(S1i0.m_Set.size()!=r->size()/n||S1i0.m_Set.size()==1||S1i0.m_Set.size()==r->size())
			continue;
		vector<int> v0=v;
		v=S1i0.m_Set;
		int iret1=IsIdeal(r,v); 
		if(iret1!=1)
			continue;
		quotientRing S1i(r,v);
		int ni=S1i.size();	
		int IDr=ID;
		bool b=IsRing(&S1i);
		if(!b){
			continue;
		}			
		int ID=IdRing(&S1i);	
		int cnt=M.size();
		M.insert(make_pair(make_pair(ni,ID),make_pair(i,j)));
		int cnt1=M.size();
		if(cnt1>cnt){		
			int IDr0=IdRing(&S1i0);
			printf("cnt1=%d:R%d_%d/R%d_%d=R%d_%d->i=%d,j=%d,k=%d\n",cnt1,r->size(),IDr,S1i0.size(),IDr0,ni,ID,i,j,k);			
			if(ni==n && ID==-1){
				printRing0(&S1i,ID);
				char sz[100]="0";
				sprintf(sz,"R%d_%d.txt",ni,time(NULL));	
				writeTable(&S1i,sz);
			}				
		}	
		if(ni==n && ID==-1) 	
		{		
			string strR=calcRingInvariant(&S1i);
			if(S.find(strR)==S.end()){				
				printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",ni,ID,strR.c_str());				
				//break;
			}
			S.insert(strR);
		}
	}
}

#endif
