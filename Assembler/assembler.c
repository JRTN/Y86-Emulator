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

static void jump(const char *fn_c) {
    char *destToken = strtok(NULL, DELIMITERS);
    if(!destToken) {
        fprintf(stderr, "ERROR: Instruction %s expected 8 character hex address\n", fn_c);
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
        fprintf(stderr, "ERROR: Invalid arguments to op %s, expected two registers\n", fn_c);
        return;
    }

    printf("%s%c%c", fn_c, rA + '0', rB + '0');
}

static void stackInstr(const char *fn_c) {
    char *reg1 = strtok(NULL, DELIMITERS);
    int rA = getRegisterCode(reg1);
    if(rA == -1) {
        fprintf(stderr, "ERROR: Invalid arguments to op %s, expected one register\n", fn_c);
    }
    printf("%s%cf", fn_c, rA + '0');
}

static void read(const char *fn_c) {
    char *displacementStr = strtok(NULL, DELIMITERS);
    int32_t displacement;
    sscanf(displacementStr, "%d", &displacement);
    char *reg1 = strtok(NULL, DELIMITERS);
    int rA = getRegisterCode(reg1);
    printf("%s%cf%08x", fn_c, rA + '0', ENDIANSWAP(displacement));
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
            printf("rrmovl\n");
        } else if(STREQ(token, IRMOVL)) {
            printf("irmovl\n");
        } else if(STREQ(token, RMMOVL)) {
            printf("rmmovl\n");
        } else if(STREQ(token, MRMOVL)) {
            printf("mrmovl\n");
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
            read(READB_C);
        } else if(STREQ(token, READL)) {
            read(READL_C);
        } else if(STREQ(token, WRITEB)) {
            printf("writeb\n");
        } else if(STREQ(token, WRITEL)) {
            printf("writel\n");
        } else if(STREQ(token, MOVSBL)) {
            printf("movsbl\n");
        } else {
            printf("Not an instruction\n");
        }
        token = strtok(NULL, DELIMITERS);
    }
}
