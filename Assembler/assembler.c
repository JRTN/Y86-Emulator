#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#define DELIMITERS " $(),%\n\t\v\f"

void assemble(char *program) {
    char *token = NULL;
    token = strtok(program, DELIMITERS);
    while( token ) {
        printf("%s\n", token);
        token = strtok(NULL, DELIMITERS);
    }
}
