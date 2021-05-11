#include "SysY.tab.hpp"
#include "eeyore_AST.hpp"
#include "node.hpp"
#include "symtab.hpp"
#include "tigger_gen.hpp"
#include <cstdio>
#include <sstream>
#include <unistd.h>
extern FILE *yyin;
int yyparse(void);
int yylex(void);
namespace sysY_AST {
node_basic *root; // root of the parse tree
}
bool has_err = false; // true if an error occurred
std::ostringstream tigger_dst;
int main(int argc, char **argv)
{
	const char *optstring = "Se:o:d";
	char c;
	bool gen_eeyore = false;
	FILE *gen_out = yyin = nullptr;
	string out_name;
	bool debug = false;
	while ((c = getopt(argc, argv, optstring)) != EOF)
	{
		switch (c)
		{
		case 'S': gen_eeyore = true; break;
		case 'e': yyin = fopen(optarg, "r"); break;
		case 'o': out_name = optarg; break;
		case 'd': debug = true; break;
		default: printf("Not support yet!\n"); return 0;
		}
	}
	if (!gen_eeyore)
	{
		printf("Not support yet!\n");
		return 0;
	}
	if (out_name.empty() || yyin == nullptr)
	{
		fprintf(stderr, "No file opened.\n");
		return 1;
	}
	// initialize symbol tables
	sysY_AST::init_tables();
	do
	{
		yyparse();
	} while (!feof(yyin));
	sysY_AST::check_main();
	if (!has_err) // good!
	{
		eeyore_AST::build_AST(sysY_AST::root->code);
		no_alloc::gen_code();
		fflush(stdout);
		gen_out = fopen(out_name.c_str(), "w");
		if (debug)
		{
			auto debug_out = fopen("eeyore.out", "w");
			fprintf(debug_out, "%s", sysY_AST::root->code.c_str());
		}
		fprintf(gen_out, "%s", tigger_dst.str().c_str());
	}
	return 0;
}