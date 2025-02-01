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

struct symtable_t {
    char* name; // Name of a symbol
};

#define NOREG -1 // Use NOREG when the AST generation

// global variables
extern_ int line;
extern_ int putback_c;
extern_ FILE* input_file;
extern_ FILE* output_file;
extern_ struct token_t recent_token;

#define TEXTLEN 512 // Length of symbols in input
extern_ char text[TEXTLEN + 1];

#define NSYMBOLS 1024 // Number of symbol table entries
extern_ struct symtable_t global_symbols[NSYMBOLS]; // Global symbol table

enum {
    T_EOF,
    T_PLUS,
    T_MINUS,
    T_STAR,
    T_SLASH,
    T_EQ,
    T_NE,
    T_LT,
    T_GT,
    T_LE,
    T_GE,
    T_INTLIT,
    T_SEMI,
    T_ASSIGN,
    T_IDENT,
    T_LBRACE,
    T_RBRACE,
    T_LPAREN,
    T_RPAREN,
    // Keywords
    T_PRINT,
    T_INT,
    T_IF,
    T_ELSE
};

// ast node types
enum {
    A_ADD = 1,
    A_SUBTRACT,
    A_MULTIPLY,
    A_DIVIDE,
    A_EQ,
    A_NE,
    A_LT,
    A_GT,
    A_LE,
    A_GE,
    A_INTLIT,
    A_IDENT,
    A_LVIDENT,
    A_ASSIGN,
    A_PRINT,
    A_GLUE,
    A_IF
};

struct ASTnode_t {
    int op; // "Operation" to be performed on this tree
    struct ASTnode_t* left; // Left, mid and right child trees
    struct ASTnode_t* mid;
    struct ASTnode_t* right;
    union {
        int int_value; // For A_INTLIT, the integer value
        int id; // For A_IDENT, the symbol slot number
    } v;
};

// scan.c
int scan(struct token_t* t);

// expr.c
struct ASTnode_t* binexpr(int ptp);

// tree.c
struct ASTnode_t* mkastnode(int op, struct ASTnode_t* left,
    struct ASTnode_t* mid,
    struct ASTnode_t* right, int intvalue);
struct ASTnode_t* mkastleaf(int op, int int_value);
struct ASTnode_t* mkastunary(int op, struct ASTnode_t* left, int intvalue);

// interp.c
// int interpret_ast(struct ASTnode_t* n);

// gen.c
// void generate_code(struct ASTnode_t* n);
// int gen_ast(struct ASTnode_t* n, int reg);
int gen_ast(struct ASTnode_t* n, int reg, int parent_ast_op);

void genpreamble(void);
void genpostamble(void);
void genfreeregs(void);
void genprintint(int reg);
void genglobsym(char* s);

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
int cgloadint(int value);
int cgloadglob(char* identifier);
int cgstorglob(int r, char* identifier);
void cgglobsym(char* sym);
int cgcompare(int r1, int r2, char* how);
int cgequal(int r1, int r2);
int cgnotequal(int r1, int r2);
int cglessthan(int r1, int r2);
int cggreaterthan(int r1, int r2);
int cglessequal(int r1, int r2);
int cggreaterequal(int r1, int r2);
int cgcompare_and_set(int ASTop, int r1, int r2);
void cglabel(int l);
void cgjump(int l);
int cgcompare_and_jump(int ASTop, int r1, int r2, int label);

// stmt.c
// void statements(void);
struct ASTnode_t* compound_statement(void);

// sym.c
int findglob(char* s);
int addglob(char* name);

// utils.c
#define RED_BOLD "\033[1;31m"
#define BLUE_BOLD "\033[1;34m"
#define GRAY "\033[0;37m"
#define RESET_COLOR "\033[0m"

void die(char* fmt, ...);
void die_on_line(char* fmt, ...);
void err(char* fmt, ...);

void match(int t, char* what);
void semi(void);
void ident(void);
void rbrace(void);
void lparen(void);
void rparen(void);
void lbrace(void);

#endif
