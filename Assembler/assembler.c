#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "assembler.h"

#define DELIMITERS " $(),%\n\t\v\f"

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

static void invalidArguments(const char *fn_c, const char *message) {
    fprintf(stderr, "ERROR: Instruction <%s> | %s\n", fn_c, message);
}

static void jump(const char *fn_c) {
    char *destToken = strtok(NULL, DELIMITERS);
    if(!destToken) {
        invalidArguments(fn_c, "expected 8 character hex address\n");
    }
    //LOWERCASESTR(destToken);
    int32_t destValue;
    sscanf(destToken, "%x", &destValue);
    
    printf("%s%08x", fn_c, ENDIANSWAP(destValue));
}

static void op(const char *fn_c) {
    char *reg1 = strtok(NULL, DELIMITERS);
    char *reg2 = strtok(NULL, DELIMITERS);

    int rA = getRegisterCode(reg1);
    int rB = getRegisterCode(reg2);

    if(rA == -1 || rB == -1) {
        invalidArguments(fn_c, "expected two registers\n");
        return;
    }

    printf("%s%c%c", fn_c, rA + '0', rB + '0');
}

static void stackInstr(const char *fn_c) {
    char *reg1 = strtok(NULL, DELIMITERS);
    int rA = getRegisterCode(reg1);
    if(rA == -1) {
        invalidArguments(fn_c, "expected one register\n");
    }
    printf("%s%cf", fn_c, rA + '0');
}

static void readWrite(const char *fn_c) {
    char *displacementStr = strtok(NULL, DELIMITERS);
    if(!displacementStr) {
    invalidArguments(fn_c, "expected decimal displacement\n");        
    }
    int32_t displacement;
    sscanf(displacementStr, "%d", &displacement);
    char *reg1 = strtok(NULL, DELIMITERS);
    int rA = getRegisterCode(reg1);
    if(rA == -1) {
        invalidArguments(fn_c, "expected register\n");
    }
    printf("%s%cf%08x", fn_c, rA + '0', ENDIANSWAP(displacement));
}

static void sblmr(const char *fn_c) {
    char *displacementStr = strtok(NULL, DELIMITERS);
    if(!displacementStr) {
        fprintf(stderr, "ERROR: Invalid arguments to op e0, expected decimal displacement\n");
    }
    int32_t displacement;
    sscanf(displacementStr, "%d", &displacement);
    char *reg2 = strtok(NULL, DELIMITERS);
    char *reg1 = strtok(NULL, DELIMITERS);
    int rB = getRegisterCode(reg2);
    int rA = getRegisterCode(reg1);
    if(rA == -1 || rB == -1) {
        fprintf(stderr, "ERROR: Invalid arguments to op e0, expected two registers\n");
    }

    printf("%s%c%c%08x", fn_c, rA + '0', rB + '0', ENDIANSWAP(displacement));
}

static void irmovl() {
    char *immediateStr = strtok(NULL, DELIMITERS);
    if(!immediateStr) {
        fprintf(stderr, "ERROR: Invalid arguments to op 30, expected decimal immediate value\n");
    }
    int immediate;
    sscanf(immediateStr, "%d", &immediate);
    char *reg1 = strtok(NULL, DELIMITERS);
    int rA = getRegisterCode(reg1);
    if(rA == -1) {
        fprintf(stderr, "ERROR: Invalid arguments to op 30, expected register\n");
    }
    printf("30f%c%08x", rA + '0', ENDIANSWAP(immediate));
}

static void rmmovl() {
    char *reg1 = strtok(NULL, DELIMITERS);
    int rA = getRegisterCode(reg1);
    char *displacementStr = strtok(NULL, DELIMITERS);
    int displacement;
    sscanf(displacementStr, "%d", &displacement);
    char *reg2 = strtok(NULL, DELIMITERS);
    int rB = getRegisterCode(reg2);
    printf("40%c%c%08x", rA + '0', rB + '0', ENDIANSWAP(displacement));
}

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
            fprintf(stderr, "ERROR: Invalid instruction %s encountered\n", token);
        }
        token = strtok(NULL, DELIMITERS);
    }
}
