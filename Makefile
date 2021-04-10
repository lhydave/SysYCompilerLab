LEX 		:= flex
YACC 		:= bison
CC 			:= g++
CFLAGS 		:= -ll -ly -lm -O2 -Wall -std=c++11
TOP 		:= .
INCLUDE 	:= util/node.cc main.cc
YACCDEBUG	:= -v --report=all

all: eeyore-parser

eeyore-parser: eeyore-tab $(INCLUDE)
	$(CC) $(CFLAGS) -I$(TOP) MiniC.tab.cc lex.yy.cc $(INCLUDE) -o parser.o

eeyore-tab: eeyore-lex $(INCLUDE)
	$(YACC) $(YACCDEBUG) -d -o MiniC.tab.cc front/MiniC.y

eeyore-lex: front/MiniC.l
	$(LEX) -o lex.yy.cc front/MiniC.l

clean:
	rm -f *.o *.output
	rm -f MiniC.tab.cc lex.yy.cc MiniC.tab.hh