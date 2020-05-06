--利用欧几里得算法计算两个数的最大公约数
function gcd(a,b)
 if(b == 0)then return a end
 if(a == 0)then return b end
 if(a%b == 0)then return b
 else return gcd(b, a%b) end
end

function totient(num)
 local ret={}--这里的括号不能省略
 if(num==1)then
  ret[1]=1
  --table.insert(ret,1)
  return ret
 end
 for i=1,num-1,1 do
  if(gcd(num,i)==1)then
   table.insert(ret,i)
  end
 end
 return ret
end

function get_len(tb)
	local len=0
	for k,v in pairs(tb) do
		len=len+1
	end
	return len
end

function get_key(v1,tb)
	for k,v in pairs(tb) do
		if v==v1 then
		  return k
		end
	end
	return -1
end

--输出Abel群U(n)=(Z/nZ)^*的凯莱表
function UnMulTable(n,v)
 local ret={}
 local m=get_len(v)
 for i=1,m,1 do
  local I={}
  for j=1,m,1 do
   local ij=(v[i]*v[j])%n
   local index=get_key(ij,v)
   I[j]=index
  end
  ret[i]=I
 end
 return ret
end

function PrintArr2(vv) 
 local m=get_len(vv)
 for i=1,m,1 do
  for j=1,m,1 do
   io.write(vv[i][j]," ")
  end
  io.write("\n")
 end 
end
--[[]]

function PrintArr(v) 
 local m=get_len(v)
  for i=1,m,1 do
   io.write(v[i]," ")
  end
  io.write("\n")
end

--U(8) ={[1]_8,[3]_8,[5]_8,[7]_8}=K_4≠C_4，
--U(15)=U(3)×U(5)=C_2×C_4，
local U8Set=totient(8)
local U8=UnMulTable(8,U8Set)
PrintArr(U8Set) 
PrintArr2(U8)

local U15Set=totient(15)
local U15=UnMulTable(15,U15Set)
PrintArr(U15Set) 
PrintArr2(U15)
--[[]]
