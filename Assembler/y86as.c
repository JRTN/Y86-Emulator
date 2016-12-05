#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"
#include "loader.h"
#include "util.h"

int main(int argc, char **argv) {
    char *programString = getInstructions(argv[1]);
    printf("Input:\n%sAssembled:\n", programString);
    assemble(programString);
    printf("\n");

    return 0;
}
