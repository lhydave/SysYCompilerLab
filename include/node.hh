/* parse node attribute struct of the symbols */
#ifndef __NODE_H__
#define __NODE_H__
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;
enum data_t { VOID, CONST_INT, INT };
enum op_t {
	NONE,
	EQ,
	NE,
	GE,
	LE,
	G,
	L,
	ADD,
	SUB,
	NEGATE,
	MUL,
	DIV,
	MOD,
	AND,
	OR,
	NOT
};
enum exp_t {
	EXP_TEMP,
	EXP_VAR,
	EXP_NUM,
	EXP_FUNC_CALL,
	EXP_ARRAY,
	EXP_INITVAL
};
struct node_basic;
struct vardef_node;
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

// global variables
extern node_basic *root;
extern int blk_id;
extern bool is_cond;
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
	node_basic *next = nullptr; // next node
	string code; // code to eeyore
	int lineno = yylineno; // line number of the node

	virtual string traverse(node_basic *parent);
};
// struct node_basic end

// struct vardef_node begin
struct vardef_node : public node_basic {
	string name; // name of the variable
	bool is_const; // is it constant variable ?
	bool is_global; // is it global variable ?
	bool is_array; // is it an array ?
	vector<int> dim; // dim of the array
	vector<exp_node *> val; // the values of array
	bool is_pt; // true if it is pointer in the parameter
	exp_node *first_dim; // first expression of dimension
	exp_node *first_val; // first value of array
	int size; // size of the array in int

	vardef_node(const char *_name, bool _is_const, bool _is_pt,
		exp_node *_first_dim = nullptr, exp_node *_first_val = nullptr);
	void set_shape();
	static vector<exp_node *> set_val(vector<int> &dim, exp_node *_first_val);
};
// struct vardef_node end

// struct funcdef_node begin
struct funcdef_node : public node_basic {
	string name; // name of the function
	data_t ret_type; // return type
	node_basic *blk; // definition body
	node_basic *first_param; // first parameter

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
	node_basic *lval; // left variable
	node_basic *assign_exp; // right assignment

	assign_stmt_node(
		node_basic *_next, node_basic *_lval, node_basic *_assign_exp);
};
// struct assign_stmt_node end

// struct exp_stmt_node begin
struct exp_stmt_node : public stmt_node {
	node_basic *exp_stmt; // expression statement

	exp_stmt_node(node_basic *_next, node_basic *_exp_stmt);
};
// struct exp_stmt_node end

// struct if_stmt_node begin
struct if_stmt_node : public stmt_node {
	node_basic *cond; // condition
	node_basic *true_stmt; // true statement

	if_stmt_node(node_basic *_cond, node_basic *_true_stmt, node_basic *_next);
};
// struct if_stmt_node end

// struct if_else_stmt begin
struct if_else_stmt_node : public stmt_node {
	node_basic *cond; // condition
	node_basic *true_stmt; // true statement
	node_basic *false_stmt; // false statement

	if_else_stmt_node(node_basic *_cond, node_basic *_true_stmt,
		node_basic *_false_stmt, node_basic *_next);
};
// struct if_else_stmt end

// struct while_stmt_node begin
struct while_stmt_node : public stmt_node {
	node_basic *cond; // condition
	node_basic *true_stmt; // true statement

	while_stmt_node(
		node_basic *_cond, node_basic *_true_stmt, node_basic *_next);
};
// struct while_stmt_node end

// struct goto_stmt_node begin
struct goto_stmt_node : public stmt_node {
	bool is_break; // true if break, false if continue
};
// struct goto_stmt_node end

// struct ret_stmt_node begin
struct ret_stmt_node : public stmt_node {
	bool is_ret_val; // whether return a value
	node_basic *ret_val; // return value

	ret_stmt_node(bool _is_ret_val, node_basic *_ret_val);
};
// struct ret_stmt_node end

// struct exp_node begin
struct exp_node : public node_basic {
	exp_node *next = nullptr; // next expression - for array use
	exp_node *child; // child expression - for array use
	string sysy_name; // name of the expression in the sysy
	op_t op = NONE; // operation of the expression
	int num = -1; // number of the expression
	exp_t exp_type;

	exp_node(exp_t _exp_type, string _sysy_name = string(), int _num = 0,
		op_t _op = NONE, exp_node *_child = nullptr);
	static string new_temp();
	virtual void reduce();
};
// struct exp_node end

// struct array_exp_node begin
struct array_exp_node : public exp_node {
	exp_node *first_dim; // first dimension expression

	array_exp_node(node_basic *_first_dim, string _sysy_name = string());
};
// struct array_exp_node end

// struct arith_exp_node begin
struct arith_exp_node : public exp_node {
	exp_node *left; // left one, or operatee for unary
	exp_node *right; // right one

	arith_exp_node(op_t _op, exp_node *_left, exp_node *_right = nullptr);
	void reduce();
};
// struct array_exp_node end

// struct logic_exp_node begin
struct logic_exp_node : public exp_node {
	node_basic *left; // left one
	node_basic *right; // right one

	logic_exp_node(op_t _op, node_basic *_left, node_basic *_right);
};
// struct logic_exp_node end

// struct func_call_exp_node begin
struct func_call_exp_node : public exp_node {
	string func_name;
	node_basic *first_param; // first parameter

	func_call_exp_node(string _func_name, node_basic *_first_param = nullptr);
};
// struct func_call_exp end
#endif // define __ATTR_H__ end