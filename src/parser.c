/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "error.h"
#include "lexer.h"

extern int yylex(void);
void yyerror(const char *msg);

static AstNode *wrapper_program;

#line 86 "parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUMBER_LITERAL = 3,             /* NUMBER_LITERAL  */
  YYSYMBOL_STRING_LITERAL = 4,             /* STRING_LITERAL  */
  YYSYMBOL_ID = 5,                         /* ID  */
  YYSYMBOL_VAR = 6,                        /* VAR  */
  YYSYMBOL_IF = 7,                         /* IF  */
  YYSYMBOL_THEN = 8,                       /* THEN  */
  YYSYMBOL_OTHERWISE = 9,                  /* OTHERWISE  */
  YYSYMBOL_WHILE = 10,                     /* WHILE  */
  YYSYMBOL_FUNCTION = 11,                  /* FUNCTION  */
  YYSYMBOL_BEGIN = 12,                     /* BEGIN  */
  YYSYMBOL_END = 13,                       /* END  */
  YYSYMBOL_RETURN = 14,                    /* RETURN  */
  YYSYMBOL_DISPLAY = 15,                   /* DISPLAY  */
  YYSYMBOL_TRUE = 16,                      /* TRUE  */
  YYSYMBOL_FALSE = 17,                     /* FALSE  */
  YYSYMBOL_AND = 18,                       /* AND  */
  YYSYMBOL_OR = 19,                        /* OR  */
  YYSYMBOL_NOT = 20,                       /* NOT  */
  YYSYMBOL_ASSIGN = 21,                    /* ASSIGN  */
  YYSYMBOL_PLUS = 22,                      /* PLUS  */
  YYSYMBOL_MINUS = 23,                     /* MINUS  */
  YYSYMBOL_MULTIPLY = 24,                  /* MULTIPLY  */
  YYSYMBOL_DIVIDE = 25,                    /* DIVIDE  */
  YYSYMBOL_LT = 26,                        /* LT  */
  YYSYMBOL_GT = 27,                        /* GT  */
  YYSYMBOL_LE = 28,                        /* LE  */
  YYSYMBOL_GE = 29,                        /* GE  */
  YYSYMBOL_EQ = 30,                        /* EQ  */
  YYSYMBOL_NE = 31,                        /* NE  */
  YYSYMBOL_LPAREN = 32,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 33,                    /* RPAREN  */
  YYSYMBOL_COMMA = 34,                     /* COMMA  */
  YYSYMBOL_SEMICOLON = 35,                 /* SEMICOLON  */
  YYSYMBOL_YYACCEPT = 36,                  /* $accept  */
  YYSYMBOL_program = 37,                   /* program  */
  YYSYMBOL_function_list = 38,             /* function_list  */
  YYSYMBOL_declaration_list = 39,          /* declaration_list  */
  YYSYMBOL_executable_list = 40,           /* executable_list  */
  YYSYMBOL_function_decl = 41,             /* function_decl  */
  YYSYMBOL_fn_body = 42,                   /* fn_body  */
  YYSYMBOL_fn_locals = 43,                 /* fn_locals  */
  YYSYMBOL_fn_execs = 44,                  /* fn_execs  */
  YYSYMBOL_fn_stmt = 45,                   /* fn_stmt  */
  YYSYMBOL_opt_params = 46,                /* opt_params  */
  YYSYMBOL_params = 47,                    /* params  */
  YYSYMBOL_block = 48,                     /* block  */
  YYSYMBOL_stmt = 49,                      /* stmt  */
  YYSYMBOL_var_decl = 50,                  /* var_decl  */
  YYSYMBOL_assignment = 51,                /* assignment  */
  YYSYMBOL_display = 52,                   /* display  */
  YYSYMBOL_if_stmt = 53,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 54,                /* while_stmt  */
  YYSYMBOL_return_stmt = 55,               /* return_stmt  */
  YYSYMBOL_expr = 56,                      /* expr  */
  YYSYMBOL_logical_or = 57,                /* logical_or  */
  YYSYMBOL_logical_and = 58,               /* logical_and  */
  YYSYMBOL_equality = 59,                  /* equality  */
  YYSYMBOL_relational = 60,                /* relational  */
  YYSYMBOL_additive = 61,                  /* additive  */
  YYSYMBOL_multiplicative = 62,            /* multiplicative  */
  YYSYMBOL_unary = 63,                     /* unary  */
  YYSYMBOL_primary = 64,                   /* primary  */
  YYSYMBOL_function_call = 65,             /* function_call  */
  YYSYMBOL_opt_expr_list = 66,             /* opt_expr_list  */
  YYSYMBOL_expr_list = 67                  /* expr_list  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   102

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  36
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  71
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  116

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   290


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
      35
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    60,    60,    78,    83,    90,    95,   102,   107,   114,
     123,   138,   143,   150,   155,   162,   163,   164,   165,   169,
     172,   178,   184,   193,   196,   200,   206,   207,   208,   209,
     210,   214,   222,   230,   237,   243,   252,   260,   267,   271,
     272,   281,   282,   291,   292,   298,   307,   308,   314,   320,
     326,   335,   336,   342,   351,   352,   358,   367,   372,   377,
     381,   385,   389,   393,   397,   401,   402,   406,   414,   415,
     419,   423
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUMBER_LITERAL",
  "STRING_LITERAL", "ID", "VAR", "IF", "THEN", "OTHERWISE", "WHILE",
  "FUNCTION", "BEGIN", "END", "RETURN", "DISPLAY", "TRUE", "FALSE", "AND",
  "OR", "NOT", "ASSIGN", "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "LT", "GT",
  "LE", "GE", "EQ", "NE", "LPAREN", "RPAREN", "COMMA", "SEMICOLON",
  "$accept", "program", "function_list", "declaration_list",
  "executable_list", "function_decl", "fn_body", "fn_locals", "fn_execs",
  "fn_stmt", "opt_params", "params", "block", "stmt", "var_decl",
  "assignment", "display", "if_stmt", "while_stmt", "return_stmt", "expr",
  "logical_or", "logical_and", "equality", "relational", "additive",
  "multiplicative", "unary", "primary", "function_call", "opt_expr_list",
  "expr_list", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-55)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -55,    17,    -6,   -55,    10,    37,   -55,    12,    46,    11,
     -55,    47,    38,    39,     7,     7,     7,     7,   -55,   -55,
     -55,   -55,   -55,   -55,   -55,    29,    36,     7,     7,   -55,
     -55,    31,   -55,   -55,     7,     7,     7,    63,    54,    56,
     -22,     5,    -3,     4,   -55,   -55,   -55,   -55,    48,    49,
      70,    80,    53,    55,     7,   -55,   -55,    58,   -55,     7,
       7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
       7,    51,   -55,   -55,   -55,   -55,   -55,   -55,   -55,    59,
      52,   -55,    40,    56,   -22,     5,     5,    -3,    -3,    -3,
      -3,     4,     4,   -55,   -55,   -55,   -55,   -55,    76,    37,
     -55,     7,   -55,   -55,   -55,    11,   -55,   -55,    62,   -55,
     -55,   -55,   -55,   -55,   -55,   -55
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     5,     1,     0,     7,     4,     0,     0,     2,
       6,    19,     0,     0,     0,     0,     0,     0,     8,    26,
      27,    28,    29,    30,    21,     0,    20,     0,     0,    60,
      61,    64,    62,    63,     0,     0,     0,     0,    38,    39,
      41,    43,    46,    51,    54,    59,    65,    23,     0,     0,
       0,     0,     0,     0,    68,    57,    58,     0,    23,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,    33,    11,    22,    31,    32,    70,     0,
      69,    66,     0,    40,    42,    44,    45,    47,    48,    49,
      50,    52,    53,    55,    56,    36,    24,    25,     0,    13,
      67,     0,    23,    35,     9,     0,    12,    71,     0,    14,
      15,    16,    17,    18,    10,    34
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,
     -55,   -55,   -54,    84,    -5,   -10,    -9,    -8,    -7,    -4,
     -14,   -55,    41,    42,   -26,    15,   -30,   -28,   -55,   -55,
     -55,   -55
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     5,     9,     6,    98,    99,   105,   109,
      25,    26,    71,    96,    97,    19,    20,    21,    22,    23,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      79,    80
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      10,    47,    48,    49,    82,     4,    55,    56,    61,    62,
      29,    30,    31,    52,    53,     7,    13,     3,    14,    67,
      68,    15,    57,    32,    33,    16,    17,    34,    69,    70,
      35,    63,    64,    65,    66,    85,    86,    91,    92,    36,
      78,    93,    94,     8,    11,    13,     8,    14,   108,   102,
      15,    12,    24,   103,    16,    17,    13,     8,    14,    27,
      28,    15,    50,    54,    95,    16,    17,    13,     8,    14,
      51,    58,    15,    59,    60,   115,    16,    17,    87,    88,
      89,    90,    74,    72,    73,    75,   101,   107,    76,   104,
      77,    81,   100,    18,   106,   110,   111,   112,   113,     0,
      83,   114,    84
};

static const yytype_int8 yycheck[] =
{
       5,    15,    16,    17,    58,    11,    34,    35,    30,    31,
       3,     4,     5,    27,    28,     5,     5,     0,     7,    22,
      23,    10,    36,    16,    17,    14,    15,    20,    24,    25,
      23,    26,    27,    28,    29,    61,    62,    67,    68,    32,
      54,    69,    70,     6,    32,     5,     6,     7,   102,     9,
      10,     5,     5,    13,    14,    15,     5,     6,     7,    21,
      21,    10,    33,    32,    13,    14,    15,     5,     6,     7,
      34,     8,    10,    19,    18,    13,    14,    15,    63,    64,
      65,    66,    12,    35,    35,     5,    34,   101,    35,    13,
      35,    33,    33,     9,    99,   105,   105,   105,   105,    -1,
      59,   105,    60
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    37,    38,     0,    11,    39,    41,     5,     6,    40,
      50,    32,     5,     5,     7,    10,    14,    15,    49,    51,
      52,    53,    54,    55,     5,    46,    47,    21,    21,     3,
       4,     5,    16,    17,    20,    23,    32,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    56,    56,    56,
      33,    34,    56,    56,    32,    63,    63,    56,     8,    19,
      18,    30,    31,    26,    27,    28,    29,    22,    23,    24,
      25,    48,    35,    35,    12,     5,    35,    35,    56,    66,
      67,    33,    48,    58,    59,    60,    60,    61,    61,    61,
      61,    62,    62,    63,    63,    13,    49,    50,    42,    43,
      33,    34,     9,    13,    13,    44,    50,    56,    48,    45,
      51,    52,    53,    54,    55,    13
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    36,    37,    38,    38,    39,    39,    40,    40,    41,
      42,    43,    43,    44,    44,    45,    45,    45,    45,    46,
      46,    47,    47,    48,    48,    48,    49,    49,    49,    49,
      49,    50,    51,    52,    53,    53,    54,    55,    56,    57,
      57,    58,    58,    59,    59,    59,    60,    60,    60,    60,
      60,    61,    61,    61,    62,    62,    62,    63,    63,    63,
      64,    64,    64,    64,    64,    64,    64,    65,    66,    66,
      67,    67
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     3,     0,     2,     0,     2,     0,     2,     8,
       3,     0,     2,     0,     2,     1,     1,     1,     1,     0,
       1,     1,     3,     0,     2,     2,     1,     1,     1,     1,
       1,     5,     4,     3,     7,     5,     4,     3,     1,     1,
       3,     1,     3,     1,     3,     3,     1,     3,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     3,     4,     0,     1,
       1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
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

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: function_list declaration_list executable_list  */
#line 60 "parser.y"
                                                     {
        size_t li;
        AstNode *p = ast_new(AST_PROGRAM, yylineno);
        for (li = 0; li < (yyvsp[-2].list).size; li++)
            ast_list_append(&p->params, (yyvsp[-2].list).items[li]);
        for (li = 0; li < (yyvsp[-1].list).size; li++)
            ast_list_append(&p->params, (yyvsp[-1].list).items[li]);
        for (li = 0; li < (yyvsp[0].list).size; li++)
            ast_list_append(&p->params, (yyvsp[0].list).items[li]);
        free((yyvsp[-2].list).items);
        free((yyvsp[-1].list).items);
        free((yyvsp[0].list).items);
        wrapper_program = p;
        (yyval.node) = p;
    }
#line 1350 "parser.c"
    break;

  case 3: /* function_list: %empty  */
#line 78 "parser.y"
                  {
        (yyval.list).items = NULL;
        (yyval.list).size = 0;
        (yyval.list).capacity = 0;
    }
#line 1360 "parser.c"
    break;

  case 4: /* function_list: function_list function_decl  */
#line 83 "parser.y"
                                  {
        ast_list_append(&(yyvsp[-1].list), (yyvsp[0].node));
        (yyval.list) = (yyvsp[-1].list);
    }
#line 1369 "parser.c"
    break;

  case 5: /* declaration_list: %empty  */
#line 90 "parser.y"
                  {
        (yyval.list).items = NULL;
        (yyval.list).size = 0;
        (yyval.list).capacity = 0;
    }
#line 1379 "parser.c"
    break;

  case 6: /* declaration_list: declaration_list var_decl  */
#line 95 "parser.y"
                                {
        ast_list_append(&(yyvsp[-1].list), (yyvsp[0].node));
        (yyval.list) = (yyvsp[-1].list);
    }
#line 1388 "parser.c"
    break;

  case 7: /* executable_list: %empty  */
#line 102 "parser.y"
                  {
        (yyval.list).items = NULL;
        (yyval.list).size = 0;
        (yyval.list).capacity = 0;
    }
#line 1398 "parser.c"
    break;

  case 8: /* executable_list: executable_list stmt  */
#line 107 "parser.y"
                           {
        ast_list_append(&(yyvsp[-1].list), (yyvsp[0].node));
        (yyval.list) = (yyvsp[-1].list);
    }
#line 1407 "parser.c"
    break;

  case 9: /* function_decl: FUNCTION ID LPAREN opt_params RPAREN BEGIN fn_body END  */
#line 114 "parser.y"
                                                             {
        (yyval.node) = (yyvsp[-4].node);
        (yyval.node)->kind = AST_FUNCTION_DECL;
        (yyval.node)->name = (yyvsp[-6].text);
        (yyval.node)->then_block = (yyvsp[-1].node);
    }
#line 1418 "parser.c"
    break;

  case 10: /* fn_body: fn_locals fn_execs return_stmt  */
#line 123 "parser.y"
                                     {
        size_t li;
        AstNode *blk = ast_new(AST_BLOCK, yylineno);
        for (li = 0; li < (yyvsp[-2].list).size; li++)
            ast_list_append(&blk->params, (yyvsp[-2].list).items[li]);
        for (li = 0; li < (yyvsp[-1].list).size; li++)
            ast_list_append(&blk->params, (yyvsp[-1].list).items[li]);
        ast_list_append(&blk->params, (yyvsp[0].node));
        free((yyvsp[-2].list).items);
        free((yyvsp[-1].list).items);
        (yyval.node) = blk;
    }
#line 1435 "parser.c"
    break;

  case 11: /* fn_locals: %empty  */
#line 138 "parser.y"
                  {
        (yyval.list).items = NULL;
        (yyval.list).size = 0;
        (yyval.list).capacity = 0;
    }
#line 1445 "parser.c"
    break;

  case 12: /* fn_locals: fn_locals var_decl  */
#line 143 "parser.y"
                         {
        ast_list_append(&(yyvsp[-1].list), (yyvsp[0].node));
        (yyval.list) = (yyvsp[-1].list);
    }
#line 1454 "parser.c"
    break;

  case 13: /* fn_execs: %empty  */
#line 150 "parser.y"
                  {
        (yyval.list).items = NULL;
        (yyval.list).size = 0;
        (yyval.list).capacity = 0;
    }
#line 1464 "parser.c"
    break;

  case 14: /* fn_execs: fn_execs fn_stmt  */
#line 155 "parser.y"
                       {
        ast_list_append(&(yyvsp[-1].list), (yyvsp[0].node));
        (yyval.list) = (yyvsp[-1].list);
    }
#line 1473 "parser.c"
    break;

  case 15: /* fn_stmt: assignment  */
#line 162 "parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1479 "parser.c"
    break;

  case 16: /* fn_stmt: display  */
#line 163 "parser.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1485 "parser.c"
    break;

  case 17: /* fn_stmt: if_stmt  */
#line 164 "parser.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1491 "parser.c"
    break;

  case 18: /* fn_stmt: while_stmt  */
#line 165 "parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1497 "parser.c"
    break;

  case 19: /* opt_params: %empty  */
#line 169 "parser.y"
                  {
        (yyval.node) = ast_new(AST_FUNCTION_DECL, yylineno);
    }
#line 1505 "parser.c"
    break;

  case 20: /* opt_params: params  */
#line 172 "parser.y"
             {
        (yyval.node) = (yyvsp[0].node);
    }
#line 1513 "parser.c"
    break;

  case 21: /* params: ID  */
#line 178 "parser.y"
         {
        AstNode *p = ast_new(AST_IDENTIFIER, yylineno);
        p->name = (yyvsp[0].text);
        (yyval.node) = ast_new(AST_FUNCTION_DECL, yylineno);
        ast_list_append(&(yyval.node)->params, p);
    }
#line 1524 "parser.c"
    break;

  case 22: /* params: params COMMA ID  */
#line 184 "parser.y"
                      {
        AstNode *p = ast_new(AST_IDENTIFIER, yylineno);
        p->name = (yyvsp[0].text);
        ast_list_append(&(yyvsp[-2].node)->params, p);
        (yyval.node) = (yyvsp[-2].node);
    }
#line 1535 "parser.c"
    break;

  case 23: /* block: %empty  */
#line 193 "parser.y"
                  {
        (yyval.node) = ast_new(AST_BLOCK, yylineno);
    }
#line 1543 "parser.c"
    break;

  case 24: /* block: block stmt  */
#line 196 "parser.y"
                 {
        ast_list_append(&(yyvsp[-1].node)->params, (yyvsp[0].node));
        (yyval.node) = (yyvsp[-1].node);
    }
#line 1552 "parser.c"
    break;

  case 25: /* block: block var_decl  */
#line 200 "parser.y"
                     {
        ast_list_append(&(yyvsp[-1].node)->params, (yyvsp[0].node));
        (yyval.node) = (yyvsp[-1].node);
    }
#line 1561 "parser.c"
    break;

  case 26: /* stmt: assignment  */
#line 206 "parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1567 "parser.c"
    break;

  case 27: /* stmt: display  */
#line 207 "parser.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1573 "parser.c"
    break;

  case 28: /* stmt: if_stmt  */
#line 208 "parser.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1579 "parser.c"
    break;

  case 29: /* stmt: while_stmt  */
#line 209 "parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1585 "parser.c"
    break;

  case 30: /* stmt: return_stmt  */
#line 210 "parser.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 1591 "parser.c"
    break;

  case 31: /* var_decl: VAR ID ASSIGN expr SEMICOLON  */
#line 214 "parser.y"
                                   {
        (yyval.node) = ast_new(AST_VAR_DECL, (yylsp[-3]).first_line);
        (yyval.node)->name = (yyvsp[-3].text);
        (yyval.node)->value = (yyvsp[-1].node);
    }
#line 1601 "parser.c"
    break;

  case 32: /* assignment: ID ASSIGN expr SEMICOLON  */
#line 222 "parser.y"
                               {
        (yyval.node) = ast_new(AST_ASSIGNMENT, (yylsp[-3]).first_line);
        (yyval.node)->name = (yyvsp[-3].text);
        (yyval.node)->value = (yyvsp[-1].node);
    }
#line 1611 "parser.c"
    break;

  case 33: /* display: DISPLAY expr SEMICOLON  */
#line 230 "parser.y"
                             {
        (yyval.node) = ast_new(AST_DISPLAY, (yylsp[-2]).first_line);
        (yyval.node)->return_expr = (yyvsp[-1].node);
    }
#line 1620 "parser.c"
    break;

  case 34: /* if_stmt: IF expr THEN block OTHERWISE block END  */
#line 237 "parser.y"
                                             {
        (yyval.node) = ast_new(AST_IF, (yylsp[-6]).first_line);
        (yyval.node)->cond = (yyvsp[-5].node);
        (yyval.node)->then_block = (yyvsp[-3].node);
        (yyval.node)->else_block = (yyvsp[-1].node);
    }
#line 1631 "parser.c"
    break;

  case 35: /* if_stmt: IF expr THEN block END  */
#line 243 "parser.y"
                             {
        (yyval.node) = ast_new(AST_IF, (yylsp[-4]).first_line);
        (yyval.node)->cond = (yyvsp[-3].node);
        (yyval.node)->then_block = (yyvsp[-1].node);
        (yyval.node)->else_block = NULL;
    }
#line 1642 "parser.c"
    break;

  case 36: /* while_stmt: WHILE expr block END  */
#line 252 "parser.y"
                           {
        (yyval.node) = ast_new(AST_WHILE, (yylsp[-3]).first_line);
        (yyval.node)->cond = (yyvsp[-2].node);
        (yyval.node)->then_block = (yyvsp[-1].node);
    }
#line 1652 "parser.c"
    break;

  case 37: /* return_stmt: RETURN expr SEMICOLON  */
#line 260 "parser.y"
                            {
        (yyval.node) = ast_new(AST_RETURN, (yylsp[-2]).first_line);
        (yyval.node)->return_expr = (yyvsp[-1].node);
    }
#line 1661 "parser.c"
    break;

  case 38: /* expr: logical_or  */
#line 267 "parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1667 "parser.c"
    break;

  case 39: /* logical_or: logical_and  */
#line 271 "parser.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 1673 "parser.c"
    break;

  case 40: /* logical_or: logical_or OR logical_and  */
#line 272 "parser.y"
                                {
        (yyval.node) = ast_new(AST_BINARY_EXPR, (yylsp[-1]).first_line);
        (yyval.node)->op = (char *)"or";
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 1684 "parser.c"
    break;

  case 41: /* logical_and: equality  */
#line 281 "parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1690 "parser.c"
    break;

  case 42: /* logical_and: logical_and AND equality  */
#line 282 "parser.y"
                               {
        (yyval.node) = ast_new(AST_BINARY_EXPR, (yylsp[-1]).first_line);
        (yyval.node)->op = (char *)"and";
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 1701 "parser.c"
    break;

  case 43: /* equality: relational  */
#line 291 "parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1707 "parser.c"
    break;

  case 44: /* equality: equality EQ relational  */
#line 292 "parser.y"
                             {
        (yyval.node) = ast_new(AST_BINARY_EXPR, (yylsp[-1]).first_line);
        (yyval.node)->op = (char *)"==";
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 1718 "parser.c"
    break;

  case 45: /* equality: equality NE relational  */
#line 298 "parser.y"
                             {
        (yyval.node) = ast_new(AST_BINARY_EXPR, (yylsp[-1]).first_line);
        (yyval.node)->op = (char *)"!=";
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 1729 "parser.c"
    break;

  case 46: /* relational: additive  */
#line 307 "parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1735 "parser.c"
    break;

  case 47: /* relational: relational LT additive  */
#line 308 "parser.y"
                             {
        (yyval.node) = ast_new(AST_BINARY_EXPR, (yylsp[-1]).first_line);
        (yyval.node)->op = (char *)"<";
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 1746 "parser.c"
    break;

  case 48: /* relational: relational GT additive  */
#line 314 "parser.y"
                             {
        (yyval.node) = ast_new(AST_BINARY_EXPR, (yylsp[-1]).first_line);
        (yyval.node)->op = (char *)">";
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 1757 "parser.c"
    break;

  case 49: /* relational: relational LE additive  */
#line 320 "parser.y"
                             {
        (yyval.node) = ast_new(AST_BINARY_EXPR, (yylsp[-1]).first_line);
        (yyval.node)->op = (char *)"<=";
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 1768 "parser.c"
    break;

  case 50: /* relational: relational GE additive  */
#line 326 "parser.y"
                             {
        (yyval.node) = ast_new(AST_BINARY_EXPR, (yylsp[-1]).first_line);
        (yyval.node)->op = (char *)">=";
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 1779 "parser.c"
    break;

  case 51: /* additive: multiplicative  */
#line 335 "parser.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1785 "parser.c"
    break;

  case 52: /* additive: additive PLUS multiplicative  */
#line 336 "parser.y"
                                   {
        (yyval.node) = ast_new(AST_BINARY_EXPR, (yylsp[-1]).first_line);
        (yyval.node)->op = (char *)"+";
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 1796 "parser.c"
    break;

  case 53: /* additive: additive MINUS multiplicative  */
#line 342 "parser.y"
                                    {
        (yyval.node) = ast_new(AST_BINARY_EXPR, (yylsp[-1]).first_line);
        (yyval.node)->op = (char *)"-";
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 1807 "parser.c"
    break;

  case 54: /* multiplicative: unary  */
#line 351 "parser.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1813 "parser.c"
    break;

  case 55: /* multiplicative: multiplicative MULTIPLY unary  */
#line 352 "parser.y"
                                    {
        (yyval.node) = ast_new(AST_BINARY_EXPR, (yylsp[-1]).first_line);
        (yyval.node)->op = (char *)"*";
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 1824 "parser.c"
    break;

  case 56: /* multiplicative: multiplicative DIVIDE unary  */
#line 358 "parser.y"
                                  {
        (yyval.node) = ast_new(AST_BINARY_EXPR, (yylsp[-1]).first_line);
        (yyval.node)->op = (char *)"/";
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 1835 "parser.c"
    break;

  case 57: /* unary: NOT unary  */
#line 367 "parser.y"
                {
        (yyval.node) = ast_new(AST_UNARY_EXPR, (yylsp[-1]).first_line);
        (yyval.node)->op = (char *)"not";
        (yyval.node)->left = (yyvsp[0].node);
    }
#line 1845 "parser.c"
    break;

  case 58: /* unary: MINUS unary  */
#line 372 "parser.y"
                  {
        (yyval.node) = ast_new(AST_UNARY_EXPR, (yylsp[-1]).first_line);
        (yyval.node)->op = (char *)"-";
        (yyval.node)->left = (yyvsp[0].node);
    }
#line 1855 "parser.c"
    break;

  case 59: /* unary: primary  */
#line 377 "parser.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1861 "parser.c"
    break;

  case 60: /* primary: NUMBER_LITERAL  */
#line 381 "parser.y"
                     {
        (yyval.node) = ast_new(AST_NUMBER_LITERAL, (yylsp[0]).first_line);
        (yyval.node)->num_value = (yyvsp[0].num);
    }
#line 1870 "parser.c"
    break;

  case 61: /* primary: STRING_LITERAL  */
#line 385 "parser.y"
                     {
        (yyval.node) = ast_new(AST_STRING_LITERAL, (yylsp[0]).first_line);
        (yyval.node)->str_value = (yyvsp[0].text);
    }
#line 1879 "parser.c"
    break;

  case 62: /* primary: TRUE  */
#line 389 "parser.y"
           {
        (yyval.node) = ast_new(AST_BOOLEAN_LITERAL, (yylsp[0]).first_line);
        (yyval.node)->bool_value = 1;
    }
#line 1888 "parser.c"
    break;

  case 63: /* primary: FALSE  */
#line 393 "parser.y"
            {
        (yyval.node) = ast_new(AST_BOOLEAN_LITERAL, (yylsp[0]).first_line);
        (yyval.node)->bool_value = 0;
    }
#line 1897 "parser.c"
    break;

  case 64: /* primary: ID  */
#line 397 "parser.y"
         {
        (yyval.node) = ast_new(AST_IDENTIFIER, (yylsp[0]).first_line);
        (yyval.node)->name = (yyvsp[0].text);
    }
#line 1906 "parser.c"
    break;

  case 65: /* primary: function_call  */
#line 401 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1912 "parser.c"
    break;

  case 66: /* primary: LPAREN expr RPAREN  */
#line 402 "parser.y"
                         { (yyval.node) = (yyvsp[-1].node); }
#line 1918 "parser.c"
    break;

  case 67: /* function_call: ID LPAREN opt_expr_list RPAREN  */
#line 406 "parser.y"
                                     {
        (yyval.node) = ast_new(AST_FUNCTION_CALL, (yylsp[-3]).first_line);
        (yyval.node)->name = (yyvsp[-3].text);
        (yyval.node)->args = (yyvsp[-1].list);
    }
#line 1928 "parser.c"
    break;

  case 68: /* opt_expr_list: %empty  */
#line 414 "parser.y"
                  { (yyval.list).items = NULL; (yyval.list).size = 0; (yyval.list).capacity = 0; }
#line 1934 "parser.c"
    break;

  case 69: /* opt_expr_list: expr_list  */
#line 415 "parser.y"
                { (yyval.list) = (yyvsp[0].list); }
#line 1940 "parser.c"
    break;

  case 70: /* expr_list: expr  */
#line 419 "parser.y"
           {
        (yyval.list).items = NULL; (yyval.list).size = 0; (yyval.list).capacity = 0;
        ast_list_append(&(yyval.list), (yyvsp[0].node));
    }
#line 1949 "parser.c"
    break;

  case 71: /* expr_list: expr_list COMMA expr  */
#line 423 "parser.y"
                           {
        ast_list_append(&(yyvsp[-2].list), (yyvsp[0].node));
        (yyval.list) = (yyvsp[-2].list);
    }
#line 1958 "parser.c"
    break;


#line 1962 "parser.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
                      yytoken, &yylval, &yylloc);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 429 "parser.y"


void yyerror(const char *msg)
{
    error_report(ERROR_SYNTAX, yylineno, "%s", msg);
}

AstNode *parse_program(void)
{
    wrapper_program = NULL;
    yyparse();
    return wrapper_program;
}
