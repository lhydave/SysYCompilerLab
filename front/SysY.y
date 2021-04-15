/* Syntax analyzer of MiniC */
%{
#include "include/node.hh"
#include "include/symtab.hh"
#ifdef DEBUG
#define dbg_printf(...) do { printf(__VA_ARGS__); fflush(stdout);} while(0);
#else
#define dbg_printf(...)
#endif
int yyparse(void);
int yylex(void);
%}
%token CONST IF ELSE WHILE CONTINUE BREAK RETURN
%token <name> ID
%token <num> NUM 
%token <op> T_GE T_LE T_EQ T_NE
%token <dtype> DTYPE
%type <node> Code CompUnit Decl ConstDecl ConstDefs VarDecl VarDefs FuncFParams
            Block BlockItems BlockItem FuncRParams
%type <vardef> ConstDef VarDef FuncFParam
%type <funcdef> FuncDef
%type <stmt> Stmt
%type <assign_stmt> AssignStmt
%type <exp_stmt> ExpStmt
%type <if_stmt> IfStmt
%type <if_else_stmt> IfElseStmt
%type <while_stmt> WhileStmt
%type <goto_stmt> GotoStmt
%type <ret_stmt> RetStmt
%type <exp_basic> ConstArray ConstInitVal ConstInitVals InitVal InitVals Exp
                  Array PrimaryExp ConstExp UnaryExp LVal MulExp AddExp RelExp EqExp
                  LOrExp LAndExp Cond FuncCall
%%

Code
    : CompUnit  { root = $$ = $1; }

CompUnit
    : Decl  { $$ = $1; }
    | FuncDef   { $$ = $1; }
    | CompUnit Decl    { $1->next = $2; $$ = $1; }
    | CompUnit FuncDef { $1->next = $2; $$ = $1; }
Decl
    : ConstDecl { $$ = $1; }
    | VarDecl { $$ = $1; }
ConstDecl : CONST DTYPE ConstDefs ';'  { if($2 != INT) yyerror("variable type must be int");
                                         $$ = $3;
                                        }
    | CONST error ConstDefs ';' { yyerror("missing type in declaration"); }
    | CONST DTYPE ConstDefs error { yyerror("expected ';'"); }
    | CONST ';' { yyerror("nothing declared"); }
    | CONST DTYPE error ';' { yyerror("invalid symbols among declarations");}
ConstDefs
    : ConstDef     { $$ = $1; }
    | ConstDef ',' ConstDefs { $1->next = $3; $$ = $1; }
ConstDef
    : ID ConstArray '=' ConstInitVal  { $$ = new vardef_node($1, true, false, $2, $4->child); }
    | ID ConstArray  { $$ = new vardef_node($1, true, false, $2, nullptr); }
ConstArray
    : '[' ConstExp ']' ConstArray { $2->next = $4; $$ = $2; }
    |   { $$ = nullptr; }
    | '[' ConstExp error ConstArray { yyerror("expected ']'"); }
    | '[' error ']' ConstArray { yyerror("array size in delaration must be constant"); }
ConstInitVal
    : ConstExp  { $$ = new exp_node(EXP_INITVAL, string(), 0, NONE, $1); }
    | '{' ConstInitVals '}' { $$ = new exp_node(EXP_INITVAL, string(), 0, NONE, $2); }
    | '{' '}'   { $$ = new exp_node(EXP_INITVAL); }
    | '{' ConstInitVals error { yyerror("expected '}'"); }
    | error { yyerror("expected constant expression"); }
ConstInitVals
    : ConstInitVal  { $$ = $1; }
    | ConstInitVal ',' ConstInitVals  { $1->next = $3; $$ = $1; }

VarDecl : DTYPE VarDefs ';'    { if($1 != INT) yyerror("variable type must be int");
                                 $$ = $2;
                               }
    | DTYPE ';' { yyerror("nothing declared"); }
    | DTYPE VarDefs error { yyerror("expected ';'"); }
    | DTYPE error ';' { yyerror("invalid symbols among declarations"); yyerrok; }
VarDefs
    : VarDef    { $$ = $1; }
    | VarDef ',' VarDefs  { $1->next = $3; $$ = $1; }
VarDef
    : ID ConstArray '=' InitVal { $$=new vardef_node($1, 0, 0, $2, $4->child); }
    | ID ConstArray { $$ = new vardef_node($1, 0, 0, $2, nullptr); dbg_printf("good!\n"); }
InitVal
    : Exp   { $$ = new exp_node(EXP_INITVAL, string(), 0, NONE, $1); }
    | '{' InitVals '}'  { $$ = new exp_node(EXP_INITVAL, string(), 0, NONE, $2); }
    | '{' '}' { $$ = new exp_node(EXP_INITVAL); }
    | '{' InitVals error { yyerror("expected '}'"); }
    | '{' error { yyerror("expected '}'"); }
InitVals
    : InitVal   { $$ = $1; }
    | InitVal ',' InitVals  { $1->next = $3; $$ = $1; }

FuncDef
    : DTYPE ID  '(' { inc_blk(); create = false; } FuncFParams ')'  Block   ;
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
    : '{' { inc_blk(); } BlockItems { dec_blk(); } '}' ;
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
    
Exp : AddExp    { $$ = $1; }
Cond : { is_cond = true; } LOrExp { is_cond = false; }
LVal : ID Array ;
Array
    : '[' Exp ']'  Array ;
    | ;
    | '[' Exp error Array { yyerror("expected ']'"); }
PrimaryExp 
    : '(' Exp ')' { $$ = $2; }
    | LVal  ;
    | NUM   { $$= new exp_node(EXP_NUM, string(), $1); }
    | '(' Exp error { yyerror("expected ')'"); }
UnaryExp
    : PrimaryExp    { $$ = $1; }
    | FuncCall      ;
    | '+' UnaryExp  { $$ = $2; }
    | '-' UnaryExp  { $$ = new arith_exp_node(NEGATE, $2); }
    | '!' UnaryExp  { $$ = new arith_exp_node(NOT, $2); }
FuncCall
    : ID '(' FuncRParams ')'   ;
    | ID '(' FuncRParams error   { yyerror("expected ')'"); }
FuncRParams
    : FuncRParams ',' Exp    ;
    | Exp    ;
    | ;
MulExp
    : UnaryExp  { $$ = $1; }
    | MulExp '*' UnaryExp   { $$ = new arith_exp_node(MUL, $1, $3); }
    | MulExp '/' UnaryExp   { $$ = new arith_exp_node(DIV, $1, $3); }
    | MulExp '%' UnaryExp   { $$ = new arith_exp_node(MOD, $1, $3); }
AddExp
    : MulExp   { $$ = $1; }
    | AddExp '+' MulExp { $$ = new arith_exp_node(ADD, $1, $3); }
    | AddExp '-' MulExp { $$ = new arith_exp_node(SUB, $1, $3); }
RelExp
    : AddExp   { $$ = $1; }
    | RelExp '<' AddExp { $$ = new arith_exp_node(L, $1, $3); }
    | RelExp '>' AddExp { $$ = new arith_exp_node(G, $1, $3); }
    | RelExp T_GE AddExp { $$ = new arith_exp_node(GE, $1, $3); }
    | RelExp T_LE AddExp { $$ = new arith_exp_node(LE, $1, $3); }
EqExp
    : RelExp   { $$ = $1; }
    | EqExp T_EQ RelExp  { $$ = new arith_exp_node(EQ, $1, $3); }
    | EqExp T_NE RelExp  { $$ = new arith_exp_node(NE, $1, $3); }
LAndExp
    : EqExp ;
    | LAndExp '&' EqExp ;
LOrExp
    : LAndExp   ;
    | LOrExp '|' LAndExp ;
ConstExp : AddExp   { if($1->exp_type != EXP_NUM)
                            yyerror("expression should be constant");
                      $$ = $1;
                    }
%%