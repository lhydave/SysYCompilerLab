// utility functions for nodes

#include "include/node.hh"
#ifdef DEBUG
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif


static int err_cnt = 0;
static const int err_max = 40;
// rewrite yyerror
void yyerror(const char *msg, int lineno)
{
	++err_cnt;
	if (err_cnt > err_max)
	{
		fprintf(stderr, "Too many errors, aborted.\n");
		exit(1);
	}
	fprintf(stderr, "error happens near line %d: %s\n", lineno, msg);
}


// allocate a new space for string
char *namestr(const char *s)
{
	char *ret = new char[strlen(s) + 1];
	strcpy(ret, s);
	return ret;
}

// covert string to op_t
op_t str2op(const char *s)
{
	switch (s[0])
	{
	case '+': return ADD;
	case '-': return SUB;
	case '*': return MUL;
	case '/': return DIV;
	case '%': return MOD;
	case '&': return AND;
	case '|': return OR;
	case '<':
		if (s[1] == '=')
			return LE;
		return L;
	case '>':
		if (s[1] == '=')
			return GE;
		return G;
	case '!':
		if (s[1] == '=')
			return NE;
		return NOT;
	default: return (op_t)-1;
	}
}

// convert string to data_type
data_t str2dtype(const char *s)
{
	if (!strcmp(s, "int"))
		return INT;
	else if (!strcmp(s, "void"))
		return VOID;
	else
		return (data_t)-1;
}

// convert string to number for 2-16 bases - do not check base
int str2num(const char *s, int base)
{
	int len = strlen(s);
	int val = 0;
	for (int i = 0; i < len; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
			val = val * base + (s[i] - '0');
		else if (s[i] >= 'a' && s[i] <= 'f')
			val = val * base + (s[i] - 'a') + 10;
		else if (s[i] >= 'A' && s[i] < 'F')
			val = val * base + (s[i] - 'A') + 10;
		else
			return -1;
	}
	return val;
}
