#include <stdio.h>
#include "loader.h"
#include "disassembler.h"

int main(int argc, char **argv) {
    char **instructions = getInstructions(argv[1]);
    disassemble(instructions);
    return 0;
}
