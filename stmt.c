#include "all.h"

void statements(void)
{
    struct ASTnode_t* tree;
    int reg;

    while (1) {
        // Match a 'print' as the first token
        match(T_PRINT, "print");

        // Parse the following expression and
        // generate the assembly code
        tree = binexpr(0);
        reg = gen_ast(tree);
        genprintint(reg);
        genfreeregs();

        // Match the following semicolon
        // and stop if we are at EOF
        semi();
        if (recent_token.token == T_EOF)
            return;
    }
}