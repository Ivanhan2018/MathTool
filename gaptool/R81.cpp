#include "IRing.h"

// idx=0~17
vector<int> FindR81ID(int idx,int p=3){
    static RIDHelper help;
	const char * szN0n0bAbO[]={ \
		"[1,2,6,18,54],81,1,1,", \
		"[1,2,6,18,54],81,1,0,", \
		"[1,8,18,54,0],27,1,1,", \
		"[1,8,18,54,0],27,1,0,", \
		"[1,8,18,54,0],27,0,1,", \
		"[1,8,18,54,0],27,0,0,", \
		"[1,8,72,0,0],9,1,1,", \
		"[1,8,72,0,0],9,1,0,", \
		"[1,8,72,0,0],9,0,1,", \
		"[1,8,72,0,0],9,0,0,", \
		"[1,26,54,0,0],9,1,1,", \
		"[1,26,54,0,0],9,1,0,", \
		"[1,26,54,0,0],9,0,1,", \
		"[1,26,54,0,0],9,0,0,", \
		"[1,80,0,0,0],3,1,1,", \
		"[1,80,0,0,0],3,1,0,", \
		"[1,80,0,0,0],3,0,1,", \
		"[1,80,0,0,0],3,0,0,", \
	};
	const char * szN0n0bAbO2[]={ \
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
	if(idx<0 || idx>=cnt){
		return v;
	}
	string str=szN0n0bAbO[idx];
	if(p==2)
		str=szN0n0bAbO2[idx];
	v = help.IDFromN0n0bAbO(str);
	return v;
}

// 81阶交换环有0种
vector<int> A1R81(int p=3){
	int idxs[]={0,1,2,3,6,7,10,11,14,15};
	int cnt=sizeof(idxs)/sizeof(idxs[0]);	
	vector<int> v;
	for(int i=0;i<cnt;i++){
		vector<int> vi=FindR81ID(idxs[i],p);
		v.insert(v.end(),vi.begin(),vi.end());
	}
	sort(v.begin(),v.end());	
	return v;
}

// 81阶交非换环有0种
vector<int> A0R81(int p=3){
	int idxs[]={4,5,8,9,12,13,16,17};
	int cnt=sizeof(idxs)/sizeof(idxs[0]);	
	vector<int> v;
	for(int i=0;i<cnt;i++){
		vector<int> vi=FindR81ID(idxs[i],p);
		v.insert(v.end(),vi.begin(),vi.end());
	}
	sort(v.begin(),v.end());
	return v;
}

// 81阶幺环有0种
vector<int> O1R81(int p=3){
	int idxs[]={0,2,4,6,8,10,12,14,16};
	int cnt=sizeof(idxs)/sizeof(idxs[0]);	
	vector<int> v;
	for(int i=0;i<cnt;i++){
		vector<int> vi=FindR81ID(idxs[i],p);
		v.insert(v.end(),vi.begin(),vi.end());
	}
	sort(v.begin(),v.end());	
	return v;
}

void printR81(int type,int p=3){
	typedef vector<int>(*pF)(int p);
	pF Func[]={A1R81,A0R81,O1R81};
	const char * szDes[]={"阶交换环","阶非交换环","阶幺环"};
	int total[]={162,228,50};
	int total2[]={162,228,50};		
	vector<int> vID=Func[type](p);
    printf("%d%s已找到%d种:\n",p*p*p*p,szDes[type],vID.size());
	for(auto it=vID.begin();it!=vID.end();it++){
		printf("%d,",*it);
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	int p[]={2,3};
	int cnt=sizeof(p)/sizeof(p[0]);
	for(int i=0;i<cnt;i++){
		printR81(0,p[i]);
		printR81(1,p[i]);
		printR81(2,p[i]);	
		printf("\n");
	}	
	
	return 0;
}