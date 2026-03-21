#define TEOF 0
#define TNL 1
#define TSEMI 2
#define TBACKGND 3
#define TAND 4
#define TOR 5
#define TPIPE 6
#define TLP 7
#define TRP 8
#define TENDCASE 9
#define TCASEFALL 10
#define TENDBQUOTE 11
#define TREDIR 12
#define TWORD 13
#define TIF 14
#define TTHEN 15
#define TELSE 16
#define TELIF 17
#define TFI 18
#define TWHILE 19
#define TUNTIL 20
#define TFOR 21
#define TDO 22
#define TDONE 23
#define TBEGIN 24
#define TEND 25
#define TCASE 26
#define TESAC 27
#define TNOT 28

/* Array indicating which tokens mark the end of a list */
const char tokendlist[] = {
	1,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	1,
	1,
	1,
	1,
	0,
	0,
	0,
	1,
	1,
	1,
	1,
	0,
	0,
	0,
	1,
	1,
	0,
	1,
	0,
	1,
	0,
};

const char *const tokname[] = {
	"end of file",
	"newline",
	"\";\"",
	"\"&\"",
	"\"&&\"",
	"\"||\"",
	"\"|\"",
	"\"(\"",
	"\")\"",
	"\";;\"",
	"\";&\"",
	"\"`\"",
	"redirection",
	"word",
	"\"if\"",
	"\"then\"",
	"\"else\"",
	"\"elif\"",
	"\"fi\"",
	"\"while\"",
	"\"until\"",
	"\"for\"",
	"\"do\"",
	"\"done\"",
	"\"{\"",
	"\"}\"",
	"\"case\"",
	"\"esac\"",
	"\"!\"",
};

#define KWDOFFSET 14

const char *const parsekwd[] = {
	"if",
	"then",
	"else",
	"elif",
	"fi",
	"while",
	"until",
	"for",
	"do",
	"done",
	"{",
	"}",
	"case",
	"esac",
	"!",
	0
};
