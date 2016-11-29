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
static void mov(int);
static void op(int);
static void jXX(int);

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
    while(status == AOK) {
        char *instruction = nt_strncpy(memory + cpu.ipointer, 2);
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
            break;
            case 0x20: /* rrmovl */
                printf("rrmovl\n");
                mov(RR);
            break;
            case 0x30: /* irmovl */
                printf("irmovl\n");
                mov(IR);
            break;
            case 0x40: /* rmmovl */
                printf("rmmovl\n");
                mov(RM);
            break;
            case 0x50: /* mrmovl */
                printf("mrmovl\n");
                mov(MR);
            break;
            case 0x60: /* addl */
                printf("addl\n");
                op(ADD);
            break;
            case 0x61: /* subl */
                printf("subl\n");
                op(SUB);
            break;
            case 0x62: /* andl */
                printf("andl\n");
                op(AND);
            break;
            case 0x63: /* xorl */
                printf("xorl\n");
                op(XOR);
            break;
            case 0x64: /* mull */
                printf("mull\n");
                op(MUL);
            break;
            case 0x65: /* cmpl */
                printf("cmpl\n");
                op(CMP);
            break;
            case 0x70: /* jmp */
                printf("jmp\n");
                jXX(JMP);
            break;
            case 0x71: /* jle */
                printf("jle\n");
                jXX(JLE);
            break;
            case 0x72: /* jl */
                printf("jl\n");
                jXX(JL);
            break;
            case 0x73: /* je */
                printf("je\n");
                jXX(JE);
            break;
            case 0x74: /* jne */
                printf("jne\n");
                jXX(JNE);
            break;
            case 0x75: /* jge */
                printf("jge\n");
                jXX(JGE);
            break;
            case 0x76: /* jg */
                printf("jg\n");
                jXX(JG);
            break;
            case 0x80: /* call */
                printf("call\n");
                call();
            break;
            case 0x90: /* ret */
                printf("ret\n");
                ret();
            break;
            case 0xA0: /* pushl */
                printf("pushl\n");
                pushl();
            break;
            case 0xB0: /* popl */
                printf("popl\n");
                popl();
            break;
            case 0xC0: /* readb */
                printf("readb\n");
                readb();
            break;
            case 0xC1: /* readl */
                printf("readl\n");
                readl();
            break;
            case 0xD0: /* writeb */
                printf("writeb\n");
                writeb();
            break;
            case 0xD1: /* writel */
                printf("writel\n");
                writel();
            break;
            case 0xE0: /* movsbl */
                printf("movsbl\n");
                movsbl();
            break;
            default:
                status = INS;
                printf("Unknown Instruction Encountered\n");
        }
        free(instruction);
    }
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
    cpu.ipointer += 1 BYTE;
}

/*
    Performs mov instructions of the given type. There are 4 types of mov:
        1. RR - Register to Register move
                rB <- rA
                2 byte length
        2. IR - Immediate to Register move
                rB <- val
                6 byte length
        3. RM - Register to Memory move
                val(rB) <- rA
                6 byte length
        4. MR - Memory to Register move
                rA <- val(rB)
    Arguments:
        int fn - The type of mov instruction to be performed
                
*/
void mov(int fn) {
    int32_t rA = memory[cpu.ipointer + 2] - '0';
    int32_t rB = memory[cpu.ipointer + 3] - '0';
    if(fn == RR) {
        printf("Copying contents [%d] of register %d into register %d\n", cpu.registers[rA], rA, rB);
        cpu.registers[rB] = cpu.registers[rA];
        cpu.ipointer += 2 BYTE;
        return;
    }
    char *valStr = nt_strncpy(memory + cpu.ipointer + 2 BYTE, 4 BYTE);
    int32_t val = hexToDec(valStr);
    switch(fn) {
        case IR:
            cpu.registers[rB] = val;
            printf("Putting value %d into register %d\n", val, rB);
        break;
        case RM: {
            int32_t dst = cpu.registers[rB] + val;
            printf("Copying value [%d] from register %d into memory location %d\n", cpu.registers[rA], rA, dst);
            putLong(val, dst);
        }
        break;
        case MR: {
            int32_t src = cpu.registers[rB] + val;
            int32_t res = getLong(src);
            printf("Copying value [%d] from memory location %d into register %d\n", res, src, rA);
            cpu.registers[rA] = res;
        }
        break;
    }
    free(valStr);
    cpu.ipointer += 6 BYTE;
}

/*
    Performs a given operation and sets the appropriate flags. There are 6 operations:
        1. ADD - rB = rB + rA
        2. SUB - rB = rB - rA
        3. AND - rB = rB && rA
        4. XOR - rB = rB ^ rA
        5. MUL - rB = rB * rA
        6. CMP - rB - rA and set flags accordingly
    Arguments:
        int fn - the operation to be performed
*/
static void op(int fn) {
    int32_t rA = memory[cpu.ipointer + 2] - '0';
    int32_t rB = memory[cpu.ipointer + 3] - '0';
    int32_t result = 0;
    int32_t valA = cpu.registers[rA];
    int32_t valB = cpu.registers[rB];
    switch(fn) {
        case ADD:
            result = valB + valA;
            /*
                Overflow if:
                    valA is positive, valB is positive, and result is negative
                    or
                    valA is negative, valB is negative, and result is positive
            */
            cpu.OF = (valA > 0 && valB > 0 && result < 0) ||
                     (valA < 0 && valB < 0 && result > 0);
        break;
        case SUB:
        case CMP:
            result = valB - valA;
            /*
                Overflow if:
                    valB is negative, valA is positive, and result is positive
                    or
                    valB is positive, valA is negative, and result is negative
            */
            cpu.OF = (valB < 0 && valA > 0 && result > 0) ||
                     (valB > 0 && valA < 0 && result < 0);
        break;
        case AND:
            result = valB && valA;
            cpu.OF = 0;
        break;
        case XOR:
            result = valB ^ valA;
            cpu.OF = 0;
        break;
        case MUL:
            result = valB * valA;
            /*
                Overflow if:
                    valA is positive, valB is positive, and result is negative
                    or
                    valA is negative, valB is negative, and result is negative
                    or
                    one val is positive, one val is negative, and result is positive
            */
            cpu.OF = (valA > 0 && valB > 0 && result < 0) ||
                     (valA < 0 && valB < 0 && result < 0) ||
                     ((valA < 0 ^ valB < 0) && (valA > 0 ^ valB > 0) && result > 0);
        break;
    }
    cpu.ZF = result == 0;
    cpu.SF = result < 0;
    if(fn != CMP) {
        cpu.registers[rB] = result;
    }
    cpu.ipointer += 2 BYTE;
}

/*
    
*/
static void jXX(int fn) {
    int shouldJump = 0;
    char *destString = nt_strncpy(memory + cpu.ipointer + 1 BYTE, 4 BYTE);
    int32_t destination = hexToDec(destString);
    printf("Jumping to destination %s = %d\n", destString, destination);
    switch(fn) {
        case JLE:
            shouldJump = (cpu.SF ^ cpu.OF) || cpu.ZF;
        break;
        case JL:
            shouldJump = cpu.SF ^ cpu.OF;
        break;
        case JE:
            shouldJump = cpu.ZF;
        break;
        case JNE:
            shouldJump = !cpu.ZF;
        break;
        case JGE:
            shouldJump = !(cpu.SF ^ cpu.OF);
        break;
        case JG:
            shouldJump = !(cpu.SF ^ cpu.OF) && !cpu.ZF;
        break;
    }

    if(shouldJump || fn == JMP) {
        cpu.ipointer = destination;
    }
}

static void call() {
    cpu.ipointer += 5 BYTE;
}

static void ret() {
    cpu.ipointer += 1 BYTE;
}

static void pushl() {
    cpu.ipointer += 2 BYTE;
}

static void popl() {
    cpu.ipointer += 2 BYTE;
}

static void readb() {
    cpu.ipointer += 6 BYTE;
}

static void readl() {
    cpu.ipointer += 6 BYTE;
}

static void writeb() {
    cpu.ipointer += 6 BYTE;
}

static void writel() {
    cpu.ipointer += 6 BYTE;
}

static void movsbl() {
    cpu.ipointer += 6 BYTE;
}
