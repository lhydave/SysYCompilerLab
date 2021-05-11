/* Syntax analyzer of MiniC */
%{
#include "node.hpp"
#include "symtab.hpp"
#if (DEBUG & 1)
#define dbg_printf(...) do { printf(__VA_ARGS__); fflush(stdout);} while(0)
#else
#define dbg_printf(...)
#endif
int yyparse(void);
int yylex(void);

int yywrap()
{
    return 1;
}
using namespace sysY_AST;
%}
%token CONST IF ELSE WHILE CONTINUE BREAK RETURN
%token <name> ID
%token <num> NUM 
%token <op> T_GE T_LE T_EQ T_NE
%token <dtype> DTYPE
%type <node> Code CompUnit Block BlockItems BlockItem 
%type <vardef> ConstDefs ConstDef VarDef VarDefs FuncFParam FuncFParams Decl ConstDecl VarDecl
%type <funcdef> FuncDef
%type <stmt> Stmt M_while M_if
%type <exp_basic> ConstArray ConstInitVal ConstInitVals InitVal InitVals Exp
                  Array PrimaryExp ConstExp UnaryExp LVal MulExp AddExp RelExp EqExp
                  FuncRParams
%type <cond_exp> LAndExp LOrExp Cond
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%%

Code
    : CompUnit  { root = $$ = $1; }

CompUnit
    : Decl  { $$ = new node_basic(); $$->child = $1; $$->code = $1->code; }
    | FuncDef   { $$ = $1; }
    | CompUnit Decl    { $1->set_next($2); $$ = $1; }
    | CompUnit FuncDef { $1->set_next($2); $$ = $1; }
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
    | ConstDef ',' ConstDefs { $1->set_next($3); $$ = $1; }
ConstDef
    : ID ConstArray '=' ConstInitVal  { $$ = new vardef_node($1, true, false, false, $2, $4); }
    | ID ConstArray  { yyerror("constant expression must be initialized");
                       $$ = new vardef_node($1, true, false, false, $2, nullptr); }
ConstArray
    : '[' ConstExp ']' ConstArray { $2->set_next($4); $$ = $2; }
    |   { $$ = nullptr; }
    | '[' ConstExp error ConstArray { yyerror("expected ']'"); }
    | '[' error ']' ConstArray { yyerror("array size in delaration must be constant"); }
ConstInitVal
    : ConstExp  { $$ = $1; }
    | '{' ConstInitVals '}' { $$ = new exp_node(EXP_INITVAL, "", 0, NONE, $2); }
    | '{' '}'   { $$ = new exp_node(EXP_INITVAL); }
    | '{' ConstInitVals error { yyerror("expected '}'"); }
    | error { yyerror("expected constant expression"); }
ConstInitVals
    : ConstInitVal  { $$ = $1; }
    | ConstInitVal ',' ConstInitVals  { $1->set_next($3); $$ = $1; }

VarDecl : DTYPE VarDefs ';'    { if($1 != INT) yyerror("variable type must be int");
                                 $$ = $2;
                               }
    | DTYPE ';' { yyerror("nothing declared"); }
    | DTYPE VarDefs error { yyerror("expected ';'"); }
    | DTYPE error ';' { yyerror("invalid symbols among declarations"); }
VarDefs
    : VarDef    { $$ = $1; }
    | VarDef ',' VarDefs  { $1->set_next($3); $$ = $1; }
VarDef
    : ID ConstArray '=' InitVal { $$=new vardef_node($1, false, false, false, $2, $4); }
    | ID ConstArray { $$ = new vardef_node($1, false, false, false, $2, nullptr); }
InitVal
    : Exp   { $$ = $1; }
    | '{' InitVals '}'  { $$ = new exp_node(EXP_INITVAL, "", 0, NONE, $2); }
    | '{' '}' { $$ = new exp_node(EXP_INITVAL);}
    | '{' InitVals error { yyerror("expected '}'"); }
    | '{' error { yyerror("expected '}'"); }
InitVals
    : InitVal   { $$ = $1; }
    | InitVal ',' InitVals  { $1->set_next($3); $$ = $1; }

FuncDef
    : DTYPE ID  '('  Minc_func FuncFParams ')'  Block   { $$ = new funcdef_node($2, $1, $7); }
    | DTYPE ID '(' Minc_func FuncFParams error { yyerror("expected ')'"); }
FuncFParams
    : FuncFParam ',' FuncFParams   ;
    | FuncFParam    ;
    | ;
FuncFParam
    : DTYPE ID '[' ']' ConstArray   { if($1 != INT) yyerror("variable type must be int"); 
                                      $$ = new vardef_node($2, false, true, true, $5); }
    | DTYPE ID  { if($1 != INT) yyerror("variable type must be int"); 
                  $$ = new vardef_node($2, false, false, true); }
    | DTYPE ID '[' ConstExp ']' ConstArray { yyerror("the first dimension should be empty"); }
    | DTYPE ID '[' error ConstArray  { yyerror("expected ']'"); }
    | ID  { yyerror("missing type of parameter"); }
    | ID '[' ']' ConstArray { yyerror("missing type of parameter"); }
    | DTYPE error { yyerror("missing identifier of parameter"); }
Minc_func: { reg_func($<name>-1, $<dtype>-2); inc_blk(); create = false; }
Minc: { inc_blk(); } ;
Block
    : '{' Minc BlockItems { dec_blk(); } '}' { $$ = $3; }
    | '{' Minc BlockItems error  { dec_blk(); yyerror("expected '}'"); }
BlockItems
    :  BlockItem BlockItems { $1->set_next($2); $$ = $1; }
    |   { $$ = nullptr; }
BlockItem
    : Decl  { $$ = $1; }
    | Stmt  { $$ = $1; }
Stmt
    : LVal '=' Exp  ';' { $$ = new assign_stmt_node($1, $3); }
    |   Exp ';' { $$ = new exp_stmt_node($1); }
    |   ';' { $$ = new stmt_node(); }
    | Block { $$ = new stmt_node($1); }
    | IF '(' M_if Cond ')' Stmt %prec LOWER_THAN_ELSE { $$ = $3; $$->update($4, $6, nullptr); }
    | IF '(' M_if Cond ')' Stmt ELSE Stmt { $$ = $3; $$->update($4, $6, $8); }
    | WHILE '(' M_while Cond ')' Stmt   { $$ = $3; $$->update($4, $6, nullptr); }
    | BREAK ';' { $$ = new goto_stmt_node(true); }
    | CONTINUE  ';' { $$ = new goto_stmt_node(false); }
    | RETURN Exp  ';' { $$ = new ret_stmt_node($2); }
    | RETURN ';' { $$ = new ret_stmt_node(nullptr); }
    | LVal '=' error { yyerror("expected expression"); }
    | LVal '=' Exp error { yyerror("expected ';'"); }
    | Exp error { yyerror("expected ';'"); }
    | IF '(' M_if error ')' Stmt { yyerror("expected condition"); }
    | IF '(' M_if Cond error Stmt { yyerror("expected ')'"); }
    | WHILE '(' M_while error ')' Stmt { yyerror("expected condition"); }
    | WHILE '(' M_while Cond error Stmt { yyerror("expected ')"); }
    | BREAK error { yyerror("expected ';'"); }
    | CONTINUE error { yyerror("expected ';'"); }
M_while : { $$ = new while_stmt_node(); }
M_if : { $$ = new if_stmt_node(); }

Exp : AddExp    { $$ = $1; }
Cond : { is_cond = true; } LOrExp { is_cond = false; $$ = $2; }
LVal : ID Array { $$ = new array_exp_node($2, $1); }
Array
    : '[' Exp ']'  Array { $2->set_next($4); $$ = $2; }
    | { $$ = nullptr; }
    | '[' Exp error Array { yyerror("expected ']'"); }
PrimaryExp 
    : '(' Exp ')' { $$ = $2; }
    | LVal  { $$ = $1; }
    | NUM   { $$= new exp_node(EXP_NUM, "", $1); }
    | '(' Exp error { yyerror("expected ')'"); }
UnaryExp
    : PrimaryExp    { $$ = $1; }
    | ID '(' FuncRParams ')'   { $$ = new func_call_exp_node($1, $3); }
    | '+' UnaryExp  { $$ = $2; }
    | '-' UnaryExp  { $$ = new arith_exp_node(NEGATE, $2); }
    | '!' UnaryExp  { $$ = new arith_exp_node(NOT, $2); }
    | ID '(' FuncRParams error   { yyerror("expected ')'"); }
FuncRParams
    : Exp  ',' FuncRParams  { $1->set_next($3); $$ = $1; }
    | Exp    { $$ = $1; }
    | { $$ = nullptr; }
MulExp
    : UnaryExp  { $$ = $1; }
    | MulExp '*' UnaryExp{ $$ = new arith_exp_node(MUL, $1, $3); }
    | MulExp '/' UnaryExp  { $$ = new arith_exp_node(DIV, $1, $3); }
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
    : EqExp     { if( $1->exp_type == EXP_PTR) 
                      yyerror("pointer should not be in expression");
                  $$ = new cond_exp_node(NONE, $1, nullptr); }
    | LAndExp '&' EqExp { $$ = new cond_exp_node(AND, $1, 
                                new cond_exp_node(NONE, $3, nullptr)); }
LOrExp
    : LAndExp   { if( $1->exp_type == EXP_PTR) 
                      yyerror("pointer should not be in expression");
                  $$ = $1; }
    | LOrExp '|' LAndExp { $$ = new cond_exp_node(OR, $1, $3); }
ConstExp : AddExp   { $1->reduce();
                      if($1->exp_type != EXP_NUM)
                          yyerror("expression should be constant");
                      $$ = $1; }
%%