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

static int scanident(int c, char* buf, int lim)
{
    int i = 0;

    // Allow digits, alpha and underscores
    while (isalpha(c) || isdigit(c) || '_' == c) {
        // Error if we hit the identifier length limit,
        // else append to buf[] and get next character
        if (lim - 1 == i) {
            die_on_line("identifier too long");
        } else if (i < lim - 1) {
            buf[i++] = c;
        }
        c = next();
    }
    // We hit a non-valid character, put it back.
    // NUL-terminate the buf[] and return the length
    putback(c);
    buf[i] = '\0';
    return (i);
}

// find keywords in the code
static int keyword(char* s)
{
    // match with the first char of the string (i.e. the pointer)
    switch (*s) {
    case 'p':
        if (!strcmp(s, "print"))
            return (T_PRINT);
        break;
    case 'i':
        if (!strcmp(s, "int"))
            return (T_INT);
        break;
    }
    return (0);
}

int scan(struct token_t* t)
{
    int c = skip();
    int tokentype;

    switch (c) {
    case EOF:
        t->token = T_EOF;
        return 0;
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
    case ';':
        t->token = T_SEMI;
        break;
    case '=':
        if ((c = next()) == '=') {
            t->token = T_EQ;
        } else {
            putback(c);
            t->token = T_ASSIGN;
        }
        break;
    case '!':
        if ((c = next()) == '=') {
            t->token = T_NE;
        } else {
            die_on_line("Unrecognised character '%c'", c);
        }
        break;
    case '<':
        if ((c = next()) == '=') {
            t->token = T_LE;
        } else {
            putback(c);
            t->token = T_LT;
        }
        break;
    case '>':
        if ((c = next()) == '=') {
            t->token = T_GE;
        } else {
            putback(c);
            t->token = T_GT;
        }
        break;
    default:
        // If it's a digit, scan the
        // literal integer value in
        if (isdigit(c)) {
            t->int_value = scanint(c);
            t->token = T_INTLIT;
            break;
        } else if (isalpha(c) || '_' == c) {
            // Read in a keyword or identifier
            scanident(c, text, TEXTLEN);

            // If it's a recognised keyword, return that token
            if ((tokentype = keyword(text))) {
                t->token = tokentype;
                break;
            }
            // Not a recognised keyword, so an error for now
            t->token = T_IDENT;
            break;
        }

        die_on_line("Unrecognised character '%c'", c);
    }

    return (1);
}
