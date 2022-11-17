#include"IRing.h"
#include<ctime>
int g_c=1;

#ifdef QR16
#include"quotientring.h"
#include<fstream>
#include<set>

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

int g_b=0;
string g_str="";
void checkring(IRing *r,int ID){
	if(r->size()==32||r->size()==64||r->size()==81||r->size()==243){
		if(ID<1){
			string strR=calcRingInvariant(r);			
			printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6N5=%s\n",r->size(),ID,strR.c_str());	
		}else{
			if(g_b!=0 && g_str!=""){
				char sz[1000]={0};				
				const char *sz1=(g_b==1?"_":"");
				char sz0[500]={0};
				sprintf(sz0,"R%d%s%d.txt",r->size(),sz1,ID);				
				//sprintf(sz,"rename %s R%d%s%d.txt",g_str.c_str(),r->size(),sz1,ID);
				if(g_str.find(sz0,0)==string::npos){
					sprintf(sz,"move /Y %s R%d%s%d.txt",g_str.c_str(),r->size(),sz1,ID);
				}
				system(sz);				
			}
		}			
	}	
}

int g_a=16;
void findquotientring(IRing *r,int n)
{
	int ID=IdRing(r);
	printf("R%d_%d\n",r->size(),ID);
#ifdef RI
	checkring(r,ID);
#endif	
	map<pair<int,int>,pair<int,int>> M;
	set<string> S;	
	for(int i=0;i<r->size()-1;i++)		
	for(int j=i+1;j<r->size();j++)
	{
		vector<int> v;
		v.push_back(i);		
		v.push_back(j);
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
			printf("cnt1=%d:R%d_%d/R%d_%d=R%d_%d->i=%d,j=%d\n",cnt1,r->size(),IDr,S1i0.size(),IDr0,ni,ID,i,j);			
			if(ni<32 && ID==-1)
				printRing0(&S1i,ID);
			if(ni>=81 && ID>0){
				char sz[100]="0";
				sprintf(sz,"R%d_%d.txt",ni,ID);	
				writeTable(&S1i,sz);
			}				
		}	
		if(ID==-1) 	
		{		
			string strR=calcRingInvariant(&S1i);
			if(S.find(strR)==S.end()){				
				printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6N5=%s\n",ni,ID,strR.c_str());				
				//break;
			}
			S.insert(strR);
		}
	}
}

void findquotientring3(IRing *r,int n)
{
	int ID=IdRing(r);
	printf("R%d_%d\n",r->size(),ID);	
#ifdef RI
	checkring(r,ID);
#endif	
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
			if(ni<32 && ID==-1)
				printRing0(&S1i,ID);
			if(ni>=81 && ID>0){
				char sz[100]="0";
				sprintf(sz,"R%d_%d.txt",ni,ID);	
				writeTable(&S1i,sz);
			}				
		}	
		if(ID==-1) 	
		{		
			string strR=calcRingInvariant(&S1i);
			if(S.find(strR)==S.end()){				
				printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6N5=%s\n",ni,ID,strR.c_str());				
				//break;
			}
			S.insert(strR);
		}
	}
}
#endif

// 根据环的加法、乘法凯莱表分析其结构的小工具IdRing.exe
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
	// 成员变量  
	int m_n; 
	int* m_Add;
	int* m_Mul; 
	int m_delt; 
};

void FiniteRing::printTable()
{
	bool b1=IsRing(this);
	if(!b1){
		printf("不是环\n");
		return;
	}	
#ifdef QR16
	if(m_n==128||m_n==256){
		findquotientring3(this,g_a);
	}else{
		findquotientring(this,g_a);		
	}
#else
	int ID=IdRing(this);
#ifdef N89
	string b8N8N9=calcb8N8N9(this);
	printf("%d,%s\n",ID,b8N8N9.c_str());
#elif defined(Q1_)
	string Q1=calcQ1(this);
	printf("%d,%s\n",ID,Q1.c_str());	
#elif defined(S2_)
	string S2=calcS2(this);
	printf("%d,%s\n",ID,S2.c_str());
#elif defined(_m0)
	int m0=calcm0(this);
	printf("%d,%d\n",ID,m0);	
#elif defined(OLR)
	int bO=OneExNum(this,1);
	int bL=OneExNum(this,2);
	int bR=OneExNum(this,3);	
	printf("%d,%d,%d,%d\n",ID,bO,bL,bR);
#elif defined(LR)
	int bL=OneExNum(this,2);
	int bR=OneExNum(this,3);	
	printf("%d,%d,%d\n",ID,bL,bR);			
#elif defined(RK)
	int rk=Rank(this);
	printf("%d,%d\n",ID,rk);	
#elif defined(RK5)
	int rk=Rank5(this);
	printf("%d,%d\n",ID,rk);	
#elif defined(SR_)
	srand(time(NULL));
	printf("%d",ID);	
	for(int i=0;i<g_c;i++){
		string SR=RandSubRing(this);
		printf(",%s",SR.c_str());
	}	
#elif defined(_I1I2)
	string I1=calcI1(this);
	string I2=calcI2(this);   
	printf("%d,%s,%s\n",ID,I1.c_str(),I2.c_str());
#elif defined(RI)
	string str=calcRingInvariant(this);  
	printf("%d,%s\n",ID,str.c_str());	
#elif defined(RI2)
	string str=calcRingInvariant2(this);  
	printf("%d,%s\n",ID,str.c_str());	
#elif defined(H1_)
	string H1=calcH1(this);
	printf("%d,%s\n",ID,H1.c_str());
#elif defined(H2_)
	string H2=calcH2(this);
	printf("%d,%s\n",ID,H2.c_str());
#elif defined(N1_)
	string N1=calcN1(this);
	printf("%d,%s\n",ID,N1.c_str());
#elif defined(bN_)
	int bN=IsNilpotent(this);
	printf("%d,%d\n",ID,bN);	
#else
	string str=calcRingInvariant(this);
	printf("R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6N5=%s\n",size(),ID,str.c_str());
	string Q1=calcQ1(this);
	printf("Q1=%s\n",Q1.c_str());
	int m0=calcm0(this);
	printf("m0=%d\n",m0);	
	string str2=calcRingInvariant2(this);  
	printf("RI2=%s\n",str2.c_str());	
	string str3=calcRingInvariant3(this);  
	printf("RI3=%s\n",str3.c_str());	
	int bN=IsNilpotent(this);
	printf("bN=%d\n",bN);		
	int rk=Rank(this);
	printf("r=%d\n",rk);
	string b8N8N9=calcb8N8N9(this);
	printf("b8N8N9=%s\n",b8N8N9.c_str());	
	string I1=calcI1(this);
	string I2=calcI2(this);   
	printf("I1I2=%s,%s\n",I1.c_str(),I2.c_str());
	if(m_n<243){		
		string I3=calcI3(this);
		printf("I3=%s\n",I3.c_str());
	}	
	if(m_n<64){	
		string I4=calcI4(this);
		printf("I4=%s\n",I4.c_str());
	}
#endif	
#endif	
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

/*
A与B的直积
(I,J)的编号为n*(I-1)+(J-1)+1，这里1<=I<=nA，1<=J<=nB
直积的乘法表为：(x1,y1)×(x2,y2)=(x1*x2,y1+y2)，显然幺元是(1,1)=1
*/
bool IsInnerDirectProduct(IRing* r,const vector<int>& A,const vector<int>& B,const vector<int>& IDs)
{
	int n=r->size();
	int nA=A.size();
	int nB=B.size();
	if(nA*nB!=n||IDs.size()!=n)
		return false;
		
	map<pair<int,int>,int> mT;
	vector<pair<int,int> > vT(n);
	vector<int> vA(n*n);
	vector<int> vM(n*n);
	for(int i=0;i<nA;i++)
	for(int j=0;j<nB;j++)
	{
        int k=nB*i+j;
		k=IDs[k];
		vT[k]=make_pair(A[i],B[j]);
		mT.insert(make_pair(make_pair(A[i],B[j]),k));
	}	
	for(int i=0;i<n;i++)
	for(int j=0;j<n;j++)
	{
		pair<int,int> a=vT[i];
		pair<int,int> b=vT[j];
		pair<int,int> s=make_pair(r->add(a.first,b.first),r->add(a.second,b.second)); 
		pair<int,int> m=make_pair(r->mul(a.first,b.first),r->mul(a.second,b.second));
		vA[n*i+j]=mT[s];
		vM[n*i+j]=mT[m];	
		if(vA[n*i+j]!=r->add(i,j))
			return false;
		if(vM[n*i+j]!=r->mul(i,j))
			return false;
	}
	printf("内直积:\n");	
	for(int i=0;i<n;i++)
	{
		pair<int,int> a=vT[i];
		printf("i=%d=>(%d,%d)\n",i,a.first,a.second);		
	}
    return true;
}

int main(int argc, char **argv)
{
	char sz[100]={0};
	if(argc<2)
	{
		printf("请输入有限环的加法、乘法凯莱表文件名：");
		scanf("%s",sz);
	}
	else
		strcpy(sz,argv[1]);
	
    if(argc>2){
		g_c=atoi(argv[2]);
	}		
#ifdef QR16
    if(argc>2){
		int _n0=atoi(argv[2]);	
		if(_n0==27)
			g_a=27;
		else if(_n0==81)
			g_a=81;	
		else if(_n0==243)
			g_a=243;
		else if(_n0==8)
			g_a=8;	
		else if(_n0==4)
			g_a=4;		
		else if(_n0==9)
			g_a=9;	
		else if(_n0==3)
			g_a=3;		
		else
			g_a=32;		
	}
    if(argc>3){
		g_b=atoi(argv[3]);
		g_str=sz;
	}
#elif defined(IDP_)
	char sz2[100]={0};
	char sz3[100]={0};
	if(argc<3)
	{
		printf("请输入理想1：");
		scanf("%s",sz2);
	}
	else
		strcpy(sz2,argv[2]);
	if(argc<4)
	{
		printf("请输入理想2：");
		scanf("%s",sz3);
	}
	else
		strcpy(sz3,argv[3]);		
#endif

	vector<char> A=lof2(sz);
	string strA=CharArrToStr(CharArrToNormal(A));
	vector<int> vA=atoV1(strA.c_str(),2,0);
	vector<int> vM=atoV1(strA.c_str(),2,1);

	int n=IsLegalTable(vA);
	if(n==0)
		return 0;

	FiniteRing r(n,&vA[0],&vM[0],1);
	r.printTable();
#if defined(IDP_)	
	bool b1=IsRing(&r);
	if(!b1){
		printf("不是环\n");
		return 0;
	}
	int m=r.size();
	vector<int> IDs(m);
	for(int i=0;i<m;i++)
		IDs[i]=i;
	if(argc>4)
	{
		vector<string> v=split(argv[4],",");
		if(v.size()!=m)
			return 0;
		for(int i=0;i<m;i++)
			IDs[i]=atoi(v[i].c_str());
	}	
	vector<int> v1;
	vector<int> v2;
	vector<string> vstrA=split(sz2,",");
	for(int i=0;i<vstrA.size();i++)
	{
		int iElem=atoi(vstrA[i].c_str());
		v1.push_back(iElem);
	}	
	vector<string> vstrB=split(sz3,",");
	for(int i=0;i<vstrB.size();i++)
	{
		int iElem=atoi(vstrB[i].c_str());
		v2.push_back(iElem);
	}
	if(argc<=4){
		std::vector<int> indexs;	
		for (int i = 0; i < m; i++)
			indexs.push_back(i);
		bool bI=false;	
		do
		{
			bI=IsInnerDirectProduct(&r,v1,v2,indexs);
			if(bI){
				string str=V2S(indexs);			
				printf("第4个参数:%s,是可分解环。\n",str.c_str());	
				break;
			}				
		} while (next_permutation(indexs.begin(), indexs.end()));		
	}else{
		bool bI=IsInnerDirectProduct(&r,v1,v2,IDs);
		if(bI)printf("是可分解环。\n");		
	}
#endif

	//system("pause");
	return 0;
}
