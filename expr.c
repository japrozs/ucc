#include "all.h"

static int operator_precedence[] = {
    0, 10, 10, // T_EOF, T_PLUS, T_MINUS
    20, 20, // T_STAR, T_SLASH
    30, 30, // T_EQ, T_NE
    40, 40, 40, 40 // T_LT, T_GT, T_LE, T_GE
};

int arithop(int tokentype)
{
    if (tokentype > T_EOF && tokentype < T_INTLIT)
        return (tokentype);
    die_on_line("Syntax error, token '%d'", tokentype);
    // just to satisfy clang [-Wreturn-mismatch]
    return -1;
}

static struct ASTnode_t* primary(void)
{
    struct ASTnode_t* n = NULL;
    int id;

    switch (recent_token.token) {
    case T_INTLIT:
        // For an INTLIT token, make a leaf AST node for it.
        n = mkastleaf(A_INTLIT, recent_token.int_value);
        break;

    case T_IDENT:
        // Check that this identifier exists
        id = findglob(text);
        if (id == -1)
            die_on_line("Unknown variable '%s'", text);

        // Make a leaf AST node for it
        n = mkastleaf(A_IDENT, id);
        break;

    default:
        die_on_line("Syntax error handling token '%d'", recent_token.token);
    }

    // Scan in the next token and return the leaf node
    scan(&recent_token);
    return (n);
}

// Check that we have a binary operator and
// return its precedence.
static int op_precedence(int tokentype)
{
    int prec = operator_precedence[tokentype];
    if (prec == 0) {
        die_on_line("Syntax error handling token %d", tokentype);
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
    if (tokentype == T_SEMI || tokentype == T_RPAREN)
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
        left = mkastnode(arithop(tokentype), left, NULL, right, 0);

        // Update the details of the current token.
        // If we hit a semicolon, return just the left node
        tokentype = recent_token.token;
        if (tokentype == T_SEMI || tokentype == T_RPAREN)
            return (left);
    }

    // Return the tree we have when the precedence
    // is the same or lower
    return (left);
}
