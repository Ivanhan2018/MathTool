#include"IRing.h"
#include<ctime>
#include"quotientring.h"
#include<fstream>
#include<set>

int g_a=16;
int g_b=0;
int g_c=0;
string g_str="";

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

vector<int> SubRings(IRing* r,int s,vector<int>& v,std::vector<std::vector<int> > &sets,std::vector<int> &J){
	vector<int> vSet;				
	Subring S1i0;
	bool b=S1i0.init(r,v,s);
	if(!b)
	   return vSet;
	if(S1i0.m_Set.size()!=s)
		return vSet;	
	vSet=S1i0.m_Set;
	std::sort(vSet.begin(),vSet.end());
	std::vector<std::vector<int> >::iterator p=std::find(sets.begin(),sets.end(),vSet);
	if(p!=sets.end()){
		return vSet;
	}	
	int iret1=IsIdeal(r,vSet);
	if(iret1==0)
		return vSet; 	
	int ID=IdRing(&S1i0);	
	if(ID<1){
		string strR=calcRingInvariant(&S1i0);			
		printf("子环R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",s,ID,strR.c_str());	
		if(ID<0 && s<32)
			printRing0(&S1i0,ID);		
	}	
	string str=V2S(v)+"=>"+V2S(vSet)+"=R"+itos(s)+"_"+itos(ID);	
	if(iret1==1){
		if(J.size()==0){
			J=vSet;
		}else{
			vector<int> J1;
			set_intersection(J.begin(),J.end(),vSet.begin(),vSet.end(),back_inserter(J1));
			J=J1;
		}
		str+="是理想";
		quotientRing S1i(r,vSet);
		int ni=S1i.size();	
		bool b=IsRing(&S1i);
		if(b){
			int IDi=IdRing(&S1i);
			if(IDi<1){
				string strR=calcRingInvariant(&S1i);			
				printf("商环R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",ni,IDi,strR.c_str());	
				if(IDi<0 && ni<32)
					printRing0(&S1i,IDi);				
			}		
			str+=",商环R"+itos(ni)+"_"+itos(IDi);	
		}	
	}
	//if(iret1==2)str+="不是理想";
	else if(iret1==2){
		int iret1L=IsLeftIdeal(r,vSet);
		int iret1R=IsRightIdeal(r,vSet);			
		if(iret1L==2)str+="不是左理想";else {
			str+="是左理想";
			if(J.size()==0){
				J=vSet;
			}else{
				vector<int> J1;
				set_intersection(J.begin(),J.end(),vSet.begin(),vSet.end(),back_inserter(J1));
				J=J1;
			}			
		}
		str+=",";
		if(iret1R==2)str+="不是右理想";else str+="是右理想";			
	}	
	printf("%s\n",str.c_str()); 
	sets.push_back(vSet);
	return vSet;
}

//从 indexs 集合中选择 num 个元素进行组合并保证返回的组合中没有重复的元素
void combination(IRing* r,int s,int num)
{
    static int s_i=0;
    static time_t s_t=0;
	std::vector<std::vector<int> > sets;
	std::vector<int> J;	
	std::vector<int> indexs;
	int n=r->size();	
	for (int i = 0; i < n; i++)
		indexs.push_back(i);
	if (num > indexs.size())return;

	std::vector<int> elements;
	elements.resize(indexs.size());
	for (int i = 0; i < num; i++)
	{
		elements.at(i) = 1;
	}

	do
	{
		s_i=(s_i+1)%1000;
		std::vector<int> currentCombination;
		for (size_t i = 0; i < elements.size(); ++i)
		{
			if (elements[i])
			{
				currentCombination.push_back(indexs[i]);
			}
		}
		int n1=sets.size();			
		SubRings(r,s,currentCombination,sets,J);
		int n2=sets.size();
		if(g_c>0){
			if(n2>n1){
				s_t=time(NULL);				
			}else{
				if(s_t>0 && s_i==0 && time(NULL)-s_t>g_c*60){
					printf("超过%d分钟，终止计算！\n",g_c);
					return;
				}
			}
		}		
	} while (prev_permutation(elements.begin(), elements.end()));
}

bool IsPIdem(IRing* r,vector<int>& J,int a,int i1){
	int n=r->size();
	for(int i=0;i<n;i++){	
		int ca=r->mul(i,a);
		vector<int>::iterator p=std::find(J.begin(),J.end(),ca);
		if(p==J.end()){
			return false;;
		}   
	}	
	return true;
}

void combination(IRing* r,int s)
{
    static int s_i=0;
    static time_t s_t=0;
	std::vector<std::vector<int> > sets;
	std::vector<int> J;
	std::vector<int> indexs;
	int n=r->size();	
	for (int i = 0; i < n; i++)
		indexs.push_back(i);
	
	for(int num=2;num<=s;num++){
		if (num > indexs.size())return;

		std::vector<int> elements;
		elements.resize(indexs.size());
		for (int i = 0; i < num; i++)
		{
			elements.at(i) = 1;
		}

		do
		{
			s_i=(s_i+1)%1000;	
			std::vector<int> currentCombination;
			for (size_t i = 0; i < elements.size(); ++i)
			{
				if (elements[i])
				{
					currentCombination.push_back(indexs[i]);
				}
			}
			int n1=sets.size();			
			SubRings(r,s,currentCombination,sets,J);
			int n2=sets.size();
			if(g_c>0){
				if(n2>n1){
					s_t=time(NULL);				
				}else{
					if(s_t>0 && s_i==0 && time(NULL)-s_t>g_c*60){
						printf("超过%d分钟，终止计算！\n",g_c);
						return;
					}
				}
			}			
		} while (prev_permutation(elements.begin(), elements.end()));
	}
	int p=2;
	for(;p<=n;p++){
		if(n%p==0)
			break;
	}
	if(n==s*p){
		vector<int> Idem=calIdem(r);
		int n2=Idem.size();	
		printf("Jacobson根:%s,幂等元集合:%s\n",V2S(J).c_str(),V2S(Idem).c_str());			
		int i1=One(r);
		if(i1>-1){
			vector<int> PIdem;
			for(int i=1;i<n2 && i!=i1;i++){
				int e=Idem[i];
				vector<int> S=Order(r,e);			
				int e1=S[S.size()-1];		 
				int a=r->add(i1,e1);	
				bool b=IsPIdem(r,J,a,i1);
				if(b)PIdem.push_back(e);
			}		
			printf("幺环,非平凡主幂等元集合:%s\n",V2S(PIdem).c_str());			
		}	
	}
}

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
#ifdef SR_
	vector<int> v;
	if(g_str!="")
	{
		vector<string> All=split(g_str,",");
		for(int i=0;i<All.size();i++)
		{
			int iElem=atoi(All[i].c_str());
			v.push_back(iElem);
		}
	}
	if(v.size()>0)
	{
		vector<int> vSet;
		IRing *r=this;
		int s=this->size();	
		Subring S1i0;
		bool b=S1i0.init(r,v,s);
		if(!b)
		   return;	
		vSet=S1i0.m_Set;
		std::sort(vSet.begin(),vSet.end());	
		int iret1=IsIdeal(r,vSet);
		if(iret1==0)
			return;  		
		int ni=S1i0.size();	
		if(ni==this->size())
			return;		
		int ID=IdRing(&S1i0);
		char sz[100]="0";
		sprintf(sz,"R%d_%d.txt",ni,ID);	
		writeTable(&S1i0,sz);
		string strR=calcRingInvariant(&S1i0);			
		printf("子环R%d_%d:N0n0bAbOn1n2n4n5n6n7n8S1N2N6=%s\n",ni,ID,strR.c_str());	
		string str=V2S(v)+"=>"+V2S(vSet)+"=R"+itos(ni)+"_"+itos(ID);	
		if(iret1==1)str+="是理想";		
		//else if(iret1==2)str+="不是理想";
		else if(iret1==2){
			int iret1L=IsLeftIdeal(r,vSet);
			int iret1R=IsRightIdeal(r,vSet);			
			if(iret1L==2)str+="不是左理想";else str+="是左理想";
			str+=",";
			if(iret1R==2)str+="不是右理想";else str+="是右理想";			
		}
		printf("%s\n",str.c_str());		
	}	
#else
	if(g_b>0)
		combination(this,g_a,g_b);
	else
		combination(this,g_a);
#endif
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
		g_a=atoi(argv[2]);
		g_str=argv[2];		
	}
    if(argc>3){
		g_b=atoi(argv[3]);
	}
    if(argc>4){
		g_c=atoi(argv[4]);
	}	

	vector<char> A=lof2(sz);
	string strA=CharArrToStr(CharArrToNormal(A));
	vector<int> vA=atoV1(strA.c_str(),2,0);
	vector<int> vM=atoV1(strA.c_str(),2,1);

	int n=IsLegalTable(vA);
	if(n==0)
		return 0;

	FiniteRing r(n,&vA[0],&vM[0],1);
	r.printTable();

	//system("pause");
	return 0;
}
