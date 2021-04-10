LEX 		:= flex
YACC 		:= bison
CC 			:= g++
CFLAGS 		:= -ll -ly -lm -O2 -Wall -std=c++11
TOP 		:= .
INCLUDE 	:= util/node.cc
YACCDEBUG	:= -v --report=all

all: eeyore-parser

eeyore-parser: eeyore-tab $(INCLUDE)
	$(CC) $(CFLAGS) -I$(TOP) MiniC.tab.cc lex.yy.cc $(INCLUDE) -o parser.o

eeyore-tab: eeyore-lex $(INCLUDE)
	$(YACC) $(YACCDEBUG) -d -o MiniC.tab.cc front/MiniC.y

eeyore-lexer: eeyore-lex $(INCLUDE)
	$(CC) $(CFLAGS) -D LEXONLY -I$(TOP) lex.yy.cc $(INCLUDE) -o lexer.o

eeyore-lex: front/MiniC.l
	$(LEX) -o lex.yy.cc front/MiniC.l

clean:
	rm -f *.o *.output *.cc *.hh