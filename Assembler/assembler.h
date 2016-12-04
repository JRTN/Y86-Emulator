#ifndef assembler_h
#define assembler_h

#define EAX "eax"
#define ECX "ecx"
#define EDX "edx"
#define EBX "ebx"
#define ESP "esp"
#define EBP "ebp"
#define ESI "esi"
#define EDI "edi"

#define EAX_C 0
#define ECX_C 1
#define EDX_C 2
#define EBX_C 3
#define ESP_C 4
#define EBP_C 5
#define ESI_C 6
#define EDI_C 7

#define NOP "nop"
#define HALT "halt"
#define RRMOVL "rrmovl"
#define IRMOVL "irmovl"
#define RMMOVL "rmmovl"
#define MRMOVL "mrmovl"
#define ADDL "addl"
#define SUBL "subl"
#define ANDL "andl"
#define XORL "xorl"
#define MULL "mull"
#define CMPL "cmpl"
#define JMP "jmp"
#define JLE "jle"
#define JL "jl"
#define JE "je"
#define JNE "jne"
#define JGE "jge"
#define JG "jg"
#define CALL "call"
#define RET "ret"
#define PUSHL "pushl"
#define POPL "popl"
#define READB "readb"
#define READL "readl"
#define WRITEB "writeb"
#define WRITEL "writel"
#define MOVSBL "movsbl"

#define NOP_C "00"
#define HALT_C "10"
#define RRMOVL_C "20"
#define IRMOVL_C "30"
#define RMMOVL_C "40"
#define MRMOVL_C "50"
#define ADDL_C "60"
#define SUBL_C "61"
#define ANDL_C "62"
#define XORL_C "63"
#define MULL_C "64"
#define CMPL_C "65"
#define JMP_C "70"
#define JLE_C "71"
#define JL_C "72"
#define JE_C "73"
#define JNE_C "74"
#define JGE_C "75"
#define JG_C "76"
#define CALL_C "80"
#define RET_C "90"
#define PUSHL_C "a0"
#define POPL_C "b0"
#define READB_C "c0"
#define READL_C "c1"
#define WRITEB_C "d0"
#define WRITEL_C "d1"
#define MOVSBL_C "e0"

void assemble(char*);

#endif
