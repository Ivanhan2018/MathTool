#ifndef _DDCOMPLEX_H
#define _DDCOMPLEX_H

#include <string.h>
#include <math.h>
#include <stdlib.h>

#include<complex>
#include<limits>
using namespace std;
typedef std::complex<double> dcomplex;

#define __stdcall
#define pi (double)3.14159265359
//Const PI=3.14159265358979
#define M_PI 3.14159265358979323846

/*复数定义*/
typedef struct
{
  double re;
  double im;
}DCOMPLEX;

extern "C" void __stdcall occos(double x,double y,double *u,double *v);

extern "C" void __stdcall ocdiv(double a,double b,double c,double d,double *e,double *f);

extern "C" void __stdcall ocexp(double x,double y,double *u,double *v);

extern "C" void __stdcall oclog(double x,double y,double *u,double *v);

extern "C" void __stdcall ocmul(double a,double b,double c,double d,double *e,double *f);

extern "C" void __stdcall ocsin(double x,double y,double *u,double *v);

extern "C" void __stdcall ontrt(double x,double y,int n,double *u,double *v);
extern "C" void __stdcall opowr(double x,double y,int n,double *u,double *v);
extern "C" DCOMPLEX __stdcall CPow(DCOMPLEX *pz,double w);
extern "C" DCOMPLEX __stdcall CSin(DCOMPLEX *pz);
extern "C" DCOMPLEX __stdcall CCos(DCOMPLEX *pz);
extern "C" DCOMPLEX __stdcall CLn(DCOMPLEX *c1);
extern "C" DCOMPLEX __stdcall CExp(DCOMPLEX *c1);
extern "C" DCOMPLEX __stdcall CTan(DCOMPLEX *c1);
extern "C" DCOMPLEX __stdcall CDiv(DCOMPLEX *c1, DCOMPLEX *c2);
extern "C" double __stdcall VCCAbs(DCOMPLEX z);/*复数求模*/
extern "C" double __stdcall CAbs(DCOMPLEX *pz);/*复数求模*/
extern "C" DCOMPLEX __stdcall Add(DCOMPLEX c1, DCOMPLEX c2);/*复数加运算*/
extern "C" DCOMPLEX __stdcall Sub(DCOMPLEX c1, DCOMPLEX c2);/*复数减运算*/
extern "C" DCOMPLEX __stdcall CMul(DCOMPLEX *pz1,DCOMPLEX *pz2);
extern "C" DCOMPLEX __stdcall Mul(DCOMPLEX c1, DCOMPLEX c2);/*复数乘运算*/
extern "C" void __stdcall FFT(DCOMPLEX * TD, DCOMPLEX * FD, int power);/*快速付里哀变换,TD为时域值,FD为频域值,power为2的幂数*/
extern "C" void __stdcall DCT(double *f, double *F, int power);/*快速离散余弦变换,利用快速付里哀变换,f为时域值,F为变换域值,power为2的幂数*/
extern "C" void __stdcall WALh(double *f, double *W, int power);/*快速沃尔什-哈达玛变换,f为时域值,F为变换域值,power为2的幂数*/

extern "C" void __stdcall GetPoints(int nWidth,int nHeight,unsigned char *lpBits,unsigned char *lpPoints);
extern "C" void __stdcall PutPoints(int nWidth,int nHeight,unsigned char *lpBits,unsigned char *lpPoints);


extern "C" void Fourier1(int nWidth,int nHeight,unsigned char *lpInput,unsigned char *lpOutput,void (* Progress)(int Pos));
extern "C" void Walsh1(int nWidth,int nHeight,unsigned char *lpInput,unsigned char *lpOutput,void (* Progress)(int Pos));
extern "C" void Dct1(int nWidth,int nHeight,unsigned char *lpInput,unsigned char *lpOutput,void (* Progress)(int Pos));

extern "C" void __stdcall Fourier(int nWidth,int nHeight,unsigned char *lpInput,unsigned char *lpOutput);

extern "C"
{
	//算术几何平均
	DCOMPLEX __stdcall agm(const DCOMPLEX *a0,const DCOMPLEX *b0,double prec,unsigned int m);
	DCOMPLEX __stdcall agm1(const DCOMPLEX a0,const DCOMPLEX b0,double prec,unsigned int m);
	//勒让德第一类完全椭圆积分
	DCOMPLEX __stdcall K(const DCOMPLEX *z);
	//雅可比theta函数
	DCOMPLEX __stdcall theta(const DCOMPLEX *z,const DCOMPLEX *tau);//
	DCOMPLEX __stdcall theta_q(const DCOMPLEX *z,const DCOMPLEX *q);//JT3
	DCOMPLEX __stdcall theta_1(const DCOMPLEX *z,const DCOMPLEX *q);//JT1
	DCOMPLEX __stdcall theta_2(const DCOMPLEX *z,const DCOMPLEX *q);//
	DCOMPLEX __stdcall theta_3(const DCOMPLEX *z,const DCOMPLEX *q);//
	DCOMPLEX __stdcall theta_4(const DCOMPLEX *z,const DCOMPLEX *q);//
	DCOMPLEX __stdcall theta_00(const DCOMPLEX *z,const DCOMPLEX *tau);//JT3
	DCOMPLEX __stdcall theta_01(const DCOMPLEX *z,const DCOMPLEX *tau);//JT4
	DCOMPLEX __stdcall theta_10(const DCOMPLEX *z,const DCOMPLEX *tau);//JT1
	DCOMPLEX __stdcall theta_11(const DCOMPLEX *z,const DCOMPLEX *tau);//JT2
	DCOMPLEX __stdcall q_from_k(const DCOMPLEX *k);
	DCOMPLEX __stdcall setup_period(const DCOMPLEX *k,const DCOMPLEX *g);
	DCOMPLEX __stdcall sn(const DCOMPLEX *u,const DCOMPLEX *k);
	DCOMPLEX __stdcall p(const DCOMPLEX *z, const DCOMPLEX *tau);//实周期为1个单位（w1=1）。计算结果同math::detail::weierstrass_e1
	DCOMPLEX __stdcall P(const DCOMPLEX *z, const DCOMPLEX *tau, const DCOMPLEX *w1);//实周期为w1个单位（w1=w1）。计算结果同math::weierstrass_p2
	DCOMPLEX __stdcall e1(const DCOMPLEX *tau);//计算结果同math::detail::weierstrass_e1
	DCOMPLEX __stdcall e2(const DCOMPLEX *tau);//计算结果同math::detail::weierstrass_e2
	DCOMPLEX __stdcall e3(const DCOMPLEX *tau);//计算结果约同（≈）math::detail::weierstrass_e3
	//平稳值e_1,e_2,e_3的计算
	DCOMPLEX __stdcall E1(const DCOMPLEX *tau, const DCOMPLEX *w1);
	DCOMPLEX __stdcall E2(const DCOMPLEX *tau, const DCOMPLEX *w1);
	DCOMPLEX __stdcall E3(const DCOMPLEX *tau, const DCOMPLEX *w1);
	//不变量g_2,g_3的计算
	DCOMPLEX __stdcall g2(const DCOMPLEX *tau, const DCOMPLEX *w1);
	DCOMPLEX __stdcall g3(const DCOMPLEX *tau, const DCOMPLEX *w1);
	DCOMPLEX __stdcall KleinInvariantJ(const DCOMPLEX *tau, const DCOMPLEX *w1);
	DCOMPLEX __stdcall J(const DCOMPLEX *tau);
};

#endif