#include "all.h"

static int gen_ast(struct ASTnode_t* n)
{
    int leftreg, rightreg;

    // Get the left and right sub-tree values
    if (n->left)
        leftreg = gen_ast(n->left);
    if (n->right)
        rightreg = gen_ast(n->right);

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
        return (cgload(n->int_value));

    default:
        err("Unknown AST operator %d", n->op);
    }
}

void generate_code(struct ASTnode_t* n)
{
    int reg;

    cgpreamble();
    reg = gen_ast(n);
    cgprintint(reg); // Print the register with the result as an int
    cgpostamble();
}