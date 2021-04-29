/* utility functions for nodes */

#include "node.hpp"
#include "symtab.hpp"
#include <numeric>
#ifdef DEBUG
#define dbg_printf(...)      \
	do                       \
	{                        \
		printf(__VA_ARGS__); \
		fflush(stdout);      \
	} while (0);
#else
#undef assert
#define assert(...)
#define dbg_printf(...)
#endif

static int err_cnt = 0;
static const int err_max = 40;
// rewrite yyerror
void yyerror(const char *msg, int lineno)
{
	has_err = true;
	++err_cnt;
	if (err_cnt > err_max)
	{
		fprintf(stderr, "Too many errors, aborted.\n");
		exit(1);
	}
	fprintf(stderr, "error happens near line %d: %s\n", lineno, msg);
}

// allocate a new space for string
char *namestr(const char *s)
{
	char *ret = new char[strlen(s) + 1];
	strcpy(ret, s);
	return ret;
}

// covert string to op_t
op_t str2op(const char *s)
{
	switch (s[0])
	{
	case '+': return ADD;
	case '-': return SUB;
	case '*': return MUL;
	case '/': return DIV;
	case '%': return MOD;
	case '&': return AND;
	case '|': return OR;
	case '<':
		if (s[1] == '=')
			return LE;
		return L;
	case '>':
		if (s[1] == '=')
			return GE;
		return G;
	case '!':
		if (s[1] == '=')
			return NE;
		return NOT;
	default: return NONE;
	}
}

// convert string to data_type
data_t str2dtype(const char *s)
{
	if (!strcmp(s, "int"))
		return INT;
	else if (!strcmp(s, "void"))
		return VOID;
	else
		return (data_t)-1;
}

// convert string to number for 2-16 bases - do not check base
int str2num(const char *s, int base)
{
	int len = strlen(s);
	int val = 0;
	for (int i = 0; i < len; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
			val = val * base + (s[i] - '0');
		else if (s[i] >= 'a' && s[i] <= 'f')
			val = val * base + (s[i] - 'a') + 10;
		else if (s[i] >= 'A' && s[i] <= 'F')
			val = val * base + (s[i] - 'A') + 10;
		else
			return -1;
	}
	return val;
}

/////////////////////////////////////////////////////////////////////

static int temp_id = 0; // temporary variable index
bool is_cond = false; // is the expression in the cond ?
static const int int_size = 4; // the size of int in eeyore
static node_basic *last_stmt; // last statement in the function - for return

// for control flow
static int label_id = 0; // now label index
static int fall_label; // fall labels
static int while_next_label; // false label
static int while_start_label; // start label only for while

static string gen_vardecl(const var_entry &func, bool indent);
static void check_single(
	exp_node *RParam, const var_entry &FParam, const string &func_name);

// generate code
void node_basic::gen_code()
{
	if (child)
		code = child->code;
}

// set next node pointer
void node_basic::set_next(node_basic *_next)
{
	next = _next;
	if (next)
		code = code + next->code;
}

vardef_node::vardef_node(const char *_name, bool _is_const, bool _is_pt,
	bool _is_param, exp_node *first_dim, exp_node *first_val)
{
	name = string(_name);
	dbg_printf("var %s, ", _name);
	is_const = _is_const;
	is_pt = _is_pt;
	is_param = _is_param;

	set_shape(first_dim);
	if (dim.size() > 2)
		throw 0;
	is_array = (dim.size() != 0);
	dbg_printf("is_array: %d, is_param: %d, is_const: %d\n", is_array, is_param,
		is_const);
	if (first_val) // initialize
	{
		if (is_array && first_val->exp_type != EXP_INITVAL)
		{
			auto msg = "array '" + name + "' must be initialized by a list";
			yyerror(msg.c_str());
		}
		if (is_array)
			val = set_val(dim, first_val->child);
		else
			val = set_val(dim, first_val);
	}
	vector<int> int_val;
	for (auto i : val)
	{
		int_val.push_back(i->num);
		dbg_printf("%d ", *int_val.rbegin());
	}
	int_val.insert(int_val.end(), size - int_val.size(), 0);
	reg_var(name, is_const, is_array, is_param, dim, int_val);
	gen_code();
}

// set the dim of array
void vardef_node::set_shape(exp_node *first_dim)
{
	size = 1;
	if (is_pt)
		dim.push_back(0);
	while (first_dim)
	{
		first_dim->reduce();
		size_t temp = first_dim->num;
		if (first_dim->exp_type != EXP_NUM || first_dim->num <= 0)
		{
			yyerror("array size in delaration must be a positive constant int");
			temp = 1;
		}
		size *= temp;
		dim.push_back(temp);
		first_dim = first_dim->next;
	}
}

// generate code
void vardef_node::gen_code()
{
	if (is_param) // do nothing for parameters
		return;
	var_entry query;
	find_var(name, query);
	if (blk_id == 0) // only declare global variables
		code = gen_vardecl(query, false);
	else if (!val.empty()) // generate initial values
	{
		if (!is_array) // variable
		{
			code += val[0]->code;
			code +=
				"\t" + query.eeyore_name + " = " + val[0]->eeyore_name + "\n";
		}
		else // an array
			for (auto i = 0; i < val.size(); i++)
			{
				code += val[i]->code;
				code += "\t" + query.eeyore_name + "[" +
					to_string(i * int_size) + "] = " + val[i]->eeyore_name +
					"\n";
			}
	}
}

// set the values of array
vector<exp_node *> vardef_node::set_val(vector<int> &_dim, exp_node *first_val)
{
	vector<exp_node *> ret;
	auto zero_exp = new exp_node(EXP_NUM, "", 0);
	if (dim.empty()) // an expression
	{
		if (first_val->exp_type ==
			EXP_INITVAL) // an expression should not have a child
			yyerror("expected an expression");
		first_val->reduce();
		first_val->new_temp();
		if (is_const && first_val->exp_type != EXP_NUM)
			yyerror("expected a constant expression");
		ret.push_back(first_val);
		return ret;
	}
	// number of entries to fill
	auto to_fill =
		std::accumulate(_dim.begin(), _dim.end(), 1, std::multiplies<int>());
	auto next_size = to_fill / _dim[0];
	// an array
	while (first_val)
	{
		if (first_val->exp_type == EXP_INITVAL) // traverse the child
		{
			if (dim.size() == 1)
			{
				auto msg = "too deep list initializer in '" + name + "'";
				yyerror(msg.c_str());
				first_val = first_val->next;
				continue;
			}
			if (to_fill % next_size != 0)
			{
				auto msg = "unaligned initializer in '" + name + "'";
				yyerror(msg.c_str());
				first_val = first_val->next;
				continue;
			}
			auto down_dim = _dim;
			down_dim.erase(down_dim.begin());
			auto res = set_val(down_dim, first_val->child);
			to_fill -= res.size();
			if (to_fill < 0)
			{
				auto msg =
					"excessive elements in array initializer of '" + name + "'";
				yyerror(msg.c_str());
				to_fill += res.size();
				ret.insert(ret.end(), res.begin(), res.begin() + to_fill);
				return ret;
			}
			ret.insert(ret.end(), res.begin(), res.end());
		}
		else // an expression
		{
			first_val->reduce();
			first_val->new_temp();
			if (is_const && first_val->exp_type != EXP_NUM)
				yyerror("expected a constant expression");
			to_fill--;
			if (to_fill < 0)
			{
				auto msg =
					"excessive elements in array initializer of '" + name + "'";
				yyerror(msg.c_str());
				to_fill++;
				return ret;
			}
			ret.push_back(first_val);
		}
		first_val = first_val->next;
	}
	ret.insert(ret.end(), to_fill, zero_exp); // all zero filled
	return ret;
}

funcdef_node::funcdef_node(
	const char *_name, data_t _ret_type, node_basic *_blk)
{
	blk = _blk;
	name = string(_name);
	dbg_printf("def a func named %s\n", _name);
	ret_type = _ret_type;
	add_ret(blk != nullptr);
	gen_code();
	temp_id = 0;
}

// always add an return statement at the end of the funcdef
void funcdef_node::add_ret(bool is_append)
{
	dbg_printf("add ret is_append: %d\n", is_append);
	now_func_name = name;
	auto has_ret_val = (func_table[name].ret_type == INT);
	ret_stmt_node *ret_stmt;
	if (has_ret_val)
		ret_stmt = new ret_stmt_node(new exp_node(EXP_NUM, "", 0));
	else
		ret_stmt = new ret_stmt_node(nullptr);
	if (is_append)
		blk->code += ret_stmt->code;
	else
		blk = ret_stmt;
	now_func_name = "";
}

// generate declaration of variable
static string gen_vardecl(const var_entry &var, bool indent)
{
	string prefix = indent ? "\t" : "";
	if (var.is_param) // parameters
		return "";
	if (!var.is_array) // variable
		return prefix + "var " + var.eeyore_name + "\n";
	else // array
	{
		auto size = std::accumulate(
			var.dim.begin(), var.dim.end(), int_size, std::multiplies<int>());
		return prefix + "var " + to_string(size) + " " + var.eeyore_name + "\n";
	}
}

// generate code for function definition
void funcdef_node::gen_code()
{
	func_entry query;
	find_func(name, query);
	code = "f_" + name + +" [" + to_string(query.params.size()) + "]\n";
	// declarations
	for (auto i : query.func_var_table)
	{
		dbg_printf("sysy_name:%s eeyore_name:%s\n", i.sysy_name.c_str(),
			i.eeyore_name.c_str());
		code += gen_vardecl(i, true);
	}
	// initial global variables
	if (name == "main")
	{
		for (auto i : var_stack[0])
		{
			auto var = i.second;
			if (var.val.empty())
				continue;
			if (!var.is_array) // a variable
				code += "\t" + var.eeyore_name + " = " + to_string(var.val[0]) +
					"\n";
			else // an array
				for (auto i = 0; i < var.val.size(); i++)
					code += "\t" + var.eeyore_name + "[" +
						to_string(i * int_size) +
						"] = " + to_string(var.val[i]) + "\n";
		}
	}
	// concatenate the block
	code += blk->code;
	code += "end f_" + name + "\n\n";
}

// set the last statement
stmt_node::stmt_node(node_basic *_child)
{
	child = _child;
	if (blk_id == 1) // in the function
		last_stmt = this;
	gen_code();
	dbg_printf("create a stmt_node...\n");
}

// void function for update
void stmt_node::update(
	cond_exp_node *cond, node_basic *true_stmt, node_basic *false_stmt)
{
}

// allocate a new label
int stmt_node::new_label()
{
	return label_id++;
}

// generate code for block
void stmt_node::gen_code()
{
	if (child)
		code += child->code;
}

assign_stmt_node::assign_stmt_node(exp_node *_lval, exp_node *_assign_exp)
{
	lval = _lval;
	assign_exp = _assign_exp;
	if (lval->exp_type != EXP_ARRAY && lval->exp_type != EXP_VAR)
		yyerror("expression is not assignable");

	assign_exp->reduce();
	if (lval->exp_type == EXP_ARRAY)
		assign_exp->new_temp();
	gen_code();
}

// generate code for assignment
void assign_stmt_node::gen_code()
{
	code = assign_exp->code + lval->code;

	auto r_lval = static_cast<array_exp_node *>(lval);
	code += "\t" + r_lval->eeyore_name + " = " + assign_exp->eeyore_name + "\n";
}

exp_stmt_node::exp_stmt_node(exp_node *_exp_stmt)
{
	exp_stmt = _exp_stmt;
	exp_stmt->reduce();
	gen_code();
}

// generate code for assignment
void exp_stmt_node::gen_code()
{
	if (exp_stmt->exp_type == EXP_FUNC_CALL)
	{
		auto func_exp_stmt = static_cast<func_call_exp_node *>(exp_stmt);
		func_entry query;
		find_func(func_exp_stmt->sysy_func_name, query);
		if (query.ret_type == INT)
		{
			exp_stmt->new_temp();
			code = exp_stmt->code;
		}
		else
		{
			code = exp_stmt->code;
			code += "\t" + exp_stmt->eeyore_name + "\n";
		}
	}
}

if_stmt_node::if_stmt_node()
{
	old_fall_label = fall_label;
	fall_label = new_label();
}

// update the AST of if statement and generate code
void if_stmt_node::update(
	cond_exp_node *_cond, node_basic *_true_stmt, node_basic *_false_stmt)
{
	cond = _cond;
	true_stmt = _true_stmt;
	false_stmt = _false_stmt;
	has_else = (false_stmt != nullptr);
	if (!has_else)
	{
		cond->true_label = true_label = fall_label;
		cond->false_label = false_label = new_label();
	}
	else
	{
		cond->true_label = true_label = new_label();
		cond->false_label = false_label = new_label();
	}
	cond->traverse();
	gen_code();
	fall_label = old_fall_label;
}

// generate the code of if statement
void if_stmt_node::gen_code()
{
	code = cond->code;

	if (has_else)
	{
		code += "l" + to_string(true_label) + ":\t// if true label\n";
		code += true_stmt->code;
		auto if_next_label = new_label();
		code += "\tgoto l" + to_string(if_next_label) + "\n";
		code += "l" + to_string(false_label) + ":\t// if false label\n";
		code += false_stmt->code;
		code += "l" + to_string(if_next_label) + ":\t// if next label\n";
	}
	else
	{
		code += true_stmt->code;
		code += "l" + to_string(false_label) + ":\t// if next label\n";
	}
}

while_stmt_node::while_stmt_node()
{
	old_start_label = while_start_label;
	old_fall_label = fall_label;
	old_next_label = while_next_label;
	while_start_label = new_label();
	fall_label = new_label();
	while_next_label = new_label();
}

// update the AST of while statement and generate code
void while_stmt_node::update(
	cond_exp_node *_cond, node_basic *_true_stmt, node_basic *_next_stmt)
{
	cond = _cond;
	true_stmt = _true_stmt;
	cond->true_label = true_label = fall_label;
	cond->false_label = while_next_label;
	cond->traverse();
	gen_code();
	fall_label = old_fall_label;
	while_start_label = old_start_label;
	while_next_label = old_next_label;
}

// generate code for while statement
void while_stmt_node::gen_code()
{
	code = "l" + to_string(while_start_label) + ":\t// while start label\n";
	code += cond->code;
	code += "l" + to_string(true_label) + ":\t// while true label\n";
	code += true_stmt->code;
	code += "\tgoto l" + to_string(while_start_label) + "\n";
	code += "l" + to_string(while_next_label) + ":\t// while next label\n";
}

goto_stmt_node::goto_stmt_node(bool _is_break)
{
	is_break = _is_break;
	gen_code();
}

// generate code for break and continue statement
void goto_stmt_node::gen_code()
{
	if (is_break)
		code = "\tgoto l" + to_string(while_next_label) + "\n";
	else
		code = "\tgoto l" + to_string(while_start_label) + "\n";
}

ret_stmt_node::ret_stmt_node(exp_node *_ret_val)
{
	dbg_printf("generate a ret_stmt_node... ret_val is null: %d\n",
		_ret_val == nullptr);
	ret_type = (_ret_val != nullptr) ? INT : VOID;
	ret_val = _ret_val;
	if (func_table[now_func_name].ret_type != ret_type)
	{
		string msg;
		switch (func_table[now_func_name].ret_type)
		{
		case INT:
			msg = "non-void function '" + now_func_name +
				"' should return a value";
			break;
		case VOID:
			msg = "void function '" + now_func_name +
				"' should not return a value";
			break;
		default: dbg_printf("there cannot be other type!\n");
		}
		yyerror(msg.c_str());
	}
	if (ret_type == INT)
	{
		ret_val->reduce();
		ret_val->new_temp();
		if (ret_val->exp_type == EXP_PTR)
		{
			string msg =
				"function '" + now_func_name + "' should not return a pointer";
			yyerror(msg.c_str());
		}
	}
	gen_code();
}

void ret_stmt_node::gen_code()
{
	if (ret_val)
	{
		code = ret_val->code;
		code += "\treturn " + ret_val->eeyore_name + "\n";
		dbg_printf("gen_code in ret_stmt...ret_val:%s\n",
			ret_val->eeyore_name.c_str());
	}
	else
		code += "\treturn\n";
}

exp_node::exp_node(exp_t _exp_type, const string &_sysy_name, int _num,
	op_t _op, exp_node *_child)
{
	exp_type = _exp_type;
	sysy_name = _sysy_name;
	child = _child;
	num = _num;
	op = _op;
	reduce();
}

// set next node pointer
void exp_node::set_next(node_basic *_next)
{
	next = static_cast<exp_node *>(_next);
}

// allocate a new temporary variable
void exp_node::new_temp()
{
	if (!temped && exp_type != EXP_NUM && exp_type != EXP_VAR)
	{
		code += "\tt" + to_string(temp_id) + " = " + eeyore_name + "\n";
		sysy_name = "#t" + to_string(temp_id);
		eeyore_name = "t" + to_string(temp_id++);
		reg_var(sysy_name);
		temped = true;
	}
}

// reduce the expression to a simpler form
void exp_node::reduce()
{
	if (exp_type == EXP_NUM)
	{
		eeyore_name = to_string(num);
		dbg_printf("eeyore_name in exp_node: %s\n", eeyore_name.c_str());
	}
	else if (exp_type == EXP_VAR)
	{
		var_entry query;
		find_var(sysy_name, query);
		eeyore_name = query.eeyore_name;
		if (query.is_const && query.dim.size() == 0) // a const
		{
			exp_type = EXP_NUM;
			eeyore_name = to_string(query.val[0]);
		}
	}
}

array_exp_node::array_exp_node(
	exp_node *first_dim, const string &_sysy_array_name) :
	exp_node(EXP_ARRAY),
	sysy_array_name(_sysy_array_name)
{
	var_entry query;
	if (!find_var(sysy_array_name, query)) // not defined
	{
		string msg = "identifier '" + sysy_array_name + "' is not defined";
		yyerror(msg.c_str());
		return;
	}
	if (!query.is_array) // not an array
	{
		if (first_dim) // should not be indexed!
		{
			string msg = "identifier '" + sysy_array_name + "' is not an array";
			yyerror(msg.c_str());
			return;
		}
		exp_type = EXP_VAR;
		eeyore_name = query.eeyore_name;
	}
	else
		while (first_dim) // an array
		{
			first_dim->reduce();
			first_dim->new_temp();
			assert(exp_type != EXP_INITVAL);
			sysy_idx.push_back(first_dim);
			first_dim = first_dim->next;
		}
	reduce();
}

// generate the index expression for sysy index vector
exp_node *array_exp_node::idx_open(const vector<exp_node *> &idx, size_t len)
{
	if (idx.empty())
		return new exp_node(EXP_NUM, "", 0);
	auto scale = new exp_node(EXP_NUM, "", len);
	auto ret = *idx.begin();
	auto size = idx.size();
	for (auto i = 1; i < size; i++)
	{
		dbg_printf("idx num: %d,\n", idx[i]->num);
		ret = new arith_exp_node(MUL, ret, scale);
		ret = new arith_exp_node(ADD, ret, idx[i]);
	}
	return ret;
}

// reduce to a eeyore form - variable, number, array or pointer
void array_exp_node::reduce()
{
	if (!sysy_name.empty())
		return;
	var_entry query;
	if (!find_var(sysy_array_name, query)) // not defined
	{
		string msg = "identifier '" + sysy_array_name + "' is not defined";
		yyerror(msg.c_str());
		return;
	}
	if (exp_type == EXP_NUM) // a number
		return;
	if (exp_type == EXP_VAR) // a variable
	{
		if (query.is_const) // a constant
		{
			exp_type = EXP_NUM;
			num = query.val[0];
			eeyore_name = to_string(num);
		}
		else
			eeyore_name = query.eeyore_name;
		return;
	}
	// an array
	if (sysy_idx.size() > query.dim.size()) // an error
	{
		yyerror("subscripted value is not an array");
		sysy_name = "?array";
		return;
	}
	else if (sysy_idx.size() == query.dim.size()) // an array
	{
		eeyore_exp = idx_open(sysy_idx, int_size);
		eeyore_exp = new arith_exp_node(
			MUL, eeyore_exp, new exp_node(EXP_NUM, "", int_size));
		auto array_id = new exp_node(EXP_VAR, sysy_array_name);
		eeyore_exp = new arith_exp_node(LOAD, array_id, eeyore_exp);
	}
	else // a pointer
	{
		auto array_id = new exp_node(EXP_VAR, sysy_array_name);
		exp_type = EXP_PTR;
		int steplen = int_size;
		auto start_dim = query.dim.size() - sysy_idx.size();
		auto dim_size = query.dim.size();
		for (auto i = start_dim; i < dim_size; i++)
			steplen *= query.dim[i];
		eeyore_exp = idx_open(sysy_idx, int_size);
		eeyore_exp = new arith_exp_node(
			MUL, eeyore_exp, new exp_node(EXP_NUM, "", steplen));
		eeyore_exp = new arith_exp_node(ADD, array_id, eeyore_exp);
	}
	eeyore_exp->reduce();
	code = eeyore_exp->code;
	eeyore_name = eeyore_exp->eeyore_name;
	sysy_name = "?array";
}

arith_exp_node::arith_exp_node(op_t _op, exp_node *_left, exp_node *_right) :
	exp_node(EXP_TEMP, "", 0, _op), left(_left), right(_right)
{
	assert(op != NONE && op != AND && op != OR);
	if (!is_cond && op == NOT)
		yyerror("operator '!' should only appear in condition expression");
	if (left->exp_type == EXP_PTR || (right && right->exp_type == EXP_PTR))
		yyerror("pointer cannot be in an expression");
	reduce();
}

// reduce the expression to a simpler form, if can't, create a temp name
void arith_exp_node::reduce()
{
	if (!sysy_name.empty())
		return;
	assert(left->exp_type != EXP_INITVAL);
	if (right) // binary operator
	{
		assert(right->exp_type != EXP_INITVAL);
		// can reduce!
		if (left->exp_type == EXP_NUM && right->exp_type == EXP_NUM)
		{
			exp_type = EXP_NUM;
			switch (op)
			{
			case EQ: num = left->num == right->num; break;
			case NE: num = left->num != right->num; break;
			case GE: num = left->num >= right->num; break;
			case LE: num = left->num <= right->num; break;
			case G: num = left->num > right->num; break;
			case L: num = left->num < right->num; break;
			case ADD: num = left->num + right->num; break;
			case SUB: num = left->num - right->num; break;
			case MUL: num = left->num * right->num; break;
			case DIV: num = left->num / right->num; break;
			case MOD: num = left->num % right->num; break;
			default: dbg_printf("%d is not a binary operator!", (int)op); break;
			}
			eeyore_name = to_string(num);
		}
		else
		{
			left->new_temp();
			right->new_temp();
			code = left->code + right->code;
			switch (op)
			{
			case EQ:
				eeyore_name = left->eeyore_name + " == " + right->eeyore_name;
				break;
			case NE:
				eeyore_name = left->eeyore_name + " != " + right->eeyore_name;
				break;
			case GE:
				eeyore_name = left->eeyore_name + " >= " + right->eeyore_name;
				break;
			case LE:
				eeyore_name = left->eeyore_name + " <= " + right->eeyore_name;
				break;
			case G:
				eeyore_name = left->eeyore_name + " > " + right->eeyore_name;
				break;
			case L:
				eeyore_name = left->eeyore_name + " < " + right->eeyore_name;
				break;
			case ADD:
				eeyore_name = left->eeyore_name + " + " + right->eeyore_name;
				break;
			case SUB:
				eeyore_name = left->eeyore_name + " - " + right->eeyore_name;
				break;
			case MUL:
				eeyore_name = left->eeyore_name + " * " + right->eeyore_name;
				break;
			case DIV:
				eeyore_name = left->eeyore_name + " / " + right->eeyore_name;
				break;
			case MOD:
				eeyore_name = left->eeyore_name + " % " + right->eeyore_name;
				break;
			case LOAD:
				eeyore_name =
					left->eeyore_name + " [" + right->eeyore_name + "]";
				break;
			default: dbg_printf("%d is not a binary operator!", (int)op); break;
			} // switch

		} // else
	} // if
	else // unary operator
	{
		if (left->exp_type == EXP_NUM)
		{
			exp_type = EXP_NUM;
			switch (op)
			{
			case NEGATE: num = -left->num; break;
			case NOT: num = !left->num; break;
			default: dbg_printf("%d is not a unary operator!", (int)op); break;
			}
			eeyore_name = to_string(num);
		}
		else
		{
			left->new_temp();
			code = left->code;
			switch (op)
			{
			case NEGATE: eeyore_name = "- " + left->eeyore_name; break;
			case NOT: eeyore_name = "! " + left->eeyore_name; break;
			default: dbg_printf("%d is not a unary operator!", (int)op); break;
			}
		}
	}
	sysy_name = "?exp_arith";
}

func_call_exp_node::func_call_exp_node(
	const string &func_name, exp_node *first_param) :
	exp_node(EXP_FUNC_CALL)
{
	sysy_func_name = func_name;
	// tackle marcos
	if (sysy_func_name == "starttime" || sysy_func_name == "stoptime")
	{
		sysy_func_name = "_sysy_" + sysy_func_name;
		if (first_param)
		{
			string msg =
				"too many arguments provided to function-like macro '" +
				func_name + "'";
			yyerror(msg.c_str());
		}
		first_param = new exp_node(EXP_NUM, "", yylineno);
	}
	dbg_printf("func call to %s \n", sysy_func_name.c_str());
	while (first_param)
	{
		params.push_back(first_param);
		first_param = first_param->next;
	}
	check_valid();
	reduce();
}

// check if a single parameter match
static void check_single(
	exp_node *RParam, const var_entry &FParam, const string &func_name)
{
	RParam->reduce();
	RParam->new_temp();
	dbg_printf("param : %s,\n", RParam->sysy_name.c_str());
	if (FParam.is_array) // an array
	{
		if (RParam->exp_type != EXP_PTR)
		{
			string msg = "mismatch type of parameter '" + FParam.sysy_name +
				"' in call to '" + func_name + "'";
			yyerror(msg.c_str());
			return;
		}
		auto r_array_exp = static_cast<array_exp_node *>(RParam);
		var_entry query;
		find_var(r_array_exp->sysy_array_name, query);
		if (query.is_const) // should not be const
		{
			string msg = "mismatch type of parameter '" + FParam.sysy_name +
				"' in call to '" + func_name + "'";
			yyerror(msg.c_str());
			return;
		}
		auto start_dim = r_array_exp->sysy_idx.size() + 1;
		auto dim_n = FParam.dim.size();
		if (query.dim.size() - r_array_exp->sysy_idx.size() != dim_n)
		{
			string msg = "mismatch type of parameter '" + FParam.sysy_name +
				"' in call to '" + func_name + "'";
			yyerror(msg.c_str());
			return;
		}
		for (auto i = start_dim; i < dim_n; i++)
			if (query.dim[i] != FParam.dim[i])
			{
				string msg = "mismatch type of parameter '" + FParam.sysy_name +
					"' in call to '" + func_name + "'";
				yyerror(msg.c_str());
				return;
			}
	}
	// an int
	else if (RParam->exp_type == EXP_PTR)
	{
		string msg = "mismatch type of parameter '" + FParam.sysy_name +
			"' in call to '" + func_name + "'";
		yyerror(msg.c_str());
		return;
	}
}

// check whether a function call is valid
void func_call_exp_node::check_valid()
{
	func_entry query;
	if (!find_func(sysy_func_name, query))
	{
		string msg = "function '" + sysy_func_name + "' is not defined";
		yyerror(msg.c_str());
		return;
	}
	auto size = params.size();
	dbg_printf("size: %lu\n", size);
	if (size != query.params.size())
	{
		dbg_printf("real %lu : formal %lu\n", size, query.params.size());
		string msg;
		if (size > query.params.size())
			msg = "too many arguments in call to '" + sysy_func_name + "'";
		else
			msg = "too few arguments in call to '" + sysy_func_name + "'";
		yyerror(msg.c_str());
		return;
	}
	for (auto i = 0; i < size; i++)
		check_single(params[i], query.params[i], sysy_func_name);
}

// check the return type of the function call
void func_call_exp_node::check_ret_type(exp_node *func_exp)
{
	if (func_exp->exp_type != EXP_FUNC_CALL) // not a function call
		return;
	auto func_call_exp = static_cast<func_call_exp_node *>(func_exp);
	func_entry query;
	find_func(func_call_exp->sysy_func_name, query);
	if (query.ret_type != INT)
	{
		string msg;
		msg = "function '" + func_call_exp->sysy_func_name +
			"' has no return value";
		yyerror(msg.c_str());
	}
}

// reduce the expression to a simpler form
void func_call_exp_node::reduce()
{
	if (!sysy_name.empty())
		return;
	for (auto i : params)
		code += i->code;
	dbg_printf("reduce size: %lu\n", params.size());
	for (auto i : params)
	{
		dbg_printf("real params: %s\n", i->eeyore_name.c_str());
		code += "\tparam " + i->eeyore_name + "\n";
	}
	eeyore_name = "call f_" + sysy_func_name;
	sysy_name = "?func_call";
}

// allocate a new temporary name for func call
void func_call_exp_node::new_temp()
{
	if (temped || exp_type == EXP_NUM || exp_type == EXP_VAR)
		return;
	check_ret_type(this);
	sysy_name = "#t" + to_string(temp_id);
	eeyore_name = "t" + to_string(temp_id++);
	code += "\t" + eeyore_name + " = call f_" + sysy_func_name + "\n";
	reg_var(sysy_name);
	temped = true;
	return;
}

cond_exp_node::cond_exp_node(op_t _op, exp_node *_left, exp_node *_right) :
	exp_node(EXP_COND)
{
	op = _op;
	left = _left;
	right = _right;
}

// traverse the AST of condition expressions and generate code
void cond_exp_node::traverse()
{
	if (right == nullptr) // leaf node
	{
		left->reduce();
		left->new_temp();
		code += left->code;
		if (true_label != fall_label && false_label != fall_label) // if-else
		{
			code += "\tif " + left->eeyore_name + " == 1 goto l" +
				to_string(true_label) + "\n";
			code += "\tgoto l" + to_string(false_label) + "\n";
		}
		else if (true_label != fall_label) // if false_label
			code += "\tif " + left->eeyore_name + " == 1 goto l" +
				to_string(true_label) + "\n";
		else if (false_label != fall_label)
			code += "\tif " + left->eeyore_name + " == 0 goto l" +
				to_string(false_label) + "\n";
		return;
	}
	// inner node
	if (op == OR)
	{
		auto c_left = static_cast<cond_exp_node *>(left);
		auto c_right = static_cast<cond_exp_node *>(right);

		if (true_label != fall_label)
			c_left->true_label = true_label;
		else
			c_left->true_label = stmt_node::new_label();
		c_left->false_label = fall_label;
		c_left->traverse();

		c_right->false_label = false_label;
		c_right->true_label = true_label;
		c_right->traverse();

		code += c_left->code + c_right->code;
		if (true_label == fall_label)
			code += "l" + to_string(c_left->true_label) + ":\n";
	}
	else if (op == AND)
	{
		auto c_left = static_cast<cond_exp_node *>(left);
		auto c_right = static_cast<cond_exp_node *>(right);
		c_left->true_label = fall_label;
		c_left->false_label = false_label;
		c_left->traverse();

		c_right->false_label = false_label;
		c_right->true_label = true_label;
		c_right->traverse();
		code += c_left->code + c_right->code;
		// code += "l" + to_string(c_right->true_label) + ":\n";
	}
	else
	{
		dbg_printf("should not have other operator!\n");
		throw -1;
	}
}