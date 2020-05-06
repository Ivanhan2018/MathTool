package main

import(
    "os"
	"encoding/json"
	"fmt"
	"sort"
	"strings"	
	"strconv"	
	"regexp"
)

func PermutationMat(a *[]int16)[][]int16{
	n:=int16(len(*a))
	var ret [][]int16=make([][]int16,n)
	var i int16=0
	var j int16=0	
	for i=0;i<n;i++{
		var v []int16=make([]int16,n)
		J:=(*a)[i]
		for j=0;j<n;j++{
		    if(j==J-1){
			   v[j]=1
			}else{
			   v[j]=0
			}
		}	
        ret[i]=v		
	}
	return ret
}

// GAP4置换表达式->双行置换表达式
func Str2Perm(s string,n int)[]int16{
	var ret []int16=IdentityPerm(n)
	if(s=="()"){		
		return ret
	}
	ss:=MySplit(s)
	M:=len(ss)
	for i:=0;i<M;i++{
		arr:=strings.Split(ss[i],",")
		N:=len(arr)
		j0:=MyAtoi(arr[0])
		//fmt.Printf("M=%d,i=%d,N=%d,j0=%d\n",M,i,N,j0)		
		for j:=1;j<N;j++{
		    idx:=MyAtoi(arr[j-1])
			//fmt.Printf("j=%d,idx=%d\n",j,idx)
			ret[idx-1]=MyAtoi(arr[j]) 			
		}
		jL:=MyAtoi(arr[N-1])
		ret[jL-1]=j0
	}	
    return ret
}

func MyAtoi(s string)int16{
	n,r:=strconv.Atoi(s)
	if(r==nil){
	 return int16(n)
	} 
	return 0
}

// 截取出包含在()里面的内容
func MySplit(s string)[]string{
    rex:=regexp.MustCompile("\\((.*?)\\)")
	out:=rex.FindAllStringSubmatch(s,-1)
	var ss []string=make([]string,len(out))
	for k,v :=range out{
	   ss[k]=v[1]
	}
	return ss
}

func Deg(s string)int{
    s=strings.Replace(s,")",",",-1)
    s=strings.Replace(s,"(",",",-1)	
    arr:=strings.Split(s,",")
	max:=0
    for _, v := range arr {
	   n,r:=strconv.Atoi(v)
	   if(r==nil && n>max){
	     max=n
		}  
	}
	//fmt.Printf("max=%d\n",max)
    return max
}

func Degree(s *[]string)int{
	max:=0
    for _, v := range *s {
	   n:=Deg(v)
	   if(n>max){
	     max=n
		}  
	}
	//fmt.Printf("max=%d\n",max)
    return max
}

// 双行置换表达式->GAP4置换表达式
func Perm2Str(a *[]int16)string{
	I:=IdentityPerm(len(*a))
	bI:=IsEqual(a,&I)
	if(bI){
		return "()"
	}
	var s string=""
	var s1 string=""	
	n:=int16(len(*a))
	var i int16=0	
	var v []int16=[]int16{}
	var set []int16=[]int16{}	
	for{	
		for i=1;i<=n;i++{
			if(getpos(&set,i)==-1){
			   set=append(set,i)
				var I int16=(*a)[i-1]
				if(getpos(&set,I)==-1){
				   set=append(set,I)
				}				
				nv:=int16(len(v))			
				if(I!=i){					
					if(nv>0 && v[nv-1]==i){
						v=v[:nv-1]
					}					
					if(getpos(&v,i)==-1){
					   v=append(v,i)
					}
					if(getpos(&v,I)==-1){
					   v=append(v,I)
					}				
					if(v[0]==(*a)[I-1]){					
						break
					}					
				}
				nv=int16(len(v))			
				if(int16(len(set))==n){
				   break
				}			   
			}
		}
		s1=Vec2Str(&v)
		if(s!="" && s1=="()"){
			s1=""	
		}
		s=s+s1
		v=v[:0]
		if(int16(len(set))==n){
		   break
		}
	}
    return s
}

func Vec2Str(v *[]int16)string{
	var s1 string="("
	n:=int16(len(*v))
	var i int16=0
	for ;i<n;i++{
	    s1=s1+fmt.Sprintf("%d",(*v)[i])
		if(i<n-1){
			s1=s1+","
		}
	}
    s1=s1+")"
	return s1
}

func getpos(v *[]int16,a int16)int16{
	n:=int16(len(*v))
	var i int16=0
	for ;i<n;i++{
		if((*v)[i]==a){
			return i
		}
	}
	return -1    
}

func IdentityPerm(n int)[]int16{
	var c []int16=make([]int16,n)
	for i:=0;i<n;i++{
		c[i]=int16(i+1)
	}
	return c    
}

func IsEqual(a *[]int16,b *[]int16)bool{
	na:=len(*a)
	nb:=len(*b)
	if(na!=nb){
	  return false
	}
	var i int16=0
	for ;i<int16(na);i++{
		if((*a)[i]!=(*b)[i]){
			return false
		}
	}
    return true
}

// 满足sort包的sort.Interface接口
type int16s []int16
func (s int16s)Len()int {return len(s)}
func (s int16s)Less(i,j int)bool {return s[i]<s[j]}
func (s int16s)Swap(i,j int) {s[i],s[j]=s[j],s[i]}

func IsPerm(a []int16)bool{
    //sort.Ints(a)
	sort.Sort(int16s(a))
	I:=IdentityPerm(len(a))
	bI:=IsEqual(&a,&I)
	return bI
/*
	n:=int16(len(a))
	var i int16=0
	for ;i<n;i++{
		if(a[i]!=i+1){
			return false
		}
	}
    return true
*/	
}

func Mul(a *[]int16,b *[]int16)[]int16{
	na:=len(*a)
	nb:=len(*b)
	if(na!=nb){
	  return []int16{}
	}
	var c []int16=make([]int16,na)
	for i:=0;i<na;i++{
		c[i]=(*a)[(*b)[i]-1]
	}
	return c
}

func getidx(vv *[][]int16,a *[]int16)int16{
	n:=len(*vv)
	na:=len(*a)
	for i:=0;i<n;i++{
	    if(len((*vv)[i])==na){
		    bFind:=true
			for j:=0;j<na;j++{
				if((*vv)[i][j]!=(*a)[j]){
				   bFind=false
				   break
				}
			}
			if(bFind){
			   return int16(i)
			}
		}
	}
	return -1
}

func TableByPermuteSet(FG *[][]int16)[][]int16{
	n:=len(*FG)
	var vv [][]int16=make([][]int16,n)	
	for i:=0;i<n;i++{
	    var v []int16=make([]int16,n)	
		for j:=0;j<n;j++{
			ij:=Mul(&(*FG)[i],&(*FG)[j])
			v[j]=getidx(FG,&ij)+1
		}
		vv[i]=v
	}	
    return vv
}


func printTable(vv *[][]int16){
   n:=len(*vv)  
   for i:=0;i<n;i++{
	   for j:=0;j<n;j++{
		  fmt.Printf("%d ",(*vv)[i][j]) 
	   } 
       fmt.Printf("\n")	   
   }
}

func test() {
    //a:=[]int16{1,4,2}
	//ret:=IsPerm(a)
	//fmt.Printf("%v %v\n",a,ret)
	
    var s3 [][]int16=[][]int16{{1,2,3},{2,3,1},{3,1,2},{3,2,1},{1,3,2},{2,1,3}}
	S3:=TableByPermuteSet(&s3)
	printTable(&S3)
	
	n:=len(s3)
	for i:=0;i<n;i++{
	  fmt.Printf("%v->%v->%v\n",s3[i],Perm2Str(&s3[i]),PermutationMat(&s3[i])) 
	} 
	
	var sd3 []string=[]string{"()","(2,3)","(1,2)","(1,2,3)","(1,3,2)","(1,3)"}
	n=len(sd3)
	d:=Degree(&sd3)
	for i:=0;i<n;i++{
	  d3Perm:=Str2Perm(sd3[i],d)
	  fmt.Printf("%v->%v->%v\n",sd3[i],d3Perm,PermutationMat(&d3Perm)) 
	} 	
	
    var d4 [][]int16=[][]int16{{1,2,3,4},{2,1,3,4},{1,2,4,3},{2,1,4,3},{3,4,1,2},{4,3,1,2},{3,4,2,1},{4,3,2,1}}
	D4:=TableByPermuteSet(&d4)
	printTable(&D4)
	
	n=len(d4)	
	for i:=0;i<n;i++{
	  fmt.Printf("%v->%v->%v\n",d4[i],Perm2Str(&d4[i]),PermutationMat(&d4[i])) 
	} 	
	
	var sd4 []string=[]string{"()","(1,2)","(3,4)","(1,2)(3,4)","(1,3)(2,4)","(1,4,2,3)","(1,3,2,4)","(1,4)(2,3)"}
	n=len(sd4)
	d=Degree(&sd4)	
	for i:=0;i<n;i++{
	  d4Perm:=Str2Perm(sd4[i],d)	  
	  fmt.Printf("%v->%v->%v\n",sd4[i],Str2Perm(sd4[i],d),PermutationMat(&d4Perm)) 
	} 	
	
}

func main() {
    if(false){
         str:="(1,2,3,7,4,8,11,18,5,9,12,19,14,21,25,32)(6,24,13,35,15,37,26,44,16,38,27,45,29,46,39,48)(10,28,20,30,22,40,33,31,23,41,34,42,36,47,43,17)"
		 d:=Deg(str)
		 Perm:=Str2Perm(str,d)
		 fmt.Printf("%v->%v\n",str,Perm) 
		 return
	}
     var str string
	 idx1:=strings.Index(os.Args[0],"Str2Perm")
	 idx2:=strings.Index(os.Args[0],"Perm2Str")
	 if(idx1>-1){
		if(len(os.Args)<2){
		 fmt.Printf("usage: Str2Perm str:\n")
		 fmt.Printf("Please input the str:\n")
		 fmt.Scanf("%s",&str)
		}else{
		 str=os.Args[1]	 
		}
		d:=Deg(str)
		Perm:=Str2Perm(str,d)
		b,err:=json.Marshal(Perm)
		if(err!=nil){
		panic(err)
		}
		fmt.Printf("%v\n",string(b)) 			 
	 }else if(idx2>-1){
		 if(len(os.Args)<2){
			 fmt.Printf("usage: Perm2Str Perm:\n")
			 fmt.Printf("Please input the Perm:\n")
			 fmt.Scanf("%s",&str)
		 }else{
			 str=os.Args[1]	 
		 }
		arr:=strings.Split(str,",")
		N:=len(arr)
		var iarr []int16=IdentityPerm(N)	
		for i:=0;i<N;i++{
		    iarr[i]=MyAtoi(arr[i])		
		}
		ret:=Perm2Str(&iarr)
		fmt.Printf("%v\n",ret) 		 
	 }
}