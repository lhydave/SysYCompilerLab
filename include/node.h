/* parse node attribute struct of the symbols */
#ifndef __NODE_H__
#define __NODE_H__
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using std::string;
using std::vector;
enum data_t { VOID, CONST_INT, INT, POINTER };
enum op_t { EQ, NE, GE, LE, G, L, ADD, SUB, MUL, DIV, MOD, AND, OR, NOT };

typedef union{
    int num;
	const char *name;
	op_t op;
	data_t dtype;
} yylval_t;
#define YYSTYPE yylval_t


// error reporting
void yyerror(const char *msg);
extern int yylineno;

// conversion
char *namestr(const char *s);
op_t str2op(const char *s);
data_t str2dtype(const char *s);
int str2num(const char *s, int base);

// struct op_node begin
struct op_node{
};
// struct op_node end



#endif // define __ATTR_H__ end