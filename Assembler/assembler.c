#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "assembler.h"

#define DELIMITERS " $(),%\n\t\v\f"

/*
    Returns the integer corresponding to the string register indicated
    by the string.
    Arguments: 
        char *reg - the string representation of the register
    Return:
        the number corresponding to the register if reg is a valid register;
        -1 otherwise.
*/
static int getRegisterCode(char *reg) {
    if(!reg) {
        return -1;
    } else if(STREQ(reg, EAX)) {
        return EAX_C;
    } else if(STREQ(reg, ECX)) {
        return ECX_C;
    } else if(STREQ(reg, EDX)) {
        return EDX_C;
    } else if(STREQ(reg, EBX)) {
        return EBX_C;
    } else if(STREQ(reg, ESP)) {
        return ESP_C;
    } else if(STREQ(reg, EBP)) {
        return EBP_C;
    } else if(STREQ(reg, ESI)) {
        return ESI_C;
    } else if(STREQ(reg, EDI)) {
        return EDI_C;
    } else {
        return -1;
    }
}

static char getNextRegister() {
    char *reg = strtok(NULL, DELIMITERS);
    int rA = getRegisterCode(reg);
    return rA != -1 ? rA + '0' : '\0';
}

/*
    Prints a standard error message and exits the program.
    Arguments:
        const char *fn_c - the string containing the code corresponding to the
        instruction with which there is a problem
        const char *message - the error message to be printed
*/
static void invalidArguments(const char *fn_c, const char *message) {
    fprintf(stderr, "ERROR: Instruction <%s> | %s\nProgram is exiting.\n", fn_c, message);
    exit(EXIT_FAILURE);
}

/*
    Handles assembling the jump and call instructions into ascii form
    Arguments:
        const char *fn_c - the code corresponding to the y86 instruction
*/
static void jump(const char *fn_c) {
    char *destToken = strtok(NULL, DELIMITERS);
    if(!destToken) {
        invalidArguments(fn_c, "expected 8 character hex address\n");
    }
    int32_t destValue;
    int scan = sscanf(destToken, "%x", &destValue);
    if(scan == EOF) {
        invalidArguments(fn_c, "could not parse destination address\n");
    }
    printf("%s", fn_c);
    printInt32LittleEndian(destValue);
}

/*
    Handles assembling the operation and rrmov instructions into ascii form
    Arguments:
        const char *fn_c - the code corresponding to the y86 instruction
*/
static void op(const char *fn_c) {
    char rA = getNextRegister();
    char rB = getNextRegister();
    if(!rA || !rB) {
        invalidArguments(fn_c, "expected two registers\n");
        return;
    }
    printf("%s%c%c", fn_c, rA, rB);
}

/*
    Handles assembling the push and pop instructions.
    Arguments:
        const char *fn_c - the code corresponding to the y86 instruction
*/
static void stackInstr(const char *fn_c) {
    char rA = getNextRegister();
    if(!rA) {
        invalidArguments(fn_c, "expected one register\n");
    }
    printf("%s%cf", fn_c, rA);
}

/*
    Handles assembling the read and write instructions.
    Arguments:
        const char *fn_c - the code corresponding to the y86 instruction
*/
static void readWrite(const char *fn_c) {
    char *displacementStr = strtok(NULL, DELIMITERS);
    if(!displacementStr) {
        invalidArguments(fn_c, "expected decimal displacement\n");        
    }
    int32_t displacement;
    int scan = sscanf(displacementStr, "%d", &displacement);
    if(scan == EOF) {
        invalidArguments(fn_c, "could not parse displacement amount\n");
    }
    char rA = getNextRegister();
    if(!rA) {
        invalidArguments(fn_c, "expected register\n");
    }
    printf("%s%cf", fn_c, rA);
    printInt32LittleEndian(displacement);
}

/*
    Handles assembling the movsbl and mrmovl instructions
    Arguments:
        const char *fn_c - the code corresponding to the y86 instruction
*/
static void sblmr(const char *fn_c) {
    char *displacementStr = strtok(NULL, DELIMITERS);
    if(!displacementStr) {
        invalidArguments(fn_c, "expected decimal displacement\n");        
    }
    int32_t displacement;
    int scan = sscanf(displacementStr, "%d", &displacement);
    if(scan == EOF) {
        invalidArguments(fn_c, "could not parse displacement amount\n");
    }
    char rB = getNextRegister();
    char rA = getNextRegister();
    if(!rA || !rB) {
        invalidArguments(fn_c, "expected two registers\n");
    }

    printf("%s%c%c", fn_c, rA, rB);
    printInt32LittleEndian(displacement);

    
}

/*
    Handles assembling the irmovl instruction
*/
static void irmovl() {
    char *immediateStr = strtok(NULL, DELIMITERS);
    if(!immediateStr) {
        invalidArguments(IRMOVL_C, "expected decimal immediate value\n");
    }
    int32_t immediate;
    int scan = sscanf(immediateStr, "%d", &immediate);
    if(scan == EOF) {
        invalidArguments(IRMOVL_C, "could not parse immediate value\n");
    }
    char rA = getNextRegister();
    if(rA == -1) {
        invalidArguments(IRMOVL_C, "expected register\n");
    }
    printf("%sf%c", IRMOVL_C, rA);
    printInt32LittleEndian(immediate);

    
}

/*
    Handles assembling the rmmovl instruction
*/
static void rmmovl() {
    char rA = getNextRegister();
    char *displacementStr = strtok(NULL, DELIMITERS);
    if(!displacementStr) {
        invalidArguments(RMMOVL_C, "expected decimal displacement value\n");
    }
    int32_t displacement;
    int scan = sscanf(displacementStr, "%d", &displacement);
    if(scan == EOF) {
        invalidArguments(RMMOVL_C, "could not parse displacement amount\n");
    }
    char rB = getNextRegister();
    if(!rA || !rB) {
        invalidArguments(RMMOVL_C, "expected two registers\n");
    }
    printf("%s%c%c", RMMOVL_C, rA, rB);
    printInt32LittleEndian(displacement);
    
}

/*
    Main loop of the assembler. Loops through every instruction in the program
    and calls the appropriate helper functions. These 
*/
void assemble(char *program) {
    char *token = NULL;
    token = strtok(program, DELIMITERS);
    while( token ) {
        if(STREQ(token, NOP)) {
            printf(NOP_C);
        } else if(STREQ(token, HALT)) {
            printf(HALT_C);
        } else if(STREQ(token, RRMOVL)) {
            op(RRMOVL_C);
        } else if(STREQ(token, IRMOVL)) {
            irmovl();
        } else if(STREQ(token, RMMOVL)) {
            rmmovl();
        } else if(STREQ(token, MRMOVL)) {
            sblmr(MRMOVL_C);
        } else if(STREQ(token, ADDL)) {
            op(ADDL_C);
        } else if(STREQ(token, SUBL)) {
            op(SUBL_C);
        } else if(STREQ(token, ANDL)) {
            op(ANDL_C);
        } else if(STREQ(token, XORL)) {
            op(XORL_C);
        } else if(STREQ(token, MULL)) {
            op(MULL_C);
        } else if(STREQ(token, CMPL)) {
            op(CMPL_C);
        } else if(STREQ(token, JMP)) {
            jump(JMP_C);
        } else if(STREQ(token, JLE)) {
            jump(JLE_C);
        } else if(STREQ(token, JL)) {
            jump(JL_C);
        } else if(STREQ(token, JE)) {
            jump(JE_C);
        } else if(STREQ(token, JNE)) {
            jump(JNE_C);
        } else if(STREQ(token, JGE)) {
            jump(JGE_C);
        } else if(STREQ(token, JG)) {
            jump(JG_C);
        } else if(STREQ(token, CALL)) {
            jump(CALL_C);
        } else if(STREQ(token, RET)) {
            printf(RET_C);
        } else if(STREQ(token, PUSHL)) {
            stackInstr(PUSHL_C);
        } else if(STREQ(token, POPL)) {
            stackInstr(POPL_C);
        } else if(STREQ(token, READB)) {
            readWrite(READB_C);
        } else if(STREQ(token, READL)) {
            readWrite(READL_C);
        } else if(STREQ(token, WRITEB)) {
            readWrite(WRITEB_C);
        } else if(STREQ(token, WRITEL)) {
            readWrite(WRITEL_C);
        } else if(STREQ(token, MOVSBL)) {
            sblmr(MOVSBL_C);
        } else {
            fprintf(stderr, "ERROR: Invalid instruction %s encountered\nProgram is exiting.\n", token);
        }
        token = strtok(NULL, DELIMITERS);
    }
}
