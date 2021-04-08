/* attribute struct of the symbols */
#ifndef __ATTR_H__
#define __ATTR_H__
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using std::string;
using std::vector;
enum id_type { VAR = 0, FUNC };
enum data_type { INT = 0, VOID, CONST_INT };

// error reporting
void yyerror(const char *msg);
extern int yylineno;

// conversion
char *namestr(const char *s);
data_type str2dtype(const char *s);
int str2num(const char *s, int base);

// struct id_node begin
struct id_node {
	const char *iname; // name of the id
	id_type itype; // variable or function ?
	data_type dtype; // int or void or const int?
	data_type *ptype; // parameters type
};
// struct id_node end

typedef union{
    int num;
    id_node inode;
	const char *op;
	data_type dtype;
} yylval_t;
#define YYSTYPE yylval_t


#endif // define __ATTR_H__ end