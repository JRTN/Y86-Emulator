#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"
#include "loader.h"
#include "util.h"

char *ops[28] = {"nop", "halt", "rrmovl", "irmovl", "rmmovl", "mrmovl", "addl", "subl", "andl", "xorl", "mull",
                 "cmpl", "jmp", "jle", "jl", "je", "jne", "jge", "jg", "call", "ret", "pushl", "popl", "readb",
                 "readl", "writeb", "writel", "movsbl"};

int main(int argc, char **argv) {
    char *programString = getInstructions(argv[1]);
    assemble(programString);
    /*int i;
    for(i = 0; i < 28; i++) {
        unsigned long h = hash(ops[i]);
        printf("%s : 0x%x\n", ops[i], h);
    }*/

    return 0;
}
