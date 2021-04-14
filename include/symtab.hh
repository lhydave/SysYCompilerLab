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

// struct var_entry begin
struct var_entry {
	string eeyore_name; // name in eeyore
	string sysy_name; // name in sysy

	bool is_array; // true if the symbol is an array
	bool is_const; // true if the symbol is constant
	vector<int> dim; // dimension of the array (in int)
	vector<int> vals; // value of the array
	int val; // value of the variable

	var_entry() = default;
	size_t size();
};
// struct var_entry end

// struct func_entry begin
struct func_entry {
	string eeyore_name; // name in eeyore
	string sysy_name; // name in sysy
	vector<vector<int>> params; // parameter shapes
	vector<var_entry> decls; // all declarations in the function
};
// struct func_entry end

using var_table_t = unordered_map<string, var_entry>;
using func_table_t = unordered_map<string, func_entry>;

extern vector<var_table_t> var_stack;
extern func_table_t func_table;
extern int blk_id;

bool defined(const char *name);
void insert_var(string name, bool is_const, bool is_array = false,
	vector<int> dim = vector<int>(), vector<int> vals = vector<int>());
void insert_func(string name, vector<vector<int>> params);
#endif // __SYMTAB_H__ end