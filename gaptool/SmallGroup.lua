local g_C8Mul={
	{0, 1, 2, 3, 4, 5, 6, 7},
	{1, 0, 3, 2, 5, 4, 7, 6},
	{2, 3, 1, 0, 6, 7, 5, 4},
	{3, 2, 0, 1, 7, 6, 4, 5},
	{4, 5, 6, 7, 2, 3, 1, 0},
	{5, 4, 7, 6, 3, 2, 0, 1},
	{6, 7, 5, 4, 1, 0, 3, 2},
	{7, 6, 4, 5, 0, 1, 2, 3}
}

local g_C2C4Mul={
	{0, 1, 2, 3, 4, 5, 6, 7},
	{1, 0, 3, 2, 5, 4, 7, 6},
	{2, 3, 0, 1, 6, 7, 4, 5},
	{3, 2, 1, 0, 7, 6, 5, 4},
	{4, 5, 6, 7, 1, 0, 3, 2},
	{5, 4, 7, 6, 0, 1, 2, 3},
	{6, 7, 4, 5, 3, 2, 1, 0},
	{7, 6, 5, 4, 2, 3, 0, 1}
}

local g_D4Mul={
	{0, 1, 2, 3, 4, 5, 6, 7},
	{1, 0, 3, 2, 5, 4, 7, 6},
	{2, 3, 0, 1, 7, 6, 5, 4},
	{3, 2, 1, 0, 6, 7, 4, 5},
	{4, 5, 6, 7, 0, 1, 2, 3},
	{5, 4, 7, 6, 1, 0, 3, 2},
	{6, 7, 4, 5, 3, 2, 1, 0},
	{7, 6, 5, 4, 2, 3, 0, 1}
}

local g_Q8Mul={
	{0, 1, 2, 3, 4, 5, 6, 7},
	{1, 0, 3, 2, 5, 4, 7, 6},
	{2, 3, 1, 0, 7, 6, 4, 5},
	{3, 2, 0, 1, 6, 7, 5, 4},
	{4, 5, 6, 7, 1, 0, 3, 2},
	{5, 4, 7, 6, 0, 1, 2, 3},
	{6, 7, 5, 4, 2, 3, 1, 0},
	{7, 6, 4, 5, 3, 2, 0, 1}
}

local g_C2C2C2Mul={
	{0, 1, 2, 3, 4, 5, 6, 7},
	{1, 0, 3, 2, 5, 4, 7, 6},
	{2, 3, 0, 1, 6, 7, 4, 5},
	{3, 2, 1, 0, 7, 6, 5, 4},
	{4, 5, 6, 7, 0, 1, 2, 3},
	{5, 4, 7, 6, 1, 0, 3, 2},
	{6, 7, 4, 5, 2, 3, 0, 1},
	{7, 6, 5, 4, 3, 2, 1, 0}
}

local g_Q12=
{
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 },
	{2, 3, 1, 5, 6, 4, 9, 7, 8, 12, 10, 11 },
	{3, 1, 2, 6, 4, 5, 8, 9, 7, 11, 12, 10 },
	{4, 5, 6, 1, 2, 3, 10, 11, 12, 7, 8, 9 },
	{5, 6, 4, 2, 3, 1, 12, 10, 11, 9, 7, 8 },
	{ 6, 4, 5, 3, 1, 2, 11, 12, 10, 8, 9, 7 },
	{ 7, 8, 9, 10, 11, 12, 4, 5, 6, 1, 2, 3 },
	{ 8, 9, 7, 11, 12, 10, 6, 4, 5, 3, 1, 2 },
	{ 9, 7, 8, 12, 10, 11, 5, 6, 4, 2, 3, 1 },
	{ 10, 11, 12, 7, 8, 9, 1, 2, 3, 4, 5, 6 },
	{ 11, 12, 10, 8, 9, 7, 3, 1, 2, 6, 4, 5 },
	{ 12, 10, 11, 9, 7, 8, 2, 3, 1, 5, 6, 4 }
};

function get_len(tb)
	local len=0
	for k,v in pairs(tb) do
		len=len+1
	end
	return len
end

function PrintArr2(vv) 
 local m=get_len(vv)
 for i=1,m,1 do
  io.write("[")
  local mi=get_len(vv[i])
  for j=1,mi,1 do
   io.write("(",vv[i][j][1],",",vv[i][j][2],") ")
  end
  io.write("]")
 end 
 io.write("\n")
end

function PrintArr(v) 
 local m=get_len(v)
  for i=1,m,1 do
   io.write(v[i]," ")
  end
  io.write("\n")
end

function doArr2(vv,delt)
 local m=get_len(vv)
 for i=1,m,1 do
  for j=1,m,1 do
   vv[i][j]=vv[i][j]+delt
  end
 end
end

doArr2(g_C8Mul,1)
doArr2(g_C2C4Mul,1)
doArr2(g_D4Mul,1)
doArr2(g_Q8Mul,1)
doArr2(g_C2C2C2Mul,1)
--PrintArr2(g_C8Mul)

function get_key(v1,tb)
	for k,v in pairs(tb) do
		if v==v1 then
		   return k
		end
	end
	return -1
end

function is_equal(a,b)
	local na=get_len(a)
	local nb=get_len(b)
	if na~=nb then
	   return false
	end
	for i=1,na,1 do
		if a[i]~=b[i] then
		   return false
		end
	end		
	return true
end

function get_key2(v1,tb)
	for k,v in pairs(tb) do
		if is_equal(v,v1) then
		   return k
		end
	end
	return -1
end

function mul(a,b)
	local ret={}
	local na=get_len(a)
	local nb=get_len(b)
	if(na~=nb)then
		return nil
	end
	for i=1,na,1 do
	  ret[i]=a[b[i]]
	end
	return ret
end

function FG(...)
	local args={...}
	local S={}
	local I={}
	local n=get_len(args[1])
	for i=1,n,1 do
		I[i]=i
	end
	table.insert(S,I)
 	for i,v in pairs(args) do
		table.insert(S,v)
	end
	local cnt=0
	local cnt1=get_len(S)
	while(cnt1>cnt)do
		cnt=get_len(S)
		for ki,vi in pairs(S) do
			for kj,vj in pairs(S) do
				local ij=mul(vi,vj)
				local IJ=get_key2(ij,S)
				if(IJ==-1)then
					table.insert(S,ij)
				end
			end
		end
		cnt1=get_len(S)
	end
	return S
end

function TableByPermuteSet(G)
	local ret={}
	for ki,vi in pairs(G) do
		local v={}
		for kj,vj in pairs(G) do
			local ij=mul(vi,vj)
			local IJ=get_key2(ij,G)
			table.insert(v,IJ)
		end
		table.insert(ret,v)
	end
	return ret
end

function DirectProduct(A,B)
	local C={}
	local nA=get_len(A)
	local nB=get_len(B)	
	local n=nA*nB
	for a=0,n-1,1 do
		local iRow={}
		for b=0,n-1,1 do
			local aj=a%nB
			local ai=(a-aj)/nB
			local bj=b%nB
			local bi=(b-bj)/nB
			local i=A[ai+1][bi+1]-1
			local j=B[aj+1][bj+1]-1
			local c=nB*i+j+1
			table.insert(iRow,c)
		end
		table.insert(C,iRow)
	end
	return C
end

-- 半直积群G:C2
function GC2(A)
	local C={}
	local B={{1,2},{2,1}}
	local nA=get_len(A)
	local nB=get_len(B)	
	local n=nA*nB	
	for a=0,n-1,1 do
		local iRow={}
		for b=0,n-1,1 do
			local aj=a%nB
			local ai=(a-aj)/nB
			local bj=b%nB
			local bi=(b-bj)/nB
			local bii=bi
			-- aj不是C_2={0,1}的单位元0
			if(aj==1)then
				for ii=0,nA-1,1 do
					if(A[bi+1][ii+1]==1)then
						bii=ii
						break
					end
				end
			end
			local i=A[ai+1][bii+1]-1
			local j=B[aj+1][bj+1]-1
			local c=nB*i+j+1
			table.insert(iRow,c)
		end
		table.insert(C,iRow)
	end
	return C
end

function CyclicGroup(n)
  local ret={}
  for i=1,n,1 do
      local I={}
	  for j=1,n,1 do
	   local ij=(i+j-2)%n+1
	   table.insert(I,ij)
	  end
	  table.insert(ret,I)
  end
  return ret
end

function AbelianGroup(tb)
  local n=get_len(tb)
  if n<1 then
     return nil
  end
  local ret=CyclicGroup(tb[1]) 
  for i=2,n,1 do
      ret=DirectProduct(ret,CyclicGroup(tb[i]))
  end
  return ret
end

--二面体群D_n1
--二面体群（Dihedral group）：D_(2n)=<a,b|a^n=b^2=1,b^(-1)ab=a^(-1)>
function Dn(n)
	local ret={}
	local n1=n/2
	local a={}
	for i=2,n1,1 do
	   table.insert(a,i)
	end
	table.insert(a,1)  
	local b={}
	--if n1%2==0 then 
		for i=n1,1,-1 do
		   table.insert(b,i)
		end	
--[[
	else
		table.insert(b,1)
		for i=n1,2,-1 do
		   table.insert(b,i)
		end	    
	end
	]]
	ret=FG(a,b)
	return ret
end

function DihedralGroup(n)
	local ret=Dn(n)
	return TableByPermuteSet(ret)
end

--9阶初等Abel群对应的18阶广义二面体群 (1,3,2)(4,6,5)(7,9,8), (1,7,4)(2,8,5)(3,9,6), (2,3)(4,7)(5,9)(6,8)
function GeneralizedDihedralGroup(tb)
    return GC2(AbelianGroup(tb))
	--[[
    local ret={}
	if get_len(tb)==2 and tb[1]==3 and tb[2]==3 then
		ret=FG({3,1,2,6,4,5,9,7,8},{7,8,9,1,2,3,4,5,6},{1,3,2,7,9,8,4,6,5})
	end
	if get_len(tb)==2 and tb[1]==4 and tb[2]==4 then
		ret=FG({1,4,3,6,5,8,7,2},{3,8,5,2,7,4,1,6},{7,6,5,4,3,2,1,8})
	end	
	return TableByPermuteSet(ret)
	]]
end

function F20Group()
	local ret=FG({1,3,2,5,4},{2,3,4,1,5})
	return TableByPermuteSet(ret)
end

function VGroup(m,n)
    local ret={} 
    if m==4 and n==2 then
		ret=FG({5,6,4,8,1,2,10,9,3,7},{6,2,3,4,5,1,7,8,9,10})--IdGroup(Group((1,5)(2,6)(3,4,8,9)(7,10),(1,6)));
	end		
    if m==4 and n==3 then
		ret=FG({1,4,3,2,5,7,6,8,10,9,11,12},{2,1,4,3,5,7,8,6,9,10,12,11})
	end	
    if m==4 and n==4 then
		ret=FG({17,23,2,19,27,28,5,25,11,31,8,9,10,29,7,32,14,1,16,13,12,24,20,21,22,18,4,26,6,3,30,15},{22,6,32,20,10,9,24,4,16,1,29,28,27,8,30,2,13,31,11,14,15,5,19,18,17,21,25,3,23,26,7,12})
	end
    if m==4 and n==6 then
		ret=FG({2,1,19,8,9,24,12,4,5,17,32,7,45,21,37,38,10,25,3,41,14,30,31,6,18,48,35,34,46,22,23,11,47,28,27,42,15,16,44,43,20,36,40,39,13,29,33,26},{28,35,30,40,41,3,37,44,45,7,17,42,4,47,11,12,13,24,46,8,48,18,19,20,31,1,14,15,25,26,27,38,2,21,22,32,33,34,5,6,29,39,9,10,36,43,16,23})
    end	
    if m==6 and n==4 then
        ret=FG({2,1,18,21,9,24,11,14,5,17,7,32,44,8,46,38,10,3,25,40,4,42,31,6,19,35,48,33,37,36,23,12,28,47,26,30,29,16,45,20,43,22,41,13,39,15,34,27},{28,35,17,47,41,3,24,48,45,7,42,31,1,40,25,12,13,46,38,2,44,32,19,20,30,14,5,6,11,39,27,37,21,9,10,18,43,34,4,29,16,26,8,36,23,33,15,22})
    end		
	return TableByPermuteSet(ret)
end

function HGroup(m,n)
    local ret={} 
    if m==4 and n==4 then
		ret=FG({2,3,4,1,5,6,7,8},{3,2,1,4,6,7,8,5})
	end	
    if m==4 and n==8 then
		ret=FG({3,7,15,11,12,13,21,17,18,19,1,26,4,23,24,25,2,30,8,27,28,29,5,6,14,31,9,10,20,32,16,22},{2,5,17,21,9,10,23,26,1,16,7,27,28,30,8,22,12,11,31,15,14,6,18,19,32,20,3,25,24,4,29,13})
    end
    if m==8 and n==4 then
		ret=FG({2,5,17,8,9,10,23,14,6,16,7,27,28,20,21,22,12,24,31,15,26,1,18,19,32,30,13,25,11,4,29,3},{3,7,4,11,12,13,8,17,18,19,1,14,15,23,24,25,2,20,21,27,28,29,5,6,26,31,9,10,30,32,16,22})
    end	
    if m==4 and n==12 then
		ret=FG({2,4,19,8,9,24,25,1,14,30,32,7,45,21,37,38,10,12,11,47,5,17,42,15,18,48,35,34,46,22,23,3,41,40,39,31,6,29,44,43,20,36,28,27,26,16,33,13},{3,7,5,11,12,13,9,18,19,20,14,1,16,25,26,27,28,21,2,23,32,33,34,35,4,29,6,31,39,40,41,8,36,10,38,43,44,45,15,42,17,47,22,46,24,48,30,37},{6,10,13,15,16,17,20,22,23,24,26,27,28,29,30,31,1,33,34,35,36,37,38,2,39,40,41,3,42,4,5,43,44,45,7,46,8,9,47,11,12,14,48,18,19,21,25,32})
	end	
    if m==4 and n==15 then
		ret=FG({5,8,10,12,13,15,17,18,20,21,22,23,24,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,1,48,49,50,51,52,2,53,54,3,55,4,56,57,6,58,7,59,9,11,60,14,16,19,25},{2,3,6,16,42,1,19,45,25,50,7,58,30,11,36,9,59,33,14,60,39,52,51,18,4,55,24,54,53,21,57,56,27,41,40,8,47,46,13,44,43,10,49,48,15,29,28,35,34,5,32,31,38,37,17,23,22,20,26,12},{4,7,9,11,12,14,16,17,19,20,1,22,23,25,26,2,28,29,3,31,32,5,34,35,6,37,38,8,40,41,10,43,44,13,46,47,15,48,49,18,51,52,21,53,54,24,55,27,56,57,30,58,33,59,36,39,60,42,45,50})
	end		
	return TableByPermuteSet(ret)
end

function M32Group()
	local ret=FG({2,4,19,8,9,10,24,5,14,15,28,29,7,20,21,22,25,31,11,6,16,26,32,17,18,30,3,12,23,1,27,13},{3,7,1,11,12,13,2,17,18,19,4,5,6,23,24,25,8,9,10,27,28,29,14,15,16,31,20,21,22,32,26,30})
	return TableByPermuteSet(ret)
end

--对称群S_n,rank(S_n)=2
function Sn(n)
	local ret={}
	local a={}
	table.insert(a,2) 
	table.insert(a,1) 
	for i=3,n,1 do
	   table.insert(a,i)
	end
	local b={}
	table.insert(b,1)
	for i=3,n,1 do
	   table.insert(b,i)
	end
    table.insert(b,2)	
	ret=FG(a,b)
	return ret	
end

function SymmetricGroup(n)
	local ret=Sn(n)
	return TableByPermuteSet(ret)	
end

--置换的逆序数
function Tau(a)
	local ret=0
	local n=get_len(a)
	for i=1,n,1 do
		for j=i+1,n,1 do
		   if a[i]>a[j] then
				ret=ret+1
		   end
		end
	end
	return ret
end

--交错群A_n，n>=3是由3循环生成的，并且事实上A_n=<(123),(124),…,(12n)>
function An(n)
    local ret={}
    local SnSet=Sn(n)	
	for k,v in pairs(SnSet) do
		if Tau(v)%2==0 then
			table.insert(ret,v)
		end
	end
	return ret
end

function AlternatingGroup(n)
    local ret=An(n)
	return TableByPermuteSet(ret)
end

--u×v=(u_yv_z-u_zv_y,u_zv_x-u_xv_z,u_xv_y-u_yv_x)
--反交换律：v×u=(v_yu_z-v_zu_y,v_zu_x-v_xu_z,v_xu_y-v_yu_x)=-(u×v)
--特殊情形1：两个非零向量a和b平行，当且仅当a×b=0（垂直时点积为0）（u×u=0
--直角坐标系 (i, j, k)与 (a,b,a×b)、(i, j, -k)与 (a,b,-a×b)同时满足左手定则或同时满足右手定则
--三维叉积的引入：向量u×v（取u，v确定平面上的垂线上的某一向量）垂直于向量u和v
function Vec3CrossP(u,v)
	return {(u[2]*v[3]-u[3]*v[2]),u[3]*v[1]-u[1]*v[3],u[1]*v[2]-u[2]*v[1]}
end

function Vec3DotP(u,v)
	return (u[1]*v[1]+u[2]*v[2]+u[3]*v[3])
end

--三矢量的混合积(a×b)·c=(a,b,c)=(a×b)c=|a×b||c|cos<a×b,c>，
--|(a×b)·c|的几何意义表示以a,b,c为棱的平行六面体的体积（标量三重积，有符号的）。
function Vec3TriplexP(a,b,c)
	return Vec3DotP(Vec3CrossP(a,b),c)
end

--[[
(a_1+a_2i+a_3j+a_4k)*(b_1+b_2i+b_3j+b_4k)=
(a_1b_1-a_2b_2-a_3b_3-a_4b_4)+
(a_1b_2+a_2b_1+a_3b_4-a_4b_3)i+
(a_1b_3+a_3b_1+a_4b_2-a_2b_4)j+
(a_1b_4+a_4b_1+a_2b_3-a_3b_2)k 
]]
function qmul(q1,q2)
   local ret={}
   ret[1]=q1[1]*q2[1]-Vec3DotP({q1[2],q1[3],q1[4]},{q2[2],q2[3],q2[4]})
   local temp=Vec3CrossP({q1[2],q1[3],q1[4]},{q2[2],q2[3],q2[4]})
   ret[2]=q1[1]*q2[2]+q1[2]*q2[1]+temp[1]
   ret[3]=q1[1]*q2[3]+q1[3]*q2[1]+temp[2]
   ret[4]=q1[1]*q2[4]+q1[4]*q2[1]+temp[3]
   return ret
end

function Q8()
	local G={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1},{-1,0,0,0},{0,-1,0,0},{0,0,-1,0},{0,0,0,-1}}
	local ret={}
	for ki,vi in pairs(G) do
		local v={}
		for kj,vj in pairs(G) do
			local ij=qmul(vi,vj)
			local IJ=get_key2(ij,G)
			table.insert(v,IJ)
		end
		table.insert(ret,v)
	end
	return ret    
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

function is_equalCM(a,b)
	local ret=(is_equalM(a[1],b[1]) and is_equalM(a[2],b[2]))
	return ret
end

function get_keyM(v1,tb)
	for k,v in pairs(tb) do
		if is_equalM(v,v1) then
		   return k
		end
	end
	return -1
end

function get_keyCM(v1,tb)
	for k,v in pairs(tb) do
		if is_equalCM(v,v1) then
		   return k
		end
	end
	return -1
end

function CMatFG(...)
	local args={...}
	local S={}
 	for i,v in pairs(args) do
		table.insert(S,v)
	end
	local cnt=0
	local cnt1=get_len(S)
	while(cnt1>cnt)do
		cnt=get_len(S)
		for ki,vi in pairs(S) do
			for kj,vj in pairs(S) do
				local ij=CMatMul(vi,vj)
				local IJ=get_keyCM(ij,S)
				if(IJ==-1)then
					table.insert(S,ij)
				end
			end
		end
		cnt1=get_len(S)
	end
	return S
end

function TableByCMSet(G)
	local ret={}
	for ki,vi in pairs(G) do
		local v={}
		for kj,vj in pairs(G) do
			local ij=CMatMul(vi,vj)
			local IJ=get_keyCM(ij,G)
			table.insert(v,IJ)
		end
		table.insert(ret,v)
	end
	return ret
end

--四元数群或双循环群（Dicyclic group）：Q_(4n)=<a,b|a^(2n)=1,a^n=b^2,b^(-1)ab=a^(-1)>,rank(Q_(4n))=2
--抽象的哈密尔顿群的定义：任何非阿贝尔群如其所有子群都是正规子群就称为哈密尔顿群。四元数群Q_8是阶数最小（8阶）的哈密尔顿群。
function Q4n(n)
	local E2n={math.cos(math.pi/n),math.sin(math.pi/n)}--2n次本原单位根
	local a1={{{E2n[1],0},{0,E2n[1]}},{{E2n[2],0},{0,-E2n[2]}}}--2n次本原单位根对应的复矩阵
	local a2={{{0,-1},{1,0}},{{0,0},{0,0}}}
	local CI2={{{1,0},{0,1}},{{0,0},{0,0}}}
	local ret=CMatFG(CI2,a1,a2)
	return ret
end

function QuaternionGroup(n)
    local ret=Q4n(n/4)
	return TableByCMSet(ret)
end

function QD8n(n)
	local En2={math.cos(4*math.pi/n),math.sin(4*math.pi/n)}--n/2次本原单位根
	local a1={{{0,En2[1]},{En2[1],0}},{{0,En2[2]},{-En2[2],0}}}--n/2次本原单位根对应的复矩阵
	local a2={{{0,-1},{1,0}},{{0,0},{0,0}}}
	local CI2={{{1,0},{0,1}},{{0,0},{0,0}}}
	local ret=CMatFG(CI2,a1,a2)
	return ret
end

--[[
n=8m阶拟二面体群
GAP[8,2]群元阶的分布:[1,3,4,0]
GAP[16,8]群元阶的分布:[1,5,6,4,0]
GAP[24,5]群元阶的分布:[1,7,2,8,2,0,4,0]
GAP[32,19]群元阶的分布:[1,9,10,4,8,0]
GAP[40,5]群元阶的分布:[1,11,12,4,0,4,8,0]
GAP[48,6]群元阶的分布:[1,13,2,14,2,4,4,0,8,0]
]]
function QDGroup(n)
    local ret=QD8n(n)
	return TableByCMSet(ret)
end

--rank(QD_16)=2
function QD16Group()
	--[[
	local ret=FG(
	{5,6,8,7,2,1,3,4},
	{3,4,2,1,7,8,6,5},
	{8,7,5,6,2,1,4,3},
	{2,1,4,3,6,5,8,7},
	{8,7,6,5,3,4,1,2},
	{4,3,2,1,6,5,7,8},
	{6,5,7,8,1,2,4,3},
	{4,3,1,2,8,7,5,6},
	{7,8,6,5,1,2,3,4},
	{7,8,5,6,4,3,2,1},	
	{3,4,1,2,5,6,8,7},
	{5,6,7,8,4,3,1,2},
	{1,2,4,3,8,7,6,5},	
	{6,5,8,7,3,4,2,1},
	{2,1,3,4,7,8,5,6},	
	)
	]]
	return QDGroup(16)
	--local ret=FG({8,7,6,5,3,4,1,2},{1,2,4,3,8,7,6,5})
	--return TableByPermuteSet(ret)
end

function M16()
	local a1={{{0,1},{0,0}},{{0,0},{1,0}}}
	local a2={{{1,0},{0,-1}},{{0,0},{0,0}}}
	local CI2={{{1,0},{0,1}},{{0,0},{0,0}}}
	local ret=CMatFG(CI2,a1,a2)
	return ret
end

function M16Group()
    local ret=M16()
	return TableByCMSet(ret)
end

--[[
function P16()
	local a1={{{0,1},{1,0}},{{0,0},{0,0}}}
	local a2={{{0,0},{0,0}},{{0,-1},{1,0}}}
	local a3={{{1,0},{0,-1}},{{0,0},{0,0}}}	
	local CI2={{{1,0},{0,1}},{{0,0},{0,0}}}
	local ret=CMatFG(CI2,a1,a2,a3)
	return ret
end

function P16Group()
    local ret=P16()
	return TableByCMSet(ret)
end
]]

function P8n(n)
	local E2n={math.cos(math.pi/n),math.sin(math.pi/n)}
	local a1={{{0,E2n[2]},{E2n[2],0}},{{0,E2n[1]},{-E2n[1],0}}}	
	local a2={{{0,0},{0,0}},{{0,-1},{1,0}}}
	local a3={{{1,0},{0,-1}},{{0,0},{0,0}}}	
	local CI2={{{1,0},{0,1}},{{0,0},{0,0}}}
	local ret=CMatFG(CI2,a1,a2,a3)
	return ret
end

function PGroup(n)
    local ret=P8n(n/8)
	return TableByCMSet(ret)
end

function PP8n(n)
	local E2n={math.cos(math.pi/n),math.sin(math.pi/n)}	
	local a1={{{0,1},{1,0}},{{0,0},{0,0}}}
	local a2={{{0,-E2n[1]},{E2n[1],0}},{{0,-E2n[2]},{E2n[2],0}}}	
	local a3={{{1,0},{0,-1}},{{0,0},{0,0}}}	
	local CI2={{{1,0},{0,1}},{{0,0},{0,0}}}
	local ret=CMatFG(CI2,a1,a2,a3)
	return ret
end

function PPGroup(n)
    local ret=PP8n(n/8)
	return TableByCMSet(ret)
end

function SL2F3Group()
	return TableByPermuteSet(FG({2,3,1,6,4,5,7,8},{1,5,8,4,7,3,2,6}))
end

function UGroup(m,n)
    local ret={}
	if m==3 and n==4 then
		ret=FG({1,2,3,4,5,6,7,8,10,11,9},{2,3,4,5,6,7,8,1,9,11,10})
	end	
	if m==3 and n==8 then
		ret=FG({2,3,7,8,9,24,4,11,12,28,18,19,35,21,37,38,10,5,14,30,25,40,41,13,32,44,45,20,46,22,23,1,31,42,15,47,26,27,48,33,34,36,17,16,29,39,43,6},{6,10,13,15,16,17,20,22,23,24,26,27,28,29,30,31,1,33,34,35,36,37,38,2,39,40,41,3,42,4,5,43,44,45,7,46,8,9,47,11,12,14,48,18,19,21,25,32})
	end
    return TableByPermuteSet(ret)
end

function W32Group()
    local ret=FG({3,7,4,11,12,13,8,17,18,19,6,14,15,23,24,25,10,20,21,27,28,29,16,1,26,31,22,2,30,32,5,9},{2,5,17,21,9,10,23,26,1,16,7,27,28,30,8,22,12,11,31,15,14,6,18,19,32,20,3,25,24,4,29,13})
    return TableByPermuteSet(ret)
end

function W48Group()
    local ret=FG({23,29,34,36,10,9,39,16,15,14,43,20,19,22,21,2,38,27,26,25,6,5,4,42,33,32,7,45,8,46,24,13,12,11,47,1,31,30,18,48,35,37,3,41,40,17,44,28},{40,44,42,28,47,11,46,35,48,18,31,30,14,41,3,25,26,38,37,21,45,7,32,33,17,5,4,29,12,13,39,24,9,8,36,19,20,43,1,16,15,27,2,23,22,34,6,10})
    return TableByPermuteSet(ret)
end

function X48Group()
	return TableByPermuteSet(FG({7,14,18,20,35,21,27,30,42,3,2,33,43,34,19,37,46,6,41,47,11,29,13,28,9,22,10,32,45,48,36,17,31,1,40,26,39,12,8,24,38,44,16,5,15,25,23,4},{17,24,30,31,1,5,37,38,2,9,41,42,3,13,4,15,6,45,46,7,20,8,22,10,47,11,26,12,28,14,16,48,18,33,19,35,21,23,25,39,27,29,32,43,34,36,40,44}))
end

function QGroup(m,n)
    local ret={}
	if m==4 and n==4 then
		ret=FG({2,5,17,21,9,10,23,26,6,16,7,27,28,30,8,22,12,24,31,4,14,1,18,19,32,20,13,25,11,15,29,3},{3,7,4,11,12,13,8,17,18,19,6,14,15,23,24,25,10,20,21,27,28,29,16,1,26,31,22,2,30,32,5,9})
    end		
	if m==4 and n==6 then
		ret=FG({2,4,19,8,9,24,25,5,14,30,32,7,45,21,37,38,10,3,11,47,1,31,42,15,18,48,35,34,46,22,23,12,28,40,39,17,16,29,44,43,20,36,41,13,26,6,33,27},{35,40,38,44,45,20,42,41,47,26,46,24,23,48,33,34,7,17,30,29,28,27,39,11,37,36,10,9,43,18,19,31,6,15,14,13,12,25,22,21,2,32,16,1,4,3,8,5})
	end	
	return TableByPermuteSet(ret)
end

local g_Gn={
    {4,2},
    {6,2},	
    {8,5},	
	{10,2},	
    {12,5},
	{14,2},	
    {16,14},	
    {18,5},	
    {20,5},	
    {24,15},
    {30,4},	
    {32,51},		
    {36,14},
	{48,52},
	{60,13},	
}

local g_G={
	{4,1,CyclicGroup(4)},	
	{4,2,AbelianGroup({2,2})},		
	{6,1,SymmetricGroup(3)},
	{6,2,CyclicGroup(6)},	
	{8,1,CyclicGroup(8)},
	{8,2,AbelianGroup({2,4})},
	{8,3,DihedralGroup(8)},
	{8,4,QuaternionGroup(8)},
	{8,5,AbelianGroup({2,2,2})},
	{10,1,DihedralGroup(10)},	
	{10,2,CyclicGroup(10)},	
	{12,1,QuaternionGroup(12)},	
	{12,2,CyclicGroup(12)},
	{12,3,AlternatingGroup(4)},		
	{12,4,DihedralGroup(12)},	
	{12,5,AbelianGroup({2,2,3})},--DirectProduct(CyclicGroup(6),CyclicGroup(2))
	{14,1,DihedralGroup(14)},	
	{14,2,CyclicGroup(14)},
	{16,1,CyclicGroup(16)},
	{16,2,AbelianGroup({4,4})},	
	{16,3,VGroup(4,2)},
	{16,4,HGroup(4,4)},	
	{16,5,AbelianGroup({2,8})},	
	{16,6,M16Group()},		
	{16,7,DihedralGroup(16)},	
	{16,8,QDGroup(16)},	
	{16,9,QuaternionGroup(16)},	
	{16,10,AbelianGroup({2,2,4})},
	{16,11,DirectProduct(DihedralGroup(8),CyclicGroup(2))},	
	{16,12,DirectProduct(QuaternionGroup(8),CyclicGroup(2))},
	{16,13,PGroup(16)},	
	{16,14,AbelianGroup({2,2,2,2})},
	{18,1,DihedralGroup(18)},	
	{18,2,CyclicGroup(18)},
	{18,3,DirectProduct(SymmetricGroup(3),CyclicGroup(3))},	
	{18,4,GeneralizedDihedralGroup({3,3})},
	{18,5,AbelianGroup({2,3,3})},
	{20,1,QuaternionGroup(20)},	
	{20,2,CyclicGroup(20)},
	{20,3,F20Group()},
	{20,4,DihedralGroup(20)},	
	{20,5,AbelianGroup({2,2,5})},	
	{24,1,UGroup(3,4)},--U_24	
	{24,2,CyclicGroup(24)},	
	{24,3,SL2F3Group()},--SL_2(F_3)=2T=Q_8:C_3	
	{24,4,QuaternionGroup(24)},	
	{24,5,DirectProduct(DihedralGroup(6),CyclicGroup(4))},--QDGroup(24)		
	{24,6,DihedralGroup(24)},	
	{24,7,DirectProduct(QuaternionGroup(12),CyclicGroup(2))},
	{24,8,VGroup(4,3)},--V_24		
	{24,9,AbelianGroup({2,12})},
	{24,10,DirectProduct(DihedralGroup(8),CyclicGroup(3))},	
	{24,11,DirectProduct(QuaternionGroup(8),CyclicGroup(3))},
	{24,12,SymmetricGroup(4)},	
	{24,13,DirectProduct(AlternatingGroup(4),CyclicGroup(2))},	
	{24,14,DirectProduct(DihedralGroup(12),CyclicGroup(2))},	
	{24,15,AbelianGroup({2,2,2,3})},
	{30,1,DirectProduct(DihedralGroup(6),CyclicGroup(5))},
	{30,2,DirectProduct(DihedralGroup(10),CyclicGroup(3))},
	{30,3,DihedralGroup(30)},
	{30,4,CyclicGroup(30)},	
	{32,1,CyclicGroup(32)},
	{32,3,AbelianGroup({4,8})},
	{32,4,TableByPermuteSet(FG({4,11,3,1,8,6,10,2,9,7,5,12},{9,2,1,12,5,4,6,8,7,3,11,10}))},--IdGroup(Group(( 1, 4)( 2,11, 5, 8)( 7,10),( 1, 9, 7, 6, 4,12,10, 3)));	
	{32,6,TableByPermuteSet(FG({2,1,4,7,6,5,8,3},{3,4,1,2,7,8,5,6}))},--IdGroup(Group([(1,2)(3,4,7,8)(5,6),(1,3)(2,4)(5,7)(6,8)]));
	{32,7,TableByPermuteSet(FG({2,7,8,5,6,3,4,1},{1,2,7,8,5,6,3,4}))},--IdGroup(Group([(1,2,7,4,5,6,3,8),(3,7)(4,8)]));	
	{32,9,VGroup(4,4)},
	{32,11,TableByPermuteSet(FG({2,3,4,1,5,6,7,8},{8,7,6,5,4,3,2,1}))},	
	{32,12,HGroup(8,4)},	
	{32,13,W32Group()},	
	{32,14,HGroup(4,8)},		
	{32,15,QGroup(4,4)},	
	{32,16,AbelianGroup({2,16})},
	{32,17,M32Group()},	
	{32,18,DihedralGroup(32)},
	{32,19,QDGroup(32)},
	{32,20,QuaternionGroup(32)},	
	{32,21,AbelianGroup({2,4,4})},
	{32,22,DirectProduct(VGroup(4,2),CyclicGroup(2))},	
	{32,23,DirectProduct(HGroup(4,4),CyclicGroup(2))},	
	{32,24,TableByPermuteSet(FG({1,2,6,4,5,3,7,8,12,10,11,9},{1,8,9,4,11,12,7,5,6,10,2,3},{7,3,2,10,6,5,4,9,8,1,12,11}))},--rank=3,IdGroup(Group(( 3, 6)( 9,12),( 2, 8, 5,11)( 3, 9, 6,12),( 1, 7, 4,10)( 2, 3)( 5, 6)( 8, 9)(11,12)));
	{32,25,DirectProduct(DihedralGroup(8),CyclicGroup(4))},		
	{32,26,DirectProduct(QuaternionGroup(8),CyclicGroup(4))},
	{32,27,TableByPermuteSet(FG({5,6,4,8,1,2,10,9,3,7},{6,2,3,4,5,1,7,8,9,10},{1,2,3,9,5,6,7,8,4,10}))},--IdGroup(Group((1,5)(2,6)(3,4,8,9)(7,10),(1,6),(4,9)));还可进一步表示为S_8的子群	
	{32,28,TableByPermuteSet(FG({2,3,4,1,5,6,7,8},{3,2,1,4,6,7,8,5},{1,2,3,4,7,6,5,8}))},--IdGroup(Group([(1,2,3,4),(1,3)(5,6,7,8),(5,7)]));
	{32,34,GeneralizedDihedralGroup({4,4})},--TableByPermuteSet(FG({1,4,3,6,5,8,7,2},{3,8,5,2,7,4,1,6},{7,6,5,4,3,2,1,8}))--IdGroup(Group([(2,4,6,8),(1,3,5,7)(2,8,6,4),(1,7)(2,6)(3,5)]));
	{32,36,AbelianGroup({2,2,8})},
	{32,37,DirectProduct(M16Group(),CyclicGroup(2))},
	{32,38,PPGroup(32)},	
	{32,39,DirectProduct(DihedralGroup(16),CyclicGroup(2))},
	{32,40,DirectProduct(QDGroup(16),CyclicGroup(2))},	
	{32,41,DirectProduct(QuaternionGroup(16),CyclicGroup(2))},	
	{32,42,PGroup(32)},		
	{32,43,TableByPermuteSet(FG({1,6,3,8,5,2,7,4},{8,7,6,5,4,3,2,1},{8,5,2,7,4,1,6,3}))},--IdGroup(Group([(2,6)(4,8),(1,8)(2,7)(3,6)(4,5),(1,8,3,2,5,4,7,6)]));	
	{32,45,AbelianGroup({2,2,2,4})},	
	{32,46,DirectProduct(DihedralGroup(8),AbelianGroup({2,2}))},	
	{32,47,DirectProduct(QuaternionGroup(8),AbelianGroup({2,2}))},	
	{32,48,DirectProduct(PGroup(16),CyclicGroup(2))},	
	{32,49,TableByPermuteSet(FG({1,2,7,8,5,6,3,4},{1,6,3,8,5,2,7,4},{2,1,4,3,6,5,8,7},{3,4,1,2,7,8,5,6}))},--IdGroup(Group([(3,7)(4,8),(2,6)(4,8),(1,2)(3,4)(5,6)(7,8),(1,3)(2,4)(5,7)(6,8)]));	
	{32,51,AbelianGroup({2,2,2,2,2})},	
	{36,1,QuaternionGroup(36)},
	{36,2,CyclicGroup(36)},	
	{36,4,DihedralGroup(36)},	
	{36,5,AbelianGroup({18,2})},
	{36,6,DirectProduct(QuaternionGroup(12),CyclicGroup(3))},	
	{36,8,AbelianGroup({12,3})},
	{36,9,TableByPermuteSet(FG({1,3,8,7,9,2,5,6,4},{2,1,5,7,3,9,4,8,6}))},--IdGroup(Group([(2,3,8,6)(4,7,5,9),(1,2)(3,5)(4,7)(6,9)]));	
	{36,10,DirectProduct(SymmetricGroup(3),SymmetricGroup(3))},
	{36,11,DirectProduct(AlternatingGroup(4),CyclicGroup(3))},	
	{36,12,DirectProduct(CyclicGroup(6),SymmetricGroup(3))},
	{36,13,TableByPermuteSet(FG({1,2,8,4,6,7,5,3,9},{1,4,3,2,5,7,6,8,9},{1,2,3,9,5,7,6,8,4}))},--rank=3,IdGroup(Group((3,8)(5,6,7),(2,4)(6,7),(4,9)(6,7)));	
	{36,14,AbelianGroup({6,6})},
	{48,1,UGroup(3,8)},
	{48,2,CyclicGroup(48)},	
	{48,3,TableByPermuteSet(FG({7,2,12,10,5,9,4,8,3,1,11,6},{9,1,11,12,4,8,6,7,2,3,10,5}))},--IdGroup(Group(( 1, 7, 4,10)( 3,12, 6, 9),( 1, 9, 2)( 3,11,10)( 4,12, 5)( 6, 8, 7)));	
	{48,4,DirectProduct(CyclicGroup(8),SymmetricGroup(3))},	
	{48,6,QDGroup(48)},	
	{48,7,DihedralGroup(48)},--O(2,23)=GO(-1,2,23)
	{48,8,QuaternionGroup(48)},	
	{48,9,DirectProduct(UGroup(3,4),CyclicGroup(2))},
	{48,10,QGroup(4,6)},	
	{48,11,W48Group()},		
	{48,13,HGroup(4,12)},		
	{48,14,VGroup(4,6)},
	{48,15,VGroup(6,4)},	
	{48,20,AbelianGroup({3,4,4})},
	{48,21,DirectProduct(VGroup(4,2),CyclicGroup(3))},
	{48,22,DirectProduct(HGroup(4,4),CyclicGroup(3))},
	{48,23,AbelianGroup({2,3,8})},
	{48,24,DirectProduct(M16Group(),CyclicGroup(3))},
	{48,25,DirectProduct(DihedralGroup(16),CyclicGroup(3))},
	{48,26,DirectProduct(QDGroup(16),CyclicGroup(3))},
	{48,27,DirectProduct(QuaternionGroup(16),CyclicGroup(3))},	
	{48,29,TableByPermuteSet(FG({1,2,3,7,8,9,4,5,6},{1,7,4,8,5,2,6,3,9}))},--GL(2,3),IdGroup(Group((4,7)(5,8)(6,9), (2,7,6)(3,4,8)));
	{48,30,TableByPermuteSet(FG({4,2,3,1,6,7,8,5},{1,3,4,2,5,6,7,8}))},--IdGroup(Group((1,4)(5,6,7,8),(2,3,4)));
	{48,31,TableByPermuteSet(FG({4,3,2,1,6,7,8,5},{1,3,4,2,5,6,7,8}))},--IdGroup(Group((1,4)(2,3)(5,6,7,8),(2,3,4)));	
	{48,32,DirectProduct(SL2F3Group(),CyclicGroup(2))},	
	{48,33,X48Group()},	
	{48,34,DirectProduct(QuaternionGroup(24),CyclicGroup(2))},
	{48,35,DirectProduct(DihedralGroup(6),AbelianGroup({2,4}))},
	{48,36,DirectProduct(DihedralGroup(24),CyclicGroup(2))},
	{48,37,PGroup(48)},	
	{48,38,DirectProduct(DihedralGroup(8),SymmetricGroup(3))},
	{48,40,DirectProduct(QuaternionGroup(8),SymmetricGroup(3))},		
	{48,42,DirectProduct(QuaternionGroup(12),AbelianGroup({2,2}))},	
	{48,43,DirectProduct(VGroup(4,3),CyclicGroup(2))},	
	{48,44,AbelianGroup({2,2,3,4})},	
	{48,45,DirectProduct(DihedralGroup(8),CyclicGroup(6))},
	{48,46,DirectProduct(QuaternionGroup(8),CyclicGroup(6))},	
	{48,47,DirectProduct(PGroup(16),CyclicGroup(3))},	
	{48,48,DirectProduct(SymmetricGroup(4),CyclicGroup(2))},	
	{48,49,DirectProduct(AlternatingGroup(4),AbelianGroup({2,2}))},	
	{48,50,TableByPermuteSet(FG({1,2,3,4,6,5,8,7},{1,3,4,2,5,7,8,6},{2,1,4,3,5,6,7,8}))},--rank=3,IdGroup(Group(( 5, 6)( 7, 8),( 2, 3, 4)( 6, 7, 8),( 1, 2)( 3, 4)));	
	{48,51,DirectProduct(DihedralGroup(12),AbelianGroup({2,2}))},		
	{48,52,AbelianGroup({2,2,2,2,3})},	
	{60,1,DirectProduct(QuaternionGroup(12),CyclicGroup(5))},
	{60,2,DirectProduct(QuaternionGroup(20),CyclicGroup(3))},
	{60,3,QuaternionGroup(60)},--HGroup(4,15)		
	{60,4,CyclicGroup(60)},		
	{60,5,AlternatingGroup(5)},	
	{60,6,DirectProduct(F20Group(),CyclicGroup(3))},	
	{60,8,DirectProduct(SymmetricGroup(3),DihedralGroup(10))},	
	{60,9,DirectProduct(AlternatingGroup(4),CyclicGroup(5))},	
	{60,10,DirectProduct(CyclicGroup(6),DihedralGroup(10))},		
	{60,11,DirectProduct(CyclicGroup(10),SymmetricGroup(3))},
	{60,12,DihedralGroup(60)},
	{60,13,AbelianGroup({2,30})},	
}

function NumberSmallGroups(n)
 	for k,v in pairs(g_Gn) do
		if v[1]==n then
		   return v[2]
		end
	end
    return 0
end

function SmallGroup(n,id)
 	for k,v in pairs(g_G) do
		if v[1]==n and v[2]==id then
		   return v[3]
		end
	end
    return nil
end

--未知群的群元的阶
function getGnEOrder(tb,a)
  local n=get_len(tb)
  if(a<1 or a>n) then
	  return -1
  end
  local t=1
  for i=1,n,1 do
   t=tb[t][a]
   if(t==1)then
    return i
   end
  end
  return -1
end

--冒泡升序排序
function BubbleAsc(tb,Asc)
	local n=get_len(tb)
	for i=2,n,1 do
		for j=1,n-i,1 do
			if(tb[j]>tb[j+1]==Asc) then
				local temp=tb[j]
				tb[j]=tb[j+1]
				tb[j+1]=temp
			end
		end
	end
end

function nFG(G,...)
	local args={...}
	local S={}
 	for i,v in pairs(args) do
		table.insert(S,v)
	end
	local cnt=0
	local cnt1=get_len(S)
	while(cnt1>cnt)do
		cnt=get_len(S)
		for ki,vi in pairs(S) do
			for kj,vj in pairs(S) do
				local ij=G[vi][vj]
				local IJ=get_key(ij,S)
				if(IJ==-1)then
					table.insert(S,ij)
				end
			end
		end
		cnt1=get_len(S)
	end
	BubbleAsc(S,true)
	return S
end

--秩为1的子群的阶的分布(经过排序)
function calS1(tb)
  local n=get_len(tb)
  local S1={}
  for i=1,n,1 do
   local oi=getGnEOrder(tb,i)
   table.insert(S1,oi)
  end
  BubbleAsc(S1,true)
  return S1
end

--两个元素生成的子群的阶的分布
function calS2Undo(tb)
  local n=get_len(tb)
  local S2={}
  for i=1,n,1 do
	  for j=i+1,n,1 do
	   if j<=n then
		   local ij=nFG(tb,i,j)
		   table.insert(S2,get_len(ij))
	   end
	  end
  end
  return S2
end

function Factors(n)
	local ret={}
	if(n<1)then
		return ret
	end
	for i=1,n,1 do
		if(n%i==0)then
			table.insert(ret,i)
		end
	end
	return ret
end

function getCnt(n,tb)
  local ret={}    
  local v=Factors(n) 
  local m=get_len(v) 
  local ret={} 
  for i=1,m,1 do
	ret[i]=0
  end  
  local t=0
  local n1=get_len(tb) 
  for i=1,n1,1 do
   local idx=get_key(tb[i],v)
   if(idx~=-1)then
	   ret[idx]=ret[idx]+1
   end
  end
  return ret
end

function calS(tb,SUndo)
  local n=get_len(tb)
  local S=getCnt(n,SUndo) 
  local m=get_len(S)  
  local s="["
  for i=1,m,1 do
   s=s..S[i]
   if(i<m)then
    s=s..","
   end
  end
  s=s.."]"
  return s
end

function calG(tb)
  local s=calS(tb,calS1(tb))
  return s
end

local g_N={4,6,8,10,12,14,16,18,20,24,30,32,36,48,60}
for k,v in pairs(g_N) do
    local nv=NumberSmallGroups(v)
	for i=1,nv,1 do
		local G8_i=SmallGroup(v,i)
		if G8_i~=nil then
			--io.write("GAP[",v,",",i,"]凯莱表:\n")
			--PrintArr2(G8_i)
			local G8_icalG=calG(G8_i)
			--io.write("GAP[",v,",",i,"]群元阶的分布:",G8_icalG,"\n\n")
			--io.write("GAP[",v,",",i,"]群元阶的分布:",G8_icalG,"\n")
			local G8_icalS2=calS(G8_i,calS2Undo(G8_i))
			io.write("GAP[",v,",",i,"]群元阶的分布:",G8_icalG,",S2=",G8_icalS2,"\n")
		end
	end
end

local d16=Dn(16)
for k,v in pairs(d16) do
	io.write(k,"->")
	PrintArr(v)
end

local qd16=QD8n(16)
for k,v in pairs(qd16) do
	io.write(k,"->")
	PrintArr2(v)
end
--[[
local G12_2=SmallGroup(12,2)
--local G12_2=CyclicGroup(12)
io.write("GAP[12,",2,"]凯莱表:\n")
PrintArr2(G12_2)
local G12_2calG=calG(G12_2)
io.write("GAP[12,",2,"]群元阶的分布:",G12_2calG,"\n")
]]

