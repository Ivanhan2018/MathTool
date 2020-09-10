#include "IRing.h"

// idx=0~17
vector<int> FindR16ID(int idx){
    static RIDHelper help;
	const char * szN0n0bAbO[]={ \
		"[1,1,2,4,8],16,1,1,", \
		"[1,1,2,4,8],16,1,0,", \
		"[1,3,4,8,0],8,1,1,", \
		"[1,3,4,8,0],8,1,0,", \
		"[1,3,4,8,0],8,0,1,", \
		"[1,3,4,8,0],8,0,0,", \
		"[1,3,12,0,0],4,1,1,", \
		"[1,3,12,0,0],4,1,0,", \
		"[1,3,12,0,0],4,0,1,", \
		"[1,3,12,0,0],4,0,0,", \
		"[1,7,8,0,0],4,1,1,", \
		"[1,7,8,0,0],4,1,0,", \
		"[1,7,8,0,0],4,0,1,", \
		"[1,7,8,0,0],4,0,0,", \
		"[1,15,0,0,0],2,1,1,", \
		"[1,15,0,0,0],2,1,0,", \
		"[1,15,0,0,0],2,0,1,", \
		"[1,15,0,0,0],2,0,0,", \
	};
	int cnt=sizeof(szN0n0bAbO)/sizeof(szN0n0bAbO[0]);
	vector<int> v;
	if(idx<0 || idx>cnt){
		return v;
	}
	string str=szN0n0bAbO[idx];
	v = help.IDFromN0n0bAbO(str);
	return v;
}

// 16阶交换环有162种
vector<int> A1R16(){
	int idxs[]={0,1,2,3,6,7,10,11,14,15};
	int cnt=sizeof(idxs)/sizeof(idxs[0]);	
	vector<int> v;
	for(int i=0;i<cnt;i++){
		vector<int> vi=FindR16ID(idxs[i]);
		v.insert(v.end(),vi.begin(),vi.end());
	}
	sort(v.begin(),v.end());	
	return v;
}

// 16阶交非换环有228种
vector<int> A0R16(){
	int idxs[]={4,5,8,9,12,13,16,17};
	int cnt=sizeof(idxs)/sizeof(idxs[0]);	
	vector<int> v;
	for(int i=0;i<cnt;i++){
		vector<int> vi=FindR16ID(idxs[i]);
		v.insert(v.end(),vi.begin(),vi.end());
	}
	sort(v.begin(),v.end());
	return v;
}

// 16阶幺环有50种
vector<int> O1R16(){
	int idxs[]={0,2,4,6,8,10,12,14,16};
	int cnt=sizeof(idxs)/sizeof(idxs[0]);	
	vector<int> v;
	for(int i=0;i<cnt;i++){
		vector<int> vi=FindR16ID(idxs[i]);
		v.insert(v.end(),vi.begin(),vi.end());
	}
	sort(v.begin(),v.end());	
	return v;
}

void printR16(int type){
	typedef vector<int>(*pF)();
	pF Func[]={A1R16,A0R16,O1R16};
	const char * szDes[]={"16阶交换环","16阶非交换环","16阶幺环"};
	int total[]={162,228,50};	
	vector<int> vID=Func[type]();
    printf("%s已找到%d种(总共%d种):\n",szDes[type],vID.size(),total[type]);
	for(auto it=vID.begin();it!=vID.end();it++){
		printf("%d,",*it);
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	printR16(0);
	printR16(1);
	printR16(2);	
	return 0;
}