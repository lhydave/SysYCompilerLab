/* Syntax analyzer of MiniC */
%{
#include "include/node.hh"
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
    | CONST error ConstDefs ';' { yyerror("missing type in declaration"); }
    | CONST DTYPE ConstDefs error { yyerror("expected ';'"); }
    | CONST ';' { yyerror("nothing declared"); }
    | CONST DTYPE error ';' { yyerror("invalid symbols among declarations");}
ConstDefs
    : ConstDef     ;
    | ConstDef ',' ConstDefs ;
ConstDef
    : ID ConstArray '=' ConstInitVal  ;
    | ID ConstArray error { yyerror("const variable must be initialized"); }
ConstArray
    : '[' ConstExp ']' ConstArray ;
    | ;
    | '[' ConstExp error ConstArray { yyerror("expected ']'"); }
    | '[' error ']' ConstArray { yyerror("array size in delaration must be constant"); }
ConstInitVal
    : ConstExp  ;
    | '{' ConstInitVals '}' ;
    | '{' '}'   ;
    | '{' ConstInitVals error { yyerror("expected '}'"); }
    | error { yyerror("expected constant expression"); }
ConstInitVals
    : ConstInitVal  ;
    | ConstInitVal ',' ConstInitVals  ;

VarDecl : DTYPE VarDefs ';'    ;
    | DTYPE ';' { yyerror("nothing declared"); }
    | DTYPE VarDefs error { yyerror("expected ';'"); }
    | DTYPE error ';' { yyerror("invalid symbols among declarations"); yyerrok; }
VarDefs
    : VarDef    ;
    | VarDef ',' VarDefs  ;
VarDef
    : ID ConstArray '=' InitVal ;
    | ID ConstArray ;
InitVal
    : Exp   ;
    | '{' InitVals '}'  ;
    | '{' '}' ;
    | '{' InitVals error { yyerror("expected '}"); }
    | error { yyerror("expected expression"); }
InitVals
    : InitVal   ;
    | InitVal ',' InitVals  ;

FuncDef
    : DTYPE ID  '(' FuncFParams ')' Block   ;
    | DTYPE ID '(' FuncFParams error { yyerror("expected ')'"); }
FuncFParams
    : FuncFParam ',' FuncFParams    ;
    | FuncFParam    ;
    | ;
FuncFParam
    : DTYPE ID ParaArray   ;
    | ID ParaArray { yyerror("missing type of parameter"); }
    | DTYPE error { yyerror("missing identifier of parameter"); }
ParaArray
    : '[' ']' ConstArray ;
    | ;
    | '[' ConstExp ']' ConstArray { yyerror("the first dimension should be empty"); }
    | '[' error ConstArray  { yyerror("expected ']'"); }

Block
    : '{' BlockItems '}' ;
    | '{' BlockItems error  { yyerror("expected '}'"); }
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
    | IF '(' Cond ')' Stmt ELSE Stmt ;
    | WHILE '(' Cond ')' Stmt   ;
    | BREAK ';' ;
    | CONTINUE  ';' ;
    | RETURN Exp  ';' ;
    | RETURN ';' ;
    | LVal '=' error { yyerror("expected expression"); }
    | LVal '=' Exp error { yyerror("expected ';'"); }
    | Exp error { yyerror("expected ';'"); }
    | IF '(' error ')' Stmt { yyerror("expected condition"); }
    | IF '(' Cond error Stmt { yyerror("expected ')'"); }
    | WHILE '(' error ')' Stmt { yyerror("expected condition"); }
    | WHILE '(' Cond error Stmt { yyerror("expected ')"); }
    | BREAK error { yyerror("expected ';'"); }
    | CONTINUE error { yyerror("expected ';'"); }

Exp : AddExp    ;
Cond : LOrExp   ;
LVal : ID Array ;
Array
    : '[' Exp ']'  Array ;
    | ;
    | '[' Exp error Array { yyerror("expected ']'"); }
PrimaryExp 
    : '(' Exp ')' ;
    | LVal  ;
    | NUM   ;
    | '(' Exp error { yyerror("expected ')'"); }
UnaryExp
    : PrimaryExp    ;
    | ID '(' FuncRParams ')'   ;
    | UnaryOp UnaryExp  ;
    | ID '(' FuncRParams error   { yyerror("expected ')'"); }
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