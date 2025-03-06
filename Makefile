LEX = flex
YACC = bison -d
CC = cc
CFLAGS = -g -ferror-limit=0 -std=c17

dragon: dragon.tab.o lex.yy.o dragon.c
	$(CC) $(CFLAGS) -o dragon dragon.c dragon.tab.o lex.yy.o -ll -lm -ly

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c
dragon.tab.o: dragon.tab.c dragon.tab.h
	$(CC) $(CFLAGS) -c dragon.tab.c

dragon.tab.c dragon.tab.h: dragon.y
	$(YACC) -dv dragon.y

lex.yy.c: dragon.l
	$(LEX) -l dragon.l

test: dragon
	@for testfile in ./Testing/Semantic/*; do \
		echo "Running test on $$testfile"; \
		./dragon < $$testfile; \
	done

clean:
	-rm -f *.o lex.yy.c *.tab.*  dragon *.output

all: dragon

.PHONY: all clean
