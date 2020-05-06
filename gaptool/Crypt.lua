function EnCrypt(t,s)
    local Crypted=""
	local n=string.len(s)
	local X1=0
	for i=1,n do
		local sana=tonumber(string.sub(s,i,i))
		X1=X1+sana
	end
	X1=math.floor(0.5+(X1*0.1)/6)
	local s1=X1
	local G=0
	local m=string.len(t)
	for i=1,m do
		local sana=tonumber(string.sub(s,i,i))
		G=G+1
		if G==6 then
			G=0
			X1=0
		elseif G==0 then
			X1=sana-(s1-2)
		elseif G==1 then
			X1=sana+(s1-5)
		elseif G==2 then
			X1=sana-(s1-4)
		elseif G==3 then
			X1=sana+(s1-2)
		elseif G==4 then
			X1=sana-(s1-3)
		elseif G==5 then
			X1=sana+(s1-5)
		end
		X1=X1+G
		Crypted=Crypted..tostring(X1)
	end
	return Crypted
end

local msg=EnCrypt("Hanxiaohua","pwd123")
print(msg)