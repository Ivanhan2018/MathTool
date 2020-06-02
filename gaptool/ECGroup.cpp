#include"IGroup.h"
#include<functional>
#include<iostream>
#include<vector>
#include<cassert>
using namespace std;

/*
本程序计算有限域F_p上的椭圆曲线E：y^2=x^3+ax+b(mod p)的#E阶交换群结构。
椭圆曲线上有理点全体构成一个Abel群。也就是说群的元素现在不是数而是点。因此我们必须规定，两个点的和（我们用(+)表示）应该是哪一点?每个点的逆元素是什么？然后再验证它们是否满足Abel群的几条公理。
1.首先我们规定每一点P的反点~P。即如果P(x,y)是椭圆曲线上一点，则(x,-y)也是椭圆曲线上一点，我们称它为P(x,y)的反点，记作~P(x,-y)。
2.现在我们定义两点P_1(x_1,y_1)和P_2(x_2,y_2)的加法运算(+)。如果连接P_1与P_2的直线与椭圆曲线相交于一点P_3，则定义其和为P_3的反点：P_1(+)P_2=~P_3。
3.每一点的逆元素为其反点。
4.零元素为无穷原点。
已知有限域F_p上的椭圆曲线群E(F_p):{(x,y)|y^2=x^3+ax+b,a,b∈F_p}∪{O},以及点P=(x,y)的阶为一个大素数，
那么给定整数a，计算点aP=(x_a,y_a)=Q很容易；给定点Q，计算整数x，使得xP=Q非常困难。
假定有椭圆曲线上的点P和B并且我们知道有某个整数k满足B=kP=P+P+…+P。找到这个k就被称为椭圆曲线上的离散对数问题。
数论和算术中常用的形式EC3:y^2=x^3+ax+b(mod p)
Mordell-Weil定理：椭圆曲线上有理点构成的群是有限生成的。
Siegel定理：椭圆曲线上的整点只有有限多个。
y^2=x^3+4x+20(mod29)=>GAP[37,1]:
y^2=x^3+1x+1(mod5)=>GAP[9,1]:
y^2=x^3+1x+1(mod7)=>GAP[5,1]:
y^2=x^3+1x+1(mod11)=>GAP[14,2]:
y^2=x^3+1x+1(mod13)=>GAP[18,2]:
y^2=x^3+1x+7(mod17)=>GAP[12,5]:
y^2=x^3+1x+11(mod17)=>GAP[20,2]:
y^2=x^3+2x+1(mod5)=>GAP[7,1]:
y^2=x^3+2x+1(mod7)=>GAP[5,1]:
y^2=x^3+2x+1(mod11)=>GAP[16,1]:
y^2=x^3+2x+1(mod13)=>GAP[8,1]:
y^2=x^3+2x+1(mod17)=>GAP[24,9]:
*/
// y^2=x^3+ax+b
typedef int(*pFuncInt2)(int x,int a,int b);
typedef pair<int,int> IntPoint;
IntPoint g_Zero=make_pair(0,0);// 无穷远点，与有限整点区分开来即可，加法群的单位元
//struct IntPoint
//{
//	int x;
//	int y;
//};
struct EC3:public IGroup
{
public:
	//  静态函数  
	static int Mod(int ret,unsigned int n);
	static int minv(int a,int b,int c);
	static int calk(int a,int p,int xp,int yp,int xq,int yq);
    static IntPoint Add(int a,int p,const IntPoint &P,const IntPoint &Q);// R=P+Q
public:
	// 实现抽象基类的方法
	virtual void printSet();
	virtual void printTable();     
	virtual int mul(int a,int b);
	virtual int size();
	virtual int inv(int a);
	// 构造函数
	EC3(int a,int b,int p);
	// 析构函数
	~EC3(){};
	// 成员函数 
	// 成员变量 
	vector<IntPoint> m_Set;
	function<int(int,int,int)> m_PolygonEC;
	int m_a;
	int m_b;
	int m_p;
};

int EC3::Mod(int ret,unsigned int n){
	assert(n>0);
	if(ret<0){
		int ret1=ret+(-ret+1)*n;
		return ret1%n;
	}
	return ret%n;
}

int EC3::minv(int a,int b,int c){
	int ret=1;
	a=a%c;
	while(b){
		if(b&1){
			ret=(ret*a)%c;
		}
		a=(a*a)%c;
		b=b>>1;
	}
	return ret;
}

int EC3::calk(int a,int p,int xp,int yp,int xq,int yq){
	if(xp==xq&&yp==yq)
		return (((3*xp*xp+a)*minv(2*yp,p-2,p))%p+p)%p;
	return (((yq-yp)*minv(xq-xp,p-2,p))%p+p)%p;
}

IntPoint EC3::Add(int a,int p,const IntPoint &P,const IntPoint &Q){
	int x1=P.first;
	int y1=P.second;
	int x2=Q.first;
	int y2=Q.second;
	int t=calk(a,p,x1,y1,x2,y2);
	int x3=((t*t-x1-x2)%p+p)%p;
	int y3=((t*(x1-x3)-y1)%p+p)%p;
	return make_pair(x3,y3);
}

EC3::EC3(int a,int b,int p):m_a(a),m_b(b),m_p(p){
	m_PolygonEC=[=](int x,int a,int b){return x*x*x+a*x+b;};
	m_Set.push_back(g_Zero);
	for(int i=0;i<p;i++){
		for(int j=0;j<p;j++){
			if(Mod(m_PolygonEC(i,a,b),p)==Mod(j*j,p)){
				m_Set.push_back(make_pair(i,j));
			}
		}
	}
}

void EC3::printSet(){
	int ID=IdGroup(this);
	printf("y^2=x^3+%dx+%d(mod%d)=>GAP[%d,%d]:\n",m_a,m_b,m_p,size(),ID);
	function<int(int)> ord=[=](int a){vector<int> S1=Order(this,a);int o=S1.size();return o;};
	for(int i=0;i<size();i++){
		IntPoint I=m_Set[i];
		int i1=inv(i);
		IntPoint I1=m_Set[i1];
		int o=ord(i);
		int o1=ord(i1);
		printf("%d->(%d,%d),ord=%d=>%d->(%d,%d),ord=%d\n",i,I.first,I.second,o,i1,I1.first,I1.second,o1);
	}
}

void EC3::printTable(){ 
	printGroup(this);
}

int EC3::mul(int a,int b){
	if(a<0||b<0)
		return -1;
	if(a==0)
		return b;
	if(b==0)
		return a;
	if(m_Set[a].first==m_Set[b].first && m_Set[a].second+m_Set[b].second==m_p)
		return 0;
	if(a==b && m_Set[a].second==0)
		return 0;
	IntPoint R=Add(m_a,m_p,m_Set[a],m_Set[b]);
	int ret=-1;
	vector<IntPoint>::iterator p=std::find(m_Set.begin(),m_Set.end(),R);
	if(p!=m_Set.end()){
		ret=p-m_Set.begin();
	}else{
	    //cout<<"("<<R.first<<","<<R.second<<")";
	}
	return ret;
}

int EC3::size(){
	return m_Set.size();
}

int EC3::inv(int a){
	vector<int> S1=Order(this,a);
	int ord=S1.size();	
	return S1[ord-1];
}

void test(){
	// E:y^2=x^3+4x+20(mod29)
	EC3 c37(4,20,29);
	//cout<<"有限整点个数："<<c37.size()-1<<endl;
	//for(int i=1;i<c37.size();i++){
	//	cout<<"("<<c37.m_Set[i].first<<","<<c37.m_Set[i].second<<")";
	//	if(i<c37.size())
	//		cout<<",";
	//}
	//cout<<endl;
	c37.printSet();
	//c37.printTable();

	EC3 c9(1,1,5);
	c9.printSet();

	EC3 c5(1,1,7);
	c5.printSet();

	EC3 c14(1,1,11);
	c14.printSet();

	EC3 c18(1,1,13);
	c18.printSet();

	EC3 c18a(1,1,17);
	c18a.printSet();

	EC3 c2c2c3(1,7,17);
	c2c2c3.printSet();

	EC3 c20(1,11,17);
	c20.printSet();

	EC3 c7(2,1,5);
	c7.printSet();

	EC3 c5a(2,1,7);
	c5a.printSet();

	EC3 c16(2,1,11);
	c16.printSet();

	EC3 c8(2,1,13);
	c8.printSet();

	EC3 c2c12(2,1,17);
	c2c12.printSet();
}

bool IsPrime(unsigned int N){
	if(N==0||N==1)
		return false;
	int up=sqrt((float)N);
	for(int i=2;i<=up;i++){
		if(N%i==0)
			return false;
	}
	return true;
}

int main(int argc, char* argv[]){
#if 0
	test();
	system("pause");
	return 0;
#endif
	char sz[100]={0};
	if(argc<4){
		printf("Usage:  ECGroup a b p\n");
		return 0;
	}
	
    int a=atoi(argv[1]);
    int b=atoi(argv[2]);
    int p=atoi(argv[3]);
	if(a<1||b<1||!IsPrime(p)){
		return 0;
	}

	EC3 E(a,b,p);
	E.printSet();
	E.printTable();

	return 0;
}
