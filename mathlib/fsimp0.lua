--2011.5.22
--�Ѻ���f(x)д��LUA�ű��Ȼ����VC/g++�������dll/so�е�fsimp���㷺��R(a,b,eps,luafun),Ҳ�����㶨���֡�[a,b]f(x)dx
--�����ڳ�������ʱ����LUA�ű������޸ķ������Ա���f(x)--C/C++������������f(x)��Ӧ�ĺ���ԭ��Ϊdouble __stdcall luafun(double x);
--LUA�ű�����Ϊ��
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