#include"DecompositionRing.h"
#include"quotientRing.h"
#include"M2r.h"
#include"Mnr.h"
#include"PolynomialRing.h"

void print(IRing* zn[],int cnt)
{
	for(int i=0;i<cnt;i++)
	{	
		zn[i]->printTable();
		vector<int> vIdem=calIdem(zn[i]);
		string strIdem=V2S(vIdem);		
		vector<int> vN9=calN9(zn[i]);
		//string strN9=V2S(vN9);
		int iret1=IsIdeal(zn[i],vIdem); 
		int iret2=IsIdeal(zn[i],vN9);	
 		Subring Idem(zn[i],vN9);
		//Idem.printTable();      		
		Subring N9(zn[i],vN9);
		//N9.printTable(); 	
		quotientRing znmodN9(zn[i],vN9);
		//znmodN9.printTable(); 
        char szN9[1024]={0};
		sprintf(szN9,"[R%d_%d,R%d_%d]",N9.size(),IdRing(&N9),znmodN9.size(),IdRing(&znmodN9));		
		printf("IdemN9=%s,%s\n",strIdem.c_str(),szN9);
		printf("幂等元集合%s,幂零元集合%s\n",IsIdealRetInfo(iret1),IsIdealRetInfo(iret2));
	}	
}

// 在幂零元定义中，幂零元可以是零元。
// 幂零元一定是零因子(n9<=n7+1)，存在不是幂零元的零因子，例如Z/6Z中~2，~3，~4。
// n7(Z/24Z)=24-φ(24)-1=24-8-1=15
// Z/nZ包含幂零元当且仅当n有平方因子。N9(Z/6Z)=[0],N7(Z/6Z)=[2,3,4],N9(Z/8Z)=[0,2,4,6],N9(Z/9Z)=[0,3,6]
// 弱布尔环：环R上的任意一个理想A都有A^2=A，即R上任意理想都为幂等理想。
// Z/2Z是一个2阶的弱布尔环、布尔环。
// 幂零理想环：环R上的任意一个理想A都有A^n=0，即R上任意理想都为幂零理想。
// R4_2=2Z/8Z=N9(Z/8Z)=[0,2,4,6]是一个4阶的幂零元环、幂零理想环。
void test1()
{
	ZmodnZ zn[]={ZmodnZ(1,6),ZmodnZ(1,8),ZmodnZ(1,9),ZmodnZ(1,12),ZmodnZ(1,24)};
	int cnt=sizeof(zn)/sizeof(zn[0]);
	vector<IRing*> v(cnt);
	for(int i=0;i<cnt;i++)
	{
		v[i]=&zn[i];
	}	
	print(&v[0],cnt);	
}

void test2()
{
    ZmodnZ r4_3(1,4);
    ZmodnZ r8_3(1,8);	
	int f1[] = {1,0,1};
	Polynomial vf1(f1,f1+sizeof(f1)/sizeof(f1[0]));
	int f2[] = {1,1,1};
	Polynomial vf2(f2,f2+sizeof(f2)/sizeof(f2[0]));
	int f3[] = {0,0,1};
	Polynomial vf3(f3,f3+sizeof(f3)/sizeof(f3[0]));
	int f4[] = {0,1,1};
	Polynomial vf4(f4,f4+sizeof(f4)/sizeof(f4[0]));
	int f5[] = {1,0,0,1};
	Polynomial vf5(f5,f5+sizeof(f5)/sizeof(f5[0]));
	int f6[] = {1,1,0,1};
	Polynomial vf6(f6,f6+sizeof(f6)/sizeof(f6[0]));
	int f7[] = {0,1,0,1};
	Polynomial vf7(f7,f7+sizeof(f7)/sizeof(f7[0]));	
	int f8[] = {0,0,1,1};
	Polynomial vf8(f8,f8+sizeof(f8)/sizeof(f8[0]));		
	int f9[] = {1,0,1,1};
	Polynomial vf9(f9,f9+sizeof(f9)/sizeof(f9[0]));
	int f10[] = {0,1,1,1};
	Polynomial vf10(f10,f10+sizeof(f10)/sizeof(f10[0]));	
	int f11[] = {1,1,1,1};
	Polynomial vf11(f11,f11+sizeof(f11)/sizeof(f11[0]));	
	int f12[] = {0,0,0,1};
	Polynomial vf12(f12,f12+sizeof(f12)/sizeof(f12[0]));	
	int f13[] = {2,1,1};
	Polynomial vf13(f13,f3+sizeof(f13)/sizeof(f13[0]));	
	int f14[] = {0,0,1};
	Polynomial vf14(f14,f14+sizeof(f14)/sizeof(f14[0]));	
	int g1[] = {0,2};
	int g2[] = {2};	
	int g3[] = {4,6};
	int g4[] = {4,7};
	int g5[] = {6,1};
	int g6[] = {6,3};	
	Polynomial vg1(g1,g1+sizeof(g1)/sizeof(g1[0]));
	Polynomial vg2(g2,g2+sizeof(g2)/sizeof(g2[0]));	
	Polynomial vg3(g3,g3+sizeof(g3)/sizeof(g3[0]));
	Polynomial vg4(g4,g4+sizeof(g4)/sizeof(g4[0]));	
	Polynomial vg5(g5,g5+sizeof(g5)/sizeof(g5[0]));
	Polynomial vg6(g6,g6+sizeof(g6)/sizeof(g6[0]));		
	PolynomialRing r16_118;
	r16_118.initFR(&r8_3,vg1,vg2,vf1);	
	PolynomialRing r64(&r8_3,vf2);
	PolynomialRing r16_119;
	r16_119.initFR(&r8_3,vg1,vg2,vf2);
	PolynomialRing r16_16;
	r16_16.initFR(&r8_3,vg3,vg4,vf3);
	PolynomialRing r16_17;
	r16_17.initFR(&r8_3,vg5,vg6,vf3);		
	IRing* zn[]={&r16_16,&r16_17,&r16_118,&r16_119,&r64};
	int cnt=sizeof(zn)/sizeof(zn[0]);
	print(zn,cnt);
}

void test3()
{
	M2r r4_7;
	r4_7.initG(2);
	M2r r4_8;
	r4_8.initH(2);
	//ZmodnZ r4_3(1,4);
	//Mnr M3Z4(&r4_3,3);	
	IRing* zn[]={&r4_7,&r4_8};
	int cnt=sizeof(zn)/sizeof(zn[0]);
	print(zn,cnt);	
}

int main()
{
	//test1();
	//test2();
	test3();
	
	return 0;
}