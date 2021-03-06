function get_len(tb)
	local len=0
	for k,v in pairs(tb) do
		len=len+1
	end
	return len
end

function doArr2(vv,delt)
 local m=get_len(vv)
 for i=1,m,1 do
  for j=1,m,1 do
   vv[i][j]=vv[i][j]+delt
  end
 end
end

g_E8={
{1,2,3,4,5,6,7,8},
{2,1,8,7,6,5,4,3},
{3,8,1,5,4,7,6,2},
{4,7,5,1,3,8,2,6},
{5,6,4,3,1,2,8,7},
{6,5,7,8,2,1,3,4},
{7,4,6,2,8,3,1,5},
{8,3,2,6,7,4,5,1}
}

g_R8_0001Mul={
{1,1,1,1,1,1,1,1},
{1,7,3,5,4,2,8,6},
{1,3,1,1,1,3,3,3},
{1,5,1,3,3,4,4,5},
{1,4,1,3,3,5,5,4},
{1,2,3,4,5,6,7,8},
{1,8,3,4,5,7,6,2},
{1,6,3,5,4,8,2,7}
}

g_R8_1011Mul={
{1,1,1,1,1,1,1,1},
{1,4,5,8,7,2,6,3},
{1,5,2,7,4,3,8,6},
{1,8,7,3,6,4,2,5},
{1,7,4,6,8,5,3,2},
{1,2,3,4,5,6,7,8},
{1,6,8,2,3,7,5,4},
{1,3,6,5,2,8,4,7}
}

doArr2(g_E8,-1)
doArr2(g_R8_0001Mul,-1)
doArr2(g_R8_1011Mul,-1)
local calR8_46=calring("g_E8","g_R8_0001Mul","R8_46=Z/(2)[x]/(x^3)")
print(calR8_46)
local calR8_52=calring("g_E8","g_R8_1011Mul","R8_52=Z/(2)[x]/(x^3+x^2+1)")
print(calR8_52)
