/*
扩展欧几里得算法，可以构造出u u1 + v u2 = u3 = gcd( u, v ).
现令v = p是素数，可知gcd = u3 = 1，我们得到 u u1 -1  = p u2
u u1=1 (mod p)
u1=InvMod(u,p)
v不是素数时，u u1 = 0 (mod v)时会导致u u1 = 1 (mod v)无解，即模逆不存在（InvMod(u,v)=0）

定理：Z/(p)为域当且仅当p为素数。
证 设p不是素数，那么Z/(p)有零因子（p的因子），故Z/(p)不是域．
反之，当p为素数时，可证Z/(p)中所有非零元素都有乘法运算的逆元，从而含么交换环Z/(p)为域．
设q是Z/(p)中任一非零元素，那么q与p互素．据数论事实,有整数m,n使mp + nq = 1
从而(mp+nq)(mod p) = 1
即mp(mod p)+nq(mod p) = 1
0 + n(mod p)*q(mod p)= 1, 或 n(mod p)*q=1
因此,q有逆元n(mod p) .
定理得证。
*/
package main

import (
	"fmt"
	"os"
	"strconv"	
)

func mod(n, p int)int{
/*
	raw_mod := n % p
	if (raw_mod == 0){
			return 0
		}else if (n >= 0){
			return raw_mod
		}else{
			return raw_mod + p
		}
*/
	if(n<0){
       ret1:=n+(-n+1)*p
 	   return ret1%p
	}
	return n%p	
} 

func inverse_mod_p(u, p int)int{
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

func main(){
     var a,n,b int
	 if(len(os.Args)<3){
		 fmt.Printf("usage: InvMod a n:\n")
		 fmt.Printf("Please input the number a,n:\n")
		 fmt.Scanf("%d,%d",&a,&n)
	 }else{
		 a,_=strconv.Atoi(os.Args[1])
		 n,_=strconv.Atoi(os.Args[2])	 
	 }
     b=inverse_mod_p(a,n)
     fmt.Printf("PowerMod(%d,-1,%d)=%d\n",a,n,b)
}