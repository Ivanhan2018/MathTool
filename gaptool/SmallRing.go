package main

import (
	"fmt"
)

var g_Z8Mul[][]int=[][]int{
	{0,0,0,0,0,0,0,0},
	{0,1,2,3,4,5,6,7},
	{0,2,4,6,0,2,4,6},
	{0,3,6,1,4,7,2,5},
	{0,4,0,4,0,4,0,4},
	{0,5,2,7,4,1,6,3},
	{0,6,4,2,0,6,4,2},
	{0,7,6,5,4,3,2,1},
}

// 模m加
func addmodm(a int,b int,m int)int{
	return (a+b)%m
}

// 模m乘
func mulmodm(a int,b int,m int)int{
	return (a*b)%m
}

func getIdx(Set []int,a int)int{
   for k,v:=range Set{
	   if(v==a){
		  return k
	   }
   }
   return -1  
}

func ZnTable(Set []int,m int,add func(int,int,int)(int),mul func(int,int,int)(int))([][]int,[][]int){
    A:=[][]int{}	
	n:=len(Set)
	for i:=0;i<n;i++{
	    I:=[]int{}
	    for j:=0;j<n;j++{
		    ij:=add(Set[i],Set[j],m)
			I=append(I,getIdx(Set,ij)+1)
		}
		A=append(A,I)
	}	
    M:=[][]int{}	
	for i:=0;i<n;i++{
	    I:=[]int{}
	    for j:=0;j<n;j++{
		    ij:=mul(Set[i],Set[j],m)
			I=append(I,getIdx(Set,ij)+1)
		}
		M=append(M,I)
	}
    return	A,M
}

func PrintRingTable(A [][]int,M [][]int){
    n:=len(A)
	fmt.Printf("[R%dAdd]\n",n)	
	for i:=0;i<n;i++{
	    for j:=0;j<n;j++{
			fmt.Printf("%d ",A[i][j])
		}
		fmt.Println("")
	}	
    n=len(M)	
	fmt.Printf("[R%dMul]\n",n)	
	for i:=0;i<n;i++{
	    for j:=0;j<n;j++{
			fmt.Printf("%d ",M[i][j])
		}
		fmt.Println("")
	}	
}

func main() {
	fmt.Printf("模8剩余类环Z/(8)={[0],[1],[2],[3],[4],[5],[6],[7]}:\n")	
    A,M:=ZnTable([]int{0,1,2,3,4,5,6,7},8,addmodm,mulmodm)
	PrintRingTable(A,M)
	
	fmt.Printf("Z/(64)的一个8阶子环M_8=8Z/64Z={[0],[8],[16],[24],[32],[40],[48],[56]}:\n")	
    A,M=ZnTable([]int{0,8,16,24,32,40,48,56},64,addmodm,mulmodm)
	PrintRingTable(A,M)	
	
	fmt.Printf("Z/(16)的一个8阶子环2Z/16Z={[0],[2],[4],[6],[8],[10],[12],[14]}:\n")	
    A,M=ZnTable([]int{0,2,4,6,8,10,12,14},16,addmodm,mulmodm)
	PrintRingTable(A,M)	

	fmt.Printf("Z/(12)的一个6阶子环2Z/12Z={[0],[2],[4],[6],[8],[10]}:\n")	
    A,M=ZnTable([]int{0,2,4,6,8,10},12,addmodm,mulmodm)
	PrintRingTable(A,M)
	
	fmt.Printf("Z/(18)的一个6阶子环3Z/18Z={[0],[3],[6],[9],[12],[15]}:\n")	
    A,M=ZnTable([]int{0,3,6,9,12,15},18,addmodm,mulmodm)
	PrintRingTable(A,M)	
	
	fmt.Printf("模4剩余类环Z/(4)={[0],[1],[2],[3]}:\n")	
    A,M=ZnTable([]int{0,1,2,3},4,addmodm,mulmodm)
	PrintRingTable(A,M)	
}