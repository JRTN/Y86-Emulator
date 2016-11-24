#include <stdio.h>
#include "loader.h"

void disassemble(char *instructions) {

}

int main(int argc, char **argv) {
    if(argc > 1) {
        char *instructions = getInstructionsFromFile(argv[1]);
        printf("%s\n", instructions);
    } else {
        printf("Need arguments\n");
        return 1;
    }
    return 0;
}
