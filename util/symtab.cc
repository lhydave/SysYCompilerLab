/* utility function for symbol table */

#include "include/symtab.hh"
#include "include/node.hh"
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

vector<var_table_t> var_stack; // variable table stack
func_table_t func_table; // function table stack
string func_name; // name of current function
int blk_id = 0; // index of current block - 0 for global
bool create = true; // whether to create a symbol table
static int var_id = 0; // index of current variable
static int param_id = 0; // index of current parameter

var_entry::var_entry(bool _is_array, bool _is_const, bool _is_param,
	const vector<int> &_dim, const vector<int> &_val)
{
	is_array = _is_array;
	is_const = _is_const;
	is_param = _is_param;
	dim = _dim;
	val = _val;
}

// initialize the tables
void init_tables()
{
	var_stack.clear();
	func_table.clear();
	var_stack.push_back(var_table_t());
}

// check if the variable is defined in current block
bool defined(string &name)
{
	if (func_table.count(name))
		return true;

	if (var_stack[blk_id].count(name))
		return true;
	return false;
}

// increment a block
void inc_blk()
{
	if (create)
	{
		blk_id++;
		var_stack.push_back(var_table_t());
	}
	create = true;
}

// decrement a block
void dec_blk()
{
	blk_id--;
	var_stack.pop_back();
}

// register a variable into the variable table
void reg_var(string name, bool is_const, bool is_array, bool is_param,
	const vector<int> &dim, const vector<int> &val)
{
	assert(!name.empty());
	if (defined(name))
	{
		string msg = "identifier '" + name + "' already defined";
		yyerror(msg.c_str());
		return;
	}
	var_entry new_entry(is_array, is_const, is_param, dim, val);
	if (is_param)
		new_entry.eeyore_name = "p" + to_string(param_id++);
	else if (name[0] != '#')
		new_entry.eeyore_name = "T" + to_string(var_id++);
	else
		new_entry.eeyore_name = name.substr(1);
	if (blk_id)
		func_table[func_name].decls.push_back(new_entry);
	var_stack[blk_id][name] = new_entry;
	return;
}

// find if variable exists and store the item
bool find_var(string name, var_entry &store)
{
	for (int i = blk_id; i >= 0; i--)
		if (var_stack[i].count(name))
		{
			store = var_stack[i][name];
			return true;
		}
	return false;
}

// find the variable if exists
bool find_var(string name)
{
	for (int i = blk_id; i >= 0; i--)
		if (var_stack[i].count(name))
			return true;
	return false;
}