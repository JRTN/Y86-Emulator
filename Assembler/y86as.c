#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"
#include "loader.h"
#include "util.h"

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "ERROR: No input file given\n");
        return 1;
    }
    char *programString = getInstructions(argv[1]);
    if(programString) {
        printf("Input:\n%sAssembled:\n", programString);
        assemble(programString);
        free(programString);
        printf("\n");
    }
    return 0;
}
