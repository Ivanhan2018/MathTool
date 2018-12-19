-- 泛函∫[a->b]f(x)dx中的自变量f(x) 
function fun(x)
--    return math.sqrt(math.sqrt(4*x*x+1)-x*x-1)
--      return math.exp(-x*x/2)/math.sqrt(2*math.pi)
 return math.exp(-x)/math.sqrt(math.pow(x,3))
end

-- 泛函∑[n=1->100]f(n)≈∑[n=1->+∞]f(n)中的自变量f(n) 
function fan(n)
    return 1/(n*n*n)
end

-- 
function fac(n)
    if n == 0 then
        return 1 
    else 
        return n*fac(n-1)
    end 
end

-- 没问题
function fac2(n)
    local a = 1
    for i = 1,n do
        a = a*i
    end
    return a
end

function factdouble(n)
    local k = 1
    local t = 1
if math.mod(n,2)==1 then
    for k = 1,n,2 do
        t=k*t
    end
elseif math.mod(n,2)==0 then
     for k = 2,n,2 do
        t=k*t
    end
end
    return t
end


function fsx(x)
    local a=0
    for i=0,16,2 do
        a=a+(1/fac(i))
    end
    return a
end

function Isodd(n)
 local t=1
 if math.mod(n,2)==0 then
    t=-1
 end
 return t
end

-- 泛函∑[n=1->100]f(n,x)≈∑[n=1->+∞]f(n,x)中的自变量f(n,x) 
function fanx(n,x)
--    return Isodd(n)/(n+x)
--    return 1/(n*(n+x))
      return 1/(math.pow(2,n)+x)
end

--[[

]]
function fsxy(x,y)
     return -0.05*math.exp(-0.05*x)
end