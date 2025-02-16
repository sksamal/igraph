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
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         igraph_dl_yyparse
#define yylex           igraph_dl_yylex
#define yyerror         igraph_dl_yyerror
#define yydebug         igraph_dl_yydebug
#define yynerrs         igraph_dl_yynerrs


/* Copy the first part of user declarations.  */
#line 23 "foreign-dl-parser.y" /* yacc.c:339  */


/* 
   IGraph library.
   Copyright (C) 2009-2012  Gabor Csardi <csardi.gabor@gmail.com>
   334 Harvard st, Cambridge, MA, 02138 USA
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc.,  51 Franklin Street, Fifth Floor, Boston, MA 
   02110-1301 USA

*/

#ifdef __clang__
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wsign-conversion"
#endif

#include "config.h"
#include "igraph_hacks_internal.h"
#include "igraph_math.h"
#include "igraph_types_internal.h"
#include "foreign-dl-header.h"
#include "foreign-dl-parser.h"
#include <stdio.h>

#define yyscan_t void*

int igraph_dl_yylex(YYSTYPE* lvalp, YYLTYPE* llocp, void* scanner);
int igraph_dl_yyerror(YYLTYPE* locp, igraph_i_dl_parsedata_t* context, 
		      const char *s);
char *igraph_dl_yyget_text (yyscan_t yyscanner );
int igraph_dl_yyget_leng (yyscan_t yyscanner );

int igraph_i_dl_add_str(char *newstr, int length, 
			igraph_i_dl_parsedata_t *context);
int igraph_i_dl_add_edge(long int from, long int to,
			 igraph_i_dl_parsedata_t *context);
int igraph_i_dl_add_edge_w(long int from, long int to, 
			   igraph_real_t weight,
			   igraph_i_dl_parsedata_t *context);

extern igraph_real_t igraph_pajek_get_number(const char *str, long int len);

#define scanner context->scanner
 

#line 131 "foreign-dl-parser.c" /* yacc.c:339  */

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
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_IGRAPH_DL_YY_FOREIGN_DL_PARSER_H_INCLUDED
# define YY_IGRAPH_DL_YY_FOREIGN_DL_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int igraph_dl_yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUM = 258,
    NEWLINE = 259,
    DL = 260,
    NEQ = 261,
    DATA = 262,
    LABELS = 263,
    LABELSEMBEDDED = 264,
    FORMATFULLMATRIX = 265,
    FORMATEDGELIST1 = 266,
    FORMATNODELIST1 = 267,
    DIGIT = 268,
    LABEL = 269,
    EOFF = 270
  };
#endif
/* Tokens.  */
#define NUM 258
#define NEWLINE 259
#define DL 260
#define NEQ 261
#define DATA 262
#define LABELS 263
#define LABELSEMBEDDED 264
#define FORMATFULLMATRIX 265
#define FORMATEDGELIST1 266
#define FORMATNODELIST1 267
#define DIGIT 268
#define LABEL 269
#define EOFF 270

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 91 "foreign-dl-parser.y" /* yacc.c:355  */

  long int integer;
  igraph_real_t real;

#line 206 "foreign-dl-parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int igraph_dl_yyparse (igraph_i_dl_parsedata_t* context);

#endif /* !YY_IGRAPH_DL_YY_FOREIGN_DL_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 236 "foreign-dl-parser.c" /* yacc.c:358  */

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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYLAST   118

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  16
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  37
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  138

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   270

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
      15
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   115,   115,   117,   117,   119,   119,   121,   122,   123,
     126,   126,   128,   128,   130,   131,   132,   135,   136,   142,
     142,   147,   147,   149,   159,   161,   163,   163,   165,   169,
     173,   178,   182,   184,   185,   186,   187,   188,   191,   192,
     195,   197,   201,   204,   205,   208,   210,   214,   217,   233,
     235,   236,   237,   238,   239,   242,   243,   246,   248,   251,
     251,   257,   258,   261,   263,   267,   267
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUM", "NEWLINE", "DL", "NEQ", "DATA",
  "LABELS", "LABELSEMBEDDED", "FORMATFULLMATRIX", "FORMATEDGELIST1",
  "FORMATNODELIST1", "DIGIT", "LABEL", "EOFF", "$accept", "input", "trail",
  "eof", "rest", "formfullmatrix", "newline", "fullmatrix", "labels",
  "fullmatrixdata", "zerooneseq", "zeroone", "labeledfullmatrixdata",
  "reallabeledfullmatrixdata", "labelseq", "label", "labeledmatrixlines",
  "labeledmatrixline", "edgelist1", "edgelist1rest", "edgelist1data",
  "edgelist1dataline", "integer", "labelededgelist1data",
  "labelededgelist1dataline", "weight", "elabel", "nodelist1",
  "nodelist1rest", "nodelist1data", "nodelist1dataline", "from", "tolist",
  "labelednodelist1data", "labelednodelist1dataline", "fromelabel",
  "labeltolist", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270
};
# endif

#define YYPACT_NINF -114

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-114)))

#define YYTABLE_NINF -22

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       8,    38,    11,    43,  -114,  -114,    44,    57,    46,    46,
      46,    46,    46,    46,  -114,  -114,  -114,  -114,  -114,  -114,
    -114,  -114,    69,    53,    63,    66,     6,    65,    46,    46,
    -114,    46,    46,    46,  -114,  -114,    46,    46,  -114,  -114,
    -114,  -114,     5,    19,  -114,  -114,  -114,    76,    84,  -114,
      82,  -114,  -114,  -114,    46,  -114,  -114,  -114,    93,    43,
      46,    46,    46,  -114,  -114,  -114,    46,    46,    46,  -114,
      85,    86,  -114,    43,    23,  -114,  -114,    88,    33,  -114,
    -114,    65,  -114,    85,  -114,  -114,  -114,    90,    46,    46,
      87,    46,  -114,  -114,    46,    46,    87,    46,    25,  -114,
    -114,  -114,    94,  -114,    95,  -114,  -114,    87,    29,  -114,
      96,  -114,  -114,  -114,    49,  -114,  -114,    43,    46,    92,
      46,    84,    46,     2,    46,  -114,  -114,   100,  -114,  -114,
    -114,  -114,  -114,    87,  -114,    87,    87,    87
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     1,    42,     0,     0,    12,    12,
      12,    12,    12,    12,     3,     7,    11,     8,     9,    13,
      19,    17,     0,     0,     0,     0,     5,    14,    12,    12,
      10,    12,    12,    12,    32,    55,    12,    12,    49,     6,
       2,     4,     0,     0,    26,    38,    17,     0,    50,    17,
       0,    20,    23,    22,    12,    18,    16,    24,    12,    33,
      12,    12,    12,    58,    56,    59,    12,    12,    12,    19,
       0,     0,    39,     0,     0,    43,    17,     0,     0,    61,
      17,    15,    21,    25,    29,    28,    27,     0,    12,    12,
      35,    12,    57,    60,    12,    12,    52,    12,     0,    30,
      47,    41,     0,    38,     0,    48,    44,     0,     0,    55,
       0,    64,    62,    65,     0,    31,    40,    34,    12,     0,
      12,    51,    12,     0,    12,    43,    46,     0,    43,    61,
      63,    66,    61,    36,    45,    37,    53,    54
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -114,  -114,  -114,  -114,  -114,  -114,    -9,    83,   -41,    36,
      26,  -114,  -114,  -114,  -114,  -114,  -114,    24,  -114,  -114,
       7,  -114,     4,  -113,  -114,    -7,   -82,  -114,  -114,     9,
    -114,  -114,  -114,   -98,  -114,  -114,  -114
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,    26,    40,    14,    15,    20,    16,    28,    27,
      42,    53,    56,    57,    58,    86,    83,    84,    17,    34,
      59,    72,    73,    90,   106,   102,   107,    18,    38,    48,
      64,    65,    77,    96,   112,   113,   123
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      21,    22,    23,    24,    25,    60,   130,     6,    66,    51,
      19,     4,   133,     1,   111,   135,   105,    41,    52,    43,
      44,    39,    45,    46,    47,   119,    54,    49,    50,   115,
      88,   136,    89,    55,   137,    91,   120,    55,    52,    97,
      94,   131,    95,    55,     3,    69,     5,    55,     7,    71,
      19,    74,    75,    76,   111,   111,   124,    78,    79,    80,
       8,     9,    10,    55,     8,     9,    10,    11,    12,    13,
      31,    32,    33,    35,    36,    37,    29,    87,   -21,   103,
     104,    93,   108,    61,    62,   109,   110,    63,   114,    67,
      68,     5,    92,   100,   101,   100,   126,    70,   116,    82,
      85,   105,   118,   122,   134,    81,    30,    99,    98,   125,
     117,   128,   127,   129,     0,   132,     0,     0,   121
};

static const yytype_int16 yycheck[] =
{
       9,    10,    11,    12,    13,    46,     4,     3,    49,     4,
       4,     0,   125,     5,    96,   128,    14,    26,    13,    28,
      29,    15,    31,    32,    33,   107,     7,    36,    37,     4,
       7,   129,     9,    14,   132,    76,     7,    14,    13,    80,
       7,   123,     9,    14,     6,    54,     3,    14,     4,    58,
       4,    60,    61,    62,   136,   137,     7,    66,    67,    68,
       7,     8,     9,    14,     7,     8,     9,    10,    11,    12,
       7,     8,     9,     7,     8,     9,     7,    73,    13,    88,
      89,    77,    91,     7,     8,    94,    95,     3,    97,     7,
       8,     3,     4,     3,     4,     3,     4,     4,     4,    14,
      14,    14,     7,     7,     4,    69,    23,    83,    82,   118,
     103,   120,   119,   122,    -1,   124,    -1,    -1,   109
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,    17,     6,     0,     3,    38,     4,     7,     8,
       9,    10,    11,    12,    20,    21,    23,    34,    43,     4,
      22,    22,    22,    22,    22,    22,    18,    25,    24,     7,
      23,     7,     8,     9,    35,     7,     8,     9,    44,    15,
      19,    22,    26,    22,    22,    22,    22,    22,    45,    22,
      22,     4,    13,    27,     7,    14,    28,    29,    30,    36,
      24,     7,     8,     3,    46,    47,    24,     7,     8,    22,
       4,    22,    37,    38,    22,    22,    22,    48,    22,    22,
      22,    25,    14,    32,    33,    14,    31,    38,     7,     9,
      39,    24,     4,    38,     7,     9,    49,    24,    26,    33,
       3,     4,    41,    22,    22,    14,    40,    42,    22,    22,
      22,    42,    50,    51,    22,     4,     4,    36,     7,    42,
       7,    45,     7,    52,     7,    22,     4,    41,    22,    22,
       4,    42,    22,    39,     4,    39,    49,    49
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    16,    17,    18,    18,    19,    19,    20,    20,    20,
      21,    21,    22,    22,    23,    23,    23,    24,    24,    25,
      25,    26,    26,    27,    28,    29,    30,    30,    31,    32,
      32,    33,    34,    35,    35,    35,    35,    35,    36,    36,
      37,    37,    38,    39,    39,    40,    40,    41,    42,    43,
      44,    44,    44,    44,    44,    45,    45,    46,    47,    48,
      48,    49,    49,    50,    51,    52,    52
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     7,     0,     2,     0,     1,     1,     1,     1,
       3,     1,     0,     1,     3,     7,     5,     0,     3,     0,
       3,     0,     2,     1,     1,     3,     0,     3,     1,     1,
       2,     3,     3,     3,     7,     5,     9,     9,     0,     2,
       4,     3,     1,     0,     2,     4,     3,     1,     1,     3,
       2,     7,     5,     9,     9,     0,     2,     3,     1,     0,
       2,     0,     2,     3,     1,     0,     2
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
      yyerror (&yylloc, context, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, context); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, igraph_i_dl_parsedata_t* context)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (context);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, igraph_i_dl_parsedata_t* context)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, context);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, igraph_i_dl_parsedata_t* context)
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , context);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, context); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, igraph_i_dl_parsedata_t* context)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (context);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (igraph_i_dl_parsedata_t* context)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

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

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yychar = yylex (&yylval, &yylloc, scanner);
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
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 115 "foreign-dl-parser.y" /* yacc.c:1646  */
    { context->n=(yyvsp[-4].integer); }
#line 1502 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 121 "foreign-dl-parser.y" /* yacc.c:1646  */
    { context->type=IGRAPH_DL_MATRIX; }
#line 1508 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 122 "foreign-dl-parser.y" /* yacc.c:1646  */
    { context->type=IGRAPH_DL_EDGELIST1; }
#line 1514 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 123 "foreign-dl-parser.y" /* yacc.c:1646  */
    { context->type=IGRAPH_DL_NODELIST1; }
#line 1520 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 126 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1526 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 126 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1532 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 130 "foreign-dl-parser.y" /* yacc.c:1646  */
    { }
#line 1538 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 131 "foreign-dl-parser.y" /* yacc.c:1646  */
    { }
#line 1544 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 132 "foreign-dl-parser.y" /* yacc.c:1646  */
    { }
#line 1550 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 135 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1556 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 136 "foreign-dl-parser.y" /* yacc.c:1646  */
    { 
	      igraph_i_dl_add_str(igraph_dl_yyget_text(scanner), 
                                  igraph_dl_yyget_leng(scanner), 
				  context); }
#line 1565 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 142 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1571 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 142 "foreign-dl-parser.y" /* yacc.c:1646  */
    {
  context->from += 1;
  context->to = 0;
 }
#line 1580 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 147 "foreign-dl-parser.y" /* yacc.c:1646  */
    { }
#line 1586 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 149 "foreign-dl-parser.y" /* yacc.c:1646  */
    {
  if (igraph_dl_yyget_text(scanner)[0]=='1') {
    IGRAPH_CHECK(igraph_vector_push_back(&context->edges, 
					 context->from));
    IGRAPH_CHECK(igraph_vector_push_back(&context->edges, 
					 context->to));
  }
  context->to += 1;
}
#line 1600 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 159 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1606 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 161 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1612 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 165 "foreign-dl-parser.y" /* yacc.c:1646  */
    { igraph_i_dl_add_str(igraph_dl_yyget_text(scanner), 
                                   igraph_dl_yyget_leng(scanner), 
				   context); }
#line 1620 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 169 "foreign-dl-parser.y" /* yacc.c:1646  */
    {
	         context->from += 1; 
		 context->to = 0;
               }
#line 1629 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 173 "foreign-dl-parser.y" /* yacc.c:1646  */
    { 
	         context->from += 1; 
		 context->to = 0;
               }
#line 1638 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 178 "foreign-dl-parser.y" /* yacc.c:1646  */
    { }
#line 1644 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 182 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1650 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 184 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1656 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 185 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1662 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 186 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1668 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 187 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1674 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 188 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1680 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 191 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1686 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 192 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1692 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 40:
#line 195 "foreign-dl-parser.y" /* yacc.c:1646  */
    {
                   igraph_i_dl_add_edge_w((yyvsp[-3].integer)-1, (yyvsp[-2].integer)-1, (yyvsp[-1].real), context); }
#line 1699 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 41:
#line 197 "foreign-dl-parser.y" /* yacc.c:1646  */
    {
		   igraph_i_dl_add_edge((yyvsp[-2].integer)-1, (yyvsp[-1].integer)-1, context);
}
#line 1707 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 42:
#line 201 "foreign-dl-parser.y" /* yacc.c:1646  */
    { (yyval.integer)=igraph_pajek_get_number(igraph_dl_yyget_text(scanner), 
					  igraph_dl_yyget_leng(scanner)); }
#line 1714 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 43:
#line 204 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1720 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 44:
#line 205 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1726 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 45:
#line 208 "foreign-dl-parser.y" /* yacc.c:1646  */
    {
                          igraph_i_dl_add_edge_w((yyvsp[-3].integer), (yyvsp[-2].integer), (yyvsp[-1].real), context); }
#line 1733 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 46:
#line 210 "foreign-dl-parser.y" /* yacc.c:1646  */
    {
			  igraph_i_dl_add_edge((yyvsp[-2].integer), (yyvsp[-1].integer), context);
 }
#line 1741 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 47:
#line 214 "foreign-dl-parser.y" /* yacc.c:1646  */
    { (yyval.real)=igraph_pajek_get_number(igraph_dl_yyget_text(scanner), 
					 igraph_dl_yyget_leng(scanner)); }
#line 1748 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 48:
#line 217 "foreign-dl-parser.y" /* yacc.c:1646  */
    {
  /* Copy label list to trie, if needed */
  if (igraph_strvector_size(&context->labels) != 0) {
    long int i, id, n=igraph_strvector_size(&context->labels);
    for (i=0; i<n; i++) {
      igraph_trie_get(&context->trie,
		      STR(context->labels, i), &id);
    }
    igraph_strvector_clear(&context->labels);
  }
  igraph_trie_get2(&context->trie, igraph_dl_yyget_text(scanner), 
		   igraph_dl_yyget_leng(scanner), &(yyval.integer));
 }
#line 1766 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 49:
#line 233 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1772 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 235 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1778 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 51:
#line 236 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1784 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 237 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1790 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 238 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1796 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 239 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1802 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 242 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1808 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 243 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1814 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 246 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1820 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 248 "foreign-dl-parser.y" /* yacc.c:1646  */
    { context->from=igraph_pajek_get_number(igraph_dl_yyget_text(scanner),
							  igraph_dl_yyget_leng(scanner)); }
#line 1827 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 251 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1833 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 251 "foreign-dl-parser.y" /* yacc.c:1646  */
    { 
  IGRAPH_CHECK(igraph_vector_push_back(&context->edges, 
				       context->from-1)); 
  IGRAPH_CHECK(igraph_vector_push_back(&context->edges, (yyvsp[0].integer)-1));
 }
#line 1843 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 257 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1849 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 258 "foreign-dl-parser.y" /* yacc.c:1646  */
    {}
#line 1855 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 261 "foreign-dl-parser.y" /* yacc.c:1646  */
    { }
#line 1861 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 263 "foreign-dl-parser.y" /* yacc.c:1646  */
    {
  context->from=(yyvsp[0].integer);
 }
#line 1869 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 267 "foreign-dl-parser.y" /* yacc.c:1646  */
    {
  IGRAPH_CHECK(igraph_vector_push_back(&context->edges, 
				       context->from));
  IGRAPH_CHECK(igraph_vector_push_back(&context->edges, (yyvsp[0].integer)));
 }
#line 1879 "foreign-dl-parser.c" /* yacc.c:1646  */
    break;


#line 1883 "foreign-dl-parser.c" /* yacc.c:1646  */
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
  *++yylsp = yyloc;

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
      yyerror (&yylloc, context, YY_("syntax error"));
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
        yyerror (&yylloc, context, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

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
                      yytoken, &yylval, &yylloc, context);
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

  yyerror_range[1] = yylsp[1-yylen];
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, context);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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
  yyerror (&yylloc, context, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, context);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, context);
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
#line 273 "foreign-dl-parser.y" /* yacc.c:1906  */


int igraph_dl_yyerror(YYLTYPE* locp, igraph_i_dl_parsedata_t* context, 
		      const char *s) {
  snprintf(context->errmsg, 
	   sizeof(context->errmsg)/sizeof(char)-1, 
	   "%s in line %i", s, locp->first_line);
  return 0;
}

int igraph_i_dl_add_str(char *newstr, int length, 
			igraph_i_dl_parsedata_t *context) {
  int tmp=newstr[length];
  newstr[length]='\0';
  IGRAPH_CHECK(igraph_strvector_add(&context->labels, newstr));
  newstr[length]=tmp;
  return 0;
}

int igraph_i_dl_add_edge(long int from, long int to, 
			 igraph_i_dl_parsedata_t *context) {
  IGRAPH_CHECK(igraph_vector_push_back(&context->edges, from));
  IGRAPH_CHECK(igraph_vector_push_back(&context->edges, to));
  return 0;
}

int igraph_i_dl_add_edge_w(long int from, long int to, 
			   igraph_real_t weight,
			   igraph_i_dl_parsedata_t *context) {
  long int n=igraph_vector_size(&context->weights);
  long int n2=igraph_vector_size(&context->edges)/2;
  if (n != n2) {
    igraph_vector_resize(&context->weights, n2);
    for (; n<n2; n++) {
      VECTOR(context->weights)[n]=IGRAPH_NAN;
    }
  }
  IGRAPH_CHECK(igraph_i_dl_add_edge(from, to, context));
  IGRAPH_CHECK(igraph_vector_push_back(&context->weights, weight));
  return 0;
}
