all: compiler

OBJS = grammar.o \
		token.o  \
		codeGen.o \
		main.o	 \
		objGen.o \
		typeSystem.o \

LLVMCONFIG = llvm-config
INCLUDEDIR = `$(LLVMCONFIG) --includedir`
CFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++17
LDFLAGS = `$(LLVMCONFIG) --ldflags`
LIBS = `$(LLVMCONFIG) --libs`
MINGW = `$(LLVMCONFIG) --system-libs`

compiler: $(OBJS)
		g++ -I $(INCLUDEDIR) $(OBJS) -o $@ $(LIBS) $(LDFLAGS) $(CFLAGS) $(MINGW)

%.o: %.cpp
		g++ -I $(INCLUDEDIR) -c $< $(LIBS) $(LDFLAGS) $(CFLAGS)

codeGen.o: codeGen.cpp grammar.hpp codeGen.hpp
		g++ -I $(INCLUDEDIR) -c codeGen.cpp $(LIBS) $(LDFLAGS) $(CFLAGS)

token.o: token.cpp grammar.hpp
		g++ -I $(INCLUDEDIR) -c token.cpp $(LIBS) $(LDFLAGS) $(CFLAGS)

grammar.o: grammar.cpp
		g++ -I $(INCLUDEDIR) -c grammar.cpp $(LIBS) $(LDFLAGS) $(CFLAGS)

grammar.cpp grammar.hpp: grammar.y
		bison -d grammar.y -o grammar.cpp

token.cpp: token.l
		flex token.l

clean:
		-rm *.o token.cpp grammar.hpp grammar.cpp compiler.exe

run:
		./compiler.exe < correct_code.c
		g++ output.o IO.o -o output
		./output.exe