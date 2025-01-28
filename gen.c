#include "all.h"

int gen_ast(struct ASTnode_t* n, int reg)
{
    int leftreg, rightreg;

    // Get the left and right sub-tree values
    if (n->left)
        leftreg = gen_ast(n->left, -1);
    if (n->right)
        rightreg = gen_ast(n->right, leftreg);

    switch (n->op) {
    case A_ADD:
        return (cgadd(leftreg, rightreg));
    case A_SUBTRACT:
        return (cgsub(leftreg, rightreg));
    case A_MULTIPLY:
        return (cgmul(leftreg, rightreg));
    case A_DIVIDE:
        return (cgdiv(leftreg, rightreg));
    case A_INTLIT:
        return (cgloadint(n->v.int_value));
    case A_IDENT:
        return (cgloadglob(global_symbols[n->v.id].name));
    case A_LVIDENT:
        return (cgstorglob(reg, global_symbols[n->v.id].name));
    case A_ASSIGN:
        // The work has already been done, return the result
        return (rightreg);

    default:
        err("Unknown AST operator %d", n->op);
    }
}

void genpreamble(void) { cgpreamble(); }
void genpostamble(void) { cgpostamble(); }
void genfreeregs(void) { freeall_registers(); }
void genprintint(int reg) { cgprintint(reg); }
void genglobsym(char* s) { cgglobsym(s); }