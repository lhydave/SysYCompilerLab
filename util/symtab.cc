/* utility function for symbol table */
#include "include/node.hh"
#include "include/symtab.hh"
vector<var_table_t> var_stack; // variable table stack
func_table_t func_table; // function table stack
int blk_id; // index of current block