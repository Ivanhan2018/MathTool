function get_len(tb)
	local len=0
	for k,v in pairs(tb) do
		len=len+1
	end
	return len
end

function ZeroMat(m,n)
	local ret={}
	for i=1,m,1 do
	    local I={}
		for j=1,n,1 do
		   table.insert(I,0)
		end
		table.insert(ret,I)
	end
	return ret
end

function OneMat(n)
	local ret={}
	for i=1,n,1 do
	    local I={}
		for j=1,n,1 do
	       if i==j then
			table.insert(I,1)
		   else
			table.insert(I,0)
		   end
		end
		table.insert(ret,I)
	end
	return ret
end

function PrintMat(vv) 
 local m=get_len(vv)
 for i=1,m,1 do
  local n=get_len(vv[i])
  for j=1,n,1 do
   io.write(vv[i][j]," ")
  end
  io.write("\n")
 end 
end

function bcmul(ar,ai,br,bi)
	local m=get_len(ar)
	local n=get_len(br)	
	local k=get_len(br[1])	
	local cr=ZeroMat(m,k)
	local ci=ZeroMat(m,k)		
	for i=1,m,1 do
		for j=1,k,1 do
			cr[i][j]=0
			ci[i][j]=0
			for l=1,n,1 do
			  local p=ar[i][l]*br[l][j]
			  local q=ai[i][l]*bi[l][j]
			  local s=(ar[i][l]+ai[i][l])*(br[l][j]+bi[l][j])
			  cr[i][j]=cr[i][j]+p-q
			  ci[i][j]=ci[i][j]+s-p-q
			 end
		 end
	end
  return cr,ci
end

--复矩阵相乘
function CMatMul(a,b)
	local cr,ci=bcmul(a[1],a[2],b[1],b[2])
	return {cr,ci}
end

local ar={
{-3,1,-1},
{-7,5,-1},
{-6,6,-2}
}
local ai=ZeroMat(3,3)
local br={
{8,-4,4},
{-8,12,4},
{-12,12,4}
}
local bi=ZeroMat(3,3)
PrintMat(ar)
PrintMat(ai)
PrintMat(br)
PrintMat(bi)
local cr,ci=bcmul(ar,ai,br,bi)
PrintMat(cr)
PrintMat(ci)
local C=CMatMul({ar,ai},{br,bi})
PrintMat(C[1])
PrintMat(C[2])
local E6={math.cos(2*math.pi/6),math.sin(2*math.pi/6)}
local a1={{{E6[1],0},{0,E6[1]}},{{E6[2],0},{0,-E6[2]}}}
local a2={{{0,-1},{1,0}},{{0,0},{0,0}}}
local a1a2=CMatMul(a1,a2)
PrintMat(a1a2[1])
PrintMat(a1a2[2])
PrintMat(a2[1])
PrintMat(a2[2])
--local CI2=CMatMul(a2,a2)
local CI2={{{1,0},{0,1}},{{0,0},{0,0}}}
--CI2[1]=OneMat(2)
--CI2[2]=ZeroMat(2)
print(get_len(CI2))
PrintMat(CI2[1])
PrintMat(CI2[2])