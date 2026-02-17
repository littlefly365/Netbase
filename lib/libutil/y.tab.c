/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 2
#define YYMINOR 0
#define YYPATCH 20260126

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#undef YYBTYACC
#define YYBTYACC 0
#define YYDEBUGSTR YYPREFIX "debug"
#define YYPREFIX "yy"

#define YYPURE 1

#line 2 "parsedate.y"
/*
**  Originally written by Steven M. Bellovin <smb@research.att.com> while
**  at the University of North Carolina at Chapel Hill.  Later tweaked by
**  a couple of people on Usenet.  Completely overhauled by Rich $alz
**  <rsalz@bbn.com> and Jim Berets <jberets@bbn.com> in August, 1990;
**
**  This grammar has 10 shift/reduce conflicts.
**
**  This code is in the public domain and has no copyright.
*/
/* SUPPRESS 287 on yaccpar_sccsid *//* Unused static variable */
/* SUPPRESS 288 on yyerrlab *//* Label unused */

#include <sys/cdefs.h>
#ifdef __RCSID
__RCSID("$NetBSD: parsedate.y,v 1.37 2022/04/23 13:02:04 christos Exp $");
#endif

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <util.h>
#include <stdlib.h>

/* NOTES on rebuilding parsedate.c (particularly for inclusion in CVS
   releases):

   We don't want to mess with all the portability hassles of alloca.
   In particular, most (all?) versions of bison will use alloca in
   their parser.  If bison works on your system (e.g. it should work
   with gcc), then go ahead and use it, but the more general solution
   is to use byacc instead of bison, which should generate a portable
   parser.  I played with adding "#define alloca dont_use_alloca", to
   give an error if the parser generator uses alloca (and thus detect
   unportable parsedate.c's), but that seems to cause as many problems
   as it solves.  */

#define EPOCH		1970
#define HOUR(x)		((time_t)((x) * 60))
#define SECSPERDAY	(24L * 60L * 60L)

#define	MAXREL	16	/* hours mins secs days weeks months years - maybe twice each ...*/

#define USE_LOCAL_TIME	99999 /* special case for Convert() and yyTimezone */

/*
**  An entry in the lexical lookup table.
*/
typedef struct _TABLE {
    const char	*name;
    int		type;
    time_t	value;
} TABLE;


/*
**  Daylight-savings mode:  on, off, or not yet known.
*/
typedef enum _DSTMODE {
    DSTon, DSToff, DSTmaybe
} DSTMODE;

/*
**  Meridian:  am, pm, or 24-hour style (plus "noon" and "midnight").
*/
typedef enum _MERIDIAN {
    MERam, MERpm, MER24, MER_NOON, MER_MN
} MERIDIAN;


struct dateinfo {
	DSTMODE	yyDSTmode;	/* DST on/off/maybe */
	time_t	yyDayOrdinal;
	time_t	yyDayNumber;
	int	yyHaveDate;
	int	yyHaveFullYear;	/* if true, year is not abbreviated. */
				/* if false, need to call AdjustYear(). */
	int	yyHaveDay;
	int	yyHaveRel;
	int	yyHaveTime;
	int	yyHaveZone;
	time_t	yyTimezone;	/* Timezone as minutes ahead/east of UTC */
	time_t	yyDay;		/* Day of month [1-31] */
	time_t	yyHour;		/* Hour of day [0-24] or [1-12] */
	time_t	yyMinutes;	/* Minute of hour [0-59] */
	time_t	yyMonth;	/* Month of year [1-12] */
	time_t	yySeconds;	/* Second of minute [0-60] */
	time_t	yyYear;		/* Year, see also yyHaveFullYear */
	MERIDIAN yyMeridian;	/* Interpret yyHour as AM/PM/24 hour clock */
	struct {
		time_t	yyRelVal;
		int	yyRelMonth;
	} yyRel[MAXREL];
};

static int RelVal(struct dateinfo *, time_t, time_t, int, int);

#define CheckRelVal(a, b, c, d, e) do {				\
		if (!RelVal((a), (b), (c), (d), (e))) {		\
			YYREJECT;				\
		}						\
	} while (0)

#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#line 110 "parsedate.y"
typedef union YYSTYPE {
    time_t		Number;
    enum _MERIDIAN	Meridian;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 143 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(struct dateinfo *param, const char **yyInput)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# ifdef YYLEX_PARAM_TYPE
#  define YYLEX_DECL() yylex(YYSTYPE *yylval, YYLEX_PARAM_TYPE YYLEX_PARAM)
# else
#  define YYLEX_DECL() yylex(YYSTYPE *yylval, void * YYLEX_PARAM)
# endif
# define YYLEX yylex(&yylval, YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(YYSTYPE *yylval, const char **yyInput)
# define YYLEX yylex(&yylval, yyInput)
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(struct dateinfo *param, const char **yyInput, const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(param, yyInput, msg)
#endif

extern int YYPARSE_DECL();

#define tAGO 257
#define tDAY 258
#define tDAYZONE 259
#define tID 260
#define tMERIDIAN 261
#define tMINUTE_UNIT 262
#define tMONTH 263
#define tMONTH_UNIT 264
#define tSEC_UNIT 265
#define tSNUMBER 266
#define tUNUMBER 267
#define tZONE 268
#define tDST 269
#define AT_SIGN 270
#define tTIME 271
#define YYERRCODE 256
typedef int YYINT;
static const YYINT yylhs[] = {                           -1,
    0,    0,    3,    3,    3,    3,    3,    3,    3,    3,
    3,   10,   11,    1,    1,    4,    4,    4,    4,    4,
    4,    4,    5,    5,    6,    6,    6,    6,    8,    8,
    8,    7,    7,    7,    7,    7,    7,    7,    7,    9,
    9,   13,   13,   13,   13,   13,   13,   13,   13,   13,
   12,    2,    2,    2,
};
static const YYINT yylen[] = {                            2,
    0,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,   11,    2,    1,    1,    2,    4,    6,    7,    7,
    1,    2,    4,    6,    1,    1,    2,    1,    1,    2,
    2,    3,    5,    3,    3,    2,    4,    2,    3,    1,
    2,    2,    2,    1,    2,    2,    1,    2,    2,    1,
    1,    0,    1,    1,
};
static const YYINT yydefred[] = {                         1,
    0,    0,   26,   44,    0,   50,   47,    0,    0,    0,
    0,   21,    2,    3,    4,    5,    6,    7,    8,    9,
   10,   11,    0,   30,    0,   43,   48,   45,   31,   16,
   42,    0,   49,   46,    0,   22,    0,    0,    0,   27,
   15,   14,   13,   41,    0,   35,   39,   34,    0,    0,
    0,   37,    0,   53,   23,   54,    0,   17,    0,    0,
    0,   33,    0,   24,    0,    0,   18,    0,   19,   20,
    0,    0,    0,   12,
};
#if defined(YYDESTRUCT_CALL) || defined(YYSTYPE_TOSTRING)
static const YYINT yystos[] = {                           0,
  273,  258,  259,  262,  263,  264,  265,  266,  267,  268,
  270,  271,  276,  277,  278,  279,  280,  281,  282,  283,
  284,  285,  286,   44,  267,  262,  264,  265,  258,  261,
  262,  263,  264,  265,  266,  271,   46,   58,   47,  269,
  266,  267,  274,  257,   44,  266,  267,  266,  267,  267,
  267,  267,   46,  261,  266,  271,   58,  275,   47,  267,
  267,  267,   46,  266,   46,   44,  275,  267,  267,  267,
   46,  267,   46,  267,
};
#endif /* YYDESTRUCT_CALL || YYSTYPE_TOSTRING */
static const YYINT yydgoto[] = {                          1,
   43,   58,   13,   14,   15,   16,   17,   18,   19,   20,
   21,   22,   23,
};
static const YYINT yysindex[] = {                         0,
 -232,  -39,    0,    0, -259,    0,    0, -258,  -33, -253,
 -257,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -240,    0,  -26,    0,    0,    0,    0,    0,
    0, -255,    0,    0, -247,    0, -246, -245, -244,    0,
    0,    0,    0,    0, -243,    0,    0,    0,  -18,  -55,
  -27,    0, -230,    0,    0,    0, -227,    0, -226,   -4,
  -44,    0, -223,    0, -222, -221,    0,    2,    0,    0,
 -220,    3, -217,    0,
};
static const YYINT yyrindex[] = {                         0,
    0,    1,    0,    0,    0,    0,    0,   85,   79,   15,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   29,    0,   43,    0,    0,    0,    0,    0,
    0,   71,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   99,
   57,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   99,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,
};
#if YYBTYACC
static const YYINT yycindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,
};
#endif
static const YYINT yygindex[] = {                         0,
    0,  -10,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,
};
#define YYTABLESIZE 369
static const YYINT yytable[] = {                         66,
   29,   65,   57,   26,   24,   27,   28,   25,   41,   42,
   46,   47,   37,   39,   25,   40,   44,   45,   48,   59,
   49,   50,   51,   52,   38,    2,    3,   53,   40,    4,
    5,    6,    7,    8,    9,   10,   60,   11,   12,   61,
   62,   63,   36,   68,   69,   70,   72,   71,   73,   74,
   67,    0,    0,    0,    0,    0,   32,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   38,    0,    0,    0,    0,    0,    0,    0,   51,    0,
    0,    0,    0,    0,   28,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   52,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   54,    0,    0,    0,    0,
   55,    0,    0,    0,    0,   56,   54,    0,    0,    0,
    0,   64,    0,    0,   29,    0,   56,   30,   31,   32,
   33,   34,   35,    0,    0,    0,    0,   36,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   29,   29,
    0,    0,   29,   29,   29,   29,   29,   29,   29,    0,
   29,   29,   25,   25,    0,    0,   25,   25,   25,   25,
   25,   25,   25,    0,   25,   25,   40,   40,    0,    0,
   40,   40,   40,   40,   40,   40,   40,    0,   40,   40,
   36,   36,    0,    0,   36,   36,   36,   36,   36,   36,
   36,    0,   36,   36,   32,   32,    0,    0,   32,   32,
   32,   32,   32,   32,   32,    0,   32,   32,   38,   38,
    0,    0,   38,   38,   38,   38,    0,   51,   38,    0,
   38,   38,   28,   28,    0,   51,   51,   28,   51,    0,
   28,   28,   28,    0,   28,   28,   52,   52,    0,    0,
   52,   52,   52,   52,    0,   52,   52,    0,   52,
};
static const YYINT yycheck[] = {                         44,
    0,   46,   58,  262,   44,  264,  265,  267,  266,  267,
  266,  267,   46,   47,    0,  269,  257,   44,  266,   47,
  267,  267,  267,  267,   58,  258,  259,   46,    0,  262,
  263,  264,  265,  266,  267,  268,  267,  270,  271,  267,
  267,   46,    0,  267,  267,  267,  267,   46,   46,  267,
   61,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  261,   -1,   -1,   -1,   -1,
  266,   -1,   -1,   -1,   -1,  271,  261,   -1,   -1,   -1,
   -1,  266,   -1,   -1,  258,   -1,  271,  261,  262,  263,
  264,  265,  266,   -1,   -1,   -1,   -1,  271,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,  259,
   -1,   -1,  262,  263,  264,  265,  266,  267,  268,   -1,
  270,  271,  258,  259,   -1,   -1,  262,  263,  264,  265,
  266,  267,  268,   -1,  270,  271,  258,  259,   -1,   -1,
  262,  263,  264,  265,  266,  267,  268,   -1,  270,  271,
  258,  259,   -1,   -1,  262,  263,  264,  265,  266,  267,
  268,   -1,  270,  271,  258,  259,   -1,   -1,  262,  263,
  264,  265,  266,  267,  268,   -1,  270,  271,  258,  259,
   -1,   -1,  262,  263,  264,  265,   -1,  259,  268,   -1,
  270,  271,  258,  259,   -1,  267,  268,  263,  270,   -1,
  266,  267,  268,   -1,  270,  271,  258,  259,   -1,   -1,
  262,  263,  264,  265,   -1,  267,  268,   -1,  270,
};
#if YYBTYACC
static const YYINT yyctable[] = {                        -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
};
#endif
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 271
#define YYUNDFTOKEN 287
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
#ifndef NULL
#define NULL (void*)0
#endif
static const char *const yyname[] = {

"$end",NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,"','",NULL,
"'.'","'/'",NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,"':'",NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,"error","tAGO","tDAY","tDAYZONE","tID","tMERIDIAN",
"tMINUTE_UNIT","tMONTH","tMONTH_UNIT","tSEC_UNIT","tSNUMBER","tUNUMBER","tZONE",
"tDST","AT_SIGN","tTIME","$accept","spec","at_number","o_merid","item","time",
"time_numericzone","zone","date","day","rel","cvsstamp","epochdate","number",
"relunit","illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : spec",
"spec :",
"spec : spec item",
"item : time",
"item : time_numericzone",
"item : zone",
"item : date",
"item : day",
"item : rel",
"item : cvsstamp",
"item : epochdate",
"item : number",
"cvsstamp : tUNUMBER '.' tUNUMBER '.' tUNUMBER '.' tUNUMBER '.' tUNUMBER '.' tUNUMBER",
"epochdate : AT_SIGN at_number",
"at_number : tUNUMBER",
"at_number : tSNUMBER",
"time : tUNUMBER tMERIDIAN",
"time : tUNUMBER ':' tUNUMBER o_merid",
"time : tUNUMBER ':' tUNUMBER ':' tUNUMBER o_merid",
"time : tUNUMBER ':' tUNUMBER ':' tUNUMBER '.' tUNUMBER",
"time : tUNUMBER ':' tUNUMBER ':' tUNUMBER ',' tUNUMBER",
"time : tTIME",
"time : tUNUMBER tTIME",
"time_numericzone : tUNUMBER ':' tUNUMBER tSNUMBER",
"time_numericzone : tUNUMBER ':' tUNUMBER ':' tUNUMBER tSNUMBER",
"zone : tZONE",
"zone : tDAYZONE",
"zone : tZONE tDST",
"zone : tSNUMBER",
"day : tDAY",
"day : tDAY ','",
"day : tUNUMBER tDAY",
"date : tUNUMBER '/' tUNUMBER",
"date : tUNUMBER '/' tUNUMBER '/' tUNUMBER",
"date : tUNUMBER tSNUMBER tSNUMBER",
"date : tUNUMBER tMONTH tSNUMBER",
"date : tMONTH tUNUMBER",
"date : tMONTH tUNUMBER ',' tUNUMBER",
"date : tUNUMBER tMONTH",
"date : tUNUMBER tMONTH tUNUMBER",
"rel : relunit",
"rel : relunit tAGO",
"relunit : tUNUMBER tMINUTE_UNIT",
"relunit : tSNUMBER tMINUTE_UNIT",
"relunit : tMINUTE_UNIT",
"relunit : tSNUMBER tSEC_UNIT",
"relunit : tUNUMBER tSEC_UNIT",
"relunit : tSEC_UNIT",
"relunit : tSNUMBER tMONTH_UNIT",
"relunit : tUNUMBER tMONTH_UNIT",
"relunit : tMONTH_UNIT",
"number : tUNUMBER",
"o_merid :",
"o_merid : tMERIDIAN",
"o_merid : tTIME",

};
#endif

#if YYDEBUG
int      yydebug;
#endif

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(loc, rhs, n) \
do \
{ \
    if (n == 0) \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 0).last_line; \
        (loc).first_column = YYRHSLOC(rhs, 0).last_column; \
        (loc).last_line    = YYRHSLOC(rhs, 0).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, 0).last_column; \
    } \
    else \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 1).first_line; \
        (loc).first_column = YYRHSLOC(rhs, 1).first_column; \
        (loc).last_line    = YYRHSLOC(rhs, n).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, n).last_column; \
    } \
} while (0)
#endif /* YYLLOC_DEFAULT */
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#if YYBTYACC

#ifndef YYLVQUEUEGROWTH
#define YYLVQUEUEGROWTH 32
#endif
#endif /* YYBTYACC */

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#ifndef YYINITSTACKSIZE
#define YYINITSTACKSIZE 200
#endif

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  *p_base;
    YYLTYPE  *p_mark;
#endif
} YYSTACKDATA;
#if YYBTYACC

struct YYParseState_s
{
    struct YYParseState_s *save;    /* Previously saved parser state */
    YYSTACKDATA            yystack; /* saved parser stack */
    int                    state;   /* saved parser state */
    int                    errflag; /* saved error recovery status */
    int                    lexeme;  /* saved index of the conflict lexeme in the lexical queue */
    YYINT                  ctry;    /* saved index in yyctable[] for this conflict */
};
typedef struct YYParseState_s YYParseState;
#endif /* YYBTYACC */
#line 479 "parsedate.y"

static short DaysInMonth[12] = {
    31, 28, 31, 30, 31, 30,
    31, 31, 30, 31, 30, 31
};

/*
 * works with tm.tm_year (ie: rel to 1900)
 */
#define	isleap(yr)  (((yr) & 3) == 0 && (((yr) % 100) != 0 || \
			((1900+(yr)) % 400) == 0))

/* Month and day table. */
static const TABLE MonthDayTable[] = {
    { "january",	tMONTH,  1 },
    { "february",	tMONTH,  2 },
    { "march",		tMONTH,  3 },
    { "april",		tMONTH,  4 },
    { "may",		tMONTH,  5 },
    { "june",		tMONTH,  6 },
    { "july",		tMONTH,  7 },
    { "august",		tMONTH,  8 },
    { "september",	tMONTH,  9 },
    { "sept",		tMONTH,  9 },
    { "october",	tMONTH, 10 },
    { "november",	tMONTH, 11 },
    { "december",	tMONTH, 12 },
    { "sunday",		tDAY, 0 },
    { "su",		tDAY, 0 },
    { "monday",		tDAY, 1 },
    { "mo",		tDAY, 1 },
    { "tuesday",	tDAY, 2 },
    { "tues",		tDAY, 2 },
    { "tu",		tDAY, 2 },
    { "wednesday",	tDAY, 3 },
    { "wednes",		tDAY, 3 },
    { "weds",		tDAY, 3 },
    { "we",		tDAY, 3 },
    { "thursday",	tDAY, 4 },
    { "thurs",		tDAY, 4 },
    { "thur",		tDAY, 4 },
    { "th",		tDAY, 4 },
    { "friday",		tDAY, 5 },
    { "fr",		tDAY, 5 },
    { "saturday",	tDAY, 6 },
    { "sa",		tDAY, 6 },
    { NULL,		0,    0 }
};

/* Time units table. */
static const TABLE UnitsTable[] = {
    { "year",		tMONTH_UNIT,	12 },
    { "month",		tMONTH_UNIT,	1 },
    { "fortnight",	tMINUTE_UNIT,	14 * 24 * 60 },
    { "week",		tMINUTE_UNIT,	7 * 24 * 60 },
    { "day",		tMINUTE_UNIT,	1 * 24 * 60 },
    { "hour",		tMINUTE_UNIT,	60 },
    { "minute",		tMINUTE_UNIT,	1 },
    { "min",		tMINUTE_UNIT,	1 },
    { "second",		tSEC_UNIT,	1 },
    { "sec",		tSEC_UNIT,	1 },
    { NULL,		0,		0 }
};

/* Assorted relative-time words. */
static const TABLE OtherTable[] = {
    { "tomorrow",	tMINUTE_UNIT,	1 * 24 * 60 },
    { "yesterday",	tMINUTE_UNIT,	-1 * 24 * 60 },
    { "today",		tMINUTE_UNIT,	0 },
    { "now",		tMINUTE_UNIT,	0 },
    { "last",		tUNUMBER,	-1 },
    { "this",		tMINUTE_UNIT,	0 },
    { "next",		tUNUMBER,	2 },
    { "first",		tUNUMBER,	1 },
    { "one",		tUNUMBER,	1 },
/*  { "second",		tUNUMBER,	2 }, */
    { "two",		tUNUMBER,	2 },
    { "third",		tUNUMBER,	3 },
    { "three",		tUNUMBER,	3 },
    { "fourth",		tUNUMBER,	4 },
    { "four",		tUNUMBER,	4 },
    { "fifth",		tUNUMBER,	5 },
    { "five",		tUNUMBER,	5 },
    { "sixth",		tUNUMBER,	6 },
    { "six",		tUNUMBER,	6 },
    { "seventh",	tUNUMBER,	7 },
    { "seven",		tUNUMBER,	7 },
    { "eighth",		tUNUMBER,	8 },
    { "eight",		tUNUMBER,	8 },
    { "ninth",		tUNUMBER,	9 },
    { "nine",		tUNUMBER,	9 },
    { "tenth",		tUNUMBER,	10 },
    { "ten",		tUNUMBER,	10 },
    { "eleventh",	tUNUMBER,	11 },
    { "eleven",		tUNUMBER,	11 },
    { "twelfth",	tUNUMBER,	12 },
    { "twelve",		tUNUMBER,	12 },
    { "ago",		tAGO,	1 },
    { NULL,		0,	0 }
};

/* The timezone table. */
/* Some of these are commented out because a time_t can't store a float. */
static const TABLE TimezoneTable[] = {
    { "gmt",	tZONE,     HOUR( 0) },	/* Greenwich Mean */
    { "ut",	tZONE,     HOUR( 0) },	/* Universal (Coordinated) */
    { "utc",	tZONE,     HOUR( 0) },
    { "wet",	tZONE,     HOUR( 0) },	/* Western European */
    { "bst",	tDAYZONE,  HOUR( 0) },	/* British Summer */
    { "wat",	tZONE,     HOUR( 1) },	/* West Africa */
    { "at",	tZONE,     HOUR( 2) },	/* Azores */
#if	0
    /* For completeness.  BST is also British Summer, and GST is
     * also Guam Standard. */
    { "bst",	tZONE,     HOUR( 3) },	/* Brazil Standard */
    { "gst",	tZONE,     HOUR( 3) },	/* Greenland Standard */
#endif
    { "nft",	tZONE,     HOUR(3.5) },	/* Newfoundland */
    { "nst",	tZONE,     HOUR(3.5) },	/* Newfoundland Standard */
    { "ndt",	tDAYZONE,  HOUR(3.5) },	/* Newfoundland Daylight */
    { "ast",	tZONE,     HOUR( 4) },	/* Atlantic Standard */
    { "adt",	tDAYZONE,  HOUR( 4) },	/* Atlantic Daylight */
    { "est",	tZONE,     HOUR( 5) },	/* Eastern Standard */
    { "edt",	tDAYZONE,  HOUR( 5) },	/* Eastern Daylight */
    { "cst",	tZONE,     HOUR( 6) },	/* Central Standard */
    { "cdt",	tDAYZONE,  HOUR( 6) },	/* Central Daylight */
    { "mst",	tZONE,     HOUR( 7) },	/* Mountain Standard */
    { "mdt",	tDAYZONE,  HOUR( 7) },	/* Mountain Daylight */
    { "pst",	tZONE,     HOUR( 8) },	/* Pacific Standard */
    { "pdt",	tDAYZONE,  HOUR( 8) },	/* Pacific Daylight */
    { "yst",	tZONE,     HOUR( 9) },	/* Yukon Standard */
    { "ydt",	tDAYZONE,  HOUR( 9) },	/* Yukon Daylight */
    { "hst",	tZONE,     HOUR(10) },	/* Hawaii Standard */
    { "hdt",	tDAYZONE,  HOUR(10) },	/* Hawaii Daylight */
    { "cat",	tZONE,     HOUR(10) },	/* Central Alaska */
    { "ahst",	tZONE,     HOUR(10) },	/* Alaska-Hawaii Standard */
    { "nt",	tZONE,     HOUR(11) },	/* Nome */
    { "idlw",	tZONE,     HOUR(12) },	/* International Date Line West */
    { "cet",	tZONE,     -HOUR(1) },	/* Central European */
    { "met",	tZONE,     -HOUR(1) },	/* Middle European */
    { "mewt",	tZONE,     -HOUR(1) },	/* Middle European Winter */
    { "mest",	tDAYZONE,  -HOUR(1) },	/* Middle European Summer */
    { "swt",	tZONE,     -HOUR(1) },	/* Swedish Winter */
    { "sst",	tDAYZONE,  -HOUR(1) },	/* Swedish Summer */
    { "fwt",	tZONE,     -HOUR(1) },	/* French Winter */
    { "fst",	tDAYZONE,  -HOUR(1) },	/* French Summer */
    { "eet",	tZONE,     -HOUR(2) },	/* Eastern Europe, USSR Zone 1 */
    { "bt",	tZONE,     -HOUR(3) },	/* Baghdad, USSR Zone 2 */
    { "it",	tZONE,     -HOUR(3.5) },/* Iran */
    { "zp4",	tZONE,     -HOUR(4) },	/* USSR Zone 3 */
    { "zp5",	tZONE,     -HOUR(5) },	/* USSR Zone 4 */
    { "ist",	tZONE,     -HOUR(5.5) },/* Indian Standard */
    { "zp6",	tZONE,     -HOUR(6) },	/* USSR Zone 5 */
#if	0
    /* For completeness.  NST is also Newfoundland Stanard, and SST is
     * also Swedish Summer. */
    { "nst",	tZONE,     -HOUR(6.5) },/* North Sumatra */
    { "sst",	tZONE,     -HOUR(7) },	/* South Sumatra, USSR Zone 6 */
#endif	/* 0 */
    { "ict",	tZONE,     -HOUR(7) },	/* Indo China Time (Thai) */
#if 0	/* this one looks to be bogus */
    { "jt",	tZONE,     -HOUR(7.5) },/* Java (3pm in Cronusland!) */
#endif
    { "wast",	tZONE,     -HOUR(8) },	/* West Australian Standard */
    { "awst",	tZONE,     -HOUR(8) },	/* West Australian Standard */
    { "wadt",	tDAYZONE,  -HOUR(8) },	/* West Australian Daylight */
    { "awdt",	tDAYZONE,  -HOUR(8) },	/* West Australian Daylight */
    { "cct",	tZONE,     -HOUR(8) },	/* China Coast, USSR Zone 7 */
    { "sgt",	tZONE,     -HOUR(8) },	/* Singapore */
    { "hkt",	tZONE,     -HOUR(8) },	/* Hong Kong */
    { "jst",	tZONE,     -HOUR(9) },	/* Japan Standard, USSR Zone 8 */
    { "cast",	tZONE,     -HOUR(9.5) },/* Central Australian Standard */
    { "acst",	tZONE,     -HOUR(9.5) },/* Central Australian Standard */
    { "cadt",	tDAYZONE,  -HOUR(9.5) },/* Central Australian Daylight */
    { "acdt",	tDAYZONE,  -HOUR(9.5) },/* Central Australian Daylight */
    { "east",	tZONE,     -HOUR(10) },	/* Eastern Australian Standard */
    { "aest",	tZONE,     -HOUR(10) },	/* Eastern Australian Standard */
    { "eadt",	tDAYZONE,  -HOUR(10) },	/* Eastern Australian Daylight */
    { "aedt",	tDAYZONE,  -HOUR(10) },	/* Eastern Australian Daylight */
    { "gst",	tZONE,     -HOUR(10) },	/* Guam Standard, USSR Zone 9 */
    { "nzt",	tZONE,     -HOUR(12) },	/* New Zealand */
    { "nzst",	tZONE,     -HOUR(12) },	/* New Zealand Standard */
    { "nzdt",	tDAYZONE,  -HOUR(12) },	/* New Zealand Daylight */
    { "idle",	tZONE,     -HOUR(12) },	/* International Date Line East */
    {  NULL,	0,	    0 }
};

/* Military timezone table. */
static const TABLE MilitaryTable[] = {
    { "a",	tZONE,	HOUR(  1) },
    { "b",	tZONE,	HOUR(  2) },
    { "c",	tZONE,	HOUR(  3) },
    { "d",	tZONE,	HOUR(  4) },
    { "e",	tZONE,	HOUR(  5) },
    { "f",	tZONE,	HOUR(  6) },
    { "g",	tZONE,	HOUR(  7) },
    { "h",	tZONE,	HOUR(  8) },
    { "i",	tZONE,	HOUR(  9) },
    { "k",	tZONE,	HOUR( 10) },
    { "l",	tZONE,	HOUR( 11) },
    { "m",	tZONE,	HOUR( 12) },
    { "n",	tZONE,	HOUR(- 1) },
    { "o",	tZONE,	HOUR(- 2) },
    { "p",	tZONE,	HOUR(- 3) },
    { "q",	tZONE,	HOUR(- 4) },
    { "r",	tZONE,	HOUR(- 5) },
    { "s",	tZONE,	HOUR(- 6) },
    { "t",	tZONE,	HOUR(- 7) },
    { "u",	tZONE,	HOUR(- 8) },
    { "v",	tZONE,	HOUR(- 9) },
    { "w",	tZONE,	HOUR(-10) },
    { "x",	tZONE,	HOUR(-11) },
    { "y",	tZONE,	HOUR(-12) },
    { "z",	tZONE,	HOUR(  0) },
    { NULL,	0,	0 }
};

static const TABLE TimeNames[] = {
    { "midnight",	tTIME,		 0 },
    { "mn",		tTIME,		 0 },
    { "noon",		tTIME,		12 },
    { "midday",		tTIME,		12 },
    { NULL,		0,		 0 }
};



/* ARGSUSED */
static int
yyerror(struct dateinfo *param, const char **inp, const char *s __unused)
{
  return 0;
}

/*
 * Save a relative value, if it fits
 */
static int
RelVal(struct dateinfo *param, time_t num, time_t unit, int scale, int type)
{
	int i;
	time_t v;
	uintmax_t m;
	int sign = 1;

	if ((i = param->yyHaveRel) >= MAXREL)
		return 0;

	if (num < 0) {
		sign = -sign;
		num = -num;
	}
	if (unit < 0) {
		sign = -sign;
		unit = -unit;
	}
	/* scale is always positive */

	m = LLONG_MAX;		/* TIME_T_MAX */
	if (scale > 1)
		m /= scale;
	if (unit > 1)
		m /= unit;
	if ((uintmax_t)num > m)
		return 0;

	m = num * unit * scale;
	v = (time_t) m;
	if (v < 0 || (uintmax_t)v != m)
		return 0;
	if (sign < 0)
		v = -v;

	param->yyRel[i].yyRelMonth = type;
	param->yyRel[i].yyRelVal = v;

	return 1;
}

/*
 * Adjust year from a value that might be abbreviated, to a full value.
 * e.g. convert 70 to 1970.
 * Input Year is either:
 *  - A negative number, which means to use its absolute value (why?)
 *  - A number from 0 to 68, which means a year from 2000 to 2068, 
 *  - A number from 69 to 99, which means a year from 1969 to 1999, or
 *  - The actual year (>=100).
 * Returns the full year.
 */
static time_t
AdjustYear(time_t Year)
{
    /* XXX Y2K */
    if (Year < 0)
	Year = -Year;
    if (Year < 69)	/* POSIX compliant, 0..68 is 2000's, 69-99 1900's */
	Year += 2000;
    else if (Year < 100)
	Year += 1900;
    return Year;
}

static time_t
Convert(
    time_t	Month,		/* month of year [1-12] */
    time_t	Day,		/* day of month [1-31] */
    time_t	Year,		/* year, not abbreviated in any way */
    time_t	Hours,		/* Hour of day [0-24] */
    time_t	Minutes,	/* Minute of hour [0-59] */
    time_t	Seconds,	/* Second of minute [0-60] */
    time_t	Timezone,	/* Timezone as minutes east of UTC,
				 * or USE_LOCAL_TIME special case */
    MERIDIAN	Meridian,	/* Hours are am/pm/24 hour clock */
    DSTMODE	DSTmode		/* DST on/off/maybe */
)
{
    struct tm tm = {.tm_sec = 0};
    struct tm otm;
    time_t result;

    tm.tm_sec = Seconds;
    tm.tm_min = Minutes;
    tm.tm_hour = ((Hours == 12 && Meridian != MER24) ? 0 : Hours) +
	(Meridian == MERpm ? 12 : 0);

    tm.tm_mday = Day;
    tm.tm_mon = Month - 1;
    tm.tm_year = Year - 1900;
    if ((time_t)tm.tm_year + 1900 != Year) {
	errno = EOVERFLOW;
	return -1;
    }
    if (Timezone == USE_LOCAL_TIME) {
	    switch (DSTmode) {
	    case DSTon:  tm.tm_isdst = 1; break;
	    case DSToff: tm.tm_isdst = 0; break;
	    default:     tm.tm_isdst = -1; break;
	    }
	    otm = tm;
	    result = mktime(&tm);
    } else {
	    /* We rely on mktime_z(NULL, ...) working in UTC */
	    tm.tm_isdst = 0;	/* hence cannot be summer time */
	    otm = tm;
	    errno = 0;
	    result = mktime_z(NULL, &tm);
	    if (result != -1 || errno == 0) {
		    result += Timezone * 60;
		    if (DSTmode == DSTon)	/* if specified sumer time */
			result -= 3600;		/* UTC is 1 hour earlier XXX */
	    }
    }

#if PARSEDATE_DEBUG
    fprintf(stderr, "%s(M=%jd D=%jd Y=%jd H=%jd M=%jd S=%jd Z=%jd"
		    " mer=%d DST=%d)",
	__func__,
	(intmax_t)Month, (intmax_t)Day, (intmax_t)Year,
	(intmax_t)Hours, (intmax_t)Minutes, (intmax_t)Seconds,
	(intmax_t)Timezone, (int)Meridian, (int)DSTmode);
    fprintf(stderr, " -> %jd", (intmax_t)result);
    fprintf(stderr, " %s", ctime(&result));
#endif

#define	TM_NE(fld) (otm.tm_ ## fld != tm.tm_ ## fld)
    if (TM_NE(year) || TM_NE(mon) || TM_NE(mday) ||
	TM_NE(hour) || TM_NE(min) || TM_NE(sec)) {
	    /* mktime() "corrected" our tm, so it must have been invalid */
	    result = -1;
	    errno = EAGAIN;
    }
#undef	TM_NE

    return result;
}


static time_t
DSTcorrect(
    time_t	Start,
    time_t	Future
)
{
    time_t	StartDay;
    time_t	FutureDay;
    struct tm	tm;

    if (localtime_r(&Start, &tm) == NULL)
	return -1;
    StartDay = (tm.tm_hour + 1) % 24;

    if (localtime_r(&Future, &tm) == NULL)
	return -1;
    FutureDay = (tm.tm_hour + 1) % 24;

    return (Future - Start) + (StartDay - FutureDay) * 60L * 60L;
}


static time_t
RelativeDate(
    time_t	Start,
    time_t	DayOrdinal,
    time_t	DayNumber
)
{
    struct tm	tm;
    time_t	now;
    time_t	change;

    now = Start;
    if (localtime_r(&now, &tm) == NULL)
	return -1;

    /* should be using TIME_T_MAX but there is no such thing, so just "know" */
    if (llabs(DayOrdinal) >= LLONG_MAX / (7 * SECSPERDAY)) {
	errno = EOVERFLOW;
	return -1;
    }

    change = SECSPERDAY * ((DayNumber - tm.tm_wday + 7) % 7);
    change += 7 * SECSPERDAY * (DayOrdinal <= 0 ? DayOrdinal : DayOrdinal - 1);

    /* same here for _MAX and _MIN */
    if ((change > 0 && LLONG_MAX - change < now) ||
	(change < 0 && LLONG_MIN - change > now)) {
	    errno = EOVERFLOW;
	    return -1;
    }

    now += change;
    return DSTcorrect(Start, now);
}


static time_t
RelativeMonth(
    time_t	Start,
    time_t	RelMonth,
    time_t	Timezone
)
{
    struct tm	tm;
    time_t	Month;
    time_t	Then;
    int		Day;

    if (RelMonth == 0)
	return 0;
    /*
     * It doesn't matter what timezone we use to do this computation,
     * as long as we use the same one to reassemble the time that we
     * used to disassemble it. So always use localtime and mktime. In
     * particular, don't use Convert() to reassemble, because it will
     * not only reassemble with the wrong timezone but it will also
     * fail if we do e.g. three months from March 31 yielding July 1.
     */
    (void)Timezone;

    if (localtime_r(&Start, &tm) == NULL)
	return -1;

    if (RelMonth >= LLONG_MAX - 12*((time_t)tm.tm_year + 1900) - tm.tm_mon) {
	errno = EOVERFLOW;
	return -1;
    }
    Month = 12 * (tm.tm_year + 1900) + tm.tm_mon + RelMonth;
    tm.tm_year = (Month / 12) - 1900;
    /* check for tm_year (an int) overflow */
    if (((time_t)tm.tm_year + 1900) != Month/12) {
	errno = EOVERFLOW;
	return -1;
    }
    tm.tm_mon = Month % 12;
    if (tm.tm_mday > (Day = DaysInMonth[tm.tm_mon] +
	((tm.tm_mon==1) ? isleap(tm.tm_year) : 0)))
	    tm.tm_mday = Day;
    errno = 0;
    Then = mktime(&tm);
    if (Then == -1 && errno != 0)
	return -1;
    return DSTcorrect(Start, Then);
}


static int
LookupWord(YYSTYPE *yylval, char *buff)
{
    register char	*p;
    register char	*q;
    register const TABLE	*tp;
    int			i;
    int			abbrev;

    /* Make it lowercase. */
    for (p = buff; *p; p++)
	if (isupper((unsigned char)*p))
	    *p = tolower((unsigned char)*p);

    if (strcmp(buff, "am") == 0 || strcmp(buff, "a.m.") == 0) {
	yylval->Meridian = MERam;
	return tMERIDIAN;
    }
    if (strcmp(buff, "pm") == 0 || strcmp(buff, "p.m.") == 0) {
	yylval->Meridian = MERpm;
	return tMERIDIAN;
    }

    /* See if we have an abbreviation for a month. */
    if (strlen(buff) == 3)
	abbrev = 1;
    else if (strlen(buff) == 4 && buff[3] == '.') {
	abbrev = 1;
	buff[3] = '\0';
    }
    else
	abbrev = 0;

    for (tp = MonthDayTable; tp->name; tp++) {
	if (abbrev) {
	    if (strncmp(buff, tp->name, 3) == 0) {
		yylval->Number = tp->value;
		return tp->type;
	    }
	}
	else if (strcmp(buff, tp->name) == 0) {
	    yylval->Number = tp->value;
	    return tp->type;
	}
    }

    for (tp = TimezoneTable; tp->name; tp++)
	if (strcmp(buff, tp->name) == 0) {
	    yylval->Number = tp->value;
	    return tp->type;
	}

    if (strcmp(buff, "dst") == 0) 
	return tDST;

    for (tp = TimeNames; tp->name; tp++)
	if (strcmp(buff, tp->name) == 0) {
	    yylval->Number = tp->value;
	    return tp->type;
	}

    for (tp = UnitsTable; tp->name; tp++)
	if (strcmp(buff, tp->name) == 0) {
	    yylval->Number = tp->value;
	    return tp->type;
	}

    /* Strip off any plural and try the units table again. */
    i = strlen(buff) - 1;
    if (buff[i] == 's') {
	buff[i] = '\0';
	for (tp = UnitsTable; tp->name; tp++)
	    if (strcmp(buff, tp->name) == 0) {
		yylval->Number = tp->value;
		return tp->type;
	    }
	buff[i] = 's';		/* Put back for "this" in OtherTable. */
    }

    for (tp = OtherTable; tp->name; tp++)
	if (strcmp(buff, tp->name) == 0) {
	    yylval->Number = tp->value;
	    return tp->type;
	}

    /* Military timezones. */
    if (buff[1] == '\0' && isalpha((unsigned char)*buff)) {
	for (tp = MilitaryTable; tp->name; tp++)
	    if (strcmp(buff, tp->name) == 0) {
		yylval->Number = tp->value;
		return tp->type;
	    }
    }

    /* Drop out any periods and try the timezone table again. */
    for (i = 0, p = q = buff; *q; q++)
	if (*q != '.')
	    *p++ = *q;
	else
	    i++;
    *p = '\0';
    if (i)
	for (tp = TimezoneTable; tp->name; tp++)
	    if (strcmp(buff, tp->name) == 0) {
		yylval->Number = tp->value;
		return tp->type;
	    }

    return tID;
}


static int
yylex(YYSTYPE *yylval, const char **yyInput)
{
    register char	c;
    register char	*p;
    char		buff[20];
    int			Count;
    int			sign;
    const char		*inp = *yyInput;

    for ( ; ; ) {
	while (isspace((unsigned char)*inp))
	    inp++;

	if (isdigit((unsigned char)(c = *inp)) || c == '-' || c == '+') {
	    if (c == '-' || c == '+') {
		sign = c == '-' ? -1 : 1;
		if (!isdigit((unsigned char)*++inp))
		    /* skip the '-' sign */
		    continue;
	    }
	    else
		sign = 0;
	    for (yylval->Number = 0; isdigit((unsigned char)(c = *inp++)); ) {
	        time_t	v;

		v = yylval->Number;
		if (v > LLONG_MAX/10 ||
		    (v == LLONG_MAX/10 && (v * 10 > LLONG_MAX - (c - '0')))) 
			yylval->Number = LLONG_MAX;
		else
			yylval->Number = 10 * yylval->Number + c - '0';
	    }
	    if (sign < 0)
		yylval->Number = -yylval->Number;
	    *yyInput = --inp;
	    return sign ? tSNUMBER : tUNUMBER;
	}
	if (isalpha((unsigned char)c)) {
	    for (p = buff; isalpha((unsigned char)(c = *inp++)) || c == '.'; )
		if (p < &buff[sizeof buff - 1])
		    *p++ = c;
	    *p = '\0';
	    *yyInput = --inp;
	    return LookupWord(yylval, buff);
	}
	if (c == '@') {
	    *yyInput = ++inp;
	    return AT_SIGN;
	}
	if (c != '(') {
	    *yyInput = ++inp;
	    return c;
	}
	Count = 0;
	do {
	    c = *inp++;
	    if (c == '\0')
		return c;
	    if (c == '(')
		Count++;
	    else if (c == ')')
		Count--;
	} while (Count > 0);
    }
}

#define TM_YEAR_ORIGIN 1900

time_t
parsedate(const char *p, const time_t *now, const int *zone)
{
    struct tm		local, *tm;
    time_t		nowt;
    int			zonet;
    time_t		Start;
    time_t		tod, rm;
    struct dateinfo	param;
    int			saved_errno;
    int			i;
    
    saved_errno = errno;
    errno = 0;

    if (now == NULL) {
        now = &nowt;
	(void)time(&nowt);
    }
    if (zone == NULL) {
	zone = &zonet;
	zonet = USE_LOCAL_TIME;
	if ((tm = localtime_r(now, &local)) == NULL)
	    return -1;
    } else {
	/*
	 * Should use the specified zone, not localtime.
	 * Fake it using gmtime and arithmetic.
	 * This is good enough because we use only the year/month/day,
	 * not other fields of struct tm.
	 */
	time_t fake = *now + (*zone * 60);
	if ((tm = gmtime_r(&fake, &local)) == NULL)
	    return -1;
    }
    param.yyYear = tm->tm_year + 1900;
    param.yyMonth = tm->tm_mon + 1;
    param.yyDay = tm->tm_mday;
    param.yyTimezone = *zone;
    param.yyDSTmode = DSTmaybe;
    param.yyHour = 0;
    param.yyMinutes = 0;
    param.yySeconds = 0;
    param.yyMeridian = MER24;
    param.yyHaveDate = 0;
    param.yyHaveFullYear = 0;
    param.yyHaveDay = 0;
    param.yyHaveRel = 0;
    param.yyHaveTime = 0;
    param.yyHaveZone = 0;

    /*
     * This one is too hard to parse using a grammar (the lexer would
     * confuse the 'T' with the Mil format timezone designator)
     * so handle it as a special case.
     */
    do {
	const unsigned char *pp = (const unsigned char *)p;
	char *ep;	/* starts as "expected, becomes "end ptr" */
	static char format[] = "-dd-ddTdd:dd:dd";
	time_t yr;

	while (isdigit(*pp))
		pp++;

	if (pp == (const unsigned char *)p)
		break;

	for (ep = format; *ep; ep++, pp++) {
		switch (*ep) {
		case 'd':
			if (isdigit(*pp))
				continue;
			break;
		case 'T':
			if (*pp == 'T' || *pp == 't' || *pp == ' ')
				continue;
			break;
		default:
			if (*pp == *ep)
				continue;
			break;
		}
		break;
	}
	if (*ep != '\0')
		break;
	if (*pp == '.' || *pp == ',') {
		if (!isdigit(pp[1]))
			break;
		while (isdigit(*++pp))
			continue;
	}
	if (*pp == 'Z' || *pp == 'z')
		pp++;
	else if (isdigit(*pp))
		break;

	if (*pp != '\0' && !isspace(*pp))
		break;

	errno = 0;
	yr = (time_t)strtol(p, &ep, 10);
	if (errno != 0)			/* out of range (can be big number) */
		break;			/* the ones below are all 2 digits */

	/*
	 * This is good enough to commit to there being an ISO format
	 * timestamp leading the input string.   We permit standard
	 * parsedate() modifiers to follow but not precede this string.
	 */
	param.yyHaveTime = 1;
	param.yyHaveDate = 1;
	param.yyHaveFullYear = 1;

	if (pp[-1] == 'Z' || pp[-1] == 'z') {
		param.yyTimezone = 0;
		param.yyHaveZone = 1;
	}

	param.yyYear = yr;
	param.yyMonth = (time_t)strtol(ep + 1, &ep, 10);
	param.yyDay = (time_t)strtol(ep + 1, &ep, 10);
	param.yyHour = (time_t)strtol(ep + 1, &ep, 10);
	param.yyMinutes = (time_t)strtol(ep + 1, &ep, 10);
	param.yySeconds = (time_t)strtol(ep + 1, &ep, 10);
	/* ignore any fractional seconds, no way to return them in a time_t */

	param.yyMeridian = MER24;

	p = (const char *)pp;
    } while (0);

    if (yyparse(&param, &p) || param.yyHaveTime > 1 || param.yyHaveZone > 1 ||
	param.yyHaveDate > 1 || param.yyHaveDay > 1) {
	errno = EINVAL;
	return -1;
    }

    if (param.yyHaveDate || param.yyHaveTime || param.yyHaveDay) {
	if (! param.yyHaveFullYear) {
		param.yyYear = AdjustYear(param.yyYear);
		param.yyHaveFullYear = 1;
	}
	errno = 0;
	Start = Convert(param.yyMonth, param.yyDay, param.yyYear, param.yyHour,
	    param.yyMinutes, param.yySeconds, param.yyTimezone,
	    param.yyMeridian, param.yyDSTmode);
	if (Start == -1 && errno != 0)
	    return -1;
    }
    else {
	Start = *now;
	if (!param.yyHaveRel)
	    Start -= ((tm->tm_hour * 60L + tm->tm_min) * 60L) + tm->tm_sec;
    }

    if (param.yyHaveRel > MAXREL) {
	errno = EINVAL;
	return -1;
    }
    for (i = 0; i < param.yyHaveRel; i++) {
	if (param.yyRel[i].yyRelMonth) {
	    errno = 0;
	    rm = RelativeMonth(Start, param.yyRel[i].yyRelVal, param.yyTimezone);
	    if (rm == -1 && errno != 0)
		return -1;
	    Start += rm;
	} else
	    Start += param.yyRel[i].yyRelVal;
    }

    if (param.yyHaveDay && !param.yyHaveDate) {
	errno = 0;
	tod = RelativeDate(Start, param.yyDayOrdinal, param.yyDayNumber);
	if (tod == -1 && errno != 0)
	    return -1;
	Start += tod;
    }

    errno = saved_errno;
    return Start;
}


#if	defined(TEST)

/* ARGSUSED */
int
main(int ac, char *av[])
{
    char	buff[128];
    time_t	d;

    (void)printf("Enter date, or blank line to exit.\n\t> ");
    (void)fflush(stdout);
    while (fgets(buff, sizeof(buff), stdin) && buff[0] != '\n') {
	errno = 0;
	d = parsedate(buff, NULL, NULL);
	if (d == -1 && errno != 0)
	    (void)printf("Bad format - couldn't convert: %s\n",
	        strerror(errno));
	else
	    (void)printf("%jd\t%s", (intmax_t)d, ctime(&d));
	(void)printf("\t> ");
	(void)fflush(stdout);
    }
    exit(0);
    /* NOTREACHED */
}
#endif	/* defined(TEST) */
#line 1443 "y.tab.c"

/* For use in generated program */
#define yydepth (int)(yystack.s_mark - yystack.s_base)
#if YYBTYACC
#define yytrial (yyps->save)
#endif /* YYBTYACC */

#if YYDEBUG
#include <stdio.h>	/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE *newps;
#endif

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == NULL)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == NULL)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    newps = (YYLTYPE *)realloc(data->p_base, newsize * sizeof(*newps));
    if (newps == NULL)
        return YYENOMEM;

    data->p_base = newps;
    data->p_mark = newps + i;
#endif

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;

#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%sdebug: stack size increased to %d\n", YYPREFIX, newsize);
#endif
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    free(data->p_base);
#endif
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif /* YYPURE || defined(YY_NO_LEAKS) */
#if YYBTYACC

static YYParseState *
yyNewState(unsigned size)
{
    YYParseState *p = (YYParseState *) malloc(sizeof(YYParseState));
    if (p == NULL) return NULL;

    p->yystack.stacksize = size;
    if (size == 0)
    {
        p->yystack.s_base = NULL;
        p->yystack.l_base = NULL;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        p->yystack.p_base = NULL;
#endif
        return p;
    }
    p->yystack.s_base    = (YYINT *) malloc(size * sizeof(YYINT));
    if (p->yystack.s_base == NULL) return NULL;
    p->yystack.l_base    = (YYSTYPE *) malloc(size * sizeof(YYSTYPE));
    if (p->yystack.l_base == NULL) return NULL;
    memset(p->yystack.l_base, 0, size * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    p->yystack.p_base    = (YYLTYPE *) malloc(size * sizeof(YYLTYPE));
    if (p->yystack.p_base == NULL) return NULL;
    memset(p->yystack.p_base, 0, size * sizeof(YYLTYPE));
#endif

    return p;
}

static void
yyFreeState(YYParseState *p)
{
    yyfreestack(&p->yystack);
    free(p);
}
#endif /* YYBTYACC */

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab
#if YYBTYACC
#define YYVALID        do { if (yyps->save)            goto yyvalid; } while(0)
#define YYVALID_NESTED do { if (yyps->save && \
                                yyps->save->save == 0) goto yyvalid; } while(0)
#endif /* YYBTYACC */

int
YYPARSE_DECL()
{
    int      yyerrflag;
    int      yychar;
    YYSTYPE  yyval;
    YYSTYPE  yylval;
    int      yynerrs;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  yyloc; /* position returned by actions */
    YYLTYPE  yylloc; /* position from the lexer */
#endif

    /* variables for the parser stack */
    YYSTACKDATA yystack;
#if YYBTYACC

    /* Current parser state */
    static YYParseState *yyps = NULL;

    /* yypath != NULL: do the full parse, starting at *yypath parser state. */
    static YYParseState *yypath = NULL;

    /* Base of the lexical value queue */
    static YYSTYPE *yylvals = NULL;

    /* Current position at lexical value queue */
    static YYSTYPE *yylvp = NULL;

    /* End position of lexical value queue */
    static YYSTYPE *yylve = NULL;

    /* The last allocated position at the lexical value queue */
    static YYSTYPE *yylvlim = NULL;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    /* Base of the lexical position queue */
    static YYLTYPE *yylpsns = NULL;

    /* Current position at lexical position queue */
    static YYLTYPE *yylpp = NULL;

    /* End position of lexical position queue */
    static YYLTYPE *yylpe = NULL;

    /* The last allocated position at the lexical position queue */
    static YYLTYPE *yylplim = NULL;
#endif

    /* Current position at lexical token queue */
    static YYINT  *yylexp = NULL;

    static YYINT  *yylexemes = NULL;
#endif /* YYBTYACC */
    int yym, yyn, yystate, yyresult;
#if YYBTYACC
    int yynewerrflag;
    YYParseState *yyerrctx = NULL;
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  yyerror_loc_range[3]; /* position of error start/end (0 unused) */
#endif
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != NULL)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
    if (yydebug)
        fprintf(stderr, "%sdebug[<# of symbols on state stack>]\n", YYPREFIX);
#endif
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    memset(yyerror_loc_range, 0, sizeof(yyerror_loc_range));
#endif

    yyerrflag = 0;
    yychar = 0;
    memset(&yyval,  0, sizeof(yyval));
    memset(&yylval, 0, sizeof(yylval));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    memset(&yyloc,  0, sizeof(yyloc));
    memset(&yylloc, 0, sizeof(yylloc));
#endif

#if YYBTYACC
    yyps = yyNewState(0); if (yyps == NULL) goto yyenomem;
    yyps->save = NULL;
#endif /* YYBTYACC */
    yym = 0;
    /* yyn is set below */
    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base;
#endif
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
#if YYBTYACC
        do {
        if (yylvp < yylve)
        {
            /* we're currently re-reading tokens */
            yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc = *yylpp++;
#endif
            yychar = *yylexp++;
            break;
        }
        if (yyps->save)
        {
            /* in trial mode; save scanner results for future parse attempts */
            if (yylvp == yylvlim)
            {   /* Enlarge lexical value queue */
                size_t p = (size_t) (yylvp - yylvals);
                size_t s = (size_t) (yylvlim - yylvals);

                s += YYLVQUEUEGROWTH;
                if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL) goto yyenomem;
                if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL) goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL) goto yyenomem;
#endif
                yylvp   = yylve = yylvals + p;
                yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp   = yylpe = yylpsns + p;
                yylplim = yylpsns + s;
#endif
                yylexp  = yylexemes + p;
            }
            *yylexp = (YYINT) YYLEX;
            *yylvp++ = yylval;
            yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *yylpp++ = yylloc;
            yylpe++;
#endif
            yychar = *yylexp++;
            break;
        }
        /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
        yychar = YYLEX;
#if YYBTYACC
        } while (0);
#endif /* YYBTYACC */
        if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, " <%s>", YYSTYPE_TOSTRING(yychar, yylval));
#endif
            fputc('\n', stderr);
        }
#endif
    }
#if YYBTYACC

    /* Do we have a conflict? */
    if (((yyn = yycindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
        yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        YYINT ctry;

        if (yypath)
        {
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: CONFLICT in state %d: following successful trial parse\n",
                                YYDEBUGSTR, yydepth, yystate);
#endif
            /* Switch to the next conflict context */
            save = yypath;
            yypath = save->save;
            save->save = NULL;
            ctry = save->ctry;
            if (save->state != yystate) YYABORT;
            yyFreeState(save);

        }
        else
        {

            /* Unresolved conflict - start/continue trial parse */
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
            {
                fprintf(stderr, "%s[%d]: CONFLICT in state %d. ", YYDEBUGSTR, yydepth, yystate);
                if (yyps->save)
                    fputs("ALREADY in conflict, continuing trial parse.\n", stderr);
                else
                    fputs("Starting trial parse.\n", stderr);
            }
#endif
            save                  = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (save == NULL) goto yyenomem;
            save->save            = yyps->save;
            save->state           = yystate;
            save->errflag         = yyerrflag;
            save->yystack.s_mark  = save->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (save->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            save->yystack.l_mark  = save->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (save->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            save->yystack.p_mark  = save->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (save->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            ctry                  = yytable[yyn];
            if (yyctable[ctry] == -1)
            {
#if YYDEBUG
                if (yydebug && yychar >= YYEOF)
                    fprintf(stderr, "%s[%d]: backtracking 1 token\n", YYDEBUGSTR, yydepth);
#endif
                ctry++;
            }
            save->ctry = ctry;
            if (yyps->save == NULL)
            {
                /* If this is a first conflict in the stack, start saving lexemes */
                if (!yylexemes)
                {
                    yylexemes = (YYINT *) malloc((YYLVQUEUEGROWTH) * sizeof(YYINT));
                    if (yylexemes == NULL) goto yyenomem;
                    yylvals   = (YYSTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYSTYPE));
                    if (yylvals == NULL) goto yyenomem;
                    yylvlim   = yylvals + YYLVQUEUEGROWTH;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpsns   = (YYLTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYLTYPE));
                    if (yylpsns == NULL) goto yyenomem;
                    yylplim   = yylpsns + YYLVQUEUEGROWTH;
#endif
                }
                if (yylvp == yylve)
                {
                    yylvp  = yylve = yylvals;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp  = yylpe = yylpsns;
#endif
                    yylexp = yylexemes;
                    if (yychar >= YYEOF)
                    {
                        *yylve++ = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                        *yylpe++ = yylloc;
#endif
                        *yylexp  = (YYINT) yychar;
                        yychar   = YYEMPTY;
                    }
                }
            }
            if (yychar >= YYEOF)
            {
                yylvp--;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp--;
#endif
                yylexp--;
                yychar = YYEMPTY;
            }
            save->lexeme = (int) (yylvp - yylvals);
            yyps->save   = save;
        }
        if (yytable[yyn] == ctry)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                                YYDEBUGSTR, yydepth, yystate, yyctable[ctry]);
#endif
            if (yychar < 0)
            {
                yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp++;
#endif
                yylexp++;
            }
            if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                goto yyoverflow;
            yystate = yyctable[ctry];
            *++yystack.s_mark = (YYINT) yystate;
            *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *++yystack.p_mark = yylloc;
#endif
            yychar  = YYEMPTY;
            if (yyerrflag > 0) --yyerrflag;
            goto yyloop;
        }
        else
        {
            yyn = yyctable[ctry];
            goto yyreduce;
        }
    } /* End of code dealing with conflicts */
#endif /* YYBTYACC */
    if (((yyn = yysindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                            YYDEBUGSTR, yydepth, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yylloc;
#endif
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if (((yyn = yyrindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag != 0) goto yyinrecovery;
#if YYBTYACC

    yynewerrflag = 1;
    goto yyerrhandler;
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */

yyerrlab:
    /* explicit YYERROR from an action -- pop the rhs of the rule reduced
     * before looking for error recovery */
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif

    yynewerrflag = 0;
yyerrhandler:
    while (yyps->save)
    {
        int ctry;
        YYParseState *save = yyps->save;
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: ERROR in state %d, CONFLICT BACKTRACKING to state %d, %d tokens\n",
                            YYDEBUGSTR, yydepth, yystate, yyps->save->state,
                    (int)(yylvp - yylvals - yyps->save->lexeme));
#endif
        /* Memorize most forward-looking error state in case it's really an error. */
        if (yyerrctx == NULL || yyerrctx->lexeme < yylvp - yylvals)
        {
            /* Free old saved error context state */
            if (yyerrctx) yyFreeState(yyerrctx);
            /* Create and fill out new saved error context state */
            yyerrctx                 = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (yyerrctx == NULL) goto yyenomem;
            yyerrctx->save           = yyps->save;
            yyerrctx->state          = yystate;
            yyerrctx->errflag        = yyerrflag;
            yyerrctx->yystack.s_mark = yyerrctx->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (yyerrctx->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yyerrctx->yystack.l_mark = yyerrctx->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (yyerrctx->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yyerrctx->yystack.p_mark = yyerrctx->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (yyerrctx->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yyerrctx->lexeme         = (int) (yylvp - yylvals);
        }
        yylvp          = yylvals   + save->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yylpp          = yylpsns   + save->lexeme;
#endif
        yylexp         = yylexemes + save->lexeme;
        yychar         = YYEMPTY;
        yystack.s_mark = yystack.s_base + (save->yystack.s_mark - save->yystack.s_base);
        memcpy (yystack.s_base, save->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
        yystack.l_mark = yystack.l_base + (save->yystack.l_mark - save->yystack.l_base);
        memcpy (yystack.l_base, save->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yystack.p_mark = yystack.p_base + (save->yystack.p_mark - save->yystack.p_base);
        memcpy (yystack.p_base, save->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
        ctry           = ++save->ctry;
        yystate        = save->state;
        /* We tried shift, try reduce now */
        if ((yyn = yyctable[ctry]) >= 0) goto yyreduce;
        yyps->save     = save->save;
        save->save     = NULL;
        yyFreeState(save);

        /* Nothing left on the stack -- error */
        if (!yyps->save)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%sdebug[%d,trial]: trial parse FAILED, entering ERROR mode\n",
                                YYPREFIX, yydepth);
#endif
            /* Restore state as it was in the most forward-advanced error */
            yylvp          = yylvals   + yyerrctx->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylpp          = yylpsns   + yyerrctx->lexeme;
#endif
            yylexp         = yylexemes + yyerrctx->lexeme;
            yychar         = yylexp[-1];
            yylval         = yylvp[-1];
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc         = yylpp[-1];
#endif
            yystack.s_mark = yystack.s_base + (yyerrctx->yystack.s_mark - yyerrctx->yystack.s_base);
            memcpy (yystack.s_base, yyerrctx->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yystack.l_mark = yystack.l_base + (yyerrctx->yystack.l_mark - yyerrctx->yystack.l_base);
            memcpy (yystack.l_base, yyerrctx->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yystack.p_mark = yystack.p_base + (yyerrctx->yystack.p_mark - yyerrctx->yystack.p_base);
            memcpy (yystack.p_base, yyerrctx->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yystate        = yyerrctx->state;
            yyFreeState(yyerrctx);
            yyerrctx       = NULL;
        }
        yynewerrflag = 1;
    }
    if (yynewerrflag == 0) goto yyinrecovery;
#endif /* YYBTYACC */

    YYERROR_CALL("syntax error");
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yyerror_loc_range[1] = yylloc; /* lookahead position is error start position */
#endif

#if !YYBTYACC
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */
yyerrlab:
#endif
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if (((yyn = yysindex[*yystack.s_mark]) != 0) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: state %d, error recovery shifting to state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* lookahead position is error end position */
                yyerror_loc_range[2] = yylloc;
                YYLLOC_DEFAULT(yyloc, yyerror_loc_range, 2); /* position of error span */
                *++yystack.p_mark = yyloc;
#endif
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: error recovery discarding state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* the current TOS position is the error start position */
                yyerror_loc_range[1] = *yystack.p_mark;
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
                if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark, yystack.p_mark);
#else
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
                --yystack.s_mark;
                --yystack.l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                --yystack.p_mark;
#endif
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, error recovery discarding token %d (%s)\n",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
        }
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval, &yylloc);
#else
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
    yym = yylen[yyn];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: state %d, reducing by rule %d (%s)",
                        YYDEBUGSTR, yydepth, yystate, yyn, yyrule[yyn]);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            if (yym > 0)
            {
                int i;
                fputc('<', stderr);
                for (i = yym; i > 0; i--)
                {
                    if (i != yym) fputs(", ", stderr);
                    fputs(YYSTYPE_TOSTRING(yystos[yystack.s_mark[1-i]],
                                           yystack.l_mark[1-i]), stderr);
                }
                fputc('>', stderr);
            }
#endif
        fputc('\n', stderr);
    }
#endif
    if (yym > 0)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)

    /* Perform position reduction */
    memset(&yyloc, 0, sizeof(yyloc));
#if YYBTYACC
    if (!yytrial)
#endif /* YYBTYACC */
    {
        YYLLOC_DEFAULT(yyloc, &yystack.p_mark[-yym], yym);
        /* just in case YYERROR is invoked within the action, save
           the start of the rhs as the error start position */
        yyerror_loc_range[1] = yystack.p_mark[1-yym];
    }
#endif

    switch (yyn)
    {
case 3:
#line 138 "parsedate.y"
	{ param->yyHaveTime++; }
#line 2177 "y.tab.c"
break;
case 4:
#line 139 "parsedate.y"
	{ param->yyHaveTime++; param->yyHaveZone++; }
#line 2182 "y.tab.c"
break;
case 5:
#line 140 "parsedate.y"
	{ param->yyHaveZone++; }
#line 2187 "y.tab.c"
break;
case 6:
#line 141 "parsedate.y"
	{ param->yyHaveDate++; }
#line 2192 "y.tab.c"
break;
case 7:
#line 142 "parsedate.y"
	{ param->yyHaveDay++; }
#line 2197 "y.tab.c"
break;
case 8:
#line 143 "parsedate.y"
	{ param->yyHaveRel++; }
#line 2202 "y.tab.c"
break;
case 9:
#line 144 "parsedate.y"
	{ param->yyHaveTime++; param->yyHaveDate++;
				  param->yyHaveZone++; }
#line 2208 "y.tab.c"
break;
case 10:
#line 146 "parsedate.y"
	{ param->yyHaveTime++; param->yyHaveDate++; 
				  param->yyHaveZone++; }
#line 2214 "y.tab.c"
break;
case 12:
#line 153 "parsedate.y"
	{
		param->yyYear = yystack.l_mark[-10].Number;
		if (param->yyYear < 100) {
			param->yyYear += 1900;
		}
		param->yyHaveFullYear = 1;
		param->yyMonth = yystack.l_mark[-8].Number;
		param->yyDay = yystack.l_mark[-6].Number;
		param->yyHour = yystack.l_mark[-4].Number;
		param->yyMinutes = yystack.l_mark[-2].Number;
		param->yySeconds = yystack.l_mark[0].Number;
		param->yyDSTmode = DSToff;
		param->yyTimezone = 0;
	}
#line 2232 "y.tab.c"
break;
case 13:
#line 170 "parsedate.y"
	{
		time_t	when = yystack.l_mark[0].Number;
		struct tm tmbuf;

		if (gmtime_r(&when, &tmbuf) != NULL) {
			param->yyYear = tmbuf.tm_year + 1900;
			param->yyMonth = tmbuf.tm_mon + 1;
			param->yyDay = tmbuf.tm_mday;

			param->yyHour = tmbuf.tm_hour;
			param->yyMinutes = tmbuf.tm_min;
			param->yySeconds = tmbuf.tm_sec;
		} else {
			param->yyYear = EPOCH;
			param->yyMonth = 1;
			param->yyDay = 1;

			param->yyHour = 0;
			param->yyMinutes = 0;
			param->yySeconds = 0;
		}
		param->yyHaveFullYear = 1;
		param->yyDSTmode = DSToff;
		param->yyTimezone = 0;
	}
#line 2261 "y.tab.c"
break;
case 16:
#line 203 "parsedate.y"
	{
		if (yystack.l_mark[-1].Number > 24)
			YYREJECT;
		param->yyMinutes = 0;
		param->yySeconds = 0;
		if (yystack.l_mark[0].Meridian == MER_NOON || yystack.l_mark[0].Meridian == MER_MN) {
			if (yystack.l_mark[-1].Number == 12) {
				switch (yystack.l_mark[0].Meridian) {
				case MER_NOON: param->yyHour = 12; break;
				case MER_MN  : param->yyHour = 0;  break;
				default:	/* impossible */;  break;
				}
				param->yyMeridian = MER24;
			} else
				YYREJECT;
		} else {
			param->yyHour = yystack.l_mark[-1].Number;
			param->yyMeridian = yystack.l_mark[0].Meridian;
		}
	  }
#line 2285 "y.tab.c"
break;
case 17:
#line 223 "parsedate.y"
	{
		if (yystack.l_mark[-3].Number > 24 || yystack.l_mark[-1].Number >= 60)
			YYREJECT;
		param->yyMinutes = yystack.l_mark[-1].Number;
		param->yySeconds = 0;
		if (yystack.l_mark[0].Meridian == MER_NOON || yystack.l_mark[0].Meridian == MER_MN) {
			if (yystack.l_mark[-3].Number == 12 && yystack.l_mark[-1].Number == 0) {
				switch (yystack.l_mark[0].Meridian) {
				case MER_NOON: param->yyHour = 12; break;
				case MER_MN  : param->yyHour = 0;  break;
				default:	/* impossible */;  break;
				}
				param->yyMeridian = MER24;
			} else
				YYREJECT;
		} else {
			param->yyHour = yystack.l_mark[-3].Number;
			param->yyMeridian = yystack.l_mark[0].Meridian;
		}
	  }
#line 2309 "y.tab.c"
break;
case 18:
#line 243 "parsedate.y"
	{
		if (yystack.l_mark[-5].Number > 24 || yystack.l_mark[-3].Number >= 60 || yystack.l_mark[-1].Number > 60)
			YYREJECT;
		param->yyMinutes = yystack.l_mark[-3].Number;
		param->yySeconds = yystack.l_mark[-1].Number;
		if (yystack.l_mark[0].Meridian == MER_NOON || yystack.l_mark[0].Meridian == MER_MN) {
			if (yystack.l_mark[-5].Number == 12 && yystack.l_mark[-3].Number == 0 && yystack.l_mark[-1].Number == 0) {
				switch (yystack.l_mark[0].Meridian) {
				case MER_NOON: param->yyHour = 12; break;
				case MER_MN  : param->yyHour = 0;  break;
				default:	/* impossible */;  break;
				}
				param->yyMeridian = MER24;
			} else
				YYREJECT;
		} else {
			param->yyHour = yystack.l_mark[-5].Number;
			param->yyMeridian = yystack.l_mark[0].Meridian;
		}
	  }
#line 2333 "y.tab.c"
break;
case 19:
#line 263 "parsedate.y"
	{
		if (yystack.l_mark[-6].Number > 24 || yystack.l_mark[-4].Number >= 60 || yystack.l_mark[-2].Number > 60)
			YYREJECT;
		param->yyHour = yystack.l_mark[-6].Number;
		param->yyMinutes = yystack.l_mark[-4].Number;
		param->yySeconds = yystack.l_mark[-2].Number;
		param->yyMeridian = MER24;
		/* XXX: Do nothing with fractional secs ($7) */
	  }
#line 2346 "y.tab.c"
break;
case 20:
#line 272 "parsedate.y"
	{
		if (yystack.l_mark[-6].Number > 24 || yystack.l_mark[-4].Number >= 60 || yystack.l_mark[-2].Number > 60)
			YYREJECT;
		param->yyHour = yystack.l_mark[-6].Number;
		param->yyMinutes = yystack.l_mark[-4].Number;
		param->yySeconds = yystack.l_mark[-2].Number;
		param->yyMeridian = MER24;
		/* XXX: Do nothing with fractional seconds ($7) */
	  }
#line 2359 "y.tab.c"
break;
case 21:
#line 281 "parsedate.y"
	{
		param->yyHour = yystack.l_mark[0].Number;
		param->yyMinutes = 0;
		param->yySeconds = 0;
		param->yyMeridian = MER24;
		/* Tues midnight --> Weds 00:00, midnight Tues -> Tues 00:00 */
		if (yystack.l_mark[0].Number == 0 && param->yyHaveDay)
			param->yyDayNumber++;
	  }
#line 2372 "y.tab.c"
break;
case 22:
#line 290 "parsedate.y"
	{
		if (yystack.l_mark[-1].Number == 12 && (yystack.l_mark[0].Number == 0 || yystack.l_mark[0].Number == 12)) {
			param->yyHour = yystack.l_mark[0].Number;
			param->yyMinutes = 0;
			param->yySeconds = 0;
			param->yyMeridian = MER24;
		} else
			YYREJECT;
	  }
#line 2385 "y.tab.c"
break;
case 23:
#line 302 "parsedate.y"
	{
		if (yystack.l_mark[0].Number < -(47 * 100 + 59) || yystack.l_mark[0].Number > (47 * 100 + 59))
			YYREJECT;
		if (yystack.l_mark[-3].Number > 24 || yystack.l_mark[-1].Number > 59)
			YYREJECT;
		param->yyHour = yystack.l_mark[-3].Number;
		param->yyMinutes = yystack.l_mark[-1].Number;
		param->yyMeridian = MER24;
		param->yyDSTmode = DSToff;
		param->yyTimezone = - (yystack.l_mark[0].Number % 100 + (yystack.l_mark[0].Number / 100) * 60);
	  }
#line 2400 "y.tab.c"
break;
case 24:
#line 313 "parsedate.y"
	{
		if (yystack.l_mark[0].Number < -(47 * 100 + 59) || yystack.l_mark[0].Number > (47 * 100 + 59))
			YYREJECT;
		if (yystack.l_mark[-5].Number > 24 || yystack.l_mark[-3].Number > 59 || yystack.l_mark[-1].Number > 60)
			YYREJECT;
		param->yyHour = yystack.l_mark[-5].Number;
		param->yyMinutes = yystack.l_mark[-3].Number;
		param->yySeconds = yystack.l_mark[-1].Number;
		param->yyMeridian = MER24;
		param->yyDSTmode = DSToff;
		param->yyTimezone = - (yystack.l_mark[0].Number % 100 + (yystack.l_mark[0].Number / 100) * 60);
	}
#line 2416 "y.tab.c"
break;
case 25:
#line 328 "parsedate.y"
	{ param->yyTimezone = yystack.l_mark[0].Number; param->yyDSTmode = DSToff; }
#line 2421 "y.tab.c"
break;
case 26:
#line 329 "parsedate.y"
	{ param->yyTimezone = yystack.l_mark[0].Number; param->yyDSTmode = DSTon; }
#line 2426 "y.tab.c"
break;
case 27:
#line 330 "parsedate.y"
	{ param->yyTimezone = yystack.l_mark[-1].Number; param->yyDSTmode = DSTon; }
#line 2431 "y.tab.c"
break;
case 28:
#line 331 "parsedate.y"
	{
			  if (param->yyHaveDate == 0 && param->yyHaveTime == 0)
				YYREJECT;
			  if (yystack.l_mark[0].Number < -(47 * 100 + 59) || yystack.l_mark[0].Number > (47 * 100 + 59))
				YYREJECT;
			  param->yyTimezone = - (yystack.l_mark[0].Number % 100 + (yystack.l_mark[0].Number / 100) * 60);
			  param->yyDSTmode = DSTmaybe;
			}
#line 2443 "y.tab.c"
break;
case 29:
#line 342 "parsedate.y"
	{ param->yyDayOrdinal = 1; param->yyDayNumber = yystack.l_mark[0].Number; }
#line 2448 "y.tab.c"
break;
case 30:
#line 343 "parsedate.y"
	{ param->yyDayOrdinal = 1; param->yyDayNumber = yystack.l_mark[-1].Number; }
#line 2453 "y.tab.c"
break;
case 31:
#line 344 "parsedate.y"
	{ param->yyDayOrdinal = yystack.l_mark[-1].Number; param->yyDayNumber = yystack.l_mark[0].Number; }
#line 2458 "y.tab.c"
break;
case 32:
#line 348 "parsedate.y"
	{
		if (yystack.l_mark[-2].Number > 12 || yystack.l_mark[0].Number > 31 || yystack.l_mark[-2].Number == 0 || yystack.l_mark[0].Number == 0)
			YYREJECT;
		param->yyMonth = yystack.l_mark[-2].Number;
		param->yyDay = yystack.l_mark[0].Number;
	  }
#line 2468 "y.tab.c"
break;
case 33:
#line 354 "parsedate.y"
	{
		if (yystack.l_mark[-4].Number >= 100) {
			if (yystack.l_mark[-2].Number > 12 || yystack.l_mark[0].Number > 31 || yystack.l_mark[-2].Number == 0 || yystack.l_mark[0].Number == 0)
				YYREJECT;
			param->yyYear = yystack.l_mark[-4].Number;
			param->yyMonth = yystack.l_mark[-2].Number;
			param->yyDay = yystack.l_mark[0].Number;
		} else {
			if (yystack.l_mark[-4].Number > 12 || yystack.l_mark[-2].Number > 31 || yystack.l_mark[-4].Number == 0 || yystack.l_mark[-2].Number == 0)
				YYREJECT;
			param->yyMonth = yystack.l_mark[-4].Number;
			param->yyDay = yystack.l_mark[-2].Number;
			param->yyYear = yystack.l_mark[0].Number;
		}
	  }
#line 2487 "y.tab.c"
break;
case 34:
#line 369 "parsedate.y"
	{
		/* ISO 8601 format.  yyyy-mm-dd.  */
		if (yystack.l_mark[-1].Number >= 0 || yystack.l_mark[-1].Number < -12 || yystack.l_mark[0].Number >= 0 || yystack.l_mark[0].Number < -31)
			YYREJECT;
		param->yyYear = yystack.l_mark[-2].Number;
		param->yyHaveFullYear = 1;
		param->yyMonth = -yystack.l_mark[-1].Number;
		param->yyDay = -yystack.l_mark[0].Number;
	  }
#line 2500 "y.tab.c"
break;
case 35:
#line 378 "parsedate.y"
	{
		if (yystack.l_mark[0].Number > 0 || yystack.l_mark[-2].Number == 0 || yystack.l_mark[-2].Number > 31)
			YYREJECT;
		/* e.g. 17-JUN-1992.  */
		param->yyDay = yystack.l_mark[-2].Number;
		param->yyMonth = yystack.l_mark[-1].Number;
		param->yyYear = -yystack.l_mark[0].Number;
	  }
#line 2512 "y.tab.c"
break;
case 36:
#line 386 "parsedate.y"
	{
		if (yystack.l_mark[0].Number == 0 || yystack.l_mark[0].Number > 31)
			YYREJECT;
		param->yyMonth = yystack.l_mark[-1].Number;
		param->yyDay = yystack.l_mark[0].Number;
	  }
#line 2522 "y.tab.c"
break;
case 37:
#line 392 "parsedate.y"
	{
		if (yystack.l_mark[-2].Number == 0 || yystack.l_mark[-2].Number > 31)
			YYREJECT;
		param->yyMonth = yystack.l_mark[-3].Number;
		param->yyDay = yystack.l_mark[-2].Number;
		param->yyYear = yystack.l_mark[0].Number;
	  }
#line 2533 "y.tab.c"
break;
case 38:
#line 399 "parsedate.y"
	{
		if (yystack.l_mark[-1].Number == 0 || yystack.l_mark[-1].Number > 31)
			YYREJECT;
		param->yyMonth = yystack.l_mark[0].Number;
		param->yyDay = yystack.l_mark[-1].Number;
	  }
#line 2543 "y.tab.c"
break;
case 39:
#line 405 "parsedate.y"
	{
		if (yystack.l_mark[-2].Number > 31 && yystack.l_mark[0].Number > 31)
			YYREJECT;
		if (yystack.l_mark[-2].Number < 35) {
			if (yystack.l_mark[-2].Number == 0)
				YYREJECT;
			param->yyDay = yystack.l_mark[-2].Number;
			param->yyYear = yystack.l_mark[0].Number;
		} else {
			if (yystack.l_mark[0].Number == 0)
				YYREJECT;
			param->yyDay = yystack.l_mark[0].Number;
			param->yyYear = yystack.l_mark[-2].Number;
		}
		param->yyMonth = yystack.l_mark[-1].Number;
	  }
#line 2563 "y.tab.c"
break;
case 41:
#line 425 "parsedate.y"
	{
		param->yyRel[param->yyHaveRel].yyRelVal =
		    -param->yyRel[param->yyHaveRel].yyRelVal;
	  }
#line 2571 "y.tab.c"
break;
case 42:
#line 432 "parsedate.y"
	{ CheckRelVal(param, yystack.l_mark[-1].Number, yystack.l_mark[0].Number, 60, 0); }
#line 2576 "y.tab.c"
break;
case 43:
#line 433 "parsedate.y"
	{ CheckRelVal(param, yystack.l_mark[-1].Number, yystack.l_mark[0].Number, 60, 0); }
#line 2581 "y.tab.c"
break;
case 44:
#line 434 "parsedate.y"
	{ CheckRelVal(param,  1, yystack.l_mark[0].Number, 60, 0); }
#line 2586 "y.tab.c"
break;
case 45:
#line 435 "parsedate.y"
	{ CheckRelVal(param, yystack.l_mark[-1].Number, 1,  1,  0); }
#line 2591 "y.tab.c"
break;
case 46:
#line 436 "parsedate.y"
	{ CheckRelVal(param, yystack.l_mark[-1].Number, 1,  1,  0); }
#line 2596 "y.tab.c"
break;
case 47:
#line 437 "parsedate.y"
	{ CheckRelVal(param,  1, 1,  1,  0); }
#line 2601 "y.tab.c"
break;
case 48:
#line 438 "parsedate.y"
	{ CheckRelVal(param, yystack.l_mark[-1].Number, yystack.l_mark[0].Number, 1,  1); }
#line 2606 "y.tab.c"
break;
case 49:
#line 439 "parsedate.y"
	{ CheckRelVal(param, yystack.l_mark[-1].Number, yystack.l_mark[0].Number, 1,  1); }
#line 2611 "y.tab.c"
break;
case 50:
#line 440 "parsedate.y"
	{ CheckRelVal(param,  1, yystack.l_mark[0].Number, 1,  1); }
#line 2616 "y.tab.c"
break;
case 51:
#line 444 "parsedate.y"
	{
		if (param->yyHaveTime && param->yyHaveDate &&
		    !param->yyHaveRel) {
			param->yyYear = yystack.l_mark[0].Number;
		} else {
			if (yystack.l_mark[0].Number > 10000) {
				param->yyHaveDate++;
				param->yyDay = (yystack.l_mark[0].Number)%100;
				param->yyMonth = (yystack.l_mark[0].Number/100)%100;
				param->yyYear = yystack.l_mark[0].Number/10000;
			}
			else {
				param->yyHaveTime++;
				if (yystack.l_mark[0].Number < 100) {
					param->yyHour = yystack.l_mark[0].Number;
					param->yyMinutes = 0;
				}
				else {
					param->yyHour = yystack.l_mark[0].Number / 100;
					param->yyMinutes = yystack.l_mark[0].Number % 100;
				}
				param->yySeconds = 0;
				param->yyMeridian = MER24;
			}
		}
	}
#line 2646 "y.tab.c"
break;
case 52:
#line 473 "parsedate.y"
	{ yyval.Meridian = MER24; }
#line 2651 "y.tab.c"
break;
case 53:
#line 474 "parsedate.y"
	{ yyval.Meridian = yystack.l_mark[0].Meridian; }
#line 2656 "y.tab.c"
break;
case 54:
#line 475 "parsedate.y"
	{ yyval.Meridian = yystack.l_mark[0].Number == 0 ? MER_MN : MER_NOON; }
#line 2661 "y.tab.c"
break;
#line 2663 "y.tab.c"
    default:
        break;
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
        {
            fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[YYFINAL], yyval));
#endif
            fprintf(stderr, "shifting from state 0 to final state %d\n", YYFINAL);
        }
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yyloc;
#endif
        if (yychar < 0)
        {
#if YYBTYACC
            do {
            if (yylvp < yylve)
            {
                /* we're currently re-reading tokens */
                yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylloc = *yylpp++;
#endif
                yychar = *yylexp++;
                break;
            }
            if (yyps->save)
            {
                /* in trial mode; save scanner results for future parse attempts */
                if (yylvp == yylvlim)
                {   /* Enlarge lexical value queue */
                    size_t p = (size_t) (yylvp - yylvals);
                    size_t s = (size_t) (yylvlim - yylvals);

                    s += YYLVQUEUEGROWTH;
                    if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL)
                        goto yyenomem;
                    if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL)
                        goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL)
                        goto yyenomem;
#endif
                    yylvp   = yylve = yylvals + p;
                    yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp   = yylpe = yylpsns + p;
                    yylplim = yylpsns + s;
#endif
                    yylexp  = yylexemes + p;
                }
                *yylexp = (YYINT) YYLEX;
                *yylvp++ = yylval;
                yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                *yylpp++ = yylloc;
                yylpe++;
#endif
                yychar = *yylexp++;
                break;
            }
            /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
            yychar = YYLEX;
#if YYBTYACC
            } while (0);
#endif /* YYBTYACC */
            if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
                fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)\n",
                                YYDEBUGSTR, yydepth, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if (((yyn = yygindex[yym]) != 0) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[yystate], yyval));
#endif
        fprintf(stderr, "shifting from state %d to state %d\n", *yystack.s_mark, yystate);
    }
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    *++yystack.p_mark = yyloc;
#endif
    goto yyloop;
#if YYBTYACC

    /* Reduction declares that this path is valid. Set yypath and do a full parse */
yyvalid:
    if (yypath) YYABORT;
    while (yyps->save)
    {
        YYParseState *save = yyps->save;
        yyps->save = save->save;
        save->save = yypath;
        yypath = save;
    }
#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%s[%d]: state %d, CONFLICT trial successful, backtracking to state %d, %d tokens\n",
                        YYDEBUGSTR, yydepth, yystate, yypath->state, (int)(yylvp - yylvals - yypath->lexeme));
#endif
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    yylvp          = yylvals + yypath->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yylpp          = yylpsns + yypath->lexeme;
#endif
    yylexp         = yylexemes + yypath->lexeme;
    yychar         = YYEMPTY;
    yystack.s_mark = yystack.s_base + (yypath->yystack.s_mark - yypath->yystack.s_base);
    memcpy (yystack.s_base, yypath->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
    yystack.l_mark = yystack.l_base + (yypath->yystack.l_mark - yypath->yystack.l_base);
    memcpy (yystack.l_base, yypath->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base + (yypath->yystack.p_mark - yypath->yystack.p_base);
    memcpy (yystack.p_base, yypath->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
    yystate        = yypath->state;
    goto yyloop;
#endif /* YYBTYACC */

yyoverflow:
    YYERROR_CALL("yacc stack overflow");
#if YYBTYACC
    goto yyabort_nomem;
yyenomem:
    YYERROR_CALL("memory exhausted");
yyabort_nomem:
#endif /* YYBTYACC */
    yyresult = 2;
    goto yyreturn;

yyabort:
    yyresult = 1;
    goto yyreturn;

yyaccept:
#if YYBTYACC
    if (yyps->save) goto yyvalid;
#endif /* YYBTYACC */
    yyresult = 0;

yyreturn:
#if defined(YYDESTRUCT_CALL)
    if (yychar != YYEOF && yychar != YYEMPTY)
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval, &yylloc);
#else
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */

    {
        YYSTYPE *pv;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYLTYPE *pp;

        for (pv = yystack.l_base, pp = yystack.p_base; pv <= yystack.l_mark; ++pv, ++pp)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv, pp);
#else
        for (pv = yystack.l_base; pv <= yystack.l_mark; ++pv)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
    }
#endif /* defined(YYDESTRUCT_CALL) */

#if YYBTYACC
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    while (yyps)
    {
        YYParseState *save = yyps;
        yyps = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
    while (yypath)
    {
        YYParseState *save = yypath;
        yypath = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
#endif /* YYBTYACC */
    yyfreestack(&yystack);
    return (yyresult);
}
