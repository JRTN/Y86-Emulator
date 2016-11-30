#ifndef disassembler_h
#define disassembler_h

#define TEXT_D ".text"
#define BYTE *2

#define ADD 0
#define SUB 1
#define AND 2
#define XOR 3
#define MUL 4
#define CMP 5

#define JMP 0
#define JLE 1
#define JL  2
#define JE  3
#define JNE 4
#define JGE 5
#define JG  6

#define RR 0
#define IR 1
#define RM 2
#define MR 3
#define SB 4

#define B 0
#define L 1

void disassemble(char**);
#endif
