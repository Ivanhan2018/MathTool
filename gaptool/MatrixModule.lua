function get_len(tb)
	local len=0
	for k,v in pairs(tb) do
		len=len+1
	end
	return len
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
--[[
'  模块名：MatrixModule.lua
'  函数名：MJacobiEigenv
'  功能：  用雅可比法（Jacobi）计算对称矩阵的特征值和特征向量
'  参数：   n    - Integer型变量，对称矩阵的阶数。
'          dblA - Double型二维数组，体积为n x n。存放对称矩阵；返回时，对角线上存放求得的n个特征值
'          dblV - Double型二维数组，体积为n x n。返回n个特征向量，其中第i列为第i个特征值dblA(i,i)对应的特征向量。
'          eps  - Double型变量。迭代过程中的控制精度参数。
'          nMaxItNum    - Integer。为求得一个特征值所允许的最大迭代次数。
'  返回值： Boolean型。False，失败无解；True, 成功
]]
function MJacobiEigenv(dblA, dblV, eps, nMaxItNum)
    local n=get_len(dblA)
    local p,q,fm
    local l = 1
    for i = 1,n do
        dblV[i][i] = 1
        for j = 1,n do
          if (i~=j) then 
			dblV[i][j] = 0
		  end
        end
    end
    while (true) do
        local fm = 0
        for i = 2,n do
            for j = 1,i-1 do
                local d = math.abs(dblA[i][j])
                if ((i ~= j) and (d > fm)) then
                    fm = d
                    p = i
                    q = j
                end
            end
        end
        if (fm < eps) then
            return true
        end
        if (l > nMaxItNum) then
            return false
        end
        l = l + 1
        local x = -dblA[p][q]
        local y = (dblA[q][q] - dblA[p][p]) / 2
        local omega = x / math.sqrt(x * x + y * y)
        if (y < 0) then 
			omega = -omega
		end
        local sn = 1 + math.sqrt(1 - omega * omega)
        sn = omega / math.sqrt(2 * sn)
        local cn = math.sqrt(1 - sn * sn)
        fm = dblA[p][p]
        dblA[p][p] = fm * cn * cn + dblA[q][q] * sn * sn + dblA[p][q] * omega
        dblA[q][q] = fm * sn * sn + dblA[q][q] * cn * cn - dblA[p][q] * omega
        dblA[p][q] = 0
        dblA[q][p] = 0
        for j = 1,n do
            if ((j ~= p) and (j ~= q)) then
                fm = dblA[p][j]
                dblA[p][j] = fm * cn + dblA[q][j] * sn
                dblA[q][j] = -fm * sn + dblA[q][j] * cn
            end
        end
        for i = 1,n do
          if ((i ~= p) and (i ~= q)) then
              fm = dblA[i][p]
              dblA[i][p] = fm * cn + dblA[i][q] * sn
              dblA[i][q] = -fm * sn + dblA[i][q] * cn
          end
        end
        for i = 1,n do
            fm = dblV[i][p]
            dblV[i][p] = fm * cn + dblV[i][q] * sn
            dblV[i][q] = -fm * sn + dblV[i][q] * cn
        end
    end
end

local mtxA={{1,2},{2,3}}
local mtxB=tbl_copy(mtxA)
local mtxV=ZeroM(2)
--求解特征值和特征向量
local msg=""
if MJacobiEigenv(mtxA, mtxV, 0.000001, 60) then
	msg = "求解成功！原对称矩阵为"..MStr(mtxB)..",特征值为λ_1="..mtxA[1][1]..",λ_2="..mtxA[2][2]..",特征向量为V_1=["..mtxV[1][1]..","..mtxV[2][1].."],V_2=[" ..mtxV[1][2]..","..mtxV[2][2].."]。"
else
	msg = "求解失败！"
end
print(msg)