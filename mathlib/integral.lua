-- float(*fun)(float x)
--���η��󶨻���
function integral(fun,a,b,n)
local s,h,y
s=(fun(a)+fun(b))/2
h=(b-a)/n--���ֲ���
for i=1,n-1,1 do 
s=s+fun(a+i*h)
end
y=s*h
return y--���ػ���ֵ
end

function integralstr(f,a,b)
local s="��["..a.."->"..b.."]"..f.."dx"
return s
end

--��������
function f(x) 
	return x*math.sin(x)
	--return math.sqrt(x)
end

y=integral(f,0,1,150)
io.write(integralstr("x*sin(x)",0,1).."="..y,"\n")
y=integral(f,1,2,150)
io.write(integralstr("x*sin(x)",1,2).."="..y,"\n")

local function f1(x) return math.sqrt(x) end
y=integral(f1,0,1,150)
io.write(integralstr("sqrt(x)",0,1).."="..y,"\n")
y=integral(f1,1,2,150)
io.write(integralstr("sqrt(x)",1,2).."="..y,"\n")
