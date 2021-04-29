LEX 		:= flex
YACC 		:= bison
CC 			:= g++
CDEBUG		:= -g -D DEBUG
CFLAGS 		:= -lm -O0 -Wall -std=c++11 $(CDEBUG)
TOP 		:= .
INCLUDE 	:= node.cpp symtab.cpp main.cpp
YACCDEBUG	:= -v --report=all

all: eeyore-parser

eeyore-parser: eeyore-tab $(INCUDE)
	$(CC) $(CFLAGS) -I$(TOP) SysY.tab.cpp lex.yy.cpp $(INCLUDE) -o parser.o

eeyore-tab: eeyore-lex $(INCLUDE)
	$(YACC) $(YACCDEBUG) -d -o SysY.tab.cpp SysY.y

eeyore-lex: SysY.l
	$(LEX) -o lex.yy.cpp SysY.l

clean:
	rm -f *.o *.output 
	rm -f SysY.tab.cpp lex.yy.cpp SysY.tab.hpp
	rm -rf *.dSYM .vscode .VSCodeCounter