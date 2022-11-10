#include "IRing.h"

// idx=0~25
vector<int> FindR32ID(int idx){
    static RIDHelper help;
	const char * szN0n0bAbO[]={ \
		"[1,1,2,4,8,16],32,1,1,", \
		"[1,1,2,4,8,16],32,1,0,", \
		"[1,3,4,8,16,0],16,1,1,", \
		"[1,3,4,8,16,0],16,1,0,", \
		"[1,3,4,8,16,0],16,0,1,", \
		"[1,3,4,8,16,0],16,0,0,", \
		"[1,3,12,16,0,0],8,1,1,", \
		"[1,3,12,16,0,0],8,1,0,", \
		"[1,3,12,16,0,0],8,0,1,", \
		"[1,3,12,16,0,0],8,0,0,", \
		"[1,7,8,16,0,0],8,1,1,", \
		"[1,7,8,16,0,0],8,1,0,", \
		"[1,7,8,16,0,0],8,0,1,", \
		"[1,7,8,16,0,0],8,0,0,", \
		"[1,7,24,0,0,0],4,1,1,", \
		"[1,7,24,0,0,0],4,1,0,", \
		"[1,7,24,0,0,0],4,0,1,", \
		"[1,7,24,0,0,0],4,0,0,", \
		"[1,15,16,0,0,0],4,1,1,", \
		"[1,15,16,0,0,0],4,1,0,", \
		"[1,15,16,0,0,0],4,0,1,", \
		"[1,15,16,0,0,0],4,0,0,", \
		"[1,31,0,0,0,0],2,1,1,", \
		"[1,31,0,0,0,0],2,1,0,", \
		"[1,31,0,0,0,0],2,0,1,", \
		"[1,31,0,0,0,0],2,0,0,", \
	};
	int cnt=sizeof(szN0n0bAbO)/sizeof(szN0n0bAbO[0]);
	//printf("%s-cnt=%d,idx=%d\n",__FUNCTION__,cnt,idx);	
	vector<int> v;
	if(idx<0 || idx>=cnt){
		return v;
	}
	string str=szN0n0bAbO[idx];
	v = help.IDFromN0n0bAbO(str);
	return v;
}

// 32阶幺环有208种
vector<int> O1R32(){
	int idxs[]={0,2,4,6,8,10,12,14,16,18,20,22,24};
	int cnt=sizeof(idxs)/sizeof(idxs[0]);
	//printf("%s-cnt=%d\n",__FUNCTION__,cnt);	
	vector<int> v;
	for(int i=0;i<cnt;i++){
		vector<int> vi=FindR32ID(idxs[i]);
		v.insert(v.end(),vi.begin(),vi.end());
	}
	sort(v.begin(),v.end());	
	return v;
}

int main(int argc, char *argv[]){
	vector<int> vID=O1R32();
    printf("32阶幺环已找到%d种:\n",vID.size());
	for(auto it=vID.begin();it!=vID.end();it++){
		printf("%d,",*it);
	}
	printf("\n");	
	
	return 0;
}