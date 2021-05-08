/* implement functions in emit_tigger */
#include "emit_tigger.hpp"
using std::to_string;
namespace tigger {

// emit global variable
string emit_global_var(int No, int num, int size, bool is_array)
{
	if (is_array)
		return to_string(No) + " = malloc " + to_string(size) + "\n";
	return to_string(No) + " = " + to_string(num) + "\n";
}

// emit the function beginning
string emit_func_begin(const string &func, int param_n, size_t stack_n)
{
	return func + " [" + to_string(param_n) + "] [" + to_string(stack_n) +
		"]\n";
}

// emit the function end
string emit_func_end(const string &func)
{
	return "end " + func + "\n\n";
}

// emit assignment
string emit_exp_assign(const string &lval, const string &left, const string &op,
	const string &right)
{
	if (op == "!" || op == "-") // unary
		return "\t" + lval + " = " + op + left + "\n";
	else if (op.empty()) // no operation
		return "\t" + lval + " = " + left + "\n";
	else // binary
		return "\t" + lval + " = " + left + " " + op + " " + right + "\n";
}

// emit goto statement
string emit_goto(
	int label, const string &op, const string &left, const string &right)
{
	if (op.empty()) // unconditional jump
		return "\tgoto l" + to_string(label) + "\n";
	// conditional jump
	return "\tif " + left + " " + op + " " + right + " goto l" +
		to_string(label) + "\n";
}

// emit label
string emit_label(int label)
{
	return "l" + to_string(label) + ":\n";
}
// emit function call
string emit_call(const string &func)
{
	return "\tcall " + func + "\n";
}

// emit function return
string emit_return()
{
	return "\treturn\n";
}

// emit store statement
string emit_store(const string &reg, int num)
{
	return "\tstore " + reg + " " + to_string(num) + "\n";
}

// emit load statement
string emit_load(const string &glb_var, const string &reg)
{
	return "\tload " + glb_var + " " + reg + "\n";
}
string emit_load(int num, const string &reg)
{
	return "\tload " + to_string(num) + " " + reg + "\n";
}

// emit load address statement
string emit_loadaddr(const string &glb_var, const string &reg)
{
	return "\tloadaddr " + glb_var + " " + reg + "\n";
}
string emit_loadaddr(int num, const string &reg)
{
	return "\tloadaddr " + to_string(num) + " " + reg + "\n";
}

} // namespace tigger