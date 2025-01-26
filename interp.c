#include "all.h"

static char* ASTop[] = { "+", "-", "*", "/" };

int interpret_ast(struct ASTnode_t* n)
{
    int leftval, rightval;

    if (n->left)
        leftval = interpret_ast(n->left);
    if (n->right)
        rightval = interpret_ast(n->right);

    // dbg statements
    if (n->op == A_INTLIT)
        printf("integer literal %d\n", n->int_value);
    else
        printf("%d %s %d\n", leftval, ASTop[n->op], rightval);

    switch (n->op) {
    case A_ADD:
        return (leftval + rightval);
    case A_SUBTRACT:
        return (leftval - rightval);
    case A_MULTIPLY:
        return (leftval * rightval);
    case A_DIVIDE:
        return (leftval / rightval);
    case A_INTLIT:
        return (n->int_value);
    default:
        err("Unknown AST operator %d", n->op);
    }
}