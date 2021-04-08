LEX 	:= flex
YACC 	:= bison
CC 		:= g++
CFLAGS 	:= -ll -lm -O2 -Wall -std=c++11
TOP 	:= .
INCLUDE := util/attr.cc

lexer: lex $(INCLUDE)
	$(CC) $(CFLAGS) -D LEXONLY -I$(TOP) lex.yy.cc $(INCLUDE) -o lexer.o

lex: front/MiniC.l
	$(LEX) -o lex.yy.cc front/MiniC.l

clean:
	rm -f *.o
	rm -f lex.yy.cc