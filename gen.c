#include "all.h"

static int gen_if_ast(struct ASTnode_t* n);

void genpreamble(void) { cgpreamble(); }
void genpostamble(void) { cgpostamble(); }
void genfreeregs(void) { freeall_registers(); }
void genprintint(int reg) { cgprintint(reg); }
void genglobsym(char* s) { cgglobsym(s); }

static int label(void)
{
    static int id = 1;
    return (id++);
}

int gen_ast(struct ASTnode_t* n, int reg, int parent_ast_op)
{
    int leftreg, rightreg;

    switch (n->op) {
    case A_IF:
        return (gen_if_ast(n));
    case A_GLUE:
        // Do each child statement, and free the
        // registers after each child
        gen_ast(n->left, NOREG, n->op);
        genfreeregs();
        gen_ast(n->right, NOREG, n->op);
        genfreeregs();
        return (NOREG);
    }

    // Get the left and right sub-tree values
    if (n->left)
        leftreg = gen_ast(n->left, NOREG, n->op);
    if (n->right)
        rightreg = gen_ast(n->right, leftreg, n->op);

    switch (n->op) {
    case A_ADD:
        return (cgadd(leftreg, rightreg));
    case A_SUBTRACT:
        return (cgsub(leftreg, rightreg));
    case A_MULTIPLY:
        return (cgmul(leftreg, rightreg));
    case A_DIVIDE:
        return (cgdiv(leftreg, rightreg));
    case A_EQ:
    case A_NE:
    case A_LT:
    case A_GT:
    case A_LE:
    case A_GE:
        // If the parent AST node is an A_IF, generate a compare
        // followed by a jump. Otherwise, compare registers and
        // set one to 1 or 0 based on the comparison.
        if (parent_ast_op == A_IF)
            return (cgcompare_and_jump(n->op, leftreg, rightreg, reg));
        else
            return (cgcompare_and_set(n->op, leftreg, rightreg));
    case A_INTLIT:
        return (cgloadint(n->v.int_value));
    case A_IDENT:
        return (cgloadglob(global_symbols[n->v.id].name));
    case A_LVIDENT:
        return (cgstorglob(reg, global_symbols[n->v.id].name));
    case A_ASSIGN:
        // The work has already been done, return the result
        return (rightreg);
    case A_PRINT:
        // Print the left-child's value
        // and return no register
        genprintint(leftreg);
        genfreeregs();
        return (NOREG);

    default:
        err("Unknown AST operator %d", n->op);
    }

    // just to satisfy clang [-Wreturn-mismatch]
    return -1;
}

static int gen_if_ast(struct ASTnode_t* n)
{
    int Lfalse, Lend;

    // Generate two labels: one for the
    // false compound statement, and one
    // for the end of the overall IF statement.
    // When there is no ELSE clause, Lfalse _is_
    // the ending label!
    Lfalse = label();
    if (n->right)
        Lend = label();

    // Generate the condition code followed
    // by a zero jump to the false label.
    // We cheat by sending the Lfalse label as a register.
    gen_ast(n->left, Lfalse, n->op);
    genfreeregs();

    // Generate the true compound statement
    gen_ast(n->mid, NOREG, n->op);
    genfreeregs();

    // If there is an optional ELSE clause,
    // generate the jump to skip to the end
    if (n->right)
        cgjump(Lend);

    // Now the false label
    cglabel(Lfalse);

    // Optional ELSE clause: generate the
    // false compound statement and the
    // end label
    if (n->right) {
        gen_ast(n->right, NOREG, n->op);
        genfreeregs();
        cglabel(Lend);
    }

    return (NOREG);
}
