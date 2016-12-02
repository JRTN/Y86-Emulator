#ifndef architecture_h
#define architecture_h

#include <stdint.h>

#define NUM_REGISTERS 8

#define EAX 0
#define ECX 1
#define EDX 2
#define EBX 3
#define ESP 4
#define EBP 5
#define ESI 6
#define EDI 7

#define ADD 0
#define SUB 1
#define AND 2
#define XOR 3
#define MUL 4
#define CMP 5

#define JMP 0
#define JLE 1
#define JL  2
#define JE  3
#define JNE 4
#define JGE 5
#define JG  6

#define RR 0
#define IR 1
#define RM 2
#define MR 3
#define SB 4

#define B 0
#define L 1

typedef union byteParts_u {
    struct {
        unsigned int first:4;
        unsigned int second:4;
    } parts;
    char c;
} byteParts_t;

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
