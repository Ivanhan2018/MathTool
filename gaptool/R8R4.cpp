#include <algorithm>
#include <utility>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

ifstream fin("R32.txt");
ofstream fout("R32out.txt");

int R32N0idx(string& str){
	 if(str.find("[1,1,2,4,8,16]",0)!=string::npos)
	   return 0;
	 if(str.find("[1,3,4,8,16,0]",0)!=string::npos)
	   return 1;
	 if(str.find("[1,3,12,16,0,0]",0)!=string::npos)
	   return 2;
	 if(str.find("[1,7,8,16,0,0]",0)!=string::npos)
	   return 3;
	 if(str.find("[1,7,24,0,0,0]",0)!=string::npos)
	   return 4;
	 if(str.find("[1,15,16,0,0,0]",0)!=string::npos)
	   return 5;
	 if(str.find("[1,31,0,0,0,0]",0)!=string::npos)
	   return 6;
	 return -1;
}

int R32id(string& str,int i){
	 if(str.find("[1,1,2,4,8,16]",0)!=string::npos)
	   return 1+i;
	 if(str.find("[1,3,4,8,16,0]",0)!=string::npos)
	   return 7+i;
	 if(str.find("[1,3,12,16,0,0]",0)!=string::npos)
	   return 1000+i;
	 if(str.find("[1,7,8,16,0,0]",0)!=string::npos)
	   return 2000+i;
	 if(str.find("[1,7,24,0,0,0]",0)!=string::npos)
	   return 3000+i;
	 if(str.find("[1,15,16,0,0,0]",0)!=string::npos)
	   return 4000+i;
	 if(str.find("[1,31,0,0,0,0]",0)!=string::npos)
	   return 5000+i;
	 return -1;
}

int old(){
 vector<string> vstr;
 char szline[1000]={0};
 while (fin.getline(szline,1000)){
	 int id=0;
	 char sz[1000]={0};
	 sscanf(szline,"R8R4_%d=%s",&id,sz);
	 vstr.push_back(sz);
 }
 sort(vstr.begin(),vstr.end(),[](string& str1,string& str2)->bool{return R32N0idx(str1)<R32N0idx(str2);});
 int n=vstr.size();
 for(int i=0;i<n;i++){
	char sz[1000]={0};
 	sprintf(sz,"m_RingInvariant.insert(make_pair(\"%s\",%d));",vstr[i].c_str(),R32id(vstr[i],i));
	fout<<sz<<endl;
 }
 system("pause");
 return 0;
}

int main(){
 vector<pair<string,int> > vstr;
 char szline[1000]={0};
 while (fin.getline(szline,1000)){
	 int id=0;
	 char sz[1000]={0};
	 sscanf(szline,"R8R4_%d=%s",&id,sz);
	 vstr.push_back(make_pair(sz,id));
 }
 sort(vstr.begin(),vstr.end(),[](pair<string,int>& str1,pair<string,int>& str2)->bool{return R32N0idx(str1.first)<R32N0idx(str2.first);});
 int n=vstr.size();
 for(int i=0;i<n;i++){
	char sz[1000]={0};
 	sprintf(sz,"m_RingInvariant.insert(make_pair(\"%s\",%d));//R8R4_%d",vstr[i].first.c_str(),R32id(vstr[i].first,i),vstr[i].second);
	fout<<sz<<endl;
 }
 system("pause");
 return 0;
}