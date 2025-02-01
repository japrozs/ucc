#include "all.h"
#include <stdlib.h>

struct ASTnode_t* mkastnode(int op, struct ASTnode_t* left,
    struct ASTnode_t* mid,
    struct ASTnode_t* right, int int_value)
{
    struct ASTnode_t* n;

    // Malloc a new ASTnode
    n = (struct ASTnode_t*)malloc(sizeof(struct ASTnode_t));
    if (n == NULL)
        err("Unable to malloc in mkastnode()");

    // Copy in the field values and return it
    n->op = op;
    n->left = left;
    n->mid = mid;
    n->right = right;
    n->v.int_value = int_value;
    return (n);
}

struct ASTnode_t* mkastleaf(int op, int int_value)
{
    return mkastnode(op, NULL, NULL, NULL, int_value);
}

struct ASTnode_t* mkastunary(int op, struct ASTnode_t* left, int int_value)
{
    return mkastnode(op, left, NULL, NULL, int_value);
}
