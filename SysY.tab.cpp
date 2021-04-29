/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     CONST = 258,
     IF = 259,
     ELSE = 260,
     WHILE = 261,
     CONTINUE = 262,
     BREAK = 263,
     RETURN = 264,
     ID = 265,
     NUM = 266,
     T_GE = 267,
     T_LE = 268,
     T_EQ = 269,
     T_NE = 270,
     DTYPE = 271,
     LOWER_THAN_ELSE = 272
   };
#endif
/* Tokens.  */
#define CONST 258
#define IF 259
#define ELSE 260
#define WHILE 261
#define CONTINUE 262
#define BREAK 263
#define RETURN 264
#define ID 265
#define NUM 266
#define T_GE 267
#define T_LE 268
#define T_EQ 269
#define T_NE 270
#define DTYPE 271
#define LOWER_THAN_ELSE 272




/* Copy the first part of user declarations.  */
#line 2 "SysY.y"

#include "node.hpp"
#include "symtab.hpp"
#ifdef DEBUG
#define dbg_printf(...) do { printf(__VA_ARGS__); fflush(stdout);} while(0);
#else
#define dbg_printf(...)
#endif
int yyparse(void);
int yylex(void);

int yywrap()
{
    return 1;
}


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 157 "SysY.tab.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  17
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   284

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  126
/* YYNRULES -- Number of states.  */
#define YYNSTATES  216

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   272

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    29,     2,     2,     2,    32,    35,     2,
      25,    26,    30,    27,    19,    28,     2,    31,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    18,
      33,    20,    34,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    21,     2,    22,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    23,    36,    24,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    12,    15,    17,    19,
      24,    29,    34,    37,    42,    44,    48,    53,    56,    61,
      62,    67,    72,    74,    78,    81,    85,    87,    89,    93,
      97,   100,   104,   108,   110,   114,   119,   122,   124,   128,
     131,   135,   138,   140,   144,   152,   159,   163,   165,   166,
     172,   175,   182,   188,   190,   195,   198,   199,   200,   201,
     207,   212,   215,   216,   218,   220,   225,   228,   230,   232,
     239,   248,   255,   258,   261,   265,   268,   272,   277,   280,
     287,   294,   301,   308,   311,   314,   315,   316,   318,   319,
     322,   325,   330,   331,   336,   340,   342,   344,   348,   350,
     355,   358,   361,   364,   369,   373,   375,   376,   378,   382,
     386,   390,   392,   396,   400,   402,   406,   410,   414,   418,
     420,   424,   428,   430,   434,   436,   440
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      38,     0,    -1,    39,    -1,    40,    -1,    52,    -1,    39,
      40,    -1,    39,    52,    -1,    41,    -1,    47,    -1,     3,
      16,    42,    18,    -1,     3,     1,    42,    18,    -1,     3,
      16,    42,     1,    -1,     3,    18,    -1,     3,    16,     1,
      18,    -1,    43,    -1,    43,    19,    42,    -1,    10,    44,
      20,    45,    -1,    10,    44,    -1,    21,    78,    22,    44,
      -1,    -1,    21,    78,     1,    44,    -1,    21,     1,    22,
      44,    -1,    78,    -1,    23,    46,    24,    -1,    23,    24,
      -1,    23,    46,     1,    -1,     1,    -1,    45,    -1,    45,
      19,    46,    -1,    16,    48,    18,    -1,    16,    18,    -1,
      16,    48,     1,    -1,    16,     1,    18,    -1,    49,    -1,
      49,    19,    48,    -1,    10,    44,    20,    50,    -1,    10,
      44,    -1,    64,    -1,    23,    51,    24,    -1,    23,    24,
      -1,    23,    51,     1,    -1,    23,     1,    -1,    50,    -1,
      50,    19,    51,    -1,    16,    10,    25,    55,    53,    26,
      57,    -1,    16,    10,    25,    55,    53,     1,    -1,    54,
      19,    53,    -1,    54,    -1,    -1,    16,    10,    21,    22,
      44,    -1,    16,    10,    -1,    16,    10,    21,    78,    22,
      44,    -1,    16,    10,    21,     1,    44,    -1,    10,    -1,
      10,    21,    22,    44,    -1,    16,     1,    -1,    -1,    -1,
      -1,    23,    56,    59,    58,    24,    -1,    23,    56,    59,
       1,    -1,    60,    59,    -1,    -1,    40,    -1,    61,    -1,
      67,    20,    64,    18,    -1,    64,    18,    -1,    18,    -1,
      57,    -1,     4,    25,    63,    65,    26,    61,    -1,     4,
      25,    63,    65,    26,    61,     5,    61,    -1,     6,    25,
      62,    65,    26,    61,    -1,     8,    18,    -1,     7,    18,
      -1,     9,    64,    18,    -1,     9,    18,    -1,    67,    20,
       1,    -1,    67,    20,    64,     1,    -1,    64,     1,    -1,
       4,    25,    63,     1,    26,    61,    -1,     4,    25,    63,
      65,     1,    61,    -1,     6,    25,    62,     1,    26,    61,
      -1,     6,    25,    62,    65,     1,    61,    -1,     8,     1,
      -1,     7,     1,    -1,    -1,    -1,    73,    -1,    -1,    66,
      77,    -1,    10,    68,    -1,    21,    64,    22,    68,    -1,
      -1,    21,    64,     1,    68,    -1,    25,    64,    26,    -1,
      67,    -1,    11,    -1,    25,    64,     1,    -1,    69,    -1,
      10,    25,    71,    26,    -1,    27,    70,    -1,    28,    70,
      -1,    29,    70,    -1,    10,    25,    71,     1,    -1,    64,
      19,    71,    -1,    64,    -1,    -1,    70,    -1,    72,    30,
      70,    -1,    72,    31,    70,    -1,    72,    32,    70,    -1,
      72,    -1,    73,    27,    72,    -1,    73,    28,    72,    -1,
      73,    -1,    74,    33,    73,    -1,    74,    34,    73,    -1,
      74,    12,    73,    -1,    74,    13,    73,    -1,    74,    -1,
      75,    14,    74,    -1,    75,    15,    74,    -1,    75,    -1,
      76,    35,    75,    -1,    76,    -1,    77,    36,    76,    -1,
      73,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    36,    36,    39,    40,    41,    42,    44,    45,    46,
      49,    50,    51,    52,    54,    55,    57,    58,    61,    62,
      63,    64,    66,    67,    68,    69,    70,    72,    73,    75,
      78,    79,    80,    82,    83,    85,    86,    88,    89,    90,
      91,    92,    94,    95,    98,    99,   101,   102,   103,   105,
     107,   109,   110,   111,   112,   113,   114,   115,   117,   117,
     118,   120,   121,   123,   124,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   149,   150,   150,
     151,   153,   154,   155,   157,   158,   159,   160,   162,   163,
     164,   165,   166,   167,   169,   170,   171,   173,   174,   175,
     176,   178,   179,   180,   182,   183,   184,   185,   186,   188,
     189,   190,   192,   195,   197,   200,   201
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CONST", "IF", "ELSE", "WHILE",
  "CONTINUE", "BREAK", "RETURN", "ID", "NUM", "T_GE", "T_LE", "T_EQ",
  "T_NE", "DTYPE", "LOWER_THAN_ELSE", "';'", "','", "'='", "'['", "']'",
  "'{'", "'}'", "'('", "')'", "'+'", "'-'", "'!'", "'*'", "'/'", "'%'",
  "'<'", "'>'", "'&'", "'|'", "$accept", "Code", "CompUnit", "Decl",
  "ConstDecl", "ConstDefs", "ConstDef", "ConstArray", "ConstInitVal",
  "ConstInitVals", "VarDecl", "VarDefs", "VarDef", "InitVal", "InitVals",
  "FuncDef", "FuncFParams", "FuncFParam", "Minc_func", "Minc", "Block",
  "@1", "BlockItems", "BlockItem", "Stmt", "M_while", "M_if", "Exp",
  "Cond", "@2", "LVal", "Array", "PrimaryExp", "UnaryExp", "FuncRParams",
  "MulExp", "AddExp", "RelExp", "EqExp", "LAndExp", "LOrExp", "ConstExp", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,    59,    44,
      61,    91,    93,   123,   125,    40,    41,    43,    45,    33,
      42,    47,    37,    60,    62,    38,   124
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    37,    38,    39,    39,    39,    39,    40,    40,    41,
      41,    41,    41,    41,    42,    42,    43,    43,    44,    44,
      44,    44,    45,    45,    45,    45,    45,    46,    46,    47,
      47,    47,    47,    48,    48,    49,    49,    50,    50,    50,
      50,    50,    51,    51,    52,    52,    53,    53,    53,    54,
      54,    54,    54,    54,    54,    54,    55,    56,    58,    57,
      57,    59,    59,    60,    60,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    62,    63,    64,    66,    65,
      67,    68,    68,    68,    69,    69,    69,    69,    70,    70,
      70,    70,    70,    70,    71,    71,    71,    72,    72,    72,
      72,    73,    73,    73,    74,    74,    74,    74,    74,    75,
      75,    75,    76,    76,    77,    77,    78
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     2,     2,     1,     1,     4,
       4,     4,     2,     4,     1,     3,     4,     2,     4,     0,
       4,     4,     1,     3,     2,     3,     1,     1,     3,     3,
       2,     3,     3,     1,     3,     4,     2,     1,     3,     2,
       3,     2,     1,     3,     7,     6,     3,     1,     0,     5,
       2,     6,     5,     1,     4,     2,     0,     0,     0,     5,
       4,     2,     0,     1,     1,     4,     2,     1,     1,     6,
       8,     6,     2,     2,     3,     2,     3,     4,     2,     6,
       6,     6,     6,     2,     2,     0,     0,     1,     0,     2,
       2,     4,     0,     4,     3,     1,     1,     3,     1,     4,
       2,     2,     2,     4,     3,     1,     0,     1,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     2,     3,     7,     8,     4,     0,
       0,    12,     0,    19,    30,     0,    33,     1,     5,     6,
      19,     0,    14,     0,     0,    32,     0,    56,    36,    31,
      29,     0,    17,    10,     0,    13,    11,     9,     0,    92,
      96,     0,     0,     0,     0,    95,    98,   107,   111,   126,
       0,    48,     0,    19,    34,     0,    15,    19,     0,   106,
      90,     0,    87,   100,   101,   102,     0,     0,     0,     0,
       0,    19,    19,    53,     0,     0,    47,     0,    35,    37,
      26,     0,    16,    22,    21,     0,   105,     0,    97,    94,
     108,   109,   110,   112,   113,    20,    18,     0,    55,    50,
      45,     0,    48,    41,    39,    42,     0,    24,    27,     0,
      92,    92,   106,   103,    99,    19,     0,    57,    44,    46,
       0,    40,    38,     0,    25,    23,    93,    91,   104,    54,
      19,    19,     0,    62,    43,    28,    52,    49,    19,     0,
       0,     0,     0,     0,     0,    67,    63,    68,     0,    62,
      64,     0,    95,    51,    86,    85,    84,    73,    83,    72,
      75,     0,    60,     0,    61,    78,    66,     0,     0,     0,
      74,    59,    76,     0,     0,     0,     0,     0,     0,    77,
      65,     0,     0,     0,   114,   119,   122,   124,    89,     0,
       0,     0,    79,    80,    69,     0,     0,     0,     0,     0,
       0,     0,     0,    81,    82,    71,     0,   117,   118,   115,
     116,   120,   121,   123,   125,    70
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,   146,     6,    21,    22,    28,   108,   109,
       7,    15,    16,   105,   106,     8,    75,    76,    51,   133,
     147,   163,   148,   149,   150,   169,   168,   151,   175,   176,
      45,    60,    46,    47,    87,    48,    62,   185,   186,   187,
     188,    83
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -130
static const yytype_int16 yypact[] =
{
      60,   178,   141,     8,    60,  -130,  -130,  -130,  -130,     4,
      97,  -130,     3,    46,  -130,    77,    32,  -130,  -130,  -130,
      43,   134,    56,   136,   146,  -130,   103,  -130,    79,  -130,
    -130,    91,   148,  -130,     4,  -130,  -130,  -130,   169,    81,
    -130,   138,   138,   138,   138,  -130,  -130,  -130,    90,   113,
      57,    -7,   220,    43,  -130,     5,  -130,    43,   138,   138,
    -130,     9,   113,  -130,  -130,  -130,   138,   138,   138,   138,
     138,    43,    43,   177,   116,    10,   176,    16,  -130,  -130,
    -130,    58,  -130,  -130,  -130,    69,   207,    12,  -130,  -130,
    -130,  -130,  -130,    90,    90,  -130,  -130,   222,  -130,   221,
    -130,   229,    -7,  -130,  -130,   235,    64,  -130,   239,    65,
     238,   238,   138,  -130,  -130,    43,    83,  -130,  -130,  -130,
     220,  -130,  -130,     5,  -130,  -130,  -130,  -130,  -130,  -130,
      43,    43,   240,   174,  -130,  -130,  -130,  -130,    43,   236,
     241,   187,   199,   228,   214,  -130,  -130,  -130,    72,   174,
    -130,   215,   243,  -130,  -130,  -130,  -130,  -130,  -130,  -130,
    -130,   242,  -130,   244,  -130,  -130,  -130,   108,   128,   133,
    -130,  -130,  -130,   218,   245,    24,   138,   246,    48,  -130,
    -130,   200,   200,   200,   113,   112,   172,   230,   231,   200,
     200,   200,  -130,  -130,   259,   138,   138,   138,   138,   138,
     138,   138,   138,  -130,  -130,  -130,   200,   113,   113,   113,
     113,   112,   112,   172,   230,  -130
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -130,  -130,  -130,   189,  -130,    -3,  -130,   -15,   219,   147,
    -130,   247,  -130,   217,   153,   271,   175,  -130,  -130,  -130,
     179,  -130,   127,  -130,    31,  -130,  -130,   -40,   110,  -130,
    -129,   124,  -130,   -20,   170,   171,   -26,    51,    80,    82,
    -130,   -24
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -89
static const yytype_int16 yytable[] =
{
      49,    61,    50,    73,   152,    32,    80,    24,    17,    74,
      88,   100,    79,   113,    20,    39,    40,   103,    85,    86,
     152,    25,    63,    64,    65,   182,    39,    40,    81,    49,
      41,    56,    42,    43,    44,    89,   101,    79,   114,    77,
     104,    41,    84,    42,    43,    44,    90,    91,    92,   190,
     183,    31,   152,   152,   152,    49,    95,    96,    71,    80,
     152,   152,   152,     1,    26,   121,   124,    26,    39,    40,
     110,    27,    86,   162,   191,    34,     2,   152,    29,    72,
      79,    81,   107,    41,   130,    42,    43,    44,   122,   125,
      49,   111,   132,    39,    40,    30,   -58,    49,    23,    52,
     129,    53,    58,   161,    38,   131,    59,    20,    41,   172,
      42,    43,    44,    39,    40,   136,   137,    98,    39,    40,
      66,    67,    68,   153,   195,   196,    99,   173,    41,   174,
      42,    43,    44,    41,   177,    42,    43,    44,   -88,   -88,
      69,    70,    12,   -88,   -88,   197,   198,    36,    39,    40,
     184,    13,    33,   -88,    35,   -88,   -88,   -88,   -88,    14,
     -88,   -88,   -88,    41,    37,    42,    43,    44,    55,   207,
     208,   209,   210,   184,   184,   184,   184,     1,   139,     9,
     140,   141,   142,   143,    39,    40,   199,   200,   156,     5,
     144,    57,   145,    18,    10,   102,    11,   117,    97,    41,
     158,    42,    43,    44,   139,   157,   140,   141,   142,   143,
      39,    40,   192,   193,   194,    12,   165,   159,   145,   179,
     203,   204,   205,   117,    53,    41,   112,    42,    43,    44,
      39,    40,    14,   166,   126,   127,   180,   215,    39,    40,
      93,    94,   116,    77,   115,    41,   160,    42,    43,    44,
     211,   212,   117,    41,   120,    42,    43,    44,   123,    58,
     170,   154,   138,   167,   206,   201,   155,   202,   171,    78,
     135,   181,   189,   134,    82,    19,   164,   119,    54,   178,
     118,   213,   128,     0,   214
};

static const yytype_int16 yycheck[] =
{
      26,    41,    26,    10,   133,    20,     1,    10,     0,    16,
       1,     1,    52,     1,    10,    10,    11,     1,    58,    59,
     149,    18,    42,    43,    44,     1,    10,    11,    23,    55,
      25,    34,    27,    28,    29,    26,    26,    77,    26,    23,
      24,    25,    57,    27,    28,    29,    66,    67,    68,     1,
      26,    19,   181,   182,   183,    81,    71,    72,     1,     1,
     189,   190,   191,     3,    21,     1,     1,    21,    10,    11,
       1,    25,   112,     1,    26,    19,    16,   206,     1,    22,
     120,    23,    24,    25,     1,    27,    28,    29,    24,    24,
     116,    22,   116,    10,    11,    18,    24,   123,     1,    20,
     115,    10,    21,   143,     1,    22,    25,    10,    25,     1,
      27,    28,    29,    10,    11,   130,   131,     1,    10,    11,
      30,    31,    32,   138,    12,    13,    10,   167,    25,     1,
      27,    28,    29,    25,     1,    27,    28,    29,    10,    11,
      27,    28,     1,    10,    11,    33,    34,     1,    10,    11,
     176,    10,    18,    25,    18,    27,    28,    29,    25,    18,
      27,    28,    29,    25,    18,    27,    28,    29,    20,   195,
     196,   197,   198,   199,   200,   201,   202,     3,     4,     1,
       6,     7,     8,     9,    10,    11,    14,    15,     1,     0,
      16,    22,    18,     4,    16,    19,    18,    23,    21,    25,
       1,    27,    28,    29,     4,    18,     6,     7,     8,     9,
      10,    11,   181,   182,   183,     1,     1,    18,    18,     1,
     189,   190,   191,    23,    10,    25,    19,    27,    28,    29,
      10,    11,    18,    18,   110,   111,    18,   206,    10,    11,
      69,    70,    21,    23,    22,    25,    18,    27,    28,    29,
     199,   200,    23,    25,    19,    27,    28,    29,    19,    21,
      18,    25,    22,    20,     5,    35,    25,    36,    24,    52,
     123,    26,    26,   120,    55,     4,   149,   102,    31,   169,
     101,   201,   112,    -1,   202
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    16,    38,    39,    40,    41,    47,    52,     1,
      16,    18,     1,    10,    18,    48,    49,     0,    40,    52,
      10,    42,    43,     1,    42,    18,    21,    25,    44,     1,
      18,    19,    44,    18,    19,    18,     1,    18,     1,    10,
      11,    25,    27,    28,    29,    67,    69,    70,    72,    73,
      78,    55,    20,    10,    48,    20,    42,    22,    21,    25,
      68,    64,    73,    70,    70,    70,    30,    31,    32,    27,
      28,     1,    22,    10,    16,    53,    54,    23,    50,    64,
       1,    23,    45,    78,    44,    64,    64,    71,     1,    26,
      70,    70,    70,    72,    72,    44,    44,    21,     1,    10,
       1,    26,    19,     1,    24,    50,    51,    24,    45,    46,
       1,    22,    19,     1,    26,    22,    21,    23,    57,    53,
      19,     1,    24,    19,     1,    24,    68,    68,    71,    44,
       1,    22,    78,    56,    51,    46,    44,    44,    22,     4,
       6,     7,     8,     9,    16,    18,    40,    57,    59,    60,
      61,    64,    67,    44,    25,    25,     1,    18,     1,    18,
      18,    64,     1,    58,    59,     1,    18,    20,    63,    62,
      18,    24,     1,    64,     1,    65,    66,     1,    65,     1,
      18,    26,     1,    26,    73,    74,    75,    76,    77,    26,
       1,    26,    61,    61,    61,    12,    13,    33,    34,    14,
      15,    35,    36,    61,    61,    61,     5,    73,    73,    73,
      73,    74,    74,    75,    76,    61
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 36 "SysY.y"
    { root = (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 3:
#line 39 "SysY.y"
    { (yyval.node) = new node_basic(); (yyval.node)->child = (yyvsp[(1) - (1)].vardef); (yyval.node)->code = (yyvsp[(1) - (1)].vardef)->code; ;}
    break;

  case 4:
#line 40 "SysY.y"
    { (yyval.node) = (yyvsp[(1) - (1)].funcdef); ;}
    break;

  case 5:
#line 41 "SysY.y"
    { (yyvsp[(1) - (2)].node)->set_next((yyvsp[(2) - (2)].vardef)); (yyval.node) = (yyvsp[(1) - (2)].node); ;}
    break;

  case 6:
#line 42 "SysY.y"
    { (yyvsp[(1) - (2)].node)->set_next((yyvsp[(2) - (2)].funcdef)); (yyval.node) = (yyvsp[(1) - (2)].node); ;}
    break;

  case 7:
#line 44 "SysY.y"
    { (yyval.vardef) = (yyvsp[(1) - (1)].vardef); ;}
    break;

  case 8:
#line 45 "SysY.y"
    { (yyval.vardef) = (yyvsp[(1) - (1)].vardef); ;}
    break;

  case 9:
#line 46 "SysY.y"
    { if((yyvsp[(2) - (4)].dtype) != INT) yyerror("variable type must be int");
                                         (yyval.vardef) = (yyvsp[(3) - (4)].vardef);
                                        ;}
    break;

  case 10:
#line 49 "SysY.y"
    { yyerror("missing type in declaration"); ;}
    break;

  case 11:
#line 50 "SysY.y"
    { yyerror("expected ';'"); ;}
    break;

  case 12:
#line 51 "SysY.y"
    { yyerror("nothing declared"); ;}
    break;

  case 13:
#line 52 "SysY.y"
    { yyerror("invalid symbols among declarations");;}
    break;

  case 14:
#line 54 "SysY.y"
    { (yyval.vardef) = (yyvsp[(1) - (1)].vardef); ;}
    break;

  case 15:
#line 55 "SysY.y"
    { (yyvsp[(1) - (3)].vardef)->set_next((yyvsp[(3) - (3)].vardef)); (yyval.vardef) = (yyvsp[(1) - (3)].vardef); ;}
    break;

  case 16:
#line 57 "SysY.y"
    { (yyval.vardef) = new vardef_node((yyvsp[(1) - (4)].name), true, false, false, (yyvsp[(2) - (4)].exp_basic), (yyvsp[(4) - (4)].exp_basic)->child); ;}
    break;

  case 17:
#line 58 "SysY.y"
    { yyerror("constant expression must be initialized.");
                       (yyval.vardef) = new vardef_node((yyvsp[(1) - (2)].name), true, false, false, (yyvsp[(2) - (2)].exp_basic), nullptr); ;}
    break;

  case 18:
#line 61 "SysY.y"
    { (yyvsp[(2) - (4)].exp_basic)->set_next((yyvsp[(4) - (4)].exp_basic)); (yyval.exp_basic) = (yyvsp[(2) - (4)].exp_basic); ;}
    break;

  case 19:
#line 62 "SysY.y"
    { (yyval.exp_basic) = nullptr; ;}
    break;

  case 20:
#line 63 "SysY.y"
    { yyerror("expected ']'"); ;}
    break;

  case 21:
#line 64 "SysY.y"
    { yyerror("array size in delaration must be constant"); ;}
    break;

  case 22:
#line 66 "SysY.y"
    { (yyval.exp_basic) = new exp_node(EXP_INITVAL, "", 0, NONE, (yyvsp[(1) - (1)].exp_basic)); ;}
    break;

  case 23:
#line 67 "SysY.y"
    { (yyval.exp_basic) = new exp_node(EXP_INITVAL, "", 0, NONE, (yyvsp[(2) - (3)].exp_basic)); ;}
    break;

  case 24:
#line 68 "SysY.y"
    { (yyval.exp_basic) = new exp_node(EXP_INITVAL); ;}
    break;

  case 25:
#line 69 "SysY.y"
    { yyerror("expected '}'"); ;}
    break;

  case 26:
#line 70 "SysY.y"
    { yyerror("expected constant expression"); ;}
    break;

  case 27:
#line 72 "SysY.y"
    { (yyval.exp_basic) = (yyvsp[(1) - (1)].exp_basic); ;}
    break;

  case 28:
#line 73 "SysY.y"
    { (yyvsp[(1) - (3)].exp_basic)->set_next((yyvsp[(3) - (3)].exp_basic)); (yyval.exp_basic) = (yyvsp[(1) - (3)].exp_basic); ;}
    break;

  case 29:
#line 75 "SysY.y"
    { if((yyvsp[(1) - (3)].dtype) != INT) yyerror("variable type must be int");
                                 (yyval.vardef) = (yyvsp[(2) - (3)].vardef);
                               ;}
    break;

  case 30:
#line 78 "SysY.y"
    { yyerror("nothing declared"); ;}
    break;

  case 31:
#line 79 "SysY.y"
    { yyerror("expected ';'"); ;}
    break;

  case 32:
#line 80 "SysY.y"
    { yyerror("invalid symbols among declarations"); yyerrok; ;}
    break;

  case 33:
#line 82 "SysY.y"
    { (yyval.vardef) = (yyvsp[(1) - (1)].vardef); ;}
    break;

  case 34:
#line 83 "SysY.y"
    { (yyvsp[(1) - (3)].vardef)->set_next((yyvsp[(3) - (3)].vardef)); (yyval.vardef) = (yyvsp[(1) - (3)].vardef); ;}
    break;

  case 35:
#line 85 "SysY.y"
    { (yyval.vardef)=new vardef_node((yyvsp[(1) - (4)].name), false, false, false, (yyvsp[(2) - (4)].exp_basic), (yyvsp[(4) - (4)].exp_basic)->child); ;}
    break;

  case 36:
#line 86 "SysY.y"
    { (yyval.vardef) = new vardef_node((yyvsp[(1) - (2)].name), false, false, false, (yyvsp[(2) - (2)].exp_basic), nullptr); ;}
    break;

  case 37:
#line 88 "SysY.y"
    { (yyval.exp_basic) = new exp_node(EXP_INITVAL, "", 0, NONE, (yyvsp[(1) - (1)].exp_basic)); ;}
    break;

  case 38:
#line 89 "SysY.y"
    { (yyval.exp_basic) = new exp_node(EXP_INITVAL, "", 0, NONE, (yyvsp[(2) - (3)].exp_basic)); ;}
    break;

  case 39:
#line 90 "SysY.y"
    { (yyval.exp_basic) = new exp_node(EXP_INITVAL); ;}
    break;

  case 40:
#line 91 "SysY.y"
    { yyerror("expected '}'"); ;}
    break;

  case 41:
#line 92 "SysY.y"
    { yyerror("expected '}'"); ;}
    break;

  case 42:
#line 94 "SysY.y"
    { (yyval.exp_basic) = (yyvsp[(1) - (1)].exp_basic); ;}
    break;

  case 43:
#line 95 "SysY.y"
    { (yyvsp[(1) - (3)].exp_basic)->set_next((yyvsp[(3) - (3)].exp_basic)); (yyval.exp_basic) = (yyvsp[(1) - (3)].exp_basic); ;}
    break;

  case 44:
#line 98 "SysY.y"
    { (yyval.funcdef) = new funcdef_node((yyvsp[(2) - (7)].name), (yyvsp[(1) - (7)].dtype), (yyvsp[(7) - (7)].node), (yyvsp[(5) - (7)].vardef)); ;}
    break;

  case 45:
#line 99 "SysY.y"
    { yyerror("expected ')'"); ;}
    break;

  case 46:
#line 101 "SysY.y"
    { (yyvsp[(1) - (3)].vardef)->set_next((yyvsp[(3) - (3)].vardef)); (yyval.vardef) = (yyvsp[(1) - (3)].vardef); ;}
    break;

  case 47:
#line 102 "SysY.y"
    { (yyval.vardef) = (yyvsp[(1) - (1)].vardef); ;}
    break;

  case 48:
#line 103 "SysY.y"
    { (yyval.vardef) = nullptr; ;}
    break;

  case 49:
#line 105 "SysY.y"
    { if((yyvsp[(1) - (5)].dtype) != INT) yyerror("variable type must be int"); 
                                      (yyval.vardef) = new vardef_node((yyvsp[(2) - (5)].name), false, true, true, (yyvsp[(5) - (5)].exp_basic)); ;}
    break;

  case 50:
#line 107 "SysY.y"
    { if((yyvsp[(1) - (2)].dtype) != INT) yyerror("variable type must be int"); 
                  (yyval.vardef) = new vardef_node((yyvsp[(2) - (2)].name), false, false, true); ;}
    break;

  case 51:
#line 109 "SysY.y"
    { yyerror("the first dimension should be empty"); ;}
    break;

  case 52:
#line 110 "SysY.y"
    { yyerror("expected ']'"); ;}
    break;

  case 53:
#line 111 "SysY.y"
    { yyerror("missing type of parameter"); ;}
    break;

  case 54:
#line 112 "SysY.y"
    { yyerror("missing type of parameter"); ;}
    break;

  case 55:
#line 113 "SysY.y"
    { yyerror("missing identifier of parameter"); ;}
    break;

  case 56:
#line 114 "SysY.y"
    { reg_func((yyvsp[(-1) - (0)].name), (yyvsp[(-2) - (0)].dtype)); inc_blk(); create = false; ;}
    break;

  case 57:
#line 115 "SysY.y"
    { inc_blk(); ;}
    break;

  case 58:
#line 117 "SysY.y"
    { dec_blk(); ;}
    break;

  case 59:
#line 117 "SysY.y"
    { (yyval.node) = (yyvsp[(3) - (5)].node); ;}
    break;

  case 60:
#line 118 "SysY.y"
    { dec_blk(); yyerror("expected '}'"); ;}
    break;

  case 61:
#line 120 "SysY.y"
    { (yyvsp[(1) - (2)].node)->set_next((yyvsp[(2) - (2)].node)); (yyval.node) = (yyvsp[(1) - (2)].node); ;}
    break;

  case 62:
#line 121 "SysY.y"
    { (yyval.node) = nullptr; ;}
    break;

  case 63:
#line 123 "SysY.y"
    { (yyval.node) = (yyvsp[(1) - (1)].vardef); ;}
    break;

  case 64:
#line 124 "SysY.y"
    { (yyval.node) = (yyvsp[(1) - (1)].stmt); ;}
    break;

  case 65:
#line 126 "SysY.y"
    { (yyval.stmt) = new assign_stmt_node((yyvsp[(1) - (4)].exp_basic),(yyvsp[(3) - (4)].exp_basic)); ;}
    break;

  case 66:
#line 127 "SysY.y"
    { (yyval.stmt) = new exp_stmt_node((yyvsp[(1) - (2)].exp_basic)); ;}
    break;

  case 67:
#line 128 "SysY.y"
    { (yyval.stmt) = new stmt_node(); ;}
    break;

  case 68:
#line 129 "SysY.y"
    { (yyval.stmt) = new stmt_node(); (yyval.stmt)->child = (yyvsp[(1) - (1)].node); ;}
    break;

  case 69:
#line 130 "SysY.y"
    { (yyval.stmt) = (yyvsp[(3) - (6)].stmt); (yyval.stmt)->update((yyvsp[(4) - (6)].cond_exp), (yyvsp[(6) - (6)].stmt), nullptr); ;}
    break;

  case 70:
#line 131 "SysY.y"
    { (yyval.stmt) = (yyvsp[(3) - (8)].stmt); (yyval.stmt)->update((yyvsp[(4) - (8)].cond_exp), (yyvsp[(6) - (8)].stmt), (yyvsp[(8) - (8)].stmt)); ;}
    break;

  case 71:
#line 132 "SysY.y"
    { (yyval.stmt) = (yyvsp[(3) - (6)].stmt); (yyval.stmt)->update((yyvsp[(4) - (6)].cond_exp), (yyvsp[(6) - (6)].stmt), nullptr); ;}
    break;

  case 72:
#line 133 "SysY.y"
    { (yyval.stmt) = new goto_stmt_node(true); ;}
    break;

  case 73:
#line 134 "SysY.y"
    { (yyval.stmt) = new goto_stmt_node(false); ;}
    break;

  case 74:
#line 135 "SysY.y"
    { (yyval.stmt) = new ret_stmt_node((yyvsp[(2) - (3)].exp_basic)); ;}
    break;

  case 75:
#line 136 "SysY.y"
    { (yyval.stmt) = new ret_stmt_node(nullptr); ;}
    break;

  case 76:
#line 137 "SysY.y"
    { yyerror("expected expression"); ;}
    break;

  case 77:
#line 138 "SysY.y"
    { yyerror("expected ';'"); ;}
    break;

  case 78:
#line 139 "SysY.y"
    { yyerror("expected ';'"); ;}
    break;

  case 79:
#line 140 "SysY.y"
    { yyerror("expected condition"); ;}
    break;

  case 80:
#line 141 "SysY.y"
    { yyerror("expected ')'"); ;}
    break;

  case 81:
#line 142 "SysY.y"
    { yyerror("expected condition"); ;}
    break;

  case 82:
#line 143 "SysY.y"
    { yyerror("expected ')"); ;}
    break;

  case 83:
#line 144 "SysY.y"
    { yyerror("expected ';'"); ;}
    break;

  case 84:
#line 145 "SysY.y"
    { yyerror("expected ';'"); ;}
    break;

  case 85:
#line 146 "SysY.y"
    { (yyval.stmt)= new while_stmt_node(); ;}
    break;

  case 86:
#line 147 "SysY.y"
    { (yyval.stmt)= new if_stmt_node(); ;}
    break;

  case 87:
#line 149 "SysY.y"
    { (yyval.exp_basic) = (yyvsp[(1) - (1)].exp_basic); ;}
    break;

  case 88:
#line 150 "SysY.y"
    { is_cond = true; ;}
    break;

  case 89:
#line 150 "SysY.y"
    { is_cond = false; ;}
    break;

  case 90:
#line 151 "SysY.y"
    { (yyval.exp_basic) = new array_exp_node((yyvsp[(2) - (2)].exp_basic), (yyvsp[(1) - (2)].name)); ;}
    break;

  case 91:
#line 153 "SysY.y"
    { (yyvsp[(2) - (4)].exp_basic)->set_next((yyvsp[(4) - (4)].exp_basic)); (yyval.exp_basic) = (yyvsp[(2) - (4)].exp_basic); ;}
    break;

  case 92:
#line 154 "SysY.y"
    { (yyval.exp_basic) = nullptr; ;}
    break;

  case 93:
#line 155 "SysY.y"
    { yyerror("expected ']'"); ;}
    break;

  case 94:
#line 157 "SysY.y"
    { (yyval.exp_basic) = (yyvsp[(2) - (3)].exp_basic); ;}
    break;

  case 95:
#line 158 "SysY.y"
    { (yyval.exp_basic) = (yyvsp[(1) - (1)].exp_basic); ;}
    break;

  case 96:
#line 159 "SysY.y"
    { (yyval.exp_basic)= new exp_node(EXP_NUM, "", (yyvsp[(1) - (1)].num)); ;}
    break;

  case 97:
#line 160 "SysY.y"
    { yyerror("expected ')'"); ;}
    break;

  case 98:
#line 162 "SysY.y"
    { (yyval.exp_basic) = (yyvsp[(1) - (1)].exp_basic); ;}
    break;

  case 99:
#line 163 "SysY.y"
    { (yyval.exp_basic) = new func_call_exp_node((yyvsp[(1) - (4)].name), (yyvsp[(3) - (4)].exp_basic)); ;}
    break;

  case 100:
#line 164 "SysY.y"
    { (yyval.exp_basic) = (yyvsp[(2) - (2)].exp_basic); ;}
    break;

  case 101:
#line 165 "SysY.y"
    { (yyval.exp_basic) = new arith_exp_node(NEGATE, (yyvsp[(2) - (2)].exp_basic)); ;}
    break;

  case 102:
#line 166 "SysY.y"
    { (yyval.exp_basic) = new arith_exp_node(NOT, (yyvsp[(2) - (2)].exp_basic)); ;}
    break;

  case 103:
#line 167 "SysY.y"
    { yyerror("expected ')'"); ;}
    break;

  case 104:
#line 169 "SysY.y"
    { (yyvsp[(1) - (3)].exp_basic)->set_next((yyvsp[(3) - (3)].exp_basic)); (yyval.exp_basic) = (yyvsp[(1) - (3)].exp_basic); ;}
    break;

  case 105:
#line 170 "SysY.y"
    { (yyval.exp_basic) = (yyvsp[(1) - (1)].exp_basic); ;}
    break;

  case 106:
#line 171 "SysY.y"
    { (yyval.exp_basic) = nullptr; ;}
    break;

  case 107:
#line 173 "SysY.y"
    { (yyval.exp_basic) = (yyvsp[(1) - (1)].exp_basic); ;}
    break;

  case 108:
#line 174 "SysY.y"
    { (yyval.exp_basic) = new arith_exp_node(MUL, (yyvsp[(1) - (3)].exp_basic), (yyvsp[(3) - (3)].exp_basic)); ;}
    break;

  case 109:
#line 175 "SysY.y"
    { (yyval.exp_basic) = new arith_exp_node(DIV, (yyvsp[(1) - (3)].exp_basic), (yyvsp[(3) - (3)].exp_basic)); ;}
    break;

  case 110:
#line 176 "SysY.y"
    { (yyval.exp_basic) = new arith_exp_node(MOD, (yyvsp[(1) - (3)].exp_basic), (yyvsp[(3) - (3)].exp_basic)); ;}
    break;

  case 111:
#line 178 "SysY.y"
    { (yyval.exp_basic) = (yyvsp[(1) - (1)].exp_basic); ;}
    break;

  case 112:
#line 179 "SysY.y"
    { (yyval.exp_basic) = new arith_exp_node(ADD, (yyvsp[(1) - (3)].exp_basic), (yyvsp[(3) - (3)].exp_basic)); ;}
    break;

  case 113:
#line 180 "SysY.y"
    { (yyval.exp_basic) = new arith_exp_node(SUB, (yyvsp[(1) - (3)].exp_basic), (yyvsp[(3) - (3)].exp_basic)); ;}
    break;

  case 114:
#line 182 "SysY.y"
    { (yyval.exp_basic) = (yyvsp[(1) - (1)].exp_basic); ;}
    break;

  case 115:
#line 183 "SysY.y"
    { (yyval.exp_basic) = new arith_exp_node(L, (yyvsp[(1) - (3)].exp_basic), (yyvsp[(3) - (3)].exp_basic)); ;}
    break;

  case 116:
#line 184 "SysY.y"
    { (yyval.exp_basic) = new arith_exp_node(G, (yyvsp[(1) - (3)].exp_basic), (yyvsp[(3) - (3)].exp_basic)); ;}
    break;

  case 117:
#line 185 "SysY.y"
    { (yyval.exp_basic) = new arith_exp_node(GE, (yyvsp[(1) - (3)].exp_basic), (yyvsp[(3) - (3)].exp_basic)); ;}
    break;

  case 118:
#line 186 "SysY.y"
    { (yyval.exp_basic) = new arith_exp_node(LE, (yyvsp[(1) - (3)].exp_basic), (yyvsp[(3) - (3)].exp_basic)); ;}
    break;

  case 119:
#line 188 "SysY.y"
    { (yyval.exp_basic) = (yyvsp[(1) - (1)].exp_basic); ;}
    break;

  case 120:
#line 189 "SysY.y"
    { (yyval.exp_basic) = new arith_exp_node(EQ, (yyvsp[(1) - (3)].exp_basic), (yyvsp[(3) - (3)].exp_basic)); ;}
    break;

  case 121:
#line 190 "SysY.y"
    { (yyval.exp_basic) = new arith_exp_node(NE, (yyvsp[(1) - (3)].exp_basic), (yyvsp[(3) - (3)].exp_basic)); ;}
    break;

  case 122:
#line 192 "SysY.y"
    { if( (yyvsp[(1) - (1)].exp_basic)->exp_type == EXP_PTR) 
                      yyerror("pointer should not be in expression");
                  (yyval.cond_exp) = new cond_exp_node(NONE, (yyvsp[(1) - (1)].exp_basic), nullptr); ;}
    break;

  case 123:
#line 195 "SysY.y"
    { (yyval.cond_exp) = new cond_exp_node(AND, (yyvsp[(1) - (3)].cond_exp), (yyvsp[(3) - (3)].exp_basic)); ;}
    break;

  case 124:
#line 197 "SysY.y"
    { if( (yyvsp[(1) - (1)].cond_exp)->exp_type == EXP_PTR) 
                      yyerror("pointer should not be in expression");
                  (yyval.cond_exp) = (yyvsp[(1) - (1)].cond_exp); ;}
    break;

  case 125:
#line 200 "SysY.y"
    { (yyval.cond_exp) = new cond_exp_node(OR, (yyvsp[(1) - (3)].cond_exp), (yyvsp[(3) - (3)].cond_exp)); ;}
    break;

  case 126:
#line 201 "SysY.y"
    { (yyvsp[(1) - (1)].exp_basic)->reduce();
                      if((yyvsp[(1) - (1)].exp_basic)->exp_type != EXP_NUM)
                          yyerror("expression should be constant");
                      (yyval.exp_basic) = (yyvsp[(1) - (1)].exp_basic); ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2208 "SysY.tab.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 205 "SysY.y"

