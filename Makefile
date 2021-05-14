LEX 		:= flex
YACC 		:= bison
CC 			:= g++
CDEBUG		:= -D DEBUG=2 -g
CFLAGS 		:= -lm -O0 -Wall -std=c++11 $(CDEBUG)
INCLUDEDIR 	:= -Ifront -Iback
INCLUDE 	:= front/node.cpp front/symtab.cpp main.cpp 
INCLUDE		+= back/eeyore_AST.cpp back/emit_tigger.cpp back/no_alloc.cpp 
INCLUDE		+= back/riscv/emit_riscv.cpp
YACCDEBUG	:= #-v --report=all

all: compiler

compiler: eeyore-tab $(INCUDE)
	$(CC) $(CFLAGS) $(INCLUDEDIR) SysY.tab.cpp lex.yy.cpp $(INCLUDE) -o compiler

eeyore-tab: eeyore-lex $(INCLUDE)
	$(YACC) $(YACCDEBUG) -d -o SysY.tab.cpp front/SysY.y

eeyore-lex: front/SysY.l
	$(LEX) -o lex.yy.cpp front/SysY.l

clean:
	rm -f compiler *.output *.out
	rm -f SysY.tab.cpp lex.yy.cpp SysY.tab.hpp
	rm -rf *.dSYM .vscode .VSCodeCounter