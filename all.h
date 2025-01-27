#ifndef _ALL_H_
#define _ALL_H_

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef extern_
#define extern_ extern
#endif

struct token_t {
    int token;
    int int_value;
};

// global variables
extern_ int line;
extern_ int putback_c;
extern_ FILE* input_file;
extern_ FILE* output_file;
extern_ struct token_t recent_token;

#define TEXTLEN 512 // Length of symbols in input
extern_ char text[TEXTLEN + 1];

enum {
    T_EOF,
    T_PLUS,
    T_MINUS,
    T_STAR,
    T_SLASH,
    T_INTLIT,
    T_SEMI,
    T_PRINT
};

// ast node types
enum {
    A_ADD,
    A_SUBTRACT,
    A_MULTIPLY,
    A_DIVIDE,
    A_INTLIT
};

struct ASTnode_t {
    int op; // "Operation" to be performed on this tree
    struct ASTnode_t* left; // Left and right child trees
    struct ASTnode_t* right;
    int int_value; // For A_INTLIT, the integer value
};

int scan(struct token_t* t);

// expr.c
struct ASTnode_t* binexpr(int ptp);

// tree.c
struct ASTnode_t* mkastnode(int op, struct ASTnode_t* left,
    struct ASTnode_t* right, int int_value);
struct ASTnode_t* mkastleaf(int op, int int_value);
struct ASTnode_t* mkastunary(int op, struct ASTnode_t* left, int intvalue);

// interp.c
int interpret_ast(struct ASTnode_t* n);

// gen.c
void generate_code(struct ASTnode_t* n);
int gen_ast(struct ASTnode_t* n);

void genpreamble(void);
void genpostamble(void);
void genfreeregs(void);
void genprintint(int reg);

// cg.c
void freeall_registers(void);
void cgpreamble(void);
void cgpostamble(void);
int cgload(int value);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);

// stmt.c
void statements(void);

// utils.c
#define RED_BOLD "\033[1;31m"
#define BLUE_BOLD "\033[1;34m"
#define GRAY "\033[0;37m"
#define RESET_COLOR "\033[0m"

void die(char* fmt, ...);
void die_on_line(char* fmt, ...);
void err(char* fmt, ...);

// Ensure that the current token is t,
// and fetch the next token. Otherwise
// throw an error
void match(int t, char* what);

// Match a semicon and fetch the next token
void semi(void);

#endif
