/* AST classes of eeyore */
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
namespace Eeyore_AST {
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
	MEM
};

// right value of an assignment
enum exp_t {
	EXP_NONE, // not a right value
	EXP_VAR,
	EXP_ARRAY,
	EXP_OP,
	EXP_NUM,
	EXP_CALL
};
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
extern string now_func;
void reg_func(const string &_eeyore_name, int param_n);
void reg_var(const string &_eeyore_name, int _size);

extern unordered_map<int, stmt_node> label_table;

void parseline();

struct var_entry {
	string eeyore_name; // name in eeyore
	int size; // space to use

	var_entry() = default;
	var_entry(const string &_eeyore_name, int size = 4);
};

struct func_entry {
	string eeyore_name; // name in eeyore
	unordered_map<string, var_entry> temps; // temporary variable
	int param_n; // number of parameters

	func_entry() = default;
	func_entry(const string &_eeyore_name, int _param_n);
};

struct stmt_node {
	exp_t exp_type = EXP_NONE; // type of the expression, for RVal only
	int label = -1; // label of the statement
};

struct assign_node : public stmt_node {
	shared_ptr<op_node> lval; // left value
	shared_ptr<op_node> rval; // right value

	assign_node(
		const shared_ptr<op_node> &_lval, const shared_ptr<op_node> &_rval);
};

struct goto_node : public stmt_node {
	shared_ptr<op_node> cond; // jump condition
	int goto_label; // where goto arrives

	goto_node(int _goto_label,
		const shared_ptr<op_node> &_cond = shared_ptr<op_node>());
};

struct ret_node : public stmt_node {
	shared_ptr<op_node> ret_val; // return value

	ret_node(const shared_ptr<op_node> &_ret_val = shared_ptr<op_node>());
};

struct op_node : public stmt_node {
	op_t op; // operator
	exp_t exp_type; // expression type
	shared_ptr<op_node> left; // left expression
	shared_ptr<op_node> right; // right expression

	op_node(exp_t _exp_type, op_t op = NONE,
		const shared_ptr<op_node> &_left = shared_ptr<op_node>(),
		const shared_ptr<op_node> &_right = shared_ptr<op_node>());
};

struct call_node : public op_node {
	vector<shared_ptr<op_node>> params; // parameters
	string func_name; // function name

	call_node(
		const string &_func_name, const vector<shared_ptr<op_node>> &_params);
};

struct var_node : public op_node {
	string eeyore_name; // name in eeyore

	var_node(const string &_eeyore_name);
};

struct num_node : public op_node {
	int val; // value of the number

	num_node(int _val);
};

} // namespace Eeyore_AST