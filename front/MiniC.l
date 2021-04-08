/* Lexical analyzer of MiniC */
%option yylineno
%option noyywrap
%option noinput
%option nounput
%{
#include "include/attr.h"
#ifndef LEXONLY
#define YYSTYPE
#include "MiniC.tab.h"
#endif
#ifdef LEXONLY
enum {
    ASSIGN=100, SEMI, COMMA, LPAREN, RPAREN, LBRACE, RBRACE, LBRACK, RBRACK,
    CONST, IF, ELSE, WHILE, CONTINUE, BREAK, ID, CONSTNUM, ARITHOP, LOGICALOP, TYPE
};
union{
    int num;
    id_node inode;
	const char *op;
	data_type dtype;
} yylval;
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

{whitespace}    { /* ignore it. */ }
{comment}       { /* ignore it. */ }
"/*"            { yyerror("comment missing */"); }

"="             { return ASSIGN; }
";"             { return SEMI; }
","             { return COMMA; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
"["             { return LBRACK; }
"]"             { return RBRACK; }

"const"         { return CONST; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"continue"      { return CONTINUE; }
"break"         { return BREAK; }

{Type}          { yylval.dtype = str2dtype(yytext); return TYPE; }
{identifier}    { yylval.inode.iname = yytext; return ID; }
{decconst}      { yylval.num = str2num(yytext, 10); return CONSTNUM; }
{octconst}      { yylval.num = str2num(yytext, 8); return CONSTNUM; }
{hexconst}      { yylval.num = str2num(yytext, 16); return CONSTNUM; }
{ArithOp}       { yylval.op = yytext; return ARITHOP; }
{LogicalOp}     { yylval.op = yytext; return LOGICALOP; }
{IllegalNum}    { yyerror("illegal octal number"); }
{IllegalName}   { yyerror("illegal identifier name"); }
{IllegalSymbol} { yyerror("illegal symbol"); }
%%

#ifdef LEXONLY
// test need to eliminate the return sentences!
int main(int argc, char** argv){
    yyin = fopen(argv[1], "r");
    yylex();
    return 0;
}
#endif