// Z(2)[x]上多项式乘法逆元求解（Golang版本）
package main

import (
	"fmt"
	//"math"	
)

func indexofmax1(value int)int{
/*
    if(value<=0){
		return 0
	}
    var ret float64=math.Log(float64(value))/math.Log(float64(2))
	var ret1 int=int(ret)
	return ret1
	*/
	var tmp int=1
	var count int=0
	var n int=4*8
	for i:=0;i<n;i++{
		if((value&tmp)!=0){
			count=i
		}
		tmp=tmp*2
	}
	return count
}

func powofvalue(value int)int{
	return 1<<uint(value)
}

func divide(b,m int,r *int)int{
	mindex:=indexofmax1(m)
	vindex:=indexofmax1(b)
	if(mindex<vindex){
	    *r=m
		return 0
	}
	c:=mindex-vindex
	tmp:=b
	tmp=tmp<<uint(c)
	m=m^tmp
	return powofvalue(c)|divide(b,m,r)
}

func div(a,b int)(int,int){
    var r int=0
    q:=divide(a,b,&r)
	return q,r
}

func Tx(ax,q,bx int)int{
	tmp:=1
	value:=0
	var n int=4*8
	for i:=0;i<n;i++{
		if((q&tmp)!=0){
			value=value^((bx<<uint(i)))
		}
		tmp=tmp*2
	}
	return ax^(value)
}

func extent_gcd(b,m int)(int,int,int){
	a1:=1
	a2:=0
	a3:=m
	b1:=0
	b2:=1
	b3:=b	
	var ret int
	var x int
	var y int
	for	{
		if(b3==0){
			ret=a3
			break
		}
		if(b3==1){
			ret=b3
			break
		}
		q,r:=div(b3,a3)
		t1:=Tx(a1,q,b1)
		t2:=Tx(a2,q,b2)
		t3:=r
		//fmt.Println("t1=",t1,",t2=",t2)
		a1=b1
		a2=b2
		a3=b3
		b1=t1
		b2=t2
		b3=t3
		x=b2
		y=b3
	}
	return ret,x,y
}

func spoly(value int)string{
    str:=""
	tmp:=1
	flag:=0
	c:=indexofmax1(value)
	var n int=4*8
	for i:=0;i<n;i++{
		if((value&tmp)!=0){
			if(i==0){
				str=str+"1"
			}else if(i==1){
				str=str+"x"
			}else{
				str=str+"x^"+fmt.Sprintf("%d",i)
			}
			flag=1
			if(i<c){
				str=str+"+"
			}
		}
		tmp=tmp*2
	}
	if(flag==0){
		str=str+"0"
	}
	return str
}

func main() {
/*
   for i:=0;i<=16;i++{
    m:=indexofmax1(i)
    n:=powofvalue(i)  
    m1:=powofvalue(m+1) 	
	fmt.Println(i,m,n,m1)
   }
*/  
   {
	a1:=8
	b1:=3
	q1,r1:=div(a1,b1)
	ret,x,y:=extent_gcd(a1,b1)
	fmt.Println(a1,b1,"->",q1,r1,"->",ret,x,y)
	fmt.Println("Z/(2)[x]上多项式(",spoly(a1),")mod(",spoly(b1),")的乘法逆元是(",spoly(x),")")
   }
   {
	a1:=83
	b1:=283
	q1,r1:=div(a1,b1)
	ret,x,y:=extent_gcd(a1,b1)
	fmt.Println(a1,b1,"->",q1,r1,"->",ret,x,y)
	fmt.Println("Z/(2)[x]上多项式(",spoly(a1),")mod(",spoly(b1),")的乘法逆元是(",spoly(x),")")
   }   
}