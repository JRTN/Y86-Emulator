#ifndef architecture_h
#define architecture_h

#include <stdint.h>

#define NUM_REGISTERS 8

#define BYTE *2

#define EAX 0
#define ECX 1
#define EDX 2
#define EBX 3
#define ESP 4
#define EBP 5
#define ESI 6
#define EDI 7

typedef int32_t reg_t;

typedef enum status_e {
    AOK, HLT, ADR, INS
} status_t;

typedef struct cpu_s {
    reg_t registers[NUM_REGISTERS];
    int32_t ipointer;
    int8_t OF;
    int8_t SF;
    int8_t ZF;
} cpu_t;

status_t execute(void);

int initialize(int32_t);
void printCPU(void);
void printMemory(void);
int insertInstructions(char*, int32_t);
int putString(char*, int32_t);
int putByte(char, int32_t);
int putLong(int32_t, int32_t);
int bss(int32_t, int32_t);


#endif
