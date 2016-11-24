#include "architecture.h"
#include <malloc.h>

static cpu_t cpu;
static char *memory;
static int32_t size;

static status_t status = AOK;

status_t execute() {
    while(status == AOK) {

    }
    return status;
}

int initialize(int32_t amt) {
    size = amt;
    memory = calloc(amt + 1, sizeof(char));
    if(memory) {
        memory[amt] = '\0';
    }
    return memory != NULL;
}

int insertInstructions(char *instructions, int32_t addr) {
    cpu.ipointer = addr;
    int i = 0;
    while(instructions[i]) {
        memory[addr + i] = instructions[i];
        i++;
    }
    printMemory();
    return 1;
}

int putByte(char byte, int32_t addr) {
    if(addr >= size) {
        return 0;
    }

    if(memory[addr]) {
        printf("WARNING: Memory at 0x%X being overwritten\n", addr);
    }
    memory[addr] = byte;
    return 1;
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

void printMemory() {
    int i;
    for(i = 0; i < size; i++) {
        if(!memory[i]) {
            printf("-");
        } else {
            printf("%c", memory[i]);
        }
    }
}
