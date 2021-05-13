/* emit risc-v by direct mapping tigger to risc-v */
#ifndef __EMIT_RISCV_H__
#define __EMIT_RISCV_H__
#include "tigger_seq.hpp"
#include <memory>
#include <sstream>
#include <string>
using std::string;
extern std::ostringstream riscv_dst;
extern vector<shared_ptr<tigger::stmt>> tigger::seq;

namespace riscv {
void gen_code();
void emit_func_begin(const shared_ptr<tigger::func_begin> &stmt);
void emit_func_end(const shared_ptr<tigger::func_end> &stmt);
void emit_global_var(const shared_ptr<tigger::global_var> &stmt);
void emit_assign(const shared_ptr<tigger::assign_stmt> &stmt);
void emit_goto(const shared_ptr<tigger::goto_stmt> &stmt);
void emit_label(const shared_ptr<tigger::label_stmt> &stmt);
void emit_call(const shared_ptr<tigger::call_stmt> &stmt);
void emit_ret(const shared_ptr<tigger::ret_stmt> &stmt);
void emit_store(const shared_ptr<tigger::store_stmt> &stmt);
void emit_load(const shared_ptr<tigger::load_stmt> &stmt);
void emit_loadaddr(const shared_ptr<tigger::loadaddr_stmt> &stmt);
} // namespace riscv
#endif // __EMIT_RISCV_H__