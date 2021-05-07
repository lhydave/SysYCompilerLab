/* build the AST for eeyore */
#include "eeyore_AST.hpp"
#include <cctype>
#include <iostream>
#include <queue>
#include <sstream>
using std::getline;
using std::make_shared;
using std::queue;

namespace eeyore_AST {

unordered_map<string, func_entry> func_table; // function table
unordered_map<int, shared_ptr<stmt_node>> label_table; // label table

static queue<int> labels; // unregistered labels
static queue<shared_ptr<op_node>> params; // unregistered parameters
static string now_func; // now function name
static std::istringstream eeyore_f; // eeyore string file

static shared_ptr<op_node> parse_var_num(const string &s);
static shared_ptr<op_node> parseop(const string &line);
static bool parseline();
static void reg_func(const string &eeyore_name, int param_n);
static void reg_var(const string &eeyore_name, int size);

// register a new function
static void reg_func(const string &eeyore_name, int param_n)
{
	func_table[eeyore_name] = func_entry(eeyore_name, param_n);
}

// register a variable
static void reg_var(const string &eeyore_name, int size)
{
	func_table[now_func].temps[eeyore_name] = var_entry(eeyore_name, size);
}

// parse variable and number
static shared_ptr<op_node> parse_var_num(const string &s)
{
	std::istringstream s_f(s);
	if (isdigit(s[0]))
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
	if (line_f.peek() == EOF)
		return false;
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
	case 'e': now_func = ""; break;
	// variable
	case 'v':
	{
		string var;
		int size;
		line_f >> var >> size;
		reg_var(var, size);
		break;
	}
	// label
	case 'l':
	{
		int label;
		char ch;
		line_f >> ch >> label;
		labels.push(label);
		break;
	}
	// an inner statement
	case '\t':
	{
		line_f >> line;
		std::istringstream line_f(line);
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
			break;
		}
		// variable
		case 'v':
		{
			string temp, name;
			int size = 4;
			line_f >> temp;
			line_f.get();
			if (isdigit(line_f.peek()))
				line_f >> size >> name;
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
			line_f >> lval >> eq >> rval;
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
		case 'c': stmt = parseop(line); break;
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
			line_f >> temp >> tempc >> label;
			auto cond_pt = parseop(cond);
			stmt = make_shared<goto_node>(label, cond_pt);
			break;
		}
		// assignment
		case 't':
		case 'T':
		{
			string lval, rval;
			char eq;
			line_f >> lval >> eq >> rval;
			auto lval_pt = parseop(lval);
			auto rval_pt = parseop(rval);
			stmt = make_shared<assign_node>(lval_pt, rval_pt);
			break;
		}
		default: break;
		} // switch
		if (stmt) // assign the labels and push the statement
		{
			func_table[now_func].funcbody.push_back(stmt);
			while (!labels.empty())
			{
				auto label = labels.front();
				labels.pop();
				label_table[label] = stmt;
			}
		}
	} // case '\t'
	default: break;
	} // switch
	return true;
}

// build the AST for eeyore
void build_AST(const string &eeyore_code)
{
	eeyore_f = std::istringstream(eeyore_code);
	now_func = "";
	func_table.clear();
	label_table.clear();
	while (!labels.empty())
		labels.pop();
	while (!params.empty())
		params.pop();
	// initialize global var_table
	func_table[""] = func_entry("", 0);
	// parseline!
	while (parseline())
		;
}
} // namespace eeyore_AST