/* build the AST for eeyore */
#include "eeyore_AST.hpp"
#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <queue>
#include <sstream>
using std::getline;
using std::make_shared;
using std::queue;

namespace eeyore_AST {

unordered_map<string, func_entry> func_table; // function table
vector<string> funcnames; // function names

static unordered_map<int, shared_ptr<stmt_node>>
	label2stmt; // label to statment
static vector<shared_ptr<goto_node>> gotos; // goto statements
static vector<int> labels; // unregistered labels
static queue<shared_ptr<op_node>> params; // unregistered parameters
static string now_func; // now function name
static std::istringstream eeyore_f; // eeyore string file

static void reg_func(const string &eeyore_name, int param_n);
static void reg_var(const string &eeyore_name, int size);
static void simple_label();
static shared_ptr<op_node> parse_var_num(const string &s);
static shared_ptr<op_node> parseop(const string &line);
static bool parseline();

// register a new function
static void reg_func(const string &eeyore_name, int param_n)
{
	funcnames.push_back(eeyore_name);
	func_table[eeyore_name] = func_entry(eeyore_name, param_n);
}

// register a variable
static void reg_var(const string &eeyore_name, int size)
{
	if (size == -1) // variable
		func_table[now_func].temps[eeyore_name] = var_entry(eeyore_name);
	else // array
		func_table[now_func].temps[eeyore_name] =
			var_entry(eeyore_name, size, true);
}

// simplify the label table
void simple_label()
{
	for (auto i : gotos)
		i->goto_label = label2stmt[i->goto_label]->label;
	gotos.clear();
	label2stmt.clear();
}

// parse variable and number
static shared_ptr<op_node> parse_var_num(const string &s)
{
	std::istringstream s_f(s);
	if (!isalpha(s[0]))
	{
		int val;
		s_f >> val;
		return make_shared<num_node>(val);
	}
	return make_shared<var_node>(s);
}

// parse the operation expression
static shared_ptr<op_node> parseop(const string &line)
{
	std::istringstream line_f(line);
	shared_ptr<op_node> ret;
	switch (line[0])
	{
	// function call
	case 'c':
	{
		string temp, func_name;
		line_f >> temp >> func_name;
		auto func_call = make_shared<call_node>(func_name);
		while (!params.empty())
		{
			func_call->params.push_back(params.front());
			params.pop();
		}
		ret = func_call;
		break;
	}
	// unary operator
	case '-':
	case '!':
	{
		char ch;
		string subline;
		line_f >> ch;
		auto op = line.substr(0, 1);
		// number
		if (isdigit(line_f.peek()))
		{
			int num;
			line_f >> num;
			num = op == "!" ? !num : -num;
			ret = make_shared<num_node>(num);
			break;
		}
		// variable
		line_f >> subline;
		auto var = make_shared<var_node>(subline);
		ret = make_shared<op_node>(EXP_OP, op, var);
		break;
	}
	// binary, number, or variable
	default:
	{
		auto first_sq = line.find('[');
		auto last_sq = line.find(']');
		if (first_sq != std::string::npos) // array
		{
			auto left = line.substr(0, first_sq);
			auto right = line.substr(first_sq + 1, last_sq - first_sq - 1);
			ret = make_shared<op_node>(
				EXP_ARRAY, "m", parse_var_num(left), parse_var_num(right));
			break;
		}
		string left, op, right;
		line_f >> left;
		if (line_f.peek() == EOF) // a number of variable
			ret = parse_var_num(left);
		else // a binary operator
		{
			line_f >> op >> right;
			ret = make_shared<op_node>(
				EXP_OP, op, parse_var_num(left), parse_var_num(right));
			printf("left: %s, right: %s\n", left.c_str(), right.c_str());
		}
		break;
	}
	} // switch
	return ret;
}

// parse a line and generate AST node
static bool parseline()
{
	string line;
	getline(eeyore_f, line);
	std::istringstream line_f(line);
	if (eeyore_f.peek() == EOF)
		return false;
	if (line.empty())
		return true;
	switch (line[0])
	{
	// function definition
	case 'f':
	{
		int param_n;
		char temp;
		line_f >> now_func >> temp >> param_n;
		reg_func(now_func, param_n);
		break;
	}
	// function end
	case 'e':
		simple_label();
		now_func = "";
		break;
	// variable
	case 'v':
	{
		string temp, var;
		int size = -1;
		line_f >> temp;
		line_f.get();
		if (isdigit(line_f.peek()))
			line_f >> size >> var;
		else
			line_f >> var;
		reg_var(var, size);
		break;
	}
	// label
	case 'l':
	{
		int label;
		char ch;
		line_f >> ch >> label;
		dbg_printf("label %d\n", label);
		labels.push_back(label);
		break;
	}
	// an inner statement
	case '\t':
	{
		line_f.get();
		line = line.substr(1, line.length() - 1);
		shared_ptr<stmt_node> stmt;
		switch (line[0])
		{
		// goto statement
		case 'g':
		{
			string temp1;
			char temp2;
			int label;
			line_f >> temp1 >> temp2 >> label;
			stmt = make_shared<goto_node>(label);
			dbg_printf("goto: %d\n", label);
			gotos.push_back(std::static_pointer_cast<goto_node>(stmt));
			break;
		}
		// variable
		case 'v':
		{
			string temp, name;
			int size = -1;
			line_f >> temp;
			line_f.get();
			if (isdigit(line_f.peek()))
				line_f >> size >> name;
			else
				line_f >> name;
			reg_var(name, size);
			break;
		}
		// parameter or assignment
		case 'p':
		{
			// parameter
			if (line[1] == 'a')
			{
				string temp, param;
				line_f >> temp >> param;
				params.push(parseop(param));
				break;
			}
			// assignment
			string lval, rval;
			char eq;
			line_f >> lval >> eq;
			line_f.get();
			getline(line_f, rval);
			auto lval_pt = parseop(lval);
			auto rval_pt = parseop(rval);
			stmt = make_shared<assign_node>(lval_pt, rval_pt);
			break;
		}
		// return
		case 'r':
		{
			string temp, ret_val;
			line_f >> temp;
			if (line_f.peek() == EOF)
				stmt = make_shared<ret_node>();
			else
			{
				line_f >> ret_val;
				stmt = make_shared<ret_node>(parseop(ret_val));
			}
			break;
		}
		// function call
		case 'c':
			stmt = parseop(line);
			stmt->stmt_type = STMT_CALL;
			break;
		// conditional jump
		case 'i':
		{
			string cond, temp;
			int label;
			char tempc;
			line_f >> temp >> cond;
			line_f >> temp;
			cond += " " + temp;
			line_f >> temp;
			cond += " " + temp;
			dbg_printf("cond on %s ", cond.c_str());
			line_f >> temp >> tempc >> label;
			auto cond_pt = parseop(cond);
			dbg_printf("if goto: %d\n", label);
			stmt = make_shared<goto_node>(label, cond_pt);
			gotos.push_back(std::static_pointer_cast<goto_node>(stmt));
			break;
		}
		// assignment
		case 't':
		case 'T':
		{
			string lval, rval;
			char eq;
			line_f >> lval >> eq;
			line_f.get();
			getline(line_f, rval);
			auto lval_pt = parseop(lval);
			auto rval_pt = parseop(rval);
			stmt = make_shared<assign_node>(lval_pt, rval_pt);
			break;
		}
		default: break;
		} // switch
		if (stmt) // push the statement
		{
			func_table[now_func].funcbody.push_back(stmt);
			if (!labels.empty()) // assign the labels
			{
				stmt->label = *std::min_element(
					labels.begin(), labels.end(), std::greater<int>());
				dbg_printf("now stmt label: %d\n", stmt->label);
				for (auto i : labels)
					label2stmt[i] = stmt;
				labels.clear();
			}
		}
	} // case '\t'
	default: break;
	} // switch
	return true;
}

// judge whether the variable is global
bool is_global(const string &name)
{
	return func_table[""].temps.count(name) != 0;
}

// judge whether the variable is a parameter
bool is_param(const string &name)
{
	return name[0] == 'p';
}

// build the AST for eeyore
void build_AST(const string &eeyore_code)
{
	eeyore_f = std::istringstream(eeyore_code);
	now_func = "";
	func_table.clear();
	label2stmt.clear();
	labels.clear();
	while (!params.empty())
		params.pop();
	// initialize global var_table
	func_table[""] = func_entry("", 0);
	// parseline!
	while (parseline())
		;
	dbg_printf("\n");
}
} // namespace eeyore_AST