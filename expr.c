#include "all.h"

static int operator_precedence[] = { 0, 10, 10, 20, 20, 0 };
//                     EOF  +   -   *   /  INTLIT

int arithop(int tok)
{
    switch (tok) {
    case T_PLUS:
        return (A_ADD);
    case T_MINUS:
        return (A_SUBTRACT);
    case T_STAR:
        return (A_MULTIPLY);
    case T_SLASH:
        return (A_DIVIDE);
    default:
        die_on_line("Unknown token in arithop()");
    }
}

static struct ASTnode_t* primary(void)
{
    struct ASTnode_t* n;

    // For an INTLIT token, make a leaf AST node for it
    // and scan in the next token. Otherwise, a syntax error
    // for any other token type.
    switch (recent_token.token) {
    case T_INTLIT:
        n = mkastleaf(A_INTLIT, recent_token.int_value);
        scan(&recent_token);
        return n;
    default:
        die_on_line("Syntax error");
    }
}

// Check that we have a binary operator and
// return its precedence.
static int op_precedence(int tokentype)
{
    int prec = operator_precedence[tokentype];
    if (prec == 0) {
        die_on_line("syntax error handling token %d", tokentype);
    }
    return (prec);
}

struct ASTnode_t* binexpr(int ptp)
{
    struct ASTnode_t *left, *right;
    int tokentype;

    // Get the integer literal on the left.
    // Fetch the next token at the same time.
    left = primary();

    // If we hit a semicolon, return just the left node
    tokentype = recent_token.token;
    if (tokentype == T_SEMI)
        return (left);

    // While the precedence of this token is
    // more than that of the previous token precedence
    while (op_precedence(tokentype) > ptp) {
        // Fetch in the next integer literal
        scan(&recent_token);

        // Recursively call binexpr() with the
        // precedence of our token to build a sub-tree
        right = binexpr(operator_precedence[tokentype]);

        // Join that sub-tree with ours. Convert the token
        // into an AST operation at the same time.
        left = mkastnode(arithop(tokentype), left, right, 0);

        // Update the details of the current token.
        // If we hit a semicolon, return just the left node
        tokentype = recent_token.token;
        if (tokentype == T_SEMI)
            return (left);
    }

    // Return the tree we have when the precedence
    // is the same or lower
    return (left);
}