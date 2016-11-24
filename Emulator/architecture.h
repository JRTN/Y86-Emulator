#ifndef architecture_h
#define architecture_h

#include <stdint.h>

#define NUM_REGISTERS 8

#define SIZE_D ".size"
#define STRING_D ".string"
#define LONG_D ".long"
#define BSS_D ".bss"
#define BYTE_D ".byte"
#define TEXT_D ".text"

#define EAX 0
#define ECX 1
#define EDX 2
#define EBX 3
#define ESP 4
#define EBP 5
#define ESI 6
#define EDI 7

typedef int32_t register_t;

typedef enum status_e {
    AOK, HLT, ADR, INS
} status_t;

typedef union ByteParts_u {
    struct {
        unsigned int first : 4;
        unsigned int second : 4;
    } halves;
    unsigned char c;
} ByteParts_t;

typedef struct cpu_s {
    register_t registers[NUM_REGISTERS];
    int32_t ipointer;

    int8_t OF;
    int8_t SF;
    int8_t ZF;
} cpu_t;

int initialize(int32_t);
void printCPU(void);
void printMemory(void);
int insertInstructions(char*, int32_t);
int putByte(char, int32_t);

void halt(void);
void rrmovl(register_t, register_t);
void irmovl(int32_t, register_t);
void rmmovl(register_t, register_t, int32_t);
void mrmovl(register_t, register_t, int32_t);

void addl(register_t, register_t);
void subl(register_t, register_t);
void andl(register_t, register_t);
void xorl(register_t, register_t);
void mull(register_t, register_t);
void cmpl(register_t, register_t);

void jmp(int32_t);
void jle(int32_t);
void jne(int32_t);
void jge(int32_t);
void jg(int32_t);
void jl(int32_t);
void je(int32_t);

void call(int32_t);

void ret(void);

void pushl(register_t);

void popl(register_t);

void readb(register_t, int32_t);
void readl(register_t, int32_t);

void writeb(register_t, int32_t);
void writel(register_t, int32_t);

void movsbl(register_t, register_t, int32_t);

#endif
