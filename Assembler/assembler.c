#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "assembler.h"

#define STREQ(x,y) strcmp(x,y)==0
#define DELIMITERS " $(),%\n\t\v\f"

void assemble(char *program) {
    char *token = NULL;
    token = strtok(program, DELIMITERS);
    while( token ) {
        if(STREQ(token, NOP)) {
            printf("nop\n");
        } else if(STREQ(token, HALT)) {
            printf("halt\n");
        } else if(STREQ(token, RRMOVL)) {
            printf("rrmovl\n");
        } else if(STREQ(token, IRMOVL)) {
            printf("irmovl\n");
        } else if(STREQ(token, RMMOVL)) {
            printf("rmmovl\n");
        } else if(STREQ(token, MRMOVL)) {
            printf("mrmovl\n");
        } else if(STREQ(token, ADDL)) {
            printf("addl\n");
        } else if(STREQ(token, SUBL)) {
            printf("subl\n");
        } else if(STREQ(token, ANDL)) {
            printf("andl\n");
        } else if(STREQ(token, XORL)) {
            printf("xorl\n");
        } else if(STREQ(token, MULL)) {
            printf("mull\n");
        } else if(STREQ(token, CMPL)) {
            printf("cmpl\n");
        } else if(STREQ(token, JMP)) {
            printf("jmp\n");
        } else if(STREQ(token, JLE)) {
            printf("jle\n");
        } else if(STREQ(token, JL)) {
            printf("jl\n");
        } else if(STREQ(token, JE)) {
            printf("je\n");
        } else if(STREQ(token, JNE)) {
            printf("jne\n");
        } else if(STREQ(token, JGE)) {
            printf("jge\n");
        } else if(STREQ(token, JG)) {
            printf("jg\n");
        } else if(STREQ(token, CALL)) {
            printf("call\n");
        } else if(STREQ(token, RET)) {
            printf("ret\n");
        } else if(STREQ(token, PUSHL)) {
            printf("pushl\n");
        } else if(STREQ(token, POPL)) {
            printf("popl\n");
        } else if(STREQ(token, READB)) {
            printf("readb\n");
        } else if(STREQ(token, READL)) {
            printf("readl\n");
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
