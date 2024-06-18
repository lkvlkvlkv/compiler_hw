all: compiler

OBJS = grammer.o \
		token.o  \
		codeGen.o \
		main.o	 \
		objGen.o \

LLVMCONFIG = llvm-config
INCLUDEDIR = `$(LLVMCONFIG) --includedir`
CFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++17
LDFLAGS = `$(LLVMCONFIG) --ldflags`
LIBS = `$(LLVMCONFIG) --libs`

compiler: $(OBJS)
		g++ -I $(INCLUDEDIR) $(OBJS) -o $@ $(LIBS) $(LDFLAGS) $(CFLAGS)

%.o: %.cpp
		g++ -I $(INCLUDEDIR) -c $< $(LIBS) $(LDFLAGS) $(CFLAGS)

token.o: token.cpp grammer.hpp
		g++ -I $(INCLUDEDIR) -c token.cpp $(LIBS) $(LDFLAGS) $(CFLAGS)

grammer.o: grammer.cpp
		g++ -I $(INCLUDEDIR) -c grammer.cpp $(LIBS) $(LDFLAGS) $(CFLAGS)

grammer.cpp grammer.hpp: grammer.y
		bison -d grammer.y -o grammer.cpp

token.cpp: token.l
		flex token.l

clean:
		-rm *.o token.cpp grammer.hpp grammer.cpp compiler.exe

run:
		./compiler.exe < correct_code.c