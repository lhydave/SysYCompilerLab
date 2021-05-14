/* implement of header files */
#include "riscv/emit_riscv.hpp"
#include <cctype>
#include <unordered_map>
using std::to_string;
using std::unordered_map;

namespace riscv {

static size_t now_stk;
static const string &temp_reg = "t0";

// generate code of riscv
void gen_code()
{
	for (auto i : tigger::seq)
	{
		switch (i->stmt_type)
		{
		case tigger::FUNC_BEGIN:
		{
			auto stmt = std::static_pointer_cast<tigger::func_begin>(i);
			emit_func_begin(stmt);
			break;
		}
		case tigger::FUNC_END:
		{
			auto stmt = std::static_pointer_cast<tigger::func_end>(i);
			emit_func_end(stmt);
			break;
		}
		case tigger::GLB_VAR:
		{
			auto stmt = std::static_pointer_cast<tigger::global_var>(i);
			emit_global_var(stmt);
			break;
		}
		case tigger::ASSIGN:
		{
			auto stmt = std::static_pointer_cast<tigger::assign_stmt>(i);
			emit_assign(stmt);
			break;
		}
		case tigger::GOTO:
		{
			auto stmt = std::static_pointer_cast<tigger::goto_stmt>(i);
			emit_goto(stmt);
			break;
		}
		case tigger::LABEL:
		{
			auto stmt = std::static_pointer_cast<tigger::label_stmt>(i);
			emit_label(stmt);
			break;
		}
		case tigger::CALL:
		{
			auto stmt = std::static_pointer_cast<tigger::call_stmt>(i);
			emit_call(stmt);
			break;
		}
		case tigger::RET:
		{
			auto stmt = std::static_pointer_cast<tigger::ret_stmt>(i);
			emit_ret(stmt);
			break;
		}
		case tigger::STORE:
		{
			auto stmt = std::static_pointer_cast<tigger::store_stmt>(i);
			emit_store(stmt);
			break;
		}
		case tigger::LOAD:
		{
			auto stmt = std::static_pointer_cast<tigger::load_stmt>(i);
			emit_load(stmt);
			break;
		}
		case tigger::LOADADDR:
		{
			auto stmt = std::static_pointer_cast<tigger::loadaddr_stmt>(i);
			emit_loadaddr(stmt);
			break;
		}
		default: break;
		} // switch
	} // for
} // void gen_code()

////////////////////////////////////////////////////////////////
/////////////  emit each kind of statement /////////////////////
////////////////////////////////////////////////////////////////

void emit_func_begin(const shared_ptr<tigger::func_begin> &stmt)
{
	// code format
	auto f = [](const string &name, int STK) {
		return "\t.text\n\t.align\t2\n\t.global\t" + name + "\n\t.type\t" +
			name + ", @function\n" + name + ":\n" + "\tli\tt0, " +
			to_string(-STK) + "\n\tadd\tsp, sp, t0\n\tli\tt0, " +
			to_string(STK - 4) + "\n\tadd\tt0, t0, sp\n\tsw\tra, 0(t0)\n";
	};
	now_stk = (stmt->stack_n / 4 + 1) * 16;
	// output the code
	riscv_dst << f(stmt->name.substr(2, stmt->name.length() - 2), now_stk);
}

void emit_func_end(const shared_ptr<tigger::func_end> &stmt)
{
	auto name = stmt->name.substr(2, stmt->name.length() - 2);
	riscv_dst << "\t.size\t" << name << ", .-" << name << "\n\n";
	now_stk = 0;
}

void emit_global_var(const shared_ptr<tigger::global_var> &stmt)
{
	if (stmt->is_array) // array
		riscv_dst << "\t.comm " << stmt->name << ", " << stmt->size << ", 4\n";
	else // variable
	{
		auto f = [](const string &name, int val) {
			return "\t.global\t" + name +
				"\n\t.section\t.sdata\n\t.align\t2\n\t.type\t" + name +
				", @object\n\t.size\t" + name + ", 4\n" + name +
				":\n\t.word\t" + to_string(val) + "\n\n";
		};
		riscv_dst << f(stmt->name, stmt->val);
	}
}

inline bool in_int12(int num)
{
	return num <= 2047 && num >= -2048;
}

void emit_assign(const shared_ptr<tigger::assign_stmt> &stmt)
{
	auto lval = stmt->lval, rval = stmt->rval;
	auto parsearray = [](const string &s) {
		std::pair<string, int> ret;
		auto l = s.find('['), r = s.find(']');
		ret.first = s.substr(0, l);
		ret.second = atoi(s.substr(l + 1, r - l - 1).c_str());
		return ret;
	};
	if (lval.find('[') != string::npos) // array store
	{
		auto temp = parsearray(lval);
		if (in_int12(temp.second))
			riscv_dst << "\tsw\t" << rval << ", " << temp.second << "("
					  << temp.first << ")\n";
		else
		{
			riscv_dst << "\tli\tt0, " << temp.second << "\n";
			riscv_dst << "\tadd\tt0, t0, " << temp.second << "\n";
			riscv_dst << "\tsw\t" << rval << ", 0(t0)\n";
		}
		return;
	}
	if (rval.find('[') != string::npos) // array load
	{
		auto temp = parsearray(rval);
		if (in_int12(temp.second))
			riscv_dst << "\tlw\t" << lval << ", " << temp.second << "("
					  << temp.first << ")\n";
		else
		{
			riscv_dst << "\tli\tt0, " << temp.second << "\n";
			riscv_dst << "\tadd\tt0, t0, " << temp.first << "\n";
			riscv_dst << "\tsw\t" << lval << ", 0(t0)\n";
		}
		return;
	}
	// ordinary assign
	if (isdigit(rval[0]) ||
		(rval[0] == '-' && isdigit(rval[1]))) // number assign
	{
		riscv_dst << "\tli\t" << lval << ", " << rval << "\n";
		return;
	}
	if (rval[0] == '-') // negate
	{
		riscv_dst << "\tneg\t" << lval << ", "
				  << rval.substr(1, rval.length() - 1) << "\n";
		return;
	}
	if (rval[0] == '!')
	{
		riscv_dst << "\tseqz\t" << lval << ", "
				  << rval.substr(1, rval.length() - 1) << "\n";
		return;
	}
	std::istringstream rval_f(rval);
	string left, op, right;
	rval_f >> left;
	if (rval_f.peek() == EOF) // single assign
	{
		riscv_dst << "\tmv\t" << lval << ", " << left << "\n";
		return;
	}
	// binary operator
	rval_f >> op >> right;
	static unordered_map<string, string> convert = {{"+", "add"}, {"-", "sub"},
		{"*", "mul"}, {"/", "div"}, {"%", "rem"}, {"<", "slt"}, {">", "sgt"},
		{"<=", "sgt"}, {">=", "slt"}, {"||", "or"}, {"!=", "xor"},
		{"==", "xor"}};
	if (op == "&&")
	{
		riscv_dst << "\tsnez" << lval << ", " << left << "\n\tsnez\tt0, "
				  << right << "\n\tand\t" << lval << ", " << lval << ", t0\n";
		return;
	}
	riscv_dst << "\t" << convert[op] << "\t" << lval << ", " << left << ", "
			  << right << "\n";
	if (op == "<=" || op == ">=" || op == "==")
	{
		riscv_dst << "\tseqz\t" << lval << ", " << lval << "\n";
		return;
	}
	if (op == "||" || op == "!=")
	{
		riscv_dst << "\tsnez\t" << lval << ", " << lval << "\n";
		return;
	}
}

void emit_goto(const shared_ptr<tigger::goto_stmt> &stmt)
{
	static unordered_map<string, string> convert = {{"<", "blt"}, {">", "bgt"},
		{"<=", "ble"}, {">=", "bge"}, {"!=", "bne"}, {"==", "beq"}};
	auto f = [](const string &cond) {
		std::istringstream temp(cond);
		vector<string> ret(3);
		temp >> ret[0] >> ret[1] >> ret[2];
		return ret;
	};
	if (!stmt->cond.empty())
	{
		auto split = f(stmt->cond);
		riscv_dst << "\t" << convert[split[1]] << "\t" << split[0] << ", "
				  << split[2] << ", .l" << stmt->goto_label << "\n";
	}
	else
		riscv_dst << "\tj\t.l" << stmt->goto_label << "\n";
}

void emit_label(const shared_ptr<tigger::label_stmt> &stmt)
{
	riscv_dst << ".l" << stmt->label << ":\n";
}

void emit_call(const shared_ptr<tigger::call_stmt> &stmt)
{
	riscv_dst << "\tcall\t" << stmt->name.substr(2, stmt->name.length() - 2)
			  << "\n";
}

void emit_ret(const shared_ptr<tigger::ret_stmt> &stmt)
{
	riscv_dst << "\tli\tt0, " << now_stk - 4 << "\n";
	riscv_dst << "\tadd\tt0, t0, sp\n\tlw\tra, 0(t0)\n\tli\tt0, " << now_stk
			  << "\n\tadd\tsp, sp, t0\n\tret\n";
}

inline bool in_int10(int num)
{
	return num <= 511 && num >= -512;
}

void emit_store(const shared_ptr<tigger::store_stmt> &stmt)
{
	if (in_int10(stmt->num))
		riscv_dst << "\tsw\t" << stmt->reg << ", " << stmt->num * 4 << "(sp)\n";
	else
	{
		riscv_dst << "\tli\tt0, " << stmt->num * 4 << "\n";
		riscv_dst << "\tsw\t" << stmt->reg << ", t0(sp)\n";
	}
}

void emit_load(const shared_ptr<tigger::load_stmt> &stmt)
{
	if (isdigit(stmt->addr[0])) // stack
	{
		auto num = atoi(stmt->addr.c_str());
		if (in_int10(num))
			riscv_dst << "\tlw\t" << stmt->reg << ", " << num * 4 << "(sp)\n";
		else
		{
			riscv_dst << "\tli\tt0, " << num * 4 << "\n";
			riscv_dst << "\tlw\t" << stmt->reg << ", t0(sp)\n";
		}
	}
	else // global_var
	{
		riscv_dst << "\tlui\t" << stmt->reg << ", %hi(" << stmt->addr << ")\n";
		riscv_dst << "\tlw\t" << stmt->reg << ", %lo(" << stmt->addr << ")("
				  << stmt->reg << ")\n";
	}
}

void emit_loadaddr(const shared_ptr<tigger::loadaddr_stmt> &stmt)
{
	if (isdigit(stmt->addr[0])) // stack
	{
		auto num = atoi(stmt->addr.c_str());
		if (in_int10(num))
			riscv_dst << "\taddi\t" << stmt->reg << ", sp, " << num * 4 << "\n";
		else
		{
			riscv_dst << "\tli\tt0, " << num * 4 << "\n";
			riscv_dst << "\tadd\t" << stmt->reg << ", sp, t0\n";
		}
	}
	else // global_var
		riscv_dst << "\tla\t" << stmt->reg << ", " << stmt->addr << "\n";
}

} // namespace riscv