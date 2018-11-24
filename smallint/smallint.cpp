#include "smallint.h"

int __stdcall apailie(int n,int m)
{
	if(m==0)
		return 1;
	else
		return (n-m+1)*apailie(n,m-1);
}

int __stdcall czuhe(int n,int m)
{
	return apailie(n,m)/apailie(m,m);
}

unsigned long long __stdcall Factdouble(short num)
{
	unsigned long long k,t=1;
	if(num%2==1)
	{
		for(k=1;k<=num;k=k+2)
			t=k*t;
	}
	else if(num%2==0)
	{
		for(k=2;k<=num;k=k+2)
			t=k*t;
	}
	return t;
}

int __stdcall gcd(int a, int b) 
{ 
	if(a*b<0)
		return -gcd(abs(a),abs(b));
	int temp=0;
	if(a<b)
	{
		temp=a;
		a=b;
		b=temp;
	}
	if(a%b==0)
		return b;
	else
		return gcd(a%b,b);
	return 0;
}

int calcyue(int x,int y)
{ 
	int n; 
	while(x%y){ 
		n=x%y; 
		x=y; 
		y=n; 
	} 
	return n; 
}

int bei(int x, int y)
{
	int ret=x*y/calcyue(x,y); 
	return ret;
}

int __stdcall Iscoprime(int a, int b)
{
	int ret=0;
	if(gcd(a,b)==1)
		ret=1;
	return ret;
}

int __stdcall euler(int num)
{
	int count=0;
	if(num==1)
		return 1;
	for(int i=1;i<=num-1;i++)
	{
	   count+=Iscoprime(num,i);
	}
	return count;
}

double __stdcall Zeta(double s)
{
	int i;
	double sum=0;
	for (i=1;i<=100;i++)
		sum+=1.0/pow((double)i,s);
	return sum;
}

/*
按定义计算二次剩余和二次非剩余
x=8,(13/17)=1
x=无解,(5/17)=-1
*/
int __stdcall Legendre(int a,int p,int *x)
{
	if(x!=NULL)
		*x=0;
	if(a%p==0)
		return 0;//a是p的倍数
	for(int i=1;i<p;i++)
	{
	   if((i*i-a)%p==0)
	   {
		   if(x!=NULL)
			   *x=i;
		   return 1;//a是p的二次剩余
	   }
	}
    return -1;//a是p的二次非剩余
}

//i=8
//i=9
//x=8,(13/17)=1
//x=0,(5/17)=-1
int __stdcall LegendreEx(int a,int p,int *x)
{
    if(x!=NULL)
		*x=0;
	if(a%p==0)
		return 0;//a是p的倍数
	int ret=-1;
	for(int i=1;i<p;i++)
	{
	   if((i*i-a)%p==0)
	   {
	   	   if(ret==-1)
		   {
		      ret=1;
			  if(x!=NULL)
				  *x=i;
		   }
		   char sz[100]={0};
		   sprintf(sz,"i=%d\n",i);
	   }
	}
    return ret;//a是p的二次非剩余
}

int __stdcall jacobi3(int a,int p)
{
 if(a%p==0)
  return 0;//a是p的倍数
 for(int i=1;i<p;i++)
 {
           if((i*i*i-a)%p==0)
    {
     return 1;//a是p的三次剩余
    }
 }
    return -1;//a是p的三次非剩余
} 

int __stdcall jacobi4(int a,int p)
{
 if(a%p==0)
  return 0;//a是p的倍数
 for(int i=1;i<p;i++)
 {
           if((i*i*i*i-a)%p==0)
    {
     return 1;//a是p的四次剩余
    }
 }
    return -1;//a是p的四次非剩余
}

int __stdcall DivisorSigma(int power, int number)
{
	int div, sum=0;
	for(div=1; div<=number; div++)
	{
		if(number % div == 0)
		{
			sum += pow((float)div, power);
		}
	}
	return sum;
}

int DivisorSigma1(int number)
{
	return DivisorSigma(1,number);
}

//除数和函数σ(n)
int __stdcall Sigmad(int n)
{
	int d=0,divisorsum=0;
	for(d=1;d<=n;d++)
	{
		if(n % d == 0)
		{
			divisorsum+=d;
		}
	}
	return divisorsum;
}


int __stdcall Sigma0(int n)
{
	int d=0,divisorsum=0;
	for(d=1;d<=n;d++)
	{
		if(n % d == 0)
		{
			divisorsum+=1;
		}
	}
	return divisorsum;
}

int __stdcall powd(int x,int y)
{
	float ret=powf(x,y);
	return (int)(ret+0.5);
}

int __stdcall Sigma(int k,int n)
{
	int d=0,divisorsum=0;
	for(d=1;d<=n;d++)
	{
		if(n % d == 0)
		{
			divisorsum+=powd(d,k);
		}
	}
	return divisorsum;
}


//计算雅可比符号(d/n)  
int __stdcall jacobi(int d,int n)   
{   
	if(n%2==0)   //n是一个偶数    
	{   
		return -100;   
	}   
	if(d>=n)    //J(d,n)=J((d mod n),n)    
	{   
		d=d%n;   
	}   
	if(d==0)    //J(0,n)=0    
	{   
		return 0;   
	}   
	if(d==1)      //J(1,n)=1    
	{   
		return 1;   
	}   

	if(d<0)                // J(d,b)=(-1){(b-1)/2 }J(-d,b)    
	{   
		if((n-1)/2%2==0)   
		{   
			return jacobi(-d,n);   
		}   
		else   
		{   
			return -jacobi(-d,n);   
		}   
	}   

	if(d%2==0)              //d是偶数    
	{   
		if(((n*n-1)/8)%2==0)   
		{   
			return +jacobi(d/2,n);   
		}   
		else   
		{   
			return -jacobi(d/2,n);   //如果(n*n-1)/8是偶数，则J(2,n)=1，否则为-1；    
			//J(2×m,n)=J(2,n)×J(m,n),    
		}   
	}   

	int g=gcd(d,n);   

	if(d%2==0)     //s是偶数    
	{   
		return -100;   
	}   

	if(g==d)    //J(d,d)=J(0,d)=0    
	{   
		return 0;   
	}   
	else   
	{   
		if(g!=1)   
		{   
			return jacobi(g,n)*jacobi(d/g,n);   
		}   
		else   
		{   
			if(((d-1)*(n-1)/4)%2==0)   
			{   
				return +jacobi(n,d);   
			}   
			else   
			{   
				return -jacobi(n,d);   
			}   
		}   
	}   
}

/*
计算雅可比符号(d/n)，n>2且为奇整数
Jacobi符号的值：
	0表示d可以整除n
	1表示d是模n的平方剩余（？NOSURE）
	-1表示d是模n的非平方剩余（？NOSURE）
*/
int __stdcall Jacobi(int d,int n)
{
	int x=d;
	int y=n;
	int j=1;
	int t;
	while(y>1){
		x=x%y;
		if(x>y/2){
			x=y-x;
			if(y%4==3) j=-1*j;
		}
		if(x==0) {x=1;y=0;j=0;}
		while(x%4==0){
			x=x/4;	
		}
		if(x%2==0){
			x=x/2;
			if((y%8==3)||(y%8==5)) j=-1*j;
		}
		if((x%4==3)&&(y%4==3)){
			j=-1*j;
			t=x;
			x=y;
			y=t;
		}
		else{
			t=x;
			x=y;
			y=t;
		}
	}
	return j;
}

/* 
输入： 自然数a  
输出： 给出满足四方定理中的至多四个自然数i,j,k,d。  
*/
void __stdcall FourInteger(int a,int *pi,int *pj,int *pk,int *pd)
{
	int i,j,k,d;
	for( i=0;i*i<=a;i++)   
	{   
		for( j=0;i*i+j*j<=a/*&&j!=i*/;j++)   
		{   
			for( k=0;i*i+j*j+k*k<=a/*&&k!=i&&k!=j*/;k++)   
			{   
				for( d=0;i*i+j*j+k*k+d*d<=a/*&&d!=i&&d!=j&&d!=k*/;d++)   
				{   
					if(i*i+j*j+k*k+d*d==a)   
					{   
						goto end;   
					}   

				}   
			}   
		}   
	}   
end:
	*pi=i;
	*pj=j;
	*pk=k;
	*pd=d; 
}

//获取小于等于给定数的素数
unsigned int __stdcall GetMaxPrime(unsigned int nLessNum)
{
	unsigned int uPrime=nLessNum;
	for(;uPrime>2;uPrime--)
	{
		unsigned int u2=2;
		for(;u2<=uPrime/2;u2++)
			if(uPrime%u2==0)
				break;
		if(u2>uPrime/2)
			break;
	}
	return uPrime;
}

int __stdcall minprimroot(int p)
{   
	bool bFind=false;
        int k=1;
	while(k<=p)
	{
          bFind=is_primitive_root(k,p);
          if(bFind)
            break;
          else
	    k++;
	}
        if(!bFind)
          k=0;
	return k;
}

int __stdcall mpower(int a,int k,int p)
{
        if(k==0)
        {
            return 1;
        }
        if(k<0)
        {
            int b=inv(a,p);
            int ret=mpower(b,-k,p);
            return ret;
        }
	int ans=1;
	for(int i=1;i<=k;i++)
	   ans=ans*a%p;
	return ans;

}

int __stdcall orderm(int a, int p)
{
	int k=1;
	while(k<=p)
	{
		if(mpower(a,k,p)==1) break;
		k++;
	}
	return k;
}

bool __stdcall is_primitive_root(int a, int p)
{
	int k=orderm(a,p);
	bool ret=(k==euler(p));
	return ret;
}

// 当p比较小的时候，F_p上离散对数的问题可以通过穷搜解决。但当p很大的时候，穷搜的效果就不好了。
int __stdcall LogFFE(int p,int a,int b)
{
	int k=1;
	while(k<=p)
	{
		if(mpower(a,k,p)==b) break;
		k++;
	}
	return k;
}

int __stdcall mod(int n, int p)
{
register int raw_mod ;      /*  The value n % p computed by C's mod function.  */
raw_mod = n % p ;
if (raw_mod == 0)
    return( 0 ) ;
else if (n >= 0)             /*  mod is not 0.  n >= 0. */
    return( raw_mod ) ;
else
    return( raw_mod + p ) ;  /* mod is not 0.  n < 0. */
}

int __stdcall inv(int a, int p)
{
    int t1 = 0 ;
    int t3 = 0 ;
    int q  = 0 ;
    int u1 = 1 ;
    int u3 = a ;
    int v1 = 0 ;
    int v3 = p ;
    int inv_v = 0 ;

    while( v3 != 0)
    {
        q = (int)(u3 / v3) ;
        t1 = u1 - v1 * q ;
        t3 = u3 - v3 * q ;
        u1 = v1 ;
        u3 = v3 ;
        v1 = t1 ;
        v3 = t3 ;
    }
    inv_v = mod( u1, p ) ;
    if ( mod( a * inv_v, p ) != 1)return 0 ;
    return inv_v ;
}