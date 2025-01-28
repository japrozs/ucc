#define extern_
#include "all.h"
#undef extern_

static void init_ucc(void)
{
    line = 1;
    putback_c = '\n';
}

char* token_strings[] = { "+", "-", "*", "/", "(integer literal)" };

// static void scan_file(void)
// {
//     struct token_t T;
//     while (scan(&T)) {
//         printf("Token %s", token_strings[T.token]);
//         if (T.token == T_INTLIT) {
//             printf(", value %d", T.int_value);
//         }
//         printf("\n");
//     }
// }

int main(int argc, char** argv)
{
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
    statements(); // Parse the statements in the input
    genpostamble(); // Output the postamble

    fclose(output_file);
    return 0;
}
