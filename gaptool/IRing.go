package main

import "fmt"

//接口的定义
type IRing interface {
   printTable()   
   add(a,b int)int  
   mul(a,b int)int 
   size()int    
}

//接口的实现
//Z/4Z
type R4_3 struct {}
func (R R4_3)printTable(){
   fmt.Println("R4_3=Z/4Z:")
   printRing(R)
}

func (R R4_3)add(a,b int)int{
   return (a+b)%4
}

func (R R4_3)mul(a,b int)int{
   return (a*b)%4
}

func (R R4_3)size()int{
   return 4
}

//接口的实现
//2Z/8Z
type R4_2 struct {}
func (R R4_2)printTable(){
   fmt.Println("R4_2=2Z/8Z:")
   printRing(R)
}

func (R R4_2)add(a,b int)int{
   return ((2*a+2*b)%8)/2
}

func (R R4_2)mul(a,b int)int{
   return ((2*a*2*b)%8)/2
}

func (R R4_2)size()int{
   return 4
}

//接口的实现
//F_4
/*
	+ O=(0,0),I=(1,1),A=(0,1),B=(1,0)
	O O         I         A          B
	I  I          O        B          A
	A A         B         O          I
	B B         A         I           O
*/
/*
	* O=(0,0),I=(1,1),A=(0,1),B=(1,0)
	O O         O         O          O
	I  O         I        A          B
	A O         A         B          I
	B O         B         I           A
	I,A,B构成一个C_3群=>A*A=B,B*B=A,A*B=B*A=I,I是乘法单位元
*/
var gF4 []string=[]string{"O","I","A","B"}
var gF4Add [][]string=[][]string{
	{"O","I","A","B"},
	{"I","O","B","A"},
	{"A","B","O","I"},
	{"B","A","I","O"},
}
var gF4Mul [][]string=[][]string{
	{"O","O","O","O"},
	{"O","I","A","B"},
	{"O","A","B","I"},
	{"O","B","I","A"},
}
type R4_11 struct {}
func (R R4_11)getIdx(s string)int{
   for i:=0;i<len(gF4);i++{
       if(gF4[i]==s){
	      return i
	   }
   }
   return -1
}

func (R R4_11)printTable(){
   fmt.Println("R4_11=F_4:")
   printRing(R)
}

func (R R4_11)add(a,b int)int{
   ret:=R.getIdx(gF4Add[a][b])
   return ret
}

func (R R4_11)mul(a,b int)int{
   ret:=R.getIdx(gF4Mul[a][b])
   return ret
}

func (R R4_11)size()int{
   return 4
}

var gF8Add [][]int=[][]int{
	{0,   1,   2,   3,   4,   5,   6,   7},  
	{1,   0,   4,   7,   2,   6,   5,   3},  
	{2,   4,   0,   5,   1,   3,   7,   6},  
	{3,   7,   5,   0,   6,   2,   4,   1},  
	{4,   2,   1,   6,   0,   7,   3,   5},  
	{5,   6,   3,   2,   7,   0,   1,   4},  
	{6,   5,   7,   4,   3,   1,   0,   2},  
	{7,   3,   6,   1,   5,   4,   2,   0},
}

var gF8Mul [][]int=[][]int{
	{0,   0,   0,   0,   0,   0,   0,   0},  
	{0,   1,   2,   3,   4,   5,   6,   7}, 
	{0,   2,   3,   4,   5,   6,   7,   1},  
	{0,   3,   4,   5,   6,   7,   1,   2},  
	{0,   4,   5,   6,   7,   1,   2,   3},  
	{0,   5,   6,   7,   1,   2,   3,   4},  
	{0,   6,   7,   1,   2,   3,   4,   5},  
	{0,   7,   1,   2,   3,   4,   5,   6}, 
}

type R8_52 struct {}
func (R R8_52)printTable(){
   fmt.Println("R8_52=F_8:")
   printRing(R)
}

func (R R8_52)add(a,b int)int{
   ret:=gF8Add[a][b]
   return ret
}

func (R R8_52)mul(a,b int)int{
   ret:=gF8Mul[a][b]
   return ret
}

func (R R8_52)size()int{
   return 8
}

func printRing(r IRing){
   n:=r.size()
   fmt.Printf("[R%dAdd]\n",n)   
   for i:=0;i<n;i++{
	   for j:=0;j<n;j++{
		  ij:=r.add(i,j)
		  fmt.Printf("%d ",ij+1) 
	   } 
       fmt.Printf("\n")	   
   }
   fmt.Printf("[R%dMul]\n",n)   
   for i:=0;i<n;i++{
	   for j:=0;j<n;j++{
		  ij:=r.mul(i,j)
		  fmt.Printf("%d ",ij+1) 
	   } 
       fmt.Printf("\n")	   
   }
}

// 环的加法群的群元的阶
func getGOrder(r IRing,a int)int{
	n:=r.size()    
	if(a<0||a>=n){
		return -1
	}
	t:=0
	for i:=1;i<=n;i++{
		t=r.add(t,a)
		if(t==0){
			return i
		}
	}
	return -1  
}

// 未经排序的群元阶的分布
func calN0(r IRing)[]int{
    ret:=[]int{}
	n:=r.size()     
	for i:=0;i<n;i++{
	    ord:=getGOrder(r,i)
		ret=append(ret,ord)
	}  
    return ret	
}

func main() {
   r8_52 := R8_52{}
   r4_11 := R4_11{}
   r4_3 := R4_3{}
   r4_2 := R4_2{}
   var i IRing
   i = r8_52
   i.printTable()
   i = r4_11
   i.printTable()   
   i = r4_3
   i.printTable()
   i = r4_2
   i.printTable()
   fmt.Println("N0(r8_52)=",calN0(r8_52))
   fmt.Println("N0(r4_11)=",calN0(r4_11))
   fmt.Println("N0(r4_3)=",calN0(r4_3))
   fmt.Println("N0(r4_2)=",calN0(r4_2))  
}