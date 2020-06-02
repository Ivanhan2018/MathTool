#include <algorithm>
#include <utility>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

ifstream fin("G72.txt");
ofstream fout("G72out.txt");

int main(){
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
 system("pause");
 return 0;
}