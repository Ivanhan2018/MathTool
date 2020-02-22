#include "RingUtil.h"
#include <cstdio>

// FR.cpp是Znm.cpp的推广
/*
2Z/8Z=R4_2
./FR "1;8;2"对应的gap命令：
R:=RingByGenerators([ZmodnZObj(2,8)]);;Size(R);L:=Elements(R);;n1:=0;;for i1 in L do if InverseMutable(i1)=fail then n1:=n1+1;fi;od;n2:=0;;for i2 in L do if IsIdempotent(i2) then n2:=n2+1;fi;od;n3:=0;;for i3 in L do if IsOne(i3) then n3:=n3+1;fi;od;n4:=0;;for i4 in L do if IsZero(i4)=false and i4^2=Zero(R) then n4:=n4+1;fi;od;n5:=0;;for i5 in L do if IsZero(i5)=false and i5^3=Zero(R) then n5:=n5+1;fi;od;n6:=0;;for i6 in L do for j6 in L do if IsZero(i6*j6) then n6:=n6+1;fi;od;od;Print("不可逆元个数n1=",n1,",幂等元个数n2=",n2,",特征：",Characteristic(R),",是否交换：",IsAbelian(R),",是否有幺元=",n3=1,",2次幂零元个数n4=",n4,",2~3次幂零元个数n5=",n5,",零乘个数n6=",n6,"\n");
Z/16Z=R16_3对应的gap命令：
./FR "1;16;1"
./FR "1;16;3"
R:=ZmodnZ(16);;Size(R);IsRing(R);L:=Elements(R);;n1:=0;;for i1 in L do if InverseMutable(i1)=fail then n1:=n1+1;fi;od;n2:=0;;for i2 in L do if IsIdempotent(i2) then n2:=n2+1;fi;od;n3:=0;;for i3 in L do if IsOne(i3) then n3:=n3+1;fi;od;n4:=0;;for i4 in L do if IsZero(i4)=false and i4^2=Zero(R) then n4:=n4+1;fi;od;n5:=0;;for i5 in L do if IsZero(i5)=false and i5^3=Zero(R) then n5:=n5+1;fi;od;n6:=0;;for i6 in L do for j6 in L do if IsZero(i6*j6) then n6:=n6+1;fi;od;od;Print("不可逆元个数n1=",n1,",幂等元个数n2=",n2,",特征：",Characteristic(R),",是否交换：",IsAbelian(R),",是否有幺元=",n3=1,",2次幂零元个数n4=",n4,",2~3次幂零元个数n5=",n5,",零乘个数n6=",n6,"\n");
R:=RingByGenerators([ZmodnZObj(3,16)]);;Size(R);IsRing(R);L:=Elements(R);;n1:=0;;for i1 in L do if InverseMutable(i1)=fail then n1:=n1+1;fi;od;n2:=0;;for i2 in L do if IsIdempotent(i2) then n2:=n2+1;fi;od;n3:=0;;for i3 in L do if IsOne(i3) then n3:=n3+1;fi;od;n4:=0;;for i4 in L do if IsZero(i4)=false and i4^2=Zero(R) then n4:=n4+1;fi;od;n5:=0;;for i5 in L do if IsZero(i5)=false and i5^3=Zero(R) then n5:=n5+1;fi;od;n6:=0;;for i6 in L do for j6 in L do if IsZero(i6*j6) then n6:=n6+1;fi;od;od;Print("不可逆元个数n1=",n1,",幂等元个数n2=",n2,",特征：",Characteristic(R),",是否交换：",IsAbelian(R),",是否有幺元=",n3=1,",2次幂零元个数n4=",n4,",2~3次幂零元个数n5=",n5,",零乘个数n6=",n6,"\n");
16阶全矩阵环M_2(F_2)=R16_300：3有零因子非交换幺环,0,1,10,8,3,3,58,9,2
./FR "2;2;1,1,0,1;1,0,1,0"对应的gap命令：
m:=2;;I:=[ [ ZmodnZObj( 1, m), ZmodnZObj( 0, m) ], [ ZmodnZObj( 0, m), ZmodnZObj( 1, m) ] ];;B:=[ [ ZmodnZObj( 1, m), ZmodnZObj( 1, m) ], [ ZmodnZObj( 0, m), ZmodnZObj( 1, m) ] ];;D:=[ [ ZmodnZObj( 1, m), ZmodnZObj( 0, m) ], [ ZmodnZObj( 1, m), ZmodnZObj( 0, m) ] ];;R:=RingByGenerators([B,D]);;Size(R);L:=Elements(R);n1:=0;;for i1 in L do if InverseMutable(i1)=fail then n1:=n1+1;fi;od;n2:=0;;for i2 in L do if IsIdempotent(i2) then n2:=n2+1;fi;od;Print("不可逆元个数n1=",n1,",幂等元个数n2=",n2,",特征：",Characteristic(R),",是否交换：",IsAbelian(R),"\n");
256阶全矩阵环M_2(Z/4Z)的一个16阶子环R16_101：4有零因子非交换无幺环,0,0,16,5,5,7,88,7,1
./FR "2;4;1,0,1,0;1,1,1,1"对应的gap命令：
m:=4;;A:=[ [ ZmodnZObj( 1, m), ZmodnZObj( 0, m ) ], [ ZmodnZObj( 1, m ), ZmodnZObj( 0, m ) ] ];;B:=[ [ ZmodnZObj( 1, m), ZmodnZObj( 1, m ) ], [ ZmodnZObj( 1, m ), ZmodnZObj( 1, m ) ] ];;R:=RingByGenerators([A,B]);;Size(R);L:=Elements(R);n1:=0;;for i1 in L do if InverseMutable(i1)=fail then n1:=n1+1;fi;od;n2:=0;;for i2 in L do if IsIdempotent(i2) then n2:=n2+1;fi;od;Print("不可逆元个数n1=",n1,",幂等元个数n2=",n2,",特征：",Characteristic(R),",是否交换：",IsAbelian(R),"\n");
256阶全矩阵环M_2(Z/4Z)的一个16阶子环R16_102：4有零因子非交换无幺环,0,0,16,5,5,7,88,15,1
./FR "2;4;2,2,1,3;1,3,3,1"对应的gap命令：
m:=4;;A:=[ [ ZmodnZObj( 2, m), ZmodnZObj( 2, m ) ], [ ZmodnZObj( 1, m ), ZmodnZObj( 3, m ) ] ];;B:=[ [ ZmodnZObj( 1, m), ZmodnZObj( 3, m ) ], [ ZmodnZObj( 3, m ), ZmodnZObj( 1, m ) ] ];;R:=RingByGenerators([A,B]);;Size(R);L:=Elements(R);n1:=0;;for i1 in L do if InverseMutable(i1)=fail then n1:=n1+1;fi;od;n2:=0;;for i2 in L do if IsIdempotent(i2) then n2:=n2+1;fi;od;Print("不可逆元个数n1=",n1,",幂等元个数n2=",n2,",特征：",Characteristic(R),",是否交换：",IsAbelian(R),"\n");
R16_385=F_2[u]/(u^4)：1有零因子交换幺环,1,1,8,2,3,3,48,7,16
./GRpf 2 x^4 R16_385.txt对应的gap命令：
R1:=PolynomialRing(GF(2),1);;x:=Indeterminate(GF(2));;poly:=x^4;;IsIrreducibleRingElement(R1,poly);Degree(poly);;Factors(poly);;I:=IdealByGenerators(R1,[poly]);;R:=R1/I;;Size(R);L:=Elements(R);;n1:=0;;for i1 in L do if InverseMutable(i1)=fail then n1:=n1+1;fi;od;n2:=0;;for i2 in L do if IsIdempotent(i2) then n2:=n2+1;fi;od;n3:=0;;for i3 in L do if IsOne(i3) then n3:=n3+1;fi;od;n4:=0;;for i4 in L do if IsZero(i4)=false and i4^2=Zero(R) then n4:=n4+1;fi;od;n5:=0;;for i5 in L do if IsZero(i5)=false and i5^3=Zero(R) then n5:=n5+1;fi;od;n6:=0;;for i6 in L do for j6 in L do if IsZero(i6*j6) then n6:=n6+1;fi;od;od;Print("不可逆元个数n1=",n1,",幂等元个数n2=",n2,",特征：",Characteristic(R),",是否交换：",IsAbelian(R),",是否有幺元=",n3=1,",2次幂零元个数n4=",n4,",2~3次幂零元个数n5=",n5,",零乘个数n6=",n6,"\n");n7:=0;;for i7 in L do for j7 in L do if IsZero(i7)=false and IsZero(j7)=false and IsZero(i7*j7) then n7:=n7+1;break;fi;od;od;Print("零因子个数n7=",n7);
*/
int main(int argc, char* argv[])
{
	char szFg[140]={0};
	if(argc<2)
	{
		printf("请输入Fg：");
		scanf("%s",szFg);
	}
	else
		strcpy(szFg,argv[1]);

	char szfn[100]={0};
	sprintf(szfn,"R.txt");
	bool bRet=RingUtil::FR(szFg,szfn);

	return 0;
}


