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
     COMMA = 258,
     NUMBER = 259,
     STRING = 260,
     TK_WALL = 261,
     TK_GATE = 262,
     TK_EARTH = 263,
     TK_WEB = 264,
     TK_BOARD = 265,
     TK_STEP = 266,
     TK_HOST = 267,
     TK_DOC = 268,
     TK_MIRAGE = 269,
     TK_THING = 270,
     TK_END = 271,
     TK_BOX_SIZE = 272,
     TK_SPHER_SIZE = 273,
     TK_TOR_SIZE = 274,
     TK_TOR_SIZE2 = 275,
     TK_DIFFUSE = 276,
     TK_AMBIENT = 277,
     TK_SPECULAR = 278,
     TK_EMISSION = 279,
     TK_SHININESS = 280,
     TEX_XP = 281,
     TEX_YP = 282,
     TEX_ZP = 283,
     TEX_XN = 284,
     TEX_YN = 285,
     TEX_ZN = 286,
     SPHER_TEX = 287,
     TLNT = 288
   };
#endif
/* Tokens.  */
#define COMMA 258
#define NUMBER 259
#define STRING 260
#define TK_WALL 261
#define TK_GATE 262
#define TK_EARTH 263
#define TK_WEB 264
#define TK_BOARD 265
#define TK_STEP 266
#define TK_HOST 267
#define TK_DOC 268
#define TK_MIRAGE 269
#define TK_THING 270
#define TK_END 271
#define TK_BOX_SIZE 272
#define TK_SPHER_SIZE 273
#define TK_TOR_SIZE 274
#define TK_TOR_SIZE2 275
#define TK_DIFFUSE 276
#define TK_AMBIENT 277
#define TK_SPECULAR 278
#define TK_EMISSION 279
#define TK_SHININESS 280
#define TEX_XP 281
#define TEX_YP 282
#define TEX_ZP 283
#define TEX_XN 284
#define TEX_YN 285
#define TEX_ZN 286
#define SPHER_TEX 287
#define TLNT 288




/* Copy the first part of user declarations.  */
#line 1 "vred.y"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "input.hpp"

#define YYDEBUG 1
//extern int yy_flex_debug;  // pour le debogage

extern FILE* yyin;

int yylineno;
extern int yyerror(char*);
extern int yylex();

void mix_texture(Tex* t1, Tex* t2);
void mix_app(App* a1, App* a2);
void moveCenter(Vect& center, Vect& size);
float deg(const float);

int curtype;

/* le groupe racine cree */ 
Group *gr;
/* pour obtenir les valeurs par defaut */
App *a;

struct structBoxProps {
  Vect* bp_box_size;
  Tex* bp_box_tex;
  App* bp_app;
};

struct structSpherProps {
  float sp_spher_size;
  char* sp_spher_tex;
  App* sp_app;
};

void free_box_props (struct structBoxProps *bp)
{
  if (bp->bp_box_size != NULL) delete(bp->bp_box_size);
  if (bp->bp_box_tex != NULL) delete(bp->bp_box_tex);
  if (bp->bp_app != NULL) delete(bp->bp_app);
  free(bp);
}

void free_spher_props (struct structSpherProps *sp)
{
  if (sp->sp_spher_tex != NULL) free(sp->sp_spher_tex);
  if (sp->sp_app != NULL) delete(sp->sp_app);
  free(sp);
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
typedef union YYSTYPE
#line 58 "vred.y"
{
  float dval;
  char*  sval;
  float* dpval;
  Vect* vectval;
  Tex*  texval;
  struct structBoxProps* bpval;
  struct structSpherProps* spval;
  App* appval;
}
/* Line 193 of yacc.c.  */
#line 230 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 243 "y.tab.c"

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
#define YYFINAL  55
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   123

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  34
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  79
/* YYNRULES -- Number of states.  */
#define YYNSTATES  138

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   288

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
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
      25,    26,    27,    28,    29,    30,    31,    32,    33
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     8,    11,    13,    15,    17,    19,
      21,    23,    25,    27,    29,    31,    34,    36,    39,    42,
      45,    47,    50,    55,    58,    60,    63,    66,    69,    71,
      74,    78,    81,    83,    86,    89,    92,    94,    97,   100,
     103,   105,   108,   112,   115,   117,   120,   124,   127,   129,
     132,   135,   138,   140,   143,   146,   152,   156,   160,   162,
     164,   168,   172,   178,   184,   191,   194,   196,   200,   202,
     204,   206,   208,   210,   212,   214,   218,   224,   230,   236
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      35,     0,    -1,    36,    16,    -1,    37,    -1,    36,    37,
      -1,    38,    -1,    41,    -1,    44,    -1,    47,    -1,    50,
      -1,    53,    -1,    56,    -1,    59,    -1,    62,    -1,    65,
      -1,     6,    39,    -1,    40,    -1,    39,    40,    -1,    70,
      68,    -1,     7,    42,    -1,    43,    -1,    42,    43,    -1,
      70,     5,     5,    68,    -1,     8,    45,    -1,    46,    -1,
      45,    46,    -1,    70,    69,    -1,     9,    48,    -1,    49,
      -1,    48,    49,    -1,    70,     5,    68,    -1,    10,    51,
      -1,    52,    -1,    51,    52,    -1,    70,    68,    -1,    11,
      54,    -1,    55,    -1,    54,    55,    -1,    70,    68,    -1,
      12,    57,    -1,    58,    -1,    57,    58,    -1,    70,    33,
      68,    -1,    13,    60,    -1,    61,    -1,    60,    61,    -1,
      70,     5,    68,    -1,    14,    63,    -1,    64,    -1,    63,
      64,    -1,    70,    68,    -1,    15,    66,    -1,    67,    -1,
      66,    67,    -1,    70,    68,    -1,    71,     3,    73,     3,
      75,    -1,    71,     3,    73,    -1,    71,     3,    75,    -1,
      71,    -1,    72,    -1,    72,     3,    32,    -1,    72,     3,
      75,    -1,    72,     3,    32,     3,    75,    -1,     4,     4,
       4,     4,     4,    -1,    17,     4,     3,     4,     3,     4,
      -1,    18,     4,    -1,    74,    -1,    73,     3,    74,    -1,
      26,    -1,    29,    -1,    27,    -1,    30,    -1,    28,    -1,
      31,    -1,    76,    -1,    75,     3,    76,    -1,    22,     3,
       4,     3,     4,    -1,    21,     3,     4,     3,     4,    -1,
      25,     3,     4,     3,     4,    -1,    23,     3,     4,     3,
       4,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    91,    91,    93,    93,    96,    97,    98,    99,   100,
     101,   102,   103,   105,   106,   111,   112,   112,   113,   126,
     127,   127,   128,   143,   144,   144,   145,   157,   158,   158,
     159,   173,   174,   174,   175,   188,   189,   189,   190,   203,
     204,   204,   205,   219,   220,   220,   221,   235,   236,   236,
     237,   250,   251,   251,   252,   268,   277,   286,   295,   306,
     315,   324,   333,   345,   357,   362,   368,   369,   376,   377,
     378,   379,   380,   381,   384,   385,   392,   396,   402,   408
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "COMMA", "NUMBER", "STRING", "TK_WALL",
  "TK_GATE", "TK_EARTH", "TK_WEB", "TK_BOARD", "TK_STEP", "TK_HOST",
  "TK_DOC", "TK_MIRAGE", "TK_THING", "TK_END", "TK_BOX_SIZE",
  "TK_SPHER_SIZE", "TK_TOR_SIZE", "TK_TOR_SIZE2", "TK_DIFFUSE",
  "TK_AMBIENT", "TK_SPECULAR", "TK_EMISSION", "TK_SHININESS", "TEX_XP",
  "TEX_YP", "TEX_ZP", "TEX_XN", "TEX_YN", "TEX_ZN", "SPHER_TEX", "TLNT",
  "$accept", "start", "solid", "solid_single", "wall_section", "wall",
  "wall_single", "gate_section", "gate", "gate_single", "earth_section",
  "earth", "earth_single", "web_section", "web", "web_single",
  "board_section", "board", "board_single", "step_section", "step",
  "step_single", "host_section", "host", "host_single", "doc_section",
  "doc", "doc_single", "mirage_section", "mirage", "mirage_single",
  "thing_section", "thing", "thing_single", "box_props", "spher_props",
  "pos_ang", "box_size", "spher_size", "box_tex", "box_tex_single", "app",
  "app_single", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    34,    35,    36,    36,    37,    37,    37,    37,    37,
      37,    37,    37,    37,    37,    38,    39,    39,    40,    41,
      42,    42,    43,    44,    45,    45,    46,    47,    48,    48,
      49,    50,    51,    51,    52,    53,    54,    54,    55,    56,
      57,    57,    58,    59,    60,    60,    61,    62,    63,    63,
      64,    65,    66,    66,    67,    68,    68,    68,    68,    69,
      69,    69,    69,    70,    71,    72,    73,    73,    74,    74,
      74,    74,    74,    74,    75,    75,    76,    76,    76,    76
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     2,     2,     2,
       1,     2,     4,     2,     1,     2,     2,     2,     1,     2,
       3,     2,     1,     2,     2,     2,     1,     2,     2,     2,
       1,     2,     3,     2,     1,     2,     3,     2,     1,     2,
       2,     2,     1,     2,     2,     5,     3,     3,     1,     1,
       3,     3,     5,     5,     6,     2,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     3,     5,     5,     5,     5
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     3,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,     0,    15,    16,     0,    19,    20,
       0,    23,    24,     0,    27,    28,     0,    31,    32,     0,
      35,    36,     0,    39,    40,     0,    43,    44,     0,    47,
      48,     0,    51,    52,     0,     1,     2,     4,     0,    17,
       0,    18,    58,    21,     0,    25,     0,    26,    59,    29,
       0,    33,    34,    37,    38,    41,     0,    45,     0,    49,
      50,    53,    54,     0,     0,     0,     0,    65,     0,    30,
      42,    46,     0,     0,     0,     0,     0,     0,    68,    70,
      72,    69,    71,    73,    56,    66,    57,    74,    22,    60,
      61,    63,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    67,    55,    75,    62,    64,
       0,     0,     0,     0,    77,    76,    79,    78
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    11,    12,    13,    14,    25,    26,    15,    28,    29,
      16,    31,    32,    17,    34,    35,    18,    37,    38,    19,
      40,    41,    20,    43,    44,    21,    46,    47,    22,    49,
      50,    23,    52,    53,    61,    67,    27,    62,    68,   104,
     105,   106,   107
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -75
static const yytype_int8 yypact[] =
{
      73,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,    11,    51,   -75,   -75,   -75,   -75,   -75,   -75,   -75,
     -75,   -75,   -75,   -75,     9,     6,   -75,     2,     6,   -75,
      10,     6,   -75,     4,     6,   -75,    18,     6,   -75,     2,
       6,   -75,     2,     6,   -75,    -8,     6,   -75,    23,     6,
     -75,     2,     6,   -75,     2,   -75,   -75,   -75,    26,   -75,
      27,   -75,    31,   -75,    28,   -75,    32,   -75,    34,   -75,
       2,   -75,   -75,   -75,   -75,   -75,     2,   -75,     2,   -75,
     -75,   -75,   -75,    35,    39,    47,     2,   -75,    -5,   -75,
     -75,   -75,    45,    50,    68,    86,    87,    88,   -75,   -75,
     -75,   -75,   -75,   -75,    89,   -75,    90,   -75,   -75,    91,
      90,   -75,    92,    93,    94,    95,    96,    47,    30,    30,
      97,    99,   100,   101,   102,   -75,    90,   -75,    90,   -75,
     103,   104,   105,   106,   -75,   -75,   -75,   -75
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -75,   -75,   -75,    84,   -75,   -75,    81,   -75,   -75,    83,
     -75,   -75,    82,   -75,   -75,    78,   -75,   -75,    77,   -75,
     -75,    75,   -75,   -75,    74,   -75,   -75,    70,   -75,   -75,
      69,   -75,   -75,    67,   -30,   -75,    -2,   -75,   -75,   -75,
       3,   -74,     5
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      30,    33,    36,    39,    42,    45,    48,    51,    54,    72,
      24,    55,    74,    58,   110,    64,    94,    95,    96,    60,
      97,    80,    66,    70,    82,    76,    30,   109,    78,    33,
      83,    84,    36,    86,    85,    39,    87,    88,    42,    92,
      89,    45,    93,   126,    48,   128,    90,    51,    91,   111,
      54,    94,    95,    96,   112,    97,   108,     1,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    56,    94,    95,
      96,   113,    97,    98,    99,   100,   101,   102,   103,     1,
       2,     3,     4,     5,     6,     7,     8,     9,    10,   114,
     115,   116,   117,   118,   119,   120,    57,   121,   122,   123,
     124,   129,   130,   131,   132,   133,    59,   134,   135,   136,
     137,    63,    69,    65,    71,    73,    77,    75,    79,    81,
     125,     0,     0,   127
};

static const yytype_int8 yycheck[] =
{
       2,     3,     4,     5,     6,     7,     8,     9,    10,    39,
       4,     0,    42,     4,    88,     5,    21,    22,    23,    17,
      25,    51,    18,     5,    54,    33,    28,    32,     5,    31,
       4,     4,    34,     5,     3,    37,     4,     3,    40,     4,
      70,    43,     3,   117,    46,   119,    76,    49,    78,     4,
      52,    21,    22,    23,     4,    25,    86,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    21,    22,
      23,     3,    25,    26,    27,    28,    29,    30,    31,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,     3,
       3,     3,     3,     3,     3,     3,    12,     4,     4,     4,
       4,     4,     3,     3,     3,     3,    25,     4,     4,     4,
       4,    28,    34,    31,    37,    40,    46,    43,    49,    52,
     117,    -1,    -1,   118
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    35,    36,    37,    38,    41,    44,    47,    50,    53,
      56,    59,    62,    65,     4,    39,    40,    70,    42,    43,
      70,    45,    46,    70,    48,    49,    70,    51,    52,    70,
      54,    55,    70,    57,    58,    70,    60,    61,    70,    63,
      64,    70,    66,    67,    70,     0,    16,    37,     4,    40,
      17,    68,    71,    43,     5,    46,    18,    69,    72,    49,
       5,    52,    68,    55,    68,    58,    33,    61,     5,    64,
      68,    67,    68,     4,     4,     3,     5,     4,     3,    68,
      68,    68,     4,     3,    21,    22,    23,    25,    26,    27,
      28,    29,    30,    31,    73,    74,    75,    76,    68,    32,
      75,     4,     4,     3,     3,     3,     3,     3,     3,     3,
       3,     4,     4,     4,     4,    74,    75,    76,    75,     4,
       3,     3,     3,     3,     4,     4,     4,     4
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
        case 18:
#line 113 "vred.y"
    {
  Vect center((yyvsp[(1) - (2)].dpval)[0], (yyvsp[(1) - (2)].dpval)[1], (yyvsp[(1) - (2)].dpval)[2]);
  Vect size(*((yyvsp[(2) - (2)].bpval)->bp_box_size));
  size *= 2;
  Vect o(0,0,deg((yyvsp[(1) - (2)].dpval)[3]));
  Tex* t = (yyvsp[(2) - (2)].bpval)->bp_box_tex;
  App* a = (yyvsp[(2) - (2)].bpval)->bp_app;
  Wall *w = new Wall("myWall", center, o, size, TEXTURED, Color::white, *t, *a);
  gr->add(w);
  free_box_props((yyvsp[(2) - (2)].bpval)); // libere aussi les objets pointes
  delete[]((yyvsp[(1) - (2)].dpval));
}
    break;

  case 22:
#line 128 "vred.y"
    {
  Vect center((yyvsp[(1) - (4)].dpval)[0], (yyvsp[(1) - (4)].dpval)[1], (yyvsp[(1) - (4)].dpval)[2]);
  Vect size(*((yyvsp[(4) - (4)].bpval)->bp_box_size));
  size *= 2;
  Vect o(0,0,deg((yyvsp[(1) - (4)].dpval)[3]));
  Tex* t = (yyvsp[(4) - (4)].bpval)->bp_box_tex;
  App* a = (yyvsp[(4) - (4)].bpval)->bp_app;
  Gate *g = new Gate("myWall", center, o, size, TEXTURED, Color::white, *t, *a,
		     (yyvsp[(2) - (4)].sval), (yyvsp[(3) - (4)].sval));
  gr->add(g);
  cout << *g;
  free_box_props((yyvsp[(4) - (4)].bpval));
  delete[]((yyvsp[(1) - (4)].dpval));
}
    break;

  case 26:
#line 145 "vred.y"
    {
  Vect center((yyvsp[(1) - (2)].dpval)[0], (yyvsp[(1) - (2)].dpval)[1], (yyvsp[(1) - (2)].dpval)[2]);
  Vect size((yyvsp[(2) - (2)].spval)->sp_spher_size,(yyvsp[(2) - (2)].spval)->sp_spher_size,(yyvsp[(2) - (2)].spval)->sp_spher_size);
  App* a = (yyvsp[(2) - (2)].spval)->sp_app;
  Earth *s = new Earth("mySphere", center, Vect::null, size, TEXTURED,
		       Color::white, Tex((yyvsp[(2) - (2)].spval)->sp_spher_tex), *a);
  gr->add(s);
  cout << *s;
  free_spher_props((yyvsp[(2) - (2)].spval));
  delete[]((yyvsp[(1) - (2)].dpval));
}
    break;

  case 30:
#line 159 "vred.y"
    {
  Vect center((yyvsp[(1) - (3)].dpval)[0], (yyvsp[(1) - (3)].dpval)[1], (yyvsp[(1) - (3)].dpval)[2]);
  Vect size(*((yyvsp[(3) - (3)].bpval)->bp_box_size));
  size *= 2;
  Vect o(0,0,deg((yyvsp[(1) - (3)].dpval)[3]));
  Tex* t = (yyvsp[(3) - (3)].bpval)->bp_box_tex;
  App* a = (yyvsp[(3) - (3)].bpval)->bp_app;
  Web *w = new Web("myWall", center, o, size, TEXTURED, Color::white, *t, *a,
                   (yyvsp[(2) - (3)].sval));
  gr->add(w);
  free_box_props((yyvsp[(3) - (3)].bpval));
  delete[]((yyvsp[(1) - (3)].dpval));
}
    break;

  case 34:
#line 175 "vred.y"
    {
  Vect center((yyvsp[(1) - (2)].dpval)[0], (yyvsp[(1) - (2)].dpval)[1], (yyvsp[(1) - (2)].dpval)[2]);
  Vect size(*((yyvsp[(2) - (2)].bpval)->bp_box_size));
  size *= 2;
  Vect o(0,0,deg((yyvsp[(1) - (2)].dpval)[3]));
  Tex* t = (yyvsp[(2) - (2)].bpval)->bp_box_tex;
  App* a = (yyvsp[(2) - (2)].bpval)->bp_app;
  Board *b = new Board("myWall", center, o, size, TEXTURED, Color::white, *t, *a);
  gr->add(b);
  free_box_props((yyvsp[(2) - (2)].bpval));
  delete[]((yyvsp[(1) - (2)].dpval));
}
    break;

  case 38:
#line 190 "vred.y"
    {
  Vect center((yyvsp[(1) - (2)].dpval)[0], (yyvsp[(1) - (2)].dpval)[1], (yyvsp[(1) - (2)].dpval)[2]);
  Vect size(*((yyvsp[(2) - (2)].bpval)->bp_box_size));
  size *= 2;
  Vect o(0,0,deg((yyvsp[(1) - (2)].dpval)[3]));
  Tex* t = (yyvsp[(2) - (2)].bpval)->bp_box_tex;
  App* a = (yyvsp[(2) - (2)].bpval)->bp_app;
  Step *s = new Step("myWall", center, o, size, TEXTURED, Color::white, *t, *a);
  gr->add(s);
  free_box_props((yyvsp[(2) - (2)].bpval));
  delete[]((yyvsp[(1) - (2)].dpval));
}
    break;

  case 42:
#line 205 "vred.y"
    {
  Vect center((yyvsp[(1) - (3)].dpval)[0], (yyvsp[(1) - (3)].dpval)[1], (yyvsp[(1) - (3)].dpval)[2]);
  Vect size(*((yyvsp[(3) - (3)].bpval)->bp_box_size));
  size *= 2;
  Vect o(0,0,deg((yyvsp[(1) - (3)].dpval)[3]));
  Tex* t = (yyvsp[(3) - (3)].bpval)->bp_box_tex == NULL ? new Tex() : new Tex(*((yyvsp[(3) - (3)].bpval)->bp_box_tex));
  App* a = (yyvsp[(3) - (3)].bpval)->bp_app == NULL ? new App() : new App(*((yyvsp[(3) - (3)].bpval)->bp_app));
  Host *h = new Host("myWall", center, o, size, TEXTURED, Color::white, *t, *a,
                     (yyvsp[(2) - (3)].sval));
  gr->add(h);
  free_box_props((yyvsp[(3) - (3)].bpval));
  delete[]((yyvsp[(1) - (3)].dpval));
}
    break;

  case 46:
#line 221 "vred.y"
    {
  Vect center((yyvsp[(1) - (3)].dpval)[0], (yyvsp[(1) - (3)].dpval)[1], (yyvsp[(1) - (3)].dpval)[2]);
  Vect size(*((yyvsp[(3) - (3)].bpval)->bp_box_size));
  size *= 2;
  Vect o(0,0,deg((yyvsp[(1) - (3)].dpval)[3]));
  Tex* t = (yyvsp[(3) - (3)].bpval)->bp_box_tex;
  App* a = (yyvsp[(3) - (3)].bpval)->bp_app;
  Doc *d = new Doc("myWall", center, o, size, TEXTURED, Color::white, *t, *a,
                   (yyvsp[(2) - (3)].sval));
  gr->add(d);
  free_box_props((yyvsp[(3) - (3)].bpval));
  delete[]((yyvsp[(1) - (3)].dpval));
}
    break;

  case 50:
#line 237 "vred.y"
    {
  Vect center((yyvsp[(1) - (2)].dpval)[0], (yyvsp[(1) - (2)].dpval)[1], (yyvsp[(1) - (2)].dpval)[2]);
  Vect size(*((yyvsp[(2) - (2)].bpval)->bp_box_size));
  size *= 2;
  Vect o(0,0,deg((yyvsp[(1) - (2)].dpval)[3]));
  Tex* t = (yyvsp[(2) - (2)].bpval)->bp_box_tex;
  App* a = (yyvsp[(2) - (2)].bpval)->bp_app;
  Mirage *m = new Mirage("myWall", center, o, size, TEXTURED, Color::white, *t, *a);
  gr->add(m);
  free_box_props((yyvsp[(2) - (2)].bpval));
  delete[]((yyvsp[(1) - (2)].dpval));
}
    break;

  case 54:
#line 252 "vred.y"
    {
  Vect center((yyvsp[(1) - (2)].dpval)[0], (yyvsp[(1) - (2)].dpval)[1], (yyvsp[(1) - (2)].dpval)[2]);
  Vect size(*((yyvsp[(2) - (2)].bpval)->bp_box_size));
  size *= 2;
  Vect o(0,0,deg((yyvsp[(1) - (2)].dpval)[3]));
  Tex* t = (yyvsp[(2) - (2)].bpval)->bp_box_tex;
  App* a = (yyvsp[(2) - (2)].bpval)->bp_app;
  Thing *th = new Thing("myWall", center, o, size, TEXTURED, Color::white, *t, *a);
  gr->add(th);
  free_box_props((yyvsp[(2) - (2)].bpval));
  delete[]((yyvsp[(1) - (2)].dpval));
}
    break;

  case 55:
#line 268 "vred.y"
    {
      struct structBoxProps *res;
      res = (struct structBoxProps *)malloc(sizeof(struct structBoxProps));
      res->bp_box_size = (yyvsp[(1) - (5)].vectval) ;
      res->bp_box_tex = (yyvsp[(3) - (5)].texval);
      res->bp_app = (yyvsp[(5) - (5)].appval);
      
      (yyval.bpval) = res;
   }
    break;

  case 56:
#line 277 "vred.y"
    {
      struct structBoxProps *res;
      res = (struct structBoxProps *)malloc(sizeof(struct structBoxProps));
      res->bp_box_size = (yyvsp[(1) - (3)].vectval) ;
      res->bp_box_tex = (yyvsp[(3) - (3)].texval);
      res->bp_app = new App();
      
      (yyval.bpval) = res;
   }
    break;

  case 57:
#line 286 "vred.y"
    {
      struct structBoxProps *res;
      res = (struct structBoxProps *)malloc(sizeof(struct structBoxProps));
      res->bp_box_size = (yyvsp[(1) - (3)].vectval) ;
      res->bp_box_tex = new Tex();
      res->bp_app = (yyvsp[(3) - (3)].appval);
      
      (yyval.bpval) = res;
   }
    break;

  case 58:
#line 295 "vred.y"
    { 
      struct structBoxProps *res;
      res = (struct structBoxProps *)malloc(sizeof(struct structBoxProps));
      res->bp_box_size = (yyvsp[(1) - (1)].vectval) ;
      res->bp_box_tex = new Tex();
      res->bp_app = new App();
      
      (yyval.bpval) = res;
    }
    break;

  case 59:
#line 306 "vred.y"
    {
      struct structSpherProps *res;
      res = (struct structSpherProps *)malloc(sizeof(struct structSpherProps));
      res->sp_spher_size = (yyvsp[(1) - (1)].dval) ;
      res->sp_spher_tex = NULL;
      res->sp_app = new App();
      
      (yyval.spval) = res;
   }
    break;

  case 60:
#line 315 "vred.y"
    {
      struct structSpherProps *res;
      res = (struct structSpherProps *)malloc(sizeof(struct structSpherProps));
      res->sp_spher_size = (yyvsp[(1) - (3)].dval) ;
      res->sp_spher_tex = (yyvsp[(3) - (3)].sval);
      res->sp_app = new App();
      
      (yyval.spval) = res;
   }
    break;

  case 61:
#line 324 "vred.y"
    {
      struct structSpherProps *res;
      res = (struct structSpherProps *)malloc(sizeof(struct structSpherProps));
      res->sp_spher_size = (yyvsp[(1) - (3)].dval) ;
      res->sp_spher_tex = NULL;
      res->sp_app = (yyvsp[(3) - (3)].appval);
      
      (yyval.spval) = res;
   }
    break;

  case 62:
#line 333 "vred.y"
    {
      struct structSpherProps *res;
      res = (struct structSpherProps *)malloc(sizeof(struct structSpherProps));
      res->sp_spher_size = (yyvsp[(1) - (5)].dval) ;
      res->sp_spher_tex = (yyvsp[(3) - (5)].sval);
      res->sp_app = (yyvsp[(5) - (5)].appval);
      
      (yyval.spval) = res;
   }
    break;

  case 63:
#line 346 "vred.y"
    {
  float *res = new float[5];
  res[0] = (yyvsp[(1) - (5)].dval); 
  res[1] = (yyvsp[(2) - (5)].dval); 
  res[2] = (yyvsp[(3) - (5)].dval); 
  res[3] = (yyvsp[(4) - (5)].dval); 
  res[4] = (yyvsp[(5) - (5)].dval); 

  (yyval.dpval) = res;
}
    break;

  case 64:
#line 358 "vred.y"
    {
  (yyval.vectval) = new Vect((yyvsp[(2) - (6)].dval), (yyvsp[(4) - (6)].dval), (yyvsp[(6) - (6)].dval));
}
    break;

  case 65:
#line 363 "vred.y"
    {
  (yyval.dval) = (yyvsp[(2) - (2)].dval);
}
    break;

  case 66:
#line 368 "vred.y"
    { (yyval.texval) = (yyvsp[(1) - (1)].texval); }
    break;

  case 67:
#line 369 "vred.y"
    { 
    mix_texture((yyvsp[(3) - (3)].texval), (yyvsp[(1) - (3)].texval));
    delete((yyvsp[(3) - (3)].texval));
    (yyval.texval) = (yyvsp[(1) - (3)].texval);
}
    break;

  case 68:
#line 376 "vred.y"
    { (yyval.texval) = new Tex((yyvsp[(1) - (1)].sval), NULL, NULL, NULL, NULL, NULL); }
    break;

  case 69:
#line 377 "vred.y"
    { (yyval.texval) = new Tex(NULL, (yyvsp[(1) - (1)].sval), NULL, NULL, NULL, NULL); }
    break;

  case 70:
#line 378 "vred.y"
    { (yyval.texval) = new Tex(NULL, NULL, (yyvsp[(1) - (1)].sval), NULL, NULL, NULL); }
    break;

  case 71:
#line 379 "vred.y"
    { (yyval.texval) = new Tex(NULL, NULL, NULL, (yyvsp[(1) - (1)].sval), NULL, NULL); }
    break;

  case 72:
#line 380 "vred.y"
    { (yyval.texval) = new Tex(NULL, NULL, NULL, NULL, (yyvsp[(1) - (1)].sval), NULL); }
    break;

  case 73:
#line 381 "vred.y"
    { (yyval.texval) = new Tex(NULL, NULL, NULL, NULL, NULL, (yyvsp[(1) - (1)].sval)); }
    break;

  case 74:
#line 384 "vred.y"
    { (yyval.appval) = (yyvsp[(1) - (1)].appval); }
    break;

  case 75:
#line 385 "vred.y"
    {
    mix_app((yyvsp[(3) - (3)].appval), (yyvsp[(1) - (3)].appval));
    delete((yyvsp[(3) - (3)].appval));
    (yyval.appval) = (yyvsp[(1) - (3)].appval);
}
    break;

  case 76:
#line 392 "vred.y"
    {
    Color v((yyvsp[(1) - (5)].dval), (yyvsp[(3) - (5)].dval), (yyvsp[(5) - (5)].dval), 1.0);
    (yyval.appval) = new App(v);
  }
    break;

  case 77:
#line 396 "vred.y"
    {
    Color v((yyvsp[(1) - (5)].dval), (yyvsp[(3) - (5)].dval), (yyvsp[(5) - (5)].dval), 1.0);
    App *a = new App();
    a->setDiffuse(v);
    (yyval.appval) = a;
  }
    break;

  case 78:
#line 402 "vred.y"
    {
    Color v((yyvsp[(1) - (5)].dval), (yyvsp[(3) - (5)].dval), (yyvsp[(5) - (5)].dval), 1.0);
    App *a = new App();
    a->setShininess(v);
    (yyval.appval) = a;
  }
    break;

  case 79:
#line 408 "vred.y"
    {
    Color v((yyvsp[(1) - (5)].dval), (yyvsp[(3) - (5)].dval), (yyvsp[(5) - (5)].dval), 1.0);
    App *a = new App();
    a->setSpecular(v);
    (yyval.appval) = a;
  }
    break;


/* Line 1267 of yacc.c.  */
#line 1956 "y.tab.c"
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


#line 415 "vred.y"


/*
  Mixe 2 textures : une texture t1 a un seul element, et une texture t2 quelconque.
  ex : t1(a1, NULL, ..., NULL) et t2(b1, ..., b6)
  resultat : t2(a1, b2, ..., b6)
 */
void mix_texture(Tex* t1, Tex* t2)
{
  if (t1->getTex_xp() != NULL) {
    t2->setTex_xp(t1->getTex_xp());
    return ;
  }
  if (t1->getTex_xn() != NULL) {
    t2->setTex_xn(t1->getTex_xn());
    return ;
  }
  if (t1->getTex_yp() != NULL) {
    t2->setTex_yp(t1->getTex_yp());
    return ;
  }
  if (t1->getTex_yn() != NULL) {
    t2->setTex_yn(t1->getTex_yn());
    return ;
  }
  if (t1->getTex_zp() != NULL) {
    t2->setTex_zp(t1->getTex_zp());
    return;
  }
  if (t1->getTex_zn() != NULL) {
    t2->setTex_zn(t1->getTex_zn());
    return ;
  }
  return ;
}

void mix_app(App* a1, App* a2)
{
  if (!(a1->getDiffuse() == a->getDiffuse())) {
    a2->setDiffuse(a1->getDiffuse());
    return ;
  }
  if (!(a1->getAmbient() == a->getAmbient())) {
    a2->setAmbient(a1->getAmbient());
    return ;
  }
  if (!(a1->getSpecular() == a->getSpecular())) {
    a2->setSpecular(a1->getSpecular());
    return ;
  }
  if (!(a1->getShininess() == a->getShininess())) {
    a2->setShininess(a1->getShininess());
    return ;
  }
}

float deg(const float rad)
{
  return (rad * 180.0 / M_PI);
}

void moveCenter(Vect& center, Vect& size)
{
  center += Vect(size[0], -size[1], size[2]);
}

int fileToGroup(FILE *in, Group *gp)
{ 
  a = new App();
  //yydebug = 0;   // debogage (yacc)
  //yy_flex_debug = 0;  // pour le debogage (lex)
  // le pointeur global pr pointe vers gp, donc c'est a *gp qu'on ajoute les objects du fichier
  gr = gp;

  yyin = in;
  yylineno = 0;
  delete(a);
  return yyparse();
}

int yyerror(char *s)
{
  fprintf(stderr, "line %d:%s\n", yylineno, s);
  return 0;
}


