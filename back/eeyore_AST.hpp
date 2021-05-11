/* AST classes of eeyore */
#ifndef __EEYORE_AST_H__
#define __EEYORE_AST_H__
#include <cstring>
#include <memory>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;
namespace eeyore_AST {

// right value of an assignment
enum exp_t {
	EXP_NONE, // not a right value
	EXP_VAR,
	EXP_ARRAY,
	EXP_OP,
	EXP_NUM,
	EXP_CALL
};

// statement type
enum stmt_t { STMT_ASSIGN, STMT_GOTO, STMT_RET, STMT_CALL };

struct var_entry;
struct func_entry;
struct stmt_node;
struct assign_node;
struct goto_node;
struct ret_node;
struct op_node;
struct call_node;
struct var_node;
struct num_node;

extern unordered_map<string, func_entry> func_table;

bool is_global(const string &name);
bool is_param(const string &name);
void build_AST(const string &eeyore_code);

struct var_entry {
	string eeyore_name; // name in eeyore
	int size; // space to use
	bool is_array; // true if it is array

	var_entry() = default;
	var_entry(
		const string &_eeyore_name, int _size = 4, bool _is_array = false) :
		eeyore_name(_eeyore_name),
		size(_size), is_array(_is_array)
	{
	}
};

struct func_entry {
	string eeyore_name; // name in eeyore
	unordered_map<string, var_entry> temps; // temporary variable
	vector<shared_ptr<stmt_node>> funcbody; // body of the function
	int param_n; // number of parameters

	func_entry() = default;
	func_entry(const string &_eeyore_name, int _param_n) :
		eeyore_name(_eeyore_name), param_n(_param_n)
	{
	}
};

struct stmt_node {
	stmt_t stmt_type;
	int label = -1;
};

struct assign_node : public stmt_node {
	shared_ptr<op_node> lval; // left value
	shared_ptr<op_node> rval; // right value

	assign_node(
		const shared_ptr<op_node> &_lval, const shared_ptr<op_node> &_rval) :
		lval(_lval),
		rval(_rval)
	{
	}
};

struct goto_node : public stmt_node {
	int goto_label; // where goto arrives
	shared_ptr<op_node> cond; // jump condition

	goto_node(int _goto_label,
		const shared_ptr<op_node> &_cond = shared_ptr<op_node>()) :
		goto_label(_goto_label),
		cond(_cond)

	{
	}
};

struct ret_node : public stmt_node {
	shared_ptr<op_node> ret_val; // return value

	ret_node(const shared_ptr<op_node> &_ret_val = shared_ptr<op_node>()) :
		ret_val(_ret_val)
	{
	}
};

struct op_node : public stmt_node {
	exp_t exp_type; // expression type
	string op; // operator
	shared_ptr<op_node> left; // left expression
	shared_ptr<op_node> right; // right expression

	op_node(exp_t _exp_type, const string &_op = "",
		const shared_ptr<op_node> &_left = shared_ptr<op_node>(),
		const shared_ptr<op_node> &_right = shared_ptr<op_node>()) :
		exp_type(_exp_type),
		op(_op), left(_left), right(_right)
	{
	}
};

struct call_node : public op_node {
	vector<shared_ptr<op_node>> params; // parameters
	string func_name; // function name

	call_node(const string &_func_name) :
		op_node(EXP_CALL), func_name(_func_name)
	{
	}
};

struct var_node : public op_node {
	string eeyore_name; // name in eeyore

	var_node(const string &_eeyore_name) :
		op_node(EXP_VAR), eeyore_name(_eeyore_name)
	{
	}
};

struct num_node : public op_node {
	int val; // value of the number

	num_node(int _val) : op_node(EXP_NUM), val(_val)
	{
	}
};
} // namespace eeyore_AST
#endif // __EEYORE_AST_H__