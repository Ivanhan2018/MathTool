package main

import "fmt"

type Vec3 struct {
	m_a1,m_a2,m_a3 int
}

var g_E8 map[Vec3]int
var g_E4 map[Vec3]int

func initE8(){
	g_E8= make(map[Vec3]int)
	var v []Vec3=[]Vec3{Vec3{0,0,0},Vec3{1,1,1},Vec3{0,0,1},Vec3{0,1,0},Vec3{0,1,1},Vec3{1,0,0},Vec3{1,0,1},Vec3{1,1,0}}
    n:=len(v)
	for i:=0;i<n;i++{
	   g_E8[v[i]] = i+1
	}
}

func initE4(){
	g_E4= make(map[Vec3]int)
	var v []Vec3=[]Vec3{Vec3{0,0,0},Vec3{0,1,0},Vec3{1,0,0},Vec3{1,1,0}}
    n:=len(v)
	for i:=0;i<n;i++{
	   g_E4[v[i]] = i+1
	}
}

// operator==
func (this *Vec3)isEqual(a Vec3)bool{
		return (this.m_a1==a.m_a1 && this.m_a2==a.m_a2 && this.m_a3==a.m_a3)
}

// 加法逆元
func (this *Vec3)InvAdd()Vec3{
	var ret Vec3
	ret.m_a1=(2-this.m_a1)%2
	ret.m_a2=(2-this.m_a2)%2
	ret.m_a3=(2-this.m_a3)%2
	return ret
}

// E_8中的加法operator+
func (this *Vec3)Add(a Vec3)Vec3{
	var ret Vec3
	//0^0=0, 1^0=1, 0^1=1, 1^1=0<=>0+0=0, 1+0=1, 0+1=1, 1+1=0
	ret.m_a1=this.m_a1^a.m_a1
	ret.m_a2=this.m_a2^a.m_a2
	ret.m_a3=this.m_a3^a.m_a3
	//ret.m_a1=(this.m_a1+a.m_a1)%2
	//ret.m_a2=(this.m_a2+a.m_a2)%2
	//ret.m_a3=(this.m_a3+a.m_a3)%2
	return ret
}

// E8群元编号
func (this *Vec3)getE8Idx()int{
    if(len(g_E8)==0){
		initE8()
	}
	Idx,ok:=g_E8[*this]
	if ok{
	   return Idx
	}else{
       return -1
	}
}

// E4群元编号
func (this *Vec3)getE4Idx()int{
    if(len(g_E4)==0){
		initE4()
	}
	Idx,ok:=g_E4[*this]
	if ok{
	   return Idx
	}else{
       return -1
	}
}

func (this *Vec3)getPoly()[]int{
	A:=[]int{}
	A=append(A,this.m_a1)
	A=append(A,this.m_a2)
	A=append(A,this.m_a3)
	doPoly(&A)
	return A
}

func printE8(){
	fmt.Printf("E8:\n") 
	var v []Vec3=[]Vec3{Vec3{0,0,0},Vec3{1,1,1},Vec3{0,0,1},Vec3{0,1,0},Vec3{0,1,1},Vec3{1,0,0},Vec3{1,0,1},Vec3{1,1,0}}
    n:=len(v)
	for i:=0;i<n;i++{
		for j:=0;j<n;j++{
		   ij:=v[i].Add(v[j])
		   IJ:=ij.getE8Idx()
		   fmt.Printf("%d ",IJ) 
		}
		fmt.Printf("\n") 
	}
}

func printE4(){
	fmt.Printf("E4:\n") 
	var v []Vec3=[]Vec3{Vec3{0,0,0},Vec3{0,1,0},Vec3{1,0,0},Vec3{1,1,0}}
    n:=len(v)
	for i:=0;i<n;i++{
		for j:=0;j<n;j++{
		   ij:=v[i].Add(v[j])
		   IJ:=ij.getE4Idx()
		   fmt.Printf("%d ",IJ) 
		}
		fmt.Printf("\n") 
	}
}

func polymul(a *[]int,m int,b *[]int,n int,c *[]int,k *int)int{
	var i,j int
	if(k!=nil){
		*k=m+n-2
	}
	for i=0;i<m+n-2;i++{
		(*c)[i]=0
	}
	for i=0;i<m;i++{
		for j=0;j<n;j++{
			(*c)[i+j]=(*a)[i]*(*b)[j]+(*c)[i+j]
		}
	}
	return 0
}

func pmul(a *[]int,b *[]int)[]int{
	m:=len(*a)
	n:=len(*b)
	k:=0
	c:=make([]int,m+n-1)
	polymul(a,m,b,n,&c,&k)
	return c
}

// 清除高位无效零、补零
func doPoly(a *[]int){
	for{
		n:=len(*a)
		if(n>0 && (*a)[n-1]==0){
			(*a)=(*a)[:len(*a)-1]
		}else{
		   break
		}
	}
	if(len(*a)==0){
		(*a)=append((*a),0)
	}	
}

func Mod(n, p int)int{
	if(n<0){
       ret1:=n+(-n+1)*p
 	   return ret1%p
	}
	return n%p	
} 

func pmulmodm(a *[]int,b *[]int,m int)[]int{
	c:=pmul(a,b)
	for i:=0;i<len(c);i++{
		c[i]=Mod(c[i],m)
	}
	doPoly(&c)
	return c
}

func polymod(a *[]int,b *[]int,m int)[]int{
	A:=(*a)[:]
	B:=(*b)[:]
	var coe int
	na:=len(A)
	nb:=len(B)
	if(na<nb){
		return A
	}else{
	for i:=0;i<na-nb+1;i++{
		for j:=A[na-1-i];;{
			if((j%B[nb-1]==0)){
				coe=j/B[nb-1]
				break
			}
				j+=2
			}
			for k:=0;k<nb;k++{
				A[na-1-i-k]=Mod(A[na-1-i-k]-coe*B[nb-1-k],m)
			}
		}
	}
	doPoly(&A)
	return A
}

func powd(x,y int)int{
	var ret int=1
	for i:=1;i<=y;i++ {
	   ret=ret*x
	}	
	return ret
}

// E_8中的一种乘法
func MulInE8(a,b int,c *[]int)int{
	var v []Vec3=[]Vec3{Vec3{0,0,0},Vec3{1,1,1},Vec3{0,0,1},Vec3{0,1,0},Vec3{0,1,1},Vec3{1,0,0},Vec3{1,0,1},Vec3{1,1,0}}
    A:=v[a].getPoly()
	B:=v[b].getPoly()	
	AB:=pmulmodm(&A,&B,2)
	ABC:=polymod(&AB,c,2)
	var ret int=0
	/*
	for j:=0;j<len(ABC);j++{
	   ret=ret+ABC[j]*powd(2,j)
	}*/
	m:=len(ABC)
    if(m<3){
		for j:=0;j<(3-m);j++{
		   ABC=append(ABC,0)
		}          
    }
	abc:=Vec3{ABC[0],ABC[1],ABC[2]}
    ret=abc.getE8Idx()	
	return ret
}

func printE8Mul(c *[]int){
	fmt.Println("c=",*c,":") 
	fmt.Printf("[R8Mul]\n") 
	var v []Vec3=[]Vec3{Vec3{0,0,0},Vec3{1,1,1},Vec3{0,0,1},Vec3{0,1,0},Vec3{0,1,1},Vec3{1,0,0},Vec3{1,0,1},Vec3{1,1,0}}
    n:=len(v)
	for i:=0;i<n;i++{
		for j:=0;j<n;j++{
		   ij:=MulInE8(i,j,c)
		   fmt.Printf("%d ",ij) 
		}
		fmt.Printf("\n") 
	}
}

// E_4中的一种乘法
func MulInE4(a,b int,c *[]int)int{
	var v []Vec3=[]Vec3{Vec3{0,0,0},Vec3{0,1,0},Vec3{1,0,0},Vec3{1,1,0}}
    A:=v[a].getPoly()
	B:=v[b].getPoly()	
	AB:=pmulmodm(&A,&B,2)
	ABC:=polymod(&AB,c,2)
	var ret int=0
	/*
	for j:=0;j<len(ABC);j++{
	   ret=ret+ABC[j]*powd(2,j)
	}*/
	m:=len(ABC)
    if(m<3){
		for j:=0;j<(3-m);j++{
		   ABC=append(ABC,0)
		}          
    }
	abc:=Vec3{ABC[0],ABC[1],ABC[2]}
    ret=abc.getE4Idx()	
	return ret
}

func printE4Mul(c *[]int){
	fmt.Println("c=",*c,":") 
	fmt.Printf("[R4Mul]\n") 
	var v []Vec3=[]Vec3{Vec3{0,0,0},Vec3{0,1,0},Vec3{1,0,0},Vec3{1,1,0}}
    n:=len(v)
	for i:=0;i<n;i++{
		for j:=0;j<n;j++{
		   ij:=MulInE4(i,j,c)
		   fmt.Printf("%d ",ij) 
		}
		fmt.Printf("\n") 
	}
}

func main() {
	printE4()
	printE4Mul(&[]int{1,1,1})	
	printE8()
	for i:=0;i<2;i++{
		for j:=0;j<2;j++{
			for k:=0;k<2;k++{
				printE8Mul(&[]int{i,j,k,1})
			}
		}
	}	
}