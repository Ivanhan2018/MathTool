#include "GroupUtil.h"
#include <cstdio>
#include <set>
using namespace std;

int main(int argc, char* argv[])
{
	char sz[100]={0};
	if(argc<2)
	{
		printf("请输入AbelianInvariants：");
		scanf("%s",sz);
	}
	else
		strcpy(sz,argv[1]);

	vector<string> vMDP=GroupUtil::split(sz,",");
	int MDP=vMDP.size();
	for(int i=0;i<MDP;i++)
	{
            char szcmd[100]={0};
            sprintf(szcmd,"./Cn %s",vMDP[i].c_str());
	    system(szcmd);             
	}
        string strfn;
        if(MDP>0)
        {
            char szfn[100]={0};
            sprintf(szfn,"C%s",vMDP[0].c_str());
            strfn=szfn;
        }
        set<string> vtmp;// 临时输出文件，不含.txt
	for(int i=1;i<MDP;i++)
	{
            char szfn[100]={0};
            sprintf(szfn,"%sC%s",strfn.c_str(),vMDP[i].c_str());
                        
            char szcmd[100]={0};
            sprintf(szcmd,"./DP %s.txt C%s.txt %s.txt",strfn.c_str(),vMDP[i].c_str(),szfn);
	    system(szcmd);

            vtmp.insert(strfn); 
            vtmp.insert("C"+vMDP[i]);

            strfn=szfn;    
	}

        for (set<string>::iterator si=vtmp.begin(); si!=vtmp.end(); si++) 
        { 
	    char szcmd1[100]={0};
	    sprintf(szcmd1,"rm %s.txt",(*si).c_str());
	    system(szcmd1);
        }

	//system("pause");
	return 0;
}


