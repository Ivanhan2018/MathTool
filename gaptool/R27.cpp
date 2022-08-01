#include "IRing.h"

// idx=0~9
vector<int> FindR27ID(int idx,int p=3){
    static RIDHelper help;
	const char * szN0n0bAbO[]={ \
		"[1,2,6,18],27,1,1,", \
		"[1,2,6,18],27,1,0,", \
		"[1,8,18,0],9,1,1,", \
		"[1,8,18,0],9,1,0,", \
		"[1,8,18,0],9,0,1,", \
		"[1,8,18,0],9,0,0,", \
		"[1,26,0,0],3,1,1,", \
		"[1,26,0,0],3,1,0,", \
		"[1,26,0,0],3,0,1,", \
		"[1,26,0,0],3,0,0,", \
	};
	const char * szN0n0bAbO2[]={ \
		"[1,1,2,4],8,1,1,", \
		"[1,1,2,4],8,1,0,", \
		"[1,3,4,0],4,1,1,", \
		"[1,3,4,0],4,1,0,", \
		"[1,3,4,0],4,0,1,", \
		"[1,3,4,0],4,0,0,", \
		"[1,7,0,0],2,1,1,", \
		"[1,7,0,0],2,1,0,", \
		"[1,7,0,0],2,0,1,", \
		"[1,7,0,0],2,0,0,", \
	};	
	int cnt=sizeof(szN0n0bAbO)/sizeof(szN0n0bAbO[0]);
	vector<int> v;
	if(idx<0 || idx>=cnt){
		return v;
	}
	string str=szN0n0bAbO[idx];
	if(p==2)
		str=szN0n0bAbO2[idx];
	v = help.IDFromN0n0bAbO(str);
	return v;
}

// 27阶交换环有0种
vector<int> A1R27(int p=3){
	int idxs[]={0,1,2,3,6,7};
	int cnt=sizeof(idxs)/sizeof(idxs[0]);	
	vector<int> v;
	for(int i=0;i<cnt;i++){
		vector<int> vi=FindR27ID(idxs[i],p);
		v.insert(v.end(),vi.begin(),vi.end());
	}
	sort(v.begin(),v.end());	
	return v;
}

// 27阶交非换环有0种
vector<int> A0R27(int p=3){
	int idxs[]={4,5,8,9};
	int cnt=sizeof(idxs)/sizeof(idxs[0]);	
	vector<int> v;
	for(int i=0;i<cnt;i++){
		vector<int> vi=FindR27ID(idxs[i],p);
		v.insert(v.end(),vi.begin(),vi.end());
	}
	sort(v.begin(),v.end());
	return v;
}

// 27阶幺环有0种
vector<int> O1R27(int p=3){
	int idxs[]={0,2,4,6,8};
	int cnt=sizeof(idxs)/sizeof(idxs[0]);	
	vector<int> v;
	for(int i=0;i<cnt;i++){
		vector<int> vi=FindR27ID(idxs[i],p);
		v.insert(v.end(),vi.begin(),vi.end());
	}
	sort(v.begin(),v.end());	
	return v;
}

void printR27(int type,int p=3){
	typedef vector<int>(*pF)(int p);
	pF Func[]={A1R27,A0R27,O1R27};
	const char * szDes[]={"阶交换环","阶非交换环","阶幺环"};
	int total[]={36,23,12};
	int total2[]={34,18,11};		
	vector<int> vID=Func[type](p);
    printf("%d%s已找到%d种(总共%d种):\n",p*p*p,szDes[type],vID.size(),p==3?total[type]:total2[type]);
	for(auto it=vID.begin();it!=vID.end();it++){
		printf("%d,",*it);
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	int p[]={2,3};
	int cnt=sizeof(p)/sizeof(p[0]);
	for(int i=0;i<cnt;i++){
		printR27(0,p[i]);
		printR27(1,p[i]);
		printR27(2,p[i]);	
		printf("\n");
	}	
	
	return 0;
}