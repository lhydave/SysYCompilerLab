/* Syntax analyzer of MiniC */
%{
#include "include/node.h"
#define YYSTYPE yylval_t
int yyparse(void);
int yylex(void);
%}
%token CONST IF ELSE WHILE CONTINUE BREAK RETURN
%token <name> ID
%token <num> NUM 
%token <op> T_GE T_LE T_EQ T_NE
%token <dtype> DTYPE
%%

CompUnit
    : Decl  ;
    | FuncDef   ;
    | CompUnit Decl    ;
    | CompUnit FuncDef ;
Decl
    : ConstDecl ;
    | VarDecl ;
ConstDecl : CONST DTYPE ConstDefs ';'  ;
ConstDefs
    : ConstDef     ;
    | ConstDef ',' ConstDefs ;
ConstDef
    : ID ConstArray '=' ConstInitVal  ;
ConstArray
    : '[' ConstExp ']' ConstArray ;
    | ;
ConstInitVal
    : ConstExp  ;
    | '{' ConstInitVals '}' ;
    | '{' '}'   ;
ConstInitVals
    : ConstInitVal  ;
    | ConstInitVal ',' ConstInitVals  ;
VarDecl : DTYPE VarDefs ';'    ;
VarDefs
    : VarDef    ;
    | VarDef ',' VarDefs  ;
VarDef
    : ID ConstArray '=' InitVal ;
InitVal
    : Exp   ;
    | '{' InitVals '}'  ;
    | '{' '}' ;
InitVals
    : InitVal   ;
    | InitVal ',' InitVals  ;

FuncDef
    : DTYPE ID  '(' FuncFParams ')' Block   ;
FuncFParams
    : FuncFParam ',' FuncFParams    ;
    | FuncFParam    ;
    | ;
FuncFParam
    : DTYPE ID ConstArray   ;

Block
    : '{' BlockItems '}' ;
BlockItems
    :  BlockItem BlockItems ;
    |   ;
BlockItem
    : Decl  ;
    | Stmt  ;
Stmt
    : LVal '=' Exp  ';' ;
    |   Exp ';' ;
    |   ';' ;
    | Block ;
    | IF '(' Cond ')' Stmt ;
    | IF '(' Cond ')' Stmt  ELSE Stmt ;
    | WHILE '(' Cond ')' Stmt   ;
    | BREAK ';' ;
    | CONTINUE  ';' ;
    | RETURN Exp  ';' ;
    | RETURN ';' ;
//ElseStmt
//   : ELSE Stmt ;
//    |   ;

Exp : AddExp    ;
Cond : LOrExp   ;
LVal : ID Array ;
Array
    : '[' Exp ']'  Array ;
    | ;
PrimaryExp 
    : '(' Exp ')' ;
    | LVal  ;
    | NUM   ;
UnaryExp
    : PrimaryExp    ;
    | ID '(' FuncRParams ')'   ;
    | UnaryOp UnaryExp  ;
UnaryOp
    : '+'   ;
    | '-' ;
    | '!' ;
FuncRParams
    : FuncRParams ',' Exp    ;
    | Exp    ;
    | ;
MulExp
    : UnaryExp  ;
    | MulExp '*' UnaryExp   ;
    | MulExp '/' UnaryExp   ;
    | MulExp '%' UnaryExp   ;
AddExp
    : MulExp   ;
    | AddExp '+' MulExp ;
    | AddExp '-' MulExp ;
RelExp
    : AddExp   ;
    | RelExp '<' AddExp ;
    | RelExp '>' AddExp ;
    | RelExp T_GE AddExp ;
    | RelExp T_LE AddExp ;
EqExp
    : RelExp   ;
    | EqExp T_EQ RelExp  ;
    | EqExp T_NE RelExp  ;
LAndExp
    : EqExp ;
    | LAndExp '&' EqExp ;
LOrExp
    : LAndExp   ;
    | LOrExp '|' LAndExp ;
ConstExp : AddExp   ; 
%%

extern FILE* yyin;
int main(int argc, char** argv){
    yyin = fopen(argv[1], "r");
    do{
        yyparse();
    }while(!feof(yyin));
    return 0;
}