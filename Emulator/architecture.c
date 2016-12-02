#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <time.h>

#include "architecture.h"
#include "util.h"

#define clear() printf("\033[H\033[J")
#define DEBUG 0
#define CALL 1

static cpu_t cpu;
static char *memory;
static int32_t size;
static status_t status = AOK;

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
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
    if(DEBUG) printf("Allocated %d bytes of memory for address space\n", amt);
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
    size_t instrLength = strlen(instructions);
    int ok = 1;
    int i = 0;
    while( (i < instrLength) && ok) {
        char *byteString = nt_strncpy(instructions + i, 2);
        char byte = (char) hexToDec(byteString);
        ok *= putByte(byte, addr + (i / 2));
        i += 2;
    }
    return ok;
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

static void checkbound(int32_t addr) {
    if(addr >= size) {
        printf("Attemped to access out of bound address 0x%x\n", addr);
        status = ADR;
    }
}

static void halt() {
    status = HLT;
    cpu.ipointer += 1;
}

/*
    Performs mov instructions of the given type. There are 4 types of mov:
        1. RR - Register to Register move
                2 byte length
        2. IR - Immediate to Register move
                6 byte length
        3. RM - Register to Memory move
                6 byte length
        4. MR - Memory to Register move
                6 byte length
    Arguments:
        int fn - The type of mov instruction to be performed
                
*/
static void mov(int fn) {
    byteParts_t parts;
    parts.c = memory[cpu.ipointer + 1];
    int rA = parts.parts.second;
    int rB = parts.parts.first;
    if(DEBUG) printf("rA:%d rB:%d\n", rA, rB);
    if(fn == RR) {
        /*
            Register to Register move
            Behavior: rB <- rA
        */
        cpu.registers[rB] = cpu.registers[rA];
        cpu.ipointer += 2;
        return;
    }
    int32_t val = getLong(cpu.ipointer + 2);;
    switch(fn) {
        case IR:
            /*
                Immediate to Register move
                Behavior: rB <- val
            */
            cpu.registers[rB] = val;
        break;
        case RM: {
            /*
                Register to Memory move
                Behavior: val(rB) <- rA
            */
            int32_t dst = cpu.registers[rB] + val;
            putLong(val, dst);
        }
        break;
        case MR: {
            /*
                Memory to Register move
                rA <- val(rB)
            */
            int32_t src = cpu.registers[rB] + val;
            int32_t res = memory[src];
            cpu.registers[rA] = res;
        }
        break;
        case SB: {
            int32_t src = cpu.registers[rB] + val;
            int8_t item = memory[src];
            int32_t extended = (int32_t) item;
            if(DEBUG) printf("Sign extended 0x%x to 0x%x\n", item, extended);
            cpu.registers[rA] = item;
        }
        break;
    }
    cpu.ipointer += 6;
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
    byteParts_t parts;
    parts.c = memory[cpu.ipointer + 1];
    int rA = parts.parts.second;
    int rB = parts.parts.first;
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
            if(DEBUG) printf("[rB]%d - [rA]%d = [result]%d\n", valB, valA, result);
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
            /*
                There can't be overflow from 'and' operations.
            */
            cpu.OF = 0;
        break;
        case XOR:
            result = valB ^ valA;
            /*
                There can't be overflow from 'xor' operations
            */
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
                     (( (valA < 0) ^ (valB < 0) ) && ( (valA > 0) ^ (valB > 0) ) && result > 0);
        break;
    }
    /*
        ZF is set if rA OP rB is 0
        SF is set if rA OP rB is less than 0
    */
    cpu.ZF = result == 0;
    cpu.SF = result < 0;
    /*
        cmp does not change registers; it only sets flags
    */
    if(fn != CMP) {
        cpu.registers[rB] = result;
    }
    cpu.ipointer += 2;
}

/*
    Performs a given jump operation based on the cpu flags.
    Arguments:
        int fn - The jump operation to be performed
*/
static void jXX(int fn) {
    int shouldJump = 0;
    int32_t destination = getLong(cpu.ipointer + 1);
    checkbound(destination);
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
        if(DEBUG) printf("Jumping to location 0x%x\n", destination);
    } else {
        cpu.ipointer += 5;
    }
}

static void push(int32_t data) {
    cpu.registers[ESP] -= 4;
    if(DEBUG) printf("Pushing value 0x%x onto the stack\n", data);
    if(DEBUG) printf("ESP: %d\n", cpu.registers[ESP]);
    putLong(data, cpu.registers[ESP]);
}

static void pushl() {
    byteParts_t parts;
    parts.c = memory[cpu.ipointer + 1];
    int rA = parts.parts.second;
    push(cpu.registers[rA]);
    cpu.ipointer += 2;
}

int32_t pop() {
    int32_t res = getLong(cpu.registers[ESP]);
    if(DEBUG) printf("Popping value 0x%x from the stack\n", res);
    cpu.registers[ESP] += 4;
    return res;
}

static void popl() {
    byteParts_t parts;
    parts.c = memory[cpu.ipointer + 1];
    int rA = parts.parts.second;
    cpu.registers[rA] = pop();
    cpu.ipointer += 2;
}

static void call() {
    int32_t destination = getLong(cpu.ipointer + 1);
    checkbound(destination);
    if(DEBUG || CALL) printf("Calling location 0x%x\n", destination);
    push(cpu.ipointer + 5); /* Push return address onto stack */
    cpu.ipointer = destination;
}

static void ret() {
    int32_t returnAddr = pop();
    if(DEBUG || CALL) printf("Returning to address 0x%x\n", returnAddr);
    cpu.ipointer = returnAddr;
}

static void read(int fn) {
    byteParts_t parts;
    parts.c = memory[cpu.ipointer + 1];
    int rA = parts.parts.second;
    int32_t displacement = getLong(cpu.ipointer + 2);
    int32_t dst = cpu.registers[rA] + displacement;
    
    int set;
    int result;
    if(fn == B) {
        char c = 0;
        set = scanf("%c", &c);
        result = putByte(c, dst);
        if(DEBUG) printf("Put Byte 0x%x at location %d\n", c, dst);
    } else {
        int32_t l = 0;
        set = scanf("%d", &l);
        result = putLong(l, dst);
        if(DEBUG) printf("Put Long 0x%x at location %d\n", l, dst);
    }

    cpu.ZF = set == EOF;
    if(DEBUG) printf("ZF set to %d\n", cpu.ZF);

    if(!result) {
        status = ADR;
    }
    
    cpu.ipointer += 6;
}

static void write(int fn) {
    byteParts_t parts;
    parts.c = memory[cpu.ipointer + 1];
    int rA = parts.parts.second;
    int32_t displacement = getLong(cpu.ipointer + 2);
    int32_t src = cpu.registers[rA] + displacement;
    checkbound(src);
    int val = fn == B ? memory[src] : getLong(src);
    printf("%c", val);
    cpu.ipointer += 6;
}

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
        unsigned char instruction = (unsigned char)memory[cpu.ipointer];
        if(DEBUG || CALL) printf("ipointer: 0x%x\n", cpu.ipointer);
        switch(instruction) {
            case 0x00: /* nop */
                if(DEBUG) printf("0x00 nop\n");
                cpu.ipointer += 1;
            break;
            case 0x10: /* halt */
                if(DEBUG) printf("0x10 halt\n");
                halt();
            break;
            case 0x20: /* rrmovl */
                if(DEBUG) printf("0x20 rrmovl\n");
                mov(RR);
            break;
            case 0x30: /* irmovl */
                if(DEBUG) printf("0x30 irmovl\n");
                mov(IR);
            break;
            case 0x40: /* rmmovl */
                if(DEBUG) printf("0x40 rmmovl\n");
                mov(RM);
            break;
            case 0x50: /* mrmovl */
                if(DEBUG) printf("0x50 mrmovl\n");
                mov(MR);
            break;
            case 0x60: /* addl */
                if(DEBUG) printf("0x60 addl\n");
                op(ADD);
            break;
            case 0x61: /* subl */
                if(DEBUG) printf("0x61 subl\n");
                op(SUB);
            break;
            case 0x62: /* andl */
                if(DEBUG) printf("0x62 andl\n");
                op(AND);
            break;
            case 0x63: /* xorl */
                if(DEBUG) printf("0x63 xorl\n");
                op(XOR);
            break;
            case 0x64: /* mull */
                if(DEBUG) printf("0x64 mull\n");
                op(MUL);
            break;
            case 0x65: /* cmpl */
                if(DEBUG) printf("0x65 cmpl\n");
                op(CMP);
            break;
            case 0x70: /* jmp */
                if(DEBUG) printf("0x70 jmp\n");
                jXX(JMP);
            break;
            case 0x71: /* jle */
                if(DEBUG) printf("0x71 jle\n");
                jXX(JLE);
            break;
            case 0x72: /* jl */
                if(DEBUG) printf("0x72 jl\n");
                jXX(JL);
            break;
            case 0x73: /* je */
                if(DEBUG) printf("0x73 je\n");
                jXX(JE);
            break;
            case 0x74: /* jne */
                if(DEBUG) printf("0x74 jne\n");
                jXX(JNE);
            break;
            case 0x75: /* jge */
                if(DEBUG) printf("0x75 jge\n");
                jXX(JGE);
            break;
            case 0x76: /* jg */
                if(DEBUG) printf("0x76 jg\n");
                jXX(JG);
            break;
            case 0x80: /* call */
                if(DEBUG) printf("0x80 call\n");
                call();
            break;
            case 0x90: /* ret */
                if(DEBUG) printf("0x90 ret\n");
                ret();
            break;
            case 0xA0: /* pushl */
                if(DEBUG) printf("0xA0 pushl\n");
                pushl();
            break;
            case 0xB0: /* popl */
                if(DEBUG) printf("0xB0 popl\n");
                popl();
            break;
            case 0xC0: /* readb */
                if(DEBUG) printf("0xC0 readb\n");
                read(B);
            break;
            case 0xC1: /* readl */
                if(DEBUG) printf("0xC1 readl\n");
                read(L);
            break;
            case 0xD0: /* writeb */
                if(DEBUG) printf("0xD0 writeb\n");
                write(B);
            break;
            case 0xD1: /* writel */
                if(DEBUG) printf("0xD1 writel\n");
                write(L);
            break;
            case 0xE0: /* movsbl */
                if(DEBUG) printf("0xE0 movsbl\n");
                mov(SB);
            break;
            default:
                status = INS;
                printf("Unknown Instruction Encountered\n");
        }
        if(DEBUG) {
            //printCPU();
            //waitFor(1);
            //clear();
        }
        
    }
    return status;
}
