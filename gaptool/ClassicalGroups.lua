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

function is_equalM(a,b)
	local na=get_len(a)
	local nb=get_len(b)
	if na~=nb then
	   return false
	end
	for i=1,na,1 do
		local nai=get_len(a[i])
		local nbi=get_len(b[i])	
		if nai~=nbi then
			return false
		end	
		for j=1,nai,1 do	
			if math.abs(a[i][j]-b[i][j])>0.01 then
				return false
			end
		end
	end		
	return true
end

function get_keyM(v1,tb)
	for k,v in pairs(tb) do
		if is_equalM(v,v1) then
		   return k
		end
	end
	return -1
end

local function tbl_copy(tb)
	local tbtype=type(tb)
	local copy
	if tbtype=="table" then
		copy={}
		for k,v in next,tb,nil do
			copy[tbl_copy(k)]=tbl_copy(v)
		end
	else
		copy=tb
	end
	return copy
end

function OpFunc(tbl,a,b)
	local c=tbl[a+1][b+1]
	return c
end

function MAdd(a,b,Add)
	local na=get_len(a)
	local nb=get_len(b)
	if(na~=nb)then
		return nil
	end
	local n=na
	local c=tbl_copy(a)
	for i=1,n,1 do
		for j=1,n,1 do
			c[i][j]=OpFunc(Add,a[i][j],b[i][j])
		end
	end
	return c
end

function MMul(a,b,Add,Mul)
	local na=get_len(a)
	local nb=get_len(b)
	if(na~=nb)then
		return nil
	end
	local n=na
	local c=tbl_copy(a)
	for i=1,n,1 do
		for j=1,n,1 do
		    local sum=0
			for k=1,n,1 do
				sum=OpFunc(Add,sum,OpFunc(Mul,a[i][k],b[k][j]))
			end
			c[i][j]=sum
		end
	end
	return c
end

function MStr(a)
	local n=get_len(a)
    local s="["
	for i=1,n,1 do
	    s=s.."["
		for j=1,n,1 do
		    s=s..a[i][j]
			if j<n then
			   s=s..","
			end
		end
		s=s.."]"
		if i<n then
			s=s..","
		end
	end	
	s=s.."]"
	return s
end

function PrintMSet(G) 
	for k,v in pairs(G) do
		io.write(k,"->")
		io.write(MStr(v))
		io.write("\n")
	end
end

function PrintArr(v) 
 local m=get_len(v)
  for i=1,m,1 do
   io.write(v[i]," ")
  end
  io.write("\n")
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

function ZeroM(n)
	local I={}
	for i=1,n,1 do
	    local Ii={}
		for j=1,n,1 do
			Ii[j]=0
		end
		I[i]=Ii
	end
    return I
end

function OneM(n)
	local I={}
	for i=1,n,1 do
	    local Ii={}
		for j=1,n,1 do
		    if i==j then
				Ii[j]=1
			else
				Ii[j]=0
			end
		end
		I[i]=Ii
	end
    return I
end

function MRFG(Add,Mul,...)
	local args={...}
	local S={}
	local n=get_len(args[1])
	table.insert(S,OneM(n))
 	for i,v in pairs(args) do
		table.insert(S,v)
	end
	local cnt=0
	local cnt1=get_len(S)
	while(cnt1>cnt)do
		cnt=get_len(S)
		for ki,vi in pairs(S) do
			for kj,vj in pairs(S) do
				local ij=MMul(vi,vj,Add,Mul)
				local IJ=get_keyM(ij,S)
				if(IJ==-1)then
					table.insert(S,ij)
				end
			end
		end
		cnt1=get_len(S)
	end
	return S
end

function TableByMSet(G,Add,Mul)
	local ret={}
	for ki,vi in pairs(G) do
		local v={}
		for kj,vj in pairs(G) do
			local ij=MMul(vi,vj,Add,Mul)
			local IJ=get_keyM(ij,G)
			table.insert(v,IJ)
		end
		table.insert(ret,v)
	end
	return ret
end

function IsInCenterOfG(vvG,j)
	local N=get_len(vvG)
	for i=1,N,1 do
		local ij=vvG[i][j]
		local ji=vvG[j][i]
		if(ij~=ji) then
			return false
		end
	end
	return true
end

function CenterOfG(vvG)
	local ret={}
	local N=get_len(vvG)
	for i=1,N,1 do
		if(IsInCenterOfG(vvG,i))then
			table.insert(ret,i);
		end
	end
	return ret
end

function Inv(vvG,j)
	local N=get_len(vvG)
	for i=1,N,1 do
		local ij=vvG[i][j]
		if(ij==1)then
			return i
		end
	end
	return -1
end

function quotientGN(vvG,vN)
	local ret={}
	local G=get_len(vvG)
	local N=get_len(vN)
	for i=1,G,1 do
		local I={}
		for j=1,N,1 do
			local ij=vvG[i][vN[j]]
			table.insert(I,ij)
		end
		local bNew=true
		local m=get_len(ret)
		for k=1,m,1 do
			--判断I中的元素是否在ret中
			local p=get_key(I[1],ret[k])
			if(p~=-1)then
				bNew=false
				break
			end
		end
		if(bNew)then
			table.insert(ret,I);
		end
	end
	return ret
end

function quotientGNTable(vvG,vN)
	local ret1=quotientGN(vvG,vN)
	local G=get_len(vvG)
	local H=get_len(ret1)
	local ret={}
	for i=1,H,1 do
		local I={}
		for j=1,H,1 do
			local ij=vvG[ret1[i][1]][ret1[j][1]]
			local IJ=-1
			for k=1,H,1 do
				local p=get_key(ij,ret1[k])
				if(p~=-1)then
					IJ=k
					break
				end
			end
			I[j]=IJ
		end
		ret[i]=I
	end
	return ret
end

local g_F4Add={
	{0,1,2,3},
	{1,0,3,2},
	{2,3,0,1},
	{3,2,1,0}
}

local g_F4Mul={
	{0,0,0,0},
	{0,1,2,3},
	{0,2,3,1},
	{0,3,1,2}
}

function ZnZAddMul(n)
	local ret1={}
	local ret2={}	
	for i=1,n,1 do
	    local Ii1={}
	    local Ii2={}		
		for j=1,n,1 do
			Ii1[j]=(i+j-2)%n
			Ii2[j]=((i-1)*(j-1))%n
		end
		ret1[i]=Ii1
		ret2[i]=Ii2		
	end
    return ret1,ret2
end

function mdetgauss(mtxa)
	local nis
	local njs
	local d
	local f = 1
	local det = 1
	local n=get_len(mtxa)
	for k = 1,n - 1,1 do
		local q = 0
		for I = k,n,1 do
			for j = k,n,1 do
				d = math.abs(mtxa[I][j])
				if (d > q) then
					q = d
					nis = I
					njs = j
				end
			end
		end
		if (q + 1.0 == 1.0) then
			return 0
		end
		if (nis ~= k) then
			f = -f
			for j = k,n,1 do
				d = mtxa[k][j]
				mtxa[k][j] = mtxa[nis][j]
				mtxa[nis][j] = d
			end
		end
		if (njs ~= k) then
			f = -f
			for I = k,n,1 do
				d = mtxa[I][njs]
				mtxa[I][njs] = mtxa[I][k]
				mtxa[I][k] = d
			end
		end
		det = det * mtxa[k][k]
		for I = k + 1,n,1 do
			d = mtxa[I][k] / mtxa[k][k]
			for j = k + 1,n,1 do
				mtxa[I][j] = mtxa[I][j] - d * mtxa[k][j]
			end
		end
	end
	det = f * det * mtxa[n][n]
	return det
end

function bsdet(a)
    local n=get_len(a)
    local q,d
    local f=1.0
	local det=1.0
    for k=1,n-1,1 do
        q=0.0
        for i=k,n,1 do
			for j=k,n,1 do
				d=math.abs(a[i][j])
				if (d>q)then
					q=d
					is=i
					js=j
				end
			end
		end
        if (q+1.0==1.0)then
          det=0.0
		  return(det)
		end
        if (is~=k) then
            f=-f
			for j=k,n,1 do
			  d=a[k][j]
			  a[k][j]=a[is][j]
			  a[is][j]=d
            end
        end
        if (js~=k) then
            f=-f
            for i=k,n,1 do
			  d=a[i][js]
			  a[i][js]=a[i][k]
			  a[i][k]=d
            end
        end
        det=det*a[k][k]
		for i=k+1,n,1 do
			if i<=n then
				  d=a[i][k]/a[k][k]
				  for j=k+1,n,1 do
					  if j<=n then
						a[i][j]=a[i][j]-d*a[k][j]
					  end
				  end
			 end
		end
	end
    det=f*det*a[n][n]
    return det
end

function Modn(ret,n)
	if(ret<0)then
		local ret1=ret+(-ret+1)*n
		return ret1%n
	end
	return ret%n
end

function MDetZnZ(ijkm,n)
	local mat=tbl_copy(ijkm)
	local det=math.floor(bsdet(mat)+0.5)
	det=Modn(det,n)
	return det
end

--general linear group
function GL(n,q)
	local g_FqAdd,g_FqMul=ZnZAddMul(q)
	local GLSet={}
	if n==2 then
		GLSet=MRFG(g_FqAdd,g_FqMul,{{0,1},{1,1}},{{1,1},{0,1}})
	elseif n==3 then
		GLSet=MRFG(g_FqAdd,g_FqMul,{{0,0,1},{0,1,1},{1,1,1}},{{1,1,1},{0,1,1},{0,0,1}})
	end	
	local GLTable=TableByMSet(GLSet,g_FqAdd,g_FqMul)
	return {GLSet,GLTable}
end

--special linear group
function SL(n,q)
	local g_FqAdd,g_FqMul=ZnZAddMul(q)
	local GLSet=GL(n,q)[1]
	local SLSet={}	
	for k,v in pairs(GLSet) do
		local det=MDetZnZ(v,q)
		if det==1 then
		   table.insert(SLSet,v)
		end
	end
	local SLTable=TableByMSet(SLSet,g_FqAdd,g_FqMul)
	return {SLSet,SLTable}
end

--称商群SL_n(F)/C(SL_n(F))为射影特殊线性群，用PSL_n(F)表示。
--1830年代，伽罗瓦研究p^v次一般方程的Galois群，构造了群PSL(2,p)、GL(v,p)。
--1870年代，若尔当构造了射影特殊线性群PSL(n,q)（一般的n，一般的q元有限域）。
function PSL(n,q)
	local sl=SL(n,q)
	local ZA=CenterOfG(sl[2])
	local PZA=quotientGNTable(sl[2],ZA)
	return PZA
end

--[[
gap> IdGroup(GO(3,5));
[ 240, 189 ]
gap> IdGroup(GO(-1,2,5));
[ 12, 4 ]
gap> IdGroup(GO(1,2,5));
[ 8, 3 ]
]]
function O(n,q)
	local g_FqAdd,g_FqMul=ZnZAddMul(q)
	local GLSet=GL(n,q)[1]
	local OSet={}		
	for k,v in pairs(GLSet) do
		local det=MDetZnZ(v,q)
		if det==1 or det==q-1 then
		   table.insert(OSet,v)
		end
	end
	local OTable=TableByMSet(OSet,g_FqAdd,g_FqMul)
	return {OSet,OTable}
end

function SO(GLSet,q)
	local g_FqAdd,g_FqMul=ZnZAddMul(q)
	local OSet={}		
	for k,v in pairs(GLSet) do
		local det=MDetZnZ(v,q)
		if det==1 then
		   table.insert(OSet,v)
		end
	end
	local OTable=TableByMSet(OSet,g_FqAdd,g_FqMul)
	return {OSet,OTable}
end

function SL2F3()
	local g_FqAdd,g_FqMul=ZnZAddMul(3)
	local g_Set=MRFG(g_FqAdd,g_FqMul,{{0,1},{2,0}},{{0,1},{2,1}})
	local g_Table=TableByMSet(g_Set,g_FqAdd,g_FqMul)
	return {g_Set,g_Table}
end

--[[
gap> a := [ [ ZmodnZObj(11,23),ZmodnZObj(15,23)],[ZmodnZObj(15,23), ZmodnZObj(12,23)] ];;b := [ [ ZmodnZObj(4,23),ZmodnZObj(10,23)],[ZmodnZObj(10,23), ZmodnZObj(19,23)] ];;G:=GroupWithGenerators([a,b]);;IdGroup(G);
[ 48, 7 ]
gap> IdGroup(GO(-1,2,23));
[ 48, 7 ]
]]
function O2F23()
	local g_FqAdd,g_FqMul=ZnZAddMul(23)
	local g_Set=MRFG(g_FqAdd,g_FqMul,{{11,15},{15,12}},{{4,10},{10,19}})
	local g_Table=TableByMSet(g_Set,g_FqAdd,g_FqMul)
	return {g_Set,g_Table}
end

--gap> for i in [1..48] do for j in [1..48] do G1:=GroupWithGenerators([L[i],L[j]]);a:=IdGroup(G1);if a[1]=24 and a[2]=6 then Print("i=",i,",j=",j,",",L[i],",",L[j],"\n");fi;od;od;
--i=1,j=16,[ [ 0*Z(23), Z(23)^0 ], [ Z(23)^0, 0*Z(23) ] ],[ [ Z(23)^6, Z(23)^9 ], [ Z(23)^20, Z(23)^6 ] ]
--[[
gap> a:=[ [ 0*Z(23), Z(23)^0 ], [ Z(23)^0, 0*Z(23) ] ];;b:=[ [ Z(23)^6, Z(23)^9 ], [ Z(23)^20, Z(23)^6 ] ];;G:=GroupWithGenerators([a,b]);;IdGroup(G);
[ 24, 6 ]
gap> PrimitiveRootMod(23);
5
gap> PowerMod(5,6,23);
8
gap> PowerMod(5,9,23);
11
gap> PowerMod(5,20,23);
12
gap> PowerMod(5,6,23);
8
gap> PowerMod(5,0,23);
1
gap> PowerMod(5,1,23);
5
gap> a := [ [ ZmodnZObj(0,23),ZmodnZObj(1,23)],[ZmodnZObj(1,23), ZmodnZObj(0,23)] ];;b := [ [ ZmodnZObj(8,23),ZmodnZObj(11,23)],[ZmodnZObj(12,23), ZmodnZObj(8,23)] ];;G:=GroupWithGenerators([a,b]);;IdGroup(G);
[ 24, 6 ]
]]
function D24inO2F23()
	local g_FqAdd,g_FqMul=ZnZAddMul(23)
	local g_Set=MRFG(g_FqAdd,g_FqMul,{{0,1},{1,0}},{{8,11},{12,8}})
	local g_Table=TableByMSet(g_Set,g_FqAdd,g_FqMul)
	return {g_Set,g_Table}
end

--[[
gap> a := [ [ ZmodnZObj(0,5),ZmodnZObj(1,5)],[ZmodnZObj(4,5), ZmodnZObj(0,5)] ];;b := [ [ ZmodnZObj(1,5),ZmodnZObj(1,5)],[ZmodnZObj(4,5), ZmodnZObj(0,5)] ];;G:=GroupWithGenerators([a,b]);;IdGroup(G);
[ 120, 5 ]
]]
function SL2F5()
	local g_FqAdd,g_FqMul=ZnZAddMul(5)
	local a={{0,1},{4,0}}
	local b={{1,1},{4,0}}
	--print(MDetZnZ(a,5),MDetZnZ(b,5))
	local g_Set=MRFG(g_FqAdd,g_FqMul,a,b)
	local g_Table=TableByMSet(g_Set,g_FqAdd,g_FqMul)
	return {g_Set,g_Table}
	--return {g_Set}
end

--[[
print("GAP4[48,29]=GL(2,3)是rank=2的(9-1)(9-3)=48阶群:")
PrintMSet(GL(2,3)[1])
PrintArr2(GL(2,3)[2])
print("GAP4[24,3]=SL(2,3)=2T是rank=2的24阶群:")
PrintMSet(SL2F3()[1])
PrintArr2(SL2F3()[2])
--PrintMSet(SL(2,3)[1])
--PrintArr2(SL(2,3)[2])
print("12阶群PSL(2,3)=T=A_4:")
PrintArr2(PSL(2,3))
print("GAP4[48,7]=O(2,23)是rank=2的48阶群:")
PrintMSet(O2F23()[1])
PrintArr2(O2F23()[2])
]]
local O2_23Set=O2F23()[1]
PrintMSet(O2_23Set)
local SO_m1_2_23=SO(O2_23Set,23)--GAP[24,2]=C_24
PrintMSet(SO_m1_2_23[1])
PrintArr2(SO_m1_2_23[2])
local D24=D24inO2F23()--GAP[24,6]=D_24
PrintMSet(D24[1])
PrintArr2(D24[2])
--PrintMSet(O(2,5)[1])--GAP[240,93]
--PrintMSet(SL(2,5)[1])--GAP[120,5]
--PrintMSet(GL(3,2)[1])--GAP[168,42]
--PrintMSet(SL(3,2)[1])--GAP[168,42]
--print(get_len(O(3,5)[1]))
-- local mtx1={{1,0,2},{0,2,0},{0,0,3}}
-- local mtx2=tbl_copy(mtx1)
-- local det1=mdetgauss(mtx1)
-- local det2=bsdet(mtx2)
-- print("det1=",det1,"det2=",det2)