#include "all.h"

// List of available registers
// and their names
static int freereg[4];
static char* reg_list[4] = { "%r8", "%r9", "%r10", "%r11" };

// Set all registers as available
void freeall_registers(void)
{
    freereg[0] = freereg[1] = freereg[2] = freereg[3] = 1;
}

// Allocate a free register. Return the number of
// the register. Die if no available registers.
static int alloc_register(void)
{
    for (int i = 0; i < 4; i++) {
        if (freereg[i]) {
            freereg[i] = 0;
            return (i);
        }
    }
    err("Out of registers!");
    exit(1);
}

// Return a register to the list of available registers.
// Check to see if it's not already there.
static void free_register(int reg)
{
    if (freereg[reg] != 0) {
        err("Error trying to free register %d", reg);
        exit(1);
    }
    freereg[reg] = 1;
}

void cgpreamble(void)
{
    freeall_registers();
    fputs(
        "\t.text\n"
        ".LC0:\n"
        "\t.string\t\"%d\\n\"\n"
        "printint:\n"
        "\tpushq\t%rbp\n"
        "\tmovq\t%rsp, %rbp\n"
        "\tsubq\t$16, %rsp\n"
        "\tmovl\t%edi, -4(%rbp)\n"
        "\tmovl\t-4(%rbp), %eax\n"
        "\tmovl\t%eax, %esi\n"
        "\tleaq	.LC0(%rip), %rdi\n"
        "\tmovl	$0, %eax\n"
        // "\tcall	printf@PLT\n"
        "\tcall _printf\n"
        "\tnop\n"
        "\tleave\n"
        "\tret\n"
        "\n"
        "\t.globl\t_main\n"
        // TODO: this is to make sure clang works,
        // 		 if the binary starts acting up,
        // 		 uncomment this
        // "\t.type\tmain, @function\n"
        "_main:\n"
        "\tpushq\t%rbp\n"
        "\tmovq	%rsp, %rbp\n",
        output_file);
}

// Print out the assembly postamble
void cgpostamble(void)
{
    fputs(
        "\tmovl	$0, %eax\n"
        "\tpopq	%rbp\n"
        "\tret\n",
        output_file);
}

// load an integer literal value into a register.
// return the number of the register
int cgload(int value)
{
    // Get a new register
    int r = alloc_register();

    // Print out the code to initialise it
    fprintf(output_file, "\tmovq\t$%d, %s\n", value, reg_list[r]);
    return (r);
}

// add two registers together and return
// the number of the register with the result
int cgadd(int r1, int r2)
{
    fprintf(output_file, "\taddq\t%s, %s\n", reg_list[r1], reg_list[r2]);
    free_register(r1);
    return (r2);
}

// multiply two registers together and return
// the number of the register with the result
int cgmul(int r1, int r2)
{
    fprintf(output_file, "\timulq\t%s, %s\n", reg_list[r1], reg_list[r2]);
    free_register(r1);
    return (r2);
}

// subtract the second register from the first and
// return the number of the register with the result
int cgsub(int r1, int r2)
{
    fprintf(output_file, "\tsubq\t%s, %s\n", reg_list[r2], reg_list[r1]);
    free_register(r2);
    return (r1);
}

// divide the first register by the second and
// return the number of the register with the result
int cgdiv(int r1, int r2)
{
    fprintf(output_file, "\tmovq\t%s,%%rax\n", reg_list[r1]);
    fprintf(output_file, "\tcqo\n");
    fprintf(output_file, "\tidivq\t%s\n", reg_list[r2]);
    fprintf(output_file, "\tmovq\t%%rax,%s\n", reg_list[r1]);
    free_register(r2);
    return (r1);
}

void cgprintint(int r)
{
    fprintf(output_file, "\tmovq\t%s, %%rdi\n", reg_list[r]);
    fprintf(output_file, "\tcall\tprintint\n");
    free_register(r);
}