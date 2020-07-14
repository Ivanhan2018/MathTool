#include <algorithm>
#include <utility>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

//ifstream fin("G72.txt");
//ofstream fout("G72out.txt");

int main(int argc, char * argv[])
{
	char sz[100]={0};
	if(argc<3)
	{
		printf("Usage:  G72 fin fout\n");
		return 0;
	}
	ifstream fin(argv[1]);
	ofstream fout(argv[2]);
        if(!fin){
		printf("文件%s不存在\n",argv[1]);
		return 0;               
	}
 	char szline[1000]={0};
 	while (fin.getline(szline,1000)){
	int n=0;
	int id=0;
	char szN0[1000]={0};
	sscanf(szline,"[%d,%d]%s",&n,&id,szN0);
	int len=strlen(szN0);
	if(szN0[len-1]==',')
		szN0[len-1]=0;
	char sz[1000]={0};
	sprintf(sz,"m_N0.insert(make_pair(\"%s\",%d));",szN0,id);
	fout<<sz<<endl;
 }
#ifdef _DEBUG
 system("pause");
#endif
 return 0;
}