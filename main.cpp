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
	bool gen_asm = false;
	yyin = nullptr;
	string eeyoreout_s, tiggerout_s;
	while ((c = getopt(argc, argv, optstring)) != EOF)
	{
		switch (c)
		{
		case 'S': gen_asm = true; break;
		case 'e': yyin = fopen(optarg, "r"); break;
		case 'o': eeyoreout_s = optarg; break;
		case 't': tiggerout_s = optarg; break;
		default: printf("Not support yet!\n"); return 0;
		}
	}
	if (!gen_asm)
	{
		printf("Not support yet!\n");
		return 0;
	}
	if ((eeyoreout_s.empty() && tiggerout_s.empty()) || yyin == nullptr)
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
		if (!eeyoreout_s.empty())
		{
			auto out = fopen(eeyoreout_s.c_str(), "w");
			fprintf(out, "%s", sysY_AST::root->code.c_str());
		}
		eeyore_AST::build_AST(sysY_AST::root->code);
		no_alloc::gen_code();
		if (!tiggerout_s.empty())
		{
			auto out = fopen(tiggerout_s.c_str(), "w");
			fprintf(out, "%s", tigger_dst.str().c_str());
		}
	}
	return 0;
}