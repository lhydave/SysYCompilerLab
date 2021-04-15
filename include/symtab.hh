/* symbol table attributes */
#ifndef __SYMTAB_H__
#define __SYMTAB_H__
#include <string>
#include <unordered_map>
#include <vector>
using std::string;
using std::unordered_map;
using std::vector;
struct var_entry;
struct func_entry;
using var_table_t = unordered_map<string, var_entry>;
using func_table_t = unordered_map<string, func_entry>;

// struct var_entry begin
struct var_entry {
	string eeyore_name; // name in eeyore

	bool is_array; // true if the symbol is an array
	bool is_const; // true if the symbol is constant
	bool is_param; // true if the symbol is a parameter
	vector<int> dim; // dimension of the array (in int)
	vector<int> val; // const value of the array

	var_entry() = default;
	var_entry(bool _is_array, bool _is_const, bool _is_param,
		const vector<int> &_dim, const vector<int> &_val = vector<int>());
};
// struct var_entry end

// struct func_entry begin
struct func_entry {
	string eeyore_name; // name in eeyore
	string sysy_name; // name in sysy
	vector<vector<int>> params; // parameter shapes
	vector<var_entry> decls; // all declarations in the function
	var_table_t func_var_table; // variable table for the function
};
// struct func_entry end

extern vector<var_table_t> var_stack;
extern func_table_t func_table;
extern string func_name;
extern int blk_id;
extern bool create;

void inc_blk();
void dec_blk();

void init_tables();

bool defined(string &name);
void reg_var(string name, bool is_const = false, bool is_array = false,
	bool is_param = false, const vector<int> &dim = vector<int>(),
	const vector<int> &val = vector<int>());
bool find_var(string name, var_entry &store);
bool find_var(string name);


void reg_func(
	string name, const vector<vector<int>> &params = vector<vector<int>>());

bool find_func(string name, func_entry &store);
bool find_func(string name);

#endif // __SYMTAB_H__ end