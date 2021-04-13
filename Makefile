LEX 		:= flex
YACC 		:= bison
CC 			:= g++
CFLAGS 		:= -ll -ly -lm -O2 -Wall -std=c++11
TOP 		:= .
INCLUDE 	:= util/node.cc main.cc
YACCDEBUG	:= -v --report=all

all: eeyore-parser

eeyore-parser: eeyore-tab $(INCLUDE)
	$(CC) $(CFLAGS) -I$(TOP) SysY.tab.cc lex.yy.cc $(INCLUDE) -o parser.o

eeyore-tab: eeyore-lex $(INCLUDE)
	$(YACC) $(YACCDEBUG) -d -o SysY.tab.cc front/SysY.y

eeyore-lex: front/SysY.l
	$(LEX) -o lex.yy.cc front/SysY.l

clean:
	rm -f *.o *.output
	rm -f SysY.tab.cc lex.yy.cc SysY.tab.hh