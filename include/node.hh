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
enum data_t { VOID, INT };
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
	NOT,
	LOAD
};
enum exp_t {
	EXP_TEMP,
	EXP_VAR,
	EXP_NUM,
	EXP_FUNC_CALL,
	EXP_ARRAY,
	EXP_INITVAL,
	EXP_PTR
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
	exp_node *exp_basic;
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
	node_basic *child = nullptr; // child node
	string code; // code to eeyore
	int lineno = yylineno; // line number of the node

	virtual void traverse(node_basic *parent);
	virtual void set_next(node_basic *_next);
};
// struct node_basic end

// struct vardef_node begin
struct vardef_node : public node_basic {
	vardef_node *next = nullptr;
	string name; // name of the variable
	bool is_const; // is it constant variable ?
	bool is_global; // is it global variable ?
	bool is_array; // is it an array ?
	vector<int> dim; // dim of the array
	vector<exp_node *> val; // the values of array
	bool is_pt; // true if it is pointer in the parameter
	bool is_param; // true if it is parameter
	int size; // size of the array in int

	vardef_node(const char *_name, bool _is_const = false, bool _is_pt = false,
		bool _is_param = false, exp_node *_first_dim = nullptr,
		exp_node *_first_val = nullptr);
	void set_shape(exp_node *first_dim);
	static vector<exp_node *> set_val(vector<int> &dim, exp_node *first_val);
};
// struct vardef_node end

// struct funcdef_node begin
struct funcdef_node : public node_basic {
	string name; // name of the function
	data_t ret_type; // return type
	node_basic *blk; // definition body
	vector<vardef_node *> params; // parameters

	funcdef_node(const char *_name, data_t _ret_type, node_basic *_blk,
		vardef_node *first_param);
	void add_ret();
};
// struct funcdef_node end

// struct stmt_node begin
struct stmt_node : public node_basic {
};
// struct stmt_node end

// struct assign_stmt_node begin
struct assign_stmt_node : public stmt_node {
	exp_node *lval; // left variable
	exp_node *assign_exp; // right assignment

	assign_stmt_node(exp_node *_lval, exp_node *_assign_exp);
};
// struct assign_stmt_node end

// struct exp_stmt_node begin
struct exp_stmt_node : public stmt_node {
	exp_node *exp_stmt; // expression statement

	exp_stmt_node(exp_node *_exp_stmt);
};
// struct exp_stmt_node end

// struct if_stmt_node begin
struct if_stmt_node : public stmt_node {
	exp_node *cond; // condition
	node_basic *true_stmt; // true statement

	if_stmt_node(exp_node *_cond, node_basic *_true_stmt);
};
// struct if_stmt_node end

// struct if_else_stmt begin
struct if_else_stmt_node : public stmt_node {
	exp_node *cond; // condition
	node_basic *true_stmt; // true statement
	node_basic *false_stmt; // false statement

	if_else_stmt_node(exp_node *_cond, node_basic *_true_stmt,
		node_basic *_false_stmt);
};
// struct if_else_stmt end

// struct while_stmt_node begin
struct while_stmt_node : public stmt_node {
	exp_node *cond; // condition
	node_basic *true_stmt; // true statement

	while_stmt_node(
		exp_node *_cond, node_basic *_true_stmt, node_basic *_next);
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
	exp_node *ret_val; // return value

	ret_stmt_node(bool _is_ret_val, exp_node *_ret_val);
};
// struct ret_stmt_node end

// struct exp_node begin
struct exp_node : public node_basic {
	exp_node *next = nullptr; // next expression - for array use
	exp_node *child; // child expression - for array use
	string sysy_name; // name of the expression in the sysy
	op_t op = NONE; // operation of the expression
	int num = 0; // number of the expression
	exp_t exp_type;

	exp_node(exp_t _exp_type, string _sysy_name = string(), int _num = 0,
		op_t _op = NONE, exp_node *_child = nullptr);
	virtual void new_temp();
	virtual void reduce(bool alloc);
	void set_next(node_basic *_next);
};
// struct exp_node end

// struct array_exp_node begin
struct array_exp_node : public exp_node {
	vector<exp_node *> sysy_idx; // index of the expression in sysy
	exp_node *eeyore_exp; // expression in eeyore
	string sysy_array_name; // name of the array in sysy

	array_exp_node(exp_node *first_dim, string _sysy_name = string());
	static exp_node *idx_open(const vector<exp_node *> &idx, size_t len);
	void reduce(bool alloc);
};
// struct array_exp_node end

// struct arith_exp_node begin
struct arith_exp_node : public exp_node {
	exp_node *left; // left one, or operatee for unary
	exp_node *right; // right one

	arith_exp_node(op_t _op, exp_node *_left, exp_node *_right = nullptr);
	void reduce(bool alloc);
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
	vector<exp_node *> params; // real parameters
	string sysy_func_name; // name of the function in sysy

	func_call_exp_node(const string& func_name, exp_node *first_param = nullptr);
	void check_valid();
	static void check_ret_type(exp_node *func_exp);
	void reduce(bool alloc);
	void new_temp();
};
// struct func_call_exp end
#endif // define __ATTR_H__ end