/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */

#line 67 "parser.tab.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.hpp".  */
#ifndef YY_YY_PARSER_TAB_HPP_INCLUDED
# define YY_YY_PARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 2 "parser.ypp" /* yacc.c:355  */

	#include<string.h> 
	#include "hw3_output.hpp" 
	#include "hw5_common.hpp"
	#include "bp.hpp"
	
	using namespace std;
	using namespace hw5_stuff;
	
	#define YYSTYPE allTypes

#line 109 "parser.tab.cpp" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    VOID = 258,
    INT = 259,
    BYTE = 260,
    BOOL = 261,
    ENUM = 262,
    RETURN = 263,
    IF = 264,
    ELSE = 265,
    WHILE = 266,
    BREAK = 267,
    CONTINUE = 268,
    SC = 269,
    COMMA = 270,
    LBRACE = 271,
    RBRACE = 272,
    ASSIGN = 273,
    OR = 274,
    AND = 275,
    RELOP_LEVEL_8_NEQ = 276,
    RELOP_LEVEL_8_EQ = 277,
    RELOP_LEVEL_9_GE = 278,
    RELOP_LEVEL_9_LE = 279,
    RELOP_LEVEL_9_GT = 280,
    RELOP_LEVEL_9_LT = 281,
    BINOP_LEVEL_11_PLUSE = 282,
    BINOP_LEVEL_11_MINUS = 283,
    BINOP_LEVEL_12_MUL = 284,
    BINOP_LEVEL_12_DIV = 285,
    NOT = 286,
    RPAREN = 287,
    LPAREN = 288,
    NUM = 289,
    STRING = 290,
    ID = 291,
    B = 292,
    TRUE = 293,
    FALSE = 294
  };
#endif

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 167 "parser.tab.cpp" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 15 "parser.ypp" /* yacc.c:359  */
	
	extern int yylex();
	extern char *yytext;
		
	int yyerror(const char * message);
	
	extern int yylineno;

#line 178 "parser.tab.cpp" /* yacc.c:359  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   292

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  90
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  171

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    73,    73,    76,    78,    79,    82,    83,    87,    87,
      87,    88,    89,    92,    93,    97,    98,    99,   102,   107,
     113,   114,   118,   119,   123,   128,   137,   138,   142,   146,
     147,   151,   151,   151,   154,   158,   161,   164,   164,   168,
     168,   171,   171,   175,   178,   181,   185,   186,   188,   189,
     190,   193,   194,   195,   196,   197,   198,   199,   202,   202,
     203,   203,   207,   208,   211,   214,   215,   216,   220,   223,
     227,   230,   233,   236,   239,   242,   248,   251,   254,   257,
     260,   263,   266,   269,   272,   275,   278,   281,   284,   287,
     290
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "VOID", "INT", "BYTE", "BOOL", "ENUM",
  "RETURN", "IF", "ELSE", "WHILE", "BREAK", "CONTINUE", "SC", "COMMA",
  "LBRACE", "RBRACE", "ASSIGN", "OR", "AND", "RELOP_LEVEL_8_NEQ",
  "RELOP_LEVEL_8_EQ", "RELOP_LEVEL_9_GE", "RELOP_LEVEL_9_LE",
  "RELOP_LEVEL_9_GT", "RELOP_LEVEL_9_LT", "BINOP_LEVEL_11_PLUSE",
  "BINOP_LEVEL_11_MINUS", "BINOP_LEVEL_12_MUL", "BINOP_LEVEL_12_DIV",
  "NOT", "RPAREN", "LPAREN", "NUM", "STRING", "ID", "B", "TRUE", "FALSE",
  "$accept", "S", "Program", "StartProgramMarker", "FinishProgramMarker",
  "Funcs", "FuncDecl", "$@1", "$@2", "StartFunctionScopeMarker",
  "EndFunctionScopeMarker", "Enums", "EnumDecl", "EnumTitle",
  "EnumEeclEndMarker", "RetType", "Formals", "FormalsList", "FormalDecl",
  "EnumeratorList", "Enumerator", "Statements", "Statement", "$@3", "$@4",
  "$@5", "$@6", "$@7", "LoopExp", "EnSMarker", "ExSMarker", "EnWMarker",
  "ExWMarker", "BrMarker", "JMarker", "Call", "$@8", "$@9", "ExpList",
  "ExpListElement", "Type", "EnumType", "Exp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294
};
# endif

#define YYPACT_NINF -150

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-150)))

#define YYTABLE_NINF -69

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-69)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -150,     2,  -150,    -2,  -150,   -23,    16,    -2,     9,  -150,
    -150,    16,    78,  -150,   -14,  -150,  -150,  -150,  -150,  -150,
    -150,    -6,  -150,  -150,    88,  -150,  -150,   -14,    32,    25,
    -150,  -150,    85,  -150,    23,    28,  -150,    60,    50,    63,
    -150,  -150,    85,  -150,  -150,    90,  -150,   147,    74,    -7,
      46,    80,    97,   100,   -12,  -150,   114,  -150,    99,   110,
      92,    93,    94,  -150,    62,     6,   101,  -150,   109,  -150,
    -150,  -150,   148,    62,  -150,  -150,  -150,  -150,   111,  -150,
    -150,   147,  -150,    35,    43,  -150,   112,   211,  -150,  -150,
    -150,  -150,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,  -150,   225,  -150,    62,    62,   115,  -150,   128,
    -150,  -150,  -150,  -150,    62,  -150,    62,    62,   254,   254,
     262,   262,   262,   262,    22,    22,  -150,  -150,   116,    62,
     165,   117,   131,   225,  -150,  -150,    62,    62,  -150,   236,
     246,  -150,  -150,  -150,  -150,    62,  -150,   182,   199,   147,
     125,  -150,  -150,  -150,  -150,  -150,  -150,  -150,   151,   147,
    -150,  -150,  -150,  -150,   147,  -150,  -150,  -150,  -150,  -150,
    -150
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     2,    13,     1,     0,    11,    13,     0,    16,
       5,    11,     0,    14,     0,     3,     7,    19,    65,    66,
      67,     0,    18,    28,     0,    26,     8,     0,     0,     0,
      27,    17,    20,    15,     0,     0,    21,    22,     0,     0,
      68,     9,     0,    24,    25,     0,    23,    31,     0,     0,
       0,     0,     0,     0,     0,    36,    31,    29,     0,     0,
       0,     0,    16,    44,     0,     0,    76,    78,    74,    79,
      80,    75,     0,     0,    56,    49,    50,    41,    58,    12,
      30,    31,    43,     0,     0,    81,     0,     0,    77,    45,
      57,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    51,    57,     0,     0,     0,    10,    31,
      34,    37,    35,    39,     0,    69,     0,     0,    85,    84,
      86,    87,    88,    89,    70,    71,    73,    72,     0,     0,
       0,     0,    62,    64,    61,    32,     0,     0,    90,    83,
      82,    52,    57,    42,    59,     0,    33,     0,     0,    31,
       0,    63,    38,    40,    53,    52,    56,    54,    46,    31,
      57,    55,    52,    53,    31,    56,    56,    57,    53,    48,
      47
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -150,  -150,  -150,  -150,  -150,   152,  -150,  -150,  -150,  -150,
    -150,   158,    71,  -150,  -150,  -150,  -150,   124,  -150,  -150,
     153,   118,   -55,  -150,  -150,  -150,  -150,  -150,    52,  -112,
    -149,  -150,  -150,  -118,   -87,   -47,  -150,  -150,    37,  -150,
      -9,   -24,   -29
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     3,    15,    10,    11,    29,    45,    12,
     108,     6,    55,     8,    33,    21,    35,    36,    37,    24,
      25,    56,    57,    58,   146,   136,   137,   105,   102,   149,
     156,   159,   163,   104,   116,    71,   106,   107,   131,   132,
      60,    61,   103
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      59,    80,     4,    22,   117,     5,    77,    63,    39,    59,
      18,    19,    20,     9,   165,   128,    -6,   129,    39,   170,
      72,    78,    23,    38,    64,    14,    65,    66,    67,    68,
      26,    69,    70,    38,    59,    85,    87,    64,   158,    65,
      66,    67,    68,   157,    69,    70,    31,   167,   168,   110,
     164,   100,   101,   111,    80,   150,    86,   112,    32,    40,
      41,   113,    59,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   162,     7,    42,   130,   133,     7,    73,
     169,    17,    18,    19,    20,   138,    43,   139,   140,    18,
      19,    20,    34,    64,   154,    65,    66,    67,    68,    44,
      69,    70,    59,    27,   161,    28,    47,   147,   148,   166,
      62,    75,    59,    74,    76,    81,   133,    59,    18,    19,
      20,    48,    49,    50,    82,    51,    52,    53,    83,    84,
     -68,    79,    18,    19,    20,    48,    49,    50,    88,    51,
      52,    53,    78,   -60,   114,   135,   145,   134,   141,   144,
      54,    18,    19,    20,    48,    49,    50,   155,    51,    52,
      53,   160,    89,    16,    54,    13,    46,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   143,
      30,   142,   151,    54,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   152,     0,     0,   109,
       0,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   153,     0,     0,     0,     0,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,     0,   115,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,    94,    95,    96,
      97,    98,    99,   100,   101,   -69,   -69,   -69,   -69,    98,
      99,   100,   101
};

static const yytype_int16 yycheck[] =
{
      47,    56,     0,    12,    91,     7,    18,    14,    32,    56,
       4,     5,     6,    36,   163,   102,     0,   104,    42,   168,
      49,    33,    36,    32,    31,    16,    33,    34,    35,    36,
      36,    38,    39,    42,    81,    64,    65,    31,   156,    33,
      34,    35,    36,   155,    38,    39,    14,   165,   166,    14,
     162,    29,    30,    18,   109,   142,    65,    14,    33,    36,
      32,    18,   109,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   160,     3,    15,   105,   106,     7,    33,
     167,     3,     4,     5,     6,   114,    36,   116,   117,     4,
       5,     6,     7,    31,   149,    33,    34,    35,    36,    36,
      38,    39,   149,    15,   159,    17,    16,   136,   137,   164,
      36,    14,   159,    33,    14,    16,   145,   164,     4,     5,
       6,     7,     8,     9,    14,    11,    12,    13,    36,    36,
      36,    17,     4,     5,     6,     7,     8,     9,    37,    11,
      12,    13,    33,    32,    32,    17,    15,    32,    32,    32,
      36,     4,     5,     6,     7,     8,     9,    32,    11,    12,
      13,    10,    14,    11,    36,     7,    42,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    14,
      27,   129,   145,    36,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    14,    -1,    -1,    81,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    14,    -1,    -1,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    23,    24,    25,
      26,    27,    28,    29,    30,    23,    24,    25,    26,    27,
      28,    29,    30
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    41,    42,    43,     0,     7,    51,    52,    53,    36,
      45,    46,    49,    51,    16,    44,    45,     3,     4,     5,
       6,    55,    80,    36,    59,    60,    36,    15,    17,    47,
      60,    14,    33,    54,     7,    56,    57,    58,    80,    81,
      36,    32,    15,    36,    36,    48,    57,    16,     7,     8,
       9,    11,    12,    13,    36,    52,    61,    62,    63,    75,
      80,    81,    36,    14,    31,    33,    34,    35,    36,    38,
      39,    75,    82,    33,    33,    14,    14,    18,    33,    17,
      62,    16,    14,    36,    36,    82,    80,    82,    37,    14,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    68,    82,    73,    67,    76,    77,    50,    61,
      14,    18,    14,    18,    32,    32,    74,    74,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    74,    74,
      82,    78,    79,    82,    32,    17,    65,    66,    82,    82,
      82,    32,    68,    14,    32,    15,    64,    82,    82,    69,
      74,    78,    14,    14,    62,    32,    70,    69,    73,    71,
      10,    62,    74,    72,    69,    70,    62,    73,    73,    74,
      70
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    40,    41,    42,    43,    44,    45,    45,    47,    48,
      46,    49,    50,    51,    51,    52,    53,    54,    55,    55,
      56,    56,    57,    57,    58,    58,    59,    59,    60,    61,
      61,    63,    64,    62,    62,    62,    62,    65,    62,    66,
      62,    67,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    68,    69,    70,    71,    72,    73,    74,    76,    75,
      77,    75,    78,    78,    79,    80,    80,    80,    81,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     4,     0,     0,     0,     2,     0,     0,
      12,     0,     0,     0,     2,     6,     2,     0,     1,     1,
       0,     1,     1,     3,     2,     2,     1,     3,     1,     1,
       2,     0,     0,     5,     3,     3,     1,     0,     6,     0,
       6,     0,     5,     2,     2,     3,     9,    15,    14,     2,
       2,     1,     0,     0,     0,     0,     0,     0,     0,     5,
       0,     4,     1,     3,     1,     1,     1,     1,     2,     3,
       3,     3,     3,     3,     1,     1,     1,     2,     1,     1,
       1,     2,     4,     4,     3,     3,     3,     3,     3,     3,
       4
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
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

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 76 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1407 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 78 "parser.ypp" /* yacc.c:1646  */
    { start_program();  }
#line 1413 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 79 "parser.ypp" /* yacc.c:1646  */
    { finish_program(); }
#line 1419 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 82 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1425 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 83 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1431 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 87 "parser.ypp" /* yacc.c:1646  */
    { new_function((yyvsp[0].string), (yyvsp[-1].rettype), yylineno); }
#line 1437 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 87 "parser.ypp" /* yacc.c:1646  */
    { emit_current_function_header(); }
#line 1443 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 87 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1449 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 88 "parser.ypp" /* yacc.c:1646  */
    { enter_function_scope(); }
#line 1455 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 89 "parser.ypp" /* yacc.c:1646  */
    { exit_function_scope();  }
#line 1461 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 92 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1467 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 93 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1473 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 97 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1479 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 98 "parser.ypp" /* yacc.c:1646  */
    { enter_enum_decl_scope((yyvsp[0].string), yylineno); }
#line 1485 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 99 "parser.ypp" /* yacc.c:1646  */
    { exit_enum_decl_scope(); }
#line 1491 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 102 "parser.ypp" /* yacc.c:1646  */
    { 
						if ((yyvsp[0].vartype) == type_int)			(yyval.rettype) = ret_type_int;
						else if ((yyvsp[0].vartype) == type_bool)	(yyval.rettype) = ret_type_bool;
						else 						(yyval.rettype) = ret_type_byte; 
					}
#line 1501 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 107 "parser.ypp" /* yacc.c:1646  */
    { 
													(yyval.rettype) = ret_type_void; 
					}
#line 1509 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 113 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1515 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 114 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1521 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 118 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1527 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 119 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1533 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 123 "parser.ypp" /* yacc.c:1646  */
    { 
								new_input((yyvsp[-1].vartype), (yyvsp[0].string), yylineno);
								(yyval.var).type = (yyvsp[-1].vartype);
								(yyval.var).name = (yyvsp[0].string);
							}
#line 1543 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 128 "parser.ypp" /* yacc.c:1646  */
    { 
								new_input_enum((yyvsp[0].string), (yyvsp[-1].string), yylineno);
								(yyval.var).type = type_enum;
								(yyval.var).name = (yyvsp[0].string);
								(yyval.var).enum_name = (yyvsp[-1].string);
							}
#line 1554 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 137 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1560 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 138 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1566 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 142 "parser.ypp" /* yacc.c:1646  */
    { add_enum_value((yyvsp[0].string), yylineno); }
#line 1572 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 146 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1578 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 147 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1584 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 151 "parser.ypp" /* yacc.c:1646  */
    { enter_scope(); }
#line 1590 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 151 "parser.ypp" /* yacc.c:1646  */
    { exit_scope(); }
#line 1596 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 151 "parser.ypp" /* yacc.c:1646  */
    { 
																				}
#line 1603 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 154 "parser.ypp" /* yacc.c:1646  */
    { 
																					check_var_id_is_not_def((yyvsp[-1].string), yylineno); 
																					new_variable((yyvsp[-2].vartype), (yyvsp[-1].string), yylineno, true); 	
																				}
#line 1612 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 158 "parser.ypp" /* yacc.c:1646  */
    { 	check_is_def_enum((yyvsp[-2].string), yylineno); 
																					check_var_id_is_not_def((yyvsp[-1].string), yylineno); new_enum_variable((yyvsp[-2].string), (yyvsp[-1].string), yylineno, true); 	
																				}
#line 1620 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 161 "parser.ypp" /* yacc.c:1646  */
    { 
																				}
#line 1627 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 164 "parser.ypp" /* yacc.c:1646  */
    { check_var_id_is_not_def((yyvsp[-1].string), yylineno);  new_variable((yyvsp[-2].vartype), (yyvsp[-1].string), yylineno, false); }
#line 1633 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 164 "parser.ypp" /* yacc.c:1646  */
    {
																																	assign_to_variable((yyvsp[-4].string), yylineno, (yyvsp[-1].expr)); 
																																}
#line 1641 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 168 "parser.ypp" /* yacc.c:1646  */
    { check_is_def_enum((yyvsp[-2].string), yylineno); check_var_id_is_not_def((yyvsp[-1].string), yylineno); new_enum_variable((yyvsp[-2].string), (yyvsp[-1].string), yylineno, false); }
#line 1647 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 168 "parser.ypp" /* yacc.c:1646  */
    { 
																																										assign_to_enum_variable((yyvsp[-5].string), (yyvsp[-4].string), yylineno, (yyvsp[-1].expr)); 
																																									}
#line 1655 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 171 "parser.ypp" /* yacc.c:1646  */
    { check_var_id_is_def((yyvsp[-1].string), yylineno);  }
#line 1661 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 171 "parser.ypp" /* yacc.c:1646  */
    { 
																																	check_assignment ((yyvsp[-4].string), (yyvsp[-1].expr), yylineno); 
																																	assign_to_variable((yyvsp[-4].string), yylineno, (yyvsp[-1].expr)); 
																																}
#line 1670 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 175 "parser.ypp" /* yacc.c:1646  */
    { 
																																	eimit_call_backpatch((yyvsp[-1].expr));
																																}
#line 1678 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 178 "parser.ypp" /* yacc.c:1646  */
    { 
																																	check_return_void(yylineno);     
																																}
#line 1686 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 181 "parser.ypp" /* yacc.c:1646  */
    { 
																																	check_return((yyvsp[-1].expr), yylineno); 
																																}
#line 1694 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 185 "parser.ypp" /* yacc.c:1646  */
    { emit_if((yyvsp[-6].expr), (yyvsp[-5].string), (yyvsp[0].number));  }
#line 1700 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 186 "parser.ypp" /* yacc.c:1646  */
    { emit_if_else((yyvsp[-12].expr), (yyvsp[-11].string), (yyvsp[-6].number), (yyvsp[-4].string), (yyvsp[-1].number));  }
#line 1706 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 188 "parser.ypp" /* yacc.c:1646  */
    { emit_while((yyvsp[-11].number), (yyvsp[-10].string), (yyvsp[-9].expr), (yyvsp[-8].string), (yyvsp[-1].number), (yyvsp[0].string));  }
#line 1712 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 189 "parser.ypp" /* yacc.c:1646  */
    { check_break	(yylineno); emit_break_command();    }
#line 1718 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 190 "parser.ypp" /* yacc.c:1646  */
    { check_continue(yylineno); emit_continue_command(); }
#line 1724 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 193 "parser.ypp" /* yacc.c:1646  */
    { check_exp_is(type_bool, (yyvsp[0].expr), yylineno); (yyval.expr) = (yyvsp[0].expr); }
#line 1730 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 194 "parser.ypp" /* yacc.c:1646  */
    { enter_scope();	 }
#line 1736 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 195 "parser.ypp" /* yacc.c:1646  */
    { exit_scope();	 }
#line 1742 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 196 "parser.ypp" /* yacc.c:1646  */
    { enter_loop();	 }
#line 1748 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 197 "parser.ypp" /* yacc.c:1646  */
    { exit_loop();		 }
#line 1754 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 198 "parser.ypp" /* yacc.c:1646  */
    { (yyval.number) = emit_br_command();  }
#line 1760 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 199 "parser.ypp" /* yacc.c:1646  */
    { (yyval.string) = strdup(CodeBuffer::instance().genLabel().c_str()); }
#line 1766 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 202 "parser.ypp" /* yacc.c:1646  */
    { check_called_function_exists((yyvsp[-1].string), yylineno); }
#line 1772 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 202 "parser.ypp" /* yacc.c:1646  */
    { (yyval.expr) = check_called_function_close(yylineno); }
#line 1778 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 203 "parser.ypp" /* yacc.c:1646  */
    { check_called_function_exists((yyvsp[-1].string), yylineno); }
#line 1784 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 203 "parser.ypp" /* yacc.c:1646  */
    { (yyval.expr) = check_called_function_close(yylineno); }
#line 1790 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 207 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1796 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 208 "parser.ypp" /* yacc.c:1646  */
    { }
#line 1802 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 211 "parser.ypp" /* yacc.c:1646  */
    { check_called_function_arg((yyvsp[0].expr), yylineno); }
#line 1808 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 214 "parser.ypp" /* yacc.c:1646  */
    { (yyval.vartype) = type_int;  }
#line 1814 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 215 "parser.ypp" /* yacc.c:1646  */
    { (yyval.vartype) = type_byte; }
#line 1820 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 216 "parser.ypp" /* yacc.c:1646  */
    { (yyval.vartype) = type_bool; }
#line 1826 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 220 "parser.ypp" /* yacc.c:1646  */
    { (yyval.string) = (yyvsp[0].string); }
#line 1832 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 223 "parser.ypp" /* yacc.c:1646  */
    { 
																		check_not_void((yyvsp[-1].expr), yylineno);
																		(yyval.expr) = (yyvsp[-1].expr);
																	}
#line 1841 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 227 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_plus_minus((yyvsp[-2].expr), (yyvsp[0].expr), yylineno, true);
																	}
#line 1849 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 230 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_plus_minus((yyvsp[-2].expr), (yyvsp[0].expr), yylineno, false);
																	}
#line 1857 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 233 "parser.ypp" /* yacc.c:1646  */
    {  
																		(yyval.expr) = emit_div((yyvsp[-2].expr), (yyvsp[0].expr), yylineno);
																	}
#line 1865 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 236 "parser.ypp" /* yacc.c:1646  */
    {  
																		(yyval.expr) = emit_mult((yyvsp[-2].expr), (yyvsp[0].expr), yylineno);
																	}
#line 1873 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 239 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_exp_ID((yyvsp[0].string), yylineno);
																	}
#line 1881 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 242 "parser.ypp" /* yacc.c:1646  */
    { 
																		//$$.type = get_type_from_call($1, yylineno); 	
																		//$$.init = true; 	
																		//$$.void_res_exp = ($1 == ret_type_void);
																		(yyval.expr) = (yyvsp[0].expr);
																	}
#line 1892 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 248 "parser.ypp" /* yacc.c:1646  */
    {
																		(yyval.expr) = emit_exp_INT32((yyvsp[0].number)); 
																	}
#line 1900 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 251 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_exp_BYTE((yyvsp[-1].number), yylineno); 
																	}
#line 1908 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 254 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_exp_STRING((yyvsp[0].string)); 
																	}
#line 1916 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 257 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_exp_TRUE(); 
																	}
#line 1924 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 260 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_exp_FALSE(); 
																	}
#line 1932 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 263 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_exp_NOT((yyvsp[0].expr), yylineno);																	
																	}
#line 1940 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 266 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_exp_AND((yyvsp[-3].expr), (yyvsp[0].expr), yylineno, (yyvsp[-1].string));
																	}
#line 1948 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 269 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_exp_OR((yyvsp[-3].expr), (yyvsp[0].expr), yylineno, (yyvsp[-1].string));
																	}
#line 1956 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 272 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_exp_EQ((yyvsp[-2].expr), (yyvsp[0].expr), yylineno, true);
																	}
#line 1964 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 275 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_exp_EQ((yyvsp[-2].expr), (yyvsp[0].expr), yylineno, false);
																	}
#line 1972 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 278 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_exp_RELOP((yyvsp[-2].expr), (yyvsp[0].expr), yylineno, 0);																		
																	}
#line 1980 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 281 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_exp_RELOP((yyvsp[-2].expr), (yyvsp[0].expr), yylineno, 1);
																	}
#line 1988 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 284 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_exp_RELOP((yyvsp[-2].expr), (yyvsp[0].expr), yylineno, 2);
																	}
#line 1996 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 287 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_exp_RELOP((yyvsp[-2].expr), (yyvsp[0].expr), yylineno, 3);
																	}
#line 2004 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 290 "parser.ypp" /* yacc.c:1646  */
    { 
																		(yyval.expr) = emit_var_convertion((yyvsp[-2].vartype), (yyvsp[0].expr), yylineno);
																	}
#line 2012 "parser.tab.cpp" /* yacc.c:1646  */
    break;


#line 2016 "parser.tab.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
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

  /* Else will try to reuse lookahead token after shifting the error
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 295 "parser.ypp" /* yacc.c:1906  */


int main() {
	yyparse();
}

int yyerror(const char * message) {
	output::errorSyn(yylineno);
	exit(0);
}
