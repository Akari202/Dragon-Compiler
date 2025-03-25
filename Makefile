LEX = flex
YACC = bison
CC = cc
CFLAGS = -Wall -ferror-limit=0 -std=c2x

dragon: dragon.tab.o lex.yy.o main.c
	$(CC) $(CFLAGS) -o dragon dragon.tab.o lex.yy.o main.c -ll -lm -ly

lex.yy.o: lex.yy.c lex.yy.h dragon.tab.h
	$(CC) $(CFLAGS) -c lex.yy.c

lex.yy.c lex.yy.h: dragon.l dragon.tab.h
	$(LEX) --header-file=lex.yy.h -l dragon.l

dragon.tab.o: dragon.tab.c dragon.tab.h
	$(CC) $(CFLAGS) -c dragon.tab.c

dragon.tab.c dragon.tab.h: dragon.y ast.h list.h
	$(YACC) -dv dragon.y

test_tree: ast.h test_tree.c
	$(CC) $(CFLAGS) -o test_tree test_tree.c

clean:
	-rm -f *.o lexer.* lex.yy.* *.tab.*  dragon *.output
	-rm -rf dragon.dSYM

all: dragon

.PHONY: all clean
