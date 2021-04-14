#include<cstdio>
#include "SysY.tab.hh"
extern FILE* yyin;
int yyparse(void);
int yylex(void);
int main(int argc, char** argv){
    yyin = fopen(argv[1], "r");
    do{
        yyparse();
    }while(!feof(yyin));
    return 0;
}