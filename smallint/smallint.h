/*==========================================*\
*                                           
*            数论算法函数库                  
*                                          
*                  copyright hxh      
*                      2018/11/24          
*                                          
\*==========================================*/
#ifndef _SMALLINT_H
#define _SMALLINT_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define __stdcall

extern "C" 
{
int __stdcall apailie(int n,int m);// n中取m的排列数P(m上,n下)
int __stdcall czuhe(int n,int m);// n中取m的组合数C(m上,n下)
unsigned long long __stdcall Factdouble(short num);// 20以内双阶乘
int __stdcall gcd(int a, int b);// 计算gcd(u,v):求2个数的最大公约数
int __stdcall calcyue(int x,int y);
int __stdcall bei(int x, int y);
int __stdcall Iscoprime(int a, int b);
int __stdcall euler(int num);// totient函数，求不大于x且与x互素的正整数个数φ(x)
double __stdcall Zeta(double s);// 计算黎曼Zeta函数在x>1上的值
int __stdcall Legendre(int a,int p,int *x);// 按定义计算二次剩余和二次非剩余
int __stdcall LegendreEx(int a,int p,int *x);
int __stdcall DivisorSigma(int power, int number);// 除数函数σ_k(n)=DivisorSigma[power=k,number=n]
int __stdcall Sigmad(int n);// 除数和函数σ(n)=DivisorSigma(1,n)
int __stdcall Sigma0(int n);// 除数的个数DivisorSigma(0,n)
int __stdcall Sigma(int k,int n);// 除数函数σ_k(n)=DivisorSigma(k,n)
int __stdcall powd(int x,int y);
int __stdcall jacobi(int d,int n);// 计算雅可比符号(d/n)
int __stdcall Jacobi(int d,int n);
int __stdcall jacobi3(int a,int p);// 按定义计算三次剩余和三次非剩余
int __stdcall jacobi4(int a,int p);// 按定义计算四次剩余和四次非剩余
void __stdcall FourInteger(int a,int *pi,int *pj,int *pk,int *pd);
unsigned int __stdcall GetMaxPrime(unsigned int nLessNum);// 获取小于等于给定数的素数 
int __stdcall minprimroot(int p);// 返回模p的最小一个原根，返回0表示没有原根
int __stdcall mpower(int a,int k,int p);// 模取幂运算，计算a^k mod p
int __stdcall orderm(int a, int p);// a模p的阶
bool __stdcall is_primitive_root(int a, int p);
int __stdcall LogFFE(int p,int a,int b);// 求离散对数k=log_a(b)mod p，就是给定正整数p,a,b，求出正整数k（如果存在的话），使b≡a^k(mod p)
int __stdcall mod(int n, int p);
int __stdcall inv(int a, int p);// a的模p的逆，计算a^-1 mod p
};

#endif