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

typedef int32_t reg_t;

typedef enum status_e {
    AOK, HLT, ADR, INS
} status_t;

typedef union ByteParts_u {
    struct {
        unsigned int first : 4;
        unsigned int second : 4;
    } halves;
    char c;
} ByteParts_t;

typedef struct cpu_s {
    reg_t registers[NUM_REGISTERS];
    int32_t ipointer;

    int8_t OF;
    int8_t SF;
    int8_t ZF;
} cpu_t;

int initialize(int32_t);
void printCPU(void);
void printMemory(void);
int insertInstructions(char*, int32_t);
int putString(char*, int32_t);
int putByte(char, int32_t);
int putLong(int32_t, int32_t);

void halt(void);
void rrmovl(reg_t, reg_t);
void irmovl(int32_t, reg_t);
void rmmovl(reg_t, reg_t, int32_t);
void mrmovl(reg_t, reg_t, int32_t);

void addl(reg_t, reg_t);
void subl(reg_t, reg_t);
void andl(reg_t, reg_t);
void xorl(reg_t, reg_t);
void mull(reg_t, reg_t);
void cmpl(reg_t, reg_t);

void jmp(int32_t);
void jle(int32_t);
void jne(int32_t);
void jge(int32_t);
void jg(int32_t);
void jl(int32_t);
void je(int32_t);

void call(int32_t);

void ret(void);

void pushl(reg_t);

void popl(reg_t);

void readb(reg_t, int32_t);
void readl(reg_t, int32_t);

void writeb(reg_t, int32_t);
void writel(reg_t, int32_t);

void movsbl(reg_t, reg_t, int32_t);

#endif
