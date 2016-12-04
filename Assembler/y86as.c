#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"
#include "loader.h"

int main(int argc, char **argv) {
    char *programString = getInstructions(argv[1]);
    assemble(programString);

    return 0;
}
