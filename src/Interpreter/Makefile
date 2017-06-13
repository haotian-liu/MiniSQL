# This makefile is for the target generated source code files complied by flex and bison.

YACC=bison -dy
LEX=flex

all: scan.c parse.c

parse.c: parse.y
	$(YACC) parse.y; mv y.tab.c parse.c

scan.c: scan.l
	$(LEX) scan.l; mv lex.yy.c scan.c

clean:
	rm parse.c scan.c
