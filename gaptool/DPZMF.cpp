#include "RingUtil.h"
#include <cstdio>
#include <set>
using namespace std;

int main(int argc, char* argv[])
{
	char sz[100]={0};
	if(argc<2)
	{
		printf("请输入AbelianInvariants(eg:M2,M2,Z2)：");
		scanf("%s",sz);
	}
	else
		strcpy(sz,argv[1]);

	vector<string> vMDP=RingUtil::split(sz,",");
	int MDP=vMDP.size();
	for(int i=0;i<MDP;i++)
	{
            char szcmd[100]={0};
            if(vMDP[i][0]=='Z')
               sprintf(szcmd,"./Zn %s",vMDP[i].substr(1,vMDP[i].size()-1).c_str());
            else if(vMDP[i][0]=='M')
               sprintf(szcmd,"./Mn %s",vMDP[i].substr(1,vMDP[i].size()-1).c_str()); 
            else if(vMDP[i][0]=='F')
               sprintf(szcmd,"./Fn %s",vMDP[i].substr(1,vMDP[i].size()-1).c_str()); 
            else if(vMDP[i][0]=='R')
            {
               int n=0,Id=0;
               sscanf(vMDP[i].c_str(),"R%d_%d",&n,&Id);
               sprintf(szcmd,"./SmallRing %d %d",n,Id); 
            }        
	    system(szcmd);             
	}
        string strfn;
        if(MDP>0)
        {
            char szfn[100]={0};
            sprintf(szfn,"%s",vMDP[0].c_str());
            strfn=szfn;
        }
        set<string> vtmp;// 临时输出文件，不含.txt
	for(int i=1;i<MDP;i++)
	{
            char szfn[100]={0};
            sprintf(szfn,"%s%s",strfn.c_str(),vMDP[i].c_str());
                        
            char szcmd[100]={0};
            sprintf(szcmd,"./DPR %s.txt %s.txt %s.txt",strfn.c_str(),vMDP[i].c_str(),szfn);
	    //printf("%s\n",szcmd);
	    system(szcmd);

            vtmp.insert(strfn); 
            vtmp.insert(vMDP[i]);

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


