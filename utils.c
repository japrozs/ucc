#include "all.h"

void die(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    // fprintf(stderr, "ucc: %serror:%s %s:%d: ", RED_BOLD, RESET_COLOR, input_file, line);
    // fprintf(stderr, "ucc: %serror:%s line %d: ", RED_BOLD, RESET_COLOR, line);
    fprintf(stderr, "ucc: %serror:%s ", RED_BOLD, RESET_COLOR);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    printf("Exited with exit code : 1\n");
    exit(EXIT_FAILURE);
}

void die_on_line(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    // fprintf(stderr, "ucc: %serror:%s %s:%d: ", RED_BOLD, RESET_COLOR, input_file, line);
    fprintf(stderr, "ucc: %serror:%s line %d: ", RED_BOLD, RESET_COLOR, line);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    printf("Exited with exit code : 1\n");
    exit(EXIT_FAILURE);
}

void err(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    // fprintf(stderr, "ucc: %serror:%s %s:%d: ", RED_BOLD, RESET_COLOR, input_file, line);
    fprintf(stderr, "%serror:%s ", RED_BOLD, RESET_COLOR);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    printf("Exited with exit code : 1\n");
    exit(EXIT_FAILURE);
}

// Ensure that the current token is t,
// and fetch the next token. Otherwise
// throw an error
void match(int t, char* what)
{
    if (recent_token.token == t) {
        scan(&recent_token);
    } else {
        die_on_line("%s expected on line", what);
        exit(1);
    }
}

// Match a semicon and fetch the next token
void semi(void)
{
    match(T_SEMI, ";");
}

void ident(void) { match(T_IDENT, "identifier"); }
