#include "loader.h"
#include "architecture.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "ERROR: Must have at least one argument\n");
        return 1;
    }
    if(strcmp("-h", argv[1]) == 0) {
        printf("Usage: y86emul <inputfile>\n");
        return 0;
    }
	if(!loadFileIntoMemory(argv[1])) {
        return 1;
    }
    status_t stat = execute();
    char *status;
    if(stat == HLT) {
        status = "HLT";
    } else if(stat == ADR) {
        status = "ADR";
    } else if(stat == INS) {
        status = "INS";
    }
    printf("\nEnd Status: %s\n", status);
    return 0;
}
