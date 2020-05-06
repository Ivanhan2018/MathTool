package main

import "fmt"

func getidx(v *[]int,id int)int{
   n:=len(*v) 
   for i:=0;i<n;i++{
		if((*v)[i]==id){
			return i
		}
	}
    return -1
}

//接口的定义
type IGroup interface {
   printTable()     
   mul(a,b int)int 
   size()int 
   inv(a int)int
}

func printGroup(g IGroup){
   n:=g.size()  
   for i:=0;i<n;i++{
	   for j:=0;j<n;j++{
		  ij:=g.mul(i,j)
		  fmt.Printf("%d ",ij+1) 
	   } 
       fmt.Printf("\n")	   
   }
}

// 群元的阶
func getGOrder(g IGroup,a int)int{
	n:=g.size()    
	if(a<0||a>=n){
		return -1
	}
	t:=0
	for i:=1;i<=n;i++{
		t=g.mul(t,a)
		if(t==0){
			return i
		}
	}
	return -1  
}

// 未经排序的群元阶的分布
func calN0(g IGroup)[]int{
    ret:=[]int{}
	n:=g.size()     
	for i:=0;i<n;i++{
	    ord:=getGOrder(g,i)
		ret=append(ret,ord)
	}  
    return ret	
}

func issubgroup(g IGroup,v *[]int,id int)bool{
	if(getidx(v,id)==-1){
		return false
	}
	n:=len(*v)
	// 乘法封闭性
	for i:=0;i<n;i++{
		for j:=i;j<n;j++{
			ij:=g.mul((*v)[i],(*v)[j])
			ji:=g.mul((*v)[j],(*v)[i])
			if(getidx(v,ij)==-1){
				return false
			}
			if(getidx(v,ji)==-1){
				return false
			}
		}
	}
	// 有乘法逆元
	for i:=0;i<n;i++{
		iinv:=g.inv((*v)[i])
		if(getidx(v,iinv)==-1){
			return false
		}
	}
	// id是乘法单位元
	for i:=0;i<n;i++{
		if(g.mul(id,(*v)[i])!=(*v)[i] || g.mul((*v)[i],id)!=(*v)[i]){
			return false
		}
	}
	return true
}

//0是正规子群
//1是子群但不是正规子群
//2不是子群
func isnormalsubgroup(g IGroup,v *[]int,id int)int{
	if(!issubgroup(g,v,id)){
		return 2
	}
	n:=g.size()  
	nv:=len(*v)
	// 进一步判断是否是正规子群
	for i:=0;i<n;i++{
		for j:=0;j<nv;j++{
			ghg1:=g.mul(g.mul(i,(*v)[j]),g.inv(i))
			if(getidx(v,ghg1)==-1){
				return 1
			}
		}
	}
	return 0
}

// bA=1是Abel群,bA=0不是Abel群
func calbA(g IGroup)int{
	n:=g.size()
	// 乘法交换性
	for i:=0;i<n;i++{
		for j:=i;j<n;j++{
			ij:=g.mul(i,j)
			ji:=g.mul(j,i)
			if(ij!=ji){
				return 0
			}
		}
	}
	return 1
}

//返回|g|阶元的集合，如果集合为空，则表示g不是循环群
func calbC(g IGroup)[]int{
    ret:=[]int{}
	n:=g.size()     
	for i:=0;i<n;i++{
	    ord:=getGOrder(g,i)
		if(ord==n){
			ret=append(ret,i)
		}
	}  
    return ret
}

func IsInCenterOfG(g IGroup,j int)bool{
	n:=g.size()     
	for i:=0;i<n;i++{
		ij:=g.mul(i,j)
		ji:=g.mul(j,i)
		if(ij!=ji){
			return false
		}
	}
	return true
}

// 计算群g的中心
func calZ(g IGroup)[]int{
    ret:=[]int{}
	n:=g.size()     
	for i:=0;i<n;i++{
	    bInZ:=IsInCenterOfG(g,i)
		if(bInZ){
			ret=append(ret,i)
		}
	}  
    return ret
}

var gS3 [][]int=[][]int{
	{1,2,3},
	{2,3,1},
	{3,1,2},
	{3,2,1},
	{1,3,2},
	{2,1,3},
}

type S3 struct {

}

func (G S3) inv(a int)int {
	n:=len(gS3)
	for i:=0;i<n;i++{
		if(G.mul(a,i)==0){
			return i
		}
	}
	return -1
}

func (G S3) printTable() {
   fmt.Printf("GAP[6,1]=S_3:\n")
   printGroup(G)
}

func (G S3) mul(a,b int)int {
	var ret []int=[]int{0,0,0}
	tArr:=gS3[a]
	aArr:=gS3[b]	
	ret[0]=aArr[tArr[0]-1]
	ret[1]=aArr[tArr[1]-1]
	ret[2]=aArr[tArr[2]-1]
	n:=len(gS3)
	for i:=0;i<n;i++{
		if(ret[0]==gS3[i][0] && ret[1]==gS3[i][1] && ret[2]==gS3[i][2]){
			return i		
		}
	}	
	return -1
}

func (G S3) size()int {
   return 6
}

var gS5 [][]int=[][]int{
	{0,1,2,3,4},
	{0,1,2,4,3},
	{0,1,3,2,4},
	{0,1,3,4,2},
	{0,1,4,2,3},
	{0,1,4,3,2},
	{0,2,1,3,4},
	{0,2,1,4,3},
	{0,2,3,1,4},
	{0,2,3,4,1}, 
	{0,2,4,1,3},
	{0,2,4,3,1},
	{0,3,1,2,4},
	{0,3,1,4,2},
	{0,3,2,1,4},
	{0,3,2,4,1},
	{0,3,4,1,2},
	{0,3,4,2,1},
	{0,4,1,2,3},
	{0,4,1,3,2}, 
	{0,4,2,1,3},
	{0,4,2,3,1},
	{0,4,3,1,2},
	{0,4,3,2,1},
	{1,0,2,3,4},
	{1,0,2,4,3},
	{1,0,3,2,4},
	{1,0,3,4,2},
	{1,0,4,2,3},
	{1,0,4,3,2}, 
	{1,2,0,3,4},
	{1,2,0,4,3},
	{1,2,3,0,4},
	{1,2,3,4,0},
	{1,2,4,0,3},
	{1,2,4,3,0},
	{1,3,0,2,4},
	{1,3,0,4,2},
	{1,3,2,0,4},
	{1,3,2,4,0}, 
	{1,3,4,0,2},
	{1,3,4,2,0},
	{1,4,0,2,3},
	{1,4,0,3,2},
	{1,4,2,0,3},
	{1,4,2,3,0},
	{1,4,3,0,2},
	{1,4,3,2,0},
	{2,0,1,3,4},
	{2,0,1,4,3},
	{2,0,3,1,4},
	{2,0,3,4,1},
	{2,0,4,1,3},
	{2,0,4,3,1},
	{2,1,0,3,4},
	{2,1,0,4,3},
	{2,1,3,0,4},
	{2,1,3,4,0},
	{2,1,4,0,3},
	{2,1,4,3,0},
	{2,3,0,1,4},
	{2,3,0,4,1},
	{2,3,1,0,4},
	{2,3,1,4,0},
	{2,3,4,0,1},
	{2,3,4,1,0},
	{2,4,0,1,3},
	{2,4,0,3,1},
	{2,4,1,0,3},
	{2,4,1,3,0},
	{2,4,3,0,1},
	{2,4,3,1,0},
	{3,0,1,2,4},
	{3,0,1,4,2},
	{3,0,2,1,4},
	{3,0,2,4,1},
	{3,0,4,1,2},
	{3,0,4,2,1},
	{3,1,0,2,4},
	{3,1,0,4,2},
	{3,1,2,0,4},
	{3,1,2,4,0},
	{3,1,4,0,2},
	{3,1,4,2,0},
	{3,2,0,1,4},
	{3,2,0,4,1},
	{3,2,1,0,4},
	{3,2,1,4,0},
	{3,2,4,0,1},
	{3,2,4,1,0}, 
	{3,4,0,1,2},
	{3,4,0,2,1},
	{3,4,1,0,2},
	{3,4,1,2,0},
	{3,4,2,0,1},
	{3,4,2,1,0},
	{4,0,1,2,3},
	{4,0,1,3,2},
	{4,0,2,1,3},
	{4,0,2,3,1}, 
	{4,0,3,1,2},
	{4,0,3,2,1},
	{4,1,0,2,3},
	{4,1,0,3,2},
	{4,1,2,0,3},
	{4,1,2,3,0},
	{4,1,3,0,2},
	{4,1,3,2,0},
	{4,2,0,1,3},
	{4,2,0,3,1},
	{4,2,1,0,3},
	{4,2,1,3,0},
	{4,2,3,0,1},
	{4,2,3,1,0},
	{4,3,0,1,2},
	{4,3,0,2,1},
	{4,3,1,0,2},
	{4,3,1,2,0},
	{4,3,2,0,1},
	{4,3,2,1,0},
}

type S5 struct {

}

func (G S5) inv(a int)int {
	n:=len(gS5)
	for i:=0;i<n;i++{
		if(G.mul(a,i)==0){
			return i
		}
	}
	return -1
}

func (G S5) printTable() {
   fmt.Printf("GAP[120,34]=S_5:\n")
   printGroup(G)
}

func (G S5) mul(a,b int)int {
	var ret []int=[]int{0,0,0,0,0}
	tArr:=gS5[a]
	aArr:=gS5[b]	
	ret[0]=aArr[tArr[0]]
	ret[1]=aArr[tArr[1]]
	ret[2]=aArr[tArr[2]]
	ret[3]=aArr[tArr[3]]
	ret[4]=aArr[tArr[4]]	
	n:=len(gS5)
	for i:=0;i<n;i++{
		if(ret[0]==gS5[i][0] && ret[1]==gS5[i][1] && ret[2]==gS5[i][2] && ret[3]==gS5[i][3] && ret[4]==gS5[i][4]){
			return i		
		}
	}	
	return -1
}

func (G S5) size()int {
   return 120
}

var gC3 [][]int=[][]int{
	{1,2,3},
	{2,3,1},
	{3,1,2},
}

type C3 struct {

}

func (G C3) inv(a int)int {
	n:=len(gC3)
	for i:=0;i<n;i++{
		if(G.mul(a,i)==0){
			return i
		}
	}
	return -1
}

func (G C3) printTable() {
   fmt.Printf("GAP[3,1]=C_3:\n")
   printGroup(G)
}

func (G C3) mul(a,b int)int {
	var ret []int=[]int{0,0,0}
	tArr:=gC3[a]
	aArr:=gC3[b]	
	ret[0]=aArr[tArr[0]-1]
	ret[1]=aArr[tArr[1]-1]
	ret[2]=aArr[tArr[2]-1]
	n:=len(gC3)
	for i:=0;i<n;i++{
		if(ret[0]==gC3[i][0] && ret[1]==gC3[i][1] && ret[2]==gC3[i][2]){
			return i		
		}
	}	
	return -1
}

func (G C3) size()int {
   return 3
}

var gS4 [][]int=[][]int{
	{1,2,3,4},
	{2,1,3,4},
	{1,2,4,3},
	{3,2,1,4},
	{1,4,3,2},
	{4,2,3,1},
	{1,3,2,4},
	{2,1,4,3},
	{3,4,1,2},
	{4,3,2,1},
	{2,3,1,4},
	{3,1,2,4},
	{3,2,4,1},
	{4,2,1,3},
	{2,4,3,1},
	{4,1,3,2},
	{1,3,4,2},
	{1,4,2,3},
	{2,3,4,1},
	{2,4,1,3},
	{3,4,2,1},
	{3,1,4,2},
	{4,3,1,2},
	{4,1,2,3},
}

type S4 struct {

}

func (G S4) inv(a int)int {
	n:=len(gS4)
	for i:=0;i<n;i++{
		if(G.mul(a,i)==0){
			return i
		}
	}
	return -1
}

func (G S4) printTable() {
   fmt.Printf("GAP[24,12]=S_4=Oh:\n")
   printGroup(G)
}

func (G S4) mul(a,b int)int {
	var ret []int=[]int{0,0,0,0}
	tArr:=gS4[a]
	aArr:=gS4[b]	
	ret[0]=aArr[tArr[0]-1]
	ret[1]=aArr[tArr[1]-1]
	ret[2]=aArr[tArr[2]-1]
	ret[3]=aArr[tArr[3]-1]
	n:=len(gS4)
	for i:=0;i<n;i++{
		if(ret[0]==gS4[i][0] && ret[1]==gS4[i][1] && ret[2]==gS4[i][2] && ret[3]==gS4[i][3]){
			return i		
		}
	}	
	return -1
}

func (G S4) size()int {
   return 24
}


var gSL2F3 [][]int=[][]int{
	{1, 0 , 0, 1}, 
	{0, 1 , 2, 0},
	{0, 1 , 2, 1},
	{0, 1 , 2, 2},
	{0, 2 , 1, 0},
	{0, 2 , 1, 1},
	{0, 2 , 1, 2},
	{1, 0 , 1, 1},
	{1, 0 , 2, 1},
	{1, 1 , 0, 1},
	{1, 1 , 1, 2},
	{1, 1 , 2, 0},
	{1, 2 , 0, 1},
	{1, 2 , 1, 0},
	{1, 2 , 2, 2},
	{2, 0 , 0, 2},
	{2, 0 , 1, 2},
	{2, 0 , 2, 2}, 
	{2, 1 , 0, 2},
	{2, 1 , 1, 1},
	{2, 1 , 2, 0},
	{2, 2 , 0, 2},
	{2, 2 , 1, 0},
	{2, 2 , 2, 1},
}

type SL2F3 struct {

}

func (G SL2F3) inv(a int)int {
	n:=len(gSL2F3)
	for i:=0;i<n;i++{
		if(G.mul(a,i)==0){
			return i
		}
	}
	return -1
}

func (G SL2F3) printTable() {
   fmt.Printf("GAP[24,3]=SL(2,3):\n")
   printGroup(G)
}

func (G SL2F3) mul(a,b int)int {
	var ret []int=[]int{0,0,0,0}
	tArr:=gSL2F3[a]
	aArr:=gSL2F3[b]	
	ret[0]=(tArr[0] * aArr[0]+tArr[1] * aArr[2])%3
	ret[1]=(tArr[0] * aArr[1]+tArr[1] * aArr[3])%3
	ret[2]=(tArr[2] * aArr[0]+tArr[3] * aArr[2])%3
	ret[3]=(tArr[2] * aArr[1]+tArr[3] * aArr[3])%3	
	n:=len(gSL2F3)
	for i:=0;i<n;i++{
		if(ret[0]==gSL2F3[i][0] && ret[1]==gSL2F3[i][1] && ret[2]==gSL2F3[i][2] && ret[3]==gSL2F3[i][3]){
			return i		
		}
	}	
	return -1
}

func (G SL2F3) size()int {
   return 24
}


func printGroupStruct(g IGroup,s string){
    fmt.Printf("群%s的不变量:",s)
    fmt.Println("N0=",calN0(g),",bA=",calbA(g),",bC=",calbC(g),",Z=",calZ(g))
}

func NSGRetInfo(isN int)string{
	if(isN==0){
		return "是正规子群"	
	}else if(isN==1){
		return "不是正规子群"
	}else if(isN==2){
		return "不是子群"
	}
	return ""
}

func main() {
    s3 := S3{}
    s3.printTable()
    printGroupStruct(s3,"GAP[6,1]")	
	Test1:=[][]int{{0,1,2},{0,3}}
	Test2:=[]int{0,0}
    for i:=0;i<len(Test1);i++{	
		isN:=isnormalsubgroup(s3,&Test1[i],Test2[i])
		fmt.Println(Test1[i],NSGRetInfo(isN))	
	}	
	c3:=C3{}
    c3.printTable()
    printGroupStruct(c3,"GAP[3,1]")		
    s5 := S5{}
    //s5.printTable()
    printGroupStruct(s5,"GAP[120,34]")
    sl2f3 := SL2F3{}
    sl2f3.printTable()
    printGroupStruct(sl2f3,"GAP[24,3]")	
    s4 := S4{}
    s4.printTable()
    printGroupStruct(s4,"GAP[24,12]")		
}