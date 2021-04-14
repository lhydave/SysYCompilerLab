/* Syntax analyzer of MiniC */
%{
#include "include/node.hh"
#include "include/symtab.hh"
int yyparse(void);
int yylex(void);
%}
%token CONST IF ELSE WHILE CONTINUE BREAK RETURN
%token <name> ID
%token <num> NUM 
%token <op> T_GE T_LE T_EQ T_NE
%token <dtype> DTYPE
%type <node> CompUnit Decl ConstDecl ConstDefs VarDecl VarDefs FuncFParams
            Block BlockItems BlockItem FuncRParams
%type <vardef> ConstDef VarDef FuncFParam
%type <funcdef> FuncDef
%type <stmt> Stmt Cond 
%type <assign_stmt> AssignStmt
%type <exp_stmt> ExpStmt
%type <if_stmt> IfStmt
%type <if_else_stmt> IfElseStmt
%type <while_stmt> WhileStmt
%type <goto_stmt> GotoStmt
%type <ret_stmt> RetStmt
%type <exp_basic> ConstArray ConstInitVal ConstInitVals InitVal InitVals Exp
                  Array PrimaryExp
%type <array_exp> LVal
%type <arith_exp> MulExp AddExp RelExp EqExp
%type <logic_exp> LOrExp LAndExp
%type <func_call_exp> FuncCall
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
    | '{' InitVals error { yyerror("expected '}'"); }
    | '{' error { yyerror("expected '}'"); }
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
    : DTYPE ID '[' ']' ConstArray   ;
    | DTYPE ID  ;
    | DTYPE ID '[' ConstExp ']' ConstArray { yyerror("the first dimension should be empty"); }
    | DTYPE ID '[' error ConstArray  { yyerror("expected ']'"); }
    | ID  { yyerror("missing type of parameter"); }
    | ID '[' ']' ConstArray { yyerror("missing type of parameter"); }
    | DTYPE error { yyerror("missing identifier of parameter"); }

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
    : AssignStmt     ;
    | ExpStmt   ;
    | ';'       ;
    | Block     ;
    | IfStmt    ;
    | IfElseStmt ;
    | WhileStmt ;
    | GotoStmt ;
    | RetStmt ;
    | BREAK error { yyerror("expected ';'"); }
    | CONTINUE error { yyerror("expected ';'"); }
AssignStmt
    : LVal '=' Exp  ';' ;
    | LVal '=' error { yyerror("expected expression"); }
    | LVal '=' Exp error { yyerror("expected ';'"); }
ExpStmt
    :   Exp ';' ;
    | Exp error { yyerror("expected ';'"); }
IfStmt
    : IF '(' Cond ')' Stmt ;
    | IF '(' error ')' Stmt { yyerror("expected condition"); }
    | IF '(' Cond error Stmt { yyerror("expected ')'"); }
IfElseStmt
    : IF '(' Cond ')' Stmt ELSE Stmt ;
WhileStmt
    : WHILE '(' Cond ')' Stmt   ;
    | WHILE '(' error ')' Stmt { yyerror("expected condition"); }
    | WHILE '(' Cond error Stmt { yyerror("expected ')"); }
GotoStmt
    : BREAK ';' ;
    | CONTINUE  ';' ;
RetStmt
    : RETURN Exp  ';' ;
    | RETURN ';' ;
    
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
    | FuncCall      ;
    | UnaryOp UnaryExp  ;
FuncCall
    : ID '(' FuncRParams ')'   ;
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