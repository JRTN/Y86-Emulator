#include "architecture.h"
#include <malloc.h>

static cpu_t cpu;

static char *memory;

int initialize(int32_t amt) {
    memory = malloc(amt);
    return memory != NULL;
}

void printCPU() {
    int i;
    for(i = 0; i < NUM_REGISTERS; i++) {
        printf("%d: %12d\n", i, cpu.registers[i]);
    }
    printf("IP: %11d\n", cpu.ipointer);
    printf("OF: %11d\n", cpu.OF);
    printf("SF: %11d\n", cpu.SF);
    printf("ZF: %11d\n", cpu.ZF);
}
