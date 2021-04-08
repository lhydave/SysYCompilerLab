/* Syntax analyzer of MiniC */
%{
#include "include/attr.h"
%}
%token ASSIGN, SEMI, COMMA, LPAREN, RPAREN, LBRACE, RBRACE, LBRACK, RBRACK,
    CONST, IF, ELSE, WHILE, CONTINUE, BREAK, ID, CONSTNUM, ARITHOP, LOGICALOP, TYPE
%%
%%