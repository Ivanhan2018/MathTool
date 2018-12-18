CC      = gcc
CXX     = g++
AR      = ar
TARGET	= liblua5.1.a
CCFLAGS = -Wall -std=gnu99
CXXFLAGS = -Wall 
ARFLAGS = cr

INCLUDES = -I.

DEFINES = -DLINUX

OBJECTS = ./lapi.o \
			./lauxlib.o \
			./lbaselib.o \
			./lcode.o \
			./ldblib.o \
			./ldebug.o \
			./ldo.o \
			./ldump.o \
			./lfunc.o \
			./lgc.o \
			./linit.o \
			./liolib.o \
			./llex.o \
			./lmathlib.o \
			./lmem.o \
			./loadlib.o \
			./lobject.o \
			./lopcodes.o \
			./loslib.o \
			./lparser.o \
			./lstate.o \
			./lstring.o \
			./lstrlib.o \
			./ltable.o \
			./ltablib.o \
			./ltm.o \
			./lua.o \
			./lundump.o \
			./lvm.o \
                        ./lzio.o \
                        ./print.o \

debug: CCFLAGS += -g3 -O0
debug: CXXFLAGS += -g3 -O0
debug: DEFINES += -DDEBUG
debug: $(TARGET)

release: CCFLAGS += -O3
release: CXXFLAGS += -O3
release: DEFINES += -DNDEBIG
release: $(TARGET)

####### Build rules
$(TARGET): $(OBJECTS) 
	$(AR) $(ARFLAGS) $(TARGET) $(OBJECTS)  

####### Compile
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

%.o: %.c
	$(CC) $(CCFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

clean: 
	rm -f $(OBJECTS) $(TARGET) core
