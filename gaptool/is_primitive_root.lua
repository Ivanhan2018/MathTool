function GCD(a,b) 
	if(a*b<0)then
		return -GCD(math.abs(a),math.abs(b))
	end
	if(a<b)then
		local temp=a
		a=b
		b=temp
	end
	if(a%b==0)then
		return b
	else
		return GCD(a%b,b)
	end
	return 0
end

function Iscoprime(a,b)
	local ret=0
	if(GCD(a,b)==1)then
		ret=1
	end
	return ret
end

function euler(n)
	local count=0
	if(n==1)then
		return 1
	end
	for i=1,n,1 do
	   count=count+Iscoprime(n,i)
	end
	return count
end

function mpower(a,k,p)
	local ans=1
	for i=1,k,1 do
		ans=(ans*a)%p
	end
	return ans
end

function orderm(a,p)
	local k=1
	while(k<=p)do
		if(mpower(a,k,p)==1)then
			return k
		end
		k=k+1
	end	
	return k
end

function is_primitive_root(a,p)
    local k=orderm(a,p)
	return k==euler(p)
end

--按原根定义求一个数的所有原根
function primroot(n)
    local ret={}
	for i=1,n,1 do
	   if(is_primitive_root(i,n)) then
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

function PrintArr(v) 
 local m=get_len(v)
  for i=1,m,1 do
   io.write(v[i]," ")
  end
  io.write("\n")
end

io.write("mpower(3,2,7)=",mpower(3,2,7),"\n")
io.write("o_7(3)=",orderm(3,7),"\n")
io.write("o_7(5)=",orderm(5,7),"\n")

for i=1,30,1 do
	local pr=primroot(i)
	if get_len(pr)>0 then
		io.write(i,"的原根:");
		PrintArr(pr)
	else
		io.write(i,"没有原根\n");
	end
end


