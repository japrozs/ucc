#define extern_
#include "all.h"
#undef extern_

static void init_ucc(void)
{
    line = 1;
    putback_c = '\n';
}

char* token_strings[] = { "+", "-", "*", "/", "(integer literal)" };

int main(int argc, char** argv)
{
    struct ASTnode_t* tree;
    if (argc < 2) {
        die("no input files");
    }

    init_ucc();
    input_file = fopen(argv[1], "r");

    if (input_file == NULL) {
        die("Unable to open %s: %s", argv[1], strerror(errno));
    }

    output_file = fopen("out.s", "w");
    if (output_file == NULL) {
        die("Unable to create out.s: %s\n", strerror(errno));
    }

    scan(&recent_token); // Get the first token from the input
    genpreamble(); // Output the preamble
    tree = compound_statement(); // Parse the compound statement in the input
    gen_ast(tree, NOREG, 0); // Generate the assembly code for it
    genpostamble();

    fclose(output_file);
    return 0;
}
