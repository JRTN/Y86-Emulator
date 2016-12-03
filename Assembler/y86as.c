#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"
#include "loader.h"

int main(int argc, char **argv) {
    char **programTokens = getInstructions(argv[1]);
    assemble(programTokens);
    free(programTokens);
}
