LEX = lex
YACC = bison -d
CC = gcc

# calc is the final object that we will generate, it is produced by
# the C compiler from the y.tab.o and from the lex.yy.o

dragon: y.tab.o lex.yy.o
	$(CC) -o dragon y.tab.o lex.yy.o -ll -lm

# These dependency rules indicate that (1) lex.yy.o depends on
# lex.yy.c and y.tab.h and (2) lex.yy.o and y.tab.o depend on dragon.h.
# Make uses the dependencies to figure out what rules must be run when
# a file has changed.

lex.yy.o: lex.yy.c y.tab.h
lex.yy.o y.tab.o: dragon.h

## This rule will use yacc to generate the files y.tab.c and y.tab.h
## from our file calc.y

y.tab.c y.tab.h: dragon.y
	$(YACC) -v dragon.y

## this is the make rule to use lex to generate the file lex.yy.c from
## our file calc.l

lex.yy.c: dragon.l
	$(LEX) dragon.l

clean:
	-rm -f *.o lex.yy.c *.tab.*  dragon *.output
