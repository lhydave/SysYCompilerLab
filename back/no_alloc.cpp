/* giving the pure stack strategy for back-end code */
#include "eeyore_AST.hpp"
#include "emit_tigger.hpp"
#include "tigger_gen.hpp"
using std::make_shared;
using std::string;
using std::to_string;
using namespace eeyore_AST;
using namespace tigger;

namespace no_alloc {
const size_t int_size = 4; // size of an int
const string global_prefix = "v"; // global prefix
const string param_prefix = "a"; // parameter prefix
const string eeyore_param_prefix = "p"; // parameter prefix in eeyore
const string ret_reg = "a0"; // return register
const string zero_reg = "x0"; // zero register
const string temp_prefix = "t"; // temporary register

func_entry now_func; // now function
// map eeyore name to global variable number
std::unordered_map<string, int> global_var2no;
// map eeyore variables and param registers to the stack number
std::unordered_map<string, int> var2no;

void gen_code();
size_t stack_size(const func_entry &func);
void gen_func(const func_entry &func);
void gen_stmt(const shared_ptr<stmt_node> &stmt);
void gen_goto(const shared_ptr<goto_node> &stmt);
void gen_ret(const shared_ptr<ret_node> &stmt);
void gen_call(const shared_ptr<call_node> &stmt, const string &ret_store = "");
void gen_assign(
	const shared_ptr<op_node> &lval, const shared_ptr<op_node> &rval);
void gen_assign(const string &lval, const shared_ptr<op_node> &rval);
void gen_array(const shared_ptr<op_node> &temp, int tempid1, int tempid2);
void gen_unary(const shared_ptr<op_node> &temp, int tempid);
void gen_binary(const shared_ptr<op_node> &temp, int tempid1, int tempid2);
string alloc_temp_reg(const shared_ptr<op_node> &temp, int tempid);
void dealloc_temp_reg(const shared_ptr<op_node> &temp, int tempid);

// generate the code of tigger
void gen_code()
{
	// for global variables
	auto globals = func_table[""].temps;
	int global_cnt = 0;
	for (auto i : globals)
	{
		auto j = i.second;
		global_var2no[j.eeyore_name] = global_cnt;
		if (j.is_array)
			tigger_dst << emit_global_var(global_cnt, 0, j.size, true);
		else
			tigger_dst << emit_global_var(global_cnt);
		global_cnt++;
		dbg_printf("global var %s init\n", j.eeyore_name.c_str());
	}
	if (!globals.empty())
		tigger_dst << "\n";
	// generate functions
	for (auto i : funcnames)
		gen_func(func_table[i]);
}

// calculate the stack space needed for function
size_t stack_size(const func_entry &func)
{
	size_t size = func.param_n;
	for (auto i : func.temps)
	{

		if (i.second.is_array)
			size += i.second.size / int_size;
		else
			size++;
	}
	return size;
}

// generate the tigger code for the function
void gen_func(const func_entry &func)
{
	auto stk_size = stack_size(func);
	now_func = func;
	var2no.clear();
	tigger_dst << emit_func_begin(func.eeyore_name, func.param_n, stk_size);
	dbg_printf("func %s begin, param: %d, size: %lu\n",
		func.eeyore_name.c_str(), func.param_n, stk_size);
	// initialize mappings
	int posi = 0;
	for (auto i : func.temps)
	{
		var2no[i.second.eeyore_name] = posi;
		dbg_printf(
			"temp var %s mapped to %d\n", i.second.eeyore_name.c_str(), posi);
		if (i.second.is_array)
			posi += i.second.size / int_size;
		else
			posi++;
	}
	for (auto i = 0; i < func.param_n; i++)
	{
		var2no[eeyore_param_prefix + to_string(i)] = posi;
		tigger_dst << emit_store(param_prefix + to_string(i), posi);
		posi++;
	}
	for (auto i : func.funcbody)
		gen_stmt(i);
	tigger_dst << emit_func_end(func.eeyore_name);
	dbg_printf("func %s end\n", func.eeyore_name.c_str());
}

// generate the tigger code for different statements
void gen_stmt(const shared_ptr<stmt_node> &stmt)
{
	if (stmt->label != -1)
		tigger_dst << emit_label(stmt->label);
	switch (stmt->stmt_type)
	{
	case STMT_ASSIGN:
	{
		dbg_printf("assign stmt gen\n");
		auto stmt_new = std::static_pointer_cast<assign_node>(stmt);
		gen_assign(stmt_new->lval, stmt_new->rval);
		break;
	}
	case STMT_GOTO:
	{
		dbg_printf("goto stmt gen\n");
		auto stmt_new = std::static_pointer_cast<goto_node>(stmt);
		gen_goto(stmt_new);
		break;
	}
	case STMT_RET:
	{
		dbg_printf("ret stmt gen\n");
		auto stmt_new = std::static_pointer_cast<ret_node>(stmt);
		gen_ret(stmt_new);
		break;
	}
	case STMT_CALL:
	{
		dbg_printf("call stmt gen\n");
		auto stmt_new = std::static_pointer_cast<call_node>(stmt);
		gen_call(stmt_new);
		break;
	}
	}
}

// generate tigger code for goto statement
void gen_goto(const shared_ptr<goto_node> &stmt)
{
	if (stmt->cond) // conditional jump
	{
		gen_assign(temp_prefix + to_string(1), stmt->cond->left);
		if (std::static_pointer_cast<num_node>(stmt->cond->right)->val)
			tigger_dst << emit_goto(stmt->goto_label,
				temp_prefix + to_string(1) + " != " + zero_reg);
		else
			tigger_dst << emit_goto(stmt->goto_label,
				temp_prefix + to_string(1) + " == " + zero_reg);
	}
	else // unconditional jump
		tigger_dst << emit_goto(stmt->goto_label);
}

// generate tigger code for return
void gen_ret(const shared_ptr<ret_node> &stmt)
{
	if (stmt->ret_val)
	{
		auto reg = alloc_temp_reg(stmt->ret_val, 1);
		tigger_dst << emit_exp_assign(ret_reg, reg);
	}
	tigger_dst << emit_return();
}

// generate tigger code for function call
void gen_call(const shared_ptr<call_node> &stmt, const string &ret_store)
{
	// set parameters
	for (auto i = 0; i < stmt->params.size(); i++)
		gen_assign(param_prefix + to_string(i), stmt->params[i]);
	// call it!
	tigger_dst << emit_call(stmt->func_name);
	if (!ret_store.empty()) // assign
		tigger_dst << emit_exp_assign(ret_store, ret_reg);
}

// generate tigger code of assign
void gen_assign(
	const shared_ptr<op_node> &lval, const shared_ptr<op_node> &rval)
{
	if (lval->exp_type == EXP_ARRAY) // store
	{
		gen_array(lval, 1, 2);
		string left = temp_prefix + to_string(1) + "[0]";
		auto right = alloc_temp_reg(rval, 2);
		tigger_dst << emit_exp_assign(left, right);
		return;
	}
	auto var = std::static_pointer_cast<var_node>(lval);
	string left = temp_prefix + to_string(1);
	gen_assign(left, rval);
	dealloc_temp_reg(lval, 1);
}

// generate the tigger code for assign
void gen_assign(const string &lval, const shared_ptr<op_node> &rval)
{
	if (rval->exp_type == EXP_NUM) // number!
	{
		auto rnum = std::static_pointer_cast<num_node>(rval);
		tigger_dst << emit_exp_assign(lval, to_string(rnum->val));
		return;
	}
	if (rval->exp_type == EXP_VAR) // variable!
	{
		auto rreg = alloc_temp_reg(rval, 2);
		tigger_dst << emit_exp_assign(lval, rreg);
		return;
	}
	if (rval->exp_type == EXP_ARRAY) // array!
	{
		gen_array(rval, 2, 3);
		string right = temp_prefix + to_string(2) + "[0]";
		tigger_dst << emit_exp_assign(lval, right);
		return;
	}
	if (rval->exp_type == EXP_CALL) // call!
	{
		gen_call(std::static_pointer_cast<call_node>(rval), lval);
		return;
	}
	if (!rval->right) // unary operator
	{
		gen_unary(rval, 2);
		tigger_dst << emit_exp_assign(lval, temp_prefix + to_string(2));
		return;
	}
	// binary operator
	gen_binary(rval, 2, 3);
	tigger_dst << emit_exp_assign(lval, temp_prefix + to_string(2));
}

// generate tigger code for array
void gen_array(const shared_ptr<op_node> &temp, int tempid1, int tempid2)
{
	dbg_printf("gen array begin...\n");
	auto base = std::static_pointer_cast<var_node>(temp->left);
	auto temp1 = temp_prefix + to_string(tempid1);
	auto temp2 = temp_prefix + to_string(tempid2);
	if (is_global(base->eeyore_name)) // global
		tigger_dst << emit_loadaddr(
			global_prefix + to_string(global_var2no[base->eeyore_name]), temp1);
	else if (!is_param(base->eeyore_name) &&
		now_func.temps[base->eeyore_name].is_array) // array
		tigger_dst << emit_loadaddr(var2no[base->eeyore_name], temp1);
	else // variable or param
		tigger_dst << emit_load(var2no[base->eeyore_name], temp1);
	auto shift = alloc_temp_reg(temp->right, tempid2);
	tigger_dst << emit_exp_assign(temp1, temp1 + " + " + temp2);
}

// generate tigger code unary operator
void gen_unary(const shared_ptr<op_node> &temp, int tempid)
{
	auto reg = alloc_temp_reg(temp->left, tempid);
	tigger_dst << emit_exp_assign(reg, temp->op + reg);
}

// generate tigger code binary operator
void gen_binary(const shared_ptr<op_node> &temp, int tempid1, int tempid2)
{
	auto reg1 = alloc_temp_reg(temp->left, tempid1);
	auto reg2 = alloc_temp_reg(temp->right, tempid2);
	tigger_dst << emit_exp_assign(reg1, reg1 + " " + temp->op + " " + reg2);
}

// allocate temp register for variable or number
string alloc_temp_reg(const shared_ptr<op_node> &temp, int tempid)
{
	auto reg = temp_prefix + to_string(tempid);
	if (temp->exp_type == EXP_NUM)
	{
		tigger_dst << emit_exp_assign(
			reg, to_string(std::static_pointer_cast<num_node>(temp)->val));
		return reg;
	}
	auto var = std::static_pointer_cast<var_node>(temp);
	dbg_printf("temp reg alloc for %s\n", var->eeyore_name.c_str());
	if (is_global(var->eeyore_name)) // global
	{
		if (func_table[""].temps[var->eeyore_name].is_array) // pointer
			tigger_dst << emit_loadaddr(
				global_prefix + to_string(global_var2no[var->eeyore_name]),
				reg);
		else // variable
			tigger_dst << emit_load(
				global_prefix + to_string(global_var2no[var->eeyore_name]),
				reg);
	}
	else if (!is_param(var->eeyore_name) &&
		now_func.temps[var->eeyore_name].is_array) // array
		tigger_dst << emit_loadaddr(var2no[var->eeyore_name], reg);
	else // variable or param
		tigger_dst << emit_load(var2no[var->eeyore_name], reg);
	return reg;
}

// deallocate the temporary register
void dealloc_temp_reg(const shared_ptr<op_node> &temp, int tempid)
{
	if (temp->exp_type == EXP_NUM)
		return;
	auto var = std::static_pointer_cast<var_node>(temp);
	auto reg = temp_prefix + to_string(tempid);
	if (is_global(var->eeyore_name)) // global
	{
		tigger_dst << emit_loadaddr(
			global_prefix + to_string(global_var2no[var->eeyore_name]),
			temp_prefix + to_string(6));
		tigger_dst << emit_exp_assign(temp_prefix + to_string(6) + "[0]", reg);
	}
	else // local
		tigger_dst << emit_store(reg, var2no[var->eeyore_name]);
}
} // namespace no_alloc