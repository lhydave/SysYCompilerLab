/* implement functions in emit_tigger */
#include "emit_tigger.hpp"
#include "tigger_seq.hpp"
using std::to_string;
namespace tigger {

// emit global variable
string emit_global_var(int No, int num, int size, bool is_array)
{
	seq.push_back(
		make_shared<global_var>("v" + to_string(No), is_array, size, num));
	if (is_array)
		return "v" + to_string(No) + " = malloc " + to_string(size) + "\n";
	return "v" + to_string(No) + " = " + to_string(num) + "\n";
}

// emit the function beginning
string emit_func_begin(const string &func, int param_n, size_t stack_n)
{
	seq.push_back(make_shared<func_begin>(func, param_n, stack_n));
	return func + " [" + to_string(param_n) + "] [" + to_string(stack_n) +
		"]\n";
}

// emit the function end
string emit_func_end(const string &func)
{
	seq.push_back(make_shared<func_end>(func));
	return "end " + func + "\n\n";
}

// emit assignment
string emit_exp_assign(const string &lval, const string &rval)
{
	seq.push_back(make_shared<assign_stmt>(lval, rval));
	return "\t" + lval + " = " + rval + "\n";
}

// emit goto statement
string emit_goto(int label, const string &cond)
{
	seq.push_back(make_shared<goto_stmt>(cond, label));
	if (cond.empty()) // unconditional jump
		return "\tgoto l" + to_string(label) + "\n";
	// conditional jump
	return "\tif " + cond + " goto l" + to_string(label) + "\n";
}

// emit label
string emit_label(int label)
{
	seq.push_back(make_shared<label_stmt>(label));
	return "l" + to_string(label) + ":\n";
}
// emit function call
string emit_call(const string &func)
{
	seq.push_back(make_shared<call_stmt>(func));
	return "\tcall " + func + "\n";
}

// emit function return
string emit_return()
{
	seq.push_back(make_shared<ret_stmt>());
	return "\treturn\n";
}

// emit store statement
string emit_store(const string &reg, int num)
{
	seq.push_back(make_shared<store_stmt>(reg, num));
	return "\tstore " + reg + " " + to_string(num) + "\n";
}

// emit load statement
string emit_load(const string &glb_var, const string &reg)
{
	seq.push_back(make_shared<load_stmt>(glb_var, reg));
	return "\tload " + glb_var + " " + reg + "\n";
}
string emit_load(int num, const string &reg)
{
	seq.push_back(make_shared<load_stmt>(to_string(num), reg));
	return "\tload " + to_string(num) + " " + reg + "\n";
}

// emit load address statement
string emit_loadaddr(const string &glb_var, const string &reg)
{
	seq.push_back(make_shared<loadaddr_stmt>(glb_var, reg));
	return "\tloadaddr " + glb_var + " " + reg + "\n";
}
string emit_loadaddr(int num, const string &reg)
{
	seq.push_back(make_shared<loadaddr_stmt>(to_string(num), reg));
	return "\tloadaddr " + to_string(num) + " " + reg + "\n";
}

} // namespace tigger