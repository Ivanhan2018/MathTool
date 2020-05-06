--*.lua在Windows 7下使用ANSI编码，非UTF-8编码

function Str2Arr(s)
	local t={}
	local n=string.len(s)
	for i=1,n do
		table.insert(t,string.byte(string.sub(s,i,i)))
	end
	return t
end

function Arr2Str(t)
	local s=""
	for k,v in pairs(t) do
		s=s..string.format("%c",v)
	end
	return s
end

--升序排序
function pcqck(arr,p1,p2)   
	local ch=arr[p1]
	local i=p1  
	local j=p2  
	while(i<j)do
		while(arr[j]>=ch and i<j)do
		 j=j-1
        end		 
		arr[i]=arr[j]
		while(arr[i]<=ch and i<j)do
		 i=i+1
		end
		arr[j]=arr[i] 
	end  
	arr[i]=ch 
	if(i-1>p1)then
		pcqck(arr,p1,i-1)
	end
	if(p2>i+1)then
		pcqck(arr,i+1,p2)
	end	
end 

function doArrAB(arr,F,A,B)
	F(arr,A,B)
	return arr
end

function doArr(arr,F)
	F(arr)
	return arr
end

function get_len(tb)
	local len=0
	for k,v in pairs(tb) do
		len=len+1
	end
	return len
end

function myreverse(t)
	local s=1--头指针
	local p=get_len(t)--尾指针
	while(s<p)do   
		t[s],t[p]=t[p],t[s]
		s=s+1
		p=p-1
    end
end

--查找s2在s1中出现的次数
function myfindcnt(s1,s2)
	local n1=string.len(s1)
	local n2=string.len(s2)
	local sum=0
	for i=1,n1,1 do
		if string.sub(s1,i,i+n2-1)==s2 then
			sum=sum+1
		end
	end
	return sum
end

local sArr = {"Hanxiao","我们C/C++ Tips","qwertyuiopasdfghjklzxcvbnm"}
for i=1,3,1 do
	local s=sArr[i]
	local s1=string.lower(s)--把字符串转换为全小写
	local s2=string.upper(s)--把字符串转换为全大写
	local s3=string.reverse(s)--把字符串翻转
	local s3a=Arr2Str(doArr(Str2Arr(s),myreverse))--把字符串翻转	
	local s4=string.sub(s,1,4)
	local slen=string.len(s)
    local p1,p2=string.find(s,"xi")
    local p3=myfindcnt(s,"xi")	
	local s5=Arr2Str(Str2Arr(s))
	local s6=Arr2Str(doArrAB(Str2Arr(s),pcqck,1,slen))	 	
	print(s,"->",s1)
	print(s,"->",s2)
	print(s,"->",s3)
	print(s,"->",s3a)	
	print(s,"->",s4)	
	print(s,"->",slen)
	print(s,"->",p1,p2)
	print(s,"->",p3)	
	print(s,"->",s5)
	print(s,"->",s6)	
end
local s1="a1b1"
local s2="a%da%d"
local si,sj=string.find(s1,s2)
print(si,sj)
if si~=nil and sj~=nil then
local sn=string.sub(s1,si+1,si+1)+string.sub(s1,si+3,si+3)
sn=sn%6
local s3="a"..sn
if sn==0 then
s3=""
end
s1,cnt=string.gsub(s1,s2,s3)
print(s1,cnt)
end