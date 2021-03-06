-- simple test
-- call a C++ function
--[[
avg, sum=average(10, 20, 30, 40, 50)
print("The average is ", avg)
print("The sum is ", sum)
--The average is  30
--The sum is      150

function testSmallRing(delt)
	for n=2,3 do
		for id=1,2 do
		local R=smallring(n,id)
			printringtable(R,delt)
		end
	end
end
testSmallRing(1)
--testSmallRing(0)

function testDirectSum(delt)
	for n1=2,3 do
		for id1=1,2 do
			for n2=2,3 do
				for id2=1,2 do
					if n2>=n1 and id2>=id1 then
						local R1=smallring(n1,id1)
						local R2=smallring(n2,id2)
						printdirectsum(R1,R2,delt)
					end
				end
			end
		end
	end
end
testDirectSum(1)
testDirectSum(0)
]]
--[[
local sArr = {"Hanxiao","我们C/C++ Tips","qwertyuiopasdfghjklzxcvbnm"}
for i=1,3,1 do
	local s=sArr[i]
	local s1=string.lower(s)--把字符串转换为全小写
	local s2=string.upper(s)--把字符串转换为全大写
	local s1a=mylower(s)--把字符串转换为全小写
	local s2a=myupper(s)--把字符串转换为全大写	
	local s3=string.reverse(s)--把字符串翻转 	
	print(s,"->",s1)
	print(s,"->",s1a)
	print(s,"->",s2)	
	print(s,"->",s2a)
	print(s,"->",s3)
	print(s,"->",s3a)		
end
]]

--[[
g_R3_2R3_2Add={
{0,1,2,3,4,5,6,7,8},
{1,2,0,4,5,3,7,8,6},
{2,0,1,5,3,4,8,6,7},
{3,4,5,6,7,8,0,1,2},
{4,5,3,7,8,6,1,2,0},
{5,3,4,8,6,7,2,0,1},
{6,7,8,0,1,2,3,4,5},
{7,8,6,1,2,0,4,5,3},
{8,6,7,2,0,1,5,3,4}
}

g_R3_2R3_2Mul={
{0,0,0,0,0,0,0,0,0},
{0,1,2,0,1,2,0,1,2},
{0,2,1,0,2,1,0,2,1},
{0,0,0,3,3,3,6,6,6},
{0,1,2,3,4,5,6,7,8},
{0,2,1,3,5,4,6,8,7},
{0,0,0,6,6,6,3,3,3},
{0,1,2,6,7,8,3,4,5},
{0,2,1,6,8,7,3,5,4}
}

local calF3F3=calring("g_R3_2R3_2Add","g_R3_2R3_2Mul","R9_10")
print(calF3F3)

g_R8_10Add={
{0,1,2,3,4,5,6,7},
{1,3,6,4,0,7,5,2},
{2,6,0,5,7,3,1,4},
{3,4,5,0,1,2,7,6},
{4,0,7,1,3,6,2,5},
{5,7,3,2,6,0,4,1},
{6,5,1,7,2,4,3,0},
{7,2,4,6,5,1,0,3}
}

g_R8_10Mul={
{0,0,0,0,0,0,0,0},
{0,3,3,0,3,3,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,3,3,0,3,3,0,0},
{0,0,0,0,0,0,0,0},
{0,3,3,0,3,3,0,0},
{0,3,3,0,3,3,0,0}
}
local calR8_10=calring("g_R8_10Add","g_R8_10Mul","R8_10")
print(calR8_10)
]]

g_C8Mul={
	{0, 1, 2, 3, 4, 5, 6, 7},
	{1, 0, 3, 2, 5, 4, 7, 6},
	{2, 3, 1, 0, 6, 7, 5, 4},
	{3, 2, 0, 1, 7, 6, 4, 5},
	{4, 5, 6, 7, 2, 3, 1, 0},
	{5, 4, 7, 6, 3, 2, 0, 1},
	{6, 7, 5, 4, 1, 0, 3, 2},
	{7, 6, 4, 5, 0, 1, 2, 3}
}

g_C8Mul_2={
	{0,1,2,3,4,5,6,7},
	{1,2,3,4,5,6,7,0},
	{2,3,4,5,6,7,0,1},
	{3,4,5,6,7,0,1,2},
	{4,5,6,7,0,1,2,3},
	{5,6,7,0,1,2,3,4},
	{6,7,0,1,2,3,4,5},
	{7,0,1,2,3,4,5,6}
}

g_C2C4Mul={
	{0, 1, 2, 3, 4, 5, 6, 7},
	{1, 0, 3, 2, 5, 4, 7, 6},
	{2, 3, 0, 1, 6, 7, 4, 5},
	{3, 2, 1, 0, 7, 6, 5, 4},
	{4, 5, 6, 7, 1, 0, 3, 2},
	{5, 4, 7, 6, 0, 1, 2, 3},
	{6, 7, 4, 5, 3, 2, 1, 0},
	{7, 6, 5, 4, 2, 3, 0, 1}
}

g_C2C4Mul_2={
	{0,1,2,3,4,5,6,7},
	{1,4,7,2,5,0,3,6},
	{2,7,4,1,6,3,0,5},
	{3,2,1,0,7,6,5,4},
	{4,5,6,7,0,1,2,3},
	{5,0,3,6,1,4,7,2},
	{6,3,0,5,2,7,4,1},
	{7,6,5,4,3,2,1,0}
}

g_C2C2C2Mul={
	{0, 1, 2, 3, 4, 5, 6, 7},
	{1, 0, 3, 2, 5, 4, 7, 6},
	{2, 3, 0, 1, 6, 7, 4, 5},
	{3, 2, 1, 0, 7, 6, 5, 4},
	{4, 5, 6, 7, 0, 1, 2, 3},
	{5, 4, 7, 6, 1, 0, 3, 2},
	{6, 7, 4, 5, 2, 3, 0, 1},
	{7, 6, 5, 4, 3, 2, 1, 0}
}

g_D4Mul={
	{0, 1, 2, 3, 4, 5, 6, 7},
	{1, 0, 3, 2, 5, 4, 7, 6},
	{2, 3, 0, 1, 7, 6, 5, 4},
	{3, 2, 1, 0, 6, 7, 4, 5},
	{4, 5, 6, 7, 0, 1, 2, 3},
	{5, 4, 7, 6, 1, 0, 3, 2},
	{6, 7, 4, 5, 3, 2, 1, 0},
	{7, 6, 5, 4, 2, 3, 0, 1}
}

g_Q8Mul={
	{0, 1, 2, 3, 4, 5, 6, 7},
	{1, 0, 3, 2, 5, 4, 7, 6},
	{2, 3, 1, 0, 7, 6, 4, 5},
	{3, 2, 0, 1, 6, 7, 5, 4},
	{4, 5, 6, 7, 1, 0, 3, 2},
	{5, 4, 7, 6, 0, 1, 2, 3},
	{6, 7, 5, 4, 2, 3, 1, 0},
	{7, 6, 4, 5, 3, 2, 0, 1}
}

G12_1={
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
}

G12_2={
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 },
	{2, 1, 4, 3, 6, 5, 8, 7, 10, 9, 12, 11 },
	{3, 4, 5, 6, 1, 2, 9, 10, 11, 12, 7, 8 },
	{4, 3, 6, 5, 2, 1, 10, 9, 12, 11, 8, 7 },
	{5, 6, 1, 2, 3, 4, 11, 12, 7, 8, 9, 10 },
	{6, 5, 2, 1, 4, 3, 12, 11, 8, 7, 10, 9},
	{7, 8, 9, 10, 11, 12, 2, 1, 4, 3, 6, 5},
	{8, 7, 10, 9, 12, 11, 1, 2, 3, 4, 5, 6},
	{9, 10, 11, 12, 7, 8, 4, 3, 6, 5, 2, 1},
	{10, 9, 12, 11, 8, 7, 3, 4, 5, 6, 1, 2},
	{11, 12, 7, 8, 9, 10, 6, 5, 2, 1, 4, 3},
	{12, 11, 8, 7, 10, 9, 5, 6, 1, 2, 3, 4}
}

G12_3={
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 },
	{2, 1, 4, 3, 8, 7, 6, 5, 11, 12, 9, 10 },
	{3, 4, 1, 2, 6, 5, 8, 7, 12, 11, 10, 9 },
	{4, 3, 2, 1, 7, 8, 5, 6, 10, 9, 12, 11 },
	{5, 6, 7, 8, 9, 10, 11, 12, 1, 2, 3, 4 },
	{6, 5, 8, 7, 12, 11, 10, 9, 3, 4, 1, 2},
	{7, 8, 5, 6, 10, 9, 12, 11, 4, 3, 2, 1},
	{8, 7, 6, 5, 11, 12, 9, 10, 2, 1, 4, 3},
	{9, 10, 11, 12, 1, 2, 3, 4, 5, 6, 7, 8},
	{10, 9, 12, 11, 4, 3, 2, 1, 7, 8, 5, 6},
	{11, 12, 9, 10, 2, 1, 4, 3, 8, 7, 6, 5},
	{12, 11, 10, 9, 3, 4, 1, 2, 6, 5, 8, 7}
}

G12_4={
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 },
	{2, 3, 1, 5, 6, 4, 9, 7, 8, 12, 10, 11  },
	{3, 1, 2, 6, 4, 5, 8, 9, 7, 11, 12, 10 },
	{4, 5, 6, 1, 2, 3, 10, 11, 12, 7, 8, 9 },
	{5, 6, 4, 2, 3, 1, 12, 10, 11, 9, 7, 8 },
	{6, 4, 5, 3, 1, 2, 11, 12, 10, 8, 9, 7},
	{7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6},
	{8, 9, 7, 11, 12, 10, 3, 1, 2, 6, 4, 5},
	{9, 7, 8, 12, 10, 11, 2, 3, 1, 5, 6, 4},
	{10, 11, 12, 7, 8, 9, 4, 5, 6, 1, 2, 3},
	{11, 12, 10, 8, 9, 7, 6, 4, 5, 3, 1, 2},
	{12, 10, 11, 9, 7, 8, 5, 6, 4, 2, 3, 1}
}

G12_5={
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 },
	{2, 3, 1, 5, 6, 4, 8, 9, 7, 11, 12, 10 },
	{3, 1, 2, 6, 4, 5, 9, 7, 8, 12, 10, 11 },
	{4, 5, 6, 1, 2, 3, 10, 11, 12, 7, 8, 9 },
	{5, 6, 4, 2, 3, 1, 11, 12, 10, 8, 9, 7 },
	{6, 4, 5, 3, 1, 2, 12, 10, 11, 9, 7, 8},
	{7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6 },
	{8, 9, 7, 11, 12, 10, 2, 3, 1, 5, 6, 4},
	{9, 7, 8, 12, 10, 11, 3, 1, 2, 6, 4, 5},
	{10, 11, 12, 7, 8, 9, 4, 5, 6, 1, 2, 3},
	{11, 12, 10, 8, 9, 7, 5, 6, 4, 2, 3, 1},
	{12, 10, 11, 9, 7, 8, 6, 4, 5, 3, 1, 2}
}

--[[lua5.3开始支持位操作，lua5.1中可使用c提供的位操作函数接口Xor、Add、Or
enum IsGroupEnum
{
	IsGroup_None=0,//不是群
	IsGroup_bG=0x00000001,//是群
	IsGroup_bC=0x00000002,//循环群
	IsGroup_bA=0x00000004,//交换群
	IsGroup_bN=0x00000008,//幂零群
	IsGroup_bS=0x00000010,//可解群
};
]]
function int(b)
	-- return tostring(b)
	if b then
		return 1
	else
		return 0
	end
end

function CANS(ret)
   local ss=""
   local bG=And(ret,1)~=0
   if bG then
      local bC=And(ret,2)~=0
      local bA=And(ret,4)~=0
      local bN=And(ret,8)~=0	
      local bS=(And(ret,16)~=0)		
      local s=""
      if bC then
         s="是循环群"
      else
		  if bA then
			 s="是非循环Abel群"
		  else
			  if bN then
				 s="是非Abel幂零群"
			  else
				  if bS then
					 s="是非幂零可解群"
				  else
					 s="不是可解群"
				  end
			  end
		  end	
      end		 
      ss=s.."(ret="..ret..",bG="..int(bG)..",bC="..int(bC)..",bA="..int(bA)..",bN="..int(bN)..",bS="..int(bS)..")"
   else
      ss="不是群" 
   end
   return ss
end

function printisgroup(name,ret)
   local ss=CANS(ret)
   io.write(name..ss.."\n")
end

local calN0C8=calN0("g_C8Mul","GAP[8,1]",1)
local calCANSG8=CANS(isgroup("g_C8Mul",1))
print(calN0C8..calCANSG8)

calN0C8=calN0("g_C8Mul_2","GAP[8,1]",1)
calCANSG8=CANS(isgroup("g_C8Mul_2",1))
print(calN0C8..calCANSG8)

local calN0C2C4=calN0("g_C2C4Mul","GAP[8,2]",1)
calCANSG8=CANS(isgroup("g_C2C4Mul",1))
print(calN0C2C4..calCANSG8)

calN0C2C4=calN0("g_C2C4Mul_2","GAP[8,2]",1)
calCANSG8=CANS(isgroup("g_C2C4Mul_2",1))
print(calN0C2C4..calCANSG8)

calN0E8=calN0("g_C2C2C2Mul","GAP[8,5]",1)
calCANSG8=CANS(isgroup("g_C2C2C2Mul",1))
print(calN0E8..calCANSG8)

calN0D4=calN0("g_D4Mul","GAP[8,3]",1)
calCANSG8=CANS(isgroup("g_D4Mul",1))
print(calN0D4..calCANSG8)

calN0Q8=calN0("g_Q8Mul","GAP[8,4]",1)
calCANSG8=CANS(isgroup("g_Q8Mul",1))
print(calN0Q8..calCANSG8)

for i=1,5 do
    local name="GAP[12,"..i.."]"
	calN0G12_i=calN0("G12_"..i,name,0)
	local ret=isgroup("G12_"..i)
	local ss=CANS(ret)	
	print(calN0G12_i..ss)
end

G12_6={
	{1,2,3,4,5,6,7,8,9,10,11,12},
	{2,1,4,3,10,9,8,7,6,5,12,11},
	{3,4,1,2,11,12,9,10,7,8,5,6},
	{4,3,2,1,8,7,10,9,12,11,6,5},
	{5,6,7,8,9,10,11,12,1,2,3,4},
	{6,5,8,7,2,1,12,11,10,9,4,3},
	{7,8,5,6,3,4,1,2,11,12,9,10},
	{8,7,6,5,12,11,2,1,4,3,10,9},
	{9,10,11,12,1,2,3,4,5,6,7,8},
	{10,9,12,11,6,5,4,3,2,1,8,7},
	{11,12,9,10,7,8,5,6,3,4,1,2},
	{12,11,10,9,4,3,6,5,8,7,2,1}
}
isG12_6=isgroup("G12_6")
printisgroup("G12_6",isG12_6)

G16_15={
	{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},
	{2,1,4,3,14,13,8,7,10,9,12,11,6,5,16,15},
	{3,4,1,2,15,16,13,14,11,12,9,10,7,8,5,6},
	{4,3,2,1,8,7,14,13,12,11,10,9,16,15,6,5},
	{5,6,7,8,9,10,11,12,13,14,15,16,1,2,3,4},
	{6,5,8,7,2,1,12,11,14,13,16,15,10,9,4,3},
	{7,8,5,6,3,4,1,2,15,16,13,14,11,12,9,10},
	{8,7,6,5,12,11,2,1,16,15,14,13,4,3,10,9},
	{9,10,11,12,13,14,15,16,1,2,3,4,5,6,7,8},
	{10,9,12,11,6,5,16,15,2,1,4,3,14,13,8,7},
	{11,12,9,10,7,8,5,6,3,4,1,2,15,16,13,14},
	{12,11,10,9,16,15,6,5,4,3,2,1,8,7,14,13},
	{13,14,15,16,1,2,3,4,5,6,7,8,9,10,11,12},
	{14,13,16,15,10,9,4,3,6,5,8,7,2,1,12,11},
	{15,16,13,14,11,12,9,10,7,8,5,6,3,4,1,2},
	{16,15,14,13,4,3,10,9,8,7,6,5,12,11,2,1}
}	
isG16_15=isgroup("G16_15")
printisgroup("G16_15",isG16_15)

G16_16={
	{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},
	{2,1,4,3,8,7,6,5,12,11,10,9,16,15,14,13},
	{3,4,1,2,7,8,5,6,11,12,9,10,15,16,13,14},
	{4,3,2,1,6,5,8,7,10,9,12,11,14,13,16,15},
	{5,6,7,8,3,4,1,2,15,16,13,14,9,10,11,12},
	{6,5,8,7,2,1,4,3,14,13,16,15,12,11,10,9},
	{7,8,5,6,1,2,3,4,13,14,15,16,11,12,9,10},
	{8,7,6,5,4,3,2,1,16,15,14,13,10,9,12,11},
	{9,10,11,12,13,14,15,16,3,4,1,2,7,8,5,6},
	{10,9,12,11,16,15,14,13,2,1,4,3,6,5,8,7},
	{11,12,9,10,15,16,13,14,1,2,3,4,5,6,7,8},
	{12,11,10,9,14,13,16,15,4,3,2,1,8,7,6,5},
	{13,14,15,16,11,12,9,10,5,6,7,8,3,4,1,2},
	{14,13,16,15,10,9,12,11,8,7,6,5,2,1,4,3},
	{15,16,13,14,9,10,11,12,7,8,5,6,1,2,3,4},
	{16,15,14,13,12,11,10,9,6,5,8,7,4,3,2,1}
}
isG16_16=isgroup("G16_16")
printisgroup("G16_16",isG16_16)

G6_1={
	{1,2,3,4,5,6},
	{2,1,6,5,4,3},
	{3,4,5,6,1,2},
	{4,3,2,1,6,5},
	{5,6,1,2,3,4},
	{6,5,4,3,2,1}
}
isG6_1=isgroup("G6_1")
printisgroup("G6_1",isG6_1)
