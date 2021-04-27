/* symbol table attributes */
#ifndef __SYMTAB_H__
#define __SYMTAB_H__
#include "include/node.hh"
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
	string sysy_name; // name in sysy
	bool is_array; // true if the symbol is an array
	bool is_const; // true if the symbol is constant
	bool is_param; // true if the symbol is a parameter
	vector<int> dim; // dimension of the array (in int)
	vector<int> val; // const value of the array

	var_entry() = default;
	var_entry(bool _is_array, bool _is_const, bool _is_param,
		const vector<int> &_dim, const vector<int> &_val = vector<int>(),
		const string &_sysy_name = string());
};
// struct var_entry end

// struct func_entry begin
struct func_entry {
	string eeyore_name; // name in eeyore
	vector<var_entry> params; // all parameters in the function
	vector<var_entry> func_var_table; // variable table for the function
	data_t ret_type;

	func_entry() = default;
	func_entry(const string &sysy_name, data_t _ret_type);
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

bool defined(const string &name);
void reg_var(const string &name, bool is_const = false, bool is_array = false,
	bool is_param = false, const vector<int> &dim = vector<int>(),
	const vector<int> &val = vector<int>());
bool find_var(const string &name, var_entry &store);
bool find_var(const string &name);

void reg_func(const char *_name, data_t ret_type);

bool find_func(const string &name, func_entry &store);
bool find_func(const string &name);

#endif // __SYMTAB_H__ end