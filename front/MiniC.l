/* Lexical analyzer of MiniC */
%option yylineno
%option noyywrap
%{
#include "include/attr.h"
#ifndef LEXONLY
#include "MiniC.tab.hh"
#endif
#ifdef LEXONLY
enum {
    ASSIGN=100, SEMI, COMMA, LPAREN, RPAREN, LBRACE, RBRACE, LBRACK, RBRACK,
    CONST, IF, ELSE, WHILE, CONTINUE, BREAK, ID, CONSTNUM, ARITHOP, LOGICALOP, TYPE
};
yylval_t yylval;
#endif
%}

whitespace      [ \n\t\r\v\f]
comment         \/\*([^\*]|\*[^\/])*\*\/|\/\/.*
identifier      [_a-zA-Z][_0-9a-zA-Z]*
decconst        [1-9][0-9]*
octconst        0[0-7]*
hexconst        0(x|X)[0-9a-fA-F]+
ArithOp         [\+\-\*\/\%]
LogicalOp       &&|\|\||<|>|<=|>=|==|!=|!
Type            int|void
IllegalNum      0[0-7]*[8-9][0-9]*
IllegalName     [0-9][_0-9a-zA-Z]*
IllegalSymbol   .

%%

{comment}                     { /* ignore it. */ }
{whitespace}                  { /* ignore it. */ }
"/*"                          { yyerror("comment missing */"); }

"="                           { return ASSIGN; }
";"                           { return SEMI; }
","                           { return COMMA; }
"("                           { return LPAREN; }
")"                           { return RPAREN; }
"{"                           { return LBRACE; }
"}"                           { return RBRACE; }
"["                           { return LBRACK; }
"]"                           { return RBRACK; }

"const"                       { return CONST; }
"if"                          { return IF; }
"else"                        { return ELSE; }
"while"                       { return WHILE; }
"continue"                    { return CONTINUE; }
"break"                       { return BREAK; }

{Type}                        { yylval.dtype = str2dtype(yytext); return TYPE; }
{identifier}                  { yylval.inode.iname = namestr(yytext); return ID; }
{decconst}                    { yylval.num = str2num(yytext, 10); return CONSTNUM; }
{octconst}                    { yylval.num = str2num(yytext, 8); return CONSTNUM; }
{hexconst}                    { yylval.num = str2num(yytext, 16); return CONSTNUM; }
{ArithOp}                     { yylval.op = namestr(yytext); return ARITHOP; }
{LogicalOp}                   { yylval.op = namestr(yytext); return LOGICALOP; }
{IllegalNum}                  { yyerror("illegal octal number"); }
{IllegalName}                 { yyerror("illegal identifier name"); }
{IllegalSymbol}               { yyerror("illegal symbol"); }
%%

#ifdef LEXONLY
int main(int argc, char** argv){
    printf("lexer starts\n");
    yyin = fopen(argv[1], "r");
    printf("file %s opened\n", argv[1]);    
    do{
        yylex();
        printf("good parse!\n");
      }while(!feof(yyin));
    return 0;
}
#endif