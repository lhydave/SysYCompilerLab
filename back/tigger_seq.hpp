/* seq classes for tigger */
#ifndef __TIGGER_SEQ_H__
#define __TIGGER_SEQ_H__
#include <memory>
#include <sstream>
#include <string>
#include <vector>
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::vector;

namespace tigger {
struct stmt;
struct func_begin;
struct func_end;
struct global_var;
struct assign_stmt;
struct goto_stmt;
struct label_stmt;
struct call_stmt;
struct ret_stmt;
struct store_stmt;
struct load_stmt;
struct loadaddr_stmt;

enum stmt_t {
	NONE,
	FUNC_BEGIN,
	FUNC_END,
	GLB_VAR,
	ASSIGN,
	GOTO,
	LABEL,
	CALL,
	RET,
	STORE,
	LOAD,
	LOADADDR
};

vector<shared_ptr<stmt>> seq;

struct stmt {
	stmt_t stmt_type;

	stmt(stmt_t stmt_type) : stmt_type(stmt_type)
	{
	}
};

struct func_begin : public stmt {
	string name;
	int param_n;
	size_t stack_n;

	func_begin(const string &name, int param_n, size_t stack_n) :
		stmt(FUNC_BEGIN), name(name), param_n(param_n), stack_n(stack_n)
	{
	}
};

struct func_end : public stmt {
	string name;

	func_end(const string &name) : stmt(FUNC_END), name(name)
	{
	}
};

struct global_var : public stmt {
	string name;
	bool is_array;
	int size;
	int val;

	global_var(const string &name, bool is_array, int size, int val) :
		stmt(GLB_VAR), name(name), is_array(is_array), size(size), val(val)
	{
	}
};

struct assign_stmt : public stmt {
	string lval, rval;

	assign_stmt(const string &lval, const string &rval) :
		stmt(ASSIGN), lval(lval), rval(rval)
	{
	}
};

struct goto_stmt : public stmt {
	string cond;
	int goto_label;

	goto_stmt(const string &cond, int goto_label) :
		stmt(GOTO), cond(cond), goto_label(goto_label)
	{
	}
};

struct label_stmt : public stmt {
	int label;

	label_stmt(int label) : stmt(LABEL), label(label)
	{
	}
};

struct call_stmt : public stmt {
	string name;

	call_stmt(const string &name) : stmt(CALL), name(name)
	{
	}
};

struct ret_stmt : public stmt {
	ret_stmt() : stmt(RET)
	{
	}
};

struct store_stmt : public stmt {
	string reg;
	int num;

	store_stmt(const string &reg, int num) : stmt(STORE), reg(reg), num(num)
	{
	}
};

struct load_stmt : public stmt {
	string addr, reg;

	load_stmt(const string &addr, const string &reg) :
		stmt(LOAD), addr(addr), reg(reg)
	{
	}
};

struct loadaddr_stmt : public stmt {
	string addr, reg;

	loadaddr_stmt(const string &addr, const string &reg) :
		stmt(LOADADDR), addr(addr), reg(reg)
	{
	}
};

} // namespace tigger
#endif // __TIGGER_SEQ_H__