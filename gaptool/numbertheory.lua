--求2个数的最大公约数
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

for i=2,100,1 do
io.write("φ(",i,")=",euler(i),"\n")
end