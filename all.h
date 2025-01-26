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

// global variables
extern_ int line;
extern_ int putback_c;
extern_ FILE* input_file;

struct token_t {
    int token;
    int int_value;
};

enum {
    T_PLUS,
    T_MINUS,
    T_STAR,
    T_SLASH,
    T_INTLIT
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

// utils.c
#define RED_BOLD "\033[1;31m"
#define BLUE_BOLD "\033[1;34m"
#define GRAY "\033[0;37m"
#define RESET_COLOR "\033[0m"

void die(char* fmt, ...);
void die_on_line(char* fmt, ...);
void err(char* fmt, ...);

#endif
