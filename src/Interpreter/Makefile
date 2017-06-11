# This makefile is for the target generated source code files complied by flex and bison.

YACC=bison
LEX=flex

all: scan.cpp parse.cpp

parse.cpp: parse.y
	$(YACC) parse.y; mv parse.tab.c parse.cpp

scan.cpp: scan.l
	$(LEX) scan.l; mv lex.yy.c scan.cpp

clean:
	rm parse.cpp scan.cpp
