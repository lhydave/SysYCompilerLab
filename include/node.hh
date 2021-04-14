/* parse node attribute struct of the symbols */
#ifndef __NODE_H__
#define __NODE_H__
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using std::string;
using std::vector;
enum data_t { VOID, CONST_INT, INT };
enum op_t { NONE, EQ, NE, GE, LE, G, L, ADD, SUB, MUL, DIV, MOD, AND, OR, NOT };
struct node_basic;
struct vardef_node;
struct arraydef_node;
struct funcdef_node;

struct stmt_node;
struct assign_stmt_node;
struct exp_stmt_node;
struct if_stmt_node;
struct if_else_stmt_node;
struct while_stmt_node;
struct goto_stmt_node;
struct ret_stmt_node;

struct exp_node;
struct array_exp_node;
struct arith_exp_node;
struct logic_exp_node;
struct func_call_exp_node;

using yylval_t = union {
	int num;
	const char *name;
	op_t op;
	data_t dtype;

	node_basic *node;
	vardef_node *vardef;
	arraydef_node *arraydef;
	funcdef_node *funcdef;

	stmt_node *stmt;
	assign_stmt_node *assign_stmt;
	exp_stmt_node *exp_stmt;
	if_stmt_node *if_stmt;
	if_else_stmt_node *if_else_stmt;
	while_stmt_node *while_stmt;
	goto_stmt_node *goto_stmt;
	ret_stmt_node *ret_stmt;

	exp_node *exp_basic;
	array_exp_node *array_exp;
	arith_exp_node *arith_exp;
	logic_exp_node *logic_exp;
	func_call_exp_node *func_call_exp;
};
#define YYSTYPE yylval_t

// error reporting
extern int yylineno;
void yyerror(const char *msg, int lineno = yylineno);

// conversion
char *namestr(const char *s);
op_t str2op(const char *s);
data_t str2dtype(const char *s);
int str2num(const char *s, int base);

// struct node_basic begin
struct node_basic {
	node_basic *next; // next node
	node_basic *ptr1; // 1st node
	node_basic *ptr2; // 2nd node
	node_basic *ptr3; // 3rd node
	string code;

	node_basic(node_basic *next) : next(next)
	{
	}
};
// struct node_basic end
// struct vardef_node begin
struct vardef_node : public node_basic {
	string name; // name of the variable
	bool is_const; // is it constant variable ?
	bool is_global; // is it global variable ?
	int val; // if var is const, the value of var

	vardef_node(const char *_name, bool _is_const, int _val, node_basic *_next);
};
// struct vardef_node end

// struct arraydef_node begin
struct arraydef_node : public vardef_node {
	vector<int> shape; // shape of the array
	vector<int> arr_val; // if array is const, the values of array
	bool is_pt; // true if it is pointer in the parameter
	node_basic *&first_dim = ptr1; // first expression of dimension
	node_basic *&first_val = ptr2; // first value of array

	arraydef_node(const char *_name, bool _is_const, bool _is_pt,
		node_basic *_first_dim, node_basic *_first_val, node_basic *_next);
	int size();
	void set_shape();
	void set_val();
};
// struct arraydef_node end

// struct funcdef_node begin
struct funcdef_node : public node_basic {
	string name; // name of the function
	data_t ret_type; // return type
	node_basic *&blk = ptr1; // definition body
	node_basic *&first_param = ptr2; // first parameter

	funcdef_node(const char *_name, data_t _ret_type, node_basic *_blk,
		node_basic *_first_param, node_basic *_next);
};
// struct funcdef_node end

// struct stmt_node begin
struct stmt_node : public node_basic {
	string label; // label of the statement

	stmt_node(node_basic *_next);
};
// struct stmt_node end

// struct assign_stmt_node begin
struct assign_stmt_node : public stmt_node {
	node_basic *&lval = ptr1; // left variable
	node_basic *&assign_exp = ptr2; // right assignment

	assign_stmt_node(
		node_basic *_next, node_basic *_lval, node_basic *_assign_exp);
};
// struct assign_stmt_node end

// struct exp_stmt_node begin
struct exp_stmt_node : public stmt_node {
	node_basic *&exp_stmt = ptr1; // expression statement

	exp_stmt_node(node_basic *_next, node_basic *_exp_stmt);
};
// struct exp_stmt_node end

// struct if_stmt_node begin
struct if_stmt_node : public stmt_node {
	node_basic *&cond = ptr1; // condition
	node_basic *&true_stmt = ptr2; // true statement

	if_stmt_node(node_basic *_cond, node_basic *_true_stmt, node_basic *_next);
};
// struct if_stmt_node end

// struct if_else_stmt begin
struct if_else_stmt_node : public stmt_node {
	node_basic *&cond = ptr1; // condition
	node_basic *&true_stmt = ptr2; // true statement
	node_basic *&false_stmt = ptr3; // false statement

	if_else_stmt_node(node_basic *_cond, node_basic *_true_stmt,
		node_basic *_false_stmt, node_basic *_next);
};
// struct if_else_stmt end

// struct while_stmt_node begin
struct while_stmt_node : public stmt_node {
	node_basic *&cond = ptr1; // condition
	node_basic *&true_stmt = ptr2; // true statement

	while_stmt_node(
		node_basic *_cond, node_basic *_true_stmt, node_basic *_next);
};
// struct while_stmt_node end

// struct goto_stmt_node begin
struct goto_stmt_node : public stmt_node {
	node_basic *&goto_ptr = ptr1; // where goto points
};
// struct goto_stmt_node end

// struct ret_stmt_node begin
struct ret_stmt_node : public stmt_node {
	bool is_ret_val; // whether return a value
	node_basic *&ret_val = ptr1; // return value

	ret_stmt_node(bool _is_ret_val, node_basic *_ret_val);
};
// struct ret_stmt_node end

// struct exp_node begin
struct exp_node : public node_basic {
	node_basic *&child = ptr1; // child expression - for array use
	string sysy_name; // name of the expression in the sysy
	string eeyore_name; // name of the expression in the eeyore
	op_t op; // operation of the expression
	int num; // number of the expression
	exp_node(string _sysy_name = string(), node_basic *_child = nullptr);
};
// struct exp_node end

// struct array_exp_node begin
struct array_exp_node : public exp_node {
	node_basic *&first_dim = ptr1; // first dimension expression

	array_exp_node(node_basic *_first_dim, string _sysy_name = string());
};
// struct array_exp_node end

// struct arith_exp_node begin
struct arith_exp_node : public exp_node {
	node_basic *&left = ptr1; // left one, or operatee for unary
	node_basic *&right = ptr2; // right one

	arith_exp_node(op_t _op, node_basic *_left, node_basic *_right = nullptr);
};
// struct array_exp_node end

// struct logic_exp_node begin
struct logic_exp_node : public exp_node {
	node_basic *&left = ptr1; // left one
	node_basic *&right = ptr2; // right one

	logic_exp_node(op_t _op, node_basic *_left, node_basic *_right);
};
// struct logic_exp_node end

// struct func_call_exp_node begin
struct func_call_exp_node : public exp_node {
	string func_name;
	node_basic *&first_param = ptr1; // first parameter

	func_call_exp_node(string _func_name, node_basic *_first_param = nullptr);
};
// struct func_call_exp end
#endif // define __ATTR_H__ end