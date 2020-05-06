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
'  ģ������MatrixModule.lua
'  ��������MJacobiEigenv
'  ���ܣ�  ���ſɱȷ���Jacobi������Գƾ��������ֵ����������
'  ������   n    - Integer�ͱ������Գƾ���Ľ�����
'          dblA - Double�Ͷ�ά���飬���Ϊn x n����ŶԳƾ��󣻷���ʱ���Խ����ϴ����õ�n������ֵ
'          dblV - Double�Ͷ�ά���飬���Ϊn x n������n���������������е�i��Ϊ��i������ֵdblA(i,i)��Ӧ������������
'          eps  - Double�ͱ��������������еĿ��ƾ��Ȳ�����
'          nMaxItNum    - Integer��Ϊ���һ������ֵ�������������������
'  ����ֵ�� Boolean�͡�False��ʧ���޽⣻True, �ɹ�
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
--�������ֵ����������
local msg=""
if MJacobiEigenv(mtxA, mtxV, 0.000001, 60) then
	msg = "���ɹ���ԭ�Գƾ���Ϊ"..MStr(mtxB)..",����ֵΪ��_1="..mtxA[1][1]..",��_2="..mtxA[2][2]..",��������ΪV_1=["..mtxV[1][1]..","..mtxV[2][1].."],V_2=[" ..mtxV[1][2]..","..mtxV[2][2].."]��"
else
	msg = "���ʧ�ܣ�"
end
print(msg)