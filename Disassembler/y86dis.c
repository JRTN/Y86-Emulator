#include <stdio.h>
#include <stdlib.h>
#include "loader.h"
#include "disassembler.h"

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "ERROR: No input file given\n");
        return 1;
    }
    char **instructions = getInstructions(argv[1]);
    if(instructions) {
        disassemble(instructions);
    }
    return 0;
}
