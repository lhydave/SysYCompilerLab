/* Lexical analyzer of MiniC */
%option yylineno
%option noyywrap
%option noinput
%option nounput
%{
#include "node.hpp"
#include "SysY.tab.hpp"
%}

whitespace      [ \n\t\r\v\f]
comment         \/\*([^\*]|\*[^\/])*\*\/|\/\/.*
identifier      [_a-zA-Z][_0-9a-zA-Z]*
decconst        [1-9][0-9]*
octconst        0[0-7]*
hexconst        0(x|X)[0-9a-fA-F]+
SingleOp        [\+\-\*\/%!<>]|&&|\|\|
Type            int|void
terminal        [=;,\(\)\{\}\[\]]
IllegalNum      0[0-7]*[8-9][0-9]*
IllegalName     [0-9][_0-9a-zA-Z]*
IllegalSymbol   .

%%

{comment}                     { /* ignore it. */ }
{whitespace}                  { /* ignore it. */ }
"/*"                          { yyerror("comment missing */"); }

{terminal}                    { return yytext[0]; }

"const"                       { return CONST; }
"if"                          { return IF; }
"else"                        { return ELSE; }
"while"                       { return WHILE; }
"continue"                    { return CONTINUE; }
"break"                       { return BREAK; }
"return"                      { return RETURN; }

"<="                          { yylval.op = str2op(yytext); return T_LE; }
">="                          { yylval.op = str2op(yytext); return T_GE; }
"=="                          { yylval.op = str2op(yytext); return T_EQ; }
"!="                          { yylval.op = str2op(yytext); return T_NE; }

{Type}                        { yylval.dtype = str2dtype(yytext); return DTYPE; }
{identifier}                  { yylval.name = namestr(yytext); return ID; }
{decconst}                    { yylval.num = str2num(yytext, 10); return NUM; }
{octconst}                    { yylval.num = str2num(yytext, 8); return NUM; }
{hexconst}                    { yylval.num = str2num(yytext + 2, 16); return NUM; }
{SingleOp}                    { yylval.op = str2op(yytext); return yytext[0]; }
{IllegalNum}                  { yyerror("illegal octal number"); }
{IllegalName}                 { yyerror("illegal identifier name"); }
{IllegalSymbol}               { yyerror("illegal symbol"); }
%%