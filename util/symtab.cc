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
	const vector<int> &_dim, const vector<int> &_val, const string &_sysy_name)
{
	is_array = _is_array;
	is_const = _is_const;
	is_param = _is_param;
	sysy_name = _sysy_name;
	dim = _dim;
	val = _val;
}

func_entry::func_entry(const string &sysy_name, data_t _ret_type)
{
	eeyore_name = "f_" + sysy_name;
	ret_type = _ret_type;
}

// initialize the tables
void init_tables()
{
	var_stack.clear();
	func_table.clear();
	var_stack.push_back(var_table_t());

	// register system functions
	reg_func("getint", INT);

	reg_func("getch", INT);

	reg_func("getarray", INT);
	auto temp = var_entry(true, false, true, vector<int>(1, 0));
	temp.sysy_name = "p0";
	func_table["getarray"].params.push_back(temp);

	reg_func("putint", VOID);
	temp = var_entry(false, false, true, vector<int>());
	func_table["putint"].params.push_back(temp);

	reg_func("putchar", VOID);
	temp = var_entry(false, false, true, vector<int>());
	func_table["putchar"].params.push_back(temp);

	reg_func("putarray", VOID);
	temp = var_entry(false, false, true, vector<int>());
	func_table["putarray"].params.push_back(temp);
	temp = var_entry(true, false, true, vector<int>(1, 0));
	func_table["putarray"].params.push_back(temp);

	reg_func("_sysy_starttime", VOID);
	temp = var_entry(false, false, true, vector<int>());
	func_table["_sysy_starttime"].params.push_back(temp);

	reg_func("_sysy_stoptime", VOID);
	temp = var_entry(false, false, true, vector<int>());
	func_table["_sysy_stoptime"].params.push_back(temp);
}

// check if the variable is defined in current block
bool defined(const string &name)
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
	if (blk_id == 0)
		func_name = "";
	var_stack.pop_back();
}

// register a variable into the variable table
void reg_var(const string &name, bool is_const, bool is_array, bool is_param,
	const vector<int> &dim, const vector<int> &val)
{
	assert(!name.empty());
	if (defined(name))
	{
		string msg = "identifier '" + name + "' was already defined";
		yyerror(msg.c_str());
	}
	var_entry new_entry(is_array, is_const, is_param, dim, val, name);
	if (is_param)
		new_entry.eeyore_name = "p" + to_string(param_id++);
	else if (name[0] != '#')
		new_entry.eeyore_name = "T" + to_string(var_id++);
	else
		new_entry.eeyore_name = name.substr(1);
	if (blk_id) // defined in the function!
	{
		assert(!func_name.empty());
		func_table[func_name].func_var_table.push_back(new_entry);
		if (is_param) // a parameter!
			func_table[func_name].params.push_back(new_entry);
	}
	var_stack[blk_id][name] = new_entry;
	return;
}

// find if variable exists and store the item
bool find_var(const string &name, var_entry &store)
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
bool find_var(const string &name)
{
	for (int i = blk_id; i >= 0; i--)
		if (var_stack[i].count(name))
			return true;
	return false;
}

// register a function into function table
void reg_func(const char *_name, data_t ret_type)
{
	dbg_printf("%s return int: %d\n", _name, ret_type == INT);
	string name(_name);
	assert(!name.empty());
	if (defined(name))
	{
		string msg = "identifier '" + name + "' was already defined";
		yyerror(msg.c_str());
		return;
	}
	func_table[name] = func_entry(name, ret_type);
	func_name = name;
}

// find if function exists and store the item
bool find_func(const string &name, func_entry &store)
{
	if (func_table.count(name) == 0)
		return false;
	store = func_table[name];
	return true;
}

// find if function exists
bool find_func(const string &name)
{
	return func_table.count(name) != 0;
}

// check whether the main function is valid
bool check_main()
{
	bool valid = true;
	func_entry query;
	if (!find_func("main", query))
	{
		yyerror("function 'main' should be defined");
		valid = false;
	}
	else if (query.ret_type != INT)
	{
		yyerror("function 'main' should return 'int'");
		valid = false;
	}
	else if (query.params.size() != 0)
	{
		yyerror("function 'main' should not have parameter");
		valid = false;
	}
	return valid;
}