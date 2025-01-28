#include "all.h"

void print_statement(void)
{
    struct ASTnode_t* tree;
    int reg;

    // Match a 'print' as the first token
    match(T_PRINT, "print");

    // Parse the following expression and
    // generate the assembly code
    tree = binexpr(0);
    reg = gen_ast(tree, -1);
    genprintint(reg);
    genfreeregs();

    // Match the following semicolon
    semi();
}

// Parse the declaration of a variable
void var_declaration(void)
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

void assignment_statement(void)
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
    match(T_EQUALS, "=");

    // Parse the following expression
    left = binexpr(0);

    // Make an assignment AST tree
    tree = mkastnode(A_ASSIGN, left, right, 0);

    // Generate the assembly code for the assignment
    gen_ast(tree, -1);
    genfreeregs();

    // Match the following semicolon
    semi();
}

void statements(void)
{
    while (1) {
        switch (recent_token.token) {
        case T_PRINT:
            print_statement();
            break;
        case T_INT:
            var_declaration();
            break;
        case T_IDENT:
            assignment_statement();
            break;
        case T_EOF:
            return;
        default:
            err("Syntax error, unexpected token %d", recent_token.token);
        }
    }
}
