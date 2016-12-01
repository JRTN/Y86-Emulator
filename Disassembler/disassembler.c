#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disassembler.h"
#include "util.h"

#define ADDR startAddr + (currPos / 2)

static int32_t startAddr;
static int32_t currPos;

static char *instructions;
static char *registers[8] = {"%eax", "%ecx", "%edx", "%ebx", "%esp", "%ebp", "%esi", "%edi"};
static void halt() {
    printf("0x%-5X| halt\n", ADDR);
    currPos += 1 BYTE;
}

static void mov(int fn) {
    int32_t rA = instructions[currPos + 2] - '0';
    int32_t rB = instructions[currPos + 3] - '0';
    if(fn == RR) {
        printf("0x%-5X| rrmovl %s, %s\n", ADDR, registers[rA], registers[rB]);
        currPos += 2 BYTE;
        return;
    }
    char *valStr = nt_strncpy(instructions + currPos + 2 BYTE, 4 BYTE);
    int32_t val = hexToDecLittleEndian(valStr);
    switch(fn) {
        case IR:
            printf("0x%-5X| irmovl $%d, %s\n", ADDR, val, registers[rB]);
        break;
        case RM:
            printf("0x%-5X| rmmovl %s, %d(%s)\n", ADDR, registers[rA], val, registers[rB]);
        break;
        case MR:
            printf("0x%-5X| mrmovl %d(%s), %s\n", ADDR, val, registers[rB], registers[rA]);
        break;
        case SB:
            printf("0x%-5X| movsbl %d(%s), %s\n", ADDR, val, registers[rB], registers[rA]);
        break;
    }
    free(valStr);
    currPos += 6 BYTE;
}

static void op(int fn) {
    int32_t rA = instructions[currPos + 2] - '0';
    int32_t rB = instructions[currPos + 3] - '0';
    char *op;
    switch(fn) {
        case ADD:
            op = "addl";
        break;
        case SUB:
            op = "subl";
        break;
        case CMP:
            op = "cmpl";
        break;
        case AND:
            op = "andl";
        break;
        case XOR:
            op = "xorl";
        break;
        case MUL:
            op = "mull";
        break;
    }
    printf("0x%-5X| %s %s, %s\n", ADDR, op, registers[rA], registers[rB]);
    currPos += 2 BYTE;
}

static void jXX(int fn) {
    char *destString = nt_strncpy(instructions + currPos + 1 BYTE, 4 BYTE);
    int32_t destination = hexToDecLittleEndian(destString);

    char *type;
    switch(fn) {
        case JMP:
            type = "jmp";
        break;
        case JLE:
            type = "jle";
        break;
        case JL:
            type = "jl";
        break;
        case JE:
            type = "je";
        break;
        case JNE:
            type = "jne";
        break;
        case JGE:
            type = "jge";
        break;
        case JG:
            type = "jg";
        break;
    }
    printf("0x%-5X| %s 0x%X\n", ADDR, type, destination);
    currPos += 5 BYTE;
}

static void call() {
    char *destString = nt_strncpy(instructions + currPos + 1 BYTE, 4 BYTE);
    int32_t destination = hexToDecLittleEndian(destString);
    printf("0x%-5X| call 0x%X\n", ADDR, destination);
    currPos += 5 BYTE;
}

static void ret() {
    printf("0x%-5X| ret\n", ADDR);
    currPos += 1 BYTE;
}

static void pushl() {
    int32_t rA = instructions[currPos + 2] - '0';
    printf("0x%-5X| pushl %s\n", ADDR, registers[rA]);
    currPos += 2 BYTE;
}

static void popl() {
    int32_t rA = instructions[currPos + 2] - '0';
    printf("0x%-5X| popl %s\n", ADDR, registers[rA]);
    currPos += 2 BYTE;
}

static void read(int fn) {
    int32_t rA = instructions[currPos + 2] - '0';
    char *displacementStr = nt_strncpy(instructions + currPos + 2 BYTE, 4 BYTE);
    int32_t displacement = hexToDecLittleEndian(displacementStr);
    char type = (fn == B ? 'b' : 'l');
    printf("0x%-5X| read%c %d(%s)\n", ADDR, type, displacement, registers[rA]);
    currPos += 6 BYTE;
}

static void write(int fn) {
    int32_t rA = instructions[currPos + 2] - '0';
    char *displacementStr = nt_strncpy(instructions + currPos + 2 BYTE, 4 BYTE);
    int32_t displacement = hexToDecLittleEndian(displacementStr);
    char type = (fn == B ? 'b' : 'l');
    printf("0x%-5X| write%c %d(%s)\n", ADDR, type, displacement, registers[rA]);
    currPos += 6 BYTE;
}

static void translate() {
    int len = strlen(instructions);
    while(currPos < len) {
        char *instruction = nt_strncpy(instructions + currPos, 2);
        int32_t n_instruction = hexToDec(instruction);
        switch(n_instruction) {
            case 0x00: /* nop */
                printf("0x%-5X| nop\n", ADDR);
                currPos += 1 BYTE;
            break;
            case 0x10: /* halt */
                halt();
            break;
            case 0x20: /* rrmovl */
                mov(RR);
            break;
            case 0x30: /* irmovl */
                mov(IR);
            break;
            case 0x40: /* rmmovl */
                mov(RM);
            break;
            case 0x50: /* mrmovl */
                mov(MR);
            break;
            case 0x60: /* addl */
                op(ADD);
            break;
            case 0x61: /* subl */
                op(SUB);
            break;
            case 0x62: /* andl */
                op(AND);
            break;
            case 0x63: /* xorl */
                op(XOR);
            break;
            case 0x64: /* mull */
                op(MUL);
            break;
            case 0x65: /* cmpl */
                op(CMP);
            break;
            case 0x70: /* jmp */
                jXX(JMP);
            break;
            case 0x71: /* jle */
                jXX(JLE);
            break;
            case 0x72: /* jl */
                jXX(JL);
            break;
            case 0x73: /* je */
                jXX(JE);
            break;
            case 0x74: /* jne */
                jXX(JNE);
            break;
            case 0x75: /* jge */
                jXX(JGE);
            break;
            case 0x76: /* jg */
                jXX(JG);
            break;
            case 0x80: /* call */
                call();
            break;
            case 0x90: /* ret */
                ret();
            break;
            case 0xA0: /* pushl */
                pushl();
            break;
            case 0xB0: /* popl */
                popl();
            break;
            case 0xC0: /* readb */
                read(B);
            break;
            case 0xC1: /* readl */
                read(L);
            break;
            case 0xD0: /* writeb */
                write(B);
            break;
            case 0xD1: /* writel */
                write(L);
            break;
            case 0xE0: /* movsbl */
                mov(SB);
            break;
            default:
                printf("Unknown Instruction Encountered %s[0x%X]\n", instruction, n_instruction);
        }
        free(instruction);        
    }
}

void disassemble(char **programTokens) {
    int32_t textPos = searchStringArray(programTokens, TEXT_D);
    if(textPos == -1) {
        fprintf(stderr, "ERROR: No .text directive in the file.\n");
        return;
    }
    startAddr = atoi(programTokens[textPos + 1]);
    instructions = programTokens[textPos + 2];
    printf("Instructions starting at address 0x%X:\n%s\n", startAddr, instructions);
    printf("Disassembled: \n");
    translate();
    free(programTokens);
}
