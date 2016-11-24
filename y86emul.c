#include "loader.h"
#include "architecture.h"

#include <stdio.h>

int main(int argc, char **argv) {
    printCPU();
	if(!loadFileIntoMemory(argv[1])) {
        printf("\nerror\n");
    }
    return 0;
}
