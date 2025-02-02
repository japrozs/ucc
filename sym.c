#include "all.h"

static int next_gslot_pos = 0; // Position of next free global symbol slot

// Determine if the symbol s is in the global symbol table.
// Return its slot position or -1 if not found.
int findglob(char* s)
{
    int i;

    for (i = 0; i < next_gslot_pos; i++) {
        if (*s == *global_symbols[i].name && !strcmp(s, global_symbols[i].name))
            return (i);
    }
    return (-1);
}

// Get the position of a new global symbol slot, or die
// if we've run out of positions.
static int newglob(void)
{
    int p;

    if ((p = next_gslot_pos++) >= NSYMBOLS)
        err("Too many global symbols");
    return (p);
}

// Add a global symbol to the symbol table.
// Return the slot number in the symbol table
int addglob(char* name)
{
    int y;

    // If this is already in the symbol table, return the existing slot
    if ((y = findglob(name)) != -1)
        return (y);

    // Otherwise get a new slot, fill it in and
    // return the slot number
    y = newglob();
    global_symbols[y].name = strdup(name);
    return (y);
}
