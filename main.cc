#include "include/node.hh"
#include "include/symtab.hh"
#include "SysY.tab.hh"
#include <cstdio>
extern FILE *yyin;
int yyparse(void);
int yylex(void);
node_basic *root; // root of the parse tree
int main(int argc, char **argv)
{
	yyin = fopen(argv[1], "r");
	// initialize symbol tables
	init_tables();
	do
	{
		yyparse();
	} while (!feof(yyin));
	return 0;
}