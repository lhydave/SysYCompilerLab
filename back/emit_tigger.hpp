/* emit tigger statement */
#ifndef __EMIT_TIGGER_H__
#define __EMIT_TIGGER_H__
#include "eeyore_AST.hpp"
#include <sstream>
#include <string>
#include <vector>

using std::string;
namespace tigger {

string emit_global_var(
	int No, int num = 0, int size = 4, bool is_array = false);
string emit_func_begin(const string &func, int param_n, size_t stack_n);
string emit_func_end(const string &func);
string emit_exp_assign(const string &lval, const string &left,
	const string &op = "", const string &right = "");
string emit_goto(int label, const string &op = "", const string &left = "",
	const string &right = "");
string emit_label(int label);
string emit_call(const string &func);
string emit_return();
string emit_store(const string &reg, int num);
string emit_load(const string &glb_var, const string &reg);
string emit_load(int num, const string &reg);
string emit_loadaddr(const string &glb_var, const string &reg);
string emit_loadaddr(int num, const string &reg);

} // namespace tigger
#endif // __EMIT_TIGGER_H__