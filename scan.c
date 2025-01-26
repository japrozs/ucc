#include "all.h"

static void putback(int c)
{
    putback_c = c;
}

static int next(void)
{
    int c;

    if (putback_c) {
        c = putback_c;
        putback_c = 0;
        return c;
    }

    c = fgetc(input_file);
    if ('\n' == c) {
        line++;
    }
    return c;
}

static int skip(void)
{
    int c;

    c = next();
    while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c) {
        c = next();
    }
    return c;
}

static int chrpos(char* s, int c)
{
    char* p;

    p = strchr(s, c);
    return (p ? p - s : -1);
}
static int scanint(int c)
{
    int k, val = 0;

    // Convert each character into an int value
    while ((k = chrpos("0123456789", c)) >= 0) {
        val = val * 10 + k;
        c = next();
    }

    // We hit a non-integer character, put it back.
    putback(c);
    return val;
}

int scan(struct token_t* t)
{
    int c = skip();
    switch (c) {
    case EOF:
        return (0);
    case '+':
        t->token = T_PLUS;
        break;
    case '-':
        t->token = T_MINUS;
        break;
    case '*':
        t->token = T_STAR;
        break;
    case '/':
        t->token = T_SLASH;
        break;
    default:
        // If it's a digit, scan the
        // literal integer value in
        if (isdigit(c)) {
            t->int_value = scanint(c);
            t->token = T_INTLIT;
            break;
        }

        die_on_line("Unrecognised character '%c'", c);
    }

    return (1);
}
