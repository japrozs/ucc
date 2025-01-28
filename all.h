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
    T_INTLIT,
    T_SEMI,
    T_PRINT,
    T_EQUALS,
    T_INT,
    T_IDENT
};

// ast node types
enum {
    A_ADD,
    A_SUBTRACT,
    A_MULTIPLY,
    A_DIVIDE,
    A_INTLIT,
    A_IDENT,
    A_LVIDENT,
    A_ASSIGN
};

struct ASTnode_t {
    int op; // "Operation" to be performed on this tree
    struct ASTnode_t* left; // Left and right child trees
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
    struct ASTnode_t* right, int int_value);
struct ASTnode_t* mkastleaf(int op, int int_value);
struct ASTnode_t* mkastunary(int op, struct ASTnode_t* left, int intvalue);

// interp.c
// int interpret_ast(struct ASTnode_t* n);

// gen.c
// void generate_code(struct ASTnode_t* n);
int gen_ast(struct ASTnode_t* n, int reg);

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

// stmt.c
void statements(void);

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

#endif
