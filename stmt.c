
#include "all.h"

static struct ASTnode_t* print_statement(void)
{
    struct ASTnode_t* tree;
    // int reg;

    // Match a 'print' as the first token
    match(T_PRINT, "print");

    // Parse the following expression
    tree = binexpr(0);

    // Make an print AST tree
    tree = mkastunary(A_PRINT, tree, 0);

    // Match the following semicolon
    // and return the AST
    semi();
    return (tree);
}

// Parse the declaration of a variable
static struct ASTnode_t* var_declaration(void)
{
    // Ensure we have an 'int' token followed by an identifier
    // and a semicolon. text now has the identifier's name.
    // Add it as a known identifier
    match(T_INT, "int");
    ident();
    addglob(text);
    genglobsym(text);
    semi();
}

static struct ASTnode_t* assignment_statement(void)
{
    struct ASTnode_t *left, *right, *tree;
    int id;

    // Ensure we have an identifier
    ident();

    // Check it's been defined then make a leaf node for it
    if ((id = findglob(text)) == -1) {
        die_on_line("Undeclared variable '%s'", text);
    }
    right = mkastleaf(A_LVIDENT, id);

    // Ensure we have an equals sign
    match(T_ASSIGN, "=");

    // Parse the following expression
    left = binexpr(0);

    // Make an assignment AST tree
    tree = mkastnode(A_ASSIGN, left, NULL, right, 0);

    // Match the following semicolon
    // and return the AST
    semi();
    return (tree);
}

struct ASTnode_t* if_statement(void)
{
    struct ASTnode_t *condAST, *trueAST, *falseAST = NULL;

    // Ensure we have 'if' '('
    match(T_IF, "if");
    lparen();

    // Parse the following expression
    // and the ')' following. Ensure
    // the tree's operation is a comparison.
    condAST = binexpr(0);

    if (condAST->op < A_EQ || condAST->op > A_GE)
        die_on_line("Bad comparison operator");
    rparen();

    // Get the AST for the compound statement
    trueAST = compound_statement();

    // If we have an 'else', skip it
    // and get the AST for the compound statement
    if (recent_token.token == T_ELSE) {
        scan(&recent_token);
        falseAST = compound_statement();
    }
    // Build and return the AST for this statement
    return (mkastnode(A_IF, condAST, trueAST, falseAST, 0));
}

// void statements(void)
// {
//     while (1) {
//         switch (recent_token.token) {
//         case T_PRINT:
//             print_statement();
//             break;
//         case T_INT:
//             var_declaration();
//             break;
//         case T_IDENT:
//             assignment_statement();
//             break;
//         case T_EOF:
//             return;
//         default:
//             err("Syntax error, unexpected token %d", recent_token.token);
//         }
//     }
// }

struct ASTnode_t* compound_statement(void)
{
    struct ASTnode_t* left = NULL;
    struct ASTnode_t* tree;

    // Require a left curly bracket
    lbrace();

    while (1) {
        switch (recent_token.token) {
        case T_PRINT:
            tree = print_statement();
            break;
        case T_INT:
            var_declaration();
            tree = NULL; // No AST generated here
            break;
        case T_IDENT:
            tree = assignment_statement();
            break;
        case T_IF:
            tree = if_statement();
            break;
        case T_RBRACE:
            // When we hit a right curly bracket,
            // skip past it and return the AST
            rbrace();
            return (left);
        default:
            die_on_line("Syntax error handling token %d", recent_token.token);
        }

        // For each new tree, either save it in left
        // if left is empty, or glue the left and the
        // new tree together
        if (tree) {
            if (left == NULL)
                left = tree;
            else
                left = mkastnode(A_GLUE, left, NULL, tree, 0);
        }
    }
}