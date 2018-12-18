--2011.5.22
--把函数f(x)写在LUA脚本里，然后在VC/g++里面调用dll/so中的fsimp计算泛函R(a,b,eps,luafun),也即计算定积分∫[a,b]f(x)dx
--可以在程序运行时，在LUA脚本里面修改泛函的自变量f(x)--C/C++宿主代码中与f(x)对应的函数原型为double __stdcall luafun(double x);
--LUA脚本代码为：
function add(x,y)
    return x+y
end
function pythagorean(a,b) 
local c2 = a^2 + b^2 
return math.sqrt(c2) 
end
function fun(x)
    return math.sqrt(math.sqrt(4*x*x+1)-x*x-1)
end