/* utility functions for nodes */

#include "include/node.hh"
#include "include/symtab.hh"
#ifdef DEBUG
#include <cassert>
#define dbg_printf(...)      \
	do                       \
	{                        \
		printf(__VA_ARGS__); \
		fflush(stdout);      \
	} while (0);
#else
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

// traverse the node and generate code
string node_basic::traverse(node_basic *basic)
{
	return code = next->traverse(this);
}

vardef_node::vardef_node(const char *_name, bool _is_const, bool _is_pt,
	exp_node *_first_dim, exp_node *_first_val)
{
	name = string(_name);
	is_const = _is_const;
	is_pt = _is_pt;
	first_dim = _first_dim;
	set_shape();
	is_array = (is_pt || dim.size() == 0);
	first_val = _first_val;
	if (!is_pt)
	{
		if (first_val) // has init val
			val = set_val(dim, first_val);
		else if (blk_id == 0 || is_const) // must be initialized
		{
			exp_node *zero = new exp_node(EXP_NUM, string(), 0);
			for (auto i = 0; i < size; i++)
				val.push_back(zero);
		}
	}
	vector<int> int_val;
	for (auto i : val)
	{
		int_val.push_back(i->num);
		dbg_printf("%d ", *int_val.rbegin());
	}
	dbg_printf("\n");
	reg_var(name, is_const, is_array, is_pt, dim, int_val);
}

// set the dim of array
void vardef_node::set_shape()
{
	size = 1;
	while (first_dim)
	{
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
vector<exp_node *> vardef_node::set_val(vector<int> &dim, exp_node *_first_val)
{
	exp_node *zero = new exp_node(EXP_NUM, string(), 0);
	vector<exp_node *> ret;
	if (dim.size() == 0) // a value !
	{
		if (!_first_val || _first_val->exp_type == EXP_INITVAL)
		{
			yyerror("expected expression");
			ret.push_back(zero);
		}
		else
		{
			// global variable
			if (blk_id == 0 && _first_val->exp_type != EXP_NUM)
			{
				yyerror("global initializer must be constant");
				ret.push_back(zero);
			}
			else
				ret.push_back(_first_val);
		}
	}
	else // an array
	{
		int valid_dim = 1;
		for (auto i = dim.begin() + 1; i != dim.end(); i++)
			valid_dim *= *i;
		while (_first_val)
		{
			if (valid_dim * dim[0] < ret.size())
			{
				yyerror("excess elements in array initializer");
				ret.erase(ret.begin() + valid_dim * dim[0], ret.end());
				return ret;
			}
			if (_first_val->child) // preorder, child first
			{
				if (_first_val->child->exp_type != EXP_INITVAL) // an expression
				{
					// global variable
					if (blk_id == 0 && _first_val->child->exp_type != EXP_NUM)
					{
						yyerror("global initializer must be constant");
						ret.push_back(zero);
					}
					else
						ret.push_back(_first_val->child);
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
					auto temp = set_val(new_shape, _first_val->child);
					ret.insert(ret.end(), temp.begin(), temp.end());
				}
			} // if end
			else // an empty brace
				ret.insert(ret.end(), valid_dim, zero);
			_first_val = _first_val->next;
		} // while end
		ret.insert(ret.end(), valid_dim * dim[0] - ret.size(), zero);
	} // else end
	return ret;
}

exp_node::exp_node(
	exp_t _exp_type, string _sysy_name, int _num, op_t _op, exp_node *_child)
{
	exp_type = _exp_type;
	sysy_name = _sysy_name;
	child = _child;
	num = _num;
	op = _op;
	reduce();
}

// allocate a new temporary variable - return a temporary variable in sysY
string exp_node::new_temp()
{
	string name = "#" + to_string(temp_id++);
	reg_var(name);
	return name;
}

// reduce the expression to a simpler form
void exp_node::reduce()
{
	if (exp_type == EXP_VAR)
	{
		var_entry query;
		if (find_var(sysy_name, query))
		{
			if (query.is_const && !query.is_array)
			{
				exp_type = EXP_NUM;
				num = query.val[0];
			}
		}
	}
}

arith_exp_node::arith_exp_node(op_t _op, exp_node *_left, exp_node *_right) :
	exp_node(EXP_TEMP, string(), 0, _op), left(_left), right(_right)
{
	assert(op != NONE && op != AND && op != OR);
	if (is_cond && op == NOT)
		yyerror("operator '!' should appear only in condition");
	reduce();
}

// reduce the expression to a simpler form, if can't, create a temp name
void arith_exp_node::reduce()
{
	left->reduce();
	if (right) // binary operator
	{
		right->reduce();
		// can reduce!
		if (left->exp_type == EXP_NUM && right->exp_type == EXP_NUM)
		{
			exp_type = EXP_NUM;
			switch (op)
			{
			case EQ: num = left->num == right->num; return;
			case NE: num = left->num != right->num; return;
			case GE: num = left->num >= right->num; return;
			case LE: num = left->num <= right->num; return;
			case G: num = left->num > right->num; return;
			case L: num = left->num < right->num; return;
			case ADD: num = left->num + right->num; return;
			case SUB: num = left->num - right->num; return;
			case MUL: num = left->num * right->num; return;
			case DIV: num = left->num / right->num; return;
			case MOD: num = left->num % right->num; return;
			default:
				dbg_printf("%d is not a binary operator!", (int)op);
				return;
			}
		}
		else // cannot reduce
			sysy_name = new_temp();
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
		else // cannot reduce
			sysy_name = new_temp();
	}
}