#include <stdio.h>

#include "util.h"

void assemble(char **programTokens) {
    int i = 0;
    while(programTokens[i]) {
        printf("%s\n", programTokens[i++]);
    }
}
