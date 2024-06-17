LEX=flex
YACC=bison
CC=g++
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
		./$(OBJECT).exe < correct_code.c

run_error:
		- ./$(OBJECT).exe < "./error_code/error_code1.c" > "./error_code/error_report1.txt"
		- ./$(OBJECT).exe < "./error_code/error_code2.c" > "./error_code/error_report2.txt"
		- ./$(OBJECT).exe < "./error_code/error_code3.c" > "./error_code/error_report3.txt"
		- ./$(OBJECT).exe < "./error_code/error_code4.c" > "./error_code/error_report4.txt"
		- ./$(OBJECT).exe < "./error_code/error_code5.c" > "./error_code/error_report5.txt"
		- ./$(OBJECT).exe < "./error_code/error_code6.c" > "./error_code/error_report6.txt"
		- ./$(OBJECT).exe < "./error_code/error_code7.c" > "./error_code/error_report7.txt"
		- ./$(OBJECT).exe < "./error_code/error_code8.c" > "./error_code/error_report8.txt"
		- ./$(OBJECT).exe < "./error_code/error_code9.c" > "./error_code/error_report9.txt"
		- ./$(OBJECT).exe < "./error_code/error_code10.c" > "./error_code/error_report10.txt"