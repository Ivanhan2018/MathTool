function get_len(tb)
	local len=0
	for k,v in pairs(tb) do
		len=len+1
	end
	return len
end

function simplify0(s)
   local ret=s 
   local n=string.len(ret)
   if n==0 then
      ret="a0b0"
   elseif n==2 then
      if(string.sub(ret,1,1)=="a")then
	     ret=ret.."b0"
	  elseif(string.sub(ret,1,1)=="b")then
	     ret="a0"..ret
	  end
   end   
   return ret 
end

function simplify1(s)
   local ret=s
   local n1=string.find(ret,"a0")
   local n2=string.find(ret,"b0") 
   local n=string.len(ret)   
   while(n>4 and (n1~=nil or n2~=nil))do
      if n1~=nil then
	      ret,cnt=string.gsub(ret,"a0","")
	  elseif n2~=nil then
	      ret,cnt=string.gsub(ret,"b0","")
	  end
	  n1=string.find(ret,"a0")
	  n2=string.find(ret,"b0") 	
      n=string.len(ret)	  
   end
   return ret
end

function simplify2(s,r)
   local ret=s 
   local n1=string.find(ret,"a%da%d")
   local n2=string.find(ret,"b%db%d")       
   while((n1~=nil or n2~=nil))do
      if n1~=nil then
			local sn=tonumber(string.sub(ret,n1+1,n1+1))+tonumber(string.sub(ret,n1+3,n1+3))
			sn=sn%6
			local s3="a"..sn
			if sn==0 then
				s3=""
			end		  
			ret,cnt=string.gsub(ret,"a%da%d",s3)
	  elseif n2~=nil then
			local sn=tonumber(string.sub(ret,n2+1,n2+1))+tonumber(string.sub(ret,n2+3,n2+3))
			local s3=""
			if sn==0 then
				s3=""
			elseif sn==1 then
			    s3="b1"
			elseif sn==2 then
			    s3=r--"a3"
			end		  
	      ret,cnt=string.gsub(ret,"b%db%d",s3)
	  end
	  n1=string.find(ret,"a%da%d")
	  n2=string.find(ret,"b%db%d") 	
      n=string.len(ret)	 	  
   end 
   return ret
end

function simplify3(s,r)
	local ret=s
	local n1=string.find(ret,"b%da1b%d")
	local n2=string.find(ret,"b0a1")	
	local n3=string.find(ret,"a1b0")	
	while((n1~=nil) and (n2==nil and n3==nil))do	
		local s1=tonumber(string.sub(ret,n1+1,n1+1))
		local s2=tonumber(string.sub(ret,n1+5,n1+5))		
		local s3=""
		if s1==1 then
		     s3=s3..r
	    else
		     s3=s3.."b"..(s1-1)..r
		end
		if s2>1 then
		     s3=s3.."b"..(s2-1)
		end
		ret,cnt=string.gsub(ret,"b%da1b%d",s3)
        n1=string.find(ret,"b%da1b%d")	
	    n2=string.find(ret,"b0a1")	
	    n3=string.find(ret,"a1b0")		
	end
	return ret
end

function simplify4(s,r)
	local ret=s
	local n1=string.find(ret,"b1a%d")
	local n2=string.find(ret,"b1a0")	
	while((n1==1) and (n2==nil))do	
		local s1=tonumber(string.sub(ret,n1+3,n1+3))
		local s3=""
		if s1==1 then
		     s3=r
		else
		     s3=r.."a"..(s1-1)
		end
		ret,cnt=string.gsub(ret,"b1a%d",s3)
        n1=string.find(ret,"b1a%d")	
		n2=string.find(ret,"b1a0")		
	end
	return ret
end

function simplify5(s,r)
	local ret=s
	local n1=string.find(ret,"a%db1a%d")
	local n2=string.find(ret,"a0b1")	
	local n3=string.find(ret,"b1a0")	
	while((n1~=nil) and (n2==nil and n3==nil))do	
		local s1=tonumber(string.sub(ret,n1+1,n1+1))
		local s2=tonumber(string.sub(ret,n1+5,n1+5))
		local s3=""
		if s1==1 then
		     s3=s3..r
	    else
		     s3=s3.."a"..(s1-1)..r
		end
		if s2==1 then
		     
		else
		     s3=s3.."a"..(s2-1)
		end		
		ret,cnt=string.gsub(ret,"a%db1a%d",s3)
        n1=string.find(ret,"a%db1a%d")
	    n2=string.find(ret,"a0b1")	
	    n3=string.find(ret,"b1a0")			
	end
	return ret
end
 
function Q12mul(s1,s2)
	local ret="a"..s1[1].."b"..s1[2].."a"..s2[1].."b"..s2[2]
	ret=simplify1(ret)  	
	ret=simplify2(ret,"a3") --根据b2=a3化简		
	ret=simplify3(ret,"a2") --根据bab=a2化简 
	ret=simplify4(ret,"a5b1") --根据ba=a5b化简  	
	ret=simplify5(ret,"b1") --根据aba=b化简  
	ret=simplify4(ret,"a5b1") --根据ba=a5b化简  	
	ret=simplify2(ret,"a3") --根据b2=a3化简
	ret=simplify5(ret,"b1") --根据aba=b化简 
	ret=simplify2(ret,"a3") --根据b2=a3化简	
	ret=simplify0(ret) 	
	local i=tonumber(string.sub(ret,2,2))
	local j=tonumber(string.sub(ret,4,4))
	return i+6*j+1
end

function PrintTable(tb,Mul)
    local n=get_len(tb)
	for i=1,n,1 do
		for j=1,n,1 do
			local ij=Mul(tb[i],tb[j])
			io.write(ij," ")
		end
		io.write("\n")
	end
end

--T={"a0b0","a1b0","a2b0","a3b0","a4b0","a5b0","a0b1","a1b1","a2b1","a3b1","a4b1","a5b1"}
local T={{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{0,1},{1,1},{2,1},{3,1},{4,1},{5,1}}
print("GAP[12,1]=T=Q_12=<a,b|a^6=1,b^2=a^3,ba=a^(-1)b>的凯莱表：")
PrintTable(T,Q12mul)

--print("GAP[8,4]=Q_8=<a,b|a^4=1,a^2=b^2,b^(-1)ab=a^(-1)>的凯莱表：")
--D_9=<a,b|a^9=b^2=1,bab^-1=a^8>

--GAP[24,1]=G24_4=U_24=U_6*4=<a,x|a^3=x^8=e,xax^(-1)=a^(-1)>：有6,8,12阶元
--GAP[24,8]=G24_10=V_24=V_8*3=<a,b|a^6=b^4=1,ba=a^(-1)b^(-1),b^(-1)a=a^(-1)b>：有6阶元，没有8,12阶元
