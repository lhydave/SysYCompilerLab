/* utility functions for nodes */

#include "include/node.hh"
#include "include/symtab.hh"
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
		else if (s[i] >= 'A' && s[i] < 'F')
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
static node_basic *last_stmt; // last statement in the function

// traverse the node and generate code
void node_basic::traverse(node_basic *basic)
{
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
	is_array = (dim.size() != 0);
	if (!is_pt && first_val)
		val = set_val(dim, first_val);
	vector<int> int_val;
	for (auto i : val)
	{
		int_val.push_back(i->num);
		dbg_printf("%d ", *int_val.rbegin());
	}
	dbg_printf("\n");
	reg_var(name, is_const, is_array, is_param, dim, int_val);
}

// set the dim of array
void vardef_node::set_shape(exp_node *first_dim)
{
	size = 1;
	if (is_pt)
		dim.push_back(0);
	while (first_dim)
	{
		first_dim->reduce(true);
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

// set the values of array
vector<exp_node *> vardef_node::set_val(vector<int> &dim, exp_node *first_val)
{
	exp_node *zero = new exp_node(EXP_NUM, string(), 0);
	vector<exp_node *> ret;
	if (dim.size() == 0) // a value !
	{
		if (!first_val || first_val->exp_type == EXP_INITVAL)
		{
			yyerror("expected expression");
			ret.push_back(zero);
		}
		else
		{
			// global variable
			if (blk_id == 0 && first_val->exp_type != EXP_NUM)
			{
				yyerror("global initializer must be constant");
				ret.push_back(zero);
			}
			else
			{
				first_val->reduce(true);
				ret.push_back(first_val);
			}
		}
	}
	else // an array
	{
		int valid_dim = 1;
		for (auto i = dim.begin() + 1; i != dim.end(); i++)
			valid_dim *= *i;
		while (first_val)
		{
			if (valid_dim * dim[0] <= ret.size())
			{
				yyerror("excess elements in array initializer");
				ret.erase(ret.begin() + valid_dim * dim[0], ret.end());
				return ret;
			}
			if (first_val->child) // preorder, child first
			{
				if (first_val->child->exp_type != EXP_INITVAL) // an expression
				{
					// global variable
					if (blk_id == 0 && first_val->child->exp_type != EXP_NUM)
					{
						yyerror("global initializer must be constant");
						ret.push_back(zero);
					}
					else
						ret.push_back(first_val->child);
				}
				else
				{
					if (valid_dim != 1 && ret.size() % valid_dim != 0)
					{
						yyerror("unaligned initializer");
						ret.insert(
							ret.end(), valid_dim * dim[0] - ret.size(), zero);
						return ret;
					}
					vector<int> new_shape(dim);
					new_shape.erase(new_shape.begin());
					auto temp = set_val(new_shape, first_val->child);
					ret.insert(ret.end(), temp.begin(), temp.end());
				}
			} // if end
			else // an empty brace
				ret.insert(ret.end(), valid_dim, zero);
			first_val = first_val->next;
		} // while end
		ret.insert(ret.end(), valid_dim * dim[0] - ret.size(), zero);
	} // else end
	return ret;
}

funcdef_node::funcdef_node(const char *_name, data_t _ret_type,
	node_basic *_blk, vardef_node *first_param)
{
	blk = _blk;
	name = string(_name);
	ret_type = _ret_type;
	while (first_param)
	{
		params.push_back(first_param);
		first_param = first_param->next;
	}
}

// always add an return statement at the end of the funcdef
void funcdef_node::add_ret()
{
	// TODO:
	// last_stmt->set_next(new ret_stmt_node(ret_type == INT, new
	// exp_node(EXP_NUM, string(), 0)));
}

exp_node::exp_node(
	exp_t _exp_type, string _sysy_name, int _num, op_t _op, exp_node *_child)
{
	exp_type = _exp_type;
	sysy_name = _sysy_name;
	child = _child;
	num = _num;
	op = _op;
	reduce(false);
}

// set next node pointer
void exp_node::set_next(node_basic *_next)
{
	next = static_cast<exp_node *>(_next);
}

// allocate a new temporary variable
void exp_node::new_temp()
{
	if (sysy_name.empty())
	{
		sysy_name = "#t" + to_string(temp_id++);
		reg_var(sysy_name);
		return;
	}
}

// reduce the expression to a simpler form
void exp_node::reduce(bool alloc)
{
	if (alloc)
		assert(exp_type != EXP_INITVAL);
}

array_exp_node::array_exp_node(exp_node *first_dim, string _sysy_name) :
	exp_node(EXP_ARRAY), sysy_array_name(_sysy_name)
{
	var_entry query;
	if (!find_var(_sysy_name, query)) // not defined
	{
		string msg = "identifier '" + _sysy_name + "' is not defined";
		yyerror(msg.c_str());
		return;
	}
	if (!query.is_array) // not an array
	{
		if (first_dim) // should not be indexed!
		{
			string msg = "identifier '" + _sysy_name + "' is not an array";
			yyerror(msg.c_str());
			return;
		}
		exp_type = EXP_VAR;
		sysy_name = sysy_array_name;
	}
	else
		while (first_dim) // an array
		{
			first_dim->reduce(true);
			assert(exp_type != EXP_INITVAL);
			sysy_idx.push_back(first_dim);
			first_dim = first_dim->next;
		}
	reduce(false);
}

// generate the index expression for sysy index vector
exp_node *array_exp_node::idx_open(const vector<exp_node *> &idx, size_t len)
{
	if (idx.empty())
		return new exp_node(EXP_NUM, string(), 0);
	auto scale = new exp_node(EXP_NUM, string(), len);
	auto ret = *idx.begin();
	auto size = idx.size();
	for (auto i = 1; i < size; i++)
	{
		ret = new arith_exp_node(MUL, ret, scale);
		ret = new arith_exp_node(ADD, ret, idx[i]);
	}
	return ret;
}

// reduce to a eeyore form - variable, number, array or pointer
void array_exp_node::reduce(bool alloc)
{
	var_entry query;
	if (!find_var(sysy_array_name, query)) // not defined
	{
		string msg = "identifier '" + sysy_array_name + "' is not defined";
		yyerror(msg.c_str());
		return;
	}
	if (exp_type == EXP_VAR) // a variable
	{
		if (query.is_const) // a constant
		{
			exp_type = EXP_NUM;
			num = query.val[0];
		}
		return;
	}
	// an array
	if (sysy_idx.size() > query.dim.size()) // an error
		yyerror("subscripted value is not an array");
	else if (sysy_idx.size() == query.dim.size()) // an array
	{
		eeyore_exp = idx_open(sysy_idx, int_size);
		eeyore_exp = new arith_exp_node(
			MUL, eeyore_exp, new exp_node(EXP_NUM, string(), int_size));
		auto array_id = new exp_node(EXP_VAR, sysy_array_name);
		eeyore_exp = new arith_exp_node(LOAD, array_id, eeyore_exp);
	}
	else // a pointer
	{
		auto array_id = new exp_node(EXP_VAR, sysy_array_name);
		exp_type = EXP_PTR;
		int steplen = 4;
		auto start_dim = query.dim.size() - sysy_idx.size();
		auto dim_size = query.dim.size();
		for (auto i = start_dim; i < dim_size; i++)
			steplen *= query.dim[i];
		eeyore_exp = idx_open(sysy_idx, int_size);
		eeyore_exp = new arith_exp_node(
			MUL, eeyore_exp, new exp_node(EXP_NUM, string(), steplen));
		eeyore_exp = new arith_exp_node(ADD, array_id, eeyore_exp);
	}
	eeyore_exp->reduce(false);
	if (alloc)
		new_temp();
}

arith_exp_node::arith_exp_node(op_t _op, exp_node *_left, exp_node *_right) :
	exp_node(EXP_TEMP, string(), 0, _op), left(_left), right(_right)
{
	assert(op != NONE && op != AND && op != OR);
	if (!is_cond && op == NOT)
		yyerror("operator '!' should only appear in condition expression");
	if (left->exp_type == EXP_PTR || (right && right->exp_type == EXP_PTR))
		yyerror("pointer cannot be in an expression");
	reduce(false);
}

// reduce the expression to a simpler form, if can't, create a temp name
void arith_exp_node::reduce(bool alloc)
{
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
		}
		else
		{
			if (left->sysy_name.empty()) // delay assignment
				left->new_temp();
			if (right->sysy_name.empty()) // delay assignment
				right->new_temp();
		}
	}
	else // unary operator
	{
		if (left->exp_type == EXP_NUM)
		{
			exp_type = EXP_NUM;
			switch (op)
			{
			case NEGATE: num = -left->num; return;
			case NOT: num = !left->num; return;
			default: dbg_printf("%d is not a unary operator!", (int)op); return;
			}
		}
		else if (left->sysy_name.empty()) // delay assignment
			left->new_temp();
	}
	if (alloc) // allocate a name
		new_temp();
}

func_call_exp_node::func_call_exp_node(
	const string &func_name, exp_node *first_param) :
	exp_node(EXP_FUNC_CALL)
{
	sysy_func_name = func_name;
	dbg_printf("func call to %s ", sysy_func_name.c_str());
	while (first_param)
	{
		params.push_back(first_param);
		first_param = first_param->next;
	}
	check_valid();
}

// check if a single parameter match
static void check_single(
	exp_node *RParam, const var_entry &FParam, const string &func_name)
{
	RParam->reduce(true);
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
void func_call_exp_node::reduce(bool alloc)
{
	if (alloc)
		new_temp();
}

// allocate a new temporary name for func call
void func_call_exp_node::new_temp()
{
	if (sysy_name.empty())
	{
		check_ret_type(this);
		sysy_name = "#t" + to_string(temp_id++);
		reg_var(sysy_name);
		return;
	}
}