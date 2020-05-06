package main

import (
	"fmt"
	"math"	
)

/* =========================== Begin of of function IsPrime、PrimitiveRootMod、LogMod ======================== */
func gcd(a,b int)int{
	if(a*b<0){
	    a1:=a
		if(a<0){
			a1=-a
		}
	    b1:=b
		if(b<0){
			b1=-b
		}	
		return -gcd(a1,b1)
	}
	var temp int=0
	if(a<b){
		temp=a
		a=b
		b=temp
	}
	if(a%b==0){
		return b
	}else{
		return gcd(a%b,b)
	}
	return 0
}

func Iscoprime(a,b int)int{
	var ret int=0
	if(gcd(a,b)==1){
		ret=1
	}
	return ret
}

func euler(num int)int{
	var count int=0
	if(num==1){
		return 1
	}
	for i:=1;i<=num-1;i++ {
	   count=count+Iscoprime(num,i)
	}
	return count
}

func powd(x,y int)int{
	var ret int=1
	for i:=1;i<=y;i++ {
	   ret=ret*x
	}	
	return ret
}

func mod(n, p int)int{
	if(n<0){
       ret1:=n+(-n+1)*p
 	   return ret1%p
	}
	return n%p	
} 

func inv(u, p int)int{
    var t1 int= 0
    var t3 int= 0
    var q  int= 0
    var u1 int= 1
    var u3 int= u
    var v1 int= 0
    var v3 int= p
    var inv_v int= 0

    for{
        q = (int)(u3 / v3)
        t1 = u1 - v1 * q
        t3 = u3 - v3 * q
        u1 = v1
        u3 = v3
        v1 = t1
        v3 = t3
		if( v3 == 0){
			break
		}
    }
    inv_v = mod( u1, p )
    if ( mod( u * inv_v, p ) != 1){
		return 0// 返回值为0表示模逆不存在
	}
    return inv_v
}

func mpower(a,k,p int)int{
	if(k==0){
		return 1
	}
	if(k<0){
		b:=inv(a,p)
		ret:=mpower(b,-k,p)
		return ret
	}
	var ans int=1
	for i:=1;i<=k;i++ {
	   ans=ans*a%p
	}
	return ans
}

// 穷搜计算log_a(b)mod(p)
func LogMod(b,a,p int)int{
	var k int=0
	for{
		if(mpower(a,k,p)==b){
			break
		}
		k=k+1
		if(k>p){
			break
		}
	}
	return k
}

// Error:o_p(a)=log_a(1)mod(p)
func orderm(a,p int)int{
    //Error:k:=LogMod(1,a,p)	
	var k int=1
	for{
		if(mpower(a,k,p)==1){
			break
		}
		k=k+1
		if(k>p){
			break
		}
	}
	return k
}

func is_primitive_root(a,p int)bool{
	k:=orderm(a,p)
	ret:=(k==euler(p))
	return ret
}

func PrimitiveRootMod(p int)int{   
	for k:=1;k<p;k++{
		bFind:=is_primitive_root(k,p)
		if(bFind){
			return k
		}
	}
	return 0
}

func IsPrime(i int)bool{
	if(i==1||i==0){
		return false
	}
	if(i==2||i==3){
		return true
	}
	for j:=2;j<=int(math.Sqrt(float64(i)));j++ {
	  if(i%j!=0){
		  return true
	  }
	}
	return false
}

/* ======================= End of function IsPrime、PrimitiveRootMod、LogMod ======================= */

type ZmodnZObj struct{
  m_k,m_mod int
}

// operator==
func (this *ZmodnZObj)isEqual(a ZmodnZObj)bool{
		return (this.m_mod==a.m_mod && this.m_k==a.m_k)
}

// 加法逆元
func (this *ZmodnZObj)InvAdd()ZmodnZObj{
	var ret ZmodnZObj
	ret.m_mod=this.m_mod
	ret.m_k=(this.m_mod-this.m_k)%this.m_mod
	return ret
}

// 剩余类环中的加法operator+
func (this *ZmodnZObj)Add(a ZmodnZObj)ZmodnZObj{
	var ret ZmodnZObj
	ret.m_mod=this.m_mod
	ret.m_k=(this.m_k+a.m_k)%this.m_mod
	return ret
}

// 剩余类环中的乘法operator*
func (this *ZmodnZObj)Mul(a ZmodnZObj)ZmodnZObj{
	var ret ZmodnZObj
	ret.m_mod=this.m_mod
	ret.m_k=(this.m_k*a.m_k)%this.m_mod
	return ret
}

func (this *ZmodnZObj)getName()string{
	var bP=IsPrime(this.m_mod)
	if(bP){
		var s string
		if(this.m_k==0){
		    s=fmt.Sprintf("0*Z(%d)",this.m_mod)
		}else{
		    a:=PrimitiveRootMod(this.m_mod)
			if(this.m_k==a){
				s=fmt.Sprintf("Z(%d)",this.m_mod)
			}else{
				// k=log_a(b)mod(p)
				k:=LogMod(this.m_k,a,this.m_mod)
				s=fmt.Sprintf("Z(%d)^%d",this.m_mod,k)
			}
		}
		return s
	}else{
		var s string
		s=fmt.Sprintf("ZmodnZObj(%d,%d)",this.m_k,this.m_mod)
		return s
	}
	return ""
}

func ZmodnZ(n int)[]ZmodnZObj{
	var v []ZmodnZObj
	for i:=0;i<n;i++{
	   v=append(v,ZmodnZObj{i,n})
	}
	return v
}

func PrintElements(v *[]ZmodnZObj){
   fmt.Printf("[")
   for i:=0;i<len(*v);i++{
       fmt.Printf((*v)[i].getName())
	   if(i<len(*v)-1){
			fmt.Printf(",") 
	   }
   }
   fmt.Printf("]\n")   
}

func main(){
   //fmt.Println("LogMod(1,2,3)=",LogMod(1,2,3)) 
   //fmt.Println("LogMod(2,2,3)=",LogMod(2,2,3)) 
   for i:=2;i<=8;i++{
	   //fmt.Printf("PrimitiveRootMod(%d)=%d\n",i,PrimitiveRootMod(i))       
       Zi:=ZmodnZ(i)
	   PrintElements(&Zi)
   }     
}