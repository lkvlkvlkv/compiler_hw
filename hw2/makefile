LEX=flex
YACC=bison
CC=gcc
OBJECT=parser

$(OBJECT): lex.yy.o yacc.tab.o
		$(CC) lex.yy.o yacc.tab.o -o $(OBJECT)

lex.yy.o: lex.yy.c yacc.tab.h
		$(CC) -c lex.yy.c

yacc.tab.o: yacc.tab.c
		$(CC) -c yacc.tab.c

yacc.tab.c yacc.tab.h: yacc.y
		$(YACC) -d yacc.y

lex.yy.c: lex.l
		$(LEX) lex.l

clean:
		-rm *.o lex.yy.c yacc.tab.h yacc.tab.c parser.exe

run:
		./$(OBJECT).exe < source_code.c