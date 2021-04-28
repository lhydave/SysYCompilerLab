#include "SysY.tab.hh"
#include "include/node.hh"
#include "include/symtab.hh"
#include <cstdio>
#include <unistd.h>
extern FILE *yyin;
int yyparse(void);
int yylex(void);
node_basic *root; // root of the parse tree
bool has_err = false; // true if an error occurred
int main(int argc, char **argv)
{
	const char *optstring = "Se:o:";
	char c;
	bool gen_eeyore = false;
	FILE *gen_out = yyin = nullptr;
	while ((c = getopt(argc, argv, optstring)) != EOF)
	{
		switch (c)
		{
		case 'S': gen_eeyore = true; break;
		case 'e': yyin = fopen(optarg, "r"); break;
		case 'o': gen_out = fopen(optarg, "w"); break;
		default: printf("Not support yet!\n"); return 0;
		}
	}
	if (gen_out == nullptr || yyin == nullptr)
	{
		fprintf(stderr, "No file opened.\n");
		return 1;
	}
	// initialize symbol tables
	init_tables();
	do
	{
		yyparse();
	} while (!feof(yyin));
	if(!has_err) // good!
		fprintf(gen_out, root->code.c_str());
	return 0;
}