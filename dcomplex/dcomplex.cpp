#include "dcomplex.h"

void __stdcall occos(double x,double y,double *u,double *v)

{ double p,q;

p=exp(y); q=exp(-y);

*u=cos(x)*(p+q)/2.0; *v=-sin(x)*(p-q)/2.0;

return;

}

void __stdcall ocdiv(double a,double b,double c,double d,double *e,double *f)

{ double p,q,s,w;

p=a*c; q=-b*d; s=(a+b)*(c-d);

w=c*c+d*d;

if (w+1.0==1.0) 

{ *e=1.0e+35*a/fabs(a);

*f=1.0e+35*b/fabs(b);

}

else

{ *e=(p-q)/w; *f=(s-p-q)/w; }

return;

}

void __stdcall ocexp(double x,double y,double *u,double *v)

{ double p;

p=exp(x);

*u=p*cos(y); *v=p*sin(y);

return;

}

void __stdcall oclog(double x,double y,double *u,double *v)

{ double p;

p=log(sqrt(x*x+y*y));

*u=p; *v=atan2(y,x);

return;

}

void __stdcall ocmul(double a,double b,double c,double d,double *e,double *f)

{ double p,q,s;

p=a*c; q=b*d; s=(a+b)*(c+d);

*e=p-q; *f=s-p-q;

return;

}

void __stdcall ocsin(double x,double y,double *u,double *v)

{ double p,q;

p=exp(y); q=exp(-y);

*u=sin(x)*(p+q)/2.0; *v=cos(x)*(p-q)/2.0;

return;

}

void __stdcall ontrt(double x,double y,int n,double *u,double *v)

{ int k;

double r,q,t;

if (n<1) return;

q=atan2(y,x);

r=sqrt(x*x+y*y);

if (r+1.0!=1.0)

{ r=(1.0/n)*log(r); r=exp(r);}

for (k=0; k<=n-1; k++)

{ t=(2.0*k*3.1415926+q)/n;

u[k]=r*cos(t); v[k]=r*sin(t);

}

return;

}
void __stdcall opowr(double x,double y,int n,double *u,double *v)

{ double r,q;

q=atan2(y,x);

r=sqrt(x*x+y*y);

if (r+1.0!=1.0)

{ r=n*log(r); r=exp(r);}

*u=r*cos(n*q); *v=r*sin(n*q);

return;

}

void __stdcall GetPoints(int nWidth,int nHeight,unsigned char *lpBits,unsigned char *lpPoints)
{
	int x,y,p;
	int nByteWidth=nWidth*3;
	if (nByteWidth%4) nByteWidth+=4-(nByteWidth%4);
	for(y=0;y<nHeight;y++)
	{
		for(x=0;x<nWidth;x++)
		{
			p=x*3+y*nByteWidth;
			lpPoints[x+y*nWidth]=(unsigned char)(0.299*(float)lpBits[p+2]+0.587*(float)lpBits[p+1]+0.114*(float)lpBits[p]+0.1);
		}
	}
}

void __stdcall PutPoints(int nWidth,int nHeight,unsigned char *lpBits,unsigned char *lpPoints)
{
	int nByteWidth=nWidth*3;
	if (nByteWidth%4) nByteWidth+=4-(nByteWidth%4);
	int x,y,p,p1;
	for(y=0;y<nHeight;y++)
	{
		for(x=0;x<nWidth;x++)
		{
			p=x*3+y*nByteWidth;
			p1=x+y*nWidth;
			lpBits[p]=lpPoints[p1];
			lpBits[p+1]=lpPoints[p1];
			lpBits[p+2]=lpPoints[p1];
		}
	}
}

DCOMPLEX __stdcall CSin(DCOMPLEX *pz)
{
DCOMPLEX c;
#if 1
c.re=sin(pz->re)*(exp(pz->im)+exp(-pz->im))/2.0; c.im=cos(pz->re)*(exp(pz->im)-exp(-pz->im))/2.0;
#else
ocsin(pz->re,pz->im,&(c.re),&(c.im));
#endif
return c;
}

DCOMPLEX __stdcall CCos(DCOMPLEX *pz)
{ 
DCOMPLEX c;
#if 1
c.re=cos(pz->re)*(exp(pz->im)+exp(-pz->im))/2.0; c.im=-sin(pz->re)*(exp(pz->im)-exp(-pz->im))/2.0;
#else
occos(pz->re,pz->im,&(ret.re),&(ret.im));
#endif
return c;
}

DCOMPLEX __stdcall CDiv(DCOMPLEX *c1, DCOMPLEX *c2)
{
	DCOMPLEX ret;
	ocdiv(c1->re,c1->im,c2->re,c2->im,&(ret.re),&(ret.im));
	return ret;
}

DCOMPLEX __stdcall CLn(DCOMPLEX *c1)
{
	DCOMPLEX ret;
	oclog(c1->re,c1->im,&(ret.re),&(ret.im));
	return ret;
}

DCOMPLEX __stdcall CExp(DCOMPLEX *c1)
{
	DCOMPLEX ret;
	ocexp(c1->re,c1->im,&(ret.re),&(ret.im));
	return ret;
}

DCOMPLEX __stdcall CTan(DCOMPLEX *c1)
{
	DCOMPLEX ret1=CSin(c1);
	DCOMPLEX ret2=CCos(c1);
	DCOMPLEX ret=CDiv(&ret1,&ret2);
	return ret;
}

DCOMPLEX __stdcall CMul(DCOMPLEX *pz1,DCOMPLEX *pz2)
{ 
DCOMPLEX c;
double p,q,s;
p=(pz1->re)*(pz2->re);
q=(pz1->im)*(pz2->im);
s=((pz1->re)+(pz1->im))*((pz2->re)+(pz2->im));
c.re=p-q;
c.im=s-p-q;
return c;
}

DCOMPLEX __stdcall CPow(DCOMPLEX *pz,double w)
{ 
DCOMPLEX c;
double r,q;
q=atan2(pz->im,pz->re);
r=sqrt((pz->re)*(pz->re)+(pz->im)*(pz->im));
if(r+1.0!=1.0)
{
r=w*log(r);
r=exp(r);
}
c.re=r*cos(w*q);
c.im=r*sin(w*q);
return c;
}

/*复数求模*/
double __stdcall VCCAbs(DCOMPLEX z)
{
        double ret;
        ret=sqrt((z.re)*(z.re)+(z.im)*(z.im));
        return ret;
}

/*复数求模*/
double __stdcall CAbs(DCOMPLEX *pz)
{
        double ret;
        ret=sqrt((pz->re)*(pz->re)+(pz->im)*(pz->im));
        return ret;
}

/*复数加运算*/
DCOMPLEX __stdcall Add(DCOMPLEX c1, DCOMPLEX c2)
{
	DCOMPLEX c;
	c.re=c1.re+c2.re;
	c.im=c1.im+c2.im;
	return c;
}

/*复数减运算*/
DCOMPLEX __stdcall Sub(DCOMPLEX c1, DCOMPLEX c2)
{
	DCOMPLEX c;
	c.re=c1.re-c2.re;
	c.im=c1.im-c2.im;
	return c;
}

/*复数乘运算*/
DCOMPLEX __stdcall Mul(DCOMPLEX c1, DCOMPLEX c2)
{
	DCOMPLEX c;
	c.re=c1.re*c2.re-c1.im*c2.im;
	c.im=c1.re*c2.im+c2.re*c1.im;
	return c;
}

/*快速付里哀变换
TD为时域值,FD为频域值,power为2的幂数*/
void __stdcall FFT(DCOMPLEX * TD, DCOMPLEX * FD, int power)
{
	int count;
	int i,j,k,bfsize,p;
	double angle;
	DCOMPLEX *W,*X1,*X2,*X;

	/*计算付里哀变换点数*/
	count=1<<power;
	/*分配运算所需存储器*/
	W=(DCOMPLEX *)malloc(sizeof(DCOMPLEX)*count/2);
	X1=(DCOMPLEX *)malloc(sizeof(DCOMPLEX)*count);
	X2=(DCOMPLEX *)malloc(sizeof(DCOMPLEX)*count);
	/*计算加权系数*/
	for(i=0;i<count/2;i++)
	{
		angle=-i*pi*2/count;
		W[i].re=cos(angle);
		W[i].im=sin(angle);
	}
	/*将时域点写入存储器*/
	memcpy(X1,TD,sizeof(DCOMPLEX)*count);
	/*蝶形运算*/
	for(k=0;k<power;k++)
	{
		for(j=0;j<1<<k;j++)
		{
			bfsize=1<<(power-k);
			for(i=0;i<bfsize/2;i++)
			{
				p=j*bfsize;
				X2[i+p]=Add(X1[i+p],X1[i+p+bfsize/2]);
				X2[i+p+bfsize/2]=Mul(Sub(X1[i+p],X1[i+p+bfsize/2]),W[i*(1<<k)]);
			}
		}
		X=X1;
		X1=X2;
		X2=X;
	}
	/*重新排序*/
	for(j=0;j<count;j++)
	{
		p=0;
		for(i=0;i<power;i++)
		{
			if (j&(1<<i)) p+=1<<(power-i-1);
		}
		FD[j]=X1[p];
	}
	/*释放存储器*/
	free(W);
	free(X1);
	free(X2);
}

/*快速离散余弦变换,利用快速付里哀变换
f为时域值,F为变换域值,power为2的幂数*/
void __stdcall DCT(double *f, double *F, int power)
{
	int i,count;
	DCOMPLEX *X;
	double s;

	/*计算离散余弦变换点数*/
	count=1<<power;
	/*分配运算所需存储器*/
	X=(DCOMPLEX *)malloc(sizeof(DCOMPLEX)*count*2);
	/*延拓*/
	memset(X,0,sizeof(DCOMPLEX)*count*2);
	/*将时域点写入存储器*/
	for(i=0;i<count;i++)
	{
		X[i].re=f[i];
	}
	/*调用快速付里哀变换*/
	FFT(X,X,power+1);
	/*调整系数*/
	s=1/sqrt(count);
	F[0]=X[0].re*s;
	s*=sqrt(2);
	for(i=1;i<count;i++)
	{
		F[i]=(X[i].re*cos(i*pi/(count*2))+X[i].im*sin(i*pi/(count*2)))*s;
	}
	/*释放存储器*/
	free(X);
}

/*快速沃尔什-哈达玛变换
f为时域值,F为变换域值,power为2的幂数*/
void __stdcall WALh(double *f, double *W, int power)
{
	int count;
	int i,j,k,bfsize,p;
	double *X1,*X2,*X;

	/*计算快速沃尔什变换点数*/
	count=1<<power;
	/*分配运算所需存储器*/
	X1=(double *)malloc(sizeof(double)*count);
	X2=(double *)malloc(sizeof(double)*count);
	/*将时域点写入存储器*/
	memcpy(X1,f,sizeof(double)*count);
	/*蝶形运算*/
	for(k=0;k<power;k++)
	{
		for(j=0;j<1<<k;j++)
		{
			bfsize=1<<(power-k);
			for(i=0;i<bfsize/2;i++)
			{
				p=j*bfsize;
				X2[i+p]=X1[i+p]+X1[i+p+bfsize/2];
				X2[i+p+bfsize/2]=X1[i+p]-X1[i+p+bfsize/2];
			}
		}
		X=X1;
		X1=X2;
		X2=X;
	}
	/*调整系数*/
//	for(i=0;i<count;i++)
//	{
//		W[i]=X1[i]/count;
//	}
	for(j=0;j<count;j++)
	{
		p=0;
		for(i=0;i<power;i++)
		{
			if (j&(1<<i)) p+=1<<(power-i-1);
		}
		W[j]=X1[p]/count;
	}
	/*释放存储器*/
	free(X1);
	free(X2);
}

void __stdcall Fourier(int nWidth,int nHeight,unsigned char *lpInput,unsigned char *lpOutput)
{
	int w=1,h=1,wp=0,hp=0;
	while(w*2<=nWidth)
	{
		w*=2;
		wp++;
	}
	while(h*2<=nHeight)
	{
		h*=2;
		hp++;
	}
	int x,y;
	unsigned char *lpPoints=new unsigned char[nWidth*nHeight];
	GetPoints(nWidth,nHeight,lpInput,lpPoints);

	DCOMPLEX *TD=new DCOMPLEX[w*h];
	DCOMPLEX *FD=new DCOMPLEX[w*h];

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			TD[x+w*y].re=lpPoints[(x)+(y)*nWidth];//Point(x,y);
			TD[x+w*y].im=0;
		}
	}

	for(y=0;y<h;y++)
	{
		FFT(&TD[w*y],&FD[w*y],wp);
	}
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			TD[y+h*x]=FD[x+w*y];
//			TD[x+w*y]=FD[x*h+y];
		}
	}
	for(x=0;x<w;x++)
	{
		FFT(&TD[x*h],&FD[x*h],hp);
	}

	memset(lpPoints,0,nWidth*nHeight);
	double m;
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			m=sqrt(FD[x*h+y].re*FD[x*h+y].re+FD[x*h+y].im*FD[x*h+y].im)/100;
			if (m>255) m=255;
			//Point((x<w/2?x+w/2:x-w/2),nHeight-1-(y<h/2?y+h/2:y-h/2))=(unsigned char)(m);
			//#define Point(x,y) lpPoints[(x)+(y)*nWidth]
			lpPoints[(x<w/2?x+w/2:x-w/2)+(nHeight-1-(y<h/2?y+h/2:y-h/2))*nWidth]=(unsigned char)(m);
		}
	}
	delete TD;
	delete FD;
	PutPoints(nWidth,nHeight,lpOutput,lpPoints);
	delete lpPoints;
}

void Fourier1(int nWidth,int nHeight,unsigned char *lpInput,unsigned char *lpOutput,void (* Progress)(int Pos))
{
	int w=1,h=1,wp=0,hp=0;
	while(w*2<=nWidth)
	{
		w*=2;
		wp++;
	}
	while(h*2<=nHeight)
	{
		h*=2;
		hp++;
	}
	int x,y;
	unsigned char *lpPoints=new unsigned char[nWidth*nHeight];
	GetPoints(nWidth,nHeight,lpInput,lpPoints);

	DCOMPLEX *TD=new DCOMPLEX[w*h];
	DCOMPLEX *FD=new DCOMPLEX[w*h];

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			TD[x+w*y].re=lpPoints[(x)+(y)*nWidth];//Point(x,y);
			TD[x+w*y].im=0;
		}
	}

	for(y=0;y<h;y++)
	{
		FFT(&TD[w*y],&FD[w*y],wp);
		Progress(y*nHeight/2/h);
	}
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			TD[y+h*x]=FD[x+w*y];
//			TD[x+w*y]=FD[x*h+y];
		}
	}
	for(x=0;x<w;x++)
	{
		FFT(&TD[x*h],&FD[x*h],hp);
		Progress(x*nHeight/2/w+nHeight/2);
	}

	memset(lpPoints,0,nWidth*nHeight);
	double m;
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			m=sqrt(FD[x*h+y].re*FD[x*h+y].re+FD[x*h+y].im*FD[x*h+y].im)/100;
			if (m>255) m=255;
			//Point((x<w/2?x+w/2:x-w/2),nHeight-1-(y<h/2?y+h/2:y-h/2))=(unsigned char)(m);
			//#define Point(x,y) lpPoints[(x)+(y)*nWidth]
			lpPoints[(x<w/2?x+w/2:x-w/2)+(nHeight-1-(y<h/2?y+h/2:y-h/2))*nWidth]=(unsigned char)(m);
		}
	}
	delete TD;
	delete FD;
	PutPoints(nWidth,nHeight,lpOutput,lpPoints);
	delete lpPoints;
}

void Walsh1(int nWidth,int nHeight,unsigned char *lpInput,unsigned char *lpOutput,void (* Progress)(int Pos))
{
	int w=1,h=1,wp=0,hp=0;
	while(w*2<=nWidth)
	{
		w*=2;
		wp++;
	}
	while(h*2<=nHeight)
	{
		h*=2;
		hp++;
	}
	int x,y;
	unsigned char *lpPoints=new unsigned char[nWidth*nHeight];
	GetPoints(nWidth,nHeight,lpInput,lpPoints);

	double *f=new double[w*h];
	double *W=new double[w*h];

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			f[x+y*w]=lpPoints[(x)+(y)*nWidth];//Point(x,y);
		}
	}

	for(y=0;y<h;y++)
	{
		WALh(f+w*y,W+w*y,wp);
		Progress(y*nHeight/2/h);
	}
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			f[x*h+y]=W[x+w*y];
		}
	}
	for(x=0;x<w;x++)
	{
		WALh(f+x*h,W+x*h,hp);
		Progress(x*nHeight/2/w+nHeight/2);
	}
	double a;
	memset(lpPoints,0,nWidth*nHeight);

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			a=fabs(W[x*h+y]*1000);
			if (a>255) a=255;
			lpPoints[(x)+(nHeight-y-1)*nWidth]=(unsigned char)a;//Point(x,nHeight-y-1)=(unsigned char)a;
		}
	}
	delete f;
	delete W;
	PutPoints(nWidth,nHeight,lpOutput,lpPoints);
	delete lpPoints;
}

void Dct1(int nWidth,int nHeight,unsigned char *lpInput,unsigned char *lpOutput,void (* Progress)(int Pos))
{
	int w=1,h=1,wp=0,hp=0;
	while(w*2<=nWidth)
	{
		w*=2;
		wp++;
	}
	while(h*2<=nHeight)
	{
		h*=2;
		hp++;
	}
	int x,y;
	unsigned char *lpPoints=new unsigned char[nWidth*nHeight];
	GetPoints(nWidth,nHeight,lpInput,lpPoints);

	double *f=new double[w*h];
	double *W=new double[w*h];

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			f[x+y*w]=lpPoints[(x)+(y)*nWidth];//Point(x,y);
		}
	}

	for(y=0;y<h;y++)
	{
		DCT(&f[w*y],&W[w*y],wp);
		Progress(y*nHeight/2/h);
	}
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			f[x*h+y]=W[x+w*y];
		}
	}
	for(x=0;x<w;x++)
	{
		DCT(&f[x*h],&W[x*h],hp);
		Progress(x*nHeight/2/w+nHeight/2);
	}
	double a;
	memset(lpPoints,0,nWidth*nHeight);

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			a=fabs(W[x*h+y]);
			if (a>255) a=255;
			lpPoints[(x)+(nHeight-y-1)*nWidth]=(unsigned char)a;//Point(x,nHeight-y-1)=(unsigned char)(a);
		}
	}
	delete f;
	delete W;
	PutPoints(nWidth,nHeight,lpOutput,lpPoints);
	delete lpPoints;
}

//算术几何平均
namespace detail{
	dcomplex agm(const dcomplex& a0,const dcomplex& b0,double prec,unsigned m);
}

//算术几何平均
namespace detail{
	dcomplex agm(const dcomplex& a0,const dcomplex& b0,double prec,unsigned m)
	{
		dcomplex a=a0;
		dcomplex b=b0;
		dcomplex t;
		for(unsigned n=0;std::abs(0.5*(a-b))>prec && n<m;++n)
		{
			t=0.5*(a+b);
			b=std::sqrt(a*b);
			a=t;
		}
		return a;
	}
}

//勒让德第一类完全椭圆积分
namespace detail{
	dcomplex K(const dcomplex& k,double e,unsigned m);
	dcomplex K(const dcomplex& z)
	{
		return K(z,std::abs(z)*1e-15,500);
	}
}

//勒让德第一类完全椭圆积分
namespace detail{
	dcomplex K(const dcomplex& k,double e,unsigned m)
	{
		dcomplex a=detail::agm(1.0,std::sqrt(1.0-k*k),e,m);
		return M_PI/2.0/a;
	}
}

//雅可比theta函数
namespace detail{
	//computed to pretty high accuracy(a few epsilon)
	dcomplex theta_q(const dcomplex& z,const dcomplex& q);//JT3
	dcomplex theta_1(const dcomplex& z,const dcomplex& q);//JT1
}

//雅可比theta函数
namespace detail{
	dcomplex theta_q(const dcomplex& z,const dcomplex& q)
	{
		if(std::abs(q)>=1)//XXX invalid q
			return 0;
		dcomplex s=0.0;
		dcomplex t=0.0;
		unsigned n=1;
		do
		{
			t=std::pow(q,n*n)*std::cos(static_cast<double>(2*n)*z);
			s+=t;
			++n;
		}while(n<100 && std::abs(t)>5*std::numeric_limits<double>::epsilon());
		//compute to high precision because we implement other function using that
		return 1.0+2.0*s;
	}

	inline dcomplex theta(const dcomplex& z,const dcomplex& tau)
	{
		return theta_q(z*M_PI,std::exp(M_PI*dcomplex(0,1)*tau));
	}

	dcomplex theta_1(const dcomplex& z,const dcomplex& q)
	{
		if(std::abs(q)>=1)//XXX invalid q
			return 0;
		dcomplex s=0.0;
		dcomplex t=0.0;
		unsigned n=0;
		int f=1;
		do
		{
			t=static_cast<double>(f)*std::pow(q,n*(n+1))*std::sin(static_cast<double>(2*n+1)*z);
			s+=t;
			++n;
			f*=-1;
		}while(n<100 && std::abs(t)>5*std::numeric_limits<double>::epsilon());
		//compute to high precision because we implement other function using that
		return 2.0*std::pow(q,1.0/4)*s;
	}

	inline dcomplex theta_2(const dcomplex& z,const dcomplex& q)
	{
		return theta_1(z+M_PI/2,q);
	}

	inline dcomplex theta_3(const dcomplex& z,const dcomplex& q)
	{
		return theta_q(z,q);
	}

	inline dcomplex theta_4(const dcomplex& z,const dcomplex& q)
	{
		return theta_3(z,-q);
	}

	//JT3
	inline dcomplex theta_00(const dcomplex& z,const dcomplex& tau)
	{
		return theta(z,tau);
	}

	//JT4
	inline dcomplex theta_01(const dcomplex& z,const dcomplex& tau)
	{
		return theta(z+0.5,tau);
	}

	//JT2
	inline dcomplex theta_10(const dcomplex& z,const dcomplex& tau)
	{
		return std::exp(M_PI*dcomplex(0,1)*(tau/4.0+z))*theta(z+tau/2.0,tau);
	}

	//JT1
	inline dcomplex theta_11(const dcomplex& z,const dcomplex& tau)
	{
		return std::exp(M_PI*dcomplex(0,1)*(tau/4.0+z+0.5))*theta(z+tau/2.0+0.5,tau);
	}
}

namespace detail{
	dcomplex q_from_k(const dcomplex& k);
	dcomplex setup_period(const dcomplex& k,const dcomplex& g);
}

namespace detail{
	dcomplex q_from_k(const dcomplex& k)
	{
		dcomplex kprimes=std::pow(1.0-k*k,1.0/4);
		dcomplex l=0.5*(1.0-kprimes)/(1.0+kprimes);
		dcomplex l4=l*l*l*l;
		dcomplex q=l;
		l*=l4;q+=2.0*l;
		l*=l4;q+=15.0*l;
		l*=l4;q+=150.0*l;
		l*=l4;q+=1707.0*l;
		l*=l4;q+=20910.0*l;
		l*=l4;q+=268616.0*l;
		//XXX manually truncated series representation,error should be less than 10^(-4) for |k|<500
		//std::cout<<268616*std::pow(l,25)<<std::endl;
		return q;
	}

	dcomplex setup_period(const dcomplex& k,const dcomplex& g)
	{
		dcomplex K;//double quarter period
		dcomplex iK_prime;
		dcomplex P;
		K=detail::K(k,1e-25,5000);
		iK_prime=dcomplex(0,1)*detail::K(std::sqrt(1.0-k*k),1e-25,5000);
		if((K/g).imag()>(iK_prime/g).imag())
			P=K;
		else
			P=iK_prime;
		P*=4.0/g;
		return P;
	}
}

namespace detail{
	dcomplex sn(const dcomplex& u,const dcomplex& k);
	dcomplex P(const dcomplex& z, const dcomplex& tau);//实周期为1个单位（w1=1）。计算结果同math::detail::weierstrass_e1
	dcomplex P(const dcomplex& z, const dcomplex& tau, const dcomplex& w1);//实周期为w1个单位（w1=w1）。计算结果同math::weierstrass_p2

	dcomplex e1(const dcomplex& tau);//计算结果同math::detail::weierstrass_e1
	dcomplex e2(const dcomplex& tau);//计算结果同math::detail::weierstrass_e2
	dcomplex e3(const dcomplex& tau);//计算结果约同（≈）math::detail::weierstrass_e3

	//平稳值e_1,e_2,e_3的计算
	inline dcomplex e1(const dcomplex& tau, const dcomplex& w1)
	{
		return e1(tau)/(w1*w1);
	}
	inline dcomplex e2(const dcomplex& tau, const dcomplex& w1)
	{
		return e2(tau)/(w1*w1);
	}
	inline dcomplex e3(const dcomplex& tau, const dcomplex& w1)
	{
		return e3(tau)/(w1*w1);
	}

	//不变量g_2,g_3的计算
	inline dcomplex g2(const dcomplex& tau, const dcomplex& w1)
	{
		dcomplex e1=detail::e1(tau,w1);
		dcomplex e2=detail::e2(tau,w1);
		dcomplex e3=detail::e3(tau,w1);
		dcomplex retg2=dcomplex(2.0,0)*(e1*e1+e2*e2+e3*e3);
		return retg2;
	}
	inline dcomplex g3(const dcomplex& tau, const dcomplex& w1)
	{
		dcomplex e1=detail::e1(tau,w1);
		dcomplex e2=detail::e2(tau,w1);
		dcomplex e3=detail::e3(tau,w1);
		dcomplex retg3=dcomplex(4.0,0)*e1*e2*e3;
		return retg3;
	}

	//void Test1()
	//{
	//	std::cout<<"g_2(实周期w_1=8.626062，虚周期w_2=7.416195i)="<<g2(dcomplex(0,7.416195/8.626062),dcomplex(8.626062,0))<<std::endl;
	//	std::cout<<"g_3(实周期w_1=8.626062，虚周期w_2=7.416195i)="<<g3(dcomplex(0,7.416195/8.626062),dcomplex(8.626062,0))<<std::endl;
	//}

	//void Test2()
	//{
	//	std::cout<<"sn(u=1.26604138177897;k=0.985171431009416)="<<sn(dcomplex(1.26604138177897,0),dcomplex(0.985171431009416,0))<<std::endl;
	//}
}

namespace detail{
	dcomplex sn(const dcomplex& u,const dcomplex& k)
	{
		if(k==0.0)
			return std::sin(u);
		static dcomplex last_k=std::numeric_limits<double>::quiet_NaN();
		static dcomplex f;
		static dcomplex g;
		static dcomplex q;
		static dcomplex P;//period to use for reduction
		if(k!=last_k)
		{
			q=q_from_k(k);
			f=theta_3(0,q)/theta_2(0,q);
			g=theta_3(0,q);
			g*=g;
			P=setup_period(k,g);
			last_k=k;
		}
		dcomplex z=u/g;

		//reduce imaginary part
		int a=static_cast<int>(z.imag()/P.imag());
		z-=static_cast<double>(a)*P;

		return f*theta_1(z,q)/theta_4(z,q);
	}

	dcomplex P(const dcomplex& z, const dcomplex& tau)
	{
		dcomplex z1=dcomplex(M_PI*M_PI,0);
		dcomplex z2=z1*std::pow(theta_00(dcomplex(0,0),tau),2);
		z2=z2*std::pow(theta_10(dcomplex(0,0),tau),2);
		z2=z2*std::pow(theta_01(z,tau),2);
		z2=z2/std::pow(theta_11(z,tau),2);
		dcomplex z3=(z1/dcomplex(3.0,0))*(std::pow(theta_00(dcomplex(0,0),tau),4)+std::pow(theta_10(dcomplex(0,0),tau),4));
		return z2-z3;
	}

	dcomplex P(const dcomplex& z, const dcomplex& tau, const dcomplex& w1)
	{
		dcomplex retP=P(z/w1,tau)/(w1*w1);
		return retP;
	}

	dcomplex e1(const dcomplex& tau)
	{
		dcomplex z1=dcomplex(M_PI*M_PI,0);
		dcomplex e1=(z1/dcomplex(3.0,0))*(std::pow(theta_00(dcomplex(0,0),tau),4)+std::pow(theta_01(dcomplex(0,0),tau),4));
		return e1;
	}

	dcomplex e2(const dcomplex& tau)
	{
		dcomplex z1=dcomplex(M_PI*M_PI,0);
		dcomplex e2=-(z1/dcomplex(3.0,0))*(std::pow(theta_00(dcomplex(0,0),tau),4)+std::pow(theta_10(dcomplex(0,0),tau),4));
		return e2;
	}

	dcomplex e3(const dcomplex& tau)
	{
		dcomplex z1=dcomplex(M_PI*M_PI,0);
		dcomplex e3=(z1/dcomplex(3.0,0))*(std::pow(theta_10(dcomplex(0,0),tau),4)-std::pow(theta_01(dcomplex(0,0),tau),4));
		return e3;
	}

}

//算术几何平均
DCOMPLEX __stdcall agm(const DCOMPLEX *a0,const DCOMPLEX *b0,double prec,unsigned int m)
{
	dcomplex ret=detail::agm(dcomplex(a0->re,a0->im),dcomplex(b0->re,b0->im),prec,m);
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

//算术几何平均
DCOMPLEX __stdcall agm1(const DCOMPLEX a0,const DCOMPLEX b0,double prec,unsigned int m)
{
	dcomplex ret=detail::agm(dcomplex(a0.re,a0.im),dcomplex(b0.re,b0.im),prec,m);
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

//勒让德第一类完全椭圆积分
DCOMPLEX __stdcall K(const DCOMPLEX *z)
{
	dcomplex ret=detail::K(dcomplex(z->re,z->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

//雅可比theta函数
DCOMPLEX __stdcall theta(const DCOMPLEX *z,const DCOMPLEX *tau)
{
	dcomplex ret=detail::theta(dcomplex(z->re,z->im),dcomplex(tau->re,tau->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall theta_q(const DCOMPLEX *z,const DCOMPLEX *q)
{
	dcomplex ret=detail::theta_q(dcomplex(z->re,z->im),dcomplex(q->re,q->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall theta_1(const DCOMPLEX *z,const DCOMPLEX *q)
{
	dcomplex ret=detail::theta_1(dcomplex(z->re,z->im),dcomplex(q->re,q->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall theta_2(const DCOMPLEX *z,const DCOMPLEX *q)
{
	dcomplex ret=detail::theta_2(dcomplex(z->re,z->im),dcomplex(q->re,q->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall theta_3(const DCOMPLEX *z,const DCOMPLEX *q)
{
	dcomplex ret=detail::theta_3(dcomplex(z->re,z->im),dcomplex(q->re,q->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall theta_4(const DCOMPLEX *z,const DCOMPLEX *q)
{
	dcomplex ret=detail::theta_4(dcomplex(z->re,z->im),dcomplex(q->re,q->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall theta_00(const DCOMPLEX *z,const DCOMPLEX *tau)
{
	dcomplex ret=detail::theta_00(dcomplex(z->re,z->im),dcomplex(tau->re,tau->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall theta_01(const DCOMPLEX *z,const DCOMPLEX *tau)
{
	dcomplex ret=detail::theta_01(dcomplex(z->re,z->im),dcomplex(tau->re,tau->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall theta_10(const DCOMPLEX *z,const DCOMPLEX *tau)
{
	dcomplex ret=detail::theta_10(dcomplex(z->re,z->im),dcomplex(tau->re,tau->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall theta_11(const DCOMPLEX *z,const DCOMPLEX *tau)
{
	dcomplex ret=detail::theta_11(dcomplex(z->re,z->im),dcomplex(tau->re,tau->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}


DCOMPLEX __stdcall q_from_k(const DCOMPLEX *k)
{
	dcomplex ret=detail::q_from_k(dcomplex(k->re,k->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall setup_period(const DCOMPLEX *k,const DCOMPLEX *g)
{
	dcomplex ret=detail::setup_period(dcomplex(k->re,k->im),dcomplex(g->re,g->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall sn(const DCOMPLEX *u,const DCOMPLEX *k)
{
	dcomplex ret=detail::sn(dcomplex(u->re,u->im),dcomplex(k->re,k->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall p(const DCOMPLEX *z, const DCOMPLEX *tau)
{
	dcomplex ret=detail::P(dcomplex(z->re,z->im),dcomplex(tau->re,tau->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall P(const DCOMPLEX *z, const DCOMPLEX *tau, const DCOMPLEX *w1)
{
	dcomplex ret=detail::P(dcomplex(z->re,z->im),dcomplex(tau->re,tau->im),dcomplex(w1->re,w1->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}


DCOMPLEX __stdcall e1(const DCOMPLEX *tau)
{
	dcomplex ret=detail::e1(dcomplex(tau->re,tau->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall e2(const DCOMPLEX *tau)
{
	dcomplex ret=detail::e2(dcomplex(tau->re,tau->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall e3(const DCOMPLEX *tau)
{
	dcomplex ret=detail::e3(dcomplex(tau->re,tau->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}
//平稳值e_1,e_2,e_3的计算
DCOMPLEX __stdcall E1(const DCOMPLEX *tau, const DCOMPLEX *w1)
{
	dcomplex ret=detail::e1(dcomplex(tau->re,tau->im),dcomplex(w1->re,w1->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall E2(const DCOMPLEX *tau, const DCOMPLEX *w1)
{
	dcomplex ret=detail::e2(dcomplex(tau->re,tau->im),dcomplex(w1->re,w1->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall E3(const DCOMPLEX *tau, const DCOMPLEX *w1)
{
	dcomplex ret=detail::e3(dcomplex(tau->re,tau->im),dcomplex(w1->re,w1->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

//不变量g_2,g_3的计算
DCOMPLEX __stdcall g2(const DCOMPLEX *tau, const DCOMPLEX *w1)
{
	dcomplex ret=detail::g2(dcomplex(tau->re,tau->im),dcomplex(w1->re,w1->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall g3(const DCOMPLEX *tau, const DCOMPLEX *w1)
{
	dcomplex ret=detail::g3(dcomplex(tau->re,tau->im),dcomplex(w1->re,w1->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

//Klein's absolute invariant
dcomplex KleinInvariantJ(const dcomplex& tau, const dcomplex& w1=dcomplex(1.0,0))
{
   dcomplex g2g2g2=std::pow(detail::g2(tau,w1),3);
   dcomplex g3g3=std::pow(detail::g3(tau,w1),2);
   dcomplex ret=g2g2g2/(g2g2g2-g3g3*dcomplex(27.0,0));
   return ret;
}

DCOMPLEX __stdcall KleinInvariantJ(const DCOMPLEX *tau, const DCOMPLEX *w1)
{
	dcomplex ret=KleinInvariantJ(dcomplex(tau->re,tau->im),dcomplex(w1->re,w1->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}

DCOMPLEX __stdcall J(const DCOMPLEX *tau)
{
	dcomplex ret=KleinInvariantJ(dcomplex(tau->re,tau->im));
	DCOMPLEX ret1;
	ret1.re=ret.real();
	ret1.im=ret.imag();
	return ret1;
}