#ifndef loader_h
#define loader_h

#define SIZE_D ".size"
#define STRING_D ".string"
#define LONG_D ".long"
#define BSS_D ".bss"
#define BYTE_D ".byte"
#define TEXT_D ".text"

int loadFileIntoMemory(char *);

#endif
