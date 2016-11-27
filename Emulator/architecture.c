#include <string.h>
#include <malloc.h>
#include <ctype.h>

#include "architecture.h"
#include "util.h"

static cpu_t cpu;
static char *memory;
static int32_t size;
static status_t status = AOK;

static void halt(void);
static void rrmovl(void);
static void irmovl(void);
static void rmmovl(void);
static void mrmovl(void);

static void addl(void);
static void subl(void);
static void andl(void);
static void xorl(void);
static void mull(void);
static void cmpl(void);

static void jmp(void);
static void jle(void);
static void jne(void);
static void jge(void);
static void jg(void);
static void jl(void);
static void je(void);

static void call(void);
static void ret(void);

static void pushl(void);
static void popl(void);
static void readb(void);
static void readl(void);
static void writeb(void);
static void writel(void);

static void movsbl(void);

/*
    Executes the instructions stored in memory until the status of the machine
    is no longer AOK. There are three stop conditions:
    HLT - This is a normal halt and is specified by the user in the machine instructions
    ADR - An invalid address has been encountered
    INS - An invalid Instruction has been encountered
    Return:
        The status of the machine when it stops.
*/
status_t execute() {
    char *instruction = calloc(3, sizeof(char));
    instruction[2] = '\0';
    while(status == AOK) {
        strncpy(instruction, memory + cpu.ipointer, 2);
        int32_t n_instruction = hexToDec(instruction);
        printf("%x\n", n_instruction);
        switch(n_instruction) {
            case 0x00: /* nop */
                printf("nop\n");
                cpu.ipointer += 1 BYTE;
            break;
            case 0x10: /* halt */
                printf("halt\n");
                halt();
                cpu.ipointer += 1 BYTE;
            break;
            case 0x20: /* rrmovl */
                printf("rrmovl\n");
                rrmovl();
                cpu.ipointer += 2 BYTE;
            break;
            case 0x30: /* irmovl */
                printf("irmovl\n");
                irmovl();
                cpu.ipointer += 6 BYTE;
            break;
            case 0x40: /* rmmovl */
                printf("rmmovl\n");
                rmmovl();
                cpu.ipointer += 6 BYTE;
            break;
            case 0x50: /* mrmovl */
                printf("mrmovl\n");
                mrmovl();
                cpu.ipointer += 6 BYTE;
            break;
            case 0x60: /* addl */
                printf("addl\n");
                addl();
                cpu.ipointer += 2 BYTE;
            break;
            case 0x61: /* subl */
                printf("subl\n");
                subl();
                cpu.ipointer += 2 BYTE;
            break;
            case 0x62: /* andl */
                printf("andl\n");
                andl();
                cpu.ipointer += 2 BYTE;
            break;
            case 0x63: /* xorl */
                printf("xorl\n");
                xorl();
                cpu.ipointer += 2 BYTE;
            break;
            case 0x64: /* mull */
                printf("mull\n");
                mull();
                cpu.ipointer += 2 BYTE;
            break;
            case 0x65: /* cmpl */
                printf("cmpl\n");
                cmpl();
                cpu.ipointer += 2 BYTE;
            break;
            case 0x70: /* jmp */
                printf("jmp\n");
                jmp();
                cpu.ipointer += 5 BYTE;
            break;
            case 0x71: /* jle */
                printf("jle\n");
                jle();
                cpu.ipointer += 5 BYTE;
            break;
            case 0x72: /* jl */
                printf("jl\n");
                jl();
                cpu.ipointer += 5 BYTE;
            break;
            case 0x73: /* je */
                printf("je\n");
                je();
                cpu.ipointer += 5 BYTE;
            break;
            case 0x74: /* jne */
                printf("jne\n");
                jne();
                cpu.ipointer += 5 BYTE;
            break;
            case 0x75: /* jge */
                printf("jge\n");
                jge();
                cpu.ipointer += 5 BYTE;
            break;
            case 0x76: /* jg */
                printf("jg\n");
                jg();
                cpu.ipointer += 5 BYTE;
            break;
            case 0x80: /* call */
                printf("call\n");
                call();
                cpu.ipointer += 5 BYTE;
            break;
            case 0x90: /* ret */
                printf("ret\n");
                ret();
                cpu.ipointer += 1 BYTE;
            break;
            case 0xA0: /* pushl */
                printf("pushl\n");
                pushl();
                cpu.ipointer += 2 BYTE;
            break;
            case 0xB0: /* popl */
                printf("popl\n");
                popl();
                cpu.ipointer += 2 BYTE;                
            break;
            case 0xC0: /* readb */
                printf("readb\n");
                readb();
                cpu.ipointer += 6 BYTE;
            break;
            case 0xC1: /* readl */
                printf("readl\n");
                readl();
                cpu.ipointer += 6 BYTE;
            break;
            case 0xD0: /* writeb */
                printf("writeb\n");
                writeb();
                cpu.ipointer += 6 BYTE;
            break;
            case 0xD1: /* writel */
                printf("writel\n");
                writel();
                cpu.ipointer += 6 BYTE;
            break;
            case 0xE0: /* movsbl */
                printf("movsbl\n");
                movsbl();
                cpu.ipointer += 6 BYTE;
            break;
            default:
                status = INS;
                printf("Unknown Instruction Encountered\n");
        }
    }
    free(instruction);
    return status;
}

/*
    Allocates the necessary amount of memory needed for program execution
    Arguments:
        int32_t amt - the amount (in bytes) of memory to be allocated
    Return:
        1 if memory allocation is successful; 0 otherwise
*/
int initialize(int32_t amt) {
    size = amt;
    memory = malloc(amt);
    if(memory) {
        printf("Allocated %d bytes of memory for the memory space\n", amt);
    }
    return memory != NULL;
}

int bss(int32_t amt, int32_t addr) {
    int i;
    int ok = 1;
    for(i = 0; i < amt; i++) {
        ok *= putByte(0, addr + i);
    }
    return ok;
}

/*
    Stores the instruction string in memory and sets the instruction pointer to
    the beginning of the instructions
    Arguments:
        char *instructions - the string containing the machine instructions
        int32_t addr - the address where the instructions will be stored and
                       where the instruction pointer will be set
    Return:
        1 if there were no issues storing the instructions; 0 otherwise  
*/
int insertInstructions(char *instructions, int32_t addr) {
    cpu.ipointer = addr;
    return putString(instructions, addr);
}

/*
    Stores a string in memory
    Arguments:
        char *str - the string to be stored in memory
        int32_t addr - the address in memory where the string will be stored
    Return:
        1 if the entire string was successfully stored in memory; 0 otherwise
*/
int putString(char *str, int32_t addr) {
    int i = 0;
    int ok = 1;
    while(str[i] && ok) {
        ok *= putByte(str[i], addr + i);
        i++;
    }
    /*TODO: Maybe include null terminating character later on down the line */
    return ok;
}

/*
    Stores a 4 byte integer in memory
    Arguments:
        int32_t num - the 32 bit integer that is to be stored in memory
        int32_t addr - the address of where the integer is to be stored
                       in memory
    Return:
        1 if the integer is successfully put in memory; 0 if the address
        is out of range.
*/
int putLong(int32_t num, int32_t addr) {
    if(addr + 4 >= size) {
        return 0;
    }

    int32_t * loc = (int32_t*)(&memory[addr]);
    *loc = num;

    return 1;
}

/*
    Retrieves a 4 byte integer stored in memory.
    Arguments:
        int32_t addr - the address of the first byte containing the value
                       of the long
    Return:
        the 32 bit integer stored at the given location in memory if there
        are no issues; 0 otherwise (which can also be a valid return)
*/
int32_t getLong(int32_t addr) {
    if(addr + 4 >= size) {
        return 0;
    }
    int32_t *loc = (int32_t*)(&memory[addr]);
    return *loc;
}

/*
    Sets a single byte at the given position in memory.
    Arguments:
        char byte - the byte to be put in memory
        int32_t addr - the position in memory that the byte will be put
    Return:
        1 if no issues are encountered and the byte is successfully set in memory;
        0 otherwise
*/
int putByte(char byte, int32_t addr) {
    if(addr >= size) {
        return 0;
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
        if(isprint(memory[i])) {
            printf("%c", memory[i]);
        } else {
            printf("-");
        }
    }
}

static void halt() {
    status = HLT;
}

static void rrmovl() {

}

static void irmovl() {

}

static void rmmovl() {

}

static void mrmovl() {

}

static void addl() {

}

static void subl() {

}

static void andl() {

}

static void xorl() {

}

static void mull() {

}

static void cmpl() {

}

static void jmp() {

}

static void jle() {

}

static void jne() {

}

static void jge() {

}

static void jg() {

}

static void jl() {

}

static void je() {

}

static void call() {

}

static void ret() {

}

static void pushl() {

}

static void popl() {

}

static void readb() {

}

static void readl() {

}

static void writeb() {

}

static void writel() {

}

static void movsbl() {

}
