LEX = flex
YACC = bison
CC = cc
#CFLAGS = -Wall -ferror-limit=0 -std=c2x
CFLAGS = -std=c2x

.PHONY: all clean test run_tests

all: dragon test

clean:
	-rm -f *.o lexer.* lex.yy.* *.tab.*  dragon *.output
	-rm -rf dragon.dSYM
	-rm -f test_symbol_table test_ast test_list

dragon: dragon.tab.o lex.yy.o main.c symbol_table.o ast.o
	$(CC) $(CFLAGS) -o dragon dragon.tab.o lex.yy.o symbol_table.o ast.o main.c -ll -lm -ly

lex.yy.o: lex.yy.c lex.yy.h dragon.tab.h
	$(CC) $(CFLAGS) -c lex.yy.c

lex.yy.c lex.yy.h: dragon.l dragon.tab.h
	$(LEX) --header-file=lex.yy.h -l dragon.l

dragon.tab.o: dragon.tab.c dragon.tab.h
	$(CC) $(CFLAGS) -c dragon.tab.c

dragon.tab.c dragon.tab.h: dragon.y ast.h list.h symbol_table.h
	$(YACC) -dv dragon.y

symbol_table.o: symbol_table.c symbol_table.h
	$(CC) $(CFLAGS) -c symbol_table.c

test_symbol_table: test_symbol_table.c symbol_table.o
	$(CC) $(CFLAGS) -o test_symbol_table test_symbol_table.c symbol_table.o

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c

test_ast: test_ast.c ast.o
	$(CC) $(CFLAGS) -o test_ast test_ast.c ast.o

test_list: list.h test_list.c
	$(CC) $(CFLAGS) -o test_list test_list.c

test: test_symbol_table test_ast test_list

run_tests: test
	./test_symbol_table
	./test_ast
	./test_list



