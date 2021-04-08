// utility functions for attributes

#include "include/attr.h"
#ifdef DEBUG
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

// rewrite yyerror
void yyerror(const char *msg)
{
	fprintf(stderr, "error happens near line %d: %s\n\n", yylineno, msg);
}

// allocate a new space for string
char *namestr(const char *s)
{
	char *ret = new char[strlen(s) + 1];
	strcpy(ret, s);
	return ret;
}

// convert string to data_type
data_type str2dtype(const char *s)
{
	if (!strcmp(s, "int"))
		return INT;
	else if (!strcmp(s, "void"))
		return VOID;
	else
		return (data_type)-1;
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
